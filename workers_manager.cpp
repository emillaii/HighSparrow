#include "workers_manager.h"

#include <QMessageBox>
#include <QVariantMap>
#include "config.h"
#include "utils/commonutils.h"
#include <QFile>
WorkersManager::WorkersManager(QObject *parent):QObject (parent)
{
    this->moveToThread(&work_thread);
    this->work_thread.start();
}

void WorkersManager::Init(QList<TcpMessager*> senders,QList<TcpMessager*> receivers)
{
    send_messagers.clear();
    foreach (TcpMessager* temp_messager, senders) {
        send_messagers.insert(temp_messager->parameters.messagerName(),temp_messager);
    }
    qDebug("send_messagers size %d",send_messagers.size());
    receive_messagers.clear();
    foreach (TcpMessager* temp_messager, receivers) {
        receive_messagers.insert(temp_messager->parameters.messagerName(),temp_messager);
        connect(temp_messager,&TcpMessager::receiveTextMessage,this,&WorkersManager::tcpResp);
    }
    connect(this,&WorkersManager::showAlarm,&alarm_shower,&AlarmMessageShower::appendAlarmMessage);
    connect(&alarm_shower,&AlarmMessageShower::feedbackOperation,this,&WorkersManager::feedbackOperation);
    connect(&alarm_shower,&AlarmMessageShower::lightOperation,this,&WorkersManager::lightOperation);
    qDebug("receive_messagers size %d",receive_messagers.size());
}

bool WorkersManager::registerWorker(ThreadWorkerBase* worker)
{
    if(!workers.contains(worker->Name()))
    {
        workers.insert(worker->Name(),worker);
        connect(this,&WorkersManager::startWorkersSignal,worker,&ThreadWorkerBase::startWork);
        connect(this,&WorkersManager::stopWorkersSignal,worker,&ThreadWorkerBase::stopWork,Qt::DirectConnection);
        connect(this,&WorkersManager::resetLogicsSignal,worker,&ThreadWorkerBase::resetLogic);
//        connect(worker,&ThreadWorkerBase::sendHandlingOperation,worker,&ThreadWorkerBase::performHandlingOperation);
        connect(worker,&ThreadWorkerBase::sendMsgSignal,this,&WorkersManager::sendMessageTest,Qt::BlockingQueuedConnection);
        connect(worker,&ThreadWorkerBase::sendModuleMessage,this,&WorkersManager::receiveModuleMessage);
        qInfo("registerWorker :%s",worker->Name().toStdString().c_str());
        return true;
    }
    return false;
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
           workers[module_name]->receivceModuleMessageBase(message_object.toVariantMap());
        }
        else if(module_name == logic_manager_name||module_name == "LogicManager")
        {
            emit sendMessageToLogicManager(message_object.toVariantMap());
        }
//        else if(module_name == "LogicManager")
//        {
//            emit sendMessageToLogicManager(message_object.toVariantMap());
//        }
        else if(module_name == "WorksManager")
        {
            QString message_content = message_object["Message"].toString();
            if(message_content == "inquirRunParameters")
            {
               emit sendMessageToDevicesManager(message_object.toVariantMap());
            }
            else if(message_content == "runParametersResp")
            {
                QVariantMap message_map = message_object.toVariantMap();
                QString station_number =  message_map["StationNumber"].toString();
                qInfo("station_number %s",station_number.toStdString().c_str());
                if(!run_parameter.contains(station_number))
                {
                    qInfo("station_number %s",station_number.toStdString().c_str());
                    run_parameter.insert(station_number,message_map);
                }

                QString target_module = message_map["OriginModule"].toString();
                qInfo("OriginModule:%s",target_module.toStdString().c_str());
                if(workers.contains(target_module)&&run_parameter.size() >= 2)
                {
                    getRunParameters(target_module);
                }
                else
                {
                    qCritical("module name error!");
                }
            }
            else if(message_content == "AlamMessage")
            {
                showAlarmMessage(message_object.toVariantMap());
            }
            else {
                qInfo("message error!");
            }
        }
        else if(module_name == "WorkersManagerStart")
        {
            startWorkers(message_object["RunMode"].toInt());
        }
        else if(module_name == "WorkersManagerReset")
        {
            resetLogics();
        }
        else if(module_name == "WorkersManagerStop")
        {
            stopWorkers(message_object["waitFinish"].toBool());
        }
        else {
            qInfo("module name error");
        }
    }
}

