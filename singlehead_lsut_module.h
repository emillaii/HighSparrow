#ifndef SINGLEHEAD_LSUT_MODULE_H
#define SINGLEHEAD_LSUT_MODULE_H

#include "XtMotion/XtCylinder.h"
#include "material_carrier.h"
#include "singleheadlsutparameter.h"
#include "thread_worker_base.h"
#include "vision_location.h"

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
        MOVE_TO_LOAD_POSITION = 3,
        MOVE_TO_PR_POSITION = 4,
        MOVE_TO_CALIBRATION_POSITION = 5,
        MOVE_TO_SAFETY_POSITION = 6,
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
        SENSOR_TO_BOND = 100,
        LENS_TO_GRIPPER = 200,
    };

    enum HandlePick
    {
        LENS_GRIPPER_MEASURE_HEIGHT = 1000,
        //GRAB_LENS_TO_GRIPPER = 2000
    };

    enum LUTState
    {
        NO_LENS,
        HAS_LENS,
        BUSY
    };

public:
    SingleheadLSutModule(QString name = "LSutModule", QObject *parent = nullptr);
    void Init(MaterialCarrier *_sut_carrier,
              VisionLocation *_downlook_location,
              VisionLocation *_updownlook_down_location,
              VisionLocation *_updownlook_up_location,
              VisionLocation *_mushroom_location,
              VisionLocation * _gripper_location,
              XtVacuum* _sutVacuum,
              XtVacuum* _lutVacuum,
              XtCylinder *_pogopin);

    MaterialCarrier* sut_carrier;

    Position3D mushroom_position;
    Position3D load_position;
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
    Q_INVOKABLE bool moveToCamPos(double pixel_x, double pixel_y, int upDownLook);

    bool moveToZPos(double z);
    bool stepMove_XY_Sync(double x,double y);
    bool stepMove_Z_Sync(double step_z);
    void recordCurrentPos();
    bool movetoRecordPos(bool check_autochthonous = false);
    bool moveToDownlookPR(PrOffset &offset,bool close_lighting = true,bool check_autochthonous = false);

    // Distance offset between lens center to uplook camera center
    Position lens_offset;
    // Distance offset between sensor center to uplook camera center
    Position sensor_offset;

signals:
    void sendLoadLensRequst(bool need_lens,int ng_lens,int ng_lens_tray);
    void sendLoadSensorFinish(double offset_x,double offset_y,double offset_z);

public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);

private:
    void run(bool isProduct);

    bool moveToMushroomPosition(bool check_autochthonous = false);
    bool moveToGripperPosition(bool check_autochthonous = false);
    bool moveToLoadPosition(bool check_autochthonous = false);
    bool moveToPRPosition(bool check_autochthonous = false);
    bool moveToCalibrationPosition(bool check_autochthonous = false);
    bool moveToSafetyPosition(bool check_autochthonous = false);
    bool moveToPickLensPosition(bool check_autochthonous = false);
    bool moveToUnpickLensPosition(bool check_autochthonous = false);

    bool performDownlookSensorPR();
    bool performUplookLensPR();
    bool performUplookGripperPR();

    bool moveToLensWorkPos();
    bool moveToSensorWorkPos();

    bool moveLensToGripper();
    bool lensGripperMeasureHight();

private:
    bool is_run = false;
    XtCylinder* pogopin = Q_NULLPTR;
    VisionLocation* vision_downlook_location = Q_NULLPTR;
    VisionLocation* vision_updownlook_down_location = Q_NULLPTR;
    VisionLocation* vision_updownlook_up_location = Q_NULLPTR;
    VisionLocation* vision_mushroom_location = Q_NULLPTR;
    VisionLocation* vision_gripper_location = Q_NULLPTR;   //Is this vision location for lens in gripper?
    XtVacuum *vacuum_lut;
    XtVacuum *vacuum_sut;
    PrOffset pr_offset;
    mPoint3D record_position;

    LUTState state = NO_LENS;
};

#endif // SINGLEHEAD_LSUT_MODULE_H
