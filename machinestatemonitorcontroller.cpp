#include "machinestatemonitorcontroller.h"

MachineStateMonitorController::MachineStateMonitorController(BaseModuleManager * baseModuleManager, QObject *parent) : QObject(parent)
{
    this->baseModuleManager = baseModuleManager;
    machineStateMonitor = new MachineStateMonitor(baseModuleManager);
    connect(machineStateMonitor, &MachineStateMonitor::receiveMotorState, this, &MachineStateMonitorController::receiveMotorState, Qt::DirectConnection);
}

MotorState MachineStateMonitorController::getMotorState(QString name, int timeout)
{
    MotorState state;
    mutex.lock();
    responseMotorState.isValid = false;  //reset the response first
    machineStateMonitor->getMotorState(name);
    bool ret = waitingResponse.wait(&mutex, timeout);
    mutex.unlock();
    if (ret) {
        qInfo("Waiting response success");
        return responseMotorState;
    } else {
        qInfo("Waiting response fail");
        return state;
    }
}

void MachineStateMonitorController::receiveMotorState(MotorState motorState)
{
    qInfo("receiveMotorState: %f", motorState.targetPos);
    responseMotorState = motorState;
    waitingResponse.wakeAll();
}

