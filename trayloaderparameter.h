#ifndef TRAYLOADERPARAMETER_H
#define TRAYLOADERPARAMETER_H

#include "propertybase.h"


class TrayLoaderModuleParameter:public PropertyBase{
    Q_OBJECT
private:
    QString m_motorLTIEName = "";
    QString m_motorLTOEName = "";
    QString m_motorLTKX1Name = "";
    QString m_motorLTKX2Name = "";
    QString m_motorLTLName = "";

    QString m_cylinderClipName = "";
    QString m_cylinderLTK1Name = "";
    QString m_cylinderLTK2Name = "";
    QString m_cylinderTrayName = "";

    double m_ltkx1GetPos = 0;
    double m_ltkx1SetPos = 0;
    double m_ltlGetPos = 0;
    double m_ltlSetPos = 0;
    double m_ltkx2GetPos = 0;
    double m_ltkx2SetPos = 0;
    double m_ltkx1RelayPos = 0;

    QString m_clipinInputName = "";

    QString m_clipoutInputName = "";

    double m_ltkx2SafeDistance = 5;

    double m_ltlWorkPos = 0;

    bool m_openQinfo = true;

    QString m_trayInputIoName = "";

    bool m_isHandly = false;

    double m_pushVelocity = 50;

    QString m_entranceTrayCheckIoName = "";

    QString m_exitTrayCheckIoName = "";

    QString m_entranceClipCheckIoName = "";

    QString m_exitClipCheckIoName = "";

    QString m_readyTrayCheckIoName = "";

    QString m_workTrayCheckIoName = "";

    bool m_handlyChangeLensTray = false;

    QString m_moduleName = "LensTrayLoaderModule";

    QString m_tcpCylinderClipIOName;

