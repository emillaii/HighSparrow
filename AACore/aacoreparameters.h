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

    double m_zpeakL1Coefficient = 0;

    double m_zpeakL2Coefficient = 0;

    double m_zpeakL3Coefficient = 0;

    double m_maxDev = 100;

    double m_minDev = -100;

    bool m_enableCheckDispense = false;

    int m_checkDispenseCount = 10;

    int m_minIntensityDiff = 10;

    double m_zPeakDiffL1Max = 3;

    double m_zPeakDiffL2Max = 3;

    double m_zPeakDiffL3Max = 5;

    double m_CCL1MinDev = -100;

    double m_CCL1MaxDev = 100;

    double m_CCL2MinDev = -100;

    double m_CCL2MaxDev = 100;

    double m_CCL3MinDev = -100;

    double m_CCL3MaxDev = 100;

    bool m_isDebug = false;

    int m_taskMode = 0;

    int m_taskNumberFSJ = 10;

    int m_taskNumberSSJ = 20;

    int m_taskNumber = 100;

    bool m_enableLensVcm = false;

    QString m_lensVcmPath ="";

    double m_lensVcmWorkPosition = 140;

    bool m_taskAccumulate = false;

    int m_currentTask = 0;

    int m_tiltRelationship = 0;

    int m_SensorOrientation = 0;

    QString m_aaCoreRunningTest;

    int m_dispenseCount = 0;

    int m_dispenseCountLimit = 3000;

    double m_circleTime = 0;

    double m_circleAverageTime = 0;

    int m_circleCount = 0;

    int m_calculatedUPH = 0;

    int m_currentUPH = 0;

    int m_mtfFrequency = 1;

    int m_aaScanMTFFrequency = 1;

    int m_aaScanCurveFitOrder = 4;

    int m_aaScanOversampling = 0;

    int m_aaScanCurveFitErrorThreshold = -10;

    QString m_lotNumber = "Silicool";

    int m_dynamicTiltUpdateIndex = 0;

    int m_vcmInitMode = 0;

    QString m_vcmSlaveId = "0x18";

    QString m_vcmRegAddress = "0x03";

