#include "sensortrayloadermodule.h"

SensorTrayLoaderModule::SensorTrayLoaderModule():ThreadWorkerBase ("SensorTrayLoaderModule")
{

}

void SensorTrayLoaderModule::Init(XtMotor *motor_tray, XtMotor *motor_kick, XtMotor *motor_stie, XtMotor *motor_stoe, XtMotor *motor_push, XtCylinder *kick1, XtCylinder *kick2, XtCylinder *hold_tray, XtCylinder *hold_vacancy, XtCylinder *entrance_clip_push, XtCylinder *exit_clip_push, XtCylinder *gripper, SensorClip *entrance_clip, SensorClip *exit_clip)
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
}

void SensorTrayLoaderModule::startWork(bool reset_logic, int run_mode)
{
    qInfo("SensorLoader start reset:%d run_mode :%d in %d",reset_logic,run_mode,QThread::currentThreadId());
    if(reset_logic)resetLogic();
    if(run_mode == RunMode::Normal)run(true);
    else if(run_mode == RunMode::NoMaterial)run(false);
}

void SensorTrayLoaderModule::stopWork(bool wait_finish)
{
    qInfo("LensLoader stop");
//    if(wait_finish)
//        finish_stop = true;
//    else
        is_run = false;
}

void SensorTrayLoaderModule::performHandlingOperation(int cmd)
{

}

void SensorTrayLoaderModule::receiveChangeTray()
{
    qInfo("receiveChangeTray");
    if(states.changingTray())
        return;
    states.setNeedChangeTray(true);
//    if(states.hasVacancyTray())
    states.setNeedChangeVacancyTray(true);
    states.setChangingTray(true);
}

