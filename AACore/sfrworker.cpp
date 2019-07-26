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
    vector<Sfr_entry> sv_result = sfr::calculateSfr(z, img, freq_factor*2);   // Freq 0.25
    //vector<Sfr_entry> sv_result = sfr::calculateSfr(z, img, freq_factor);   //Freq: 0.125
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
//void SfrWorker::doWork(unsigned int index, double z, cv::Mat img, bool is_display_image, sfr::EdgeFilter edgeFilter)
//{
//    if (index == 0) {  //Reset all the curve analysis
//        prev_cc_score = 0; prev_ul_score = 0; prev_ur_score = 0; prev_ll_score = 0; prev_lr_score = 0;
//        is_cc_falling = false; is_ul_falling = false; is_ur_falling = false; is_ll_falling = false; is_lr_falling = false;
//    }
//    cv::Mat image = img.clone();
//    QImage outImage;
//    unsigned int ccIndex = 0, ulIndex = 0, urIndex = 0, lrIndex = 0, llIndex = 0;
//    std::vector<AA_Helper::patternAttr> patterns = AA_Helper::AA_Search_MTF_Pattern(img, outImage, true, ccIndex, ulIndex, urIndex, llIndex, lrIndex, this->max_intensity, this->min_area, this->max_area);
//    //Add protection here
//    std::vector<Sfr_entry> sfr_v;
//    if (patterns.size() < 5) {
//        //qInfo() << "Cannot find sufficient pattern size" << QString::number(patterns.size());
//        emit sfrResultsReady(index, std::move(sfr_v), 0);
//        image.release();
//        return;
//    }
//    cv::Rect roi;
//    double area = patterns[0].area;
//    roi.width = sqrt(area)*this->roi_ratio;
//    roi.height = roi.width;

//    cv::Mat ccMat, cc1Mat, ulMat, urMat, llMat, lrMat;
//    QElapsedTimer timerTest;
//    timerTest.start();
//    for (unsigned int k = 0; k < patterns.size(); k++) {
//        if (k == ccIndex || k == ulIndex || k == urIndex || k == llIndex || k == lrIndex)
//        {
//            roi.x = patterns[k].center.x() - roi.width/2;
//            roi.y = patterns[k].center.y() - roi.height/2;
//            if (roi.x < 0 || roi.y < 0 || roi.x + roi.width > img.cols || roi.y + roi.height > img.rows)
//            {
//                //qInfo() << "Cannot find sufficient pixel data for SFR calculation. Hitting boundary" ;
//                emit sfrResultsReady(index, std::move(sfr_v), 0);
//                image.release();
//                return;
//            }
//            cv::Mat cropImage = img(roi);
//            if (k == ulIndex) ulMat = cropImage.clone();
//            if (k == urIndex) urMat = cropImage.clone();
//            if (k == llIndex) llMat = cropImage.clone();
//            if (k == lrIndex) lrMat = cropImage.clone();
//            //if (k == ccIndex) ccMat = cropImage.clone();
//            //imwrite(QString::number(index).append(QString::number(k)).append(".bmp").toStdString().c_str(), cropImage);
//            if (k == ccIndex)
//            {
//                vector<Sfr_entry> sv = sfr::calculateSfr(z, cropImage, edgeFilter);
//                if (sv.size() == 1)
//                {
//                    sv[0].px = patterns[k].center.x();
//                    sv[0].py = patterns[k].center.y();
//                    sv[0].area = patterns[k].area;
//                    if (k == ccIndex) {
//                        if (sv[0].sfr + 10 < prev_cc_score) {
//                            is_cc_falling = true;
//                        }
//                        prev_cc_score = sv[0].sfr;
//                        qInfo("cc sfr: %f", sv[0].sfr);
//                    } else if (k == ulIndex) {
//                        if (sv[0].sfr + 10 < prev_ul_score) {
//                            is_ul_falling = true;
//                        }
//                        prev_ul_score = sv[0].sfr;
//                        qInfo("ul sfr: %f", sv[0].sfr);
//                    } else if (k == urIndex) {
//                        if (sv[0].sfr + 10 < prev_ur_score) {
//                            is_ur_falling = true;
//                        }
//                        prev_ur_score = sv[0].sfr;
//                        qInfo("ur sfr: %f", sv[0].sfr);
//                    } else if (k == llIndex) {
//                        if (sv[0].sfr + 10 < prev_ll_score) {
//                            is_ll_falling = true;
//                        }
//                        prev_ll_score = sv[0].sfr;
//                        qInfo("ll sfr: %f", sv[0].sfr);
//                    } else if (k == lrIndex) {
//                        if (sv[0].sfr + 10 < prev_lr_score) {
//                            is_lr_falling = true;
//                        }
//                        prev_lr_score = sv[0].sfr;
//                        qInfo("lr sfr: %f", sv[0].sfr);
//                    }
//                    sfr_v.push_back(sv[0]);
//                }
//            }
//        }
//    }

