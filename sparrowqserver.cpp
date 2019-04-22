#include "sparrowqserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include "commonutils.h"
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
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &SparrowQServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &SparrowQServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &SparrowQServer::socketDisconnected);

    m_clients << pSocket;
}

void SparrowQServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Server Message received:" << message;
    QJsonObject obj = getJsonObjectFromString(message);
    commandQueue.enqueue(obj);
    qInfo("Command queue: %d", commandQueue.count());
//    if (pClient) {
//        pClient->sendTextMessage(message);
//    }
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

QJsonObject SparrowQServer::commandDequeue()
{
    QJsonObject emptyObj;
    if (commandQueue.size() > 0) return commandQueue.dequeue();
    return emptyObj;
}

int SparrowQServer::commandQueueSize()
{
    return commandQueue.size();
}
