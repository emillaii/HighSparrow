#ifndef SENSORTRAYLOADERPARAMETER_H
#define SENSORTRAYLOADERPARAMETER_H

#include "propertybase.h"



class SensorTrayLoaderParameter:public PropertyBase
{
    Q_OBJECT
public:
    SensorTrayLoaderParameter():PropertyBase(){}
    Q_PROPERTY(QString motorTrayName READ motorTrayName WRITE setMotorTrayName NOTIFY motorTrayNameChanged)
    Q_PROPERTY(QString motorSTIEName READ motorSTIEName WRITE setMotorSTIEName NOTIFY motorSTIENameChanged)
    Q_PROPERTY(QString motorSTKName READ motorSTKName WRITE setMotorSTKName NOTIFY motorSTKNameChanged)
    Q_PROPERTY(QString motorSTOEName READ motorSTOEName WRITE setMotorSTOEName NOTIFY motorSTOENameChanged)
    Q_PROPERTY(QString motorSPOName READ motorSPOName WRITE setMotorSPOName NOTIFY motorSPONameChanged)
    Q_PROPERTY(QString cylinderHoldTrayName READ cylinderHoldTrayName WRITE setCylinderHoldTrayName NOTIFY cylinderHoldTrayNameChanged)
    Q_PROPERTY(QString cylinderVacancyTrayName READ cylinderVacancyTrayName WRITE setCylinderVacancyTrayName NOTIFY cylinderVacancyTrayNameChanged)
    Q_PROPERTY(QString cylinderSTK1Name READ cylinderSTK1Name WRITE setCylinderSTK1Name NOTIFY cylinderSTK1NameChanged)
    Q_PROPERTY(QString cylinderSTK2Name READ cylinderSTK2Name WRITE setCylinderSTK2Name NOTIFY cylinderSTK2NameChanged)
    Q_PROPERTY(QString cylinderEntanceClipPushName READ cylinderEntanceClipPushName WRITE setCylinderEntanceClipPushName NOTIFY cylinderEntanceClipPushNameChanged)
    Q_PROPERTY(QString cylinderExitClipPushName READ cylinderExitClipPushName WRITE setCylinderExitClipPushName NOTIFY cylinderExitClipNamePushChanged)
    Q_PROPERTY(QString cylinderGripperName READ cylinderGripperName WRITE setCylinderGripperName NOTIFY cylinderGripperNameChanged)
    Q_PROPERTY(double  pushoutPosition READ pushoutPosition WRITE setPushoutPosition NOTIFY pushoutPositionChanged)
    Q_PROPERTY(double getTrayPosition READ getTrayPosition WRITE setGetTrayPosition NOTIFY getTrayPositionChanged)
    Q_PROPERTY(double putTrayPosition READ putTrayPosition WRITE setPutTrayPosition NOTIFY putTrayPositionChanged)
    Q_PROPERTY(double startKickTrayPosition READ startKickTrayPosition WRITE setStartKickTrayPosition NOTIFY startKickTrayPositionChanged)
    Q_PROPERTY(double vacancyTrayPosition READ vacancyTrayPosition WRITE setVacancyTrayPosition NOTIFY vacancyTrayPositionChanged)
    Q_PROPERTY(double finishKickTrayPosition READ finishKickTrayPosition WRITE setFinishKickTrayPosition NOTIFY finishKickTrayPositionChanged)
    Q_PROPERTY(double trayWorkPosition READ trayWorkPosition WRITE setTrayWorkPosition NOTIFY trayWorkPositionChanged)
    Q_PROPERTY(double downTrayPosition READ downTrayPosition WRITE setDownTrayPosition NOTIFY downTrayPositionChanged)
    Q_PROPERTY(double backDistance READ backDistance WRITE setBackDistance NOTIFY backDistanceChanged)
    Q_PROPERTY(double pushMotorSafePosition READ pushMotorSafePosition WRITE setPushMotorSafePosition NOTIFY pushMotorSafePositionChanged)
    Q_PROPERTY(bool handleVacancyTray READ handleVacancyTray WRITE setHandleVacancyTray NOTIFY handleVacancyTrayChanged)
    Q_PROPERTY(double waitVacancyTrayPosition READ waitVacancyTrayPosition WRITE setWaitVacancyTrayPosition NOTIFY waitVacancyTrayPositionChanged)
    Q_PROPERTY(double pushVelocity READ pushVelocity WRITE setPushVelocity NOTIFY pushVelocityChanged)
    Q_PROPERTY(QString entranceClipCheckoName READ entranceClipCheckoName WRITE setEntranceClipCheckoName NOTIFY entranceClipCheckoNameChanged)
    Q_PROPERTY(QString exitClipCheckIoName READ exitClipCheckIoName WRITE setExitClipCheckIoName NOTIFY exitClipCheckIoNameChanged)
    Q_PROPERTY(QString readyTrayCheckIoName READ readyTrayCheckIoName WRITE setReadyTrayCheckIoName NOTIFY readyTrayCheckIoNameChanged)
    Q_PROPERTY(QString kickTrayCheckIoName READ kickTrayCheckIoName WRITE setKickTrayCheckIoName NOTIFY kickTrayCheckIoNameChanged)
    Q_PROPERTY(QString sensorTrayCheckIoName READ sensorTrayCheckIoName WRITE setSensorTrayCheckIoName NOTIFY sensorTrayCheckIoNameChanged)
    Q_PROPERTY(QString vacancyTrayCheckIoName READ vacancyTrayCheckIoName WRITE setVacancyTrayCheckIoName NOTIFY vacancyTrayCheckIoNameChanged)
    Q_PROPERTY(QString entanceTrayChcekIoName READ entanceTrayChcekIoName WRITE setEntanceTrayChcekIoName NOTIFY entanceTrayChcekIoNameChanged)
    Q_PROPERTY(QString exitTrayCheckIoName READ exitTrayCheckIoName WRITE setExitTrayCheckIoName NOTIFY exitTrayCheckIoNameChanged)
    Q_PROPERTY(bool isHandly READ isHandly WRITE setIsHandly NOTIFY isHandlyChanged)
    QString motorTrayName() const
    {
        return m_motorTrayName;
    }
    QString motorSTIEName() const
    {
        return m_motorSTIEName;
    }

