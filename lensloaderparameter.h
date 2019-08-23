#ifndef LENSPICKARMMODULEPARAMETER_H
#define LENSPICKARMMODULEPARAMETER_H

#include "propertybase.h"



class LensLoaderModuleParameter:public PropertyBase
{
    Q_OBJECT
public:
    LensLoaderModuleParameter():PropertyBase(){}
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(double vcmWorkForce READ vcmWorkForce WRITE setVcmWorkForce NOTIFY vcmWorkForceChanged)
    Q_PROPERTY(double vcmWorkSpeed READ vcmWorkSpeed WRITE setVcmWorkSpeed NOTIFY vcmWorkSpeedChanged)
    Q_PROPERTY(double vcmMargin READ vcmMargin WRITE setVcmMargin NOTIFY vcmMarginChanged)
    Q_PROPERTY(double pickLensZ READ pickLensZ WRITE setPickLensZ NOTIFY pickLensZChanged)
    Q_PROPERTY(double placeLensZ READ placeLensZ WRITE setPlaceLensZ NOTIFY placeLensZChanged)
    Q_PROPERTY(double pickTheta READ pickTheta WRITE setPickTheta NOTIFY pickThetaChanged)
    Q_PROPERTY(double placeTheta READ placeTheta WRITE setPlaceTheta NOTIFY placeThetaChanged)
    Q_PROPERTY(double visonPositionX READ visonPositionX WRITE setVisonPositionX NOTIFY visonPositionXChanged)
    Q_PROPERTY(QString lensLocationName READ lensLocationName WRITE setLensLocationName NOTIFY lensLocationNameChanged)
    Q_PROPERTY(QString vacancyLocationName READ vacancyLocationName WRITE setVacancyLocationName NOTIFY vacancyLocationNameChanged)
    Q_PROPERTY(QString lutLocationName READ lutLocationName WRITE setLutLocationName NOTIFY lutLocationNameChanged)
    Q_PROPERTY(QString lutLensLocationName READ lutLensLocationName WRITE setLutLensLocationName NOTIFY lutLensLocationNameChanged)
    Q_PROPERTY(QString lpaUpdownlookUpLocationName READ lpaUpdownlookUpLocationName WRITE setLpaUpdownlookUpLocation NOTIFY lpaUpdownlookUpkLocationChanged)
    Q_PROPERTY(QString lpaUpdownlookDownLocationName READ lpaUpdownlookDownLocationName WRITE setLpaupdownlookDownLocationName NOTIFY lpaUpdownlookDownLocaationNameChanged)
    Q_PROPERTY(int finishDelay READ finishDelay WRITE setFinishDelay NOTIFY finishDelayChanged)
    Q_PROPERTY(QString lpaUplookPickerLocationName READ lpaUplookPickerLocationName WRITE setLpaUplookPickerLocationName NOTIFY lpaUplookPickerLocationNameChanged)
    Q_PROPERTY(int changeTrayTimeOut READ changeTrayTimeOut WRITE setChangeTrayTimeOut NOTIFY changeTrayTimeOutChanged)
    Q_PROPERTY(bool usePlan READ usePlan WRITE setUsePlan NOTIFY usePlanChanged)
    Q_PROPERTY(int planNumber READ planNumber WRITE setPlanNumber NOTIFY planNumberChanged)
    Q_PROPERTY(int currentNumber READ currentNumber WRITE setCurrentNumber NOTIFY currentNumberChanged)
    Q_PROPERTY(int currentNgNumber READ currentNgNumber WRITE setCurrentNgNumber NOTIFY currentNgNumberChanged)
    Q_PROPERTY(double lensYield READ lensYield WRITE setLensYield NOTIFY lensYieldChanged)
    Q_PROPERTY(double accumulatredHour READ accumulatredHour WRITE setAccumulatredHour NOTIFY accumulatredHourChanged)
    Q_PROPERTY(int lensUPH READ lensUPH WRITE setLensUPH NOTIFY lensUPHChanged)

    Q_PROPERTY(int repeatTime READ repeatTime WRITE setRepeatTime NOTIFY repeatTimeChanged)
    Q_PROPERTY(int testLensCount READ testLensCount WRITE setTestLensCount NOTIFY testLensCountChanged)
    Q_PROPERTY(bool staticTest READ staticTest WRITE setStaticTest NOTIFY staticTestChanged)
    Q_PROPERTY(bool openTimeLog READ openTimeLog WRITE setOpenTimeLog NOTIFY openTimeLogChanged)
    int runMode() const
    {
        return m_runMode;
    }
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

