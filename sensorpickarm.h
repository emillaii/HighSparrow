#ifndef SENSORPICKARM_H
#define SENSORPICKARM_H

#include "errorcode.h"
#include "material_picker.h"
#include "pixel2mech.h"
#include "sensorpickarmparameter.h"


class SensorPickArm: public ErrorBase
{
public:
    SensorPickArm(QString name = "SensorPickArm");
    void Init(XtMotor* motor_x,XtMotor* motor_y,MaterialPicker * picker1,MaterialPicker * picker2);
    bool move_XY_Synic(const QPointF position,const bool check_softlanding = false,int timeout = 10000);
    bool checkXYT1Arrived(const double x,const double y,const double t);
    bool move_XYT1_Synic(const double x,const double y,const double t,const bool check_arrived = false,const bool check_softlanding = false,int timeout = 10000);
    bool move_XYT1_Pos(const double x,const double y,const double t,const bool check_arrived = false,const bool check_softlanding = false,int timeout = 10000);
    bool checkXYT2Arrived(const double x,const double y,const double t);
    bool move_XYT2_Synic(const double x,const double y,const double t,const bool check_arrived = false,const bool check_softlanding = false,int timeout = 10000);
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

    bool Z1MoveToPick(double target,bool open_vacuum = true,bool check_softlanding = false,bool need_return = true,int timeout = 10000);
    bool Z1SerchByForce(double speed,double force,bool check_softlanding = false,int timeout = 10000);
    bool Z1SerchByForce(double speed,double force,double limit,double margin,int finish_time,bool open_vacuum = true,bool need_return = true, int timeout = 10000);
    bool ZSerchReturn(int timeout = 10000);
    double GetSoftladngPosition(bool get_current = false);


    bool Z2MoveToPick(double target,bool open_vacuum = true,bool check_softlanding = false,bool need_return = true,int timeout = 10000);
    bool Z2SerchByForce(double speed,double force,bool check_softlanding = false,int timeout = 10000);
    bool Z2SerchByForce(double speed,double force,double limit,double margin,int finish_time,bool open_vacuum = true,bool need_return = true, int timeout = 10000);
    bool ZSerchReturn2(int timeout = 10000);
    double GetSoftladngPosition2(bool get_current = false);
private:
public:
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
