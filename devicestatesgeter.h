#ifndef MOTORSTATESGETER_H
#define MOTORSTATESGETER_H

#include <QObject>
#include <QMutex>

class DeviceStatesGeter : public QObject
{
    Q_OBJECT
public:
    struct motorState
    {
        bool result = false;
        bool has_alarm = false;
        double current_position = 0;
        double target_position = 0;
        bool isHome = false;
        bool isEnabled = false;
        bool alarmState = false;
    };
    struct IoState
    {
        bool result = false;
        bool current_state = false;
    };
    explicit DeviceStatesGeter(QObject *parent = nullptr);
public:
    motorState getMotorState(QString motor_name);
    IoState getInputIoState(QString input_io_name);
    IoState getOutputIoState(QString output_io_name);
    QJsonArray getOutputIoState(QJsonArray io_name_array);
    void toggleOutputIoState(QString output_io_name, int input_state = -1);
signals:
    QString sendGetDeviceState(QString motor_name);
private:
    QMutex mutex;
public slots:
};

#endif // MOTORSTATESGETER_H
