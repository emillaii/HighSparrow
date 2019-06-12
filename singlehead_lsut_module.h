#ifndef SINGLEHEAD_LSUT_MODULE_H
#define SINGLEHEAD_LSUT_MODULE_H

#include "XtCylinder.h"
#include "material_carrier.h"
#include "singleheadlsutparameter.h"
#include "thread_worker_base.h"
#include "vision_location.h"


//! This class is defined as LSUT module in single-head AA machine
//! LSUT is named as LUT and SUT is combined in single-head AA machine
class SingleheadLSutModule : public ThreadWorkerBase
{
    Q_OBJECT
    Q_ENUMS(HandleEnum)

public:
    enum HandleMoveToPoaition
    {
        MOVE_TO_MUSHROOM_POSITION = 1,
        MOVE_TO_LOAD_POSITION = 2,
        MOVE_TO_PR_POSITION = 3,
        MOVE_TO_CALIBRATION_POSITION = 4,
    };

public:
    SingleheadLSutModule(QString name = "LSutModule", QObject *parent = nullptr);
    void Init(MaterialCarrier* _sut_carrier,XtCylinder* _pogopin);
    Q_INVOKABLE void performHandling(int cmd);

    MaterialCarrier* sut_carrier;

    Position3D load_uplook_position;
    Position lpa_camera_to_picker_offset;
    Position3D lut_load_position;
    Position3D lut_downlook_load_position;
    Position3D lut_downlook_unload_position;
    Position3D aa_updownlook_position;
    Position3D aa_picklens_position;
    Position3D aa_unpicklens_position;
    Position3D aa_uplook_position;
    Position3D aa_mushroom_position;

    Position3D sut_load_position;
    Position3D downlook_position;
    Position3D tool_downlook_position;
    Position3D mushroom_positon;
    Position3D tool_uplook_positon;
    PositionT  up_downlook_offset;

    SingleHeadLSutParameter parameters;
    //! Load LSUT parameters from config file
    void loadParams(QString file_name);
    //! Save LSUT parameters to config file
    void saveParams(QString file_name);

public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
private:
    XtCylinder* pogopin = Q_NULLPTR;
    VisionLocation* vision_downlook_location = Q_NULLPTR;
    VisionLocation* vision_updownlook_down_location = Q_NULLPTR;
    VisionLocation* vision_updownlook_up_location = Q_NULLPTR;
    VisionLocation* uplook_location = Q_NULLPTR;
    VisionLocation* load_location = Q_NULLPTR;
    VisionLocation* mushroom_location = Q_NULLPTR;
    VisionLocation* gripper_location = Q_NULLPTR;
};

#endif // SINGLEHEAD_LSUT_MODULE_H
