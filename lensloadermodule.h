#ifndef LENSPICKARMMODULE_H
#define LENSPICKARMMODULE_H

#include "lensloaderparameter.h"
#include "material_carrier.h"
#include "lenspickarm.h"
#include "thread_worker_base.h"

//namespace LensPickArmEnum {

class LensLoaderModule:public ThreadWorkerBase
{
    Q_OBJECT
public:
    enum HandlePosition
        {
            LENS_TRAY1 = 1,
            LENS_TRAY2 = 2,
            LUT_POS1 = 3,
            LUT_POS2 = 4,
            LENS_TRAY1_START_POS = 5,
            LENS_TRAY2_START_POS = 6,
            LENS_TRAY1_END_POS = 7,
            UPDOWNLOOK_DOWN_POS = 8,
            UPDOWNLOOK_UP_POS =9
        };
    enum HandlePR
    {
        RESET_PR = 10,
        LENS_PR = 20,
        VACANCY_PR = 30,
        LUT_PR = 40,
        LUT_LENS_PR = 50,
        UPDOWNLOOK_DOWN_PR = 60,
        PICKER_PR = 70
    };
    enum HandleToWorkPos
    {
        ToWork = 100,
        ToPrOffset = 200,
    };
    enum HandlePickerAction
    {
        PICK_LENS_FROM_TRAY = 1000,
        PLACE_LENS_TO_LUT = 2000,
        PICK_NG_LENS_FROM_LUT = 3000,
        PLACE_NG_LENS_TO_TRAY = 4000,
        MeasureLensInTray = 5000,
        MeasureLensInLUT = 6000
    };

public:
    LensLoaderModule(QString name = "LensPickArmModule");
    void Init(LensPickArm *pick_arm,MaterialTray *tray,MaterialCarrier *lut_carrier,XtVacuum* load_vacuum, XtVacuum* unload_vacuum,
              VisionLocation * lens_vision,VisionLocation * vacancy_vision,VisionLocation * lut_vision, VisionLocation *lut_lens_vision,
              VisionLocation *lpa_picker_vision,VisionLocation *lpa_updownlook_up_vision, VisionLocation *lpa_updownlook_down_vision);
    void loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);
    bool performUpDownlookDownPR(PrOffset &offset);
    bool performUpdowlookUpPR(PrOffset &offset);
//    void calculateCameraToPickerOffset();
signals:
    void sendChangeTrayRequst();
    void sendLoadLensFinish(int lens,int lens_tray);
public slots:
    void receiveLoadLensRequst(bool need_lens,int ng_lens,int ng_lens_tray);
    void receiveChangeTrayFinish();
private:
    void run(bool has_material);

    bool moveToNextTrayPos(int tray_index);
    bool moveToLUTPRPos1(bool check_softlanding = false);
    bool movePickerToLUTPos1();
    bool moveToLUTPRPos2(bool check_softlanding = false);
    bool movePickerToLUTPos2();

    bool checkNeedChangeTray();

    bool performLensPR();
    bool performVacancyPR();
    bool performLUTPR();
    bool performLUTLensPR();
    bool performPickerPR();

    bool moveToWorkPos(bool check_state);
    bool moveToWorkPos();

    bool checkPickedLensOrNg(bool check_state);
    bool checkLutLens(bool check_state);
    bool checkLutNgLens(bool check_state);

    bool moveToPrOffset(bool check_softlanding = false);
    bool vcmSearchZ(double z,bool is_open = true);
    bool vcmSearchLUTZ(double z,bool is_open = true);
    bool vcmSearchReturn();
    bool pickTrayLens();
    bool placeLensToLUT();
    bool pickLUTLens();
    bool placeLensToTray();
    bool measureHight(bool is_tray);

    bool moveToTrayEmptyPos(int index,int tray_index);
    bool moveToTrayPos(int index,int tray_index);
    bool moveToTrayPos(int tray_index);
    bool moveToStartPos(int tray_index);
    bool moveToTray1EndPos();

    bool moveToUpdownlookDownPos();
    bool moveToUpdownlookUpPos();

    bool isRunning();
    // ThreadWorkerBase interface
public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
public:
    LensLoaderModuleParameter parameters;
    LensPickArmModuleState states;
    Position lut_pr_position1;
    Position lut_pr_position2;
    Position lut_camera_position;
    Position lut_picker_position;
    Position lens_updnlook_offset;
    Position camera_to_picker_offset;
private:
    LensPickArm *pick_arm = Q_NULLPTR;
    MaterialTray *tray = Q_NULLPTR;
    MaterialCarrier *lut_carrier = Q_NULLPTR;
    XtVacuum* load_vacuum = Q_NULLPTR;
    XtVacuum* unload_vacuum = Q_NULLPTR;
    VisionLocation * lens_vision = Q_NULLPTR;
    VisionLocation * vacancy_vision = Q_NULLPTR;
    VisionLocation * lut_vision = Q_NULLPTR;
    VisionLocation * lut_lens_vision = Q_NULLPTR;
    VisionLocation * lpa_picker_vision = Q_NULLPTR;
    VisionLocation * lpa_updownlook_up_vision = Q_NULLPTR;
    VisionLocation * lpa_updownlook_down_vision = Q_NULLPTR;
    QMutex lut_mutex;
    QMutex tray_mutex;
    bool is_run = false;
    bool interruput = false;
    bool finish_stop = false;
    bool debug = false;
    ErrorLevel error_level;
    PrOffset pr_offset;
};

#endif // LENSPICKARMMODULE_H
