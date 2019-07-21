#ifndef SPARROWQSERVER_H
#define SPARROWQSERVER_H

#include <QObject>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QQueue>

class SparrowQServer : public QObject
{
    Q_OBJECT
public:
    explicit SparrowQServer(quint16 port, QObject *parent = nullptr);
    ~SparrowQServer();
private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
Q_SIGNALS:
    void closed();
    void receiveRequestMessage(QString, QString);
public Q_SLOTS:
    void sendMessageToClient(QString destAddress, QString message);
    int getConnectedClients();
private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
};

#endif // SPARROWQSERVER_H
