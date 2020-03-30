#ifndef LUT_PARAMERTER_H
#define LUT_PARAMERTER_H
#include "propertybase.h"

class LutParameter:public PropertyBase
{
    Q_OBJECT
public:
    LutParameter():PropertyBase(){}
    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)
    Q_PROPERTY(double pickForce READ pickForce WRITE setPickForce NOTIFY paramsChanged)
    Q_PROPERTY(double placeForce READ placeForce WRITE setPlaceForce NOTIFY placeForceChanged)
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(QString motorZName READ motorZName WRITE setMotorZName NOTIFY motorZNameChanged)
    Q_PROPERTY(QString vacuum1Name READ vacuum1Name WRITE setVacuum1Name NOTIFY vacuum1NameChanged)
    Q_PROPERTY(QString vacuum2Name READ vacuum2Name WRITE setVacuum2Name NOTIFY vacuum2NameChanged)
    Q_PROPERTY(QString uplookLocationName READ uplookLocationName WRITE setUplookLocationName NOTIFY uplookLocationNameChanged)
    Q_PROPERTY(QString loadlookLocationName READ loadlookLocationName WRITE setLoadlookLocationName NOTIFY loadlookLocationNameChanged)
    Q_PROPERTY(QString mushroomLocationName READ mushroomLocationName WRITE setMushroomLocationName NOTIFY mushroomLocationNameChanged)
    Q_PROPERTY(double accumulatedHour READ accumulatedHour WRITE setAccumulatedHour NOTIFY accumulatedHourChanged)
    Q_PROPERTY(double lensHeight READ lensHeight WRITE setLensHeight NOTIFY lensHeightChanged)
    Q_PROPERTY(double pickSpeed READ pickSpeed WRITE setPickSpeed NOTIFY pickSpeedChanged)
    Q_PROPERTY(double placeSpeed READ placeSpeed WRITE setPlaceSpeed NOTIFY placeSpeedChanged)
    Q_PROPERTY(int gripperDelay READ gripperDelay WRITE setGripperDelay NOTIFY gripperDelayChanged)
    Q_PROPERTY(int placeGripperDelay READ placeGripperDelay WRITE setPlaceGripperDelay NOTIFY placeGripperDelayChanged)
    Q_PROPERTY(int repeatTime READ repeatTime WRITE setRepeatTime NOTIFY repeatTimeChanged)
    Q_PROPERTY(bool enablePickForce READ enablePickForce WRITE setEnablePickForce NOTIFY enablePickForceChanged)
    Q_PROPERTY(bool staticTest READ staticTest WRITE setStaticTest NOTIFY staticTestChanged)
    Q_PROPERTY(int testLensCount READ testLensCount WRITE setTestLensCount NOTIFY testLensCountChanged)
    Q_PROPERTY(int griperOperationOutTime READ griperOperationOutTime WRITE setGriperOperationOutTime NOTIFY griperOperationOutTimeChanged)
    Q_PROPERTY(QString tcpLutVacuum1Name READ tcpLutVacuum1Name WRITE setTcpLutVacuum1Name NOTIFY tcpLutVacuumSensor1Changed)
    Q_PROPERTY(QString tcpLutVacuum2Name READ tcpLutVacuum2Name WRITE setTcpLutVacuum2Name NOTIFY tcpLutVacuumSensor2Changed)
    Q_PROPERTY(QString tcpLutVacuumSensor1Name READ tcpLutVacuumSensor1Name WRITE setTcpLutVacuumSensor1Name NOTIFY tcpLutVacuumSensor1NameChanged)
    Q_PROPERTY(QString tcpLutVacuumSensor2Name READ tcpLutVacuumSensor2Name WRITE setTcpLutVacuumSensor2Name NOTIFY tcpLutVacuumSensor2NameChanged)
    double pickForce() const
    {
        return m_PickForce;
    }

    QString motorXName() const
    {
        return m_motorXName;
    }

    QString motorYName() const
    {
        return m_motorYName;
    }

    QString motorZName() const
    {
        return m_motorZName;
    }

    QString vacuum1Name() const
    {
        return m_vacuum1Name;
    }

    QString vacuum2Name() const
    {
        return m_vacuum2Name;
    }

    QString uplookLocationName() const
    {
        return m_uplookLocationName;
    }

    QString loadlookLocationName() const
    {
        return m_loadlookLocationName;
    }

    QString mushroomLocationName() const
    {
        return m_mushroomLocationName;
    }

    double accumulatedHour() const
    {
        return m_accumulatedHour;
    }

    double lensHeight() const
    {
        return m_lensHeight;
    }

    double pickSpeed() const
    {
        return m_pickSpeed;
    }

    int gripperDelay() const
    {
        return m_gripperDelay;
    }

    int repeatTime() const
    {
        return m_repeatTime;
    }

    bool enablePickForce() const
    {
        return m_enablePickForce;
    }

    bool staticTest() const
    {
        return m_staticTest;
    }

    int testLensCount() const
    {
        return m_testLensCount;
    }

    QString moduleName() const
    {
        return m_moduleName;
    }

    int griperOperationOutTime() const
    {
        return m_griperOperationOutTime;
    }

    QString tcpLutVacuum1Name() const
    {
        return m_tcpLutVacuum1Name;
    }

    QString tcpLutVacuum2Name() const
    {
        return m_tcpLutVacuum2Name;
    }

    QString tcpLutVacuumSensor1Name() const
    {
        return m_tcpLutVacuumSensor1Name;
    }

    QString tcpLutVacuumSensor2Name() const
    {
        return m_tcpLutVacuumSensor2Name;
    }

    double placeForce() const
    {
        return m_placeForce;
    }

    double placeSpeed() const
    {
        return m_placeSpeed;
    }

    int placeGripperDelay() const
    {
        return m_placeGripperDelay;
    }

