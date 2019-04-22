#include "pick_arm_xxyp.h"

PickArmXXYP::PickArmXXYP(QString name):ErrorBase (name)
{

}

void PickArmXXYP::Init(XtMotor *motor_x_tray, XtMotor *motor_x, XtMotor *motor_y, MaterialPicker *picker)
{
    this->motor_x_tray = motor_x_tray;
    this->motor_x = motor_x;
    this->motor_y = motor_y;
    this->picker = picker;
}

bool PickArmXXYP::move_XtXY_Synic(QPointF position,double x,bool check_softlanding,int timeout)
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

bool PickArmXXYP::stepMove_XYTp_Synic(mPositionT position,bool check_softlanding,int timeout)
{
    if(check_softlanding)if(!picker->motor_z->resetSoftLanding(timeout))return false;
    double target_x = position.X + motor_x->GetFeedbackPos();
    double target_y = position.Y +  motor_x->GetFeedbackPos();
    double target_t = position.Theta +  motor_x->GetFeedbackPos();
    motor_x->StepMove(position.X);
    motor_y->StepMove(position.Y);
    picker->motor_t->StepMove(position.Theta);
    bool resut = motor_x->WaitArrivedTargetPos(target_x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(target_y,timeout);
    resut &= picker->motor_t->WaitArrivedTargetPos(target_t,timeout);
    return resut;
}

bool PickArmXXYP::Move_SZ_Sync(double z,bool check_softlanding, int timeout)
{
    if(check_softlanding)if(!picker->motor_z->resetSoftLanding(timeout))return false;
    picker->motor_z->MoveToPos(z);
    return   picker->motor_z->WaitArrivedTargetPos(z,timeout);
}

double PickArmXXYP::ZSerchByForce(double speed, double force,bool check_softlanding,int timeout)
{
    if(check_softlanding)if(!picker->motor_z->resetSoftLanding(timeout))return false;
    return  picker->motor_z->SearchPosByForce(speed,force);
}

bool PickArmXXYP::ZSerchByForce(double speed, double force, double limit, double margin,int finish_time,bool open_vacuum, int timeout)
{
    bool result = picker->motor_z->SearchPosByForce(speed,force,limit,margin,timeout);
    if(result)
        result &= picker->vacuum->Set(open_vacuum,true,finish_time);
    result &= picker->motor_z->DoSoftLandingReturn();
    return result;
}

QString PickArmXXYP::GetCurrentError()
{
//    AppendLineError(motor_x_tray->GetCurrentError());
//    AppendLineError(motor_x->GetCurrentError());
//    AppendLineError(motor_y->GetCurrentError());
//    AppendLineError(picker->motor_z->GetCurrentError());
//    AppendLineError(picker->motor_t->GetCurrentError());
//    AppendLineError(picker->vacuum->GetCurrentError());
    return ErrorBase::GetCurrentError();
}
