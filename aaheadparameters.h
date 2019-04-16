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
    Q_PROPERTY(double PickLensPositionZ READ PickLensPositionZ WRITE setPickLensPositionZ NOTIFY paramsChanged)
    Q_PROPERTY(double OCPositionZ READ OCPositionZ WRITE setOCPositionZ NOTIFY paramsChanged)
    Q_PROPERTY(double rotateZOffset READ rotateZOffset WRITE setRotateZOffset NOTIFY rotateZOffsetChanged)

double PickLensPositionZ() const
{
    return m_PickLensPositionZ;
}

double OCPositionZ() const
{
    return m_OCPositionZ;
}

double rotateZOffset() const
{
    return m_rotateZOffset;
}

public slots:
void setPickLensPositionZ(double PickLensPositionZ)
{
    m_PickLensPositionZ = PickLensPositionZ;
    emit paramsChanged();
}

void setOCPositionZ(double OCPositionZ)
{
    m_OCPositionZ = OCPositionZ;
    emit paramsChanged();
}

void setRotateZOffset(double rotateZOffset)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_rotateZOffset, rotateZOffset))
        return;

    m_rotateZOffset = rotateZOffset;
    emit rotateZOffsetChanged(m_rotateZOffset);
}

signals:
void paramsChanged();
void rotateZOffsetChanged(double rotateZOffset);

private:
double m_PickLensPositionZ = 0;
double m_OCPositionZ = 0;
double m_rotateZOffset = 31.5;
};

#endif // AAHEADPARAMETERS_H
