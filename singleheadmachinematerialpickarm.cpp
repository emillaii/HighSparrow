#include "singleheadmachinematerialpickarm.h"
#include "singleheadmachinematerialloadermodule.h"

bool PickArmPos::isEquilPos(PickArmPos target_pos, double error)
{
    bool lable_x = abs(x - target_pos.x) < error;
    bool lable_y = abs(y - target_pos.y) < error;
    bool lable_z = abs(z - v - (target_pos.z - target_pos.v)) < error;
    bool lable_th = abs(th - target_pos.th) < error;
    return  lable_x|lable_y|lable_z|lable_th;
}

bool PickArmPos::isAbovePos(PickArmPos target_pos, double error)
{
    bool lable_x = abs(x - target_pos.x) < error;
    bool lable_y = abs(y - target_pos.y) < error;
    bool lable_z = z - v > target_pos.z - target_pos.v - error ;
    bool lable_th = abs(th - target_pos.th) < error;
    if(!lable_x)
        qInfo("pa_x pos check fail .curent pos: %f,target: pos:%f",x,target_pos.x);
    if(!lable_y)
        qInfo("pa_y pos check fail .curent pos: %f,target: pos %f",y,target_pos.y);
    if(!lable_z)
    {
        qInfo("pa_z pos check fail .curent pos: %f - %f,target pos: %f - &f",z,v,target_pos.z,target_pos.v);
    }
    if(!lable_th)
        qInfo("pa_th pos check fail .curent pos: %f,target: pos %f",th,target_pos.th);
    return  lable_x|lable_y|lable_z|lable_th;
}

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

bool SingleHeadMachineMaterialPickArm::CheckZV1V2AboveSafeHeight(double z_height, double error, int times)
{
    do
    {
        double current_z = motor_z->GetFeedbackPos();
        double current_v1 = motor_vcm1->GetFeedbackPos();
        double current_v2 = motor_vcm2->GetFeedbackPos();
        bool lable_z = current_z > z_height - error;
        bool lable_v1 = current_v1 < (parent->parameters.vcm1BaseHeight() - parent->parameters.vcmSafeHeight()) + error;
        bool lable_v2 = current_v2 < (parent->parameters.vcm2BaseHeight() - parent->parameters.vcmSafeHeight()) + error;

        if(!lable_z)
            qInfo("check z pos fail.current pos :%f target pos : above %f",current_z ,z_height);
        if(!lable_v1)
            qInfo("check v1 pos fail.current pos :%f target pos : %f",current_v1 ,parent->parameters.vcm1BaseHeight() - parent->parameters.vcmSafeHeight());
        if(!lable_v2)
            qInfo("check v2 pos fail.current pos :%f target pos : %f",current_v2 ,parent->parameters.vcm2BaseHeight() - parent->parameters.vcmSafeHeight());
        if(lable_z&lable_v1&lable_v2)
        {
            return true;
        }
        else {
            QThread::msleep(10);
            times --;
        }
    }while(times>0);
    return false;
}

bool SingleHeadMachineMaterialPickArm::ZV1V2ToSafeHeight(double z_height)
{
    motor_z->MoveToPos(z_height);
    motor_vcm1->MoveToPos(parent->parameters.vcm1BaseHeight() - parent->parameters.vcmSafeHeight());
    motor_vcm2->MoveToPos(parent->parameters.vcm2BaseHeight() - parent->parameters.vcmSafeHeight());
    bool res = motor_z->WaitMoveStop();
    res &= motor_vcm1->WaitMoveStop();
    res &= motor_vcm2->WaitMoveStop();
    return res;
}

