#include "logmodel.h"


#include <iostream>


LogBuffer::LogBuffer()
{
    levelToColor[0] = "white";
    levelToColor[1] = "white";
    levelToColor[2] = "yellow";
    levelToColor[3] = "tomato";
    levelToColor[4] = "tomato";
}

void LogBuffer::addLog(int level, QString &log)
{
    QMutexLocker tmpLocker(&logBufferLocker);
    logBuffer.append(new LogItem{levelToColor[level], log});
}

void LogBuffer::stopThd()
{
    isRun = false;
    this->wait();
}

void LogBuffer::startThd()
{
    isRun = true;
    this->start();
}

void LogBuffer::run()
{
    while (isRun) {

        msleep(DETECT_NEW_LOG_INTERVAL);

        QMutexLocker tmpLocker(&logBufferLocker);
        if(logBuffer.count() == 0)
            continue;
        emit logBufferChanged(logBuffer);
        logBuffer.clear();
    }
}



LogModel::LogModel()
{
    logRoleNames[LogModelColorRole] = "logColor";
    logRoleNames[LogModelLogRole] = "logStr";
}

void LogModel::onLogBufferChanged(QList<LogItem *> logBuffer)
{
    if(logs.count() > MAX_LINES)
    {
        removeFrontRows(N_REMOVE_LINES);
    }

    beginInsertRows(QModelIndex(), logs.count(), logs.count() + logBuffer.count() - 1);
    foreach(auto logItem, logBuffer)
    {
        logs.append(logItem);
    }
    endInsertRows();
}

void LogModel::onClearLog()
{
    if(logs.count() > 0)
    {
        removeFrontRows(logs.count());
    }
}

void LogModel::removeFrontRows(int rowCount)
{
    beginRemoveRows(QModelIndex(), 0, rowCount - 1);
    for (int i = 0; i < rowCount; i++) {
        LogItem* logItem = logs.last();
        logs.removeLast();
        delete logItem;
    }
    endRemoveRows();
}
