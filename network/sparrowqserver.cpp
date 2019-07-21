#include "network/sparrowqserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include "utils/commonutils.h"
QT_USE_NAMESPACE

SparrowQServer::SparrowQServer(quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Echo Server"),
                                               QWebSocketServer::NonSecureMode, this))
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
       qDebug() << "SparrowQServer listening on port" << port;
       connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
               this, &SparrowQServer::onNewConnection);
       connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &SparrowQServer::closed);
   }
}

SparrowQServer::~SparrowQServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void SparrowQServer::onNewConnection()
{
    qInfo("New connection established");
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &SparrowQServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &SparrowQServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &SparrowQServer::socketDisconnected);

    m_clients << pSocket;
}

int SparrowQServer::getConnectedClients()
{
    return m_clients.size();
}

void SparrowQServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    emit receiveRequestMessage(message, pClient->peerAddress().toString());
    qDebug() << "Server Message received:" << message;
}

void SparrowQServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

void SparrowQServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void SparrowQServer::sendMessageToClient(QString dest, QString message)
{
    qInfo("send Message to client dest: %s message: %s size of m_client: %d", dest.toStdString().c_str(), message.toStdString().c_str(), m_clients.size());
    for(int i = 0; i < m_clients.size(); i++)
    {
        QString temp_ip = m_clients[i]->peerAddress().toString();
        if (dest == m_clients[i]->peerAddress().toString()) {
            qInfo("addr %s", m_clients[i]->peerAddress().toString().toStdString().c_str());
            m_clients[i]->sendTextMessage(message);
            break;
        }
        if (dest == "remote" && m_clients[i]->peerAddress().toString() != "::1") {
            qInfo("Send to remote AA %s", message.toStdString().c_str());
            m_clients[i]->sendTextMessage(message);
            break;
        }
    }
}
