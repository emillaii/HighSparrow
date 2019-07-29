#include "workers_manager.h"

#include <QMessageBox>
#include <QVariantMap>

WorkersManager::WorkersManager(QObject *parent):QObject (parent)
{
//    this->moveToThread(&work_thread);
//    this->work_thread.start();
}

void WorkersManager::Init(QList<TcpMessager*> senders,QList<TcpMessager*> receivers)
{
    send_messagers.clear();
    foreach (TcpMessager* temp_messager, senders) {
        send_messagers.insert(temp_messager->parameters.messagerName(),temp_messager);
    }
    qInfo("send_messagers size %d",send_messagers.size());
    receive_messagers.clear();
    foreach (TcpMessager* temp_messager, receivers) {
        receive_messagers.insert(temp_messager->parameters.messagerName(),temp_messager);
        connect(temp_messager,&TcpMessager::receiveTextMessage,this,&WorkersManager::tcpResp);
    }
    qInfo("receive_messagers size %d",receive_messagers.size());
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
        connect(this,&WorkersManager::feedbackOperation,worker,&ThreadWorkerBase::receiveOperation,Qt::DirectConnection);
        connect(worker,&ThreadWorkerBase::sendMsgSignal,this,&WorkersManager::sendMessageTest,Qt::BlockingQueuedConnection);
        connect(worker,&ThreadWorkerBase::sendModuleMessage,this,&WorkersManager::receiveModuleMessage,Qt::DirectConnection);
        worker->setAlarmId(workers.count());
        qInfo("registerWorker :%s",worker->Name().toStdString().c_str());
        return true;
    }
    return false;
}

void WorkersManager::receiveAlarm(int sender_id,int level, QString error_message)
{
    qInfo("received alarm %d from sender %d in thread id :%d",level,sender_id,QThread::currentThreadId());
    if(level == ErrorLevel::ErrorMustStop)
        emit stopWorkers();

    qInfo("show alarm %s",error_message.toStdString().c_str());

    this->setShowAlarmDialog(true);
    this->workersState.insert(sender_id, level);
    this->workersError.insert(sender_id, error_message);
    //三色灯
    //    showAlarm(sender_id,level,error_message);
}

void WorkersManager::tcpResp(QString message)
{
    qInfo("receive tcp module message %s", message.toStdString().c_str());
    QJsonObject message_object = TcpMessager::getJsonObjectFromString(message);
    if(message_object.contains("TargetModule"))
    {
        QString module_name = message_object["TargetModule"].toString();
        if(workers.contains(module_name))
        {
           workers[module_name]->receivceModuleMessage(message_object.toVariantMap());
        }
        else if(module_name == "LogicManager")
        {
            sendMessageToLogicManager(message_object.toVariantMap());
        }
        else {
            qInfo("module name error");
        }
    }
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

void WorkersManager::receiveModuleMessage(QVariantMap message)
{
    qInfo("receive module message %s to %s",message["Message"].toString().toStdString().c_str(),message["TargetModule"].toString().toStdString().c_str());
    if(message.contains("TargetModule"))
    {
        QString target_module = message["TargetModule"].toString();
        if(workers.contains(target_module))
        {
            workers[target_module]->receivceModuleMessage(message);
        }
        else
        {
            sendTcpMessage(message);
        }
    }
}

void WorkersManager::receiveMessageFromLogicManger(QVariantMap message)
{

}

void WorkersManager::showAlarm(const int sender_id, const int level, const QString error_message)
{
    qInfo("ShowAlarm sender_id: %d level: %d", sender_id, level);
    this->setShowAlarmDialog(true);
    this->workersState.insert(sender_id, level);
    this->workersError.insert(sender_id, error_message);
}

void WorkersManager::sendTcpMessage(QVariantMap message)
{
    foreach (TcpMessager* temp_messager, send_messagers) {
         temp_messager->sendMessage(TcpMessager::getStringFromQvariantMap(message));
     }
}

void WorkersManager::startWorkers(int run_mode)
{
    qInfo("start all worker %d",workers.size());

    // Reset logic if necessary
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否重置逻辑？(Yes: 重置逻辑并开始 No: 不重置逻辑并开始 Cancle: 取消)"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if(rb==QMessageBox::Yes)
    {
        resetLogics();
        qInfo("Reset logics before start workers signal.");
    }
    else if (rb==QMessageBox::No){
        qInfo("Not reset logics before start workers signal.");
    }
    else if (rb==QMessageBox::Cancel || rb==QMessageBox::Close) {
        qInfo("Cancle start workers.");
        return;
    }

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

void WorkersManager::sendOperation(QString workerName, int operation_type)
{
    qInfo("Workername: %s operationType: %d", workerName.toStdString().c_str(), operation_type);
    //ToDo: Send back to worker for handling the user response
    emit feedbackOperation(workers[workerName]->getAlarmId(),operation_type);
    int sender_id = workers[workerName]->getAlarmId();
    workersState.remove(sender_id);
    workersError.remove(sender_id);
    //this->setShowAlarmDialog(false);
}
