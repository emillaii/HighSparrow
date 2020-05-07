#include "propertybase.h"

#include <QDateTime>
#include <QFile>
#include <QJsonDocument>
#include <QMetaProperty>
#include <qjsonarray.h>

QMutex PropertyBase::rwMutex;

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
            this->setProperty(name,json[name].toVariant());
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
        const char *name = metaproperty.name();
        if(temp_type == QVariant::Type::Double){
            json[name] = this->property(name).toDouble();
        }
        else if (temp_type == QVariant::Type::Int ||
                 temp_type == QVariant::Type::UInt) {
            json[name] = this->property(name).toInt();
        }
        else if (temp_type == QVariant::Type::LongLong ||
                 temp_type == QVariant::Type::ULongLong) {
            json[name] = this->property(name).toLongLong();
        }
        else if (temp_type == QVariant::Type::Bool) {
            json[name] = this->property(name).toBool();
        }
        else if (temp_type == QVariant::Type::String) {
            json[name] = this->property(name).toString();
        }
        else if (temp_type == QVariant::Type::List) {
            json[name] = QJsonArray::fromVariantList(this->property(name).toList());
        }
        else if (temp_type == QVariant::Type::StringList) {
            json[name] = QJsonArray::fromStringList(this->property(name).toStringList());
        }
        else if (temp_type == QVariant::Type::Map) {
            json[name] = QJsonObject::fromVariantMap(this->property(name).toMap());
        }
        else {
            qWarning("write parameter error,unrecognized type : %d : %s",temp_type,metaproperty.name());
        }
    }
}

void PropertyBase::reset()
{
    const QMetaObject *metaobject = this->metaObject();
    int p_count = metaobject->propertyCount();
    for(int i = 0;i < p_count;i++)
    {
        QMetaProperty metaproperty = metaobject->property(i);
        const char *name = metaproperty.name();
        if(init_values.contains(QString::fromUtf8(name)))
            this->setProperty(name,init_values[name]);
        else if(metaproperty.type() == QVariant::Type::Double)
            this->setProperty(name,0.0);
        else if (metaproperty.type() == QVariant::Type::Int)
            this->setProperty(name,0);
        else if (metaproperty.type() == QVariant::Type::Bool)
            this->setProperty(name,false);
        else if (metaproperty.type() == QVariant::Type::String)
            this->setProperty(name,"");
        else if (metaproperty.type() == QVariant::Type::Map)
            this->property(name).clear();
        else
            qWarning("reset parameter %s error,unrecognized type : %d : %s",metaproperty.name(),metaproperty.type());
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
    rwMutex.lock();

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

    rwMutex.unlock();
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
    rwMutex.lock();

    QString val;
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("load parameter from %s fail",file_name.toStdString().c_str());
        return false;
    }
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
    rwMutex.unlock();
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

bool PropertyBase::loadJsonArray(QString file_name,QJsonArray &array)
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

    QJsonParseError error;
    QJsonDocument doucment = QJsonDocument::fromJson(val.toUtf8(), &error);
    if (!doucment.isNull() && (error.error == QJsonParseError::NoError)) { //解析否出现错误
        if (doucment.isArray()) { // 数组判断
            array = doucment.array(); // 转数组
        }
    }
    if(array.size()>0)
        return true;
    else
    {
        qWarning("load parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
}

bool PropertyBase::saveJsonArray(QString file_name,QJsonArray &array)
{
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QFile::WriteOnly)){
        qWarning("save parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    QJsonDocument document;
    document.setArray(array);
    file.write(document.toJson());
    file.close();
    return true;
}

bool PropertyBase::loadJsonObject(QString file_name, QJsonObject &object)
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

    QJsonParseError error;
    QJsonDocument doucment = QJsonDocument::fromJson(val.toUtf8(), &error);
    if (!doucment.isNull() && (error.error == QJsonParseError::NoError)) { //解析否出现错误
        if (doucment.isObject()) { // 数组判断
            object = doucment.object(); // 转数组
            return true;
        }
    }
    qWarning("load parameters to %s failed, load file error:%s.",file_name.toStdString().data(),error.errorString().toStdString().c_str());
    return false;
}

bool PropertyBase::saveJsonObject(QString file_name, QJsonObject &object)
{
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QFile::WriteOnly)){
        qWarning("save parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    QJsonDocument document;
    document.setObject(object);
    file.write(document.toJson());
    file.close();
    return true;
}

