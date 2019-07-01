#include "tcpclientmessager.h"

TcpClientMessager::TcpClientMessager():TcpMessager ()
{

}

void TcpClientMessager::Init()
{
    this->m_webSocket = new QWebSocket(parameters.messagerName());
    connect(m_webSocket, &QWebSocket::connected, this, &TcpClientMessager::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &TcpClientMessager::onClosed);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &TcpMessager::onReceiveTextMessage,Qt::DirectConnection);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &TcpMessager::receiveTextMessage);
    m_webSocket->open(QUrl(parameters.address()));
}

void TcpClientMessager::onConnected()
{
    if (parameters.needQInfo())
        qDebug() << "tcp WebSocket connected" << parameters.address();
    m_is_connected = true;
}


void TcpClientMessager::onClosed()
{
    if (parameters.needQInfo())
        qDebug() << "tcp WebSocket disconnect. Going to retry the connection" << parameters.address();
    m_is_connected = false;
    m_webSocket->open(QUrl(parameters.address()));
}
