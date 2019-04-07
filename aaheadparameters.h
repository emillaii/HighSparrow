#ifndef AAHEADPARAMETERS_H
#define AAHEADPARAMETERS_H

#include <QObject>
#include <propertybase.h>
#include "config.h"
#include <QChart>
#include <QSplineSeries>
using namespace QtCharts;

class AAHeadParameters : public PropertyBase
{
    Q_OBJECT
public:
    explicit AAHeadParameters(){}
    Q_PROPERTY(double PickLensPositionX READ PickLensPositionX WRITE setPickLensPositionX NOTIFY PickLensPositionXChanged)
    Q_PROPERTY(double PickLensPositionY READ PickLensPositionY WRITE setPickLensPositionY NOTIFY PickLensPositionYChanged)
    Q_PROPERTY(double PickLensPositionZ READ PickLensPositionZ WRITE setPickLensPositionZ NOTIFY PickLensPositionZChanged)
    Q_PROPERTY(double PickLensPositionA READ PickLensPositionA WRITE setPickLensPositionA NOTIFY PickLensPositionAChanged)
    Q_PROPERTY(double PickLensPositionB READ PickLensPositionB WRITE setPickLensPositionB NOTIFY PickLensPositionBChanged)
    Q_PROPERTY(double PickLensPositionC READ PickLensPositionC WRITE setPickLensPositionC NOTIFY PickLensPositionCChanged)
    Q_PROPERTY(double OCPositionX READ OCPositionX WRITE setOCPositionX NOTIFY OCPositionXChanged)
    Q_PROPERTY(double OCPositionY READ OCPositionY WRITE setOCPositionY NOTIFY OCPositionYChanged)
    Q_PROPERTY(double OCPositionZ READ OCPositionZ WRITE setOCPositionZ NOTIFY OCPositionZChanged)
    Q_PROPERTY(double OCPositionA READ OCPositionA WRITE setOCPositionA NOTIFY OCPositionAChanged)
    Q_PROPERTY(double OCPositionB READ OCPositionB WRITE setOCPositionB NOTIFY OCPositionBChanged)
    Q_PROPERTY(double OCPositionC READ OCPositionC WRITE setOCPositionC NOTIFY OCPositionCChanged)

double PickLensPositionX() const
{
    return m_PickLensPositionX;
}
double PickLensPositionY() const
{
    return m_PickLensPositionY;
}

double PickLensPositionZ() const
{
    return m_PickLensPositionZ;
}

double PickLensPositionA() const
{
    return m_PickLensPositionA;
}

double PickLensPositionB() const
{
    return m_PickLensPositionB;
}

double PickLensPositionC() const
{
    return m_PickLensPositionC;
}

double OCPositionX() const
{
    return m_OCPositionX;
}

double OCPositionY() const
{
    return m_OCPositionY;
}

double OCPositionZ() const
{
    return m_OCPositionZ;
}

double OCPositionA() const
{
    return m_OCPositionA;
}

double OCPositionB() const
{
    return m_OCPositionB;
}

double OCPositionC() const
{
    return m_OCPositionC;
}

public slots:
void setPickLensPositionX(double PickLensPositionX)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_PickLensPositionX, PickLensPositionX))
        return;

    m_PickLensPositionX = PickLensPositionX;
    emit PickLensPositionXChanged(m_PickLensPositionX);
}
void setPickLensPositionY(double PickLensPositionY)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_PickLensPositionY, PickLensPositionY))
        return;

    m_PickLensPositionY = PickLensPositionY;
    emit PickLensPositionYChanged(m_PickLensPositionY);
}

void setPickLensPositionZ(double PickLensPositionZ)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_PickLensPositionZ, PickLensPositionZ))
        return;

    m_PickLensPositionZ = PickLensPositionZ;
    emit PickLensPositionZChanged(m_PickLensPositionZ);
}

void setPickLensPositionA(double PickLensPositionA)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_PickLensPositionA, PickLensPositionA))
        return;

    m_PickLensPositionA = PickLensPositionA;
    emit PickLensPositionAChanged(m_PickLensPositionA);
}

void setPickLensPositionB(double PickLensPositionB)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_PickLensPositionB, PickLensPositionB))
        return;

    m_PickLensPositionB = PickLensPositionB;
    emit PickLensPositionBChanged(m_PickLensPositionB);
}

void setPickLensPositionC(double PickLensPositionC)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_PickLensPositionC, PickLensPositionC))
        return;

    m_PickLensPositionC = PickLensPositionC;
    emit PickLensPositionCChanged(m_PickLensPositionC);
}

void setOCPositionX(double OCPositionX)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_OCPositionX, OCPositionX))
        return;

    m_OCPositionX = OCPositionX;
    emit OCPositionXChanged(m_OCPositionX);
}

void setOCPositionY(double OCPositionY)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_OCPositionY, OCPositionY))
        return;

    m_OCPositionY = OCPositionY;
    emit OCPositionYChanged(m_OCPositionY);
}

void setOCPositionZ(double OCPositionZ)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_OCPositionZ, OCPositionZ))
        return;

    m_OCPositionZ = OCPositionZ;
    emit OCPositionZChanged(m_OCPositionZ);
}

void setOCPositionA(double OCPositionA)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_OCPositionA, OCPositionA))
        return;

    m_OCPositionA = OCPositionA;
    emit OCPositionAChanged(m_OCPositionA);
}

void setOCPositionB(double OCPositionB)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_OCPositionB, OCPositionB))
        return;

    m_OCPositionB = OCPositionB;
    emit OCPositionBChanged(m_OCPositionB);
}

void setOCPositionC(double OCPositionC)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_OCPositionC, OCPositionC))
        return;

    m_OCPositionC = OCPositionC;
    emit OCPositionCChanged(m_OCPositionC);
}

signals:
void PickLensPositionXChanged(double PickLensPositionX);
void PickLensPositionYChanged(double PickLensPositionY);

void PickLensPositionZChanged(double PickLensPositionZ);

void PickLensPositionAChanged(double PickLensPositionA);

void PickLensPositionBChanged(double PickLensPositionB);

void PickLensPositionCChanged(double PickLensPositionC);

void OCPositionXChanged(double OCPositionX);

void OCPositionYChanged(double OCPositionY);

void OCPositionZChanged(double OCPositionZ);

void OCPositionAChanged(double OCPositionA);

void OCPositionBChanged(double OCPositionB);

void OCPositionCChanged(double OCPositionC);

private:
double m_PickLensPositionX;
double m_PickLensPositionY;
double m_PickLensPositionZ;
double m_PickLensPositionA;
double m_PickLensPositionB;
double m_PickLensPositionC;
double m_OCPositionX;
double m_OCPositionY;
double m_OCPositionZ;
double m_OCPositionA;
double m_OCPositionB;
double m_OCPositionC;
};

#endif // AAHEADPARAMETERS_H
