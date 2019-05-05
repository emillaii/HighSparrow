#ifndef SENSORLOADERMODULE_H
#define SENSORLOADERMODULE_H

#include "materialtray.h"
#include "sensorloaderparameter.h"
#include "sensorpickarm.h"
#include "sparrowqserver.h"
#include "thread_worker_base.h"
#include "vision_location.h"



class SensorLoaderModule:public ThreadWorkerBase
{
public:
    SensorLoaderModule();
    void Init(SensorPickArm* pick_arm,MaterialTray *tray,
              VisionLocation * sensor_vision,VisionLocation * vacancy_vision,VisionLocation * sut_vision,
              VisionLocation *sut_sensor_vision,VisionLocation *sut_product_vision);
    bool loadJsonConfig();
    void saveJsonConfig();
    // ThreadWorkerBase interface
public slots:
    void startWork(bool reset_logic, int run_mode);
    void stopWork(bool wait_finish);
    void performHandlingOperation(int cmd);
public:
    void resetLogic();
private:
    void run(bool has_material);

    bool moveToNextTrayPos(int tray_index);
    bool moveToSUTPRPos(bool check_softlanding = false);

    bool performSensorPR();
    bool performVacancyPR();
    bool performSUTSensorPR();
    bool performSUTProductPR();
    void resetPR();

    bool moveToWorkPos(bool check_softlanding = false);
    bool moveToWorkPos2(bool check_softlanding = false);
    bool picker1SearchZ(double z,bool is_open = true,bool check_softlanding = false);
    bool picker2SearchZ(double z,bool is_open = true,bool check_softlanding = false);
    bool pickTraySensor(bool check_softlanding = false);
    bool placeSensorToSUT(bool check_softlanding = false);
    bool pickSUTSensor(bool check_softlanding = false);
    bool pickSUTProduct(bool check_softlanding = false);
    bool placeSensorToTray(bool check_softlanding = false);
    bool placeProductToTray(bool check_softlanding = false);
    bool measureHight(bool is_tray);


    bool moveToTrayPos(int index,int tray_index);
    bool moveToTrayPos(int tray_index);
    bool moveToStartPos(int tray_index);
    bool moveToTray1EndPos();
public:
    SensorLoaderParameter parameters;
    SensorLoaderState states;
    Position sut_pr_position;
    Position picker1_offset;
    Position picker2_offset;
private:
    SensorPickArm* pick_arm = Q_NULLPTR;
    MaterialTray *tray = Q_NULLPTR;
    VisionLocation * sensor_vision = Q_NULLPTR;
    VisionLocation * vacancy_vision = Q_NULLPTR;
    VisionLocation * sut_vision = Q_NULLPTR;
    VisionLocation *sut_sensor_vision = Q_NULLPTR;
    VisionLocation *sut_product_vision = Q_NULLPTR;
    VisionLocation *spa_updownlook_up_vision = Q_NULLPTR;
    VisionLocation *spa_updownlook_down_vision = Q_NULLPTR;

    SparrowQServer * server;
    QQueue<QJsonObject> requestQueue;
    QQueue<QJsonObject> actionQueue;

    bool is_run = false;
    bool finish_stop = false;
    PrOffset pr_offset;
    int sut_raw_material;
    int sut_used_material;
    int picked_material;
};

#endif // SENSORLOADERMODULE_H
