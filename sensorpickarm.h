#ifndef SENSORPICKARM_H
#define SENSORPICKARM_H

#include "utils/errorcode.h"
#include "material_picker.h"
#include "utils/pixel2mech.h"
#include "sensorpickarmparameter.h"


class SensorPickArm: public ErrorBase
{
public:
    SensorPickArm(QString name = "SensorPickArm");
    void Init(XtMotor* motor_x,XtMotor* motor_y,MaterialPicker * picker1,MaterialPicker * picker2);
    bool move_XY_Synic(const QPointF position,const bool check_arrived = false,const bool check_softlanding = false,int timeout = 10000);
    bool checkXYArrived(const double x,const double y);
    bool checkXYT1Arrived(const double x,const double y,const double t);
    bool move_XYT1_Synic(const double x,const double y,const double t,const bool check_arrived = false,const bool check_softlanding = false,int timeout = 10000);
    bool move_XYT1_Pos(const double x,const double y,const double t,const bool check_arrived = false,const bool check_softlanding = false,int timeout = 10000);
    bool checkXYT2Arrived(const double x,const double y,const double t);
    bool move_XYT2_Synic(const double x,const double y,const double t,const bool check_arrived = false,const bool check_softlanding = false,int timeout = 10000);
    bool checkXYT1T2Arrived(const double x,const double y,const double t1,const double t2);
    bool move_XYT1T2_Synic(const double x,const double y,const double t1,const double t2,const bool check_arrived = false,const bool check_softlanding = false,int timeout = 10000);
    bool move_XYT2_Pos(const double x,const double y,const double t,const bool check_arrived = false,const bool check_softlanding = false,int timeout = 10000);
    bool move_XeYe_Z1_XY(double z, double escape_x,double escape_y, const bool check_softlanding = false,int timeout = 10000);
    bool move_XeYe_Z2(double z, double escape_x,double escape_y, const bool check_softlanding = false,int timeout = 10000);
    bool stepMove_XY_Synic(const double step_x,const double step_y,const bool check_softlanding = false,int timeout = 10000);
    bool stepMove_XYT1_Synic(const double step_x,const double step_y,const double step_t1,const bool check_softlanding = false,int timeout = 10000);
    bool stepMove_XYT1_Pos(const double step_x,const double step_y,const double step_t1,const bool check_softlanding = false,int timeout = 10000);
    bool wait_XYT1_Arrived(int timeout = 10000);
    bool stepMove_XYT2_Synic(const double step_x,const double step_y,const double step_t2,const bool check_softlanding = false,int timeout = 10000);
    bool stepMove_XYT2_Pos(const double step_x,const double step_y,const double step_t2,const bool check_softlanding = false,int timeout = 10000);
    bool wait_XYT2_Arrived(int timeout = 10000);

    bool MoveZ1Synic(double target,int timeout = 10000);
    bool Z1MeasureHeight(double speed,double force,int hold_time = 200,int timeout = 10000);
    bool Z1SearchByForce(double speed,double force,double limit,double margin,int timeout = 10000);
    bool Z1SerchReturn(int timeout = 10000);
    bool MoveZ1ToSafeHeighSync();
    double GetZ1MeasuredHeight(bool get_current = false);


    bool MoveZ2Synic(double target,int timeout = 10000);
    bool Z2MeasureHeight(double speed,double force,int hold_time = 200,int outtime = 10000);
    bool Z2SerchByForce(double speed,double force,double limit,double margin, int timeout = 10000);
    bool Z2SerchReturn(int timeout = 10000);
    bool MoveZ2ToSafeHeighSync();
    double GetZ2MeasuredHeight(bool get_current = false);
private:
public:
    QString callerName = "";
    void setCallerName(QString name) { callerName = name; }
    SensorPickArmParameter parameters;
    XtMotor* motor_x = Q_NULLPTR;
    XtMotor* motor_y = Q_NULLPTR;
    MaterialPicker * picker1 = Q_NULLPTR;
    MaterialPicker * picker2 = Q_NULLPTR;
private:
    double softlanding_position;
    double softlanding_position2;
};

#endif // SENSORPICKARM_H
