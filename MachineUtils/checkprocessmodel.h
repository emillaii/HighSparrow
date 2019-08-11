#ifndef CHECKPROCESSMODEL_H
#define CHECKPROCESSMODEL_H

class CheckProcessItem;

#include <QList>
#include <QAbstractItemModel>



class CheckProcessModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    CheckProcessModel(QObject *parent=nullptr);
    ~CheckProcessModel();


    enum CheckProcessRoles{
        ItemName= Qt::UserRole +1,
        Operate
    };
    int rowCount(const QModelIndex& parent=QModelIndex())const;
    int columnCount(const QModelIndex &parent = QModelIndex())const;
    QModelIndex index(int row,int column,const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index,int role)const;
    QModelIndex parent(const QModelIndex& index) const;
    QHash<int,QByteArray>roleNames()const;

private:
    QHash<int,QByteArray> mRoleNames;
    QList<CheckProcessItem*> mRootEntrys;
    void _addEntryNode(CheckProcessItem* node,CheckProcessItem* parent=0);
};

#endif // CHECKPROCESSMODEL_H
