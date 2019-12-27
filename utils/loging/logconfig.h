#ifndef LOGPARAMETER_H
#define LOGPARAMETER_H

#include "../configManager/configfile.h"

class LogConfig:public ConfigObject
{
    Q_OBJECT


    qint64 m_maxSize = 1024 * 1024 * 3;

    int m_nBackupFile = 20;

    bool m_flushImmediately = true;

    int m_logLevel  = 0;

public:

    explicit LogConfig()
    {
        init();
    }

    Q_PROPERTY(qint64 maxSize READ maxSize WRITE setMaxSize NOTIFY maxSizeChanged)
    Q_PROPERTY(int nBackupFile READ nBackupFile WRITE setNBackupFile NOTIFY nBackupFileChanged)
    Q_PROPERTY(bool flushImmediately READ flushImmediately WRITE setFlushImmediately NOTIFY flushImmediatelyChanged)
    Q_PROPERTY(int logLevel READ logLevel WRITE setLogLevel NOTIFY logLevelChanged)

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
        if (m_maxSize == maxSize)
            return;

        m_maxSize = maxSize;
        emit maxSizeChanged(m_maxSize);
    }
    void setNBackupFile(int nBackupFile)
    {
        if (m_nBackupFile == nBackupFile)
            return;

        m_nBackupFile = nBackupFile;
        emit nBackupFileChanged(m_nBackupFile);
    }

    void setFlushImmediately(bool flushImmediately)
    {
        if (m_flushImmediately == flushImmediately)
            return;

        m_flushImmediately = flushImmediately;
        emit flushImmediatelyChanged(m_flushImmediately);
    }

    void setLogLevel(int logLevel)
    {
        if (m_logLevel == logLevel)
            return;

        m_logLevel = logLevel;
        emit logLevelChanged(m_logLevel);
    }

signals:
    void maxSizeChanged(qint64 maxSize);
    void nBackupFileChanged(int nBackupFile);
    void flushImmediatelyChanged(bool flushImmediately);
    void logLevelChanged(int logLevel);
};

#endif // LOGPARAMETER_H
