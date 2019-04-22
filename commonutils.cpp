#include <commonutils.h>
#include <QTime>
#include <config.h>
#include <QDir>
#include <QJsonDocument>
#include <QDebug>

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


// QString >> QJson
QJsonObject getJsonObjectFromString(const QString jsonString){
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< jsonString.toLocal8Bit().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}
// QJson >> QString
QString getStringFromJsonObject(const QJsonObject& jsonObject){
    return QString(QJsonDocument(jsonObject).toJson());
}
