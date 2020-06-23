#ifndef SENSORLOADERPARAMETER_H
#define SENSORLOADERPARAMETER_H

#include "propertybase.h"



class SensorLoaderParameter:public PropertyBase
{
    Q_OBJECT
public:
    SensorLoaderParameter():PropertyBase (){}
	Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)
    Q_PROPERTY(bool handlyChangeSensor READ handlyChangeSensor WRITE setHandlyChangeSensor NOTIFY handlyChangeSensorChanged)
    Q_PROPERTY(double vcmWorkForce READ vcmWorkForce WRITE setVcmWorkForce NOTIFY vcmWorkForceChanged)
    Q_PROPERTY(double vcmWorkSpeed READ vcmWorkSpeed WRITE setVcmWorkSpeed NOTIFY vcmWorkSpeedChanged)
    Q_PROPERTY(double vcmMargin READ vcmMargin WRITE setVcmMargin NOTIFY vcmMarginChanged)
    Q_PROPERTY(double sutMargin READ sutMargin WRITE setSutMargin NOTIFY sutMarginChanged)
    Q_PROPERTY(double placeTrayMargin READ placeTrayMargin WRITE setPlaceTrayMargin NOTIFY placeTrayMarginChanged)
    Q_PROPERTY(bool enableForceLimit READ enableForceLimit WRITE setEnableForceLimit NOTIFY enableForceLimitChanged)
    Q_PROPERTY(bool enableSutForceLimit READ enableSutForceLimit WRITE setEnableSutForceLimit NOTIFY enableSutForceLimitChanged)

    Q_PROPERTY(bool disablePickFromTrayForceLimit READ disablePickFromTrayForceLimit WRITE setDisablePickFromTrayForceLimit NOTIFY disablePickFromTrayForceLimitChanged)
    Q_PROPERTY(bool disableBackToTrayForceLimit READ disableBackToTrayForceLimit WRITE setDisableBackToTrayForceLimit NOTIFY disableBackToTrayForceLimitChanged)
    Q_PROPERTY(bool disablePlaceToSutForceLimmit READ disablePlaceToSutForceLimmit WRITE setDisablePlaceToSutForceLimmit NOTIFY disablePlaceToSutForceLimmitChanged)
    Q_PROPERTY(bool disablePickFromSutForceLimit READ disablePickFromSutForceLimit WRITE setDisablePickFromSutForceLimit NOTIFY disablePickFromSutForceLimitChanged)
    Q_PROPERTY(bool disablePlaceToGoodTrayForceLimit READ disablePlaceToGoodTrayForceLimit WRITE setDisablePlaceToGoodTrayForceLimit NOTIFY disablePlaceToGoodTrayForceLimitChanged)
    Q_PROPERTY(bool disablePlaceToNgTrayForceLimit READ disablePlaceToNgTrayForceLimit WRITE setDisablePlaceToNgTrayForceLimit NOTIFY disablePlaceToNgTrayForceLimitChanged)
    Q_PROPERTY(double pickFromTrayMargin READ pickFromTrayMargin WRITE setPickFromTrayMargin NOTIFY pickFromTrayMarginChanged)
    Q_PROPERTY(double backToTrayMargin READ backToTrayMargin WRITE setBackToTrayMargin NOTIFY backToTrayMarginChanged)
    Q_PROPERTY(double placeToSut1Margin READ placeToSut1Margin WRITE setPlaceToSut1Margin NOTIFY placeToSut1MarginChanged)
    Q_PROPERTY(double pickFromSut1Margin READ pickFromSut1Margin WRITE setPickFromSut1Margin NOTIFY pickFromSut1MarginChanged)
    Q_PROPERTY(double placeToSut2Margin READ placeToSut2Margin WRITE setPlaceToSut2Margin NOTIFY placeToSut2MarginChanged)
    Q_PROPERTY(double pickFromSut2Margin READ pickFromSut2Margin WRITE setPickFromSut2Margin NOTIFY pickFromSut2MarginChanged)
    Q_PROPERTY(double placeToGoodTrayMargin READ placeToGoodTrayMargin WRITE setPlaceToGoodTrayMargin NOTIFY placeToGoodTrayMarginChanged)
    Q_PROPERTY(double placeToNgTrayMargin READ placeToNgTrayMargin WRITE setPlaceToNgTrayMargin NOTIFY placeToNgTrayMarginChanged)


    Q_PROPERTY(double pickSensorZ READ pickSensorZ WRITE setPickSensorZ NOTIFY pickSensorZChanged)
    Q_PROPERTY(double pickSensorZ2 READ pickSensorZ2 WRITE setPickSensorZ2 NOTIFY pickSensorZ2Changed)
    Q_PROPERTY(double placeSUT1SensorZ READ placeSUT1SensorZ WRITE setPlaceSUT1SensorZ NOTIFY placeSUT1SensorZChanged)
    Q_PROPERTY(double placeSUT2SensorZ READ placeSUT2SensorZ WRITE setPlaceSUT2SensorZ NOTIFY placeSUT2SensorZChanged)
    Q_PROPERTY(QString sensorLocationName READ sensorLocationName WRITE setSensorLocationName NOTIFY sensorLocationNameChanged)
    Q_PROPERTY(QString vacancyLocationName READ vacancyLocationName WRITE setVacancyLocationName NOTIFY vacancyLocationNameChanged)
    Q_PROPERTY(QString calibrationGlassLocationName READ calibrationGlassLocationName WRITE setCalibrationGlassLocationName NOTIFY calibrationGlassLocationNameChanged)
    Q_PROPERTY(QString sutLocationName READ sutLocationName WRITE setSutLocationName NOTIFY sutLocationNameChanged)
    Q_PROPERTY(QString sutSensorLocationName READ sutSensorLocationName WRITE setSutSensorLocationName NOTIFY sutSensorLocationNameChanged)
    Q_PROPERTY(QString sutProductLocationName READ sutProductLocationName WRITE setSutProductLocationName NOTIFY sutProductLocationNameChanged)
    Q_PROPERTY(int finishDelay READ finishDelay WRITE setFinishDelay NOTIFY finishDelayChanged)
    Q_PROPERTY(double pickSUT1ProductZ READ pickSUT1ProductZ WRITE setPickSUT1ProductZ NOTIFY pickSUT1ProductZChanged)
    Q_PROPERTY(double pickSUT2ProductZ READ pickSUT2ProductZ WRITE setPickSUT2ProductZ NOTIFY pickSUT2ProductZChanged)
    Q_PROPERTY(double placeProductZ READ placeProductZ WRITE setPlaceProductZ NOTIFY placeProductZChanged)
    Q_PROPERTY(double placeProductZ2 READ placeProductZ2 WRITE setPlaceProductZ2 NOTIFY placeProductZ2Changed)
    Q_PROPERTY(double placeBufferProductZ READ placeBufferProductZ WRITE setPlaceBufferProductZ NOTIFY placeBufferProductZChanged)
    Q_PROPERTY(double placeNgProductZ READ placeNgProductZ WRITE setPlaceNgProductZ NOTIFY placeNgProductZChanged)
    Q_PROPERTY(double zOffset READ zOffset WRITE setZOffset NOTIFY zOffsetChanged)
    Q_PROPERTY(double pickSUT1NgSensorZ READ pickSUT1NgSensorZ WRITE setPickSUT1NgSensorZ NOTIFY pickSUT1NgSensorZChanged)
    Q_PROPERTY(double pickSUT2NgSensorZ READ pickSUT2NgSensorZ WRITE setPickSUT2NgSensorZ NOTIFY pickSUT2NgSensorZChanged)
    Q_PROPERTY(double placeNgSensorZ READ placeNgSensorZ WRITE setPlaceNgSensorZ NOTIFY placeNgSensorZChanged)
    Q_PROPERTY(bool enableEscape READ enableEscape WRITE setEnableEscape NOTIFY enableEscapeChanged)
    Q_PROPERTY(double escapeHeight READ escapeHeight WRITE setEscapeHeight NOTIFY escapeHeightChanged)
    Q_PROPERTY(double escapeX READ escapeX WRITE setEscapeX NOTIFY escapeXChanged)
    Q_PROPERTY(double escapeY READ escapeY WRITE setEscapeY NOTIFY escapeYChanged)
    Q_PROPERTY(int changeTrayTimeOut READ changeTrayTimeOut WRITE setChangeTrayTimeOut NOTIFY changeTrayTimeOutChanged)
    Q_PROPERTY(double picker1PickTheta READ picker1PickTheta WRITE setPicker1PickTheta NOTIFY picker1PickThetaChanged)
    Q_PROPERTY(double picker2PickTheta READ picker2PickTheta WRITE setPicker2PickTheta NOTIFY picker2PickThetaChanged)
    Q_PROPERTY(double picker1PlaceTheta READ picker1PlaceTheta WRITE setPicker1PlaceTheta NOTIFY picker1PlaceThetaChanged)
    Q_PROPERTY(double picker2PlaceTheta READ picker2PlaceTheta WRITE setPicker2PlaceTheta NOTIFY picker2PlaceThetaChanged)
    Q_PROPERTY(double pickProductForce READ pickProductForce WRITE setPickProductForce NOTIFY pickProductForceChanged)
    Q_PROPERTY(bool useSensorOffset READ useSensorOffset WRITE setUseSensorOffset NOTIFY useSensorOffsetChanged)
    Q_PROPERTY(double sensorOffsetX READ sensorOffsetX WRITE setSensorOffsetX NOTIFY sensorOffsetXChanged)
    Q_PROPERTY(double sensorOffsetY READ sensorOffsetY WRITE setSensorOffsetY NOTIFY sensorOffsetYChanged)

    Q_PROPERTY(bool usePlan READ usePlan WRITE setUsePlan NOTIFY usePlanChanged)
    Q_PROPERTY(int planNumber READ planNumber WRITE setPlanNumber NOTIFY planNumberChanged)
    Q_PROPERTY(double accumulatedHour READ accumulatedHour WRITE setAccumulatedHour NOTIFY accumulatedHourChanged)

    Q_PROPERTY(int repeatTime READ repeatTime WRITE setRepeatTime NOTIFY repeatTimeChanged)
    Q_PROPERTY(int testSensorCount READ testSensorCount WRITE setTestSensorCount NOTIFY testSensorCountChanged)
    Q_PROPERTY(int staticTest READ staticTest WRITE setStaticTest NOTIFY staticTestChanged)
    Q_PROPERTY(bool enableNgSensorPr READ enableNgSensorPr WRITE setEnableNgSensorPr NOTIFY enableNgSensorPrChanged)
    Q_PROPERTY(bool enableProductPr READ enableProductPr WRITE setEnableProductPr NOTIFY enableProductPrChanged)
    Q_PROPERTY(bool enableBufferProductPr READ enableBufferProductPr WRITE setEnableBufferProductPr NOTIFY enableBufferProductPrChanged)
    Q_PROPERTY(bool enableNgProductPr READ enableNgProductPr WRITE setEnableNgProductPr NOTIFY enableNgProductPrChanged)
    Q_PROPERTY(bool enablePlaceProdcutPr READ enablePlaceProdcutPr WRITE setEnablePlaceProdcuPr NOTIFY enablePlaceProdcutPrChanged)
    Q_PROPERTY(bool enablePlaceNgProductPr READ enablePlaceNgProductPr WRITE setEnablePlaceNgProductPr NOTIFY enablePlacceNgProductPrChanged)
    Q_PROPERTY(bool enablePlaceNgSensorPr READ enablePlaceNgSensorPr WRITE setEnablePlaceNgSensorPr NOTIFY enablePlaceNgSensorPrChanged)
    Q_PROPERTY(int autoPrTime READ autoPrTime WRITE setAutoPrTime NOTIFY autoPrTimeChanged)
    Q_PROPERTY(bool enablePlaceForce READ enablePlaceForce WRITE setEnablePlaceForce NOTIFY enablePlaceForceChanged)
    Q_PROPERTY(double sut1Theta READ sut1Theta WRITE setSut1Theta NOTIFY sut1ThetaChanged)
    Q_PROPERTY(double sut2Theta READ sut2Theta WRITE setSut2Theta NOTIFY sut2ThetaChanged)
    Q_PROPERTY(bool openTimeLog READ openTimeLog WRITE setOpenTimeLog NOTIFY openTimeLogChanged)
    Q_PROPERTY(int vacuumOperationOutTime READ vacuumOperationOutTime WRITE setVacuumOperationOutTime NOTIFY vacuumOperationOutTimeChanged)
    Q_PROPERTY(int holdTime READ holdTime WRITE setHoldTime NOTIFY holdTimeChanged)
    double vcmWorkForce() const
    {
        return m_vcmWorkForce;
    }
    double vcmWorkSpeed() const
    {
        return m_vcmWorkSpeed;
    }

    double vcmMargin() const
    {
        return m_vcmMargin;
    }

    QString sensorLocationName() const
    {
        return m_sensorLocationName;
    }

    QString vacancyLocationName() const
    {
        return m_vacancyLocationName;
    }

    QString sutLocationName() const
    {
        return m_sutLocationName;
    }

    QString sutSensorLocationName() const
    {
        return m_sutSensorLocationName;
    }

    QString sutProductLocationName() const
    {
        return m_sutProductLocationName;
    }

    int finishDelay() const
    {
        return m_finishDelay;
    }

    double pickSUT1ProductZ() const
    {
        return m_pickSUT1ProductZ;
    }

    double placeProductZ() const
    {
        return m_placeProductZ;
    }

    double pickSensorZ() const
    {
        return m_pickSensorZ;
    }

    double placeSUT1SensorZ() const
    {
        return m_placeSUT1SensorZ;
    }

    double zOffset() const
    {
        return m_zOffset;
    }

    double pickSUT1NgSensorZ() const
    {
        return m_pickSUT1NgSensorZ;
    }

    double placeNgSensorZ() const
    {
        return m_placeNgSensorZ;
    }

    QString calibrationGlassLocationName() const
    {
        return m_calibrationGlassLocationName;
    }

    double escapeHeight() const
    {
        return m_escapeHeight;
    }

    double escapeX() const
    {
        return m_escapeX;
    }

    double escapeY() const
    {
        return m_escapeY;
    }


    int changeTrayTimeOut() const
    {
        return m_changeTrayTimeOut;
    }

    bool usePlan() const
    {
        return m_usePlan;
    }

    int planNumber() const
    {
        return m_planNumber;
    }
    double accumulatedHour() const
    {
        return m_accumulatedHour;
    }

    double placeNgProductZ() const
    {
        return m_placeNgProductZ;
    }

    double pickProductForce() const
    {
        return m_pickProductForce;
    }

    bool useSensorOffset() const
    {
        return m_useSensorOffset;
    }

    double sensorOffsetX() const
    {
        return m_sensorOffsetX;
    }

    double sensorOffsetY() const
    {
        return m_sensorOffsetY;
    }

    int repeatTime() const
    {
        return m_repeatTime;
    }

    int testSensorCount() const
    {
        return m_testSensorCount;
    }

    int staticTest() const
    {
        return m_staticTest;
    }
    bool enableEscape() const
    {
        return m_enableEscape;
    }

    bool enableForceLimit() const
    {
        return m_enableForceLimit;
    }

    bool enableNgSensorPr() const
    {
        return m_enableNgSensorPr;
    }

    bool enableProductPr() const
    {
        return m_enableProductPr;
    }

    bool enableNgProductPr() const
    {
        return m_enableNgProductPr;
    }

    bool enablePlaceProdcutPr() const
    {
        return m_enablePlaceProdcutPr;
    }

    bool enablePlaceNgProductPr() const
    {
        return m_enablePlaceNgProductPr;
    }

    bool enablePlaceNgSensorPr() const
    {
        return m_enablePlaceNgSensorPr;
    }

    int autoPrTime() const
    {
        return m_autoPrTime;
    }

    bool enablePlaceForce() const
    {
        return m_enablePlaceForce;
    }

    double sut1Theta() const
    {
        return m_sut1Theta;
    }

    double sut2Theta() const
    {
        return m_sut2Theta;
    }

    bool enableSutForceLimit() const
    {
        return m_enableSutForceLimit;
    }

    double placeTrayMargin() const
    {
        return m_placeTrayMargin;
    }

    double sutMargin() const
    {
        return m_sutMargin;
    }

    bool openTimeLog() const
    {
        return m_openTimeLog;
    }

    double pickSensorZ2() const
    {
        return m_pickSensorZ2;
    }

    double placeProductZ2() const
    {
        return m_placeProductZ2;
    }

    double placeBufferProductZ() const
    {
        return m_placeBufferProductZ;
    }

    int vacuumOperationOutTime() const
    {
        return m_vacuumOperationOutTime;
    }

    bool disablePickFromTrayForceLimit() const
    {
        return m_disablePickFromTrayForceLimit;
    }

    bool disableBackToTrayForceLimit() const
    {
        return m_disableBackToTrayForceLimit;
    }

    bool disablePlaceToSutForceLimmit() const
    {
        return m_disablePlaceToSutForceLimmit;
    }

    bool disablePickFromSutForceLimit() const
    {
        return m_disablePickFromSutForceLimit;
    }

    bool disablePlaceToGoodTrayForceLimit() const
    {
        return m_disablePlaceToGoodTrayForceLimit;
    }

    bool disablePlaceToNgTrayForceLimit() const
    {
        return m_disablePlaceToNgTrayForceLimit;
    }

    double pickFromTrayMargin() const
    {
        return m_pickFromTrayMargin;
    }

    double backToTrayMargin() const
    {
        return m_backToTrayMargin;
    }

    double placeToSut1Margin() const
    {
        return m_placeToSut1Margin;
    }

    double pickFromSut1Margin() const
    {
        return m_pickFromSut1Margin;
    }

    double placeToSut2Margin() const
    {
        return m_placeToSut2Margin;
    }

    double pickFromSut2Margin() const
    {
        return m_pickFromSut2Margin;
    }

    double placeToGoodTrayMargin() const
    {
        return m_placeToGoodTrayMargin;
    }

    double placeToNgTrayMargin() const
    {
        return m_placeToNgTrayMargin;
    }

    bool enableBufferProductPr() const
    {
        return m_enableBufferProductPr;
    }


    double picker1PickTheta() const
    {
        return m_picker1PickTheta;
    }

    double picker2PickTheta() const
    {
        return m_picker2PickTheta;
    }

    double picker1PlaceTheta() const
    {
        return m_picker1PlaceTheta;
    }

    double picker2PlaceTheta() const
    {
        return m_picker2PlaceTheta;
    }
	
    bool handlyChangeSensor() const
    {
        return m_handlyChangeSensor;
    }

    int holdTime() const
    {
        return m_holdTime;
    }
	
    QString moduleName() const
    {
        return m_moduleName;
    }	

    double placeSUT2SensorZ() const
    {
        return m_placeSUT2SensorZ;
    }

    double pickSUT2ProductZ() const
    {
        return m_pickSUT2ProductZ;
    }

    double pickSUT2NgSensorZ() const
    {
        return m_pickSUT2NgSensorZ;
    }

