#include "basemodulemanager.h"
#include "machinestatemonitor.h"
#include "utils/commonutils.h"
#include <QUuid>

#define CMD_GET_MOTOR_STATE_REQ    "getMotorStateReq"
#define CMD_GET_MOTOR_STATE_RESP   "getMotorStateResp"
#define CMD_GET_IO_STATE_REQ       "getIOStateReq"
#define CMD_GET_IO_STATE_RESP      "getIOStateResp"
#define CMD_TEST                   "test"

MachineStateMonitor::MachineStateMonitor(BaseModuleManager* baseModuleManager, QObject *parent) : QObject(parent)
{
    this->baseModuleManager = baseModuleManager;

    server = new SparrowQServer(20001);
    if (baseModuleManager->ServerMode() == 0) {
        client = new SparrowClient(QUrl("ws://192.168.0.251:20001"));
    }
    else {
        //client = new SparrowClient(QUrl("ws://192.168.0.250:20001"));
        qInfo("connect to 20001");
        client = new SparrowClient(QUrl("ws://localhost:20001"), false);
    }
    connect(server, &SparrowQServer::receiveRequestMessage, this, &MachineStateMonitor::receiveRequestMessage);
    connect(this, &MachineStateMonitor::sendMessageToNextMachine, client, &SparrowClient::sendMessage);
}

void MachineStateMonitor::getMotorState(QString name)
{
    MotorState motorState;
    QString uuid = QUuid::createUuid().toString().mid(1,32).toUpper();
    XtMotor * motor = baseModuleManager->GetMotorByName(name);
    if (motor) {
        motorState.currentPos = motor->GetFeedbackPos();
        motorState.targetPos = motor->GetCurrentTragetPos();
        motorState.isValid = true;
    } else {  //Try to ask next tcp server
        QJsonObject obj;
        obj.insert("cmd", CMD_GET_MOTOR_STATE_REQ);
        obj.insert("name", name);
        obj.insert("req_id", uuid);
        QString jsonString = getStringFromJsonObject(obj);
        emit sendMessageToNextMachine(jsonString);
    }
}

void MachineStateMonitor::getIOState(QString name, STATE_TYPE type)
{
    IOState ioState;
    QString uuid = QUuid::createUuid().toString().mid(1,32).toUpper();
}

void MachineStateMonitor::receiveRequestMessage(QString message, QString client_ip)
{
    MotorState motorState;
    qInfo("Request from client_ip: %s message: %s", client_ip.toStdString().c_str(), message.toStdString().c_str());
    QJsonObject json = getJsonObjectFromString(message);
    QString cmd = json["cmd"].toString("");
    QString name = json["name"].toString("");
    if (cmd == CMD_GET_MOTOR_STATE_REQ) {
        QString req_id = json["req_id"].toString("");
        XtMotor * motor = baseModuleManager->GetMotorByName(name);
        if (motor) {
            motorState.currentPos = motor->GetFeedbackPos();
            motorState.targetPos = motor->GetCurrentTragetPos();
            motorState.isValid = true;
        }
        QJsonObject obj;
        obj.insert("cmd", CMD_GET_MOTOR_STATE_RESP);
        obj.insert("name", name);
        obj.insert("resp_id", req_id);
        obj.insert("currentPos", motorState.currentPos);
        obj.insert("targetPos", motorState.targetPos);
        obj.insert("isHome", motorState.isHome);
        obj.insert("isValid", motorState.isValid);

        QString jsonString = getStringFromJsonObject(obj);
        emit sendMessageToNextMachine(jsonString);
        return;
    } else if (cmd == CMD_GET_MOTOR_STATE_RESP) {
        qInfo("Receive get motor state response");
        motorState.currentPos = json["currentPos"].toDouble(0);
        motorState.targetPos = json["targetPos"].toDouble(0);
        motorState.isValid = json["isValid"].toBool(false);
        emit receiveMotorState(motorState);
    }
}
