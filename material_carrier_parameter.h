#ifndef MATERIAL_CARRIER_PARAMETER_H
#define MATERIAL_CARRIER_PARAMETER_H

#include "propertybase.h"

class MaterialCarrierParameter:public PropertyBase
{
    Q_OBJECT
public:
    MaterialCarrierParameter():PropertyBase (){}
    Q_PROPERTY(QString Name READ Name WRITE setName NOTIFY paramsChanged)
    Q_PROPERTY(double SafetyZ READ SafetyZ WRITE setSafetyZ NOTIFY paramsChanged)
    Q_PROPERTY(double SafetyY READ SafetyY WRITE setSafetyY NOTIFY paramsChanged)
    Q_PROPERTY(double SafetyX READ SafetyX WRITE setSafetyX NOTIFY paramsChanged)
    Q_PROPERTY(double VisionX READ VisionX WRITE setVisionX NOTIFY paramsChanged)
    Q_PROPERTY(double VisionY READ VisionY WRITE setVisionY NOTIFY paramsChanged)
    Q_PROPERTY(double VisionZ READ VisionZ WRITE setVisionZ NOTIFY paramsChanged)
    Q_PROPERTY(double CameraOffsetX READ CameraOffsetX WRITE setCameraOffsetX NOTIFY paramsChanged)
    Q_PROPERTY(double CameraOffsetY READ CameraOffsetY WRITE setCameraOffsetY NOTIFY paramsChanged)

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

    double SafetyY() const
    {
        return m_SafetyY;
    }

public slots:

    void setSafetyZ(double SafetyZ)
    {
        m_SafetyZ = SafetyZ;
        emit paramsChanged();
    }

    void setSafetyX(double SafetyX)
    {
        m_SafetyX = SafetyX;
        emit paramsChanged();
    }

    void setVisionX(double VisionX)
    {
        m_VisionX = VisionX;
        emit paramsChanged();
    }

    void setVisionY(double VisionY)
    {
        m_VisionY = VisionY;
        emit paramsChanged();
    }

    void setVisionZ(double VisionZ)
    {
        m_VisionZ = VisionZ;
        emit paramsChanged();
    }

    void setCameraOffsetX(double CameraOffsetX)
    {
        m_CameraOffsetX = CameraOffsetX;
        emit paramsChanged();
    }

    void setCameraOffsetY(double CameraOffsetY)
    {
        m_CameraOffsetY = CameraOffsetY;   
        emit paramsChanged();
    }

    void setName(QString Name)
    {
        if (m_Name == Name)
            return;

        m_Name = Name;
        emit paramsChanged();
    }

    void setSafetyY(double SafetyY)
    {
        m_SafetyY = SafetyY;
        emit paramsChanged();
    }

signals:
    void paramsChanged();

private:
    double m_SafetyZ = 0;
    double m_SafetyX = 0;
    double m_VisionX = 0;
    double m_VisionY = 0;
    double m_VisionZ = 0;
    double m_CameraOffsetX = 0;
    double m_CameraOffsetY = 0;
    QString m_Name = "";
    double m_SafetyY = 0;
};

#endif // MATERIAL_CARRIER_PARAMETER_H