public slots:
    void setPickForce(double PickForce)
    {
        m_PickForce = PickForce;
        emit paramsChanged();
    }

    void setMotorXName(QString motorXName)
    {
        if (m_motorXName == motorXName)
            return;

        m_motorXName = motorXName;
        emit motorXNameChanged(m_motorXName);
    }

    void setMotorYName(QString motorYName)
    {
        if (m_motorYName == motorYName)
            return;

        m_motorYName = motorYName;
        emit motorYNameChanged(m_motorYName);
    }

    void setMotorZName(QString motorZName)
    {
        if (m_motorZName == motorZName)
            return;

        m_motorZName = motorZName;
        emit motorZNameChanged(m_motorZName);
    }

    void setVacuum1Name(QString vacuum1Name)
    {
        if (m_vacuum1Name == vacuum1Name)
            return;

        m_vacuum1Name = vacuum1Name;
        emit vacuum1NameChanged(m_vacuum1Name);
    }

    void setVacuum2Name(QString vacuum2Name)
    {
        if (m_vacuum2Name == vacuum2Name)
            return;

        m_vacuum2Name = vacuum2Name;
        emit vacuum2NameChanged(m_vacuum2Name);
    }

    void setUplookLocationName(QString uplookLocationName)
    {
        if (m_uplookLocationName == uplookLocationName)
            return;

        m_uplookLocationName = uplookLocationName;
        emit uplookLocationNameChanged(m_uplookLocationName);
    }

    void setLoadlookLocationName(QString loadlookLocationName)
    {
        if (m_loadlookLocationName == loadlookLocationName)
            return;

        m_loadlookLocationName = loadlookLocationName;
        emit loadlookLocationNameChanged(m_loadlookLocationName);
    }

    void setMushroomLocationName(QString mushroomLocationName)
    {
        if (m_mushroomLocationName == mushroomLocationName)
            return;

        m_mushroomLocationName = mushroomLocationName;
        emit mushroomLocationNameChanged(m_mushroomLocationName);
    }

    void setAccumulatedHour(double accumulatedHour)
    {
        if (qFuzzyCompare(m_accumulatedHour, accumulatedHour))
            return;

        m_accumulatedHour = accumulatedHour;
        emit accumulatedHourChanged(m_accumulatedHour);
    }

    void setLensHeight(double lensHeight)
    {
        if (qFuzzyCompare(m_lensHeight, lensHeight))
            return;

        m_lensHeight = lensHeight;
        emit lensHeightChanged(m_lensHeight);
    }

    void setPickSpeed(double pickSpeed)
    {
        if (qFuzzyCompare(m_pickSpeed, pickSpeed))
            return;

        m_pickSpeed = pickSpeed;
        emit pickSpeedChanged(m_pickSpeed);
    }
    void setGripperDelay(int gripperDelay)
    {
        if (m_gripperDelay == gripperDelay)
            return;

        m_gripperDelay = gripperDelay;
        emit gripperDelayChanged(m_gripperDelay);
    }

    void setRepeatTime(int repeatTime)
    {
        if (m_repeatTime == repeatTime)
            return;

        m_repeatTime = repeatTime;
        emit repeatTimeChanged(m_repeatTime);
    }

    void setEnablePickForce(bool enablePickForce)
    {
        if (m_enablePickForce == enablePickForce)
            return;

        m_enablePickForce = enablePickForce;
        emit enablePickForceChanged(m_enablePickForce);
    }

    void setStaticTest(bool staticTest)
    {
        if (m_staticTest == staticTest)
            return;

        m_staticTest = staticTest;
        emit staticTestChanged(m_staticTest);
    }

    void setTestLensCount(int testLensCount)
    {
        if (m_testLensCount == testLensCount)
            return;

        m_testLensCount = testLensCount;
        emit testLensCountChanged(m_testLensCount);
    }

    void setModuleName(QString moduleName)
    {
        if (m_moduleName == moduleName)
            return;

        m_moduleName = moduleName;
        emit moduleNameChanged(m_moduleName);
    }

    void setGriperOperationOutTime(int griperOperationOutTime)
    {
        if (m_griperOperationOutTime == griperOperationOutTime)
            return;

        m_griperOperationOutTime = griperOperationOutTime;
        emit griperOperationOutTimeChanged(m_griperOperationOutTime);
    }

    void setTcpLutVacuum1Name(QString tcpLutVacuum1Name)
    {
        if (m_tcpLutVacuum1Name == tcpLutVacuum1Name)
            return;

        m_tcpLutVacuum1Name = tcpLutVacuum1Name;
        emit tcpLutVacuumSensor1Changed(m_tcpLutVacuum1Name);
    }

    void setTcpLutVacuum2Name(QString tcpLutVacuum2Name)
    {
        if (m_tcpLutVacuum2Name == tcpLutVacuum2Name)
            return;

        m_tcpLutVacuum2Name = tcpLutVacuum2Name;
        emit tcpLutVacuumSensor2Changed(m_tcpLutVacuum2Name);
    }

    void setTcpLutVacuumSensor1Name(QString tcpLutVacuumSensor1Name)
    {
        if (m_tcpLutVacuumSensor1Name == tcpLutVacuumSensor1Name)
            return;

        m_tcpLutVacuumSensor1Name = tcpLutVacuumSensor1Name;
        emit tcpLutVacuumSensor1NameChanged(m_tcpLutVacuumSensor1Name);
    }

    void setTcpLutVacuumSensor2Name(QString tcpLutVacuumSensor2Name)
    {
        if (m_tcpLutVacuumSensor2Name == tcpLutVacuumSensor2Name)
            return;

        m_tcpLutVacuumSensor2Name = tcpLutVacuumSensor2Name;
        emit tcpLutVacuumSensor2NameChanged(m_tcpLutVacuumSensor2Name);
    }

    void setPlaceForce(double placeForce)
    {
        if (qFuzzyCompare(m_placeForce, placeForce))
            return;

        m_placeForce = placeForce;
        emit placeForceChanged(m_placeForce);
    }

    void setPlaceSpeed(double placeSpeed)
    {
        if (qFuzzyCompare(m_placeSpeed, placeSpeed))
            return;

        m_placeSpeed = placeSpeed;
        emit placeSpeedChanged(m_placeSpeed);
    }

    void setPlaceGripperDelay(int placeGripperDelay)
    {
        if (m_placeGripperDelay == placeGripperDelay)
            return;

        m_placeGripperDelay = placeGripperDelay;
        emit placeGripperDelayChanged(m_placeGripperDelay);
    }

