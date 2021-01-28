#ifndef LUT_MODULE_H
#define LUT_MODULE_H

#include "material_carrier.h"
#include "vision/wordoplight.h"
#include "vision/baslerpyloncamera.h"
#include "lutModule/lut_parameter.h"
#include "vision/visionmodule.h"
#include "calibration/calibration.h"
#include "vision/vision_location.h"
#include "network/sparrowqserver.h"
#include <QObject>
#include <QQueue>
#include "thread_worker_base.h"
#include "sutModule/sut_module.h"
#include "uphhelper.h"

enum LUTState
{
    NO_LENS,
    HAS_LENS,
    BUSY
};

class LutModule : public ThreadWorkerBase
{
    Q_OBJECT
    Q_ENUMS(HandlePosition)
    Q_ENUMS(HandlePR)
    Q_ENUMS(HandlerAction)
public:
    enum HandlePosition
    {
        LOAD_POS = 1,
        UNLOAD_POS = 2,
        LOAD_UPLOOK_POS = 3,
        DOWNLOOK_LOAD_POS = 4,
        DOWNLOOK_UNLOAD_POS = 5,
        AA1_UPLOOK_POS = 6,
        AA1_PICK_LENS_POS = 7,
        AA1_UNPICK_LENS_POS = 8,
        AA2_UPLOOK_POS =9,
        AA2_PICK_LENS_POS = 10,
        AA2_UNPICK_LENS_POS = 11
    };
    enum HandlePR
    {
        RESET_PR = 100,
        AA1_UPLOOK_PR = 200,
        AA2_UPLOOK_PR = 300
    };
    enum HandlerAction
    {
        Measure_AA1_PICK_POS = 1000,
        Measure_AA2_PICK_POS = 2000,
        HOME_TILT = 3000,
        AA1_PICK_LENS = 4000,
        AA2_PICK_LENS = 5000,
        AA1_UNPICK_LENS = 6000,
        AA2_UNPICK_LENS = 7000,
        AA1_PICK_LENS_FROM_POCKET2 = 8000,
        AA2_PICK_LENS_FROM_POCKET2 = 9000,
        AA1_UNPICK_LENS_FROM_POCKET1 = 10000,
        AA2_UNPICK_LENS_FROM_POCKET1 = 11000
    };
    explicit LutModule(QString name = "LUTModule", QObject * parent = nullptr);
    void Init(MaterialCarrier* carrier,
              VisionLocation* uplook_location,VisionLocation* load_location,VisionLocation* mushroom_location,
              XtVacuum* load_vacuum, XtVacuum* unload_vacuum,XtGeneralOutput* gripper, SutModule* sut,int check_thread);
    void loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);
    void openServer(int port);
    LutParameter parameters;
    LutState states;
    Position3D load_uplook_position;
    Position lpa_camera_to_picker_offset;
    Position3D load_position;
    Position3D lut_downlook_load_position;
    Position3D lut_downlook_unload_position;
    Position3D aa1_updownlook_position;
    Position3D aa1_picklens_position;
    Position3D aa1_unpicklens_position;
    Position3D aa1_uplook_position;
    Position3D aa1_ready_position;

    Position3D aa2_updownlook_position;
    Position3D aa2_picklens_position;
    Position3D aa2_unpicklens_position;
    Position3D aa2_uplook_position;
    Position3D aa2_ready_position;

    Position3D aa1_mushroom_position;
    Position3D aa2_mushroom_position;

    UPHHelper lens_uph;
    UPHHelper left_lens_uph;
    UPHHelper right_lens_uph;
    MaterialCarrier* carrier;
    XtVacuum* load_vacuum;
    XtVacuum* unload_vacuum;
signals:
    void sendMessageToClient(QString destAddress, QString module_message);
    void sendLoadLensRequst(bool need_lens,int ng_lens,int ng_lens_tray);
    void postCSVDataToUnit(QString uuid,QVariantMap data);
public slots:
    //ThreadWorkerBase
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd,QVariant param);
    //End of ThreadWorkerBase
    void receiveRequestMessage(QString module_message, QString client_ip);
    void receiveLoadLensRequstFinish(int lens,int lens_tray);
    void receiveToolUpPRRequest(PrOffset &offset);
