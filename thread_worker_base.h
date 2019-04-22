#ifndef WORKER_BASE_H
#define WORKER_BASE_H

#include "errorcode.h"

#include <QObject>
#include <qmutex.h>
#include <qthread.h>
struct ErroeMessage
{
    ErrorLevel level;
    QString message;
};
enum RunMode
{
    Normal = 1<<0,//1
    ChangeType = 1<<1,//2
    NoMaterial = 1<<2,//4
};

class ThreadWorkerBase : public QObject,public ErrorBase
{
    Q_OBJECT
public:
    explicit ThreadWorkerBase(QString name = "UnnamingWorker",QObject *parent = nullptr);
    Q_PROPERTY(QString Name READ Name WRITE setName NOTIFY NameChanged)

    QString Name() const;
    void setAlarmId(int id);
    void sendAlarmMessage(int error_level,QString error_message);
    int waitMessageReturn(bool &interruput);

signals:
    void sendErrorMessage(int alarm_id,int error_level,QString error_message);

    void NameChanged(QString Name);

public slots:
    virtual void startWork(bool reset_logic = false,int run_mode = 0) = 0;
    virtual void stopWork(bool wait_finish = true) = 0;
    void receiveMessage(const int message_id,const int result_message);
    void setName(QString Name);
private:
    QThread work_thread;
    QString m_Name;
    int message_result;
    bool message_returned;
    int alarm_id;
    QMutex message_mutex;
};

#endif // WORKER_BASE_H
