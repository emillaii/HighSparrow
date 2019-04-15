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
class AACore : public QThread
{
    Q_OBJECT
public:
    explicit AACore(AAHeadModule* aa_head,LutModule* lut,SutModule* sut,Dothinkey *dk, ChartCalibration * chartCalibration,  QObject *parent = nullptr);
    ~AACore();

protected:
    void run();

public:
    void performAAOffline();
    ErrorCodeStruct performPRToBond();
    ErrorCodeStruct performAAPickLens();
    ErrorCodeStruct performAA(double start, double stop, double step_size,
                   bool enableMotion, int zSleepInMs, bool isWaitTiltMotion,
                   int zScanMode = 0, double estimated_aa_fov = 0,
                   bool is_debug = false, sfr::EdgeFilter edgeFilter = sfr::EdgeFilter::NO_FILTER,
                   double estimated_fov_slope = -16, double zOffset=0);
    ErrorCodeStruct performOC(bool enableMotion, bool fastMode);
    double calculateDFOV(cv::Mat img);
    void setSfrWorkerController(SfrWorkerController*);
    bool runFlowchartTest();
    ErrorCodeStruct performTest(QString testItemName, QJsonValue properties);
    AAData aaData_1;  // For Display Channel 1
    AAData aaData_2;  // For Display Channel 2
    ImageProvider * ocImageProvider_1;

private:
    AAHeadModule* aa_head;
    LutModule* lut;
    SutModule* sut;
    Dothinkey* dk;
    ChartCalibration* chartCalibration;
    SfrWorkerController * sfrWorkerController = Q_NULLPTR;
    std::unordered_map<unsigned int, std::vector<Sfr_entry>> clustered_sfr_map;
    QJsonDocument flowchartDocument;
    bool isZScanNeedToStop = false;
    double cmosPixelToMM_X = 0,cmosPixelToMM_Y = 0;
    int currentChartDisplayChannel = 0;
    int currentOCDisplayChannel = 0;
    void sfrFitCurve_Advance(double imageWidth, double imageHeight, double &xTilt, double &yTilt,
                             double &zPeak, double &ul_zPeak, double &ur_zPeak, double &ll_zPeak, double &lr_zPeak);
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
public slots:

    void storeSfrResults(unsigned int index, vector<Sfr_entry> sfrs, int timeElasped);
    void stopZScan();
    void setFlowchartDocument(QString json){
        flowchartDocument = QJsonDocument::fromJson(json.toUtf8());
    }
};

#endif // AACORE_H
