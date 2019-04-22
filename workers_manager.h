#ifndef WORKERS_MANAGER_H
#define WORKERS_MANAGER_H

#include "thread_worker_base.h"

#include <qmap.h>

class WorkersManager:public QObject
{
    Q_OBJECT
public:
    WorkersManager(QObject *parent = nullptr);

    bool registerWorker(ThreadWorkerBase* worker);
signals:
    void startWorkersSignal(bool reset = false,int run_mode = 0);
    void stopWorkersSignal(bool wait_finish = true);
    void startWorkerSignal(bool reset = false,int run_mode = 0);
    void stopWorkerSignal(bool wait_finish = true);
public:
    Q_INVOKABLE void startWorkers(bool reset = false,int run_mode = 0);
    Q_INVOKABLE void stopWorkers(bool wait_finish = true);
    Q_INVOKABLE void startWorker(QString name,bool reset = false,int run_mode = 0);
    Q_INVOKABLE void stopWorker(QString name,bool wait_finish = true);
    Q_INVOKABLE QList<QString> getWorkersNames();
private:
    QMap<QString,ThreadWorkerBase*> workers;
    QString current_name = "";
};

#endif // WORKERS_MANAGER_H
