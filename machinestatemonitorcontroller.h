#ifndef MACHINESTATEMONITORCONTROLLER_H
#define MACHINESTATEMONITORCONTROLLER_H

#include <QObject>
#include "machinestatemonitor.h"
class MachineStateMonitorController : public QObject
{
    Q_OBJECT
public:
    explicit MachineStateMonitorController(BaseModuleManager * baseModuleManager, QObject *parent = nullptr);
    MotorState getMotorState(QString name, int timeout = 500);
private:
    BaseModuleManager * baseModuleManager;
    MachineStateMonitor * machineStateMonitor;
    QWaitCondition waitingResponse;
    QMutex mutex;
    MotorState responseMotorState;
signals:

public slots:
    void receiveMotorState(MotorState);
};

#endif // MACHINESTATEMONITORCONTROLLER_H
