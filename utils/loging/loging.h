#ifndef LOGING_H
#define LOGING_H

#include "rollbackfile.h"
#include "propertybase.h"
#include "logmodel.h"
#include <iostream>
#include <QQmlApplicationEngine>
#include <QQmlContext>


class LogParameter:public PropertyBase
{
    Q_OBJECT


    qint64 m_maxSize = 1024 * 1024 * 3;

    int m_nBackupFile = 20;

    bool m_flushImmediately = true;

    int m_logLevel  = 0;

public:

    explicit LogParameter()
    {
    }

    Q_PROPERTY(qint64 maxSize READ maxSize WRITE setMaxSize)
    Q_PROPERTY(int nBackupFile READ nBackupFile WRITE setNBackupFile)
    Q_PROPERTY(bool flushImmediately READ flushImmediately WRITE setFlushImmediately)
    Q_PROPERTY(int logLevel READ logLevel WRITE setLogLevel)


    qint64 maxSize() const
    {
        return m_maxSize;
    }
    int nBackupFile() const
    {
        return m_nBackupFile;
    }

    bool flushImmediately() const
    {
        return m_flushImmediately;
    }

    int logLevel() const
    {
        return m_logLevel;
    }

public slots:
    void setMaxSize(qint64 maxSize)
    {
        m_maxSize = maxSize;
    }
    void setNBackupFile(int nBackupFile)
    {
        m_nBackupFile = nBackupFile;
    }
    void setFlushImmediately(bool flushImmediately)
    {
        m_flushImmediately = flushImmediately;
    }
    void setLogLevel(int logLevel)
    {
        m_logLevel = logLevel;
    }
};


class LogManager: public QObject
{
    Q_OBJECT

public:
    static void initLogSystem();
    static void disposeLogSystem();
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    Q_PROPERTY(int logLevel READ logLevel WRITE setLogLevel NOTIFY logLevelChanged)

public:
    int logLevel() const
    {
        return logParameter.logLevel();
    }

signals:
    void logLevelChanged(int logLevel);

public slots:
    void setLogLevel(int logLevel)
    {
        if(logLevel != logParameter.logLevel())
        {
            logParameter.setLogLevel(logLevel);
            logParameter.saveJsonConfig(logConfigFileName, QString("LogParameter"));
            emit logLevelChanged(logLevel);
        }
    }

public:
    static LogModel logModel;

private:
    static QString folder;
    static QString allLogFileName;
    static QString warnLogFileName;
    static QString logConfigFileName;
    static RollbackFile allLogFile;
    static RollbackFile warnLogFile;
    static LogParameter logParameter;
    static QMap<QtMsgType, int> msgTypeToLevel;
    static QMutex stdoutLocker;
    static LogBuffer logBuffer;
};


#endif // LOGING_H
