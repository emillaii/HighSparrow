#include "lenspickarm.h"

LensPickArm::LensPickArm(QString name):ErrorBase (name)
{

}

void LensPickArm::Init(XtMotor *motor_x_tray, XtMotor *motor_x, XtMotor *motor_y, MaterialPicker *picker)
{
    this->motor_x_tray = motor_x_tray;
    parts.append(motor_x_tray);
    this->motor_x = motor_x;
    parts.append(motor_x);
    this->motor_y = motor_y;
    parts.append(motor_y);
    this->picker = picker;
    parts.append(picker->motor_z);
    parts.append(picker->motor_t);
    parts.append(picker->vacuum);
}

bool LensPickArm::move_XtXY_Synic(QPointF position,double x,bool check_softlanding,int timeout)
{
    if(check_softlanding)if(!picker->motor_z->resetSoftLanding(timeout))return false;
    motor_x_tray->MoveToPos(position.x());
    motor_y->MoveToPos(position.y());
    motor_x->MoveToPos(x);
    bool resut = motor_x_tray->WaitArrivedTargetPos(position.x(),timeout);
    resut &= motor_y->WaitArrivedTargetPos(position.y(),timeout);
    resut &= motor_x->WaitArrivedTargetPos(x,timeout);
    return resut;
}

bool LensPickArm::move_XtXYT_Synic(QPointF position, double x, double t, bool check_softlanding, int timeout)
{
    if(check_softlanding)if(!picker->motor_z->resetSoftLanding(timeout))return false;
    motor_x_tray->MoveToPos(position.x());
    motor_y->MoveToPos(position.y());
    motor_x->MoveToPos(x);
    picker->motor_t->MoveToPosSync(t - 2);
    picker->motor_t->MoveToPos(t);
    bool resut = motor_x_tray->WaitArrivedTargetPos(position.x(),timeout);
    resut &= motor_y->WaitArrivedTargetPos(position.y(),timeout);
    resut &= motor_x->WaitArrivedTargetPos(x,timeout);
    resut &= picker->motor_t->WaitArrivedTargetPos(t,timeout);
    return resut;
}


bool LensPickArm::move_XY_Synic(double x, double y, bool check_softlanding, int timeout)
{
    if(check_softlanding)if(!picker->motor_z->resetSoftLanding(timeout))return false;
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    bool resut = motor_x->WaitArrivedTargetPos(x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y,timeout);
    return resut;
}

bool LensPickArm::checkXYTArrived(double x, double y, double t)
{
    return motor_x->CheckArrivedTargetPos(x)&&motor_y->CheckArrivedTargetPos(y)&&picker->motor_t->CheckArrivedTargetPos(t);
}

bool LensPickArm::move_XYT_Synic(double x, double y, double t,bool check_arrived, bool check_softlanding, int timeout)
{
    if(check_softlanding)if(!picker->motor_z->resetSoftLanding(timeout))return false;
    if(check_arrived&&checkXYTArrived(x,y,t))return true;
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    picker->motor_t->MoveToPosSync(t - 2);
    picker->motor_t->MoveToPos(t);
    bool resut = motor_x->WaitArrivedTargetPos(x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y,timeout);
    resut &= picker->motor_t->WaitArrivedTargetPos(t,timeout);
    return resut;
}

bool LensPickArm::stepMove_XYTp_Synic(PrOffset position,bool check_softlanding,int timeout)
{
    if(check_softlanding)if(!picker->motor_z->resetSoftLanding(timeout))return false;
    double target_x = position.X + motor_x->GetFeedbackPos();
    double target_y = position.Y +  motor_y->GetFeedbackPos();
    double target_t = position.Theta +  picker->motor_t->GetFeedbackPos();
    motor_x->MoveToPos(target_x);
    motor_y->MoveToPos(target_y);
    picker->motor_t->MoveToPosSync(target_t - 2);
    picker->motor_t->MoveToPos(target_t);
    bool result = motor_x->WaitArrivedTargetPos(target_x,timeout);
    result &= motor_y->WaitArrivedTargetPos(target_y,timeout);
    result &= picker->motor_t->WaitArrivedTargetPos(target_t,timeout);
    return result;
}

