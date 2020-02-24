#ifndef WORKER_BASE_H
#define WORKER_BASE_H

#include "Utils/errorcode.h"

#include <QObject>
#include <qmutex.h>
#include <qthread.h>
enum AlarmOperation
{
    Continue = 0,
    GiveUp = 1,
    Retray = 2
};
enum RunMode
{
    Normal = 0,
    ChangeType = 1,
    NoMaterial = 2,
    VibrationTest = 3,
    AAFlowChartTest = 4,
    OnlyLeftAA = 5,
    OnlyRightAA =6,
    ChangeLensTrayHandle = 7
};
enum FinishedType
{
    Success,
    Alarm
};
enum OperationType
{
    Ok,
    Cancel
};

class ThreadWorkerBase : public QObject,public ErrorBase
{
    Q_OBJECT
public:
    explicit ThreadWorkerBase(QString name = "UnnamingWorker",QObject *parent = nullptr);
    ~ThreadWorkerBase();
//    Q_PROPERTY(QString Name READ Name WRITE setName NOTIFY NameChanged)

    QString Name() const;
    void setAlarmId(int id);
    int getAlarmId();
    void sendAlarmMessage(int error_level,QString error_message);
    int waitMessageReturn(bool &interruput);
    void performHandling(int cmd);
signals:
    void sendErrorMessage(int alarm_id,int error_level,QString error_message);
    void sendHandlingOperation(int cmd);
    void NameChanged(QString Name);
    bool sendMsgSignal(QString,QString);

public slots:
    virtual void startWork(int run_mode) = 0;
    virtual void stopWork(bool wait_finish = true) = 0;
    virtual void resetLogic() = 0;
    virtual void performHandlingOperation(int cmd) = 0;
    void receiveOperation(const int sender_id,const int operation_type);
public:
    void setName(QString Name);
private:
    QThread work_thread;
    QString m_Name;
    int operation_type = 0;
    bool message_returned = false;
    int alarm_id = 0;
    QMutex message_mutex;
};

#endif // WORKER_BASE_H
