#ifndef SUTCILENT_H
#define SUTCILENT_H

#include "XtVacuum.h"
#include "network/sparrowqclient.h"

#include <qobject.h>

class SutClient: public QObject
{
    Q_OBJECT
public:
    enum MaterialType
    {
        NG_SENSOR = 1,
        GOOD_PRODUCT = 2,
        NG_PRODUCT = 3
    };
public:
    explicit SutClient(QString address, QObject *parent = nullptr);
    void Init(XtVacuum *sut_vacuum);
    bool sendSensorRequest(bool &is_run,int material_type);
private:
    bool be_comuniting = false;
    SparrowClient * socketClient = Q_NULLPTR;
    XtVacuum *sut_vacuum = Q_NULLPTR;
    int curren_type;
signals:
    void sendMessageToServer(QString message);
    void triggerSutGrippers(bool isOn);
public slots:
    void receiveMessage(QString message);
};

#endif // SUTCILENT_H
