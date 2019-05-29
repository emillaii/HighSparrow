#ifndef BASEMODULEMANAGER_H
#define BASEMODULEMANAGER_H

#include <QObject>
#include "baslerpyloncamera.h"
#include "visionmodule.h"
#include "wordoplight.h"
#include "propertybase.h"
#include "LontryLight.h"
#include "xtmotor.h"
#include "XT_MotionControler_Client_Lib.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include "XtVacuum.h"
#include "XtCylinder.h"
#include "XtVcMotor.h"
#include "aaheadmodule.h"
#include "material_carrier.h"
#include "lut_module.h"
#include "sut_module.h"
#include "calibration.h"
#include "dothinkey.h"
#include "imagegrabbingworkerthread.h"
#include "dispenser.h"
#include "dispense_module.h"
#include "vision_location.h"
#include "chart_calibration.h"
#include "sparrowqserver.h"
#include "sparrowqclient.h"
#include "lensloadermodule.h"
#include "lutclient.h"
#include "workers_manager.h"
#include "trayloadermodule.h"
#include "unitlog.h"
#include "sensorloadermodule.h"
#include "aacorenew.h"
#include "sensorclip.h"
#include "sensortrayloadermodule.h"
#include "modulemanagerparameter.h"
#include "singleheadmachinematerialloadermodule.h"
class BaseModuleManager : public PropertyBase,public ErrorBase
{
    Q_OBJECT
    int m_lightPanelLighting;
public:
    explicit BaseModuleManager(QObject *parent = nullptr);
    ~BaseModuleManager();
    Q_PROPERTY(int lightPanelLighting READ lightPanelLighting WRITE setLightPanelLighting NOTIFY lightPanelValueChanged)
    Q_PROPERTY(int ServerMode READ ServerMode WRITE setServerMode NOTIFY paramsChanged)
    Q_PROPERTY(int ServerPort READ ServerPort WRITE setServerPort NOTIFY paramsChanged)
    Q_PROPERTY(bool HomeState READ HomeState WRITE setHomeState NOTIFY paramsChanged)
    Q_PROPERTY(QString ServerURL READ ServerURL WRITE setServerURL NOTIFY paramsChanged)
    Q_PROPERTY(QString DataServerURL READ DataServerURL WRITE setDataServerURL NOTIFY paramsChanged)

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

    MaterialCarrier lut_carrier;
    MaterialCarrier sut_carrier;
    MaterialPicker lens_picker;
    LensPickArm lens_pick_arm;
    MaterialPicker sensor_picker1;
    MaterialPicker sensor_picker2;
    SensorPickArm sensor_pickarm;
    MaterialTray material_tray;
    AAHeadModule aa_head_module;
    LutModule lut_module;
    SutModule sut_module;
    DispenseModule dispense_module;
    Dispenser dispenser;
    LensLoaderModule lens_loader_module;
    SensorLoaderModule sensor_loader_module;
    SfrWorkerController * sfrWorkerController;
    AACoreNew aaCoreNew;
    TrayLoaderModule tray_loader_module;

    SingleHeadMachineMaterialLoaderModule single_station_material_loader_module;
    SingleHeadMachineMaterialPickArm single_station_material_pickarm;


    TrayClip trayClipIn;
    TrayClip trayClipOut;

    SparrowQServer * sparrowQServer;
    SparrowClient * sparrowQClient;
    LutClient * lutClient;
    SutClient * sut_clitent;
    SensorClip entrance_clip;
    SensorClip exit_clip;
    SensorClipStandParameter sensor_clip_stand;
    SensorTrayLoaderModule sensor_tray_loder_module;

    Unitlog unitlog;

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
public slots:
    void alarmChecking();
    void performHandlingOperation(int cmd);
	void receiveImageFromAACore(int type) {
        qInfo("Display SFR image in UI: %d", type);
        if (type == 0) emit displaySfrImageInUI();
        else if (type == 1) emit displayOCImageInUI();
    }
    bool sendMessageTest(QString title,QString content);
    void setLightPanelLighting(int lightPanelLighting)
    {
        if (m_lightPanelLighting == lightPanelLighting)
            return;

        m_lightPanelLighting = lightPanelLighting;

        emit lightPanelValueChanged(m_lightPanelLighting);
    }
    void setServerMode(int ServerMode)
    {
        if (m_ServerMode == ServerMode)
            return;

        m_ServerMode = ServerMode;
        emit paramsChanged();
    }

    void setServerPort(int ServerPort)
    {
        if (m_ServerPort == ServerPort)
        return;

        m_ServerPort = ServerPort;
        emit paramsChanged();
    }

    void setServerURL(QString ServerURL)
    {
        if (m_ServerURL == ServerURL)
        return;

        m_ServerURL = ServerURL;
        emit paramsChanged();
    }

    void setHomeState(bool HomeState)
    {
        m_HomeState = HomeState;
    }

