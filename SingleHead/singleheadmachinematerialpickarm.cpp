#include "SingleHead/singleheadmachinematerialpickarm.h"
#include "SingleHead/singleheadmachinematerialloadermodule.h"


SingleHeadMachineMaterialPickArm::SingleHeadMachineMaterialPickArm(QString name):
    ErrorBase (name)
{

}

void SingleHeadMachineMaterialPickArm::Init(XtMotor *motor_x,
                                            XtMotor *motor_y,
                                            XtMotor *motor_th1,
                                            XtMotor *motor_th2,
                                            XtVcMotor *motor_vcm1,
                                            XtVcMotor *motor_vcm2,
                                            XtVcMotor *motor_vcmx,
                                            XtVacuum *vacuum_lens_suction,
                                            XtVacuum *vacuum_sensor_suction,
                                            XtVacuum *vacuum_lut,
                                            XtVacuum *vacuum_sut,
                                            XtCylinder *pogopin)
{
    this->motor_x = motor_x;
    this->motor_y = motor_y;
    this->motor_th1 = motor_th1;
    this->motor_th2 = motor_th2;
    this->motor_vcm1 = motor_vcm1;
    this->motor_vcm2 = motor_vcm2;
    this->motor_vcmx = motor_vcmx;
    this->vacuum_lens_suction = vacuum_lens_suction;
    this->vacuum_sensor_suction = vacuum_sensor_suction;
    this->vacuum_lut = vacuum_lut;
    this->vacuum_sut = vacuum_sut;
    this->pogopin = pogopin;
}



bool SingleHeadMachineMaterialPickArm::move_Xm_Origin()
{
    return motor_vcmx->MoveToPosSync(-4);
}

bool SingleHeadMachineMaterialPickArm::move_XY_Synic(const QPointF position, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!motor_vcm1->resetSoftLanding(timeout))return false;
        if(!motor_vcm2->resetSoftLanding(timeout))return false;
    }
    motor_x->MoveToPos(position.x());
    motor_y->MoveToPos(position.y());
    bool result = motor_x->WaitArrivedTargetPos(position.x(),timeout);
    result &= motor_y->WaitArrivedTargetPos(position.y(),timeout);
    return result;
}

bool SingleHeadMachineMaterialPickArm::move_XmY_Synic(const QPointF position, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!motor_vcm1->resetSoftLanding(timeout))return false;
        if(!motor_vcm2->resetSoftLanding(timeout))return false;
    }
    motor_vcmx->MoveToPos(position.x());
    motor_y->MoveToPos(position.y());
    bool result = motor_vcmx->WaitArrivedTargetPos(position.x(),timeout);
    result &= motor_y->WaitArrivedTargetPos(position.y(),timeout);
    return result;
}

bool SingleHeadMachineMaterialPickArm::stepMove_XmYT1_Synic(const double step_x, const double step_y, const double step_t1, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!motor_vcm1->resetSoftLanding(timeout))return false;
        if(!motor_vcm2->resetSoftLanding(timeout))return false;
    }
    double target_x = motor_vcmx->GetFeedbackPos() + step_x;
    double target_y = motor_y->GetFeedbackPos() + step_y;
    double target_t = motor_th1->GetFeedbackPos() + step_t1;
    motor_vcmx->StepMove(step_x);
    motor_y->StepMove(step_y);
    motor_th1->StepMove(step_t1);
    bool resut = motor_vcmx->WaitArrivedTargetPos(target_x);
    resut &= motor_y->WaitArrivedTargetPos(target_y,timeout);
    resut &= motor_th1->WaitArrivedTargetPos(target_t,timeout);
    return resut;
}

bool SingleHeadMachineMaterialPickArm::stepMove_XmYT2_Synic(const double step_x, const double step_y, const double step_t2, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!motor_vcm1->resetSoftLanding(timeout))return false;
        if(!motor_vcm2->resetSoftLanding(timeout))return false;
    }
    double target_x = motor_vcmx->GetFeedbackPos() + step_x;
    double target_y = motor_y->GetFeedbackPos() + step_y;
    double target_t = motor_th2->GetFeedbackPos() + step_t2;
    motor_vcmx->StepMove(step_x);
    motor_y->StepMove(step_y);
    motor_th2->StepMove(step_t2);
    bool resut = motor_vcmx->WaitArrivedTargetPos(target_x);
    resut &= motor_y->WaitArrivedTargetPos(target_y,timeout);
    resut &= motor_th2->WaitArrivedTargetPos(target_t,timeout);
    return resut;
}

