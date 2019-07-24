#ifndef AACOREPARAMETERS_H
#define AACOREPARAMETERS_H

#include <QObject>
#include <QVariant>
#include <propertybase.h>
#include "config.h"

class AACoreParameters : public PropertyBase
{
    Q_OBJECT
    double m_EFL = 1.98;

    int m_MaxIntensity = 50;

    int m_MinArea = 10000;

    int m_MaxArea = 90000;

    int m_SensorXRatio = 892;

    int m_SensorYRatio = 892;

    double m_ROIRatio = 1.45;

    bool m_firstRejectSensor = false;

    int m_rejectTimes = 3;

    QVariantList m_WeightList;

    double m_EstimatedFOVSlope = -15;

    double m_EstimatedAAFOV = 70;

    double m_minCircleTime = 1;

    double m_maxCicleTime = 100;

    int m_peakProfile = 0;

    QString m_moduleName = "AACoreNew";

    bool m_enableZpeakCoefficient = false;

    double m_zpeakccCoefficient = 1;

    double m_zpeak03Coefficient = 0;

    double m_zpeak05Coefficient = 0;

    double m_zpeak08Coefficient = 0;

    double m_maxDev = 100;

    double m_minDev = 0;

public:
    explicit AACoreParameters(){
        for (int i = 0; i < 4*4; i++) // 4 field of view * 4 edge number
        {
            m_WeightList.push_back(QVariant(0.25));
        }
    }
    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)
    Q_PROPERTY(double EFL READ EFL WRITE setEFL NOTIFY paramsChanged)
    Q_PROPERTY(int MaxIntensity READ MaxIntensity WRITE setMaxIntensity NOTIFY paramsChanged)
    Q_PROPERTY(int MinArea READ MinArea WRITE setMinArea NOTIFY paramsChanged)
    Q_PROPERTY(int MaxArea READ MaxArea WRITE setMaxArea NOTIFY paramsChanged)
    Q_PROPERTY(int SensorXRatio READ SensorXRatio WRITE setSensorXRatio NOTIFY paramsChanged)
    Q_PROPERTY(int SensorYRatio READ SensorYRatio WRITE setSensorYRatio NOTIFY paramsChanged)
    Q_PROPERTY(double ROIRatio READ ROIRatio WRITE setROIRatio NOTIFY paramsChanged)
    Q_PROPERTY(QVariantList WeightList READ WeightList WRITE setWeightList NOTIFY paramsChanged)
    Q_PROPERTY(bool firstRejectSensor READ firstRejectSensor WRITE setFirstRejectSensor NOTIFY firstRejectSensorChanged)
    Q_PROPERTY(int rejectTimes READ rejectTimes WRITE setRejectTimes NOTIFY rejectTimesChanged)
    Q_PROPERTY(double EstimatedFOVSlope READ EstimatedFOVSlope WRITE setEstimatedFOVSlope NOTIFY paramsChanged)
    Q_PROPERTY(double EstimatedAAFOV READ EstimatedAAFOV WRITE setEstimatedAAFOV NOTIFY paramsChanged)
    Q_PROPERTY(double minCircleTime READ minCircleTime WRITE setMinCircleTime NOTIFY minCircleTimeChanged)
    Q_PROPERTY(double maxCicleTime READ maxCicleTime WRITE setMaxCicleTime NOTIFY maxCicleTimeChanged)
    Q_PROPERTY(int peakProfile READ PeakProfile WRITE setPeakProfile NOTIFY paramsChanged)
    Q_PROPERTY(bool enableZpeakCoefficient READ enableZpeakCoefficient WRITE setEnableZpeakCoefficient NOTIFY enableZpeakCoefficientChanged)
    Q_PROPERTY(double zpeakccCoefficient READ zpeakccCoefficient WRITE setZpeakccCoefficient NOTIFY zpeakccCoefficientChanged)
    Q_PROPERTY(double zpeak03Coefficient READ zpeak03Coefficient WRITE setZpeak03Coefficient NOTIFY zpeak03CoefficientChanged)
    Q_PROPERTY(double zpeak05Coefficient READ zpeak05Coefficient WRITE setZpeak05Coefficient NOTIFY zpeak05CoefficientChanged)
    Q_PROPERTY(double zpeak08Coefficient READ zpeak08Coefficient WRITE setZpeak08Coefficient NOTIFY zpeak08CoefficientChanged)
    Q_PROPERTY(double maxDev READ maxDev WRITE setMaxDev NOTIFY maxDevChanged)
    Q_PROPERTY(double minDev READ minDev WRITE setMinDev NOTIFY minDevChanged)
    double EFL() const
    {
        return m_EFL;
    }
    int MaxIntensity() const
    {
        return m_MaxIntensity;
    }

    int MinArea() const
    {
        return m_MinArea;
    }

    int MaxArea() const
    {
        return m_MaxArea;
    }

    int SensorXRatio() const
    {
        return m_SensorXRatio;
    }

    int SensorYRatio() const
    {
        return m_SensorYRatio;
    }

    double ROIRatio() const
    {
        return m_ROIRatio;
    }

    bool firstRejectSensor() const
    {
        return m_firstRejectSensor;
    }

    int rejectTimes() const
    {
        return m_rejectTimes;
    }

    QVariantList WeightList() const
    {
        return m_WeightList;
    }

    double EstimatedFOVSlope() const
    {
        return m_EstimatedFOVSlope;
    }

    double EstimatedAAFOV() const
    {
        return m_EstimatedAAFOV;
    }

    double minCircleTime() const
    {
        return m_minCircleTime;
    }

    double maxCicleTime() const
    {
        return m_maxCicleTime;
    }

    int setPeakProfile() const
    {
        return m_peakProfile;
    }

    int PeakProfile() const
    {
        return m_peakProfile;
    }

    QString moduleName() const
    {
        return m_moduleName;
    }

    bool enableZpeakCoefficient() const
    {
        return m_enableZpeakCoefficient;
    }

    double zpeakccCoefficient() const
    {
        return m_zpeakccCoefficient;
    }

    double zpeak03Coefficient() const
    {
        return m_zpeak03Coefficient;
    }

    double zpeak05Coefficient() const
    {
        return m_zpeak05Coefficient;
    }

    double zpeak08Coefficient() const
    {
        return m_zpeak08Coefficient;
    }

    double maxDev() const
    {
        return m_maxDev;
    }

    double minDev() const
    {
        return m_minDev;
    }

