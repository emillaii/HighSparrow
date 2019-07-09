#ifndef SENSORCLIP_H
#define SENSORCLIP_H

#include "XtMotion/XtCylinder.h"
#include "sensorclipparameter.h"
#include "XtMotion/xtmotor.h"

#include <QObject>

class SensorClip : public QObject,public ErrorBase
{
    Q_OBJECT
public:
    explicit SensorClip(QObject *parent = nullptr);
    void Init(QString name,SensorClipStandParameter* stand_parameter);
    int getCurrentIndex();
    void caculateStandParameter();
    void setCurrentIndex(int index);
    double getCurrentPosition();
    double getNextPosition();
    double getChangeState();
    void finishCurrentPosition();
    void resetClip();
public:
    SensorClipParameter parameters;
private:
    SensorClipStandParameter* stand_parameter = Q_NULLPTR;
};

#endif // SENSORCLIP_H
