#ifndef SINGLEHEAD_LSUT_MODULE_H
#define SINGLEHEAD_LSUT_MODULE_H

#include "XtMotion/XtCylinder.h"
#include "material_carrier.h"
#include "singleheadlsutparameter.h"
#include "thread_worker_base.h"
#include "Vision/vision_location.h"
#include "AAHeadModule/aaheadmodule.h"

//! This class is defined as LSUT module in single-head AA machine
//! LSUT is named as LUT and SUT is combined in single-head AA machine
class SingleheadLSutModule : public ThreadWorkerBase
{
    Q_OBJECT
    Q_ENUMS(HandlePosition)
    Q_ENUMS(HandlePR)
    Q_ENUMS(HandleToWorkPos)
    Q_ENUMS(HandlePick)

public:
    enum HandlePosition
    {
        MOVE_TO_MUSHROOM_POSITION = 1,
        MOVE_TO_GRIPPER_POSITION = 2,
        MOVE_TO_LOAD_SENSOR_POSITION = 3,
        MOVE_TO_LOAD_LENS_POSITION = 4,
        MOVE_TO_PR_POSITION = 5,
        MOVE_TO_CALIBRATION_POSITION = 6,
        MOVE_TO_SAFETY_POSITION = 7,
        MOVE_TO_PICK_LENS_POSITION = 8,
        MOVE_TO_UNPICK_LENS_POSITION = 9,
    };

    enum HandlePR
    {
        RESET_PR = 10,
        DOWNLOOK_SENSOR_PR = 20,    // Move sensor to downlook camera and do PR
        UPLOOK_LENS_PR = 30,         // lens in gripper, do PR with uplook camera
        UPLOOK_GRIPPER_PR = 40
    };

    enum HandleToWorkPos
    {
        LENS_TO_GRIPPER_CENTER = 100,
        LENS_TO_GRIPPER = 200,
    };

    enum HandlePick
    {
        LENS_GRIPPER_MEASURE_HEIGHT = 1000,
        GRAB_LENS_TO_GRIPPER = 2000,
        UNPICK_LENS_TO_LUT = 3000
    };

    enum LUTState
    {
        NO_LENS,
        HAS_LENS,
        BUSY,
        HAS_NG_LENS
    };

public:
    SingleheadLSutModule(QString name = "LSutModule", QObject *parent = nullptr);
    void Init(MaterialCarrier *sut_carrier,
              VisionLocation *downlook_location,
              VisionLocation *mushroom_location,
              VisionLocation * gripper_location,
              XtVacuum* sutVacuum,
              XtVacuum* lutVacuum,
              XtGeneralOutput *pogopin,
              AAHeadModule * aaHead);

    MaterialCarrier* sut_carrier;
    AAHeadModule* aa_head;
    Position3D mushroom_position;
    Position3D load_sensor_position;
    Position3D load_lens_position;
    Position3D downlook_position;
    Position3D gripper_position;
    Position3D calibration_position;
    Position3D safety_position;
    Position3D pick_lens_position;
    Position3D unpick_lens_position;
    PositionT  up_downlook_offset;

    SingleHeadLSutParameter parameters;
    LSutState states;

    //! Load LSUT parameters from config file
    void loadParams(QString file_name);
    //! Save LSUT parameters to config file
    void saveParams(QString file_name);

    Q_INVOKABLE void performHandling(int cmd);

    bool moveToZPos(double z);
    bool stepMove_XY_Sync(double x,double y);
    bool stepMove_Z_Sync(double step_z);
    void recordCurrentPos();
    bool movetoRecordPos(bool check_autochthonous = false);
    bool moveToDownlookPR(PrOffset &offset,bool close_lighting = true,bool check_autochthonous = false);
    bool moveToDownlookSaveImage(QString filename);
    // Distance offset between lens center to uplook camera center
    Position lens_offset;
    // Distance offset between sensor center to uplook camera center
    Position sensor_offset;

    bool gripLens();
    bool unpickLens();
signals:
//    void sendLoadLensRequst(bool need_lens,int ng_lens,int ng_lens_tray);
//    void sendLoadSensorFinish(double offset_x,double offset_y,double offset_z);
    void sendLoadMaterialRequest(bool need_sensor, bool need_lens, bool has_ng_sensor, bool has_ng_lens, bool has_product);
    void sendStartAAProcessRequest();
public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
    void receiveMaterialResponse(int sensor_index, int lens_index);
    void receiveAAProcessResponse(bool has_ng_sensor, bool has_ng_lens, bool has_product, bool has_ng_product);
public:
    void run(bool isProduct);
    void lens_logic();
    void sensor_logic();
    bool moveToMushroomPosition(bool check_autochthonous = false);
    bool moveToGripperPosition(bool check_autochthonous = false);
    bool moveToLoadSensorPosition(bool check_autochthonous = false);
    bool moveToLoadLensPosition(bool check_autochthonous = false);
    bool moveToPRPosition(bool check_autochthonous = false);
    bool moveToCalibrationPosition(bool check_autochthonous = false);
    bool moveToSafetyPosition(bool check_autochthonous = false);
    bool moveToPickLensPosition(bool check_autochthonous = false);
    bool moveToUnpickLensPosition(bool check_autochthonous = false);

    bool performDownlookSensorPR();
    bool performUplookLensPR();
    bool performUplookGripperPR();

    bool moveLensToGripperCenter();
    bool moveLensToGripperPos(double step_z= 0);
    bool lensGripperMeasureHight();




private:
    bool is_run = false;
    XtGeneralOutput* pogopin = Q_NULLPTR;
    VisionLocation* vision_downlook_location = Q_NULLPTR;
    VisionLocation* vision_mushroom_location = Q_NULLPTR;
    VisionLocation* vision_gripper_location = Q_NULLPTR;   //Is this vision location for lens in gripper?
    XtVacuum *vacuum_lut;
    XtVacuum *vacuum_sut;
    PrOffset pr_offset;
    mPoint3D record_position;
    LUTState state = NO_LENS;
};

#endif // SINGLEHEAD_LSUT_MODULE_H
