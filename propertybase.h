#ifndef PROPERTYBASE_H
#define PROPERTYBASE_H

#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QVariantMap>
#include <QMutex>

class PropertyBase : public QObject
{
    Q_OBJECT
public:
    explicit PropertyBase(QObject *parent = nullptr);
    ~PropertyBase();
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void reset();
    bool loadJsonConfig(QString file_name,QString param_name);
    void saveJsonConfig(QString file_name,QString param_name);

    static void saveJsonConfig(QString file_name,const QMap<QString, PropertyBase*> &parameters);
    static void saveJsonConfig(QString file_path,const QString name,const PropertyBase* parameters);
    static bool loadJsonConfig(QString file_name,QMap<QString, PropertyBase*> &parameters);
    static bool loadJsonConfig(QString file_path,const QString name,PropertyBase* parameters);

    static bool loadJsonArray(QString file_name, QJsonArray &array);
    static bool saveJsonArray(QString file_name,QJsonArray &array);
    static bool loadJsonObject(QString file_name, QJsonObject &object);
    static bool saveJsonObject(QString file_name,QJsonObject &object);

    static QMutex rwMutex;
protected:
    QVariantMap init_values;
};

#endif // PROPERTYBASE_H
