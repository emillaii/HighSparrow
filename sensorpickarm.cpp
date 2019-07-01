#include "sensorpickarm.h"

SensorPickArm::SensorPickArm(QString name):ErrorBase (name)
{

}

void SensorPickArm::Init(XtMotor *motor_x, XtMotor *motor_y, MaterialPicker *picker1, MaterialPicker *picker2)
{
    this->motor_x = motor_x;
    parts.append(this->motor_x);
    this->motor_y = motor_y;
    parts.append(this->motor_y);
    this->picker1 = picker1;
    parts.append(this->picker1->motor_t);
    parts.append(this->picker1->motor_z);
    parts.append(this->picker1->vacuum);
    this->picker2 = picker2;
    parts.append(this->picker2->motor_t);
    parts.append(this->picker2->motor_z);
    parts.append(this->picker2->vacuum);
}

bool SensorPickArm::move_XY_Synic(QPointF position, bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    motor_x->MoveToPos(position.x());
    motor_y->MoveToPos(position.y());
    bool resut = motor_x->WaitArrivedTargetPos(position.x(),timeout);
    resut &= motor_y->WaitArrivedTargetPos(position.y(),timeout);
    return resut;
}

bool SensorPickArm::move_XYT1_Synic(const double x, const double y, const double t, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    picker1->motor_t->MoveToPos(t);
    bool resut = motor_x->WaitArrivedTargetPos(x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y,timeout);
    resut &= picker1->motor_t->WaitArrivedTargetPos(t,timeout);
    return resut;

}

bool SensorPickArm::checkXYT2Arrived(const double x, const double y, const double t)
{
    return motor_x->CheckArrivedTargetPos(x)&&motor_y->CheckArrivedTargetPos(y)&&picker2->motor_t->CheckArrivedTargetPos(t);
}

bool SensorPickArm::move_XYT2_Synic(const double x, const double y, const double t,const bool check_arrived, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    if(check_arrived&&checkXYT2Arrived(x,y,t))return true;
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    picker2->motor_t->MoveToPos(t);
    bool resut = motor_x->WaitArrivedTargetPos(x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y,timeout);
    resut &= picker2->motor_t->WaitArrivedTargetPos(t,timeout);
    return resut;
}

bool SensorPickArm::move_XeYe_Z1_XY(double z, double escape_x, double escape_y, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    double x = motor_x->GetOutpuPos();
    double y = motor_y->GetOutpuPos();
    motor_x->MoveToPos(x + escape_x);
    motor_y->MoveToPos(y + escape_y);
    bool resut = motor_x->WaitArrivedTargetPos(x + escape_x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y + escape_y,timeout);
    if(!resut)
    {
        AppendError(u8"move to escape pos fail");
        return false;
    }
    resut &= picker1->motor_z->MoveToPosSync(z);
    if(!resut)
    {
        AppendError(u8"move to escape z fail");
        return false;
    }
    QThread::msleep(100);
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    resut &= motor_x->WaitArrivedTargetPos(x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y,timeout);
    return resut;
}

bool SensorPickArm::move_XeYe_Z2(double z, double escape_x, double escape_y, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    double x = motor_x->GetOutpuPos();
    double y = motor_y->GetOutpuPos();
    motor_x->MoveToPos(x + escape_x);
    motor_y->MoveToPos(y + escape_y);
    bool resut = motor_x->WaitArrivedTargetPos(x + escape_x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y + escape_y,timeout);
    if(!resut)
    {
        AppendError(u8"move to escape pos fail");
        return false;
    }
    resut &= picker2->motor_z->MoveToPosSync(z);
    if(!resut)
    {
        AppendError(u8"move to escape z fail");
        return false;
    }
//    motor_x->MoveToPos(x);
//    motor_y->MoveToPos(y);
//    resut &= motor_x->WaitArrivedTargetPos(x,timeout);
//    resut &= motor_y->WaitArrivedTargetPos(y,timeout);
    return resut;
}

bool SensorPickArm::stepMove_XY_Synic(const double step_x, const double step_y, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    double target_x = motor_x->GetFeedbackPos() + step_x;
    double target_y = motor_y->GetFeedbackPos() + step_y;
    motor_x->StepMove(step_x);
    motor_y->StepMove(step_y);
    bool resut = motor_x->WaitArrivedTargetPos(target_x);
    resut &= motor_y->WaitArrivedTargetPos(target_y,timeout);
    return resut;
}

bool SensorPickArm::stepMove_XYT1_Synic(const double step_x, const double step_y, const double step_t1, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    double target_x = motor_x->GetFeedbackPos() + step_x;
    double target_y = motor_y->GetFeedbackPos() + step_y;
    double target_t = picker1->motor_t->GetFeedbackPos() + step_t1;
    motor_x->StepMove(step_x);
    motor_y->StepMove(step_y);
    picker1->motor_t->StepMove(step_t1);
    bool resut = motor_x->WaitArrivedTargetPos(target_x);
    resut &= motor_y->WaitArrivedTargetPos(target_y,timeout);
    resut &= picker1->motor_t->WaitArrivedTargetPos(target_t,timeout);
    return resut;
}