public slots:
    void setVcmWorkForce(double vcmWorkForce)
    {
        
        if (qFuzzyCompare(m_vcmWorkForce, vcmWorkForce))
            return;

        m_vcmWorkForce = vcmWorkForce;
        emit vcmWorkForceChanged(m_vcmWorkForce);
    }
    void setVcmWorkSpeed(double vcmWorkSpeed)
    {
        
        if (qFuzzyCompare(m_vcmWorkSpeed, vcmWorkSpeed))
            return;

        m_vcmWorkSpeed = vcmWorkSpeed;
        emit vcmWorkSpeedChanged(m_vcmWorkSpeed);
    }

    void setVcmMargin(double vcmMargin)
    {
        
        if (qFuzzyCompare(m_vcmMargin, vcmMargin))
            return;

        m_vcmMargin = vcmMargin;
        emit vcmMarginChanged(m_vcmMargin);
    }


    void setSensorLocationName(QString sensorLocationName)
    {
        if (m_sensorLocationName == sensorLocationName)
            return;

        m_sensorLocationName = sensorLocationName;
        emit sensorLocationNameChanged(m_sensorLocationName);
    }

    void setVacancyLocationName(QString vacancyLocationName)
    {
        if (m_vacancyLocationName == vacancyLocationName)
            return;

        m_vacancyLocationName = vacancyLocationName;
        emit vacancyLocationNameChanged(m_vacancyLocationName);
    }

    void setSutLocationName(QString sutLocationName)
    {
        if (m_sutLocationName == sutLocationName)
            return;

        m_sutLocationName = sutLocationName;
        emit sutLocationNameChanged(m_sutLocationName);
    }

    void setSutSensorLocationName(QString sutSensorLocationName)
    {
        if (m_sutSensorLocationName == sutSensorLocationName)
            return;

        m_sutSensorLocationName = sutSensorLocationName;
        emit sutSensorLocationNameChanged(m_sutSensorLocationName);
    }
    void setSutProductLocationName(QString sutProductLocationName)
    {
        if (m_sutProductLocationName == sutProductLocationName)
            return;

        m_sutProductLocationName = sutProductLocationName;
        emit sutProductLocationNameChanged(m_sutProductLocationName);
    }

    void setFinishDelay(int finishDelay)
    {
        if (m_finishDelay == finishDelay)
            return;

        m_finishDelay = finishDelay;
        emit finishDelayChanged(m_finishDelay);
    }

    void setPickSUT1ProductZ(double pickSUT1ProductZ)
    {
        
        if (qFuzzyCompare(m_pickSUT1ProductZ, pickSUT1ProductZ))
            return;

        m_pickSUT1ProductZ = pickSUT1ProductZ;
        emit pickSUT1ProductZChanged(m_pickSUT1ProductZ);
    }

    void setPlaceProductZ(double placeProductZ)
    {
        
        if (qFuzzyCompare(m_placeProductZ, placeProductZ))
            return;

        m_placeProductZ = placeProductZ;
        emit placeProductZChanged(m_placeProductZ);
    }

    void setPickSensorZ(double pickSensorZ)
    {
        
        if (qFuzzyCompare(m_pickSensorZ, pickSensorZ))
            return;

        m_pickSensorZ = pickSensorZ;
        emit pickSensorZChanged(m_pickSensorZ);
    }

    void setPlaceSUT1SensorZ(double placeSUT1SensorZ)
    {
        
        if (qFuzzyCompare(m_placeSUT1SensorZ, placeSUT1SensorZ))
            return;

        m_placeSUT1SensorZ = placeSUT1SensorZ;
        emit placeSUT1SensorZChanged(m_placeSUT1SensorZ);
    }

    void setZOffset(double zOffset)
    {
        
        if (qFuzzyCompare(m_zOffset, zOffset))
            return;

        m_zOffset = zOffset;
        emit zOffsetChanged(m_zOffset);
    }

    void setPickSUT1NgSensorZ(double pickSUT1NgSensorZ)
    {
        
        if (qFuzzyCompare(m_pickSUT1NgSensorZ, pickSUT1NgSensorZ))
            return;

        m_pickSUT1NgSensorZ = pickSUT1NgSensorZ;
        emit pickSUT1NgSensorZChanged(m_pickSUT1NgSensorZ);
    }

    void setPlaceNgSensorZ(double placeNgSensorZ)
    {
        
        if (qFuzzyCompare(m_placeNgSensorZ, placeNgSensorZ))
            return;

        m_placeNgSensorZ = placeNgSensorZ;
        emit placeNgSensorZChanged(m_placeNgSensorZ);
    }

    void setCalibrationGlassLocationName(QString calibrationGlassLocationName)
    {
        if (m_calibrationGlassLocationName == calibrationGlassLocationName)
            return;

        m_calibrationGlassLocationName = calibrationGlassLocationName;
        emit calibrationGlassLocationNameChanged(m_calibrationGlassLocationName);
    }

    void setEscapeHeight(double escapeHeight)
    {
        
        if (qFuzzyCompare(m_escapeHeight, escapeHeight))
            return;

        m_escapeHeight = escapeHeight;
        emit escapeHeightChanged(m_escapeHeight);
    }

    void setEscapeX(double escapeX)
    {
        
        if (qFuzzyCompare(m_escapeX, escapeX))
            return;

        m_escapeX = escapeX;
        emit escapeXChanged(m_escapeX);
    }

    void setEscapeY(double escapeY)
    {
        
        if (qFuzzyCompare(m_escapeY, escapeY))
            return;

        m_escapeY = escapeY;
        emit escapeYChanged(m_escapeY);
    }

    void setChangeTrayTimeOut(int changeTrayTimeOut)
    {
        if (m_changeTrayTimeOut == changeTrayTimeOut)
            return;

        m_changeTrayTimeOut = changeTrayTimeOut;
        emit changeTrayTimeOutChanged(m_changeTrayTimeOut);
    }
    void setUsePlan(bool usePlan)
    {
        if (m_usePlan == usePlan)
            return;

        m_usePlan = usePlan;
        emit usePlanChanged(m_usePlan);
    }

    void setPlanNumber(int planNumber)
    {
        if (m_planNumber == planNumber)
            return;

        m_planNumber = planNumber;
        emit planNumberChanged(m_planNumber);
    }

    void setAccumulatedHour(double accumulatedHour)
    {
        
        if (qFuzzyCompare(m_accumulatedHour, accumulatedHour))
            return;

        m_accumulatedHour = accumulatedHour;
        emit accumulatedHourChanged(m_accumulatedHour);
    }

    void setPlaceNgProductZ(double placeNgProductZ)
    {
        
        if (qFuzzyCompare(m_placeNgProductZ, placeNgProductZ))
            return;

        m_placeNgProductZ = placeNgProductZ;
        emit placeNgProductZChanged(m_placeNgProductZ);
    }

    void setPickProductForce(double pickProductForce)
    {
        
        if (qFuzzyCompare(m_pickProductForce, pickProductForce))
            return;

        m_pickProductForce = pickProductForce;
        emit pickProductForceChanged(m_pickProductForce);
    }

    void setUseSensorOffset(bool useSensorOffset)
    {
        if (m_useSensorOffset == useSensorOffset)
            return;

        m_useSensorOffset = useSensorOffset;
        emit useSensorOffsetChanged(m_useSensorOffset);
    }

    void setSensorOffsetX(double sensorOffsetX)
    {
        
        if (qFuzzyCompare(m_sensorOffsetX, sensorOffsetX))
            return;

        m_sensorOffsetX = sensorOffsetX;
        emit sensorOffsetXChanged(m_sensorOffsetX);
    }

    void setSensorOffsetY(double sensorOffsetY)
    {
        
        if (qFuzzyCompare(m_sensorOffsetY, sensorOffsetY))
            return;

        m_sensorOffsetY = sensorOffsetY;
        emit sensorOffsetYChanged(m_sensorOffsetY);
    }

    void setRepeatTime(int repeatTime)
    {
        if (m_repeatTime == repeatTime)
            return;

        m_repeatTime = repeatTime;
        emit repeatTimeChanged(m_repeatTime);
    }

    void setTestSensorCount(int testSensorCount)
    {
        if (m_testSensorCount == testSensorCount)
            return;

        m_testSensorCount = testSensorCount;
        emit testSensorCountChanged(m_testSensorCount);
    }

    void setStaticTest(int staticTest)
    {
        if (m_staticTest == staticTest)
            return;

        m_staticTest = staticTest;
        emit staticTestChanged(m_staticTest);
    }

    void setEnableEscape(bool enableEscape)
    {
        if (m_enableEscape == enableEscape)
            return;

        m_enableEscape = enableEscape;
        emit enableEscapeChanged(m_enableEscape);
    }

    void setEnableForceLimit(bool enableForceLimit)
    {
        if (m_enableForceLimit == enableForceLimit)
            return;

        m_enableForceLimit = enableForceLimit;
        emit enableForceLimitChanged(m_enableForceLimit);
    }

    void setEnableNgSensorPr(bool enableNgSensorPr)
    {
        if (m_enableNgSensorPr == enableNgSensorPr)
            return;

        m_enableNgSensorPr = enableNgSensorPr;
        emit enableNgSensorPrChanged(m_enableNgSensorPr);
    }

    void setEnableProductPr(bool enableProductPr)
    {
        if (m_enableProductPr == enableProductPr)
            return;

        m_enableProductPr = enableProductPr;
        emit enableProductPrChanged(m_enableProductPr);
    }

    void setEnableNgProductPr(bool enableNgProductPr)
    {
        if (m_enableNgProductPr == enableNgProductPr)
            return;

        m_enableNgProductPr = enableNgProductPr;
        emit enableNgProductPrChanged(m_enableNgProductPr);
    }

    void setEnablePlaceProdcuPr(bool enableProdcutTrayPr)
    {
        if (m_enablePlaceProdcutPr == enableProdcutTrayPr)
            return;

        m_enablePlaceProdcutPr = enableProdcutTrayPr;
        emit enablePlaceProdcutPrChanged(m_enablePlaceProdcutPr);
    }

    void setEnablePlaceNgProductPr(bool enableNgProductTrayPr)
    {
        if (m_enablePlaceNgProductPr == enableNgProductTrayPr)
            return;

        m_enablePlaceNgProductPr = enableNgProductTrayPr;
        emit enablePlacceNgProductPrChanged(m_enablePlaceNgProductPr);
    }

    void setEnablePlaceNgSensorPr(bool enableNgSensorTrayPr)
    {
        if (m_enablePlaceNgSensorPr == enableNgSensorTrayPr)
            return;

        m_enablePlaceNgSensorPr = enableNgSensorTrayPr;
        emit enablePlaceNgSensorPrChanged(m_enablePlaceNgSensorPr);
    }

    void setAutoPrTime(int autoPrTime)
    {
        if (m_autoPrTime == autoPrTime)
            return;

        m_autoPrTime = autoPrTime;
        emit autoPrTimeChanged(m_autoPrTime);
    }

    void setEnablePlaceForce(bool enablePlaceForce)
    {
        if (m_enablePlaceForce == enablePlaceForce)
            return;

        m_enablePlaceForce = enablePlaceForce;
        emit enablePlaceForceChanged(m_enablePlaceForce);
    }

    void setSut1Theta(double sut1Theta)
    {
        
        if (qFuzzyCompare(m_sut1Theta, sut1Theta))
            return;

        m_sut1Theta = sut1Theta;
        emit sut1ThetaChanged(m_sut1Theta);
    }

    void setSut2Theta(double sut2Theta)
    {
        
        if (qFuzzyCompare(m_sut2Theta, sut2Theta))
            return;

        m_sut2Theta = sut2Theta;
        emit sut2ThetaChanged(m_sut2Theta);
    }

    void setEnableSutForceLimit(bool enableSutForceLimit)
    {
        if (m_enableSutForceLimit == enableSutForceLimit)
            return;

        m_enableSutForceLimit = enableSutForceLimit;
        emit enableSutForceLimitChanged(m_enableSutForceLimit);
    }

    void setPlaceTrayMargin(double placeTrayMargin)
    {
        
        if (qFuzzyCompare(m_placeTrayMargin, placeTrayMargin))
            return;

        m_placeTrayMargin = placeTrayMargin;
        emit placeTrayMarginChanged(m_placeTrayMargin);
    }

    void setSutMargin(double sutMargin)
    {
        
        if (qFuzzyCompare(m_sutMargin, sutMargin))
            return;

        m_sutMargin = sutMargin;
        emit sutMarginChanged(m_sutMargin);
    }

    void setOpenTimeLog(bool openTimeLog)
    {
        if (m_openTimeLog == openTimeLog)
            return;

        m_openTimeLog = openTimeLog;
        emit openTimeLogChanged(m_openTimeLog);
    }

    void setPickSensorZ2(double pickSensorZ2)
    {
        
        if (qFuzzyCompare(m_pickSensorZ2, pickSensorZ2))
            return;

        m_pickSensorZ2 = pickSensorZ2;
        emit pickSensorZ2Changed(m_pickSensorZ2);
    }

    void setPlaceProductZ2(double placeProductZ2)
    {
        
        if (qFuzzyCompare(m_placeProductZ2, placeProductZ2))
            return;

        m_placeProductZ2 = placeProductZ2;
        emit placeProductZ2Changed(m_placeProductZ2);
    }

    void setPlaceBufferProductZ(double placeBufferProductZ)
    {
        
        if (qFuzzyCompare(m_placeBufferProductZ, placeBufferProductZ))
            return;

        m_placeBufferProductZ = placeBufferProductZ;
        emit placeBufferProductZChanged(m_placeBufferProductZ);
    }

    void setDisablePickFromTrayForceLimit(bool disablePickFromTrayForceLimit)
    {
        if (m_disablePickFromTrayForceLimit == disablePickFromTrayForceLimit)
            return;

        m_disablePickFromTrayForceLimit = disablePickFromTrayForceLimit;
        emit disablePickFromTrayForceLimitChanged(m_disablePickFromTrayForceLimit);
    }

    void setDisableBackToTrayForceLimit(bool disableBackToTrayForceLimit)
    {
        if (m_disableBackToTrayForceLimit == disableBackToTrayForceLimit)
            return;

        m_disableBackToTrayForceLimit = disableBackToTrayForceLimit;
        emit disableBackToTrayForceLimitChanged(m_disableBackToTrayForceLimit);
    }

    void setDisablePlaceToSutForceLimmit(bool disablePlaceToSutForceLimmit)
    {
        if (m_disablePlaceToSutForceLimmit == disablePlaceToSutForceLimmit)
            return;

        m_disablePlaceToSutForceLimmit = disablePlaceToSutForceLimmit;
        emit disablePlaceToSutForceLimmitChanged(m_disablePlaceToSutForceLimmit);
    }

    void setDisablePickFromSutForceLimit(bool disablePickFromSutForceLimit)
    {
        if (m_disablePickFromSutForceLimit == disablePickFromSutForceLimit)
            return;

        m_disablePickFromSutForceLimit = disablePickFromSutForceLimit;
        emit disablePickFromSutForceLimitChanged(m_disablePickFromSutForceLimit);
    }

    void setDisablePlaceToGoodTrayForceLimit(bool disablePlaceToGoodTrayForceLimit)
    {
        if (m_disablePlaceToGoodTrayForceLimit == disablePlaceToGoodTrayForceLimit)
            return;

        m_disablePlaceToGoodTrayForceLimit = disablePlaceToGoodTrayForceLimit;
        emit disablePlaceToGoodTrayForceLimitChanged(m_disablePlaceToGoodTrayForceLimit);
    }

    void setDisablePlaceToNgTrayForceLimit(bool disablePlaceToNgTrayForceLimit)
    {
        if (m_disablePlaceToNgTrayForceLimit == disablePlaceToNgTrayForceLimit)
            return;

        m_disablePlaceToNgTrayForceLimit = disablePlaceToNgTrayForceLimit;
        emit disablePlaceToNgTrayForceLimitChanged(m_disablePlaceToNgTrayForceLimit);
    }

    void setPickFromTrayMargin(double pickFromTrayMargin)
    {
        
        if (qFuzzyCompare(m_pickFromTrayMargin, pickFromTrayMargin))
            return;

        m_pickFromTrayMargin = pickFromTrayMargin;
        emit pickFromTrayMarginChanged(m_pickFromTrayMargin);
    }

    void setBackToTrayMargin(double backToTrayMargin)
    {
        
        if (qFuzzyCompare(m_backToTrayMargin, backToTrayMargin))
            return;

        m_backToTrayMargin = backToTrayMargin;
        emit backToTrayMarginChanged(m_backToTrayMargin);
    }

    void setPlaceToSut1Margin(double placeToSut1Margin)
    {
        
        if (qFuzzyCompare(m_placeToSut1Margin, placeToSut1Margin))
            return;

        m_placeToSut1Margin = placeToSut1Margin;
        emit placeToSut1MarginChanged(m_placeToSut1Margin);
    }

    void setPickFromSut1Margin(double pickFromSut1Margin)
    {
        
        if (qFuzzyCompare(m_pickFromSut1Margin, pickFromSut1Margin))
            return;

        m_pickFromSut1Margin = pickFromSut1Margin;
        emit pickFromSut1MarginChanged(m_pickFromSut1Margin);
    }

    void setPlaceToSut2Margin(double placeToSut2Margin)
    {

        if (qFuzzyCompare(m_placeToSut2Margin, placeToSut2Margin))
            return;

        m_placeToSut2Margin = placeToSut2Margin;
        emit placeToSut2MarginChanged(m_placeToSut2Margin);
    }

    void setPickFromSut2Margin(double pickFromSut2Margin)
    {

        if (qFuzzyCompare(m_pickFromSut2Margin, pickFromSut2Margin))
            return;

        m_pickFromSut2Margin = pickFromSut2Margin;
        emit pickFromSut2MarginChanged(m_pickFromSut2Margin);
    }

    void setPlaceToGoodTrayMargin(double placeToGoodTrayMargin)
    {
        
        if (qFuzzyCompare(m_placeToGoodTrayMargin, placeToGoodTrayMargin))
            return;

        m_placeToGoodTrayMargin = placeToGoodTrayMargin;
        emit placeToGoodTrayMarginChanged(m_placeToGoodTrayMargin);
    }

    void setPlaceToNgTrayMargin(double placeToNgTrayMargin)
    {
        
        if (qFuzzyCompare(m_placeToNgTrayMargin, placeToNgTrayMargin))
            return;

        m_placeToNgTrayMargin = placeToNgTrayMargin;
        emit placeToNgTrayMarginChanged(m_placeToNgTrayMargin);
    }

    void setEnableBufferProductPr(bool enableBufferProductPr)
    {
        if (m_enableBufferProductPr == enableBufferProductPr)
            return;

        m_enableBufferProductPr = enableBufferProductPr;
        emit enableBufferProductPrChanged(m_enableBufferProductPr);
    }

    void setVacuumOperationOutTime(int vacuumOperationOutTime)
    {
        if (m_vacuumOperationOutTime == vacuumOperationOutTime)
            return;

        m_vacuumOperationOutTime = vacuumOperationOutTime;
        emit vacuumOperationOutTimeChanged(m_vacuumOperationOutTime);
    }

    void setPicker1PickTheta(double picker1PickTheta)
    {
        
        if (qFuzzyCompare(m_picker1PickTheta, picker1PickTheta))
            return;

        m_picker1PickTheta = picker1PickTheta;
        emit picker1PickThetaChanged(m_picker1PickTheta);
    }

    void setPicker2PickTheta(double picker2PickTheta)
    {
        
        if (qFuzzyCompare(m_picker2PickTheta, picker2PickTheta))
            return;

        m_picker2PickTheta = picker2PickTheta;
        emit picker2PickThetaChanged(m_picker2PickTheta);
    }

    void setPicker1PlaceTheta(double picker1PlaceTheta)
    {
        
        if (qFuzzyCompare(m_picker1PlaceTheta, picker1PlaceTheta))
            return;

        m_picker1PlaceTheta = picker1PlaceTheta;
        emit picker1PlaceThetaChanged(m_picker1PlaceTheta);
    }

    void setPicker2PlaceTheta(double picker2PlaceTheta)
    {
        
        if (qFuzzyCompare(m_picker2PlaceTheta, picker2PlaceTheta))
            return;

        m_picker2PlaceTheta = picker2PlaceTheta;
        emit picker2PlaceThetaChanged(m_picker2PlaceTheta);
    }

    void setHandlyChangeSensor(bool handlyChangeSensor)
    {
        if (m_handlyChangeSensor == handlyChangeSensor)
            return;

        m_handlyChangeSensor = handlyChangeSensor;
        emit handlyChangeSensorChanged(m_handlyChangeSensor);
    }

    void setHoldTime(int holdTime)
    {
        if (m_holdTime == holdTime)
            return;

        m_holdTime = holdTime;
        emit holdTimeChanged(m_holdTime);
    }

    void setModuleName(QString moduleName)
    {
        if (m_moduleName == moduleName)
            return;

        m_moduleName = moduleName;
        emit moduleNameChanged(m_moduleName);
    }

    void setPlaceSUT2SensorZ(double placeSUT2SensorZ)
    {
        if (qFuzzyCompare(m_placeSUT2SensorZ, placeSUT2SensorZ))
            return;

        m_placeSUT2SensorZ = placeSUT2SensorZ;
        emit placeSUT2SensorZChanged(m_placeSUT2SensorZ);
    }

    void setPickSUT2ProductZ(double pickSUT2ProductZ)
    {
        if (qFuzzyCompare(m_pickSUT2ProductZ, pickSUT2ProductZ))
            return;

        m_pickSUT2ProductZ = pickSUT2ProductZ;
        emit pickSUT2ProductZChanged(m_pickSUT2ProductZ);
    }

    void setPickSUT2NgSensorZ(double pickSUT2NgSensorZ)
    {
        if (qFuzzyCompare(m_pickSUT2NgSensorZ, pickSUT2NgSensorZ))
            return;

        m_pickSUT2NgSensorZ = pickSUT2NgSensorZ;
        emit pickSUT2NgSensorZChanged(m_pickSUT2NgSensorZ);
    }

