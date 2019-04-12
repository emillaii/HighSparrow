#ifndef HIGHSPRROW_H
#define HIGHSPRROW_H
#include <QObject>
#include "errorcode.h"
#include "aaheadmodule.h"
#include "sut_module.h"
#include "logicmanager.h"
#include "basemodulemanager.h"
#include "STD.h"

class HighSprrow : public QObject
{
    Q_OBJECT
public:
    HighSprrow();
    ~HighSprrow();
private:
    ErrorCodeStruct InitComponent();
public:
    BaseModuleManager * baseModuleManager = Q_NULLPTR;
    LogicManager * logicManager = Q_NULLPTR;
public:
    Q_INVOKABLE void init();
    Q_INVOKABLE void home();
    Q_INVOKABLE void performSUTDownlookPR();
    Q_INVOKABLE void performLUTUplookPR();
    Q_INVOKABLE void callQProcess(QString arg);
public slots:
    void homeAllMotors();
};

#endif // HIGHSPRROW_H
