#ifndef BASEMODULEMANAGERPARAMETER_H
#define BASEMODULEMANAGERPARAMETER_H

#include "propertybase.h"
class ModuleManangerConfig:public PropertyBase
{
    Q_OBJECT
    Q_PROPERTY(int serverMode READ serverMode WRITE setServerMode NOTIFY serverModeChanged)

public:
    int serverMode() const
    {
        return m_serverMode;
    }

public slots:
    void setServerMode(int serverMode)
    {
        if (m_serverMode == serverMode)
            return;

        m_serverMode = serverMode;
        emit serverModeChanged(m_serverMode);
    }

signals:
    void serverModeChanged(int serverMode);

private:
    int m_serverMode = 0;
};


class ModuleManagerParameter:public PropertyBase
{
    Q_OBJECT
public:
    ModuleManagerParameter(){}
    Q_PROPERTY(int serverMode READ serverMode WRITE setServerMode NOTIFY serverModeChanged)
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(QString materialType READ materialType WRITE setMaterialType NOTIFY materialTypeChanged)
    int serverMode() const
    {
        return m_serverMode;
    }
    int runMode() const
    {
        return m_runMode;
    }

    QString materialType() const
    {
        return m_materialType;
    }

public slots:
    void setServerMode(int serverMode)
    {
        if (m_serverMode == serverMode)
            return;

        m_serverMode = serverMode;
        emit serverModeChanged(m_serverMode);
    }
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

signals:
    void serverModeChanged(int serverMode);
    void runModeChanged(int runMode);

    void materialTypeChanged(QString materialType);

private:
    int m_serverMode = 0;
    int m_runMode = 0;
    QString m_materialType = "04b";
};
class ModuleManagerState:public PropertyBase
{
    Q_OBJECT
public:
    ModuleManagerState(){}
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
