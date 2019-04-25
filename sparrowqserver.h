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
    QJsonObject commandDequeue();
    int commandQueueSize();
    void clearCommandQueue();
private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    QQueue<QJsonObject> commandQueue;

Q_SIGNALS:
    void closed();
    void receiveRequestMessage(QString, QString);
private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
};

#endif // SPARROWQSERVER_H
