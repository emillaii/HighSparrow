#ifndef VISION_LOCATION_PARAMETER_H
#define VISION_LOCATION_PARAMETER_H

#include "Utils/propertybase.h"

class VisionLocationParameter:public PropertyBase
{
    Q_OBJECT
public:
    VisionLocationParameter():PropertyBase(){}
    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY locationNameChanged)
    Q_PROPERTY(QString prFileName READ prFileName WRITE setPrFileName NOTIFY prFileNameChanged)
    Q_PROPERTY(QString cameraName READ cameraName WRITE setCameraName NOTIFY cameraNameChanged)
    Q_PROPERTY(int lightChannel READ lightChannel WRITE setlightChannel NOTIFY lightChannelChanged)
    Q_PROPERTY(int lightBrightness READ lightBrightness WRITE setLightBrightness NOTIFY lightBrightnessChanged)
    Q_PROPERTY(double maximunAngle READ maximunAngle WRITE setMaximunAngle NOTIFY maximunAngleChanged)
    Q_PROPERTY(double maximumLength READ maximumLength WRITE setMaximumLength NOTIFY maximumLengthChanged)
    Q_PROPERTY(QString calibrationName READ calibrationName WRITE setCalibrationName NOTIFY calibrationNameChanged)
    Q_PROPERTY(bool needCalibration READ needCalibration WRITE setNeedCalibration NOTIFY needCalibrationChanged)
    Q_PROPERTY(bool canMotion READ canMotion WRITE setCanMotion NOTIFY canMotionChanged)
    Q_PROPERTY(double waitImageDelay READ waitImageDelay WRITE setWaitImageDelay NOTIFY waitImageDelayChanged)

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

    double maximunAngle() const
    {
        return m_maximunAngle;
    }

    double maximumLength() const
    {
        return m_maximumLength;
    }

    QString locationName() const
    {
        return m_locationName;
    }

    QString calibrationName() const
    {
        return m_calibrationName;
    }

    bool needCalibration() const
    {
        return m_needCalibration;
    }

    bool canMotion() const
    {
        return m_canMotion;
    }

    double waitImageDelay() const
    {
        return m_waitImageDelay;
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

    void setMaximunAngle(double maximunAngle)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maximunAngle, maximunAngle))
            return;

        m_maximunAngle = maximunAngle;
        emit maximunAngleChanged(m_maximunAngle);
    }

    void setMaximumLength(double maximumLength)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maximumLength, maximumLength))
            return;

        m_maximumLength = maximumLength;
        emit maximumLengthChanged(m_maximumLength);
    }

    void setLocationName(QString lcationName)
    {
        if (m_locationName == lcationName)
            return;

        m_locationName = lcationName;
        emit locationNameChanged(m_locationName);
    }

    void setCalibrationName(QString calibrationName)
    {
        if (m_calibrationName == calibrationName)
            return;

        m_calibrationName = calibrationName;
        emit calibrationNameChanged(m_calibrationName);
    }

    void setNeedCalibration(bool needCalibration)
    {
        if (m_needCalibration == needCalibration)
            return;

        m_needCalibration = needCalibration;
        emit needCalibrationChanged(m_needCalibration);
    }

    void setCanMotion(bool canMotion)
    {
        if (m_canMotion == canMotion)
            return;

        m_canMotion = canMotion;
        emit canMotionChanged(m_canMotion);
    }

    void setWaitImageDelay(double waitImageDelay)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_waitImageDelay, waitImageDelay))
            return;

        m_waitImageDelay = waitImageDelay;
        emit waitImageDelayChanged(m_waitImageDelay);
    }

signals:
    void prFileNameChanged(QString prFileName);

    void cameraNameChanged(QString cameraName);

    void lightChannelChanged(int lightChannel);

    void lightBrightnessChanged(uint8_t lightBrightness);

    void maximunAngleChanged(double maximunAngle);

    void maximumLengthChanged(double maximumLength);

    void locationNameChanged(QString locationName);

    void calibrationNameChanged(QString calibrationName);

    void needCalibrationChanged(bool needCalibration);

    void canMotionChanged(bool canMotion);

    void waitImageDelayChanged(double waitImageDelay);

private:
    QString m_prFileName = "";
    QString m_cameraName = "";
    int m_lightChannel = 0;
    int m_lightBrightness = 0;
    double m_maximunAngle = 15;
    double m_maximumLength = 3;
    QString m_locationName = "Location";
    QString m_calibrationName = "Calibration";
    bool m_needCalibration = false;
    bool m_canMotion = true;
    double m_waitImageDelay = 100;
};

#endif // VISION_LOCATION_PARAMETER_H
