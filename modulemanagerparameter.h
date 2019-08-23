#ifndef BASEMODULEMANAGERPARAMETER_H
#define BASEMODULEMANAGERPARAMETER_H

#include "propertybase.h"

#include <QVariantList>
class ModuleManangerConfig:public PropertyBase
{
    Q_OBJECT
    Q_PROPERTY(int serverMode READ serverMode WRITE setServerMode NOTIFY serverModeChanged)
    Q_PROPERTY(QString dataServerURL READ dataServerURL WRITE setDataServerURL NOTIFY dataServerURLChanged)
public:
    int serverMode() const
    {
        return m_serverMode;
    }

    QString dataServerURL() const
    {
        return m_dataServerURL;
    }

public slots:
    void setServerMode(int serverMode)
    {
        if (m_serverMode == serverMode)
            return;

        m_serverMode = serverMode;
        emit serverModeChanged(m_serverMode);
    }

    void setDataServerURL(QString dataServerURL)
    {
        if (m_dataServerURL == dataServerURL)
            return;

        m_dataServerURL = dataServerURL;
        emit dataServerURLChanged(m_dataServerURL);
    }

signals:
    void serverModeChanged(int serverMode);

    void dataServerURLChanged(QString dataServerURL);

private:
    int m_serverMode = 0;
    QString m_dataServerURL;
};


class ModuleManagerParameter:public PropertyBase
{
    Q_OBJECT
public:
    ModuleManagerParameter():PropertyBase (){}
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(bool handlyChangeLens READ handlyChangeLens WRITE setHandlyChangeLens NOTIFY handlyChangeLensChanged)
    Q_PROPERTY(bool handlyChangeSensor READ handlyChangeSensor WRITE setHandlyChangeSensor NOTIFY handlyChangeSensorChanged)
    Q_PROPERTY(bool handlyChangeLensTray READ handlyChangeLensTray WRITE setHandlyChangeLensTray NOTIFY handlyChangeLensTrayChanged)
    Q_PROPERTY(bool handlyChangeSensorTray READ handlyChangeSensorTray WRITE setHandlyChangeSensorTray NOTIFY handlyChangeSensorTrayChanged)
    Q_PROPERTY(QString materialType READ materialType WRITE setMaterialType NOTIFY materialTypeChanged)
    Q_PROPERTY(QString machineType READ machineType WRITE setMachineType NOTIFY machineTypeChanged)
    Q_PROPERTY(QVariantList respMessagerNames READ respMessagerNames WRITE setRespMessagerNames NOTIFY respMessagerNamesChanged)
    Q_PROPERTY(QVariantList cmsMessageerNames READ cmsMessageerNames WRITE setCmsMessageerNames NOTIFY cmsMessageerNamesChanged)
    Q_PROPERTY(QString flowChartURL READ flowChartURL WRITE setFlowChartURL NOTIFY flowChartURLChanged)
    Q_PROPERTY(bool disableStation READ disableStation WRITE setDisableStation NOTIFY disableStationChanged)
    int runMode() const
    {
        return m_runMode;
    }
    QString materialType() const
    {
        return m_materialType;
    }

    QString machineType() const
    {
        return m_machineType;
    }

    QVariantList respMessagerNames() const
    {
        return m_respMessagerNames;
    }

    QString flowChartURL() const
    {
        return m_flowChartURL;
    }

    QVariantList cmsMessageerNames() const
    {
        return m_cmsMessageerNames;
    }

    bool disableStation() const
    {
        return m_disableStation;
    }

    bool handlyChangeLens() const
    {
        return m_handlyChangeLens;
    }

    bool handlyChangeSensor() const
    {
        return m_handlyChangeSensor;
    }

    bool handlyChangeLensTray() const
    {
        return m_handlyChangeLensTray;
    }

