#ifndef BASEMODULEMANAGER_H
#define BASEMODULEMANAGER_H

#include <QObject>
#include "Vision/baslerpyloncamera.h"
#include "Vision/visionmodule.h"
#include "Vision/wordoplight.h"
#include "Utils/propertybase.h"
#include "Light/LontryLight.h"
#include "XtMotion/xtmotor.h"
#include "XT_MotionControler_Client_Lib.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include "XtMotion/XtVacuum.h"
#include "XtMotion/towerlightbuzzer.h"
#include "XtMotion/XtCylinder.h"
#include "XtMotion/XtVcMotor.h"
#include "AAHeadModule/aaheadmodule.h"
#include "material_carrier.h"
#include "Vision/calibration.h"
#include "ImageGrabber/dothinkey.h"
#include "ImageGrabber/imagegrabbingworkerthread.h"
#include "Dispense/dispenser.h"
#include "Dispense/dispense_module.h"
#include "Vision/vision_location.h"
#include "Vision/chart_calibration.h"
#include "workers_manager.h"
#include "Utils/unitlog.h"
#include "AACore/aacorenew.h"
#include "modulemanagerparameter.h"
#include "SingleHead/singleheadmachinematerialloadermodule.h"
#include "SingleHead/singlehead_lsut_module.h"
#include "materialtray.h"
#include "DOE/startcameradoe.h"

class BaseModuleManager : public PropertyBase,public ErrorBase
{
    Q_OBJECT
public:
    explicit BaseModuleManager(QObject *parent = nullptr);
    ~BaseModuleManager();
    Q_PROPERTY(int lightPanelLighting READ lightPanelLighting WRITE setLightPanelLighting NOTIFY lightPanelValueChanged)
    Q_PROPERTY(bool HomeState READ HomeState WRITE setHomeState NOTIFY paramsChanged)
    Q_PROPERTY(bool InitState READ InitState WRITE setInitState NOTIFY InitStateChanged)
    Q_PROPERTY(QString FlowchartFilename READ FlowchartFilename WRITE setFlowchartFilename NOTIFY paramsChanged)
    QMap<QString,XtMotor*> motors;
    QMap<QString,XtGeneralInput*> input_ios;
    QMap<QString,XtGeneralOutput*> output_ios;
    QMap<QString,Calibration*> calibrations;
    QMap<QString,VisionLocation*> vision_locations;
    QMap<QString,XtVacuum*> vacuums;
    QMap<QString,XtCylinder*> cylinder;
    ChartCalibration * chart_calibration = Q_NULLPTR;
    BaslerPylonCamera * pylonDownlookCamera = Q_NULLPTR;
    BaslerPylonCamera * pylonUplookCamera = Q_NULLPTR;
    BaslerPylonCamera * pylonPickarmCamera = Q_NULLPTR;
    VisionModule * visionModule = Q_NULLPTR;
    WordopLight * lightingModule = Q_NULLPTR;
    LontryLight * lightPanel = Q_NULLPTR;
    Dothinkey * dothinkey = Q_NULLPTR;
    ImageGrabbingWorkerThread * imageGrabberThread = Q_NULLPTR;


    MaterialCarrier sut_carrier;
    MaterialTray sensor_tray;
    MaterialTray reject_tray;
    MaterialTray lens_tray;
    AAHeadModule aa_head_module;
    SingleheadLSutModule sh_lsut_module;
    DispenseModule dispense_module;
    Dispenser dispenser;
    SfrWorkerController * sfrWorkerController;
    AACoreNew aaCoreNew;
    LSutState lsutState;
    SingleHeadMachineMaterialLoaderModule single_station_material_loader_module;
    SingleHeadMachineMaterialPickArm single_station_material_pickarm;

    Unitlog unitlog;

    StartCameraDOE* startCameraDoe;
    TowerLightBuzzer tower_light_buzzer;

    int lightPanelLighting() const
    {
        return m_lightPanelLighting;
    }

signals:
    void sendHandlingOperation(int cmd);
    void displaySfrImageInUI();
    void displayOCImageInUI();
    void lightingValueChanged(int downlookLighting);
    void lightPanelValueChanged(int lightPanelLighting);
    void paramsChanged();
    bool sendMsgSignal(QString,QString);
    void sendAlarm(int sender_id,int level, QString error_message);
    void InitStateChanged(bool InitState);

public slots:
    void alarmChecking();
    void receiveImageFromAACore(int type) {
        qInfo("Display SFR image in UI: %d", type);
        if (type == 0) emit displaySfrImageInUI();
        else if (type == 1) emit displayOCImageInUI();
    }
    bool receiveMsgSignal(QString title,QString content);
    void setLightPanelLighting(int lightPanelLighting)
    {
        if (m_lightPanelLighting == lightPanelLighting)
            return;

        m_lightPanelLighting = lightPanelLighting;

        emit lightPanelValueChanged(m_lightPanelLighting);
    }
    void setHomeState(bool HomeState)
    {
        m_HomeState = HomeState;
    }

