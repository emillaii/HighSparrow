#ifndef SENDMESSAGETOOL_H
#define SENDMESSAGETOOL_H

#include <QObject>

class SendMessageTool : public QObject{
    Q_OBJECT
public:
    SendMessageTool();
    ~SendMessageTool();
    bool CustomerSendMessage(QString title,QString content,QString btnOk="OK",QString btnQuit="");
signals:

};

#endif // SENDMESSAGETOOL_H
