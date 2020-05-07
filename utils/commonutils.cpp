#include <utils/commonutils.h>
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
QString getShareDir(){ return getDir(SHARE_DIR); }
QString getGrabberLogDir(){ return getDir(GRABBER_DEBUG_DIR); }
QString getMTFLogDir(){ return getDir(MTF_DEBUG_DIR); }
QString getUnitLogDir(){ return getDir(UNIT_LOG_DIR); }
QString getAALogDir(){ return getDir(AA_LOG_DIR); }
QString getDispensePrLogDir() { return getDir(DISPENSE_VISION_PR_DIR); }
QString getYLevelDir() { return getDir(Y_LEVEL_DIR); }
QString getAlarmLogDir() { return getDir(ALARM_LOG_DIR); }
QString getRejectLogDir() { return getDir(REJECT_LOG_DIR); }
QString getParticalCheckDir() { return getDir(PARTICAL_CHECK_DIR); }

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
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8().data());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< jsonString.toUtf8().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}
// QJson >> QString
QString getStringFromJsonObject(const QJsonObject& jsonObject){
    return QString(QJsonDocument(jsonObject).toJson());
}

bool fitCircle(const std::vector<cv::Point2d> &points, cv::Point2d &center, double &radius)
{
     center.x = 0.0f;
     center.y = 0.0f;
     radius = 0.0f;
     if (points.size() < 3)
     {
         return false;
     }

     double sum_x = 0.0f, sum_y = 0.0f;
     double sum_x2 = 0.0f, sum_y2 = 0.0f;
     double sum_x3 = 0.0f, sum_y3 = 0.0f;
     double sum_xy = 0.0f, sum_x1y2 = 0.0f, sum_x2y1 = 0.0f;

     int N = points.size();
     for (int i = 0; i < N; i++)
     {
         double x = points[i].x;
         double y = points[i].y;
         double x2 = x * x;
         double y2 = y * y;
         sum_x += x;
         sum_y += y;
         sum_x2 += x2;
         sum_y2 += y2;
         sum_x3 += x2 * x;
         sum_y3 += y2 * y;
         sum_xy += x * y;
         sum_x1y2 += x * y2;
         sum_x2y1 += x2 * y;
     }

     double C, D, E, G, H;
     double a, b, c;

     C = N * sum_x2 - sum_x * sum_x;
     D = N * sum_xy - sum_x * sum_y;
     E = N * sum_x3 + N * sum_x1y2 - (sum_x2 + sum_y2) * sum_x;
     G = N * sum_y2 - sum_y * sum_y;
     H = N * sum_x2y1 + N * sum_y3 - (sum_x2 + sum_y2) * sum_y;
     a = (H * D - E * G) / (C * G - D * D);
     b = (H * C - E * D) / (D * D - G * C);
     c = -(a * sum_x + b * sum_y + sum_x2 + sum_y2) / N;

     center.x = a / (-2);
     center.y = b / (-2);
     radius = sqrt(a * a + b * b - 4 * c) / 2;
     return true;

}

QString getPerformanceLogDir(){ return getDir(PERFORMANCE_DIR); }

QString getAAProcessLogDir(){ return getDir(AA_PROCESS_DIR); }

QString getProduceProcessLogDir(){ return getDir(PRODUCE_PROCESS_DIR); }

QString getStringFromQvariantMap(const QVariantMap &qvariantMap)
{
    return QString(QJsonDocument(QJsonObject::fromVariantMap(qvariantMap)).toJson());
}