signals:
    void vcmWorkForceChanged(double vcmWorkForce);
    void vcmWorkSpeedChanged(double vcmWorkSpeed);

    void vcmMarginChanged(double vcmMargin);


    void sensorLocationNameChanged(QString sensorLocationName);

    void vacancyLocationNameChanged(QString vacancyLocationName);

    void sutLocationNameChanged(QString sutLocationName);

    void sutSensorLocationNameChanged(QString sutSensorLocationName);

    void sutProductLocationNameChanged(QString sutProductLocationName);

    void finishDelayChanged(int finishDelay);

    void pickSUT1ProductZChanged(double pickSUT1ProductZ);

    void placeProductZChanged(double placeProductZ);

    void pickSensorZChanged(double pickSensorZ);

    void placeSUT1SensorZChanged(double placeSUT1SensorZ);

    void zOffsetChanged(double zOffset);

    void pickSUT1NgSensorZChanged(double pickSUT1NgSensorZ);

    void placeNgSensorZChanged(double placeNgSensorZ);

    void calibrationGlassLocationNameChanged(QString calibrationGlassLocationName);

    void escapeHeightChanged(double escapeHeight);

    void escapeXChanged(double escapeX);

    void escapeYChanged(double escapeY);


    void changeTrayTimeOutChanged(int changeTrayTimeOut);

    void usePlanChanged(bool usePlan);

    void planNumberChanged(int planNumber);

    void accumulatedHourChanged(double accumulatedHour);

    void placeNgProductZChanged(double placeNgProductZ);

    void pickProductForceChanged(double pickProductForce);

    void useSensorOffsetChanged(bool useSensorOffset);

    void sensorOffsetXChanged(double sensorOffsetX);

    void sensorOffsetYChanged(double sensorOffsetY);

    void repeatTimeChanged(int repeatTime);

    void testSensorCountChanged(int testSensorCount);

    void staticTestChanged(int staticTest);

    void enableEscapeChanged(bool enableEscape);

    void enableForceLimitChanged(bool enableForceLimit);

    void enableNgSensorPrChanged(bool enableNgSensorPr);

    void enableProductPrChanged(bool enableProductPr);

    void enableNgProductPrChanged(bool enableNgProductPr);

    void enablePlaceProdcutPrChanged(bool enablePlaceProdcutPr);

    void enablePlacceNgProductPrChanged(bool enablePlaceNgProductPr);

    void enablePlaceNgSensorPrChanged(bool enablePlaceNgSensorPr);

    void autoPrTimeChanged(int autoPrTime);

    void enablePlaceForceChanged(bool enablePlaceForce);

    void sut1ThetaChanged(double sut1Theta);

    void sut2ThetaChanged(double sut2Theta);

    void enableSutForceLimitChanged(bool enableSutForceLimit);

    void placeTrayMarginChanged(double placeTrayMargin);

    void sutMarginChanged(double sutMargin);

    void openTimeLogChanged(bool openTimeLog);

    void pickSensorZ2Changed(double pickSensorZ2);

    void placeProductZ2Changed(double placeProductZ2);

    void placeBufferProductZChanged(double placeBufferProductZ);

    void vacuumOperationOutTimeChanged(int vacuumOperationOutTime);

    void disablePickFromTrayForceLimitChanged(bool disablePickFromTrayForceLimit);

    void disableBackToTrayForceLimitChanged(bool disableBackToTrayForceLimit);

    void disablePlaceToSutForceLimmitChanged(bool disablePlaceToSutForceLimmit);

    void disablePickFromSutForceLimitChanged(bool disablePickFromSutForceLimit);

    void disablePlaceToGoodTrayForceLimitChanged(bool disablePlaceToGoodTrayForceLimit);

    void disablePlaceToNgTrayForceLimitChanged(bool disablePlaceToNgTrayForceLimit);

    void pickFromTrayMarginChanged(double pickFromTrayMargin);

    void backToTrayMarginChanged(double backToTrayMargin);

    void placeToSut1MarginChanged(double placeToSut1Margin);

    void pickFromSut1MarginChanged(double pickFromSut1Margin);

    void placeToSut2MarginChanged(double placeToSut2Margin);

    void pickFromSut2MarginChanged(double pickFromSut2Margin);

    void placeToGoodTrayMarginChanged(double placeToGoodTrayMargin);

    void placeToNgTrayMarginChanged(double placeToNgTrayMargin);

    void enableBufferProductPrChanged(bool enableBufferProductPr);

    void picker1PickThetaChanged(double picker1PickTheta);

    void picker2PickThetaChanged(double picker2PickTheta);

    void picker1PlaceThetaChanged(double picker1PlaceTheta);

    void picker2PlaceThetaChanged(double picker2PlaceTheta);

    void handlyChangeSensorChanged(bool handlyChangeSensor);

    void holdTimeChanged(int holdTime);

    void moduleNameChanged(QString moduleName);

    void placeSUT2SensorZChanged(double placeSUT2SensorZ);

    void pickSUT2ProductZChanged(double pickSUT2ProductZ);

    void pickSUT2NgSensorZChanged(double pickSUT2NgSensorZ);

