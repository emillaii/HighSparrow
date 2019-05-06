#include "sensorpickarm.h"

SensorPickArm::SensorPickArm(QString name):ErrorBase (name)
{

}

void SensorPickArm::Init(XtMotor *motor_x, XtMotor *motor_y, MaterialPicker *picker1, MaterialPicker *picker2)
{
    this->motor_x = motor_x;
    this->motor_y = motor_y;
    this->picker1 = picker1;
    this->picker2 = picker2;
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
    bool result = picker1->motor_z->SearchPosByForce(speed,force,limit,margin,timeout);
    if(result)
        result &= picker1->vacuum->Set(open_vacuum,true,finish_time);
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
        result &= picker2->vacuum->Set(open_vacuum,true,finish_time);
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
