#ifndef LUT_PARAMERTER_H
#define LUT_PARAMERTER_H
#include "propertybase.h"

class LutParameter:public PropertyBase
{
    Q_OBJECT
public:
    LutParameter():PropertyBase(){}
    Q_PROPERTY(double pickForce READ pickForce WRITE setPickForce NOTIFY paramsChanged)
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
    Q_PROPERTY(int gripperDelay READ gripperDelay WRITE setGripperDelay NOTIFY gripperDelayChanged)
    Q_PROPERTY(int repeatTime READ repeatTime WRITE setRepeatTime NOTIFY repeatTimeChanged)
    Q_PROPERTY(bool enablePickForce READ enablePickForce WRITE setEnablePickForce NOTIFY enablePickForceChanged)
    Q_PROPERTY(bool staticTest READ staticTest WRITE setStaticTest NOTIFY staticTestChanged)
    Q_PROPERTY(int testLensCount READ testLensCount WRITE setTestLensCount NOTIFY testLensCountChanged)

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
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_accumulatedHour, accumulatedHour))
            return;

        m_accumulatedHour = accumulatedHour;
        emit accumulatedHourChanged(m_accumulatedHour);
    }

    void setLensHeight(double lensHeight)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_lensHeight, lensHeight))
            return;

        m_lensHeight = lensHeight;
        emit lensHeightChanged(m_lensHeight);
    }

    void setPickSpeed(double pickSpeed)
    {
        qWarning("Floating point comparison needs context sanity check");
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
};

class LutState:public PropertyBase
{
    Q_OBJECT
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(bool disableStation1 READ disableStation1 WRITE setDisableStation1 NOTIFY disableStation1Changed)
    Q_PROPERTY(bool disableStation2 READ disableStation2 WRITE setDisableStation2 NOTIFY disableStation2Changed)
    Q_PROPERTY(bool handlyChangeLens READ handlyChangeLens WRITE setHandlyChangeLens NOTIFY handlyChangeLensChanged)
    Q_PROPERTY(int lutTrayID READ lutTrayID WRITE setLutTrayID NOTIFY lutTrayIDChanged)
    Q_PROPERTY(int lutLensID READ lutLensID WRITE setLutLensID NOTIFY lutLensIDChanged)
    Q_PROPERTY(int lutNgTrayID READ lutNgTrayID WRITE setLutNgTrayID NOTIFY lutNgTrayIDChanged)
    Q_PROPERTY(int lutNgLensID READ lutNgLensID WRITE setLutNgLensID NOTIFY lutNgLensIDChanged)
    Q_PROPERTY(int aa1TrayID READ aa1TrayID WRITE setAa1TrayID NOTIFY aa1TrayIDChanged)
    Q_PROPERTY(int aa1LensID READ aa1LensID WRITE setAa1LensID NOTIFY aa1LensIDChanged)
    Q_PROPERTY(int aa2TrayID READ aa2TrayID WRITE setAa2TrayID NOTIFY aa2TrayIDChanged)
    Q_PROPERTY(int aa2LensID READ aa2LensID WRITE setAa2LensID NOTIFY aa2LensIDChanged)
    Q_PROPERTY(bool waitingLens READ waitingLens WRITE setWaitingLens NOTIFY waitLensChanged)
    Q_PROPERTY(QString servingIP READ servingIP WRITE setServingIP NOTIFY servingIPChanged)
    Q_PROPERTY(bool lutHasLens READ lutHasLens WRITE setLutHasLens NOTIFY lutHasLensChanged)
    Q_PROPERTY(bool pickingLens READ pickingLens WRITE setPickingLens NOTIFY pickingLensChanged)
    Q_PROPERTY(QString cmd READ cmd WRITE setCmd NOTIFY cmdChanged)
    Q_PROPERTY(bool pickedLens READ pickedLens WRITE setPickedLens NOTIFY pickedLensChanged)
    Q_PROPERTY(bool unpickedNgLens READ unpickedNgLens WRITE setUnpickedNgLens NOTIFY unpickedNgLensChanged)
    Q_PROPERTY(bool finishWaitLens READ finishWaitLens WRITE setFinishWaitLens NOTIFY finishWaitLensChanged)
    Q_PROPERTY(bool lutHasNgLens READ lutHasNgLens WRITE setLutHasNgLens NOTIFY lutHasNgLensChanged)
    Q_PROPERTY(bool aaPickedLens READ aaPickedLens WRITE setAaPickedLens NOTIFY aaPickedLensChanged)
    Q_PROPERTY(bool lutLoadReady READ lutLoadReady WRITE setLutLoadReady NOTIFY lutLensReadyChanged)
    Q_PROPERTY(bool needUplookPr READ needUplookPr WRITE setNeedUplookPr NOTIFY needUplookPrChanged)
    Q_PROPERTY(QString lutUuid READ lutUuid WRITE setLutUuid NOTIFY lutUuidChanged)
    Q_PROPERTY(QString aa1Uuid READ aa1Uuid WRITE setAa1Uuid NOTIFY aa1UuidChanged)
    Q_PROPERTY(QString aa2Uuid READ aa2Uuid WRITE setAa2Uuid NOTIFY aa2UuidChanged)
    Q_PROPERTY(bool beExchangeMaterial READ beExchangeMaterial WRITE setBeExchangeMaterial NOTIFY beExchangeMaterialChanged)
public:
    LutState():PropertyBase()
    {
        init_values.insert("lutTrayID",-1);
        init_values.insert("lutLensID",-1);
        init_values.insert("lutNgTrayID",-1);
        init_values.insert("lutTrayID",-1);
        init_values.insert("lutTrayID",-1);
    }
    int lutTrayID() const
    {
        return m_lutTrayID;
    }

