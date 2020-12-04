#ifndef UNITLOGGER_H
#define UNITLOGGER_H

#include <string>
#include <qstring.h>
#include <QObject>
#include <QTime>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <windows.h>
#include "userManagement/usermanagement.h"

#define SunnyPrism

class Unitlog : public QObject
{
    Q_OBJECT

public:
    Unitlog(QObject* parent = 0);
    ~Unitlog();
    QString createUnit();
    bool saveToCSV(QString uuid);
    void setServerAddress(QString);
    void setUserManagement(UserManagement *userManagement){ this->userManagement = userManagement; }
    bool sendToMES = false;
    bool isSendDataToMES = false;
    QString opType = "";
#ifdef SunnyPrism
    QString fuseID = "";
    QString sensorName = "";
    QString moduleName = "";
    bool currentTestResultOK = true;
#endif
private:
    QMap<QString, QVariantMap> unit_log_list;
    QMap<QString, std::vector<QString>> unit_log_test_name_list;
    QNetworkAccessManager * nam = Q_NULLPTR;
    QString serverAddress;
    QStringList headers;
    int headers_length;
    QString temp_file_name = "";
    UserManagement *userManagement;
#ifdef SunnyPrism
    HINSTANCE sunny_prism_hDll;
    typedef int(CALLBACK *CREATEANDINSERTTESTDATAFORPRISM)(const char*,const char*,const char*,const char*,const char*,const char*,const char*,const char*,const char*);
    CREATEANDINSERTTESTDATAFORPRISM CreateAndInsertTestDataForPrism;
#endif
public slots:
    void setSendDataToMES(bool isSendDataToMES) {
        qInfo("isSendDataToMES: %d", isSendDataToMES);
        this->isSendDataToMES = isSendDataToMES;
    }
    void setOpType(QString operationType)
    {
        qInfo("opType: %s", operationType.toStdString().c_str());
        this->opType = operationType;
    }
    bool pushDataToUnit(QString uuid, QString name, QVariantMap map);
    bool pushNgDataToCSV(QString uuid, QString lotNumber, QString sensorId, QString testItemName, QString errorMessage);
    void clearHeaders();
    bool postDataToELK(QString uuid, QString lotNumber);    // send data to mes in sunny
    bool postDataToELKInternal(QString uuid, QString lotNumber);    // only send data to elk
    bool postUnitDataToCSV(QString uuid);
    bool postSfrDataToELK(QString uuid, QVariantMap data);
    QString getCSVString(QString data_name,QVariantMap map);
    void pushCSVDataToUnit(QString uuid,QVariantMap data);
    void saveUnitDataToCSV(QString uuid);
};

#endif // UNITLOGGER_H
