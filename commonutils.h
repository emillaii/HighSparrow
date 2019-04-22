#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QString>
#include <QJsonObject>

void createAllDir();
QString getCurrentTimeString();
QString getCurrentDateString();
QString getVisionLogDir();
QString getGrabberLogDir();
QString getMTFLogDir();
QString getDir(QString);

void writeFile(QString data, QString dir, QString filename);
QJsonObject getJsonObjectFromString(const QString jsonString);
QString getStringFromJsonObject(const QJsonObject& jsonObject);

#endif // COMMONUTILS_H
