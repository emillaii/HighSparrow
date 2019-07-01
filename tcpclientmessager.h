#ifndef TCPMESSAGERCLIENT_H
#define TCPMESSAGERCLIENT_H

#include "tcpmessager.h"

class TcpClientMessager: public TcpMessager
{
    Q_OBJECT
public:
    TcpClientMessager();
    void Init();

signals:
public slots:
    void onConnected();
    void onClosed();
};

#endif // TCPMESSAGERCLIENT_H
