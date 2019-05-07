#include "lutclient.h"
#include <QJsonObject>
#include <QThread>
#include "commonutils.h"
LutClient::LutClient(AAHeadModule * aaHead, QString address, QObject *parent) : QObject(parent)
{
    socketClient = new SparrowClient(QUrl(address),  true);
    this->aaHead = aaHead;
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
        if(has_ng_lens)
        {
            qInfo("aa unpick ng lens");
            obj.insert("cmd", "unpickNgLensReq");
        }
        else
        {
            qInfo("aa pick lens");
            obj.insert("cmd", "pickLensReq");
        }
    } else if (event == "unpickNgLensResp") {
        qInfo("aa pick lens");
        obj.insert("cmd", "pickLensReq");
    }else if (event == "picklensResp") {
        qInfo("LUT move to load lens position");
        qInfo("perform pickedlens pr");
        obj.insert("cmd", "prReq");
    }else if (event == "prResp") {
        isValid = true;
        obj.insert("cmd", "lutLeaveReq");
        double prOffsetT = json["prOffsetT"].toDouble(0);
        double prOffsetX = json["prOffsetX"].toDouble(0);
        double prOffsetY = json["prOffsetY"].toDouble(0);
        qInfo("PR Result...offsetX %f offsetY %f offsetT %f", prOffsetX, prOffsetY, prOffsetT);
        aaHead->stepMove_XYC_Sync(-prOffsetX, -prOffsetY, -prOffsetT);
        this->state = LutClientState::WAITING_LUT_LEAVE_EVENT;
    } else if (event == "lutLeaveResp") {
        qInfo("LUT move to load lens position");
        this->state = LutClientState::LUT_CLIENT_IDLE;
        return;
    }
    if (cmd == "gripperOnReq") {
        isValid = true;
        qInfo("AA Gripper On Request");
        aaHead->openGripper();
        QThread::msleep(200);
    } else if (cmd == "gripperOffReq") {
        isValid = true;
        qInfo("AA Gripper Of Request");
        aaHead->closeGripper();
    }
    if (isValid) {
        QString jsonString = getStringFromJsonObject(obj);
        emit sendMessageToServer(jsonString);
    }
}

bool LutClient::sendLensRequest(bool has_ng_lens,bool is_wait)
{
    qInfo("sendLensRequest has_ng_lens %d",has_ng_lens);
    this->has_ng_lens = has_ng_lens;
    QJsonObject obj;
    obj.insert("cmd", "lensReq");
    QString jsonString = getStringFromJsonObject(obj);
    this->state = LutClientState::WAITING_LENS_PICK_EVENT;
    qInfo("ready to sendMessageToServer");
    emit sendMessageToServer(jsonString);
    if(!is_wait)return true;
    int timeout = 60;
    while (timeout>0 && this->state != LutClientState::LUT_CLIENT_IDLE)
    {
        timeout--;
        //qInfo("Waiting LUT ....");
        QThread::msleep(1000);
    }
    if (timeout == 0) {
        qInfo("Lut Client send lens request timeout. current state = %d", this->state);
        return false;
    }
    return true;
}
