#include "AACore/sfrworker.h"
#include <visionavadaptor.h>
#include "AACore/aacorenew.h"
#include <QElapsedTimer>
#include <QImage>
#include <QPainter>
#include "imagegrabbingworkerthread.h"

#define CONSTANT_REFERENCE 2304

double prev_cc_score = 0, prev_ul_score = 0, prev_ur_score = 0, prev_ll_score = 0, prev_lr_score = 0;
bool is_cc_falling = false, is_ul_falling = false, is_ur_falling = false, is_ll_falling = false, is_lr_falling = false;

int count = 0;

void SfrWorker::doWork(unsigned int index, double z, cv::Mat img, bool is_display_image, int freq_factor)
{
    qInfo("doWork");
    QElapsedTimer timerTest;
    timerTest.start();
    if (index == 0) {  //Reset all the curve analysis
        prev_cc_score = 0; prev_ul_score = 0; prev_ur_score = 0; prev_ll_score = 0; prev_lr_score = 0;
        is_cc_falling = false; is_ul_falling = false; is_ur_falling = false; is_ll_falling = false; is_lr_falling = false;
    }
    vector<Sfr_entry> sv_result = sfr::calculateSfr(z, img, freq_factor);
    vector<Sfr_entry> sv = sv_result;
    if (sv.size() == 0) {
        qInfo("Cannot find any mtf pattern. Sfr calculation fail");
        emit sfrResultsReady(index, std::move(sv_result), 0);
        return;
    }
    qInfo("Emit sfr results: %d %p", index);
    emit sfrResultsReady(index, std::move(sv_result), timerTest.elapsed());
}

SfrWorkerController::SfrWorkerController(AACoreNew *a)
{
   std::vector<Sfr_entry> sfr_v;
   aaCore_ = a;
   worker = new SfrWorker();
   worker->max_intensity = a->parameters.MaxIntensity();
   worker->min_area = a->parameters.MinArea();
   worker->max_area = a->parameters.MaxArea();
   worker->roi_ratio = a->parameters.ROIRatio();
   qInfo("Min Area: %d Max Area: %d Max I: %d Roi Ratio: %f", a->parameters.MinArea(), a->parameters.MaxArea(), a->parameters.MaxIntensity(), a->parameters.ROIRatio());
   worker->moveToThread(&workerThread);
   connect(this, &SfrWorkerController::calculate, worker, &SfrWorker::doWork);
   //connect(worker, &SfrWorker::imageReady, aaCore_, &AACoreNew::sfrImageReady, Qt::DirectConnection);
   qInfo("aaCore %p", aaCore_);
   connect(worker, &SfrWorker::sfrResultsReady, aaCore_, &AACoreNew::storeSfrResults, Qt::DirectConnection);
   connect(worker, &SfrWorker::sfrResultsDetectFinished, aaCore_, &AACoreNew::sfrResultsDetectFinished, Qt::DirectConnection);
   workerThread.start();
}

SfrWorkerController::~SfrWorkerController()
{
    workerThread.quit();
    workerThread.wait();
}

void SfrWorkerController::setSfrWorkerParams(QJsonValue params)
{
    if (worker != nullptr && aaCore_ != nullptr) {
        qInfo("setSfrWorkerParams is called");
        worker->max_intensity = aaCore_->parameters.MaxIntensity();
        worker->min_area = aaCore_->parameters.MinArea();
        worker->max_area = aaCore_->parameters.MaxArea();
        worker->roi_ratio = aaCore_->parameters.ROIRatio();
    }
}
