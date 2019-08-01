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
    };
    explicit AACoreNew(QString name = "AACoreNew", QObject * parent = nullptr);
    void Init(AAHeadModule* aa_head,LutClient* lut,SutModule* sut,Dothinkey *dk,
              ChartCalibration * chartCalibration,DispenseModule* dispense,
              ImageGrabbingWorkerThread * imageThread, Unitlog * unitlog, int serverMode);
    void performAAOffline();
    Q_INVOKABLE void performHandling(int cmd, QString params);
    Q_INVOKABLE void captureLiveImage();
    ErrorCodeStruct performInitSensor(int finish_delay = 0,bool check_map = false);
    ErrorCodeStruct performPRToBond(int finish_delay);
    ErrorCodeStruct performLoadMaterial(int finish_delay);
    ErrorCodeStruct performAA(QJsonValue params);
    ErrorCodeStruct performOC(QJsonValue params);
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

    static bool performThreadTest(vector<QString> testList, QJsonValue params);
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
    ImageProvider * aaCoreTuningProvider;
    AACoreParameters parameters;
    AACoreStates states;
private:
    bool is_run = false;
    QMutex lut_mutex;
    void run(bool has_material);
    void LogicNg(int & ng_time);
    void NgLens();
    void NgSensor();
    bool HasLens();
    bool HasSensorOrProduct();
    void NgProduct();
    void SetLens();
    void SetSensor();
    void SetProduct();
    double getSFRDev_mm(int count,double numbers,...);
    double getzPeakDev_um(int count,double numbers,...);
private:
    QString sensorID = "";
    int serverMode = 0;
    QString loopTestResult;
    int currentAAMode;
    QString runningUnit;
    AAHeadModule* aa_head;
    LutClient* lut;
    SutModule* sut;
    Dothinkey* dk;
    ImageGrabbingWorkerThread* imageThread;
    ChartCalibration* chartCalibration;
    DispenseModule* dispense;
    Unitlog *unitlog;
    SfrWorkerController * sfrWorkerController = Q_NULLPTR;
    std::unordered_map<unsigned int, std::vector<Sfr_entry>> clustered_sfr_map;
    QVariantMap current_dfov;
    double current_fov_slope;
    QJsonDocument flowchartDocument;
    bool isZScanNeedToStop = false;
    int currentChartDisplayChannel = 0;
    int currentOCDisplayChannel = 0;
    bool has_product = false;
    bool has_ng_product = false;
    bool has_ng_lens = false;
    bool has_ng_sensor = false;
    bool has_sensor = false;
    bool send_lens_request = false;
    bool send_sensor_request = false;
    bool has_lens = false;

    int current_aa_ng_time = 0;
    int current_oc_ng_time = 0;
    int current_mtf_ng_time = 0;
    int grr_repeat_time = 0;
    int grr_change_time = 0;
    QString handlingParams = "";
    double mtf_oc_x = 0;
    double mtf_oc_y = 0;
    int current_dispense = 0;

    QVariantMap sfrFitCurve_Advance(int resize_factor, double start_pos);
    std::vector<AA_Helper::patternAttr> search_mtf_pattern(cv::Mat inImage, QImage & image, bool isFastMode,
                                                               unsigned int & ccROIIndex,
                                                               unsigned int & ulROIIndex,
                                                               unsigned int & urROIIndex,
                                                               unsigned int & llROIIndex,
                                                               unsigned int & lrROIIndex);
public slots:
    //ThreadWorkerBase
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
    //End of ThreadWorkerBase
signals:
    void sfrResultsReady(unsigned int, vector<Sfr_entry>, int);
    void sfrResultsDetectFinished();
    void callQmlRefeshImg(int);
    void pushDataToUnit(QString uuid, QString name, QVariantMap map);
    void clearHeaders();
    void postDataToELK(QString);
    void postSfrDataToELK(QString, QVariantMap);
    void sendLensRequestToLut();
public slots:
    void triggerGripperOn(bool isOn);
    void storeSfrResults(unsigned int index, vector<Sfr_entry> sfrs, int timeElasped);
    void stopZScan();
    void setFlowchartDocument(QString json){
        flowchartDocument = QJsonDocument::fromJson(json.toUtf8());
    }
    void sfrImageReady(QImage);
    void aaCoreParametersChanged();

    // ThreadWorkerBase interface
public:
    PropertyBase *getModuleState();
};

#endif // AACORENEW_H
