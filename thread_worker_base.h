#ifndef WORKER_BASE_H
#define WORKER_BASE_H

#include "utils/errorcode.h"

#include "propertybase.h"

#include <QObject>
#include <QVariantMap>
#include <qmutex.h>
#include <qthread.h>

#define OK_OPERATION u8"确定"
#define CONTINUE_OPERATION u8"继续"
#define RETRY_OPERATION u8"重试"
#define REJECT_OPERATION u8"人工抛料"
#define ACCEPT_OPERATION u8"有料"
#define AUTOREJECT_OPERATION u8"自动抛料"
#define SKIP_OPERATION u8"跳过"
#define NEXT_OPERATION u8"下一个"
#define BLIND_OPERATION u8"盲取/放"
#define SKIPTRAY_OPERATION u8"跳过整盘"
#define LAST_OPERATION u8"最后一盘"
#define NEXTMAGAZINE_OPERATION u8"下一盒"

#define CONTINUE_RETRY_OPERATION u8"继续|重试"
#define CONTINUE_SKIP_OPERATION u8"继续|跳过"
#define CONTINUE_REJECT_OPERATION u8"继续|人工抛料"
#define CONTINUE_AUTOREJECT_OPERATION u8"继续|自动抛料"
#define CONTINUE_ACCEPT_OPERATION u8"继续|有料"
#define CONTINUE_NEXT_OPERATION u8"继续|下一个"
#define CONTINUE_LAST_OPERATION u8"继续|最后一盘"

#define CONTINUE_RETRY_REJECT_OPERATION u8"继续|重试|人工抛料"
#define CONTINUE_RETRY_NEXT_OPERATION u8"继续|重试|下一个"
#define CONTINUE_RETRY_NEXTMAGAZINE_OPERATION u8"继续|重试|下一盒"
#define CONTINUE_BLIND_SKIPTRAY_OPERATION u8"继续|盲取/放|跳过整盘"

#define CONTINUE_RETRY_AUTOREJECT_REJECT_OPERATION u8"继续|重试|自动抛料|人工抛料"

enum AlarmOperation
{
    Continue = 0,
    GiveUp = 1,
    Retray = 2
};
enum RunMode
{
    Normal = 0,
    NoMaterial = 1,
    VibrationTest = 2,
    AAFlowChartTest = 3,
    UNLOAD_ALL_LENS = 4
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
    void setName(QString Name);
    int sendAlarmMessage(QString error_tips,QString error_message,int error_level = 1);
    QString waitMessageReturn(bool &interruput,int alarm_id);
    bool waitResponseMessage(bool &is_run,QString target_message);
    void receivceModuleMessageBase(QVariantMap message);
    virtual void receivceModuleMessage(QVariantMap message) = 0;
    bool waitVisionResponseMessage(QString cameraName);
    virtual PropertyBase* getModuleState() = 0;
    virtual QMap<QString,PropertyBase*> getModuleParameter() = 0;
    virtual void setModuleParameter(QMap<QString, PropertyBase*> parameters) = 0;
    Q_INVOKABLE void performHandling(int cmd,QVariant param = 0);
    bool waitPerformHandling();
    Q_INVOKABLE void sendMessageToModule(QString module_name,QString module_message,QJsonValue param = 0);
    Q_INVOKABLE QString getModuleMessage();
    Q_INVOKABLE bool loadJsonConfig(QString file_name);
    Q_INVOKABLE void saveJsonConfig(QString file_name);
    QVariantMap inquirRunParameters(int out_time = 1000);
private:
    int getAlarmId();

signals:
    void sendHandlingOperation(int cmd,QVariant param);
    void NameChanged(QString Name);
    bool sendMsgSignal(QString,QString);
    void sendModuleMessage(QVariantMap module_message);

public slots:
    void clearAlarm();
    virtual void startWork(int run_mode) = 0;
    virtual void stopWork(bool wait_finish = true) = 0;
    virtual void resetLogic() = 0;
    virtual void performHandlingOperation(int cmd,QVariant param) = 0;
private:
    QThread work_thread;
    QString m_Name;
    QMap<int,QString> choosed_operations;
    bool message_returned = false;
    int alarm_id = 0;
protected:
    bool is_handling = false;
    bool is_error = false;
    bool handling_finish = false;
    QMutex message_mutex;
    QVariantMap module_message;
    QMap<QString, QString> vision_operations;
};

#endif // WORKER_BASE_H
