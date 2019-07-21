#ifndef HIGHSPRROW_H
#define HIGHSPRROW_H
#include <QObject>
#include "errorcode.h"
#include "aaheadmodule.h"
#include "sut_module.h"
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
private:
    ErrorCodeStruct InitComponent();
public:
    WorkersManager* worker_manager;
    BaseModuleManager * baseModuleManager = Q_NULLPTR;
    LogicManager * logicManager = Q_NULLPTR;
public:

    Q_INVOKABLE void performLUTMushroomPR();
    Q_INVOKABLE void performSUTDownlookPR();
    Q_INVOKABLE void performOnlySUTDownlookPR();
    Q_INVOKABLE void performLUTUplookPR();

    Q_INVOKABLE void performLPALensPR();
    Q_INVOKABLE void performLUTLensPR();
    Q_INVOKABLE void performVacancyLensPR();

    Q_INVOKABLE void callQProcess(QString arg);
    Q_INVOKABLE void aa_A_Inter(double step);
    Q_INVOKABLE void aa_B_Inter(double step);

signals:
    void displaySfrImageInUI();
    void displayOCImageInUI();
    void displayAACoreTuningImageInUI();
    void displayGlueInspectImageInUI();
public slots:
    void homeAllMotors();
    Q_INVOKABLE void receiveImageFromAACore(int type) {
        qInfo("Display image in UI: %d", type);
        if (type == 0) emit displaySfrImageInUI();
        else if (type == 1) emit displayOCImageInUI();
        else if (type == 2) emit displayAACoreTuningImageInUI();
        else if (type == 3) emit displayGlueInspectImageInUI();
    };
};

#endif // HIGHSPRROW_H