bool SingleHeadMachineMaterialPickArm::stepMove_XY_Synic(double step_x, double step_y, int timeout)
{
    bool result;

    // pickarm PR use VCMX, but distance limited.
    double cur_x = motor_x->GetFeedbackPos();
    double cur_y = motor_y->GetFeedbackPos();
    double target_x = cur_x + step_x;
    double target_y = cur_y + step_y;
    motor_x->MoveToPos(target_x);
    motor_y->MoveToPos(target_y);
    result = motor_x->WaitArrivedTargetPos(target_x,timeout);
    result = motor_y->WaitArrivedTargetPos(target_y,timeout);

    return result;
}

bool SingleHeadMachineMaterialPickArm::ZSerchByForce(int picker, double speed, double force, bool check_softlanding, int timeout)
{
    XtVcMotor* motor_vcm = picker==0?motor_vcm1:motor_vcm2;
    if(check_softlanding)if(!motor_vcm->resetSoftLanding(timeout))return false;
    bool result = motor_vcm->SearchPosByForce(speed,force);
    QThread::msleep(5000);
    softlanding_position = motor_vcm->GetFeedbackPos();
    result &= motor_vcm->DoSoftLandingReturn();
    result &= motor_vcm->WaitSoftLandingDone(timeout);
    return result;
}

bool SingleHeadMachineMaterialPickArm::ZSerchByForce(int picker, double speed, double force, double limit, double margin, int finish_time, bool open_vacuum, bool need_return, int timeout)
{
    XtVcMotor* motor_vcm = picker==0?motor_vcm1:motor_vcm2;
    XtVacuum* vacuum = picker==0?vacuum_sensor_suction:vacuum_lens_suction;
    qInfo("SensorPickArm::ZSerchByForce timeout %d",timeout);
    bool result = motor_vcm->SearchPosByForce(speed,force,limit,margin,timeout);
    if(result)
        result &= vacuum->Set(open_vacuum,true,finish_time);
    softlanding_position = motor_vcm->GetFeedbackPos();
    if(need_return)
    {
        result &= motor_vcm->DoSoftLandingReturn();
        result &= motor_vcm->WaitSoftLandingDone(timeout);
    }
    return result;
}

bool SingleHeadMachineMaterialPickArm::ZSerchReturn(int picker, int timeout)
{
    XtVcMotor* motor_vcm = picker==0?motor_vcm1:motor_vcm2;
    return motor_vcm->resetSoftLanding(timeout);
}

bool SingleHeadMachineMaterialPickArm::move_XmY_Z1_XmY(double z, double escape_x, double escape_y, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!motor_vcm1->resetSoftLanding(timeout))return false;
        if(!motor_vcm2->resetSoftLanding(timeout))return false;
    }
    double x = motor_vcmx->GetOutpuPos();
    double y = motor_y->GetOutpuPos();
    motor_vcmx->MoveToPos(x + escape_x);
    motor_y->MoveToPos(y + escape_y);
    bool resut = motor_vcmx->WaitArrivedTargetPos(x + escape_x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y + escape_y,timeout);
    if(!resut)
    {
        AppendError(u8"move to escape pos fail");
        return false;
    }
    resut &= motor_vcm1->MoveToPosSync(z);
    if(!resut)
    {
        AppendError(u8"move to escape z fail");
        return false;
    }
    QThread::msleep(100);
    motor_vcmx->MoveToPos(x);
    motor_y->MoveToPos(y);
    resut &= motor_vcmx->WaitArrivedTargetPos(x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y,timeout);
    return resut;
}

bool SingleHeadMachineMaterialPickArm::move_XmY_Z2_XmY(double z, double escape_x, double escape_y, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!motor_vcm1->resetSoftLanding(timeout))return false;
        if(!motor_vcm2->resetSoftLanding(timeout))return false;
    }
    double x = motor_vcmx->GetOutpuPos();
    double y = motor_y->GetOutpuPos();
    motor_vcmx->MoveToPos(x + escape_x);
    motor_y->MoveToPos(y + escape_y);
    bool resut = motor_vcmx->WaitArrivedTargetPos(x + escape_x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y + escape_y,timeout);
    if(!resut)
    {
        AppendError(u8"move to escape pos fail");
        return false;
    }
    resut &= motor_vcm2->MoveToPosSync(z);
    if(!resut)
    {
        AppendError(u8"move to escape z fail");
        return false;
    }
    QThread::msleep(100);
    motor_vcmx->MoveToPos(x);
    motor_y->MoveToPos(y);
    resut &= motor_vcmx->WaitArrivedTargetPos(x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y,timeout);
    return resut;
}