void WorkersManager::feedbackOperation(const QString module_name,const int alarm_id, const QString operation)
{
    QVariantMap message;
    message.insert("TargetModule",module_name);
    message.insert("Message","feedbackOperation");
    message.insert("AlarmId",alarm_id);
    message.insert("Operation",operation);
    message.insert("OriginModule","AlarmModule");
    QString alarmMessage = getCurrentDateString().append("-")
                           .append(getCurrentTimeString()).append(",")
                           .append("Remit,")
                           .append(module_name).append(",")
                           .append(message["AlarmId"].toString()).append(",")
                           .append(message["Operation"].toString())
                           .append("\n");
    QString filename;
    fileLock.lock();
    filename.append(getAlarmLogDir()).append("alarm.log");
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    file.write(alarmMessage.toStdString().c_str());
    file.close();
    fileLock.unlock();
    receiveModuleMessage(message);
}

void WorkersManager::lightOperation()
{
    QVariantMap message;
    message.insert("Message", "CloseAlarmLight");
    sendMessageToLogicManager(message);
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
    qInfo("receive module message %s to %s",TcpMessager::getStringFromQvariantMap(message).toStdString().c_str(),message["TargetModule"].toString().toStdString().c_str());
    if(message.contains("TargetModule"))
    {
        QString target_module = message["TargetModule"].toString();
        if(workers.contains(target_module))
        {
            workers[target_module]->receivceModuleMessageBase(message);
        }
        else if(target_module == logic_manager_name)
        {
            emit sendMessageToLogicManager(message);
        }
//        else if(target_module == "LogicManager")
//        {
//            sendTcpMessage(message);
//        }
        else if(target_module == "WorksManager")
        {
            QString message_content = message["Message"].toString();
            if(message_content == "SendLogicManagerName")
            {
                logic_manager_name = message["LogicManagerName"].toString();
            }
            else if(message_content == "inquirRunParameters")
            {
                QString target_module = message["OriginModule"].toString();
                if(workers.contains(target_module)&&run_parameter.size() >= 2)
                {
                    qInfo("target_module %s skip inquirRunParameters",target_module.toStdString().c_str());
                    getRunParameters(target_module);
                }
                else
                {
                    emit sendMessageToDevicesManager(message);
                    sendTcpMessage(message);
                }
            }
            else if(message_content == "runParametersResp")
            {
                QString station_number =  message["StationNumber"].toString();
                 qInfo("station_number %s",station_number.toStdString().c_str());
                if(!run_parameter.contains(station_number))
                {
                    run_parameter.insert(station_number,message);
                    local_station_number = station_number;
                }

                QString target_module = message["OriginModule"].toString();
                if(workers.contains(target_module)&&run_parameter.size() >= 2)
                    getRunParameters(target_module);
                else
                    sendTcpMessage(message);
            }
            else if(message_content == "AlamMessage"&&parameters.showAlarm())
            {
                showAlarmMessage(message);
            }
            else if(message_content == "StopAllWorkers")
            {
                stopAllWorkers(true);
            }
            else
            {
                sendTcpMessage(message);
            }
        }
        else
        {
            sendTcpMessage(message);
        }
    }
    else
    {
        qInfo("has no TargetModule");
    }
}

void WorkersManager::sendTcpMessage(QVariantMap message)
{
    foreach (TcpMessager* temp_messager, send_messagers) {
         temp_messager->sendMessage(TcpMessager::getStringFromQvariantMap(message));
     }
}

void WorkersManager::getRunParameters(QString target_module)
{
    qInfo("getRunParameters target_module %s",target_module.toStdString().c_str());
    QVariantMap temp_message;
    QVariantMap station_disable;
    QVariantMap station_task;
    foreach (QString temp_station, run_parameter.keys()) {
        foreach (QString temp_key, run_parameter[temp_station].keys()) {
            if(temp_key == "DisableStation")
                station_disable.insert(run_parameter[temp_station]["StationNumber"].toString(),run_parameter[temp_station][temp_key]);
            else if(temp_key == "StationTask")
                station_task.insert(run_parameter[temp_station]["StationNumber"].toString(),run_parameter[temp_station][temp_key]);
            else
                temp_message.insert(temp_key,run_parameter[temp_station][temp_key]);
        }
    }
    temp_message["StationNumber"] = local_station_number;
    temp_message["OriginModule"] = target_module;
    temp_message["DisableStation"] = station_disable;
    temp_message["StationTask"] = station_task;
    workers[target_module]->receivceModuleMessageBase(temp_message);
}

