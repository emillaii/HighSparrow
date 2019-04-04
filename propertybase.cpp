#include "propertybase.h"

#include <QDateTime>
#include <QFile>
#include <QJsonDocument>
#include <QMetaProperty>


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
        else {
            qInfo("write parameter error,unrecognized type : %d : %s",temp_type,metaproperty.name());
        }
    }
}

void PropertyBase::saveJsonConfig(QString file_path, const QMap<QString, PropertyBase*> &parameters)
{
    QFile saveFile(file_path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("save parameters to %s failed, Couldn't open save file.",file_path.toStdString().data());
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
}

void PropertyBase::loadJsonConfig(QString file_path, QMap<QString, PropertyBase*> &parameters)
{
    QString val;
    QFile file;
    file.setFileName(file_path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
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
}
