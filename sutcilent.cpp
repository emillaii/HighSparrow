#include "sutcilent.h"
#include <commonutils.h>
#include <qthread.h>


SutCilent::SutCilent(QString address, QObject *parent)
{
    socketClient = new SparrowClient(QUrl(address),  true);
    connect(socketClient, &SparrowClient::receiveMessage, this, &SutCilent::receiveMessage);
    connect(this, &SutCilent::sendMessageToServer, this->socketClient, &SparrowClient::sendMessage);
}

void SutCilent::Init(SutGripper *sut_gripper)
{
    this->sut_gripper = sut_gripper;
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
        sut_gripper->openGripper();
        QThread::msleep(200);
    } else if (cmd == "gripperOffReq") {
        isValid = true;
        qInfo("AA Gripper Of Request");
        sut_gripper->closeGripper();
    }

    if (isValid) {
        QString jsonString = getStringFromJsonObject(obj);
        emit sendMessageToServer(jsonString);
    }
}

void SutGripper::Init(XtVacuum *vacuum, XtCylinder *cylinder)
{
    this->vacuum = vacuum;
    this->cylinder = cylinder;
}

bool SutGripper::openGripper()
{
    bool result = vacuum->Set(true,false);
    result &= cylinder->Set(true);
    result &= vacuum->Wait(true);
    return result;
}

bool SutGripper::closeGripper()
{
    bool result = vacuum->Set(false,false);
    result &= cylinder->Set(false);
    result &= vacuum->Wait(false);
    return result;
}
