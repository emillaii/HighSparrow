#include "utils/unitlog.h"
#include <QUuid>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <qfileinfo.h>
#include "config.h"
#include "utils/commonutils.h"
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

        QString temp_header;
        foreach (QString temp_key, map.keys()) {
            temp_header = QString(name).append(".").append(temp_key);
            if(!headers.contains(temp_header))
                headers.append(temp_header);
        }
        return true;
    }
    qInfo("Cannot find the unit: %f", uuid.toStdString().c_str());
    return false;
}

void Unitlog::clearHeaders()
{
    temp_file_name = "";
    headers_length = 0;
    headers.clear();
}

bool Unitlog::postDataToELK(QString uuid, QString lotNumber)
{
    if (unit_log_list.contains(uuid))
    {
         QJsonObject json = QJsonObject::fromVariantMap(unit_log_list[uuid]);
         json["uuid"] = uuid;
         json["lotNumber"] = lotNumber;
         QJsonDocument doc(json);
         QUrl unitLogEndpoint = QString("http://192.168.0.250:5044");
         // QString unitLogEndpoint = serverAddress;
         //unitLogEndpoint.append(":").append(QString::number(UNIT_LOG_PORT));
         QNetworkRequest request(unitLogEndpoint);
         request.setHeader(QNetworkRequest::ContentTypeHeader,
                           QVariant(QString("application/json")));
         //json log
         QString filename = "";
         filename.append(getUnitLogDir())
                         .append(getCurrentTimeString())
                         .append("_")
                         .append(lotNumber)
                         .append("_")
                         .append(uuid)
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
             qDebug("Push data to ELK success: %s", uuid.toStdString().c_str());
         } else {
             qDebug("Post data to ELK fail");
         }
         unit_log_list.remove(uuid);
    } else {
        qInfo("Cannot find the unit: %s", uuid.toStdString().c_str());
    }
    return true;
}