bool SensorPickArm::stepMove_XYT1_Pos(const double step_x, const double step_y, const double step_t1, const bool check_softlanding, int timeout)
{

    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    motor_x->StepMove(step_x);
    motor_y->StepMove(step_y);
    picker1->motor_t->StepMove(step_t1);
}

bool SensorPickArm::waitStepMove_XYT1(int timeout)
{
    bool resut = motor_x->WaitArrivedTargetPos(timeout);
    resut &= motor_y->WaitArrivedTargetPos(timeout);
    resut &= picker1->motor_t->WaitArrivedTargetPos(timeout);
    return resut;
}

bool SensorPickArm::stepMove_XYT2_Synic(const double step_x, const double step_y, const double step_t2, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    double target_x = motor_x->GetFeedbackPos() + step_x;
    double target_y = motor_y->GetFeedbackPos() + step_y;
    double target_t = picker2->motor_t->GetFeedbackPos() + step_t2;
    motor_x->StepMove(step_x);
    motor_y->StepMove(step_y);
    picker2->motor_t->StepMove(step_t2);
    bool resut = motor_x->WaitArrivedTargetPos(target_x);
    resut &= motor_y->WaitArrivedTargetPos(target_y,timeout);
    resut &= picker2->motor_t->WaitArrivedTargetPos(target_t,timeout);
    return resut;
}

bool SensorPickArm::stepMove_XYT2_Pos(const double step_x, const double step_y, const double step_t2, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    bool result = motor_x->StepMove(step_x);
    result &= motor_y->StepMove(step_y);
    result &= picker2->motor_t->StepMove(step_t2);
    return result;
}

bool SensorPickArm::waitStepMove_XYT2(int timeout)
{
    bool result = motor_x->WaitArrivedTargetPos(timeout);
    result &= motor_y->WaitArrivedTargetPos(timeout);
    result &= picker2->motor_t->WaitArrivedTargetPos(timeout);
    return result;
}

bool SensorPickArm::ZSerchByForce(double speed, double force, bool check_softlanding, int timeout)
{
    if(check_softlanding)if(!picker1->motor_z->resetSoftLanding(timeout))return false;
    bool result = picker1->motor_z->SearchPosByForce(speed,force);
    QThread::msleep(200);
    softlanding_position = picker1->motor_z->GetFeedbackPos();
    result &= picker1->motor_z->DoSoftLandingReturn();
    result &= picker1->motor_z->WaitSoftLandingDone(timeout);
    return result;
}

bool SensorPickArm::ZSerchByForce(double speed, double force, double limit, double margin, int finish_time, bool open_vacuum,bool need_return, int timeout)
{
    qInfo("SensorPickArm::ZSerchByForce timeout %d",timeout);
    bool result = picker1->motor_z->SearchPosByForce(speed,force,limit,margin,timeout);
    if(result)
        result &= picker1->vacuum->Set(open_vacuum);
    softlanding_position = picker1->motor_z->GetFeedbackPos();
    if(need_return)
    {
        result &= picker1->motor_z->DoSoftLandingReturn();
        result &= picker1->motor_z->WaitSoftLandingDone(timeout);
    }
    return result;
}

bool SensorPickArm::ZSerchReturn(int timeout)
{
    return picker1->motor_z->resetSoftLanding(timeout);
}

double SensorPickArm::GetSoftladngPosition(bool get_current)
{
    if(get_current)
        return picker1->motor_z->GetFeedbackPos();
    return softlanding_position;
}

bool SensorPickArm::ZSerchByForce2(double speed, double force, bool check_softlanding, int timeout)
{
    if(check_softlanding)if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    bool result = picker2->motor_z->SearchPosByForce(speed,force);
    QThread::msleep(200);
    softlanding_position = picker2->motor_z->GetFeedbackPos();
    result &= picker2->motor_z->DoSoftLandingReturn();
    result &= picker2->motor_z->WaitSoftLandingDone(timeout);
    return result;

}

bool SensorPickArm::ZSerchByForce2(double speed, double force, double limit, double margin, int finish_time, bool open_vacuum,bool need_return, int timeout)
{
    bool result = picker2->motor_z->SearchPosByForce(speed,force,limit,margin,timeout);
    if(result)
        result &= picker2->vacuum->Set(open_vacuum);
    softlanding_position = picker2->motor_z->GetFeedbackPos();
    if(need_return)
    {
        result &= picker2->motor_z->DoSoftLandingReturn();
        result &= picker2->motor_z->WaitSoftLandingDone(timeout);
    }
    return result;
}

bool SensorPickArm::ZSerchReturn2(int timeout)
{
    return picker2->motor_z->resetSoftLanding(timeout);
}

double SensorPickArm::GetSoftladngPosition2(bool get_current)
{
    if(get_current)
        return picker1->motor_z->GetFeedbackPos();
    return softlanding_position;
}
