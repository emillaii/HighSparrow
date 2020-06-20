#ifndef SUT_MODULE_H
#define SUT_MODULE_H

#include "XtMotion/XtCylinder.h"
#include "Vision/baslerpyloncamera.h"
#include "Vision/calibration.h"
#include "material_carrier.h"
#include "sut_parameter.h"
#include "sutclient.h"
#include "thread_worker_base.h"
#include "Vision/vision_location.h"
#include "Vision/visionmodule.h"
#include "Vision/wordoplight.h"

enum SUT_STATE
{
    NO_MATERIAL = 0,
    HAS_NG_SENSOR = 1,
    HAS_PRODUCT = 2
};
class SutModule : public ThreadWorkerBase
{
    Q_OBJECT
public:
    SutModule();
    void Init(MaterialCarrier* carrier,SutClient* sut_cilent,
              VisionLocation* vision_downlook_location,VisionLocation* updownlook_down_location,VisionLocation* updownlook_up_locationn,
              XtVacuum *vacuum,XtCylinder* popgpin);
    void loadParams(QString file_name);
    void saveJsonConfig(QString file_name);
    Q_INVOKABLE bool moveToLoadPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToDownlookPR(PrOffset &offset,bool close_lighting = true,bool check_autochthonous = false);
    Q_INVOKABLE bool moveToDownlookPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToUpDwonlookPR(PrOffset &offset,bool close_lighting = true,bool check_autochthonous = false);
    Q_INVOKABLE bool toolDownlookPR(PrOffset &offset,bool close_lighting = true,bool motion = false);
    Q_INVOKABLE bool toolUplookPR(PrOffset &offset,bool close_lighting = true,bool motion = false);
    Q_INVOKABLE bool toolDownlookPR(bool close_lighting = true,bool motion = false);
    Q_INVOKABLE bool moveToToolDownlookPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToToolUplookPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToMushroomPos(bool check_autochthonous = false);
    Q_INVOKABLE bool stepMove_XY_Sync(double x,double y);
    Q_INVOKABLE bool stepMove_Z_Sync(double step_z);
    Q_INVOKABLE bool moveToZPos(double z);
    Q_INVOKABLE void recordCurrentPos();
    Q_INVOKABLE bool movetoRecordPos(bool check_autochthonous = false);
signals:
    void sendLoadSensorFinish(double offset_x,double offset_y,double offset_z);
public slots:
    void receiveLoadSensorRequst(int sut_state);
private:
    void run(bool has_material);
    // ThreadWorkerBase interface
public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
public:
    SutParameter parameters;
    SutState states;
    Position3D load_position;
    Position3D downlook_position;
    Position3D tool_downlook_position;
    Position3D mushroom_positon;
    Position3D tool_uplook_positon;
    PositionT  up_downlook_offset;
    MaterialCarrier* carrier;
private:
    VisionLocation* vision_downlook_location;
    VisionLocation* vision_updownlook_down_location;
    VisionLocation* vision_updownlook_up_location;
    XtVacuum* vacuum;
    XtCylinder *popgpin;
    SutClient* sut_cilent;
    mPoint3D record_position;
    PrOffset offset;
    bool is_run;
    bool has_material;

    int sut_state = 0;
    bool has_raw_sensor = false;
    bool loading_sensor = false;
    Qt::HANDLE gui_thread_id;
};

#endif // SUT_MODULE_H