#ifndef WORKERS_MANAGER_H
#define WORKERS_MANAGER_H

#include "thread_worker_base.h"
#include <qmap.h>

class WorkersManager:public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ShowAlarmDialog READ ShowAlarmDialog WRITE setShowAlarmDialog)
public:
    WorkersManager(QObject *parent = nullptr);
    bool registerWorker(ThreadWorkerBase* worker);
signals:
    void startWorkersSignal(int run_mode);
    void stopWorkersSignal(bool wait_finish = true);
    void resetLogicsSignal();
    void startWorkerSignal(int run_mode);
    void stopWorkerSignal(bool wait_finish = true);
    void resetLogicSignal();
    void sendOperationSignal(const int sender_id,const int operation_type);

public slots:
    void receiveAlarm(int sender_id,int level, QString error_message);
    bool receiveMsgSignal(QString title,QString content);

    void setShowAlarmDialog(bool ShowAlarmDialog)
    {
        m_ShowAlarmDialog = ShowAlarmDialog;
    }

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
    Q_INVOKABLE void performAlarmOperation(QString workerName, int operation_type);

bool ShowAlarmDialog() const
{
    return m_ShowAlarmDialog;
}

private:
    void showAlarm(const int sender_id,const int level, const QString error_message);

private:
    QMap<QString,ThreadWorkerBase*> workers;
    QString current_name = "";
    QMap<int, QString> workersError;
    QMap<int, int> workersState;
    bool m_ShowAlarmDialog = false;
};

#endif // WORKERS_MANAGER_H
