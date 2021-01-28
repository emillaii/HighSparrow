#ifndef LENSPICKARMMODULE_H
#define LENSPICKARMMODULE_H

#include "lensloaderparameter.h"
#include "material_carrier.h"
#include "lenspickarm.h"
#include "thread_worker_base.h"
#include "utils/unitlog.h"
#include "utils/commonutils.h"
#include "lutModule/lut_module.h"
//namespace LensPickArmEnum {

class LensLoaderModule:public ThreadWorkerBase
{
    Q_OBJECT
    Q_ENUMS(HandlePosition)
    Q_ENUMS(HandlePR)
    Q_ENUMS(HandleToWorkPos)
    Q_ENUMS(HandlePickerAction)
    Q_ENUMS(HandleMarcoAction)

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
        PICKER_PR = 70,
        LUT_NG_SLOT_PR = 80
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
        PLACE_LENS_TO_LUT2 = 3000,
        PICK_NG_LENS_FROM_LUT1 = 4000,
        PICK_NG_LENS_FROM_LUT2 = 5000,
        PLACE_NG_LENS_TO_TRAY = 6000,
        MeasureLensInTray = 7000,
        MeasureLensInLUT = 8000
    };
    enum HandleMarcoAction
    {
        CLEARANCE = 10000,
        LOAD_ONE_LENS_TO_LUT = 20000
    };

public:
    LensLoaderModule(QString name = "LensLoaderModule");
    void Init(LensPickArm *pick_arm,MaterialTray *tray,MaterialCarrier *lut_carrier,XtVacuum* load_vacuum, XtVacuum* unload_vacuum,
              VisionLocation * lens_vision,VisionLocation * vacancy_vision,VisionLocation * lut_vision, VisionLocation *lut_lens_vision,
              VisionLocation *lpa_picker_vision,VisionLocation *lpa_updownlook_up_vision, VisionLocation *lpa_updownlook_down_vision,
              VisionLocation * lut_ng_slow_vision, LutModule *lutModule);
    void loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);
    bool performUpDownlookDownPR(PrOffset &offset);
    bool performUpdowlookUpPR(PrOffset &offset);
    void setCallerName(QString name){ this->callerName = name; }
    QString callerName = "";
//    void calculateCameraToPickerOffset();
signals:
    void sendChangeTrayRequst();
    void sendLoadLensFinish(int lens,int lens_tray);
    void postCSVDataToUnit(QString uuid,QVariantMap data);
    void saveUnitDataToCSV(QString uuid);
public slots:
    void receiveLoadLensRequst(bool need_lens,int ng_lens,int ng_lens_tray);
    void receiveChangeTrayFinish();
private:
    void run(bool has_material);
//    void runTest();

    bool moveToNextTrayPos(int tray_index);
    bool moveToLUTPRPos1(bool check_softlanding = false);
    bool movePickerToLUTPos1(bool check_arrived = false,bool check_softlanding = false);
    bool moveToLUTPRPos2(bool check_softlanding = false);
    bool movePickerToLUTPos2(bool check_arrived = false,bool check_softlanding = false);

    bool checkNeedChangeTray();

    bool performLensPR();
    bool performVacancyPR();
    bool performLUTPR();
    bool performLUTNGSlotPR();
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
    bool placeLensToLUT2();
    bool pickLUT1Lens();
    bool pickLUTLens();
    bool placeLensToTray();
    bool measureHight(bool is_tray);

    bool moveToTrayEmptyPos(int index,int tray_index,int& result_tray);
    bool moveToTrayPos(int index,int tray_index);
    bool moveToTrayPos(int tray_index);
    bool moveToStartPos(int tray_index);
    bool moveToTray1EndPos();

    bool moveToUpdownlookDownPos();
    bool moveToUpdownlookUpPos();

    bool unloadAllLens();
    bool loadOneLensToLUT();

    void addCurrentNumber();
    void addCurrentNgNumber();
    Q_INVOKABLE void clearNumber();
    void updateNumber();
    double getHourSpace(QTime time_label);

    //真空操作
    bool openLoadVacuum();
    bool closeLoadVacuum();
    bool openUnloadVacuum();
    bool closeUnloadVacuum();

    bool isRunning();
    // ThreadWorkerBase interface
public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd,QVariant param);
public:
    LensLoaderModuleParameter parameters;
    LensLoaderModuleState states;
    Position lut_pr_position1;
    Position lut_pr_position2;
    Position lut_camera_position;
    Position lut_picker_position;
    Position lens_updnlook_offset;
    Position camera_to_picker_offset;
    MaterialTray *tray = Q_NULLPTR;
private:
    LensPickArm *pick_arm = Q_NULLPTR;
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
    VisionLocation * lut_ng_slot_vision = Q_NULLPTR;
    LutModule * lut = Q_NULLPTR;
    QMutex lut_mutex;
    QMutex tray_mutex;
    bool is_run = false;
    bool interruput = false;
    bool finish_stop = false;
    bool debug = false;
    ErrorLevel error_level;
    PrOffset pr_offset;

    QTime time_label;

    bool has_material = true;
    QString picked_uuid = "";
    QString lut_uuid = "";
    Unitlog unitlog;



    QMap<QString,QVariantMap> prRecordMap;

    QString getUuid(bool is_right,int current_count, int current_time);
public:
    void recordLutLensPr(QString uuid);
    void recordLutVacancyPr(QString uuid);
    void recordTrayLensPr(QString uuid);
    void recordTrayVacancyPr(QString uuid);
    void recordNgLensPr(QString uuid);
    // ThreadWorkerBase interface
public:
    void receivceModuleMessage(QVariantMap module_message);
    PropertyBase *getModuleState();
private:
    bool checkNeedPickLens();
    // ThreadWorkerBase interface
public:
    QMap<QString, PropertyBase *> getModuleParameter();
    void setModuleParameter(QMap<QString, PropertyBase *>);
};

#endif // LENSPICKARMMODULE_H
