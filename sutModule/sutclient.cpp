#include "sutModule/sutclient.h"
#include <utils/commonutils.h>
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

bool SutClient::sendSensorRequest(bool &is_run,int material_type)
{
    this->curren_type = material_type;
    qInfo("curren_type %d",curren_type);
    QJsonObject obj;
    obj.insert("cmd", "sensorReq");
    QString jsonString = getStringFromJsonObject(obj);
    be_comuniting = true;
    emit sendMessageToServer(jsonString);
    while (be_comuniting)
    {
        if(!is_run)
            return false;
        QThread::msleep(10);
    }
    return !be_comuniting;
}

void SutClient::receiveMessage(QString message)
{
    QJsonObject json = getJsonObjectFromString(message);
    qInfo("Sut Client receive message: %s", message.toStdString().c_str());
    QString event = json["event"].toString("");
    QString cmd = json["cmd"].toString("");
    QJsonObject obj;
    bool isValid = false;
    if (event == "sensorResp") {
        isValid = true;
        qInfo("AA Head need to pick sensor");
        be_comuniting = true;
        if(curren_type == MaterialType::GOOD_PRODUCT)
        {
            qInfo("pickarm unload prooduct");
            obj.insert("cmd", "unloadProductReq");
        }
        else if(curren_type == MaterialType::NG_PRODUCT)
        {
            qInfo("pickarm unload ng prooduct");
            obj.insert("cmd", "unloadNgProductReq");
        }
        else if(curren_type == MaterialType::NG_SENSOR)
        {
            qInfo("pickarm take ng sensor");
            obj.insert("cmd", "unloadNgSensorReq");
        }
        else
        {
            qInfo("pickarm place sensor");
            obj.insert("cmd", "loadSensorReq");
        }
    } else if (event == "unloadNgProductResp"||event == "unloadProductResp"||event == "unloadNgSensorResp") {
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
