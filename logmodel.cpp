﻿#include "logmodel.h"

LogModel::LogModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_logs()
{
}

LogModel* LogModel::instance()
{
    static LogModel* logModel = new LogModel;
    return logModel;
}

void LogModel::addLog(const QString &log)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_logs << log;
    endInsertRows();
}

int LogModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_logs.count();
}

QVariant LogModel::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() >= m_logs.count())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_logs[index.row()];

    return QVariant();
}

QHash<int, QByteArray> LogModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "logString";
    return roles;
}
