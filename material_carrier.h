#ifndef MATERIAL_CARRIER_H
#define MATERIAL_CARRIER_H

#include "XtVacuum.h"
#include "XtVcMotor.h"
#include "material_carrier_parameter.h"
#include "position_define.h"
#include "xtmotor.h"
class MaterialCarrier
{
public:
    MaterialCarrier();
    void Init(QString name, XtMotor* motor_x,XtMotor* motor_y,XtVcMotor* motor_z,XtVacuum* vacuum);
private:
    XtMotor* motor_x = Q_NULLPTR;
    XtMotor* motor_y = Q_NULLPTR;
    XtVcMotor* motor_z = Q_NULLPTR;
    XtVacuum* vacuum = Q_NULLPTR;
public:
    MaterialCarrierParameter parameters;
public:
    bool Move_SZ_XY_Z_Sync(double x,double y,double z,int timeout = 3000);
    bool Move_SZ_SX_Y_X_Z_Sync(double x,double y,double z,bool check_autochthonous = false,int timeout = 3000);
    bool Move_SZ_SY_X_Y_Z_Sync(double x,double y,double z,bool check_autochthonous = false,int timeout = 3000);
    bool Move_SZ_XY_ToPos(double x,double y,int timeout = 3000);
    bool Wait_XY_ToPos(double x,double y,int timeout = 3000);
    bool StepMove_XY_Sync(double step_x,double step_y,int timeout = 3000);
    bool StepMove_SZ_XY_Sync(double step_x,double step_y,int timeout = 3000);
    void Move_XY_ToPos(double x,double y);
    bool Move_Z_Sync(double z,int timeout = 3000);
    bool ZSerchByForce(double& result_pos,double force,double search_limit = -1,int vacuum_state = -1,XtVacuum* excute_vacuum = nullptr);
    bool ZSerchReturn();
    mPoint3D GetFeedBackPos();
};

#endif // MATERIAL_CARRIER_H