    double pickLensZ() const
    {
        return m_pickLensZ;
    }

    double placeLensZ() const
    {
        return m_placeLensZ;
    }

    double visonPositionX() const
    {
        return m_visonPositionX;
    }
    QString lensLocationName() const
    {
        return m_lensLocationName;
    }

    QString vacancyLocationName() const
    {
        return m_vacancyLocationName;
    }

    QString lutLocationName() const
    {
        return m_lutLocationName;
    }

    QString lutLensLocationName() const
    {
        return m_lueLensLocationName;
    }
    QString lpaUpdownlookUpLocationName() const
    {
        return m_uplookPickLocation;
    }

    QString lpaUpdownlookDownLocationName() const
    {
        return m_uplookCameraLocaation;
    }

    int finishDelay() const
    {
        return m_finishDelay;
    }

    double placeTheta() const
    {
        return m_placeTheta;
    }

    double pickTheta() const
    {
        return m_pickTheta;
    }

    QString lpaUplookPickerLocationName() const
    {
        return m_lpaUplookPickerLocationName;
    }

    int changeTrayTimeOut() const
    {
        return m_changeTrayTimeOut;
    }

    int planNumber() const
    {
        return m_PlanNumber;
    }

    bool usePlan() const
    {
        return m_usePlan;
    }

    int currentNumber() const
    {
        return m_currentNumber;
    }

    int currentNgNumber() const
    {
        return m_currentNgNumber;
    }

    double lensYield() const
    {
        return m_lensYield;
    }

    double accumulatredHour() const
    {
        return m_accumulatredHour;
    }

    int lensUPH() const
    {
        return m_lensUPH;
    }

    int repeatTime() const
    {
        return m_prTestTime;
    }

    int testLensCount() const
    {
        return m_testLensCount;
    }

    bool staticTest() const
    {
        return m_staticTest;
    }

