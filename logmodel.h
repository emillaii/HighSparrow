#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QAbstractListModel>
#include <QLoggingCategory>


class LogModel : public QAbstractListModel
{
    Q_OBJECT

    LogModel(QObject *parent = nullptr);

public:
    static LogModel* instance();

    void addLog(const QString &log);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<QString> m_logs;
};

#endif // LOGMODEL_H
