#ifndef PROPERTYBASE_H
#define PROPERTYBASE_H

#include <QJsonObject>
#include <QMap>
#include <QObject>

class PropertyBase : public QObject
{
    Q_OBJECT
public:
    explicit PropertyBase(QObject *parent = nullptr);
    ~PropertyBase();
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    bool loadJsonConfig(QString file_name,QString param_name);
    void saveJsonConfig(QString file_name,QString param_name);

    static void saveJsonConfig(QString file_name,const QMap<QString, PropertyBase*> &parameters);
    static void saveJsonConfig(QString file_path,const QString name,const PropertyBase* parameters);
    static bool loadJsonConfig(QString file_name,QMap<QString, PropertyBase*> &parameters);
    static bool loadJsonConfig(QString file_path,const QString name,PropertyBase* parameters);

};

#endif // PROPERTYBASE_H
