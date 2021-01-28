#include "material_carrier.h"
#include <QElapsedTimer>

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

bool MaterialCarrier::Move_SZ_SX_Y_X_Z_Sync(double x, double y, double z,bool check_autochthonous,bool check_softlanding,double check_distance, int timeout)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    QString temp;
    if (check_softlanding)
    {
        if (!motor_z->resetSoftLanding(timeout)) return false;
    }

    if(check_autochthonous&&CheckXYZArrived(x,y,z))
        return true;
    bool result;
    if(CheckXYDistanceBigger(x,y,check_distance))
    {
        double dist_z = fabs( motor_z->GetFeedbackPos() - parameters.SafetyZ());
        smallTimer.restart();
        result = motor_z->MoveToPosSync(parameters.SafetyZ());
        temp.append(" dist_z ").append(QString::number(dist_z))
            .append(" move_safety_z ").append(QString::number(smallTimer.elapsed()));
        if(!result) return false;
        if(fabs(y - motor_y->GetFeedbackPos()) > check_distance)
        {
            double dist_x = fabs( motor_x->GetFeedbackPos() - parameters.SafetyX());
            smallTimer.restart();
            result = motor_x->MoveToPosSync(parameters.SafetyX());
            temp.append(" dist_x ").append(QString::number(dist_x))
                .append(" move_safety_x ").append(QString::number(smallTimer.elapsed()));
            if(!result) return false;
        }
    }

    double dist_y = fabs( motor_y->GetFeedbackPos() - y);
    smallTimer.restart();
    result = motor_y->MoveToPosSync(y);
    temp.append(" dist_y ").append(QString::number(dist_y))
        .append(" move_y ").append(QString::number(smallTimer.elapsed()));
    if(!result) return false;
    double dist_x = fabs( motor_x->GetFeedbackPos() - x);
    smallTimer.restart();
    result = motor_x->MoveToPosSync(x);
    temp.append(" dist_x ").append(QString::number(dist_x))
        .append(" move_x ").append(QString::number(smallTimer.elapsed()));
    if(!result) return false;
    double dist_z = fabs( motor_z->GetFeedbackPos() - z);
    smallTimer.restart();
    result = motor_z->MoveToPosSync(z);
    temp.append(" dist_z ").append(QString::number(dist_z))
        .append(" move_z ").append(QString::number(smallTimer.elapsed()));
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qInfo(log.toStdString().c_str());
    return result;
}

bool MaterialCarrier::Move_SZ_SX_Y_X_Sync(double x, double y, double y_error, bool check_autochthonous, bool check_softlanding, double check_distance, int timeout)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    QString temp;
    if (check_softlanding)
    {
        if (!motor_z->resetSoftLanding(timeout)) return false;
    }

    if(check_autochthonous&&CheckXYZArrived(x,y,parameters.SafetyZ()))
        return true;
    bool result;
    if(CheckXYDistanceBigger(x,y,check_distance))
    {
        double dist_z = fabs(motor_z->GetFeedbackPos() - parameters.SafetyZ());
        smallTimer.start();
        result = motor_z->MoveToPosSync(parameters.SafetyZ(),0.1);
        temp.append(" dist_z ").append(QString::number(dist_z))
            .append(" move_z_safety ").append(QString::number(smallTimer.elapsed()));
        if(!result) return false;
        if(fabs(y - motor_y->GetFeedbackPos()) > check_distance)
        {
            smallTimer.restart();
            double dist_x = fabs(motor_x->GetFeedbackPos() - parameters.SafetyX());
            result = motor_x->MoveToPosSync(parameters.SafetyX());
            temp.append(" dist_x ").append(QString::number(dist_x))
                .append(" move_x_safety ").append(QString::number(smallTimer.elapsed()));
            if(!result) return false;
        }
    }
    QElapsedTimer t; t.start();
    double dist_y = fabs(motor_y->GetFeedbackPos() - y);
    smallTimer.restart();
    result = motor_y->MoveToPosSync(y,y_error);
    temp.append(" dist_y ").append(QString::number(dist_y))
        .append(" move_y ").append(QString::number(smallTimer.elapsed()));
    if(!result) return false;
    t.restart();
    double dist_x = fabs(motor_x->GetFeedbackPos() - x);
    smallTimer.restart();
    result = motor_x->MoveToPosSync(x);
    temp.append(" dist_x ").append(QString::number(dist_x))
        .append(" move_x ").append(QString::number(smallTimer.elapsed()));
    QString log = QString(" [Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                        .append(QString::number(timer.elapsed()))
                                        .append(temp);

    qWarning(log.toStdString().c_str());
    return result;
}