    QString motorSTKName() const
    {
        return m_motorSTKName;
    }

    QString motorSTOEName() const
    {
        return m_motorSTOEName;
    }

    QString motorSPOName() const
    {
        return m_motorSPOName;
    }

    QString cylinderHoldTrayName() const
    {
        return m_cylinderTray1Name;
    }

    QString cylinderVacancyTrayName() const
    {
        return m_cylinderTray2Name;
    }

    QString cylinderSTK1Name() const
    {
        return m_cylinderSTK1Name;
    }

    QString cylinderSTK2Name() const
    {
        return m_cylinderSTK2Name;
    }

    double pushoutPosition() const
    {
        return m_pushoutPosition;
    }

    double getTrayPosition() const
    {
        return m_getTrayPosition;
    }

    double putTrayPosition() const
    {
        return m_putTrayPosition;
    }

    double startKickTrayPosition() const
    {
        return m_startKickTrayPosition;
    }

    double vacancyTrayPosition() const
    {
        return m_vacancyTrayPosition;
    }

    double finishKickTrayPosition() const
    {
        return m_finishKickTrayPosition;
    }

    double trayWorkPosition() const
    {
        return m_trayWorkPosition;
    }

    double downTrayPosition() const
    {
        return m_downTrayPosition;
    }

    double backDistance() const
    {
        return m_backDistance;
    }

    double pushMotorSafePosition() const
    {
        return m_pushMotorSafePosition;
    }

    QString cylinderEntanceClipPushName() const
    {
        return m_cylinderEntanceClip;
    }

    QString cylinderExitClipPushName() const
    {
        return m_cylinderExitClip;
    }

    QString cylinderGripperName() const
    {
        return m_cylinderGripper;
    }

    bool handleVacancyTray() const
    {
        return m_handleVacancyTray;
    }

    double waitVacancyTrayPosition() const
    {
        return m_waitVacancyTrayPosition;
    }

    double pushVelocity() const
    {
        return m_pushVelocity;
    }

    QString entranceClipCheckoName() const
    {
        return m_entranceClipCheckoName;
    }

    QString exitClipCheckIoName() const
    {
        return m_exitClipCheckIoName;
    }

    QString readyTrayCheckIoName() const
    {
        return m_readyTrayCheckIoName;
    }

    QString kickTrayCheckIoName() const
    {
        return m_kickTrayCheckIoName;
    }

    QString sensorTrayCheckIoName() const
    {
        return m_sensorTrayCheckIoName;
    }

    QString vacancyTrayCheckIoName() const
    {
        return m_vacancyTrayCheckIoName;
    }

    QString entanceTrayChcekIoName() const
    {
        return m_entanceTrayChcekIoName;
    }

    QString exitTrayCheckIoName() const
    {
        return m_exitTrayCheckIoName;
    }

    bool isHandly() const
    {
        return m_isHandly;
    }

public slots:
    void setMotorTrayName(QString motorTrayName)
    {
        if (m_motorTrayName == motorTrayName)
            return;

        m_motorTrayName = motorTrayName;
        emit motorTrayNameChanged(m_motorTrayName);
    }
    void setMotorSTIEName(QString motorSTIEName)
    {
        if (m_motorSTIEName == motorSTIEName)
            return;

        m_motorSTIEName = motorSTIEName;
        emit motorSTIENameChanged(m_motorSTIEName);
    }

