﻿#include "sensortrayloadermodule.h"

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
}

void SensorTrayLoaderModule::startWork(int run_mode)
{
    qInfo("SensorLoader start run_mode :%d in %d",run_mode,QThread::currentThreadId());
    if(run_mode == RunMode::Normal||run_mode == RunMode::OnllyLeftAA||run_mode == RunMode::OnlyRightAA)
    {
        if(parameters.isHandly())
            runHandly();
        else
            run(true);
    }
    else if(run_mode == RunMode::NoMaterial)
    {
        if(parameters.isHandly())
            runHandly();
        else
            run(false);
    }
}

void SensorTrayLoaderModule::stopWork(bool wait_finish)
{
    qInfo("Sensor Loader stop");
//    if(wait_finish)
//        finish_stop = true;
//    else
        is_run = false;
}

void SensorTrayLoaderModule::performHandlingOperation(int cmd)
{
    is_handling = false;

}

void SensorTrayLoaderModule::receiveChangeTray()
{
    qInfo("receiveChangeTray");
    if(states.changingTray())
        return;
    states.setNeedChangeTray(true);
    if(states.hasVacancyTray())
        states.setHasProductTray(true);
    states.setChangingTray(true);
}

void SensorTrayLoaderModule:: run(bool has_material)
{
    is_run = true;
    while (is_run)
    {
        QThread::msleep(100);

        //去拿盘位置
        if(states.needChangeTray()&&(!states.hasCarrierReady())&&(!(states.hasUpTray()&&states.hasReadyTray()))&&(!states.hasKickReady())&&(!states.hasWorkTray())&&(!states.hasKickTray()))
        {
            if((!moveToUpReadyTray(states.hasReadyTray(),has_material)&&has_material))
            {
                AppendError(u8"去顶起空盘失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setHasReadyTray(false);
                    states.setHasUpTray(false);
                    continue;
                }
                else
                {
                    if(states.hasReadyTray())
                        states.setHasUpTray(true);
                    states.setHasReadyTray(false);
                    states.setHasCarrierReady(true);
                }
                if(!is_run)break;
            }
            else
            {
                if(states.hasReadyTray())
                    states.setHasUpTray(true);
                states.setHasReadyTray(false);
                states.setHasCarrierReady(true);
            }
        }
        //夹盘
        if(states.needChangeTray()&&states.hasCarrierReady()&&(!states.hasKickReady())&&(!states.hasWorkTray())&&(!states.hasKickTray())&&(!states.hasReadyTray())&&states.entranceClipReady())
        {
            if((!moveToPullNextTray(has_material))&&has_material)
            {
                AppendError(u8"去拉入空盘失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setEntranceClipReady(false);
                    states.setHasCarrierReady(false);
                    continue;
                }
                else
                {
                    states.setHasCarrierReady(false);
                    states.setHasReadyTray(true);
                    states.setEntranceClipReady(false);
                    if(states.isFirstTray()&&states.hasUpTray())
                    {
                        states.setIsFirstTray(false);
                        states.setHasWorkTray(true);
                    }

                }
                if(!is_run)break;
            }
            else
            {
                states.setHasCarrierReady(false);
                states.setHasReadyTray(true);
                states.setEntranceClipReady(false);
                if(states.isFirstTray()&&states.hasUpTray())
                {
                    states.setIsFirstTray(false);
                    states.setHasWorkTray(true);
                }
            }
        }
        //放下第一个盘
        if(states.needChangeTray()&&states.hasReadyTray()&&(!states.hasUpTray())&&(!states.hasWorkTray()))
        {
            if((!moveToPutFirstTray())&&has_material)
            {
                AppendError(u8"去放空盘失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                if(!is_run)break;
            }
        }
        //去准备推盘位置
        //去放工作盘
        if(states.needChangeTray()&&states.hasReadyTray()&&states.hasWorkTray()&&(!states.hasKickTray())&&(!states.hasKickReady()))
        {
            if((!moveToDownTrayAndReadyToPush(has_material))&&has_material)
            {
                AppendError(u8"去放工作盘位置失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                else
                {
                    states.setIsFirstTray(true);
                    states.setHasWorkTray(false);
                    states.setHasKickTray(true);
                    states.setHasKickReady(true);
                }
                if(!is_run)break;
            }
            else
            {
                states.setHasWorkTray(false);
                states.setHasKickTray(true);
                states.setHasKickReady(true);
            }
        }
        //去空盘位置
        //去顶盘
        if(states.needChangeTray()&&(!states.hasCarrierReady())&&states.hasReadyTray()&&(!states.hasWorkTray())&&states.hasKickTray()&&states.hasKickReady())
        {
            if((!moveToChangeVacancyTrayAndUpReadyTray(states.hasProductTray(),has_material))&&has_material)
            {
                AppendError(u8"换空盘并顶盘失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                else
                {
                    if(states.hasReadyTray())
                        states.setHasUpTray(true);
                    states.setHasReadyTray(false);
                    states.setHasKickTray(false);
                    states.setHasVacancyTray(true);
                    states.setHasCarrierReady(true);
                }
                if(!is_run)break;
            }
            else
            {
                if(states.hasReadyTray())
                    states.setHasUpTray(true);
                states.setHasReadyTray(false);
                states.setHasKickTray(false);
                states.setHasVacancyTray(true);
                states.setHasCarrierReady(true);
            }
        }
        //夹盘
        //推出成品盘
        if(states.needChangeTray()&&states.hasCarrierReady()&&(!states.hasWorkTray())&&(!states.hasKickTray())&&states.hasVacancyTray()&&(!states.hasReadyTray())&&states.hasKickReady()&&states.entranceClipReady()&&states.exitClipReay())
        {
            if((!moveToPullNextTrayAndPushOutTray(states.hasProductTray(),has_material))&&has_material)
            {
                AppendError(u8"取盘失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(!waitMessageReturn(is_run))
                {
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
                if(!is_run)break;
            }
            else
            {
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
        }
        //去工作位置
        if(states.needChangeTray()&&(!states.hasWorkTray())&&states.hasVacancyTray()&&states.hasReadyTray()&&states.hasUpTray()&&(!states.hasKickReady()))
        {
            if(!moveToWorkPos(has_material))
            {
                AppendError(u8"去工作位置失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    hold_tray->Set(true);
                    states.setHasUpTray(false);
                    continue;
                }
                if(!is_run)break;
            }
            states.setHasUpTray(false);
            states.setNeedChangeTray(false);
            states.setHasWorkTray(true);
            emit sendChangeTrayFinish();
            states.setChangingTray(false);
        }

        //检测换进料弹夹

        //检测换出料弹夹

        //进盘弹夹到位
        if(!states.entranceClipReady())
        {
            if((!moveToEntranceClipNextPos())&&has_material)
            {
                AppendError(u8"进盘弹夹到位失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                else
                {
                    states.setEntranceClipReady(true);
                }
                if(!is_run)break;
            }
            else
            {
                states.setEntranceClipReady(true);
            }
        }
        //出盘弹夹到位
        if(!states.exitClipReay())
        {
            if((!moveToExitClipNextPos())&&has_material)
            {
                AppendError(u8"出盘弹夹到位失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                else
                {
                    states.setExitClipReady(true);
                }
                if(!is_run)break;
            }
            else
            {
                states.setExitClipReady(true);
            }
        }
        //todo提示换盘
    }
    qInfo("sensor tray loader end of thread");
}

void SensorTrayLoaderModule::runHandly()
{
    qInfo("runHandly");
    is_run = true;
    while (is_run) {
        QThread::msleep(1000);
        if(states.needChangeTray())
        {
            if(!moveToWorkPos(false))
            {
                AppendError(u8"去工作位置失败！");
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
           bool result = hold_tray->Set(true);
            result &= hold_vacancy->Set(true);
            if(!result)
            {
                AppendError(u8"放下料盘失败");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                if(!is_run)break;
            }

            sendAlarmMessage(ErrorLevel::TipNonblock,u8"请手动更换sensor料盘和空盘，谢谢配合!");
            waitMessageReturn(is_run);
            if(!is_run)break;

            result = hold_tray->Set(false);
            result &= hold_vacancy->Set(false);
            result &= kick1->Set(true);
            if(!result)
            {
                AppendError(u8"顶起料盘失败");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                if(!is_run)break;
            }
            if(!moveToWorkPos(true))
            {
                AppendError(u8"去工作位置失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                    continue;
            }
            states.setNeedChangeTray(false);
            emit sendChangeTrayFinish();
            states.setChangingTray(false);
        }
    }
}

void SensorTrayLoaderModule::resetLogic()
{
    if(is_run)return;
    states.setHasTrayToGet(false);
    states.setHasVacancyTray(false);
    states.setHandlePutVacancyTray(false);
    states.setFirstTrayIsVacancy(false);
    states.setNeedChangeTray(false);
    states.setChangingTray(false);
    states.setEntranceClipReady(false);
    states.setExitClipReady(false);
    states.setHasGetedTray(false);
    states.setHasWorkTray(false);
    states.setHasKickTray(false);
    states.setNeedChangeVacancyTray(false);
    states.setNeedChangeEntranceClip(false);
    states.setNeedChangeExitClip(false);
    states.setUseSpareEntanceClip(false);
    states.setUseSpareExitClip(false);
    states.setHoldVacancyTray(false);
    states.setGetedVacancyTray(false);
    states.setHasProductTray(false);
    states.setIsFirstTray(true);
    states.setHasReadyTray(false);
    states.setHasUpTray(false);
    states.setHasCarrierReady(false);

    entrance_clip->resetClip();
    exit_clip->resetClip();
    qInfo("SensorTrayLoaderModule::resetLogic");
}

bool SensorTrayLoaderModule::movetoSTIEColumnIndex(int n)
{
    entrance_clip->setCurrentIndex(n);
    bool result = motor_stie->MoveToPosSync(entrance_clip->getCurrentPosition());
    return result;
}

bool SensorTrayLoaderModule::movetoSTOEColumnIndex(int n)
{
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
    return motor_tray->MoveToPosSync(parameters.getTrayPosition());
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

bool SensorTrayLoaderModule:: moveToDownTrayAndReadyToPush(bool need_check)
{
    bool kick_result = kick1->Set(false);
    kick_result &= kick2->Set(false);
    if(kick_result)
        kick_result &= motor_kick->MoveToPos(parameters.startKickTrayPosition());
    bool result = motor_tray->MoveToPos(parameters.downTrayPosition());
    bool check_result = true;
    if(need_check)
    {
        check_result &= checkSensorTray(true);
        check_result &= checkKickTray(false);
    }
    if(result)
        result &= motor_tray->WaitArrivedTargetPos(parameters.downTrayPosition());
    if(result)
    {
        result &= hold_tray->Set(true);
        if(result)
            motor_tray->MoveToPos(parameters.getTrayPosition());//提前动作
    }
    if(kick_result)
        kick_result &= motor_kick->WaitArrivedTargetPos(parameters.startKickTrayPosition());
    qInfo(u8"去放下盘并准备推盘，返回值%d",kick_result&&result&&check_result);
    return kick_result&&result&&check_result;
}

bool SensorTrayLoaderModule::moveToChangeVacancyTrayAndUpReadyTray(bool has_vacancy_tray,bool need_check)
{
    bool kick_result = kick1->Set(true,false);
    kick_result &= hold_vacancy->Set(true);
    if(kick_result&&has_vacancy_tray)
        kick_result &= kick2->Set(true);
    kick_result &= kick1->Wait(true);
    if(kick_result)
        kick_result &= motor_kick->SlowMoveToPos(parameters.vacancyTrayPosition(),parameters.pushVelocity());

    bool result = checkEntanceTray(false);
    if(result)
        result &= motor_tray->MoveToPosSync(parameters.getTrayPosition());
    if(result)
    {
       result &= hold_tray->Set(false);
       result &= gripper->Set(false);
    }
    if(kick_result)
        kick_result &= motor_kick->WaitArrivedTargetPos(parameters.vacancyTrayPosition());
    if(kick_result)
        kick_result &= kick1->Set(false);
    if(kick_result)
        kick_result &= hold_vacancy->Set(false);
    if(need_check&&kick_result)
    {
        kick_result &= checkVacancyTray(true);
        kick_result &= checkReadyTray(true);
    }
    qInfo(u8"去换空盘并取备用盘，返回值%d",result&&kick_result);
    return result&&kick_result;
}

bool SensorTrayLoaderModule::moveToUpReadyTray(bool has_tray,bool need_check)
{
//    bool check_result = true;
//    if(need_check)
//        check_result &= checkReadyTray(has_tray);
     bool result =checkEntanceTray(false);
    if(result)
    result &= motor_tray->MoveToPosSync(parameters.getTrayPosition());
    if(result)
    {
        result &= hold_tray->Set(false);
        result &= gripper->Set(false);
    }
    if(result&&need_check&has_tray)
       result &= checkSensorTray(true);
    qInfo(u8"去取备用盘，返回值%d",result);
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

bool SensorTrayLoaderModule::moveToPullNextTray(bool need_check)
{
    bool result_push = motor_push->MoveToPosSync(parameters.pushoutPosition());
    if(result_push)
        result_push &= gripper->Set(true,false);
    bool result_return = motor_push->MoveToPos(0);
    if(result_push)
        result_push &= gripper->Wait(true);
    if(result_push&&need_check)
        result_push &= checkEntanceTray(true);
    bool result = motor_tray->MoveToPosSync(parameters.putTrayPosition());
    if(result)
        result &= gripper->Set(false);
    if(result_return)
        result_return &= motor_push->WaitArrivedTargetPos(0);

    if(result&&need_check)
        result &= checkEntanceTray(false);
    result = result&&result_push&&result_return;
    qInfo(u8"送出新盘，返回值%d",result);
    return result;
}

bool SensorTrayLoaderModule::moveToPullNextTrayAndPushOutTray(bool has_vacancy_tray,bool need_check)
{
    bool kick_result = kick1->Set(false);
    if(kick_result)
    {
        if(has_vacancy_tray)
            kick_result &= motor_kick->SlowMoveToPos(parameters.finishKickTrayPosition(),parameters.pushVelocity());
        else
            motor_kick->MoveToPos(parameters.finishKickTrayPosition() - parameters.backDistance());
    }
    bool result_push = motor_push->MoveToPosSync(parameters.pushoutPosition());
    if(result_push)
        result_push &= gripper->Set(true,false);
    bool result_return = motor_push->MoveToPos(0);
    if(result_push)
        result_push &= gripper->Wait(true);

    if(result_push&&need_check)
        result_push &= checkEntanceTray(true);

    bool result = motor_tray->MoveToPosSync(parameters.putTrayPosition());
    if(result)
        result &= gripper->Set(false);
    if(result_return)
        result_return &= motor_push->WaitArrivedTargetPos(0);

    if(kick_result)
    {
        if(has_vacancy_tray)
        {
            kick_result &= motor_kick->WaitArrivedTargetPos(parameters.finishKickTrayPosition());
            if(kick_result)
                kick_result &= motor_kick->MoveToPosSync(parameters.finishKickTrayPosition() - parameters.backDistance());
            if(kick_result)
            {
                kick_result &= kick1->Set(true);
                kick_result &= kick2->Set(false);
            }
        }
        else
        {
            motor_kick->WaitArrivedTargetPos(parameters.finishKickTrayPosition() - parameters.backDistance());
            kick_result &= kick1->Set(true);
        }
    }

    if(result&&need_check)
        result &= checkEntanceTray(false);
    result = result&&result_push&&kick_result&&result_return;
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

bool SensorTrayLoaderModule::moveToWorkPos(bool need_check)
{
    bool result = motor_tray->MoveToPosSync(parameters.trayWorkPosition());
    if(!result)
        AppendError(QString(u8"去工作位置失败"));
    if(result&&need_check)
        result &= checkSensorTray(true);
    qInfo(u8"去工作位置,返回值%d",result);
    return result;

}

bool SensorTrayLoaderModule::moveToEntranceClipNextPos()
{
    qInfo("moveToEntranceClipNextPos");
    if(motor_push->GetFeedbackPos() > parameters.pushMotorSafePosition())
    {
        qInfo("motor_push %f",motor_push->GetFeedbackPos());
        AppendError(u8"推出sendor盘的轴不在安全位置");
        return false;
    }

    if(entrance_clip->getChangeState())
    {
        states.setUseSpareEntanceClip(!states.useSpareEntanceClip());
        entrance_clip->resetClip();
    }

    bool result;
    if(states.useSpareEntanceClip())
       result = entrance_clip_push->Set(true);
    else
        result = entrance_clip_push->Set(false);
    //check clip
    result &= motor_stie->MoveToPosSync(entrance_clip->getNextPosition());
    if(result)
        entrance_clip->finishCurrentPosition();
    return result;
}

bool SensorTrayLoaderModule::moveToExitClipNextPos()
{
    if(exit_clip->getChangeState())
    {
        states.setUseSpareExitClip(!states.useSpareExitClip());
        exit_clip->resetClip();
    }

    bool result;
    if(states.useSpareExitClip())
       result = exit_clip_push->Set(true);
    else
        result = exit_clip_push->Set(false);
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
    if(entrance_tray_check_io->Value() == check_state)
        return true;
    QString logic_state = check_state?u8"有":u8"无";
    QString checked_state = check_state?u8"无":u8"有";
    AppendError(QString(u8"轨道入口处逻辑%1盘，但检测到%2盘！").arg(logic_state).arg(checked_state));
    qInfo(u8"轨道入口处逻辑%s盘，但检测到%s盘！",logic_state.toStdString().c_str(),checked_state.toStdString().c_str());
    return false;
}

bool SensorTrayLoaderModule::checkSensorTray(bool check_state)
{
    if(sensor_tray_check_io->Value() == check_state)
        return true;
    QString logic_state = check_state?u8"有":u8"无";
    QString checked_state = check_state?u8"无":u8"有";
    AppendError(QString(u8"sensor盘爪上逻辑%1盘，但检测到%2盘！").arg(logic_state).arg(checked_state));
    qInfo(u8"sensor盘爪上逻辑%s盘，但检测到%s盘！",logic_state.toStdString().c_str(),checked_state.toStdString().c_str());
    return false;
}

bool SensorTrayLoaderModule::checkKickTray(bool check_state)
{
    if(kick_tray_check_io->Value() == check_state)
        return true;
    QString logic_state = check_state?u8"有":u8"无";
    QString checked_state = check_state?u8"无":u8"有";
    AppendError(QString(u8"放取空的Sensor盘位置逻辑%1盘，但检测到%2盘！").arg(logic_state).arg(checked_state));
    qInfo(u8"放取空的Sensor盘位置逻辑%s盘，但检测到%s盘！",logic_state.toStdString().c_str(),checked_state.toStdString().c_str());
    return false;
}

bool SensorTrayLoaderModule::checkReadyTray(bool check_state)
{
    if(ready_tray_check_io->Value() == check_state)
        return true;
    QString logic_state = check_state?u8"有":u8"无";
    QString checked_state = check_state?u8"无":u8"有";
    AppendError(QString(u8"备用盘位置逻辑%1盘，但检测到%2盘！").arg(logic_state).arg(checked_state));
    qInfo(u8"备用盘位置逻辑%s盘，但检测到%s盘！",logic_state.toStdString().c_str(),checked_state.toStdString().c_str());
    return false;
}

bool SensorTrayLoaderModule::checkVacancyTray(bool check_state)
{
    if(vacancy_tray_check_io->Value() == check_state)
        return true;
    QString logic_state = check_state?u8"有":u8"无";
    QString checked_state = check_state?u8"无":u8"有";
    AppendError(QString(u8"成品盘爪上逻辑%1盘，但检测到%2盘!").arg(logic_state).arg(checked_state));
    qInfo(u8"成品盘爪上位置逻辑%s盘，但检测到%s盘!",logic_state.toStdString().c_str(),checked_state.toStdString().c_str());
    return false;
}

bool SensorTrayLoaderModule::checkExitTray(bool check_state)
{
    if(exit_tray_check_io->Value() == check_state)
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

