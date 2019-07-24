#ifndef WORKER_BASE_H
#define WORKER_BASE_H

#include "utils/errorcode.h"

#include "propertybase.h"

#include <QObject>
#include <QVariantMap>
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
    OnllyLeftAA = 5,
    OnlyRightAA = 6,
    MachineTest = 7,
    DispensTest = 8
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
    Q_PROPERTY(QString Name READ Name WRITE setName NOTIFY NameChanged)

    QString Name() const;
    void setAlarmId(int id);
    int getAlarmId();
    void sendAlarmMessage(int error_level,QString error_message);
    int waitMessageReturn(bool &interruput);
    bool waitResponseMessage(bool &is_run,QString target_message);
    virtual void receivceModuleMessage(QVariantMap message);
    virtual PropertyBase* getModuleState() = 0;

    Q_INVOKABLE void performHandling(int cmd);
    bool  waitPerformHandling();
    Q_INVOKABLE void sendMessageToModule(QString module_name,QString message);
    Q_INVOKABLE QString getModuleMessage();
signals:
    void sendErrorMessage(int alarm_id,int error_level,QString error_message);
    void sendHandlingOperation(int cmd);
    void NameChanged(QString Name);
    bool sendMsgSignal(QString,QString);
    void sendModuleMessage(QVariantMap message);

public slots:
    virtual void startWork(int run_mode) = 0;
    virtual void stopWork(bool wait_finish = true) = 0;
    virtual void resetLogic() = 0;
    virtual void performHandlingOperation(int cmd) = 0;
    void receiveOperation(const int sender_id,const int operation_type);
public:
    void setName(QString Name);
//    virtual void receiveModuleMessage(QVariantMap messsage) = 0;
private:
    QThread work_thread;
    QString m_Name;
    int operation_type = 0;
    bool message_returned = false;
    int alarm_id = 0;
protected:
    bool is_handling = false;
    bool is_error = false;
    bool handling_finish = false;
    QMutex message_mutex;
    QVariantMap message;
};

#endif // WORKER_BASE_H
