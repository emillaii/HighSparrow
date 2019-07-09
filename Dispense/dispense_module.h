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
    void Init(QString file_path,QString name,Calibration* calibration,Dispenser* dispenser,VisionModule* vision, MaterialCarrier* carrier,XtGeneralOutput* dispense_io);
//    void loadConfig();
//    void saveConfig();
    Q_INVOKABLE void updatePath();
    void setMapPosition(double pos_x,double pos_y);
    void setPRPosition(double pr_x,double pr_y,double pr_theta);
    Q_INVOKABLE void moveToDispenseDot(bool record_z = true);
    Q_INVOKABLE void calulateOffset(int digit = 4);
    Q_INVOKABLE bool performDispense();
private:
    QVector<mPoint3D> getDispensePath();
private:
public:
    DispenseParameter parameters;
private:
    QVector<QPointF> mechPoints;
    Calibration* calibration;
    double pos_x = 0;
    double pos_y = 0;
    double pr_x = 0;
    double pr_y = 0;
    double pr_theta = 0;
    Dispenser* dispenser = Q_NULLPTR;
    VisionModule* vision = Q_NULLPTR;
    MaterialCarrier* carrier = Q_NULLPTR;
    XtGeneralOutput* dispense_io = Q_NULLPTR;
    mPoint3D start_pos;
    QString file_path = "";
    QString name = "";
};

#endif // DISPENSE_MODULE_H
