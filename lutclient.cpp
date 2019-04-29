#include "lutclient.h"
#include <QJsonObject>
#include <QThread>
#include "commonutils.h"
LutClient::LutClient(QString address, QObject *parent) : QObject(parent)
{
    socketClient = new SparrowClient(QUrl(address),  true);
    connect(socketClient, &SparrowClient::receiveMessage, this, &LutClient::receiveMessage);
    connect(this, &LutClient::sendMessageToServer, this->socketClient, &SparrowClient::sendMessage);
}

void LutClient::receiveMessage(QString message)
{
    QJsonObject json = getJsonObjectFromString(message);
    qInfo("Lut Client receive message: %s", message.toStdString().c_str());
    QString event = json["event"].toString("");
    QString cmd = json["cmd"].toString("");
    QJsonObject obj;
    bool isValid = false;
    if (event == "lensResp") {
        isValid = true;
        qInfo("AA Head need to pick lens");
        this->state = LutClientState::WAITING_LENS_PR_EVENT;
        obj.insert("cmd", "prReq");
    } else if (event == "prResp") {
        isValid = true;
        qInfo("PR Result...");
        obj.insert("cmd", "lutLeaveReq");
        this->state = LutClientState::WAITING_LUT_LEAVE_EVENT;
    } else if (event == "lutLeaveResp") {
        qInfo("LUT move to load lens position");
        this->state = LutClientState::LUT_CLIENT_IDLE;
        return;
    }
    if (cmd == "gripperOnReq") {
        isValid = true;
        qInfo("AA Gripper On Request");
        emit this->triggerAAGripper(true);
        QThread::msleep(200);
    } else if (cmd == "gripperOffReq") {
        isValid = true;
        qInfo("AA Gripper Of Request");
        emit this->triggerAAGripper(false);
        QThread::msleep(200);
    }
    if (isValid) {
        QString jsonString = getStringFromJsonObject(obj);
        emit sendMessageToServer(jsonString);
    }
}

void LutClient::sendLensRequest()
{
    QJsonObject obj;
    obj.insert("cmd", "lensReq");
    QString jsonString = getStringFromJsonObject(obj);
    this->state = LutClientState::WAITING_LENS_PICK_EVENT;
    emit sendMessageToServer(jsonString);
    int timeout = 60;
    while (timeout>0 && this->state != LutClientState::LUT_CLIENT_IDLE)
    {
        timeout--;
        //qInfo("Waiting LUT ....");
        QThread::msleep(1000);
    }
}