bool MaterialCarrier::Move_SZ_SX_YS_X_Z_Sync(double x, double y, double z, bool check_autochthonous, bool check_softlanding, double check_distance, int timeout)
{
    if (check_softlanding)
    {
        if (!motor_z->resetSoftLanding(timeout)) return false;
    }

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

bool MaterialCarrier::Move_SZ_SY_X_Y_Z_Sync(double x, double y, double z,bool check_autochthonous,bool check_softlanding, double check_distance, int timeout)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    QString temp;
    if (check_softlanding)
    {
        if (!motor_z->resetSoftLanding(timeout)) return false;
    }

    if(check_autochthonous&&CheckXYZArrived(x,y,z))
        return true;
    bool result;
    if(CheckXYDistanceBigger(x,y,check_distance))
    {
        double dist_z = fabs(motor_z->GetFeedbackPos() - parameters.SafetyZ());
        smallTimer.restart();
        result = motor_z->MoveToPosSync(parameters.SafetyZ());
        temp.append(" dist_z ").append(QString::number(dist_z))
            .append(" move_z ").append(QString::number(smallTimer.elapsed()));
        if(!result) return false;
        if(fabs(x - motor_x->GetFeedbackPos()) > check_distance)
        {
            smallTimer.restart();
            double dist_y = fabs(motor_y->GetFeedbackPos() - parameters.SafetyY());
            result = motor_y->MoveToPosSync(parameters.SafetyY());
            temp.append(" dist_y ").append(QString::number(dist_y))
                .append(" move_y ").append(QString::number(smallTimer.elapsed()));
            if(!result) return false;
        }
    }
    double dist_x = fabs(motor_x->GetFeedbackPos() - x);
    double dist_y = fabs(motor_y->GetFeedbackPos() - y);
    double dist_z = fabs(motor_z->GetFeedbackPos() - z);
    smallTimer.restart();
    result = motor_x->MoveToPosSync(x);
    temp.append(" dist_x ").append(QString::number(dist_x))
        .append(" move_x ").append(QString::number(smallTimer.elapsed()));
    if(!result) return false;
    smallTimer.restart();
    result = motor_y->MoveToPosSync(y);
    temp.append(" dist_y ").append(QString::number(dist_y))
        .append(" move_y ").append(QString::number(smallTimer.elapsed()));
    if(!result) return false;
    smallTimer.restart();
    result = motor_z->MoveToPosSync(z);
    temp.append(" dist_z ").append(QString::number(dist_z))
        .append(" move_z ").append(QString::number(smallTimer.elapsed()));
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
    return result;
}

bool MaterialCarrier::Move_SZ_SY_X_YS_Z_Sync(double x, double y, double z, bool check_autochthonous, bool check_softlanding, double check_distance, int timeout)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    QString temp;
    if (check_softlanding)
    {
        if (!motor_z->resetSoftLanding(timeout)) return false;
    }

    if(check_autochthonous&&CheckXYZArrived(x,y,z))
        return true;
    bool result;
    if(CheckXYDistanceBigger(x,y,check_distance))
    {
        double dist_z = fabs(motor_z->GetFeedbackPos() - parameters.SafetyZ());
        smallTimer.restart();
        result = motor_z->MoveToPosSync(parameters.SafetyZ());
        temp.append(" dist_z ").append(QString::number(dist_z))
            .append(" move_safety_z ").append(QString::number(smallTimer.elapsed()));
        if(!result) return false;
        if(fabs(x - motor_x->GetFeedbackPos()) > check_distance)
        {
            smallTimer.restart();
            double dist_y = fabs(motor_y->GetFeedbackPos() - parameters.SafetyY());
            result = motor_y->MoveToPosSync(parameters.SafetyY());
            temp.append(" dist_y ").append(QString::number(dist_y))
                .append(" move_safety_y ").append(QString::number(smallTimer.elapsed()));
            if(!result) return false;
        }
    }
    double dist_x = fabs(motor_x->GetFeedbackPos() - x);
    smallTimer.restart();
    result = motor_x->MoveToPosSync(x);
    temp.append(" dist_x ").append(QString::number(dist_x))
        .append(" move_x ").append(QString::number(smallTimer.elapsed()));
    if(!result) return false;
    double dist_y = fabs(motor_y->GetFeedbackPos() - y);
    smallTimer.restart();
    result = motor_y->MoveToPosSaftySync(y);
    temp.append(" dist_y ").append(QString::number(dist_y))
        .append(" move_y ").append(QString::number(smallTimer.elapsed()));
    if(!result) return false;
    smallTimer.restart();
    double dist_z = fabs(motor_z->GetFeedbackPos() - z);
    result = motor_z->MoveToPosSync(z);
    temp.append(" dist_z ").append(QString::number(dist_z))
        .append(" move_z ").append(QString::number(smallTimer.elapsed()));
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
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

bool MaterialCarrier::Move_X_Sync(double x, int timeout)
{
    motor_x->MoveToPos(x);
    return motor_x->WaitArrivedTargetPos(x, timeout);
}

void MaterialCarrier::Move_XY_ToPos(double x, double y)
{
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
}

bool MaterialCarrier::ZSerchByForce(const double speed,const double force,const double search_limit,const int vacuum_state,XtVacuum* excute_vacuum)
{
    if(nullptr != excute_vacuum)vacuum = excute_vacuum;
    bool result  = motor_z->SearchPosByForce(speed,force);
    if(!result)
    {
        motor_z->resetSoftLanding();
    }
    return result;
}

bool MaterialCarrier::ZSerchReturn()
{
    bool result = motor_z->resetSoftLanding();
//    bool result = motor_z->DoSoftLandingReturn();
//    result &= motor_z->WaitSoftLandingDone();
    return result;
}

mPoint3D MaterialCarrier::GetFeedBackPos()
{
    return mPoint3D(motor_x->GetFeedbackPos(),motor_y->GetFeedbackPos(),motor_z->GetFeedbackPos());
}

