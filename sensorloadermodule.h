#ifndef SENSORLOADERMODULE_H
#define SENSORLOADERMODULE_H

#include "materialtray.h"
#include "sensorloaderparameter.h"
#include "sensorpickarm.h"
#include "network/sparrowqserver.h"
#include "thread_worker_base.h"
#include "uphhelper.h"
#include "vision/vision_location.h"

#define TIMES_1 100
#define TIMES_2 1000
#define TIMES_3 100000
#define TIMES_4 10000000

class SensorLoaderModule:public ThreadWorkerBase
{
    Q_OBJECT
    Q_ENUMS(HandleCameraPosition)
    Q_ENUMS(HandlePR)
    Q_ENUMS(HandlePickerPos)
    Q_ENUMS(handlePickerAction)
public:
    enum HandleCameraPosition
    {
        SENSOR_TRAY_1_POS = 1,
        SENSOR_TRAY_2_POS = 2,
        NG_SENSOR_TRAY_POS = 3,
        BUFFER_TRAY_POS = 4,
        SUT_POS1 = 5,
        SUT_POS2 = 6,
        SENSOR_TRAY_1_START_POS = 7,
        SENSOR_TRAY_2_START_POS = 8,
        BUFFER_TRAY_START_POS = 9,
        NG_SENSOR_TRAY_START_POS = 10,
        SENSOR_TRAY_1_END_POS = 11,
        SPA_STANDBY_POS = 12,
    };
    enum HandlePR
    {
        TRAY_SENSOR_PR = 1*TIMES_1,
        TRAY_EMPTY_PR = 2*TIMES_1,
        SUT_EMPTY_PR = 3*TIMES_1,
        SUT_SENSOR_PR = 4*TIMES_1,
        SUT_PRODUCT_PR = 5*TIMES_1,
        TRAY_SENSOR_OFFSET = 6*TIMES_1
    };
    enum HandlePickerPos
    {
        TO_PICK_SENSOR_POS1 = 1*TIMES_2,
        TO_PICK_SENSOR_POS2 = 2*TIMES_2,
        TO_PLACE_SENSOR_POS1 = 3*TIMES_2,
        TO_PLACE_SENSOR_POS2 = 4*TIMES_2,
        TO_PICK_PRODUCT_POS1 = 5*TIMES_2,
        TO_PICK_PRODUCT_POS2 = 6*TIMES_2,
        TO_PLACE_PRODUCT_POS1 = 7*TIMES_2,
        TO_PLACE_PRODUCT_POS2 = 8*TIMES_2,
        TO_PICK_NG_SENSOR_POS1 = 9*TIMES_2,
        TO_PICK_NG_SENSOR_POS2 = 10*TIMES_2,
        TO_PLACE_NG_POS = 11*TIMES_2,
        TO_PLACE_BUFFER_POS = 12*TIMES_2,
        TO_PR_OFFSET = 13*TIMES_2
    };

    enum handlePickerAction
    {
        PICK_SENSOR_FROM_TRAY1 = 1*TIMES_3,
        PICK_SENSOR_FROM_TRAY2 = 2*TIMES_3,
        PLACE_SENSOR_TO_SUT1 = 3*TIMES_3,
        PLACE_SENSOR_TO_SUT2 = 4*TIMES_3,
        PICK_NG_SENSOR_FROM_SUT1 = 5*TIMES_3,
        PICK_NG_SENSOR_FROM_SUT2 = 6*TIMES_3,
        PLACE_NG_SENSOR_TO_TRAY = 7*TIMES_3,
        PICK_PRODUCT_FROM_SUT1 = 8*TIMES_3,
        PICK_PRODUCT_FROM_SUT2 = 9*TIMES_3,
        PLACE_PRODUCT_TO_TRAY1 = 10*TIMES_3,
        PLACE_PRODUCT_TO_TRAY2 = 11*TIMES_3,
        PLACE_PRODUCT_TO_BUFFER_TRAY = 12*TIMES_3,
        PLACE_NG_PRODUCT_TO_TRAY = 13*TIMES_3,