    QString m_tcpCylinderLTKX1IOName;

QString m_tcpCylinderLTKX2IOName;

QString m_tcpCylinderLTLIOName;

QString m_tcpCylinderClipSensorName;

QString m_tcpCylinderLTKX1SensorName;

QString m_tcpCylinderLTKX2SensorName;

QString m_tcpCylinderLTLSensorName;

signals:
    void testTrayTest();
    void trayReady();
public:
    TrayLoaderModuleParameter():PropertyBase(){}
    Q_PROPERTY(bool handlyChangeLensTray READ handlyChangeLensTray WRITE setHandlyChangeLensTray NOTIFY handlyChangeLensTrayChanged)
    Q_PROPERTY(double ltkx1RelayPos READ ltkx1RelayPos WRITE setLtkx1RelayPos NOTIFY ltkx1RelayPosChanged)
    Q_PROPERTY(double ltkx1PressPos READ ltkx1PressPos WRITE setLtkx1PressPos NOTIFY ltkx1PressPosChanged)
    Q_PROPERTY(double ltkx1ReleasePos READ ltkx1ReleasePos WRITE setLtkx1ReleasePos NOTIFY ltkx1ReleasePosChanged)
    Q_PROPERTY(double ltlPressPos READ ltlPressPos WRITE setLtlPressPos NOTIFY ltlPressPosChanged)
    Q_PROPERTY(double ltlReleasePos READ ltlReleasePos WRITE setLtlReleasePos NOTIFY ltlReleasePosChanged)
    Q_PROPERTY(double ltkx2PressPos READ ltkx2PressPos WRITE setLtkx2PressPos NOTIFY ltkx2PressPosChanged)
    Q_PROPERTY(double ltkx2ReleasePos READ ltkx2ReleasePos WRITE setLtkx2ReleasePos NOTIFY ltkx2ReleasePosChanged)
    Q_PROPERTY(double ltkx2SafeDistance READ ltkx2SafeDistance WRITE setLtkx2SafeDistance NOTIFY ltkx2SafeDistanceChanged)
    Q_PROPERTY(double ltlWorkPos READ ltlWorkPos WRITE setLtlWorkPos NOTIFY ltlWorkPosChanged)
    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)
    Q_PROPERTY(QString motorLTIEName READ motorLTIEName WRITE setMotorLTIEName NOTIFY motorTLIENameChanged)
    Q_PROPERTY(QString motorLTOEName READ motorLTOEName WRITE setMotorLTOEName NOTIFY motorLTOENameChanged)
    Q_PROPERTY(QString motorLTKX1Name READ motorLTKX1Name WRITE setMotorLTKX1Name NOTIFY motorLTKX1NameChanged)
    Q_PROPERTY(QString motorLTKX2Name READ motorLTKX2Name WRITE setMotorLTKX2Name NOTIFY motorLTKX2NameChanged)
    Q_PROPERTY(QString motorLTLXName READ motorLTLXName WRITE setMotorLTLName NOTIFY motorLTLNameChanged)
    Q_PROPERTY(QString cylinderClipName READ cylinderClipName WRITE setCylinderClip NOTIFY cylinderClipChanged)
    Q_PROPERTY(QString cylinderLTK1Name READ cylinderLTK1Name WRITE setCylinderLTK1Name NOTIFY cylinderLTK1NameChanged)
    Q_PROPERTY(QString cylinderLTK2Name READ cylinderLTK2Name WRITE setCylinderLTK2Name NOTIFY cylinderLTK2NameChanged)
    Q_PROPERTY(QString cylinderTrayName READ cylinderTrayName WRITE setCylinderTrayName NOTIFY cylinderTrayNameChanged)
    Q_PROPERTY(QString clipinInputName READ clipinInputName WRITE setClipinInputName NOTIFY clipinInputNameChanged)
    Q_PROPERTY(QString clipoutInputName READ clipoutInputName WRITE setClipoutInputName NOTIFY clipoutInputNameChanged)
    Q_PROPERTY(bool openQinfo READ openQinfo WRITE setOpenQinfo NOTIFY openQinfoChanged)
    Q_PROPERTY(QString trayInputIoName READ trayInputIoName WRITE setTrayInputIoName NOTIFY trayInputIoNameChanged)
    Q_PROPERTY(bool isHandly READ isHandly WRITE setIsHandly NOTIFY isHandlyChanged)
    Q_PROPERTY(double pushVelocity READ pushVelocity WRITE setPushVelocity NOTIFY pushVelocityChanged)
    Q_PROPERTY(QString entranceTrayCheckIoName READ entranceTrayCheckIoName WRITE setEntranceTrayCheckIoName NOTIFY entranceTrayCheckIoNameChanged)
    Q_PROPERTY(QString exitTrayCheckIoName READ exitTrayCheckIoName WRITE setExitTrayCheckIoName NOTIFY exitTrayCheckIoNameChanged)
    Q_PROPERTY(QString entranceClipCheckIoName READ entranceClipCheckIoName WRITE setEntranceClipCheckIoName NOTIFY entranceClipCheckIoNameChanged)
    Q_PROPERTY(QString exitClipCheckIoName READ exitClipCheckIoName WRITE setExitClipCheckIoName NOTIFY exitClipCheckIoNameChanged)
    Q_PROPERTY(QString readyTrayCheckIoName READ readyTrayCheckIoName WRITE setReadyTrayCheckIoName NOTIFY readyTrayCheckIoNameChanged)
    Q_PROPERTY(QString workTrayCheckIoName READ workTrayCheckIoName WRITE setWorkTrayCheckIoName NOTIFY workTrayCheckIoNameChanged)
    Q_PROPERTY(QString tcpCylinderClipIOName READ tcpCylinderClipIOName WRITE setTcpCylinderClipIOName NOTIFY tcpCylinderClipIONameChanged)
    Q_PROPERTY(QString tcpCylinderLTKX1IOName READ tcpCylinderLTKX1IOName WRITE setTcpCylinderLTKX1IOName NOTIFY tcpCylinderLTKX1IONameChanged)
    Q_PROPERTY(QString tcpCylinderLTKX2IOName READ tcpCylinderLTKX2IOName WRITE setTcpCylinderLTKX2IOName NOTIFY tcpCylinderLTKX2IONameChanged)
    Q_PROPERTY(QString tcpCylinderLTLIOName READ tcpCylinderLTLIOName WRITE setTcpCylinderLTLIOName NOTIFY tcpCylinderLTLIONameChanged)
    Q_PROPERTY(QString tcpCylinderClipSensorName READ tcpCylinderClipSensorName WRITE setTcpCylinderClipSensorName NOTIFY tcpCylinderClipSensorNameChanged)
    Q_PROPERTY(QString tcpCylinderLTKX1SensorName READ tcpCylinderLTKX1SensorName WRITE setTcpCylinderLTKX1SensorName NOTIFY tcpCylinderLTKX1SensorNameChanged)
    Q_PROPERTY(QString tcpCylinderLTKX2SensorName READ tcpCylinderLTKX2SensorName WRITE setTcpCylinderLTKX2SensorName NOTIFY tcpCylinderLTKX2SensorNameChanged)
    Q_PROPERTY(QString tcpCylinderLTLSensorName READ tcpCylinderLTLSensorName WRITE setTcpCylinderLTLSensorName NOTIFY tcpCylinderLTLSensorNameChanged)
    QString motorLTIEName() const
    {
        return m_motorLTIEName;
    }

    QString motorLTOEName() const
    {
        return m_motorLTOEName;
    }

    QString motorLTKX1Name() const
    {
        return m_motorLTKX1Name;
    }

    QString motorLTKX2Name() const
    {
        return m_motorLTKX2Name;
    }

    QString motorLTLXName() const
    {
        return m_motorLTLName;
    }

    QString cylinderClipName() const
    {
        return m_cylinderClipName;
    }

    QString cylinderLTK1Name() const
    {
        return m_cylinderLTK1Name;
    }

    QString cylinderLTK2Name() const
    {
        return m_cylinderLTK2Name;
    }

    QString cylinderTrayName() const
    {
        return m_cylinderTrayName;
    }

    double ltkx1PressPos() const
    {
        return m_ltkx1GetPos;
    }

    double ltkx1ReleasePos() const
    {
        return m_ltkx1SetPos;
    }

    double ltlPressPos() const
    {
        return m_ltlGetPos;
    }

    double ltlReleasePos() const
    {
        return m_ltlSetPos;
    }

    double ltkx2PressPos() const
    {
        return m_ltkx2GetPos;
    }

    double ltkx2ReleasePos() const
    {
        return m_ltkx2SetPos;
    }

    double ltkx1RelayPos() const
    {
        return m_ltkx1RelayPos;
    }

    QString clipinInputName() const
    {
        return m_clipinInputName;
    }

    QString clipoutInputName() const
    {
        return m_clipoutInputName;
    }

    double ltkx2SafeDistance() const
    {
        return m_ltkx2SafeDistance;
    }

    double ltlWorkPos() const
    {
        return m_ltlWorkPos;
    }

    bool openQinfo() const
    {
        return m_openQinfo;
    }

    QString trayInputIoName() const
    {
        return m_trayInputIoName;
    }

    bool isHandly() const
    {
        return m_isHandly;
    }

    double pushVelocity() const
    {
        return m_pushVelocity;
    }

    QString entranceTrayCheckIoName() const
    {
        return m_entranceTrayCheckIoName;
    }

    QString exitTrayCheckIoName() const
    {
        return m_exitTrayCheckIoName;
    }

    QString entranceClipCheckIoName() const
    {
        return m_entranceClipCheckIoName;
    }

    QString exitClipCheckIoName() const
    {
        return m_exitClipCheckIoName;
    }

    QString readyTrayCheckIoName() const
    {
        return m_readyTrayCheckIoName;
    }

    QString workTrayCheckIoName() const
    {
        return m_workTrayCheckIoName;
    }

    bool handlyChangeLensTray() const
    {
        return m_handlyChangeLensTray;
    }

    QString moduleName() const
    {
        return m_moduleName;
    }

    QString tcpCylinderClipIOName() const
    {
        return m_tcpCylinderClipIOName;
    }

    QString tcpCylinderLTKX1IOName() const
    {
        return m_tcpCylinderLTKX1IOName;
    }

    QString tcpCylinderLTKX2IOName() const
    {
        return m_tcpCylinderLTKX2IOName;
    }

    QString tcpCylinderLTLIOName() const
    {
        return m_tcpCylinderLTLIOName;
    }

    QString tcpCylinderClipSensorName() const
    {
        return m_tcpCylinderClipSensorName;
    }

    QString tcpCylinderLTKX1SensorName() const
    {
        return m_tcpCylinderLTKX1SensorName;
    }

    QString tcpCylinderLTKX2SensorName() const
    {
        return m_tcpCylinderLTKX2SensorName;
    }

    QString tcpCylinderLTLSensorName() const
    {
        return m_tcpCylinderLTLSensorName;
    }

