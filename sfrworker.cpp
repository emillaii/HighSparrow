#include <sfrworker.h>
#include <visionavadaptor.h>
//#include "sparrow.h"
//#include "imagegrabbingworkerthread.h"
#include "aacorenew.h"
#include <QElapsedTimer>
#include <QImage>
#include "imagegrabbingworkerthread.h"
double prev_cc_score = 0, prev_ul_score = 0, prev_ur_score = 0, prev_ll_score = 0, prev_lr_score = 0;
bool is_cc_falling = false, is_ul_falling = false, is_ur_falling = false, is_ll_falling = false, is_lr_falling = false;

void SfrWorker::doWork(unsigned int index, double z, cv::Mat img, bool is_display_image, sfr::EdgeFilter edgeFilter)
{
    if (index == 0) {  //Reset all the curve analysis
        prev_cc_score = 0; prev_ul_score = 0; prev_ur_score = 0; prev_ll_score = 0; prev_lr_score = 0;
        is_cc_falling = false; is_ul_falling = false; is_ur_falling = false; is_ll_falling = false; is_lr_falling = false;
    }
    cv::Mat image = img.clone();
    QImage outImage;
    unsigned int ccIndex = 0, ulIndex = 0, urIndex = 0, lrIndex = 0, llIndex = 0;
    std::vector<AA_Helper::patternAttr> patterns = AA_Helper::AA_Search_MTF_Pattern(img, outImage, true, ccIndex, ulIndex, urIndex, lrIndex, llIndex, this->max_intensity, this->min_area, this->max_area);
    //Add protection here
    std::vector<Sfr_entry> sfr_v;
    if (patterns.size() < 5) {
        //qInfo() << "Cannot find sufficient pattern size" << QString::number(patterns.size());
        emit sfrResultsReady(index, std::move(sfr_v), 0);
        image.release();
        return;
    }
    cv::Rect roi;
    double area = patterns[0].area;
    roi.width = sqrt(area)*this->roi_ratio;
    roi.height = roi.width;

    cv::Mat ccMat, ulMat, urMat, llMat, lrMat;
    QElapsedTimer timerTest;
    timerTest.start();
    for (unsigned int k = 0; k < patterns.size(); k++) {
        if (k == ccIndex || k == ulIndex || k == urIndex || k == llIndex || k == lrIndex)
        {
            roi.x = patterns[k].center.x() - roi.width/2;
            roi.y = patterns[k].center.y() - roi.height/2;
            if (roi.x < 0 || roi.y < 0 || roi.x + roi.width > img.cols || roi.y + roi.height > img.rows)
            {
                //qInfo() << "Cannot find sufficient pixel data for SFR calculation. Hitting boundary" ;
                emit sfrResultsReady(index, std::move(sfr_v), 0);
                image.release();
                return;
            }
            cv::Mat cropImage = img(roi);
            if (k == ulIndex) ulMat = cropImage.clone();
            if (k == urIndex) urMat = cropImage.clone();
            if (k == llIndex) llMat = cropImage.clone();
            if (k == lrIndex) lrMat = cropImage.clone();
            //if (k == ccIndex) ccMat = cropImage.clone();
            //imwrite(QString::number(index).append(QString::number(k)).append(".bmp").toStdString().c_str(), cropImage);
            if (k == ccIndex)
            {
                vector<Sfr_entry> sv = sfr::calculateSfr(z, cropImage, edgeFilter);
                if (sv.size() == 1)
                {
                    sv[0].px = patterns[k].center.x();
                    sv[0].py = patterns[k].center.y();
                    sv[0].area = patterns[k].area;
                    if (k == ccIndex) {
                        if (sv[0].sfr + 10 < prev_cc_score) {
                            is_cc_falling = true;
                        }
                        prev_cc_score = sv[0].sfr;
                        qInfo("cc sfr: %f", sv[0].sfr);
                    } else if (k == ulIndex) {
                        if (sv[0].sfr + 10 < prev_ul_score) {
                            is_ul_falling = true;
                        }
                        prev_ul_score = sv[0].sfr;
                        qInfo("ul sfr: %f", sv[0].sfr);
                    } else if (k == urIndex) {
                        if (sv[0].sfr + 10 < prev_ur_score) {
                            is_ur_falling = true;
                        }
                        prev_ur_score = sv[0].sfr;
                        qInfo("ur sfr: %f", sv[0].sfr);
                    } else if (k == llIndex) {
                        if (sv[0].sfr + 10 < prev_ll_score) {
                            is_ll_falling = true;
                        }
                        prev_ll_score = sv[0].sfr;
                        qInfo("ll sfr: %f", sv[0].sfr);
                    } else if (k == lrIndex) {
                        if (sv[0].sfr + 10 < prev_lr_score) {
                            is_lr_falling = true;
                        }
                        prev_lr_score = sv[0].sfr;
                        qInfo("lr sfr: %f", sv[0].sfr);
                    }
                    sfr_v.push_back(sv[0]);
                }
            }
        }
    }

    cv::hconcat(ulMat, urMat, ulMat);
    cv::hconcat(llMat, lrMat, llMat);
    cv::vconcat(ulMat, llMat, ulMat);
    // imwrite(QString::number(count).append(".bmp").toStdString().c_str(), ulMat);
    // qInfo("sfr timer concat elapsed: %d", timerconcatTest.elapsed());
    // QElapsedTimer timerTest;
    // timerTest.start();
    vector<Sfr_entry> sv = sfr::calculateSfr(z, ulMat, edgeFilter);
    int tolerence = 70;
    bool isULIndexFound = false;
    bool isURIndexFound = false;
    bool isLLIndexFound = false;
    bool isLRIndexFound = false;
    for (unsigned int i = 0; i < sv.size(); i++) {
        int roi_index;
        double ul_d = sqrt(pow(sv[i].px - roi.width/2, 2) + pow(sv[i].py - roi.height/2, 2));
        double ur_d = sqrt(pow(sv[i].px - roi.width/2*3, 2) + pow(sv[i].py - roi.height/2, 2));
        double ll_d = sqrt(pow(sv[i].px - roi.width/2, 2) + pow(sv[i].py - roi.height/2*3, 2));
        double lr_d = sqrt(pow(sv[i].px - roi.width/2*3, 2) + pow(sv[i].py - roi.height/2*3, 2));
        if (ul_d < tolerence) {
            roi_index = ulIndex;
            if (sv[i].sfr + 10 < prev_ul_score) {
                is_ul_falling = true;
            }
            prev_ul_score = sv[i].sfr;
            isULIndexFound = true;
        } else if (ur_d < tolerence) {
            roi_index = urIndex;
            if (sv[i].sfr + 10 < prev_ur_score) {
                is_ur_falling = true;
            }
            prev_ur_score = sv[i].sfr;
            isURIndexFound = true;
        } else if (ll_d < tolerence) {
            roi_index = llIndex;
            if (sv[i].sfr + 10 < prev_ll_score) {
                is_ll_falling = true;
            }
            prev_ll_score = sv[i].sfr;
            isLLIndexFound = true;
        } else if (lr_d < tolerence) {
            roi_index = lrIndex;
            if (sv[i].sfr + 10 < prev_lr_score) {
                is_lr_falling = true;
            }
            prev_lr_score = sv[i].sfr;
            isLRIndexFound = true;
        }
        sv[i].px = patterns[roi_index].center.x();
        sv[i].py = patterns[roi_index].center.y();
        sv[i].area = patterns[roi_index].area;

        sfr_v.push_back(sv[i]);
    }

    if (!isURIndexFound) {
        qInfo("Cannot find UR Index");
        Sfr_entry sfr_entry(patterns[urIndex].center.x(), patterns[urIndex].center.y(),
                            z, 0, patterns[urIndex].area);
        sfr_v.push_back(sfr_entry);
    }
    if (!isULIndexFound) {

        qInfo("Cannot find UL Index");
        Sfr_entry sfr_entry(patterns[ulIndex].center.x(), patterns[ulIndex].center.y(),
                            z, 0, patterns[ulIndex].area);
        sfr_v.push_back(sfr_entry);
    }
    if (!isLRIndexFound) {

        qInfo("Cannot find LR Index sfr_v size: %d", sfr_v.size());
        Sfr_entry sfr_entry(patterns[lrIndex].center.x(), patterns[lrIndex].center.y(),
                            z, 0, patterns[lrIndex].area);
        sfr_v.push_back(sfr_entry);

        qInfo("Cannot find LR Index sfr_v size: %d", sfr_v.size());
    }
    if (!isLLIndexFound) {

        qInfo("Cannot find LL Index");
        Sfr_entry sfr_entry(patterns[llIndex].center.x(), patterns[llIndex].center.y(),
                            z, 0, patterns[llIndex].area);
        sfr_v.push_back(sfr_entry);
    }

    qInfo("sfr test elapsed: %d", timerTest.elapsed());
    if (is_cc_falling &&
        is_ur_falling &&
        is_ul_falling &&
        is_lr_falling &&
        is_ll_falling )
    {
        qInfo("All peak is just passed");
        emit sfrResultsDetectFinished();
    }
    if (is_display_image)
    {
         int radius = 2;
         CvFont font;
         cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 2, 2, 5, 3);
         for (unsigned int i = 0; i < sfr_v.size(); i++){
            double x = sfr_v[i].px;
            double y = sfr_v[i].py;
            cv::Point centroid(x, y);
            cv::Point centroid_shift(x+10, y-10);
            cv::circle(image, centroid, radius, cv::Scalar(0, 255, 0), 3);
            QString score_str = QString::number(sfr_v[i].sfr);
            if (score_str.size() > 5)
                score_str = score_str.remove(5, score_str.size());
            cv::putText(image, score_str.toStdString(), centroid_shift, cv::FONT_HERSHEY_SIMPLEX, 2.5, cv::Scalar(255, 0,255), 3);
         }
        QImage qImage = ImageGrabbingWorkerThread::cvMat2QImage(image);
        emit imageReady(std::move(qImage));
    }
    emit sfrResultsReady(index, std::move(sfr_v),timerTest.elapsed());
    image.release();
    ulMat.release();
    urMat.release();
    llMat.release();
    lrMat.release();
}

SfrWorkerController::SfrWorkerController(AACoreNew *a)
{
   std::vector<Sfr_entry> sfr_v;
   aaCore_ = a;
   SfrWorker * worker = new SfrWorker;
   worker->max_intensity = a->parameters.MaxIntensity();
   worker->min_area = a->parameters.MinArea();
   worker->max_area = a->parameters.MaxArea();
   worker->roi_ratio = a->parameters.ROIRatio();
   qInfo("Min Area: %d Max Area: %d Max I: %d Roi Ratio: %f", a->parameters.MinArea(), a->parameters.MaxArea(), a->parameters.MaxIntensity(), a->parameters.ROIRatio());
   worker->moveToThread(&workerThread);
   connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
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
