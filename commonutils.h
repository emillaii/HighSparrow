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

#endif // COMMONUTILS_H
