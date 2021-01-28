#include "sensortrayloadermodule.h"
#include "tcpmessager.h"
#include <QElapsedTimer>
SensorTrayLoaderModule::SensorTrayLoaderModule():ThreadWorkerBase ("SensorTrayLoaderModule")
{

}

void SensorTrayLoaderModule::Init(XtMotor *motor_tray, XtMotor *motor_kick, XtMotor *motor_stie, XtMotor *motor_stoe, XtMotor *motor_push,
                                  XtCylinder *kick1, XtCylinder *kick2, XtCylinder *hold_tray, XtCylinder *hold_vacancy, XtCylinder *entrance_clip_push, XtCylinder *exit_clip_push, XtCylinder *gripper,
                                  SensorClip *entrance_clip, SensorClip *exit_clip,
                                  XtGeneralInput* entrance_clip_check_io,XtGeneralInput* exit_clip_check_io,XtGeneralInput* ready_tray_check_io,XtGeneralInput* kick_tray_check_io,XtGeneralInput* sensor_tray_check_io,XtGeneralInput* vacancy_tray_check_io,XtGeneralInput* entrance_tray_check_io,XtGeneralInput* exit_tray_check_io)
{
    this->motor_tray = motor_tray;
    parts.append(this->motor_tray);
    this->motor_kick = motor_kick;
    parts.append(this->motor_kick);
    this->motor_stie = motor_stie;
    parts.append(this->motor_stie);
    this->motor_stoe = motor_stoe;
    parts.append(this->motor_stoe);
    this->motor_push = motor_push;
    parts.append(this->motor_push);
    this->kick1 = kick1;
    parts.append(this->kick1);
    this->kick2 = kick2;
    parts.append(this->kick2);
    this->hold_tray = hold_tray;
    parts.append(this->hold_tray);
    this->hold_vacancy = hold_vacancy;
    parts.append(this->hold_vacancy);
    this->entrance_clip_push = entrance_clip_push;
    parts.append(this->entrance_clip_push);
    this->exit_clip_push = exit_clip_push;
    parts.append(this->exit_clip_push);
    this->gripper =  gripper;
    parts.append(this->gripper);
    this->entrance_clip = entrance_clip;
    parts.append(this->entrance_clip);
    this->exit_clip = exit_clip;
    parts.append(this->exit_clip);

    this->entrance_clip_check_io = entrance_clip_check_io;
    parts.append(entrance_clip_check_io);
    this->exit_clip_check_io = exit_clip_check_io;
    parts.append(exit_clip_check_io);
    this->ready_tray_check_io = ready_tray_check_io;
    parts.append(ready_tray_check_io);
    this->kick_tray_check_io = kick_tray_check_io;
    parts.append(kick_tray_check_io);
    this->sensor_tray_check_io = sensor_tray_check_io;
    parts.append(sensor_tray_check_io);
    this->vacancy_tray_check_io = vacancy_tray_check_io;
    parts.append(vacancy_tray_check_io);
    this->entrance_tray_check_io = entrance_tray_check_io;
    parts.append(entrance_tray_check_io);
    this->exit_tray_check_io = exit_tray_check_io;
    parts.append(exit_tray_check_io);

    if (gripper)
        this->parameters.setCylinderGripperIoName(this->gripper->parameters.oneOutName());
    if (hold_tray)
        this->parameters.setCylinderHoldTrayIoName(this->hold_tray->parameters.oneOutName());
    if (hold_vacancy)
        this->parameters.setCylinderVacancyTrayIoName(this->hold_vacancy->parameters.oneOutName());
    if (kick1)
        this->parameters.setCylinderSTK1IoName(this->kick1->parameters.oneOutName());
    if (kick2)
        this->parameters.setCylinderSTK2IoName(this->kick2->parameters.oneOutName());
}

