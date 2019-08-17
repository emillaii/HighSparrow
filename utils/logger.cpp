#include "utils/logger.h"
#include "logmodel.h"
#include "config.h"
#include <QDateTime>
#include <QDir>
#include <QThread>
#include <iostream>
#include <QMutex>
// declear a motion logging category:
// QLoggingCategory motionLog("motion");

static QFile logFile;
static QTextStream logStream;
QMutex mutex;

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

    std::cout << log.toStdString().c_str() << std::endl; // print to system console
    //LogModel::instance()->addLog(log); // add to log model
    logStream << log << endl; // stream to log file
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


//#include <utils/commonutils.h>
//#include <config.h>
//#include <stdio.h>
//#include <fstream>
//#include <QDateTime>
//#include <QDir>

//static int line_count = 0;
//static std::ofstream log_file;

//void sparrowLogOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
//{
//    QString log_msg = qFormatLogMessage(type, context, msg);
//    if (!QDir(BASE_LOG_DIR).exists()) {
//        QDir().mkdir(BASE_LOG_DIR);
//    }
//    if (!QDir(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR)).exists()) {
//        QDir().mkdir(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR));
//    }
//    if (!log_file.is_open()) {
//        struct stat buffer;
//        if (stat(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR).append("\\log.txt").toStdString().c_str(), &buffer) == 0) {
//            QDateTime now(QDateTime::currentDateTime());
//            QString newname = QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR) + "\\log_" + now.toString("yyyy-MM-dd-HH-mm-ss-zzz") + ".txt";
//            QDir r;
//            r.rename(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR).append("\\log.txt"), newname);
//        }
//        log_file.open(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR).append("\\log.txt").toStdString().c_str());
//    }
//    ++line_count;
//    log_file << qPrintable(log_msg) << std::endl;
//    //log_file.close();
////    if (log_ui) {
////        emit log_ui->add_log(log_msg);
////    }
//}