public slots:
    void setEFL(double EFL)
    {
        m_EFL = EFL;
        emit paramsChanged();
    }
    void setMaxIntensity(int MaxIntensity)
    {
        m_MaxIntensity = MaxIntensity;
        emit paramsChanged();
    }

    void setMinArea(int MinArea)
    {
        m_MinArea = MinArea;
        emit paramsChanged();
    }

    void setMaxArea(int MaxArea)
    {
        m_MaxArea = MaxArea;
        emit paramsChanged();
    }

    void setSensorXRatio(int SensorXRatio)
    {
        m_SensorXRatio = SensorXRatio;
        emit paramsChanged();
    }

    void setSensorYRatio(int SensorYRatio)
    {
        m_SensorYRatio = SensorYRatio;
        emit paramsChanged();
    }

    void setROIRatio(double ROIRatio)
    {
        m_ROIRatio = ROIRatio;
        emit paramsChanged();
    }

    void setFirstRejectSensor(bool firstRejectSensor)
    {
        if (m_firstRejectSensor == firstRejectSensor)
            return;

        m_firstRejectSensor = firstRejectSensor;
        emit firstRejectSensorChanged(m_firstRejectSensor);
    }

    void setRejectTimes(int rejectTimes)
    {
        if (m_rejectTimes == rejectTimes)
            return;

        m_rejectTimes = rejectTimes;
        emit rejectTimesChanged(m_rejectTimes);
    }

    void setWeightValue(int index, double value)
    {
        qInfo("Set Weight Value: %d %f", index, value);
        m_WeightList[index].setValue(value);
        emit paramsChanged();
    }

    void setWeightList(QVariantList WeightList)
    {
        if (m_WeightList == WeightList)
            return;

        m_WeightList = WeightList;
        emit paramsChanged();
    }

    void setEstimatedFOVSlope(double EstimatedFOVSlope)
    {
        m_EstimatedFOVSlope = EstimatedFOVSlope;
        emit paramsChanged();
    }

    void setEstimatedAAFOV(double EstimatedAAFOV)
    {
        m_EstimatedAAFOV = EstimatedAAFOV;
        emit paramsChanged();
    }

    void setMinCircleTime(double minCircleTime)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_minCircleTime, minCircleTime))
            return;

        m_minCircleTime = minCircleTime;
        emit minCircleTimeChanged(m_minCircleTime);
    }

    void setMaxCicleTime(double maxCicleTime)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maxCicleTime, maxCicleTime))
            return;

        m_maxCicleTime = maxCicleTime;
        emit maxCicleTimeChanged(m_maxCicleTime);
    }

    void setPeakProfile(int peakProfile)
    {
        if (m_peakProfile == peakProfile)
            return;

        m_peakProfile = peakProfile;
        emit paramsChanged();
    }

    void setModuleName(QString moduleName)
    {
        if (m_moduleName == moduleName)
            return;

        m_moduleName = moduleName;
        emit moduleNameChanged(m_moduleName);
    }

    void setEnableZpeakCoefficient(bool enableZpeakCoefficient)
    {
        if (m_enableZpeakCoefficient == enableZpeakCoefficient)
            return;

        m_enableZpeakCoefficient = enableZpeakCoefficient;
        emit enableZpeakCoefficientChanged(m_enableZpeakCoefficient);
    }

    void setZpeakccCoefficient(double zpeakccCoefficient)
    {
        if(zpeakccCoefficient<0)return;
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_zpeakccCoefficient, zpeakccCoefficient))
            return;

        m_zpeakccCoefficient = zpeakccCoefficient;
        emit zpeakccCoefficientChanged(m_zpeakccCoefficient);
    }

    void setZpeak03Coefficient(double zpeak03Coefficient)
    {
        if(zpeak03Coefficient<0)return;
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_zpeak03Coefficient, zpeak03Coefficient))
            return;

        m_zpeak03Coefficient = zpeak03Coefficient;
        emit zpeak03CoefficientChanged(m_zpeak03Coefficient);
    }

    void setZpeak05Coefficient(double zpeak05Coefficient)
    {
        if(zpeak05Coefficient<0)return;
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_zpeak05Coefficient, zpeak05Coefficient))
            return;

        m_zpeak05Coefficient = zpeak05Coefficient;
        emit zpeak05CoefficientChanged(m_zpeak05Coefficient);
    }

    void setZpeak08Coefficient(double zpeak08Coefficient)
    {
        if(zpeak08Coefficient<0)return;
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_zpeak08Coefficient, zpeak08Coefficient))
            return;

        m_zpeak08Coefficient = zpeak08Coefficient;
        emit zpeak08CoefficientChanged(m_zpeak08Coefficient);
    }

    void setMaxDev(double maxDev)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maxDev, maxDev))
            return;

        m_maxDev = maxDev;
        emit maxDevChanged(m_maxDev);
    }

    void setMinDev(double minDev)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_minDev, minDev))
            return;

        m_minDev = minDev;
        emit minDevChanged(m_minDev);
    }

