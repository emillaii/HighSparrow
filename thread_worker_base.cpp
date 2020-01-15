#include "thread_worker_base.h"
#include <utils/commonutils.h>
#include "config.h"
#include <QFile>
ThreadWorkerBase::ThreadWorkerBase(QString name,QObject *parent) : QObject(parent),ErrorBase (name)
{
    static int auto_id = 0;
    if("UnnamingWorker" == name)
    {
        auto_id++;
        name.append(auto_id);
    }
    setName(name);
    this->setObjectName(name);
    this->moveToThread(&work_thread);
//    connect(&work_thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    work_thread.start();
    connect(this,&ThreadWorkerBase::sendHandlingOperation,this,&ThreadWorkerBase::performHandlingOperation);
}

ThreadWorkerBase::~ThreadWorkerBase()
{
    work_thread.quit();
    work_thread.wait();
}

QString ThreadWorkerBase::Name() const
{
    return m_Name;
}


int ThreadWorkerBase::getAlarmId()
{
    int alarm_id = choosed_operations.size() + 1;
    while (choosed_operations.contains(alarm_id))
        alarm_id++;
    return alarm_id;
}

void ThreadWorkerBase::clearAlarm()
{
    choosed_operations.clear();
}

int ThreadWorkerBase::sendAlarmMessage(QString error_tips, QString error_message,int error_level)
{
    QVariantMap message_map;
    int alarm_id = 1 == error_level?getAlarmId():0;
    message_map.insert("AlarmId",alarm_id);
    message_map.insert("TargetModule","WorksManager");
    message_map.insert("Message","AlamMessage");
    message_map.insert("ErrorTips",error_tips);
    message_map.insert("ErrorMessage",error_message);
    message_map.insert("ErrorLevel",error_level);
    message_map.insert("OriginModule",Name());
    emit sendModuleMessage(message_map);
    sendMessageToModule("LogicManager2", "OpenAlarmLight");
    return alarm_id;
}

QString ThreadWorkerBase::waitMessageReturn(bool &interruput,int alarm_id)
{
    while (interruput)
    {
        QThread::msleep(1000);
        {
            QMutexLocker temp_locker(&message_mutex);
            if(choosed_operations.contains(alarm_id))
            {
                QString operation = choosed_operations[alarm_id];
                qInfo("%s waitMessageReturn %s",Name().toStdString().c_str(),operation.toStdString().c_str());
                choosed_operations.remove(alarm_id);
                sendMessageToModule("LogicManager2", "CloseAlarmLight");
                return operation;
            }
        }
    }
    return "";
}

void ThreadWorkerBase::performHandling(int cmd,QVariant param)
{
    if(is_handling)
    {
        qInfo("is_handling");
        return;
    }
    is_handling = true;
    is_error = false;
    emit sendHandlingOperation(cmd,param);
}

bool ThreadWorkerBase::waitPerformHandling()
{
    while (is_handling)
    {
        QThread::msleep(10);
    }
    return is_error;
}

bool ThreadWorkerBase::waitResponseMessage(bool &is_run, QString target_message)
{
    int current_time = 0;
    while (is_run)
    {
        current_time++;
        QThread::msleep(10);
        {
            QMutexLocker temp_locker(&message_mutex);
            if(choosed_operations.contains(1))
            {
                if(module_message.contains("Response")&&module_message["Response"] == target_message)
                {
                    qInfo("wait repnonse time : %d",current_time*10);
                    return true;
                }
            }
        }
    }
    return false;
}

//ToDo: check the thread tcp response instead of checking file existence
bool ThreadWorkerBase::waitVisionResponseMessage(QString filename)
{
    int current_time = 0;
    while (current_time < 30)
    {
        current_time++;
        QThread::msleep(100);
        {
            QFile mFile(filename);
            if(mFile.exists())
            {
                qInfo("wait repnonse time : %d filename:Image Grabber is not me: %s",current_time*10, filename.toStdString().c_str());
                QThread::msleep(400); //ToDo: Minimize this wait time for loading image
                return true;
            }
        }
    }
    return false;
}

void ThreadWorkerBase::receivceModuleMessageBase(QVariantMap message)
{
    qInfo("%s receive module message %s",Name().toStdString().c_str(),getStringFromQvariantMap(message).toStdString().c_str());
    QMutexLocker temp_locker(&message_mutex);
    if(message.contains("TargetModule")&&message["TargetModule"].toString() == "WorksManager")
    {
        qInfo("receive WorksManager message");
        this->module_message = message;
    }
    else if(message.contains("OriginModule")&&message["OriginModule"].toString() == "AlarmModule"&&(!choosed_operations.contains(message["AlarmId"].toInt())))
    {
        qInfo("receive AlarmModule message");
        choosed_operations.insert(message["AlarmId"].toInt(),message["Operation"].toString());
    } else if (message["TargetModule"].toString() == VISION_MODULE_2) {
        QString cameraName = message["cameraName"].toString();
        QString filename = message["filename"].toString();
        qInfo("Receive vision module message cameraName: %s filename: %s", cameraName.toStdString().c_str(), filename.toStdString().c_str());
        //this->vision_operations.insert(cameraName, filename);
    }
    receivceModuleMessage(message);
}

void ThreadWorkerBase::sendMessageToModule(QString module_name, QString message,QJsonValue param)
{
    QVariantMap message_map;
    message_map.insert("TargetModule",module_name);
    message_map.insert("Message",message);
    foreach (QString param_name, param.toObject().keys())
        message_map.insert(param_name,param[param_name].toVariant());
    message_map.insert("OriginModule",Name());
    emit sendModuleMessage(message_map);
}

QString ThreadWorkerBase::getModuleMessage()
{
    return module_message["Message"].toString();
}

bool ThreadWorkerBase::loadJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map = getModuleParameter();
    PropertyBase::loadJsonConfig(file_name, temp_map);
    return true;
}

void ThreadWorkerBase::saveJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map = getModuleParameter();
    PropertyBase::saveJsonConfig(file_name, temp_map);
}

QVariantMap ThreadWorkerBase::inquirRunParameters(int out_time)
{
    module_message.clear();
    sendMessageToModule("WorksManager","inquirRunParameters");
    int current_time = 0;
    while (current_time < out_time)
    {
        if(!module_message.isEmpty())
            return module_message;
        QThread::msleep(10);
        current_time += 10;
    }
    qInfo("inquirRunParameters timeout");
    return module_message;
}
void ThreadWorkerBase::setName(QString Name)
{
    if (m_Name == Name)
        return;

    m_Name = Name;
    emit NameChanged(m_Name);
}
