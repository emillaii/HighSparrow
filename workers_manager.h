#ifndef WORKERS_MANAGER_H
#define WORKERS_MANAGER_H

#include "thread_worker_base.h"
#include <qmap.h>

class WorkersManager:public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ShowAlarmDialog READ ShowAlarmDialog WRITE setShowAlarmDialog NOTIFY paramsChanged)
public:
    WorkersManager(QObject *parent = nullptr);
    bool registerWorker(ThreadWorkerBase* worker);
signals:
    void startWorkersSignal(bool reset = false,int run_mode = 0);
    void stopWorkersSignal(bool wait_finish = true);
    void startWorkerSignal(bool reset = false,int run_mode = 0);
    void stopWorkerSignal(bool wait_finish = true);
    void paramsChanged(bool ShowAlarmDialog);
    void feedbackOperation(const int sender_id,const int operation_type);
public slots:
void receiveAlarm(int sender_id,int level, QString error_message);
void setShowAlarmDialog(bool ShowAlarmDialog)
{
    qInfo("setShowAlarm: %d", ShowAlarmDialog);
    if (m_ShowAlarmDialog == ShowAlarmDialog)
        return;

    m_ShowAlarmDialog = ShowAlarmDialog;
    emit paramsChanged(m_ShowAlarmDialog);
}
bool sendMessageTest(QString title,QString content);
public:
    Q_INVOKABLE void startWorkers(bool reset = false,int run_mode = 0);
    Q_INVOKABLE void stopWorkers(bool wait_finish = true);
    Q_INVOKABLE void startWorker(QString name,bool reset = false,int run_mode = 0);
    Q_INVOKABLE void stopWorker(QString name,bool wait_finish = true);
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
public:
//    WorkersManagerParameter parameters;
private:
    QMap<QString,ThreadWorkerBase*> workers;
    QString current_name = "";
    bool m_ShowAlarmDialog = false;
    QMap<int, QString> workersError;
    QMap<int, int> workersState;
};

#endif // WORKERS_MANAGER_H
