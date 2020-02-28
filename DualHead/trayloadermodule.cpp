﻿#include "DualHead/trayloadermodule.h"

#include <QDebug>
#include <QObject>

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
                            XtGeneralInput* _tray_entry_input,
                            XtGeneralInput*_tray_exit_input,
                            XtGeneralInput*tray_check_input)
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
    this->tray_clip = trayClipIn;
    this->tray_clip_out = trayClipOut;
    this->tray_entry_input = _tray_entry_input;
    this->tray_exit_input = _tray_exit_input;
    this->tray_check_input = tray_check_input;
}

void TrayLoaderModule::resetLogic()
{
    if(is_run)return;
    states.setHasTrayReady(false);

    states.setHasWorkTray(false);
    states.setIsFirstTray(true);
    states.setHasReadyTray(false);
    states.setHasPulledTray(false);
    states.setIsExchangeTray(false);
    states.setAllowChangeTray(false);
    states.setHasExitClipFull(false);
    tray_clip->reset();
    states.setHasEntranceClipEmpty(false);
    tray_clip_out->reset();
    states.setEntanceClipReady(false);
    states.setExitClipReady(false);
    states.setReadyToPusReadyTray(false);
    states.setReadyToPushEmptyTray(false);
}

void TrayLoaderModule::performHandling(int cmd)
{
    emit sendHandlingOperation(cmd);
}

bool TrayLoaderModule::startUp()
{
    if(!moveToLtlSetPos()){
        qDebug()<<"LTL_X move to setpos failed";
        return false;
    }
    onReset();
    return true;
}

//void TrayLoaderModule::run(bool has_tray)
//{
//    is_run = true;
//    while(is_run){
//        QThread::msleep(100);

//        bool result = false;
//        if(!states.hasTrayReady()){
//            result = motorOutRelease();
//            if(!result){
//                AppendError(tr(u8"LTK_X2释放失败,是否继续移动LTLX"));
//                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
//                result = waitMessageReturn(is_run);
//                if(!result){
//                    is_run = false;
//                    continue;
//                }
//            }
//            result = moveToSafePos();
//            if(!result){
//                AppendError(tr(u8"LTL_X未移动到工作位置，是否出盘"));
//                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
//                result = waitMessageReturn(is_run);
//                if(result){
//                    states.setHasTrayReadyPosClear(true);
//                }else{
//                    continue;
//                }
//            }else{
//                states.setHasTrayReadyPosClear(true);
//            }
//            if(!is_run)continue;

//            result = ejectTray();
//            if(!result){
//                AppendError(tr(u8"发射tray盘失败，是否继续"));
//                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
//                result = waitMessageReturn(is_run);
//                if(result){
//                    states.setHasPrevTrayEjected(true);
//                    states.setHasEntranceTrayPulledAway(false);
//                }else{
//                    continue;
//                }
//            }else{
//                states.setHasPrevTrayEjected(true);
//                states.setHasEntranceTrayPulledAway(false);
//            }
//            if(!is_run)continue;

//            if(states.hasTrayReadyPosClear()){
//                if(!motorInRelease()){
//                    AppendError(tr(u8"LTK_X1释放失败,是否继续移动LTk_X1"));
//                    sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
//                    result = waitMessageReturn(is_run);
//                    if(!result){
//                        is_run = false;
//                        continue;
//                    }
//                }
//                if(!moveToReadyFirstKick()){
//                    AppendError(tr(u8"LTK_X1拉tray盘就位失败，是否继续"));
//                    sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
//                    result = waitMessageReturn(is_run);
//                    if(result){
//                        states.setHasLTKX1ReadytoPull(true);
//                    }else{
//                        continue;
//                    }
//                }else{
//                    states.setHasLTKX1ReadytoPull(true);
//                }
//                if(!is_run)continue;

