#ifndef UIOPERATION_H
#define UIOPERATION_H

#include "../commonmethod.h"
#include "msgboxmodel.h"
#include <QDebug>
#include <QEvent>
#include <QMutex>
#include <QObject>
#include <QQuickItem>
#include <QThread>
#include <QUuid>
#include <QWaitCondition>

const QString YesBtn = "Yes";
const QString NoBtn = "No";
const QString PassBtn = "Pass";
const QString FailBtn = "Fail";
const QString OkBtn = "Ok";
const QString CancelBtn = "Cancel";
const QString RetryBtn = "Retry";

const QString BtnSeperator = "|";

const QString YesNoBtns = YesBtn + BtnSeperator + NoBtn;
const QString PassFailBtns = PassBtn + BtnSeperator + FailBtn;
const QString OkCancelBtns = OkBtn + BtnSeperator + CancelBtn;

class UIResponse
{
public:
    QWaitCondition waitResponse;
    QString clickedButton;
    bool gotResponse = false;
};

///
/// \brief The UIOperation class
/// UI操作类，不阻塞UI
/// 可以同时调用showMessage
/// 不同线程可以同时调用getUIResponse
/// 非主线程，调用getUIResponse后，必须等函数返回才会处理其他事件
/// 主线程调用getUIResponse后，可以处理其他事件，因此若前一次getUIResponse未返回结果，仍然可以再次调用getUIResponse
///
class UIOperation : public QObject
{
    Q_OBJECT

private:
    UIOperation();

public:
    static UIOperation *getIns()
    {
        static UIOperation instance;
        return &instance;
    }

    Q_INVOKABLE void setGrabMouseItem(QQuickItem *item);

    void init(MsgBoxModel *msgBoxModel, Qt::HANDLE mainThreadID);

    void showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons);
    void showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QString button);
    QString getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons);
    QString getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QString button);

signals:
    void addMsgBox(MsgBoxItem msgBox);
    void removeMsgBox(QString uuid);

public slots:
    void onUIResponse(QString uuid, QString clickedButton);

private:
    QDebug getLogger(MsgBoxIcon::Icon icon)
    {
        switch (icon)
        {
            case MsgBoxIcon::Error:
                return qCritical();
            case MsgBoxIcon::Warning:
                return qWarning();
            default:
                return qInfo();
        }
    }

private:
    MsgBoxModel *msgBoxModel;
    QMap<QString, UIResponse *> uiRsp;
    Qt::HANDLE mainThreadID;
    QQuickItem *grabMouseItem = nullptr;
};

#endif    // UIOPERATION_H
