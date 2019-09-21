#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QLoggingCategory>

// Logging can be grouped into several modules, below is an example of how to declear a module
// Logging module can be enabled or disabled independently.
// We can call qCInfo(module) << msg to add a msg to a logging module,
// qInfo() << msg will add the log message to the default module

// declear a motion logging category:
// extern QLoggingCategory motionLog;

void initLoggingSystem();
void setLoggerDisplayLog(QtMsgType type, bool on);
void messageLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void setLoggingEnabled(QLoggingCategory& cat, bool enable);
bool isLoggingEnabled(QLoggingCategory& cat);

//void sparrowLogOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // LOGGER_H