public slots:

    void setMotorLTIEName(QString motorLTIEName)
    {
        if (m_motorLTIEName == motorLTIEName)
            return;

        m_motorLTIEName = motorLTIEName;
        emit motorTLIENameChanged(m_motorLTIEName);
    }

    void setMotorLTOEName(QString motorLTOEName)
    {
        if (m_motorLTOEName == motorLTOEName)
            return;

        m_motorLTOEName = motorLTOEName;
        emit motorLTOENameChanged(m_motorLTOEName);
    }

    void setMotorLTKX1Name(QString motorLTKX1Name)
    {
        if (m_motorLTKX1Name == motorLTKX1Name)
            return;

        m_motorLTKX1Name = motorLTKX1Name;
        emit motorLTKX1NameChanged(m_motorLTKX1Name);
    }

    void setMotorLTKX2Name(QString motorLTKX2Name)
    {
        if (m_motorLTKX2Name == motorLTKX2Name)
            return;

        m_motorLTKX2Name = motorLTKX2Name;
        emit motorLTKX2NameChanged(m_motorLTKX2Name);
    }

    void setMotorLTLName(QString motorLTLName)
    {
        if (m_motorLTLName == motorLTLName)
            return;

        m_motorLTLName = motorLTLName;
        emit motorLTLNameChanged(m_motorLTLName);
    }

    void setCylinderClip(QString cylinderClipName)
    {
        if (m_cylinderClipName == cylinderClipName)
            return;

        m_cylinderClipName = cylinderClipName;
        emit cylinderClipChanged(m_cylinderClipName);
    }

    void setCylinderLTK1Name(QString cylinderLTK1Name)
    {
        if (m_cylinderLTK1Name == cylinderLTK1Name)
            return;

        m_cylinderLTK1Name = cylinderLTK1Name;
        emit cylinderLTK1NameChanged(m_cylinderLTK1Name);
    }

    void setCylinderLTK2Name(QString cylinderLTK2Name)
    {
        if (m_cylinderLTK2Name == cylinderLTK2Name)
            return;

        m_cylinderLTK2Name = cylinderLTK2Name;
        emit cylinderLTK2NameChanged(m_cylinderLTK2Name);
    }

    void setCylinderTrayName(QString cylinderTrayName)
    {
        if (m_cylinderTrayName == cylinderTrayName)
            return;

        m_cylinderTrayName = cylinderTrayName;
        emit cylinderTrayNameChanged(m_cylinderTrayName);
    }

    void setLtkx1PressPos(double ltkx1GetPos)
    {
        if (qFuzzyCompare(m_ltkx1GetPos, ltkx1GetPos))
            return;

        m_ltkx1GetPos = ltkx1GetPos;
        emit ltkx1PressPosChanged(m_ltkx1GetPos);
    }

    void setLtkx1ReleasePos(double ltkx1SetPos)
    {
        if (qFuzzyCompare(m_ltkx1SetPos, ltkx1SetPos))
            return;

        m_ltkx1SetPos = ltkx1SetPos;
        emit ltkx1ReleasePosChanged(m_ltkx1SetPos);
    }

    void setLtlPressPos(double ltlGetPos)
    {
        if (qFuzzyCompare(m_ltlGetPos, ltlGetPos))
            return;

        m_ltlGetPos = ltlGetPos;
        emit ltlPressPosChanged(m_ltlGetPos);
    }

    void setLtlReleasePos(double ltlSetPos)
    {
        if (qFuzzyCompare(m_ltlSetPos, ltlSetPos))
            return;

        m_ltlSetPos = ltlSetPos;
        emit ltlReleasePosChanged(m_ltlSetPos);
    }

    void setLtkx2PressPos(double ltkx2GetPos)
    {
        if (qFuzzyCompare(m_ltkx2GetPos, ltkx2GetPos))
            return;

        m_ltkx2GetPos = ltkx2GetPos;
        emit ltkx2PressPosChanged(m_ltkx2GetPos);
    }

    void setLtkx2ReleasePos(double ltkx2SetPos)
    {
        if (qFuzzyCompare(m_ltkx2SetPos, ltkx2SetPos))
            return;

        m_ltkx2SetPos = ltkx2SetPos;
        emit ltkx2ReleasePosChanged(m_ltkx2SetPos);
    }

    void setLtkx1RelayPos(double ltkx1RelayPos)
    {
        if (qFuzzyCompare(m_ltkx1RelayPos, ltkx1RelayPos))
            return;

        m_ltkx1RelayPos = ltkx1RelayPos;
        emit ltkx1RelayPosChanged(m_ltkx1RelayPos);
    }

    void setClipinInputName(QString clipinInputName)
    {
        if (m_clipinInputName == clipinInputName)
            return;

        m_clipinInputName = clipinInputName;
        emit clipinInputNameChanged(m_clipinInputName);
    }

    void setClipoutInputName(QString clipoutInputName)
    {
        if (m_clipoutInputName == clipoutInputName)
            return;

        m_clipoutInputName = clipoutInputName;
        emit clipoutInputNameChanged(m_clipoutInputName);
    }

    void setLtkx2SafeDistance(double ltkx2SafeDistance)
    {
        if (qFuzzyCompare(m_ltkx2SafeDistance, ltkx2SafeDistance))
            return;

        m_ltkx2SafeDistance = ltkx2SafeDistance;
        emit ltkx2SafeDistanceChanged(m_ltkx2SafeDistance);
    }

    void setLtlWorkPos(double ltlWorkPos)
    {
        if (qFuzzyCompare(m_ltlWorkPos, ltlWorkPos))
            return;

        m_ltlWorkPos = ltlWorkPos;
        emit ltlWorkPosChanged(m_ltlWorkPos);
    }

    void setOpenQinfo(bool openQinfo)
    {
        if (m_openQinfo == openQinfo)
            return;

        m_openQinfo = openQinfo;
        emit openQinfoChanged(m_openQinfo);
    }

    void setTrayInputIoName(QString trayInputIoName)
    {
        if (m_trayInputIoName == trayInputIoName)
            return;

        m_trayInputIoName = trayInputIoName;
        emit trayInputIoNameChanged(m_trayInputIoName);
    }

    void setIsHandly(bool isHandly)
    {
        if (m_isHandly == isHandly)
            return;

        m_isHandly = isHandly;
        emit isHandlyChanged(m_isHandly);
    }

    void setPushVelocity(double pushVelocity)
    {
        if (qFuzzyCompare(m_pushVelocity, pushVelocity))
            return;

        m_pushVelocity = pushVelocity;
        emit pushVelocityChanged(m_pushVelocity);
    }

    void setEntranceTrayCheckIoName(QString entranceTrayCheckIoName)
    {
        if (m_entranceTrayCheckIoName == entranceTrayCheckIoName)
            return;

        m_entranceTrayCheckIoName = entranceTrayCheckIoName;
        emit entranceTrayCheckIoNameChanged(m_entranceTrayCheckIoName);
    }

    void setExitTrayCheckIoName(QString exitTrayCheckIoName)
    {
        if (m_exitTrayCheckIoName == exitTrayCheckIoName)
            return;

        m_exitTrayCheckIoName = exitTrayCheckIoName;
        emit exitTrayCheckIoNameChanged(m_exitTrayCheckIoName);
    }

    void setEntranceClipCheckIoName(QString entranceClipCheckIoName)
    {
        if (m_entranceClipCheckIoName == entranceClipCheckIoName)
            return;

        m_entranceClipCheckIoName = entranceClipCheckIoName;
        emit entranceClipCheckIoNameChanged(m_entranceClipCheckIoName);
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

    void setWorkTrayCheckIoName(QString workTrayCheckIoName)
    {
        if (m_workTrayCheckIoName == workTrayCheckIoName)
            return;

        m_workTrayCheckIoName = workTrayCheckIoName;
        emit workTrayCheckIoNameChanged(m_workTrayCheckIoName);
    }

    void setHandlyChangeLensTray(bool handlyChangeLensTray)
    {
        if (m_handlyChangeLensTray == handlyChangeLensTray)
            return;

        m_handlyChangeLensTray = handlyChangeLensTray;
        emit handlyChangeLensTrayChanged(m_handlyChangeLensTray);
    }

    void setModuleName(QString moduleName)
    {
        if (m_moduleName == moduleName)
            return;

        m_moduleName = moduleName;
        emit moduleNameChanged(m_moduleName);
    }

    void setTcpCylinderClipIOName(QString tcpCylinderClipIOName)
    {
        if (m_tcpCylinderClipIOName == tcpCylinderClipIOName)
            return;

        m_tcpCylinderClipIOName = tcpCylinderClipIOName;
        emit tcpCylinderClipIONameChanged(m_tcpCylinderClipIOName);
    }

    void setTcpCylinderLTKX1IOName(QString tcpCylinderLTKX1IOName)
    {
        if (m_tcpCylinderLTKX1IOName == tcpCylinderLTKX1IOName)
            return;

        m_tcpCylinderLTKX1IOName = tcpCylinderLTKX1IOName;
        emit tcpCylinderLTKX1IONameChanged(m_tcpCylinderLTKX1IOName);
    }

    void setTcpCylinderLTKX2IOName(QString tcpCylinderLTKX2IOName)
    {
        if (m_tcpCylinderLTKX2IOName == tcpCylinderLTKX2IOName)
            return;

        m_tcpCylinderLTKX2IOName = tcpCylinderLTKX2IOName;
        emit tcpCylinderLTKX2IONameChanged(m_tcpCylinderLTKX2IOName);
    }

    void setTcpCylinderLTLIOName(QString tcpCylinderLTLIOName)
    {
        if (m_tcpCylinderLTLIOName == tcpCylinderLTLIOName)
            return;

        m_tcpCylinderLTLIOName = tcpCylinderLTLIOName;
        emit tcpCylinderLTLIONameChanged(m_tcpCylinderLTLIOName);
    }

    void setTcpCylinderClipSensorName(QString tcpCylinderClipSensorName)
    {
        if (m_tcpCylinderClipSensorName == tcpCylinderClipSensorName)
            return;

        m_tcpCylinderClipSensorName = tcpCylinderClipSensorName;
        emit tcpCylinderClipSensorNameChanged(m_tcpCylinderClipSensorName);
    }

    void setTcpCylinderLTKX1SensorName(QString tcpCylinderLTKX1SensorName)
    {
        if (m_tcpCylinderLTKX1SensorName == tcpCylinderLTKX1SensorName)
            return;

        m_tcpCylinderLTKX1SensorName = tcpCylinderLTKX1SensorName;
        emit tcpCylinderLTKX1SensorNameChanged(m_tcpCylinderLTKX1SensorName);
    }

    void setTcpCylinderLTKX2SensorName(QString tcpCylinderLTKX2SensorName)
    {
        if (m_tcpCylinderLTKX2SensorName == tcpCylinderLTKX2SensorName)
            return;

        m_tcpCylinderLTKX2SensorName = tcpCylinderLTKX2SensorName;
        emit tcpCylinderLTKX2SensorNameChanged(m_tcpCylinderLTKX2SensorName);
    }

    void setTcpCylinderLTLSensorName(QString tcpCylinderLTLSensorName)
    {
        if (m_tcpCylinderLTLSensorName == tcpCylinderLTLSensorName)
            return;

        m_tcpCylinderLTLSensorName = tcpCylinderLTLSensorName;
        emit tcpCylinderLTLSensorNameChanged(m_tcpCylinderLTLSensorName);
    }

signals:
    void motorTLIENameChanged(QString motorLTIEName);
    void motorLTOENameChanged(QString motorLTOEName);
    void motorLTKX1NameChanged(QString motorLTKX1Name);
    void motorLTKX2NameChanged(QString motorLTKX2Name);
    void motorLTLNameChanged(QString motorLTLXName);
    void cylinderClipChanged(QString cylinderClipName);
    void cylinderLTK1NameChanged(QString cylinderLTK1Name);
    void cylinderLTK2NameChanged(QString cylinderLTK2Name);
    void cylinderTrayNameChanged(QString cylinderTrayName);
    void ltkx1PressPosChanged(double ltkx1PressPos);
    void ltkx1ReleasePosChanged(double ltkx1ReleasePos);
    void ltlPressPosChanged(double ltlPressPos);
    void ltlReleasePosChanged(double ltlReleasePos);
    void ltkx2PressPosChanged(double ltkx2PressPos);
    void ltkx2ReleasePosChanged(double ltkx2ReleasePos);
    void ltkx1RelayPosChanged(double ltkx1RelayPos);
    void clipinInputNameChanged(QString clipinInputName);
    void clipoutInputNameChanged(QString clipoutInputName);
    void ltkx2SafeDistanceChanged(double ltkx2SafeDistance);
    void ltlWorkPosChanged(double ltlWorkPos);
    void openQinfoChanged(bool openQinfo);
    void trayInputIoNameChanged(QString trayInputIoName);
    void isHandlyChanged(bool isHandly);
    void pushVelocityChanged(double pushVelocity);
    void entranceTrayCheckIoNameChanged(QString entranceTrayCheckIoName);
    void exitTrayCheckIoNameChanged(QString exitTrayCheckIoName);
    void entranceClipCheckIoNameChanged(QString entranceClipCheckIoName);
    void exitClipCheckIoNameChanged(QString exitClipCheckIoName);
    void readyTrayCheckIoNameChanged(QString readyTrayCheckIoName);
    void workTrayCheckIoNameChanged(QString workTrayCheckIoName);
    void handlyChangeLensTrayChanged(bool handlyChangeLensTray);
    void moduleNameChanged(QString moduleName);
    void tcpCylinderClipIONameChanged(QString tcpCylinderClipIOName);
    void tcpCylinderLTKX1IONameChanged(QString tcpCylinderLTKX1IOName);
    void tcpCylinderLTKX2IONameChanged(QString tcpCylinderLTKX2IOName);
    void tcpCylinderLTLIONameChanged(QString tcpCylinderLTLIOName);
    void tcpCylinderClipSensorNameChanged(QString tcpCylinderClipSensorName);
    void tcpCylinderLTKX1SensorNameChanged(QString tcpCylinderLTKX1SensorName);
    void tcpCylinderLTKX2SensorNameChanged(QString tcpCylinderLTKX2SensorName);
    void tcpCylinderLTLSensorNameChanged(QString tcpCylinderLTLSensorName);
};

