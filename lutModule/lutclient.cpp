#include "lutModule/lutclient.h"
#include <QJsonObject>
#include <QThread>
#include "utils/commonutils.h"
LutClient::LutClient(AAHeadModule * aaHead, QString address, QObject *parent) : QObject(parent)
{
    socketClient = new SparrowClient(QUrl(address),  true);
    this->aaHead = aaHead;
    connect(socketClient, &SparrowClient::receiveMessage, this, &LutClient::receiveMessage);
    connect(this, &LutClient::sendMessageToServer, this->socketClient, &SparrowClient::sendMessage);
}

bool LutClient::waitLensRespond(bool &is_run)
{
    while (this->state != LutClientState::LUT_CLIENT_IDLE)
    {
        if(!is_run)
            return false;
        //qInfo("Waiting LUT ....");
        QThread::msleep(10);
    }
    return this->state == LutClientState::LUT_CLIENT_IDLE;
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
        isValid = true;
        qInfo("aa pick lens");
        obj.insert("cmd", "pickLensReq");
    }else if (event == "pickLensResp") {
        isValid = true;
        aaHead->moveToMushroomPosition();
        qInfo("LUT move to load lens position");
        qInfo("perform pickedLens pr");
        obj.insert("cmd", "prReq");
    }else if (event == "prResp") {
        isValid = true;
        obj.insert("cmd", "lutLeaveReq");
        double prOffsetT = json["prOffsetT"].toDouble(0);
        double prOffsetX = json["prOffsetX"].toDouble(0);
        double prOffsetY = json["prOffsetY"].toDouble(0);
        qInfo("PR Result...offsetX %f offsetY %f offsetT %f", prOffsetX, prOffsetY, prOffsetT);
        aaHead->receiveLensFromLut(prOffsetX, prOffsetY, prOffsetT);
        this->state = LutClientState::WAITING_LUT_LEAVE_EVENT;
    } else if (event == "lutLeaveResp") {
        qInfo("LUT move to load lens position");
        this->state = LutClientState::LUT_CLIENT_IDLE;
        return;
    } else if (event == "moveToUnloadPosResp") {
        qInfo("LUT move to unload position");
        this->state = LutClientState::LUT_CLIENT_IDLE;
    } else if (event == "moveToAA1UplookPosResp") {
        qInfo("LUT move to AA1 uplook position");
        this->state = LutClientState::LUT_CLIENT_IDLE;
    } else if (event == "moveToAA2UplookPosResp") {
        qInfo("LUT move to AA2 uplook position");
        this->state = LutClientState::LUT_CLIENT_IDLE;
    } else if (event == "tooluplookPRResp") {
        qInfo("LUT tooluplook PR Resp");
        double prOffsetT = json["prOffsetT"].toDouble(0);
        double prOffsetX = json["prOffsetX"].toDouble(0);
        double prOffsetY = json["prOffsetY"].toDouble(0);
        tempPrResult.X = prOffsetX; tempPrResult.Y = prOffsetY; tempPrResult.Theta = prOffsetT;
        qInfo("PR Result...offsetX %f offsetY %f offsetT %f", prOffsetX, prOffsetY, prOffsetT);
        this->state = LutClientState::LUT_CLIENT_IDLE;
    }
    if (cmd == "gripperOnReq") {
        qInfo("AA Gripper On Request");
        aaHead->openGripper();
        QThread::msleep(200);
    } else if (cmd == "gripperOffReq") {
        qInfo("AA Gripper Of Request");
        aaHead->closeGripper();
    }
    if (isValid) {
        QString jsonString = getStringFromJsonObject(obj);
        emit sendMessageToServer(jsonString);
    }
}

bool LutClient::sendLensRequest(bool has_ng_lens)
{
    qInfo("sendLensRequest has_ng_lens %d",has_ng_lens);
    this->has_ng_lens = has_ng_lens;
    QJsonObject obj;
    obj.insert("cmd", "lensReq");
    QString jsonString = getStringFromJsonObject(obj);
    this->state = LutClientState::WAITING_LENS_PICK_EVENT;
    qInfo("ready to sendMessageToServer");
    emit sendMessageToServer(jsonString);
    return true;
}

//Used for manual operation or calibration, not for auto run
bool LutClient::sendLUTMoveToPos(int type)
{
    qInfo("LUT Client request lut move to unload position");
    QJsonObject obj;
    if (type == 0)
        obj.insert("cmd", "moveToUnloadPosReq");
    else if (type == 1)
        obj.insert("cmd", "moveToAA1UplookPosReq");
    else if (type == 2)
        obj.insert("cmd", "moveToAA2UplookPosReq");
    QString jsonString = getStringFromJsonObject(obj);
    this->state = LutClientState::WAITING_LENS_PICK_EVENT;
    emit sendMessageToServer(jsonString);
    int timeout = 60;
    while (timeout>0 && this->state != LutClientState::LUT_CLIENT_IDLE)
    {
        timeout--;
        qInfo("Waiting LUT ....");
        QThread::msleep(1000);
    }
    if (timeout == 0) {
        qInfo("Lut Client send move to unload position timeout. current state = %d", this->state);
        return false;
    }
    return true;
}

bool LutClient::requestToolUpPRResult(PrOffset &offset)
{
    tempPrResult.X = 0; tempPrResult.Y = 0; tempPrResult.Theta = 0;
    qInfo("LUT Client request Tool Uplook PRResult");
    QJsonObject obj;
    obj.insert("cmd", "tooluplookPRReq");
    QString jsonString = getStringFromJsonObject(obj);
    this->state = LutClientState::WAITING_LENS_PICK_EVENT;
    emit sendMessageToServer(jsonString);
    int timeout = 60;
    while (timeout>0 && this->state != LutClientState::LUT_CLIENT_IDLE)
    {
        timeout--;
        qInfo("Waiting PR Result ....");
        QThread::msleep(1000);
    }
    if (timeout == 0) {
        qInfo("Lut Client send move to unload position timeout. current state = %d", this->state);
        return false;
    }
    offset = tempPrResult;
    return true;
}

bool LutClient::isLutClientConnected()
{
    return this->socketClient->isConnected();
}
