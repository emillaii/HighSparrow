#ifndef MATERIAL_CARRIER_H
#define MATERIAL_CARRIER_H

#include "XtMotion/XtVacuum.h"
#include "XtMotion/XtVcMotor.h"
#include "material_carrier_parameter.h"
#include "Utils/position_define.h"
#include "XtMotion/xtmotor.h"
class MaterialCarrier:public ErrorBase
{
public:
    MaterialCarrier();
    void Init(QString name, XtMotor* motor_x,XtMotor* motor_y,XtVcMotor* motor_z,XtVacuum* vacuum);

    void loadParams(QString file_name);
    void saveParams(QString file_name);
public:
    XtMotor* motor_x = Q_NULLPTR;
    XtMotor* motor_y = Q_NULLPTR;
    XtVcMotor* motor_z = Q_NULLPTR;
    XtVacuum* vacuum = Q_NULLPTR;
public:
    MaterialCarrierParameter parameters;
public:
    bool Move_SZ_XY_Z_Sync(double x,double y,double z,int timeout = 3000);
    bool Move_SZ_SX_Y_X_Z_Sync(double x,double y,double z,bool check_autochthonous = false,double check_distance = 0.1,int timeout = 3000);
    bool Wait_XY_ToPos(double x,double y,int timeout = 3000);
    bool StepMove_XY_Sync(double step_x,double step_y,int timeout = 3000);
    bool StepMove_SZ_XY_Sync(double step_x,double step_y,int timeout = 3000);
    bool StepMove_Z(double z, int timeout = 3000);
    bool Move_Z_Sync(double z,int timeout = 3000);
    bool ZSerchByForce(const double speed,const double force,const double search_limit = -1,const int vacuum_state = -1,XtVacuum* excute_vacuum = nullptr);
    bool ZSerchReturn();
    bool ZSerchByForce(const double speed,const double force,bool check_softlanding, int timeout = 10000);
    mPoint3D GetFeedBackPos();

    double GetSoftladngPosition(bool get_current = false);
    double softlandingPosition;

};

#endif // MATERIAL_CARRIER_H
