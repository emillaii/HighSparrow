#ifndef SUTCILENT_H
#define SUTCILENT_H

#include "XtCylinder.h"
#include "XtVacuum.h"
#include "sparrowqclient.h"

#include <qobject.h>

class SutGripper:public ErrorBase
{
public:
    SutGripper();
    void Init(XtVacuum* vacuum,XtCylinder* cylinder);
    bool openGripper();
    bool closeGripper();
private:
    XtVacuum* vacuum;
    XtCylinder* cylinder;
};

class SutCilent: public QObject
{
    Q_OBJECT
public:
    explicit SutCilent(QString address, QObject *parent = nullptr);
    void Init(SutGripper *sut_gripper);
    bool sendLensRequest(bool has_product,bool has_ng_sensor);
private:
    bool be_comuniting = false;
    SparrowClient * socketClient = Q_NULLPTR;
    SutGripper *sut_gripper = Q_NULLPTR;
    bool has_ng_sennsor = false;
    bool has_product = false;
signals:
    void sendMessageToServer(QString message);
    void triggerSutGrippers(bool isOn);
public slots:
    void receiveMessage(QString message);
};

#endif // SUTCILENT_H
