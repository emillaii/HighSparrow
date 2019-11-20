#ifndef SINGLEHEADMACHINEMATERIALLOADERMODULE_H
#define SINGLEHEADMACHINEMATERIALLOADERMODULE_H

#include "materialtray.h"
#include "singleheadmachinematerialloadermoduleparameter.h"
#include "SingleHead/singleheadmachinematerialpickarm.h"
#include "thread_worker_base.h"
#include "Vision/vision_location.h"
#include "Utils/commonutils.h"

#define DELAY_JET 1000


class SingleHeadMachineMaterialLoaderModule:public ThreadWorkerBase
{
    Q_OBJECT
    Q_ENUMS(HandlePosition)
    Q_ENUMS(HandlePR)
    Q_ENUMS(HandleToWorkPos)
    Q_ENUMS(HandlePickerAction)
    Q_ENUMS(HandlePROffset)
public:
    enum HandlePosition{
        LENS_TRAY1 = 1,
        LENS_TRAY2 = 2,
        LENS_TRAY1_START_POS = 3,
        LENS_TRAY2_START_POS = 4,
        LENS_TRAY1_END_POS = 5,
        SENSOR_TRAY = 6,
        SENSOR_TRAY_START_POS = 7,
        SENSOR_TRAY_END_POS = 8,
        REJECT_TRAY = 9,
        REJECT_TRAY_START_POS = 10,
        REJECT_TRAY_END_POS = 11,
        LUT_PR_POS = 12,
        SUT_PR_POS = 13,

        HANDLE_POSITION = (1<<6)-1
    };
    enum HandlePR{
        RESET_PR = 1<<6,
        LENS_PR = 2<<6,
        LENS_VACANCY_PR =3<<6,
        SENSOR_PR = 4<<6,
        SENSOR_VACANCY_PR = 5<<6,
        LUT_PR = 6<<6,
        NG_LENS_PR = 7<<6,
        SUT_PR = 8<<6,
        NG_SENSOR_PR = 9<<6,
        PRODUCT_PR = 10<<6,

        HANDLE_PR = (1<<11) - (1<<6)
    };
    enum HandleToWorkPos{

        PICKER1_TO_WORKPOS = 1<<11,
        PICKER2_TO_WORKPOS = 2<<11,

        HANDLE_TO_WORKPOS = (1<<16)-(1<<11)
    };
    enum HandlePickerAction{

        //Start
        PICKER1_PICK_LENS_FROM_TRAY = 1<<16,
        PICKER2_PICK_SENSOR_FROM_TRAY =2<<16,
        //Pass
        PICKER1_PLACE_LENS_TO_LUT = 3<<16,
        PICKER1_PICK_PRODUCT_FROM_SUT = 4<<16,
        PICKER2_PLACE_SENSOR_TO_SUT = 5<<16,
        PICKER1_PLACE_PRODUCT_TO_TRAY = 6<<16,
        //NG Both
        PICKER1_PLACE_LENS_TO_TRAY = 7<<16,
        PICKER2_PLACE_SENSOR_TO_TRAY = 8<<16,
        PICKER1_PICK_NG_LENS_FROM_LUT = 9<<16,
        PICKER2_PICK_NG_SENSOR_FROM_SUT = 10<<16,
        PICKER1_PLACE_NG_LENS_TO_TRAY = 11<<16,
        PICKER2_PLACE_NG_SENSOR_TO_TRAY = 12<<16,
        //NG Lens
        PICKER2_PICK_NG_LENS_FROM_LUT = 13<<16,
        PICKER2_PLACE_NG_LENS_TO_TRAY = 14<<16,
        //NG Sensor
        PICKER1_PICK_NG_SENSOR_FROM_SUT = 15<<16,
        PICKER1_PLACE_NG_SENSOR_TO_TRAY = 16<<16,
        //NG Product
        PICKER1_PLACE_NG_PRODUCT_TO_TRAY = 17<<16,
        //Measure high
        PICKER1_MEASURE_LENS_IN_TRAY = 18<<16,
        PICKER1_MEASURE_LENS_IN_LUT = 19<<16,
        PICKER1_MEASURE_PRODUCT_IN_SUT = 20<<16,
        PICKER1_MEASURE_PRODUCT_IN_TRAY = 21<<16,

