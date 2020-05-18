#ifndef AACORENEW_H
#define AACORENEW_H

#include <QObject>
#include "thread_worker_base.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <utils/errorcode.h>
#include <sfr_entry.h>
#include <unordered_map>
#include <QObject>
#include <QJsonDocument>
#include <utils/errorcode.h>
#include <QMap>
#include "AACore/sfrworker.h"
#include "AACore/aadata.h"
#include "aaHeadModule/aaheadmodule.h"
#include "lutModule/lut_module.h"
#include "sutModule/sut_module.h"
#include "imageGrabber/dothinkey.h"
#include "visionavadaptor.h"
#include "utils/imageprovider.h"
#include "calibration/chart_calibration.h"
#include "dispenseModule/dispense_module.h"
#include "AACore/aacoreparameters.h"
#include "lutModule/lutclient.h"
#include "imageGrabber/imagegrabbingworkerthread.h"
#include "utils/unitlog.h"
#include "i2cControl/i2ccontrol.h"
#include <QProcess>
class AACoreNew : public ThreadWorkerBase
{
    Q_OBJECT
    Q_ENUMS(HandleTest)
public:
    enum HandleTest
    {
        Dispense = 1,
        PR_To_Bond = 2,
        OC = 3,
        MTF = 4,
        AA = 5,
        INIT_CAMERA = 6,
        Y_LEVEL = 7,
        UV = 8,
        OTP = 9,
        UNLOAD_CAMERA = 10,
        Z_OFFSET = 11,
        LENS_VCM_POS =12,
        MOVE_LENS = 13,
        INIT_VCM =14,
        XY_OFFSET = 15,
        HOME_TILT = 16,
        AA_HEAD_MOVE_TO_MUSHROOM = 17,
        AA_HEAD_MOVE_TO_PICK_LENS = 18,
        DISPENSE_XY_OFFSET_TEST = 19,
        DISPENSE_Z_TEST = 20,
        PARTICAL_CHECK = 21
    };
    explicit AACoreNew(QString name = "AACoreNew", QObject * parent = nullptr);
    void Init(AAHeadModule* aa_head,SutModule* sut,Dothinkey *dk,
              ChartCalibration * chartCalibration,DispenseModule* dispense,
              ImageGrabbingWorkerThread * imageThread, Unitlog * unitlog, int serverMode);
    void performAAOffline();
    void performAAOfflineCCOnly();
    Q_INVOKABLE void performHandling(int cmd, QString params);
    Q_INVOKABLE void captureLiveImage();
    Q_INVOKABLE void clearCurrentDispenseCount();
    ErrorCodeStruct performVCMInit(QJsonValue params);
    ErrorCodeStruct performVCMDirectMode(QJsonValue params);
    ErrorCodeStruct performInitSensor(int finish_delay = 0,bool check_map = false);
    ErrorCodeStruct performPRToBond(int finish_delay);
    ErrorCodeStruct performLoadMaterial(int finish_delay);
    ErrorCodeStruct performAA(QJsonValue params);
    ErrorCodeStruct performOC(QJsonValue params);
    ErrorCodeStruct performMTFNew(QJsonValue params,bool write_log = false);
    ErrorCodeStruct performMTF(QJsonValue params);
    ErrorCodeStruct performMTFOffline(QJsonValue params);
    ErrorCodeStruct performZOffset(QJsonValue params);
    ErrorCodeStruct performXYOffset(QJsonValue params);
    ErrorCodeStruct performDelay(int);
    ErrorCodeStruct performCameraUnload(int finish_delay);
    ErrorCodeStruct performUV(QJsonValue params);
    ErrorCodeStruct performReject();
    ErrorCodeStruct performAccept();
    ErrorCodeStruct performTerminate();
    ErrorCodeStruct performGRR(bool change_lens,bool change_sensor,int repeat_time,int change_time);
    ErrorCodeStruct performYLevelTest(QJsonValue params);
    ErrorCodeStruct performOTP(QJsonValue params);
    ErrorCodeStruct performParallelTest(vector<QString> testList1, vector<QString> testList2, QJsonValue params1, QJsonValue params2);
    ErrorCodeStruct performParticalCheck(QJsonValue params);

    static bool performThreadTest(vector<QString> testList, QJsonValue params);
    static double performMTFInThread( cv::Mat input, int freq);
    bool blackScreenCheck(cv::Mat inImage);
    void performMTFLoopTest();
    double calculateDFOV(cv::Mat img);
    void setSfrWorkerController(SfrWorkerController*);
    bool runFlowchartTest();
    ErrorCodeStruct performTest(QString testItemName, QJsonValue properties);
    ErrorCodeStruct performDispense(QJsonValue params);
    void loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);
    AAData aaData_1;  // For Display Channel 1
    AAData aaData_2;  // For Display Channel 2
    AAData mtf_log;   // For Display MTF Log
    AAData intensity_profile; //For Display intensity profile
    ImageProvider * ocImageProvider_1;
    ImageProvider * sfrImageProvider;
    ImageProvider * dispenseImageProvider;
    AACoreParameters parameters;
    AACoreStates states;
    AAHeadModule* aa_head;
    DispenseModule* dispense;
    QJsonDocument flowchartDocument;
    QString flowchartJsonString;