signals:
    void paramsChanged();

    void motorXNameChanged(QString motorXName);

    void motorYNameChanged(QString motorYName);

    void motorZNameChanged(QString motorZName);

    void vacuum1NameChanged(QString vacuum1Name);

    void vacuum2NameChanged(QString vacuum2Name);

    void uplookLocationNameChanged(QString uplookLocationName);

    void loadlookLocationNameChanged(QString loadlookLocationName);

    void mushroomLocationNameChanged(QString mushroomLocationName);

    void accumulatedHourChanged(double accumulatedHour);

    void lensHeightChanged(double lensHeight);

    void pickSpeedChanged(double pickSpeed);


    void gripperDelayChanged(int gripperDelay);

    void repeatTimeChanged(int repeatTime);

    void enablePickForceChanged(bool enablePickForce);

    void staticTestChanged(bool staticTest);

    void testLensCountChanged(int testLensCount);

    void moduleNameChanged(QString moduleName);

    void griperOperationOutTimeChanged(int griperOperationOutTime);

    void tcpLutVacuumSensor1Changed(QString tcpLutVacuum1Name);

    void tcpLutVacuumSensor2Changed(QString tcpLutVacuum2Name);

    void tcpLutVacuumSensor1NameChanged(QString tcpLutVacuumSensor1Name);

    void tcpLutVacuumSensor2NameChanged(QString tcpLutVacuumSensor2Name);

    void placeForceChanged(double placeForce);

    void placeSpeedChanged(double placeSpeed);

    void placeGripperDelayChanged(int placeGripperDelay);

