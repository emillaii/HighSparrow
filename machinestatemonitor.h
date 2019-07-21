#ifndef MACHINESTATEMONITOR_H
#define MACHINESTATEMONITOR_H

#include <QObject>
#include "network/sparrowqserver.h"
#include "network/sparrowqclient.h"
#include <QWaitCondition>
#include <QMutex>
#include <QThread>

class BaseModuleManager;

struct MotorState
{
public:
    MotorState() { currentPos = 0; targetPos = 0; isHome = false; isValid = false; }
    double currentPos;
    double targetPos;
    bool isHome;
    bool isValid;
};

struct IOState
{
public:
    IOState() { isOn = false; isValid = false; }
    bool isOn;
    bool isValid;
};

enum STATE_TYPE{
    QUERY,
    CMD_MOTION,
    CMD_IO
};

class MachineStateMonitor : public QObject
{
    Q_OBJECT
public:
    explicit MachineStateMonitor(BaseModuleManager* baseModuleManager, QObject *parent = nullptr);
    void getMotorState(QString name);
    void getIOState(QString name, STATE_TYPE type);
private:
    BaseModuleManager *baseModuleManager;
    SparrowQServer *server;
    SparrowClient *client;
signals:
    void sendMessageToNextMachine(QString);
    void sendMessageToClient(QString destAddress, QString message);

    void receiveMotorState(MotorState state);
public slots:
    void receiveRequestMessage(QString message, QString client_ip);
};

#endif // MACHINESTATEMONITOR_H