    void setMotorSTKName(QString motorSTKName)
    {
        if (m_motorSTKName == motorSTKName)
            return;

        m_motorSTKName = motorSTKName;
        emit motorSTKNameChanged(m_motorSTKName);
    }

    void setMotorSTOEName(QString motorSTOEName)
    {
        if (m_motorSTOEName == motorSTOEName)
            return;

        m_motorSTOEName = motorSTOEName;
        emit motorSTOENameChanged(m_motorSTOEName);
    }

    void setMotorSPOName(QString motorSPOName)
    {
        if (m_motorSPOName == motorSPOName)
            return;

        m_motorSPOName = motorSPOName;
        emit motorSPONameChanged(m_motorSPOName);
    }

    void setCylinderHoldTrayName(QString cylinderTray1Name)
    {
        if (m_cylinderTray1Name == cylinderTray1Name)
            return;

        m_cylinderTray1Name = cylinderTray1Name;
        emit cylinderHoldTrayNameChanged(m_cylinderTray1Name);
    }

    void setCylinderVacancyTrayName(QString cylinderTray2Name)
    {
        if (m_cylinderTray2Name == cylinderTray2Name)
            return;

        m_cylinderTray2Name = cylinderTray2Name;
        emit cylinderVacancyTrayNameChanged(m_cylinderTray2Name);
    }

    void setCylinderSTK1Name(QString cylinderSTK1Name)
    {
        if (m_cylinderSTK1Name == cylinderSTK1Name)
            return;

        m_cylinderSTK1Name = cylinderSTK1Name;
        emit cylinderSTK1NameChanged(m_cylinderSTK1Name);
    }

    void setCylinderSTK2Name(QString cylinderSTK2Name)
    {
        if (m_cylinderSTK2Name == cylinderSTK2Name)
            return;

        m_cylinderSTK2Name = cylinderSTK2Name;
        emit cylinderSTK2NameChanged(m_cylinderSTK2Name);
    }