private:
    double m_PickForce = 0;
    QString m_motorXName = "LUT_X";
    QString m_motorYName = "LUT_Y";
    QString m_motorZName = "LUT_Z";
    QString m_vacuum1Name = "LUT_V1";
    QString m_vacuum2Name = "LUT_V2";
    QString m_uplookLocationName = "";
    QString m_loadlookLocationName = "";
    QString m_mushroomLocationName = "";
    double m_accumulatedHour = 0;
    double m_lensHeight = 1;
    double m_pickSpeed = 10;
    int m_gripperDelay = 500;
    int m_repeatTime = 10;
    bool m_enablePickForce = false;
    bool m_staticTest = false;
    int m_testLensCount = 10;
    QString m_moduleName = "LUTModule";
    int m_griperOperationOutTime = 1000;
    QString m_tcpLutVacuum1Name;
    QString m_tcpLutVacuum2Name;
    QString m_tcpLutVacuumSensor1Name;
    QString m_tcpLutVacuumSensor2Name;
    double m_placeForce = 0;
    double m_placeSpeed = 10;
    int m_placeGripperDelay = 500;
};

class LutState:public PropertyBase
{
    Q_OBJECT
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(bool disableStation1 READ disableStation1 WRITE setDisableStation1 NOTIFY disableStation1Changed)
    Q_PROPERTY(bool disableStation2 READ disableStation2 WRITE setDisableStation2 NOTIFY disableStation2Changed)
    Q_PROPERTY(bool handlyChangeLens READ handlyChangeLens WRITE setHandlyChangeLens NOTIFY handlyChangeLensChanged)

