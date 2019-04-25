#include "trayloadermodule.h"



void TrayLoaderModule::Init(XtMotor *motor_clip, XtMotor *motor_in, XtMotor *motor_work, XtMotor *motor_out)
{
    this->motor_clip = motor_clip;
    this->motor_in = motor_in;
    this->motor_work = motor_work;
    this->motor_out = motor_out;
}

void TrayLoaderModule::ResetLogic()
{

}

void TrayLoaderModule::run(bool has_tray)
{

}

bool TrayLoaderModule::moveMotorClip()
{
    if(moveToCurrentPos()&&ejectTray())
        return true;
    else {
        return false;
    }

}

bool TrayLoaderModule::moveToCurrentPos()
{
    double pos = tray_clip->getCurrentPosition();
    motor_clip->MoveToPos(pos);
    bool result = motor_clip->WaitArrivedTargetPos(pos);
    return result;
}

bool TrayLoaderModule::ejectTray()
{
    return 1;
}

bool TrayLoaderModule::moveMotorIn()
{
    if(motorInPress()&&moveToPosition1()&&motorInRealease()){
        return true;
    }else{
        return false;
    }
}

bool TrayLoaderModule::motorInPress()
{
    return 1;
}

bool TrayLoaderModule::moveToPosition1()
{
    motor_in->MoveToPos(parameters.position1());
    bool result = motor_in->WaitArrivedTargetPos(parameters.position1());
    return result;
}

bool TrayLoaderModule::motorInRealease()
{
    return 1;
}

bool TrayLoaderModule::moveMotorWork()
{
    if(motorWorkPress()&&moveToPosition2()&&motorWorkRelease())
        return true;
    else {
        return false;
    }
}

bool TrayLoaderModule::motorWorkPress()
{
    return 1;
}

bool TrayLoaderModule::moveToPosition2()
{
    motor_work->MoveToPos(parameters.position2());
    bool result = motor_work->WaitArrivedTargetPos(parameters.position2());
    return result;
}

bool TrayLoaderModule::motorWorkRelease()
{
    return 1;
}

bool TrayLoaderModule::moveMotorOut()
{
    return 1;
}

bool TrayLoaderModule::motorOutPress()
{
    return 1;
}

bool TrayLoaderModule::moveToPosition3()
{
    motor_out->MoveToPos(parameters.position3());
    bool result = motor_out->WaitArrivedTargetPos(parameters.position3());
    return result;
}

bool TrayLoaderModule::motorOutRelease()
{
    return 1;
}

void TrayLoaderModule::startWork(bool reset_logic, int run_mode)
{
    if(reset_logic)ResetLogic();
    if(run_mode&RunMode::Normal)run(true);
    else if(run_mode&RunMode::NoMaterial)run(false);
}

void TrayLoaderModule::stopWork(bool wait_finish)
{
    is_run = false;
}

void TrayLoaderModule::performHandlingOperation(int cmd, int &finished_type)
{

}
