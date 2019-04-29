#ifndef LUT_MODULE_H
#define LUT_MODULE_H

#include "material_carrier.h"
#include "wordoplight.h"
#include "baslerpyloncamera.h"
#include "lut_parameter.h"
#include "visionmodule.h"
#include "calibration.h"
#include "vision_location.h"
#include "sparrowqserver.h"
#include <QObject>
#include <QQueue>
#include "thread_worker_base.h"

enum LUTState
{
    NO_LENS,
    HAS_LENS,
    BUSY
};

class LutModule : public ThreadWorkerBase
{
    Q_OBJECT
public:
    explicit LutModule(QString name = "LUTModule", QObject * parent = nullptr);
    void Init(MaterialCarrier* carrier,
              VisionLocation* uplook_location,VisionLocation* load_location,VisionLocation* mushroom_location,
              XtVacuum* load_vacuum, XtVacuum* unload_vacuum,XtGeneralOutput* gripper);
    void loadParams();
    void openServer(int port);
    LutParameter parameters;
    Position3D load_uplook_position;
    Position3D load_position;
    Position3D unload_position;
    Position3D aa1_updownlook_position;
    Position3D aa1_picklens_position;
    Position3D aa1_unpicklens_position;
    Position3D aa1_uplook_position;
    Position3D aa2_updownlook_position;
    Position3D aa2_picklens_position;
    Position3D aa2_unpicklens_position;
    Position3D aa2_uplook_position;
    Position3D aa1_mushroom_position;
    Position3D aa2_mushroom_position;
signals:
    void sendMessageToClient(QString destAddress, QString message);
public slots:
    //ThreadWorkerBase
    void startWork(bool reset_logic, int run_mode) override;
    void stopWork(bool wait_finish) override;
    void performHandlingOperation(int cmd) override;
    //End of ThreadWorkerBase
    void saveJsonConfig();
    void receiveRequestMessage(QString string, QString client_ip);
    LUTState getLUTState();
private:
    MaterialCarrier* carrier;
    VisionLocation* uplook_location;
    VisionLocation* load_location;
    VisionLocation* mushroom_location;
    XtGeneralOutput* gripper;
    XtVacuum* load_vacuum;
    XtVacuum* unload_vacuum;
    PRResultStruct pr_result;
    SparrowQServer * server;
    QQueue<QJsonObject> requestQueue;
    QQueue<QJsonObject> actionQueue;
    LUTState state = NO_LENS;
    bool is_run = false;
    void run(bool has_material);

public:
    Q_INVOKABLE bool moveToAA1UplookPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToAA1UplookPR(PrOffset &offset,bool close_lighting = true,bool check_autochthonous = false);
    Q_INVOKABLE bool moveToAA2UplookPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToAA2UplookPR(PrOffset &offset,bool close_lighting = true,bool check_autochthonous = false);
    Q_INVOKABLE bool moveToLoadPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToUnloadPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToLoadUplookPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToLoadUplookPR(bool check_autochthonous = false);
    Q_INVOKABLE double getLoadUplookPRX();
    Q_INVOKABLE double getLoadUplookPRY();
    Q_INVOKABLE bool moveToAA1PickLens(bool need_return = true,bool check_autochthonous = false);
    Q_INVOKABLE bool vcmReturn();
    Q_INVOKABLE bool moveToAA1PickLensPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToAA1UnPickLens(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToAA2PickLensPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToAA2PickLens(bool need_return = true,bool check_autochthonous = false);
    Q_INVOKABLE bool moveToAA2UnPickLens(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToAA1MushroomLens(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToAA2MushroomLens(bool check_autochthonous = false);
    bool stepMove_XY_Sync(double x,double y);

};

#endif // LUT_MODULE_H
