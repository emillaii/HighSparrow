#ifndef TCPMANAGERPARAMETER_H
#define TCPMANAGERPARAMETER_H

#include "propertybase.h"



class TcpManagerParameter : public PropertyBase
{
    Q_OBJECT
public:
    explicit TcpManagerParameter(QObject *parent = nullptr):PropertyBase(parent){}
    Q_PROPERTY(bool openServer READ openServer WRITE setOpenServer NOTIFY openServerChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)

    bool openServer() const
    {
        return m_openServer;
    }
    int serverPort() const
    {
        return m_serverPort;
    }

public slots:
    void setOpenServer(bool openServer)
    {
        if (m_openServer == openServer)
            return;

        m_openServer = openServer;
        emit openServerChanged(m_openServer);
    }
    void setServerPort(int serverPort)
    {
        if (m_serverPort == serverPort)
            return;

        m_serverPort = serverPort;
        emit serverPortChanged(m_serverPort);
    }

signals:
    void openServerChanged(bool openServer);
    void serverPortChanged(int serverPort);

private:
    bool m_openServer = true;
    int m_serverPort = 19996;
};

#endif // TCPMANAGERPARAMETER_H
