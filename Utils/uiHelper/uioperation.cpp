#include "uioperation.h"

UIOperation::UIOperation() {}

void UIOperation::setGrabMouseItem(QQuickItem *item)
{
    grabMouseItem = item;
}

void UIOperation::init(MsgBoxModel *msgBoxModel, Qt::HANDLE mainThreadID)
{
    this->msgBoxModel = msgBoxModel;
    this->mainThreadID = mainThreadID;
    connect(this, &UIOperation::addMsgBox, msgBoxModel, &MsgBoxModel::onAddMsgBox);
    connect(this, &UIOperation::removeMsgBox, msgBoxModel, &MsgBoxModel::onRemoveMsgBox);
}

void UIOperation::showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons)
{
    getLogger(icon) << QString("showMessage, title: %1, content: %2").arg(title).arg(content);

    QString uuid = QUuid::createUuid().toString();
    MsgBoxItem msgBox(uuid, title, icon, content, buttons);
    emit addMsgBox(msgBox);
}

void UIOperation::showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QString button)
{
    showMessage(title, content, icon, button.split(BtnSeperator));
}

QString UIOperation::getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons)
{
    auto logger = getLogger(icon);
    logger << QString("getUIResponse, title: %1, content: %2, buttons: %3")
                  .arg(title)
                  .arg(content)
                  .arg(combineString("|", buttons));

    QString uuid = QUuid::createUuid().toString();
    MsgBoxItem msgBox(uuid, title, icon, content, buttons);
    UIResponse *rsp = new UIResponse;
    uiRsp[uuid] = rsp;
    emit addMsgBox(msgBox);

    if (QThread::currentThreadId() == mainThreadID)
    {
        bool hasGrabMouse = false;
        while (!rsp->gotResponse)
        {
            if (!hasGrabMouse)
            {
                if (grabMouseItem)
                {
                    grabMouseItem->grabMouse();
                    grabMouseItem->ungrabMouse();
                    hasGrabMouse = true;
                }
            }
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }
    else
    {
        QMutex locker;
        locker.lock();
        rsp->waitResponse.wait(&locker);
        locker.unlock();
    }

    QString clickedButton = rsp->clickedButton;
    uiRsp.remove(uuid);
    delete rsp;

    logger << QString("getUIResponse, user clicked button: %1").arg(clickedButton);
    return clickedButton;
}

QString UIOperation::getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QString button)
{
    return getUIResponse(title, content, icon, button.split(BtnSeperator));
}

void UIOperation::onUIResponse(QString uuid, QString clickedButton)
{
    emit removeMsgBox(uuid);
    if (uiRsp.contains(uuid))
    {
        uiRsp[uuid]->clickedButton = clickedButton;
        uiRsp[uuid]->gotResponse = true;
        uiRsp[uuid]->waitResponse.wakeOne();
    }
}
