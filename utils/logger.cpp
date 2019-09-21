#include "utils/logger.h"
#include "logmodel.h"
#include "config.h"
#include <QDateTime>
#include <QDir>
#include <QThread>
#include <iostream>
#include <QMutex>
#include <qtextcodec.h>
// declear a motion logging category:
// QLoggingCategory motionLog("motion");

static QFile logFile;
static QTextStream logStream;
QMutex mutex;
bool isDisplayDebug = false;
bool isDisplayInfo = false;
void initLoggingSystem()
{
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss.zzz} [%{category}] [%{type}]: %{message} (%{function})%{file}:%{line}");

    if (!QDir(BASE_LOG_DIR).exists())
    {
        QDir().mkdir(BASE_LOG_DIR);
    }

    if (!QDir(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR)).exists())
    {
        QDir().mkdir(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR));
    }

    if (!QDir(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR).append(QDateTime::currentDateTime().toString("yyyy-MM-dd"))).exists())
    {
        QDir().mkdir(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR).append(QDateTime::currentDateTime().toString("yyyy-MM-dd")));
    }

    QString filename = QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR).append(QDateTime::currentDateTime().toString("yyyy-MM-dd")).append("\\log.txt");
    logFile.setFileName(filename);
    if(logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        logStream.setDevice(&logFile);
        logStream.setCodec(QTextCodec::codecForName("utf8"));
    }
}

void messageLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&mutex);
    QString typeStr;
    if(type == QtDebugMsg)
    {
        typeStr = "Debug";
    }
    else if(type == QtWarningMsg)
    {
        typeStr = "Warning";
    }
    else if(type == QtCriticalMsg)
    {
        typeStr = "Critical";
    }
    else if(type == QtFatalMsg)
    {
        typeStr = "Fatal";
    }
    else if(type == QtInfoMsg)
    {
        typeStr = "Info";
    }

    // format log text
    QString fileName = context.file;
    int index = fileName.lastIndexOf(QDir::separator());
    fileName = fileName.mid(index + 1);
    QString log = qFormatLogMessage(type, context, msg);

    QTextCodec *gbk = QTextCodec::codecForName("gbk");
    std::cout << gbk->fromUnicode(log).toStdString().c_str() << std::endl; // print to system console
    //LogModel::instance()->addLog(log); // add to log model
    logStream << log << endl; // stream to log file
    if (type == QtInfoMsg && isDisplayInfo)
        LogModel::instance()->addLog(log); // add to log model
}

void setLoggingEnabled(QLoggingCategory& cat, bool enable)
{
    cat.setEnabled(QtInfoMsg, enable);
}

bool isLoggingEnabled(QLoggingCategory& cat)
{
    return cat.isEnabled(QtInfoMsg);
}

void setLoggerDisplayLog(QtMsgType type, bool on)
{
    if(type == QtDebugMsg)
    {
        isDisplayDebug = on;
    }
//    else if(type == QtWarningMsg)
//    {
//        typeStr = "Warning";
//    }
//    else if(type == QtCriticalMsg)
//    {
//        typeStr = "Critical";
//    }
//    else if(type == QtFatalMsg)
//    {
//        typeStr = "Fatal";
//    }
    else if(type == QtInfoMsg)
    {
        isDisplayInfo = on;
        qInfo("Display log info : %d", isDisplayInfo);
    }
}
