#include "sutcilent.h"
#include <commonutils.h>


SutCilent::SutCilent(Sut *sut_carrier,QString address, QObject *parent)
{
    socketClient = new SparrowClient(QUrl(address),  true);
    this->sut_carrier = sut_carrier;
    connect(socketClient, &SparrowClient::receiveMessage, this, &SutCilent::receiveMessage);
    connect(this, &SutCilent::sendMessageToServer, this->socketClient, &SparrowClient::sendMessage);
}

bool SutCilent::sendLensRequest(bool has_product, bool has_ng_sensor)
{
    this->has_product = has_product;
    this->has_ng_sennsor = has_ng_sensor;
    QJsonObject obj;
    obj.insert("cmd", "lensReq");
    QString jsonString = getStringFromJsonObject(obj);
    be_comuniting = true;
    emit sendMessageToServer(jsonString);
    int timeout = 60;
    while (timeout>0 && be_comuniting)
    {
        timeout--;
        //qInfo("Waiting LUT ....");
        QThread::msleep(1000);
    }
    if (timeout == 0) {
        qInfo("Lut Client send lens request timeout. current state = %d",be_comuniting);
        return false;
    }
    return true;
}

void SutCilent::receiveMessage(QString message)
{
    QJsonObject json = getJsonObjectFromString(message);
    qInfo("Lut Client receive message: %s", message.toStdString().c_str());
    QString event = json["event"].toString("");
    QString cmd = json["cmd"].toString("");
    QJsonObject obj;
    bool isValid = false;
    if (event == "sensorResp") {
        isValid = true;
        qInfo("AA Head need to pick lens");
        be_comuniting = true;
        if(has_product)
        {
            qInfo("pickarm take prooduct");
            obj.insert("cmd", "takePrcductReq");
        }
        else if(has_ng_sennsor)
        {
            qInfo("pickarm take ng sensor");
            obj.insert("cmd", "takeNgSensorReq");
        }
    } else if (event == "takePrcductResp"||event == "takeNgSensorResp") {
        qInfo("pickarm place sensor");
        obj.insert("cmd", "placeSensorReq");
    }else if (event == "placeSensorResp") {
        qInfo("pickarm leave from sut");
        obj.insert("cmd", "pickarmLeaveReq");
    }else if (event == "pickarmLeaveResp") {
        qInfo("LUT move to load lens position");
        be_comuniting = false;
        return;
    }

    if (cmd == "gripperOnReq") {
        isValid = true;
        qInfo("AA Gripper On Request");
        sut_carrier->openGripper();
        QThread::msleep(200);
    } else if (cmd == "gripperOffReq") {
        isValid = true;
        qInfo("AA Gripper Of Request");
        sut_carrier->closeGripper();
    }

    if (isValid) {
        QString jsonString = getStringFromJsonObject(obj);
        emit sendMessageToServer(jsonString);
    }
}
