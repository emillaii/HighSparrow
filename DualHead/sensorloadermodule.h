#ifndef SENSORLOADERMODULE_H
#define SENSORLOADERMODULE_H

#include "materialtray.h"
#include "sensorloaderparameter.h"
#include "sensorpickarm.h"
#include "sparrowqserver.h"
#include "thread_worker_base.h"
#include "Vision/vision_location.h"



class SensorLoaderModule:public ThreadWorkerBase
{
    Q_OBJECT
    Q_ENUMS(HandlePosition)
    Q_ENUMS(HandlePR)
    Q_ENUMS(HandleToWorkPos)
    Q_ENUMS(handlePickerAction)

public:
    enum HandlePosition
    {
        SENSOR_TRAY1 = 1,
        SENSOR_TRAY2 = 2,
        SUT_POS1 = 3,
        SUT_POS2 = 4,
        SENSOR_TRAY1_START_POS = 5,
        SENSOR_TRAY2_START_POS = 6,
        SENSOR_TRAY1_END_POS = 7,
    };
    enum HandlePR
    {
        RESET_PR = 10,
        SENSOR_PR = 20,
        VACANCY_PR = 30,
        SUT_PR = 40,
        NG_SENSOR_PR = 50,
        PRODUCT_PR = 60
    };
    enum HandleToWorkPos
    {
        TO_PICK1 = 100,
        TO_PICK2 = 200,
        TO_PR_OFFSET = 300
    };
    enum handlePickerAction
    {
        PICK_SENSOR_FROM_TRAY = 1000,
        PLACE_SENSOR_TO_SUT1 = 2000,
        PLACE_SENSOR_TO_SUT2 = 3000,
        PICK_NG_SENSOR_FROM_SUT1 = 4000,
        PICK_NG_SENSOR_FROM_SUT2 = 5000,
        PLACE_NG_SENSOR_TO_TRAY = 6000,
        PICK_PRODUCT_FROM_SUT1 = 7000,
        PICK_PRODUCT_FROM_SUT2 = 8000,
        PLACE_PRODUCT_TO_TRAY = 9000,
        MEASURE_SENSOR_IN_TRAY = 10000,
        MEASURE_NG_SENSOR_IN_TRAY = 11000,
        MEASURE_PRODUCT_IN_TRAY = 12000,
        MEASURE_SENSOR_IN_SUT1 = 13000,
        MEASURE_NG_SENSOR_IN_SUT1 = 14000,
        MEASURE_PRODUCT_IN_SUT1 = 15000,
        MEASURE_Z_OFFSET = 16000
    };

    SensorLoaderModule();
    void Init(SensorPickArm* pick_arm,MaterialTray *tray,
              VisionLocation * sensor_vision,VisionLocation * vacancy_vision,VisionLocation * sut_vision,
              VisionLocation *sut_sensor_vision,VisionLocation *sut_product_vision, VisionLocation *sensor_pickarm_calibration_glass_vision);
    bool loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);
    void openServer(int port);
    Q_INVOKABLE void performHandling(int cmd);
    Q_INVOKABLE void cameraTipOffsetCalibration(int pickhead);
signals:
    void sendMessageToClient(QString destAddress, QString message);
    void sendChangeTrayRequst();
    // ThreadWorkerBase interface
public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
    void receiveRequestMessage(QString message, QString client_ip);
    void receiveChangeTrayFinish();
private:
    void run(bool has_material);

    bool checkTrayNeedChange();
    bool moveToNextTrayPos(int tray_index);
    bool moveToSUTPRPos(bool is_local = true,bool check_softlanding = false);

    bool performSensorPR();
    bool performVacancyPR();
    bool performSUTPR();
    bool performSUTSensorPR();
    bool performSUTProductPR();
    void resetPR();

    bool moveToWorkPos(bool check_softlanding = false);
    bool moveToWorkPos2(bool check_softlanding = false);
    bool moveToPRResultPos(bool check_softlanding = false);

    bool picker1SearchZ(double z,bool is_open = true,int time_out = 10000);
    bool picker1SearchSutZ(double z,QString dest,QString cmd,bool is_open = true,int time_out = 10000);
    bool picker2SearchZ(double z,bool is_open = true,int time_out = 10000);
    bool picker2SearchSutZ(double z,QString dest,QString cmd,bool is_open = true,int time_out = 10000);
    bool pickTraySensor(int time_out = 10000);
    bool placeSensorToSUT(QString dest,int time_out = 10000);
    bool pickSUTSensor(QString dest,int time_out = 10000);
    bool pickSUTProduct(QString dest, int time_out = 10000);
    bool placeSensorToTray(int time_out = 10000);
    bool placeProductToTray(int time_out = 10000);
    bool picker1MeasureHight(bool is_tray);
    bool picker2MeasureHight(bool is_tray,bool is_product);
    bool measureZOffset();


    bool moveToTrayPos(int index,int tray_index);
    bool moveToTrayPos(int tray_index);
    bool moveToStartPos(int tray_index);
    bool moveToTray1EndPos();


    void sendEvent(const QString event);
    void sendCmd(QString serving_ip,const QString cmd);
public:
    SensorLoaderParameter parameters;
    SensorLoaderState states;
    Position sut1_pr_position;
    Position sut2_pr_position;
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
    VisionLocation * sensor_pickarm_calibration_glass_vision = Q_NULLPTR;

    SparrowQServer * server;
    QQueue<QJsonObject> requestQueue;
    QQueue<QJsonObject> actionQueue;

    bool is_run = false;
    bool finish_stop = false;
    QMutex tray_mutex;
    PrOffset pr_offset;
    int sut_raw_material;
    int sut_used_material;
    int picked_material;
    QString servingIP = "";
    bool isLocalHost = false;
};

#endif // SENSORLOADERMODULE_H
