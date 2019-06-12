#include "singleheadmachinematerialpickarm.h"

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
                                            XtMotor *motor_z,
                                            XtMotor *motor_th1,
                                            XtMotor *motor_th2,
                                            XtVcMotor *motor_vcm1,
                                            XtVcMotor *motor_vcm2,
                                            XtVacuum *vacuum_lens_suction,
                                            XtVacuum *vacuum_sensor_suction,
                                            XtVacuum *vacuum_lut,
                                            XtVacuum *vacuum_sut,
                                            XtCylinder *pogopin)
{
    this->motor_x = motor_x;
    this->motor_y = motor_y;
    this->motor_z = motor_z;
    this->motor_th1 = motor_th1;
    this->motor_th2 = motor_th2;
    this->motor_vcm1 = motor_vcm1;
    this->motor_vcm2 = motor_vcm2;
    this->vacuum_lens_suction = vacuum_lens_suction;
    this->vacuum_sensor_suction = vacuum_sensor_suction;
    this->vacuum_lut = vacuum_lut;
    this->vacuum_sut = vacuum_sut;
    this->pogopin = pogopin;
}



