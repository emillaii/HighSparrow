#ifndef LOGICMANAGERPARAMETER_H
#define LOGICMANAGERPARAMETER_H

#include "propertybase.h"



class LogicManagerParameter:public PropertyBase
{
    Q_OBJECT
public:
    LogicManagerParameter():PropertyBase(){}
    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)

    QString moduleName() const
    {
        return m_moduleName;
    }
public slots:
    void setModuleName(QString moduleName)
    {
        if (m_moduleName == moduleName)
            return;

        m_moduleName = moduleName;
        emit moduleNameChanged(m_moduleName);
    }
signals:
    void moduleNameChanged(QString moduleName);
private:
    QString m_moduleName = "LogicManager1";
};
class LogicManagerState: public PropertyBase
{
    Q_OBJECT
public:
    LogicManagerState():PropertyBase() {}
    Q_PROPERTY(bool isHandling READ isHandling WRITE setIsHandling NOTIFY isHandlingChanged)
    Q_PROPERTY(QString handlingMessage READ handlingMessage WRITE setHandlingMessage NOTIFY handlingMessageChanged)
    Q_PROPERTY(QString currentLocationName READ currentLocationName WRITE setCurrentLocationName NOTIFY currentLocationNameChanged)
    Q_PROPERTY(bool useOriginPr READ useOriginPr WRITE setUseOriginPr NOTIFY useOriginPrChanged)
    Q_PROPERTY(QString currentCalibrationName READ currentCalibrationName WRITE setCurrentCalibrationName NOTIFY currentCalibrationNameChanged)
    Q_PROPERTY(QString currentChannelName READ currentChannelName WRITE setCurrentChannelName NOTIFY currentChannelNameChanged)
    bool isHandling() const
    {
        return m_isHandling;
    }
    QString handlingMessage() const
    {
        return m_handlingMessage;
    }

    QString currentLocationName() const
    {
        return m_currentLocationName;
    }

    bool useOriginPr() const
    {
        return m_useOriginPr;
    }

    QString currentCalibrationName() const
    {
        return m_currentCalibrationName;
    }

    QString currentChannelName() const
    {
        return m_currentChannelName;
    }

public slots:
    void setIsHandling(bool isHandling)
    {
        if (m_isHandling == isHandling)
            return;

        m_isHandling = isHandling;
        emit isHandlingChanged(m_isHandling);
    }
    void setHandlingMessage(QString handlingMessage)
    {
        if (m_handlingMessage == handlingMessage)
            return;

        m_handlingMessage = handlingMessage;
        emit handlingMessageChanged(m_handlingMessage);
    }

    void setCurrentLocationName(QString currentLocationName)
    {
        if (m_currentLocationName == currentLocationName)
            return;

        m_currentLocationName = currentLocationName;
        emit currentLocationNameChanged(m_currentLocationName);
    }

    void setUseOriginPr(bool useOriginPr)
    {
        if (m_useOriginPr == useOriginPr)
            return;

        m_useOriginPr = useOriginPr;
        emit useOriginPrChanged(m_useOriginPr);
    }

    void setCurrentCalibrationName(QString currentCalibrationName)
    {
        if (m_currentCalibrationName == currentCalibrationName)
            return;

        m_currentCalibrationName = currentCalibrationName;
        emit currentCalibrationNameChanged(m_currentCalibrationName);
    }

    void setCurrentChannelName(QString currentChannelName)
    {
        if (m_currentChannelName == currentChannelName)
            return;

        m_currentChannelName = currentChannelName;
        emit currentChannelNameChanged(m_currentChannelName);
    }

signals:
    void isHandlingChanged(bool isHandling);
    void handlingMessageChanged(QString handlingMessage);

    void currentLocationNameChanged(QString currentLocationName);

    void useOriginPrChanged(bool useOriginPr);

    void currentCalibrationNameChanged(QString currentCalibrationName);

    void currentChannelNameChanged(QString currentChannelName);

private:
    bool m_isHandling = false;
    QString m_handlingMessage = "";
    QString m_currentLocationName = "";
    bool m_useOriginPr = false;
    QString m_currentCalibrationName = "";
    QString m_currentChannelName = "";
};
#endif // LOGICMANAGERPARAMETER_H