signals:
    void paramsChanged();
    void firstRejectSensorChanged(bool firstRejectSensor);
    void rejectTimesChanged(int rejectTimes);
    void minCircleTimeChanged(double minCircleTime);
    void maxCicleTimeChanged(double maxCicleTime);
    void moduleNameChanged(QString moduleName);
    void enableZpeakCoefficientChanged(bool enableZpeakCoefficient);
    void zpeakccCoefficientChanged(double zpeakccCoefficient);
    void zpeak03CoefficientChanged(double zpeak03Coefficient);
    void zpeak05CoefficientChanged(double zpeak05Coefficient);
    void zpeak08CoefficientChanged(double zpeak08Coefficient);
    void maxDevChanged(double maxDev);
    void minDevChanged(double minDev);
};
class AACoreStates: public PropertyBase
{
    Q_OBJECT
public:
    explicit AACoreStates(){}
    bool needSendLensRequest(){return(!hasLens())&&(!isWaitingLens());}
    bool needSendSensorRequest(){return (!hasSensor())&&(!isWaitingSensor());}

    Q_PROPERTY(bool isWaitingLens READ isWaitingLens WRITE setIsWaitingLens NOTIFY isWaitingLensChanged)
    Q_PROPERTY(bool isWaitingSensor READ isWaitingSensor WRITE setIsWaitingSensor NOTIFY isWaitingSensorChanged)
    Q_PROPERTY(bool hasLens READ hasLens WRITE setHasLens NOTIFY hasLensChanged)
    Q_PROPERTY(bool hasNgLens READ hasNgLens WRITE setHasNgLens NOTIFY hasNgLensChanged)
    Q_PROPERTY(bool hasSensor READ hasSensor WRITE setHasSensor NOTIFY hasSensorChanged)
    Q_PROPERTY(bool hasNgSensor READ hasNgSensor WRITE setHasNgSensor NOTIFY hasNgSensorChanged)
    Q_PROPERTY(bool hasProduct READ hasProduct WRITE setHasProduct NOTIFY hasProductChanged)
    Q_PROPERTY(bool hasNgProduct READ hasNgProduct WRITE setHasNgProduct NOTIFY hasNgProductChanged)
    Q_PROPERTY(double circleTime READ circleTime WRITE setCircleTime NOTIFY circleTimeChanged)
    Q_PROPERTY(double circleAverageTime READ circleAverageTime WRITE setCircleAverageTime NOTIFY circleAverageTimeChanged)
    Q_PROPERTY(int circleCount READ circleCount WRITE setCircleCount NOTIFY circleCountChanged)
    bool isWaitingLens() const
    {
        return m_isWaitingLens;
    }
    bool isWaitingSensor() const
    {
        return m_isWaitingSensor;
    }

