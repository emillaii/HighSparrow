#ifndef MATERIAL_CARRIER_H
#define MATERIAL_CARRIER_H

#include "XtVacuum.h"
#include "XtVcMotor.h"
#include "material_carrier_parameter.h"
#include "position_define.h"
#include "xtmotor.h"
class MaterialCarrier:public ErrorBase
{
public:
    MaterialCarrier();
    void Init(QString name, XtMotor* motor_x,XtMotor* motor_y,XtVcMotor* motor_z,XtVacuum* vacuum);
public:
    XtMotor* motor_x = Q_NULLPTR;
    XtMotor* motor_y = Q_NULLPTR;
    XtVcMotor* motor_z = Q_NULLPTR;
    XtVacuum* vacuum = Q_NULLPTR;
public:
    MaterialCarrierParameter parameters;
    QString callerName = "";
public:
    void setCallerFunctionName(QString name) { this->callerName = name; }
    bool CheckXYZArrived(double x,double y,double z);
    bool CheckXYDistanceBigger(double x,double y,double check_distance);
    bool Move_SZ_XY_Z_Sync(double x,double y,double z,int timeout = 3000);
    bool Move_SZ_SX_Y_X_Z_Sync(double x,double y,double z,bool check_autochthonous = false,bool check_softlanding = false, double check_distance = 0.1,int timeout = 3000);
    bool Move_SZ_SX_Y_X_Sync(double x,double y,double y_error,bool check_autochthonous = false,bool check_softlanding = false, double check_distance = 0.1,int timeout = 3000);
    bool Move_SZ_SX_YS_X_Z_Sync(double x,double y,double z,bool check_autochthonous = false,bool check_softlanding = false,double check_distance = 0.1,int timeout = 3000);
    bool Move_SZ_SY_X_Y_Z_Sync(double x,double y,double z,bool check_autochthonous = false,bool check_softlanding = false,double check_distance = 5, int timeout = 3000);
    bool Move_SZ_SY_X_YS_Z_Sync(double x,double y,double z,bool check_autochthonous = false,bool check_softlanding = false,double check_distance = 5, int timeout = 3000);
    bool Move_SZ_XY_ToPos(double x,double y,int timeout = 3000);
    bool Wait_XY_ToPos(double x,double y,int timeout = 3000);
    bool StepMove_XY_Sync(double step_x,double step_y,int timeout = 3000);
    bool StepMove_SZ_XY_Sync(double step_x,double step_y,int timeout = 3000);
    bool StepMove_Z(double z, int timeout = 3000);
    void Move_XY_ToPos(double x,double y);
    bool Move_Z_Sync(double z,int timeout = 3000);
    bool Move_X_Sync(double x, int timeout = 3000);
    bool ZSerchByForce(const double speed,const double force,const double search_limit = -1,const int vacuum_state = -1,XtVacuum* excute_vacuum = nullptr);
    bool ZSerchReturn();
    mPoint3D GetFeedBackPos();
};

#endif // MATERIAL_CARRIER_H
