#include "mysqltablemodel.h"

MySqlTableModel::MySqlTableModel(QObject *parent, QSqlDatabase db): QSqlTableModel (parent, db)
{

}

QVariant MySqlTableModel::get(int rowIndex, QString fieldName)
{
    if(rowIndex >= 0 && rowIndex < rowCount())
    {
        return record(rowIndex).value(fieldName);
    }
    return QVariant();
}

QVariant MySqlTableModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if(role < Qt::UserRole)
    {
        value = QSqlTableModel::data(index, role);
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlTableModel::data(modelIndex, Qt::DisplayRole);
    }

    return value;
}

QHash<int, QByteArray> MySqlTableModel::roleNames() const
{
    QHash<int, QByteArray> _roleNames;
    for(int i = 0; i < record().count(); i++)
    {
        _roleNames[Qt::UserRole + 1 + i] = record().fieldName(i).toUtf8();
    }
    return _roleNames;
}