    bool openTimeLog() const
    {
        return m_openTimeLog;
    }

public slots:
    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }
    void setVcmWorkForce(double vcmWorkForce)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcmWorkForce, vcmWorkForce))
            return;

        m_vcmWorkForce = vcmWorkForce;
        emit vcmWorkForceChanged(m_vcmWorkForce);
    }

    void setVcmWorkSpeed(double vcmWorkSpeed)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcmWorkSpeed, vcmWorkSpeed))
            return;

        m_vcmWorkSpeed = vcmWorkSpeed;
        emit vcmWorkSpeedChanged(m_vcmWorkSpeed);
    }

    void setVcmMargin(double vcmMargin)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcmMargin, vcmMargin))
            return;

        m_vcmMargin = vcmMargin;
        emit vcmMarginChanged(m_vcmMargin);
    }

    void setPickLensZ(double pickLensZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pickLensZ, pickLensZ))
            return;

        m_pickLensZ = pickLensZ;
        emit pickLensZChanged(m_pickLensZ);
    }

    void setPlaceLensZ(double placeLensZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_placeLensZ, placeLensZ))
            return;

        m_placeLensZ = placeLensZ;
        emit placeLensZChanged(m_placeLensZ);
    }

    void setVisonPositionX(double visonPositionX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_visonPositionX, visonPositionX))
            return;

        m_visonPositionX = visonPositionX;
        emit visonPositionXChanged(m_visonPositionX);
    }

    void setLensLocationName(QString lensLocationName)
    {
        if (m_lensLocationName == lensLocationName)
            return;

        m_lensLocationName = lensLocationName;
        emit lensLocationNameChanged(m_lensLocationName);
    }

    void setVacancyLocationName(QString vacancyLocationName)
    {
        if (m_vacancyLocationName == vacancyLocationName)
            return;

        m_vacancyLocationName = vacancyLocationName;
        emit vacancyLocationNameChanged(m_vacancyLocationName);
    }

    void setLutLocationName(QString lutLocationName)
    {
        if (m_lutLocationName == lutLocationName)
            return;

        m_lutLocationName = lutLocationName;
        emit lutLocationNameChanged(m_lutLocationName);
    }

    void setLutLensLocationName(QString lueLensLocationName)
    {
        if (m_lueLensLocationName == lueLensLocationName)
            return;

        m_lueLensLocationName = lueLensLocationName;
        emit lutLensLocationNameChanged(m_lueLensLocationName);
    }

    void setLpaUpdownlookUpLocation(QString uplookPickLocation)
    {
        if (m_uplookPickLocation == uplookPickLocation)
            return;

        m_uplookPickLocation = uplookPickLocation;
        emit lpaUpdownlookUpkLocationChanged(m_uplookPickLocation);
    }

    void setLpaupdownlookDownLocationName(QString uplookCameraLocaation)
    {
        if (m_uplookCameraLocaation == uplookCameraLocaation)
            return;

        m_uplookCameraLocaation = uplookCameraLocaation;
        emit lpaUpdownlookDownLocaationNameChanged(m_uplookCameraLocaation);
    }

    void setFinishDelay(int finishDelay)
    {
        if (m_finishDelay == finishDelay)
            return;

        m_finishDelay = finishDelay;
        emit finishDelayChanged(m_finishDelay);
    }

    void setPlaceTheta(double placeTheta)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_placeTheta, placeTheta))
            return;

        m_placeTheta = placeTheta;
        emit placeThetaChanged(m_placeTheta);
    }

    void setPickTheta(double pickTheta)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pickTheta, pickTheta))
            return;

        m_pickTheta = pickTheta;
        emit pickThetaChanged(m_pickTheta);
    }

    void setLpaUplookPickerLocationName(QString lpaUplookPickerLocationName)
    {
        if (m_lpaUplookPickerLocationName == lpaUplookPickerLocationName)
            return;

        m_lpaUplookPickerLocationName = lpaUplookPickerLocationName;
        emit lpaUplookPickerLocationNameChanged(m_lpaUplookPickerLocationName);
    }

    void setChangeTrayTimeOut(int changeTrayTimeOut)
    {
        if (m_changeTrayTimeOut == changeTrayTimeOut)
            return;

        m_changeTrayTimeOut = changeTrayTimeOut;
        emit changeTrayTimeOutChanged(m_changeTrayTimeOut);
    }

    void setPlanNumber(int PlanNumber)
    {
        if (m_PlanNumber == PlanNumber)
            return;

        m_PlanNumber = PlanNumber;
        emit planNumberChanged(m_PlanNumber);
    }

    void setUsePlan(bool usePlan)
    {
        if (m_usePlan == usePlan)
            return;

        m_usePlan = usePlan;
        emit usePlanChanged(m_usePlan);
    }

    void setCurrentNumber(int currentNumber)
    {
        if (m_currentNumber == currentNumber)
            return;

        m_currentNumber = currentNumber;
        emit currentNumberChanged(m_currentNumber);
    }

    void setCurrentNgNumber(int currentNgNumber)
    {
        if (m_currentNgNumber == currentNgNumber)
            return;

        m_currentNgNumber = currentNgNumber;
        emit currentNgNumberChanged(m_currentNgNumber);
    }

    void setLensYield(double lensYield)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_lensYield, lensYield))
            return;

        m_lensYield = lensYield;
        emit lensYieldChanged(m_lensYield);
    }

    void setAccumulatredHour(double accumulatredHour)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_accumulatredHour, accumulatredHour))
            return;

        m_accumulatredHour = accumulatredHour;
        emit accumulatredHourChanged(m_accumulatredHour);
    }

    void setLensUPH(int lensUPH)
    {
        if (m_lensUPH == lensUPH)
            return;

        m_lensUPH = lensUPH;
        emit lensUPHChanged(m_lensUPH);
    }

    void setRepeatTime(int prTestTime)
    {
        if (m_prTestTime == prTestTime)
            return;

        m_prTestTime = prTestTime;
        emit repeatTimeChanged(m_prTestTime);
    }

    void setTestLensCount(int testLensCount)
    {
        if (m_testLensCount == testLensCount)
            return;

        m_testLensCount = testLensCount;
        emit testLensCountChanged(m_testLensCount);
    }

    void setStaticTest(bool staticTest)
    {
        if (m_staticTest == staticTest)
            return;

        m_staticTest = staticTest;
        emit staticTestChanged(m_staticTest);
    }

    void setOpenTimeLog(bool openTimeLog)
    {
        if (m_openTimeLog == openTimeLog)
            return;

        m_openTimeLog = openTimeLog;
        emit openTimeLogChanged(m_openTimeLog);
    }

