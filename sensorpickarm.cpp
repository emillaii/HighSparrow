#include "sensorpickarm.h"
#include <QElapsedTimer>

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
    this->parameters.setSpaVaccum1InIoName(picker1->vacuum->parameters.inIoName());
    this->parameters.setSpaVaccum2InIoName(picker2->vacuum->parameters.inIoName());
    this->parameters.setSpaVaccum1OutIoName(picker1->vacuum->parameters.outIoName());
    this->parameters.setSpaVaccum2OutIoName(picker2->vacuum->parameters.outIoName());
}

bool SensorPickArm::checkXYArrived(const double x, const double y)
{
    return motor_x->CheckArrivedTargetPos(x)&&motor_y->CheckArrivedTargetPos(y);
}

bool SensorPickArm::move_XY_Synic(QPointF position,bool check_arrived, bool check_softlanding, int timeout)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer;
    QString temp;
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    if(check_arrived&&checkXYArrived(position.x(),position.y()))return true;
    double dist_x = fabs(motor_x->GetFeedbackPos() - position.x());
    double dist_y = fabs(motor_y->GetFeedbackPos() - position.y());
    motor_x->MoveToPos(position.x());
    motor_y->MoveToPos(position.y());
    smallTimer.start();
    bool resut = motor_x->WaitArrivedTargetPos(position.x(),timeout);
    temp.append(" dist_x ").append(QString::number(dist_x))
        .append(" time ").append(QString::number(smallTimer.elapsed()));
    smallTimer.restart();
    resut &= motor_y->WaitArrivedTargetPos(position.y(),timeout);
    temp.append(" dist_y ").append(QString::number(dist_y))
        .append(" time ").append(QString::number(smallTimer.elapsed()));
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
    return resut;
}

bool SensorPickArm::checkXYT1Arrived(const double x, const double y, const double t)
{
    return motor_x->CheckArrivedTargetPos(x)&&motor_y->CheckArrivedTargetPos(y)&&picker1->motor_t->CheckArrivedTargetPos(t);
}

bool SensorPickArm::move_XYT1_Synic(const double x, const double y, const double t,const bool check_arrived, const bool check_softlanding, int timeout)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.restart();
    QString temp;
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    if(check_arrived&&checkXYT1Arrived(x,y,t))return true;
    double dist_x = fabs(motor_x->GetFeedbackPos() - x);
    double dist_y = fabs(motor_y->GetFeedbackPos() - y);
    double dist_t = fabs(picker1->motor_t->GetFeedbackPos() - t + 2);
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    smallTimer.restart();
    picker1->motor_t->MoveToPosSync(t - 2);
    temp.append(" dist_t ").append(QString::number(dist_t))
        .append(" move_t ").append(QString::number(smallTimer.elapsed()));
    picker1->motor_t->MoveToPos(t);
    smallTimer.restart();
    bool resut = motor_x->WaitArrivedTargetPos(x,parameters.processPositionError(),timeout);
    temp.append(" dist_x ").append(QString::number(dist_x))
        .append(" move_x ").append(QString::number(smallTimer.elapsed()));
    smallTimer.restart();
    resut &= motor_y->WaitArrivedTargetPos(y,parameters.processPositionError(),timeout);
    temp.append(" dist_y ").append(QString::number(dist_y))
        .append(" move_y ").append(QString::number(smallTimer.elapsed()));
    dist_t = fabs(picker1->motor_t->GetFeedbackPos() - t);
    smallTimer.restart();
    resut &= picker1->motor_t->WaitArrivedTargetPos(t,timeout);
    temp.append(" dist_t ").append(QString::number(dist_t))
        .append(" move_t ").append(QString::number(smallTimer.elapsed()));
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
    return resut;

}

bool SensorPickArm::move_XYT1_Pos(const double x, const double y, const double t,const bool check_arrived, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    if(check_arrived&&checkXYT1Arrived(x,y,t))return true;
    bool result = motor_x->MoveToPos(x);
    result &= motor_y->MoveToPos(y);
    result &=  picker1->motor_t->MoveToPosSync(t - 2);
    result &=  picker1->motor_t->MoveToPos(t);
    return result;
}

