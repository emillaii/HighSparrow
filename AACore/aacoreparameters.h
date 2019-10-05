﻿#ifndef AACOREPARAMETERS_H
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

    double m_minDev = -100;

    bool m_enableCheckDispense = false;

    int m_checkDispenseCount = 10;

    int m_minIntensityDiff = 10;

    double m_zPeakDiff05Max = 3;

    double m_zPeakDiff08Max = 5;

    double m_CC05MinDev = -100;

    double m_CC05MaxDev = 100;

    double m_CC08MinDev = -100;

    double m_CC08MaxDev = 100;

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

public:
    explicit AACoreParameters(){
        for (int i = 0; i < 4*4; i++) // 4 field of view * 4 edge number
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
    Q_PROPERTY(double zpeak03Coefficient READ zpeak03Coefficient WRITE setZpeak03Coefficient NOTIFY zpeak03CoefficientChanged)
    Q_PROPERTY(double zpeak05Coefficient READ zpeak05Coefficient WRITE setZpeak05Coefficient NOTIFY zpeak05CoefficientChanged)
    Q_PROPERTY(double zpeak08Coefficient READ zpeak08Coefficient WRITE setZpeak08Coefficient NOTIFY zpeak08CoefficientChanged)
    Q_PROPERTY(double maxDev READ maxDev WRITE setMaxDev NOTIFY maxDevChanged)
    Q_PROPERTY(double minDev READ minDev WRITE setMinDev NOTIFY minDevChanged)
    Q_PROPERTY(bool enableCheckDispense READ enableCheckDispense WRITE setEnableCheckDispense NOTIFY enableCheckDispenseChanged)
    Q_PROPERTY(int checkDispenseCount READ checkDispenseCount WRITE setCheckDispenseCount NOTIFY checkDispenseCountChanged)
    Q_PROPERTY(int minIntensityDiff READ minIntensityDiff WRITE setMinIntensityDiff NOTIFY minIntensityDiffChanged)
    Q_PROPERTY(double zPeakDiff05Max READ zPeakDiff05Max WRITE setZPeakDiff05Max NOTIFY zPeakDiff05MaxChanged)
    Q_PROPERTY(double zPeakDiff08Max READ zPeakDiff08Max WRITE setZPeakDiff08Max NOTIFY zPeakDiff08MaxChanged)
    Q_PROPERTY(double CC05MinDev READ CC05MinDev WRITE setCC05MinDev NOTIFY CC05MinDevChanged)
    Q_PROPERTY(double CC05MaxDev READ CC05MaxDev WRITE setCC05MaxDev NOTIFY CC05MaxDevChanged)
    Q_PROPERTY(double CC08MinDev READ CC08MinDev WRITE setCC08MinDev NOTIFY CC08MinDevChanged)
    Q_PROPERTY(double CC08MaxDev READ CC08MaxDev WRITE setCC08MaxDev NOTIFY CC08MaxDevChanged)
    Q_PROPERTY(bool isDebug READ isDebug WRITE setIsDebug NOTIFY isDebugChanged)
    Q_PROPERTY(bool enableLensVcm READ enableLensVcm WRITE setEnableLensVcm NOTIFY enableLensVcmChanged)
    Q_PROPERTY(QString lensVcmPath READ lensVcmPath WRITE setLensVcmPath NOTIFY lensVcmPathChanged)
    Q_PROPERTY(double lensVcmWorkPosition READ lensVcmWorkPosition WRITE setLensVcmWorkPosition NOTIFY lensVcmWorkPositionChanged)
    Q_PROPERTY(int tiltRelationship READ tiltRelationship WRITE setTiltRelationship NOTIFY tiltRelationshipChanged)
    Q_PROPERTY(QString aaCoreRunningTest READ aaCoreRunningTest WRITE setAACoreRunningTest NOTIFY aaCoreRunningTestChanged)
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

    double zPeakDiff05Max() const
    {
        return m_zPeakDiff05Max;
    }

    double zPeakDiff08Max() const
    {
        return m_zPeakDiff08Max;
    }

    double CC05MinDev() const
    {
        return m_CC05MinDev;
    }

    double CC05MaxDev() const
    {
        return m_CC05MaxDev;
    }

    double CC08MinDev() const
    {
        return m_CC08MinDev;
    }

    double CC08MaxDev() const
    {
        return m_CC08MaxDev;
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

    void setZpeak03Coefficient(double zpeak03Coefficient)
    {
        if(zpeak03Coefficient<0)return;

        if (qFuzzyCompare(m_zpeak03Coefficient, zpeak03Coefficient))
            return;

        m_zpeak03Coefficient = zpeak03Coefficient;
        emit zpeak03CoefficientChanged(m_zpeak03Coefficient);
    }

    void setZpeak05Coefficient(double zpeak05Coefficient)
    {
        if(zpeak05Coefficient<0)return;

        if (qFuzzyCompare(m_zpeak05Coefficient, zpeak05Coefficient))
            return;

        m_zpeak05Coefficient = zpeak05Coefficient;
        emit zpeak05CoefficientChanged(m_zpeak05Coefficient);
    }

    void setZpeak08Coefficient(double zpeak08Coefficient)
    {
        if(zpeak08Coefficient<0)return;

        if (qFuzzyCompare(m_zpeak08Coefficient, zpeak08Coefficient))
            return;

        m_zpeak08Coefficient = zpeak08Coefficient;
        emit zpeak08CoefficientChanged(m_zpeak08Coefficient);
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
        emit checkDispenseCountChanged(m_checkDispenseCount);
    }

    void setMinIntensityDiff(int minIntensityDiff)
    {
        if (m_minIntensityDiff == minIntensityDiff)
            return;

        m_minIntensityDiff = minIntensityDiff;
        emit minIntensityDiffChanged(m_minIntensityDiff);
    }

    void setZPeakDiff05Max(double zPeakDiff05Max)
    {

        if (qFuzzyCompare(m_zPeakDiff05Max, zPeakDiff05Max))
            return;

        m_zPeakDiff05Max = zPeakDiff05Max;
        emit zPeakDiff05MaxChanged(m_zPeakDiff05Max);
    }

    void setZPeakDiff08Max(double zPeakDiff08Max)
    {

        if (qFuzzyCompare(m_zPeakDiff08Max, zPeakDiff08Max))
            return;

        m_zPeakDiff08Max = zPeakDiff08Max;
        emit zPeakDiff08MaxChanged(m_zPeakDiff08Max);
    }

    void setCC05MinDev(double CC05MinDev)
    {

        if (qFuzzyCompare(m_CC05MinDev, CC05MinDev))
            return;

        m_CC05MinDev = CC05MinDev;
        emit CC05MinDevChanged(m_CC05MinDev);
    }

    void setCC05MaxDev(double CC05MaxDev)
    {

        if (qFuzzyCompare(m_CC05MaxDev, CC05MaxDev))
            return;

        m_CC05MaxDev = CC05MaxDev;
        emit CC05MaxDevChanged(m_CC05MaxDev);
    }

    void setCC08MinDev(double CC08MinDev)
    {

        if (qFuzzyCompare(m_CC08MinDev, CC08MinDev))
            return;

        m_CC08MinDev = CC08MinDev;
        emit CC08MinDevChanged(m_CC08MinDev);
    }

    void setCC08MaxDev(double CC08MaxDev)
    {

        if (qFuzzyCompare(m_CC08MaxDev, CC08MaxDev))
            return;

        m_CC08MaxDev = CC08MaxDev;
        emit CC08MaxDevChanged(m_CC08MaxDev);
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
    void enableCheckDispenseChanged(bool enableCheckDispense);
    void checkDispenseCountChanged(int checkDispenseCount);
    void minIntensityDiffChanged(int minIntensityDiff);
    void zPeakDiff05MaxChanged(double zPeakDiff05Max);
    void zPeakDiff08MaxChanged(double zPeakDiff08Max);
    void CC05MinDevChanged(double CC05MinDev);
    void CC05MaxDevChanged(double CC05MaxDev);
    void CC08MinDevChanged(double CC08MinDev);
    void CC08MaxDevChanged(double CC08MaxDev);
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

        if (qFuzzyCompare(m_circleAverageTime, circleAverageTime))
            return;

        m_circleAverageTime = circleAverageTime;
        emit circleAverageTimeChanged(m_circleAverageTime);
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

    void runModeChanged(int runMode);

    void disableStationChanged(bool disableStation);

    void stationNumberChanged(int stationNumber);

    void stationTaskChanged(int stationTask);

    void currentTaskChanged(int currentTask);

    void finishSensorTaskChanged(bool finishStationTask);

    void finishLensTaskChanged(bool finishLensTask);

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
    int m_runMode = 0;
    bool m_disableStation = false;
    int m_stationNumber = 0;
    int m_stationTask = 0;
    int m_currentTask = 0;
    bool m_finishSensorTask = false;
    bool m_finishLensTask = false;
};

#endif // AACOREPARAMETERS_H
