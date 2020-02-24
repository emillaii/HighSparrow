#include "SingleHead/singleheadmachinematerialpickarm.h"
#include "SingleHead/singleheadmachinematerialloadermodule.h"


SingleHeadMachineMaterialPickArm::SingleHeadMachineMaterialPickArm(QString name):
    ErrorBase (name)
{

}

void SingleHeadMachineMaterialPickArm::Init(XtMotor *motor_x,
                                            XtMotor *motor_y,
                                            XtMotor *motor_z,
                                            XtMotor *motor_th1,
                                            XtMotor *motor_th2,
                                            XtVcMotor *motor_vcm1,
                                            XtVcMotor *motor_vcm2,
                                            XtVcMotor *motor_vcmx,
                                            XtVacuum *vacuum_lens_suction,
                                            XtVacuum *vacuum_sensor_suction,
                                            XtVacuum *vacuum_lut,
                                            XtVacuum *vacuum_sut,
                                            XtCylinder *pogopin
                                            )
{
    this->motor_x = motor_x;
    this->motor_y = motor_y;
    this->motor_z = motor_z;
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
    return motor_vcmx->MoveToPosSync(parameters.pickArmOriginX());
}

bool SingleHeadMachineMaterialPickArm::move_Z_Synic(const double position, int timeout)
{
    motor_z->MoveToPos(position);
    bool result = motor_z->WaitArrivedTargetPos(position, timeout);
    return result;
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

bool SingleHeadMachineMaterialPickArm::move_XYXm_Sync(PickArmPos pos, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!motor_vcm1->resetSoftLanding(timeout))return false;
        if(!motor_vcm2->resetSoftLanding(timeout))return false;
    }
    motor_x->MoveToPos(pos.TL_X);
    motor_y->MoveToPos(pos.PA_Y);
    motor_vcmx->MoveToPos(pos.PA_X);
    return motor_x->WaitArrivedTargetPos(pos.TL_X,timeout)&
           motor_y->WaitArrivedTargetPos(pos.PA_Y,timeout) &
           motor_vcmx->WaitArrivedTargetPos(pos.PA_X,timeout);
}

bool SingleHeadMachineMaterialPickArm::move_XmY_Synic(const QPointF position, const bool check_softlanding, int timeout)
{
    if(check_softlanding)
    {
        if(!motor_vcm1->resetSoftLanding(timeout))return false;
        if(!motor_vcm2->resetSoftLanding(timeout))return false;
    }
//    motor_vcmx->MoveToPos(position.x());
    motor_y->MoveToPos(position.y());
    bool result = false;
//    bool result = motor_vcmx->WaitArrivedTargetPos(position.x(),timeout);
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
//    double target_x = motor_vcmx->GetFeedbackPos() + step_x;
    double target_y = motor_y->GetFeedbackPos() + step_y;
    double target_t = motor_th1->GetFeedbackPos() + step_t1;
    motor_vcmx->StepMove(step_x);
    motor_y->StepMove(step_y);
    motor_th1->StepMove(step_t1);
//    bool resut = motor_vcmx->WaitArrivedTargetPos(target_x);
      bool resut = false;
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
//    double target_x = motor_vcmx->GetFeedbackPos() + step_x;
    double target_y = motor_y->GetFeedbackPos() + step_y;
    double target_t = motor_th2->GetFeedbackPos() + step_t2;
    motor_vcmx->StepMove(step_x);
    motor_y->StepMove(step_y);
    motor_th2->StepMove(step_t2);
//    bool resut = motor_vcmx->WaitArrivedTargetPos(target_x);
      bool resut = false;
    resut &= motor_y->WaitArrivedTargetPos(target_y,timeout);
    resut &= motor_th2->WaitArrivedTargetPos(target_t,timeout);
    return resut;
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
    XtVacuum* vacuum = picker==0?vacuum_lens_suction:vacuum_sensor_suction;
    qInfo("SensorPickArm::ZSerchByForce timeout %d, force: %f limit: f",timeout);
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

double SingleHeadMachineMaterialPickArm::GetSoftladngPosition(bool get_current,int index)
{
    XtVcMotor* motor_vcm = index==0?motor_vcm1:motor_vcm2;
    if(get_current)
        return motor_vcm->GetFeedbackPos();
    return softlanding_position;
}



