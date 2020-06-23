#ifndef SENSORTRAYLOADERMODULE_H
#define SENSORTRAYLOADERMODULE_H

#include "XtCylinder.h"
#include "sensorclip.h"
#include "sensortrayloaderparameter.h"
#include "thread_worker_base.h"
#include "xtmotor.h"

class SensorTrayLoaderModule:public ThreadWorkerBase
{
    Q_OBJECT
    Q_ENUMS(HandlePosition)
    Q_ENUMS(HandleAction)
public:
    enum HandlePosition
    {
        START_POSITION = 1,
        END_POSITION = 2,
        INPUT_MAGAZINE_POS1 = 3,
        INPUT_MAGAZINE_POS2 = 4,
        OUTPUT_MAGAZINE_POS1 = 5,
        OUTPUT_MAGAZINE_POS2 = 6
    };
    enum HandleAction
    {
        STPO_PUSH = 10
    };

    SensorTrayLoaderModule();
    void Init(XtMotor* motor_tray,XtMotor* motor_kick,XtMotor* motor_stie,XtMotor* motor_stoe,XtMotor* motor_push,
              XtCylinder* kick1,XtCylinder* kick2,XtCylinder* hold_tray,XtCylinder* hold_vacancy,
              XtCylinder* entrance_clip_push,XtCylinder* exit_clip_push,XtCylinder* gripper,
              SensorClip* entrance_clip,SensorClip* exit_clip,
              XtGeneralInput* entrance_clip_check_io,
              XtGeneralInput* exit_clip_check_io,
              XtGeneralInput* ready_tray_check_io,
              XtGeneralInput* kick_tray_check_io,
              XtGeneralInput* sensor_tray_check_io,
              XtGeneralInput* vacancy_tray_check_io,
              XtGeneralInput* entrance_tray_check_io,
              XtGeneralInput* exit_tray_check_io);
signals:
//    void sendChangeTrayFinish();
    // ThreadWorkerBase interface
    void sendStopAllWorkersSignal();
public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void performHandlingOperation(int cmd,QVariant param);
    void resetLogic();
    void receiveChangeTray();
private:
    void run();
    void runHandly();
public:
    SensorTrayLoaderParameter parameters;
    SensorTrayLoaderState states;
    XtMotor* motor_tray = Q_NULLPTR;
    XtMotor* motor_kick = Q_NULLPTR;
    XtMotor* motor_stie = Q_NULLPTR;
    XtMotor* motor_stoe = Q_NULLPTR;
    XtMotor* motor_push = Q_NULLPTR;
    XtCylinder* kick1 = Q_NULLPTR;
    XtCylinder* kick2 = Q_NULLPTR;
    XtCylinder* hold_tray = Q_NULLPTR;
    XtCylinder* hold_vacancy = Q_NULLPTR;
    XtCylinder* entrance_clip_push = Q_NULLPTR;
    XtCylinder* exit_clip_push = Q_NULLPTR;
    XtCylinder* gripper = Q_NULLPTR;
    SensorClip* entrance_clip;
    SensorClip* exit_clip;

    XtGeneralInput* entrance_clip_check_io = Q_NULLPTR;
    XtGeneralInput* exit_clip_check_io = Q_NULLPTR;
    XtGeneralInput* ready_tray_check_io = Q_NULLPTR;
    XtGeneralInput* kick_tray_check_io = Q_NULLPTR;
    XtGeneralInput* sensor_tray_check_io = Q_NULLPTR;
    XtGeneralInput* vacancy_tray_check_io = Q_NULLPTR;
    XtGeneralInput* entrance_tray_check_io = Q_NULLPTR;
    XtGeneralInput* exit_tray_check_io = Q_NULLPTR;

    bool movetoSTIEColumnIndex(int);
    bool movetoSTOEColumnIndex(int);
    bool movetoPushMotorSafePosotion();
    bool movetoDownTrayPosition();
    bool movetoGetTrayPosition();
    bool movetoFinishKickTrayPosition();
    bool movetoPushoutPosition();
    bool movetoPutTrayPosition();
    bool movetoStartKickTrayPosition();
    Q_INVOKABLE bool movetoTrayWorkPosition();
    bool movetoVacancyTrayPosition();

private:
    bool is_run = false;
    int retryTime;
    bool moveToDownTrayAndReadyToPush();
    bool moveToUpReadyTray(bool has_tray);
    bool moveToWaitHandleTray();
    bool moveToChangeVacancyTrayAndUpReadyTray(bool has_vacancy_tray);
    bool moveToPullNextTray();
    bool moveToPullNextTray1();
    bool moveToPutFirstTray();
    bool moveToPutTray();
    bool moveToWorkPos(bool has_tray = true);
    bool moveToEntranceClipNextPos();
    bool moveToExitClipNextPos();

    bool moveToDownsensorTray();

    bool checkEntanceTray(bool check_state);
    bool checkSensorTray(bool check_state, bool showErrorMsg = true);
    bool checkKickTray(bool check_state);
    bool checkReadyTray(bool check_state);
    bool checkVacancyTray(bool check_state, bool showErrorMsg = true);
    bool checkExitTray(bool check_state);

    // ThreadWorkerBase interface
public:
    PropertyBase *getModuleState();

    // ThreadWorkerBase interface
public:
    void receivceModuleMessage(QVariantMap module_message);
    QMap<QString, PropertyBase *> getModuleParameter();
    void setModuleParameter(QMap<QString, PropertyBase *>);
};

#endif // SENSORTRAYLOADERMODULE_H