void SensorTrayLoaderModule::run(bool has_material)
{
    bool is_run = true;
    while (is_run)
    {
        QThread::msleep(100);
        //送入空盤
        if(states.needChangeTray()&&(!states.hasVacancyTray())&&(!states.getedVacancyTray()))
        {
            if((!moveToGetUpSensorTray())&&has_material)
            {
                AppendError(u8"去拿空盘位置失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
            }
            if((!moveToGetTray())&&has_material)
            {
                AppendError(u8"去拿空盘失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
            }
            if((!moveToDownsensorTray())&&has_material)
            {
                AppendError(u8"去放空盘失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                else
                {
                    states.setGetedVacancyTray(true);
                    states.setEntranceClipReay(false);
                }
            }
            else
            {
                states.setGetedVacancyTray(true);
                states.setEntranceClipReay(false);
            }
        }
        //拿起空盘，拖出第一个料盘
        if(states.needChangeTray()&&(!states.hasVacancyTray())&&states.getedVacancyTray())
        {
            if((!moveToGetUpSensorTray())&&has_material)
            {
                AppendError(u8"去拿第一个料盘位置失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
            }
            if((!moveToGetTray())&&has_material)
            {
                AppendError(u8"去拿第一个料盘失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
            }
            if((!moveToStartKick())&&has_material)
            {
                AppendError(u8"！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                else
                {
                    states.setGetedVacancyTray(true);
                    states.setEntranceClipReay(false);
                }
            }
            else
            {
                states.setGetedVacancyTray(true);
                states.setEntranceClipReay(false);
            }
        }



        //去Kick位置
        //去放工作盘
        if(states.needChangeTray()&&states.hasWorkTray()&&(!states.hasKickTray()))
        {
            if((!moveToStartKick())&&has_material)
            {
                AppendError(u8"去工作位置失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                else
                {
                    states.setHasWorkTray(false);
                    states.setHasKickTray(true);
                }
            }
            else
            {
                states.setHasWorkTray(false);
                states.setHasKickTray(true);
            }
        }
        //去空盘位置
        //去顶盘
        if(states.needChangeTray()&&states.needChangeVacancyTray()&&states.hasKickTray())
        {
            if((!moveToChangeVacancyTrayAndUpSensorTray())&&has_material)
            {
                AppendError(u8"换空盘并顶盘失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                else
                {
                    states.setNeedChangeVacancyTray(false);
                }
            }
            else
            {
                states.setNeedChangeVacancyTray(false);
            }
        }
        //夹盘
        if(states.needChangeTray()&&(!states.hasWorkTray())&&(!states.hasGetedTray())&&states.entranceClipReady())
        {
            if((!moveToGetTrayAndKickOutTray())&&has_material)
            {
                AppendError(u8"取盘失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                else
                {
                    states.setHasGetedTray(true);
                }
            }
            else {
                states.setHasGetedTray(true);
            }
        }
        //推出Kick盘(需要返回)
        // 去放盘
        if(states.needChangeTray()&&states.hasKickTray()&&(!states.needChangeVacancyTray())&&states.exitClipReay())
        {
            if((!moveToBackKickAndPutSensorTray())&&has_material)
            {
                AppendError(u8"推出Kick盘失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                else
                {
                    states.setHasKickTray(false);
                    states.setExitClipReady(false);
                    states.setEntranceClipReay(false);
                }
            }
            else
            {
                states.setHasKickTray(false);
                states.setExitClipReady(false);
                states.setEntranceClipReay(false);
            }
        }

        //去工作位置
        if(states.needChangeTray()&&(!states.hasWorkTray())&&states.hasGetedTray()&&(!states.hasKickTray()))
        {
            if(!moveToWorkPos())
            {
                AppendError(u8"去工作位置失败！");
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            states.setNeedChangeTray(false);
            states.setHasWorkTray(true);
            emit sendChangeTrayFinish();
            states.setChangingTray(false);
        }
        //进盘弹夹到位
        if(!states.entranceClipReady())
        {
            if((!moveToEntranceClipNextPos())&&has_material)
            {
                AppendError(u8"去工作位置失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                else
                {
                    states.setEntranceClipReay(true);
                }
            }
            else
            {
                states.setEntranceClipReay(true);
            }
        }
        //出盘弹夹到位
        if(!states.exitClipReay())
        {
            if((!moveToExitClipNextPos())&&has_material)
            {
                AppendError(u8"去工作位置失败！");
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                else
                {
                    states.setExitClipReady(true);
                }
            }
            else
            {
                states.setExitClipReady(true);
            }
        }
        //todo提示换盘
    }
}

void SensorTrayLoaderModule::resetLogic()
{
    states.setHasWorkTray(true);
}

bool SensorTrayLoaderModule:: moveToStartKick()
{
    qInfo("moveToStartKick");
    bool kick_result = kick1->Set(false);
    kick_result &= kick2->Set(false);
    //kick_result &= hold_vacancy->Set(true);
    hold_vacancy->Set(true);
    if(kick_result)
        motor_kick->MoveToPos(parameters.startKickTrayPosition());

    bool result = motor_tray->MoveToPosSync(parameters.downTrayPosition());
    if(result)
    {
        result &= hold_tray->Set(true);
        if(result)
            motor_tray->MoveToPos(parameters.getTrayPosition());
    }
    if(kick_result)
        kick_result &= motor_kick->WaitArrivedTargetPos(parameters.startKickTrayPosition());
    if(kick_result)
    {
        kick_result &= kick1->Set(true);
        kick_result &= kick2->Set(true);
    }
    return kick_result&&result;
}

bool SensorTrayLoaderModule::moveToChangeVacancyTrayAndUpSensorTray()
{
    qInfo("moveToChangeVacancyTrayAndUpSensorTray");
    bool kick_result = kick1->Set(true);
    kick_result &= kick2->Set(true);
//    kick_result &= hold_vacancy->Set(true);
     hold_vacancy->Set(true);
    if(kick_result)
        motor_kick->MoveToPos(parameters.vacancyTrayPosition());
    bool result = motor_tray->MoveToPosSync(parameters.getTrayPosition());
    if(result)
    {
       result &= hold_tray->Set(false);
       result &= gripper->Set(false);
    }
    if(motor_kick->WaitArrivedTargetPos(parameters.vacancyTrayPosition()))
    {
        kick_result &= kick1->Set(false);
        if(kick_result)
            kick_result &= hold_vacancy->Set(false);
    }
    return result&&kick_result;
}

bool SensorTrayLoaderModule::moveToGetUpSensorTray()
{
    bool result = motor_tray->MoveToPosSync(parameters.getTrayPosition());
    if(result)
    {
       result &= hold_tray->Set(false);
       result &= gripper->Set(false);
    }
    return result;
}

bool SensorTrayLoaderModule::moveToGetTray()
{
    bool result = motor_push->MoveToPosSync(parameters.pushoutPosition());
    if(result)
        result &= gripper->Set(true,false);
    result &= motor_push->MoveToPosSync(0);
    if(result)
        result &= gripper->Wait(true);
    return result;
}

bool SensorTrayLoaderModule::moveToDownsensorTray()
{
    bool result = motor_push->MoveToPosSync(parameters.pushoutPosition());
    if(result)
    {
        result &= gripper->Set(false);
        QThread::sleep(300);
    }
    if(result)
        result &= motor_push->MoveToPosSync(parameters.pushoutPosition()+ parameters.backDistance());
    if(result)
        result &= hold_tray->Set(true);
    return result;
}

bool SensorTrayLoaderModule::moveToGetTrayAndKickOutTray()
{
    qInfo("moveToGetTray");
    bool kick_result = kick1->Set(false);
    if(kick_result)
        motor_kick->MoveToPos(parameters.finishKickTrayPosition());
    bool result = motor_push->MoveToPosSync(parameters.pushoutPosition());
    if(result)
        result &= gripper->Set(true,false);
    result &= motor_push->MoveToPosSync(0);
    if(result)
        result &= gripper->Wait(true);
    if(kick_result)
        kick_result &= motor_kick->WaitArrivedTargetPos(parameters.finishKickTrayPosition());
    if(kick_result)
        motor_kick->MoveToPos(parameters.finishKickTrayPosition() - parameters.backDistance());
    return result&&kick_result;
}

bool SensorTrayLoaderModule::moveToBackKickAndPutSensorTray()
{
    qInfo("moveToPushKickTrayAndPutSensorTray");
    motor_tray->MoveToPos(parameters.putTrayPosition());
    bool kick_result = motor_kick->MoveToPosSync(parameters.finishKickTrayPosition() - parameters.backDistance());
    bool result = motor_tray->WaitArrivedTargetPos(parameters.putTrayPosition());
    if(result)
       result &= gripper->Set(false);
    return result&&kick_result;
}

bool SensorTrayLoaderModule::moveToWorkPos()
{
    qInfo("moveToWorkPos");
    return motor_tray->MoveToPosSync(parameters.trayWorkPosition());
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
    bool result;
    if(states.useSpareEntanceClip())
       result = entrance_clip_push->Set(true);
    else
        result = entrance_clip_push->Set(false);
    result &= motor_stie->MoveToPosSync(entrance_clip->getNextPosition());
    if(result)
        entrance_clip->finishCurrentPosition();
    return result;
}

bool SensorTrayLoaderModule::moveToExitClipNextPos()
{
    qInfo("moveToExitClipNextPos");
    bool result;
    if(states.useSpareExitClip())
       result = exit_clip_push->Set(true);
    else
        result = exit_clip_push->Set(false);
    result &= motor_stoe->MoveToPosSync(exit_clip->getNextPosition());
    if(result)
        exit_clip->finishCurrentPosition();
    return result;
}