//                if(states.hasLTKX1ReadytoPull()){
//                    result = moveToFirstKick();
//                    if(!motorInRelease()){
//                        AppendError(tr(u8"LTK_X1释放失败,是否继续移动LTk_X1"));
//                        sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
//                        result = waitMessageReturn(is_run);
//                        if(!result){
//                            is_run = false;
//                            continue;
//                        }
//                    }
//                    result &= moveToReadySecondKick();
//                    if(!result){
//                        AppendError(tr(u8"tray盘到准备位置失败，是否继续"));
//                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
//                        result = waitMessageReturn(is_run);
//                        if(result){
//                            states.setHasTrayReady(true);
//                            states.setHasLTKX1ReadytoPull(false);
//                            states.setHasEntranceTrayPulledAway(true);
//                        }else{
//                            continue;
//                        }
//                    }else{
//                        states.setHasTrayReady(true);
//                        states.setHasLTKX1ReadytoPull(false);
//                        states.setHasEntranceTrayPulledAway(true);
//                    }
//                }
//                if(!is_run)continue;
//                //moveToNextTrayPos();
//            }
//            if(!is_run)break;
//        }
//        if(states.hasTrayUsed()){
//            result = motorOutRelease();
//            if(!result){
//                AppendError(tr("LTK_X2释放失败，是否继续？"));
//                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
//                if(!waitMessageReturn(is_run)){
//                    is_run = false;
//                    continue;
//                }
//            }
//            if(!moveToTrayOutHandOverPos()){
//                AppendError(tr(u8"tray盘到准备位置失败，是否继续"));
//                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
//                result = waitMessageReturn(is_run);
//                if(result){
//                    states.setHasTrayOutHandOverReady(true);
//                }else{
//                    continue;
//                }
//            }else{
//                states.setHasTrayOutHandOverReady(true);
//            }
//            if(!is_run)continue;
//            if(states.hasTrayOutHandOverReady()){
//                if(!motorWorkRelease()){
//                    AppendError(tr("LTL_X释放失败，是否继续？"));
//                    sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
//                    if(!waitMessageReturn(is_run)){
//                        is_run = false;
//                        continue;
//                    }
//                }
//                if(!motorOutPress()){
//                    AppendError(tr("LTk_X2下压失败，是否继续？"));
//                    sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
//                    if(!waitMessageReturn(is_run)){
//                        is_run = false;
//                        continue;
//                    }
//                }
//                states.setHasTrayOutHandOverReady(false);
//            }
//            if(!is_run)continue;
//            result = moveToTrayInHandOverPos();
//            if(!motorInRelease()){
//                AppendError(tr(u8"LTK_X1释放失败，是否继续？"));
//                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
//                if(!waitMessageReturn(is_run)){
//                    is_run = false;
//                    continue;
//                }
//            }
//            result &= motor_work->WaitArrivedTargetPos(parameters.ltlPressPos());
//            motorWorkPress();
//            //
//            result &= moveToSafePos();
//            if(!result){
//                AppendError(tr(u8"换盘失败，是否继续？"));
//                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
//                if(!waitMessageReturn(is_run)){
//                    is_run = false;
//                    continue;
//                }
//            }
//            emit trayReady();
//            //emit parameters.trayReady();

//            result = motor_out->WaitArrivedTargetPos(parameters.ltkx2ReleasePos());
//            motorOutRelease();

//            moveToNextEmptyPos();
//            states.setHasTrayReady(false);
//            states.setHasTrayUsed(false);
//            if(!is_run)break;
//        }
//        if(states.hasPrevTrayEjected()&&states.hasEntranceTrayPulledAway()){
//            moveToNextTrayPos();
//            states.setHasPrevTrayEjected(false);
//        }
//    }

//}

bool TrayLoaderModule::moveToNextTrayPos()
{
    //    if(!tray_entry_input->Value()){
    //        //qDebug()<<"入口处检测到有盘";
    //        AppendError(u8"入口处检测到有盘,请把盘拿走");
    //        sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
    //        waitMessageReturn(is_run);
    //        return false;
    //    }
    double pos = tray_clip->getCurrentPosition();
    motor_clip_in->MoveToPos(pos);
    bool result = motor_clip_in->WaitArrivedTargetPos(pos);
    if(result){
        if(tray_clip->standards_parameters.currentIndex()+1<tray_clip->standards_parameters.columnCount())
            tray_clip->standards_parameters.setCurrentIndex(tray_clip->standards_parameters.currentIndex()+1);
        else{
            AppendError(tr(u8"进盘弹夹已用完，请更换弹夹后继续下一步操作"));
            sendAlarmMessage(ErrorLevel::WarningBlock,tr(u8"更换完毕后点击继续"));
            waitMessageReturn(is_run);
            tray_clip->standards_parameters.setCurrentIndex(0);
        }
    }
    return result;
}

bool TrayLoaderModule::ejectTray()
{
    bool res = cylinder_clip->Set(true);
    if(!res){
        qDebug()<<"弹出失败";
    }
    //cylinder_clip->Set(true);
    res &= cylinder_clip->Set(false);
    if(!res){
        qDebug()<<"retraction failed";
    }
    return res;
}

bool TrayLoaderModule::motorInPress()
{
    int res = cylinder_ltk1->Set(true);
    return res;
}

bool TrayLoaderModule::moveToReadyFirstKick()
{
    bool result = motor_in->MoveToPosSync(parameters.ltkx1PressPos());
    result &= motorInPress();
    return result;
}

