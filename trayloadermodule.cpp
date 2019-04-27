#include "trayloadermodule.h"



TrayLoaderModule::TrayLoaderModule(QString name):ThreadWorkerBase(name)
{

}

void TrayLoaderModule::Init(XtMotor *_motor_clip_in,
                            XtMotor *_motor_in,
                            XtMotor *_motor_work,
                            XtMotor *_motor_out,
                            XtMotor *_motor_clip_out,
                            XtCylinder* _cylinder_clip,
                            XtCylinder* _cylinder_ltk1,
                            XtCylinder* _cylinder_ltk2,
                            XtCylinder* _cylinder_tray)
{
    this->motor_clip_in = _motor_clip_in;
    this->motor_in = _motor_in;
    this->motor_work = _motor_work;
    this->motor_out = _motor_out;
    this->motor_clip_out = _motor_clip_out;
    this->cylinder_clip = _cylinder_clip;
    this->cylinder_ltk1 = _cylinder_ltk1;
    this->cylinder_ltk2 = _cylinder_ltk2;
    this->cylinder_tray = _cylinder_tray;
}

void TrayLoaderModule::ResetLogic()
{

}

void TrayLoaderModule::performHandling(int cmd, int &finished_type)
{
    emit sendHandlingOperation(cmd,finished_type);
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
    motor_clip_in->MoveToPos(pos);
    bool result = motor_clip_in->WaitArrivedTargetPos(pos);
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
