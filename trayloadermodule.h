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
    void Init(XtMotor*,XtMotor*,XtMotor*,XtMotor*,XtMotor*,XtCylinder*,XtCylinder*,XtCylinder*,XtCylinder*);
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
    void performHandlingOperation(int cmd,int& finished_type);

};

#endif // TRAYLOADERMODULE_H
