#ifndef XTMOTORPARAMETER_H
#define XTMOTORPARAMETER_H

#include "propertybase.h"

class XtMotorParameter:public PropertyBase
{
    Q_OBJECT
public:
    XtMotorParameter():PropertyBase(){}
    Q_PROPERTY(int arrivedDelay READ arrivedDelay WRITE setarrivedDelay NOTIFY arrivedDelayChanged)
    Q_PROPERTY(bool useDelay READ useDelay WRITE setUseDelay NOTIFY useDelayChanged)
    Q_PROPERTY(double positionError READ positionError WRITE setPositionError NOTIFY positionErrorChanged)
    int arrivedDelay() const
    {
        return m_arrivedDelay;
    }
    bool useDelay() const
    {
        return m_useDelay;
    }

    double positionError() const
    {
        return m_positionError;
    }

public slots:
    void setarrivedDelay(int arrivedDelay)
    {
        if (m_arrivedDelay == arrivedDelay)
            return;

        m_arrivedDelay = arrivedDelay;
        emit arrivedDelayChanged(m_arrivedDelay);
    }
    void setUseDelay(bool useDelay)
    {
        if (m_useDelay == useDelay)
            return;

        m_useDelay = useDelay;
        emit useDelayChanged(m_useDelay);
    }

    void setPositionError(double positionError)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_positionError, positionError))
            return;

        m_positionError = positionError;
        emit positionErrorChanged(m_positionError);
    }

signals:
    void arrivedDelayChanged(int arrivedDelay);
    void useDelayChanged(bool useDelay);

    void positionErrorChanged(double positionError);

private:
    int m_arrivedDelay = 100;
    bool m_useDelay = true;
    double m_positionError = 0.01;
};
class XtMotorState:public PropertyBase
{
    Q_OBJECT
public:
    XtMotorState():PropertyBase () {}
    Q_PROPERTY(bool isInit READ isInit WRITE setIsInit NOTIFY isInitChanged)
    Q_PROPERTY(bool isEnabled READ isEnabled WRITE setIsEnabled NOTIFY isEnabledChanged)
    Q_PROPERTY(bool seekedOrigin READ seekedOrigin WRITE setSeekedOrigin NOTIFY seekedOriginChanged)

    bool isInit() const
    {
        return m_isInit;
    }
    bool isEnabled() const
    {
        return m_isEnabled;
    }

    bool seekedOrigin() const
    {
        return m_seekedOrigin;
    }

public slots:
    void setIsInit(bool isInit)
    {
        if (m_isInit == isInit)
            return;

        m_isInit = isInit;
        emit isInitChanged(m_isInit);
    }
    void setIsEnabled(bool isEnabled)
    {
        if (m_isEnabled == isEnabled)
            return;

        m_isEnabled = isEnabled;
        emit isEnabledChanged(m_isEnabled);
    }

    void setSeekedOrigin(bool seekedOrigin)
    {
        if (m_seekedOrigin == seekedOrigin)
            return;

        m_seekedOrigin = seekedOrigin;
        emit seekedOriginChanged(m_seekedOrigin);
    }

signals:
    void isInitChanged(bool isInit);
    void isEnabledChanged(bool isEnabled);

    void seekedOriginChanged(bool seekedOrigin);

private:
    bool m_isInit = false;
    bool m_isEnabled = false;
    bool m_seekedOrigin = false;
};
#endif // XTMOTORPARAMETER_H