        PICKER2_MEASURE_SENSOR_IN_TRAY = 22<<16,
        PICKER2_MEASURE_SENSOR_IN_SUT = 23<<16,

        //measure camer tip


        HANDLE_PICKER_ACTION = (1<<23)-(1<<16)
    };
    enum HandlePROffset
    {
        APPLY_PLACE_NG_SENSOR_TO_TRAY_OFFSET = 2 << 23,
        APPLY_PLACE_NG_PRODUCT_TO_TRAY_OFFSET = 3 << 23,

        HANDLE_PR_OFFSET = (1 << 25) - (1 << 23)
    };

    SingleHeadMachineMaterialLoaderModule(QString name = "SingleHeadMachineMaterialLoaderModule");
    void Init(SingleHeadMachineMaterialPickArm* pick_arm,
              MaterialTray* sensorTray,
              MaterialTray* lensTray,
              MaterialTray* rejectTray,
              VisionLocation* sensor_vision = nullptr,
              VisionLocation* sensor_vacancy_vision = nullptr,
              VisionLocation* sut_vision = nullptr,
              VisionLocation* sut_sensor_vision = nullptr,
              VisionLocation* sut_product_vision = nullptr,
              VisionLocation* lens_vision = nullptr,
              VisionLocation* lens_vacancy_vision = nullptr,
              VisionLocation* lut_vision = nullptr,
              VisionLocation* lut_lens_vision = nullptr,
//              VisionLocation* camera_to_picker_offest_vision = nullptr,
              XtVacuum* sutVacuum = nullptr,
              XtVacuum* lutVacuum = nullptr);
    void loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);
    Q_INVOKABLE void performHandling(int cmd);
    Q_INVOKABLE bool moveToChangeTrayPos();

public:
    SingleHeadMachineMaterialLoaderModuleParameter parameters;
    MaterialLoaderState states;

    Position sut_pr_position;
    Position lut_pr_position;
    Position camera_to_picker1_offset;
    Position camera_to_picker2_offset;

    PositionT placeNgSensorToTrayOffset;
    PositionT placeNgProductToTrayOffset;
    Position placeOkProductToTrayOffset;