    void setPushoutPosition(double pushoutPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pushoutPosition, pushoutPosition))
            return;

        m_pushoutPosition = pushoutPosition;
        emit pushoutPositionChanged(m_pushoutPosition);
    }

    void setGetTrayPosition(double getTrayPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_getTrayPosition, getTrayPosition))
            return;

        m_getTrayPosition = getTrayPosition;
        emit getTrayPositionChanged(m_getTrayPosition);
    }

    void setPutTrayPosition(double putTrayPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_putTrayPosition, putTrayPosition))
            return;

        m_putTrayPosition = putTrayPosition;
        emit putTrayPositionChanged(m_putTrayPosition);
    }

    void setStartKickTrayPosition(double startKickTrayPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_startKickTrayPosition, startKickTrayPosition))
            return;

        m_startKickTrayPosition = startKickTrayPosition;
        emit startKickTrayPositionChanged(m_startKickTrayPosition);
    }

    void setVacancyTrayPosition(double vacancyTrayPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vacancyTrayPosition, vacancyTrayPosition))
            return;

        m_vacancyTrayPosition = vacancyTrayPosition;
        emit vacancyTrayPositionChanged(m_vacancyTrayPosition);
    }

    void setFinishKickTrayPosition(double finishKickTrayPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_finishKickTrayPosition, finishKickTrayPosition))
            return;

        m_finishKickTrayPosition = finishKickTrayPosition;
        emit finishKickTrayPositionChanged(m_finishKickTrayPosition);
    }

    void setTrayWorkPosition(double trayWorkPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_trayWorkPosition, trayWorkPosition))
            return;

        m_trayWorkPosition = trayWorkPosition;
        emit trayWorkPositionChanged(m_trayWorkPosition);
    }

    void setDownTrayPosition(double downTrayPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_downTrayPosition, downTrayPosition))
            return;

        m_downTrayPosition = downTrayPosition;
        emit downTrayPositionChanged(m_downTrayPosition);
    }

    void setBackDistance(double backDistance)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_backDistance, backDistance))
            return;

        m_backDistance = backDistance;
        emit backDistanceChanged(m_backDistance);
    }

    void setPushMotorSafePosition(double pushMotorSafePosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pushMotorSafePosition, pushMotorSafePosition))
            return;

        m_pushMotorSafePosition = pushMotorSafePosition;
        emit pushMotorSafePositionChanged(m_pushMotorSafePosition);
    }

    void setCylinderEntanceClipPushName(QString cylinderEntanceClip)
    {
        if (m_cylinderEntanceClip == cylinderEntanceClip)
            return;

        m_cylinderEntanceClip = cylinderEntanceClip;
        emit cylinderEntanceClipPushNameChanged(m_cylinderEntanceClip);
    }

    void setCylinderExitClipPushName(QString cylinderExitClip)
    {
        if (m_cylinderExitClip == cylinderExitClip)
            return;

        m_cylinderExitClip = cylinderExitClip;
        emit cylinderExitClipNamePushChanged(m_cylinderExitClip);
    }

    void setCylinderGripperName(QString cylinderGripper)
    {
        if (m_cylinderGripper == cylinderGripper)
            return;

        m_cylinderGripper = cylinderGripper;
        emit cylinderGripperNameChanged(m_cylinderGripper);
    }

    void setHandleVacancyTray(bool handleVacancyTray)
    {
        if (m_handleVacancyTray == handleVacancyTray)
            return;

        m_handleVacancyTray = handleVacancyTray;
        emit handleVacancyTrayChanged(m_handleVacancyTray);
    }

    void setWaitVacancyTrayPosition(double waitVacancyTrayPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_waitVacancyTrayPosition, waitVacancyTrayPosition))
            return;

        m_waitVacancyTrayPosition = waitVacancyTrayPosition;
        emit waitVacancyTrayPositionChanged(m_waitVacancyTrayPosition);
    }

    void setPushVelocity(double pushVelocity)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pushVelocity, pushVelocity))
            return;

        m_pushVelocity = pushVelocity;
        emit pushVelocityChanged(m_pushVelocity);
    }

    void setEntranceClipCheckoName(QString entranceClipCheckoName)
    {
        if (m_entranceClipCheckoName == entranceClipCheckoName)
            return;

        m_entranceClipCheckoName = entranceClipCheckoName;
        emit entranceClipCheckoNameChanged(m_entranceClipCheckoName);
    }

    void setExitClipCheckIoName(QString exitClipCheckIoName)
    {
        if (m_exitClipCheckIoName == exitClipCheckIoName)
            return;

        m_exitClipCheckIoName = exitClipCheckIoName;
        emit exitClipCheckIoNameChanged(m_exitClipCheckIoName);
    }

    void setReadyTrayCheckIoName(QString readyTrayCheckIoName)
    {
        if (m_readyTrayCheckIoName == readyTrayCheckIoName)
            return;

        m_readyTrayCheckIoName = readyTrayCheckIoName;
        emit readyTrayCheckIoNameChanged(m_readyTrayCheckIoName);
    }

    void setKickTrayCheckIoName(QString kickTrayCheckIoName)
    {
        if (m_kickTrayCheckIoName == kickTrayCheckIoName)
            return;

        m_kickTrayCheckIoName = kickTrayCheckIoName;
        emit kickTrayCheckIoNameChanged(m_kickTrayCheckIoName);
    }

    void setSensorTrayCheckIoName(QString sensorTrayCheckIoName)
    {
        if (m_sensorTrayCheckIoName == sensorTrayCheckIoName)
            return;

        m_sensorTrayCheckIoName = sensorTrayCheckIoName;
        emit sensorTrayCheckIoNameChanged(m_sensorTrayCheckIoName);
    }

    void setVacancyTrayCheckIoName(QString vacancyTrayCheckIoName)
    {
        if (m_vacancyTrayCheckIoName == vacancyTrayCheckIoName)
            return;

        m_vacancyTrayCheckIoName = vacancyTrayCheckIoName;
        emit vacancyTrayCheckIoNameChanged(m_vacancyTrayCheckIoName);
    }

    void setEntanceTrayChcekIoName(QString entanceTrayChcekIoName)
    {
        if (m_entanceTrayChcekIoName == entanceTrayChcekIoName)
            return;

        m_entanceTrayChcekIoName = entanceTrayChcekIoName;
        emit entanceTrayChcekIoNameChanged(m_entanceTrayChcekIoName);
    }

    void setExitTrayCheckIoName(QString exitTrayCheckIoName)
    {
        if (m_exitTrayCheckIoName == exitTrayCheckIoName)
            return;

        m_exitTrayCheckIoName = exitTrayCheckIoName;
        emit exitTrayCheckIoNameChanged(m_exitTrayCheckIoName);
    }

    void setIsHandly(bool isHandly)
    {
        if (m_isHandly == isHandly)
            return;

        m_isHandly = isHandly;
        emit isHandlyChanged(m_isHandly);
    }

