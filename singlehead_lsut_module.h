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
    Q_ENUMS(HandleMoveToPosition)

public:
    enum HandleMoveToPosition
    {
        MOVE_TO_MUSHROOM_POSITION = 1,
        MOVE_TO_LOAD_POSITION = 2,
        MOVE_TO_PR_POSITION = 3,
        MOVE_TO_UPDN_DOWNLOOK_CALIBRATION_POSITION = 4,
        MOVE_TO_UPDN_UPLOOK_CALIBRATION_POSITION = 5,
        MOVE_TO_SAFETY_POSITION = 6,
    };

public:
    SingleheadLSutModule(QString name = "LSutModule", QObject *parent = nullptr);
    void Init(MaterialCarrier* _sut_carrier,XtCylinder* _pogopin);

    MaterialCarrier* sut_carrier;

    Position3D mushroom_position;
    Position3D load_position;
    Position3D downlook_position;
    Position3D updn_downlook_calibration_position;
    Position3D updn_uplook_calibration_position;
    Position3D safety_position;

    SingleHeadLSutParameter parameters;

    //! Load LSUT parameters from config file
    void loadParams(QString file_name);
    //! Save LSUT parameters to config file
    void saveParams(QString file_name);

    Q_INVOKABLE void performHandling(int cmd);

public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
    void run(bool isProduct);

private:
    bool moveToMushroomPosition(bool check_autochthonous);
    bool moveToLoadPosition(bool check_autochthonous);
    bool moveToPRPosition(bool check_autochthonous);
    bool moveToUpDnDownlookCalibrationPosition(bool check_autochthonous);
    bool moveToUpDnUplookCalibrationPosition(bool check_autochthonous);
    bool moveToSafetyPosition(bool check_autochthonous);
    bool moveToAAPickLens(bool check_autochthonous);
private:
    bool is_run = false;
    XtCylinder* pogopin = Q_NULLPTR;
    VisionLocation* vision_downlook_location = Q_NULLPTR;
    PrOffset offset;
};

#endif // SINGLEHEAD_LSUT_MODULE_H
