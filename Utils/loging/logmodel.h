#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QAbstractListModel>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>
#include <QThread>

class LogItem
{
public:
    QString color;
    QString log;
};

Q_DECLARE_METATYPE(QList<LogItem*>)

class LogBuffer:public QThread
{
    Q_OBJECT

public:
    explicit LogBuffer();

    void addLog(int level, QString& log);
    void stopThd();
    void startThd();

signals:
    void logBufferChanged(QList<LogItem*> logBuffer);

protected:
    void run() override;

private:
    bool isRun;
    const ulong DETECT_NEW_LOG_INTERVAL = 180;
    QList<LogItem*> logBuffer;
    QHash<int, QString> levelToColor;
    QMutex logBufferLocker;
};

enum {
    LogModelColorRole = Qt::UserRole + 1,
    LogModelLogRole
};



class LogModel: public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LogModel();

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return logs.count();
    }

    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (index.row() >= 0 && index.row() < logs.count() )
        {
            if (role == LogModelColorRole)
            {
                return QVariant(logs[index.row()]->color);
            }
            if (role == LogModelLogRole)
            {
                return QVariant(logs[index.row()]->log);
            }
        }
        return QVariant();
    }

    virtual QHash<int,QByteArray> roleNames() const override
    {
        return logRoleNames;
    }

public slots:
    void onLogBufferChanged(QList<LogItem*> logBuffer);
    void onClearLog();

private:
    void removeFrontRows(int rowCount);

private:
    const int MAX_LINES = 1500;
    const int N_REMOVE_LINES = 500;
    QList<LogItem*> logs;
    QHash<int, QByteArray> logRoleNames;
};

#endif // LOGMODEL_H