    Q_PROPERTY(bool waitingLens READ waitingLens WRITE setWaitingLens NOTIFY waitLensChanged)
    Q_PROPERTY(bool finishWaitLens READ finishWaitLens WRITE setFinishWaitLens NOTIFY finishWaitLensChanged)
    Q_PROPERTY(int busyState READ busyState WRITE setBusyState NOTIFY busyStateChanged)
    Q_PROPERTY(int lastState READ lastState WRITE setLastState NOTIFY lastStateChanged)
    Q_PROPERTY(bool lutHasLens READ lutHasLens WRITE setLutHasLens NOTIFY lutHasLensChanged)
    Q_PROPERTY(bool lutHasNgLens READ lutHasNgLens WRITE setLutHasNgLens NOTIFY lutHasNgLensChanged)
    Q_PROPERTY(bool station1NeedLens READ station1NeedLens WRITE setStation1NeedLens NOTIFY station1NeedLensChanged)
    Q_PROPERTY(bool station2NeedLens READ station2NeedLens WRITE setStation2NeedLens NOTIFY station2NeedLensChanged)
    Q_PROPERTY(int aa1HeadMaterialState READ aa1HeadMaterialState WRITE setAa1HeadMaterialState NOTIFY aa1HeadMaterialStateChanged)
    Q_PROPERTY(int aa2HeadMaterialState READ aa2HeadMaterialState WRITE setAa2HeadMaterialState NOTIFY aa2HeadMaterialStateChanged)
    Q_PROPERTY(QVariantMap lensData READ lensData WRITE setLensData NOTIFY lensDataChanged)
    Q_PROPERTY(QVariantMap ngLensData READ ngLensData WRITE setNgLensData NOTIFY ngLensDataChanged)
    Q_PROPERTY(QVariantMap aa1LensData READ aa1LensData WRITE setAa1LensData NOTIFY aa1LensDataChanged)
    Q_PROPERTY(QVariantMap aa2LensData READ aa2LensData WRITE setAa2LensData NOTIFY aa2LensDataChanged)
    Q_PROPERTY(bool station1HasRequest READ station1HasRequest WRITE setStation1HasRequest NOTIFY station1HasRequestChanged)
    Q_PROPERTY(bool station2HasRequest READ station2HasRequest WRITE setStation2HasRequest NOTIFY station2HasRequestChanged)
    Q_PROPERTY(int taskOfStation1 READ taskOfStation1 WRITE setTaskOfStation1 NOTIFY taskOfStation1Changed)
    Q_PROPERTY(int taskOfStation2 READ taskOfStation2 WRITE setTaskOfStation2 NOTIFY taskOfStation2Changed)

    Q_PROPERTY(bool finishStation1Task READ finishStation1Task WRITE setFinishStation1Task NOTIFY finishStation1TaskChanged)
    Q_PROPERTY(bool finishStation2Task READ finishStation2Task WRITE setFinishStation2Task NOTIFY finishStation2TaskChanged)
    Q_PROPERTY(bool station1Unload READ station1Unload WRITE setStation1Unload NOTIFY station1UnloadChanged)
    Q_PROPERTY(bool station2Unload READ station2Unload WRITE setStation2Unload NOTIFY station2UnloadChanged)
    Q_PROPERTY(bool waitingTask READ waitingTask WRITE setWaitingTask NOTIFY waitingTaskChanged)
    Q_PROPERTY(int griperOperationResult READ griperOperationResult WRITE setGriperOperationResult NOTIFY griperOperationResultChanged)
    //TCP remote module device state
    Q_PROPERTY(bool tcpVaccum1State READ tcpVaccum1State WRITE setTcpVaccum1State NOTIFY tcpVaccum1StateChanged)
    Q_PROPERTY(bool tcpVaccum2State READ tcpVaccum2State WRITE setTcpVaccum2State NOTIFY tcpVaccum2StateChanged)
public:
    int runMode() const
    {
        return m_runMode;
    }

