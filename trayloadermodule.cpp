#include "trayloadermodule.h"

#include <QDebug>
#include <QObject>

TrayLoaderModule::TrayLoaderModule(QString name):ThreadWorkerBase(name)
{
    connect(this,SIGNAL(testTrayUsed()),this,SLOT(onTestTrayUsed()),Qt::QueuedConnection);
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
                            XtGeneralInput*_tray_exit_input)
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
}

void TrayLoaderModule::ResetLogic()
{

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

void TrayLoaderModule::run(bool has_tray)
{
    if(is_run){
        startUp();
    }

}

bool TrayLoaderModule::moveToNextTrayPos()
{
    if(!tray_entry_input->Value()){
        //qDebug()<<"入口处检测到有盘";
        AppendError(u8"入口处检测到有盘,请把盘拿走");
        sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
        waitMessageReturn(is_run);
        return false;
    }
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
    int res = cylinder_clip->Set(true,true);
    Sleep(2000);
    //cylinder_clip->Set(true);
    if(!cylinder_clip->Set(false,true)){
        qDebug()<<"retraction failed";
    }
    return res;
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

bool TrayLoaderModule::moveToLtkx1RelayPos()
{
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
    cylinder_ltk1->Set(false);
    return 1;
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
    int res = cylinder_tray->Set(1);
    return true;
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
    int res = cylinder_ltk2->Value();
    if(res){
        res = cylinder_ltk2->Set(false);
        return res;
    }
    return 1;
}

bool TrayLoaderModule::moveToNextEmptyPos()
{
    if(!tray_exit_input->Value()){
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
