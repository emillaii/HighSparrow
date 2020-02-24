#include "uioperation.h"


UIOperation::UIOperation()
{
    yesNoButtons.append(Yes);
    yesNoButtons.append(No);
    passFailButtons.append(Pass);
    passFailButtons.append(Fail);
    okCancelButtons.append(Ok);
    okCancelButtons.append(Cancel);
    retryIgnoreAbortButtons.append(Retry);
    retryIgnoreAbortButtons.append(Ignore);
    retryIgnoreAbortButtons.append(Abort);
    retryAbortButtons.append(Retry);
    retryAbortButtons.append(Abort);
    retryIgnoreButtons.append(Retry);
    retryIgnoreButtons.append(Ignore);
}

void UIOperation::init(MsgBoxModel *msgBoxModel)
{
    this->msgBoxModel = msgBoxModel;
    connect(this, &UIOperation::addMsgBox, msgBoxModel, &MsgBoxModel::onAddMsgBox);
    connect(this, &UIOperation::removeMsgBox, msgBoxModel, &MsgBoxModel::onRemoveMsgBox);
}

void UIOperation::showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons)
{
    qDebug("showMessage, title: %s, content: %s", title.toStdString().c_str(),
           content.toStdString().c_str());

    QString uuid = QUuid::createUuid().toString();
    MsgBoxItem msgBox(uuid, title, icon, content, buttons);
    emit addMsgBox(msgBox);
}

void UIOperation::showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QString button)
{
    QList<QString> buttons;
    buttons.append(button);
    showMessage(title, content, icon, buttons);
}

QString UIOperation::getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons)
{
    QString buttonsRepr;
    for(int i = 0; i < buttons.count(); i++)
    {
        buttonsRepr.append(buttons[i]);
        if(i < buttons.count() -1)
        {
            buttonsRepr.append("|");
        }
    }
    qDebug("getUIResponse, title: %s, content: %s, buttons: %s", title.toStdString().c_str(),
           content.toStdString().c_str(), buttonsRepr.toStdString().c_str());

    QString uuid = QUuid::createUuid().toString();
    MsgBoxItem msgBox(uuid, title, icon, content, buttons);
    UIResponse* rsp = new UIResponse;
    uiRsp[uuid] = rsp;
    emit addMsgBox(msgBox);
    QMutex locker;
    locker.lock();
    rsp->waitResponse.wait(&locker);
    locker.unlock();
    uiRsp.remove(uuid);
    QString clickedButton = rsp->clickedButton;
    delete rsp;

    qDebug("getUIResponse, user clicked button: %s", clickedButton.toStdString().c_str());

    return clickedButton;
}

QString UIOperation::getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QString button)
{
    QList<QString> buttons;
    buttons.append(button);
    return getUIResponse(title, content, icon, buttons);
}

void UIOperation::onUIResponse(QString uuid, QString clickedButton)
{
    emit removeMsgBox(uuid);
    if(uiRsp.contains(uuid))
    {
        uiRsp[uuid]->clickedButton = clickedButton;
        uiRsp[uuid]->waitResponse.wakeOne();
    }
}
