#ifndef MATERIAL_CARRIER_PARAMETER_H
#define MATERIAL_CARRIER_PARAMETER_H

#include "propertybase.h"

class MaterialCarrierParameter:public PropertyBase
{
    Q_OBJECT
    double m_SafetyZ;

    double m_SafetyY;

double m_SafetyX;

public:
    MaterialCarrierParameter():PropertyBase (){}
    Q_PROPERTY(double SafetyZ READ SafetyZ WRITE setSafetyZ NOTIFY paramsChanged)
    Q_PROPERTY(double SafetyY READ SafetyY WRITE setSafetyY NOTIFY paramsChanged)
    Q_PROPERTY(double SafetyX READ SafetyX WRITE setSafetyX NOTIFY paramsChanged)
    double SafetyZ() const
    {
        return m_SafetyZ;
    }
    double SafetyY() const
    {
        return m_SafetyY;
    }

    double SafetyX() const
    {
        return m_SafetyX;
    }

public slots:
    void setSafetyZ(double SafetyZ)
    {
        if (qFuzzyCompare(m_SafetyZ, SafetyZ))
            return;

        m_SafetyZ = SafetyZ;
        emit paramsChanged(m_SafetyZ);
    }
    void setSafetyY(double SafetyY)
    {
        if (qFuzzyCompare(m_SafetyY, SafetyY))
            return;

        m_SafetyY = SafetyY;
        emit paramsChanged(m_SafetyY);
    }

    void setSafetyX(double SafetyX)
    {
        if (qFuzzyCompare(m_SafetyX, SafetyX))
            return;

        m_SafetyX = SafetyX;
        emit paramsChanged(m_SafetyX);
    }

signals:
    void paramsChanged(double SafetyZ);
};

#endif // MATERIAL_CARRIER_PARAMETER_H
