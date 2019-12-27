#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QDateTime>
#include "configobject.h"

class ConfigFile: public QObject
{
    Q_OBJECT

public:
    ConfigFile(QString configBaseName, ConfigBase* configBase, QString fileName, bool encript=true):
        configBaseName(configBaseName), configBase(configBase), fileName(fileName), encript(encript)
    {
        if(configBase == nullptr){
            qFatal("configBase is null!");
            return;
        }
        bool gotSlotIndex = false;
        for (int i = 0; i < metaObject()->methodCount(); i++) {
            auto method = metaObject()->method(i);
            if(method.methodType() == QMetaMethod::Slot &&
                    QString(method.name()) == QString("save")){
                m_saveSlotIndex = method.methodIndex();
                gotSlotIndex = true;
                break;
            }
        }
        if(!gotSlotIndex){
            qFatal("Did not find slot! Name: save");
        } else {
            configBase->connectConfigChangedSignalToSlot(this, m_saveSlotIndex, true);
        }
    }

    bool populate()
    {
        configBase->connectConfigChangedSignalToSlot(this, m_saveSlotIndex, false);
        bool result = _populate();
        configBase->connectConfigChangedSignalToSlot(this, m_saveSlotIndex, true);
        return result;
    }

private slots:
    void save()
    {
        qDebug() << QString("Config changed! Auto save! File name: %1").arg(fileName);

        QFileInfo fileInfo(fileName);
        QDir dir = fileInfo.dir();
        if(!dir.exists()){
            dir.mkpath(fileInfo.path());
        }
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            qCritical() << "Open file failed: " << fileName;
            return;
        }
        QJsonObject topJsonObj;
        QJsonValue JsonValue;
        configBase->write(JsonValue);
        topJsonObj[configBaseName] = JsonValue;
        topJsonObj["Timestamp"] = QDateTime::currentDateTime().toString();
        QJsonDocument newJsonDoc(topJsonObj);
        //        file.write(newJsonDoc.toJson());
        if(encript){
            file.write(newJsonDoc.toJson().toBase64());
        } else {
            file.write(newJsonDoc.toJson());
        }
        file.close();
    }

    bool _populate()
    {
        QFile file(fileName);
        if(!file.exists()){
            qCritical() << QString("Did not exist json file: %1").arg(fileName);
            return false;
        }
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qCritical() << QString("Open json file failed: %1").arg(fileName);
            return false;
        }
        QByteArray fileContent = file.readAll();
        file.close();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromBase64(fileContent));
        if(jsonDoc.isNull() || jsonDoc.isEmpty()){
            jsonDoc = QJsonDocument::fromJson(fileContent);
            if(jsonDoc.isNull() || jsonDoc.isEmpty()){
                qCritical() << QString("Json file %1 is invalid!").arg(fileName);
                return false;
            }
        }

        if(!jsonDoc.object().contains(configBaseName)){
            qCritical() << QString("Did not exist config base named %1 in json file %2.").
                           arg(configBaseName).arg(fileName);
            return false;
        }
        return configBase->read(jsonDoc.object()[configBaseName]);
    }

private:
    QString configBaseName;
    ConfigBase* configBase = nullptr;
    QString fileName;
    bool encript;
    int m_saveSlotIndex;
};

#endif // CONFIGFILE_H