public:
    explicit AACoreParameters(){
        for (int i = 0; i < 4*5; i++) // 4 field of view * 4 edge number
        {
            m_WeightList.push_back(QVariant(0.25));
        }
    }
    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)
    Q_PROPERTY(int taskMode READ taskMode WRITE setTaskMode NOTIFY taskModeChanged)
    Q_PROPERTY(int taskNumberFSJ READ taskNumberFSJ WRITE setTaskNumberFSJ NOTIFY taskNumberFSJChanged)
    Q_PROPERTY(int taskNumberSSJ READ taskNumberSSJ WRITE setTaskNumberSSJ NOTIFY taskNumberSSJChanged)
    Q_PROPERTY(int taskNumber READ taskNumber WRITE setTaskNumber NOTIFY taskNumberChanged)
    Q_PROPERTY(bool taskAccumulate READ taskAccumulate WRITE setTaskAccumulate NOTIFY taskAccumulateChanged)
    Q_PROPERTY(int currentTask READ currentTask WRITE setCurrentTask NOTIFY currentTaskChanged)
    Q_PROPERTY(double EFL READ EFL WRITE setEFL NOTIFY paramsChanged)
    Q_PROPERTY(int MaxIntensity READ MaxIntensity WRITE setMaxIntensity NOTIFY paramsChanged)
    Q_PROPERTY(int MinArea READ MinArea WRITE setMinArea NOTIFY paramsChanged)
    Q_PROPERTY(int MaxArea READ MaxArea WRITE setMaxArea NOTIFY paramsChanged)
    Q_PROPERTY(int SensorXRatio READ SensorXRatio WRITE setSensorXRatio NOTIFY paramsChanged)
    Q_PROPERTY(int SensorYRatio READ SensorYRatio WRITE setSensorYRatio NOTIFY paramsChanged)
    Q_PROPERTY(int SensorOrientation READ SensorOrientation WRITE setSensorOrientation NOTIFY paramsChanged)
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
    Q_PROPERTY(double zpeakL1Coefficient READ zpeakL1Coefficient WRITE setZpeakL1Coefficient NOTIFY zpeakL1CoefficientChanged)
    Q_PROPERTY(double zpeakL2Coefficient READ zpeakL2Coefficient WRITE setZpeakL2Coefficient NOTIFY zpeakL2CoefficientChanged)
    Q_PROPERTY(double zpeakL3Coefficient READ zpeakL3Coefficient WRITE setZpeakL3Coefficient NOTIFY zpeakL3CoefficientChanged)
    Q_PROPERTY(double maxDev READ maxDev WRITE setMaxDev NOTIFY maxDevChanged)
    Q_PROPERTY(double minDev READ minDev WRITE setMinDev NOTIFY minDevChanged)
    Q_PROPERTY(bool enableCheckDispense READ enableCheckDispense WRITE setEnableCheckDispense NOTIFY enableCheckDispenseChanged)
    Q_PROPERTY(int checkDispenseCount READ checkDispenseCount WRITE setCheckDispenseCount NOTIFY checkDispenseCountChanged)
    Q_PROPERTY(int minIntensityDiff READ minIntensityDiff WRITE setMinIntensityDiff NOTIFY minIntensityDiffChanged)
    Q_PROPERTY(double zPeakDiffL1Max READ zPeakDiffL1Max WRITE setZPeakDiffL1Max NOTIFY zPeakDiffL1MaxChanged)
    Q_PROPERTY(double zPeakDiffL2Max READ zPeakDiffL2Max WRITE setZPeakDiffL2Max NOTIFY zPeakDiffL2MaxChanged)
    Q_PROPERTY(double zPeakDiffL3Max READ zPeakDiffL3Max WRITE setZPeakDiffL3Max NOTIFY zPeakDiffL3MaxChanged)
    Q_PROPERTY(double CCL1MinDev READ CCL1MinDev WRITE setCCL1MinDev NOTIFY CCL1MinDevChanged)
    Q_PROPERTY(double CCL1MaxDev READ CCL1MaxDev WRITE setCCL1MaxDev NOTIFY CCL1MaxDevChanged)
    Q_PROPERTY(double CCL2MinDev READ CCL2MinDev WRITE setCCL2MinDev NOTIFY CCL2MinDevChanged)
    Q_PROPERTY(double CCL2MaxDev READ CCL2MaxDev WRITE setCCL2MaxDev NOTIFY CCL2MaxDevChanged)
    Q_PROPERTY(double CCL3MinDev READ CCL3MinDev WRITE setCCL3MinDev NOTIFY CCL3MinDevChanged)
    Q_PROPERTY(double CCL3MaxDev READ CCL3MaxDev WRITE setCCL3MaxDev NOTIFY CCL3MaxDevChanged)
    Q_PROPERTY(bool isDebug READ isDebug WRITE setIsDebug NOTIFY isDebugChanged)
    Q_PROPERTY(bool enableLensVcm READ enableLensVcm WRITE setEnableLensVcm NOTIFY enableLensVcmChanged)
    Q_PROPERTY(QString lensVcmPath READ lensVcmPath WRITE setLensVcmPath NOTIFY lensVcmPathChanged)
    Q_PROPERTY(double lensVcmWorkPosition READ lensVcmWorkPosition WRITE setLensVcmWorkPosition NOTIFY lensVcmWorkPositionChanged)
    Q_PROPERTY(int tiltRelationship READ tiltRelationship WRITE setTiltRelationship NOTIFY tiltRelationshipChanged)
    Q_PROPERTY(QString aaCoreRunningTest READ aaCoreRunningTest WRITE setAACoreRunningTest NOTIFY aaCoreRunningTestChanged)
    Q_PROPERTY(int dispenseCount READ dispenseCount WRITE setDispenseCount NOTIFY dispenseCountChanged)
    Q_PROPERTY(int dispenseCountLimit READ dispenseCountLimit WRITE setDispenseCountLimit NOTIFY dispenseCountLimitChanged)
    Q_PROPERTY(double circleTime READ circleTime WRITE setCircleTime NOTIFY circleTimeChanged)
    Q_PROPERTY(double circleAverageTime READ circleAverageTime WRITE setCircleAverageTime NOTIFY circleAverageTimeChanged)
    Q_PROPERTY(int circleCount READ circleCount WRITE setCircleCount NOTIFY circleCountChanged)
    Q_PROPERTY(int calculatedUPH READ calculatedUPH WRITE setCalculatedUPH NOTIFY calculatedUPHChanged)
    Q_PROPERTY(int currentUPH READ currentUPH WRITE setCurrentUPH NOTIFY currentUPHChanged)
    Q_PROPERTY(int mtfFrequency READ mtfFrequency WRITE setMTFFrequency NOTIFY mtfFrequencyChanged)
    Q_PROPERTY(int aaScanMTFFrequency READ aaScanMTFFrequency WRITE setAAScanMTFFrequency NOTIFY aaScanMTFFrequencyChanged)
    Q_PROPERTY(int aaScanCurveFitOrder READ aaScanCurveFitOrder WRITE setAAScanCurveFitOrder NOTIFY aaScanCurveFitOrderChanged)
    Q_PROPERTY(int aaScanOversampling READ aaScanOversampling WRITE setAAScanOversampling NOTIFY aaScanOversamplingChanged)
    Q_PROPERTY(int aaScanCurveFitErrorThreshold READ aaScanCurveFitErrorThreshold WRITE setAAScanCurveFitErrorThreshold NOTIFY aaScanCurveFitErrorThresholdChanged)
    Q_PROPERTY(QString lotNumber READ lotNumber WRITE setLotNumber NOTIFY LotNumberChanged)
    Q_PROPERTY(int dynamicTiltUpdateIndex READ dynamicTiltUpdateIndex WRITE setDynamicTiltUpdateIndex NOTIFY dynamicTiltUpdateIndexChanged)
    Q_PROPERTY(int vcmInitMode READ vcmInitMode WRITE setVCMInitMode NOTIFY vcmInitModeChanged)
    Q_PROPERTY(QString vcmSlaveId READ vcmSlaveId WRITE setVCMSlaveId NOTIFY vcmSlaveIdChanged)
    Q_PROPERTY(QString vcmRegAddress READ vcmRegAddress WRITE setVCMRegAddress NOTIFY vcmRegAddressChanged)

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

    double zpeakL1Coefficient() const
    {
        return m_zpeakL1Coefficient;
    }

    double zpeakL2Coefficient() const
    {
        return m_zpeakL2Coefficient;
    }

    double zpeakL3Coefficient() const
    {
        return m_zpeakL3Coefficient;
    }

    double maxDev() const
    {
        return m_maxDev;
    }

    double minDev() const
    {
        return m_minDev;
    }

    bool enableCheckDispense() const
    {
        return m_enableCheckDispense;
    }

    int checkDispenseCount() const
    {
        return m_checkDispenseCount;
    }

    int minIntensityDiff() const
    {
        return m_minIntensityDiff;
    }

    double zPeakDiffL1Max() const
    {
        return m_zPeakDiffL1Max;
    }

    double zPeakDiffL2Max() const
    {
        return m_zPeakDiffL2Max;
    }

    double zPeakDiffL3Max() const
    {
        return m_zPeakDiffL3Max;
    }

    double CCL1MinDev() const
    {
        return m_CCL1MinDev;
    }

    double CCL1MaxDev() const
    {
        return m_CCL1MaxDev;
    }

    double CCL2MinDev() const
    {
        return m_CCL2MinDev;
    }

    double CCL2MaxDev() const
    {
        return m_CCL2MaxDev;
    }

    double CCL3MinDev() const
    {
        return m_CCL3MinDev;
    }

    double CCL3MaxDev() const
    {
        return m_CCL3MaxDev;
    }

    bool isDebug() const
    {
        return m_isDebug;
    }

    int taskMode() const
    {
        return m_taskMode;
    }

    int taskNumberFSJ() const
    {
        return m_taskNumberFSJ;
    }

    int taskNumberSSJ() const
    {
        return m_taskNumberSSJ;
    }

    int taskNumber() const
    {
        return m_taskNumber;
    }

    bool enableLensVcm() const
    {
        return m_enableLensVcm;
    }

    QString lensVcmPath() const
    {
        return m_lensVcmPath;
    }

    double lensVcmWorkPosition() const
    {
        return m_lensVcmWorkPosition;
    }

    bool taskAccumulate() const
    {
        return m_taskAccumulate;
    }

    int currentTask() const
    {
        return m_currentTask;
    }

    int tiltRelationship() const
    {
        return m_tiltRelationship;
    }

    int SensorOrientation() const
    {
        return m_SensorOrientation;
    }

    QString aaCoreRunningTest() const
    {
        return m_aaCoreRunningTest;
    }

    int dispenseCount() const
    {
        return m_dispenseCount;
    }

    int dispenseCountLimit() const
    {
        return m_dispenseCountLimit;
    }

    double circleTime() const
    {
        return m_circleTime;
    }

    double circleAverageTime() const
    {
        return m_circleAverageTime;
    }

    int circleCount() const
    {
        return m_circleCount;
    }

    int calculatedUPH() const
    {
        return m_calculatedUPH;
    }

    int mtfFrequency() const
    {
        return m_mtfFrequency;
    }

    int aaScanMTFFrequency() const
    {
        return m_aaScanMTFFrequency;
    }

    int aaScanCurveFitOrder() const
    {
        return m_aaScanCurveFitOrder;
    }

    int aaScanOversampling() const
    {
        return m_aaScanOversampling;
    }

    int aaScanCurveFitErrorThreshold() const
    {
        return m_aaScanCurveFitErrorThreshold;
    }

    QString lotNumber() const
    {
        return m_lotNumber;
    }

    int dynamicTiltUpdateIndex() const
    {
        return m_dynamicTiltUpdateIndex;
    }

    int currentUPH() const
    {
        return m_currentUPH;
    }

    int vcmInitMode() const
    {
        return m_vcmInitMode;
    }

    QString vcmSlaveId() const
    {
        return m_vcmSlaveId;
    }

    QString vcmRegAddress() const
    {
        return m_vcmRegAddress;
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
        qInfo("Set Weight Value: %d %f m_WeightList size: %d", index, value, m_WeightList.size());
        if (m_WeightList.size() - 1 < index) {
            for(int i = 0; i < index - (m_WeightList.size() - 1); i++) {
                m_WeightList.append(value);
            }
        }  else {
           m_WeightList[index].setValue(value);
           emit paramsChanged();
        }
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

        if (qFuzzyCompare(m_minCircleTime, minCircleTime))
            return;

        m_minCircleTime = minCircleTime;
        emit minCircleTimeChanged(m_minCircleTime);
    }

    void setMaxCicleTime(double maxCicleTime)
    {

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

        if (qFuzzyCompare(m_zpeakccCoefficient, zpeakccCoefficient))
            return;

        m_zpeakccCoefficient = zpeakccCoefficient;
        emit zpeakccCoefficientChanged(m_zpeakccCoefficient);
    }

    void setZpeakL1Coefficient(double zpeakL1Coefficient)
    {
        if(zpeakL1Coefficient<0)return;
        if (qFuzzyCompare(m_zpeakL1Coefficient, zpeakL1Coefficient))
            return;

        m_zpeakL1Coefficient = zpeakL1Coefficient;
        emit zpeakL1CoefficientChanged(m_zpeakL1Coefficient);
    }

    void setZpeakL2Coefficient(double zpeakL2Coefficient)
    {
        if(zpeakL2Coefficient<0)return;
        if (qFuzzyCompare(m_zpeakL2Coefficient, zpeakL2Coefficient))
            return;

        m_zpeakL2Coefficient = zpeakL2Coefficient;
        emit zpeakL2CoefficientChanged(m_zpeakL2Coefficient);
    }

    void setZpeakL3Coefficient(double zpeakL3Coefficient)
    {
        if(zpeakL3Coefficient<0)return;
        if (qFuzzyCompare(m_zpeakL3Coefficient, zpeakL3Coefficient))
            return;

        m_zpeakL3Coefficient = zpeakL3Coefficient;
        emit zpeakL3CoefficientChanged(m_zpeakL3Coefficient);
    }

    void setMaxDev(double maxDev)
    {

        if (qFuzzyCompare(m_maxDev, maxDev))
            return;

        m_maxDev = maxDev;
        emit maxDevChanged(m_maxDev);
    }

    void setMinDev(double minDev)
    {

        if (qFuzzyCompare(m_minDev, minDev))
            return;

        m_minDev = minDev;
        emit minDevChanged(m_minDev);
    }

    void setEnableCheckDispense(bool enableCheckDispense)
    {
        if (m_enableCheckDispense == enableCheckDispense)
            return;

        m_enableCheckDispense = enableCheckDispense;
        emit enableCheckDispenseChanged(m_enableCheckDispense);
    }

    void setCheckDispenseCount(int checkDispenseCount)
    {
        if (m_checkDispenseCount == checkDispenseCount)
            return;

        m_checkDispenseCount = checkDispenseCount;
        emit checkDispenseCountChanged();
    }

    void setMinIntensityDiff(int minIntensityDiff)
    {
        if (m_minIntensityDiff == minIntensityDiff)
            return;

        m_minIntensityDiff = minIntensityDiff;
        emit minIntensityDiffChanged(m_minIntensityDiff);
    }

    void setZPeakDiffL1Max(double zPeakDiffL1Max)
    {
        if (qFuzzyCompare(m_zPeakDiffL1Max, zPeakDiffL1Max))
            return;

        m_zPeakDiffL1Max = zPeakDiffL1Max;
        emit zPeakDiffL1MaxChanged(m_zPeakDiffL1Max);
    }

    void setZPeakDiffL2Max(double zPeakDiffL2Max)
    {
        if (qFuzzyCompare(m_zPeakDiffL2Max, zPeakDiffL2Max))
            return;

        m_zPeakDiffL2Max = zPeakDiffL2Max;
        emit zPeakDiffL2MaxChanged(m_zPeakDiffL2Max);
    }

    void setZPeakDiffL3Max(double zPeakDiffL3Max)
    {
        if (qFuzzyCompare(m_zPeakDiffL3Max, zPeakDiffL3Max))
            return;

        m_zPeakDiffL3Max = zPeakDiffL3Max;
        emit zPeakDiffL3MaxChanged(m_zPeakDiffL3Max);
    }

    void setCCL1MinDev(double CCL1MinDev)
    {
        if (qFuzzyCompare(m_CCL1MinDev, CCL1MinDev))
            return;

        m_CCL1MinDev = CCL1MinDev;
        emit CCL1MinDevChanged(m_CCL1MinDev);
    }

    void setCCL1MaxDev(double CCL1MaxDev)
    {
        if (qFuzzyCompare(m_CCL1MaxDev, CCL1MaxDev))
            return;

        m_CCL1MaxDev = CCL1MaxDev;
        emit CCL1MaxDevChanged(m_CCL1MaxDev);
    }

    void setCCL2MinDev(double CCL2MinDev)
    {
        if (qFuzzyCompare(m_CCL2MinDev, CCL2MinDev))
            return;

        m_CCL2MinDev = CCL2MinDev;
        emit CCL2MinDevChanged(m_CCL2MinDev);
    }

    void setCCL2MaxDev(double CCL2MaxDev)
    {
        if (qFuzzyCompare(m_CCL2MaxDev, CCL2MaxDev))
            return;

        m_CCL2MaxDev = CCL2MaxDev;
        emit CCL2MaxDevChanged(m_CCL2MaxDev);
    }

    void setCCL3MinDev(double CCL3MinDev)
    {
        if (qFuzzyCompare(m_CCL3MinDev, CCL3MinDev))
            return;

        m_CCL3MinDev = CCL3MinDev;
        emit CCL3MinDevChanged(m_CCL3MinDev);
    }

    void setCCL3MaxDev(double CCL3MaxDev)
    {
        if (qFuzzyCompare(m_CCL3MaxDev, CCL3MaxDev))
            return;

        m_CCL3MaxDev = CCL3MaxDev;
        emit CCL3MaxDevChanged(m_CCL3MaxDev);
    }

    void setIsDebug(bool isDebug)
    {
        if (m_isDebug == isDebug)
            return;

        m_isDebug = isDebug;
        emit isDebugChanged(m_isDebug);
    }

    void setTaskMode(int taskMode)
    {
        if (m_taskMode == taskMode)
            return;

        m_taskMode = taskMode;
        emit taskModeChanged(m_taskMode);
    }

    void setTaskNumberFSJ(int taskNumberFSJ)
    {
        if (m_taskNumberFSJ == taskNumberFSJ)
            return;

        m_taskNumberFSJ = taskNumberFSJ;
        emit taskNumberFSJChanged(m_taskNumberFSJ);
    }

    void setTaskNumberSSJ(int taskNumberSSJ)
    {
        if (m_taskNumberSSJ == taskNumberSSJ)
            return;

        m_taskNumberSSJ = taskNumberSSJ;
        emit taskNumberSSJChanged(m_taskNumberSSJ);
    }

    void setTaskNumber(int taskNumber)
    {
        if (m_taskNumber == taskNumber)
            return;

        m_taskNumber = taskNumber;
        emit taskNumberChanged(m_taskNumber);
    }

    void setEnableLensVcm(bool enableLensVcm)
    {
        if (m_enableLensVcm == enableLensVcm)
            return;

        m_enableLensVcm = enableLensVcm;
        emit enableLensVcmChanged(m_enableLensVcm);
    }

    void setLensVcmPath(QString lensVcmPath)
    {
        if (m_lensVcmPath == lensVcmPath)
            return;

        m_lensVcmPath = lensVcmPath;
        emit lensVcmPathChanged(m_lensVcmPath);
    }

    void setLensVcmWorkPosition(double lensVcmWorkPosition)
    {

        if (qFuzzyCompare(m_lensVcmWorkPosition, lensVcmWorkPosition))
            return;

        m_lensVcmWorkPosition = lensVcmWorkPosition;
        emit lensVcmWorkPositionChanged(m_lensVcmWorkPosition);
    }

    void setTaskAccumulate(bool taskAccumulate)
    {
        if (m_taskAccumulate == taskAccumulate)
            return;

        m_taskAccumulate = taskAccumulate;
        emit taskAccumulateChanged(m_taskAccumulate);
    }

    void setCurrentTask(int currentTask)
    {
        if (m_currentTask == currentTask)
            return;

        m_currentTask = currentTask;
        emit currentTaskChanged(m_currentTask);
    }

    void setTiltRelationship(int tiltRelationship)
    {
        if (m_tiltRelationship == tiltRelationship)
            return;

        m_tiltRelationship = tiltRelationship;
        emit tiltRelationshipChanged(m_tiltRelationship);
    }

    void setSensorOrientation(int SensorOrientation)
    {
        if (m_SensorOrientation == SensorOrientation)
            return;

        m_SensorOrientation = SensorOrientation;
        emit paramsChanged();
    }

    void setAACoreRunningTest(QString aaCoreRunningTest)
    {
        m_aaCoreRunningTest = aaCoreRunningTest;
        emit aaCoreRunningTestChanged(m_aaCoreRunningTest);
    }

    void setDispenseCount(int dispenseCount)
    {
        if (m_dispenseCount == dispenseCount)
            return;

        m_dispenseCount = dispenseCount;
        emit dispenseCountChanged();
    }

    void setDispenseCountLimit(int dispenseCountLimit)
    {
        if (m_dispenseCountLimit == dispenseCountLimit)
            return;

        m_dispenseCountLimit = dispenseCountLimit;
        emit dispenseCountLimitChanged(m_dispenseCountLimit);
    }

    void setCircleTime(double circleTime)
    {
        if (qFuzzyCompare(m_circleTime, circleTime))
            return;

        m_circleTime = circleTime;
        emit circleTimeChanged(m_circleTime);
    }

    void setCircleAverageTime(double circleAverageTime)
    {
        if (qFuzzyCompare(m_circleAverageTime, circleAverageTime))
            return;

        m_circleAverageTime = circleAverageTime;
        emit circleAverageTimeChanged(m_circleAverageTime);
    }

    void setCircleCount(int circleCount)
    {
        if (m_circleCount == circleCount)
            return;

        m_circleCount = circleCount;
        emit circleCountChanged(m_circleCount);
    }

    void setCalculatedUPH(int calculatedUPH)
    {
        if (m_calculatedUPH == calculatedUPH)
            return;

        m_calculatedUPH = calculatedUPH;
        emit calculatedUPHChanged(m_calculatedUPH);
    }

    void setMTFFrequency(int mtfFrequency)
    {
        if (m_mtfFrequency == mtfFrequency)
            return;

        m_mtfFrequency = mtfFrequency;
        emit mtfFrequencyChanged(m_mtfFrequency);
    }

    void setAAScanMTFFrequency(int aaScanMTFFrequency)
    {
        if (m_aaScanMTFFrequency == aaScanMTFFrequency)
            return;

        m_aaScanMTFFrequency = aaScanMTFFrequency;
        emit aaScanMTFFrequencyChanged(m_aaScanMTFFrequency);
    }

    void setAAScanCurveFitOrder(int aaScanCurveFitOrder)
    {
        if (m_aaScanCurveFitOrder == aaScanCurveFitOrder)
            return;

        m_aaScanCurveFitOrder = aaScanCurveFitOrder;
        emit aaScanCurveFitOrderChanged(m_aaScanCurveFitOrder);
    }

    void setAAScanOversampling(int aaScanOversampling)
    {
        if (m_aaScanOversampling == aaScanOversampling)
            return;

        m_aaScanOversampling = aaScanOversampling;
        emit aaScanOversamplingChanged(m_aaScanOversampling);
    }

    void setAAScanCurveFitErrorThreshold(int aaScanCurveFitErrorThreshold)
    {
        if (m_aaScanCurveFitErrorThreshold == aaScanCurveFitErrorThreshold)
            return;

        m_aaScanCurveFitErrorThreshold = aaScanCurveFitErrorThreshold;
        emit aaScanCurveFitErrorThresholdChanged(m_aaScanCurveFitErrorThreshold);
    }

    void setLotNumber(QString lotNumber)
    {
        if (m_lotNumber == lotNumber)
            return;

        m_lotNumber = lotNumber;
        emit LotNumberChanged(m_lotNumber);
    }

    void setDynamicTiltUpdateIndex(int dynamicTiltUpdateIndex)
    {
        if (m_dynamicTiltUpdateIndex == dynamicTiltUpdateIndex)
            return;

        m_dynamicTiltUpdateIndex = dynamicTiltUpdateIndex;
        emit dynamicTiltUpdateIndexChanged(m_dynamicTiltUpdateIndex);
    }

    void setCurrentUPH(int currentUPH)
    {
        if (m_currentUPH == currentUPH)
            return;

        m_currentUPH = currentUPH;
        emit currentUPHChanged(m_currentUPH);
    }

    void setVCMInitMode(int vcmInitMode)
    {
        if (m_vcmInitMode == vcmInitMode)
            return;

        m_vcmInitMode = vcmInitMode;
        emit vcmInitModeChanged(m_vcmInitMode);
    }

    void setVCMSlaveId(QString vcmSlaveId)
    {
        if (m_vcmSlaveId == vcmSlaveId)
            return;

        m_vcmSlaveId = vcmSlaveId;
        emit vcmSlaveIdChanged(m_vcmSlaveId);
    }

    void setVCMRegAddress(QString vcmRegAddress)
    {
        if (m_vcmRegAddress == vcmRegAddress)
            return;

        m_vcmRegAddress = vcmRegAddress;
        emit vcmRegAddressChanged(m_vcmRegAddress);
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
    void zpeakL1CoefficientChanged(double zpeakL1Coefficient);
    void zpeakL2CoefficientChanged(double zpeakL2Coefficient);
    void zpeakL3CoefficientChanged(double zpeakL3Coefficient);
    void maxDevChanged(double maxDev);
    void minDevChanged(double minDev);
    void enableCheckDispenseChanged(bool enableCheckDispense);
    void checkDispenseCountChanged();
    void minIntensityDiffChanged(int minIntensityDiff);
    void zPeakDiffL1MaxChanged(double zPeakDiffL1Max);
    void zPeakDiffL2MaxChanged(double zPeakDiffL2Max);
    void zPeakDiffL3MaxChanged(double zPeakDiffL3Max);
    void CCL1MinDevChanged(double CCL1MinDev);
    void CCL1MaxDevChanged(double CCL1MaxDev);
    void CCL2MinDevChanged(double CCL2MinDev);
    void CCL2MaxDevChanged(double CCL2MaxDev);
    void CCL3MinDevChanged(double CCL3MinDev);
    void CCL3MaxDevChanged(double CCL3MaxDev);
    void isDebugChanged(bool isDebug);
    void taskModeChanged(int taskMode);
    void taskNumberFSJChanged(int taskNumberFSJ);
    void taskNumberSSJChanged(int taskNumberSSJ);
    void taskNumberChanged(int taskNumber);
    void enableLensVcmChanged(bool enableLensVcm);
    void lensVcmPathChanged(QString lensVcmPath);
    void lensVcmWorkPositionChanged(double lensVcmWorkPosition);
    void taskAccumulateChanged(bool taskAccumulate);
    void currentTaskChanged(int currentTask);
    void tiltRelationshipChanged(int tiltRelationship);
    void aaCoreRunningTestChanged(QString aaCoreRunningTest);
    void dispenseCountChanged();
    void dispenseCountLimitChanged(int dispenseCountLimit);
    void circleTimeChanged(double circleTime);
    void circleAverageTimeChanged(double circleAverageTime);
    void circleCountChanged(int circleCount);
    void calculatedUPHChanged(int calculatedUPH);
    void mtfFrequencyChanged(int mtfFrequency);
    void aaScanMTFFrequencyChanged(int aaScanMTFFrequency);
    void aaScanCurveFitOrderChanged(int aaScanCurveFitOrder);
    void aaScanOversamplingChanged(int aaScanOversampling);
    void aaScanCurveFitErrorThreshold(int aaScanCurveFitErrorThreshold);
    void aaScanCurveFitErrorThresholdChanged(int aaScanCurveFitErrorThreshold);
    void LotNumberChanged(QString lotNumber);
    void dynamicTiltUpdateIndexChanged(int dynamicTiltUpdateIndex);
    void currentUPHChanged(int currentUPH);
    void vcmInitModeChanged(int vcmInitMode);
    void vcmSlaveIdChanged(QString vcmSlaveId);
    void vcmRegAddressChanged(QString vcmRegAddress);
};
class AACoreStates: public PropertyBase
{
    Q_OBJECT
public:
    explicit AACoreStates(){}
    bool needSendLensRequest(){return(!hasLens())&&(!isWaitingLens());}
    bool needSendSensorRequest(){return (!hasSensor())&&(!isWaitingSensor());}

    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(int stationNumber READ stationNumber WRITE setStationNumber NOTIFY stationNumberChanged)
    Q_PROPERTY(bool disableStation READ disableStation WRITE setDisableStation NOTIFY disableStationChanged)
    Q_PROPERTY(int stationTask READ stationTask WRITE setStationTask NOTIFY stationTaskChanged)
    Q_PROPERTY(int currentTask READ currentTask WRITE setCurrentTask NOTIFY currentTaskChanged)
    Q_PROPERTY(bool finishStationTask READ finishStationTask WRITE setFinishSensorTask NOTIFY finishSensorTaskChanged)
    Q_PROPERTY(bool finishLensTask READ finishLensTask WRITE setFinishLensTask NOTIFY finishLensTaskChanged)
    Q_PROPERTY(bool isWaitingLens READ isWaitingLens WRITE setIsWaitingLens NOTIFY isWaitingLensChanged)
    Q_PROPERTY(bool isWaitingSensor READ isWaitingSensor WRITE setIsWaitingSensor NOTIFY isWaitingSensorChanged)
    Q_PROPERTY(bool hasLens READ hasLens WRITE setHasLens NOTIFY hasLensChanged)
    Q_PROPERTY(bool hasNgLens READ hasNgLens WRITE setHasNgLens NOTIFY hasNgLensChanged)
    Q_PROPERTY(bool hasSensor READ hasSensor WRITE setHasSensor NOTIFY hasSensorChanged)
    Q_PROPERTY(bool hasNgSensor READ hasNgSensor WRITE setHasNgSensor NOTIFY hasNgSensorChanged)
    Q_PROPERTY(bool hasProduct READ hasProduct WRITE setHasProduct NOTIFY hasProductChanged)
    Q_PROPERTY(bool hasNgProduct READ hasNgProduct WRITE setHasNgProduct NOTIFY hasNgProductChanged)
    Q_PROPERTY(bool aaGripperState READ aaGripperState WRITE setAaGripperState NOTIFY aaGripperStateChanged)
    Q_PROPERTY(bool tcpAAGripperState READ tcpAAGripperState WRITE setTcpAAGripperState NOTIFY tcpAAGripperStateChanged)
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

