#include "trayloadermodule.h"

#include <QDebug>
#include <QObject>

TrayLoaderModule::TrayLoaderModule(QString name):ThreadWorkerBase(name)
{
    connect(this,SIGNAL(dragIn()),this,SLOT(onDragIn()));
    connect(this,SIGNAL(dragOut()),this,SLOT(onDragOut()));
    connect(this,SIGNAL(relayToLTLX()),this,SLOT(onRelayToLTLX()));
    connect(this,SIGNAL(relayToLTKX2()),this,SLOT(onRelayToLTLX()));
    connect(this,SIGNAL(layDown()),this,SLOT(onLayDown()));
}

void TrayLoaderModule::Init(XtMotor *_motor_clip_in,
                            XtMotor *_motor_in,
                            XtMotor *_motor_work,
                            XtMotor *_motor_out,
                            XtMotor *_motor_clip_out,
                            XtCylinder* _cylinder_clip,
                            XtCylinder* _cylinder_ltk1,
                            XtCylinder* _cylinder_ltk2,
                            XtCylinder* _cylinder_tray, TrayClip *trayClipIn, TrayClip *trayClipOut)
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
    this->tray_clip = trayClipIn;
    this->tray_clip_out = trayClipOut;
}

void TrayLoaderModule::ResetLogic()
{

}

void TrayLoaderModule::performHandling(int cmd)
{
    emit sendHandlingOperation(cmd);
}

void TrayLoaderModule::run(bool has_tray)
{

}

bool TrayLoaderModule::moveMotorClip()
{
    if(moveToNextTrayPos()&&ejectTray())
        return true;
    else {
        return false;
    }

}

bool TrayLoaderModule::moveToNextTrayPos()
{
    double pos = tray_clip->getCurrentPosition();
    motor_clip_in->MoveToPos(pos);
    bool result = motor_clip_in->WaitArrivedTargetPos(pos);
    if(result){
        if(tray_clip->standards_parameters.currentIndex()+1<tray_clip->standards_parameters.columnCount())
            tray_clip->standards_parameters.setCurrentIndex(tray_clip->standards_parameters.currentIndex()+1);
    }
    return result;
}

bool TrayLoaderModule::ejectTray()
{
    int res = cylinder_clip->Set(true);
    if(!cylinder_clip->Set(false)){
        qDebug()<<"retraction failed";
        //warning
    }
    return res;
}

bool TrayLoaderModule::moveMotorIn()
{
    if(motorInPress()&&moveToLtkx1GetPos()&&motorInRealease()){
        return true;
    }else{
        return false;
    }
}

bool TrayLoaderModule::motorInPress()
{
    int res = cylinder_ltk1->Set(true);
    return res;
}

bool TrayLoaderModule::moveToLtkx1GetPos()
{
    motor_in->MoveToPos(parameters.ltkx1PressPos());
    bool result = motor_in->WaitArrivedTargetPos(parameters.ltkx1PressPos());
    return result;
}

bool TrayLoaderModule::moveToLtkx1SetPos()
{
    motor_in->MoveToPos(parameters.ltkx1ReleasePos());
    bool result = motor_in->WaitArrivedTargetPos(parameters.ltkx1ReleasePos());
    return result;
}

bool TrayLoaderModule::motorInRealease()
{
    int res = cylinder_ltk1->Value();
    if(res)res = cylinder_ltk1->Set(false);
    return res;
}

bool TrayLoaderModule::moveMotorWork()
{
    if(motorWorkPress()&&moveToLtlSetPos()&&motorWorkRelease())
        return true;
    else {
        return false;
    }
}

bool TrayLoaderModule::moveToLtlGetPos()
{
    motor_work->MoveToPos(parameters.ltlPressPos());
    bool result = motor_work->WaitArrivedTargetPos(parameters.ltlPressPos());
    return result;
}

bool TrayLoaderModule::motorWorkPress()
{
    int res = cylinder_tray->Set(true);
    return res;
}

bool TrayLoaderModule::moveToLtlSetPos()
{
    motor_work->MoveToPos(parameters.ltlReleasePos());
    bool result = motor_work->WaitArrivedTargetPos(parameters.ltlReleasePos());
    return result;
}

bool TrayLoaderModule::motorWorkRelease()
{
    int res = cylinder_tray->Value();
    if(res)res = cylinder_tray->Set(false);
    return res;
}

bool TrayLoaderModule::moveMotorOut()
{
    return 1;
}

bool TrayLoaderModule::moveToLtkx2GetPos()
{
    motor_out->MoveToPos(parameters.ltkx2PressPos());
    bool result = motor_out->WaitArrivedTargetPos(parameters.ltkx2PressPos());
    return result;
}

bool TrayLoaderModule::motorOutPress()
{
    return 1;
}

bool TrayLoaderModule::moveToLtkx2SetPos()
{
    motor_out->MoveToPos(parameters.ltkx2ReleasePos());
    bool result = motor_out->WaitArrivedTargetPos(parameters.ltkx2ReleasePos());
    return result;
}

bool TrayLoaderModule::motorOutRelease()
{
    return 1;
}

bool TrayLoaderModule::moveToNextEmptyPos()
{
    double pos = tray_clip_out->getCurrentPosition();
    motor_clip_out->MoveToPos(pos);
    bool result = motor_clip_out->WaitArrivedTargetPos(pos);
    if(result){
        if(tray_clip_out->standards_parameters.currentIndex()+1<tray_clip_out->standards_parameters.columnCount())
            tray_clip_out->standards_parameters.setCurrentIndex(tray_clip_out->standards_parameters.currentIndex()+1);
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

void TrayLoaderModule::performHandlingOperation(int cmd)
{

}

void TrayLoaderModule::onDragIn()
{
    if(cylinder_ltk1->Value())cylinder_ltk1->Set(false);
    if(!moveToLtkx1GetPos())
       return;
    if(!motorInPress())
        return;
    if(!moveToLtkx1SetPos())
        return;
    emit relayToLTLX();
}

void TrayLoaderModule::onDragOut()
{
    if(!moveToLtkx2SetPos())
        return;
    if(!motorOutRelease())
        return;
    emit layDown();
}

void TrayLoaderModule::onRelayToLTLX()
{
    if(!moveToLtlGetPos())
        return;
    if(!motorWorkPress())
        return;
    if(!motorInRealease())
        return;
    //emit some signals;
}

void TrayLoaderModule::onRelayToLTKX2()
{
    if(!moveToLtlSetPos())
        return;
    if(!moveToLtkx2GetPos())
        return;
    if(!motorOutPress())
        return;
    if(!motorWorkRelease())
        return;
    emit dragOut();
}

void TrayLoaderModule::onLayDown()
{
    moveToNextEmptyPos();
    //emit some signals
}
