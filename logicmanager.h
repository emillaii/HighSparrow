#ifndef LOGICMANAGER_H
#define LOGICMANAGER_H

#include "basemodulemanager.h"
#include "logicmanagerparameter.h"

#include <QThread>
#include <utils/unitlog.h>
//模块联动


class LogicManager:public QObject,public ErrorBase
{
    Q_OBJECT
    Q_ENUMS(CommandType)
public:
    enum CommandType{
        IDLE,
        STOP,
        MOTION_INIT,
        MOTION_HOME,
        MOTION_INIT_ONLYSELF,
        MOTION_HOME_ONLYSELF,
        MOTION_STOP_HOME,     // Interrupt
        MOTION_SINGLE_MOTOR_HOME,
        MODE_AUTO_RUN,

        HANDLING_ORIGIN_LOCATION,
        HANDLING_OFFSET_LOCATION,
        HANDLING_CALIBRATION,
        HANDLING_OPEN_CYLINDER,
        HANDLING_CLOSE_CYLINDER,
        HANDLING_OPEN_OUTPUT,
        HANDLING_CLOSE_OUTPUT,
        HANDLING_OPEN_VACUUM,
        HANDLING_CLOSE_VACUUM,

        AA_MOVETO_MUSHROOM_CMD,
        AA_MOVETO_PICK_LENS_CMD,
        AA_MOVETO_OC_CMD,
        SUT_MOVETO_MUSHROOM_CMD,
        SUT_MOVETO_PR_CMD,
        LUT_MOVETO_LOAD_CMD,
        LUT_MOVETO_UNLOAD_CMD,
        LUT_MOVETO_LOAD_UPLOOK_CMD,
        LUT_MOVETO_AA1_UPLOOK_CMD,
        LUT_MOVETO_AA2_UPLOOK_CMD,
        LUT_PICK_LENS_TO_AA2_CMD,
        LUT_PICK_LENS_TO_AA1_CMD,
        LUT_PICK_LENS_TO_AA1_CMD1,

        PERFORM_CALIBRATION,
        PERFORM_UPDNLOOK_CALIBRATION,
        PERFORM_LENS_UPDNLOOK_CALIBRATION,
        PERFORM_SENSOR_PICKHEAD_CALIBRATION,
        PERFORM_LOCATION,

        PERFORM_OC,
        PERFORM_LOOP_TEST,
        PERFORM_UV,
        LOAD_LENS_FROM_TRAY_1_AND_PICK_TO_AA1,
        LOAD_LENS_FROM_TRAY_1_AND_PICK_TO_AA2,
        LOAD_LENS_FROM_TRAY_2_AND_PICK_TO_AA1,
        LOAD_LENS_FROM_TRAY_2_AND_PICK_TO_AA2,
        UNPICK_LENS_FROM_AA1_AND_PLACE_TO_TRAY1,
        UNPICK_LENS_FROM_AA1_AND_PLACE_TO_TRAY2,
        UNPICK_LENS_FROM_AA2_AND_PLACE_TO_TRAY1,
        UNPICK_LENS_FROM_AA2_AND_PLACE_TO_TRAY2,

        SAVE_PARAMETERS,
        LOAD_PARAMETERS
    };
public:
    explicit LogicManager(QObject *parent = nullptr);
    void init(BaseModuleManager* device_manager);
//    bool registerWorker(ThreadWorkerBase* worker);
    Q_INVOKABLE void performHandling(QString module_name,int cmd,QVariant param = 0);
    Q_INVOKABLE void performHandling(int cmd,QVariant param = 0);
private:
public:
    LogicManagerParameter parameters;
    LogicManagerState states;
    void allMotorsSeekOrigin();
//    Q_PROPERTY(int currentMode READ currentMode WRITE setCurrentMode)
//    Q_PROPERTY(QString stateMessage READ stateMessage WRITE setStateMessage NOTIFY stateMessageChanged)

    void init();
    void home();
    void init_only_self();
    void home_only_self();
    void stopHome();
    void stop();

    void autoRun(QString);

    void aaMoveToMushroomPos();
    void aaMoveToPickLensPos();
    void aaMoveToOCPos();

    void sutMoveToMushroomPos();
    void sutMoveToPRPos();

    void lutMoveToLoadPos();
    void lutMoveToUnloadPos();
    void lutMoveToLoadUplookPos();
    void lutMoveToAA1UplookPos();
    void lutMoveToAA2UplookPos();

    void lutPickLensToAA1();
    void lutPickLensToAA11();
    void lutPickLensToAA2();

    void performChartCalibration();
//    void performCalibration(QString calibration_name);
    void performUpDnLookCalibration();
    void performLensUpDnLookCalibration();
    void performSensorPickheadCalibration();

//    void performLocation(QString location_name,bool use_origin);

    void performOC();
    void performLoopTest(int mode);
    void performUV();