    int lutLensID() const
    {
        return m_lutLensID;
    }

    int lutNgTrayID() const
    {
        return m_lutNgTrayID;
    }

    int lutNgLensID() const
    {
        return m_lutNgLensID;
    }

    int aa1TrayID() const
    {
        return m_aa1TrayID;
    }

    int aa1LensID() const
    {
        return m_aa1LensID;
    }

    int aa2TrayID() const
    {
        return m_aa2TrayID;
    }

    int aa2LensID() const
    {
        return m_aa2LensID;
    }

    bool waitingLens() const
    {
        return m_waitingLens;
    }

    QString servingIP() const
    {
        return m_serviceIP;
    }

    bool lutHasLens() const
    {
        return m_lutHasLens;
    }

    bool pickingLens() const
    {
        return m_pickingLens;
    }

    QString cmd() const
    {
        return m_cmd;
    }

    bool pickedLens() const
    {
        return m_pickedLens;
    }

    bool unpickedNgLens() const
    {
        return m_unpickedNgLens;
    }

    bool finishWaitLens() const
    {
        return m_finishWaitLens;
    }

    bool lutHasNgLens() const
    {
        return m_lutHasNgLens;
    }

    bool aaPickedLens() const
    {
        return m_aaPickedLens;
    }

    bool lutLoadReady() const
    {
        return m_lutloadReady;
    }

    bool needUplookPr() const
    {
        return m_needUplookPr;
    }

    QString lutUuid() const
    {
        return m_lutUuid;
    }

    QString aa1Uuid() const
    {
        return m_aa1Uuid;
    }

    QString aa2Uuid() const
    {
        return m_aa2Uuid;
    }

    bool beExchangeMaterial() const
    {
        return m_beExchangeMaterial;
    }

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

public slots:
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

    void setAa1TrayID(int aa1TrayID)
    {
        if (m_aa1TrayID == aa1TrayID)
            return;

        m_aa1TrayID = aa1TrayID;
        emit aa1TrayIDChanged(m_aa1TrayID);
    }

    void setAa1LensID(int aa1LensID)
    {
        if (m_aa1LensID == aa1LensID)
            return;

        m_aa1LensID = aa1LensID;
        emit aa1LensIDChanged(m_aa1LensID);
    }

    void setAa2TrayID(int aa2TrayID)
    {
        if (m_aa2TrayID == aa2TrayID)
            return;

        m_aa2TrayID = aa2TrayID;
        emit aa2TrayIDChanged(m_aa2TrayID);
    }

    void setAa2LensID(int aa2LensID)
    {
        if (m_aa2LensID == aa2LensID)
            return;

        m_aa2LensID = aa2LensID;
        emit aa2LensIDChanged(m_aa2LensID);
    }

    void setWaitingLens(bool waitLens)
    {
        if (m_waitingLens == waitLens)
            return;

        m_waitingLens = waitLens;
        emit waitLensChanged(m_waitingLens);
    }

    void setServingIP(QString serviceIP)
    {
        if (m_serviceIP == serviceIP)
            return;

        m_serviceIP = serviceIP;
        emit servingIPChanged(m_serviceIP);
    }

    void setLutHasLens(bool lutHasLens)
    {
        if (m_lutHasLens == lutHasLens)
            return;

        m_lutHasLens = lutHasLens;
        emit lutHasLensChanged(m_lutHasLens);
    }

    void setPickingLens(bool pickingLens)
    {
        if (m_pickingLens == pickingLens)
            return;

        m_pickingLens = pickingLens;
        emit pickingLensChanged(m_pickingLens);
    }

    void setCmd(QString cmd)
    {
        if (m_cmd == cmd)
            return;

        m_cmd = cmd;
        emit cmdChanged(m_cmd);
    }

    void setPickedLens(bool pickedLens)
    {
        if (m_pickedLens == pickedLens)
            return;

        m_pickedLens = pickedLens;
        emit pickedLensChanged(m_pickedLens);
    }

    void setUnpickedNgLens(bool unpickedNgLens)
    {
        if (m_unpickedNgLens == unpickedNgLens)
            return;

        m_unpickedNgLens = unpickedNgLens;
        emit unpickedNgLensChanged(m_unpickedNgLens);
    }

