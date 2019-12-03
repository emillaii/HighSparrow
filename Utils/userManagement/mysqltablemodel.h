#ifndef USERTABLEMODEL_H
#define USERTABLEMODEL_H

#include <QObject>
#include <qsqltablemodel.h>
#include <qsqlrecord.h>
#include <qsqlfield.h>


class MySqlTableModel: public QSqlTableModel
{
    Q_OBJECT

public:
    MySqlTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    Q_INVOKABLE QVariant get(int rowIndex, QString fieldName);

    // QAbstractItemModel interface
public:
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
};

#endif // USERTABLEMODEL_H
