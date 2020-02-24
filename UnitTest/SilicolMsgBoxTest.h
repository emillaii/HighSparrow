#ifndef TESTER_H
#define TESTER_H

#include <QObject>
#include <QtGlobal>
#include <QTime>
#include "./Utils/singletoninstances.h"

class MsgBoxThreadTester: public QObject
{
    Q_OBJECT

public:
    MsgBoxThreadTester()
    {

    }

public slots:
    void onGetMsgRsp(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons)
    {
        SI::ui.getUIResponse(title, content, icon, buttons);
    }

    void onShowMsg(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons)
    {
        SI::ui.showMessage(title, content, icon, buttons);
    }
};


class MsgBoxTester:public QObject
{
    Q_OBJECT

public:
    MsgBoxTester()
    {
        thdTester.moveToThread(&thd);
        thd.start();
        connect(this, &MsgBoxTester::getMsgRsp, &thdTester, &MsgBoxThreadTester::onGetMsgRsp);
        connect(this, &MsgBoxTester::showMsg, &thdTester, &MsgBoxThreadTester::onShowMsg);
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    }

    Q_INVOKABLE void randomGetMsgRsp()
    {
        emit getMsgRsp(QString::number(qrand()), QString::number(qrand()), generateIcon(), generateButtons());
    }

    Q_INVOKABLE void randomShowMsg()
    {
        QString content;
        for(int i = 0; i < 1000; i++)
        {
            content.append(QString::number(qrand()));
            content.append(" ");
        }
        emit showMsg(QString::number(qrand()), content, generateIcon(), generateButtons());
    }

signals:
    void getMsgRsp(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons);
    void showMsg(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons);

private:
    MsgBoxIcon::Icon generateIcon()
    {
        return (MsgBoxIcon::Icon)(qrand() % 4);
    }

    QList<QString> generateButtons()
    {
        switch (qrand() % 6) {
        case 0:
            return SI::ui.yesNoButtons;
        case 1:
            return SI::ui.passFailButtons;
        case 2:
            return SI::ui.retryAbortButtons;
        case 3:
            return SI::ui.retryIgnoreButtons;
        case 4:
            return SI::ui.okCancelButtons;
        case 5:
            return SI::ui.retryIgnoreAbortButtons;
        }
        return SI::ui.retryIgnoreAbortButtons;
    }

    QThread thd;
    MsgBoxThreadTester thdTester;
};

#endif // TESTER_H
