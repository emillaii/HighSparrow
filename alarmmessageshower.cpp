#include "alarmmessageshower.h"

#include <qthread.h>

AlarmMessageShower::AlarmMessageShower(QObject *parent) : QObject(parent)
{

}

int AlarmMessageShower::getAlarmId()
{
    return current_message.alarm_id;
}

QString AlarmMessageShower::getAlarmColor()
{
    return current_message.alarm_color;
}

QString AlarmMessageShower::getModuleName()
{
    return current_message.module_name;
}

QString AlarmMessageShower::getMessageContent()
{
    return current_message.message_content;
}

bool AlarmMessageShower::getFeedBack1()
{
    return current_message.feed_back1;
}

QString AlarmMessageShower::getChooseTip1()
{
    return current_message.choose_tip1;
}

bool AlarmMessageShower::getFeedBack2()
{
    return current_message.feed_back2;
}

QString AlarmMessageShower::getChooseTip2()
{
    return current_message.choose_tip2;
}
bool AlarmMessageShower::getFeedBack3()
{
    return current_message.feed_back3;
}

QString AlarmMessageShower::getChooseTip3()
{
    return current_message.choose_tip3;
}
bool AlarmMessageShower::getFeedBack4()
{
    return current_message.feed_back4;
}

QString AlarmMessageShower::getChooseTip4()
{
    return current_message.choose_tip4;
}

bool AlarmMessageShower::startShow(int time_out)
{
    qInfo("tryLock");
    return show_mutex.tryLock(time_out);
}
void AlarmMessageShower::finihShow()
{
    qInfo("unlock");
    show_mutex.unlock();
}

void AlarmMessageShower::closeLight()
{
    emit lightOperation();
}

void AlarmMessageShower::sendOperation(const QString module_name,const int alarm_id,const QString operation)
{
    emit feedbackOperation(module_name,alarm_id,operation);
    qInfo("sendOperation %s(id %d)to %s",operation.toStdString().c_str(),alarm_id,module_name.toStdString().c_str());
}

void AlarmMessageShower::appendAlarmMessage()
{
    qInfo("showAlarm %d",QThread::currentThreadId());
    emit showAlarm();
}
