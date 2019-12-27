#include "loging.h"


QString LogManager::folder("log/system_log");
QString LogManager::allLogFileName("HighSparrowQ");
QString LogManager::warnLogFileName("HighSparrowQWarn");
LogConfig* LogManager::logParameter = nullptr;
RollbackFile LogManager::allLogFile;
RollbackFile LogManager::warnLogFile;
QMap<QtMsgType, int> LogManager::msgTypeToLevel;
QMutex LogManager::stdoutLocker;
LogBuffer LogManager::logBuffer;
LogModel LogManager::logModel;

void LogManager::initLogSystem(LogConfig* _logParameter)
{
    if(_logParameter == nullptr){
        qFatal("LogParameter is null!");
        return;
    }
    logParameter = _logParameter;
    msgTypeToLevel[QtDebugMsg] = 0;
    msgTypeToLevel[QtInfoMsg] = 1;
    msgTypeToLevel[QtWarningMsg] = 2;
    msgTypeToLevel[QtCriticalMsg] = 3;
    msgTypeToLevel[QtFatalMsg] = 4;

    qRegisterMetaType<QList<LogItem*>>();

    logBuffer.startThd();
    connect(&logBuffer, &LogBuffer::logBufferChanged, &logModel, &LogModel::onLogBufferChanged);

    allLogFile.init(folder, allLogFileName, logParameter->maxSize(), logParameter->nBackupFile(), logParameter->flushImmediately());
    warnLogFile.init(folder, warnLogFileName, logParameter->maxSize(), logParameter->nBackupFile(), logParameter->flushImmediately());
    qInstallMessageHandler(messageHandler);
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} [%{type}] %{file}:%{line}(%{function}):%{message}");
}

void LogManager::disposeLogSystem()
{
    logBuffer.stopThd();
    qInstallMessageHandler(nullptr);
}

void LogManager::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(msgTypeToLevel[type] < logParameter->logLevel())
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