private:
    i2cControl i2cControl;
    bool is_run = false;
    bool hasDispense = false;
    QMutex lut_mutex;
    void run(bool has_material);
    void LogicNg(int & ng_time);
    void NgLens();
    void NgSensorOrProduct();
    void NgSensor();
    bool HasLens();
    bool HasSensorOrProduct();
    void NgProduct();
    void SetLens();
    void SetNoLens();
    void SetSensor();
    void SetNoSensor();
    void SetProduct();
    double getSFRDev_mm(int count,double numbers,...);
    double getzPeakDev_um(int count,double numbers,...);
private:
    QString sensorID = "";
    int serverMode = 0;
    QString loopTestResult;
    int currentAAMode;
    QString runningUnit;
    LutClient* lut;
    SutModule* sut;
    Dothinkey* dk;
    ImageGrabbingWorkerThread* imageThread;
    ChartCalibration* chartCalibration;
    Unitlog *unitlog;
    SfrWorkerController * sfrWorkerController = Q_NULLPTR;
    std::unordered_map<unsigned int, std::vector<Sfr_entry>> clustered_sfr_map;
    QVariantMap current_dfov;
    double current_fov_slope;
    bool isZScanNeedToStop = false;
    int currentChartDisplayChannel = 0;
    int currentOCDisplayChannel = 0;
    bool has_product = false;
    bool has_ng_product = false;
    bool has_ng_lens = false;
    bool has_ng_sensor = false;
    bool has_sensor = false;
    bool send_lens_request = false;
    bool finish_lens_request = false;
    bool send_sensor_request = false;
    bool finish_sensor_request = false;
    bool has_lens = false;

    int current_aa_ng_time = 0;
    int current_oc_ng_time = 0;
    int current_mtf_ng_time = 0;
    int grr_repeat_time = 0;
    int grr_change_time = 0;
//    QString handlingParams = "";
    double mtf_oc_x = 0;
    double mtf_oc_y = 0;
    int current_dispense = 0;
    QString runningTestName = "";
    QVariantMap sfrFitCurve_Advance(int resize_factor, double start_pos);
    std::vector<AA_Helper::patternAttr> search_mtf_pattern(cv::Mat inImage, QImage & image, bool isFastMode,
                                                               unsigned int & ccROIIndex,
                                                               unsigned int & ulROIIndex,
                                                               unsigned int & urROIIndex,
                                                               unsigned int & llROIIndex,
                                                               unsigned int & lrROIIndex);
    double oc_fov = -1;
    double oc_z = 0;
    QProcess vcmCmdServer;

    double sumA=0,sumB=0,sumC=0,sumX=0,sumY=0,sumZ=0;
    int recordedTiltNum=0;
    Position6D temp_mushroom_position;
public slots:
    //ThreadWorkerBase
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd,QVariant param);
    //End of ThreadWorkerBase
signals:
    void sfrResultsReady(unsigned int, vector<Sfr_entry>, int);
    void sfrResultsDetectFinished();
    void callQmlRefeshImg(int);
    void pushDataToUnit(QString uuid, QString name, QVariantMap map);
    void pushNgDataToCSV(QString uuid, QString lotNumber, QString sensorId, QString testItemName, QString errorMessage);
    void clearHeaders();
    void postDataToELK(QString, QString);
    void postSfrDataToELK(QString, QVariantMap);
    void sendLensRequestToLut();
    void needUpdateParameterInTcpModule();
public slots:
    void triggerGripperOn(bool isOn);
    void storeSfrResults(unsigned int index, vector<Sfr_entry> sfrs, int timeElasped);
    void stopZScan();
    void setFlowchartDocument(QString json){
        this->flowchartJsonString = json;
        flowchartDocument = QJsonDocument::fromJson(json.toUtf8());
    }
    void sfrImageReady(QImage);
    void aaCoreParametersChanged();
    void updateAACoreSensorParameters(double scaleX, double scaleY, double angle);
    // ThreadWorkerBase interface
public:
    PropertyBase *getModuleState();

    // ThreadWorkerBase interface
public:
    void receivceModuleMessage(QVariantMap module_message);
    QMap<QString, PropertyBase *> getModuleParameter();
    void setModuleParameter(QMap<QString, PropertyBase *>);
};

#endif // AACORENEW_H
