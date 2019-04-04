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
    Q_INVOKABLE void testAAOffline();
    void setSfrWorkerController(SfrWorkerController*);
private:
    SfrWorkerController * sfrWorkerController = Q_NULLPTR;
    std::unordered_map<unsigned int, std::vector<Sfr_entry>> clustered_sfr_map;
    bool isZScanNeedToStop = false;
signals:
    void sfrResultsReady(unsigned int, vector<Sfr_entry>, int);
    void sfrResultsDetectFinished();
public slots:
    void storeSfrResults(unsigned int index, vector<Sfr_entry> sfrs, int timeElasped);
    void stopZScan();
};

#endif // AACORE_H