    void setFlowchartFilename(QString FlowchartFilename)
    {
        if (m_FlowchartFilename == FlowchartFilename)
            return;

        m_FlowchartFilename = FlowchartFilename;
        emit paramsChanged();
    }

    void setInitState(bool InitState)
    {
        if (m_InitState == InitState)
            return;

        m_InitState = InitState;
        emit InitStateChanged(m_InitState);
    }

public:
    ModuleManangerConfig configs;
    ModuleManagerParameter paramers;
    ModuleManagerState states;
private:
    bool is_init;
    bool profile_loaded;
    static wchar_t ip[];

    VCM_Parameter_struct lut_vcm_parameters = {
        500/*MaxVel*/,20000/*MaxAcc*/,200000/*MaxJerk*/,0/*MaxRange*/,-19/*MinRange*/,9/*CanID*/,1/*dir*/,1024/*scale*/};
    VCM_Parameter_struct sut_vcm_parameters = {
        500/*MaxVel*/,10000/*MaxAcc*/,200000/*MaxJerk*/,33/*MaxRange*/,0/*MinRange*/,10/*CanID*/,1/*dir*/,5000/*scale*/};
private:
    bool InitStruct();

    bool m_HomeState = false;
    int m_lightPanelLighting;
    QTimer timer;
    QThread work_thread;
    bool m_InitState = false;

public:
    bool loadProfile();
    bool loadStructConfig(QString file_dir);
    bool loadMachineConfig(QString file_paths);
    Q_INVOKABLE bool loadParameters();
    Q_INVOKABLE bool saveParameters();
    bool loadVcmFile(QString file_name);
    bool saveVcmfile(QString file_name);
    bool loadMotorFile(QString file_name);
    bool saveMotorFile(QString file_name);
    bool loadVacuumFiles(QString file_name);
    bool saveVacuumFiles(QString file_name);
    bool loadCylinderFiles(QString file_name);
    bool saveCylinderFiles(QString file_name);
    bool loadVisionLoactionFiles(QString file_name);
    bool saveVisionLoactionFiles(QString file_name);
    bool loadCalibrationFiles(QString file_name);
    bool saveCalibrationFiles(QString file_name);
    bool loadMotorLimitFiles(QString file_name);
    bool saveMotorLimitFiles(QString file_name);

private:
    bool loadJsonArray(QString file_name, QJsonArray &array);
    bool saveJsonArray(QString file_name,QJsonArray &array);
    bool loadJsonObject(QString file_name, QJsonObject &object);
    bool saveJsonObject(QString file_name,QJsonObject &object);
    QString getCurrentParameterDir();
    QString m_FlowchartFilename;

public:
    bool registerWorkers(WorkersManager* manager);
    Q_INVOKABLE bool initialDevice();
    Q_INVOKABLE bool stepMove(QString name, double step, bool isPositive);
    Q_INVOKABLE void setMotorParamByName(QString name,double vel,double acc,double jert);
    bool performCalibration(QString calibration_name);
    bool performLocation(QString location_name);
    Q_INVOKABLE void setOutput(QString name, bool on);
    Q_INVOKABLE bool getOutput(QString name);
    Q_INVOKABLE bool getInput(QString name);
    Q_INVOKABLE void motorSeekOrigin(QString name);
    Q_INVOKABLE double getMotorFeedbackPos(QString name);
    Q_INVOKABLE void setLightingBrightness(QString location_name);
    Q_INVOKABLE bool initSensor();
    Q_INVOKABLE bool closeSensor();
    Q_INVOKABLE double showChartCalibrationRotation();
    void EnableMotors();
    void DisableAllMotors();
    Q_INVOKABLE bool allMotorsSeekOrigin();
    void stopSeeking();
    Q_INVOKABLE int getNumberOfMotors();
    Q_INVOKABLE QString getMotorsName(int);
    Q_INVOKABLE void updateParams();
    Q_INVOKABLE void loadFlowchart(QString, QString filename = "");

    XtMotor* GetMotorByName(QString name);
    XtVcMotor *GetVcMotorByName(QString name);
    XtGeneralOutput *GetOutputIoByName(QString name);
    XtGeneralInput *GetInputIoByName(QString name);
    XtVacuum *GetVacuumByName(QString name);
    XtCylinder *GetCylinderByName(QString name);
    VisionLocation *GetVisionLocationByName(QString name);
    Pixel2Mech *GetPixel2MechByName(QString name);
    Calibration *GetCalibrationByName(QString name);
    bool HomeState() const
    {
        return m_HomeState;
    }
    QString FlowchartFilename() const
    {
        return m_FlowchartFilename;
    }
    bool InitState() const
    {
        return m_InitState;
    }
};

#endif // dBASEMODULEMANAGER_H
