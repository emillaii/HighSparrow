#ifndef DISPENSE_MODULE_H
#define DISPENSE_MODULE_H

#include "dispenseModule/dispenser.h"
#include "calibration/calibration.h"
#include "position_define.h"
#include "dispenseModule/dispense_parameter.h"
#include "material_carrier.h"
#include "vision/visionmodule.h"
class DispenseModule:public QObject
{
    Q_OBJECT
public:
    DispenseModule();
    void Init(QString file_path,QString name,Calibration* calibration,Dispenser* dispenser,VisionModule* vision, MaterialCarrier* carrier,XtGeneralOutput* dispense_io);
//    void loadConfig();
//    void saveConfig();
    Q_INVOKABLE void updatePath();
    Q_INVOKABLE void updateSpeed();
    void setMapPosition(double pos_x,double pos_y);
    void setPRPosition(double pr_x,double pr_y,double pr_theta);
    Q_INVOKABLE void moveToDispenseDot(bool record_z = true);
    Q_INVOKABLE void calulateOffset(int digit = 4);
    Q_INVOKABLE bool performDispense();
    Dispenser* dispenser = Q_NULLPTR;
private:
    QVector<mPoint3D> getDispensePath();
public:
    DispenseParameter parameters;
    QDateTime lastDispenseDateTime;
private:
    QVector<QPointF> mechPoints;
    Calibration* calibration;
    double pos_x = 0;
    double pos_y = 0;
    double pr_x = 0;
    double pr_y = 0;
    double pr_theta = 0;
    VisionModule* vision = Q_NULLPTR;
    MaterialCarrier* carrier = Q_NULLPTR;
    XtGeneralOutput* dispense_io = Q_NULLPTR;
    mPoint3D start_pos;
    QString file_path = "";
    QString name = "";
    bool cancalculation = false;
signals:
    void callQmlRefeshImg(int);
};

#endif // DISPENSE_MODULE_H