    bool handlyChangeSensorTray() const
    {
        return m_handlyChangeSensorTray;
    }

public slots:
    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }
    void setMaterialType(QString materialType)
    {
        if (m_materialType == materialType)
            return;

        m_materialType = materialType;
        emit materialTypeChanged(m_materialType);
    }

    void setMachineType(QString machineType)
    {
        if (m_machineType == machineType)
            return;

        m_machineType = machineType;
        emit machineTypeChanged(m_machineType);
    }

    void setRespMessagerNames(QVariantList respMessagerNames)
    {
        if (m_respMessagerNames == respMessagerNames)
            return;

        m_respMessagerNames = respMessagerNames;
        emit respMessagerNamesChanged(m_respMessagerNames);
    }

    void setFlowChartURL(QString flowChartURL)
    {
        if (m_flowChartURL == flowChartURL)
            return;

        m_flowChartURL = flowChartURL;
        emit flowChartURLChanged(m_flowChartURL);
    }

    void setCmsMessageerNames(QVariantList cmsMessageerNames)
    {
        if (m_cmsMessageerNames == cmsMessageerNames)
            return;

        m_cmsMessageerNames = cmsMessageerNames;
        emit cmsMessageerNamesChanged(m_cmsMessageerNames);
    }

    void setDisableStation(bool disableStation)
    {
        if (m_disableStation == disableStation)
            return;

        m_disableStation = disableStation;
        emit disableStationChanged(m_disableStation);
    }

    void setHandlyChangeLens(bool handlyChangeLens)
    {
        if (m_handlyChangeLens == handlyChangeLens)
            return;

        m_handlyChangeLens = handlyChangeLens;
        emit handlyChangeLensChanged(m_handlyChangeLens);
    }

    void setHandlyChangeSensor(bool handlyChangeSensor)
    {
        if (m_handlyChangeSensor == handlyChangeSensor)
            return;

        m_handlyChangeSensor = handlyChangeSensor;
        emit handlyChangeSensorChanged(m_handlyChangeSensor);
    }

    void setHandlyChangeLensTray(bool handlyChangeLensTray)
    {
        if (m_handlyChangeLensTray == handlyChangeLensTray)
            return;

        m_handlyChangeLensTray = handlyChangeLensTray;
        emit handlyChangeLensTrayChanged(m_handlyChangeLensTray);
    }

    void setHandlyChangeSensorTray(bool handlyChangeSensorTray)
    {
        if (m_handlyChangeSensorTray == handlyChangeSensorTray)
            return;

        m_handlyChangeSensorTray = handlyChangeSensorTray;
        emit handlyChangeSensorTrayChanged(m_handlyChangeSensorTray);
    }

signals:
    void runModeChanged(int runMode);
    void materialTypeChanged(QString materialType);

    void machineTypeChanged(QString machineType);

    void respMessagerNamesChanged(QVariantList respMessagerNames);

    void flowChartURLChanged(QString flowChartURL);

    void cmsMessageerNamesChanged(QVariantList cmsMessageerNames);

    void disableStationChanged(bool disableStation);

    void handlyChangeLensChanged(bool handlyChangeLens);

    void handlyChangeSensorChanged(bool handlyChangeSensor);

    void handlyChangeLensTrayChanged(bool handlyChangeLensTray);

    void handlyChangeSensorTrayChanged(bool handlyChangeSensorTray);

private:
    int m_runMode = 0;
    QString m_materialType = "TESTTYPE";
    QString m_machineType = "DAA_001";
    QVariantList m_respMessagerNames;
    QString m_flowChartURL = "http://192.168.0.251//flowchart/flowchart.html";
    QVariantList m_cmsMessageerNames;
    bool m_disableStation = false;
    bool m_handlyChangeLens = false;
    bool m_handlyChangeSensor = false;
    bool m_handlyChangeLensTray = false;
    bool m_handlyChangeSensorTray = false;
};
class ModuleManagerState:public PropertyBase
{
    Q_OBJECT
public:
    ModuleManagerState():PropertyBase(){}
    Q_PROPERTY(bool seekedOrigin READ seekedOrigin WRITE setSeekedOrigin NOTIFY seekedOriginChanged)
    Q_PROPERTY(bool isAutoRun READ isAutoRun WRITE setIsAutoRun NOTIFY isAutoRunChanged)
    Q_PROPERTY(bool isInit READ isInit WRITE setIsInit NOTIFY isInitChanged)
public:
    bool seekedOrigin() const
    {
        return m_seekedOrigin;
    }

    bool isAutoRun() const
    {
        return m_IsAutoRun;
    }

    bool isInit() const
    {
        return m_isInit;
    }

public slots:
    void setSeekedOrigin(bool seekedOrigin)
    {
        if (m_seekedOrigin == seekedOrigin)
            return;

        m_seekedOrigin = seekedOrigin;
        emit seekedOriginChanged(m_seekedOrigin);
    }

    void clearSeekedOrigin()
    {
        if (!m_seekedOrigin)
            return;
        m_seekedOrigin = false;
    }

    void setIsAutoRun(bool IsAutoRun)
    {
        if (m_IsAutoRun == IsAutoRun)
            return;

        m_IsAutoRun = IsAutoRun;
        emit isAutoRunChanged(m_IsAutoRun);
    }

    void setIsInit(bool isInit)
    {
        if (m_isInit == isInit)
            return;

        m_isInit = isInit;
        emit isInitChanged(m_isInit);
    }

signals:
    void seekedOriginChanged(bool seekedOrigin);

    void isAutoRunChanged(bool isAutoRun);

    void isInitChanged(bool isInit);

private:
    bool m_seekedOrigin = false;
    bool m_IsAutoRun = false;
    bool m_isInit = false;
};

#endif // BASEMODULEMANAGERPARAMETER_H
