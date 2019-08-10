#ifndef HIGHSPRROW_H
#define HIGHSPRROW_H
#include <QObject>
#include "Utils/errorcode.h"
#include "AAHeadModule/aaheadmodule.h"
#include "DualHead/sut_module.h"
#include "logicmanager.h"
#include "basemodulemanager.h"
#include "STD.h"
#include "workers_manager.h"

class HighSprrow : public QObject
{
    Q_OBJECT
public:
    HighSprrow();
    ~HighSprrow();

public:
    WorkersManager* worker_manager;
    BaseModuleManager * baseModuleManager = Q_NULLPTR;
    LogicManager * logicManager = Q_NULLPTR;

signals:
    void displaySfrImageInUI();
    void displayOCImageInUI();
public slots:
    Q_INVOKABLE void receiveImageFromAACore(int type) {
        qInfo("Display SFR image in UI: %d", type);
        if (type == 0) emit displaySfrImageInUI();
        else if (type == 1) emit displayOCImageInUI();
    };
};

#endif // HIGHSPRROW_H
