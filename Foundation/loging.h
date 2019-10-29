#ifndef LOGING_H
#define LOGING_H

#include "rollbackfile.h"
#include "propertybase.h"
#include <iostream>
#include "Utils/config.h"


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


static QString folder(QString(BASE_LOG_DIR) + QString(SYSTEM_LOG_DIR));
static QString allLogFileName("HighSparrowQ");
static QString warnLogFileName("HighSparrowQWarn");

static RollbackFile allLogFile;
static RollbackFile warnLogFile;
static LogParameter logParameter;
static QString logConfigFileName(QString(CONFIG_DIR) + QString(LOG_PARAM_FILE));
static QMap<QtMsgType, int> msgTypeToLevel;
static QMutex stdoutLocker;


void initLogSystem();
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void changeLogLevel(int level);



#endif // LOGING_H
