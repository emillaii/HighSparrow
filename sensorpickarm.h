#ifndef SENSORPICKARM_H
#define SENSORPICKARM_H

#include "errorcode.h"
#include "material_picker.h"
#include "pixel2mech.h"


class SensorPickArm: public ErrorBase
{
public:
    SensorPickArm(QString name = "SensorPickArm");
    void Init(XtMotor* motor_x,XtMotor* motor_y,MaterialPicker * picker1,MaterialPicker * picker2);
    bool move_XY_Synic(const QPointF position,const bool check_softlanding = false,int timeout = 10000);
    bool stepMove_XYT1_Synic(const PrOffset offset,const bool check_softlanding = false,int timeout = 10000);
    bool stepMove_XYT2_Synic(const PrOffset offset,const bool check_softlanding = false,int timeout = 10000);
    bool move_XYT_Synic(const PrOffset offset,const bool check_softlanding = false,int timeout = 10000);
private:
    XtMotor* motor_x = Q_NULLPTR;
    XtMotor* motor_y = Q_NULLPTR;
    MaterialPicker * picker1 = Q_NULLPTR;
    MaterialPicker * picker2 = Q_NULLPTR;
};

#endif // SENSORPICKARM_H