        MEASURE_SENSOR_IN_TRAY1 = 14*TIMES_3,
        MEASURE_SENSOR_IN_TRAY2 = 15*TIMES_3,
        MEASURE_SENSOR_IN_SUT1 = 16*TIMES_3,
        MEASURE_SENSOR_IN_SUT2 = 17*TIMES_3,
        MEASURE_PRODUCT_IN_SUT1 = 18*TIMES_3,
        MEASURE_PRODUCT_IN_SUT2 = 19*TIMES_3,
        MEASURE_PRODUCT_IN_TRAY1 = 20*TIMES_3,
        MEASURE_PRODUCT_IN_TRAY2 = 21*TIMES_3,
        MEASURE_PRODUCT_IN_BUFFER = 22*TIMES_3,
        MEASURE_NG_PRODUCT_IN_TRAY = 23*TIMES_3,
        MEASURE_NG_SENSOR_IN_SUT1 = 24*TIMES_3,
        MEASURE_NG_SENSOR_IN_SUT2 = 25*TIMES_3,
        MEASURE_NG_SENSOR_IN_TRAY = 26*TIMES_3,
        MEASURE_Z_OFFSET = 27*TIMES_3,
        PLACE_SENSOR_BACK_TO_TRAY1= 28*TIMES_3,
        PLACE_SENSOR_BACK_TO_TRAY2= 29*TIMES_3,
        CLEARANCE = 30*TIMES_3
    };

    enum SensorPosition
    {
        SENSOR_TRAY_1,//tray id 0
        SENSOR_TRAY_2,//tray id 1
        NG_SENSOR_TRAY,//tray id 2
        BUFFER_TRAY,//tray id 3

        TRAY_NG_PRODUCT,
        SUT1_SENSOR,
        SUT2_SENSOR,
        SUT1_PRODUCT,
        SUT2_PRODUCT
    };
    enum BusyState
    {
        IDLE,
        SUT1,
        SUT2
    };

    SensorLoaderModule();
    void Init(SensorPickArm* pick_arm,
              MaterialTray *tray,
              VisionLocation * tray_sensor_location,
              VisionLocation * tray_empty_location,
              VisionLocation * sut_empty_location,
              VisionLocation *sut_sensor_location,
              VisionLocation *sut_product_location,
              VisionLocation *sensor_pickarm_calibration_glass_location,
              int thread_id);
//    void openServer(int port);
    Q_INVOKABLE void cameraTipOffsetCalibration(int pickhead);
    Q_INVOKABLE void clearNumber();
    // ThreadWorkerBase interface
    void receivceModuleMessage(QVariantMap module_message);
    PropertyBase *getModuleState();
    QMap<QString,PropertyBase*> getModuleParameter();
    void setModuleParameter(QMap<QString, PropertyBase *>);
    Q_INVOKABLE void changeBufferTray();
    Q_INVOKABLE void changeNgTray();
signals:
    void sendMessageToClient(QString destAddress, QString module_message);
    void sendChangeTrayRequst();
    void postCSVDataToUnit(QString uuid,QVariantMap data);
    void saveUnitDataToCSV(QString uuid);
    // ThreadWorkerBase interface
public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd,QVariant param);
private:
    void run();
//    void runTest();
//    bool moveToSensorTrayNextPos();
//    bool moveToProductTrayNextPos();
//    bool moveToNgTrayNextPos();
    //状态机条件检测
    bool checkTrayNeedChange();
    int checkForceChageStation();
    bool checkNeedPickSensor();
    bool checkSut1WaitCondition();
    bool checkSut2WaitCondition();
    //查找盘可用位置
    bool findTrayNextSensorPos(bool allow_change_tray);
    bool findTrayNextEmptyPos();
    bool findTrayNextInitStatePos(int tray_index);
    int getTrayIndex();
    //移动相机位置
    bool moveCameraToTrayCurrentPos(int tray_index,bool check_softlanding = false);
    bool moveCameraToSUTPRPos(bool is_local = true,bool check_softlanding = false);
    bool moveCameraToStandbyPos(bool check_arrived = false,bool check_softlanding = false);
    //执行视觉
    bool performTraySensorPR();
    bool performTrayEmptyPR();
    bool performSUTEmptyPR();
    bool performSUTSensorPR();
    bool performSUTProductPR();
    //吸料偏移
    QPointF getPickerResultOffset(double theta);
    void setSensorOffset();
    //走视觉偏差
    bool moveToPRResultPos(PrOffset pr_result,bool check_softlanding = false);
    //带偏差值走位
    bool movePicker1ToTrayCurrentPos(int tray_index,bool check_softlanding = false);
    bool movePicker2ToTrayCurrentPos(int tray_index,bool check_softlanding = false);
    bool movePicker1ToSUTPos(bool is_local,bool check_softlanding = false);
    bool movePicker2ToSUTPos(bool is_local,bool is_product,bool check_softlanding = false);

    //取放料动作
    bool pickSensorFromTray(int tray_id,int time_out = 10000);
    bool backSensorToTray(int tray_id,int time_out = 10000);
    bool placeSensorToSUT(bool is_local,int time_out = 10000);
    bool pickNgSensorFromSut(bool is_local,int time_out = 10000);
    bool pickProductFromSut(bool is_local, int time_out = 10000);
    bool placeNgSensorToTray(int time_out = 10000);
    bool placeNgProductToTray(int time_out = 10000);
    bool placeProductToTray(int tray_id,int time_out = 10000);
    bool placeProductToBuferr(int time_out = 10000);

    bool picker1PickFromTray(double z,int time_out = 10000);
    bool picker1BackToTray(double z,int time_out = 10000);
    bool picker1PlaceToSut(double z,bool is_local,int time_out = 10000);
    bool picker2PickFromSut(double z,double force,bool is_local,int time_out = 10000);
    bool picker2PlaceToTray(double z,double force,bool is_product,int time_out = 10000);
    //真空动作
    void openSut1Vacuum();
    void closeSut1Vacuum();
    bool waitSutVacuumFinish();
    void openSut2Vacuum();
    void closeSut2Vacuum();

    //真空检测
    bool checkPicker1HasMaterialSync();
    bool checkPicker1HasMateril();
    bool waitPicker1CheckResult(bool check_state);
    bool checkPicker2HasMaterialSync();
    bool checkPicker2HasMateril();
    bool waitPicker2CheckResult(bool check_state);
    //真空动作
    bool setPicker1Vacuum(bool state);
    bool setPicker2Vacuum(bool state);
    //手动特殊操作
    bool moveToStartPos(int tray_index);
    bool moveToTray1EndPos();
    bool picker1MeasureHight(int tray_id);
    bool picker2MeasureHight(int tray_id);
    bool measureZOffset();
    bool unloadAllSensor();

