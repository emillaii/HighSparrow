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
public:
    explicit AACoreNew(QString name = "AACoreNew", QObject * parent = nullptr);
    void Init(AAHeadModule* aa_head,LutClient* lut,SutModule* sut,Dothinkey *dk,
              ChartCalibration * chartCalibration,DispenseModule* dispense,
              ImageGrabbingWorkerThread * imageThread, Unitlog * unitlog);
    void performAAOffline();
    ErrorCodeStruct performInitSensor();
    ErrorCodeStruct performPRToBond();
    ErrorCodeStruct performAAPickLens();
    ErrorCodeStruct performAA(double start, double stop, double step_size,
                   bool enableMotion, int zSleepInMs, bool isWaitTiltMotion,
                   int zScanMode = 0, double estimated_aa_fov = 0,
                   bool is_debug = false, sfr::EdgeFilter edgeFilter = sfr::EdgeFilter::NO_FILTER,
                   double estimated_fov_slope = -16, double zOffset=0);
    ErrorCodeStruct performOC(bool enableMotion, bool fastMode);
    ErrorCodeStruct performMTF(bool write_log = false);
    ErrorCodeStruct performZOffset(double zOffset);
    ErrorCodeStruct performDelay(int);
    ErrorCodeStruct performCameraUnload();
    ErrorCodeStruct performUV(int uv_time);
    void performMTFLoopTest();
    double calculateDFOV(cv::Mat img);
    void setSfrWorkerController(SfrWorkerController*);
    bool runFlowchartTest();
    ErrorCodeStruct performTest(QString testItemName, QJsonValue properties);
    ErrorCodeStruct performDispense();
    AACoreParameters parameters;
    void loadParams();
    void updateParams();
    AAData aaData_1;  // For Display Channel 1
    AAData aaData_2;  // For Display Channel 2
    AAData mtf_log;   // For Display MTF Log
    ImageProvider * ocImageProvider_1;
    ImageProvider * sfrImageProvider;
private:
    bool is_run = false;
    void run(bool has_material);
private:
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
    QJsonDocument flowchartDocument;
    bool isZScanNeedToStop = false;
    int currentChartDisplayChannel = 0;
    int currentOCDisplayChannel = 0;
    bool has_product = false;
    bool has_ng_lens = false;
    bool has_ng_sensor = false;
    bool has_sensor = false;
    bool has_lens = false;
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
    void startWork(bool reset_logic, int run_mode) override;
    void stopWork(bool wait_finish) override;
    void performHandlingOperation(int cmd) override;
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
