#ifndef LUTCLIENT_H
#define LUTCLIENT_H

#include <QObject>
#include "sparrowqclient.h"

enum LutClientState{
    LUT_CLIENT_IDLE,
    WAITING_LENS_PICK_EVENT,
    WAITING_LENS_PR_EVENT,
    WAITING_LUT_LEAVE_EVENT
};

class LutClient : public QObject
{
    Q_OBJECT
public:
    explicit LutClient(QString address, QObject *parent = nullptr);
    void sendLensRequest();
private:
    LutClientState state;
    SparrowClient * socketClient;
signals:
    void sendMessageToServer(QString message);
    void triggerAAGripper(bool isOn);
public slots:
    void receiveMessage(QString messsage);
};

#endif // LUTCLIENT_H
