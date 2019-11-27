#include "msgboxmodel.h"

MsgBoxModel::MsgBoxModel()
{
    msgBoxRoleNames[MsgBoxRole::UuidRole] = "uuid";
    msgBoxRoleNames[MsgBoxRole::TitleRole] = "title";
    msgBoxRoleNames[MsgBoxRole::IconRole] = "icon";
    msgBoxRoleNames[MsgBoxRole::ContentRole] = "content";
    msgBoxRoleNames[MsgBoxRole::ButtonsRole] = "buttons";
}

int MsgBoxModel::rowCount(const QModelIndex &parent) const
{
    return msgBoxes.count();
}

QVariant MsgBoxModel::data(const QModelIndex &index, int role) const
{
    if(index.row() >= 0 && index.row() < msgBoxes.count())
    {
        switch (role) {
        case MsgBoxRole::UuidRole:
            return QVariant(msgBoxes[index.row()].uuid);
        case MsgBoxRole::TitleRole:
            return QVariant(msgBoxes[index.row()].title);
        case MsgBoxRole::IconRole:
            return QVariant(msgBoxes[index.row()].icon);
        case MsgBoxRole::ContentRole:
            return QVariant(msgBoxes[index.row()].content);
        case MsgBoxRole::ButtonsRole:
            return QVariant(msgBoxes[index.row()].buttons);
        }
    }
    return QVariant();
}

QHash<int, QByteArray> MsgBoxModel::roleNames() const
{
    return msgBoxRoleNames;
}

void MsgBoxModel::onAddMsgBox(MsgBoxItem msgBox)
{
    beginInsertRows(QModelIndex(), msgBoxes.count(), msgBoxes.count());
    msgBoxes.append(msgBox);
    endInsertRows();
    emit msgBoxCountChanged(msgBoxes.count());
}

void MsgBoxModel::onRemoveMsgBox(QString uuid)
{
    int index = -1;
    for (int i = 0; i < msgBoxes.count(); i++) {
        if(msgBoxes[i].uuid == uuid)
        {
            index = i;
            break;
        }
    }
    if(index != -1)
    {
        beginRemoveRows(QModelIndex(), index, index);
        msgBoxes.removeAt(index);
        endRemoveRows();
        emit msgBoxCountChanged(msgBoxes.count());
    }
}