class TrayLoaderState : public PropertyBase{
    Q_OBJECT
    int m_runMode=0;
    bool m_hasTrayReady=false;
    bool m_hasTrayUsed=false;
    bool m_hasPrevTrayEjected=false;
    bool m_hasEntranceTrayPulledAway=true;
    bool m_hasTrayReadyPosClear=false;
    bool m_hasLTKX1ReadytoPull = false;

    bool m_hasTrayOutHandOverReady=false;

    bool m_isExchangeTray = false;
    bool m_hasEntranceClipEmpty=false;
    bool m_hasExitClipFull=false;

    bool m_allowChangeTray = false;

    bool m_hasReadyTray = false;

    bool m_hasWorkTray = false;

    bool m_hasPulledTray = false;

    bool m_readyToPushEmptyTray = false;

    bool m_entanceClipReady = false;

    bool m_exitClipReady = false;

    bool m_readyToPushReadyTray = false;

    bool m_isFirstTray = true;

    bool m_isReadyTrayPushed = false;

    bool m_handlyChangeLens = false;

    bool m_handlyChangeLensTray = false;

    bool m_allowReadyTray = false;

    bool m_tcpCylinderClipSensorName;

    bool m_tcpCylinderLTK1SensorName;

bool m_tcpCylinderLTK2SensorName;

bool m_tcpCylinderLTLSensorName;

bool m_tcpCylinderClipSensorState;

bool m_tcpCylinderLTK1SensorState;

bool m_tcpCylinderLTK2SensorState;

bool m_tcpCylinderLTLSensorState;

public:
    TrayLoaderState():PropertyBase(){}
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(bool handlyChangeLens READ handlyChangeLens WRITE setHandlyChangeLens NOTIFY handlyChangeLensChanged)
    Q_PROPERTY(bool hasTrayReady READ hasTrayReady WRITE setHasTrayReady NOTIFY hasTrayReadyChanged)
    Q_PROPERTY(bool handlyChangeLensTray READ handlyChangeLensTray WRITE setHandlyChangeLensTray NOTIFY handlyChangeLensTrayChanged)

