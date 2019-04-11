#ifndef DISPENSE_MODULE_H
#define DISPENSE_MODULE_H

#include "dispenser.h"
#include "calibration.h"
#include "position_define.h"
#include "dispense_parameter.h"
#include "material_carrier.h"
class DispenseModule
{
public:
    DispenseModule();
    void Init(Calibration* calibration,Dispenser* dispenser,VisionModule* vision, MaterialCarrier* carrier,XtGeneralOutput* dispense_io);
    void loadConfig();
    void saveConfig();
    void updatePath();
    void setMapPosition(double x,double y,double pr_theta);
    void MoveToDispenseDot();
    void CalulateOffset();
private:
    QVector<mPoint3D> getDispensePath();
    bool PerformDispense();
private:
public:
private:
    QVector<QPointF> mechPoints;
    Calibration* calibration;
    double x = 0;
    double y = 0;
    double pr_theta = 0;
    DispenseParameter parameters;
    Dispenser* dispenser;
    VisionModule* vision;
    MaterialCarrier* carrier;
    XtGeneralOutput* dispense_io;
    mPoint3D start_pos;
};

#endif // DISPENSE_MODULE_H
