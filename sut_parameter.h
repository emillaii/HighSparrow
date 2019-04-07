#ifndef SUT_PARAMETER_H
#define SUT_PARAMETER_H

#include "PropertyBase.h"

class SutParameter : public PropertyBase
{
public:
    explicit SutParameter():PropertyBase (){}

    Q_PROPERTY(double OCX READ OCX WRITE setOCX NOTIFY OCXChanged)
    Q_PROPERTY(double OCY READ OCY WRITE setOCY NOTIFY OCYChanged)
    Q_PROPERTY(double OCZ READ OCZ WRITE setOCZ NOTIFY OCZChanged)
    Q_PROPERTY(double LoadX READ LoadX WRITE setLoadX NOTIFY LoadXChanged)
    Q_PROPERTY(double LoadY READ LoadY WRITE setLoadY NOTIFY LoadYChanged)
    Q_PROPERTY(double LoadZ READ LoadZ WRITE setLoadZ NOTIFY LoadZChanged)
    Q_PROPERTY(double Lighting READ Lighting WRITE setLighting NOTIFY LightingChanged)
    Q_PROPERTY(double Force READ Force WRITE setForce NOTIFY ForceChanged)
    double OCX() const
    {
        return m_OCX;
    }
    double OCY() const
    {
        return m_OCY;
    }

    double OCZ() const
    {
        return m_OCZ;
    }

    double LoadX() const
    {
        return m_LoadX;
    }

    double LoadY() const
    {
        return m_LoadY;
    }

    double LoadZ() const
    {
        return m_LoadZ;
    }

    double Lighting() const
    {
        return m_Lighting;
    }

    double Force() const
    {
        return m_Force;
    }

public slots:
    void setOCX(double OCX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_OCX, OCX))
            return;

        m_OCX = OCX;
        emit OCXChanged(m_OCX);
    }
    void setOCY(double OCY)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_OCY, OCY))
            return;

        m_OCY = OCY;
        emit OCYChanged(m_OCY);
    }

    void setOCZ(double OCZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_OCZ, OCZ))
            return;

        m_OCZ = OCZ;
        emit OCZChanged(m_OCZ);
    }

    void setLoadX(double LoadX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_LoadX, LoadX))
            return;

        m_LoadX = LoadX;
        emit LoadXChanged(m_LoadX);
    }

    void setLoadY(double LoadY)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_LoadY, LoadY))
            return;

        m_LoadY = LoadY;
        emit LoadYChanged(m_LoadY);
    }

    void setLoadZ(double LoadZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_LoadZ, LoadZ))
            return;

        m_LoadZ = LoadZ;
        emit LoadZChanged(m_LoadZ);
    }

    void setLighting(double Lighting)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_Lighting, Lighting))
            return;

        m_Lighting = Lighting;
        emit LightingChanged(m_Lighting);
    }

    void setForce(double Force)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_Force, Force))
            return;

        m_Force = Force;
        emit ForceChanged(m_Force);
    }

signals:
    void OCXChanged(double OCX);
    void OCYChanged(double OCY);

    void OCZChanged(double OCZ);

    void LoadXChanged(double LoadX);

    void LoadYChanged(double LoadY);

    void LoadZChanged(double LoadZ);

    void LightingChanged(double Lighting);

    void ForceChanged(double Force);

private:
    double m_OCX = 0;
    double m_OCY = 0;
    double m_OCZ = 0;
    double m_LoadX = 0;
    double m_LoadY = 0;
    double m_LoadZ = 0;
    double m_Lighting = 0;
    double m_Force = 0;
};

#endif // SUT_PARAMETER_H
