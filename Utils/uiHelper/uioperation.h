#ifndef UIOPERATION_H
#define UIOPERATION_H

#include <QObject>
#include <QUuid>
#include <QWaitCondition>
#include <QMutex>
#include <QThread>
#include <QDebug>
#include "msgboxmodel.h"

class UIResponse
{
public:
    QWaitCondition waitResponse;
    QString clickedButton;
};

///
/// \brief The UIOperation class
/// UI操作类，不阻塞UI
/// 可以同时调用showMessage
/// 不同线程可以同时调用getUIResponse
///
class UIOperation: public QObject
{
    Q_OBJECT

public:
    UIOperation();

    void init(MsgBoxModel* msgBoxModel);

    void showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons);
    void showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QString button);
    QString getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons);
    QString getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QString button);

signals:
    void addMsgBox(MsgBoxItem msgBox);
    void removeMsgBox(QString uuid);

public slots:
    void onUIResponse(QString uuid, QString clickedButton);

public:
    const QString Yes = "Yes";
    const QString No = "No";
    const QString Pass = "Pass";
    const QString Fail = "Fail";
    const QString Ok = "Ok";
    const QString Cancel = "Cancel";
    const QString Retry = "Retry";
    const QString Ignore = "Ignore";
    const QString Abort = "Abort";

    QList<QString> yesNoButtons;
    QList<QString> passFailButtons;
    QList<QString> okCancelButtons;
    QList<QString> retryIgnoreAbortButtons;
    QList<QString> retryAbortButtons;
    QList<QString> retryIgnoreButtons;


private:
    MsgBoxModel* msgBoxModel;
    QMap<QString, UIResponse*> uiRsp;
};

#endif // UIOPERATION_H
