#include "Utils/unitlog.h"
#include <QUuid>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <qfileinfo.h>
#include "Utils/config.h"
#include "Utils/commonutils.h"
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

void Unitlog::clearHeaders()
{
    qInfo("clearHeaders");
    temp_file_name = "";
    headers_length = 0;
    headers.clear();
}

bool Unitlog::postDataToELK(QString uuid)
{
    if (unit_log_list.contains(uuid))
    {
         QJsonObject json = QJsonObject::fromVariantMap(unit_log_list[uuid]);
         json["timestamp"] = QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddThh:mm:ss.zzzZ");
         QJsonDocument doc(json);
         QString unitLogEndpoint = QString("http://35.194.139.163:8000/unitlog_richtek/unitlog");
         QNetworkRequest request(unitLogEndpoint);
         request.setHeader(QNetworkRequest::ContentTypeHeader,
                           QVariant(QString("application/json")));
         //json log
//         qInfo(doc.toJson().toStdString().c_str());
         QString filename = "";
         filename.append(getUnitLogDir())
                         .append(getCurrentTimeString())
                         .append(".json");
         QFile file(filename);
         file.open(QIODevice::WriteOnly | QIODevice::Text);
         file.write(doc.toJson());
         file.close();
         //csv
         postUnitDataToCSV(uuid);
         //elk
         if (nam) {
             nam->post(request, doc.toJson());
             qInfo(doc.toJson().toStdString().c_str());
             qInfo("Push data to ELK success: %s", uuid.toStdString().c_str());
         } else {
             qInfo("Post data to ELK fail");
         }
    } else {
        qInfo("Cannot find the unit: %s", uuid.toStdString().c_str());
    }
    return true;
}

bool Unitlog::postUnitDataToCSV(QString uuid)
{
    if(!unit_log_list.contains(uuid))
    {
        qDebug("can not find uuid %s",uuid.toStdString().c_str());
        return false;
    }
    QString file_name = "";
    file_name.append(getProduceProcessLogDir())
                    .append(getCurrentDateString());
    QVariantMap data_map =  unit_log_list[uuid];
    if(data_map.size() == 1)
    {
        file_name.append("_");
        file_name.append(data_map.keys()[0]);
    }
    file_name.append(".csv");
    QFile file(file_name);
    QFileInfo fileInfo(file_name);
    QString content = "";
    if(!fileInfo.isFile())
    {
        headers_length = headers.length();
        foreach (QString temp_name, headers)
        {
            content.append(temp_name);
            content.append(",");
        }
        content.remove(content.lastIndexOf(","),1);
        content.append("\n");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(content.toStdString().c_str());
        file.close();
    }
    else if(headers_length < headers.length()||headers_length == 0)
    {
        headers_length = headers.length();
        foreach (QString temp_name, headers)
        {
            content.append(temp_name);
            content.append(",");
        }
        content.remove(content.lastIndexOf(","),1);
        content.append("\n");
        bool result = file.open(QIODevice::Append | QIODevice::Text);
        int count = 0;
        while (!result) {
            if(count>10)
                break;
            file.setFileName(file_name.append("_temp.csv"));
            result = file.open(QIODevice::Append | QIODevice::Text);
        }
        file.write(content.toStdString().c_str());
        file.close();
    }

    content = "";
    bool result = file.open(QIODevice::Append | QIODevice::Text);

    if(!result)
    {
        int count = 0;
        while (!result) {
            if(count>10)
                return false;
            file.setFileName(file_name.append("_temp.csv"));
            result = file.open(QIODevice::Append | QIODevice::Text);
        }
        if(temp_file_name == file_name)
        {
            if(headers_length < headers.length()||headers_length == 0)
            {
                headers_length = headers.length();
                foreach (QString temp_name, headers)
                {
                    content.append(temp_name);
                    content.append(",");
                }
                content.remove(content.lastIndexOf(","),1);
                content.append("\n");
            }
        }
        else
        {
            temp_file_name = file_name;
            headers_length = headers.length();
            foreach (QString temp_name, headers)
            {
                content.append(temp_name);
                content.append(",");
            }
            content.remove(content.lastIndexOf(","),1);
            content.append("\n");
        }
    }
    foreach (QString temp_header, headers) {
        QStringList temp_names = temp_header.split(".");
        if(data_map.contains(temp_names[0]))
        {
            QVariantMap temp_data = data_map[temp_names[0]].toMap();
            content.append(temp_data[temp_names[1]].toString());
        }
        content.append(",");
    }
    content.remove(content.lastIndexOf(","),1);
    content.append("\n");
    file.write(content.toStdString().c_str());
    file.close();
    qInfo("postUnitDataToCSV success. uuid: %s",uuid.toStdString().c_str());
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