void WorkersManager::showAlarmMessage(QVariantMap message)
{
    int error_level  = message["ErrorLevel"].toInt();
    if(error_level == ErrorLevel::ErrorMustStop)
        stopAllWorkers(true);
    if(!alarm_shower.startShow())
    {
        qInfo("alarm mutex fail");
        return;
    }
    QString error_tips = message["ErrorTips"].toString();
    if(error_tips.contains('|'))
    {
        QStringList tips = error_tips.split('|');
        if(tips.size() == 2)
        {
            alarm_shower.current_message.choose_tip1 = tips[0];
            alarm_shower.current_message.feed_back1 = true;
            alarm_shower.current_message.choose_tip2 = tips[1];
            alarm_shower.current_message.feed_back2 = true;
            alarm_shower.current_message.feed_back3 = false;
            alarm_shower.current_message.feed_back4 = false;
        }
        else if(tips.size() == 3)
        {
            alarm_shower.current_message.choose_tip1 = tips[0];
            alarm_shower.current_message.feed_back1 = true;
            alarm_shower.current_message.choose_tip2 = tips[1];
            alarm_shower.current_message.feed_back2 = true;
            alarm_shower.current_message.choose_tip3 = tips[2];
            alarm_shower.current_message.feed_back3 = true;
            alarm_shower.current_message.feed_back4 = false;
        }
        else
        {
            alarm_shower.current_message.choose_tip1 = tips[0];
            alarm_shower.current_message.feed_back1 = true;
            alarm_shower.current_message.choose_tip2 = tips[1];
            alarm_shower.current_message.feed_back2 = true;
            alarm_shower.current_message.choose_tip3 = tips[2];
            alarm_shower.current_message.feed_back3 = true;
            alarm_shower.current_message.choose_tip4 = tips[3];
            alarm_shower.current_message.feed_back4 = true;
        }
    }
    else
    {
        alarm_shower.current_message.choose_tip1 = error_tips;
        alarm_shower.current_message.feed_back1 = true;
        alarm_shower.current_message.feed_back2 = false;
        alarm_shower.current_message.feed_back3 = false;
        alarm_shower.current_message.feed_back4 = false;
    }
    if(error_level == 1)
        alarm_shower.current_message.alarm_color = "orange";
    else if(error_level == 2)
        alarm_shower.current_message.alarm_color = "red";
    else
        alarm_shower.current_message.alarm_color = "lightGreen";
    alarm_shower.current_message.alarm_id = message["AlarmId"].toInt();
    alarm_shower.current_message.module_name = message["OriginModule"].toString();
    alarm_shower.current_message.message_content = message["ErrorMessage"].toString();

    qInfo("Alarm: %s: %s", message["OriginModule"].toString().toStdString().c_str(), message["ErrorMessage"].toString().toStdString().c_str());
    QString alarmMessage = getCurrentDateString().append("-")
                           .append(getCurrentTimeString()).append(",")
                           .append("Assert,")
                           .append(message["OriginModule"].toString()).append(",")
                           .append(message["AlarmId"].toString()).append(",")
                           .append(message["ErrorLevel"].toString()).append(",")
                           .append(message["ErrorMessage"].toString())
                           .append("\n");
    QString filename;
    fileLock.lock();
    filename.append(getAlarmLogDir()).append("alarm.log");
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    file.write(alarmMessage.toStdString().c_str());
    file.close();
    fileLock.unlock();
    emit showAlarm();
}


void WorkersManager::startAllWorkers(int run_mode)
{
    qInfo("start all worker %d %d",workers.size(), run_mode);

    // Reset logic if necessary
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否重置逻辑？(Yes: 重置逻辑并开始 No: 不重置逻辑并开始 Cancle: 取消)"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if(rb==QMessageBox::Yes)
    {
        QVariantMap message;
        message.insert("TargetModule","WorkersManagerReset");
        sendTcpMessage(message);

        resetLogics();
        QThread::msleep(100);
        qInfo("Reset logics before start workers signal.");
    }
    else if (rb==QMessageBox::No){
        qInfo("Not reset logics before start workers signal.");
    }
    else if (rb==QMessageBox::Cancel || rb==QMessageBox::Close) {
        qInfo("Cancle start workers.");
        return;
    }

    startWorkers(run_mode);
    QVariantMap message;
    message.insert("TargetModule","WorkersManagerStart");
    message.insert("RunMode", run_mode);
    sendTcpMessage(message);
}

void WorkersManager::stopAllWorkers(bool wait_finish)
{
    stopWorkers(wait_finish);
    QVariantMap message;
    message.insert("TargetModule","WorkersManagerStop");
    message.insert("waitFinish",wait_finish);
    sendTcpMessage(message);
}

void WorkersManager::startWorkers(int run_mode)
{
    run_parameter.clear();
    qInfo("startSelfWorkers run_mode: %d", run_mode);
    emit startWorkersSignal(run_mode);
}

void WorkersManager::stopWorkers(bool wait_finish)
{
    run_parameter.clear();
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
    run_parameter.clear();
    qInfo("Start worker: %s run_mode: %d current_name: %s", name.toStdString().c_str(), run_mode, current_name.toStdString().c_str());
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