signals:
    void runModeChanged(int runMode);
    void vcmWorkForceChanged(double vcmWorkForce);

    void vcmWorkSpeedChanged(double vcmWorkSpeed);

    void vcmMarginChanged(double vcmMargin);

    void pickLensZChanged(double pickLensZ);

    void placeLensZChanged(double placeLensZ);

    void visonPositionXChanged(double visonPositionX);
    void lensLocationNameChanged(QString lensLocationName);

    void vacancyLocationNameChanged(QString vacancyLocationName);

    void lutLocationNameChanged(QString lutLocationName);

    void lutLensLocationNameChanged(QString lutLensLocationName);

    void lpaUpdownlookUpkLocationChanged(QString lpaUpdownlookUpLocationName);

    void lpaUpdownlookDownLocaationNameChanged(QString lpaUpdownlookDownLocationName);

    void finishDelayChanged(int finishDelay);

    void placeThetaChanged(double placeTheta);

    void pickThetaChanged(double pickTheta);

    void lpaUplookPickerLocationNameChanged(QString lpaUplookPickerLocationName);

    void changeTrayTimeOutChanged(int changeTrayTimeOut);

    void planNumberChanged(int planNumber);

    void usePlanChanged(bool usePlan);

    void currentNumberChanged(int currentNumber);

    void currentNgNumberChanged(int currentNgNumber);

    void lensYieldChanged(double lensYield);

    void accumulatredHourChanged(double accumulatredHour);

    void lensUPHChanged(int lensUPH);

    void repeatTimeChanged(int repeatTime);

    void testLensCountChanged(int testLensCount);

    void staticTestChanged(bool staticTest);

    void openTimeLogChanged(bool openTimeLog);

private:
    int m_runMode = 0;
    double m_vcmWorkForce = 0;
    double m_vcmWorkSpeed = 0;
    double m_vcmMargin = 0;
    double m_pickLensZ = 0;
    double m_placeLensZ = 0;
    double m_visonPositionX = 0;
    QString m_lensLocationName = "";
    QString m_vacancyLocationName = "";
    QString m_lutLocationName = "";
    QString m_lueLensLocationName = "";
    QString m_uplookPickLocation = "";
    QString m_uplookCameraLocaation = "";
    int m_finishDelay = 100;
    double m_placeTheta = 0;
    double m_pickTheta = 0;
    QString m_lpaUplookPickerLocationName = "";
    int m_changeTrayTimeOut = 180000;
    int m_PlanNumber = 10;
    bool m_usePlan = false;
    int m_planNumber = 0;
    int m_currentNumber = 0;
    int m_currentNgNumber = 0;
    double m_lensYield = 0;
    double m_accumulatredHour = 0;
    int m_lensUPH = 0;
    int m_prTestTime = 10;
    int m_testLensCount = 10;
    bool m_staticTest = false;
    bool m_openTimeLog = false;
};

class LensPickArmModuleState:public PropertyBase
{
    Q_OBJECT
public:
    LensPickArmModuleState():PropertyBase(){}
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(bool handlyChangeLens READ handlyChangeLens WRITE setHandlyChangeLens NOTIFY handlyChangeLensChanged)
    Q_PROPERTY(bool hasTray READ hasTray WRITE setHasTray NOTIFY hasTrayChanged)
    Q_PROPERTY(bool lutHasLens READ lutHasLens WRITE setLutHasLens NOTIFY lutHasLensChanged)
    Q_PROPERTY(bool lutHasNgLens READ lutHasNgLens WRITE setLutHasNgLens NOTIFY lutHasNgLensChanged)
    Q_PROPERTY(bool needLoadLens READ needLoadLens WRITE setNeedLoadLens NOTIFY needLoadLensChanged)
    Q_PROPERTY(int currentTray READ currentTray WRITE setCurrentTray NOTIFY currentTrayChanged)
    Q_PROPERTY(bool needChangTray READ needChangTray WRITE setNeedChangTray NOTIFY needChangTrayChanged)
    Q_PROPERTY(bool allowChangeTray READ allowChangeTray WRITE setAllowChangeTray NOTIFY allowChangeTrayChanged)
    Q_PROPERTY(bool hasPickedLens READ hasPickedLens WRITE setHasPickedLens NOTIFY hasPickedLensChanged)
    Q_PROPERTY(bool hasPickedNgLens READ hasPickedNgLens WRITE setHasPickedNgLens NOTIFY hasPickedNgLensChanged)
    Q_PROPERTY(int lutTrayID READ lutTrayID WRITE setLutTrayID NOTIFY lutTrayIDChanged)
    Q_PROPERTY(int lutLensID READ lutLensID WRITE setLutLensID NOTIFY lutLensIDChanged)
    Q_PROPERTY(int pickedTrayID READ pickedTrayID WRITE setPickedTrayID NOTIFY pickedTrayIDChanged)
    Q_PROPERTY(int pickedLensID READ pickedLensID WRITE setPickedLensID NOTIFY pickedLensIDChanged)
    Q_PROPERTY(int lutNgTrayID READ lutNgTrayID WRITE setLutNgTrayID NOTIFY lutNgTrayIDChanged)
    Q_PROPERTY(int lutNgLensID READ lutNgLensID WRITE setLutNgLensID NOTIFY lutNgLensIDChanged)
    Q_PROPERTY(bool loadingLens READ loadingLens WRITE setLoadingLens NOTIFY loadingLensChanged)
    Q_PROPERTY(bool waitingChangeTray READ waitingChangeTray WRITE setWaitingChangeTray NOTIFY waitingChangeTrayChanged)
    Q_PROPERTY(bool finishChangeTray READ finishChangeTray WRITE setFinishChangeTray NOTIFY finishChangeTrayChanged)
public:
    bool hasTray() const
    {
        return m_hasTray;
    }

