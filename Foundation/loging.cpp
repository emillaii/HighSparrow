#include "loging.h"



void initLogSystem()
{
    msgTypeToLevel[QtDebugMsg] = 0;
    msgTypeToLevel[QtInfoMsg] = 1;
    msgTypeToLevel[QtWarningMsg] = 2;
    msgTypeToLevel[QtCriticalMsg] = 3;
    msgTypeToLevel[QtFatalMsg] = 4;

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

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(msgTypeToLevel[type] < logParameter.logLevel())
        return;

    QString log = qFormatLogMessage(type, context, msg);

    QTextCodec *gbk = QTextCodec::codecForName("gbk");
    {
        QMutexLocker tmpLocker(&stdoutLocker);
        std::cout << gbk->fromUnicode(log).toStdString().c_str() << std::endl;
    }

    allLogFile.appendLine(log);
    if(msgTypeToLevel[type] >= 2)
    {
        warnLogFile.appendLine(log);
    }
}

void changeLogLevel(int level)
{
    logParameter.setLogLevel(level);
    logParameter.saveJsonConfig(logConfigFileName, QString("LogParameter"));
}