//    cv::hconcat(ulMat, urMat, ulMat);
//    cv::hconcat(llMat, lrMat, llMat);
//    cv::vconcat(ulMat, llMat, ulMat);
//    imwrite(QString::number(count).append(".bmp").toStdString().c_str(), ulMat);
//    count++;
//    // qInfo("sfr timer concat elapsed: %d", timerconcatTest.elapsed());
//    // QElapsedTimer timerTest;
//    // timerTest.start();
//    vector<Sfr_entry> sv = sfr::calculateSfr(z, ulMat, edgeFilter);
//    qInfo("SFR Block result number : %d", sv.size());
//    int tolerence = 100;
//    bool isULIndexFound = false;
//    bool isURIndexFound = false;
//    bool isLLIndexFound = false;
//    bool isLRIndexFound = false;
//    for (unsigned int i = 0; i < sv.size(); i++) {
//        int roi_index;
//        double ul_d = sqrt(pow(sv[i].px - roi.width/2, 2) + pow(sv[i].py - roi.height/2, 2));
//        double ur_d = sqrt(pow(sv[i].px - roi.width/2*3, 2) + pow(sv[i].py - roi.height/2, 2));
//        double ll_d = sqrt(pow(sv[i].px - roi.width/2, 2) + pow(sv[i].py - roi.height/2*3, 2));
//        double lr_d = sqrt(pow(sv[i].px - roi.width/2*3, 2) + pow(sv[i].py - roi.height/2*3, 2));
//        if (ul_d < tolerence) {
//            roi_index = ulIndex;
//            if (sv[i].sfr + 10 < prev_ul_score) {
//                is_ul_falling = true;
//            }
//            prev_ul_score = sv[i].sfr;
//            isULIndexFound = true;
//        } else if (ur_d < tolerence) {
//            roi_index = urIndex;
//            if (sv[i].sfr + 10 < prev_ur_score) {
//                is_ur_falling = true;
//            }
//            prev_ur_score = sv[i].sfr;
//            isURIndexFound = true;
//        } else if (ll_d < tolerence) {
//            roi_index = llIndex;
//            if (sv[i].sfr + 10 < prev_ll_score) {
//                is_ll_falling = true;
//            }
//            prev_ll_score = sv[i].sfr;
//            isLLIndexFound = true;
//        } else if (lr_d < tolerence) {
//            roi_index = lrIndex;
//            if (sv[i].sfr + 10 < prev_lr_score) {
//                is_lr_falling = true;
//            }
//            prev_lr_score = sv[i].sfr;
//            isLRIndexFound = true;
//        }
//        sv[i].px = patterns[roi_index].center.x();
//        sv[i].py = patterns[roi_index].center.y();
//        sv[i].area = patterns[roi_index].area;

//        sfr_v.push_back(sv[i]);
//    }

//    if (!isURIndexFound) {
//        qInfo("Cannot find UR Index");
//        Sfr_entry sfr_entry(patterns[urIndex].center.x(), patterns[urIndex].center.y(),
//                            z, 0, patterns[urIndex].area,0,0,0,0);
//        sfr_v.push_back(sfr_entry);
//    }
//    if (!isULIndexFound) {

//        qInfo("Cannot find UL Index");
//        Sfr_entry sfr_entry(patterns[ulIndex].center.x(), patterns[ulIndex].center.y(),
//                            z, 0, patterns[ulIndex].area,0,0,0,0);
//        sfr_v.push_back(sfr_entry);
//    }
//    if (!isLRIndexFound) {

//        qInfo("Cannot find LR Index sfr_v size: %d", sfr_v.size());
//        Sfr_entry sfr_entry(patterns[lrIndex].center.x(), patterns[lrIndex].center.y(),
//                            z, 0, patterns[lrIndex].area,0,0,0,0);
//        sfr_v.push_back(sfr_entry);

//        qInfo("Cannot find LR Index sfr_v size: %d", sfr_v.size());
//    }
//    if (!isLLIndexFound) {

//        qInfo("Cannot find LL Index");
//        Sfr_entry sfr_entry(patterns[llIndex].center.x(), patterns[llIndex].center.y(),
//                            z, 0, patterns[llIndex].area,0,0,0,0);
//        sfr_v.push_back(sfr_entry);
//    }

