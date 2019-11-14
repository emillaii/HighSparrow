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
    QJsonObject object = getJsonObjectFromString(message);
    object["sender_name"] = parameters.messagerName();
    if(m_is_connected)
    {
        bool result = emit sendTextMessage(getStringFromJsonObject(object));
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
           bool try_result = message_mutex.tryLock(parameters.outTime());
           if(try_result)
           {
               if(message == received_message)
               {
                   message_mutex.unlock();
                   return true;
               }
               message_mutex.unlock();
           }
           else if(parameters.needQInfo())
               qInfo("message_mutex lock fail");
        }
        time_out --;
        QThread::msleep(1);
    }
    return false;
}

bool TcpMessager::checkMessage(QString message)
{
    bool try_result = message_mutex.tryLock(parameters.outTime());
    if(try_result)
    {
        if(message == received_message)
        {
            message_mutex.unlock();
            return true;
        }
        message_mutex.unlock();
    }
    else if(parameters.needQInfo())
        qInfo("message_mutex lock fail");
    return false;
}

void TcpMessager::clearMessage(QString)
{
    bool try_result = message_mutex.tryLock(parameters.outTime());
    if(try_result)
    {
        received_message = "";
        message_mutex.unlock();
    }
    else if(parameters.needQInfo())
        qInfo("message_mutex lock fail");
}

QString TcpMessager::inquiryMessage(QString message)
{
    if (parameters.needQInfo())
        qDebug("%s inquiryquareMessage %s in thread %d",parameters.messagerName().toStdString().c_str(),message.toStdString().c_str(),QThread::currentThreadId());
    bool result = qure_mutex.tryLock(parameters.outTime());
    QJsonObject result_message;
    if(result)
    {
        is_waitting = true;//多个消息的返回需要分开。。
        result = sendMessage(message);
        if(result)
        {
            int current_time = parameters.outTime();
            result = false;
            while (current_time > 0)
            {
                if(!is_waitting)
                {
                    bool try_result = message_mutex.tryLock(parameters.outTime());
                    if(try_result)
                    {
                        result_message = getJsonObjectFromString(received_message);
                        message_mutex.unlock();
                    }
                    else if(parameters.needQInfo())
                        qInfo("message_mutex lock fail");
                    result = true;
                    if (parameters.needQInfo())
                        qDebug("wait time:%d in thread %d",(parameters.outTime() - current_time),QThread::currentThreadId());
                    break;
                }
                current_time --;
                if (parameters.needQInfo())
                    qDebug("current_time: %d",current_time);
                QThread::msleep(1);
            }
            if(result_message.keys().count()<= 0)
                result_message["error"] = "inquiry time out";
        }
        else
            result_message["error"] = "send inquiry message fail";
        qure_mutex.unlock();
    }
    else
        result_message["error"] = "tryLock fail";
    is_waitting = false;
    if(!result)
    qDebug("inquiry Result:%d in thread %d error:%s",result,QThread::currentThreadId(),result_message["error"].toString().toStdString().c_str());
    return  getStringFromJsonObject(result_message);
}

QJsonObject TcpMessager::getJsonObjectFromString(const QString json_string)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json_string.toUtf8().data());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< json_string.toUtf8().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

QString TcpMessager::getStringFromJsonObject(const QJsonObject &json_object)
{
    return QString(QJsonDocument(json_object).toJson());
}

QString TcpMessager::getStringFromQvariantMap(const QVariantMap &qvariantMap)
{
    return QString(QJsonDocument(QJsonObject::fromVariantMap(qvariantMap)).toJson());
}

bool TcpMessager::onSendTextMessage(QString message)
{
    if (parameters.needQInfo())
        qDebug("%s tcp WebSocket send %s in thread %d",parameters.messagerName().toStdString().c_str(),message.toStdString().c_str(),QThread::currentThreadId());
     qint64 result = m_webSocket->sendTextMessage(message);
     if (parameters.needQInfo())
         qDebug("%s tcp WebSocket send result %d message size %d in thread %d",parameters.messagerName().toStdString().c_str(),result,message.size(),QThread::currentThreadId());
     return result > 0;
}

void TcpMessager::onReceiveTextMessage(QString message)
{
    if (parameters.needQInfo())
        qDebug("%s tcp WebSocket receive %s in thread %d",parameters.messagerName().toStdString().c_str(),message.toStdString().c_str(),QThread::currentThreadId());
    bool try_result = message_mutex.tryLock(parameters.outTime());
    if(try_result)
    {
        received_message = message;
        message_mutex.unlock();
    }
    else if(parameters.needQInfo())
        qInfo("message_mutex lock fail");
    is_waitting = false;
}