private:
    QString m_moduleName = "SensorLoaderModule";
    double m_vcmWorkForce = 0;
    double m_vcmWorkSpeed = 0;
    double m_vcmMargin = 0;
    double m_pickSensorZ = 0;
    double m_placeSUT1SensorZ = 0;
    double m_placeSUT2SensorZ = 0;
    QString m_sensorLocationName = "";
    QString m_vacancyLocationName = "";
    QString m_sutLocationName = "";
    QString m_sutSensorLocationName = "";
    QString m_sutProductLocationName = "";
    int m_finishDelay = 100;
    double m_pickSUT1ProductZ = 0;
    double m_pickSUT2ProductZ = 0;
    double m_placeProductZ = 0;
    double m_zOffset = 0;
    double m_pickSUT1NgSensorZ = 0;
    double m_pickSUT2NgSensorZ = 0;
    double m_placeNgSensorZ = 0;
    QString m_calibrationGlassLocationName;
    double m_escapeHeight = 3;
    double m_escapeX = 5;
    double m_escapeY = 0;
    int m_changeTrayTimeOut = 180000;
    bool m_usePlan = false;
    int m_planNumber = 0;
    double m_accumulatedHour = 0;
    double m_placeNgProductZ = 0;
    double m_pickProductForce = 100;
    bool m_useSensorOffset = false;
    double m_sensorOffsetX = 4.8;
    double m_sensorOffsetY = 0;
    int m_repeatTime = 10;
    int m_testSensorCount = 10;
    int m_staticTest = false;
    bool m_enableEscape = false;
    bool m_enableForceLimit = false;
    bool m_enableNgSensorPr = false;
    bool m_enableProductPr = false;
    bool m_enableNgProductPr = false;
    bool m_enablePlaceProdcutPr = false;
    bool m_enablePlaceNgProductPr = false;
    bool m_enablePlaceNgSensorPr = false;
    int m_autoPrTime = 5;
    bool m_enablePlaceForce = false;
    double m_sut1Theta = 0;
    double m_sut2Theta = 0;
    bool m_enableSutForceLimit = false;
    double m_placeTrayMargin = 2;
    double m_sutMargin = 0;
    bool m_openTimeLog = false;
    double m_pickSensorZ2 = 0;
    double m_placeProductZ2 = 0;
    double m_placeBufferProductZ = 0;
    int m_vacuumOperationOutTime = 500;
    bool m_disablePickFromTrayForceLimit = false;
    bool m_disableBackToTrayForceLimit = false;
    bool m_disablePlaceToSutForceLimmit = false;
    bool m_disablePickFromSutForceLimit = false;
    bool m_disablePlaceToGoodTrayForceLimit = false;
    bool m_disablePlaceToNgTrayForceLimit = false;
    double m_pickFromTrayMargin = 0;
    double m_backToTrayMargin = 0;
    double m_placeToSut1Margin = 0;
    double m_pickFromSut1Margin = 0;
    double m_placeToSut2Margin = 0;
    double m_pickFromSut2Margin = 0;
    double m_placeToGoodTrayMargin = 0;
    double m_placeToNgTrayMargin = 0;
    bool m_enableBufferProductPr = false;
    double m_picker1PickTheta = 0;
    double m_picker2PickTheta = 0;
    double m_picker1PlaceTheta = 0;
    double m_picker2PlaceTheta = 0;
    bool m_handlyChangeSensor = false;
    int m_holdTime = 200;
};
class SensorLoaderState:public PropertyBase
{
    Q_OBJECT
public:
    SensorLoaderState():PropertyBase (){init_values.insert("crossTrayPlaceProduct",true);}
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(bool disableStation1 READ disableStation1 WRITE setDisableStation1 NOTIFY disableStation1Changed)
    Q_PROPERTY(bool disableStation2 READ disableStation2 WRITE setDisableStation2 NOTIFY disableStation2Changed)
    Q_PROPERTY(int taskOfStation1 READ taskOfStation1 WRITE setTaskOfStation1 NOTIFY taskOfStation1Changed)
    Q_PROPERTY(int taskOfStation2 READ taskOfStation2 WRITE setTaskOfStation2 NOTIFY taskOfStation2Changed)
    Q_PROPERTY(bool handlyChangeSensor READ handlyChangeSensor WRITE setHandlyChangeSensor NOTIFY handlyChangeSensorChanged)
    Q_PROPERTY(bool hasSensorTray1 READ hasSensorTray1 WRITE setHasSensorTray1 NOTIFY hasSensorTray1Changed)
    Q_PROPERTY(bool hasSensorTray2 READ hasSensorTray2 WRITE setHasSensorTray2 NOTIFY hasSensorTray2Changed)
    Q_PROPERTY(bool allowChangeTray READ allowChangeTray WRITE setAllowChangeTray NOTIFY allowChangeTrayChanged)
    Q_PROPERTY(bool waitingChangeTray READ waitingChangeTray WRITE setWaitingChangeTray NOTIFY waitingChangeTrayChanged)
    Q_PROPERTY(bool finishChangeTray READ finishChangeTray WRITE setFinishChangeTray NOTIFY finishChangeTrayChanged)
    Q_PROPERTY(int sut1MaterialState READ sut1MaterialState WRITE setSut1MaterialState NOTIFY sut1MaterialStateChanged)
    Q_PROPERTY(int sut2MaterialState READ sut2MaterialState WRITE setSut2MaterialState NOTIFY sut2MaterialStateChanged)
    Q_PROPERTY(int picker1MaterialState READ picker1MaterialState WRITE setPicker1MaterialState NOTIFY picker1MaterialStateChanged)
    Q_PROPERTY(int picker2MaterialState READ picker2MaterialState WRITE setPicker2MaterialState NOTIFY picker2MaterialStateChanged)
    Q_PROPERTY(int busyState READ busyState WRITE setBusyState NOTIFY busyStateChanged)
    Q_PROPERTY(int lastState READ lastState WRITE setLastState NOTIFY lastStateChanged)
    Q_PROPERTY(bool station1NeedSensor READ station1NeedSensor WRITE setStation1NeedSensor NOTIFY station1NeedSensorChanged)
    Q_PROPERTY(bool station2NeedSensor READ station2NeedSensor WRITE setStation2NeedSensor NOTIFY station2NeedSensorChanged)
    Q_PROPERTY(bool station1HasRequest READ station1HasRequest WRITE setStation1HasRequest NOTIFY station1HasRequestChanged)
    Q_PROPERTY(bool station2HasRequest READ station2HasRequest WRITE setStation2HasRequest NOTIFY station2HasRequestChanged)
    Q_PROPERTY(bool finishStation1Task READ finishStation1Task WRITE setFinishStation1Task NOTIFY finishStation1TaskChanged)
    Q_PROPERTY(bool finishStation2Task READ finishStation2Task WRITE setFinishStation2Task NOTIFY finishStation2TaskChanged)
    Q_PROPERTY(bool sut1Ready READ sut1Ready WRITE setSut1Ready NOTIFY sut1ReadyChanged)
    Q_PROPERTY(bool sut2Ready READ sut2Ready WRITE setSut2Ready NOTIFY sut2ReadyChanged)
    Q_PROPERTY(bool finishVacuumOperation READ finishVacuumOperation WRITE setFinishVacuumOperation NOTIFY finishVacuumOperationChanged)
    Q_PROPERTY(int vacuumOperationResult READ vacuumOperationResult WRITE setVacuumOperationResult NOTIFY vacuumOperationResultChanged)
    Q_PROPERTY(int currentTrayID READ currentTrayID WRITE setCurrentTrayID NOTIFY currentTrayIDChanged)
    Q_PROPERTY(bool handlyChangeTray READ handlyChangeTray WRITE setHandlyChangeTray NOTIFY handlyChangeTrayChanged)
    Q_PROPERTY(bool station1Unload READ station1Unload WRITE setStation1Unload NOTIFY station1UnloadChanged)
    Q_PROPERTY(bool station2Unload READ station2Unload WRITE setStation2Unload NOTIFY station2UnloadChanged)
    Q_PROPERTY(bool crossTrayPlaceProduct READ crossTrayPlaceProduct WRITE setCrossTrayPlaceProduct NOTIFY crossTrayPlaceProductChanged)
    Q_PROPERTY(bool hasNgTrayPrOffset READ hasNgTrayPrOffset WRITE setHasNgTrayPrOffset NOTIFY hasNgTrayPrOffsetChanged)
    Q_PROPERTY(bool hasBufferTrayPrOffset READ hasBufferTrayPrOffset WRITE setHasBufferTrayPrOffset NOTIFY hasBufferTrayPrOffsetChanged)
    Q_PROPERTY(bool spaVaccum1State READ spaVaccum1State WRITE setSpaVaccum1State NOTIFY spaVaccum1StateChanged)
    Q_PROPERTY(bool spaVaccum2State READ spaVaccum2State WRITE setSpaVaccum2State NOTIFY spaVaccum2StateChanged)
    Q_PROPERTY(bool isLastTray READ isLastTray WRITE setIsLastTray NOTIFY isLastTrayChanged)
    int runMode() const
    {
        return m_runMode;
    }

