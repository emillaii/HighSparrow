#ifndef SINGLEHEADMACHINEMATERIALLOADERMODULE_H
#define SINGLEHEADMACHINEMATERIALLOADERMODULE_H

#include "materialtray.h"
#include "singleheadmachinematerialloadermoduleparameter.h"
#include "singleheadmachinematerialpickarm.h"
#include "thread_worker_base.h"
#include "vision_location.h"
#define DELAY_JET 1000


class SingleHeadMachineMaterialLoaderModule:public ThreadWorkerBase
{
    Q_OBJECT
    Q_ENUMS(HandlePosition)
    Q_ENUMS(HandlePR)
    Q_ENUMS(HandleToWorkPos)
    Q_ENUMS(HandlePickerAction)
public:
    enum HandlePosition{
        LENS_TRAY1 = 1,
        LENS_TRAY2 = 2,
        LUT_POS = 3,
        LENS_TRAY1_START_POS = 5,
        LENS_TRAY2_START_POS = 6,
        LENS_TRAY1_END_POS = 7,
        UPDOWNLOOK_DOWN_POS = 8,
        UPDOWNLOOK_UP_POS =9,
        SENSOR_TRAY = 10,
        SUT_POS = 11,
        SENSOR_TRAY_START_POS = 12,
        SENSOR_TRAY_END_POS = 13,

        HANDLE_POSITION = (1<<6)-1
    };
    enum HandlePR{
        RESET_PR = 1<<6,
        SENSOR_PR = 2<<6,
        SENSOR_VACANCY_PR = 3<<6,
        SUT_PR = 4<<6,
        NG_SENSOR_PR = 5<<6,
        PRODUCT_PR = 6<<6,
        LENS_PR = 7<<6,
        LENS_VACANCY_PR = 8<<6,
        LUT_PR = 9<<6,
        UPDOWNLOOK_DOWN_PR = 10<<6,
        LENS_PICKER_PR = 11<<6,

        HANDLE_PR = (1<<11) - (1<<6)
    };
    enum HandleToWorkPos{
        LPA_TO_WORK = 1<<11,
        LPA_TO_PROFFSET = 2<<11,

        SENSOR_TO_PICK = 3<<11,
        SENSOR_TO_PR_OFFSET = 4<<11,

        HANDLE_TO_WORKPOS = (1<<16)-(1<<11)
    };
    enum HandlePickerAction{
        PICK_SENSOR_FROM_TRAY = 1<<16,
        PLACE_SENSOR_TO_SUT = 2<<16,
        PICK_NG_SENSOR_FROM_SUT = 3<<16,
        PLACE_NG_SENSOR_TO_TRAY = 4<<16,
        PICK_PRODUCT_FROM_SUT = 5<<16,
        PLACE_PRODUCT_TO_TRAY = 6<<16,
        MEASURE_SENSOR_IN_TRAY = 7<<16,
        MEASURE_NG_SENSOR_IN_TRAY = 8<<16,
        MEASURE_PRODUCT_IN_TRAY = 9<<16,
        MEASURE_SENSOR_IN_SUT = 10<<16,
        MEASURE_NG_SENSOR_IN_SUT = 11<<16,
        MEASURE_PRODUCT_IN_SUT = 12<<16,

        PICK_LENS_FROM_TRAY = 13<<16,
        PLACE_LENS_TO_LUT = 14<<16,
        PICK_NG_LENS_FROM_LUT = 15<<16,
        PLACE_NG_LENS_TO_TRAY = 16<<16,
        MEASURE_LENS_IN_TRAY = 17<<16,
        MEASURE_LENS_IN_LUT = 18<<16,

        HANDLE_PICKER_ACTION = (1<<23)-(1<<16)
    };

    SingleHeadMachineMaterialLoaderModule(QString name = "SingleHeadMachineMaterialLoaderModule");
    void Init(SingleHeadMachineMaterialPickArm* pick_arm,
              MaterialTray* _sensorTray,
              MaterialTray* _lensTray,
              VisionLocation* _sensor_vision = nullptr,
              VisionLocation* _sensor_vacancy_vision = nullptr,
              VisionLocation* _sut_vision = nullptr,
              VisionLocation* _sut_sensor_vision = nullptr,
              VisionLocation* _sut_product_vision = nullptr,
              VisionLocation* _lens_vision = nullptr,
              VisionLocation* _lens_vacancy_vision = nullptr,
              VisionLocation* _lut_vision = nullptr,
              VisionLocation* _lut_lens_vision = nullptr,
              XtVacuum* sutVacuum = nullptr,
              XtVacuum* lutVacuum = nullptr);
    void loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);
    SingleHeadMachineMaterialLoaderModuleParameter parameters;
    Q_INVOKABLE void performHandling(int cmd);

