#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QObject>
#include "pixel2mech.h"
//#include "motor.h"
#include <qmath.h>
#include <QVector>
#include <QPointF>
//#include "dothinkey.h"
#include "calibration_parameter.h"
#include "errorcode.h"
#include "visionmodule.h"
#include "wordoplight.h"
#include "xtmotor.h"


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

const static QPointF IMG_CENTER=QPointF(329,247);
const static QPointF CHART_IMG_CENTER=QPointF(3264/2,2448/2);

const static QList<QString> CaliName={ "upMush", "upDownGlass", "downSensor",
                                      "downLen", "dispenseTipOffset", "upLens",
                                      "pickarmSensor","sensorChart"};

class Calibration : public QObject,public ErrorBase
{
    Q_OBJECT
public:
    explicit Calibration(QString name,QString file_name,XtMotor* motor_x,XtMotor* motor_y,WordopLight * light_controller,int ch,int lighting,QString camera_name, QString pr_name, QObject *parent = nullptr);

    Calibration(const Calibration &calibration) = delete;
    Calibration &operator=(const Calibration &calibration) = delete;
    void ChangeParameter(WordopLight * light_controller,int ch,int lighting,QString camera_name, QString pr_name);
    void loadJsonConfig();
    void saveJsonConfig();
    bool performCalibration();
    double getRotationAngle();
    bool getDeltaDistanceFromCenter(const QPointF pixelPoint, QPointF &distanceMech);
    bool getMechPoint(QPointF pixelPoint, QPointF &mechPoint);
    bool getCaliMapping(Pixel2Mech &caliMapping);
    bool getDeltaDistanceFromCenter(CaliType caliType, QPointF pixelPoint, QPointF &distanceMech);
    bool getMechPoint(CaliType caliType, QPointF pixelPoint, QPointF &mechPoint);
    bool getCaliMapping(CaliType caliType, Pixel2Mech &caliMapping);

signals:
    void vision_image_changed_signal(QImage image);
public slots:
private:
    virtual bool GetPixelPoint(double &x,double &y);
    bool coordinateA2BMapping(const QVector<QPointF>& APoints, const QVector<QPointF>& BPoints);
    double caculateRotationAngle();
public:
protected:
    QString name;
    QString file_name;
    XtMotor* motor_x;
    XtMotor* motor_y;
    WordopLight *light_controller;
    int ch;
    int lighting;
    VisionModule * visionModule;
    QString camera_name;
    QString pr_name;

    CalibrationParameter parameters;
    Pixel2Mech mapping;

    Pixel2Mech mA2BMapping;
    bool mIsMapping;
    QMatrix mA2BMatrix;
    QPointF mOriginB;
    double mRotationB;
};

#endif // CALIBRATION_H
