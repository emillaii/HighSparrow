#include "sparrowqclient.h"

#include <QtCore/QDebug>
#include <QJsonObject>
QT_USE_NAMESPACE

//! [constructor]
SparrowClient::SparrowClient(const QUrl &url, bool debug, QObject *parent) :
    QObject(parent),
    m_url(url),
    m_debug(debug)
{
    if (m_debug)
        qDebug() << "WebSocket server:" << url;
    connect(&m_webSocket, &QWebSocket::connected, this, &SparrowClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &SparrowClient::closed);
    m_webSocket.open(QUrl(url));
}
//! [constructor]

//! [onConnected]
void SparrowClient::onConnected()
{
    if (m_debug)
        qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &SparrowClient::onTextMessageReceived);
}
//! [onConnected]

//! [onTextMessageReceived]
void SparrowClient::onTextMessageReceived(QString message)
{
    if (m_debug)
        qDebug() << "Client Socket Message received:" << message;
    //m_webSocket.close();
    emit receiveMessage(message);
}
//! [onTextMessageReceived]

QJsonObject SparrowClient::commandDequeue()
{
    QJsonObject emptyObj;
    if (commandQueue.size() > 0) return commandQueue.dequeue();
    return emptyObj;
}

int SparrowClient::commandQueueSize()
{
    return commandQueue.size();
}

void SparrowClient::sendMessage(QString message)
{
    m_webSocket.sendTextMessage(message);
}
