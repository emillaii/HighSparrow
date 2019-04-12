#ifndef VISION_LOCATION_PARAMETER_H
#define VISION_LOCATION_PARAMETER_H

#include "propertybase.h"

class VisionLocationParameter:public PropertyBase
{
    Q_OBJECT
public:
    VisionLocationParameter():PropertyBase(){}
    Q_PROPERTY(QString prFileName READ prFileName WRITE setPrFileName NOTIFY prFileNameChanged)
    Q_PROPERTY(QString cameraName READ cameraName WRITE setCameraName NOTIFY cameraNameChanged)
    Q_PROPERTY(int lightChannel READ lightChannel WRITE setlightChannel NOTIFY lightChannelChanged)
    Q_PROPERTY(int lightBrightness READ lightBrightness WRITE setLightBrightness NOTIFY lightBrightnessChanged)

    QString prFileName() const
    {
        return m_prFileName;
    }

    QString cameraName() const
    {
        return m_cameraName;
    }

    int lightChannel() const
    {
        return m_lightChannel;
    }

    int lightBrightness() const
    {
        return m_lightBrightness;
    }

public slots:
    void setPrFileName(QString prFileName)
    {
        if (m_prFileName == prFileName)
            return;

        m_prFileName = prFileName;
        emit prFileNameChanged(m_prFileName);
    }

    void setCameraName(QString cameraName)
    {
        if (m_cameraName == cameraName)
            return;

        m_cameraName = cameraName;
        emit cameraNameChanged(m_cameraName);
    }

    void setlightChannel(int lightChannel)
    {
        if (m_lightChannel == lightChannel)
            return;

        m_lightChannel = lightChannel;
        emit lightChannelChanged(m_lightChannel);
    }

    void setLightBrightness(int lightBrightness)
    {
        if (m_lightBrightness == lightBrightness)
            return;

        m_lightBrightness = lightBrightness;
        emit lightBrightnessChanged(m_lightBrightness);
    }

signals:
    void prFileNameChanged(QString prFileName);

    void cameraNameChanged(QString cameraName);

    void lightChannelChanged(int lightChannel);

    void lightBrightnessChanged(uint8_t lightBrightness);

private:
    QString m_prFileName = "";
    QString m_cameraName = "";
    int m_lightChannel = 0;
    int m_lightBrightness = 0;
};

#endif // VISION_LOCATION_PARAMETER_H
