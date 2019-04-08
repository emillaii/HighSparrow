#include "material_carrier.h"

MaterialCarrier::MaterialCarrier(QString name,XtMotor* motor_x,XtMotor* motor_y,XtVcMotor* motor_z,XtVacuum* vacuum)
{
//    parameters.setDisplayName(name);
    this->motor_x = motor_x;
    this->motor_y = motor_y;
    this->motor_z = motor_z;
    this->vacuum = vacuum;
}

bool MaterialCarrier::Move_SZ_XY_Z_Sync(double x, double y, double z, int timeout)
{
    bool result;
    result = motor_z->MoveToPosSync(parameters.SafetyZ());
    if(!result) return false;
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    result = motor_x->WaitArrivedTargetPos(x,timeout);
    result &= motor_y->WaitArrivedTargetPos(y,timeout);
    if(!result) return false;
    result = motor_z->MoveToPosSync(z);
    return result;
}

bool MaterialCarrier::Move_SZ_SX_Y_X_Z_Sync(double x, double y, double z, int timeout)
{
    bool result;
    result = motor_z->MoveToPosSync(parameters.SafetyZ());
    if(!result) return false;
    result = motor_x->MoveToPosSync(parameters.SafetyX());
    if(!result) return false;
    result = motor_y->MoveToPosSync(y);
    if(!result) return false;
    result = motor_x->MoveToPosSync(x);
    if(!result) return false;
    result = motor_z->MoveToPosSync(z);
    return result;
}

bool MaterialCarrier::Move_SZ_SY_X_Y_Z_Sync(double x, double y, double z, int timeout)
{

    bool result;
    result = motor_z->MoveToPosSync(parameters.SafetyZ());
    if(!result) return false;
    result = motor_y->MoveToPosSync(parameters.SafetyY());
    if(!result) return false;
    result = motor_x->MoveToPosSync(x);
    if(!result) return false;
    result = motor_y->MoveToPosSync(y);
    if(!result) return false;
    result = motor_z->MoveToPosSync(z);
    return result;
}

bool MaterialCarrier::Move_SZ_XY_ToPos(double x, double y,bool use_offset, int timeout)
{
    bool result;
    result = motor_z->MoveToPosSync(parameters.SafetyZ());
    if(!result) return false;
    if(use_offset)
    {
        motor_x->MoveToPos(x + parameters.CameraOffsetX());
        motor_y->MoveToPos(y + parameters.CameraOffsetY());
    }
    else
    {
        motor_x->MoveToPos(x);
        motor_y->MoveToPos(y);
    }
    return true;
}

bool MaterialCarrier::Wait_XY_ToPos(double x, double y,bool use_offset, int timeout)
{
    bool result;
    if(use_offset)
    {
        result = motor_x->WaitArrivedTargetPos(x + parameters.CameraOffsetX(),timeout);
        result &= motor_y->WaitArrivedTargetPos(y + parameters.CameraOffsetY(),timeout);
    }
    else
    {
        result = motor_x->WaitArrivedTargetPos(x,timeout);
        result &= motor_y->WaitArrivedTargetPos(y,timeout);
    }
    return result;
}

bool MaterialCarrier::StepMove_XY_Sync(double step_x, double step_y, int timeout)
{
    double cur_x = motor_x->GetFeedbackPos();
    double cur_y = motor_y->GetFeedbackPos();
    double target_x = cur_x + step_x;
    double target_y = cur_y + step_y;
    motor_x->MoveToPos(target_x);
    motor_y->MoveToPos(target_y);
    return Wait_XY_ToPos(target_x,target_y,timeout);
}

void MaterialCarrier::Move_XY_ToPos(double x, double y)
{
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
}

bool MaterialCarrier::Move_Z_Sync(double z, int timeout)
{
    return  motor_z->MoveToPosSync(z);
}

bool MaterialCarrier::ZSerchByForce(double &result_pos, double force, double search_limit, int vacuum_state,XtVacuum* excute_vacuum)
{
    if(nullptr != excute_vacuum)vacuum = excute_vacuum;
    bool result  = motor_z->SearchPosByForce(result_pos,force,search_limit);
    if(result && vacuum_state > -1)
        result &= vacuum->Set(vacuum_state > 0);
//    result &= motor_z->DoSoftLandingReturn();
//    result &= motor_z->WaitSoftLandingDone();
    return result;
}

bool MaterialCarrier::ZSerchReturn()
{
    bool result = motor_z->DoSoftLandingReturn();
    result &= motor_z->WaitSoftLandingDone();
    return result;
}

bool MaterialCarrier::Move_Vision_Sync()
{
    return  Move_SZ_XY_Z_Sync(parameters.VisionX(),parameters.VisionY(),parameters.VisionZ());
}

