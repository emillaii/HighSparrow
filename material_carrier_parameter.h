#ifndef MATERIAL_CARRIER_PARAMETER_H
#define MATERIAL_CARRIER_PARAMETER_H

#include "PropertyBase.h"



class MaterialCarrierParameter:public PropertyBase
{
    Q_OBJECT
public:
    MaterialCarrierParameter():PropertyBase (){}
    Q_PROPERTY(QString Name READ Name WRITE setName NOTIFY NameChanged)
    Q_PROPERTY(double SafetyZ READ SafetyZ WRITE setSafetyZ NOTIFY SafetyZChanged)
    Q_PROPERTY(double SafetyX READ SafetyX WRITE setSafetyX NOTIFY SafetyXChanged)
    Q_PROPERTY(double VisionX READ VisionX WRITE setVisionX NOTIFY VisionXChanged)
    Q_PROPERTY(double VisionY READ VisionY WRITE setVisionY NOTIFY VisionYChanged)
    Q_PROPERTY(double VisionZ READ VisionZ WRITE setVisionZ NOTIFY VisionZChanged)
    Q_PROPERTY(double CameraOffsetX READ CameraOffsetX WRITE setCameraOffsetX NOTIFY CameraOffsetXChanged)
    Q_PROPERTY(double CameraOffsetY READ CameraOffsetY WRITE setCameraOffsetY NOTIFY CameraOffsetYChanged)

    double SafetyZ() const
    {
        return m_SafetyZ;
    }

    double SafetyX() const
    {
        return m_SafetyX;
    }

    double VisionX() const
    {
        return m_VisionX;
    }

    double VisionY() const
    {
        return m_VisionY;
    }

    double VisionZ() const
    {
        return m_VisionZ;
    }

    double CameraOffsetX() const
    {
        return m_CameraOffsetX;
    }

    double CameraOffsetY() const
    {
        return m_CameraOffsetY;
    }

    QString Name() const
    {
        return m_Name;
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

    void setSafetyX(double SafetyX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_SafetyX, SafetyX))
            return;

        m_SafetyX = SafetyX;
        emit SafetyXChanged(m_SafetyX);
    }

    void setVisionX(double VisionX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_VisionX, VisionX))
            return;

        m_VisionX = VisionX;
        emit VisionXChanged(m_VisionX);
    }

    void setVisionY(double VisionY)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_VisionY, VisionY))
            return;

        m_VisionY = VisionY;
        emit VisionYChanged(m_VisionY);
    }

    void setVisionZ(double VisionZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_VisionZ, VisionZ))
            return;

        m_VisionZ = VisionZ;
        emit VisionZChanged(m_VisionZ);
    }

    void setCameraOffsetX(double CameraOffsetX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_CameraOffsetX, CameraOffsetX))
            return;

        m_CameraOffsetX = CameraOffsetX;
        emit CameraOffsetXChanged(m_CameraOffsetX);
    }

    void setCameraOffsetY(double CameraOffsetY)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_CameraOffsetY, CameraOffsetY))
            return;

        m_CameraOffsetY = CameraOffsetY;
        emit CameraOffsetYChanged(m_CameraOffsetY);
    }

    void setName(QString Name)
    {
        if (m_Name == Name)
            return;

        m_Name = Name;
        emit NameChanged(m_Name);
    }

signals:
    void SafetyZChanged(double SafetyZ);

    void SafetyXChanged(double SafetyX);

    void VisionXChanged(double VisionX);

    void VisionYChanged(double VisionY);

    void VisionZChanged(double VisionZ);

    void CameraOffsetXChanged(double CameraOffsetX);

    void CameraOffsetYChanged(double CameraOffsetY);

    void NameChanged(QString Name);

private:
    double m_SafetyZ = 0;
    double m_SafetyX = 0;
    double m_VisionX = 0;
    double m_VisionY = 0;
    double m_VisionZ = 0;
    double m_CameraOffsetX = 0;
    double m_CameraOffsetY = 0;
    QString m_Name = "";
};

#endif // MATERIAL_CARRIER_PARAMETER_H