signals:
    void motorTrayNameChanged(QString motorTrayName);
    void motorSTIENameChanged(QString motorSTIEName);

    void motorSTKNameChanged(QString motorSTKName);

    void motorSTOENameChanged(QString motorSTOEName);

    void motorSPONameChanged(QString motorSPOName);

    void cylinderHoldTrayNameChanged(QString cylinderHoldTrayName);

    void cylinderVacancyTrayNameChanged(QString cylinderVacancyTrayName);

    void cylinderSTK1NameChanged(QString cylinderSTK1Name);

    void cylinderSTK2NameChanged(QString cylinderSTK2Name);

    void pushoutPositionChanged(double pushoutPosition);

    void getTrayPositionChanged(double getTrayPosition);

    void putTrayPositionChanged(double putTrayPosition);

    void startKickTrayPositionChanged(double startKickTrayPosition);

    void vacancyTrayPositionChanged(double vacancyTrayPosition);

    void finishKickTrayPositionChanged(double finishKickTrayPosition);

    void trayWorkPositionChanged(double trayWorkPosition);

    void downTrayPositionChanged(double downTrayPosition);

    void backDistanceChanged(double backDistance);

    void pushMotorSafePositionChanged(double pushMotorSafePosition);

    void cylinderEntanceClipPushNameChanged(QString cylinderEntanceClipPushName);

    void cylinderExitClipNamePushChanged(QString cylinderExitClipPushName);

    void cylinderGripperNameChanged(QString cylinderGripperName);

    void handleVacancyTrayChanged(bool handleVacancyTray);

    void waitVacancyTrayPositionChanged(double waitVacancyTrayPosition);

    void pushVelocityChanged(double pushVelocity);

    void entranceClipCheckoNameChanged(QString entranceClipCheckoName);

    void exitClipCheckIoNameChanged(QString exitClipCheckIoName);

    void readyTrayCheckIoNameChanged(QString readyTrayCheckIoName);

    void kickTrayCheckIoNameChanged(QString kickTrayCheckIoName);

    void sensorTrayCheckIoNameChanged(QString sensorTrayCheckIoName);

    void vacancyTrayCheckIoNameChanged(QString vacancyTrayCheckIoName);

    void entanceTrayChcekIoNameChanged(QString entanceTrayChcekIoName);

    void exitTrayCheckIoNameChanged(QString exitTrayCheckIoName);

    void isHandlyChanged(bool isHandly);

private:
    QString m_motorTrayName = "";
    QString m_motorSTIEName = "";
    QString m_motorSTKName = "";
    QString m_motorSTOEName = "";
    QString m_motorSPOName = "";
    QString m_cylinderTray1Name = "";
    QString m_cylinderTray2Name = "";
    QString m_cylinderSTK1Name = "";
    QString m_cylinderSTK2Name = "";
    double m_pushoutPosition = 0;
    double m_getTrayPosition = 0;
    double m_putTrayPosition = 0;
    double m_startKickTrayPosition = 0;
    double m_vacancyTrayPosition = 0;
    double m_finishKickTrayPosition = 0;
    double m_trayWorkPosition = 0;
    double m_downTrayPosition = 0;
    double m_backDistance = 5;
    double m_pushMotorSafePosition = 1;
    QString m_cylinderEntanceClip = "";
    QString m_cylinderExitClip = "";
    QString m_cylinderGripper = "";
    bool m_handleVacancyTray = false;
    double m_waitVacancyTrayPosition = 0;
    double m_pushVelocity = 100;
    QString m_entranceClipCheckoName = "";
    QString m_exitClipCheckIoName = "";
    QString m_readyTrayCheckIoName = "";
    QString m_kickTrayCheckIoName = "";
    QString m_sensorTrayCheckIoName = "";
    QString m_vacancyTrayCheckIoName = "";
    QString m_entanceTrayChcekIoName = "";
    QString m_exitTrayCheckIoName = "";
    bool m_isHandly = false;
};
class SensorTrayLoaderState:public PropertyBase
{
    Q_OBJECT
public:
    SensorTrayLoaderState():PropertyBase() {init_values.insert("isFirstTray",true);}
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(bool handlyChangeTray READ handlyChangeTray WRITE setHandlyChangeTray NOTIFY handlyChangeTrayChanged)
    Q_PROPERTY(bool handlyChangeSensor READ handlyChangeSensor WRITE setHandlyChangeSensor NOTIFY handlyChangeSensorChanged)
    Q_PROPERTY(bool hasTrayToGet READ hasTrayToGet WRITE setHasTrayToGet NOTIFY hasTrayToGetChanged)
    Q_PROPERTY(bool hasVacancyTray READ hasVacancyTray WRITE setHasVacancyTray NOTIFY hasVacancyTrayChanged)
    Q_PROPERTY(bool handlePutVacancyTray READ handlePutVacancyTray WRITE setHandlePutVacancyTray NOTIFY handlePutVacancyTrayChanged)
    Q_PROPERTY(bool firstTrayIsVacancy READ firstTrayIsVacancy WRITE setFirstTrayIsVacancy NOTIFY firstTrayIsVacancyChanged)
    Q_PROPERTY(bool needChangeTray READ needChangeTray WRITE setNeedChangeTray NOTIFY needChangeTrayChanged)
    Q_PROPERTY(bool changingTray READ changingTray WRITE setChangingTray NOTIFY changingTrayChanged)
    Q_PROPERTY(bool entranceClipReady READ entranceClipReady WRITE setEntranceClipReady NOTIFY entranceClipReadyChanged)
    Q_PROPERTY(bool exitClipReay READ exitClipReay WRITE setExitClipReady NOTIFY exitClipReadyChanged)
    Q_PROPERTY(bool hasGetedTray READ hasGetedTray WRITE setHasGetedTray NOTIFY hasGetedTrayChanged)
    Q_PROPERTY(bool hasWorkTray READ hasWorkTray WRITE setHasWorkTray NOTIFY hasWorkTrayChanged)
    Q_PROPERTY(bool hasKickTray READ hasKickTray WRITE setHasKickTray NOTIFY hasKickTrayChanged)
    Q_PROPERTY(bool needChangeVacancyTray READ needChangeVacancyTray WRITE setNeedChangeVacancyTray NOTIFY needdChangeVacancyTrayChanged)
    Q_PROPERTY(bool needChangeEntranceClip READ needChangeEntranceClip WRITE setNeedChangeEntranceClip NOTIFY needChangeEntranceClipChanged)
    Q_PROPERTY(bool needChangeExitClip READ needChangeExitClip WRITE setNeedChangeExitClip NOTIFY needChangeExitClipChanged)
    Q_PROPERTY(bool useSpareEntanceClip READ useSpareEntanceClip WRITE setUseSpareEntanceClip NOTIFY useSpareEntanceClipChanged)
    Q_PROPERTY(bool useSpareExitClip READ useSpareExitClip WRITE setUseSpareExitClip NOTIFY useSpareExitClipChanged)
    Q_PROPERTY(bool holdVacancyTray READ holdVacancyTray WRITE setHoldVacancyTray NOTIFY holdVacancyTrayChanged)
    Q_PROPERTY(bool getedVacancyTray READ getedVacancyTray WRITE setGetedVacancyTray NOTIFY getedVacancyTrayChanged)
    Q_PROPERTY(bool hasReadyTray READ hasReadyTray WRITE setHasReadyTray NOTIFY hasReadyTrayChanged)
    Q_PROPERTY(bool hasProductTray READ hasProductTray WRITE setHasProductTray NOTIFY hasProductTrayChanged)
    Q_PROPERTY(bool hasKickReady READ hasKickReady WRITE setHasKickReady NOTIFY hasKickReadyChanged)
    Q_PROPERTY(bool isFirstTray READ isFirstTray WRITE setIsFirstTray NOTIFY isFirstTrayChanged)
    Q_PROPERTY(bool hasCarrierReady READ hasCarrierReady WRITE setHasCarrierReady NOTIFY hasCarrierReadyChanged)
    Q_PROPERTY(bool hasUpTray READ hasUpTray WRITE setHasUpTray NOTIFY hasUpTrayChanged)
    bool hasTrayToGet() const
    {
        return m_hasTrayToGet;
    }
    bool hasVacancyTray() const
    {
        return m_hasVacancyTray;
    }

