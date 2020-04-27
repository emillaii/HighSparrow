#ifndef HIGHSPRROW_H
#define HIGHSPRROW_H
#include <QObject>
#include "utils/errorcode.h"
#include "aaHeadModule/aaheadmodule.h"
#include "sutModule/sut_module.h"
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
    Q_INVOKABLE void callAVSPRSettingProcess(QString exe_name, QString pr_name);
    Q_INVOKABLE void aa_A_Inter(double step);
    Q_INVOKABLE void aa_B_Inter(double step);
    Q_INVOKABLE void tcp_aa_A_Inter(double step);
    Q_INVOKABLE void tcp_aa_B_Inter(double step);

    double scaleValue = 1.1;
    int scaleLevel = 0;

signals:
    void displaySfrImageInUI();
    void displayOCImageInUI();
    void displayAACoreTuningImageInUI();
    void displayGlueInspectImageInUI();
    void displayUplookResultImageInUI();
    void displayAA1DownlookResultImageInUI();
    void displayLensPickarmResultImageInUI();
    void displayAA2DownlookResultImageInUI();
    void displaySensorPickarmResultImageInUI();
    void displaySensorUplookResultImageInUI();
    void displayBarcodeResultImageInUI();
    void displayDispenseImageInUI();
public slots:
    void homeAllMotors();
    Q_INVOKABLE void receiveImageFromAACore(int type) {
        qDebug("Display image in UI: %d", type);
        if (type == 0) emit displaySfrImageInUI();
        else if (type == 1) emit displayOCImageInUI();
        else if (type == 2) emit displayAACoreTuningImageInUI();
        else if (type == 3) emit displayGlueInspectImageInUI();
        else if (type == 4) emit displayAACoreTuningImageInUI();
        else if (type == 5) emit displayUplookResultImageInUI();
        else if (type == 6) emit displayAA1DownlookResultImageInUI();
        else if (type == 7) emit displayLensPickarmResultImageInUI();
        else if (type == 8) emit displayAA2DownlookResultImageInUI();
        else if (type == 9) emit displaySensorPickarmResultImageInUI();
        else if (type == 10) emit displaySensorUplookResultImageInUI();
        else if (type == 11) emit displayBarcodeResultImageInUI();
        else if (type == 12) emit displayDispenseImageInUI();
    };
};

#endif // HIGHSPRROW_H
