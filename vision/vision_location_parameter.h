#ifndef VISION_LOCATION_PARAMETER_H
#define VISION_LOCATION_PARAMETER_H

#include "propertybase.h"
#include "utils/configManager/configobject.h"

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
    Q_PROPERTY(double maximumOffset READ maximumOffset WRITE setMaximumOffset NOTIFY maximumOffsetChanged)
    Q_PROPERTY(QString calibrationName READ calibrationName WRITE setCalibrationName NOTIFY calibrationNameChanged)
    Q_PROPERTY(bool needCalibration READ needCalibration WRITE setNeedCalibration NOTIFY needCalibrationChanged)
    Q_PROPERTY(bool canMotion READ canMotion WRITE setCanMotion NOTIFY canMotionChanged)
    Q_PROPERTY(double waitImageDelay READ waitImageDelay WRITE setWaitImageDelay NOTIFY waitImageDelayChanged)
    Q_PROPERTY(bool enablePrTest READ enablePrTest WRITE setEnablePrTest NOTIFY enablePrTestChanged)
    Q_PROPERTY(int testIndex READ testIndex WRITE setTestIndex NOTIFY testIndexChanged)
    Q_PROPERTY(double objectScore READ objectScore WRITE setObjectScore NOTIFY objectScoreChanged)
    Q_PROPERTY(bool useOrigin READ useOrigin WRITE setUseOrigin NOTIFY useOriginChanged)
    Q_PROPERTY(int performTime READ performTime WRITE setPerformTime NOTIFY performTimeChanged)
    Q_PROPERTY(int prismPRType READ prismPRType WRITE setPrismPRType NOTIFY prismPRTypeChanged)
    Q_PROPERTY(int auxLightChannel READ auxLightChannel WRITE setAuxLightChannel NOTIFY auxLightChannelChanged)
    Q_PROPERTY(int auxLightBrightness READ auxLightBrightness WRITE setAuxLightBrightness NOTIFY auxLightBrightnessChanged)
    Q_PROPERTY(bool displaySmallHoleDetectionSetting READ displaySmallHoleDetectionSetting WRITE setDisplaySmallHoleDetectionSetting NOTIFY displaySmallHoleDetectionSettingChanged)
    Q_PROPERTY(bool enableSmallHoleDetection READ enableSmallHoleDetection WRITE setEnableSmallHoleDetection NOTIFY enableSmallHoleDetectionChanged)
    Q_PROPERTY(int smallCircleEdgeResponse READ smallCircleEdgeResponse WRITE setSmallCircleEdgeResponse NOTIFY smallCircleEdgeResponseChanged)
    Q_PROPERTY(int smallCircleScanWidth READ smallCircleScanWidth WRITE setSmallCircleScanWidth NOTIFY smallCircleScanWidthChanged)
    Q_PROPERTY(int smallCircleScanCount READ smallCircleScanCount WRITE setSmallCircleScanCount NOTIFY smallCircleScanCountChanged)
    Q_PROPERTY(double smallCircleRadiusMax READ smallCircleRadiusMax WRITE setSmallCircleRadiusMax NOTIFY smallCircleRadiusMaxChanged)
    Q_PROPERTY(double smallCircleRadiusMin READ smallCircleRadiusMin WRITE setSmallCircleRadiusMin NOTIFY smallCircleRadiusMinChanged)
    Q_PROPERTY(int retryCount READ retryCount WRITE setRetryCount NOTIFY retryCountChanged)
    Q_PROPERTY(bool closeLightAfterPR READ closeLightAfterPR WRITE setCloseLightAfterPR NOTIFY closeLightAfterPRChanged)

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

    bool enablePrTest() const
    {
        return m_enablePrTest;
    }

    int testIndex() const
    {
        return m_testIndex;
    }

    double maximumOffset() const
    {
        return m_maximumOffset;
    }

    double objectScore() const
    {
        return m_objectScore;
    }

    bool useOrigin() const
    {
        return m_useOrigin;
    }


    int performTime() const
    {
        return m_performTime;
    }

    int prismPRType() const
    {
        return m_prismPRType;
    }

    int auxLightChannel() const
    {
        return m_auxLightChannel;
    }

    int auxLightBrightness() const
    {
        return m_auxLightBrightness;
    }

    bool displaySmallHoleDetectionSetting() const
    {
        return m_displaySmallHoleDetectionSetting;
    }

    bool enableSmallHoleDetection() const
    {
        return m_enableSmallHoleDetection;
    }

    bool closeLightAfterPR() const
    {
        return m_closeLightAfterPR;
    }

    int smallCircleEdgeResponse() const
    {
        return m_smallCircleEdgeResponse;
    }

    int retryCount() const
    {
        return m_retryCount;
    }

    int smallCircleScanWidth() const
    {
        return m_smallCircleScanWidth;
    }

    int smallCircleScanCount() const
    {
        return m_smallCircleScanCount;
    }

    double smallCircleRadiusMax() const
    {
        return m_smallCircleRadiusMax;
    }

    double smallCircleRadiusMin() const
    {
        return m_smallCircleRadiusMin;
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
        if (qFuzzyCompare(m_maximunAngle, maximunAngle))
            return;

        m_maximunAngle = maximunAngle;
        emit maximunAngleChanged(m_maximunAngle);
    }

    void setMaximumLength(double maximumLength)
    {
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
        if (qFuzzyCompare(m_waitImageDelay, waitImageDelay))
            return;

        m_waitImageDelay = waitImageDelay;
        emit waitImageDelayChanged(m_waitImageDelay);
    }

    void setEnablePrTest(bool enablePrTest)
    {
        if (m_enablePrTest == enablePrTest)
            return;

        m_enablePrTest = enablePrTest;
        emit enablePrTestChanged(m_enablePrTest);
    }

    void setTestIndex(int testIndex)
    {
        if (m_testIndex == testIndex)
            return;

        m_testIndex = testIndex;
        emit testIndexChanged(m_testIndex);
    }

    void setMaximumOffset(double maximumOffset)
    {
        if (qFuzzyCompare(m_maximumOffset, maximumOffset))
            return;

        m_maximumOffset = maximumOffset;
        emit maximumOffsetChanged(m_maximumOffset);
    }

    void setObjectScore(double objectScore)
    {
        if (qFuzzyCompare(m_objectScore, objectScore))
            return;

        m_objectScore = objectScore;
        emit objectScoreChanged(m_objectScore);
    }

    void setUseOrigin(bool useOrigin)
    {
        if (m_useOrigin == useOrigin)
            return;

        m_useOrigin = useOrigin;
        emit useOriginChanged(m_useOrigin);
    }

    void setPerformTime(int performTime)
    {
        if (m_performTime == performTime)
            return;

        m_performTime = performTime;
        emit performTimeChanged(m_performTime);
    }

    void setPrismPRType(int prismPRType)
    {
        if (m_prismPRType == prismPRType)
            return;

        m_prismPRType = prismPRType;
        emit prismPRTypeChanged(m_prismPRType);
    }

    void setAuxLightChannel(int auxLightChannel)
    {
        if (m_auxLightChannel == auxLightChannel)
            return;

        m_auxLightChannel = auxLightChannel;
        emit auxLightChannelChanged(m_auxLightChannel);
    }

    void setAuxLightBrightness(int auxLightBrightness)
    {
        if (m_auxLightBrightness == auxLightBrightness)
            return;

        m_auxLightBrightness = auxLightBrightness;
        emit auxLightBrightnessChanged(m_auxLightBrightness);
    }

    void setDisplaySmallHoleDetectionSetting(bool displaySmallHoleDetectionSetting)
    {
        if (m_displaySmallHoleDetectionSetting == displaySmallHoleDetectionSetting)
            return;

        m_displaySmallHoleDetectionSetting = displaySmallHoleDetectionSetting;
        emit displaySmallHoleDetectionSettingChanged(m_displaySmallHoleDetectionSetting);
    }

    void setEnableSmallHoleDetection(bool enableSmallHoleDetection)
    {
        if (m_enableSmallHoleDetection == enableSmallHoleDetection)
            return;

        m_enableSmallHoleDetection = enableSmallHoleDetection;
        emit enableSmallHoleDetectionChanged(m_enableSmallHoleDetection);
    }

    void setCloseLightAfterPR(bool closeLightAfterPR)
    {
        if (m_closeLightAfterPR == closeLightAfterPR)
            return;

        m_closeLightAfterPR = closeLightAfterPR;
        emit closeLightAfterPRChanged(m_closeLightAfterPR);
    }

    void setSmallCircleEdgeResponse(int smallCircleEdgeResponse)
    {
        if (m_smallCircleEdgeResponse == smallCircleEdgeResponse)
            return;

        m_smallCircleEdgeResponse = smallCircleEdgeResponse;
        emit smallCircleEdgeResponseChanged(m_smallCircleEdgeResponse);
    }

    void setRetryCount(int retryCount)
    {
        if (m_retryCount == retryCount)
            return;

        m_retryCount = retryCount;
        emit retryCountChanged(m_retryCount);
    }

    void setSmallCircleScanWidth(int smallCircleScanWidth)
    {
        if (m_smallCircleScanWidth == smallCircleScanWidth)
            return;

        m_smallCircleScanWidth = smallCircleScanWidth;
        emit smallCircleScanWidthChanged(m_smallCircleScanWidth);
    }

    void setSmallCircleScanCount(int smallCircleScanCount)
    {
        if (m_smallCircleScanCount == smallCircleScanCount)
            return;

        m_smallCircleScanCount = smallCircleScanCount;
        emit smallCircleScanCountChanged(m_smallCircleScanCount);
    }

    void setSmallCircleRadiusMax(double smallCircleRadiusMax)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_smallCircleRadiusMax, smallCircleRadiusMax))
            return;

        m_smallCircleRadiusMax = smallCircleRadiusMax;
        emit smallCircleRadiusMaxChanged(m_smallCircleRadiusMax);
    }

    void setSmallCircleRadiusMin(double smallCircleRadiusMin)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_smallCircleRadiusMin, smallCircleRadiusMin))
            return;

        m_smallCircleRadiusMin = smallCircleRadiusMin;
        emit smallCircleRadiusMinChanged(m_smallCircleRadiusMin);
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

    void enablePrTestChanged(bool enablePrTest);

    void testIndexChanged(int testIndex);

    void maximumOffsetChanged(double maximumOffset);

    void objectScoreChanged(double objectScore);

    void useOriginChanged(bool useOrigin);

    void performTimeChanged(int performTime);

    void prismPRTypeChanged(int prismPRType);

    void auxLightChannelChanged(int auxLightChannel);

    void auxLightBrightnessChanged(int auxLightBrightness);

    void displaySmallHoleDetectionSettingChanged(bool displaySmallHoleDetectionSetting);

    void enableSmallHoleDetectionChanged(bool enableSmallHoleDetection);

    void closeLightAfterPRChanged(bool closeLightAfterPR);

    void smallCircleEdgeResponseChanged(int smallCircleEdgeResponse);

    void retryCountChanged(int retryCount);

    void smallCircleScanWidthChanged(int smallCircleScanWidth);

    void smallCircleScanCountChanged(int smallCircleScanCount);

    void smallCircleRadiusMaxChanged(double smallCircleRadiusMax);

    void smallCircleRadiusMinChanged(double smallCircleRadiusMin);

private:
    QString m_prFileName = "";
    QString m_cameraName = "";
    int m_lightChannel = 0;
    int m_lightBrightness = 0;
    double m_maximunAngle = 10;
    double m_maximumLength = 3;
    QString m_locationName = "Location";
    QString m_calibrationName = "Calibration";
    bool m_needCalibration = false;
    bool m_canMotion = true;
    double m_waitImageDelay = 100;
    bool m_enablePrTest = false;
    int m_testIndex = 1;
    double m_maximumOffset = 10;
    double m_objectScore = 0.7;
    bool m_useOrigin = false;
    int m_performTime = 0;
    int m_prismPRType = 0;
    int m_auxLightChannel = -1;
    int m_auxLightBrightness = 0;
    bool m_displaySmallHoleDetectionSetting = false;
    bool m_enableSmallHoleDetection = false;
    bool m_closeLightAfterPR = true;
    int m_smallCircleEdgeResponse = 10;
    int m_smallCircleScanWidth = 10;
    int m_smallCircleScanCount = 10;
    double m_smallCircleRadiusMax = 7;
    double m_smallCircleRadiusMin = 6;
    int m_retryCount = 3;
};


#endif // VISION_LOCATION_PARAMETER_H