    bool lutHasLens() const
    {
        return m_lutHasLens;
    }

    bool lutHasNgLens() const
    {
        return m_lutHasNgLens;
    }

    bool needLoadLens() const
    {
        return m_needloadLens;
    }

    int currentTray() const
    {
        return m_currentTray;
    }

    bool needChangTray() const
    {
        return m_needChangTray;
    }

    bool allowChangeTray() const
    {
        return m_allowChangeTray;
    }

    bool hasPickedLens() const
    {
        return m_hasPickedLens;
    }

    bool hasPickedNgLens() const
    {
        return m_hasPickedNgLens;
    }

    int lutTrayID() const
    {
        return m_lutTrayID;
    }

    int lutLensID() const
    {
        return m_lutLensID;
    }

    int pickedTrayID() const
    {
        return m_pickedTrayID;
    }

    int pickedLensID() const
    {
        return m_pickedLensID;
    }

    int lutNgTrayID() const
    {
        return m_lutNgTrayID;
    }

    int lutNgLensID() const
    {
        return m_lutNgLensID;
    }

    bool loadingLens() const
    {
        return m_loadingLens;
    }

    bool waitingChangeTray() const
    {
        return m_waitingChangeTray;
    }

    bool finishChangeTray() const
    {
        return m_finishChangeTray;
    }

    int runMode() const
    {
        return m_runMode;
    }

    bool handlyChangeLens() const
    {
        return m_handlyChangeLens;
    }

public slots:
    void setHasTray(bool hasTray)
    {
        if (m_hasTray == hasTray)
            return;

        m_hasTray = hasTray;
        emit hasTrayChanged(m_hasTray);
    }

    void setLutHasLens(bool lutHasLens)
    {
        if (m_lutHasLens == lutHasLens)
            return;

        m_lutHasLens = lutHasLens;
        emit lutHasLensChanged(m_lutHasLens);
    }

    void setLutHasNgLens(bool lutHasNgLens)
    {
        if (m_lutHasNgLens == lutHasNgLens)
            return;

        m_lutHasNgLens = lutHasNgLens;
        emit lutHasNgLensChanged(m_lutHasNgLens);
    }

    void setNeedLoadLens(bool needloadLens)
    {
        if (m_needloadLens == needloadLens)
            return;

        m_needloadLens = needloadLens;
        emit needLoadLensChanged(m_needloadLens);
    }

    void setCurrentTray(int currentTray)
    {
        if (m_currentTray == currentTray)
            return;

        m_currentTray = currentTray;
        emit currentTrayChanged(m_currentTray);
    }

    void setNeedChangTray(bool needChangTray)
    {
        if (m_needChangTray == needChangTray)
            return;

        m_needChangTray = needChangTray;
        emit needChangTrayChanged(m_needChangTray);
    }

    void setAllowChangeTray(bool allowChangeTray)
    {
        if (m_allowChangeTray == allowChangeTray)
            return;

        m_allowChangeTray = allowChangeTray;
        emit allowChangeTrayChanged(m_allowChangeTray);
    }

