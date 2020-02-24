#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QObject>
#include "Vision/pixel2mech.h"
#include <qmath.h>
#include <QVector>
#include <QPointF>
#include "calibration_parameter.h"
#include "Utils/errorcode.h"
#include "Vision/vision_location.h"
#include "XtMotion/xtmotor.h"


enum CaliType{
    caliUpMush,
    caliUpDownGlass,
    caliDownSensor,
    caliDownLen,
    caliDispenseTipOffset,
    caliUpLen,
    caliPickarmSensor,
    caliSensorChart
};

class Calibration : public QObject,public ErrorBase
{
    Q_OBJECT
public:
    explicit Calibration(QString name = "UnmaningCalibration",QString file_path = "config//", QObject *parent = nullptr);
    void Init(XtMotor* motor_x,XtMotor* motor_y,VisionLocation* location);
    Calibration(const Calibration &calibration) = delete;
    Calibration &operator=(const Calibration &calibration) = delete;
    bool performCalibration();
    bool performPRResult(PrOffset offset);
    double getRotationAngle();
    bool getDeltaDistanceFromCenter(const QPointF pixelPoint, QPointF &distanceMech);
    QPointF getOnePxielDistance();
    QPointF getOneXPxielDistance();
    QPointF getOneYPxielDistance();
    double caculateRotationAngle();
    Pixel2Mech *getCaliMapping();

public:
    virtual bool GetPixelPoint(double &x,double &y);
private:
    bool coordinateA2BMapping(const QVector<QPointF>& APoints, const QVector<QPointF>& BPoints);
public:
    CalibrationParameter parameters;
protected:
    QString name;
    QString file_path;
    XtMotor* motor_x;
    XtMotor* motor_y;
    VisionLocation* location;

    Pixel2Mech mapping;
    Pixel2Mech mA2BMapping;
    bool mIsMapping;
    QMatrix mA2BMatrix;
    QPointF mOriginB;
    double mRotationB;
};

#endif // CALIBRATION_H
