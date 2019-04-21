#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QString>
/*
 * This function will return time string in "11-14-499"
 */

void createAllDir();
QString getCurrentTimeString();
QString getCurrentDateString();
QString getVisionLogDir();
QString getGrabberLogDir();
QString getMTFLogDir();
QString getDir(QString);

void writeFile(QString data, QString dir, QString filename);

#endif // COMMONUTILS_H