double SingleHeadMachineMaterialPickArm::GetSoftladngPosition(bool get_current,int index)
{
    XtVcMotor* motor_vcm = index==0?motor_vcm1:motor_vcm2;
    if(get_current)
        return motor_vcm->GetFeedbackPos();
    return softlanding_position;
}

bool SingleHeadMachineMaterialPickArm::move_XYXmT1_Synic(QPointF position, double x, double t, bool check_softlanding, int timeout)
{
    qInfo("move to (%f,%f,%f,%f)",position.x(),position.y(),x,t);
    if(check_softlanding)if(!motor_vcm1->resetSoftLanding(timeout))return false;
    motor_x->MoveToPos(position.x());
    motor_y->MoveToPos(position.y());
    motor_vcmx->MoveToPos(x);
    motor_th1->MoveToPos(t);
    bool resut = motor_x->WaitArrivedTargetPos(position.x(),timeout);
    resut &= motor_y->WaitArrivedTargetPos(position.y(),timeout);
    resut &= motor_vcmx->WaitArrivedTargetPos(x,timeout);
    resut &= motor_th1->WaitArrivedTargetPos(t,timeout);
    return resut;
}
bool SingleHeadMachineMaterialPickArm::move_XYXmT2_Synic(QPointF position, double x, double t, bool check_softlanding, int timeout)
{
    qInfo("move to (%f,%f,%f,%f)",position.x(),position.y(),x,t);
    if(check_softlanding)if(!motor_vcm1->resetSoftLanding(timeout))return false;
    motor_x->MoveToPos(position.x());
    motor_y->MoveToPos(position.y());
    motor_vcmx->MoveToPos(x);
    motor_th2->MoveToPos(t);
    bool resut = motor_x->WaitArrivedTargetPos(position.x(),timeout);
    resut &= motor_y->WaitArrivedTargetPos(position.y(),timeout);
    resut &= motor_vcmx->WaitArrivedTargetPos(x,timeout);
    resut &= motor_th2->WaitArrivedTargetPos(t,timeout);
    return resut;
}

bool SingleHeadMachineMaterialPickArm::move_XmYT1_Synic(double x, double y, double t, bool check_softlanding, int timeout)
{
    qInfo("move to (%f,%f,%f)",x,y,t);
    if(check_softlanding)if(!motor_vcm1->resetSoftLanding(timeout))return false;
    motor_vcmx->MoveToPos(x);
    motor_y->MoveToPos(y);
    motor_th1->MoveToPos(t);
    bool resut = motor_vcmx->WaitArrivedTargetPos(x,timeout);
    resut &= motor_y->WaitArrivedTargetPos(y,timeout);
    resut &= motor_th1->WaitArrivedTargetPos(t,timeout);
    return resut;
}

bool SingleHeadMachineMaterialPickArm::stepMove_XmYT2_Synic(PrOffset position, bool check_softlanding, int timeout)
{
    if(check_softlanding)if(!motor_vcm2->resetSoftLanding(timeout))return false;
    double target_x = position.X + motor_vcmx->GetFeedbackPos();
    double target_y = position.Y +  motor_y->GetFeedbackPos();
    double target_t = position.Theta +  motor_th2->GetFeedbackPos();
    motor_vcmx->MoveToPos(target_x);
    motor_y->MoveToPos(target_y);
    motor_th2->MoveToPos(target_t);
    bool result = motor_vcmx->WaitArrivedTargetPos(target_x,timeout);
    result &= motor_y->WaitArrivedTargetPos(target_y,timeout);
    result &= motor_th2->WaitArrivedTargetPos(target_t,timeout);
    return result;
}

bool SingleHeadMachineMaterialPickArm::move_XYXm_Synic(QPointF position, double x, bool check_softlanding, int timeout)
{
    qInfo("move to (%f,%f,%f)",position.x(),position.y(),x);
    if(check_softlanding)if(!motor_vcm2->resetSoftLanding(timeout))return false;
    motor_x->MoveToPos(position.x());
    motor_y->MoveToPos(position.y());
    motor_vcmx->MoveToPos(x);
    bool resut = motor_x->WaitArrivedTargetPos(position.x(),timeout);
    resut &= motor_y->WaitArrivedTargetPos(position.y(),timeout);
    resut &= motor_vcmx->WaitArrivedTargetPos(x,timeout);
    return resut;
}



