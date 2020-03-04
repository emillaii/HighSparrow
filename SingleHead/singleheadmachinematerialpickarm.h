#ifndef SINGLEHEADMACHINEMATERIALPICKARM_H
#define SINGLEHEADMACHINEMATERIALPICKARM_H

#include "XtMotion/XtCylinder.h"
#include "XtMotion/XtVacuum.h"
#include "XtMotion/XtVcMotor.h"
#include "Utils/errorcode.h"
#include "singleheadmachinematerialpickarmparameter.h"
#include "XtMotion/xtmotor.h"
#include "Vision/vision_location.h"

struct PickArmPos
{
    double TL_X;
    double PA_X;
    double PA_Y;
};

class SingleHeadMachineMaterialPickArm:public ErrorBase
{
public:

    SingleHeadMachineMaterialPickArm(QString name = "SingleHeadMachineMaterialPickArm");

    void Init(XtMotor *motor_x,XtMotor *motor_y,XtMotor *motor_z,
              XtMotor *motor_th1,XtMotor *motor_th2,
              XtVcMotor *motor_vcm1,XtVcMotor *motor_vcm2,XtMotor *motor_pax,
              XtVacuum *vacuum_picker1_suction,XtVacuum *vacuum_picker2_suction,
              XtVacuum *vacuum_lut,XtVacuum *vacuum_sut,XtCylinder *pogopin
              );

    bool move_Xm_Origin();
    bool move_Z_Synic(const double position, int timeout = 10000); //huawei AA
    bool move_XY_Synic(const QPointF position,const bool check_softlanding = false,int timeout = 10000);
    bool move_XYXm_Sync(PickArmPos pos, const bool check_softlanding = false,int timeout = 10000);
    bool move_XmY_Synic(const QPointF position,const bool check_softlanding = false,int timeout = 10000);
    bool stepMove_XmYT1_Synic(const double step_x,const double step_y,const double step_t1,const bool check_softlanding = false,int timeout = 10000);
    bool stepMove_XmYT2_Synic(const double step_x,const double step_y,const double step_t2,const bool check_softlanding = false,int timeout = 10000);
    bool ZSerchByForce(int picker,double speed,double force,bool check_softlanding = false,int timeout = 10000);
    bool ZSerchByForce(int picker,double speed,double force,double limit,double margin,int finish_time,bool open_vacuum = true,bool need_return = true, int timeout = 10000);
    bool ZSerchReturn(int picker,int timeout = 10000);
    double GetSoftladngPosition(bool get_current = false,int index=0);
public:
    SingleHeadMachineMaterialPickArmParameter parameters;
    XtMotor *motor_x;
    XtMotor *motor_y;
    XtMotor *motor_z;
    XtMotor *motor_th1;
    XtMotor *motor_th2;
    XtVcMotor *motor_vcm1;
    XtVcMotor *motor_vcm2;
    XtMotor *motor_pax;
    XtVacuum *vacuum_picker1_suction;//lens吸嘴，右吸嘴
    XtVacuum *vacuum_picker2_suction;//cmos吸嘴，左吸嘴
    XtVacuum *vacuum_lut;//LUT真空
    XtVacuum *vacuum_sut;//SUT真空
    XtCylinder *pogopin;
private:
    double softlanding_position;
};

#endif // SINGLEHEADMACHINEMATERIALPICKARM_H