//old motions
    bool ToPickCmosPosition();
    bool PickCMOS(double force);
    bool ToPickLensPosition();
    bool PickLens(double force);
    bool ToSUTPosition();
    bool CheckSUTExistCmos();
    bool PlaceToSUT(double force);
    bool ToLUTPosition();
    bool picker1ToLUTPosition();
    bool CheckLUTExistLens();
    bool PlaceToLUT(double force);

    bool CheckVcm1NoWorkPeice();
    bool CheckVcm2NoWorkPeice();

    bool ToPickProductPosition();
    bool PickProduct(double force);
    bool picker1ToSUTPosition();
    bool picker1PickFromSUT(double force);
    bool picker2ToLUTPosition();
    bool picker2PickFromLUT(double force);
    bool ToPlaceProductPosition();
    bool PlaceProductToTray(double force);
    bool picker1ToCMOSPosition();
    bool picker1PlaceCMOSToTray(double force);
    bool picker2ToLensPosition();
    bool picker2PlacelensToTray(double force);
    bool picker1ToLensPosition();
    bool picker1PlaceLensToTray(double force);
    bool picker2ToCmosPosition();
    bool picker2PlaceCmosToTray(double force);

    bool MoveSensorPicker2Cam();
    bool MoveCam2SensorPicker();
    bool MoveLensPikcer2Cam();
    bool MoveCam2LensPicker();
    bool XYZSyncMove(double xpos,double ypos,double zpos);
    bool ToSaftyHeight(double safty_height);
//old motions

    Position sut_pr_position;
    Position lut_pr_position;

    Position updownlook_up_position;
    Position updownlook_down_position;

    Position lens_suction_offset;
    Position sensor_suction_offset;
//new motions
private:
    void run(bool has_material);

    bool checkTrayNeedChange();
    bool moveToNextSensorTrayPos(int tray_index);
    bool moveToSUTPRPos(bool is_local = true,bool check_softlanding = false);

    bool performSensorPR();
    bool performSensorVacancyPR();
    bool performSUTPR();
    bool performSUTSensorPR();
    bool performSUTProductPR();

    bool moveToSPAWorkPos(bool check_softlanding = false);

    bool sensorPickerSearchZ(double z,bool is_open = true,int time_out = 10000,int picker = 0);
    bool sensorPickerSearchSutZ(double z,QString dest,QString cmd,bool is_open = true,int time_out = 10000);
    bool lensPickerSearchSutZ(double z,QString dest,QString cmd,bool is_open = true,int time_out = 10000);
    bool pickTraySensor(int time_out = 10000);
    bool placeSensorToSUT(QString dest,int time_out = 10000);
    bool pickSUTSensor(QString dest,int time_out = 10000);
    bool pickSUTProduct(QString dest, int time_out = 10000);
    bool placeSensorToTray(int time_out = 10000);
    bool placeProductToTray(int time_out = 10000);
    bool sensorPickerMeasureHight(bool is_tray,bool is_product);


    bool moveToSensorTrayPos(int index,int tray_index);
    bool moveToSensorTrayPos(int tray_index);
    bool moveToSensorStartPos(int tray_index);
    bool moveToSensorTray1EndPos();


    bool moveToNextLensTrayPos(int tray_index);
    bool moveToLUTPRPos(bool check_softlanding = false);

    bool checkNeedChangeTray();

    bool performLensPR();
    bool performLensVacancyPR();
    bool performLUTPR();
    bool performLensPickerPR();

    bool moveToLPAWorkPos(bool check_softlanding = false);
    bool moveToLPAPrOffset(bool check_softlanding = false);
    bool vcm2SearchZ(double z,bool is_open = true);
    bool pickTrayLens();
    bool placeLensToLUT();
    bool pickLUTLens();
    bool placeLensToTray();
    bool lensPickerMeasureHight(bool is_tray);

    bool moveToLensTrayEmptyPos(int index,int tray_index);
    bool moveToLensTrayPos(int index,int tray_index);
    bool moveToLensTrayPos(int tray_index);
    bool moveToLensStartPos(int tray_index);
    bool moveToLensTray1EndPos();

    bool moveToUpdownlookDownPos();
    bool moveToUpdownlookUpPos();

    bool isRunning();


public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
private:
    bool is_run = false;
    SingleHeadMachineMaterialPickArm* pick_arm = Q_NULLPTR;
    MaterialTray *sensorTray;
    MaterialTray *lensTray;
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

    VisionLocation * lpa_picker_vision = Q_NULLPTR;

    PrOffset pr_offset;
};

#endif // SINGLEHEADMACHINEMATERIALLOADERMODULE_H
