#include "tcpmessager.h"
#include "trayloadermodule.h"
#include "config.h"
#include <QDebug>
#include <QObject>
#include <QElapsedTimer>

TrayLoaderModule::TrayLoaderModule(QString name):ThreadWorkerBase(name)
{
    connect(this,SIGNAL(testTrayUsed()),this,SLOT(onTestTrayUsed()),Qt::DirectConnection);
}

void TrayLoaderModule::Init(XtMotor *_motor_clip_in,
                            XtMotor *_motor_in,
                            XtMotor *_motor_work,
                            XtMotor *_motor_out,
                            XtMotor *_motor_clip_out,
                            XtCylinder* _cylinder_clip,
                            XtCylinder* _cylinder_ltk1,
                            XtCylinder* _cylinder_ltk2,
                            XtCylinder* _cylinder_tray,
                            TrayClip *trayClipIn,
                            TrayClip *trayClipOut,
                            XtGeneralInput *work_tray_check_io,
                            XtGeneralInput* entrance_tray_check_io,
                            XtGeneralInput* exit_tray_check_io,
                            XtGeneralInput* entrance_clip_check_io,
                            XtGeneralInput* exit_clip_check_io,
                            XtGeneralInput* ready_tray_check_io)
{
    parts.clear();
    this->motor_clip_in = _motor_clip_in;
    parts.append(this->motor_clip_in);
    this->motor_in = _motor_in;
    parts.append(this->motor_in);
    this->motor_work = _motor_work;
    parts.append(this->motor_work);
    this->motor_out = _motor_out;
    parts.append(this->motor_out);
    this->motor_clip_out = _motor_clip_out;
    parts.append(this->motor_clip_out);
    this->cylinder_clip = _cylinder_clip;
    parts.append(this->cylinder_clip);
    this->cylinder_ltk1 = _cylinder_ltk1;
    parts.append(this->cylinder_ltk1);
    this->cylinder_ltk2 = _cylinder_ltk2;
    parts.append(this->cylinder_ltk2);
    this->cylinder_tray = _cylinder_tray;
    parts.append(this->cylinder_tray);
    this->tray_clip_in = trayClipIn;
    this->tray_clip_out = trayClipOut;

    this->work_tray_check_io = work_tray_check_io;
    parts.append(this->work_tray_check_io);
    this->entrance_tray_check_io = entrance_tray_check_io;
    parts.append(this->entrance_tray_check_io);
    this->exit_tray_check_io = exit_tray_check_io;
    parts.append(this->exit_tray_check_io);
    this->entrance_clip_check_io = entrance_clip_check_io;
    parts.append(this->entrance_clip_check_io);
    this->exit_clip_check_io = exit_clip_check_io;
    parts.append(this->exit_clip_check_io);
    this->ready_tray_check_io = ready_tray_check_io;
    parts.append(this->ready_tray_check_io);
    //Set some IO name in parameters for the ease of access in tcp channel
    if (cylinder_clip)
    {
        parameters.setTcpCylinderClipIOName(cylinder_clip->parameters.oneOutName());
        parameters.setTcpCylinderClipSensorName(cylinder_clip->parameters.oneInName());
    }
    if (cylinder_ltk1)
    {
        parameters.setTcpCylinderLTKX1IOName(cylinder_ltk1->parameters.oneOutName());
        parameters.setTcpCylinderLTKX1SensorName(cylinder_ltk1->parameters.oneInName());
    }
    if (cylinder_ltk2)
    {
        parameters.setTcpCylinderLTKX2IOName(cylinder_ltk2->parameters.oneOutName());
        parameters.setTcpCylinderLTKX2SensorName(cylinder_ltk2->parameters.oneInName());
    }
    if (cylinder_tray)
    {
        parameters.setTcpCylinderLTLIOName(cylinder_tray->parameters.oneOutName());
        parameters.setTcpCylinderLTLSensorName(cylinder_tray->parameters.oneInName());
    }
}