    bool hasLens() const
    {
        return m_hasLens;
    }

    bool hasNgLens() const
    {
        return m_hasNgLens;
    }

    bool hasSensor() const
    {
        return m_hasSensor;
    }

    bool hasNgSensor() const
    {
        return m_hasNgSensor;
    }

    bool hasProduct() const
    {
        return m_hasProduct;
    }

    bool hasNgProduct() const
    {
        return m_hasNgProduct;
    }


    double circleTime() const
    {
        return m_circleTime;
    }

    int circleCount() const
    {
        return m_circleCount;
    }

    double circleAverageTime() const
    {
        return m_circleAverageTime;
    }

public slots:
    void setIsWaitingLens(bool isWaitingLens)
    {
        if (m_isWaitingLens == isWaitingLens)
            return;

        m_isWaitingLens = isWaitingLens;
        emit isWaitingLensChanged(m_isWaitingLens);
    }
    void setIsWaitingSensor(bool isWaitingSensor)
    {
        if (m_isWaitingSensor == isWaitingSensor)
            return;

        m_isWaitingSensor = isWaitingSensor;
        emit isWaitingSensorChanged(m_isWaitingSensor);
    }

    void setHasLens(bool hasLens)
    {
        if (m_hasLens == hasLens)
            return;

        m_hasLens = hasLens;
        emit hasLensChanged(m_hasLens);
    }

    void setHasNgLens(bool hasNgLens)
    {
        if (m_hasNgLens == hasNgLens)
            return;

        m_hasNgLens = hasNgLens;
        emit hasNgLensChanged(m_hasNgLens);
    }

    void setHasSensor(bool hasSensor)
    {
        if (m_hasSensor == hasSensor)
            return;

        m_hasSensor = hasSensor;
        emit hasSensorChanged(m_hasSensor);
    }

    void setHasNgSensor(bool hasNgSensor)
    {
        if (m_hasNgSensor == hasNgSensor)
            return;

        m_hasNgSensor = hasNgSensor;
        emit hasNgSensorChanged(m_hasNgSensor);
    }

    void setHasProduct(bool hasProduct)
    {
        if (m_hasProduct == hasProduct)
            return;

        m_hasProduct = hasProduct;
        emit hasProductChanged(m_hasProduct);
    }

    void setHasNgProduct(bool hasNgProduct)
    {
        if (m_hasNgProduct == hasNgProduct)
            return;

        m_hasNgProduct = hasNgProduct;
        emit hasNgProductChanged(m_hasNgProduct);
    }

    void setCircleTime(double circleTime)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_circleTime, circleTime))
            return;

        m_circleTime = circleTime;
        emit circleTimeChanged(m_circleTime);
    }

    void setCircleCount(int circleCount)
    {
        if (m_circleCount == circleCount)
            return;

        m_circleCount = circleCount;
        emit circleCountChanged(m_circleCount);
    }

    void setCircleAverageTime(double circleAverageTime)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_circleAverageTime, circleAverageTime))
            return;

        m_circleAverageTime = circleAverageTime;
        emit circleAverageTimeChanged(m_circleAverageTime);
    }

signals:
    void isWaitingLensChanged(bool isWaitingLens);
    void isWaitingSensorChanged(bool isWaitingSensor);

    void hasLensChanged(bool hasLens);

    void hasNgLensChanged(bool hasNgLens);

    void hasSensorChanged(bool hasSensor);

    void hasNgSensorChanged(bool hasNgSensor);

    void hasProductChanged(bool hasProduct);

    void hasNgProductChanged(bool hasNgProduct);

    void circleTimeChanged(double circleTime);

    void circleCountChanged(int circleCount);

    void circleAverageTimeChanged(double circleAverageTime);

private:
    bool m_isWaitingLens = false;
    bool m_isWaitingSensor = false;
    bool m_hasLens = false;
    bool m_hasNgLens = false;
    bool m_hasSensor = false;
    bool m_hasNgSensor = false;
    bool m_hasProduct = false;
    bool m_hasNgProduct = false;
    double m_circleTime = 0;
    int m_circleCount = 0;
    double m_circleAverageTime = 0;
};

#endif // AACOREPARAMETERS_H
