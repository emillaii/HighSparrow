#ifndef UNITLOGGER_H
#define UNITLOGGER_H

#include <string>
#include <qstring.h>
#include <QObject>
#include <QTime>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

class Unitlog : public QObject
{
    Q_OBJECT

public:
    Unitlog(QObject* parent = 0);
    ~Unitlog();
    QString createUnit();
    bool saveToCSV(QString uuid);
    void setServerAddress(QString);
private:
    QMap<QString, QVariantMap> unit_log_list;
    QMap<QString, std::vector<QString>> unit_log_test_name_list;
    QNetworkAccessManager * nam = Q_NULLPTR;
    QString serverAddress;
public slots:
    bool pushDataToUnit(QString uuid, QString name, QVariantMap map);
    bool postDataToELK(QString uuid);
    bool postSfrDataToELK(QString uuid, QVariantMap data);
    QString getCSVString(QString data_name,QVariantMap map);
};

#endif // UNITLOGGER_H
