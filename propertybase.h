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

    static void saveJsonConfig(QString file_path,const QMap<QString, PropertyBase*> &parameters);
    static void loadJsonConfig(QString file_path,QMap<QString, PropertyBase*> &parameters);

};

#endif // PROPERTYBASE_H
