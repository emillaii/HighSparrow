#ifndef DISPENSE_MODULE_H
#define DISPENSE_MODULE_H

#include "dispensepather.h"
#include "dispenser.h"



class DispenseModule
{
public:
    DispenseModule();
    void Init(Calibration* calibration,Dispenser* dispenser);
    void loadConfig();
    void saveConfig();
    void updatePath();
    void setMapPosition(double x,double y,double pr_theta);
private:
    QVector<mPoint3D> getDispensePath();
    QVector<QPoint> Read_Dispense_Path();
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
};

#endif // DISPENSE_MODULE_H
