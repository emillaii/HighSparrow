#ifndef WORKERS_MANAGER_H
#define WORKERS_MANAGER_H

#include "alarmmessageshower.h"
#include "tcpmessager.h"
#include "thread_worker_base.h"
#include "workersmanagerparameter.h"
#include <qmap.h>

class WorkersManager:public QObject
{
    Q_OBJECT
public:
    WorkersManager(QObject *parent = nullptr);
    void Init(QList<TcpMessager*> senders,QList<TcpMessager*> receivers);
    bool registerWorker(ThreadWorkerBase* worker);
signals:
    void startWorkersSignal(int run_mode);
    void stopWorkersSignal(bool wait_finish = true);
    void resetLogicsSignal();
    void startWorkerSignal(int run_mode);
    void stopWorkerSignal(bool wait_finish = true);
    void resetLogicSignal();
    void paramsChanged(bool ShowAlarmDialog);

    void showAlarm();

    void sendMessageToLogicManager(QVariantMap message);
    void sendMessageToDevicesManager(QVariantMap message);
public slots:
    void tcpResp(QString message);
    void feedbackOperation(const QString module_name,const int alarm_id,const QString operation);
    void lightOperation();
    bool sendMessageTest(QString title,QString content);
    void receiveModuleMessage(QVariantMap message);

public:
    Q_INVOKABLE void startAllWorkers(int run_mode = 0);
    Q_INVOKABLE void stopAllWorkers(bool wait_finish);
    void startWorkers(int run_mode = 0);
    Q_INVOKABLE void stopWorkers(bool wait_finish = true);
    Q_INVOKABLE void resetLogics();
    Q_INVOKABLE void startWorker(QString name,int run_mode = 0);
    Q_INVOKABLE void stopWorker(QString name,bool wait_finish = true);
    Q_INVOKABLE void resetLogic(QString name);
    Q_INVOKABLE QList<QString> getWorkersNames();
private:
    void sendTcpMessage(QVariantMap message);
    void getRunParameters(QString target_module);
    void showAlarmMessage(QVariantMap message);
private:
    QMap<QString,ThreadWorkerBase*> workers;
    QString logic_manager_name = "";
    QString current_name = "";
    QMap<QString,TcpMessager*> receive_messagers;
    QMap<QString,TcpMessager*> send_messagers;
    QThread work_thread;
    QMap<QString,QVariantMap> run_parameter;
    QString local_station_number = "0";
    QMutex fileLock;
public:
    WorkersManagerParameter parameters;
    AlarmMessageShower alarm_shower;
};

#endif // WORKERS_MANAGER_H