    Q_PROPERTY(bool hasTrayUsed READ hasTrayUsed WRITE setHasTrayUsed NOTIFY hasTrayUsedChanged)
    Q_PROPERTY(bool hasPrevTrayEjected READ hasPrevTrayEjected WRITE setHasPrevTrayEjected NOTIFY hasPrevTrayEjectedChanged)
    Q_PROPERTY(bool hasEntranceTrayPulledAway READ hasEntranceTrayPulledAway WRITE setHasEntranceTrayPulledAway NOTIFY hasEntranceTrayPulledAwayChanged)
    Q_PROPERTY(bool hasTrayReadyPosClear READ hasTrayReadyPosClear WRITE setHasTrayReadyPosClear NOTIFY hasTrayReadyPosClearChanged)
    Q_PROPERTY(bool hasLTKX1ReadytoPull READ hasLTKX1ReadytoPull WRITE setHasLTKX1ReadytoPull NOTIFY hasLTKX1ReadytoPullChanged)
    Q_PROPERTY(bool hasTrayOutHandOverReady READ hasTrayOutHandOverReady WRITE setHasTrayOutHandOverReady NOTIFY hasTrayOutHandOverReadyChanged)
    Q_PROPERTY(bool isExchangeTray READ isExchangeTray WRITE setIsExchangeTray NOTIFY isExchangeTrayChanged)

