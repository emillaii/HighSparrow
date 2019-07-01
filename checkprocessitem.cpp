#include "checkprocessitem.h"

CheckProcessItem::CheckProcessItem()
{

}

PCheckProcessItem CheckProcessItem::parent() const
{
    return mParent;
}

void CheckProcessItem::setParent(const PCheckProcessItem parent)
{
    this->mParent = parent;
}

void CheckProcessItem::appendNode(PCheckProcessItem node)
{
    childs.append(node);
}

void CheckProcessItem::removeNode(int row)
{
    childs.removeAt(row);
}

void CheckProcessItem::setData(int role, QVariant value)
{
    mRecord[role] = value;
}

QVariant CheckProcessItem::data(int role)
{
    return mRecord[role];
}
