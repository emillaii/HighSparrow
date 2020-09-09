#ifndef SFRWORKER_H
#define SFRWORKER_H

#include <QThread>
#include <QImage>
#include <vector>
#include "sfr.h"
#include <opencv2/core/core.hpp>
#include <sfr_entry.h>

class AACoreNew;

class SfrWorker : public QObject
{
    Q_OBJECT
public slots:
    void doWork(unsigned int index, double z, cv::Mat img, int max_intensity, int min_area, int max_area, bool is_display_image = false, int freq_factor = 1, int overSampling = 1, int customCenterX = -1, int customCenterY = -1);
signals:
    void imageReady(QImage img);
    void sfrResultsReady(unsigned int index, std::vector<Sfr_entry> res, int timeElapsed);
    void sfrResultsDetectFinished();
public:
    int max_intensity = 50;
    int min_area = 10000;
    int max_area = 90000;
    double roi_ratio = 1.4;
};

class SfrWorkerController: public QObject
{
    Q_OBJECT
public:
    SfrWorkerController(AACoreNew *aaCore);
    void setSfrWorkerParams(QJsonValue params);
    ~SfrWorkerController();

signals:
    void calculate(unsigned int index, double z, cv::Mat image, int max_intensity, int min_area, int max_area, bool is_display_image = false, int freq_factor = 1, int overSampling = 1, int customCenterX = -1, int customCenterY = -1);
    void test();
private:
    QThread workerThread;
    AACoreNew * aaCore_;
    SfrWorker * worker;
};

#endif // SFRWORKER_H
