#ifndef CHECKPROCESSITEM_H
#define CHECKPROCESSITEM_H
#include <QList>
#include <QVariant>
#include <QVector>
class CheckProcessItem;
typedef CheckProcessItem* PCheckProcessItem;
class CheckProcessItem{
public:
    explicit CheckProcessItem();
    PCheckProcessItem parent() const;
    void setParent(const PCheckProcessItem parent);
    void appendNode(PCheckProcessItem node);
    void removeNode(int row);
    void setData(int role,QVariant value);
    QVariant data(int role);
    QList<PCheckProcessItem> childs;
private:
    PCheckProcessItem mParent=NULL;
    QHash<int,QVariant>mRecord;
};

#endif // CHECKPROCESSITEM_H
