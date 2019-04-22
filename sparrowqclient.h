#ifndef SPARROWQCLIENT_H
#define SPARROWQCLIENT_H
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QQueue>
class SparrowClient : public QObject
{
    Q_OBJECT
public:
    explicit SparrowClient(const QUrl &url, bool debug = false, QObject *parent = nullptr);
    QJsonObject commandDequeue();
    int commandQueueSize();
    void clearCommandQueue();
Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);

private:
    QQueue<QJsonObject> commandQueue;
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;
};


#endif // SPARROWQCLIENT_H
