﻿#ifndef SINGLEHEADMACHINEMATERIALPICKARM_H
#define SINGLEHEADMACHINEMATERIALPICKARM_H

#include "XtCylinder.h"
#include "XtVacuum.h"
#include "XtVcMotor.h"
#include "errorcode.h"
#include "pickarmposition.h"
#include "position_define.h"
#include "singleheadmachinematerialpickarmparameter.h"
#include "xtmotor.h"

# define POS_ERROR 0.001
class SingleHeadMachineMaterialLoaderModule;
struct PickArmPos
{
    double x;
    double y;
    double z;
    double v;
    double th;
    bool isEquilPos(PickArmPos target_pos , double error = POS_ERROR);
    bool isAbovePos(PickArmPos target_pos, double error = POS_ERROR);
};

class SingleHeadMachineMaterialPickArm:public ErrorBase
{
public:
    SingleHeadMachineMaterialPickArmParameter parameters;
    SingleHeadMachineMaterialLoaderModule* parent = nullptr;
    SingleHeadMachineMaterialPickArm(QString name = "SingleHeadMachineMaterialPickArm");
    void Init(XtMotor *motor_x,XtMotor *motor_y,XtMotor *motor_z,
              XtMotor *motor_th1,XtMotor *motor_th2,
              XtVcMotor *motor_vcm1,XtVcMotor *motor_vcm2,
              XtVacuum *vacuum_lens_suction,XtVacuum *vacuum_sensor_suction,
              XtVacuum *vacuum_lut,XtVacuum *vacuum_sut,XtCylinder *pogopin);
//    PickArmPos pick_lens_base_pos;//pick lens的基础位置，以相机中心为准，要加吸嘴偏移
//    PickArmPos pick_cmos_base_pos;//pick cmos的基础位置，以相机中心为准，要加吸嘴便宜
//    PickArmPos pick_product_base_pos;//pick 产品的基础位置，以相机中心为准，要加吸嘴偏移
//    PickArmPos place_lens_base_pos;//放lens的位置，以相机中心为准，要加吸嘴偏移
//    PickArmPos place_cmos_base_pos;//放cmos的位置，以相机中心为准，要加吸嘴偏移
//    PickArmPos place_product_base_pos;//放产品的基础位置，以相机中心为准，要加吸嘴偏移
//    PickArmPos cmos_escape_offset;//place cmos 到sut 的时候，需要一个偏移来避开pogopin的压板,v為物理高度

    PickArmPosition pick_lens_base_position;
    PickArmPosition pick_cmos_base_position;
    PickArmPosition pick_product_base_position;
    PickArmPosition place_lens_base_position;
    PickArmPosition place_cmos_base_position;
    PickArmPosition place_product_base_position;
    PickArmPosition cmos_escape_offset_position;
    Position lens_to_pr_distance_position;
    Position cmos_to_pr_distance_position;

//    QPointF lens_to_pr_distance;//lens吸嘴到相机的偏移
//    QPointF cmos_to_pr_distance;//cmos吸嘴到相机的偏移

    XtMotor *motor_x;
    XtMotor *motor_y;
    XtMotor *motor_z;
    XtMotor *motor_th1;
    XtMotor *motor_th2;
    XtVcMotor *motor_vcm1;
    XtVcMotor *motor_vcm2;
    XtVacuum *vacuum_lens_suction;//lens吸嘴，右吸嘴
    XtVacuum *vacuum_sensor_suction;//cmos吸嘴，左吸嘴
    XtVacuum *vacuum_lut;//LUT真空
    XtVacuum *vacuum_sut;//SUT真空
    XtCylinder *pogopin;
    bool CheckZV1V2AboveSafeHeight(double z_height,double error = POS_ERROR,int times = 10);
    bool ZV1V2ToSafeHeight(double z_height);
    bool vcm1SoftLanding(double force,double target_pos);
    bool vcm2SoftLanding(double force,double target_pos);
    bool vcm1SoftLandingReturn();
    bool vcm2SoftLandingReturn();
    PickArmPos sucker1getCurrentPos();
    PickArmPos sucker2getCurrentPos();
    double vcm1GetPhysicHeight(double current_v1,double current_z);
    double vcm2GetPhysicHeight(double current_v2,double current_z);
    double vcm1GetMotorPos(double current_p,double current_z);
    double vcm2GetMotorPos(double current_p,double current_z);
    bool LiftToSafeHeight();
    bool RightToSafeHeight();
    bool ZV1MoveSync(double zpos,double v1pos);
    bool ZV2MoveSync(double zpos,double v2pos);
    bool XY_ZMoveSync(double xpos,double ypos,double zpos);

    bool XYC2SyncMove(double xpos,double ypos,double cpos);
    bool XYC1SyncMove(double xpos,double ypos,double cpos);
    bool XYZC1SyncMove(double xpos,double ypos,double zpos,double cpos);
    bool XYZC2SyncMove(double xpos,double ypos,double zpos,double cpos);
};

#endif // SINGLEHEADMACHINEMATERIALPICKARM_H