#include "sutclient.h"
#include <commonutils.h>
#include <qthread.h>


SutClient::SutClient(QString address, QObject *parent)
{
    socketClient = new SparrowClient(QUrl(address),  true);
    connect(socketClient, &SparrowClient::receiveMessage, this, &SutClient::receiveMessage);
    connect(this, &SutClient::sendMessageToServer, this->socketClient, &SparrowClient::sendMessage);
}

void SutClient::Init(XtVacuum *sut_vacuum)
{
    this->sut_vacuum = sut_vacuum;
}

bool SutClient::sendLensRequest(bool has_product, bool has_ng_sensor)
{
    this->has_product = has_product;
    this->has_ng_sennsor = has_ng_sensor;
    QJsonObject obj;
    obj.insert("cmd", "sensorReq");
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
        qInfo("Sut Client send lens request timeout. current state = %d",be_comuniting);
        return false;
    }
    return true;
}

void SutClient::receiveMessage(QString message)
{
    QJsonObject json = getJsonObjectFromString(message);
    qInfo("Lut Client receive message: %s", message.toStdString().c_str());
    QString event = json["event"].toString("");
    QString cmd = json["cmd"].toString("");
    QJsonObject obj;
    bool isValid = false;
    if (event == "sensorResp") {
        isValid = true;
        qInfo("AA Head need to pick sensor");
        be_comuniting = true;
        if(has_product)
        {
            qInfo("pickarm unload prooduct");
            obj.insert("cmd", "unloadProductReq");
        }
        else if(has_ng_sennsor)
        {
            qInfo("pickarm take ng sensor");
            obj.insert("cmd", "unloadNgSensorReq");
        }
        else
        {
            qInfo("pickarm place sensor");
            obj.insert("cmd", "loadSensorReq");
        }
    } else if (event == "unloadProductResp"||event == "unloadNgSensorResp") {
        isValid = true;
        qInfo("pickarm place sensor");
        obj.insert("cmd", "loadSensorReq");
    }else if (event == "loadSensorResp") {
        qInfo("finish communicate");
        be_comuniting = false;
        return;
    }

    if (cmd == "vacuumOnReq") {
        isValid = true;
        qInfo("Sut Vacuum On Request");
        sut_vacuum->Set(0);
        QThread::msleep(200);
    } else if (cmd == "vacuumOffReq") {
        isValid = true;
        qInfo("Sut Vacuum Off Request");
        sut_vacuum->Set(1);
    }

    if (isValid) {
        QString jsonString = getStringFromJsonObject(obj);
        emit sendMessageToServer(jsonString);
    }
}
