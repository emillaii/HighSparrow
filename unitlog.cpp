#include "unitlog.h"
#include <QUuid>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include "config.h"
#include "commonutils.h"
Unitlog::Unitlog(QObject * parent) : QObject(parent)
{
    nam = new QNetworkAccessManager();
}

Unitlog::~Unitlog() {}

void Unitlog::setServerAddress(QString address)
{
    this->serverAddress = address;
}

QString Unitlog::createUnit() {
    QString uuid = QUuid::createUuid().toString().mid(1,32).toUpper();
    QVariantMap map;
    std::vector<QString> nameList;
    this->unit_log_list.insert(uuid, map);
    this->unit_log_test_name_list.insert(uuid, nameList);
    qInfo("UUID : %s is created", uuid.toStdString().c_str());
    return uuid;
}

bool Unitlog::pushDataToUnit(QString uuid, QString test_name, QVariantMap map)
{
    QVariantMap testMap;
    if (unit_log_list.contains(uuid))
    {
        int index = 1;
        for (QString s : unit_log_test_name_list[uuid]) {
            QString name = test_name;
            name.append("_").append(QString::number(index));
            if (s == name) {
                index++;
            }
        }
        QString name = test_name.append("_").append(QString::number(index));
        unit_log_test_name_list[uuid].push_back(name);
        testMap.insert(name, map);
        unit_log_list[uuid].insert(test_name, map);
        unit_log_test_name_list[uuid].push_back(test_name.append("_").append(QString::number(index)));
        qInfo("Push data to unit: %s", uuid.toStdString().c_str());
        return true;
    }
    qInfo("Cannot find the unit: %f", uuid.toStdString().c_str());
    return false;
}

bool Unitlog::postDataToELK(QString uuid)
{
    if (unit_log_list.contains(uuid))
    {
         QJsonObject json = QJsonObject::fromVariantMap(unit_log_list[uuid]);
         QJsonDocument doc(json);
         QUrl unitLogEndpoint = QString("http://192.168.0.250:5044");
         // QString unitLogEndpoint = serverAddress;
         //unitLogEndpoint.append(":").append(QString::number(UNIT_LOG_PORT));
         QNetworkRequest request(unitLogEndpoint);
         request.setHeader(QNetworkRequest::ContentTypeHeader,
                           QVariant(QString("application/json")));
         if (nam) {
             nam->post(request, doc.toJson());
             qInfo("Push data to ELK success: %s", uuid.toStdString().c_str());
         } else {
             qInfo("Post data to ELK fail");
         }
    } else {
        qInfo("Cannot find the unit: %s", uuid.toStdString().c_str());
    }
    return true;
}

bool Unitlog::postSfrDataToELK(QString uuid, QVariantMap data)
{
     data.insert("device_id", uuid);
     QJsonObject json = QJsonObject::fromVariantMap(data);
     QJsonDocument doc(json);
     //qInfo("Sfr log: %s", doc.toJson().toStdString().c_str());
     QString sfrLogEndpoint = QString("http://192.168.0.250:5045"); //= serverAddress;
     //sfrLogEndpoint.append(":").append(QString::number(5045));
     qInfo(sfrLogEndpoint.toStdString().c_str());
     QNetworkRequest request(sfrLogEndpoint);
     request.setHeader(QNetworkRequest::ContentTypeHeader,
                       QVariant(QString("application/json")));
     if (nam)
     {
         nam->post(request, doc.toJson());
         qInfo("Push sfr data to ELK success: %s", uuid.toStdString().c_str());
//     }
//     else
//     {
         qInfo("save json to a txt");
         QString filename = "";
         filename.append(getMTFLogDir())
                 .append("_")
                 .append(data["SensorID"].toString())
                 .append("_")
                 .append(getCurrentTimeString())
                 .append("_")
                 .append(uuid)
                 .append("_sfrlog.csv");
         QFile file(filename);
         QString data_content = "";
         data_content.append("ll_peak,");
         data_content.append("ul_peak,");
         data_content.append("lr_peak,");
         data_content.append("ur_peak,");
         data_content.append("cc_peak,");
         data_content.append("dev,");
         data_content.append("xTilt,");
         data_content.append("yTilt,");
         data_content.append(" \r\n");
         data_content.append(data["ll_peak"].toString());
         data_content.append(",");
         data_content.append(data["ul_peak"].toString());
         data_content.append(",");
         data_content.append(data["lr_peak"].toString());
         data_content.append(",");
         data_content.append(data["ur_peak"].toString());
         data_content.append(",");
         data_content.append(data["cc_peak"].toString());
         data_content.append(",");
         data_content.append(data["dev"].toString());
         data_content.append(",");
         data_content.append(data["xTilt"].toString());
         data_content.append(",");
         data_content.append(data["yTilt"].toString());
         data_content.append(" \r\n");
         data_content.append(getCSVString("CC",data["CC"].toMap()));
         data_content.append(getCSVString("LL",data["LL"].toMap()));
         data_content.append(getCSVString("LR",data["LR"].toMap()));
         data_content.append(getCSVString("UL",data["UL"].toMap()));
         data_content.append(getCSVString("UR",data["UR"].toMap()));
         file.open(QIODevice::WriteOnly | QIODevice::Text);
         file.write(data_content.toStdString().c_str());
         file.close();
     }
     return true;
}

QString Unitlog::getCSVString(QString data_name,QVariantMap map)
{
    QString data_string = "";
    data_string.append(" \r\n");
    if(data_name == "CC")
    {
        data_string.append("DFOV");
        data_string.append(" \r\n");
        foreach (QString temp_name, map.keys()) {
           QVariantMap temp_data = map[temp_name].toMap();
           data_string.append(temp_data["dfov"].toString());
           data_string.append(",");
        }
        data_string.append(" \r\n");
    }
    data_string.append(data_name);
    data_string.append(" \r\n");
    foreach (QString temp_name, map.keys()) {
       QVariantMap temp_data = map[temp_name].toMap();
       data_string.append(temp_data["t_sfr"].toString());
       data_string.append(",");
    }
    data_string.append(" \r\n");
    foreach (QString temp_name, map.keys()) {
       QVariantMap temp_data = map[temp_name].toMap();
       data_string.append(temp_data["b_sfr"].toString());
       data_string.append(",");
    }
    data_string.append(" \r\n");
    foreach (QString temp_name, map.keys()) {
       QVariantMap temp_data = map[temp_name].toMap();
       data_string.append(temp_data["l_sfr"].toString());
       data_string.append(",");
    }
    data_string.append(" \r\n");
    foreach (QString temp_name, map.keys()) {
       QVariantMap temp_data = map[temp_name].toMap();
       data_string.append(temp_data["r_sfr"].toString());
       data_string.append(",");
    }
    data_string.append(" \r\n");
    foreach (QString temp_name, map.keys()) {
       QVariantMap temp_data = map[temp_name].toMap();
       data_string.append(temp_data["pz"].toString());
       data_string.append(",");
    }
    data_string.append(" \r\n");
    qInfo("data_string: %s", data_string.toStdString().c_str());
    return data_string;
}

bool Unitlog::saveToCSV(QString uuid)
{
    QVariantMap output;
    if (unit_log_list.contains(uuid))
    {
        QJsonObject json = QJsonObject::fromVariantMap(unit_log_list[uuid]);
        QJsonDocument doc(json);
//        qInfo("Doc: %s", doc.toJson().toStdString().c_str());

        QString filename = "";
        filename.append(getMTFLogDir())
                        .append(getCurrentTimeString())
                        .append(".txt");
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(doc.toJson());
        file.close();
    }
    return false;
}
