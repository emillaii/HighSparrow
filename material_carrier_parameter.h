#ifndef MATERIAL_CARRIER_PARAMETER_H
#define MATERIAL_CARRIER_PARAMETER_H

#include "Utils/propertybase.h"

class MaterialCarrierParameter:public PropertyBase
{
    Q_OBJECT

public:
    MaterialCarrierParameter():PropertyBase (){}
    Q_PROPERTY(double SafetyZ READ SafetyZ WRITE setSafetyZ NOTIFY SafetyZChanged)
    Q_PROPERTY(double SafetyY READ SafetyY WRITE setSafetyY NOTIFY SafetyYChanged)
    Q_PROPERTY(double SafetyX READ SafetyX WRITE setSafetyX NOTIFY SafetyXChanged)

public:

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
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_SafetyZ, SafetyZ))
            return;

        m_SafetyZ = SafetyZ;
        emit SafetyZChanged(m_SafetyZ);
    }

    void setSafetyY(double SafetyY)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_SafetyY, SafetyY))
            return;

        m_SafetyY = SafetyY;
        emit SafetyYChanged(m_SafetyY);
    }

    void setSafetyX(double SafetyX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_SafetyX, SafetyX))
            return;

        m_SafetyX = SafetyX;
        emit SafetyXChanged(m_SafetyX);
    }

signals:
    void SafetyZChanged(double SafetyZ);

    void SafetyYChanged(double SafetyY);

    void SafetyXChanged(double SafetyX);

private:

    double m_SafetyZ = 0;
    double m_SafetyY = 0;
    double m_SafetyX = 0;
};

#endif // MATERIAL_CARRIER_PARAMETER_H
