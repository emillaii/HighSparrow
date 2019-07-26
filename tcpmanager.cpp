#include "tcpmanager.h"

TcpManager::TcpManager(QObject *parent) : QObject(parent)
{
    this->moveToThread(&work_thread);
    work_thread.start();
//    work_thread.setPriority(QThread::TimeCriticalPriority);
    connect(this,&TcpManager::sendInit,this,&TcpManager::onInit);
    connect(this,&TcpManager::sendCreat,this,&TcpManager::onCreat,Qt::BlockingQueuedConnection);
    connect(this,&TcpManager::sendDelete,this,&TcpManager::onDelete,Qt::BlockingQueuedConnection);
}

TcpManager::~TcpManager()
{
    qDebug("~TcpManager in thread %d",QThread::currentThreadId());
    emit sendDelete();
    work_thread.quit();
    work_thread.wait();
}

void TcpManager::Init()
{
    emit sendInit();
}

bool TcpManager::loadJsonConfig(QString file_name)
{
    QJsonObject temp_object;
    bool result = PropertyBase::loadJsonObject(file_name,temp_object);
    emit sendCreat(TcpMessager::getStringFromJsonObject(temp_object));
    return result;
}

bool TcpManager::saveJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("TCP_MANAGER_PARAMETER",&parameters);
    foreach (TcpClientMessager* messager, m_client_messagers)
        temp_map.insert(QString("Client_").append(messager->parameters.messagerName()), &messager->parameters);
    foreach (TcpServerMessager* messager, m_server_messagers)
        temp_map.insert(QString("Server_").append(messager->parameters.messagerName()), &messager->parameters);
    PropertyBase::saveJsonConfig(file_name, temp_map);
    return true;
}

bool TcpManager::generateConfig(QString filename)
{
    if(m_server_messagers.isEmpty())
    {
        TcpServerMessager* temp_server = new TcpServerMessager();
        m_server_messagers.insert(temp_server->parameters.messagerName(),temp_server);
    }
    if(m_client_messagers.isEmpty())
    {
        TcpClientMessager* temp_client = new TcpClientMessager();
        m_client_messagers.insert(temp_client->parameters.messagerName(),temp_client);
    }
    return saveJsonConfig(filename);
}

TcpMessager* TcpManager::GetTcpMessager(QString messager_name)
{
    if(m_client_messagers.contains(messager_name))
        return m_client_messagers[messager_name];
    else {
        qInfo("TcpMessage(%s) is not exist",messager_name.toStdString().c_str());
    }
    return nullptr;
}

TcpMessager *TcpManager::GetAllTcpMessager(QString messager_name)
{
    if(m_client_messagers.contains(messager_name))
        return m_client_messagers[messager_name];
    else if(m_server_messagers.contains(messager_name))
        return m_server_messagers[messager_name];
    else {
        qInfo("all TcpMessage(%s) is not exist",messager_name.toStdString().c_str());
    }
    return nullptr;
}

TcpMessager *TcpManager::GetPeerTcpMessager(QString messager_name)
{
    if(m_server_messagers.contains(messager_name))
        return m_server_messagers[messager_name];
    else {
        qInfo("Peer TcpMessage(%s) is not exist",messager_name.toStdString().c_str());
    }
    return nullptr;
}

QList<TcpMessager *> TcpManager::GetAllTcpMessager()
{
    QList<TcpMessager *> temp_list;
    foreach (TcpMessager * temp_messager, m_server_messagers.values())
        temp_list.append(temp_messager);
    foreach (TcpMessager * temp_messager, m_client_messagers.values())
        temp_list.append(temp_messager);
    return temp_list;
}

void TcpManager::onCreat(QString paramter)
{
    qDebug("onCreat thread id:%d",QThread::currentThreadId());
    QJsonObject temp_object = TcpMessager::getJsonObjectFromString(paramter);
    foreach (QString key, temp_object.keys()) {
        if(key.contains("Client_"))
        {
            TcpClientMessager* temp_client = new TcpClientMessager();
            temp_client->parameters.read(temp_object[key].toObject());
            if(m_client_messagers.contains(temp_client->parameters.messagerName()))
            {
                m_client_messagers[temp_client->parameters.messagerName()]->parameters.read(temp_object[key].toObject());
                temp_client->deleteLater();
            }
            else
            {
                m_client_messagers.insert(temp_client->parameters.messagerName(),temp_client);
            }
        }
        else if(key.contains("Server_"))
        {
            TcpServerMessager* temp_server = new TcpServerMessager();
            temp_server->parameters.read(temp_object[key].toObject());
            if(m_server_messagers.contains(temp_server->parameters.messagerName()))
            {
                m_server_messagers[temp_server->parameters.messagerName()]->parameters.read(temp_object[key].toObject());
                temp_server->deleteLater();
            }
            else
            {
                m_server_messagers.insert(temp_server->parameters.messagerName(),temp_server);
            }
        }
        else if(key == "TCP_MANAGER_PARAMETER")
        {
            parameters.read(temp_object[key].toObject());
        }
    }
}

void TcpManager::onDelete()
{
    if(m_WebSocketServer != nullptr)
    m_WebSocketServer->close();
    foreach (TcpClientMessager* temp_client, m_client_messagers)
        delete temp_client;
    foreach (TcpServerMessager* temp_server, m_server_messagers)
       delete temp_server;
    m_client_messagers.clear();
    m_server_messagers.clear();
    if(m_WebSocketServer != nullptr)
    m_WebSocketServer->deleteLater();
}

void TcpManager::onInit()
{
    qDebug("onInit thread id:%d",QThread::currentThreadId());
    if(parameters.openServer())
    {
        qDebug("openServer port:%d",parameters.serverPort());
        m_WebSocketServer = new QWebSocketServer(QStringLiteral("Echo Server"),QWebSocketServer::NonSecureMode, this);
        connect(m_WebSocketServer, &QWebSocketServer::newConnection,this, &TcpManager::onNewConnection);
        m_WebSocketServer->listen(QHostAddress::Any,parameters.serverPort());
    }
    foreach (TcpClientMessager* messager, m_client_messagers)
        messager->Init();
}

void TcpManager::onNewConnection()
{
    qDebug("onNewConnection thread id:%d",QThread::currentThreadId());
    QWebSocket *pSocket = m_WebSocketServer->nextPendingConnection();
    QString temp_socket = pSocket->origin();
    if(m_server_messagers.contains(temp_socket)&&m_server_messagers[temp_socket]!=nullptr)
    {
       m_server_messagers[temp_socket]->Init(pSocket);
    }
    else {
        qInfo("not define tcp mssager %s",temp_socket.toStdString().c_str());
    }
}

