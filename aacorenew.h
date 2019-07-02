#ifndef AACORENEW_H
#define AACORENEW_H

#include <QObject>
#include "thread_worker_base.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <errorcode.h>
#include <sfr_entry.h>
#include <unordered_map>
#include <sfrworker.h>
#include <QObject>
#include <QJsonDocument>
#include <errorcode.h>
#include <QMap>
#include "aadata.h"
#include "aaheadmodule.h"
#include "lut_module.h"
#include "sut_module.h"
#include "singlehead_lsut_module.h"
#include "dothinkey.h"
#include "visionavadaptor.h"
#include "imageprovider.h"
#include "chart_calibration.h"
#include "dispense_module.h"
#include "aacoreparameters.h"
#include "lutclient.h"
#include "imagegrabbingworkerthread.h"
#include "unitlog.h"

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
        AA = 5
    };
    explicit AACoreNew(QString name = "AACoreNew", QObject * parent = nullptr);
    void Init(AAHeadModule* aa_head, SingleheadLSutModule *lsut, Dothinkey *dk,
              ChartCalibration * chartCalibration,DispenseModule* dispense,
              ImageGrabbingWorkerThread * imageThread, Unitlog * unitlog);
    void performAAOffline();
    Q_INVOKABLE void performHandling(int cmd, QString params);
    ErrorCodeStruct performInitSensor();
    ErrorCodeStruct performPRToBond();
    ErrorCodeStruct performAAPickLens();
    ErrorCodeStruct performAA(QJsonValue params);
    ErrorCodeStruct performOC(QJsonValue params);
    ErrorCodeStruct performMTF(QJsonValue params, bool write_log = false);
    ErrorCodeStruct performZOffset(double zOffset);
    ErrorCodeStruct performXYOffset(double xOffset, double yOffset);
    ErrorCodeStruct performDelay(int);
    ErrorCodeStruct performCameraUnload();
    ErrorCodeStruct performUV(int uv_time);
    ErrorCodeStruct performReject();
    ErrorCodeStruct performAccept();
    void performMTFLoopTest();
    double calculateDFOV(cv::Mat img);
    void setSfrWorkerController(SfrWorkerController*);
    bool runFlowchartTest();
    ErrorCodeStruct performTest(QString testItemName, QJsonValue properties);
    ErrorCodeStruct performDispense();
    void loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);
    AAData aaData_1;  // For Display Channel 1
    AAData aaData_2;  // For Display Channel 2
    AAData mtf_log;   // For Display MTF Log
    ImageProvider * ocImageProvider_1;
    ImageProvider * sfrImageProvider;

    AACoreParameters parameters;
private:
    bool is_run = false;
    QMutex lut_mutex;
    void run(bool has_material);
    void LogicNg(int & ng_time);
    void NgLens();
    void NgSensor();
    void NgProduct();
private:
    QString loopTestResult;
    int currentAAMode;
    QString runningUnit;
    AAHeadModule* aa_head;
    LutClient* lut;
    SutModule* sut;
    SingleheadLSutModule *lsut;
    Dothinkey* dk;
    ImageGrabbingWorkerThread* imageThread;
    ChartCalibration* chartCalibration;
    DispenseModule* dispense;
    Unitlog *unitlog;
    SfrWorkerController * sfrWorkerController = Q_NULLPTR;
    std::unordered_map<unsigned int, std::vector<Sfr_entry>> clustered_sfr_map;
    QVariantMap current_dfov;
    QJsonDocument flowchartDocument;
    QJsonValue currentTestParams;
    bool isZScanNeedToStop = false;
    int currentChartDisplayChannel = 0;
    int currentOCDisplayChannel = 0;
    bool has_product = false;
    bool has_ng_product = false;
    bool has_ng_lens = false;
    bool has_ng_sensor = false;
    bool has_sensor = false;
    bool has_lens = false;

    int current_aa_ng_time = 0;
    int current_oc_ng_time = 0;
    int current_mtf_ng_time = 0;

    void sfrFitCurve_Advance(double imageWidth, double imageHeight, double &xTilt, double &yTilt,
                             double &zPeak, double &ul_zPeak, double &ur_zPeak, double &ll_zPeak, double &lr_zPeak, double &dev);
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
};

#endif // AACORENEW_H
