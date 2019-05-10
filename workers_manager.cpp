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
        connect(worker,&ThreadWorkerBase::sendHandlingOperation,worker,&ThreadWorkerBase::performHandlingOperation);
        connect(worker,&ThreadWorkerBase::sendErrorMessage,this,&WorkersManager::receiveAlarm);
        connect(this,&WorkersManager::feedbackOperation,worker,&ThreadWorkerBase::receiveOperation,Qt::DirectConnection);
        connect(worker,&ThreadWorkerBase::sendMsgSignal,this,&WorkersManager::sendMessageTest,Qt::BlockingQueuedConnection);
        worker->setAlarmId(workers.count());
        qInfo("registerWorker :%s",worker->Name().toStdString().c_str());
        return true;
    }
    return false;
}

void WorkersManager::receiveAlarm(int sender_id,int level, QString error_message)
{
    qInfo("dad received alarm %d from sender %d in thread id :%d",level,sender_id,QThread::currentThreadId());
    if(level == ErrorLevel::ErrorMustStop)
        emit stopWorkers();

    //三色灯
    qInfo("..Check Point.......");
    showAlarm(sender_id,level,error_message);
}

bool WorkersManager::sendMessageTest(QString title, QString content)
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

void WorkersManager::startWorkers(bool reset,int run_mode)
{
    qInfo("start all worker %d",workers.size());
    emit startWorkersSignal(reset,run_mode);
}

void WorkersManager::stopWorkers(bool wait_finish)
{
    qInfo("stop all worker");
    emit stopWorkersSignal(wait_finish);
}


void WorkersManager::startWorker(QString name,bool reset,int run_mode)
{
    if(name != current_name)
    {
        if("" != current_name)
            disconnect(this,&WorkersManager::startWorkerSignal,workers[current_name],&ThreadWorkerBase::startWork);
        connect(this,&WorkersManager::startWorkerSignal,workers[name],&ThreadWorkerBase::startWork);
        current_name = name;
    }
    emit startWorkerSignal(reset,run_mode);
}

void WorkersManager::stopWorker(QString name, bool wait_finish)
{
    if(name != current_name)
    {
        if("" != current_name)
            disconnect(this,&WorkersManager::startWorkerSignal,workers[current_name],&ThreadWorkerBase::startWork);
        connect(this,&WorkersManager::startWorkerSignal,workers[name],&ThreadWorkerBase::startWork);
        current_name = name;
    }
    emit stopWorkerSignal(wait_finish);
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

void WorkersManager::sendOperation(QString workerName, int operation_type)
{
    qInfo("Workername: %s operationType: %d", workerName.toStdString().c_str(), operation_type);
    //ToDo: Send back to worker for handling the user response
    emit feedbackOperation(workers[workerName]->getAlarmId(),operation_type);
}