    //lenpickarmModule
    void lensPickArmMoveToTray1Pos();
    void lensPickArmMoveToTray2Pos();
    void lensPickArmMoveToTray1StartPos();
    void lensPickArmMoveToTray2StartPos();
    void lensPickArmMoveToTray1EndPos();
    void lensPickArmMoveToLutPos1();
    void lensPickArmMoveToLutPos2();
    void lensPickArmMoveToPickLensFromTray1();
    void lensPickArmMoveToPickLensFromTray2();
    void lensPickArmMoveToPickLensFromLut();
    void lensPickArmMoveToPlaceLensToTray1();
    void lensPickArmMoveToPlaceLensToTray2();
    void lensPickArmMoveToPlaceLensToLut();
    void lensPickArmLensPR();//取
    void lensPickArmVacancyTrayPR();//放
    void lensPickArmLUTPR();//取
    void lensPickArmLensMeasureHeight();//tray lens测高
    void lensPickArmLUTMeasureHeight();//lut 测高
    void lensPickArmMoveToUpdownlookDownPos();
    void lensPickArmMoveToUpdownlookUpPos();

    void trayLoaderModuleLTIEMovetoFirstPos();
    void trayLoaderModuleLTIEMovetoLastPos();
    void trayLoaderModuleLTOEMovetoFirstPos();
    void trayLoaderModuleLTOEMovetoLastPos();

    void trayLoaderModuleLTKX1MovetoGetPos();
    void trayLoaderModuleLTKX1MovetoSetPos();
    void trayLoaderModuleLTKX2MovetoGetPos();
    void trayLoaderModuleLTKX2MovetoSetPos();
    void trayLoaderModuleLTLMovetoGetPos();
    void trayLoaderModuleLTLMovetoSetPos();

    void trayLoaderModuleLTIEMovetoColumnIndex(int);
    void trayLoaderModuleLTOEMovetoColumnIndex(int);

    void trayLoaderModuleEjectTray();

    void trayLoaderModuleLTKX1CylinderOn();
    void trayLoaderModuleLTKX1CylinderOff();

    void trayLoaderModuleLTKX2CylinderOn();
    void trayLoaderModuleLTKX2CylinderOff();

    void trayLoaderModuleLTLXCylinderOn();
    void trayLoaderModuleLTLXCylinderOff();

    void trayLoaderModuleStartup();
    void trayLoaderModuleEmitTestTrayUsed();

    void trayLoaderModuleLTKX1MoveToRelayPos();

    bool trayLoaderModuleCheckLTLXGetPos(double);
    bool trayLoaderModuleCheckLTLXSetPos(double);

    void trayLoaderModuleLTLXPickUpTray();
    void trayLoaderModuleLTLXPutDownTray();

    Q_INVOKABLE void sensorTrayLoaderModuleSTIEMovetoColumnIndex(int);
    Q_INVOKABLE void sensorTrayLoaderModuleSTOEMovetoColumnIndex(int);
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoPushMotorSafePosotion();
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoDownTrayPosition();
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoGetTrayPosition();
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoFinishKickTrayPosition();
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoPushoutPosition();
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoPutTrayPosition();
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoStartKickTrayPosition();
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoTrayWorkPosition();
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoVacancyTrayPosition();

//    int currentMode() const
//    {
//        return m_currentMode;
//    }

//    QString stateMessage() const
//    {
//        return m_stateMessage;
//    }

     void sendLogicManangerName();
public slots:
    void performHandlingOperation(QString module_name,int cmd,QVariant param);
    void performTcpOperation(QVariantMap message);
    void receiveMessageFromWorkerManger(QVariantMap message);


    void setCurrentMode(int currentMode)
    {
        m_currentMode = currentMode;
    }

    void setStateMessage(QString stateMessage)
    {
        m_stateMessage = stateMessage;
    }
    void receiveCommand(int cmd);

signals:
    void sendPerformHandling(QString module_name,int cmd,QVariant param);
    void sendPerformTcp(QVariantMap message);
    void sendMessageToWorkerManger(QVariantMap message);

    void stateMessageChanged(QString stateMessage);
    bool sendMsgSignal(QString,QString);
private:
    BaseModuleManager * baseModuleManage;
    QThread work_thread;


    void moveToCmd(int);
    int m_currentMode;
    QString m_stateMessage;

    QString location_name;
    bool use_origin;

    QString calibration_name;
//    QMap<QString,ThreadWorkerBase*> workers;


    bool is_handling = false;
    bool result;
    QVariantMap  return_message;
protected:
    void run();
private:
    bool motorSeekOrigin(QString motor_name);
    void sendCmdMessage(QVariantMap message,int cmd);
    void sendRespMessage(QVariantMap message,bool result);
    void sendMessageToModule(QString module_name, QString message);
    bool waitReturnMessage();
    bool performLocation(QString location_name,bool use_origin);
    bool performCalibration(QString location_name);
    bool performCylinderOperation(QString cylinder_name,bool state);
    bool performOutputOperation(QString output_name,bool state);
    bool performVacuumOperation(QString vacuum_name,bool state);
};

#endif // LOGICMANAGER_H