//    bool checkPickedSensor(bool check_state);
//    bool checkPickedNgOrProduct(bool check_state);


    bool moveToTrayPos(int index,int tray_index);
//    bool moveToTrayPos(int tray_index);



//uph 统计
    double updateAccumulatedHour(bool calculate = true);
    double getHourSpace(QTime time_label);

//    void sendEvent(const QString event);
//    void sendCmd(QString serving_ip,const QString cmd);

//    void getPicker1SensorOffset();
//    void getPicker2SensorOffset();
//    void setPicker1SensorOffset();
//    void setPicker2SensorOffset();
public:
    SensorLoaderParameter parameters;
    SensorLoaderState states;
    Position sut1_pr_position;
    Position sut2_pr_position;
    Position spa_standby_position;
    Position picker1_offset;
    Position picker2_offset;
    Position sensorTray1PickOffset;
    Position sensorTray2PickOffset;
    Position sut1PickOffset;
    Position sut2PickOffset;
    Position sut1PlaceOffset;
    Position sut2PlaceOffset;
	Position sensorTray1PlaceOffset;
    Position sensorTray2PlaceOffset;
    Position ngTrayPlaceOffset;
    Position bufferTrayPlaceOffset;

    UPHHelper sensor_uph;
    UPHHelper product_uph;
    UPHHelper comprehensive_uph;

    UPHHelper left_sensor_uph;
    UPHHelper left_product_uph;
    UPHHelper left_comprehensive_uph;

    UPHHelper right_sensor_uph;
    UPHHelper right_product_uph;
    UPHHelper right_comprehensive_uph;

private:
    SensorPickArm * pick_arm = Q_NULLPTR;
    MaterialTray * tray = Q_NULLPTR;
    VisionLocation * tray_sensor_location = Q_NULLPTR;
    VisionLocation * tray_empty_location = Q_NULLPTR;
    VisionLocation * sut_empty_location = Q_NULLPTR;
    VisionLocation *sut_sensor_location = Q_NULLPTR;
    VisionLocation *sut_product_location = Q_NULLPTR;
    VisionLocation * sensor_pickarm_calibration_glass_location = Q_NULLPTR;
    int thread_id = 0;
    bool is_run = false;
    bool finish_stop = false;
    bool allowChangeBufferTray = false;
    bool allowChangeNgTray = false;
    QMutex tray_mutex;
    QVariantMap picker1_senseor_data;
    QVariantMap sut1_sensor_data;
    QVariantMap sut2_sensor_data;
    QVariantMap picker2_senseor_data;
    PrOffset pr_offset;
    int sut_raw_material;
    int sut_used_material;
    int picked_material;
    QString servingIP = "";
    bool isLocalHost = false;
    QTime time_label;

    QMap<QString,QVariantMap> prRecordMap;
    bool is_test_finish;
    bool current_count;
    bool current_time;
    QString getUuid(int sensor_index);
public:
    QString callerName = "";
    void setCallerName(QString name) { this->callerName = name; }
    void startRecord(QString uuid);
    void recordPrTest(QString uuid,VisionLocation location);
    void endRecord(QString uuid);
};

#endif // SENSORLOADERMODULE_H