bool TrayLoaderModule::moveToFirstKick()
{
    return motor_in->MoveToPosSync(parameters.ltkx1ReleasePos());
}

bool TrayLoaderModule::moveToReadySecondKick()
{
    bool result = motor_in->MoveToPosSync(parameters.ltkx1RelayPos());
    result &= motorInPress();
    return result;
}

bool TrayLoaderModule::moveToLtkx1GetPos()
{
    if(!cylinder_tray->Value()){
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"检测到tray盘升起状态，是否继续操作？"))){
            return true;
        }
    }
    motor_in->MoveToPos(parameters.ltkx1PressPos());
    bool result = motor_in->WaitArrivedTargetPos(parameters.ltkx1PressPos());
    return result;
}

bool TrayLoaderModule::moveToLtkx1RelayPos()
{
    if(!cylinder_tray->Value()){
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"检测到tray盘升起状态，是否继续操作？"))){
            return true;
        }
    }
    motor_in->MoveToPos(parameters.ltkx1RelayPos());
    bool result = motor_in->WaitArrivedTargetPos(parameters.ltkx1RelayPos());
    return result;
}

bool TrayLoaderModule::moveToLtkx1SetPos()
{
    motor_in->MoveToPos(parameters.ltkx1ReleasePos());
    bool result = motor_in->WaitArrivedTargetPos(parameters.ltkx1ReleasePos());
    return result;
}

bool TrayLoaderModule::motorInRelease()
{
    int res =cylinder_ltk1->Set(false);
    return res;
    /*
    int res = cylinder_ltk1->Value();
    if(res){
        res = cylinder_ltk1->Set(false);
        return res;
    }
    return 1;
//*/
}

bool TrayLoaderModule::moveToLtlGetPos()
{
    motor_work->MoveToPos(parameters.ltlPressPos());
    bool result = motor_work->WaitArrivedTargetPos(parameters.ltlPressPos());
    return result;
}

bool TrayLoaderModule::moveToSafePos()
{
    return motor_work->MoveToPosSync(parameters.ltlPressPos()+200);
}

bool TrayLoaderModule::motorWorkPress()
{
    int res = cylinder_tray->Value();
    if(res){
        res = cylinder_tray->Set(false);
        return res;
    }
    return 1;
}

bool TrayLoaderModule::moveToLtlSetPos()
{
    motor_work->MoveToPos(parameters.ltlReleasePos());
    bool result = motor_work->WaitArrivedTargetPos(parameters.ltlReleasePos(),10000);
    return result;
}

bool TrayLoaderModule::motorWorkRelease()
{
    int res = cylinder_tray->Set(true);
    return true;
}

bool TrayLoaderModule::moveToTrayOutHandOverPos()
{
    motor_out->MoveToPos(parameters.ltkx2PressPos());
    motor_work->MoveToPos(parameters.ltlReleasePos());
    bool result = motor_work->WaitArrivedTargetPos(parameters.ltlReleasePos());
    result &= motor_out->WaitArrivedTargetPos(parameters.ltkx2PressPos());
    return result;
}

bool TrayLoaderModule::moveToTrayInHandOverPos()
{
    bool result;
    motor_out->MoveToPos(parameters.ltkx2ReleasePos());
    motor_in->MoveToPos(parameters.ltkx1ReleasePos());
    motor_work->MoveToPos(parameters.ltlPressPos());
    result = motor_in->WaitArrivedTargetPos(parameters.ltkx1ReleasePos());
    return result;
}

bool TrayLoaderModule::moveToLtkx2GetPos()
{
    motor_out->MoveToPos(parameters.ltkx2PressPos());
    bool result = motor_out->WaitArrivedTargetPos(parameters.ltkx2PressPos());
    return result;
}

bool TrayLoaderModule::motorOutPress()
{
    int res = cylinder_ltk2->Set(true);
    return res;
}

bool TrayLoaderModule::moveToLtkx2SetPos()
{
    motor_out->MoveToPos(parameters.ltkx2ReleasePos());
    bool result = motor_out->WaitArrivedTargetPos(parameters.ltkx2ReleasePos());
    return result;
}

bool TrayLoaderModule::motorOutRelease()
{
    int res = cylinder_ltk2->Set(false);
    return res;
}

