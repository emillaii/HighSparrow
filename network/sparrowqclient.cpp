#include "network/sparrowqclient.h"

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
    connect(&m_webSocket, &QWebSocket::disconnected, this, &SparrowClient::onClosed);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &SparrowClient::onTextMessageReceived);
    m_webSocket.open(QUrl(url));
}
//! [constructor]

//! [onConnected]
void SparrowClient::onConnected()
{
    if (m_debug)
        qDebug() << "WebSocket connected" << m_url;
    m_is_connected = true;
}
//! [onConnected]

void SparrowClient::onClosed()
{
    m_is_connected = false;
    qDebug("sparrow client disconnect..Going to retry the connection :%s", m_url.url().toStdString().c_str());
      m_webSocket.open(QUrl(m_url));
}

//! [onTextMessageReceived]
void SparrowClient::onTextMessageReceived(QString message)
{
//    if (m_debug)
//       qInfo("Client Socket Message received: %s",message.toStdString().c_str());
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
    if(m_is_connected) {
        qDebug("SparrowClient::sendMessage: %s",message.toStdString().c_str());
        m_webSocket.sendTextMessage(message);
    } else {
        qDebug("SparrowClient::sendMessage fail due to socket is ot connected");
    }
}

bool SparrowClient::isConnected()
{
    return this->m_is_connected;
}
