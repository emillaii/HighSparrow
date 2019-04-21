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

QString getDir(QString dir)
{
    QString dir_name;
    if (!QDir(BASE_LOG_DIR).exists()) { QDir().mkdir(BASE_LOG_DIR); }
    if (!QDir(QString(BASE_LOG_DIR).append(dir)).exists()) {
         QDir().mkdir(QString(BASE_LOG_DIR).append(dir));
    }
    if (!QDir(QString(BASE_LOG_DIR).append(dir).append(getCurrentDateString())).exists()) {
         QDir().mkdir(QString(BASE_LOG_DIR).append(dir).append(getCurrentDateString()));
    }
    return QString(BASE_LOG_DIR).append(dir).append(getCurrentDateString()).append("//");
}

QString getVisionLogDir(){ return getDir(VISION_PR_DEBUG_DIR); }
QString getGrabberLogDir(){ return getDir(GRABBER_DEBUG_DIR); }
QString getMTFLogDir(){ return getDir(MTF_DEBUG_DIR); }

void writeFile(QString data, QString dir, QString filename)
{
    QString target;
    target.append(getMTFLogDir())
            .append(getCurrentTimeString())
            .append("_").append(filename);
    QFile file(target);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(data.toStdString().c_str());
    file.close();
}