    int runMode() const
    {
        return m_runMode;
    }

    bool disableStation() const
    {
        return m_disableStation;
    }

    int stationNumber() const
    {
        return m_stationNumber;
    }

    int stationTask() const
    {
        return m_stationTask;
    }

    int currentTask() const
    {
        return m_currentTask;
    }

    bool finishStationTask() const
    {
        return m_finishSensorTask;
    }

    bool finishLensTask() const
    {
        return m_finishLensTask;
    }

    bool tcpAAGripperState() const
    {
        return m_tcpAAGripperState;
    }

    bool aaGripperState() const
    {
        return m_aaGripperState;
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

    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }

    void setDisableStation(bool disableStation)
    {
        if (m_disableStation == disableStation)
            return;

        m_disableStation = disableStation;
        emit disableStationChanged(m_disableStation);
    }

    void setStationNumber(int stationNumber)
    {
        if (m_stationNumber == stationNumber)
            return;

        m_stationNumber = stationNumber;
        emit stationNumberChanged(m_stationNumber);
    }

    void setStationTask(int stationTask)
    {
        if (m_stationTask == stationTask)
            return;

        m_stationTask = stationTask;
        emit stationTaskChanged(m_stationTask);
    }

    void setCurrentTask(int currentTask)
    {
        if (m_currentTask == currentTask)
            return;

        m_currentTask = currentTask;
        emit currentTaskChanged(m_currentTask);
    }

