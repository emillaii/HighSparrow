#include "loging.h"


QString LogManager::folder("log/system_log");
QString LogManager::allLogFileName("HighSparrowQ");
QString LogManager::warnLogFileName("HighSparrowQWarn");
QString LogManager::logConfigFileName("logParameter.json");
RollbackFile LogManager::allLogFile;
RollbackFile LogManager::warnLogFile;
LogParameter LogManager::logParameter;
QMap<QtMsgType, int> LogManager::msgTypeToLevel;
QMutex LogManager::stdoutLocker;
LogBuffer LogManager::logBuffer;
LogModel LogManager::logModel;

void LogManager::initLogSystem()
{
    msgTypeToLevel[QtDebugMsg] = 0;
    msgTypeToLevel[QtInfoMsg] = 1;
    msgTypeToLevel[QtWarningMsg] = 2;
    msgTypeToLevel[QtCriticalMsg] = 3;
    msgTypeToLevel[QtFatalMsg] = 4;

    qRegisterMetaType<QList<LogItem*>>();

    logBuffer.startThd();
    connect(&logBuffer, &LogBuffer::logBufferChanged, &logModel, &LogModel::onLogBufferChanged);

    if(QFile::exists(logConfigFileName) &&
            logParameter.loadJsonConfig(logConfigFileName, QString("LogParameter")))
    {

    }
    else {
        logParameter.saveJsonConfig(logConfigFileName, QString("LogParameter"));
    }
    allLogFile.init(folder, allLogFileName, logParameter.maxSize(), logParameter.nBackupFile(), logParameter.flushImmediately());
    warnLogFile.init(folder, warnLogFileName, logParameter.maxSize(), logParameter.nBackupFile(), logParameter.flushImmediately());
    qInstallMessageHandler(messageHandler);
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} [%{type}] %{file}:%{line}(%{function}):%{message}");
}

void LogManager::disposeLogSystem()
{
    logBuffer.stopThd();
}

void LogManager::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(msgTypeToLevel[type] < logParameter.logLevel())
        return;

    QString log = qFormatLogMessage(type, context, msg);

    QTextCodec *gbk = QTextCodec::codecForName("gbk");
    {
        QMutexLocker tmpLocker(&stdoutLocker);
        if(msgTypeToLevel[type] >= 2)
        {
            std::cerr << gbk->fromUnicode(log).toStdString().c_str() << std::endl;
        }
        else {
            std::cout << gbk->fromUnicode(log).toStdString().c_str() << std::endl;
        }
    }

    allLogFile.appendLine(log);
    if(msgTypeToLevel[type] >= 2)
    {
        warnLogFile.appendLine(log);
    }
    logBuffer.addLog(msgTypeToLevel[type], log);
}
