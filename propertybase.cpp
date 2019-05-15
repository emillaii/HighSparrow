#include "propertybase.h"

#include <QDateTime>
#include <QFile>
#include <QJsonDocument>
#include <QMetaProperty>
#include <qjsonarray.h>


PropertyBase::PropertyBase(QObject *parent) : QObject(parent)
{

}

PropertyBase::~PropertyBase()
{

}

void PropertyBase::read(const QJsonObject &json)
{
    const QMetaObject *metaobject = this->metaObject();
    int p_count = metaobject->propertyCount();
    for(int i = 0;i < p_count;i++)
    {
        QMetaProperty metaproperty = metaobject->property(i);
        const char *name = metaproperty.name();
        if(json.contains(name))
        {
            this->setProperty(name,json[name]);
        }
    }
}

void PropertyBase::write(QJsonObject &json) const
{
    const QMetaObject *metaobject = this->metaObject();
    int p_count = metaobject->propertyCount();
    for(int i = 0;i < p_count;i++)
    {
        QMetaProperty metaproperty = metaobject->property(i);
        QVariant::Type temp_type = metaproperty.type();
        int index = metaproperty.propertyIndex();
        if(index != temp_type)
        {
            index = temp_type;
        }
        if(temp_type == QVariant::Type::Double){
            const char *name = metaproperty.name();
            json[name] = this->property(name).toDouble();
        }
        else if (temp_type == QVariant::Type::Int) {
            const char *name = metaproperty.name();
            json[name] = this->property(name).toInt();
        }
        else if (temp_type == QVariant::Type::Bool) {
            const char *name = metaproperty.name();
            json[name] = this->property(name).toBool();
        }
        else if (temp_type == QVariant::Type::String) {
            const char *name = metaproperty.name();
            json[name] = this->property(name).toString();
        }
        else if (temp_type == QVariant::Type::List) {
            const char *name = metaproperty.name();
            json[name] = QJsonArray::fromVariantList(this->property(name).toList());
        }
        else {
            qInfo("write parameter error,unrecognized type : %d : %s",temp_type,metaproperty.name());
        }
    }
}

bool PropertyBase::loadJsonConfig(QString file_name,QString param_name)
{
    QString val;
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("load parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject jsonObject = d.object();
    this->read(jsonObject.value(param_name).toObject());
    return true;
}

void PropertyBase::saveJsonConfig(QString file_name,QString param_name)
{
    QFile saveFile(file_name);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("save parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return;
    }
    QJsonObject json;
    QJsonObject temp_data;
    this->write(temp_data);
    json[param_name] = temp_data;
    json["Timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
}

void PropertyBase::saveJsonConfig(QString file_name, const QMap<QString, PropertyBase*> &parameters)
{
    QFile saveFile(file_name);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("save parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return;
    }
    QJsonObject json;
    for (QMap<QString, PropertyBase*>::const_iterator i = parameters.constBegin(); i != parameters.constEnd(); ++i) {
        QJsonObject temp_data;
        i.value()->write(temp_data);
        json[i.key()] = temp_data;
    }
    json["Timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
}

void PropertyBase::saveJsonConfig(QString file_path, const QString name, const PropertyBase *parameters)
{
    QString file_name = file_path;
    file_name.append(name).append(".json");
    QFile saveFile(file_name);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("save parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return;
    }
    QJsonObject json;
    QJsonObject temp_data;
    parameters->write(temp_data);
    json[name] = temp_data;
    json["Timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
}

bool PropertyBase::loadJsonConfig(QString file_name, QMap<QString, PropertyBase*> &parameters)
{
    QString val;
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))return false;
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject jsonObject = d.object();
    for (QMap<QString, PropertyBase*>::const_iterator i = parameters.constBegin(); i != parameters.constEnd(); ++i) {
        if(jsonObject.contains(i.key()))
        {
           i.value()->read(jsonObject.value(i.key()).toObject());
        }
    }
    return true;
}

bool PropertyBase::loadJsonConfig(QString file_path, const QString name, PropertyBase *parameters)
{
    QString file_name = file_path;
    file_name.append(name).append(".json");
    QString val;
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("load parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject jsonObject = d.object();
    parameters->read(jsonObject.value(name).toObject());
    return true;
}
