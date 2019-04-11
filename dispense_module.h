#ifndef DISPENSE_MODULE_H
#define DISPENSE_MODULE_H

#include "dispensepather.h"
#include "dispenser.h"



class DispenseModule
{
public:
    DispenseModule();
    void Init(Calibration* calibration,Dispenser* dispenser,VisionModule* vision);
    void loadConfig();
    void saveConfig();
    void updatePath();
    void setMapPosition(double x,double y,double pr_theta);
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
};

#endif // DISPENSE_MODULE_H
