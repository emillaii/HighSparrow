#ifndef DISPENSE_MODULE_H
#define DISPENSE_MODULE_H

#include "dispenser.h"
#include "calibration.h"
#include "position_define.h"
#include "dispense_parameter.h"
#include "material_carrier.h"
#include "visionmodule.h"
class DispenseModule:public QObject
{
    Q_OBJECT
public:
    DispenseModule();
    void Init(Calibration* calibration,Dispenser* dispenser,VisionModule* vision, MaterialCarrier* carrier,XtGeneralOutput* dispense_io);
    void loadConfig();
    void saveConfig();
    void updatePath();
    void setMapPosition(double x,double y,double pr_theta);
    Q_INVOKABLE void moveToDispenseDot(bool record_z = true);
    Q_INVOKABLE void calulateOffset();
    Q_INVOKABLE bool performDispense();
private:
    QVector<mPoint3D> getDispensePath();
private:
public:
    DispenseParameter parameters;
private:
    QVector<QPointF> mechPoints;
    Calibration* calibration;
    double x = 0;
    double y = 0;
    double pr_theta = 0;
    Dispenser* dispenser;
    VisionModule* vision;
    MaterialCarrier* carrier;
    XtGeneralOutput* dispense_io;
    mPoint3D start_pos;
};

#endif // DISPENSE_MODULE_H
