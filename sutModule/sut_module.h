#ifndef SUT_MODULE_H
#define SUT_MODULE_H

#include "XtCylinder.h"
#include "vision/baslerpyloncamera.h"
#include "calibration/calibration.h"
#include "material_carrier.h"
#include "sutModule/sut_parameter.h"
#include "sutModule/sutclient.h"
#include "thread_worker_base.h"
#include "vision/vision_location.h"
#include "vision/visionmodule.h"
#include "vision/wordoplight.h"

enum SUT_STATE
{
    NO_MATERIAL = 0,
    HAS_NG_SENSOR = 1,
    HAS_PRODUCT = 2,
    HAS_NG_PRODUCT = 3
};
class SutModule : public ThreadWorkerBase
{
    Q_OBJECT
    Q_ENUMS(HandlePosition)
    Q_ENUMS(HandlePR)
    Q_ENUMS(HandlerAction)
public:
    enum HandlePosition
    {
        DOWNLOOK_PR_POS = 1,
        DOWNLOOK_ON_THE_FLY_POS = 2,
        LOAD_POS = 3,
        MUSHROOM_POS = 4,
        PARTICAL_CHECK_POS = 5
    };
    enum HandlePR{
        DOWNLOOK_PR = 10
    };
    enum HandlerAction
    {
    };
    SutModule();
    void Init(MaterialCarrier* carrier,
              VisionLocation* vision_downlook_location,VisionLocation* updownlook_down_location,VisionLocation* updownlook_up_locationn,
              XtVacuum *vacuum,XtCylinder* popgpin,XtGeneralOutput* camera_trig,int thread_id);
    void loadParams(QString file_name);
    void saveJsonConfig(QString file_name);

//    bool checkSutSensorOrProduct(bool check_state);
    bool checkSutHasMaterialSynic();
    bool checkSutHasMaterial();
    bool waitSutCheckResult(bool check_state);

    Q_INVOKABLE bool moveToLoadPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToDownlookPR(PrOffset &offset,bool close_lighting = true,bool check_autochthonous = false);
    bool moveToDownlookPR(bool close_lighting = true,bool check_autochthonous = false);
    Q_INVOKABLE bool moveToDownlookPos(bool check_autochthonous = false);
    bool moveToDownlookSaveImage(QString imageName,bool close_lighting = true,bool check_autochthonous = false);
    bool glueInspection(QString afterDispenseImage);
    Q_INVOKABLE bool moveToReadyPos();
    Q_INVOKABLE bool moveToUpDwonlookPR(PrOffset &offset,bool close_lighting = true,bool check_autochthonous = false);
    bool moveToDownLookFlyPr();
    Q_INVOKABLE bool toolDownlookPR(PrOffset &offset,bool close_lighting = true,bool motion = false);
    Q_INVOKABLE bool toolUplookPR(PrOffset &offset,bool close_lighting = true,bool motion = false);
    Q_INVOKABLE bool toolDownlookPR(bool close_lighting = true,bool motion = false);
    Q_INVOKABLE bool moveToToolDownlookPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToToolUplookPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToMushroomPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToParticalCheckPos(bool check_autochthonous = false);
    Q_INVOKABLE bool stepMove_XY_Sync(double x,double y);
    Q_INVOKABLE bool stepMove_Z_Sync(double step_z);
    Q_INVOKABLE bool moveToZPos(double z);
    Q_INVOKABLE bool moveToXPos(double x);
    Q_INVOKABLE bool moveZToSaftyInMushroom();
    bool moveZToSafety();
    Q_INVOKABLE void recordCurrentPos();
    Q_INVOKABLE bool movetoRecordPos(bool check_autochthonous = false);
    Q_INVOKABLE bool movetoRecordPosAddOffset(double x_offset,double y_offset,double z_offset,bool check_autochthonous = false);
    Q_INVOKABLE bool moveToSafetyPos(bool check_autochthonous = false);

    //PR
    bool performDownLookPR();

    bool OpenSutVacuum();
    bool CloseSutVacuum();

    QString getRecordDownlookImage() { return this->record_downlook_image; }
signals:
    void sendLoadSensorFinish(double offset_x,double offset_y,double offset_z);
public slots:
    void receiveLoadSensorRequst(int sut_state);
private:
    void run();
    // ThreadWorkerBase interface
public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd,QVariant param);
public:
    SutParameter parameters;
    SutState states;
    Position3D load_position;
    Position3D downlook_position;
    Position3D partical_check_position;
    Position3D tool_downlook_position;
    Position3D mushroom_positon;
    Position3D tool_uplook_positon;
    Position down_look_fly_end_position;
    PositionT  up_downlook_offset;
    MaterialCarrier* carrier;
    VisionLocation* vision_downlook_location;
    XtGeneralOutput* camera_trig;
    bool DownlookPrDone = false;
private:
    VisionLocation* vision_updownlook_down_location;
    VisionLocation* vision_updownlook_up_location;
    XtVacuum* vacuum;
    XtCylinder *popgpin;
//    SutClient* sut_cilent;
    int thread_id;
    mPoint3D record_position;
    QString record_downlook_image;
    PrOffset offset;
    bool is_run = false;

    int sut_state = 0;
    bool has_raw_sensor = false;
    bool loading_sensor = false;
    Qt::HANDLE gui_thread_id;

    // ThreadWorkerBase interface
public:
    void receivceModuleMessage(QVariantMap module_message);
    PropertyBase *getModuleState();

    // ThreadWorkerBase interface
public:
    QMap<QString, PropertyBase *> getModuleParameter();
    void setModuleParameter(QMap<QString, PropertyBase *>);
};

#endif // SUT_MODULE_H
