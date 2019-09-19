#ifndef WORKERMANAGERPARAMETER_H
#define WORKERMANAGERPARAMETER_H

#include "propertybase.h"



class WorkersManagerParameter:public PropertyBase
{
    Q_OBJECT
public:
    WorkersManagerParameter():PropertyBase(){}
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(QString stationName READ stationName WRITE setStationName NOTIFY stationNameChanged)
    Q_PROPERTY(QVariantList respMessagerNames READ respMessagerNames WRITE setRespMessagerNames NOTIFY respMessagerNamesChanged)
    Q_PROPERTY(QVariantList cmsMessagerNames READ cmsMessagerNames WRITE setCmsMessagerNames NOTIFY cmsMessagerNamesChanged)
    Q_PROPERTY(bool showAlarm READ showAlarm WRITE setShowAlarm NOTIFY showAlarmChanged)
    int runMode() const
    {
        return m_runMode;
    }
    QString stationName() const
    {
        return m_stationName;
    }

    QVariantList respMessagerNames() const
    {
        return m_respMessagerNames;
    }

    QVariantList cmsMessagerNames() const
    {
        return m_cmsMessageerNames;
    }

    bool showAlarm() const
    {
        return m_showAlarm;
    }

public slots:
    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }
    void setStationName(QString stationName)
    {
        if (m_stationName == stationName)
            return;

        m_stationName = stationName;
        emit stationNameChanged(m_stationName);
    }

    void setRespMessagerNames(QVariantList respMessagerNames)
    {
        if (m_respMessagerNames == respMessagerNames)
            return;

        m_respMessagerNames = respMessagerNames;
        emit respMessagerNamesChanged(m_respMessagerNames);
    }

    void setCmsMessagerNames(QVariantList cmsMessageerNames)
    {
        if (m_cmsMessageerNames == cmsMessageerNames)
            return;

        m_cmsMessageerNames = cmsMessageerNames;
        emit cmsMessagerNamesChanged(m_cmsMessageerNames);
    }

    void setShowAlarm(bool showAlarm)
    {
        if (m_showAlarm == showAlarm)
            return;

        m_showAlarm = showAlarm;
        emit showAlarmChanged(m_showAlarm);
    }

signals:
    void runModeChanged(int runMode);
    void stationNameChanged(QString stationName);

    void respMessagerNamesChanged(QVariantList respMessagerNames);

    void cmsMessagerNamesChanged(QVariantList cmsMessagerNames);

    void showAlarmChanged(bool showAlarm);

private:
    int m_runMode = 0;
    QString m_stationName = "LeftStation";
    QVariantList m_respMessagerNames;
    QVariantList m_cmsMessageerNames;
    bool m_showAlarm = false;
};

#endif // WORKERMANAGERPARAMETER_H
