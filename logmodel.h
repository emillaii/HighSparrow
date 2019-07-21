#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QAbstractTableModel>
#include <QLoggingCategory>


class LogModel : public QAbstractTableModel
{
    Q_OBJECT

    LogModel(QObject *parent = nullptr);

public:
    static LogModel* instance();

    void addLog(const QString &log);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

signals:
    void newLog(const QString& log);

private slots:
    void onNewLog(const QString& log);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<QString> m_logs;
};

#endif // LOGMODEL_H