    void setHasPickedLens(bool hasPickedLens)
    {
        if (m_hasPickedLens == hasPickedLens)
            return;

        m_hasPickedLens = hasPickedLens;
        emit hasPickedLensChanged(m_hasPickedLens);
    }

    void setHasPickedNgLens(bool hasPickedNgLens)
    {
        if (m_hasPickedNgLens == hasPickedNgLens)
            return;

        m_hasPickedNgLens = hasPickedNgLens;
        emit hasPickedNgLensChanged(m_hasPickedNgLens);
    }

    void setLutTrayID(int lutTrayID)
    {
        if (m_lutTrayID == lutTrayID)
            return;

        m_lutTrayID = lutTrayID;
        emit lutTrayIDChanged(m_lutTrayID);
    }

    void setLutLensID(int lutLensID)
    {
        if (m_lutLensID == lutLensID)
            return;

        m_lutLensID = lutLensID;
        emit lutLensIDChanged(m_lutLensID);
    }

    void setPickedTrayID(int pickedTrayID)
    {
        if (m_pickedTrayID == pickedTrayID)
            return;

        m_pickedTrayID = pickedTrayID;
        emit pickedTrayIDChanged(m_pickedTrayID);
    }

    void setPickedLensID(int pickedLensID)
    {
        if (m_pickedLensID == pickedLensID)
            return;

        m_pickedLensID = pickedLensID;
        emit pickedLensIDChanged(m_pickedLensID);
    }

    void setLutNgTrayID(int lutNgTrayID)
    {
        if (m_lutNgTrayID == lutNgTrayID)
            return;

        m_lutNgTrayID = lutNgTrayID;
        emit lutNgTrayIDChanged(m_lutNgTrayID);
    }

    void setLutNgLensID(int lutNgLensID)
    {
        if (m_lutNgLensID == lutNgLensID)
            return;

        m_lutNgLensID = lutNgLensID;
        emit lutNgLensIDChanged(m_lutNgLensID);
    }

    void setLoadingLens(bool loadingLens)
    {
        if (m_loadingLens == loadingLens)
            return;

        m_loadingLens = loadingLens;
        emit loadingLensChanged(m_loadingLens);
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

    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }

    void setHandlyChangeLens(bool handlyChangeLens)
    {
        if (m_handlyChangeLens == handlyChangeLens)
            return;

        m_handlyChangeLens = handlyChangeLens;
        emit handlyChangeLensChanged(m_handlyChangeLens);
    }

signals:
    void hasTrayChanged(bool hasTray);

    void lutHasLensChanged(bool lutHasLens);

    void lutHasNgLensChanged(bool lutHasNgLens);

    void needLoadLensChanged(bool needLoadLens);

    void currentTrayChanged(int currentTray);

    void needChangTrayChanged(bool needChangTray);

    void allowChangeTrayChanged(bool allowChangeTray);

    void hasPickedLensChanged(bool hasPickedLens);

    void hasPickedNgLensChanged(bool hasPickedNgLens);

    void lutTrayIDChanged(int lutTrayID);

    void lutLensIDChanged(int lutLensID);

    void pickedTrayIDChanged(int pickedTrayID);

    void pickedLensIDChanged(int pickedLensID);

    void lutNgTrayIDChanged(int lutNgTrayID);

    void lutNgLensIDChanged(int lutNgLensID);

    void loadingLensChanged(bool loadingLens);

    void waitingChangeTrayChanged(bool waitingChangeTray);

    void finishChangeTrayChanged(bool finishChangeTray);

    void runModeChanged(int runMode);

    void handlyChangeLensChanged(bool handlyChangeLens);

private:
    bool m_hasTray = true;
    bool m_lutHasLens = false;
    bool m_lutHasNgLens = false;
    bool m_needloadLens = false;
    int m_currentTray = false;
    bool m_needChangTray = false;
    bool m_allowChangeTray = false;
    bool m_hasPickedLens = false;
    bool m_hasPickedNgLens = false;
    int m_lutTrayID = -1;
    int m_lutLensID = -1;
    int m_pickedTrayID = -1;
    int m_pickedLensID = -1;
    int m_lutNgTrayID = -1;
    int m_lutNgLensID = -1;
    bool m_loadingLens = false;
    bool m_waitingChangeTray = false;
    bool m_finishChangeTray = false;
    int m_runMode = 0;
    bool m_handlyChangeLens = false;
};

#endif // LENSPICKARMMODULEPARAMETER_H