bool TrayLoaderModule::moveToNextEmptyPos()
{
    if(tray_exit_input->Value()){
        qDebug()<<"出口处检测到有盘";
        AppendError(QString(u8"出口处有盘，请把盘移走"));
        sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
        waitMessageReturn(is_run);
        return false;
    }
    double pos = tray_clip_out->getCurrentPosition();
    motor_clip_out->MoveToPos(pos);
    bool result = motor_clip_out->WaitArrivedTargetPos(pos);
    if(result){
        if(tray_clip_out->standards_parameters.currentIndex()+1<tray_clip_out->standards_parameters.columnCount())
            tray_clip_out->standards_parameters.setCurrentIndex(tray_clip_out->standards_parameters.currentIndex()+1);
        else{
            AppendError(tr(u8"出盘弹夹已装满，请更换弹夹后继续下一步操作"));
            sendAlarmMessage(Continue,tr(u8"更换完毕后点击继续"));
            waitMessageReturn(is_run);
            tray_clip_out->standards_parameters.setCurrentIndex(0);
        }
    }
    return result;
}

bool TrayLoaderModule::LTIEMovetoColumnIndex(int col)
{
    if(col<tray_clip->standards_parameters.columnCount()){
        double pos = tray_clip->standards_parameters.firstTrayPos() + col*tray_clip->standards_parameters.columnDelta();
        motor_clip_in->MoveToPos(pos);
        int res = motor_clip_in->WaitArrivedTargetPos(pos);
        return res;
    }else{
        return false;
    }
}

bool TrayLoaderModule::LTOEMovetoColumnIndex(int col)
{
    if(col<tray_clip_out->standards_parameters.columnCount()){
        double pos = tray_clip_out->standards_parameters.firstTrayPos() + col*tray_clip_out->standards_parameters.columnDelta();
        motor_clip_out->MoveToPos(pos);
        int res = motor_clip_out->WaitArrivedTargetPos(pos);
        return res;
    }else{
        return false;
    }
}

bool TrayLoaderModule::sendoutAndReayPushOutEmptyTray(bool check_tray)
{
    bool result = cylinder_ltk2->Set(false);
    if(!result)return false;
    result &= motor_work->MoveToPos(parameters.ltlReleasePos());
    result &= motor_out->MoveToPos(parameters.ltkx2PressPos());
    result &= motor_work->WaitArrivedTargetPos(parameters.ltlReleasePos());
    if(result)
        result &= cylinder_tray->Set(true);
    if(result&&check_tray)
    {
        result &= tray_check_input->Value();
        if(!result)
            AppendError(u8"盘托上未检测到料盘！");
    }
    result &= motor_out->WaitArrivedTargetPos(parameters.ltkx2PressPos());
    if(!result)
        AppendError(QString(u8"放出空盘并准备推空盘失败"));
    if(parameters.openQinfo())
        qInfo(u8"放出空盘并准备推空盘失败 %d",result);
    return result;
}

bool TrayLoaderModule::moveToGetAndPushInNewTrayAndPushOutTray(bool check_tray)
{
    bool result = cylinder_tray->Set(true);
    if(result)
        result &= motor_work->MoveToPos(parameters.ltlPressPos());
    bool out_result = cylinder_ltk2->Set(true);
    if(out_result)
        out_result &= motor_out->MoveToPos(parameters.ltkx2ReleasePos());
    bool in_result = cylinder_ltk1->Set(true);
    if(in_result)
        in_result &= motor_in->MoveToPos(parameters.ltkx1ReleasePos());
    result &= motor_work->WaitArrivedTargetPos(parameters.ltkx2PressPos());
    in_result &= motor_in->WaitArrivedTargetPos(parameters.ltkx1ReleasePos());
    if(in_result)
        in_result &= cylinder_ltk1->Set(false);
    out_result &= motor_out->WaitArrivedTargetPos(parameters.ltkx2ReleasePos());
    if(out_result)
    {
        out_result &= cylinder_ltk2->Set(false);
        out_result &=motor_out->MoveToPosSync(parameters.ltlReleasePos() - parameters.ltkx2SafeDistance());
    }
    if(!result)
        AppendError(QString(u8"去拿新盘、推入新盘并推出空盘失败"));
    if(parameters.openQinfo())
        qInfo(u8"去拿新盘、推入新盘并推出空盘失败 %d",result&&out_result&&in_result);
    return result&&out_result&&in_result;
}

bool TrayLoaderModule::moveToGetAndPushInNewTray(bool check_tray)
{
    bool result = cylinder_tray->Set(true);
    if(result)
        result &= motor_work->MoveToPos(parameters.ltlPressPos());
    bool in_result = cylinder_ltk1->Set(true);
    if(in_result)
        in_result &= motor_in->MoveToPos(parameters.ltkx1ReleasePos());
    result &= motor_work->WaitArrivedTargetPos(parameters.ltkx2PressPos());
    in_result &= motor_in->WaitArrivedTargetPos(parameters.ltkx1ReleasePos());
    if(in_result)
        in_result &= cylinder_ltk1->Set(false);
    if(!result)
        AppendError(QString(u8"去拿新盘并推入新盘失败"));
    if(parameters.openQinfo())
        qInfo(u8"去拿新盘并推入新盘失败 %d",result&&in_result);
    return result&&in_result;
}

