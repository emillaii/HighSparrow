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
//    connect(&work_thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    work_thread.start();
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

void ThreadWorkerBase::setAlarmId(int id)
{
    if(id > 0 && alarm_id == 0)
        alarm_id = id;
}

int ThreadWorkerBase::getAlarmId()
{
   return alarm_id;
}

void ThreadWorkerBase::sendAlarmMessage(int error_level, QString error_message)
{
    message_returned = false;
    emit sendErrorMessage(alarm_id,error_level,error_message);
}

void ThreadWorkerBase::receiveOperation(const int sender_id,const int operation_type)
{
    if(alarm_id == sender_id)
    {
        QMutexLocker temp_locker(&message_mutex);
        message_returned = true;
        this->operation_type = operation_type;
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
                return  operation_type;
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
