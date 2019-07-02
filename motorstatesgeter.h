#ifndef MOTORSTATESGETER_H
#define MOTORSTATESGETER_H

#include <QObject>

class MotorStatesGeter : public QObject
{
    Q_OBJECT
public:
    struct motorState
    {
        bool result = false;
        bool has_alarm = false;
        double current_position = 0;
        double target_position = 0;
    };
    explicit MotorStatesGeter(QObject *parent = nullptr);
public:
    motorState getMotorState(QString motor_name);
signals:
    QString sendGetMotorState(QString motor_name);
public slots:
};

#endif // MOTORSTATESGETER_H
