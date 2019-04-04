#ifndef HIGHSPRROW_H
#define HIGHSPRROW_H
#include <QObject>
#include "errorcode.h"
#include "aaheadmodule.h"
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
    AAHeadModule * aaHeadModule = Q_NULLPTR;
    BaseModuleManager * baseModuleManager = Q_NULLPTR;
    LogicManager * logicManager = Q_NULLPTR;

public:
    Q_INVOKABLE void init();
    Q_INVOKABLE void home();
public slots:
    void homeAllMotors();
};

#endif // HIGHSPRROW_H
