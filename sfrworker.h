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
    void doWork(unsigned int index, double z, cv::Mat img, bool is_display_image = false, sfr::EdgeFilter edgeFilter = sfr::EdgeFilter::NO_FILTER);
signals:
    void imageReady(QImage img);
    void sfrResultsReady(unsigned int index, std::vector<Sfr_entry> res, int timeElapsed);
    void sfrResultsDetectFinished();
};

class SfrWorkerController: public QObject
{
    Q_OBJECT
public:
    SfrWorkerController(AACoreNew *aaCore);
    ~SfrWorkerController();

signals:
    void calculate(unsigned int index, double z, cv::Mat image, bool is_display_image = false, sfr::EdgeFilter edgeFilter = sfr::EdgeFilter::NO_FILTER);
    void test();
private:
    QThread workerThread;
    AACoreNew * aaCore_;
};

#endif // SFRWORKER_H