bool SensorPickArm::checkXYT2Arrived(const double x, const double y, const double t)
{
    return motor_x->CheckArrivedTargetPos(x)&&motor_y->CheckArrivedTargetPos(y)&&picker2->motor_t->CheckArrivedTargetPos(t);
}

bool SensorPickArm::move_XYT2_Synic(const double x, const double y, const double t,const bool check_arrived, const bool check_softlanding, int timeout)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    QString temp;
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    if(check_arrived&&checkXYT2Arrived(x,y,t))return true;
    double dist_x = fabs(motor_x->GetFeedbackPos() - x);
    double dist_y = fabs(motor_y->GetFeedbackPos() - y);
    double dist_t = fabs(picker2->motor_t->GetFeedbackPos() - (t-2));
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    smallTimer.restart();
    picker2->motor_t->MoveToPosSync(t - 2);
    temp.append(" dist_t ").append(QString::number(dist_t))
        .append(" move_t ").append(QString::number(smallTimer.elapsed()));
    dist_t = fabs(picker2->motor_t->GetFeedbackPos() - t);
    picker2->motor_t->MoveToPos(t);
    smallTimer.restart();
    bool resut = motor_x->WaitArrivedTargetPos(x,parameters.processPositionError(),timeout);
    temp.append(" dist_x ").append(QString::number(dist_x))
        .append(" move_x ").append(QString::number(smallTimer.elapsed()));
    smallTimer.restart();
    resut &= motor_y->WaitArrivedTargetPos(y,parameters.processPositionError(),timeout);
    temp.append(" dist_y ").append(QString::number(dist_y))
        .append(" move_y ").append(QString::number(smallTimer.elapsed()));
    smallTimer.restart();
    resut &= picker2->motor_t->WaitArrivedTargetPos(t,timeout);
    temp.append(" dist_t ").append(QString::number(dist_t))
        .append(" move_t ").append(QString::number(smallTimer.elapsed()));
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
    return resut;
}

bool SensorPickArm::checkXYT1T2Arrived(const double x, const double y, const double t1, const double t2)
{
    return motor_x->CheckArrivedTargetPos(x)&&motor_y->CheckArrivedTargetPos(y)&&picker1->motor_t->CheckArrivedTargetPos(t1)&&picker2->motor_t->CheckArrivedTargetPos(t2);
}

bool SensorPickArm::move_XYT1T2_Synic(const double x, const double y, const double t1, const double t2, const bool check_arrived, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    if(check_arrived&&checkXYT1T2Arrived(x,y,t1,t2))return true;
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    picker1->motor_t->MoveToPosSync(t1 - 2);
    picker2->motor_t->MoveToPosSync(t2 - 2);
    picker1->motor_t->MoveToPos(t1);
    picker2->motor_t->MoveToPos(t2);
    bool resut = motor_x->WaitArrivedTargetPos(x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y,timeout);
    resut &= picker1->motor_t->WaitArrivedTargetPos(t1,timeout);
    resut &= picker2->motor_t->WaitArrivedTargetPos(t2,timeout);
    return resut;
}

bool SensorPickArm::move_XYT2_Pos(const double x, const double y, const double t, const bool check_arrived, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    if(check_arrived&&checkXYT2Arrived(x,y,t))return true;
    bool result = motor_x->MoveToPos(x);
    result &= motor_y->MoveToPos(y);
    result &= picker2->motor_t->MoveToPosSync(t - 2);
    result &= picker2->motor_t->MoveToPos(t);
    return result;
}

