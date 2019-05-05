#ifndef SUTCILENT_H
#define SUTCILENT_H

#include "material_carrier.h"
#include "sparrowqclient.h"

#include <qobject.h>

class SutGripper
{

};

class SutCilent: public QObject
{
    Q_OBJECT
public:
    explicit SutCilent(MaterialCarrier *sut_carrier,QString address, QObject *parent = nullptr);
    void Init(MaterialCarrier *sut_carrier);
    bool sendLensRequest(bool has_product,bool has_ng_sensor);
private:
    bool be_comuniting = false;
    SparrowClient * socketClient = Q_NULLPTR;
    MaterialCarrier *sut_carrier = Q_NULLPTR;
    bool has_ng_sennsor = false;
    bool has_product = false;
signals:
    void sendMessageToServer(QString message);
    void triggerSutGrippers(bool isOn);
public slots:
    void receiveMessage(QString message);
};

#endif // SUTCILENT_H
