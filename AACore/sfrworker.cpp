#include "AACore/sfrworker.h"
#include <visionavadaptor.h>
#include "AACore/aacorenew.h"
#include <QElapsedTimer>
#include <QImage>
#include <QPainter>
#include "ImageGrabber/imagegrabbingworkerthread.h"

#define CONSTANT_REFERENCE 2304

double prev_cc_score = 0, prev_ul_score = 0, prev_ur_score = 0, prev_ll_score = 0, prev_lr_score = 0;
bool is_cc_falling = false, is_ul_falling = false, is_ur_falling = false, is_ll_falling = false, is_lr_falling = false;

int count = 0;

void SfrWorker::doWork(unsigned int index, double z, cv::Mat img, int max_intensity_1, int max_intensity_2, int min_area, int max_area, int freq_factor)
{
    qInfo("Max I_1: %d Max I_2: %d Min A: %d Max A: %d", max_intensity_1, max_intensity_2, min_area, max_area);
    QElapsedTimer timerTest;
    timerTest.start();
    if (index == 0) {  //Reset all the curve analysis
        prev_cc_score = 0; prev_ul_score = 0; prev_ur_score = 0; prev_ll_score = 0; prev_lr_score = 0;
        is_cc_falling = false; is_ul_falling = false; is_ur_falling = false; is_ll_falling = false; is_lr_falling = false;
    }
    vector<Sfr_entry> sv_result;
    {
        std::vector<MTF_Pattern_Position> vec;
        double imageCenterX = img.cols/2;
        double imageCenterY = img.rows/2;
        double r1 = sqrt(imageCenterX*imageCenterX + imageCenterY*imageCenterY);
        std::vector<AA_Helper::patternAttr> patterns = AA_Helper::AAA_Search_MTF_Pattern_Ex(img, max_intensity_1, min_area, max_area, -1);
//        std::vector<AA_Helper::patternAttr> patterns1 = AA_Helper::AAA_Search_MTF_Pattern_Ex(img, max_intensity_1, min_area, max_area, -1);
//        std::vector<AA_Helper::patternAttr> patterns2 = AA_Helper::AAA_Search_MTF_Pattern_Ex(img, max_intensity_2, min_area, max_area, -1);
//        std::vector<AA_Helper::patternAttr> patterns;

//        if (patterns2.size() < patterns1.size()) {
//            patterns2.swap(patterns1);
//        }

//        if (patterns2.size() >= patterns1.size()) {
//            for (uint i = 0; i < patterns2.size(); i++) {
//                patterns.push_back(patterns2[i]);
//            }
//            for (uint i = 0; i < patterns1.size(); i++) {
//                bool isDuplicated = false;
//                for (uint j = 0; j < patterns2.size(); j++) {
//                    double positionDiff = sqrt(pow(patterns1[i].center.x()-patterns2[j].center.x(), 2) + pow(patterns1[i].center.y()-patterns2[j].center.y(),2));
//                    if (positionDiff < 20) { //threshold diff
//                        isDuplicated = true;
//                    }
//                }
//                if (!isDuplicated) {
//                    patterns.push_back(patterns1[i]);
//                }
//            }
//        }
        for (uint i = 0; i < patterns.size(); i++) {
            //Crop ROI
            {
                cv::Rect roi; cv::Mat copped_roi;
                double width = sqrt(patterns[i].area)/2;
                roi.width = width*4; roi.height = width*4;
                roi.x = patterns[i].center.x() - width*2;
                roi.y = patterns[i].center.y() - width*2;
                if (roi.x < 0) roi.x = 0;
                if (roi.x + roi.width > img.cols) { roi.width = img.cols - roi.x; }
                if (roi.y < 0) roi.y = 0;
                if (roi.y + roi.height > img.rows) { roi.height = img.rows - roi.y; }

                img(roi).copyTo(copped_roi);
                double radius = sqrt(pow(patterns[i].center.x() - imageCenterX, 2) + pow(patterns[i].center.y() - imageCenterY, 2));
                double f = radius/r1;
                double t_sfr = 0, r_sfr = 0, b_sfr = 0, l_sfr = 0;
                sfr::sfr_calculation_single_pattern(copped_roi, t_sfr, r_sfr, b_sfr, l_sfr, 8*freq_factor);
                double avg_sfr = ( t_sfr + r_sfr + b_sfr + l_sfr)/4;
                vec.emplace_back(patterns[i].center.x(), patterns[i].center.y(),
                                 f, t_sfr*100, r_sfr*100, b_sfr*100, l_sfr*100, patterns[i].area, avg_sfr*100);
             }
        }

        vector<int> layers = sfr::classifyLayers(vec);

        if (layers.size() >= 1) {
            Sfr_entry entry = Sfr_entry(vec[0].x, vec[0].y, z, vec[0].avg_sfr, vec[0].area,
                                       vec[0].t_sfr, vec[0].r_sfr, vec[0].b_sfr, vec[0].l_sfr, vec[0].layer, 0);
            sv_result.push_back(entry);
        }
        if (layers.size() >= 2) {
            for (size_t i = 1; i < vec.size(); i++) {
                int location = 1;
                if ( (vec[i].x < imageCenterX) && (vec[i].y < imageCenterY))
                {
                    location = 1; //UL
                } else if ((vec[i].x > imageCenterX) && (vec[i].y < imageCenterY))
                {
                    location = 2; //UR
                } else if ((vec[i].x > imageCenterX) && (vec[i].y > imageCenterY))
                {
                    location = 3; //LR
                } else if ((vec[i].x < imageCenterX) && (vec[i].y > imageCenterY))
                {
                    location = 4; //LL
                }
                Sfr_entry entry = Sfr_entry(vec[i].x, vec[i].y, z, vec[i].avg_sfr, vec[i].area,
                                           vec[i].t_sfr, vec[i].r_sfr, vec[i].b_sfr, vec[i].l_sfr, vec[i].layer, location);
                sv_result.push_back(entry);
            }
        }
    }

    if (sv_result.size() == 0) {
        qInfo("Cannot find any mtf pattern. Sfr calculation fail");
        emit sfrResultsReady(index, std::move(sv_result), 0);
        return;
    }

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