    void setFinishWaitLens(bool finishWaitLens)
    {
        if (m_finishWaitLens == finishWaitLens)
            return;

        m_finishWaitLens = finishWaitLens;
        emit finishWaitLensChanged(m_finishWaitLens);
    }

    void setLutHasNgLens(bool lutHasNgLens)
    {
        if (m_lutHasNgLens == lutHasNgLens)
            return;

        m_lutHasNgLens = lutHasNgLens;
        emit lutHasNgLensChanged(m_lutHasNgLens);
    }

    void setAaPickedLens(bool aaPickedLens)
    {
        if (m_aaPickedLens == aaPickedLens)
            return;

        m_aaPickedLens = aaPickedLens;
        emit aaPickedLensChanged(m_aaPickedLens);
    }

    void setLutLoadReady(bool lutLensReady)
    {
        if (m_lutloadReady == lutLensReady)
            return;

        m_lutloadReady = lutLensReady;
        emit lutLensReadyChanged(m_lutloadReady);
    }

    void setNeedUplookPr(bool finishUplookPr)
    {
        if (m_needUplookPr == finishUplookPr)
            return;

        m_needUplookPr = finishUplookPr;
        emit needUplookPrChanged(m_needUplookPr);
    }

    void setLutUuid(QString lutUuid)
    {
        if (m_lutUuid == lutUuid)
            return;

        m_lutUuid = lutUuid;
        emit lutUuidChanged(m_lutUuid);
    }

    void setAa1Uuid(QString aa1Uuid)
    {
        if (m_aa1Uuid == aa1Uuid)
            return;

        m_aa1Uuid = aa1Uuid;
        emit aa1UuidChanged(m_aa1Uuid);
    }

    void setAa2Uuid(QString aa2Uuid)
    {
        if (m_aa2Uuid == aa2Uuid)
            return;

        m_aa2Uuid = aa2Uuid;
        emit aa2UuidChanged(m_aa2Uuid);
    }

    void setBeExchangeMaterial(bool beExchangeMaterial)
    {
        if (m_beExchangeMaterial == beExchangeMaterial)
            return;

        m_beExchangeMaterial = beExchangeMaterial;
        emit beExchangeMaterialChanged(m_beExchangeMaterial);
    }

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

signals:
    void lutTrayIDChanged(int lutTrayID);

    void lutLensIDChanged(int lutLensID);

    void lutNgTrayIDChanged(int lutNgTrayID);

    void lutNgLensIDChanged(int lutNgLensID);

    void aa1TrayIDChanged(int aa1TrayID);

    void aa1LensIDChanged(int aa1LensID);

    void aa2TrayIDChanged(int aa2TrayID);

    void aa2LensIDChanged(int aa2LensID);

    void waitLensChanged(bool waitingLens);

    void servingIPChanged(QString servingIP);

    void lutHasLensChanged(bool lutHasLens);

    void pickingLensChanged(bool pickingLens);


    void cmdChanged(QString cmd);


    void pickedLensChanged(bool pickedLens);

    void unpickedNgLensChanged(bool unpickedNgLens);

    void finishWaitLensChanged(bool finishWaitLens);

    void lutHasNgLensChanged(bool lutHasNgLens);

    void aaPickedLensChanged(bool aaPickedLens);

    void lutLensReadyChanged(bool lutLoadReady);

    void needUplookPrChanged(bool needUplookPr);

    void lutUuidChanged(QString lutUuid);

    void aa1UuidChanged(QString aa1Uuid);

    void aa2UuidChanged(QString aa2Uuid);

    void beExchangeMaterialChanged(bool beExchangeMaterial);

    void runModeChanged(int runMode);

    void disableStation1Changed(bool disableStation1);

    void disableStation2Changed(bool disableStation2);

    void handlyChangeLensChanged(bool handlyChangeLens);

private:
    int m_lutTrayID = -1;
    int m_lutLensID = -1;
    int m_lutNgTrayID = -1;
    int m_lutNgLensID = -1;
    int m_aa1TrayID = -1;
    int m_aa1LensID = -1;
    int m_aa2TrayID = -1;
    int m_aa2LensID = -1;
    bool m_waitingLens = false;
    QString m_serviceIP = "";
    bool m_lutHasLens = false;
    bool m_pickingLens = false;
    QString m_cmd = "";
    bool m_pickedLens = false;
    bool m_unpickedNgLens = false;
    bool m_finishWaitLens = false;
    bool m_lutHasNgLens = false;
    bool m_aaPickedLens = false;
    bool m_lutloadReady = false;
    bool m_needUplookPr = false;
    QString m_lutUuid = "";
    QString m_aa1Uuid = "";
    QString m_aa2Uuid = "";
    bool m_beExchangeMaterial = false;
    int m_runMode = 0;
    bool m_disableStation1 = false;
    bool m_disableStation2 = false;
    bool m_handlyChangeLens = false;
};

#endif // LUT_PARAMERTER_H
