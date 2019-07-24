#ifndef WORKERS_MANAGER_H
#define WORKERS_MANAGER_H

#include "tcpmessager.h"
#include "thread_worker_base.h"
#include "workersmanagerparameter.h"
#include <qmap.h>

class WorkersManager:public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ShowAlarmDialog READ ShowAlarmDialog WRITE setShowAlarmDialog NOTIFY paramsChanged)
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
    void feedbackOperation(const int sender_id,const int operation_type);

    void sendMessageToLogicManager(QVariantMap message);
public slots:
    void receiveAlarm(int sender_id,int level, QString error_message);
    void tcpResp(QString message);
    void setShowAlarmDialog(bool ShowAlarmDialog)
    {
        qInfo("setShowAlarm: %d", ShowAlarmDialog);
        if (m_ShowAlarmDialog == ShowAlarmDialog)
            return;

        m_ShowAlarmDialog = ShowAlarmDialog;
        emit paramsChanged(m_ShowAlarmDialog);
    }
    bool sendMessageTest(QString title,QString content);
    void receiveModuleMessage(QVariantMap message);

    void receiveMessageFromLogicManger(QVariantMap message);
public:
    Q_INVOKABLE void startWorkers(int run_mode = 0);
    Q_INVOKABLE void stopWorkers(bool wait_finish = true);
    Q_INVOKABLE void resetLogics();
    Q_INVOKABLE void startWorker(QString name,int run_mode = 0);
    Q_INVOKABLE void stopWorker(QString name,bool wait_finish = true);
    Q_INVOKABLE void resetLogic(QString name);
    Q_INVOKABLE QList<QString> getWorkersNames();
    Q_INVOKABLE QString getAlarmMessage(QString workerName);
    Q_INVOKABLE int getAlarmState(QString workerName);
    Q_INVOKABLE void sendOperation(QString workerName, int operation_type);
    bool ShowAlarmDialog() const
    {
        return m_ShowAlarmDialog;
    }
private:
    void showAlarm(const int sender_id,const int level, const QString error_message);
    void sendTcpMessage(QVariantMap message);
private:
    QMap<QString,ThreadWorkerBase*> workers;
    QString current_name = "";
    bool m_ShowAlarmDialog = false;
    QMap<int, QString> workersError;
    QMap<int, int> workersState;
    QMap<QString,TcpMessager*> receive_messagers;
    QMap<QString,TcpMessager*> send_messagers;
    QThread work_thread;
public:
    WorkersManagerParameter parameters;
};

#endif // WORKERS_MANAGER_H