    bool disableStation1() const
    {
        return m_disableStation1;
    }

    bool disableStation2() const
    {
        return m_disableStation2;
    }

    bool handlyChangeLens() const
    {
        return m_handlyChangeLens;
    }

    bool waitingLens() const
    {
        return m_waitingLens;
    }

    bool finishWaitLens() const
    {
        return m_finishWaitLens;
    }

    int busyState() const
    {
        return m_busyState;
    }

    bool lutHasLens() const
    {
        return m_lutHasLens;
    }

    bool lutHasNgLens() const
    {
        return m_lutHasNgLens;
    }

    QVariantMap lensData() const
    {
        return m_lensData;
    }

    QVariant lensData(QString key) const
    {
        return m_lensData[key];
    }

    QVariantMap ngLensData() const
    {
        return m_ngLensData;
    }

    QVariant ngLensData(QString key) const
    {
        return m_ngLensData[key];
    }

    QVariantMap aa1LensData() const
    {
        return m_aa1LensData;
    }

    QVariant aa1LensData(QString key) const
    {
        return m_aa1LensData[key];
    }

    QVariantMap aa2LensData() const
    {
        return m_aa2LensData;
    }

    QVariant aa2LensData(QString key) const
    {
        return m_aa2LensData[key];
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

    int aa1HeadMaterialState() const
    {
        return m_aa1HeadMaterialState;
    }

    int aa2HeadMaterialState() const
    {
        return m_aa2HeadMaterialState;
    }

    int lastState() const
    {
        return m_lastState;
    }

    bool station1Unload() const
    {
        return m_station1Unload;
    }

    bool station2Unload() const
    {
        return m_station2Unload;
    }

    bool station1NeedLens() const
    {
        return m_station1NeedLens;
    }

    bool station2NeedLens() const
    {
        return m_station2NeedLens;
    }

    int taskOfStation1() const
    {
        return m_taskOfStation1;
    }

    int taskOfStation2() const
    {
        return m_taskOfStation2;
    }

    bool waitingTask() const
    {
        return m_waitingTask;
    }

    int griperOperationResult() const
    {
        return m_griperOperationResult;
    }

    bool tcpVaccum1State() const
    {
        return m_tcpVaccum1State;
    }

    bool tcpVaccum2State() const
    {
        return m_tcpVaccum2State;
    }

public slots:
    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
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

    void setHandlyChangeLens(bool handlyChangeLens)
    {
        if (m_handlyChangeLens == handlyChangeLens)
            return;

        m_handlyChangeLens = handlyChangeLens;
        emit handlyChangeLensChanged(m_handlyChangeLens);
    }

    void setWaitingLens(bool waitingLens)
    {
        if (m_waitingLens == waitingLens)
            return;

        m_waitingLens = waitingLens;
        emit waitLensChanged(m_waitingLens);
    }

    void setFinishWaitLens(bool finishWaitLens)
    {
        if (m_finishWaitLens == finishWaitLens)
            return;

        m_finishWaitLens = finishWaitLens;
        emit finishWaitLensChanged(m_finishWaitLens);
    }

    void setBusyState(int busyState)
    {
        if (m_busyState == busyState)
            return;

        m_busyState = busyState;
        emit busyStateChanged(m_busyState);
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

    void setLensData(QVariantMap lensData)
    {
        if (m_lensData == lensData)
            return;

        m_lensData = lensData;
        emit lensDataChanged(m_lensData);
    }

    void setLensData(QString key,QVariant value)
    {
        m_lensData[key] = value;
        emit lensDataChanged(m_lensData);
    }

    void copyInLensData(QVariantMap lensData)
    {
        foreach (QString param_name, lensData.keys())
            m_lensData[param_name] = lensData[param_name];
        emit lensDataChanged(m_lensData);
    }

    void clearLensData()
    {
        m_lensData.clear();
        emit lensDataChanged(m_lensData);
    }

    void setNgLensData(QVariantMap ngLensData)
    {
        if (m_ngLensData == ngLensData)
            return;

        m_ngLensData = ngLensData;
        emit ngLensDataChanged(m_ngLensData);
    }

    void setNgLensData(QString key,QVariant value)
    {
        m_ngLensData[key] = value;
        emit ngLensDataChanged(m_ngLensData);
    }

    void copyInNgLensData(QVariantMap ngLensData)
    {
        foreach (QString param_name, ngLensData.keys())
            m_ngLensData[param_name] = ngLensData[param_name];
        emit ngLensDataChanged(m_ngLensData);
    }

    void clearNgLensData()
    {
        m_ngLensData.clear();
        emit ngLensDataChanged(m_ngLensData);
    }

    void setAa1LensData(QVariantMap aa1LensData)
    {
        if (m_aa1LensData == aa1LensData)
            return;

        m_aa1LensData = aa1LensData;
        emit aa1LensDataChanged(m_aa1LensData);
    }

    void setAa1LensData(QString key,QVariant value)
    {
        m_aa1LensData[key] = value;
        emit aa1LensDataChanged(m_aa1LensData);
    }

    void copyInAa1LensData(QVariantMap aa1LensData)
    {
        foreach (QString param_name, aa1LensData.keys())
            m_aa1LensData[param_name] = aa1LensData[param_name];
        emit aa1LensDataChanged(m_aa1LensData);
    }

    void clearAa1LensData()
    {
        m_aa1LensData.clear();
        emit aa1LensDataChanged(m_aa1LensData);
    }

    void setAa2LensData(QVariantMap aa2LensData)
    {
        if (m_aa2LensData == aa2LensData)
            return;

        m_aa2LensData = aa2LensData;
        emit aa2LensDataChanged(m_aa2LensData);
    }

    void setAa2LensData(QString key,QVariant value)
    {
        m_aa2LensData[key] = value;
        emit aa2LensDataChanged(m_aa2LensData);
    }

    void copyInAa2LensData(QVariantMap aa2LensData)
    {
        foreach (QString param_name, aa2LensData.keys())
            m_aa2LensData[param_name] = aa2LensData[param_name];
        emit aa2LensDataChanged(m_aa2LensData);
    }

    void clearAa2LensData()
    {
        m_aa2LensData.clear();
        emit aa2LensDataChanged(m_aa2LensData);
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

    void setAa1HeadMaterialState(int aa1HeadMaterialState)
    {
        if (m_aa1HeadMaterialState == aa1HeadMaterialState)
            return;

        m_aa1HeadMaterialState = aa1HeadMaterialState;
        emit aa1HeadMaterialStateChanged(m_aa1HeadMaterialState);
    }

    void setAa2HeadMaterialState(int aa2HeadMaterialState)
    {
        if (m_aa2HeadMaterialState == aa2HeadMaterialState)
            return;

        m_aa2HeadMaterialState = aa2HeadMaterialState;
        emit aa2HeadMaterialStateChanged(m_aa2HeadMaterialState);
    }

    void setLastState(int lastState)
    {
        if (m_lastState == lastState)
            return;

        m_lastState = lastState;
        emit lastStateChanged(m_lastState);
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

    void setStation1NeedLens(bool station1NeedLens)
    {
        if (m_station1NeedLens == station1NeedLens)
            return;

        m_station1NeedLens = station1NeedLens;
        emit station1NeedLensChanged(m_station1NeedLens);
    }

    void setStation2NeedLens(bool station2NeedLens)
    {
        if (m_station2NeedLens == station2NeedLens)
            return;

        m_station2NeedLens = station2NeedLens;
        emit station2NeedLensChanged(m_station2NeedLens);
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

    void setWaitingTask(bool waitingTask)
    {
        if (m_waitingTask == waitingTask)
            return;

        m_waitingTask = waitingTask;
        emit waitingTaskChanged(m_waitingTask);
    }

    void setGriperOperationResult(int griperOperationResult)
    {
        if (m_griperOperationResult == griperOperationResult)
            return;

        m_griperOperationResult = griperOperationResult;
        emit griperOperationResultChanged(m_griperOperationResult);
    }

    void setTcpVaccum1State(bool tcpVaccum1State)
    {
        if (m_tcpVaccum1State == tcpVaccum1State)
            return;

        m_tcpVaccum1State = tcpVaccum1State;
        emit tcpVaccum1StateChanged(m_tcpVaccum1State);
    }

    void setTcpVaccum2State(bool tcpVaccum2State)
    {
        if (m_tcpVaccum2State == tcpVaccum2State)
            return;

        m_tcpVaccum2State = tcpVaccum2State;
        emit tcpVaccum2StateChanged(m_tcpVaccum2State);
    }

signals:
    void runModeChanged(int runMode);

    void disableStation1Changed(bool disableStation1);

    void disableStation2Changed(bool disableStation2);

    void handlyChangeLensChanged(bool handlyChangeLens);

    void waitLensChanged(bool waitingLens);

    void finishWaitLensChanged(bool finishWaitLens);

    void busyStateChanged(int busyState);

    void lutHasLensChanged(bool lutHasLens);

    void lutHasNgLensChanged(bool lutHasNgLens);

    void lensDataChanged(QVariantMap lensData);

    void ngLensDataChanged(QVariantMap ngLensData);

    void aa1LensDataChanged(QVariantMap aa1LensData);

    void aa2LensDataChanged(QVariantMap aa2LensData);

    void station1HasRequestChanged(bool station1HasRequest);

    void station2HasRequestChanged(bool station2HasRequest);

    void finishStation1TaskChanged(bool finishStation1Task);

    void finishStation2TaskChanged(bool finishStation2Task);

    void aa1HeadMaterialStateChanged(int aa1HeadMaterialState);

    void aa2HeadMaterialStateChanged(int aa2HeadMaterialState);

    void lastStateChanged(int lastState);

    void station1UnloadChanged(bool station1Unload);

    void station2UnloadChanged(bool station2Unload);

    void station1NeedLensChanged(bool station1NeedLens);

    void station2NeedLensChanged(bool station2NeedLens);

    void taskOfStation1Changed(int taskOfStation1);

    void taskOfStation2Changed(int taskOfStation2);

    void waitingTaskChanged(bool waitingTask);


    void griperOperationResultChanged(int griperOperationResult);

    void tcpVaccum1StateChanged(bool tcpVaccum1State);

    void tcpVaccum2StateChanged(bool tcpVaccum2State);

private:
    int m_runMode = 0;
    bool m_disableStation1 = false;
    bool m_disableStation2 = false;
    bool m_handlyChangeLens = false;
    bool m_waitingLens = false;
    bool m_finishWaitLens = false;
    int m_busyState = 0;
    bool m_lutHasLens = false;
    bool m_lutHasNgLens = false;
    QVariantMap m_lensData;
    QVariantMap m_ngLensData;
    QVariantMap m_aa1LensData;
    QVariantMap m_aa2LensData;
    bool m_station1HasRequest = false;
    bool m_station2HasRequest = false;
    bool m_finishStation1Task = false;
    bool m_finishStation2Task = false;
    int m_aa1HeadMaterialState = 0;
    int m_aa2HeadMaterialState = 0;
    int m_lastState = 0;
    bool m_station1Unload = false;
    bool m_station2Unload = false;
    bool m_station1NeedLens = false;
    bool m_station2NeedLens = false;
    int m_taskOfStation1 = 0;
    int m_taskOfStation2 = 0;
    bool m_waitingTask = false;
    int m_griperOperationResult = 0;
    bool m_tcpVaccum1State = false;
    bool m_tcpVaccum2State;
};

#endif // LUT_PARAMERTER_H
