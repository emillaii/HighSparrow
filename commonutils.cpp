#include <commonutils.h>
#include <QTime>
#include <config.h>
#include <QDir>

QString getCurrentTimeString()
{
    QDateTime local = QDateTime::currentDateTime();
    QString timeString = local.toString("HH-mm-ss-zzz");
    return timeString;
}

QString getCurrentDateString()
{
    QDateTime local = QDateTime::currentDateTime();
    QString timeString = local.toString("yyyy-MM-dd");
    return timeString;
}

QString getVisionLogDir()
{
    QString dir_name;
    if (!QDir(BASE_LOG_DIR).exists()) { QDir().mkdir(BASE_LOG_DIR); }
    if (!QDir(QString(BASE_LOG_DIR).append(VISION_PR_DEBUG_DIR)).exists()) {
         QDir().mkdir(QString(BASE_LOG_DIR).append(VISION_PR_DEBUG_DIR));
    }
    if (!QDir(QString(BASE_LOG_DIR).append(VISION_PR_DEBUG_DIR).append(getCurrentDateString())).exists()) {
         QDir().mkdir(QString(BASE_LOG_DIR).append(VISION_PR_DEBUG_DIR).append(getCurrentDateString()));
    }
    return QString(BASE_LOG_DIR).append(VISION_PR_DEBUG_DIR).append(getCurrentDateString()).append("//");
}

QString getGrabberLogDir()
{
    QString dir_name;
    if (!QDir(BASE_LOG_DIR).exists()) { QDir().mkdir(BASE_LOG_DIR); }
    if (!QDir(QString(BASE_LOG_DIR).append(GRABBER_DEBUG_DIR)).exists()) {
         QDir().mkdir(QString(BASE_LOG_DIR).append(GRABBER_DEBUG_DIR));
    }
    if (!QDir(QString(BASE_LOG_DIR).append(GRABBER_DEBUG_DIR).append(getCurrentDateString())).exists()) {
         QDir().mkdir(QString(BASE_LOG_DIR).append(GRABBER_DEBUG_DIR).append(getCurrentDateString()));
    }
    return QString(BASE_LOG_DIR).append(GRABBER_DEBUG_DIR).append(getCurrentDateString()).append("//");
}

QString getMTFLogDir()
{
    QString dir_name;
    if (!QDir(BASE_LOG_DIR).exists()) { QDir().mkdir(BASE_LOG_DIR); }
    if (!QDir(QString(BASE_LOG_DIR).append(MTF_DEBUG_DIR)).exists()) {
         QDir().mkdir(QString(BASE_LOG_DIR).append(MTF_DEBUG_DIR));
    }
    if (!QDir(QString(BASE_LOG_DIR).append(MTF_DEBUG_DIR).append(getCurrentDateString())).exists()) {
         QDir().mkdir(QString(BASE_LOG_DIR).append(MTF_DEBUG_DIR).append(getCurrentDateString()));
    }
    return QString(BASE_LOG_DIR).append(MTF_DEBUG_DIR).append(getCurrentDateString()).append("//");
}