bool SensorPickArm::move_XeYe_Z1_XY(double z, double escape_x, double escape_y, const bool check_softlanding, int timeout)
{
    QString temp;
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    double x = motor_x->GetOutpuPos();
    double y = motor_y->GetOutpuPos();
    double dist_x = fabs( motor_x->GetFeedbackPos() - (x - escape_x));
    double dist_y = fabs( motor_y->GetFeedbackPos() - (y - escape_y));
    motor_x->MoveToPos(x + escape_x);
    motor_y->MoveToPos(y + escape_y);
    smallTimer.restart();
    bool resut = motor_x->WaitArrivedTargetPos(x + escape_x,timeout);
    temp.append(" dist_x ").append(QString::number(dist_x,'g',8))
        .append(" escape_x_time ").append(QString::number(smallTimer.elapsed()));
    smallTimer.restart();
    resut &= motor_y->WaitArrivedTargetPos(y + escape_y,timeout);
    temp.append(" dist_y ").append(QString::number(dist_y,'g',8))
        .append(" escape_y_time ").append(QString::number(smallTimer.elapsed()));
    if(!resut)
    {
        AppendError(u8"move to escape pos fail");
        return false;
    }
    double dist_z = fabs( picker1->motor_z->GetFeedbackPos() - z);
    smallTimer.restart();
    resut &= picker1->motor_z->MoveToPosSync(z);
    temp.append(" dist_z ").append(QString::number(dist_z,'g',8))
        .append(" move_z_time ").append(QString::number(smallTimer.elapsed()));
    if(!resut)
    {
        AppendError(u8"move to escape z fail");
        return false;
    }
    QThread::msleep(100);
    temp.append(" delay ").append(QString::number(100));
    dist_x = fabs(motor_x->GetFeedbackPos() - x);
    dist_y = fabs(motor_y->GetFeedbackPos() - y);
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    smallTimer.restart();
    resut &= motor_x->WaitArrivedTargetPos(x,timeout);
    temp.append(" dist_x ").append(QString::number(dist_x,'g',8))
        .append(" move_x_time ").append(QString::number(smallTimer.elapsed()));
    smallTimer.restart();
    resut &= motor_y->WaitArrivedTargetPos(y,timeout);
    temp.append(" dist_y ").append(QString::number(dist_y,'g',8))
        .append(" move_y_time ").append(QString::number(smallTimer.elapsed()));
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
    return resut;
}

bool SensorPickArm::move_XeYe_Z2(double z, double escape_x, double escape_y, const bool check_softlanding, int timeout)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    QString temp;
    if(check_softlanding)
    {
        if(!picker1->motor_z->resetSoftLanding(timeout))return false;
        if(!picker2->motor_z->resetSoftLanding(timeout))return false;
    }
    double x = motor_x->GetOutpuPos();
    double y = motor_y->GetOutpuPos();
    double dist_x = fabs(motor_x->GetFeedbackPos() - (x + escape_x));
    double dist_y = fabs(motor_y->GetFeedbackPos() - (y + escape_y));
    motor_x->MoveToPos(x + escape_x);
    motor_y->MoveToPos(y + escape_y);
    smallTimer.restart();
    bool resut = motor_x->WaitArrivedTargetPos(x + escape_x,timeout);
    temp.append(" dist_x ").append(QString::number(dist_x))
        .append(" move_x ").append(QString::number(smallTimer.elapsed()));
    smallTimer.restart();
    resut &= motor_y->WaitArrivedTargetPos(y + escape_y,timeout);
    temp.append(" dist_y ").append(QString::number(dist_y))
        .append(" move_y ").append(QString::number(smallTimer.elapsed()));
    if(!resut)
    {
        AppendError(u8"move to escape pos fail");
        return false;
    }
    smallTimer.restart();
    double dist_z = fabs(picker2->motor_z->GetFeedbackPos() - z);
    resut &= picker2->motor_z->MoveToPosSync(z);
    temp.append(" dist_z ").append(QString::number(dist_z))
        .append(" move_z ").append(QString::number(smallTimer.elapsed()));
    if(!resut)
    {
        AppendError(u8"move to escape z fail");
        return false;
    }
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
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
    motor_x->MoveToPos(target_x);
    motor_y->MoveToPos(target_y);
    picker1->motor_t->MoveToPosSync(target_t - 2);
    picker1->motor_t->MoveToPos(target_t);
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
    bool result = motor_x->StepMove(step_x);
    result &= motor_y->StepMove(step_y);
    double target = picker1->motor_t->GetFeedbackPos() + step_t1;
    result &= picker1->motor_t->MoveToPosSync(target - 2);
    result &= picker1->motor_t->MoveToPos(target);
    return result;
}