bool SingleHeadMachineMaterialPickArm::vcm1SoftLanding(double force, double target_pos)
{
    //    qInfo("Vcm1 parametr:vcm1_svel %f vcm1_sacc %f vcm1_pickforce %f vcm1_margin %f",vcm1_svel,vcm1_sacc,vcm1_pickforce,vcm1_margin);
        double current_pos = motor_vcm1->GetOutpuPos();
        motor_vcm1->SetSoftLanding(parent->parameters.vcm1Svel(), parent->parameters.vcm1Sacc(), force,current_pos,target_pos,parent->parameters.vcm1Margin());
        bool res = motor_vcm1->DoSoftLanding();
        res &= motor_vcm1->WaitSoftLandingDone();
        return res;
}

bool SingleHeadMachineMaterialPickArm::vcm2SoftLanding(double force, double target_pos)
{
    //    qInfo("Vcm2 parametr:vcm2_svel %f vcm2_sacc %f vcm2_pickforce %f vcm2_margin %f",vcm2_svel,vcm2_sacc,vcm2_pickforce,vcm2_margin);
        double current_pos = motor_vcm2->GetOutpuPos();
        motor_vcm2->SetSoftLanding(parent->parameters.vcm2Svel(), parent->parameters.vcm2Sacc(), force,current_pos,target_pos,parent->parameters.vcm2Margin());
        bool res = motor_vcm2->DoSoftLanding();
        res &= motor_vcm2->WaitSoftLandingDone();
        return res;
}

bool SingleHeadMachineMaterialPickArm::vcm1SoftLandingReturn()
{
    bool res = motor_vcm1->DoSoftLandingReturn();
    res &= motor_vcm1->WaitSoftLandingDone();
    return  res;
}

bool SingleHeadMachineMaterialPickArm::vcm2SoftLandingReturn()
{
    bool res = motor_vcm2->DoSoftLandingReturn();
    res &= motor_vcm2->WaitSoftLandingDone();
    return  res;
}

PickArmPos SingleHeadMachineMaterialPickArm::sucker1getCurrentPos()
{
    PickArmPos current_pos;
    current_pos.x = motor_x->GetFeedbackPos();
    current_pos.y = motor_y->GetFeedbackPos();
    current_pos.z = motor_z->GetFeedbackPos();
    current_pos.v = motor_vcm1->GetFeedbackPos();
    current_pos.th = motor_th1->GetFeedbackPos();
    return  current_pos;
}

PickArmPos SingleHeadMachineMaterialPickArm::sucker2getCurrentPos()
{
    PickArmPos current_pos;
    current_pos.x = motor_x->GetFeedbackPos();
    current_pos.y = motor_y->GetFeedbackPos();
    current_pos.z = motor_z->GetFeedbackPos();
    current_pos.v = motor_vcm2->GetFeedbackPos();
    current_pos.th = motor_th2->GetFeedbackPos();
    return  current_pos;
}

double SingleHeadMachineMaterialPickArm::vcm1GetPhysicHeight(double current_v1, double current_z)
{
    return  parent->parameters.vcm1BaseHeight() - current_v1 + current_z;
}

double SingleHeadMachineMaterialPickArm::vcm2GetPhysicHeight(double current_v2, double current_z)
{
    return  parent->parameters.vcm2BaseHeight() - current_v2 + current_z;
}

double SingleHeadMachineMaterialPickArm::vcm1GetMotorPos(double current_p, double current_z)
{
    return  parent->parameters.vcm1BaseHeight() - current_p + current_z;
}

double SingleHeadMachineMaterialPickArm::vcm2GetMotorPos(double current_p, double current_z)
{
    return  parent->parameters.vcm2BaseHeight() - current_p + current_z;
}

bool SingleHeadMachineMaterialPickArm::LiftToSafeHeight()
{
    bool res;
    motor_z->MoveToPos(parent->parameters.saftyZHeight());
    motor_vcm1->MoveToPos(0);
    res = motor_z->WaitMoveStop();
    res &= motor_vcm1->WaitMoveStop();
    return res;
}

bool SingleHeadMachineMaterialPickArm::RightToSafeHeight()
{
    bool res;
    motor_z->MoveToPos(parent->parameters.saftyZHeight());
    motor_vcm2->MoveToPos(0);
    res = motor_z->WaitMoveStop();
    res &= motor_vcm2->WaitMoveStop();
    return res;
}