bool Unitlog::pushNgDataToCSV(QString uuid, QString lotNumber, QString sensorId, QString testItemName, QString errorMessage)
{
    QString ngMessage = getCurrentDateString().append("-")
                       .append(getCurrentTimeString()).append(",")
                       .append(uuid).append(",")
                       .append(lotNumber).append(",")
                       .append(sensorId).append(",")
                       .append(testItemName).append(",")
                       .append(errorMessage)
                       .append("\n");
    QString filename;
    filename.append(getRejectLogDir()).append("ng.log");
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    file.write(ngMessage.toStdString().c_str());
    file.close();
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
     data.insert("uuid", uuid);
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
//       nam->post(request, doc.toJson());
//       qInfo("Push sfr data to ELK success: %s", uuid.toStdString().c_str());
         {
             QString filename = "";
             filename.append(getAALogDir())
                             .append(getCurrentTimeString())
                             .append("_")
                             .append(uuid)
                             .append(".json");
             QFile file(filename);
             file.open(QIODevice::WriteOnly | QIODevice::Text);
             file.write(doc.toJson());
             file.close();
         }

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
         data_content.append("zPeak_1_UL,");
         data_content.append("zPeak_1_UR,");
         data_content.append("zPeak_1_LR,");
         data_content.append("zPeak_1_LL,");
         data_content.append("zPeak_2_UL,");
         data_content.append("zPeak_2_UR,");
         data_content.append("zPeak_2_LR,");
         data_content.append("zPeak_2_LL,");
         data_content.append("zPeak_3_UL,");
         data_content.append("zPeak_3_UR,");
         data_content.append("zPeak_3_LR,");
         data_content.append("zPeak_3_LL,");
         data_content.append("zPeak,");
         data_content.append("cczPeak,");
         data_content.append("dev,");
         data_content.append("xTilt_1,");
         data_content.append("yTilt_1,");
         data_content.append("xTilt_2,");
         data_content.append("yTilt_2,");
         data_content.append("xTilt_3,");
         data_content.append("yTilt_3,");
         data_content.append("fov_slope,");
         data_content.append(" \r\n");
         data_content.append(data["zPeak_1_UL"].toString());
         data_content.append(",");
         data_content.append(data["zPeak_1_UR"].toString());
         data_content.append(",");
         data_content.append(data["zPeak_1_LR"].toString());
         data_content.append(",");
         data_content.append(data["zPeak_1_LL"].toString());
         data_content.append(",");
         data_content.append(data["zPeak_2_UL"].toString());
         data_content.append(",");
         data_content.append(data["zPeak_2_UR"].toString());
         data_content.append(",");
         data_content.append(data["zPeak_2_LR"].toString());
         data_content.append(",");
         data_content.append(data["zPeak_2_LL"].toString());
         data_content.append(",");
         data_content.append(data["zPeak_3_UL"].toString());
         data_content.append(",");
         data_content.append(data["zPeak_3_UR"].toString());
         data_content.append(",");
         data_content.append(data["zPeak_3_LR"].toString());
         data_content.append(",");
         data_content.append(data["zPeak_3_LL"].toString());
         data_content.append(",");
         data_content.append(data["zPeak"].toString());
         data_content.append(",");
         data_content.append(data["cczPeak"].toString());
         data_content.append(",");
         data_content.append(data["dev"].toString());
         data_content.append(",");
         data_content.append(data["xTilt_1"].toString());
         data_content.append(",");
         data_content.append(data["yTilt_1"].toString());
         data_content.append(",");
         data_content.append(data["xTilt_2"].toString());
         data_content.append(",");
         data_content.append(data["yTilt_2"].toString());
         data_content.append(",");
         data_content.append(data["xTilt_3"].toString());
         data_content.append(",");
         data_content.append(data["yTilt_3"].toString());
         data_content.append(",");
         data_content.append(data["fov_slope"].toString());
         data_content.append(" \r\n");
         data_content.append(getCSVString("CC",data["CC"].toMap()));
         data_content.append(getCSVString("LL_1",data["LL_1"].toMap()));
         data_content.append(getCSVString("LR_1",data["LR_1"].toMap()));
         data_content.append(getCSVString("UL_1",data["UL_1"].toMap()));
         data_content.append(getCSVString("UR_1",data["UR_1"].toMap()));

         data_content.append(getCSVString("LL_2",data["LL_2"].toMap()));
         data_content.append(getCSVString("LR_2",data["LR_2"].toMap()));
         data_content.append(getCSVString("UL_2",data["UL_2"].toMap()));
         data_content.append(getCSVString("UR_2",data["UR_2"].toMap()));

         data_content.append(getCSVString("LL_3",data["LL_3"].toMap()));
         data_content.append(getCSVString("LR_3",data["LR_3"].toMap()));
         data_content.append(getCSVString("UL_3",data["UL_3"].toMap()));
         data_content.append(getCSVString("UR_3",data["UR_3"].toMap()));
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
       data_string.append(temp_data["pz"].toString());
       data_string.append(",");
    }
    data_string.append(" \r\n");
    foreach (QString temp_name, map.keys()) {
       QVariantMap temp_data = map[temp_name].toMap();
       data_string.append(temp_data["t_sfr"].toString());
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

    //qInfo("data_string: %s", data_string.toStdString().c_str());
    return data_string;
}

void Unitlog::pushCSVDataToUnit(QString uuid,QVariantMap data)
{
    if(!unit_log_list.contains(uuid))
    {
        unit_log_list.insert(uuid,QVariantMap());
    }
    qInfo("postCSVDataToUnit %s :  %d",uuid.toStdString().c_str(),data.size());
    foreach(QString temp_data, data.keys())
    {
        qInfo("postCSVDataToUnit %s : %s",temp_data.toStdString().c_str(),data[temp_data].toString().toStdString().c_str());
        unit_log_list[uuid][temp_data] = data[temp_data];
    }
}

void Unitlog::saveUnitDataToCSV(QString uuid)
{
    if(!unit_log_list.contains(uuid))
        return;
    QString file_name = "";
    file_name.append(getPerformanceLogDir())
                    .append(getCurrentDateString())
                    .append(".csv");
    QFile file(file_name);
    QFileInfo fileInfo(file_name);
    QString content = "";
    if((!fileInfo.isFile()))
    {
        foreach (QString temp_key, unit_log_list[uuid].keys())
        {
           content.append(temp_key);
           content.append(",");
        }
        content.remove(content.lastIndexOf(","),1);
        content.append("\n");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(content.toStdString().c_str());
        file.close();
    }
    content = "";
    foreach (QString temp_key, unit_log_list[uuid].keys())
    {
       content.append(unit_log_list[uuid][temp_key].toString());
       content.append(",");
    }
    content.remove(content.lastIndexOf(","),1);
    content.append("\n");
    file.open(QIODevice::Append | QIODevice::Text);
    file.write(content.toStdString().c_str());
    file.close();
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
