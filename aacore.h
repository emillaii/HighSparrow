#ifndef AACORE_H
#define AACORE_H

#include <QThread>
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
#include "unitlog.h"

typedef enum {
    AA_IDLE_MODE,
    AA_REPEATABILITY_TEST_MODE,
    AA_ACCURACY_TEST_MODE,
    AA_TILTING_TEST_MODE,
    AA_MTF_TEST_MODE,
    AA_OC_TEST_MODE,
    AA_OFFLINE_TEST_MODE,
    AA_AUTO_MODE
} AA_DIGNOSTICS_MODE;

class AACore : public QThread
{
    Q_OBJECT
public:
    explicit AACore(AAHeadModule* aa_head,LutModule* lut,SutModule* sut,Dothinkey *dk, ChartCalibration * chartCalibration,DispenseModule* dispense,  QObject *parent = nullptr);
    ~AACore();

protected:
    void run();

public:
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
    ErrorCodeStruct performMTF();
    ErrorCodeStruct performZOffset(double zOffset);
    ErrorCodeStruct performDelay(int);
    ErrorCodeStruct performCameraUnload();

    void performLoopTest(AA_DIGNOSTICS_MODE);

    double calculateDFOV(cv::Mat img);
    void setSfrWorkerController(SfrWorkerController*);
    bool runFlowchartTest();
    ErrorCodeStruct performTest(QString testItemName, QJsonValue properties);
    ErrorCodeStruct performDispense();
    AAData aaData_1;  // For Display Channel 1
    AAData aaData_2;  // For Display Channel 2
    ImageProvider * ocImageProvider_1;
    ImageProvider * sfrImageProvider;
private:
    QString loopTestResult;
    AA_DIGNOSTICS_MODE currentAAMode;
    Unitlog unitLog;
    QString runningUnit;
    AAHeadModule* aa_head;
    LutModule* lut;
    SutModule* sut;
    Dothinkey* dk;
    ChartCalibration* chartCalibration;
    DispenseModule* dispense;
    SfrWorkerController * sfrWorkerController = Q_NULLPTR;
    std::unordered_map<unsigned int, std::vector<Sfr_entry>> clustered_sfr_map;
    QJsonDocument flowchartDocument;
    bool isZScanNeedToStop = false;
    double cmosPixelToMM_X = 0,cmosPixelToMM_Y = 0;
    int currentChartDisplayChannel = 0;
    int currentOCDisplayChannel = 0;
    void sfrFitCurve_Advance(double imageWidth, double imageHeight, double &xTilt, double &yTilt,
                             double &zPeak, double &ul_zPeak, double &ur_zPeak, double &ll_zPeak, double &lr_zPeak, double &dev);
    std::vector<AA_Helper::patternAttr> search_mtf_pattern(cv::Mat inImage, QImage & image, bool isFastMode,
                                                               unsigned int & ccROIIndex,
                                                               unsigned int & ulROIIndex,
                                                               unsigned int & urROIIndex,
                                                               unsigned int & llROIIndex,
                                                               unsigned int & lrROIIndex);
signals:
    void sfrResultsReady(unsigned int, vector<Sfr_entry>, int);
    void sfrResultsDetectFinished();
    void callQmlRefeshImg();
    void callQmlRefeshSfrImg();
public slots:

    void storeSfrResults(unsigned int index, vector<Sfr_entry> sfrs, int timeElasped);
    void stopZScan();
    void setFlowchartDocument(QString json){
        flowchartDocument = QJsonDocument::fromJson(json.toUtf8());
    }
    void sfrImageReady(QImage);
};

#endif // AACORE_H
