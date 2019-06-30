#include "tcpmessager.h"

#include <qjsondocument.h>
#include <qthread.h>

TcpMessager::TcpMessager(QObject *parent) : QObject(parent)
{
    connect(this,&TcpMessager::sendTextMessage,this,&TcpMessager::onSendTextMessage,Qt::BlockingQueuedConnection);
}

TcpMessager::~TcpMessager()
{
    if(m_webSocket != nullptr)
        m_webSocket->deleteLater();
}

bool TcpMessager::sendMessage(QString message)
{
    if(m_is_connected)
    {
        bool result = emit sendTextMessage(message);
        if(result)
            return true;
    }
    return false;
}

bool TcpMessager::waitMessage(QString message, bool &is_run)
{
    int time_out = parameters.outTime();
    while (time_out > 0) {
        if(is_run)
            return false;
        {
            QMutexLocker locker(&message_mutex);
            if(message == received_message)
                return true;
        }
        time_out -= parameters.waitIntervel();
        QThread::msleep(parameters.waitIntervel());
    }
    return false;
}

bool TcpMessager::checkMessage(QString message)
{
    QMutexLocker locker(&message_mutex);
    if(message == received_message)
        return true;
    return false;
}

void TcpMessager::clearMessage(QString)
{
    QMutexLocker locker(&message_mutex);
    received_message = "";
}

QString TcpMessager::quareMessage(QString message)
{
    if (parameters.needQInfo())
        qDebug("%s quareMessage %s in thread %d",parameters.messagerName().toStdString().c_str(),message.toStdString().c_str(),QThread::currentThreadId());
    bool result = qure_mutex.tryLock(parameters.outTime());
    QJsonObject result_message;
    if(result)
    {
        is_waitting = true;
        result = sendMessage(message);
        if(result)
        {
            int time_out = parameters.outTime();
            result = false;
            while (time_out > 0)
            {
                if(!is_waitting)
                {
                    QMutexLocker locker(&message_mutex);
                    result_message = getJsonObjectFromString(received_message);
                    result = true;
                    qDebug("wait time:%d in thread %d",(parameters.outTime() - time_out),QThread::currentThreadId());
                    break;
                }
                time_out -= parameters.waitIntervel();
                QThread::msleep(parameters.waitIntervel());
            }
        }
    }
    is_waitting = false;
    qure_mutex.unlock();
//    if(!result)
    qDebug("QuareResult:%d in thread %d",result,QThread::currentThreadId());
    result_message["QuareResult"] = result;
    return  getStringFromJsonObject(result_message);
}

QJsonObject TcpMessager::getJsonObjectFromString(const QString json_string)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json_string.toLocal8Bit().data());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< json_string.toLocal8Bit().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

QString TcpMessager::getStringFromJsonObject(const QJsonObject &json_object)
{
    return QString(QJsonDocument(json_object).toJson());
}

bool TcpMessager::onSendTextMessage(QString message)
{
    if (parameters.needQInfo())
        qDebug("%s WebSocket send %s in thread %d",parameters.messagerName().toStdString().c_str(),message.toStdString().c_str(),QThread::currentThreadId());
     qint64 result = m_webSocket->sendTextMessage(message);
     return result == message.size();
}

void TcpMessager::onReceiveTextMessage(QString message)
{
    if (parameters.needQInfo())
        qDebug("%s WebSocket receive %s in thread %d",parameters.messagerName().toStdString().c_str(),message.toStdString().c_str(),QThread::currentThreadId());
    QMutexLocker locker(&message_mutex);
    is_waitting = false;
    received_message = message;
}