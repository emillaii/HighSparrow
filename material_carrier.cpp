#include "material_carrier.h"

MaterialCarrier::MaterialCarrier():ErrorBase ()
{
}

void MaterialCarrier::Init(QString name, XtMotor *motor_x, XtMotor *motor_y, XtVcMotor *motor_z, XtVacuum *vacuum)
{
    this->motor_x = motor_x;
    parts.append(motor_x);
    this->motor_y = motor_y;
    parts.append(motor_y);
    this->motor_z = motor_z;
    parts.append(motor_z);
    this->vacuum = vacuum;
    parts.append(vacuum);
    setName(name);
}

bool MaterialCarrier::CheckXYZArrived(double x, double y, double z)
{
    return motor_x->CheckArrivedTargetPos(x)&&motor_y->CheckArrivedTargetPos(y)&&motor_z->CheckArrivedTargetPos(z);
}

bool MaterialCarrier::CheckXYDistanceBigger(double x, double y, double check_distance)
{
    return fabs(x - motor_x->GetFeedbackPos()) > check_distance || fabs(y - motor_y->GetFeedbackPos()) > check_distance;
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

bool MaterialCarrier::Move_SZ_SX_Y_X_Z_Sync(double x, double y, double z,bool check_autochthonous,double check_distance, int timeout)
{
    if(check_autochthonous&&CheckXYZArrived(x,y,z))
        return true;
    bool result;
    if(CheckXYDistanceBigger(x,y,check_distance))
    {
        result = motor_z->MoveToPosSync(parameters.SafetyZ());
        if(!result) return false;
        if(fabs(y - motor_y->GetFeedbackPos()) > check_distance)
        {
            result = motor_x->MoveToPosSync(parameters.SafetyX());
            if(!result) return false;
        }
    }
    result = motor_y->MoveToPosSync(y);
    if(!result) return false;
    result = motor_x->MoveToPosSync(x);
    if(!result) return false;
    result = motor_z->MoveToPosSync(z);
    //    QThread::msleep(300);
    return result;
}

bool MaterialCarrier::Move_SZ_SX_YS_X_Z_Sync(double x, double y, double z, bool check_autochthonous, double check_distance, int timeout)
{

    if(check_autochthonous&&CheckXYZArrived(x,y,z))
        return true;
    bool result;
    if(CheckXYDistanceBigger(x,y,check_distance))
    {
        result = motor_z->MoveToPosSync(parameters.SafetyZ());
        if(!result) return false;
        if(fabs(y - motor_y->GetFeedbackPos()) > check_distance)
        {
            result = motor_x->MoveToPosSync(parameters.SafetyX());
            if(!result) return false;
        }
    }
    result = motor_y->MoveToPosSaftySync(y);
    if(!result) return false;
    result = motor_x->MoveToPosSync(x);
    if(!result) return false;
    result = motor_z->MoveToPosSync(z);
    //    QThread::msleep(300);
    return result;
}

bool MaterialCarrier::Move_SZ_SY_X_Y_Z_Sync(double x, double y, double z,bool check_autochthonous,double check_distance, int timeout)
{
    if(check_autochthonous&&CheckXYZArrived(x,y,z))
        return true;
    bool result;
    if(CheckXYDistanceBigger(x,y,check_distance))
    {
        result = motor_z->MoveToPosSync(parameters.SafetyZ());
        if(!result) return false;
        if(fabs(x - motor_x->GetFeedbackPos()) > check_distance)
        {
            result = motor_y->MoveToPosSync(parameters.SafetyY());
            if(!result) return false;
        }
    }
    result = motor_x->MoveToPosSync(x);
    if(!result) return false;
    result = motor_y->MoveToPosSync(y);
    if(!result) return false;
    result = motor_z->MoveToPosSync(z);
    //    QThread::msleep(300);
    return result;
}

bool MaterialCarrier::Move_SZ_SY_X_YS_Z_Sync(double x, double y, double z, bool check_autochthonous, double check_distance, int timeout)
{
    if(check_autochthonous&&CheckXYZArrived(x,y,z))
        return true;
    bool result;
    if(CheckXYDistanceBigger(x,y,check_distance))
    {
        result = motor_z->MoveToPosSync(parameters.SafetyZ());
        if(!result) return false;
        if(fabs(x - motor_x->GetFeedbackPos()) > check_distance)
        {
            result = motor_y->MoveToPosSync(parameters.SafetyY());
            if(!result) return false;
        }
    }
    result = motor_x->MoveToPosSync(x);
    if(!result) return false;
    result = motor_y->MoveToPosSaftySync(y);
    if(!result) return false;
    result = motor_z->MoveToPosSync(z);
    //    QThread::msleep(300);
    return result;
}

bool MaterialCarrier::Move_SZ_XY_ToPos(double x, double y, int timeout)
{
    bool result;
    result = motor_z->MoveToPosSync(parameters.SafetyZ());
    if(!result) return false;
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    return true;
}

bool MaterialCarrier::Wait_XY_ToPos(double x, double y, int timeout)
{
    bool result;
    result = motor_x->WaitArrivedTargetPos(x,timeout);
    result &= motor_y->WaitArrivedTargetPos(y,timeout);
    return result;
}

bool MaterialCarrier::StepMove_XY_Sync(double step_x, double step_y, int timeout)
{
    double cur_x = motor_x->GetFeedbackPos();
    double cur_y = motor_y->GetFeedbackPos();
    double target_x = cur_x + step_x;
    double target_y = cur_y + step_y;
    bool result = motor_x->MoveToPos(target_x);
    result &= motor_y->MoveToPos(target_y);
    result &= Wait_XY_ToPos(target_x,target_y,timeout);
    return result;
}

bool MaterialCarrier::StepMove_SZ_XY_Sync(double step_x, double step_y, int timeout)
{
    if(!motor_z->MoveToPosSync(parameters.SafetyZ()))
        return false;
    return StepMove_XY_Sync(step_x,step_y);
}

bool MaterialCarrier::StepMove_Z(double step_z, int timeout)
{
    double cur_z = motor_z->GetFeedbackPos();
    double target_z = cur_z + step_z;
    motor_z->MoveToPos(target_z);
    return motor_z->WaitArrivedTargetPos(target_z, timeout);
}

bool MaterialCarrier::Move_Z_Sync(double z, int timeout)
{
    motor_z->MoveToPos(z);
    return motor_z->WaitArrivedTargetPos(z, timeout);
}

void MaterialCarrier::Move_XY_ToPos(double x, double y)
{
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
}

bool MaterialCarrier::ZSerchByForce(const double speed,const double force,const double search_limit,const int vacuum_state,XtVacuum* excute_vacuum)
{
    qInfo("ZSerchByForce Start");
    if(nullptr != excute_vacuum)vacuum = excute_vacuum;
    qInfo("CP 1");
    bool result  = motor_z->SearchPosByForce(speed,force);
    qInfo("CP 2");
    if(!result)
    {
        motor_z->resetSoftLanding();
        qInfo("CP 3");
        motor_z->RestoreForce();
        qInfo("CP 4");
    }
    //    if(result && vacuum_state > -1&&vacuum != nullptr)
    //        result &= vacuum->Set(vacuum_state > 0);
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

mPoint3D MaterialCarrier::GetFeedBackPos()
{
    return mPoint3D(motor_x->GetFeedbackPos(),motor_y->GetFeedbackPos(),motor_z->GetFeedbackPos());
}

