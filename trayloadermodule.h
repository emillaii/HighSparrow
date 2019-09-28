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
    TrayLoaderModule(QString name="LensTrayLoaderModule");
    void Init(XtMotor*,XtMotor*,XtMotor*,XtMotor*,XtMotor*,XtCylinder*,XtCylinder*,XtCylinder*,XtCylinder*,TrayClip*,TrayClip*,
              XtGeneralInput *work_tray_check_io,XtGeneralInput* entrance_tray_check_io,XtGeneralInput* exit_tray_check_io,
              XtGeneralInput* entrance_clip_check_io,XtGeneralInput* exit_clip_check_io,XtGeneralInput* ready_tray_check_io);

    TrayClip* tray_clip = Q_NULLPTR;
    TrayClip* tray_clip_out = Q_NULLPTR;
    TrayLoaderModuleParameter parameters;
    TrayLoaderState states;

    bool sendoutAndReayPushOutEmptyTray();
    bool moveToGetAndPushInNewTrayAndPushOutTray();
    bool moveToGetAndPushInNewTray();
    bool moveToPushOutTray();
    bool moveToWorkPosAndReayPullNewTray();
    bool entranceClipMoveToNextPos();
    bool moveToReayPullNewTray();
    bool moveToPushReadyTray();
    bool clipPushoutTray();
    bool existClipMoveToNextPos();

    bool moveToChangeChangeHandlly();
    bool moveToWorkPos();
    bool moveToChangeClipPos();
private:
    void run();
    bool resumeState();
    void runHandle();

    //传感器检测
    bool checkWorkTray(bool check_state);
    bool checkEntranceTray(bool check_state);

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

    XtGeneralInput* work_tray_check_io = Q_NULLPTR;
    XtGeneralInput* entrance_tray_check_io = Q_NULLPTR;
    XtGeneralInput* exit_tray_check_io = Q_NULLPTR;
    XtGeneralInput* entrance_clip_check_io = Q_NULLPTR;
    XtGeneralInput* exit_clip_check_io = Q_NULLPTR;
    XtGeneralInput* ready_tray_check_io = Q_NULLPTR;

public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
    // ThreadWorkerBase interface
public:
    PropertyBase *getModuleState();

    // ThreadWorkerBase interface
public:
    void receivceModuleMessage(QVariantMap module_message);
    QMap<QString, PropertyBase *> getModuleParameter();
};

#endif // TRAYLOADERMODULE_H