//    qInfo("sfr test elapsed: %d", timerTest.elapsed());
//    if (is_cc_falling &&
//        is_ur_falling &&
//        is_ul_falling &&
//        is_lr_falling &&
//        is_ll_falling )
//    {
//        qInfo("All peak is just passed");
//        emit sfrResultsDetectFinished();
//    }
//    if (is_display_image)
//    {
//         int radius = 2;
//         CvFont font;
//         cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 2,2, 2, 8);
//         for (unsigned int i = 0; i < sfr_v.size(); i++){
//            double x = sfr_v[i].px;
//            double y = sfr_v[i].py;
//            cv::Point centroid(x, y);
//            cv::Point centroid_shift(x-20, y-10);
//            cv::circle(image, centroid, radius, cv::Scalar(0, 255, 0), 3);
//            QString score_str = QString::number(sfr_v[i].sfr);
//            QString score_t_str = QString::number(sfr_v[i].t_sfr);
//            QString score_r_str = QString::number(sfr_v[i].r_sfr);
//            QString score_b_str = QString::number(sfr_v[i].b_sfr);
//            QString score_l_str = QString::number(sfr_v[i].l_sfr);
//            if (score_str.size() > 5)
//                score_str = score_str.remove(5, score_str.size());
//            cv::putText(image, score_str.toStdString(), centroid_shift, cv::FONT_HERSHEY_SIMPLEX, 3*(image.cols/CONSTANT_REFERENCE), cv::Scalar(255, 0,255), 8*(image.cols/CONSTANT_REFERENCE));
//            cv::Scalar ok_color = cv::Scalar(128, 128, 255);
//            if ( y > image.rows/2)
//            {
//                cv::putText(image, score_t_str.toStdString(), cv::Point(centroid_shift.x, centroid.y-200), cv::FONT_HERSHEY_SIMPLEX, 3*(image.cols/CONSTANT_REFERENCE)/1.5, ok_color, 8*(image.cols/CONSTANT_REFERENCE)/2);
//                cv::putText(image, score_l_str.toStdString(), cv::Point(centroid_shift.x, centroid.y-300), cv::FONT_HERSHEY_SIMPLEX, 3*(image.cols/CONSTANT_REFERENCE)/1.5, ok_color, 8*(image.cols/CONSTANT_REFERENCE)/2);
//                cv::putText(image, score_b_str.toStdString(), cv::Point(centroid_shift.x, centroid.y-400), cv::FONT_HERSHEY_SIMPLEX, 3*(image.cols/CONSTANT_REFERENCE)/1.5, ok_color, 8*(image.cols/CONSTANT_REFERENCE)/2);
//                cv::putText(image, score_r_str.toStdString(), cv::Point(centroid_shift.x, centroid.y-500), cv::FONT_HERSHEY_SIMPLEX, 3*(image.cols/CONSTANT_REFERENCE)/1.5, ok_color, 8*(image.cols/CONSTANT_REFERENCE)/2);
//            } else {
//                cv::putText(image, score_t_str.toStdString(), cv::Point(centroid_shift.x, centroid.y+100), cv::FONT_HERSHEY_SIMPLEX, 3*(image.cols/CONSTANT_REFERENCE)/1.5, ok_color, 8*(image.cols/CONSTANT_REFERENCE)/2);
//                cv::putText(image, score_l_str.toStdString(), cv::Point(centroid_shift.x, centroid.y+200), cv::FONT_HERSHEY_SIMPLEX, 3*(image.cols/CONSTANT_REFERENCE)/1.5, ok_color, 8*(image.cols/CONSTANT_REFERENCE)/2);
//                cv::putText(image, score_b_str.toStdString(), cv::Point(centroid_shift.x, centroid.y+300), cv::FONT_HERSHEY_SIMPLEX, 3*(image.cols/CONSTANT_REFERENCE)/1.5, ok_color, 8*(image.cols/CONSTANT_REFERENCE)/2);
//                cv::putText(image, score_r_str.toStdString(), cv::Point(centroid_shift.x, centroid.y+400), cv::FONT_HERSHEY_SIMPLEX, 3*(image.cols/CONSTANT_REFERENCE)/1.5, ok_color, 8*(image.cols/CONSTANT_REFERENCE)/2);
//            }
//        }
//        QImage qImage = ImageGrabbingWorkerThread::cvMat2QImage(image);
//        emit imageReady(std::move(qImage));
//    }
//    emit sfrResultsReady(index, std::move(sfr_v),timerTest.elapsed());
//    image.release();
//    ulMat.release();
//    urMat.release();
//    llMat.release();
//    lrMat.release();
//}

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