    Q_PROPERTY(bool hasEntranceClipEmpty READ hasEntranceClipEmpty WRITE setHasEntranceClipEmpty NOTIFY hasEntranceClipEmptyChanged)
    Q_PROPERTY(bool hasExitClipFull READ hasExitClipFull WRITE setHasExitClipFull NOTIFY hasExitClipFullChanged)
    Q_PROPERTY(bool allowChangeTray READ allowChangeTray WRITE setAllowChangeTray NOTIFY allowChangeTrayChanged)
    Q_PROPERTY(bool allowReadyTray READ allowReadyTray WRITE setallowReadyTray NOTIFY allowReadyTrayChanged)
    Q_PROPERTY(bool hasReadyTray READ hasReadyTray WRITE setHasReadyTray NOTIFY hasReadyTrayChanged)
    Q_PROPERTY(bool hasWorkTray READ hasWorkTray WRITE setHasWorkTray NOTIFY hasWorkTrayChanged)
    Q_PROPERTY(bool hasPulledTray READ hasPulledTray WRITE setHasPulledTray NOTIFY hasPulledTrayChanged)
    Q_PROPERTY(bool readyToPushReadyTray READ readyToPushReadyTray WRITE setReadyToPusReadyTray NOTIFY readyToPushReadyTrayChanged)
    Q_PROPERTY(bool readyToPushEmptyTray READ readyToPushEmptyTray WRITE setReadyToPushEmptyTray NOTIFY readyToPushEmptyTrayChanged)
    Q_PROPERTY(bool entanceClipReady READ entanceClipReady WRITE setEntanceClipReady NOTIFY entanceClipReadyChanged)
    Q_PROPERTY(bool exitClipReady READ exitClipReady WRITE setExitClipReady NOTIFY exitClipReadyChanged)
    Q_PROPERTY(bool isFirstTray READ isFirstTray WRITE setIsFirstTray NOTIFY isFirstTrayChanged)
    Q_PROPERTY(bool isReadyTrayPushed READ isReadyTrayPushed WRITE setIsReadyTrayPushed NOTIFY isReadyTrayPushedChanged)
    Q_PROPERTY(bool tcpCylinderClipSensorState READ tcpCylinderClipSensorState WRITE setTcpCylinderClipSensorState NOTIFY tcpCylinderClipSensorStateChanged)
    Q_PROPERTY(bool tcpCylinderLTK1SensorState READ tcpCylinderLTK1SensorState WRITE setTcpCylinderLTK1SensorState NOTIFY tcpCylinderLTK1SensorStateChanged)
    Q_PROPERTY(bool tcpCylinderLTK2SensorState READ tcpCylinderLTK2SensorState WRITE setTcpCylinderLTK2SensorState NOTIFY tcpCylinderLTK2SensorStateChanged)
    Q_PROPERTY(bool tcpCylinderLTLSensorState READ tcpCylinderLTLSensorState WRITE setTcpCylinderLTLSensorState NOTIFY tcpCylinderLTLSensorStateChanged)

    int runMode() const
    {
        return m_runMode;
    }
    bool hasTrayReady() const
    {
        return m_hasTrayReady;
    }
    bool hasTrayUsed() const
    {
        return m_hasTrayUsed;
    }

    bool hasPrevTrayEjected() const
    {
        return m_hasPrevTrayEjected;
    }

    bool hasEntranceTrayPulledAway() const
    {
        return m_hasEntranceTrayPulledAway;
    }

    bool hasTrayReadyPosClear() const
    {
        return m_hasTrayReadyPosClear;
    }

    bool hasLTKX1ReadytoPull() const
    {
        return m_hasLTKX1ReadytoPull;
    }

    bool hasTrayOutHandOverReady() const
    {
        return m_hasTrayOutHandOverReady;
    }

    bool isExchangeTray() const
    {
        return m_isExchangeTray;
    }

    bool hasEntranceClipEmpty() const
    {
        return m_hasEntranceClipEmpty;
    }

    bool hasExitClipFull() const
    {
        return m_hasExitClipFull;
    }

    bool allowChangeTray() const
    {
        return m_allowChangeTray;
    }

    bool hasReadyTray() const
    {
        return m_hasReadyTray;
    }

    bool hasWorkTray() const
    {
        return m_hasWorkTray;
    }

    bool hasPulledTray() const
    {
        return m_hasPulledTray;
    }

    bool readyToPushEmptyTray() const
    {
        return m_readyToPushEmptyTray;
    }

    bool entanceClipReady() const
    {
        return m_entanceClipReady;
    }

    bool exitClipReady() const
    {
        return m_exitClipReady;
    }

    bool readyToPushReadyTray() const
    {
        return m_readyToPushReadyTray;
    }

    bool isFirstTray() const
    {
        return m_isFirstTray;
    }

    bool isReadyTrayPushed() const
    {
        return m_isReadyTrayPushed;
    }

    bool handlyChangeLens() const
    {
        return m_handlyChangeLens;
    }

    bool handlyChangeLensTray() const
    {
        return m_handlyChangeLensTray;
    }

    bool allowReadyTray() const
    {
        return m_allowReadyTray;
    }

    bool tcpCylinderClipSensorName() const
    {
        return m_tcpCylinderClipSensorName;
    }

    bool tcpCylinderLTK1SensorName() const
    {
        return m_tcpCylinderLTK1SensorName;
    }

    bool tcpCylinderLTK2SensorName() const
    {
        return m_tcpCylinderLTK2SensorName;
    }

    bool tcpCylinderLTLSensorName() const
    {
        return m_tcpCylinderLTLSensorName;
    }

    bool tcpCylinderClipSensorState() const
    {
        return m_tcpCylinderClipSensorState;
    }

    bool tcpCylinderLTK1SensorState() const
    {
        return m_tcpCylinderLTK1SensorState;
    }

    bool tcpCylinderLTK2SensorState() const
    {
        return m_tcpCylinderLTK2SensorState;
    }

