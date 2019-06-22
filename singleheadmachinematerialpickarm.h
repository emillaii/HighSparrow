#ifndef SINGLEHEADMACHINEMATERIALPICKARM_H
#define SINGLEHEADMACHINEMATERIALPICKARM_H

#include "XtCylinder.h"
#include "XtVacuum.h"
#include "XtVcMotor.h"
#include "errorcode.h"
#include "singleheadmachinematerialpickarmparameter.h"
#include "xtmotor.h"

# define POS_ERROR 0.001

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
    SingleHeadMachineMaterialPickArmParameter parametrs;
    SingleHeadMachineMaterialPickArm(QString name = "SingleHeadMachineMaterialPickArm");
    void Init(XtMotor *motor_x,XtMotor *motor_y,XtMotor *motor_z,
              XtMotor *motor_th1,XtMotor *motor_th2,
              XtVcMotor *motor_vcm1,XtVcMotor *motor_vcm2,
              XtVacuum *vacuum_lens_suction,XtVacuum *vacuum_sensor_suction,
              XtVacuum *vacuum_lut,XtVacuum *vacuum_sut,XtCylinder *pogopin);
    PickArmPos pick_lens_base_pos;//pick lens的基础位置，以相机中心为准，要加吸嘴偏移
    PickArmPos pick_cmos_base_pos;//pick cmos的基础位置，以相机中心为准，要加吸嘴便宜
    PickArmPos pick_product_base_pos;//pick 产品的基础位置，以相机中心为准，要加吸嘴偏移
    PickArmPos place_lens_base_pos;//放lens的位置，以相机中心为准，要加吸嘴偏移
    PickArmPos place_cmos_base_pos;//放cmos的位置，以相机中心为准，要加吸嘴偏移
    PickArmPos place_product_base_pos;//放产品的基础位置，以相机中心为准，要加吸嘴偏移
    PickArmPos cmos_escape_offset;//place cmos 到sut 的时候，需要一个偏移来避开pogopin的压板,v為物理高度
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
};

#endif // SINGLEHEADMACHINEMATERIALPICKARM_H
