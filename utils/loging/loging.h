#ifndef LOGING_H
#define LOGING_H

#include <iostream>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "rollbackfile.h"
#include "logmodel.h"
#include "logconfig.h"


class LogManager: public QObject
{
    Q_OBJECT

public:
    static void initLogSystem(LogConfig* _logParameter);
    static void disposeLogSystem();

private:
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

public:
    static LogModel logModel;

private:
    static QString folder;
    static QString allLogFileName;
    static QString warnLogFileName;
    static LogConfig* logParameter;
    static RollbackFile allLogFile;
    static RollbackFile warnLogFile;
    static QMap<QtMsgType, int> msgTypeToLevel;
    static QMutex stdoutLocker;
    static LogBuffer logBuffer;
};


#endif // LOGING_H
