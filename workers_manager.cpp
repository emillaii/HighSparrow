#include "workers_manager.h"

WorkersManager::WorkersManager(QObject *parent):QObject (parent)
{

}

bool WorkersManager::registerWorker(ThreadWorkerBase* worker)
{
    if(!workers.contains(worker->Name()))
    {
        workers.insert(worker->Name(),worker);
        connect(this,&WorkersManager::startWorkersSignal,worker,&ThreadWorkerBase::startWork);
        connect(this,&WorkersManager::stopWorkersSignal,worker,&ThreadWorkerBase::stopWork);
        connect(worker,&ThreadWorkerBase::sendHandlingOperation,worker,&ThreadWorkerBase::performHandlingOperation);
        qInfo("registerWorker :%s",worker->Name().toStdString().c_str());
        return true;
    }
    return false;
}

void WorkersManager::startWorkers(bool reset,int run_mode)
{
    emit startWorkers(reset,run_mode);
}

void WorkersManager::stopWorkers(bool wait_finish)
{
    emit stopWorkers(wait_finish);
}


void WorkersManager::startWorker(QString name,bool reset,int run_mode)
{
    if("" != current_name && name != current_name)
        disconnect(this,&WorkersManager::startWorkerSignal,workers[current_name],&ThreadWorkerBase::startWork);
    connect(this,&WorkersManager::startWorkerSignal,workers[name],&ThreadWorkerBase::startWork);
    emit startWorkerSignal(reset,run_mode);
}

void WorkersManager::stopWorker(QString name, bool wait_finish)
{
    if("" != current_name && name != current_name)
        disconnect(this,&WorkersManager::stopWorkerSignal,workers[current_name],&ThreadWorkerBase::stopWork);
    connect(this,&WorkersManager::stopWorkerSignal,workers[name],&ThreadWorkerBase::stopWork);
    emit stopWorkerSignal(wait_finish);
}

QList<QString> WorkersManager::getWorkersNames()
{
    return workers.keys();
}
