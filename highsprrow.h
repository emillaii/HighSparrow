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
    Q_INVOKABLE void performLUTMushroomPR();
    Q_INVOKABLE void performSUTDownlookPR();
    Q_INVOKABLE void performLUTUplookPR();
    Q_INVOKABLE void callQProcess(QString arg);
    Q_INVOKABLE void aa_A_Inter(double step);
    Q_INVOKABLE void aa_B_Inter(double step);
public slots:
    void homeAllMotors();
};

#endif // HIGHSPRROW_H