    bool hasSensorTray1() const
    {
        return m_hasSensorTray1;
    }

    bool hasSensorTray2() const
    {
        return m_hasSensorTray2;
    }

    bool allowChangeTray() const
    {
        return m_allowChangeTray;
    }

    bool waitingChangeTray() const
    {
        return m_waitingChangeTray;
    }

    bool finishChangeTray() const
    {
        return m_finishChangeTray;
    }

    int sut1MaterialState() const
    {
        return m_sut1MaterialState;
    }

    int sut2MaterialState() const
    {
        return m_sut2MaterialState;
    }

    int picker1MaterialState() const
    {
        return m_picker1MaterialState;
    }

    int picker2MaterialState() const
    {
        return m_picker2MaterialState;
    }

    int busyState() const
    {
        return m_busyState;
    }

    bool station1NeedSensor() const
    {
        return m_station1NeedSensor;
    }

    bool station2NeedSensor() const
    {
        return m_station2NeedSensor;
    }

    bool station1HasRequest() const
    {
        return m_station1HasRequest;
    }

    bool station2HasRequest() const
    {
        return m_station2HasRequest;
    }

    bool finishStation1Task() const
    {
        return m_finishStation1Task;
    }

    bool finishStation2Task() const
    {
        return m_finishStation2Task;
    }

