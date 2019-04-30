#ifndef PICK_ARM_XXYP_H
#define PICK_ARM_XXYP_H

#include "material_picker.h"
#include "materialtray.h"
#include "lenspickarmparameter.h"
#include "vision_location.h"



class LensPickArm:public ErrorBase
{
public:
    LensPickArm(QString name = "LensPickArm");
    void Init(XtMotor *motor_x_tray,XtMotor *motor_x,XtMotor *motor_y,MaterialPicker * picker);
    bool move_XtXY_Synic(QPointF position,double x,bool check_softlanding = false,int timeout = 30000);
    bool move_XY_Synic(double x,double y,bool check_softlanding = false,int timeout = 10000);
    bool stepMove_XYTp_Synic(PrOffset position,bool check_softlanding = false,int timeout = 10000);
    bool Move_SZ_Sync(double z,bool check_softlanding = false,int timeout = 10000);
    bool ZSerchByForce(double speed,double force,bool check_softlanding = false,int timeout = 10000);
    bool ZSerchByForce(double speed,double force,double limit,double margin,int finish_time,bool open_vacuum = true, bool need_z_return = true, int timeout = 10000);
    bool ZSerchReturn(int timeout = 10000);
    double GetSoftladngPosition(bool get_current = false);
    bool pickarmVaccum(bool isOn);
private:
    XtMotor *motor_x_tray = Q_NULLPTR;
    XtMotor *motor_x = Q_NULLPTR;
    XtMotor *motor_y = Q_NULLPTR;
    MaterialPicker * picker = Q_NULLPTR;
    double softlanding_position = 0;
public:
    LensPickArmParameter parameters;
    // ErrorBase interface
public:
    QString GetCurrentError();
};

#endif // PICK_ARM_XXYP_H
