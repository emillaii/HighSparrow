#include "AACore/sfrworker.h"
#include <visionavadaptor.h>
#include "AACore/aacorenew.h"
#include <QElapsedTimer>
#include <QImage>
#include <QPainter>
#include "imageGrabber/imagegrabbingworkerthread.h"

#define CONSTANT_REFERENCE 2304

double prev_cc_score = 0, prev_ul_score = 0, prev_ur_score = 0, prev_ll_score = 0, prev_lr_score = 0;
bool is_cc_falling = false, is_ul_falling = false, is_ur_falling = false, is_ll_falling = false, is_lr_falling = false;

int count = 0;

void SfrWorker::doWork(unsigned int index, double z, cv::Mat img, bool is_display_image, int freq_factor)
{
    double display_factor = img.cols/CONSTANT_REFERENCE;
    QElapsedTimer timerTest;
    timerTest.start();
    if (index == 0) {  //Reset all the curve analysis
        prev_cc_score = 0; prev_ul_score = 0; prev_ur_score = 0; prev_ll_score = 0; prev_lr_score = 0;
        is_cc_falling = false; is_ul_falling = false; is_ur_falling = false; is_ll_falling = false; is_lr_falling = false;
    }
    cv::Mat displayImage = img.clone();

    vector<Sfr_entry> sv_result = sfr::calculateSfr(z, img, freq_factor);
    vector<Sfr_entry> sv = sv_result;
    if (sv.size() == 0) {
        qInfo("Cannot find any mtf pattern. Sfr calculation fail");
        emit sfrResultsReady(index, std::move(sv_result), 0);
        return;
    }
    int roi_width = sqrt(sv[0].area)*this->roi_ratio;

    emit sfrResultsReady(index, std::move(sv_result), timerTest.elapsed());
    if (is_display_image) {
        QImage qImage = ImageGrabbingWorkerThread::cvMat2QImage(displayImage);
        QPainter qPainter(&qImage);
        qPainter.setBrush(Qt::NoBrush);
        qPainter.setPen(QPen(Qt::blue, 4.0));
        qPainter.setFont(QFont("Times",75*display_factor, QFont::Light));
        for (Sfr_entry sfr_entry : sv) {
            qPainter.drawText(sfr_entry.px - 50 , sfr_entry.py - roi_width/2, QString::number(sfr_entry.t_sfr, 'g', 4));
            qPainter.drawText(sfr_entry.px + roi_width/2, sfr_entry.py,  QString::number(sfr_entry.r_sfr, 'g', 4));
            qPainter.drawText(sfr_entry.px - 50, sfr_entry.py + roi_width/2,  QString::number(sfr_entry.b_sfr, 'g', 4));
            qPainter.drawText(sfr_entry.px - roi_width/2 - 100, sfr_entry.py,  QString::number(sfr_entry.l_sfr, 'g', 4));
        }
        qPainter.end();
        emit imageReady(std::move(qImage));
    }
    displayImage.release();
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
   connect(worker, &SfrWorker::imageReady, aaCore_, &AACoreNew::sfrImageReady, Qt::DirectConnection);
   connect(worker, &SfrWorker::sfrResultsReady, aaCore_, &AACoreNew::sfrResultsReady, Qt::DirectConnection);
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