    bool handlePutVacancyTray() const
    {
        return m_handlePutVacancyTray;
    }

    bool firstTrayIsVacancy() const
    {
        return m_firstTrayIsVacancy;
    }

    bool needChangeTray() const
    {
        return m_needChangeTray;
    }

    bool changingTray() const
    {
        return m_changingTray;
    }

    bool entranceClipReady() const
    {
        return m_entranceClipReady;
    }

    bool exitClipReay() const
    {
        return m_exitClipReady;
    }

    bool hasGetedTray() const
    {
        return m_hasGetedTray;
    }

    bool hasWorkTray() const
    {
        return m_hasWorkTray;
    }

    bool hasKickTray() const
    {
        return m_hasKickTray;
    }

    bool needChangeVacancyTray() const
    {
        return m_needChangeVacancyTray;
    }

    bool needChangeEntranceClip() const
    {
        return m_needChangeEntranceClip;
    }

    bool needChangeExitClip() const
    {
        return m_needChangeExitClip;
    }

    bool useSpareEntanceClip() const
    {
        return m_useSpareEntanceClip;
    }

    bool useSpareExitClip() const
    {
        return m_useSpareExitClip;
    }

    bool holdVacancyTray() const
    {
        return m_holdVacancyTray;
    }

    bool getedVacancyTray() const
    {
        return m_getedVacancyTray;
    }

    bool hasReadyTray() const
    {
        return m_hasReadyTray;
    }

    bool hasProductTray() const
    {
        return m_hasProductTray;
    }

    bool hasKickReady() const
    {
        return m_hasKickReady;
    }

    bool isFirstTray() const
    {
        return m_isFirstTray;
    }

    bool hasCarrierReady() const
    {
        return m_hasCarrierReady;
    }

    bool hasUpTray() const
    {
        return m_hasUpTray;
    }

    int runMode() const
    {
        return m_runMode;
    }

    bool handlyChangeTray() const
    {
        return m_handlyChangeTray;
    }

    bool name() const
    {
        return m_handlyChangeSensor;
    }

