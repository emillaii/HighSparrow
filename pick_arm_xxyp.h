#ifndef PICK_ARM_XXYP_H
#define PICK_ARM_XXYP_H

#include "material_picker.h"
#include "materialtray.h"
#include "vision_location.h"



class PickArmXXYP:public ErrorBase
{
public:
    PickArmXXYP(QString name);
    void Init(XtMotor *motor_x_tray,XtMotor *motor_x,XtMotor *motor_y,MaterialPicker * picker);
    bool move_XtXY_Synic(QPointF position,double x,bool check_softlanding = false,int timeout = 10000);
    bool stepMove_XYTp_Synic(mPositionT position,bool check_softlanding = false,int timeout = 10000);
    bool Move_SZ_Sync(double z,bool check_softlanding = false,int timeout = 10000);
    double ZSerchByForce(double speed,double force,bool check_softlanding = false,int timeout = 10000);
    bool ZSerchByForce(double speed,double force,double limit,double margin,int finish_time,bool open_vacuum = true, int timeout = 10000);
    bool ZSerchReturn();
private:
    XtMotor *motor_x_tray = Q_NULLPTR;
    XtMotor *motor_x = Q_NULLPTR;
    XtMotor *motor_y = Q_NULLPTR;
    MaterialPicker * picker = Q_NULLPTR;

    // ErrorBase interface
public:
    QString GetCurrentError();
};

#endif // PICK_ARM_XXYP_H
