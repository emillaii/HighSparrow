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
    Q_ENUMS(handlePickerAction)
public:
    enum HandlePosition{

    };
    enum HandlePR{

    };
    enum HandleToWorkPos{

    };
    enum HandlePickerAction{

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
    void performHandling(int cmd);

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

    bool sensorPickerSearchZ(double z,bool is_open = true,int time_out = 10000);
    bool sensorPickerSearchSutZ(double z,QString dest,QString cmd,bool is_open = true,int time_out = 10000);
    bool pickTraySensor(int time_out = 10000);
    bool placeSensorToSUT(QString dest,int time_out = 10000);
    bool pickSUTSensor(QString dest,int time_out = 10000);
    bool pickSUTProduct(QString dest, int time_out = 10000);
    bool placeSensorToTray(int time_out = 10000);
    bool placeProductToTray(int time_out = 10000);
    bool sensorPickerMeasureHight(bool is_tray,bool is_product);

    bool measureZOffset();


    bool moveToTrayPos(int index,int tray_index);
    bool moveToTrayPos(int tray_index);
    bool moveToStartPos(int tray_index);
    bool moveToTray1EndPos();


    bool PickFromSUTPosition();



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

    PrOffset pr_offset;
};

#endif // SINGLEHEADMACHINEMATERIALLOADERMODULE_H