    void setDataServerURL(QString DataServerURL)
    {
        if (m_DataServerURL == DataServerURL)
            return;
        m_DataServerURL = DataServerURL;
    }
public:
    ModuleManangerConfig configs;
    ModuleManagerParameter paramers;
    ModuleManagerState states;
private:
    bool is_init;
    bool profile_loaded;
    static wchar_t ip[];
    static wchar_t profile_path1[];
    static wchar_t profile_path2[];

    VCM_Parameter_struct lut_vcm_parameters = {
        500/*MaxVel*/,20000/*MaxAcc*/,200000/*MaxJerk*/,0/*MaxRange*/,-19/*MinRange*/,9/*CanID*/,1/*dir*/,1024/*scale*/};
    VCM_Parameter_struct sut_vcm_parameters = {
        500/*MaxVel*/,10000/*MaxAcc*/,200000/*MaxJerk*/,33/*MaxRange*/,0/*MinRange*/,10/*CanID*/,1/*dir*/,5000/*scale*/};
private:
    bool InitStruct();
    int m_ServerPort = 9999;
    QString m_ServerURL = "ws://localhost:61916";
    int m_ServerMode = 0;

    bool m_HomeState = false;
    QTimer timer;
    QString m_DataServerURL;
    QThread work_thread;

public:
    bool loadProfile();
    bool loadStructConfig(QString file_dir);
    bool loadMachineConfig(QString file_paths);
    Q_INVOKABLE bool generateConfigFiles();
    Q_INVOKABLE bool loadParameters();
    Q_INVOKABLE bool loadconfig();
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

    bool loadJsonArray(QString file_name, QJsonArray &array);
    bool saveJsonArray(QString file_name,QJsonArray &array);
    bool loadJsonObject(QString file_name, QJsonObject &object);
    bool saveJsonObject(QString file_name,QJsonObject &object);
    QString getCurrentParameterDir();

    bool registerWorkers(WorkersManager* manager);

    Q_INVOKABLE void performHandling(int cmd);

    Q_INVOKABLE bool initialDevice();
    Q_INVOKABLE bool stepMove(QString name, double step, bool isPositive);
    Q_INVOKABLE bool stepMove(int index, double step, bool isPositive);
    Q_INVOKABLE void setMotorParamByName(QString name,double vel,double acc,double jert);
    bool performCalibration(QString calibration_name);
    bool performUpDnLookCalibration();
    bool performLensUpDnLookCalibration();
    bool performLocation(QString location_name);
    Q_INVOKABLE QString getCalibrationParam(QString calibration_name);
    Q_INVOKABLE void setOutput(QString name, bool on);
    Q_INVOKABLE bool getOutput(QString name);
    Q_INVOKABLE bool getInput(QString name);
    Q_INVOKABLE void motorSeekOrigin(QString name);
    Q_INVOKABLE double getPROffsetX(QString location_name);
    Q_INVOKABLE double getPROffsetY(QString location_name);

    Q_INVOKABLE double getMotorFeedbackPos(QString name);
    Q_INVOKABLE double getMotorFeedbackPos(int index);
    Q_INVOKABLE void setLightingBrightness(QString location_name);
    Q_INVOKABLE void sendLoadLens(bool has_ng);
    Q_INVOKABLE void sendLoadSensor(bool has_product,bool has_ng);
    Q_INVOKABLE void sendChangeSensorTray();

    Q_INVOKABLE bool initSensor();
    Q_INVOKABLE bool closeSensor();
    Q_INVOKABLE void loadSensorLoaderParameter();
    Q_INVOKABLE double showChartCalibrationRotation();
    void EnableMotors();
    void DisableAllMotors();
    Q_INVOKABLE bool allMotorsSeekOrigin();
    bool allMotorsSeekOriginal1();
    bool allMotorsSeekOriginal2();
    bool allMotorsSeekOriginal3();
    void stopSeeking();

    Q_INVOKABLE int getNumberOfMotors();
    Q_INVOKABLE QString getMotorsName(int);

    Q_INVOKABLE void updateParams();
    Q_INVOKABLE void loadFlowchart(QString);
    Q_INVOKABLE void loadSensorTrayLoaderMuduleParameter();

    Q_INVOKABLE int getServerMode() { return m_ServerMode; }

    XtMotor* GetMotorByName(QString name);
    XtVcMotor *GetVcMotorByName(QString name);
    XtGeneralOutput *GetOutputIoByName(QString name);
    XtGeneralInput *GetInputIoByName(QString name);
    XtVacuum *GetVacuumByName(QString name);
    XtCylinder *GetCylinderByName(QString name);
    VisionLocation *GetVisionLocationByName(QString name);
    Pixel2Mech *GetPixel2MechByName(QString name);
    Calibration *GetCalibrationByName(QString name);
    int ServerMode() const
    {
        return m_ServerMode;
    }
    int ServerPort() const
    {
        return m_ServerPort;
    }
    QString ServerURL() const
    {
        return m_ServerURL;
    }
    bool HomeState() const
    {
        return m_HomeState;
    }
    QString DataServerURL() const
    {
        return m_DataServerURL;
    }
};

#endif // dBASEMODULEMANAGER_H
