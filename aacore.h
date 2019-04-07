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
class AACore : public QThread
{
    Q_OBJECT
public:
    explicit AACore(QObject *parent = nullptr);
    ~AACore();

protected:
    void run();

public:
    void performAAOffline();
    double calculateDFOV(cv::Mat img);
    void setSfrWorkerController(SfrWorkerController*);
    bool runFlowchartTest();
    AAData aaData_1;

private:
    SfrWorkerController * sfrWorkerController = Q_NULLPTR;
    std::unordered_map<unsigned int, std::vector<Sfr_entry>> clustered_sfr_map;
    QJsonDocument flowchartDocument;
    bool isZScanNeedToStop = false;
    void sfrFitCurve_Advance(double imageWidth, double imageHeight, double &xTilt, double &yTilt,
                             double &zPeak, double &ul_zPeak, double &ur_zPeak, double &ll_zPeak, double &lr_zPeak);

signals:
    void sfrResultsReady(unsigned int, vector<Sfr_entry>, int);
    void sfrResultsDetectFinished();
public slots:
    void storeSfrResults(unsigned int index, vector<Sfr_entry> sfrs, int timeElasped);
    void stopZScan();
    void setFlowchartDocument(QString json){
        flowchartDocument = QJsonDocument::fromJson(json.toUtf8());
    }
};

#endif // AACORE_H