    void setFinishSensorTask(bool finishSensorTask)
    {
        if (m_finishSensorTask == finishSensorTask)
            return;

        m_finishSensorTask = finishSensorTask;
        emit finishSensorTaskChanged(m_finishSensorTask);
    }

    void setFinishLensTask(bool finishLensTask)
    {
        if (m_finishLensTask == finishLensTask)
            return;

        m_finishLensTask = finishLensTask;
        emit finishLensTaskChanged(m_finishLensTask);
    }

    void setTcpAAGripperState(bool tcpAAGripperState)
    {
        if (m_tcpAAGripperState == tcpAAGripperState)
            return;

        m_tcpAAGripperState = tcpAAGripperState;
        emit tcpAAGripperStateChanged(m_tcpAAGripperState);
    }

    void setAaGripperState(bool aaGripperState)
    {
        if (m_aaGripperState == aaGripperState)
            return;

        m_aaGripperState = aaGripperState;
        emit aaGripperStateChanged(m_aaGripperState);
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

    void runModeChanged(int runMode);

    void disableStationChanged(bool disableStation);

    void stationNumberChanged(int stationNumber);

    void stationTaskChanged(int stationTask);

    void currentTaskChanged(int currentTask);

    void finishSensorTaskChanged(bool finishStationTask);

    void finishLensTaskChanged(bool finishLensTask);

    void tcpAAGripperStateChanged(bool tcpAAGripperState);

    void aaGripperStateChanged(bool aaGripperState);

private:
    bool m_isWaitingLens = false;
    bool m_isWaitingSensor = false;
    bool m_hasLens = false;
    bool m_hasNgLens = false;
    bool m_hasSensor = false;
    bool m_hasNgSensor = false;
    bool m_hasProduct = false;
    bool m_hasNgProduct = false;
    int m_runMode = 0;
    bool m_disableStation = false;
    int m_stationNumber = 0;
    int m_stationTask = 0;
    int m_currentTask = 0;
    bool m_finishSensorTask = false;
    bool m_finishLensTask = false;
    bool m_tcpAAGripperState;
    bool m_aaGripperState;
};

#endif // AACOREPARAMETERS_H
