#ifndef SENSORCLIP_H
#define SENSORCLIP_H

#include "XtCylinder.h"
#include "sensorclipparameter.h"
#include "xtmotor.h"

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
    bool getChangeState();
    void setChangeState(bool needChange = false);
    void finishCurrentPosition();
    void resetClip();
public:
    SensorClipParameter parameters;
private:
    SensorClipStandParameter* stand_parameter = Q_NULLPTR;
};

#endif // SENSORCLIP_H