    bool sut1Ready() const
    {
        return m_sut1Ready;
    }

    bool sut2Ready() const
    {
        return m_sut2Ready;
    }

    bool finishVacuumOperation() const
    {
        return m_finishVacuumOperation;
    }

    int currentTrayID() const
    {
        return m_currentTrayID;
    }

    bool handlyChangeTray() const
    {
        return m_handlyChangeTray;
    }

    bool station1Unload() const
    {
        return m_station1Unload;
    }

    bool station2Unload() const
    {
        return m_station2Unload;
    }

    bool crossTrayPlaceProduct() const
    {
        return m_crossTrayPlaceProduct;
    }

    bool hasNgTrayPrOffset() const
    {
        return m_hasNgTrayPrOffset;
    }

    bool hasBufferTrayPrOffset() const
    {
        return m_hasBufferTrayPrOffset;
    }

    int lastState() const
    {
        return m_lastState;
    }

    int vacuumOperationResult() const
    {
        return m_vacuumOperationResult;
    }

    bool disableStation1() const
    {
        return m_disableStation1;
    }

    bool disableStation2() const
    {
        return m_disableStation2;
    }

    bool handlyChangeSensor() const
    {
        return m_handlyChangeSensor;
    }

    int taskOfStation1() const
    {
        return m_taskOfStation1;
    }

