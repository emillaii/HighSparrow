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
public:
    SensorTrayLoaderModule();
    void Init(XtMotor* motor_tray,XtMotor* motor_kick,XtMotor* motor_stie,XtMotor* motor_stoe,XtMotor* motor_push,
              XtCylinder* kick1,XtCylinder* kick2,XtCylinder* hold_tray,XtCylinder* hold_vacancy,
              XtCylinder* entrance_clip_push,XtCylinder* exit_clip_push,XtCylinder* gripper,
              SensorClip* entrance_clip,SensorClip* exit_clip);
signals:
    void sendChangeTrayFinish();
    // ThreadWorkerBase interface
public slots:
    void startWork(bool reset_logic, int run_mode);
    void stopWork(bool wait_finish);
    void performHandlingOperation(int cmd);
    void receiveChangeTray();
private:
    void run(bool has_material);
    void resetLogic();
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

    bool movetoSTIEColumnIndex(int);
    bool movetoSTOEColumnIndex(int);
    bool movetoPushMotorSafePosotion();
    bool movetoDownTrayPosition();
    bool movetoGetTrayPosition();
    bool movetoFinishKickTrayPosition();
    bool movetoPushoutPosition();
    bool movetoPutTrayPosition();

private:
    bool is_run = false;
    bool moveToStartKick();
    bool moveToUpSensorTray();
    bool moveToChangeVacancyTrayAndUpSensorTray();
    bool moveToGetTray();
    bool moveToGetTrayAndKickOutTray(bool has_vacancy_tray = true);
    bool moveToPutVacancyTray();
    bool moveToPutSensorTray();
    bool moveToBackKickAndPutSensorTray();
    bool moveToWorkPos();
    bool moveToEntranceClipNextPos();
    bool moveToExitClipNextPos();

    bool moveToDownsensorTray();
};

#endif // SENSORTRAYLOADERMODULE_H
