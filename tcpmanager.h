#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include "tcpclientmessager.h"
#include "tcpmanagerparameter.h"
#include "tcpservermessager.h"

#include <qthread.h>

#include <QtWebSockets/qwebsocket.h>
#include <QtWebSockets/qwebsocketserver.h>

class TcpManager : public QObject
{
    Q_OBJECT
public:
    explicit TcpManager(QObject *parent = nullptr);
    ~TcpManager();
    void Init();
    bool loadJsonConfig(QString file_name);
    bool saveJsonConfig(QString file_name);
    bool generateConfig(QString filename);
    TcpMessager* GetTcpMessager(QString messager_name);
    TcpMessager *GetAllTcpMessager(QString messager_name);
    TcpMessager *GetPeerTcpMessager(QString messager_name);
    QList<TcpMessager *> GetAllTcpMessager();
signals:
    void sendInit();
    void sendCreat(QString paramter);
    void sendDelete();
public slots:
    void onInit();
    void onCreat(QString paramter);
    void onDelete();
    void onNewConnection();
private:
    QWebSocketServer * m_WebSocketServer = Q_NULLPTR;
    QMap<QString,TcpServerMessager*> m_server_messagers;
    QMap<QString,TcpClientMessager*> m_client_messagers;
    QThread work_thread;
public:
    TcpManagerParameter parameters;
};

#endif // TCPMANAGER_H