    int taskOfStation2() const
    {
        return m_taskOfStation2;
    }

    bool spaVaccum1State() const
    {
        return m_spaVaccum1State;
    }

    bool spaVaccum2State() const
    {
        return m_spaVaccum2State;
    }

    bool isLastTray() const
    {
        return m_isLastTray;
    }

public slots:
    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }

    void setHasSensorTray1(bool hasSensorTray1)
    {
        if (m_hasSensorTray1 == hasSensorTray1)
            return;

        m_hasSensorTray1 = hasSensorTray1;
        emit hasSensorTray1Changed(m_hasSensorTray1);
    }

    void setHasSensorTray2(bool hasSensorTray2)
    {
        if (m_hasSensorTray2 == hasSensorTray2)
            return;

        m_hasSensorTray2 = hasSensorTray2;
        emit hasSensorTray2Changed(m_hasSensorTray2);
    }

    void setAllowChangeTray(bool allowChangeTray)
    {
        if (m_allowChangeTray == allowChangeTray)
            return;

        m_allowChangeTray = allowChangeTray;
        emit allowChangeTrayChanged(m_allowChangeTray);
    }

    void setWaitingChangeTray(bool waitingChangeTray)
    {
        if (m_waitingChangeTray == waitingChangeTray)
            return;

        m_waitingChangeTray = waitingChangeTray;
        emit waitingChangeTrayChanged(m_waitingChangeTray);
    }

    void setFinishChangeTray(bool finishChangeTray)
    {
        if (m_finishChangeTray == finishChangeTray)
            return;

        m_finishChangeTray = finishChangeTray;
        emit finishChangeTrayChanged(m_finishChangeTray);
    }

    void setSut1MaterialState(int sut1MaterialState)
    {
        if (m_sut1MaterialState == sut1MaterialState)
            return;

        m_sut1MaterialState = sut1MaterialState;
        emit sut1MaterialStateChanged(m_sut1MaterialState);
    }

    void setSut2MaterialState(int sut2MaterialState)
    {
        if (m_sut2MaterialState == sut2MaterialState)
            return;

        m_sut2MaterialState = sut2MaterialState;
        emit sut2MaterialStateChanged(m_sut2MaterialState);
    }

    void setPicker1MaterialState(int picker1MaterialState)
    {
        if (m_picker1MaterialState == picker1MaterialState)
            return;

        m_picker1MaterialState = picker1MaterialState;
        emit picker1MaterialStateChanged(m_picker1MaterialState);
    }

    void setPicker2MaterialState(int picker2MaterialState)
    {
        if (m_picker2MaterialState == picker2MaterialState)
            return;

        m_picker2MaterialState = picker2MaterialState;
        emit picker2MaterialStateChanged(m_picker2MaterialState);
    }

    void setBusyState(int busyState)
    {
        if (m_busyState == busyState)
            return;

        m_busyState = busyState;
        emit busyStateChanged(m_busyState);
    }

    void setStation1NeedSensor(bool station1NeedSensor)
    {
        if (m_station1NeedSensor == station1NeedSensor)
            return;

        m_station1NeedSensor = station1NeedSensor;
        emit station1NeedSensorChanged(m_station1NeedSensor);
    }

    void setStation2NeedSensor(bool station2NeedSensor)
    {
        if (m_station2NeedSensor == station2NeedSensor)
            return;

        m_station2NeedSensor = station2NeedSensor;
        emit station2NeedSensorChanged(m_station2NeedSensor);
    }

    void setStation1HasRequest(bool station1HasRequest)
    {
        if (m_station1HasRequest == station1HasRequest)
            return;

        m_station1HasRequest = station1HasRequest;
        emit station1HasRequestChanged(m_station1HasRequest);
    }

    void setStation2HasRequest(bool station2HasRequest)
    {
        if (m_station2HasRequest == station2HasRequest)
            return;

        m_station2HasRequest = station2HasRequest;
        emit station2HasRequestChanged(m_station2HasRequest);
    }

    void setFinishStation1Task(bool finishStation1Task)
    {
        if (m_finishStation1Task == finishStation1Task)
            return;

        m_finishStation1Task = finishStation1Task;
        emit finishStation1TaskChanged(m_finishStation1Task);
    }

    void setFinishStation2Task(bool finishStation2Task)
    {
        if (m_finishStation2Task == finishStation2Task)
            return;

        m_finishStation2Task = finishStation2Task;
        emit finishStation2TaskChanged(m_finishStation2Task);
    }

    void setSut1Ready(bool sut1Ready)
    {
        if (m_sut1Ready == sut1Ready)
            return;

        m_sut1Ready = sut1Ready;
        emit sut1ReadyChanged(m_sut1Ready);
    }

    void setSut2Ready(bool sut2Ready)
    {
        if (m_sut2Ready == sut2Ready)
            return;

        m_sut2Ready = sut2Ready;
        emit sut2ReadyChanged(m_sut2Ready);
    }

    void setFinishVacuumOperation(bool finishVacuumOperation)
    {
        if (m_finishVacuumOperation == finishVacuumOperation)
            return;

        m_finishVacuumOperation = finishVacuumOperation;
        emit finishVacuumOperationChanged(m_finishVacuumOperation);
    }

    void setCurrentTrayID(int currentTrayID)
    {
        if (m_currentTrayID == currentTrayID)
            return;

        m_currentTrayID = currentTrayID;
        emit currentTrayIDChanged(m_currentTrayID);
    }

    void setHandlyChangeTray(bool handlyChangeTray)
    {
        if (m_handlyChangeTray == handlyChangeTray)
            return;

        m_handlyChangeTray = handlyChangeTray;
        emit handlyChangeTrayChanged(m_handlyChangeTray);
    }

    void setStation1Unload(bool station1Unload)
    {
        if (m_station1Unload == station1Unload)
            return;

        m_station1Unload = station1Unload;
        emit station1UnloadChanged(m_station1Unload);
    }

    void setStation2Unload(bool station2Unload)
    {
        if (m_station2Unload == station2Unload)
            return;

        m_station2Unload = station2Unload;
        emit station2UnloadChanged(m_station2Unload);
    }

    void setCrossTrayPlaceProduct(bool crossTrayPlaceProduct)
    {
        if (m_crossTrayPlaceProduct == crossTrayPlaceProduct)
            return;

        m_crossTrayPlaceProduct = crossTrayPlaceProduct;
        emit crossTrayPlaceProductChanged(m_crossTrayPlaceProduct);
    }

    void setHasNgTrayPrOffset(bool hasNgTrayPrOffset)
    {
        if (m_hasNgTrayPrOffset == hasNgTrayPrOffset)
            return;

        m_hasNgTrayPrOffset = hasNgTrayPrOffset;
        emit hasNgTrayPrOffsetChanged(m_hasNgTrayPrOffset);
    }

    void setHasBufferTrayPrOffset(bool hasBufferTrayPrOffset)
    {
        if (m_hasBufferTrayPrOffset == hasBufferTrayPrOffset)
            return;

        m_hasBufferTrayPrOffset = hasBufferTrayPrOffset;
        emit hasBufferTrayPrOffsetChanged(m_hasBufferTrayPrOffset);
    }

    void setLastState(int lastState)
    {
        if (m_lastState == lastState)
            return;

        m_lastState = lastState;
        emit lastStateChanged(m_lastState);
    }

    void setVacuumOperationResult(int vacuumOperationResult)
    {
        if (m_vacuumOperationResult == vacuumOperationResult)
            return;

        m_vacuumOperationResult = vacuumOperationResult;
        emit vacuumOperationResultChanged(m_vacuumOperationResult);
    }

    void setDisableStation1(bool disableStation1)
    {
        if (m_disableStation1 == disableStation1)
            return;

        m_disableStation1 = disableStation1;
        emit disableStation1Changed(m_disableStation1);
    }

    void setDisableStation2(bool disableStation2)
    {
        if (m_disableStation2 == disableStation2)
            return;

        m_disableStation2 = disableStation2;
        emit disableStation2Changed(m_disableStation2);
    }

    void setHandlyChangeSensor(bool handlyChangeSensor)
    {
        if (m_handlyChangeSensor == handlyChangeSensor)
            return;

        m_handlyChangeSensor = handlyChangeSensor;
        emit handlyChangeSensorChanged(m_handlyChangeSensor);
    }

    void setTaskOfStation1(int taskOfStation1)
    {
        if (m_taskOfStation1 == taskOfStation1)
            return;

        m_taskOfStation1 = taskOfStation1;
        emit taskOfStation1Changed(m_taskOfStation1);
    }

    void setTaskOfStation2(int taskOfStation2)
    {
        if (m_taskOfStation2 == taskOfStation2)
            return;

        m_taskOfStation2 = taskOfStation2;
        emit taskOfStation2Changed(m_taskOfStation2);
    }

    void setSpaVaccum1State(bool spaVaccum1State)
    {
        if (m_spaVaccum1State == spaVaccum1State)
            return;

        m_spaVaccum1State = spaVaccum1State;
        emit spaVaccum1StateChanged(m_spaVaccum1State);
    }

    void setSpaVaccum2State(bool spaVaccum2State)
    {
        if (m_spaVaccum2State == spaVaccum2State)
            return;

        m_spaVaccum2State = spaVaccum2State;
        emit spaVaccum2StateChanged(m_spaVaccum2State);
    }

    void setIsLastTray(bool isLastTray)
    {
        if (m_isLastTray == isLastTray)
            return;

        m_isLastTray = isLastTray;
        emit isLastTrayChanged(m_isLastTray);
    }

