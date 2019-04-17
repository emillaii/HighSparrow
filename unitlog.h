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
    bool pushDataToUnit(QString uuid, QString name, QVariantMap&);
    bool saveToCSV(QString uuid);
private:
    QMap<QString, QVariantMap> unit_log_list;
    QMap<QString, std::vector<QString>> unit_log_test_name_list;
};

#endif // UNITLOGGER_H
