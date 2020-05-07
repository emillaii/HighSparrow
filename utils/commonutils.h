#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QString>
#include <QJsonObject>
#include <opencv/cv.h>

void createAllDir();
QString getCurrentTimeString();
QString getCurrentDateString();
QString getVisionLogDir();
QString getGrabberLogDir();
QString getMTFLogDir();
QString getUnitLogDir();
QString getAlarmLogDir();
QString getRejectLogDir();
QString getParticalCheckDir();
QString getAALogDir();
QString getDispensePrLogDir();
QString getYLevelDir();
QString getPerformanceLogDir();
QString getAAProcessLogDir();
QString getProduceProcessLogDir();
QString getShareDir();
QString getDir(QString);

void writeFile(QString data, QString dir, QString filename);
QJsonObject getJsonObjectFromString(const QString jsonString);
QString getStringFromJsonObject(const QJsonObject& jsonObject);
QString getStringFromQvariantMap(const QVariantMap& qvariantMap);
bool fitCircle(const std::vector<cv::Point2d> &pnts, cv::Point2d &center, double &radius);

#endif // COMMONUTILS_H
