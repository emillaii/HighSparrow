#include <qlocalsocket.h>
#include <qobject.h>


class CClient
     : public QObject
{
public:
     CClient()
         : m_socket(nullptr)
     {
         m_socket = new QLocalSocket();

         QObject::connect(m_socket, &QLocalSocket::connected, this, &CClient::socketConnectedHandler);
         QObject::connect(m_socket, &QLocalSocket::disconnected, this,  &CClient::socketDisConnectedHandler);
         //QObject::connect(m_socket, &QLocalSocket::error, this, SLOT(socketErrorHandler(QLocalSocket::LocalSocketError)));
    }

    ~CClient()
    {
        m_socket->disconnectFromServer();

        delete m_socket;
    }

public:
    bool ConnectToServer(const QString &strServerName)
    {
        // 服务端的serverNewConnectionHandler成员方法将被调用
        m_socket->connectToServer("localserver-test");
        if (m_socket->waitForConnected())
        {
            return true;
        }
        return false;
    }

    void sendMessage(const QString &msg)
    {
        m_socket->write(msg.toStdString().c_str());
        m_socket->flush();
        int result = m_socket->waitForBytesWritten();
        qInfo("send cmd %d",result);
    }
    QString waitRespond(int out_tine = 10000)
    {
        // waitForReadyRead将激发信号readyRead()， 我们在
        // 与服务端创建连接时，已将readyRead()，绑定到了服务
        // 端的槽socketReadyReadHandler成员方法
        bool wait_result = false;
        if (!m_socket->bytesAvailable())
            wait_result = m_socket->waitForReadyRead(out_tine);
        if(!wait_result)
            return u8"等待LensVCM超时";
        QTextStream stream(m_socket);
        QString respond = stream.readAll();
        qDebug() << "Read Data From Server:" << respond;
        return respond;
    }

private slots:
    void socketConnectedHandler()
    {
        qDebug() << "connected.";
    }

    void socketDisConnectedHandler()
    {
        qDebug() << "disconnected.";
    }

    void socketErrorHandler(QLocalSocket::LocalSocketError error)
    {
        qWarning() << error;
    }

private:
    QLocalSocket *m_socket;
};
