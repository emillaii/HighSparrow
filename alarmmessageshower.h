#ifndef ALARMMESSAGESHOWER_H
#define ALARMMESSAGESHOWER_H

#include <QObject>
#include <QVariantMap>
#include <qmutex.h>

class AlarmMessageShower : public QObject
{
    Q_OBJECT
public:
    explicit AlarmMessageShower(QObject *parent = nullptr);
    Q_INVOKABLE int getAlarmId();
    Q_INVOKABLE QString getAlarmColor();
    Q_INVOKABLE QString getModuleName();
    Q_INVOKABLE QString getMessageContent();
    Q_INVOKABLE bool getFeedBack1();
    Q_INVOKABLE QString getChooseTip1();
    Q_INVOKABLE bool getFeedBack2();
    Q_INVOKABLE QString getChooseTip2();
    Q_INVOKABLE bool getFeedBack3();
    Q_INVOKABLE QString getChooseTip3();
    Q_INVOKABLE bool getFeedBack4();
    Q_INVOKABLE QString getChooseTip4();
    bool startShow(int time_out = 5000);
    Q_INVOKABLE void finihShow();
    Q_INVOKABLE void closeLight();
    Q_INVOKABLE void sendOperation(const QString module_name,const int alarm_id,const QString operation);
signals:
    void showAlarm();
    void feedbackOperation(const QString module_name,const int alarm_id,const QString operation);
    void lightOperation();
public slots:
    void appendAlarmMessage();
public:
    struct alarmMessage
    {
        int alarm_id;
        QString module_name;
        QString alarm_color;
        QString message_content;
        bool feed_back1;
        QString choose_tip1;
        bool feed_back2;
        QString choose_tip2;
        bool feed_back3;
        QString choose_tip3;
        bool feed_back4;
        QString choose_tip4;
    };
    alarmMessage current_message;
private:
    QList<int> current_alarms;
    QMutex show_mutex;
};

#endif // ALARMMESSAGESHOWER_H
