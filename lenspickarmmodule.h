#ifndef LENSPICKARMMODULE_H
#define LENSPICKARMMODULE_H

#include "lenspickarmmoduleparameter.h"
#include "material_carrier.h"
#include "pick_arm_xxyp.h"
#include "thread_worker_base.h"

struct materialMessage
{
    int material_id;
    int tray_id;
};

class LensPickArmModule:public ThreadWorkerBase
{
public:
    LensPickArmModule(QString name = "LensPickArm");
    void Init(PickArmXXYP *pick_arm,MaterialTray *lens_tray,MaterialCarrier *lut_carrier,
              VisionLocation * lens_vision,VisionLocation * vacancy_vision,VisionLocation * lut_vision);
    void ResetLogic();
    void loadJsonConfig();
    void saveJsonConfig();
signals:
    void changeTray();
private:
    void run(bool has_material);

    bool moveToNextTrayPos(int tray_index);
    bool moveToLUTPRPos1();
    bool moveToLUTPRPos2();

    bool performLensPR();
    bool performVacancyPR();
    bool performLUTPR();

    bool moveToWorkPos();
    bool vcmSearchZ(double z);
    bool pickTrayLens();
    bool placeLensToLUT();
    bool pickLUTLens();
    bool placeLensToTray();


    bool moveToTrayPos(int index,int tray_index);
    bool moveToTrayPos(int column,int row,int tray_index);
    bool moveToPerformPR(int selected_pr,bool back_position,mPositionT& result);
    bool moveToPRResult(const mPositionT pr_result);
    bool moveToPerformAction(int selected_action,bool do_vision);

    bool isRunning();
    // ErrorBase interface
public:
    QString GetCurrentError();
    ErrorLevel GetCurrentErrorLevel();
    // ThreadWorkerBase interface
public slots:
    void startWork(bool reset_logic, int run_mode);
    void stopWork(bool wait_finish);
public:
    LensPickArmModuleParameter module_parameters;
    LensPickArmModuleState module_states;
    Position lut_pr_position1;
    Position lut_pr_position2;
    Position lut_camera_position;
    Position lut_picker_position;
private:
    PickArmXXYP *pick_arm = Q_NULLPTR;
    MaterialTray *lens_tray = Q_NULLPTR;
    MaterialCarrier *lut_carrier = Q_NULLPTR;
    VisionLocation * lens_vision = Q_NULLPTR;
    VisionLocation * vacancy_vision = Q_NULLPTR;
    VisionLocation * lut_vision = Q_NULLPTR;
    bool is_run = false;
    ErrorLevel error_level;
    PrOffset pr_offset;
    materialMessage lut_material;
    materialMessage lut_ng_material;
    materialMessage picked_material;
};

#endif // LENSPICKARMMODULE_H
