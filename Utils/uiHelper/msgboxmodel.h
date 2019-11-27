#ifndef MSGBOXMODEL_H
#define MSGBOXMODEL_H

#include <QObject>
#include <QAbstractListModel>

enum MsgBoxRole
{
    UuidRole = Qt::UserRole + 1,
    TitleRole,
    IconRole,
    ContentRole,
    ButtonsRole
};

class MsgBoxItem;

class MsgBoxModel: public QAbstractListModel
{
    Q_OBJECT

public:
    enum MsgBoxIcon
    {
        Question,
        Information,
        Warning,
        Error
    };
    Q_ENUMS(MsgBoxIcon)

    MsgBoxModel();

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

public slots:
    void onAddMsgBox(MsgBoxItem msgBox);
    void onRemoveMsgBox(QString uuid);

signals:
    void msgBoxCountChanged(int count);

private:
    QList<MsgBoxItem> msgBoxes;
    QHash<int, QByteArray> msgBoxRoleNames;
};

Q_DECLARE_METATYPE(MsgBoxModel::MsgBoxIcon)

class MsgBoxItem
{
public:
    MsgBoxItem()
    {

    }

    MsgBoxItem(QString uuid, QString title, MsgBoxModel::MsgBoxIcon icon, QString content, QList<QString> buttons)
    {
        this->uuid = uuid;
        this->title = title;
        this->icon = icon;
        this->content = content;
        this->buttons = buttons;
    }

public:
    QString uuid;
    QString title;
    MsgBoxModel::MsgBoxIcon icon;
    QString content;
    QList<QString> buttons;
};

Q_DECLARE_METATYPE(MsgBoxItem)

#endif // MSGBOXMODEL_H
