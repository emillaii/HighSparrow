 #ifndef TRAYLOADERMODULE_H
#define TRAYLOADERMODULE_H

#include "thread_worker_base.h"
#include "trayclip.h"
#include "trayloaderparameter.h"
#include "XtMotion/xtmotor.h"
#include "XtMotion/XtCylinder.h"

class TrayLoaderModule : public ThreadWorkerBase{
    Q_OBJECT
public:
    TrayLoaderModule(QString name="TrayLoaderModule");
    void Init(XtMotor*,XtMotor*,XtMotor*,XtMotor*,XtMotor*,XtCylinder*,XtCylinder*,XtCylinder*,XtCylinder*,TrayClip*,TrayClip*,
              XtGeneralInput* trayinio,XtGeneralInput* trayoutio,XtGeneralInput*tray_check_io);
    void performHandling(int cmd);

    TrayClip* tray_clip = Q_NULLPTR;
    TrayClip* tray_clip_out = Q_NULLPTR;
    TrayLoaderModuleParameter parameters;
    TrayLoaderState states;
    bool startUp();

    bool moveToNextTrayPos();
    bool ejectTray();

    bool motorInPress();
    bool moveToReadyFirstKick();
    bool moveToFirstKick();
    bool moveToReadySecondKick();
    bool moveToLtkx1GetPos();
    bool moveToLtkx1RelayPos();
    bool moveToLtkx1SetPos();
    bool motorInRelease();

    bool moveToLtlGetPos();
    bool moveToSafePos();
    bool motorWorkPress();
    bool moveToLtlSetPos();
    bool motorWorkRelease();
    bool moveToTrayOutHandOverPos();
    bool moveToTrayInHandOverPos();

    bool moveToLtkx2GetPos();
    bool motorOutPress();
    bool moveToLtkx2SetPos();
    bool motorOutRelease();

    bool moveToNextEmptyPos();

    bool LTIEMovetoColumnIndex(int);
    bool LTOEMovetoColumnIndex(int);

    bool sendoutAndReayPushOutEmptyTray(bool check_tray);
    bool moveToGetAndPushInNewTrayAndPushOutTray(bool check_tray);
    bool moveToGetAndPushInNewTray(bool check_tray);
    bool moveToPushOutTray();
    bool moveToWorkPosAndReayPullNewTray();
    bool entranceClipMoveToNextPos();
    bool moveToReayPullNewTray();
    bool moveToPushReadyTray();
    bool clipPushoutTray(bool check_tray);
//    bool moveToReadyPushTray();
    bool existClipMoveToNextPos();

    bool moveToChangeChangeHandlly();
    bool moveToWorkPos();
private:
    void run(bool has_tray);
    bool resumeState();
    void runHandle();

    bool is_run = false;
    QMutex tray_mutex;
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

    XtGeneralInput* tray_entry_input = Q_NULLPTR;
    XtGeneralInput* tray_exit_input = Q_NULLPTR;


signals:
    void nextTrayPos();
    void ltkx1Pickup();
    void ltlxPickup();
    void ltlxPutdown();
    void ltkx2Pickup();
    void nextEmptyPos();
    void trayReady();
    void testTrayUsed();

    void testLTLXPickUpTray();
    void testLTLXPutDownTray();
    void reset();

public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);

    void onNextTrayPos();
    void onLtkx1Pickup();
    void onLtlxPickup();
    void onLtlxPutdown();
    void onLtkx2Pickup();
    void onNextEmptyPos();

    void onTestTrayUsed();
    void onTestLTLXPickUpTray();
    void onTestLTLXPutDownTray();
    void onReset();


};

#endif // TRAYLOADERMODULE_H