    bool handlyChangeSensor() const
    {
        return m_handlyChangeSensor;
    }

public slots:
    void setHasTrayToGet(bool hasTrayToGet)
    {
        if (m_hasTrayToGet == hasTrayToGet)
            return;

        m_hasTrayToGet = hasTrayToGet;
        emit hasTrayToGetChanged(m_hasTrayToGet);
    }
    void setHasVacancyTray(bool hasVacancyTray)
    {
        if (m_hasVacancyTray == hasVacancyTray)
            return;

        m_hasVacancyTray = hasVacancyTray;
        emit hasVacancyTrayChanged(m_hasVacancyTray);
    }

    void setHandlePutVacancyTray(bool handlePutVacancyTray)
    {
        if (m_handlePutVacancyTray == handlePutVacancyTray)
            return;

        m_handlePutVacancyTray = handlePutVacancyTray;
        emit handlePutVacancyTrayChanged(m_handlePutVacancyTray);
    }

    void setFirstTrayIsVacancy(bool firstTrayIsVacancy)
    {
        if (m_firstTrayIsVacancy == firstTrayIsVacancy)
            return;

        m_firstTrayIsVacancy = firstTrayIsVacancy;
        emit firstTrayIsVacancyChanged(m_firstTrayIsVacancy);
    }

    void setNeedChangeTray(bool needChangeTray)
    {
        if (m_needChangeTray == needChangeTray)
            return;

        m_needChangeTray = needChangeTray;
        emit needChangeTrayChanged(m_needChangeTray);
    }

    void setChangingTray(bool changingTray)
    {
        if (m_changingTray == changingTray)
            return;

        m_changingTray = changingTray;
        emit changingTrayChanged(m_changingTray);
    }

    void setEntranceClipReady(bool hasEntranceClip)
    {
        if (m_entranceClipReady == hasEntranceClip)
            return;

        m_entranceClipReady = hasEntranceClip;
        emit entranceClipReadyChanged(m_entranceClipReady);
    }

    void setExitClipReady(bool hasExitClip)
    {
        if (m_exitClipReady == hasExitClip)
            return;

        m_exitClipReady = hasExitClip;
        emit exitClipReadyChanged(m_exitClipReady);
    }

    void setHasGetedTray(bool hasGetedTray)
    {
        if (m_hasGetedTray == hasGetedTray)
            return;

        m_hasGetedTray = hasGetedTray;
        emit hasGetedTrayChanged(m_hasGetedTray);
    }

    void setHasWorkTray(bool hasWorkTray)
    {
        if (m_hasWorkTray == hasWorkTray)
            return;

        m_hasWorkTray = hasWorkTray;
        emit hasWorkTrayChanged(m_hasWorkTray);
    }

    void setHasKickTray(bool hasKickTray)
    {
        if (m_hasKickTray == hasKickTray)
            return;

        m_hasKickTray = hasKickTray;
        emit hasKickTrayChanged(m_hasKickTray);
    }

    void setNeedChangeVacancyTray(bool hasChangeVacancyTray)
    {
        if (m_needChangeVacancyTray == hasChangeVacancyTray)
            return;

        m_needChangeVacancyTray = hasChangeVacancyTray;
        emit needdChangeVacancyTrayChanged(m_needChangeVacancyTray);
    }

    void setNeedChangeEntranceClip(bool needChangeEntranceClip)
    {
        if (m_needChangeEntranceClip == needChangeEntranceClip)
            return;

        m_needChangeEntranceClip = needChangeEntranceClip;
        emit needChangeEntranceClipChanged(m_needChangeEntranceClip);
    }

    void setNeedChangeExitClip(bool needChangeExitClip)
    {
        if (m_needChangeExitClip == needChangeExitClip)
            return;

        m_needChangeExitClip = needChangeExitClip;
        emit needChangeExitClipChanged(m_needChangeExitClip);
    }

    void setUseSpareEntanceClip(bool useSpareEntanceClip)
    {
        if (m_useSpareEntanceClip == useSpareEntanceClip)
            return;

        m_useSpareEntanceClip = useSpareEntanceClip;
        emit useSpareEntanceClipChanged(m_useSpareEntanceClip);
    }

    void setUseSpareExitClip(bool useSpareExitClip)
    {
        if (m_useSpareExitClip == useSpareExitClip)
            return;

        m_useSpareExitClip = useSpareExitClip;
        emit useSpareExitClipChanged(m_useSpareExitClip);
    }

    void setHoldVacancyTray(bool holdVacancyTray)
    {
        if (m_holdVacancyTray == holdVacancyTray)
            return;

        m_holdVacancyTray = holdVacancyTray;
        emit holdVacancyTrayChanged(m_holdVacancyTray);
    }

    void setGetedVacancyTray(bool getedVacancyTray)
    {
        if (m_getedVacancyTray == getedVacancyTray)
            return;

        m_getedVacancyTray = getedVacancyTray;
        emit getedVacancyTrayChanged(m_getedVacancyTray);
    }

