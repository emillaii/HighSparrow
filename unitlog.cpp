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
         //QUrl sfrlog_endpoint = QString("http://192.168.0.252:5044");
         QString unitLogEndpoint = serverAddress;
         unitLogEndpoint.append(":").append(QString::number(UNIT_LOG_PORT));
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
     qInfo("Sfr log: %s", doc.toJson().toStdString().c_str());
     QString sfrLogEndpoint = serverAddress;
     sfrLogEndpoint.append(":").append(QString::number(5045));
     qInfo(sfrLogEndpoint.toStdString().c_str());
     QNetworkRequest request(sfrLogEndpoint);
     request.setHeader(QNetworkRequest::ContentTypeHeader,
                       QVariant(QString("application/json")));
     if (nam) {
         nam->post(request, doc.toJson());
         qInfo("Push sfr data to ELK success: %s", uuid.toStdString().c_str());
     } else {
         qInfo("Post sfr data to ELK fail");
     }
     return true;
}

bool Unitlog::saveToCSV(QString uuid)
{
    QVariantMap output;
    if (unit_log_list.contains(uuid))
    {
        QJsonObject json = QJsonObject::fromVariantMap(unit_log_list[uuid]);
        QJsonDocument doc(json);
        qInfo("Doc: %s", doc.toJson().toStdString().c_str());

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