private:
    void run();

    //PR動作
    bool performSensorPR();
    bool performSensorVacancyPR();
    bool performSUTPR();
    bool performSUTSensorPR();
    bool performSUTProductPR();
    bool performLensPR();
    bool performLensVacancyPR();
    bool performLUTPR();
    bool performLUTLensPR();

    //pickArm and trayloder動作
    bool moveToNextLensTrayPos(int tray_index);
    bool moveToLensTrayPos(int tray_index);
    bool moveToLensTrayStartPos(int tray_index);
    bool moveToLensTrayEndPos();

    bool moveToNextSensorTrayPos(int tray_index);
    bool moveToSensorTrayPos(int tray_index);
    bool moveToSensorTrayStartPos(int tray_index);
    bool moveToSensorTrayEndPos();

    bool moveToNextRejectTrayPos(int tray_index);
    bool moveToRejectTrayPos(int tray_index);
    bool moveToRejectTrayStartPos(int tray_index);
    bool moveToRejectTrayEndPos();

    bool moveToPicker1WorkPos(bool check_softlanding = false);
    bool moveToPicker2WorkPos(bool check_softlanding = false);
    bool moveToLUTPRPos(bool check_softlanding = false);
    bool moveToSUTPRPos(bool check_softlanding = false);


    //start
    bool picker1PickLensFromTray(int time_out = 10000);
    bool picker2PickSensorFromTray(int time_out = 10000);
    //pass
    bool picker1PlaceLensToLUT(int time_out = 10000);
    bool picker1PickProductFormSUT(int time_out = 10000);
    bool picker2PlaceSensorToSUT(int time_out = 10000);
    bool picker1PlaceProductToTray(int time_out = 10000);
    //NG both
    bool picker1PlaceLensToTray(int time_out = 10000);
    bool picker2PlaceSensorToTray(int time_out = 10000);
    bool picker1PickNgLensFromLUT(int time_out = 10000);
    bool picker2PickNgSensorFromSUT(int time_out = 10000);
    bool picker1PlaceNgLensToTray(int time_out = 10000);
    bool picker2PlaceNgSensorToTray(int time_out = 10000);
    //NG lens
    bool picker2PickNgLensFromLUT(int time_out = 10000);
    bool picker2PlaceNgLensToTray(int time_out = 10000);
    //NG sensor
    bool picker1PickNgSensorFromSUT(int time_out = 10000);
    bool picker1PlaceNgSensorToTray(int time_out = 10000);

    //Picker motion
    bool picker1SearchZ(double z,bool is_open = true,int time_out = 10000,int picker = 0);
    bool picker1SearchSutZ(double z,bool is_open = true,int time_out = 10000);
    bool picker2SearchZ(double z,bool is_open = true,int time_out = 10000,int picker = 0);
    bool picker2SearchSutZ(double z,bool is_open = true,int time_out = 10000);
    bool picker2SearchSutZ2(double z,bool is_open = true,int time_out = 10000);
    bool picker2SearchSutZ2Revert(double z,bool is_open = true,int time_out = 10000);
    bool picker1SearchSutZ2(double z,bool is_open = true, int time_out = 10000);
    bool picker1MeasureHight(bool is_tray,bool is_product = false);
    bool picker2MeasureHight(bool is_tray,bool is_product = false);

    //PR Offset
    void applyPrOffset(PositionT& offset);

signals:
    void sendLoadMaterialFinishSignal(int sensor_index, int lens_index);

public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);

    void receiveLoadMaterialRequestResponse(bool need_sensor, bool need_lens, bool has_ng_sensor, bool has_ng_lens, bool has_product, bool isSutReadyToLoadMaterial);
private:

    SingleHeadMachineMaterialPickArm* pick_arm = Q_NULLPTR;
    MaterialTray *sensorTray;
    MaterialTray *lensTray;
    MaterialTray *rejectTray = Q_NULLPTR;
    XtVacuum* sut_vacuum = Q_NULLPTR;
    XtVacuum* lut_vacuum = Q_NULLPTR;

    VisionLocation* sensor_vision = Q_NULLPTR;
    VisionLocation* sensor_vacancy_vision = Q_NULLPTR;
    VisionLocation* sut_vision = Q_NULLPTR;
    VisionLocation* sut_sensor_vision = Q_NULLPTR;
    VisionLocation* sut_product_vision = Q_NULLPTR;
    VisionLocation* lens_vision = Q_NULLPTR;
    VisionLocation* lens_vacancy_vision = Q_NULLPTR;
    VisionLocation* lut_vision = Q_NULLPTR;
    VisionLocation* lut_lens_vision = Q_NULLPTR;

//    VisionLocation* camera_to_picker_offest_vision = Q_NULLPTR;

    QMutex lsut_mutex;
    QMutex materialLoader_mutex;

    bool is_run = false;
    bool finish_stop = false;
    PrOffset pr_offset;
    QPointF lastPickSensorPos;
    QPointF currentPickSensorPos;

    int lensPrFailedTimes = 0;
    int sensorPrFailedTimes = 0;
    const int MaxPickDutFailedTimes = 5;
};

#endif // SINGLEHEADMACHINEMATERIALLOADERMODULE_H
