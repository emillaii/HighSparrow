#ifndef TCPSERVERMESSAGER_H
#define TCPSERVERMESSAGER_H

#include "tcpmessager.h"



class TcpServerMessager:public TcpMessager
{
    Q_OBJECT
public:
    TcpServerMessager();
    void Init(QWebSocket* web_socket);

signals:

public slots:
    void onClosed();
};

#endif // TCPSERVERMESSAGER_H