void SensorTrayLoaderModule::startWork(int run_mode)
{
    if(run_mode == RunMode::AAFlowChartTest || run_mode == RunMode::UNLOAD_ALL_LENS) return;
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
    if(run_params.contains("HandlyChangeSensorTray"))
    {
        states.setHandlyChangeTray(run_params["HandlyChangeSensorTray"].toBool());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数HandlyChangeSensorTray缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(run_params.contains("HandlyChangeSensor"))
    {
        states.setHandlyChangeSensor(run_params["HandlyChangeSensor"].toBool());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数HandlyChangeSensor缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(states.handlyChangeSensor())
        return;
    qInfo("SensorLoader start run_mode :%d in %d",run_mode,QThread::currentThreadId());
    if(run_mode == RunMode::Normal||run_mode == RunMode::NoMaterial)
    {
        if(states.handlyChangeTray())
            runHandly();
        else
            run();
    }
}

void SensorTrayLoaderModule::stopWork(bool wait_finish)
{
    qInfo("Sensor Loader stop");
    is_run = false;
}

void SensorTrayLoaderModule::performHandlingOperation(int cmd,QVariant param)
{
    bool result = false;
    int temp_value = 10;
    if (cmd%temp_value == HandlePosition::INPUT_MAGAZINE_POS1)
    {

    }
    else if (cmd%temp_value == HandlePosition::INPUT_MAGAZINE_POS2)
    {

    }
    if (cmd%temp_value == HandlePosition::OUTPUT_MAGAZINE_POS1)
    {

    }
    else if (cmd%temp_value == HandlePosition::OUTPUT_MAGAZINE_POS2)
    {

    }
    cmd = cmd/temp_value*temp_value;
    temp_value = 100;
    if (cmd%temp_value == HandleAction::STPO_PUSH)
    {
        result &= motor_push->MoveToPosSync(parameters.pushoutPosition());
        result &= motor_push->MoveToPosSync(0);
    }
    is_handling = false;
    return;
}


void SensorTrayLoaderModule:: run()
{
    is_run = true;
    retryTime = parameters.checkEntranceTrayRetryTimes();
    while (is_run)
    {
        QThread::msleep(10);

        //去拿盘位置
        if(states.needChangeTray()&&(!states.hasCarrierReady())&&(!(states.hasUpTray()&&states.hasReadyTray()))&&(!states.hasKickReady())&&(!states.hasWorkTray())&&(!states.hasKickTray()))
        {
            if((!moveToUpReadyTray(states.hasReadyTray())))
            {
                AppendError(u8"去顶起空盘失败！");
                int alarm_id = sendAlarmMessage(CONTINUE_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setHasReadyTray(false);
                    states.setHasUpTray(false);
                    continue;
                }
            }
            if(states.hasReadyTray())
                states.setHasUpTray(true);
            states.setHasReadyTray(false);
            states.setHasCarrierReady(true);
        }
        //夹盘
        if(states.needChangeTray()&&states.hasCarrierReady()&&(!states.hasKickReady())&&(!states.hasWorkTray())&&(!states.hasKickTray())&&(!states.hasReadyTray())&&states.entranceClipReady())
        {
            if(!moveToPullNextTray())
            {
                if (retryTime > 1)
                {
                    retryTime--;
                    states.setEntranceClipReady(false);
                    states.setHasCarrierReady(false);
                    continue;
                }
                else {
                    retryTime = parameters.checkEntranceTrayRetryTimes();
                    AppendError(u8"去拉入空盘失败！");
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_NEXTMAGAZINE_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if( RETRY_OPERATION == operation)
                    {
                        states.setEntranceClipReady(false);
                        states.setHasCarrierReady(false);
                        continue;
                    }
                    else if (NEXTMAGAZINE_OPERATION == operation)
                    {
                        entrance_clip->setChangeState(true);
                        states.setEntranceClipReady(false);
                        states.setHasCarrierReady(false);
                        moveToEntranceClipNextPos();
                        continue;
                    }
                }
            }
            retryTime = parameters.checkEntranceTrayRetryTimes();

            states.setHasCarrierReady(false);
            states.setHasReadyTray(true);
            states.setEntranceClipReady(false);
            if(states.isFirstTray()&&states.hasUpTray())
            {
                states.setIsFirstTray(false);
                states.setHasWorkTray(true);
            }
        }
        //放下第一个盘
        if(states.needChangeTray()&&states.hasReadyTray()&&(!states.hasUpTray())&&(!states.hasWorkTray()))
        {
            if((!moveToPutFirstTray()))
            {
                AppendError(u8"去放空盘失败！");
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
        }
        //去准备推盘位置
        //去放工作盘
        if(states.needChangeTray()&&states.hasReadyTray()&&states.hasWorkTray()&&(!states.hasKickTray())&&(!states.hasKickReady()))
        {
            if((!moveToDownTrayAndReadyToPush()))
            {
                AppendError(u8"去放工作盘位置失败！");
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setHasWorkTray(false);
            states.setHasKickTray(true);
            states.setHasKickReady(true);
        }
        //去空盘位置
        //去顶盘
        if(states.needChangeTray()&&(!states.hasCarrierReady())&&states.hasReadyTray()&&(!states.hasWorkTray())&&states.hasKickTray()&&states.hasKickReady())
        {
            if((!moveToChangeVacancyTrayAndUpReadyTray(states.hasProductTray())))
            {
                AppendError(u8"换空盘并顶盘失败！");
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            if(!is_run)break;
            if(states.hasReadyTray())
                states.setHasUpTray(true);
            states.setHasReadyTray(false);
            states.setHasKickTray(false);
            states.setHasVacancyTray(true);
            states.setHasCarrierReady(true);
            sendMessageToModule("SensorLoaderModule","FinishChangeTray2");
        }
        //夹盘
        //推出成品盘
        if(states.needChangeTray()&&states.hasCarrierReady()&&(!states.hasWorkTray())&&(!states.hasKickTray())&&states.hasVacancyTray()&&(!states.hasReadyTray())&&states.hasKickReady()&&states.entranceClipReady()&&states.exitClipReady())
        {
            if(!moveToPullNextTray1())
            {
                if (retryTime > 0)
                {
                    retryTime--;
                    states.setEntranceClipReady(false);
                    movetoGetTrayPosition();
                    continue;
                }
                else {
                    retryTime = parameters.checkEntranceTrayRetryTimes();
                    AppendError(u8"取盘失败！");
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
            }
            retryTime = parameters.checkEntranceTrayRetryTimes();
            if(states.hasProductTray())
            {
                states.setExitClipReady(false);
                states.setHasProductTray(false);
            }
            states.setHasCarrierReady(false);
            states.setHasReadyTray(true);
            states.setEntranceClipReady(false);
            states.setHasKickReady(false);
        }

        // Check stop Condition
        if (states.isLastTray()&&checkSensorTray(false, false)&&checkVacancyTray(false, false))
        {
            is_run = false;
            sendMessageToModule("LogicManager2", "StopAllWorkers");
            sendAlarmMessage(CONTINUE_OPERATION, u8"当前生产结束，重置逻辑再开始！");
            break;
        }

        //去工作位置
        if(states.needChangeTray()&&(!states.hasWorkTray())&&states.hasVacancyTray()&&states.hasReadyTray()&&states.hasUpTray()&&(!states.hasKickReady()))
        {
            if(!moveToWorkPos())
            {
                AppendError(u8"去工作位置失败！");
                int alarm_id = sendAlarmMessage(CONTINUE_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    hold_tray->Set(true);
                    states.setHasUpTray(false);
                    continue;
                }
            }
            states.setHasUpTray(false);
            states.setNeedChangeTray(false);
            states.setHasWorkTray(true);
            if (states.isLastTray()&&checkSensorTray(false, false)) {
                qInfo(u8"去工作位置 check no sensor tray");
                sendMessageToModule("SensorLoaderModule","NoSensorTray");
            }
            else {
                sendMessageToModule("SensorLoaderModule","FinishChangeTray1");
            }
            states.setChangingTray(false);
        }

        //检测换进料弹夹
        if(!states.hasEntranceClip())
        {
            if (states.isLastTray())
            {
                states.setHasEntranceClip(true);
            }
            else
            {
                if(!entrance_clip_push->Set(states.useSpareEntanceClip()))
                {
                    AppendError(u8"切换sensor进料弹夹失败。");
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
                retryTime = parameters.checkEntranceTrayRetryTimes();
                int alarm_id = sendAlarmMessage(CONTINUE_OPERATION,u8"换进料弹夹：请在备用sensor进料弹夹位置放入带料弹夹!");
                waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                states.setHasEntranceClip(true);
            }
        }
        //检测换出料弹夹
        if(!states.hasExitClip())
        {
            if(!exit_clip_push->Set(!states.useSpareExitClip()))
            {
                AppendError(u8"切换sensor出料弹夹失败。");
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            int alarm_id = sendAlarmMessage(CONTINUE_OPERATION,u8"换出料弹夹：请在备用sensor出料弹夹位置放入空弹夹!");
            waitMessageReturn(is_run,alarm_id);
            if(!is_run)break;
            states.setHasExitClip(true);
        }

        //进盘弹夹到位
        if(!states.entranceClipReady())
        {
            if(states.isLastTray())
            {
                states.setEntranceClipReady(true);
            }
            else
            {
                if((!moveToEntranceClipNextPos()))
                {
                    AppendError(u8"进盘弹夹到位失败！");
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
                states.setEntranceClipReady(true);
            }
        }
        //出盘弹夹到位
        if(!states.exitClipReady())
        {
            if((!moveToExitClipNextPos()))
            {
                AppendError(u8"出盘弹夹到位失败！");
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            else
                states.setExitClipReady(true);
        }
        //todo提示换盘
    }
    states.setRunMode(RunMode::Normal);
    qInfo("sensor tray loader end of thread");
}

void SensorTrayLoaderModule::runHandly()
{
    qInfo("runHandly");
    is_run = true;
    while (is_run) {
        if(!states.needChangeTray())
        {
            QThread::msleep(1000);
            continue;
        }
        if(states.needChangeTray()&&(!states.hasWorkTray()))
        {
            if(!moveToWorkPos(false))
            {
                AppendError(u8"去工作位置失败！");
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            bool result = hold_tray->Set(true);
            result &= hold_vacancy->Set(true);
            if(!result)
            {
                AppendError(u8"放下料盘失败");
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setHasWorkTray(true);
        }
        if(states.needChangeTray())
        {
            int alarm_id = sendAlarmMessage(u8"已换盘",u8"请手动更换1号sensor料盘和2号sensor料盘，谢谢配合!");
            waitMessageReturn(is_run,alarm_id);
            if(!is_run)break;
        }
        if(states.needChangeTray()&&states.hasWorkTray())
        {
            bool result = hold_tray->Set(false);
            result &= hold_vacancy->Set(false);
            result &= kick1->Set(true);
            if(!result)
            {
                AppendError(u8"顶起料盘失败");
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            if(!moveToWorkPos())
            {
                AppendError(u8"去工作位置失败！");
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setHasWorkTray(false);
            states.setNeedChangeTray(false);
            sendMessageToModule("SensorLoaderModule","FinishChangeTray");
            states.setChangingTray(false);
        }
    }
    states.setRunMode(RunMode::Normal);
    qInfo("sensor tray loader end of thread");
}

void SensorTrayLoaderModule::resetLogic()
{
    if(is_run)return;
    states.reset();

    entrance_clip->resetClip();
    exit_clip->resetClip();
    qInfo("SensorTrayLoaderModule::resetLogic");
}

bool SensorTrayLoaderModule::movetoSTIEColumnIndex(int n)
{
    if (!motor_stie || !entrance_clip) {
        qWarning("Missing STIE motor or Entrance clip");
        return false;
    }
    if (entrance_clip) entrance_clip->setCurrentIndex(n);
    bool result = motor_stie->MoveToPosSync(entrance_clip->getCurrentPosition());
    return result;
}

bool SensorTrayLoaderModule::movetoSTOEColumnIndex(int n)
{
    if (!exit_clip || !motor_stoe) {
        qWarning("Missing STOE motor or Exit clip");
        return false;
    }
    exit_clip->setCurrentIndex(n);
    bool result = motor_stoe->MoveToPosSync(exit_clip->getCurrentPosition());
    return result;
}

bool SensorTrayLoaderModule::movetoPushMotorSafePosotion()
{
    return motor_push->MoveToPosSync(parameters.pushMotorSafePosition());
}

bool SensorTrayLoaderModule::movetoDownTrayPosition()
{
    return motor_tray->MoveToPosSync(parameters.downTrayPosition());
}

bool SensorTrayLoaderModule::movetoGetTrayPosition()
{
    bool result = gripper->Set(true);
    if(result)
        result &= motor_tray->MoveToPosSync(parameters.getTrayPosition());
    result &= gripper->Set(false);
    return result;
}

bool SensorTrayLoaderModule::movetoFinishKickTrayPosition()
{
    return motor_kick->SlowMoveToPosSync(parameters.finishKickTrayPosition(),parameters.pushVelocity());
}

bool SensorTrayLoaderModule::movetoPushoutPosition()
{
    return motor_push->MoveToPosSync(parameters.pushoutPosition());
}

bool SensorTrayLoaderModule::movetoPutTrayPosition()
{
    return motor_tray->MoveToPosSync(parameters.putTrayPosition());
}

bool SensorTrayLoaderModule::movetoStartKickTrayPosition()
{
    return motor_kick->MoveToPosSync(parameters.startKickTrayPosition());
}

bool SensorTrayLoaderModule::movetoTrayWorkPosition()
{
    return motor_tray->MoveToPosSync(parameters.trayWorkPosition());
}

bool SensorTrayLoaderModule::movetoVacancyTrayPosition()
{
    return motor_kick->SlowMoveToPosSync(parameters.vacancyTrayPosition(),parameters.pushVelocity());
}

bool SensorTrayLoaderModule:: moveToDownTrayAndReadyToPush()
{
    bool result = true;
    bool check_result = true;
    bool kick_result = kick1->Set(false);
    kick_result &= kick2->Set(false);
    if(kick_result)
    kick_result &= motor_kick->MoveToPos(parameters.startKickTrayPosition());
    result = motor_tray->MoveToPos(parameters.downTrayPosition());
    if (!states.isLastTray())
    {
        check_result &= checkSensorTray(true);
    }
    //check_result &= checkKickTray(false);
    if(result)
        result &= motor_tray->WaitArrivedTargetPos(parameters.downTrayPosition());
    if(result)
    {
        result &= hold_tray->Set(true);
        result &= gripper->Set(true);
        if(result)
            motor_tray->MoveToPos(parameters.getTrayPosition());//提前动作
    }
    if(kick_result)
        kick_result &= motor_kick->WaitArrivedTargetPos(parameters.startKickTrayPosition());
    qInfo(u8"去放下盘并准备推盘，返回值%d",kick_result&&result&&check_result);
    return kick_result&&result&&check_result;
}

bool SensorTrayLoaderModule::moveToChangeVacancyTrayAndUpReadyTray(bool has_vacancy_tray)
{
    bool kick_result = kick1->Set(true,false);
    kick_result &= hold_vacancy->Set(true);
    if(kick_result&&has_vacancy_tray)
        kick_result &= kick2->Set(true);
    kick_result &= kick1->Wait(true);
    //去放空盘
    kick_result &= checkExitTray(false);
    if(kick_result)
        kick_result &= motor_kick->SlowMoveToPos(parameters.vacancyTrayPosition(),parameters.pushVelocity());

    //去放顶盘
    bool result = true;
    if (!states.isLastTray())
        checkEntanceTray(false);
    if(result)
        result &= motor_tray->MoveToPos(parameters.getTrayPosition());
    //放空盘
    if(kick_result)
        kick_result &= motor_kick->WaitArrivedTargetPos(parameters.vacancyTrayPosition());
    if(kick_result)
        kick_result &= kick1->Set(false);
    if(kick_result)
        kick_result &= hold_vacancy->Set(false);
    if(kick_result&&(!states.isLastTray()))
        kick_result &= checkVacancyTray(true);
    if(kick_result)
        kick_result &= motor_kick->WaitArrivedTargetPos(parameters.vacancyTrayPosition());
    //推出满盘
    if(kick_result)
    {
        if(has_vacancy_tray)
        {
            kick_result &= motor_kick->SlowMoveToPosSync(parameters.finishKickTrayPosition(),parameters.pushVelocity());
            kick_result &= kick2->Set(false);
            if(kick_result)
                kick_result &= motor_kick->MoveToPosSync(parameters.finishKickTrayPosition() - parameters.backDistance());
            if(kick_result)
                kick_result &= kick1->Set(true);
        }
        else
        {
            motor_kick->MoveToPosSync(parameters.finishKickTrayPosition() - parameters.backDistance());
            kick_result &= kick1->Set(true);
        }
    } else {
        qInfo("Error handling for the sensor tray handling");
        bool kick_resul2 = true;
        if(has_vacancy_tray)
        {
            kick_resul2 &= motor_kick->SlowMoveToPosSync(parameters.finishKickTrayPosition(),parameters.pushVelocity());
            kick_resul2 &= kick2->Set(false);
            if(kick_resul2)
                kick_resul2 &= motor_kick->MoveToPosSync(parameters.finishKickTrayPosition() - parameters.backDistance());
            if(kick_resul2)
                kick_resul2 &= kick1->Set(true);
        }
        else
        {
            motor_kick->MoveToPosSync(parameters.finishKickTrayPosition() - parameters.backDistance());
            kick_resul2 &= kick1->Set(true);
        }
    }
    kick_result &= checkExitTray(false);

    //顶盘
    if(result)
        result &= motor_tray->WaitArrivedTargetPos(parameters.getTrayPosition());
    if(result)
    {
        result &= hold_tray->Set(false);
        result &= gripper->Set(false);
    }
    if(result&&(!states.isLastTray()))
        result &= checkReadyTray(true);
    qInfo(u8"去换空盘并取备用盘，返回值%d",result&&kick_result);
    return result&&kick_result;
}

bool SensorTrayLoaderModule::moveToUpReadyTray(bool has_tray)
{
    QElapsedTimer timer; timer.start();
    bool result =checkEntanceTray(false);
    result &= gripper->Set(true);
    if(result)
        result &= motor_tray->MoveToPosSync(parameters.getTrayPosition());
    if(result)
    {
        result &= hold_tray->Set(false);
        result &= gripper->Set(false);
    }
    if((result&&has_tray)&&(!states.isLastTray()))
        result &= checkSensorTray(true);
    qInfo(u8"去取备用盘，返回值%d",result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool SensorTrayLoaderModule::moveToWaitHandleTray()
{
    bool result = motor_tray->MoveToPosSync(parameters.waitVacancyTrayPosition());
    if(result)
    {
        result &= hold_tray->Set(false);
        result &= gripper->Set(false);
    }
    qInfo(u8"去等手动放盘，返回值%d",result);
    return result;
}

bool SensorTrayLoaderModule::moveToPullNextTray()
{
    QElapsedTimer timer; timer.start();
    bool result_push = true;
    bool result_return = true;
    if(!states.isLastTray())
    {
        result_push = motor_push->MoveToPosSync(parameters.pushoutPosition()); //STPO推出sensor tray
        if(result_push)
            result_push &= gripper->Set(true,false);    //STL夹爪合上
        result_return = motor_push->MoveToPos(0);
        if(result_push)
            result_push &= gripper->Wait(true);
        if(result_push)
            result_push &= checkEntanceTray(true);
    }

    bool result = motor_tray->MoveToPosSync(parameters.putTrayPosition());  //STL去到放下sensor tray的位置,等待顶盘
    if(result&&(!states.isLastTray()))
        result &= gripper->Set(false);  //STL夹爪松开
    if(result_return&&!states.isLastTray())
        result_return &= motor_push->WaitArrivedTargetPos(0);
    if(result&&(!states.isLastTray()))
        result &= checkEntanceTray(false);
    result = result&&result_push&&result_return;
    qInfo(u8"送出新盘，返回值%d",result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool SensorTrayLoaderModule::moveToPullNextTray1()
{
    bool result_push = motor_push->MoveToPosSync(parameters.pushoutPosition());
    if(result_push)
        result_push &= gripper->Set(true,false);
    bool result_return = motor_push->MoveToPos(0);
    if(result_push)
        result_push &= gripper->Wait(true);

    if(result_push&&(!states.isLastTray()))
        result_push &= checkEntanceTray(true);

    bool result = motor_tray->MoveToPosSync(parameters.putTrayPosition());
    if(result)
        result &= gripper->Set(false);
    if(result_return)
        result_return &= motor_push->WaitArrivedTargetPos(0);
    if(result&&(!states.isLastTray()))
        result &= checkEntanceTray(false);
    result = result&&result_push&&result_return;
    qInfo(u8"送出新盘并推出成品盘，返回值%d",result);
    return result;
}

bool SensorTrayLoaderModule::moveToPutFirstTray()
{
    bool result = gripper->Set(false);
    if(result)
        result &= motor_tray->MoveToPosSync(parameters.putTrayPosition()+ parameters.backDistance());
    if(result)
        result &= hold_tray->Set(true);
    qInfo(u8"放下第一个盘，返回值%d",result);
    return result;
}

bool SensorTrayLoaderModule::moveToPutTray()
{
    bool result = motor_tray->MoveToPosSync(parameters.putTrayPosition());
    if(result)
    {
        result &= gripper->Set(false);
    }
    return result;
}

bool SensorTrayLoaderModule::moveToWorkPos(bool has_tray)
{
    bool result = motor_tray->MoveToPosSync(parameters.trayWorkPosition());
    if(!result)
        AppendError(QString(u8"去工作位置失败"));
    if((result&&has_tray)&&(!states.isLastTray()))
    {
        result &= checkSensorTray(true);
    }
    qInfo(u8"去工作位置,返回值%d",result);
    return result;

}

bool SensorTrayLoaderModule::moveToEntranceClipNextPos()
{
    QElapsedTimer timer; timer.start();
    qInfo("moveToEntranceClipNextPos");
    if(motor_push->GetFeedbackPos() > parameters.pushMotorSafePosition())
    {
        qInfo("motor_push %f",motor_push->GetFeedbackPos());
        AppendError(u8"推出sensor盘的轴(STPO)不在安全位置");
        qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
        return false;
    }
    if(!checkEntanceTray(false))
    {
        AppendError(u8"入料口感应器检测到有tray盘，请手动移走！");
        return false;
    }

    if(entrance_clip->getChangeState())
    {
        states.setUseSpareEntanceClip(!states.useSpareEntanceClip());
        entrance_clip->resetClip();
        if(states.hasAuxiliaryEntranceClip())
        {
            states.setHasAuxiliaryEntranceClip(false);
        }
        else
        {
            int alarm_id = sendAlarmMessage(CONTINUE_LAST_OPERATION,u8"请在备用sensor进料弹夹位置放入带料弹夹!");
            QString operation = waitMessageReturn(is_run,alarm_id);
            if (LAST_OPERATION == operation)
            {
                states.setIsLastTray(true);
                states.setEntranceClipReady(true);
                return true;
            }
            else
            {
                states.setHasAuxiliaryEntranceClip(false);
            }
            if(!is_run)return false;

        }
        //sendAlarmMessage(u8"已放备用进料弹夹",u8"请在备用sensor进料弹夹位置放入带料弹夹!");
    }

    bool result = entrance_clip_push->Set(!states.useSpareEntanceClip());
    //check clip
    result &= motor_stie->MoveToPosSync(entrance_clip->getNextPosition());
    if(result)
        entrance_clip->finishCurrentPosition();
//    retryTime = parameters.checkEntranceTrayRetryTimes();
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool SensorTrayLoaderModule::moveToExitClipNextPos()
{
    if(!checkEntanceTray(false))
    {
        AppendError(u8"出料口感应器检测到有tray盘，请手动移走！");
        return false;
    }

    if(exit_clip->getChangeState())
    {
        states.setUseSpareExitClip(!states.useSpareExitClip());
        exit_clip->resetClip();
        if(states.hasAuxiliaryExitClip())
        {
            states.setHasAuxiliaryExitClip(false);
        }
        else
        {
            int alarm_id = sendAlarmMessage(CONTINUE_OPERATION,u8"请在备用sensor出料弹夹位置放入空弹夹!");
            waitMessageReturn(is_run,alarm_id);
            if(!is_run)return false;
            states.setHasAuxiliaryExitClip(false);
        }
        //sendAlarmMessage(u8"已放备用出料弹夹",u8"请在备用sensor出料弹夹位置放入空弹夹!");
    }

    bool result = exit_clip_push->Set(states.useSpareExitClip());
    result &= motor_stoe->MoveToPosSync(exit_clip->getNextPosition());
    if(!result)
        AppendError(QString(u8"去出口弹夹盘下一个位置失败"));
    if(result)
        exit_clip->finishCurrentPosition();
    qInfo(u8"去出口弹夹盘下一个位置，返回值%d",result);
    return result;
}

bool SensorTrayLoaderModule::checkEntanceTray(bool check_state)
{
    if(entrance_tray_check_io->Value() == check_state||states.runMode() == RunMode::NoMaterial)
        return true;
    QString logic_state = check_state?u8"有":u8"无";
    QString checked_state = check_state?u8"无":u8"有";
    //AppendError(QString(u8"轨道入口处逻辑%1盘，但检测到%2盘！").arg(logic_state).arg(checked_state));
    qInfo(u8"轨道入口处逻辑%s盘，但检测到%s盘！",logic_state.toStdString().c_str(),checked_state.toStdString().c_str());
    return false;
}

bool SensorTrayLoaderModule::checkSensorTray(bool check_state, bool showErrorMsg)
{
    if(sensor_tray_check_io->Value() == check_state||states.runMode() == RunMode::NoMaterial)
        return true;
    QString logic_state = check_state?u8"有":u8"无";
    QString checked_state = check_state?u8"无":u8"有";
    if (showErrorMsg)
    {
        AppendError(QString(u8"sensor盘爪上逻辑%1盘，但检测到%2盘！").arg(logic_state).arg(checked_state));
        qInfo(u8"sensor盘爪上逻辑%s盘，但检测到%s盘！",logic_state.toStdString().c_str(),checked_state.toStdString().c_str());
    }
    return false;
}

bool SensorTrayLoaderModule::checkKickTray(bool check_state)
{
    if(kick_tray_check_io->Value() == check_state||states.runMode() == RunMode::NoMaterial)
        return true;
    QString logic_state = check_state?u8"有":u8"无";
    QString checked_state = check_state?u8"无":u8"有";
    AppendError(QString(u8"放取空的Sensor盘位置逻辑%1盘，但检测到%2盘！").arg(logic_state).arg(checked_state));
    qInfo(u8"放取空的Sensor盘位置逻辑%s盘，但检测到%s盘！",logic_state.toStdString().c_str(),checked_state.toStdString().c_str());
    return false;
}

bool SensorTrayLoaderModule::checkReadyTray(bool check_state)
{
    if(ready_tray_check_io->Value() == check_state||states.runMode() == RunMode::NoMaterial)
        return true;
    QString logic_state = check_state?u8"有":u8"无";
    QString checked_state = check_state?u8"无":u8"有";
    AppendError(QString(u8"备用盘位置逻辑%1盘，但检测到%2盘！").arg(logic_state).arg(checked_state));
    qInfo(u8"备用盘位置逻辑%s盘，但检测到%s盘！",logic_state.toStdString().c_str(),checked_state.toStdString().c_str());
    return false;
}

bool SensorTrayLoaderModule::checkVacancyTray(bool check_state, bool showErrorMsg)
{
    if(vacancy_tray_check_io->Value() == check_state||states.runMode() == RunMode::NoMaterial)
        return true;
    QString logic_state = check_state?u8"有":u8"无";
    QString checked_state = check_state?u8"无":u8"有";
    if (showErrorMsg)
    {
        AppendError(QString(u8"成品盘爪上逻辑%1盘，但检测到%2盘!").arg(logic_state).arg(checked_state));
        qInfo(u8"成品盘爪上位置逻辑%s盘，但检测到%s盘!",logic_state.toStdString().c_str(),checked_state.toStdString().c_str());
    }
    return false;
}

bool SensorTrayLoaderModule::checkExitTray(bool check_state)
{
    if(exit_tray_check_io->Value() == check_state||states.runMode() == RunMode::NoMaterial)
        return true;
    QString logic_state = check_state?u8"有":u8"无";
    QString checked_state = check_state?u8"无":u8"有";
    AppendError(QString(u8"轨道出口位置逻辑%1盘，但检测到%2盘!").arg(logic_state).arg(checked_state));
    qInfo(u8"轨道出口位置逻辑%s盘，但检测到%s盘!",logic_state.toStdString().c_str(),checked_state.toStdString().c_str());
    return false;
}

PropertyBase *SensorTrayLoaderModule::getModuleState()
{
    return &states;
}

void SensorTrayLoaderModule::receivceModuleMessage(QVariantMap message)
{
    qInfo("receive module message %s",TcpMessager::getStringFromQvariantMap(message).toStdString().c_str());
    if(!message.contains("OriginModule"))
    {
        qInfo("message error! has no OriginModule.");
        return;
    }
    if(message["OriginModule"].toString() == "SensorLoaderModule" || message["OriginModule"].toString() == "SensorTrayLoaderModule")
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString()=="ChangeTrayResquest")
            {
                if(states.changingTray())
                {
                    qInfo("receive ChangeTrayResquest in changingTray");
                    return;
                }
                states.setNeedChangeTray(true);
                if(states.hasVacancyTray())
                    states.setHasProductTray(true);
                states.setChangingTray(true);
            }
        }
        else
        {
            qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            return;
        }
    }
    else if(message["OriginModule"].toString() == "AlarmModule")
    {
        if(message["Operation"].toString() == u8"已放备用进料弹夹")
        {
            states.setHasAuxiliaryEntranceClip(true);
        }
        else if(message["Operation"].toString() == u8"已放备用出料弹夹")
        {
            states.setHasAuxiliaryExitClip(true);
        }
    }
    else
    {
        qInfo("module name error %s",message["OriginModule"].toString().toStdString().c_str());
        return;
    }
}

QMap<QString, PropertyBase *> SensorTrayLoaderModule::getModuleParameter()
{
    QMap<QString, PropertyBase *> temp;
    return temp;
}

void SensorTrayLoaderModule::setModuleParameter(QMap<QString, PropertyBase *>)
{

}
