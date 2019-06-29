#ifndef AACOREPARAMETERS_H
#define AACOREPARAMETERS_H

#include <QObject>
#include <propertybase.h>
#include "config.h"

class AACoreParameters : public PropertyBase
{
    Q_OBJECT
    double m_EFL = 1.98;

    int m_MaxIntensity = 50;

    int m_MinArea = 10000;

    int m_MaxArea = 90000;

    int m_SensorXRatio = 892;

    int m_SensorYRatio = 892;

    double m_ROIRatio = 1.45;

    bool m_firstRejectSensor = false;

    int m_rejectTimes = 3;

public:
    explicit AACoreParameters(){}
    Q_PROPERTY(double EFL READ EFL WRITE setEFL NOTIFY paramsChanged)
    Q_PROPERTY(int MaxIntensity READ MaxIntensity WRITE setMaxIntensity NOTIFY paramsChanged)
    Q_PROPERTY(int MinArea READ MinArea WRITE setMinArea NOTIFY paramsChanged)
    Q_PROPERTY(int MaxArea READ MaxArea WRITE setMaxArea NOTIFY paramsChanged)
    Q_PROPERTY(int SensorXRatio READ SensorXRatio WRITE setSensorXRatio NOTIFY paramsChanged)
    Q_PROPERTY(int SensorYRatio READ SensorYRatio WRITE setSensorYRatio NOTIFY paramsChanged)
    Q_PROPERTY(double ROIRatio READ ROIRatio WRITE setROIRatio NOTIFY paramsChanged)
    Q_PROPERTY(bool firstRejectSensor READ firstRejectSensor WRITE setFirstRejectSensor NOTIFY firstRejectSensorChanged)
    Q_PROPERTY(int rejectTimes READ rejectTimes WRITE setRejectTimes NOTIFY rejectTimesChanged)

double EFL() const
{
    return m_EFL;
}
int MaxIntensity() const
{
    return m_MaxIntensity;
}

int MinArea() const
{
    return m_MinArea;
}

int MaxArea() const
{
    return m_MaxArea;
}

int SensorXRatio() const
{
    return m_SensorXRatio;
}

int SensorYRatio() const
{
    return m_SensorYRatio;
}

double ROIRatio() const
{
    return m_ROIRatio;
}

bool firstRejectSensor() const
{
    return m_firstRejectSensor;
}

int rejectTimes() const
{
    return m_rejectTimes;
}

public slots:
void setEFL(double EFL)
{
    m_EFL = EFL;
    emit paramsChanged();
}
void setMaxIntensity(int MaxIntensity)
{
    m_MaxIntensity = MaxIntensity;
    emit paramsChanged();
}

void setMinArea(int MinArea)
{
    m_MinArea = MinArea;
    emit paramsChanged();
}

void setMaxArea(int MaxArea)
{
    m_MaxArea = MaxArea;
    emit paramsChanged();
}

void setSensorXRatio(int SensorXRatio)
{
    m_SensorXRatio = SensorXRatio;
    emit paramsChanged();
}

void setSensorYRatio(int SensorYRatio)
{
    m_SensorYRatio = SensorYRatio;
    emit paramsChanged();
}

void setROIRatio(double ROIRatio)
{
    m_ROIRatio = ROIRatio;
    emit paramsChanged();
}

void setFirstRejectSensor(bool firstRejectSensor)
{
    if (m_firstRejectSensor == firstRejectSensor)
        return;

    m_firstRejectSensor = firstRejectSensor;
    emit firstRejectSensorChanged(m_firstRejectSensor);
}

void setRejectTimes(int rejectTimes)
{
    if (m_rejectTimes == rejectTimes)
        return;

    m_rejectTimes = rejectTimes;
    emit rejectTimesChanged(m_rejectTimes);
}

signals:
void paramsChanged();
void firstRejectSensorChanged(bool firstRejectSensor);
void rejectTimesChanged(int rejectTimes);
};
class AACoreStates: public PropertyBase
{
    Q_OBJECT
public:
    explicit AACoreStates(){}
    Q_PROPERTY(bool ngSensor READ ngSensor WRITE setNgSensor NOTIFY ngSensorChanged)
    Q_PROPERTY(bool ngLens READ ngLens WRITE setNgLens NOTIFY ngLensChanged)
    Q_PROPERTY(bool hasPrduct READ hasPrduct WRITE setHasPrduct NOTIFY hasPrductChanged)

public:
    bool ngSensor() const
    {
        return m_ngSensor;
    }

    bool ngLens() const
    {
        return m_ngLens;
    }

    bool hasPrduct() const
    {
        return m_hasPrduct;
    }

public slots:
    void setNgSensor(bool ngSensor)
    {
        if (m_ngSensor == ngSensor)
            return;

        m_ngSensor = ngSensor;
        emit ngSensorChanged(m_ngSensor);
    }

    void setNgLens(bool ngLens)
    {
        if (m_ngLens == ngLens)
            return;

        m_ngLens = ngLens;
        emit ngLensChanged(m_ngLens);
    }

    void setHasPrduct(bool hasPrduct)
    {
        if (m_hasPrduct == hasPrduct)
            return;

        m_hasPrduct = hasPrduct;
        emit hasPrductChanged(m_hasPrduct);
    }

signals:
    void ngSensorChanged(bool ngSensor);

    void ngLensChanged(bool ngLens);

    void hasPrductChanged(bool hasPrduct);

private:
    bool m_ngSensor;
    bool m_ngLens;
    bool m_hasPrduct;
};

#endif // AACOREPARAMETERS_H
