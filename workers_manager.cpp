#include "workers_manager.h"

#include <QMessageBox>

WorkersManager::WorkersManager(QObject *parent):QObject (parent)
{

}

bool WorkersManager::registerWorker(ThreadWorkerBase* worker)
{
    if(!workers.contains(worker->Name()))
    {
        workers.insert(worker->Name(),worker);
        connect(this,&WorkersManager::startWorkersSignal,worker,&ThreadWorkerBase::startWork);
        connect(this,&WorkersManager::stopWorkersSignal,worker,&ThreadWorkerBase::stopWork,Qt::DirectConnection);
        connect(this,&WorkersManager::resetLogicsSignal,worker,&ThreadWorkerBase::resetLogic);
        connect(worker,&ThreadWorkerBase::sendHandlingOperation,worker,&ThreadWorkerBase::performHandlingOperation);
        connect(worker,&ThreadWorkerBase::sendErrorMessage,this,&WorkersManager::receiveAlarm);
        connect(this,&WorkersManager::sendOperationSignal,worker,&ThreadWorkerBase::receiveOperation,Qt::DirectConnection);
        connect(worker,&ThreadWorkerBase::sendMsgSignal,this,&WorkersManager::receiveMsgSignal,Qt::BlockingQueuedConnection);
        worker->setAlarmId(workers.count());
        qInfo("registerWorker :%s",worker->Name().toStdString().c_str());
        return true;
    }
    return false;
}

void WorkersManager::receiveAlarm(int sender_id,int level, QString error_message)
{
    qInfo("received alarm %d from sender %d in thread id :%d",level,sender_id,QThread::currentThreadId());
    showAlarm(sender_id,level,error_message);
    if(level == ErrorLevel::ErrorMustStop)
        emit stopWorkers();
}

bool WorkersManager::receiveMsgSignal(QString title, QString content)
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,title,content,QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::Yes){
        return true;
    }else{
        return false;
    }
}

void WorkersManager::showAlarm(const int sender_id, const int level, const QString error_message)
{
    qInfo("ShowAlarm sender_id: %d level: %d", sender_id, level);
    this->setShowAlarmDialog(true);
    this->workersState.insert(sender_id, level);
    this->workersError.insert(sender_id, error_message);
    if(level == ErrorLevel::TipNonblock||level == ErrorLevel::ErrorMustStop)
        return;
    //todo 显示错误窗口
}

void WorkersManager::startWorkers(int run_mode)
{
    qInfo("start all worker %d",workers.size());

    emit startWorkersSignal(run_mode);
}

void WorkersManager::stopWorkers(bool wait_finish)
{
    qInfo("stop all worker");
    emit stopWorkersSignal(wait_finish);
}

void WorkersManager::resetLogics()
{
    qInfo("reset all logics");
    emit resetLogicsSignal();
}


void WorkersManager::startWorker(QString name,int run_mode)
{
    if(name != current_name)
    {
        if("" != current_name)
            disconnect(this,&WorkersManager::startWorkerSignal,workers[current_name],&ThreadWorkerBase::startWork);
        connect(this,&WorkersManager::startWorkerSignal,workers[name],&ThreadWorkerBase::startWork);
        current_name = name;
    }
    emit startWorkerSignal(run_mode);
}

void WorkersManager::stopWorker(QString name, bool wait_finish)
{
    if(name != current_name)
    {
        if("" != current_name)
            disconnect(this,&WorkersManager::stopWorkerSignal,workers[current_name],&ThreadWorkerBase::stopWork);
        connect(this,&WorkersManager::stopWorkerSignal,workers[name],&ThreadWorkerBase::stopWork);
        current_name = name;
    }
    emit stopWorkerSignal(wait_finish);
}

void WorkersManager::resetLogic(QString name)
{
    if(name != current_name)
    {
        if("" != current_name)
            disconnect(this,&WorkersManager::resetLogicSignal,workers[current_name],&ThreadWorkerBase::resetLogic);
        connect(this,&WorkersManager::resetLogicSignal,workers[name],&ThreadWorkerBase::resetLogic);
        current_name = name;
    }
    emit resetLogicSignal();
}

QList<QString> WorkersManager::getWorkersNames()
{
    return workers.keys();
}

QString WorkersManager::getAlarmMessage(QString workerName)
{
    if (workers.contains(workerName)){
        int sender_id = workers[workerName]->getAlarmId();
        return workersError[sender_id];
    } else {
        return "";
    }
}

int WorkersManager::getAlarmState(QString workerName)
{
    if (workers.contains(workerName)){
        int sender_id = workers[workerName]->getAlarmId();
        return workersState[sender_id];
    } else {
        return 0;
    }
}

void WorkersManager::performAlarmOperation(QString workerName, int operation_type)
{
    qInfo("Workername: %s operationType: %d", workerName.toStdString().c_str(), operation_type);
    //ToDo: Send back to worker for handling the user response
    emit sendOperationSignal(workers[workerName]->getAlarmId(),operation_type);
    int sender_id = workers[workerName]->getAlarmId();
    workersState.remove(sender_id);
    workersError.remove(sender_id);
    this->setShowAlarmDialog(false);
}
