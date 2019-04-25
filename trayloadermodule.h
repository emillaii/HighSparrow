#ifndef TRAYLOADERMODULE_H
#define TRAYLOADERMODULE_H

#include "thread_worker_base.h"
#include "trayclip.h"
#include "trayloaderparameter.h"
#include "xtmotor.h"

class TrayLoaderModule : public ThreadWorkerBase{
    Q_OBJECT
public:
    TrayLoaderModule(QString name="");
    void Init(XtMotor*,XtMotor*,XtMotor*,XtMotor*);
    void ResetLogic();
    void performHandling(int cmd,int& finished_type);

    TrayClip* tray_clip = Q_NULLPTR;
    TrayLoaderModuleParameter parameters;
signals:

private:
    void run(bool has_tray);
    bool moveMotorClip();
    bool moveToCurrentPos();
    bool ejectTray();

    bool moveMotorIn();
    bool motorInPress();
    bool moveToPosition1();
    bool motorInRealease();



    bool moveMotorWork();
    bool motorWorkPress();
    bool moveToPosition2();
    bool motorWorkRelease();


    bool moveMotorOut();
    bool motorOutPress();
    bool moveToPosition3();
    bool motorOutRelease();


    bool is_run=false;
    XtMotor* motor_clip = Q_NULLPTR;
    XtMotor* motor_in = Q_NULLPTR;
    XtMotor* motor_work = Q_NULLPTR;
    XtMotor* motor_out = Q_NULLPTR;
public slots:
    void startWork(bool reset_logic,int run_mode);
    void stopWork(bool wait_finish);
    void performHandlingOperation(int cmd,int& finished_type);

};

#endif // TRAYLOADERMODULE_H
