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
    bool startUp();

    bool moveToNextTrayPos();
    bool ejectTray();

    bool motorInPress();
    bool moveToLtkx1GetPos();
    bool moveToLtkx1RelayPos();
    bool moveToLtkx1SetPos();
    bool motorInRealease();

    bool moveToLtlGetPos();
    bool motorWorkPress();
    bool moveToLtlSetPos();
    bool motorWorkRelease();

    bool moveToLtkx2GetPos();
    bool motorOutPress();
    bool moveToLtkx2SetPos();
    bool motorOutRelease();

    bool moveToNextEmptyPos();

    bool LTIEMovetoColumnIndex(int);
    bool LTOEMovetoColumnIndex(int);
private:
    void run(bool has_tray);


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

    XtGeneralInput *tray_check_input = Q_NULLPTR;

signals:
    void nextTrayPos();
    void ltkx1Pickup();
    void ltlxPickup();
    void ltlxPutdown();
    void ltkx2Pickup();
    void nextEmptyPos();
    void trayReady();
    void testTrayUsed();

public slots:
    void startWork(bool reset_logic,int run_mode);
    void stopWork(bool wait_finish);
    void performHandlingOperation(int cmd);

    void onNextTrayPos();
    void onLtkx1Pickup();
    void onLtlxPickup();
    void onLtlxPutdown();
    void onLtkx2Pickup();
    void onNextEmptyPos();

    void onTestTrayUsed();


};

#endif // TRAYLOADERMODULE_H