    void setHasReadyTray(bool hasReadyTray)
    {
        if (m_hasReadyTray == hasReadyTray)
            return;

        m_hasReadyTray = hasReadyTray;
        emit hasReadyTrayChanged(m_hasReadyTray);
    }

    void setHasProductTray(bool hasProductTray)
    {
        if (m_hasProductTray == hasProductTray)
            return;

        m_hasProductTray = hasProductTray;
        emit hasProductTrayChanged(m_hasProductTray);
    }

    void setHasKickReady(bool hasKickReady)
    {
        if (m_hasKickReady == hasKickReady)
            return;

        m_hasKickReady = hasKickReady;
        emit hasKickReadyChanged(m_hasKickReady);
    }

    void setIsFirstTray(bool isFirstTray)
    {
        if (m_isFirstTray == isFirstTray)
            return;

        m_isFirstTray = isFirstTray;
        emit isFirstTrayChanged(m_isFirstTray);
    }

    void setHasCarrierReady(bool hasCarrierReady)
    {
        if (m_hasCarrierReady == hasCarrierReady)
            return;

        m_hasCarrierReady = hasCarrierReady;
        emit hasCarrierReadyChanged(m_hasCarrierReady);
    }

    void setHasUpTray(bool hasUpTray)
    {
        if (m_hasUpTray == hasUpTray)
            return;

        m_hasUpTray = hasUpTray;
        emit hasUpTrayChanged(m_hasUpTray);
    }

    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }

    void setHandlyChangeTray(bool handlyChangeTray)
    {
        if (m_handlyChangeTray == handlyChangeTray)
            return;

        m_handlyChangeTray = handlyChangeTray;
        emit handlyChangeTrayChanged(m_handlyChangeTray);
    }

    void setHandlyChangeSensor(bool handlyChangeSensor)
    {
        if (m_handlyChangeSensor == handlyChangeSensor)
            return;

        m_handlyChangeSensor = handlyChangeSensor;
        emit handlyChangeSensorChanged(m_handlyChangeSensor);
    }

signals:
    void hasTrayToGetChanged(bool hasTrayToGet);
    void hasVacancyTrayChanged(bool hasVacancyTray);

    void handlePutVacancyTrayChanged(bool handlePutVacancyTray);

    void firstTrayIsVacancyChanged(bool firstTrayIsVacancy);

    void needChangeTrayChanged(bool needChangeTray);

    void changingTrayChanged(bool changingTray);

    void entranceClipReadyChanged(bool entranceClipReady);

    void exitClipReadyChanged(bool exitClipReay);

    void hasGetedTrayChanged(bool hasGetedTray);

    void hasWorkTrayChanged(bool hasWorkTray);

    void hasKickTrayChanged(bool hasKickTray);

    void needdChangeVacancyTrayChanged(bool needChangeVacancyTray);

    void needChangeEntranceClipChanged(bool needChangeEntranceClip);

    void needChangeExitClipChanged(bool needChangeExitClip);

    void useSpareEntanceClipChanged(bool useSpareEntanceClip);

    void useSpareExitClipChanged(bool useSpareExitClip);

    void holdVacancyTrayChanged(bool holdVacancyTray);

    void getedVacancyTrayChanged(bool getedVacancyTray);

    void hasReadyTrayChanged(bool hasReadyTray);

    void hasProductTrayChanged(bool hasProductTray);

    void hasKickReadyChanged(bool hasKickReady);

    void isFirstTrayChanged(bool isFirstTray);

    void hasCarrierReadyChanged(bool hasCarrierReady);

    void hasUpTrayChanged(bool hasUpTray);

    void runModeChanged(int runMode);

    void handlyChangeTrayChanged(bool handlyChangeTray);

    void handlyChangeSensorChanged(bool handlyChangeSensor);

private:
    bool m_hasTrayToGet = false;
    bool m_hasVacancyTray = false;
    bool m_handlePutVacancyTray = false;
    bool m_firstTrayIsVacancy = false;
    bool m_needChangeTray = false;
    bool m_changingTray = false;
    bool m_entranceClipReady = false;
    bool m_exitClipReady = false;
    bool m_hasGetedTray = false;
    bool m_hasWorkTray = false;
    bool m_hasKickTray = false;
    bool m_needChangeVacancyTray = false;
    bool m_needChangeEntranceClip = false;
    bool m_needChangeExitClip = false;
    bool m_useSpareEntanceClip  = false;
    bool m_useSpareExitClip  = false;
    bool m_holdVacancyTray  = false;
    bool m_getedVacancyTray = false;
    bool m_hasReadyTray = false;
    bool m_hasProductTray = false;
    bool m_hasKickReady = false;
    bool m_isFirstTray = true;
    bool m_hasCarrierReady = false;
    bool m_hasUpTray = false;
    int m_runMode = 0;
    bool m_handlyChangeTray = false;
    bool m_handlyChangeSensor = false;
};
#endif // SENSORTRAYLOADERPARAMETER_H