bool TrayLoaderModule::moveToPushOutTray()
{
    bool result = cylinder_ltk2->Set(true);
    if(result)
        result &= motor_out->MoveToPos(parameters.ltkx2ReleasePos());
    if(result)
    {
        result &= cylinder_ltk2->Set(false);
        result &=motor_out->MoveToPosSync(parameters.ltlReleasePos() - parameters.ltkx2SafeDistance());
    }
    if(!result)
        AppendError(QString(u8"推出空盘失败"));
    if(parameters.openQinfo())
        qInfo(u8"推出空盘失败 %d",result);
    return result;
}

bool TrayLoaderModule::moveToWorkPosAndReayPullNewTray()
{
    bool in_result = cylinder_ltk1->Set(false);
    int result = true;
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
    return result&&in_result;
}

bool TrayLoaderModule::entranceClipMoveToNextPos()
{
    bool result = motor_clip_in->MoveToPosSync(tray_clip->getCurrentPosition());
    if(!result)
        AppendError(QString(u8"移动进料弹夹失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动进料弹夹 %d",result);
    return result;
}

bool TrayLoaderModule::moveToReayPullNewTray()
{
    bool result = cylinder_ltk1->Set(false);
    if(result)
        result &= motor_in->MoveToPosSync(parameters.ltkx1PressPos());
    if(!result)
        AppendError(QString(u8"去拉盘准备位置失败"));
    if(parameters.openQinfo())
        qInfo(u8"去拉盘准备位置 %d",result);
    return result;
}

bool TrayLoaderModule::moveToPushReadyTray()
{
    bool result = cylinder_ltk1->Set(true);
    if(result)
        result &= motor_in->MoveToPosSync(parameters.ltkx1ReleasePos());
    if(result)
        result &= cylinder_ltk1->Set(false);
    if(!result)
        AppendError(QString(u8"推备用盘失败"));
    if(parameters.openQinfo())
        qInfo(u8"推备用盘失败 %d",result);
    return result;
}

bool TrayLoaderModule::clipPushoutTray(bool check_tray)
{
    bool result = cylinder_ltk1->Set(false);
    if(result)
        result &= cylinder_clip->Set(true);
    if(result)
        result &= cylinder_clip->Set(false);
    if(result&&check_tray)
    {
        result &= tray_entry_input->Value();
        if(!result)
            AppendError(u8"入料口未检测到料盘！");
    }
    bool in_result = cylinder_ltk1->Set(true);
    if(in_result)
        in_result &= motor_in->MoveToPosSync(parameters.ltkx1ReleasePos());
    in_result &= cylinder_ltk1->Set(false);
    if(!result)
        AppendError(QString(u8"进料弹夹出盘失败"));
    if(parameters.openQinfo())
        qInfo(u8"进料弹夹出盘 %d",result);
    return result;
}

bool TrayLoaderModule::existClipMoveToNextPos()
{
    bool result = motor_clip_out->MoveToPosSync(tray_clip_out->getCurrentPosition());
    if(!result)
        AppendError(QString(u8"移动出料弹夹失败"));
    if(parameters.openQinfo())
        qInfo(u8"移动出料弹夹 %d",result);
    return result;
}

bool TrayLoaderModule::moveToChangeChangeHandlly()
{
    bool result = cylinder_ltk1->Set(false);
    if(result)
        result &= motor_in->MoveToPosSync(0);
    if(result)
        result &= motor_work->MoveToPosSync(parameters.ltlPressPos());
    if(result)
        result &= cylinder_tray->Set(true);
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

void TrayLoaderModule::startWork(int run_mode)
{
    if(run_mode == RunMode::Normal)run(true);
    else if(run_mode == RunMode::NoMaterial)run(false);
    else if(run_mode == RunMode::ChangeLensTrayHandle) runHandle();
}

void TrayLoaderModule::stopWork(bool wait_finish)
{
    is_run = false;
}

void TrayLoaderModule::performHandlingOperation(int cmd)
{

}

void TrayLoaderModule::onNextTrayPos()
{
    moveToNextTrayPos();
    ejectTray();
}

void TrayLoaderModule::onLtkx1Pickup()
{
    if(!motorInPress()){
        return;
    }
    if(!moveToLtkx1SetPos()){
        qDebug()<<"LTK_X1 move to setpos false";
        return;
    }
    if(!motorInRelease()){
        qDebug()<<"LTK_X1 realease false";
        return;
    }
    if(!moveToLtkx1RelayPos()){
        return;
    }
    if(!motorInPress()){
        return;
    }
    if(!moveToLtkx1SetPos()){
        return;
    }
    Sleep(rand()%5000);
    qDebug()<<"2.ltk_x1 has picked up prev tray,emit nextTrayPos signal eject next one";
    emit nextTrayPos();
    if(!motorInRelease()){
        return;
    }
    if(!moveToLtkx1GetPos()){
        return;
    }
}

void TrayLoaderModule::onLtlxPickup()
{
    if(!moveToLtlGetPos()){
        return;
    }
    if(!motorWorkPress()){
        return;
    }
    qDebug()<<"3.emit trayReady signal to start working";
    emit parameters.trayReady();//debug
    //emit trayReady();
}

void TrayLoaderModule::onLtlxPutdown()
{
    if(!moveToLtlSetPos()){
        qDebug()<<"LTLX move to setpos false";
        return;
    }
    if(!motorWorkRelease()){
        qDebug()<<"LTLX Release false";
        return;
    }
    Sleep(rand()%5000);
    qDebug()<<"4.ltl_x put down prev tray, emit ltkx2Pickup signal and ltlxPickup signal";
    emit ltkx2Pickup();
    emit ltlxPickup();
}

void TrayLoaderModule::onLtkx2Pickup()
{
    //*
    if(!motorOutPress()){
        return;
    }
    if(!moveToLtkx2SetPos()){
        return;
    }
    if(!motorOutRelease()){
        return;
    }
    if(!moveToLtkx2GetPos()){
        return;
    }
    //*/
    qDebug()<<"5.recieve a tray,move to next empty pos";
    emit nextEmptyPos();
}

void TrayLoaderModule::onNextEmptyPos()
{
    moveToNextEmptyPos();
}

void TrayLoaderModule::onTestTrayUsed()
{
    //LTL_X 与 LTK_X2 会合
    /*
    motorOutRelease();
    motor_work->MoveToPos(parameters.ltlReleasePos());
    motor_out->MoveToPos(parameters.ltkx2PressPos());
    bool result = motor_work->WaitArrivedTargetPos(parameters.ltlReleasePos());
    result &= motor_out->WaitArrivedTargetPos(parameters.ltkx2PressPos());
    if(!result){
        return;
    }
    if(!motorWorkRelease()){
        return;
    }
    if(!motorOutPress()){
        return;
    }
    //LTL_X 与 LTK_X1 会合，LTK_X2出盘
    motor_out->MoveToPos(parameters.ltkx2ReleasePos());
    motor_in->MoveToPos(parameters.ltkx1ReleasePos());
    motor_work->MoveToPos(parameters.ltlPressPos());
    result = motor_in->WaitArrivedTargetPos(parameters.ltkx1ReleasePos());
    result &= motor_work->WaitArrivedTargetPos(parameters.ltlPressPos());
    if(!result){
        return;
    }
    if(!motorInRelease()){
        return;
    }
    if(!motorWorkPress()){
        return;
    }
    result = motor_out->WaitArrivedTargetPos(parameters.ltkx2ReleasePos());
    if(!result){
        return;
    }
    if(!motorOutRelease()){
        return;
    }
    qDebug()<<"trayReady.....";
    if(!moveToLtlSetPos()){
        return;
    }
    emit parameters.trayReady();
    emit onReset();
//*/
    states.setHasTrayUsed(true);
    qInfo("receive change tray");
    QMutexLocker temp_locker(&tray_mutex);
    if(states.isExchangeTray())
        return;
    states.setAllowChangeTray(true);
    states.setIsExchangeTray(true);
}

void TrayLoaderModule::onTestLTLXPickUpTray()
{
    if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"测试前先将轨道上tray盘拿走，是否开始测试？"))){
        return;
    }
    motorInRelease();
    motorWorkRelease();
    ejectTray();
    moveToNextTrayPos();
    motor_in->MoveToPos(parameters.ltkx1PressPos());
    motor_in->WaitArrivedTargetPos(parameters.ltkx1PressPos());
    motorInPress();
    motor_in->MoveToPos(parameters.ltkx1ReleasePos());
    motor_in->WaitArrivedTargetPos(parameters.ltkx1ReleasePos());
    motorInRelease();
    motor_in->MoveToPos(parameters.ltkx1RelayPos());
    motor_in->WaitArrivedTargetPos(parameters.ltkx1RelayPos());
    motorInPress();
    motor_in->MoveToPos(parameters.ltkx1ReleasePos());
    motor_work->MoveToPos(parameters.ltlPressPos());
    int result = motor_in->WaitArrivedTargetPos(parameters.ltkx1ReleasePos());
    result &= motor_work->WaitArrivedTargetPos(parameters.ltlPressPos());
    if(result){
        motorInRelease();
        motorWorkPress();
    }
}
void TrayLoaderModule::onTestLTLXPutDownTray()
{
    if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"测试前先将轨道上多余tray盘拿走，是否开始测试？"))){
        return;
    }
    motorOutRelease();
    motor_work->MoveToPos(parameters.ltlReleasePos());
    motor_out->MoveToPos(parameters.ltkx2PressPos());
    int result = motor_work->WaitArrivedTargetPos(parameters.ltlReleasePos());
    result &= motor_out->WaitArrivedTargetPos(parameters.ltkx2ReleasePos());
    if(result){
        motorWorkRelease();
        motorOutPress();
    }
    motor_out->MoveToPos(parameters.ltkx2ReleasePos());
    result = motor_out->WaitArrivedTargetPos(parameters.ltkx2ReleasePos());
    moveToNextEmptyPos();
}