    bool tcpCylinderLTLSensorState() const
    {
        return m_tcpCylinderLTLSensorState;
    }

public slots:
    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }
    void setHasTrayReady(bool hasTrayReady)
    {
        if (m_hasTrayReady == hasTrayReady)
            return;

        m_hasTrayReady = hasTrayReady;
        emit hasTrayReadyChanged(m_hasTrayReady);
    }

    void setHasTrayUsed(bool hasTrayUsed)
    {
        if (m_hasTrayUsed == hasTrayUsed)
            return;

        m_hasTrayUsed = hasTrayUsed;
        emit hasTrayUsedChanged(m_hasTrayUsed);
    }

    void setHasPrevTrayEjected(bool hasPrevTrayEjected)
    {
        if (m_hasPrevTrayEjected == hasPrevTrayEjected)
            return;

        m_hasPrevTrayEjected = hasPrevTrayEjected;
        emit hasPrevTrayEjectedChanged(m_hasPrevTrayEjected);
    }

    void setHasEntranceTrayPulledAway(bool hasEntranceTrayPulledAway)
    {
        if (m_hasEntranceTrayPulledAway == hasEntranceTrayPulledAway)
            return;

        m_hasEntranceTrayPulledAway = hasEntranceTrayPulledAway;
        emit hasEntranceTrayPulledAwayChanged(m_hasEntranceTrayPulledAway);
    }

    void setHasTrayReadyPosClear(bool hasTrayReadyPosClear)
    {
        if (m_hasTrayReadyPosClear == hasTrayReadyPosClear)
            return;

        m_hasTrayReadyPosClear = hasTrayReadyPosClear;
        emit hasTrayReadyPosClearChanged(m_hasTrayReadyPosClear);
    }

    void setHasLTKX1ReadytoPull(bool hasLTKX1ReadytoPull)
    {
        if (m_hasLTKX1ReadytoPull == hasLTKX1ReadytoPull)
            return;

        m_hasLTKX1ReadytoPull = hasLTKX1ReadytoPull;
        emit hasLTKX1ReadytoPullChanged(m_hasLTKX1ReadytoPull);
    }

    void setHasTrayOutHandOverReady(bool hasTrayOutHandOverReady)
    {
        if (m_hasTrayOutHandOverReady == hasTrayOutHandOverReady)
            return;

        m_hasTrayOutHandOverReady = hasTrayOutHandOverReady;
        emit hasTrayOutHandOverReadyChanged(m_hasTrayOutHandOverReady);
    }

    void setIsExchangeTray(bool isExchangeTray)
    {
        if (m_isExchangeTray == isExchangeTray)
            return;

        m_isExchangeTray = isExchangeTray;
        emit isExchangeTrayChanged(m_isExchangeTray);
    }

    void setHasEntranceClipEmpty(bool hasEntranceClipEmpty)
    {
        if (m_hasEntranceClipEmpty == hasEntranceClipEmpty)
            return;

        m_hasEntranceClipEmpty = hasEntranceClipEmpty;
        emit hasEntranceClipEmptyChanged(m_hasEntranceClipEmpty);
    }

    void setHasExitClipFull(bool hasExitClipFull)
    {
        if (m_hasExitClipFull == hasExitClipFull)
            return;

        m_hasExitClipFull = hasExitClipFull;
        emit hasExitClipFullChanged(m_hasExitClipFull);
    }

    void setAllowChangeTray(bool allowChangeTray)
    {
        if (m_allowChangeTray == allowChangeTray)
            return;

        m_allowChangeTray = allowChangeTray;
        emit allowChangeTrayChanged(m_allowChangeTray);
    }

    void setHasReadyTray(bool hasReadyTray)
    {
        if (m_hasReadyTray == hasReadyTray)
            return;

        m_hasReadyTray = hasReadyTray;
        emit hasReadyTrayChanged(m_hasReadyTray);
    }

    void setHasWorkTray(bool hasWorkTray)
    {
        if (m_hasWorkTray == hasWorkTray)
            return;

        m_hasWorkTray = hasWorkTray;
        emit hasWorkTrayChanged(m_hasWorkTray);
    }

    void setHasPulledTray(bool hasPulledTray)
    {
        if (m_hasPulledTray == hasPulledTray)
            return;

        m_hasPulledTray = hasPulledTray;
        emit hasPulledTrayChanged(m_hasPulledTray);
    }

    void setReadyToPushEmptyTray(bool readyToPushEmptyTray)
    {
        if (m_readyToPushEmptyTray == readyToPushEmptyTray)
            return;

        m_readyToPushEmptyTray = readyToPushEmptyTray;
        emit readyToPushEmptyTrayChanged(m_readyToPushEmptyTray);
    }

    void setEntanceClipReady(bool entanceClipReady)
    {
        if (m_entanceClipReady == entanceClipReady)
            return;

        m_entanceClipReady = entanceClipReady;
        emit entanceClipReadyChanged(m_entanceClipReady);
    }

    void setExitClipReady(bool exitClipReady)
    {
        if (m_exitClipReady == exitClipReady)
            return;

        m_exitClipReady = exitClipReady;
        emit exitClipReadyChanged(m_exitClipReady);
    }

    void setReadyToPusReadyTray(bool readyToPushReadyTray)
    {
        if (m_readyToPushReadyTray == readyToPushReadyTray)
            return;

        m_readyToPushReadyTray = readyToPushReadyTray;
        emit readyToPushReadyTrayChanged(m_readyToPushReadyTray);
    }

    void setIsFirstTray(bool isFirstTray)
    {
        if (m_isFirstTray == isFirstTray)
            return;

        m_isFirstTray = isFirstTray;
        emit isFirstTrayChanged(m_isFirstTray);
    }

    void setIsReadyTrayPushed(bool isReadyTrayPushed)
    {
        if (m_isReadyTrayPushed == isReadyTrayPushed)
            return;

        m_isReadyTrayPushed = isReadyTrayPushed;
        emit isReadyTrayPushedChanged(m_isReadyTrayPushed);
    }

    void setHandlyChangeLens(bool handlyChangeLens)
    {
        if (m_handlyChangeLens == handlyChangeLens)
            return;

        m_handlyChangeLens = handlyChangeLens;
        emit handlyChangeLensChanged(m_handlyChangeLens);
    }

    void setHandlyChangeLensTray(bool handlyChangeLensTray)
    {
        if (m_handlyChangeLensTray == handlyChangeLensTray)
            return;

        m_handlyChangeLensTray = handlyChangeLensTray;
        emit handlyChangeLensTrayChanged(m_handlyChangeLensTray);
    }

    void setallowReadyTray(bool allowReadyTray)
    {
        if (m_allowReadyTray == allowReadyTray)
            return;

        m_allowReadyTray = allowReadyTray;
        emit allowReadyTrayChanged(m_allowReadyTray);
    }

    void setTcpCylinderClipSensorName(bool tcpCylinderClipSensorName)
    {
        if (m_tcpCylinderClipSensorName == tcpCylinderClipSensorName)
            return;

        m_tcpCylinderClipSensorName = tcpCylinderClipSensorName;
        emit tcpCylinderClipSensorNameChanged(m_tcpCylinderClipSensorName);
    }

    void setTcpCylinderLTK1SensorName(bool tcpCylinderLTK1SensorName)
    {
        if (m_tcpCylinderLTK1SensorName == tcpCylinderLTK1SensorName)
            return;

        m_tcpCylinderLTK1SensorName = tcpCylinderLTK1SensorName;
        emit tcpCylinderLTK1SensorNameChanged(m_tcpCylinderLTK1SensorName);
    }

    void setTcpCylinderLTK2SensorName(bool tcpCylinderLTK2SensorName)
    {
        if (m_tcpCylinderLTK2SensorName == tcpCylinderLTK2SensorName)
            return;

        m_tcpCylinderLTK2SensorName = tcpCylinderLTK2SensorName;
        emit tcpCylinderLTK2SensorNameChanged(m_tcpCylinderLTK2SensorName);
    }

    void setTcpCylinderLTLSensorName(bool tcpCylinderLTLSensorName)
    {
        if (m_tcpCylinderLTLSensorName == tcpCylinderLTLSensorName)
            return;

        m_tcpCylinderLTLSensorName = tcpCylinderLTLSensorName;
        emit tcpCylinderLTLSensorNameChanged(m_tcpCylinderLTLSensorName);
    }

    void setTcpCylinderClipSensorState(bool tcpCylinderClipSensorState)
    {
        if (m_tcpCylinderClipSensorState == tcpCylinderClipSensorState)
            return;

        m_tcpCylinderClipSensorState = tcpCylinderClipSensorState;
        emit tcpCylinderClipSensorStateChanged(m_tcpCylinderClipSensorState);
    }

    void setTcpCylinderLTK1SensorState(bool tcpCylinderLTK1SensorState)
    {
        if (m_tcpCylinderLTK1SensorState == tcpCylinderLTK1SensorState)
            return;

        m_tcpCylinderLTK1SensorState = tcpCylinderLTK1SensorState;
        emit tcpCylinderLTK1SensorStateChanged(m_tcpCylinderLTK1SensorState);
    }

    void setTcpCylinderLTK2SensorState(bool tcpCylinderLTK2SensorState)
    {
        if (m_tcpCylinderLTK2SensorState == tcpCylinderLTK2SensorState)
            return;

        m_tcpCylinderLTK2SensorState = tcpCylinderLTK2SensorState;
        emit tcpCylinderLTK2SensorStateChanged(m_tcpCylinderLTK2SensorState);
    }

    void setTcpCylinderLTLSensorState(bool tcpCylinderLTLSensorState)
    {
        if (m_tcpCylinderLTLSensorState == tcpCylinderLTLSensorState)
            return;

        m_tcpCylinderLTLSensorState = tcpCylinderLTLSensorState;
        emit tcpCylinderLTLSensorStateChanged(m_tcpCylinderLTLSensorState);
    }