void TrayLoaderModule::resetLogic()
{
    if(is_run)return;
    states.setIsFirstTray(true);
    states.setHasWorkTray(false);
    states.setHasTrayReady(false);
    states.setHasReadyTray(false);
    states.setHasPulledTray(false);
    states.setIsExchangeTray(false);
    states.setAllowChangeTray(false);
    states.setHasExitClipFull(false);
    tray_clip_in->reset();
    states.setHasEntranceClipEmpty(false);
    tray_clip_out->reset();
    states.setEntanceClipReady(false);
    states.setExitClipReady(false);
    states.setReadyToPusReadyTray(false);
    states.setReadyToPushEmptyTray(false);
}

bool TrayLoaderModule::sendoutAndReayPushOutEmptyTray()
{
    QElapsedTimer timer; timer.start();
    bool result = cylinder_ltk2->Set(false);
    if(!result)return false;
    result &= motor_work->MoveToPos(parameters.ltlReleasePos());
    result &= motor_out->MoveToPos(parameters.ltkx2PressPos());
    result &= motor_work->WaitArrivedTargetPos(parameters.ltlReleasePos());
    if(result)
        result &= cylinder_tray->Set(true);
    if(parameters.openQinfo())
        qInfo(u8"盘托上料盘检测！%d",result);
    result &= checkWorkTray(true);
    if(!result)
        AppendError(u8"盘托上未检测到料盘！");
    result &= motor_out->WaitArrivedTargetPos(parameters.ltkx2PressPos());
    if(!result)
        AppendError(QString(u8"放出空盘并准备推空盘失败"));
    if(parameters.openQinfo())
        qInfo(u8"放出空盘并准备推空盘 %d",result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool TrayLoaderModule::moveToGetAndPushInNewTrayAndPushOutTray()
{
    QElapsedTimer timer; timer.start();
    bool result = cylinder_tray->Set(true);
    bool out_result = false;
    if(result)
        out_result = cylinder_ltk2->Set(true);
    if(result)
        result &= motor_work->MoveToPos(parameters.ltlPressPos());
    if(out_result)
        out_result &= motor_out->SlowMoveToPos(parameters.ltkx2ReleasePos(),parameters.pushVelocity());
    bool in_result = cylinder_ltk1->Set(true);
    if(in_result)
        in_result &= motor_in->SlowMoveToPos(parameters.ltkx1RelayPos(),parameters.pushVelocity());
    in_result &= motor_in->WaitArrivedTargetPos(parameters.ltkx1RelayPos());
    if(in_result)
        in_result &= cylinder_ltk1->Set(false);
    out_result &= motor_out->WaitArrivedTargetPos(parameters.ltkx2ReleasePos());
    if(out_result)
    {
        out_result &= motor_out->MoveToPosSync(parameters.ltkx2ReleasePos() - parameters.ltkx2SafeDistance());
        out_result &= cylinder_ltk2->Set(false);
    }
    result &= motor_work->WaitArrivedTargetPos(parameters.ltlPressPos());
    if(!(result&&out_result&&in_result))
        AppendError(QString(u8"去拿新盘、推入新盘并推出空盘失败"));
    if(parameters.openQinfo())
        qInfo(u8"去拿新盘、推入新盘并推出空盘 %d",result&&out_result&&in_result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result&&out_result&&in_result;
}

bool TrayLoaderModule::moveToGetAndPushInNewTray()
{
    QElapsedTimer timer; timer.start();
    bool result = cylinder_tray->Set(true);
    if(result)
        result &= motor_work->MoveToPos(parameters.ltlPressPos());
    bool in_result = cylinder_ltk1->Set(true);
    if(in_result)
        in_result &= motor_in->SlowMoveToPos(parameters.ltkx1RelayPos(),parameters.pushVelocity());
    result &= motor_work->WaitArrivedTargetPos(parameters.ltlPressPos());
    in_result &= motor_in->WaitArrivedTargetPos(parameters.ltkx1RelayPos());
    if(in_result)
        in_result &= cylinder_ltk1->Set(false);
    if(!result)
        AppendError(QString(u8"去拿新盘并推入新盘失败"));
    if(parameters.openQinfo())
        qInfo(u8"去拿新盘并推入新盘 %d",result&&in_result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result&&in_result;
}

bool TrayLoaderModule::moveToPushOutTray()
{
    QElapsedTimer timer; timer.start();
    bool result = cylinder_ltk2->Set(true);
    if(result)
        result &= motor_out->SlowMoveToPosSync(parameters.ltkx2ReleasePos(),parameters.pushVelocity());
    if(result)
    {
        result &= cylinder_ltk2->Set(false);
        result &= motor_out->MoveToPosSync(parameters.ltlReleasePos() - parameters.ltkx2SafeDistance());
    }
    if(!result)
        AppendError(QString(u8"推出空盘失败"));
    if(parameters.openQinfo())
        qInfo(u8"推出空盘失败 %d",result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool TrayLoaderModule::moveToWorkPosAndReayPullNewTray()
{
    QElapsedTimer timer; timer.start();
    bool in_result = cylinder_ltk1->Set(false);
    bool result = true;
    if(in_result)
    {
        in_result &= motor_in->MoveToPos(parameters.ltkx1PressPos());
        result = cylinder_tray->Set(false);
    }
    if(result)
        result &= motor_work->MoveToPos(parameters.ltlWorkPos());
    in_result &= motor_in->WaitArrivedTargetPos(parameters.ltkx1PressPos());
    result &= motor_work->WaitArrivedTargetPos(parameters.ltlWorkPos());
    if(!result)
        AppendError(QString(u8"去工作位置并去拉盘准备位置失败"));
    if(parameters.openQinfo())
        qInfo(u8"去工作位置并去拉盘准备位置 %d",result&&in_result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result&&in_result;
}

bool TrayLoaderModule::entranceClipMoveToNextPos()
{
    QElapsedTimer timer; timer.start();
    bool result = motor_clip_in->MoveToPosSync(tray_clip_in->getCurrentPosition());
    if(!result)
        AppendError(QString(u8"移动进料弹夹失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动进料弹夹 %d",result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool TrayLoaderModule::moveToReayPullNewTray()
{
    QElapsedTimer timer; timer.start();
    bool result = cylinder_ltk1->Set(false);
    if(result)
        result &= motor_in->MoveToPosSync(parameters.ltkx1PressPos());
    if(!result)
        AppendError(QString(u8"去拉盘准备位置失败"));
    if(parameters.openQinfo())
        qInfo(u8"去拉盘准备位置 %d",result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool TrayLoaderModule::moveToPushReadyTray()
{
    bool result = cylinder_ltk1->Set(true);
    if(result)
        result &= motor_in->SlowMoveToPosSync(parameters.ltkx1RelayPos(),parameters.pushVelocity());
    if(result)
        result &= cylinder_ltk1->Set(false);
    if(!result)
        AppendError(QString(u8"推备用盘失败"));
    if(parameters.openQinfo())
        qInfo(u8"推备用盘 %d",result);
    return result;
}

bool TrayLoaderModule::clipPushoutTray()
{
    QElapsedTimer timer; timer.start();
    bool result = cylinder_ltk1->Set(false);
    if(result)
        result &= cylinder_clip->Set(true);
    if(result)
        result &= cylinder_clip->Set(false);
    result &= checkEntranceTray(true);
    if(parameters.openQinfo())
        qInfo(u8"入料口料盘检测 %d",result);
    if(!result)
        AppendError(u8"入料口未检测到料盘.");

    bool in_result = cylinder_ltk1->Set(true);
    if(in_result)
        in_result &= motor_in->SlowMoveToPosSync(parameters.ltkx1ReleasePos(),parameters.pushVelocity());
    in_result &= cylinder_ltk1->Set(false);
    if(!result)
        AppendError(QString(u8"进料弹夹出盘失败"));
    if(parameters.openQinfo())
        qInfo(u8"进料弹夹出盘 %d",result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool TrayLoaderModule::existClipMoveToNextPos()
{
    QElapsedTimer timer; timer.start();
    bool result = motor_clip_out->MoveToPosSync(tray_clip_out->getCurrentPosition());
    if(!result)
        AppendError(QString(u8"移动出料弹夹失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动出料弹夹 %d",result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool TrayLoaderModule::moveToChangeChangeHandlly()
{
    bool result = cylinder_ltk1->Set(false);
    if(result)
        result &= motor_in->MoveToPosSync(0);
    if(result)
        result &= motor_work->MoveToPosSync(parameters.ltlPressPos());
//    if(result)
//        result &= cylinder_tray->Set(true);
    if(!result)
        AppendError(u8"去手动换盘位置失败");
    if(parameters.openQinfo())
        qInfo(u8"去手动换盘位置%d",result);
    return result;
}

bool TrayLoaderModule::moveToWorkPos()
{
    bool result = cylinder_tray->Set(false);
    if(result)
        result &= motor_work->MoveToPosSync(parameters.ltlWorkPos());
    if(!result)
        AppendError(u8"去工作位置失败");
    if(parameters.openQinfo())
        qInfo(u8"去工作位置失败 %d",result);
    return result;
}

bool TrayLoaderModule::moveToChangeClipPos()
{
    QElapsedTimer timer; timer.start();
    qInfo("moveToChangeClipPos");
    motor_clip_in->MoveToPos(tray_clip_in->standards_parameters.changeClipPos());
    bool result = motor_clip_in->WaitArrivedTargetPos(tray_clip_in->standards_parameters.changeClipPos());
    if (result)
    {
        AppendError(tr(u8"移动到换料位置失败！"));
    }
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}
void TrayLoaderModule::startWork(int run_mode)
{
    if(run_mode == RunMode::AAFlowChartTest || run_mode == RunMode::UNLOAD_ALL_LENS) return;
    qInfo("startWork: %d", run_mode);
    QVariantMap run_params = inquirRunParameters();
    if(run_params.isEmpty())
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数为空.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(run_params.contains("RunMode"))
    {
        states.setRunMode(run_params["RunMode"].toInt());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数RunMode缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(run_params.contains("HandlyChangeLens"))
    {
        states.setHandlyChangeLens(run_params["HandlyChangeLens"].toBool());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数HandlyChangeLens缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }

    if(run_params.contains("HandlyChangeLensTray"))
    {
        states.setHandlyChangeLensTray(run_params["HandlyChangeLensTray"].toBool());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数HandlyChangeLensTray缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(states.handlyChangeLens())
        return;

    if(run_mode == RunMode::Normal||run_mode == RunMode::NoMaterial)
    {
        if(states.handlyChangeLensTray())
        {
            qInfo("Running handlyChangeLensTray");
            runHandle();
        }
        else
        {
            qInfo("Running auto load tray mode");
            run();
        }
    }
}

void TrayLoaderModule::stopWork(bool wait_finish)
{
    is_run = false;
}

void TrayLoaderModule::performHandlingOperation(int cmd,QVariant param)
{
    qInfo("performHandlingOperation cmd: %d", cmd);
    bool result = true;
    if(cmd == HandlePosition::ENTRANCE_CLIP_POS)
    {
        int layer_index = param.toInt();
        result = moveEntranceClipToPos(layer_index);
    }
    else if(cmd == HandlePosition::ENTRANCE_CLIP_WAIT_POS)
        result = moveEntranceClipToWaitPos();
    else if(cmd == HandlePosition::ENTRANCE_CLIP_CHANGE_POS)
        result = moveToChangeClipPos();
    else if(cmd == HandlePosition::ENTRANCE_CLIP_TOP)
        result = moveEntranceClipToTop();
    else if(cmd == HandlePosition::ENTRANCE_CLIP_BOTTOM)
        result = moveEntranceClipToBottom();
    else if(cmd == HandlePosition::EXIT_CLIP_POS)
    {
        int layer_index = param.toInt();
        result = moveExitClipToPos(layer_index);
    }
    else if(cmd == HandlePosition::EXIT_CLIP_TOP)
        result = moveExitClipToTop();
    else if(cmd == HandlePosition::EXIT_CLIP_BOTTOM)
        result = moveExitClipToBottom();
    else if(cmd == HandlePosition::ENTRANCE_KICK_READY_POS)
        result = moveEntranceKickToReadyPos();
    else if(cmd == HandlePosition::ENTRANCE_KICK_PULL_POS)
        result = moveEntranceKickToPullPos();
    else if(cmd == HandlePosition::ENTRANCE_KICK_PUSH_POS)
        result = moveEntranceKickToPushPos();
    else if(cmd == HandlePosition::EXIT_KICK_READY_POS)
        result = moveExitKickToReadyPos();
    else if(cmd == HandlePosition::EXIT_KICK_PUSH_POS)
        result = moveExitKickToPushPos();
    else if(cmd == HandlePosition::LOADER_WAIT_POS)
        result = moveTrayLoaderToWaitPos();
    else if(cmd == HandlePosition::LOADER_WORK_POS)
        result = moveTrayLoaderToWorkPos();
    else if(cmd == HandlePosition::LOADER_RELEASE_POS)
        result = moveTrayLoaderToReleasePos();
    else if(cmd == HandlePosition::EJECT_TRAY)
        result = ejectTray();
    if(!result)
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
    is_handling = false;
    qInfo("performHandlingOperation cmd:%d finished", cmd);
    is_error = !result;
}

PropertyBase *TrayLoaderModule::getModuleState()
{
    return &states;
}

void TrayLoaderModule::receivceModuleMessage(QVariantMap message)
{
    qInfo("receive module message %s",TcpMessager::getStringFromQvariantMap(message).toStdString().c_str());
    if(!message.contains("OriginModule"))
    {
        qInfo("message error! has no OriginModule.");
        return;
    }
    if(message["OriginModule"].toString() == "LensLoaderModule" || message["OriginModule"].toString() == "LensTrayLoaderModule")
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString()=="ChangeTrayResquest")
            {
                if(states.allowChangeTray())
                {
                    qInfo("receive ChangeTrayResquest in changingTray");
                    return;
                }
                states.setAllowChangeTray(true);
            }
            else if(message["Message"].toString()=="ReadyTrayResquest")
            {
                if(states.allowChangeTray())
                {
                    qInfo("receive ReadyTrayResquest in changingTray");
                    return;
                }
                states.setallowReadyTray(true);
            }
        }
        else
        {
            qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            return;
        }
    }
    else
    {
        qInfo("module name error %s",message["OriginModule"].toString().toStdString().c_str());
        return;
    }
}

QMap<QString, PropertyBase *> TrayLoaderModule::getModuleParameter()
{
    QMap<QString, PropertyBase *> temp_map;
    temp_map.insert(TRAY_LOADER_PARAMETER, &parameters);
    temp_map.insert(TRAY_CLIPIN_PARAMETER, &tray_clip_in->standards_parameters);
    temp_map.insert(TRAY_CLIPOUT_PARAMETER, &tray_clip_out->standards_parameters);
    return temp_map;
}

void TrayLoaderModule::setModuleParameter(QMap<QString, PropertyBase *>)
{

}

void TrayLoaderModule::run()
{
    if(!resumeState())
    {
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::ErrorMustStop);
        return;
    }
    is_run = true;
    bool has_task = false;
    bool allow_change_tray;
    while(is_run){
            QThread::msleep(100);
        has_task = false;
        {
            QMutexLocker temp_locker(&tray_mutex);
            allow_change_tray = states.allowChangeTray();
        }
        if(!is_run)break;
        //送出空盘
        //准备推出空盘
        if(allow_change_tray&&states.hasReadyTray()&&states.hasWorkTray()&&(!states.readyToPushEmptyTray()))
        {
            if((!sendoutAndReayPushOutEmptyTray()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setHasWorkTray(false);
            states.setReadyToPushEmptyTray(true);
            if(!is_run)break;
        }
        //去取盘
        //推入满盘
        //推出空盘
        if(allow_change_tray&&states.hasReadyTray()&&(!states.isReadyTrayPushed())&&(!states.hasWorkTray())&&states.readyToPushReadyTray()&&states.readyToPushEmptyTray()&&states.exitClipReady())
        {
            has_task = true;
            if((!moveToGetAndPushInNewTrayAndPushOutTray()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setReadyToPusReadyTray(false);
            states.setReadyToPushEmptyTray(false);
            states.setExitClipReady(false);
            states.setIsReadyTrayPushed(true);
            if(!is_run)break;
        }
        //无空盘时推出满盘
        if(allow_change_tray&&states.hasReadyTray()&&(!states.isReadyTrayPushed())&&(!states.hasWorkTray())&&states.readyToPushReadyTray())
        {
            has_task = true;
            if(!moveToGetAndPushInNewTray())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setReadyToPusReadyTray(false);
            states.setIsReadyTrayPushed(true);
            if(!is_run)break;
        }
        if(!is_run)break;
        //到工作位置
        //准备接备用盘
        if(allow_change_tray&&states.hasReadyTray()&&states.isReadyTrayPushed()&&(!states.hasWorkTray())&&(!states.readyToPushEmptyTray()))
        {
            has_task = true;
            if((!moveToWorkPosAndReayPullNewTray()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_NEXT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(NEXT_OPERATION == operation)
                {
                    states.setHasReadyTray(false);
                    continue;
                }
                if(RETRY_OPERATION == operation)
                    continue;
            }
            {
                QMutexLocker temp_locker(&tray_mutex);
                states.setAllowChangeTray(false);
                states.setIsExchangeTray(false);
            }
            sendMessageToModule("LensLoaderModule","FinishChangeTray");
            if(states.isFirstTray())
                states.setIsFirstTray(false);
            states.setHasReadyTray(false);
            states.setHasWorkTray(true);
            states.setReadyToPusReadyTray(true);
            states.setIsReadyTrayPushed(false);
            if(!is_run)break;
        }
        if(!is_run)break;
        //无备用盘时推出空盘
        if((!states.hasReadyTray())&&(!states.hasWorkTray())&&states.readyToPushEmptyTray()&&states.exitClipReady())
        {
            has_task = true;
            if(!moveToPushOutTray())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_NEXT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(NEXT_OPERATION == operation)
                {
                    states.setExitClipReady(false);
                    continue;
                }
                if(RETRY_OPERATION == operation)
                    continue;
            }
            else
            {

            }
            states.setExitClipReady(false);
            states.setReadyToPushEmptyTray(false);
        }
        if(!is_run)break;
        //提示换出料弹夹
        if(states.hasExitClipFull()&&(!states.exitClipReady()))
        {
            has_task = true;
            int alarm_id =sendAlarmMessage(u8"已换弹夹",u8"出料弹夹已满，请更换后再点击确认,谢谢配合！");
            waitMessageReturn(is_run,alarm_id);
            if(is_run)
                states.setHasExitClipFull(false);
        }
        if(!is_run)break;
        //入料弹夹到位
        if((!states.hasEntranceClipEmpty())&&(!states.entanceClipReady()))
        {
            has_task = true;
            if((!entranceClipMoveToNextPos()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            else
            {
                if(!tray_clip_in->updateCurrentIndex())
                    states.setHasEntranceClipEmpty(true);
                states.setEntanceClipReady(true);
            }
        }
        if(!is_run)break;
        //准备接备用盘
        if((!states.readyToPushReadyTray()))
        {
            has_task = true;
            if((!moveToReayPullNewTray()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setReadyToPusReadyTray(true);
            if(states.hasPulledTray())
            {
                states.setHasReadyTray(true);
                states.setHasPulledTray(false);
            }
        }
        if(!is_run)break;
        //弹夹出盘并拉盘到拉备用位置
        if((states.allowChangeTray()||states.allowReadyTray())&&(!states.hasReadyTray())&&(!states.hasPulledTray())&&states.entanceClipReady()&&states.readyToPushReadyTray())
        {
            if((!clipPushoutTray()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setReadyToPusReadyTray(false);
                    states.setEntanceClipReady(false);
                    continue;
                }
            }
            states.setReadyToPusReadyTray(false);
            states.setEntanceClipReady(false);
            states.setHasPulledTray(true);
            states.setallowReadyTray(false);
        }
        if(!is_run)break;
        //提示换入料弹夹
        if(states.hasEntranceClipEmpty()&&(!states.entanceClipReady()))
        {
            has_task = true;
            if(!moveToChangeClipPos())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
           int alarm_id = sendAlarmMessage(u8"已换弹夹",u8"入料弹夹已空，请更换后再点击确认,谢谢配合！");
            waitMessageReturn(is_run,alarm_id);
            if(is_run)
                states.setHasEntranceClipEmpty(false);
        }
        if(!is_run)break;
        //出料弹夹到位
        if((!states.hasExitClipFull())&&(!states.exitClipReady()))
        {
            has_task = true;
            if((!existClipMoveToNextPos()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            else
            {
                if(!tray_clip_out->updateCurrentIndex())
                    states.setHasExitClipFull(true);
                states.setExitClipReady(true);
            }
        }
        if(!is_run)break;
    }
    states.setRunMode(RunMode::Normal);
    qInfo("tray load module end of thread");
}

bool TrayLoaderModule::resumeState()
{
    if(states.isFirstTray())
    {
        bool result = cylinder_tray->Set(true);
        if(result)
            result &=  motor_work->MoveToPosSync(parameters.ltlPressPos());
        if(!result)
            AppendError(u8"lens换盘线程状态恢复");
        qInfo(u8"lens换盘线程状态恢复 %d",result);
        return result;
    }
    return true;
    //todo
}

void TrayLoaderModule::runHandle()
{
    is_run = true;
    bool has_task = false;
    bool allow_change_tray;
    while(is_run)
    {
//        if(!has_task)
        QThread::msleep(100);
        has_task = false;
        {
            QMutexLocker temp_locker(&tray_mutex);
            allow_change_tray = states.allowChangeTray();
        }
        if(allow_change_tray)
        {
            has_task = true;
            if(!moveToChangeChangeHandlly())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            int alarm_id = sendAlarmMessage(u8"已放盘",u8"请放入新料盘在载盘，谢谢配合！");
            waitMessageReturn(is_run,alarm_id);
            if(!is_run)break;
            if(!moveToWorkPos())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_NEXT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(NEXT_OPERATION == operation)
                    continue;
            }
            {
                QMutexLocker temp_locker(&tray_mutex);
                states.setAllowChangeTray(false);
                states.setIsExchangeTray(false);
            }
            sendMessageToModule("LensLoaderModule","FinishChangeTray");
        }
    }
}

bool TrayLoaderModule::moveEntranceClipToPos(int layer_index)
{
    bool result = motor_clip_in->MoveToPosSync(tray_clip_in->getPosition(layer_index));
    if(!result)
        AppendError(QString(u8"移动进料弹夹去%1层失败").arg(layer_index));
    if(parameters.openQinfo())
        qInfo(u8"移动进料弹夹去%d层 %d",layer_index,result);
    return result;
}

bool TrayLoaderModule::moveEntranceClipToWaitPos()
{
    bool result = motor_clip_in->MoveToPosSync(tray_clip_in->standards_parameters.changeClipPos());
    if(!result)
        AppendError(QString(u8"移动进料弹夹去换弹夹位置失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动进料弹夹去换弹夹位置 %d",result);
    return result;
}

bool TrayLoaderModule::moveEntranceClipToTop()
{
    bool result = motor_clip_in->MoveToPosSync(tray_clip_in->standards_parameters.firstTrayPos());
    if(!result)
        AppendError(QString(u8"移动进料弹夹去起始层失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动进料弹夹去起始层 %d",result);
    return result;
}

bool TrayLoaderModule::moveEntranceClipToBottom()
{
    bool result = motor_clip_in->MoveToPosSync(tray_clip_in->standards_parameters.lastTrayPos());
    if(!result)
        AppendError(QString(u8"移动进料弹夹去结束层失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动进料弹夹去结束层 %d",result);
    return result;
}

bool TrayLoaderModule::moveExitClipToPos(int layer_index)
{
    bool result = motor_clip_out->MoveToPosSync(tray_clip_out->getPosition(layer_index));
    if(!result)
        AppendError(QString(u8"移动出料弹夹去%1层失败").arg(layer_index));
    if(parameters.openQinfo())
        qInfo(u8"移动出料弹夹去%d层 %d",layer_index,result);
    return result;
}

bool TrayLoaderModule::moveExitClipToTop()
{
    bool result = motor_clip_out->MoveToPosSync(tray_clip_out->standards_parameters.firstTrayPos());
    if(!result)
        AppendError(QString(u8"移动出料弹夹去起始层失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动出料弹夹去起始层 %d",result);
    return result;
}

bool TrayLoaderModule::moveExitClipToBottom()
{
    bool result = motor_clip_out->MoveToPosSync(tray_clip_out->standards_parameters.lastTrayPos());
    if(!result)
        AppendError(QString(u8"移动出料弹夹去结束层失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动出料弹夹去结束层 %d",result);
    return result;
}

bool TrayLoaderModule::moveEntranceKickToReadyPos()
{
    bool result = cylinder_ltk1->Set(false);
    if(result)
        result &= motor_in->MoveToPosSync(parameters.ltkx1PressPos());
    if(!result)
        AppendError(QString(u8"移动入口钩到准备位置失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动入口钩到准备位置 %d",result);
    return result;
}

bool TrayLoaderModule::moveEntranceKickToPullPos()
{
    bool result = motor_in->SlowMoveToPosSync(parameters.ltkx1ReleasePos(),parameters.pushVelocity());
    result &= cylinder_ltk1->Set(false);
    if(!result)
        AppendError(QString(u8"移动入口钩到拉盘结束位置失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动入口钩到拉盘结束位置 %d",result);
    return result;
}

bool TrayLoaderModule::moveEntranceKickToPushPos()
{
    bool result = motor_in->SlowMoveToPosSync(parameters.ltkx1RelayPos(),parameters.pushVelocity());
    result &= cylinder_ltk1->Set(false);
    if(!result)
        AppendError(QString(u8"移动入口钩到推盘结束位置失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动入口钩到推盘结束位置 %d",result);
    return result;
}

bool TrayLoaderModule::moveExitKickToReadyPos()
{
    bool result = cylinder_ltk2->Set(false);
    if(result)
        result &= motor_out->MoveToPosSync(parameters.ltkx2PressPos());
    if(!result)
        AppendError(QString(u8"移动出口钩到准备位置失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动出口钩到准备位置 %d",result);
    return result;
}

bool TrayLoaderModule::moveExitKickToPushPos()
{
    bool result = motor_out->SlowMoveToPosSync(parameters.ltkx2ReleasePos(),parameters.pushVelocity());
    result &= cylinder_ltk2->Set(false);
    if(!result)
        AppendError(QString(u8"移动出口钩到推盘结束位置失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动出口钩到推盘结束位置 %d",result);
    return result;
}

bool TrayLoaderModule::moveTrayLoaderToWaitPos()
{
    bool result = cylinder_tray->Set(true);
    if(result)
        result &= motor_work->MoveToPosSync(parameters.ltlPressPos());
    if(!result)
        AppendError(QString(u8"移动载盘到等待位置失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动载盘到等待位置 %d",result);
    return result;
}

bool TrayLoaderModule::moveTrayLoaderToWorkPos()
{
    bool result = motor_work->MoveToPosSync(parameters.ltlWorkPos());
    if(!result)
        AppendError(QString(u8"移动载盘到工作位置失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动载盘到工作位置 %d",result);
    return result;
}

bool TrayLoaderModule::moveTrayLoaderToReleasePos()
{
    bool result = motor_work->MoveToPosSync(parameters.ltlReleasePos());
    result &= cylinder_tray->Set(true);
    if(!result)
        AppendError(QString(u8"移动载盘到放盘位置失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动载盘到放盘位置 %d",result);
    return result;
}

bool TrayLoaderModule::checkWorkTray(bool check_state)
{
   if(work_tray_check_io->checkState(check_state)||states.runMode() == RunMode::NoMaterial)
       return true;
   AppendError(QString(u8"%1的状态不为%2").arg(work_tray_check_io->Name()).arg(check_state));
   return false;
}

bool TrayLoaderModule::checkEntranceTray(bool check_state)
{
    if(entrance_tray_check_io->checkState(check_state)||states.runMode() == RunMode::NoMaterial)
        return true;
    AppendError(QString(u8"%1的状态不为%2").arg(entrance_tray_check_io->Name()).arg(check_state));
    return false;
}

bool TrayLoaderModule::ejectTray()
{
    bool res = cylinder_clip->Set(true);
    if(!res){
        qInfo(u8"入料弹夹气缸推出失败");
    }
    res &= cylinder_clip->Set(false);
    if(!res){
        qInfo(u8"入料弹夹气缸收回失败");
    }
    return res;
}
