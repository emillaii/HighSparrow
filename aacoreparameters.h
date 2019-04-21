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

public:
    explicit AACoreParameters(){}
    Q_PROPERTY(double EFL READ EFL WRITE setEFL NOTIFY paramsChanged)
    Q_PROPERTY(int MaxIntensity READ MaxIntensity WRITE setMaxIntensity NOTIFY paramsChanged)
    Q_PROPERTY(int MinArea READ MinArea WRITE setMinArea NOTIFY paramsChanged)
    Q_PROPERTY(int MaxArea READ MaxArea WRITE setMaxArea NOTIFY paramsChanged)
    Q_PROPERTY(int SensorXRatio READ SensorXRatio WRITE setSensorXRatio NOTIFY paramsChanged)
    Q_PROPERTY(int SensorYRatio READ SensorYRatio WRITE setSensorYRatio NOTIFY paramsChanged)

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

public slots:
void setEFL(double EFL)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_EFL, EFL))
        return;

    m_EFL = EFL;
}
void setMaxIntensity(int MaxIntensity)
{
    m_MaxIntensity = MaxIntensity;
}

void setMinArea(int MinArea)
{
    m_MinArea = MinArea;
}

void setMaxArea(int MaxArea)
{
    m_MaxArea = MaxArea;
}

void setSensorXRatio(int SensorXRatio)
{
    m_SensorXRatio = SensorXRatio;
}

void setSensorYRatio(int SensorYRatio)
{
    m_SensorYRatio = SensorYRatio;
}

signals:
void paramsChanged(int MaxIntensity);
};

#endif // AACOREPARAMETERS_H
