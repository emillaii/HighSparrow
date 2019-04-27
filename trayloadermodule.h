#ifndef TRAYLOADERMODULE_H
#define TRAYLOADERMODULE_H

#include "thread_worker_base.h"
#include "trayclip.h"
#include "trayloaderparameter.h"
#include "xtmotor.h"
#include "XtCylinder.h"

class TrayLoaderModule : public ThreadWorkerBase{
    Q_OBJECT
public:
    TrayLoaderModule(QString name="TrayLoaderModule");
    void Init(XtMotor*,XtMotor*,XtMotor*,XtMotor*,XtMotor*,XtCylinder*,XtCylinder*,XtCylinder*,XtCylinder*,TrayClip*,TrayClip*);
    void ResetLogic();
    void performHandling(int cmd);

    TrayClip* tray_clip = Q_NULLPTR;
    TrayClip* tray_clip_out = Q_NULLPTR;
    TrayLoaderModuleParameter parameters;
signals:

private:
    void run(bool has_tray);
    bool moveMotorClip();
    bool moveToNextTrayPos();
    bool ejectTray();

    bool moveMotorIn();
    bool motorInPress();
    bool moveToLtkx1GetPos();
    bool moveToLtkx1SetPos();
    bool motorInRealease();



    bool moveMotorWork();
    bool moveToLtlGetPos();
    bool motorWorkPress();
    bool moveToLtlSetPos();
    bool motorWorkRelease();


    bool moveMotorOut();
    bool moveToLtkx2GetPos();
    bool motorOutPress();
    bool moveToLtkx2SetPos();
    bool motorOutRelease();

    bool moveToNextEmptyPos();


    bool is_run=false;
    XtMotor* motor_clip_in = Q_NULLPTR;  //LTIE
    XtMotor* motor_in = Q_NULLPTR;       //LTK_X1
    XtMotor* motor_work = Q_NULLPTR;     //LTL_X
    XtMotor* motor_out = Q_NULLPTR;     //LTK_X2
    XtMotor* motor_clip_out = Q_NULLPTR; //LTOE

    XtCylinder* cylinder_clip = Q_NULLPTR; //弹夹推料气缸
    XtCylinder* cylinder_ltk1 = Q_NULLPTR; //LTK1气缸
    XtCylinder* cylinder_ltk2 = Q_NULLPTR; //LTK2气缸
    XtCylinder* cylinder_tray = Q_NULLPTR; //LTL_TRAY气缸

public slots:
    void startWork(bool reset_logic,int run_mode);
    void stopWork(bool wait_finish);
    void performHandlingOperation(int cmd);

};

#endif // TRAYLOADERMODULE_H
