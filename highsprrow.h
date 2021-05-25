#ifndef HIGHSPRROW_H
#define HIGHSPRROW_H
#include <QObject>
#include "Utils/errorcode.h"
#include "AAHeadModule/aaheadmodule.h"
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
public:
    Q_INVOKABLE void callQProcess(QString arg);

signals:
    void displaySfrImageInUI();
    void displayOCImageInUI();
    void displayGlueInspectImageInUI();
    void displayAACoreTuningImageInUI();
    void displayAACoreTuningImageInUI2();
public slots:
    Q_INVOKABLE void receiveImageFromAACore(int type) {
        qInfo("Display SFR image in UI: %d", type);
        if (type == 0) emit displaySfrImageInUI();
        else if (type == 1) emit displayOCImageInUI();
        else if (type == 2) emit displayGlueInspectImageInUI();
        else if (type == 3) {
            emit displayAACoreTuningImageInUI();
            emit displayAACoreTuningImageInUI2();
        }
    };
};

#endif // HIGHSPRROW_H
