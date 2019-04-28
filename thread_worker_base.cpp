#include "thread_worker_base.h"


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
    work_thread.start();
}

QString ThreadWorkerBase::Name() const
{
    return m_Name;
}

void ThreadWorkerBase::setAlarmId(int id)
{
    if(id > 0 && alarm_id == 0)
        alarm_id = id;
}

void ThreadWorkerBase::sendAlarmMessage(int error_level, QString error_message)
{
    message_returned = false;
    emit sendErrorMessage(alarm_id,error_level,error_message);
}

void ThreadWorkerBase::receiveMessage(const int message_id,const int result_message)
{
    if(alarm_id == message_id)
    {
        QMutexLocker temp_locker(&message_mutex);
        message_returned = true;
        message_result = result_message;
    }
}

int ThreadWorkerBase::waitMessageReturn(bool &interruput)
{
    while (!interruput) {
        QThread::msleep(100);
        {
            QMutexLocker temp_locker(&message_mutex);
            if(message_returned)
            {
                message_returned = false;
                return  message_result;
            }
        }
    }
    return 1;
}

void ThreadWorkerBase::setName(QString Name)
{
    if (m_Name == Name)
        return;

    m_Name = Name;
    emit NameChanged(m_Name);
}
