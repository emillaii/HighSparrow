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

bool SensorPickArm::stepMove_XYT1_Synic(const PrOffset offset, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }

    motor_x->StepMove(offset.X);
    motor_y->StepMove(offset.Y);
    picker1->motor_t->StepMove(offset.Theta);
    bool resut = motor_x->WaitArrivedTargetPos(offset.X,timeout);
    resut &= motor_x->WaitArrivedTargetPos(offset.X,timeout);
    resut &= picker1->motor_t->WaitArrivedTargetPos(offset.Theta,timeout);
    return resut;
}

bool SensorPickArm::stepMove_XYT2_Synic(const PrOffset offset, const bool check_softlanding, int timeout)
{
    return true;
}

bool SensorPickArm::move_XYT_Synic(const PrOffset offset, const bool check_softlanding, int timeout)
{
    return true;
}