void TrayLoaderModule::onReset(){
    onNextTrayPos();
    onNextEmptyPos();
    if(!moveToLtkx1GetPos()){
        return;
    }
    if(!motorInPress()){
        return;
    }
    if(!moveToLtkx1SetPos()){
        return;
    }
    if(!motorInRelease()){
        return;
    }
    if(!moveToLtkx1RelayPos()){
        return;
    }
    if(!motorInPress()){
        return;
    }
    qDebug()<<"Reset completete...";
}



void TrayLoaderModule::run(bool has_tray)
{
    if(!resumeState())
    {
        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
        return;
    }
    is_run = true;
    bool has_task = false;
    bool allow_change_tray;
    while(is_run){
        if(!has_task)
            QThread::msleep(1000);
        has_task = false;
        {
            QMutexLocker temp_locker(&tray_mutex);
            allow_change_tray = states.allowChangeTray();
        }
        //送出空盘
        //准备推出空盘
        if(allow_change_tray&&states.hasReadyTray()&&states.hasWorkTray()&&(!states.readyToPushEmptyTray()))
        {
            has_task = true;
            if((!sendoutAndReayPushOutEmptyTray(has_tray))&&has_tray)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(!waitMessageReturn(is_run))
                {
                    states.setHasWorkTray(false);
                    states.setReadyToPushEmptyTray(true);
                }
            }
            else
            {
                states.setHasWorkTray(false);
                states.setReadyToPushEmptyTray(true);
            }
        }
        //去取盘
        //推入满盘
        //推出空盘
        if(allow_change_tray&&states.hasReadyTray()&&(!states.isReadyTrayPushed())&&(!states.hasWorkTray())&&states.readyToPushReadyTray()&&states.readyToPushEmptyTray()&&states.exitClipReady())
        {
            has_task = true;
            if((!moveToGetAndPushInNewTrayAndPushOutTray(has_tray))&&has_tray)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(!waitMessageReturn(is_run))
                {
                    states.setReadyToPusReadyTray(false);
                    states.setReadyToPushEmptyTray(false);
                    states.setExitClipReady(false);
                    states.setIsReadyTrayPushed(true);
                }
            }
            else
            {
                states.setReadyToPusReadyTray(false);
                states.setReadyToPushEmptyTray(false);
                states.setExitClipReady(false);
                states.setIsReadyTrayPushed(true);
            }
        }
        //无空盘时推出满盘
        if(allow_change_tray&&states.hasReadyTray()&&(!states.isReadyTrayPushed())&&(!states.hasWorkTray())&&states.readyToPushReadyTray())
        {
            has_task = true;
            if(!moveToGetAndPushInNewTray(has_tray)&&has_tray)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(!waitMessageReturn(is_run))
                {
                    states.setReadyToPusReadyTray(false);
                    states.setIsReadyTrayPushed(true);
                }
            }
            else
            {
                states.setReadyToPusReadyTray(false);
                states.setIsReadyTrayPushed(true);
            }
        }
        //到工作位置
        //准备接备用盘
        if(allow_change_tray&&states.hasReadyTray()&&states.isReadyTrayPushed()&&(!states.hasWorkTray())&&(!states.readyToPushEmptyTray()))
        {
            has_task = true;
            if((!moveToWorkPosAndReayPullNewTray())&&has_tray)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setHasReadyTray(false);
                }
                else
                {
                    {
                        QMutexLocker temp_locker(&tray_mutex);
                        states.setAllowChangeTray(false);
                        states.setIsExchangeTray(false);
                    }
                    emit trayReady();
                    if(states.isFirstTray())
                        states.setIsFirstTray(false);
                    states.setHasReadyTray(false);
                    states.setHasWorkTray(true);
                    states.setReadyToPusReadyTray(true);
                    states.setIsReadyTrayPushed(false);
                }
            }
            else
            {
                {
                    QMutexLocker temp_locker(&tray_mutex);
                    states.setAllowChangeTray(false);
                    states.setIsExchangeTray(false);
                }
                emit trayReady();
                if(states.isFirstTray())
                    states.setIsFirstTray(false);
                states.setHasReadyTray(false);
                states.setHasWorkTray(true);
                states.setReadyToPusReadyTray(true);
                states.setIsReadyTrayPushed(false);
            }
        }
        //无备用盘时推出空盘
        if((!states.hasReadyTray())&&(!states.hasWorkTray())&&states.readyToPushEmptyTray()&&states.exitClipReady())
        {
            has_task = true;
            if(!moveToPushOutTray())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setExitClipReady(false);
                }
            }
            else
            {

                states.setExitClipReady(false);
            }
            states.setReadyToPushEmptyTray(false);
        }
        //提示换出料弹夹
        if(states.hasExitClipFull()&&(!states.exitClipReady()))
        {
            has_task = true;
            sendAlarmMessage(ErrorLevel::WarningBlock,u8"出料弹夹已满，请更换后再点击确认,谢谢配合！");
            waitMessageReturn(is_run);
            if(is_run)
                states.setHasExitClipFull(false);
        }
        //入料弹夹到位
        if((!states.hasEntranceClipEmpty())&&(!states.entanceClipReady()))
        {
            has_task = true;
            if((!entranceClipMoveToNextPos())&&has_tray)
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            else
            {
                if(!tray_clip->updateCurrentIndex())
                    states.setHasEntranceClipEmpty(true);
                states.setEntanceClipReady(true);
            }
        }
        //准备接备用盘
        if((!states.readyToPushReadyTray()))
        {
            has_task = true;
            if((!moveToReayPullNewTray())&&has_tray)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(!waitMessageReturn(is_run))
                {
                    states.setReadyToPusReadyTray(true);
                    if(states.hasPulledTray())
                    {
                        states.setHasReadyTray(true);
                        states.setHasPulledTray(false);
                    }
                }
            }
            else
            {
                states.setReadyToPusReadyTray(true);
                if(states.hasPulledTray())
                {
                    states.setHasReadyTray(true);
                    states.setHasPulledTray(false);
                }

            }
        }
        //弹夹出盘并拉盘到拉备用位置
        if((!states.hasReadyTray())&&(!states.hasPulledTray())&&states.entanceClipReady()&&states.readyToPushReadyTray())
        {
            has_task = true;
            if((!clipPushoutTray(has_task))&&has_tray)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setEntanceClipReady(false);
                }
                else
                {
                    states.setReadyToPusReadyTray(false);
                    states.setEntanceClipReady(false);
                    states.setHasPulledTray(true);
                }
            }
            else
            {
                states.setReadyToPusReadyTray(false);
                states.setEntanceClipReady(false);
                states.setHasPulledTray(true);
            }
        }
        //提示换入料弹夹
        if(states.hasEntranceClipEmpty()&&(!states.entanceClipReady()))
        {
            has_task = true;
            sendAlarmMessage(ErrorLevel::WarningBlock,u8"入料弹夹已满，请更换后再点击确认,谢谢配合！");
            waitMessageReturn(is_run);
            if(is_run)
                states.setHasEntranceClipEmpty(false);
        }
        //出料弹夹到位
        if((!states.hasExitClipFull())&&(!states.exitClipReady()))
        {
            has_task = true;
            if((!existClipMoveToNextPos())&&has_tray)
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            else
            {
                if(!tray_clip->updateCurrentIndex())
                    states.setHasExitClipFull(true);
                states.setExitClipReady(true);
            }
        }
    }

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
        if(!has_task)
            QThread::msleep(1000);
        has_task = false;
        {
            QMutexLocker temp_locker(&tray_mutex);
            allow_change_tray =states.allowChangeTray();
        }
        if(allow_change_tray)
        {
            has_task = true;
            if(moveToChangeChangeHandlly())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                    continue;
                if(!is_run)break;
            }
            sendAlarmMessage(ErrorLevel::WarningBlock,u8"请放入新料盘在夹爪上方，谢谢配合！");
            waitMessageReturn(is_run);
            if(!is_run)break;
            if(moveToWorkPos())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                    continue;
            }
            {
                QMutexLocker temp_locker(&tray_mutex);
                states.setAllowChangeTray(false);
                states.setIsExchangeTray(false);
            }
            emit trayReady();
        }
    }
}
