#include "tcpservermessager.h"

TcpServerMessager::TcpServerMessager():TcpMessager ()
{

}

void TcpServerMessager::Init(QWebSocket *web_socket)
{
    if(m_webSocket != nullptr)
    {
        m_webSocket->deleteLater();
        m_webSocket = nullptr;
    }
    m_webSocket = web_socket;
    connect(m_webSocket, &QWebSocket::disconnected, this, &TcpServerMessager::onClosed);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &TcpMessager::onReceiveTextMessage,Qt::DirectConnection);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &TcpMessager::receiveTextMessage);
    m_is_connected = true;
}

void TcpServerMessager::onClosed()
{
    m_is_connected = false;
    if(m_webSocket != nullptr)
    {
        m_webSocket->deleteLater();
        m_webSocket = nullptr;
    }
}
