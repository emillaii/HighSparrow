#ifndef TCPMESSAGERPARAMETER_H
#define TCPMESSAGERPARAMETER_H

#include "propertybase.h"



class TcpMessagerParameter: public PropertyBase
{
    Q_OBJECT
public:
    TcpMessagerParameter():PropertyBase(){}
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(bool needQInfo READ needQInfo WRITE setNeedQInfo NOTIFY needQInfoChanged)
    Q_PROPERTY(QString messagerName READ messagerName WRITE setMessagerName NOTIFY messagerNameChanged)
    Q_PROPERTY(int outTime READ outTime WRITE setOutTime NOTIFY outTimeChanged)
    Q_PROPERTY(int waitIntervel READ waitIntervel WRITE setWaitIntervel NOTIFY waitIntervelChanged)
    QString address() const
    {
        return m_address;
    }

    bool needQInfo() const
    {
        return m_needQInfo;
    }

    QString messagerName() const
    {
        return m_messagerName;
    }

    int outTime() const
    {
        return m_outTime;
    }

    int waitIntervel() const
    {
        return m_waitIntervel;
    }

public slots:
    void setAddress(QString address)
    {
        if (m_address == address)
            return;

        m_address = address;
        emit addressChanged(m_address);
    }

    void setNeedQInfo(bool needQInfo)
    {
        if (m_needQInfo == needQInfo)
            return;

        m_needQInfo = needQInfo;
        emit needQInfoChanged(m_needQInfo);
    }

    void setMessagerName(QString messagerName)
    {
        if (m_messagerName == messagerName)
            return;

        m_messagerName = messagerName;
        emit messagerNameChanged(m_messagerName);
    }

    void setOutTime(int outTime)
    {
        if (m_outTime == outTime)
            return;

        m_outTime = outTime;
        emit outTimeChanged(m_outTime);
    }

    void setWaitIntervel(int waitIntervel)
    {
        if (m_waitIntervel == waitIntervel || waitIntervel < 1)
            return;
        m_waitIntervel = waitIntervel;
        emit waitIntervelChanged(m_waitIntervel);
    }

signals:
    void addressChanged(QString address);

    void needQInfoChanged(bool needQInfo);

    void messagerNameChanged(QString messagerName);

    void outTimeChanged(int outTime);

    void waitIntervelChanged(int waitIntervel);

private:
    QString m_address = "ws://localhost:19996";
    bool m_needQInfo = true;
    QString m_messagerName = "unnaming";
    int m_outTime = 100;
    int m_waitIntervel = 10;
};

#endif // TCPMESSAGERPARAMETER_H
