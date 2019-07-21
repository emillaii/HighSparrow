#ifndef TCPMESSAGER_H
#define TCPMESSAGER_H

#include "tcpmessagerparameter.h"

#include <QMutex>
#include <QObject>
#include <qjsonobject.h>
#include <QJsonDocument>

#include <QtWebSockets/qwebsocket.h>

class TcpMessager : public QObject
{
    Q_OBJECT
public:
    explicit TcpMessager(QObject *parent = nullptr);
    ~TcpMessager();
    bool sendMessage(QString message);
    bool waitMessage(QString message,bool& is_run);
    bool checkMessage(QString message);
    void clearMessage(QString);
    QString inquiryMessage(QString message);
    static QJsonObject getJsonObjectFromString(const QString json_string);
    static QString getStringFromJsonObject(const QJsonObject& jsonObject);
    static QString getStringFromQvariantMap(const QVariantMap& qvariantMap);
private:
    bool waitMessage(QString& message);
signals:
    bool sendTextMessage(QString message);
    void receiveTextMessage(QString message);
public slots:
    bool onSendTextMessage(QString message);
    void onReceiveTextMessage(QString message);
public:
    TcpMessagerParameter parameters;
protected:
    QString sended_meesage;
    QMutex qure_mutex;
    QMutex message_mutex;
    QString received_message;
    QWebSocket* m_webSocket = Q_NULLPTR;
    bool m_is_connected = false;
    bool is_waitting = false;
};

#endif // TCPMESSAGER_H
