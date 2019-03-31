#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTime>

void sparrowLogOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // LOGGER_H
