#ifndef AAHEADPARAMETERS_H
#define AAHEADPARAMETERS_H

#include <QObject>
#include <propertybase.h>
#include "config.h"

class AAHeadParameters : public PropertyBase
{
    Q_OBJECT
public:
    explicit AAHeadParameters(){}
    Q_PROPERTY(double PickLensPositionX READ PickLensPositionX WRITE setPickLensPositionX NOTIFY paramsChanged)
    Q_PROPERTY(double PickLensPositionY READ PickLensPositionY WRITE setPickLensPositionY NOTIFY paramsChanged)
    Q_PROPERTY(double PickLensPositionZ READ PickLensPositionZ WRITE setPickLensPositionZ NOTIFY paramsChanged)
    Q_PROPERTY(double PickLensPositionA READ PickLensPositionA WRITE setPickLensPositionA NOTIFY paramsChanged)
    Q_PROPERTY(double PickLensPositionB READ PickLensPositionB WRITE setPickLensPositionB NOTIFY paramsChanged)
    Q_PROPERTY(double PickLensPositionC READ PickLensPositionC WRITE setPickLensPositionC NOTIFY paramsChanged)
    Q_PROPERTY(double OCPositionX READ OCPositionX WRITE setOCPositionX NOTIFY paramsChanged)
    Q_PROPERTY(double OCPositionY READ OCPositionY WRITE setOCPositionY NOTIFY paramsChanged)
    Q_PROPERTY(double OCPositionZ READ OCPositionZ WRITE setOCPositionZ NOTIFY paramsChanged)
    Q_PROPERTY(double OCPositionA READ OCPositionA WRITE setOCPositionA NOTIFY paramsChanged)
    Q_PROPERTY(double OCPositionB READ OCPositionB WRITE setOCPositionB NOTIFY paramsChanged)
    Q_PROPERTY(double OCPositionC READ OCPositionC WRITE setOCPositionC NOTIFY paramsChanged)
    Q_PROPERTY(QString UplookPRName READ UplookPRName WRITE setUplookPRName NOTIFY paramsChanged)
    Q_PROPERTY(double UplookPRLighting READ UplookPRLighting WRITE setUplookPRLighting NOTIFY paramsChanged)
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

QString UplookPRName() const
{
    return m_UplookPRName;
}

double UplookPRLighting() const
{
    return m_UplookPRLighting;
}

public slots:
void setPickLensPositionX(double PickLensPositionX)
{
    m_PickLensPositionX = PickLensPositionX;
    emit paramsChanged();
}
void setPickLensPositionY(double PickLensPositionY)
{
    m_PickLensPositionY = PickLensPositionY;
    emit paramsChanged();
}

void setPickLensPositionZ(double PickLensPositionZ)
{
    m_PickLensPositionZ = PickLensPositionZ;
    emit paramsChanged();
}

void setPickLensPositionA(double PickLensPositionA)
{
    m_PickLensPositionA = PickLensPositionA;
    emit paramsChanged();
}

void setPickLensPositionB(double PickLensPositionB)
{
    m_PickLensPositionB = PickLensPositionB;
    emit paramsChanged();
}

void setPickLensPositionC(double PickLensPositionC)
{
    m_PickLensPositionC = PickLensPositionC;
    emit paramsChanged();
}

void setOCPositionX(double OCPositionX)
{
    m_OCPositionX = OCPositionX;
    emit paramsChanged();
}

void setOCPositionY(double OCPositionY)
{
    m_OCPositionY = OCPositionY;
    emit paramsChanged();
}

void setOCPositionZ(double OCPositionZ)
{
    m_OCPositionZ = OCPositionZ;
    emit paramsChanged();
}

void setOCPositionA(double OCPositionA)
{
    m_OCPositionA = OCPositionA;
    emit paramsChanged();
}

void setOCPositionB(double OCPositionB)
{
    m_OCPositionB = OCPositionB;
    emit paramsChanged();
}

void setOCPositionC(double OCPositionC)
{
    m_OCPositionC = OCPositionC;
    emit paramsChanged();
}

void setUplookPRName(QString UplookPRName)
{
    if (m_UplookPRName == UplookPRName)
        return;

    m_UplookPRName = UplookPRName;
    emit paramsChanged();
}

void setUplookPRLighting(double UplookPRLighting)
{
    m_UplookPRLighting = UplookPRLighting;
    emit paramsChanged();
}

signals:
void paramsChanged();

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
QString m_UplookPRName;
double m_UplookPRLighting;
};

#endif // AAHEADPARAMETERS_H
