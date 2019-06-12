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
public:
    SingleHeadMachineMaterialLoaderModule(QString name = "SingleHeadMachineMaterialLoaderModule");
    void Init(SingleHeadMachineMaterialPickArm* pick_arm,MaterialTray*,MaterialTray*);
    void loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);
    SingleHeadMachineMaterialLoaderModuleParameter parameters;

    bool ToPickCmosPosition();
    bool PickCMOS(double force,int time_out = 10000);
    bool ToPickLensPosition();
    bool PickLens(double force);
    bool ToSUTPosition();
    bool CheckSUTExistCmos();
    bool PlaceToSUT(double force,QString dest,int time_out = 10000);
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

    bool picker1SearchZ(double z,bool is_open = true,int time_out = 10000);
    bool picker2SearchSutZ(double z,QString dest,QString cmd,bool is_open = true,int time_out = 10000);

//    bool PickFromSUTPosition();

    bool ToSaftyHeight(double safty_height);

    Position sut_pr_position;
    Position lut_pr_position;
    Position updownlook_up_position;
    Position updownlook_down_position;
    Position lens_suction_offset;
    Position sensor_suction_offset;

public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
private:
    SingleHeadMachineMaterialPickArm* pick_arm = Q_NULLPTR;
    MaterialTray *sensorTray;
    MaterialTray *lensTray;
    VisionLocation* sensor_vision = Q_NULLPTR;
    VisionLocation* sensor_vacancy_vision = Q_NULLPTR;
    VisionLocation* sut_vision = Q_NULLPTR;
    VisionLocation* sut_sensor_vision = Q_NULLPTR;
    VisionLocation* sut_product_vision = Q_NULLPTR;

    VisionLocation* lens_vision = Q_NULLPTR;
    VisionLocation* lens_vacancy_vision = Q_NULLPTR;
    VisionLocation* lut_vision = Q_NULLPTR;
    VisionLocation* lut_lens_vision = Q_NULLPTR;


};

#endif // SINGLEHEADMACHINEMATERIALLOADERMODULE_H