private:
    SutModule* sut;
    VisionLocation* uplook_location;
    VisionLocation* load_location;
    VisionLocation* mushroom_location;
    XtGeneralOutput* gripper;
    QMutex loader_mutext;
    SparrowQServer * server;
    QMutex tcp_mutex;
    QQueue<QJsonObject> requestQueue;
    QQueue<QJsonObject> actionQueue;
    bool is_run = false;
    bool isLocalHost = false;
    QTime time_label;
    bool has_material = true;
    PrOffset pr_offset;
    QString uuid = "";
    int check_thread = 0;

    void run(bool has_material);
//    void runTest();
    bool isActionEmpty();
    QString servingIP = "";
    void sendEvent(const QString event);
    void sendCmd(QString serving_ip,const QString cmd);
    void sendPrEvent(const PrOffset pr_offset);

public:
    QString callerName;
    void setCallerName(QString name) { this->callerName = name; }
    Q_INVOKABLE int getConnectedClient();
    Q_INVOKABLE void calculcateRelativePosition();
    Q_INVOKABLE bool moveToAA1UplookPos(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA1UplookPR(PrOffset &offset,bool close_lighting = true,bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA1UplookPR(bool close_lighting = true,bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA2UplookPos(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA2UplookPR(PrOffset &offset,bool close_lighting = true,bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA2UplookPR(bool close_lighting = true,bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToLoadPos(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToLoadPosAndCheckMaterial(bool has_lens,bool has_ng_lens,bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToLutDownlookloadPos(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToLutDownlookUnloadPos(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToLoadUplookPos(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToLoadUplookPR(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE double getLoadUplookPRX();
    Q_INVOKABLE double getLoadUplookPRY();
    Q_INVOKABLE bool moveToAA1PickLens(bool need_return = true,bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA1PickLensFromPocket2(bool need_return = true,bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool vcmReturn();
    Q_INVOKABLE bool moveToAA1PickLensPos(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAAMeasurePickHeight(bool ishost,bool check_autochthonous = false, bool check_softlanding = false);

    Q_INVOKABLE bool moveToAA1UnPickLens(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA1UnPickLensFromPocket1(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA2PickLensPos(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA2PickLens(bool need_return = true,bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA2PickLensFromPocket2(bool need_return = true,bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA2UnPickLens(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA2UnPickLensFromPocket1(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA1MushroomLens(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA2MushroomLens(bool check_autochthonous = false, bool check_softlanding = false);

    Q_INVOKABLE bool moveToAA1UnPickLensPos(bool check_autochthonous = false, bool check_softlanding = false);
    Q_INVOKABLE bool moveToAA2UnPickLensPos(bool check_autochthonous = false, bool check_softlanding = false);

    bool moveToAA1ReturnPos(bool check_autochthonous = false, bool check_softlanding = false);
    bool moveToAA2ReturnPos(bool check_autochthonous = false, bool check_softlanding = false);
    bool moveToAA1ReadyPos(bool check_autochthonous = false, bool check_softlanding = false);
    bool moveToAA2ReadyPos(bool check_autochthonous = false, bool check_softlanding = false);

    bool checkLutLensSync(bool check_state);
    bool checkLutNgLensSync(bool check_state);
    bool waitLutLensCheckResult(bool check_state);
    bool waitLutNgLensCheckResult(bool check_state);
    bool stepMove_XY_Sync(double x,double y);
    //PR
    bool performUpLookPR();
    bool checkNeedLens();
    //Remote AA Head
    void aa2HeadMoveToPickPos();
    //Gripper操作
    void openAA1Griper();
    void closeAA1Griper();
    void openAA2Griper();
    void closeAA2Griper();
    bool waitGripermFinish();
    //真空操作
    bool openLoadVacuum();
    bool closeLoadVacuum();
    bool openUnloadVacuum();
    bool closeUnloadVacuum();

    double updateAccumulatedHour(bool calculate = true);
    double getHourSpace(QTime time_label);
    Q_INVOKABLE void clearNumber();
private:
    QString getUuid(bool is_right,int current_count, int current_time);
public:
    void recordAALensPr(QString uuid);


    // ThreadWorkerBase interface
public:
    void receivceModuleMessage(QVariantMap module_message);
    PropertyBase *getModuleState();

    // ThreadWorkerBase interface
public:
    QMap<QString, PropertyBase *> getModuleParameter();
    void setModuleParameter(QMap<QString, PropertyBase *>);
};

#endif // LUT_MODULE_H