signals:
    void runModeChanged(int runMode);
    void hasTrayOnEntryChanged(bool hasTrayOnEntry);
    void hasTrayReadyChanged(bool hasTrayReady);
    void hasTrayUsedChanged(bool hasTrayUsed);
    void hasPrevTrayEjectedChanged(bool hasPrevTrayEjected);
    void hasEntranceTrayPulledAwayChanged(bool hasEntranceTrayPulledAway);
    void hasTrayReadyPosClearChanged(bool hasTrayReadyPosClear);
    void hasLTKX1ReadytoPullChanged(bool hasLTKX1ReadytoPull);
    void hasTrayOutHandOverReadyChanged(bool hasTrayOutHandOverReady);
    void isExchangeTrayChanged(bool isExchangeTray);
    void hasEntranceClipEmptyChanged(bool hasEntranceClipEmpty);
    void hasExitClipFullChanged(bool hasExitClipFull);
    void allowChangeTrayChanged(bool allowChangeTray);
    void hasReadyTrayChanged(bool hasReadyTray);
    void hasWorkTrayChanged(bool hasWorkTray);
    void hasPulledTrayChanged(bool hasPulledTray);
    void readyToPushEmptyTrayChanged(bool readyToPushEmptyTray);
    void entanceClipReadyChanged(bool entanceClipReady);
    void exitClipReadyChanged(bool exitClipReady);
    void readyToPushReadyTrayChanged(bool readyToPushReadyTray);
    void isFirstTrayChanged(bool isFirstTray);
    void isReadyTrayPushedChanged(bool isReadyTrayPushed);
    void handlyChangeLensChanged(bool handlyChangeLens);
    void handlyChangeLensTrayChanged(bool handlyChangeLensTray);
    void allowReadyTrayChanged(bool allowReadyTray);
    void tcpCylinderClipSensorNameChanged(bool tcpCylinderClipSensorName);
    void tcpCylinderLTK1SensorNameChanged(bool tcpCylinderLTK1SensorName);
    void tcpCylinderLTK2SensorNameChanged(bool tcpCylinderLTK2SensorName);
    void tcpCylinderLTLSensorNameChanged(bool tcpCylinderLTLSensorName);
    void tcpCylinderClipSensorStateChanged(bool tcpCylinderClipSensorState);
    void tcpCylinderLTK1SensorStateChanged(bool tcpCylinderLTK1SensorState);
    void tcpCylinderLTK2SensorStateChanged(bool tcpCylinderLTK2SensorState);
    void tcpCylinderLTLSensorStateChanged(bool tcpCylinderLTLSensorState);
};

#endif // TRAYLOADERPARAMETER_H