signals:
    void runModeChanged(int runMode);

    void hasSensorTray1Changed(bool hasSensorTray1);

    void hasSensorTray2Changed(bool hasSensorTray2);

    void allowChangeTrayChanged(bool allowChangeTray);

    void waitingChangeTrayChanged(bool waitingChangeTray);

    void finishChangeTrayChanged(bool finishChangeTray);

    void sut1MaterialStateChanged(int sut1MaterialState);

    void sut2MaterialStateChanged(int sut2MaterialState);

    void picker1MaterialStateChanged(int picker1MaterialState);

    void picker2MaterialStateChanged(int picker2MaterialState);

    void busyStateChanged(int busyState);

    void station1NeedSensorChanged(bool station1NeedSensor);

    void station2NeedSensorChanged(bool station2NeedSensor);

    void station1HasRequestChanged(bool station1HasRequest);

    void station2HasRequestChanged(bool station2HasRequest);

    void finishStation1TaskChanged(bool finishStation1Task);

    void finishStation2TaskChanged(bool finishStation2Task);

    void sut1ReadyChanged(bool sut1Ready);

    void sut2ReadyChanged(bool sut2Ready);

    void finishVacuumOperationChanged(bool finishVacuumOperation);

    void currentTrayIDChanged(int currentTrayID);

    void handlyChangeTrayChanged(bool handlyChangeTray);

    void station1UnloadChanged(bool station1Unload);

    void station2UnloadChanged(bool station2Unload);

    void crossTrayPlaceProductChanged(bool crossTrayPlaceProduct);

    void hasNgTrayPrOffsetChanged(bool hasNgTrayPrOffset);

    void hasBufferTrayPrOffsetChanged(bool hasBufferTrayPrOffset);

    void lastStateChanged(int lastState);

    void vacuumOperationResultChanged(int vacuumOperationResult);

    void disableStation1Changed(bool disableStation1);

    void disableStation2Changed(bool disableStation2);

    void handlyChangeSensorChanged(bool handlyChangeSensor);

    void taskOfStation1Changed(int taskOfStation1);

    void taskOfStation2Changed(int taskOfStation2);

    void spaVaccum1StateChanged(bool spaVaccum1State);

    void spaVaccum2StateChanged(bool spaVaccum2State);

    void isLastTrayChanged(bool isLastTray);

private:
    int m_runMode = 0;
    bool m_hasSensorTray1 = false;
    bool m_hasSensorTray2 = false;
    bool m_allowChangeTray = false;
    bool m_waitingChangeTray = false;
    bool m_finishChangeTray = false;
    int m_sut1MaterialState = 0;
    int m_sut2MaterialState = 0;
    int m_picker1MaterialState = 0;
    int m_picker2MaterialState = 0;
    int m_busyState = 0;
    bool m_station1NeedSensor = false;
    bool m_station2NeedSensor = false;
    bool m_station1HasRequest = false;
    bool m_station2HasRequest = false;
    bool m_finishStation1Task = false;
    bool m_finishStation2Task = false;
    bool m_sut1Ready = false;
    bool m_sut2Ready = false;
    bool m_finishVacuumOperation = false;
    int m_currentTrayID = 0;
    bool m_handlyChangeTray = false;
    bool m_station1Unload = false;
    bool m_station2Unload = false;
    bool m_crossTrayPlaceProduct = true;
    bool m_hasNgTrayPrOffset = false;
    bool m_hasBufferTrayPrOffset = false;
    int m_lastState = 0;
    int m_vacuumOperationResult = 0;
    bool m_disableStation1 = false;
    bool m_disableStation2 = false;
    bool m_handlyChangeSensor = false;
    int m_taskOfStation1 = 0;
    int m_taskOfStation2 = 0;
    bool m_spaVaccum1State = 0;
    bool m_spaVaccum2State = 0;
    bool m_isLastTray = false;
};

#endif // SENSORLOADERPARAMETER_H
