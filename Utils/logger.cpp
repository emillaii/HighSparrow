#include "Utils/logger.h"
#include "Utils/commonutils.h"
#include "Utils/config.h"
#include <stdio.h>
#include <fstream>
#include <QDateTime>
#include <QDir>

static int line_count = 0;
static std::ofstream log_file;

void sparrowLogOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString log_msg = qFormatLogMessage(type, context, msg);
    if (!QDir(BASE_LOG_DIR).exists()) {
        QDir().mkdir(BASE_LOG_DIR);
    }
    if (!QDir(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR)).exists()) {
        QDir().mkdir(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR));
    }
    if (!log_file.is_open()) {
        struct stat buffer;
        if (stat(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR).append("\\log.txt").toStdString().c_str(), &buffer) == 0) {
            QDateTime now(QDateTime::currentDateTime());
            QString newname = QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR) + "\\log_" + now.toString("yyyy-MM-dd-HH-mm-ss-zzz") + ".txt";
            QDir r;
            r.rename(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR).append("\\log.txt"), newname);
        }
        log_file.open(QString(BASE_LOG_DIR).append(SYSTEM_LOG_DIR).append("\\log.txt").toStdString().c_str());
    }
    ++line_count;
    log_file << qPrintable(log_msg) << std::endl;
    //log_file.close();
//    if (log_ui) {
//        emit log_ui->add_log(log_msg);
//    }
}
