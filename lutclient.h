#ifndef LUTCLIENT_H
#define LUTCLIENT_H

#include <QObject>
#include "sparrowqclient.h"
#include "aaheadmodule.h"
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
    explicit LutClient(AAHeadModule *aaHead, QString address, QObject *parent = nullptr);
    bool sendLensRequest(bool has_ng_lens = false);
private:
    LutClientState state;
    SparrowClient * socketClient;
    AAHeadModule * aaHead;
    bool has_ng_lens = false;
signals:
    void sendMessageToServer(QString message);
    void triggerAAGripper(bool isOn);
public slots:
    void receiveMessage(QString messsage);
};

#endif // LUTCLIENT_H