bool SensorPickArm::wait_XYT1_Arrived(int timeout)
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
    motor_x->MoveToPos(target_x);
    motor_y->MoveToPos(target_y);
    picker2->motor_t->MoveToPosSync(target_t - 2);
    picker2->motor_t->MoveToPos(target_t);
    bool resut = motor_x->WaitArrivedTargetPos(target_x,timeout);
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
    result &= picker2->motor_t->StepMoveSync(step_t2 - 2);
    result &= picker2->motor_t->StepMove(step_t2);
    return result;
}

bool SensorPickArm::wait_XYT2_Arrived(int timeout)
{
    bool result = motor_x->WaitArrivedTargetPos(timeout);
    result &= motor_y->WaitArrivedTargetPos(timeout);
    result &= picker2->motor_t->WaitArrivedTargetPos(timeout);
    return result;
}

bool SensorPickArm::MoveZ1Synic(double target, int timeout)
{
    return picker1->motor_z->MoveToPosSync(target,-1,timeout);
}

bool SensorPickArm::Z1MeasureHeight(double speed, double force,int hold_time, int timeout)
{
    if(!picker1->motor_z->resetSoftLanding(timeout))return false;
    bool result = picker1->motor_z->SearchPosByForce(speed,force);
    if(hold_time > 0)
        QThread::msleep(hold_time);
    softlanding_position = picker1->motor_z->GetFeedbackPos();
    result &= picker1->motor_z->resetSoftLanding(timeout);
    return result;
}

bool SensorPickArm::Z1SearchByForce(double speed,double force,double limit,double margin,int timeout)
{
    return picker1->motor_z->SearchPosByForce(speed,force,limit,margin,timeout);
}

bool SensorPickArm::Z1SerchReturn(int timeout)
{
    return picker1->motor_z->resetSoftLanding(timeout);
}

bool SensorPickArm::MoveZ1ToSafeHeighSync()
{
    return picker1->motor_z->MoveToPosSync(parameters.motor1SafeHeight());
}

bool SensorPickArm::MoveZ2ToSafeHeighSync()
{
    return picker2->motor_z->MoveToPosSync(parameters.motor2SafeHeight());
}

double SensorPickArm::GetZ1MeasuredHeight(bool get_current)
{
    if(get_current)
        return picker1->motor_z->GetFeedbackPos();
    return softlanding_position;
}

bool SensorPickArm::MoveZ2Synic(double target, int out_time)
{
    return picker2->motor_z->MoveToPosSync(target,-1,out_time);
}

bool SensorPickArm::Z2MeasureHeight(double speed, double force,int hold_time, int out_time)
{
    if(!picker2->motor_z->resetSoftLanding(out_time))return false;
    bool result = picker2->motor_z->SearchPosByForce(speed,force);
    if(hold_time > 0)
    QThread::msleep(hold_time);
    softlanding_position = picker2->motor_z->GetFeedbackPos();
    result &= picker2->motor_z->resetSoftLanding(out_time);
    return result;

}

bool SensorPickArm::Z2SerchByForce(double speed,double force,double limit,double margin, int timeout)
{
    return picker2->motor_z->SearchPosByForce(speed,force,limit,margin,timeout);
}

bool SensorPickArm::Z2SerchReturn(int timeout)
{
    return picker2->motor_z->resetSoftLanding(timeout);
}

double SensorPickArm::GetZ2MeasuredHeight(bool get_current)
{
    if(get_current)
        return picker1->motor_z->GetFeedbackPos();
    return softlanding_position;
}
