#ifndef SUTCILENT_H
#define SUTCILENT_H

#include "XtVacuum.h"
#include "sparrowqclient.h"

#include <qobject.h>

class SutClient: public QObject
{
    Q_OBJECT
public:
    explicit SutClient(QString address, QObject *parent = nullptr);
    void Init(XtVacuum *sut_vacuum);
    bool sendSensorRequest(bool &is_run,bool has_product,bool has_ng_sensor);
private:
    bool be_comuniting = false;
    SparrowClient * socketClient = Q_NULLPTR;
    XtVacuum *sut_vacuum = Q_NULLPTR;
    bool has_ng_sennsor = false;
    bool has_product = false;
signals:
    void sendMessageToServer(QString message);
    void triggerSutGrippers(bool isOn);
public slots:
    void receiveMessage(QString message);
};

#endif // SUTCILENT_H