bool SingleHeadMachineMaterialPickArm::ZV1MoveSync(double zpos, double v1pos)
{
    //XtMotor::Move2AxisToPos(z,zpos,vcm1,v1pos);
    motor_z->MoveToPos(zpos);
    motor_vcm1->MoveToPos(v1pos);
    bool res = motor_z->WaitMoveStop();
    res&= motor_vcm1->WaitMoveStop();
    return res;
}

bool SingleHeadMachineMaterialPickArm::ZV2MoveSync(double zpos, double v2pos)
{
    //XtMotor::Move2AxisToPos(z,zpos,vcm2,v2pos);
    motor_z->MoveToPos(zpos);
    motor_vcm2->MoveToPos(v2pos);
    bool res = motor_z->WaitMoveStop();
    res &= motor_vcm2->WaitMoveStop();
    return res;
}

bool SingleHeadMachineMaterialPickArm::XY_ZMoveSync(double xpos, double ypos, double zpos)
{
    //XtMotor::Move2AxisToPos(x,xpos,y,ypos);
    motor_x->MoveToPos(xpos);
    motor_y->MoveToPos(ypos);
    bool res = motor_x->WaitMoveStop();
    res &= motor_y->WaitMoveStop();
    if(!res)
        res = motor_z->MoveToPosSync(zpos);
    return res;
}

bool SingleHeadMachineMaterialPickArm::XYC2SyncMove(double xpos, double ypos, double cpos)
{
    bool res;
    //XtMotor::Move2AxisToPos(x,xpos,y,ypos);
    motor_x->MoveToPos(xpos);
    motor_y->MoveToPos(ypos);
    motor_th2->MoveToPos(cpos);
    res = motor_x->WaitMoveStop();
    res &= motor_y->WaitMoveStop();
    res &= motor_th2->WaitMoveStop();
    return res;
}

bool SingleHeadMachineMaterialPickArm::XYC1SyncMove(double xpos, double ypos, double cpos)
{
    bool res;
    //XtMotor::Move2AxisToPos(motor_x,xpos,motor_y,ypos);
    motor_x->MoveToPos(xpos);
    motor_y->MoveToPos(ypos);
    motor_th1->MoveToPos(cpos);
    res = motor_x->WaitMoveStop();
    res &= motor_y->WaitMoveStop();
    res &= motor_th1->WaitMoveStop();

    return res;
}

bool SingleHeadMachineMaterialPickArm::XYZC1SyncMove(double xpos, double ypos, double zpos, double cpos)
{
    bool res;
//    XtMotor::Move2AxisToPos(x,xpos,y,ypos);
//    XtMotor::Move2AxisToPos(z,zpos,th1,cpos);
    motor_x->MoveToPos(xpos);
    motor_y->MoveToPos(ypos);
    motor_z->MoveToPos(zpos);
    motor_th1->MoveToPos(cpos);
    res = motor_x->WaitMoveStop();
    res &= motor_y->WaitMoveStop();
    res &= motor_z->WaitMoveStop();
    res &= motor_th1->WaitMoveStop();
    return res;
}

bool SingleHeadMachineMaterialPickArm::XYZC2SyncMove(double xpos, double ypos, double zpos, double cpos)
{
    bool res;
//    XtMotor::Move2AxisToPos(x,xpos,y,ypos);
//    XtMotor::Move2AxisToPos(z,zpos,th2,cpos);
    motor_x->MoveToPos(xpos);
    motor_y->MoveToPos(ypos);
    motor_z->MoveToPos(zpos);
    motor_th2->MoveToPos(cpos);
    res = motor_x->WaitMoveStop();
    res &= motor_y->WaitMoveStop();
    res &= motor_z->WaitMoveStop();
    res &= motor_th2->WaitMoveStop();
    return res;
}



