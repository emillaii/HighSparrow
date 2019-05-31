#ifndef SINGLEHEADMACHINEMATERIALLOADERMODULE_H
#define SINGLEHEADMACHINEMATERIALLOADERMODULE_H

#include "singleheadmachinematerialloadermoduleparameter.h"
#include "singleheadmachinematerialpickarm.h"
#include "thread_worker_base.h"
#include "vision_location.h"
#define DELAY_JET 1000


class SingleHeadMachineMaterialLoaderModule:public ThreadWorkerBase
{
public:
    SingleHeadMachineMaterialLoaderModule(QString name = "SingleHeadMachineMaterialLoaderModule");
    void Init(SingleHeadMachineMaterialPickArm* pick_arm);
    void loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);
    SingleHeadMachineMaterialLoaderModuleParameter parameters;

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

//    bool PickFromSUTPosition();

    bool ToSaftyHeight(double safty_height);

public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
private:
    SingleHeadMachineMaterialPickArm* pick_arm = Q_NULLPTR;

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