bool LensPickArm::stepMove_XYTp_Pos(PrOffset position, bool check_softlanding, int timeout)
{
    if(check_softlanding)if(!picker->motor_z->resetSoftLanding(timeout))return false;
    double target_x = position.X + motor_x->GetFeedbackPos();
    double target_y = position.Y +  motor_y->GetFeedbackPos();
    double target_t = position.Theta +  picker->motor_t->GetFeedbackPos();
    qInfo("target_x: %f target_y: %f target_t: %f",target_x,target_y,target_t);
    bool result = motor_x->MoveToPos(target_x);
    result &= motor_y->MoveToPos(target_y);
    result &= picker->motor_t->MoveToPosSync(target_t - 2);
    result &= picker->motor_t->MoveToPos(target_t);
    return result;
}

bool LensPickArm::waitStepMove_XYTp(int timeout)
{
    bool result = motor_x->WaitArrivedTargetPos(timeout);
    result &= motor_y->WaitArrivedTargetPos(timeout);
    result &= picker->motor_t->WaitArrivedTargetPos(timeout);
    return result;
}

bool LensPickArm::stepMove_T_Syncic(double t, int timeout)
{
     double target_t = t +  picker->motor_t->GetFeedbackPos();
     picker->motor_t->StepMove(t);
     bool result = picker->motor_t->WaitArrivedTargetPos(target_t,timeout);
     return result;
}

bool LensPickArm::Move_SZ_Sync(double z,bool check_softlanding, int timeout)
{
    if(check_softlanding)if(!picker->motor_z->resetSoftLanding(timeout))return false;
    picker->motor_z->MoveToPos(z);
    return   picker->motor_z->WaitArrivedTargetPos(z,timeout);
}

bool LensPickArm::ZSerchByForce(double speed, double force,bool check_softlanding,int timeout)
{
    if(check_softlanding)if(!picker->motor_z->resetSoftLanding(timeout))return false;
    bool result = picker->motor_z->SearchPosByForce(speed,force);
    QThread::msleep(200);
    softlanding_position = picker->motor_z->GetFeedbackPos();
    result &= picker->motor_z->resetSoftLanding(timeout);
    return result;
}

bool LensPickArm::ZSerchByForce(double speed, double force, double limit, double margin,int finish_time,bool open_vacuum, bool need_z_return, int timeout)
{
    bool result = picker->motor_z->SearchPosByForce(speed,force,limit,margin, timeout);
    if(result)
        picker->vacuum->Set(open_vacuum);
    else {
        qInfo("SearchPosByForce fail");
    }
    softlanding_position = picker->motor_z->GetFeedbackPos();
    if (need_z_return)
    {
        result &= picker->motor_z->resetSoftLanding(timeout);
    }
    return result;
}

bool LensPickArm::ZSerchReturn(int timeout)
{
    return picker->motor_z->resetSoftLanding(timeout);
}

double LensPickArm::GetSoftladngPosition(bool get_current)
{
    if(get_current)
        return picker->motor_z->GetFeedbackPos();
    return softlanding_position;
}

bool LensPickArm::pickarmVaccum(bool isOn)
{
    return picker->vacuum->Set(isOn);
}

QString LensPickArm::GetCurrentError()
{
//    AppendLineError(motor_x_tray->GetCurrentError());
//    AppendLineError(motor_x->GetCurrentError());
//    AppendLineError(motor_y->GetCurrentError());
//    AppendLineError(picker->motor_z->GetCurrentError());
//    AppendLineError(picker->motor_t->GetCurrentError());
//    AppendLineError(picker->vacuum->GetCurrentError());
    return ErrorBase::GetCurrentError();
}
