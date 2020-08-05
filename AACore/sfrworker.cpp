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

void SfrWorker::doWork(unsigned int index, double z, cv::Mat img, int max_intensity, int min_area, int max_area, bool is_display_image, int freq_factor, int overSampling)
{
    qInfo("Max I: %d Min A: %d Max A: %d", max_intensity, min_area, max_area);
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
        std::vector<AA_Helper::patternAttr> patterns = AA_Helper::AAA_Search_MTF_Pattern_Ex(img, max_intensity, min_area/(overSampling*overSampling), max_area/(overSampling*overSampling), -1);

        QList<QProcess *> processList;
        QList<QString> filenameList;
        for (uint i = 0; i < patterns.size(); i++) {
           //qInfo("Pattern width : %f height: %f", patterns[i].width, patterns[i].height);
           cv::Mat copped_roi;
           cv::Mat cropped_l_img, cropped_r_img, cropped_t_img, cropped_b_img;
           QString filename;
           //Crop ROI
           {
               cv::Rect roi;
               double width = sqrt(patterns[i].area)/2;
               roi.width = width*4; roi.height = width*4;
               roi.x = patterns[i].center.x() - width*2;
               roi.y = patterns[i].center.y() - width*2;
               img(roi).copyTo(copped_roi);
               filename = "ROI_";
               filename.append(QString::number(i)).append("_").append(QString::number(index)).append(".bmp");
               cv::imwrite(filename.toStdString(), copped_roi);
               QStringList args;
               args.append(filename);
               args.append(filename);
               QProcess *child = new QProcess();
               child->setWorkingDirectory(QDir::currentPath());
               child->start(QDir::currentPath() + "/sfr.bat", args);
               processList.push_back(child);
               //child->waitForFinished();
           }
           double radius = sqrt(pow(patterns[i].center.x() - imageCenterX, 2) + pow(patterns[i].center.y() - imageCenterY, 2));
           double f = radius/r1;
           filenameList.push_back(QString("temp/").append(filename).append("/raw_sfr_values.txt"));
           vec.emplace_back(patterns[i].center.x(), patterns[i].center.y(),
                            f, 0, 0, 0, 0, patterns[i].area, 0);
        }

        for (int i = 0; i < processList.size(); i++){
           processList.at(i)->waitForFinished();
        }

        for (int i = 0; i < filenameList.size(); i++) {
            QString filename = filenameList.at(i);
            qInfo(filename.toStdString().c_str());
            {
                QFile file;
                file.setFileName(filename);
                double sfr_l = 0;
                double sfr_r = 0;
                double sfr_t = 0;
                double sfr_b = 0;
                double avg_sfr = 0;

                if (file.open(QIODevice::ReadOnly))
                {
                   QTextStream in(&file);
                   int i = 0;
                   while (!in.atEnd())
                   {
                      QString line = in.readLine();
                      QStringList list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
                      QString value = list[24];
                      //qInfo(list[20].toStdString().c_str());
                      if ( i == 0 ) sfr_t = list[8].toDouble()*100;
                      if ( i == 1 ) sfr_r = list[8].toDouble()*100;
                      if ( i == 2 ) sfr_b = list[8].toDouble()*100;
                      if ( i == 3 ) sfr_l = list[8].toDouble()*100;
                      i++;
                   }
                   qInfo("sfr_t: %f ", sfr_t);
                   file.close();
                }
                avg_sfr = (sfr_t + sfr_r + sfr_b + sfr_l) /4;
                vec.at(i).l_sfr = sfr_l;
                vec.at(i).r_sfr = sfr_r;
                vec.at(i).b_sfr = sfr_b;
                vec.at(i).t_sfr = sfr_t;
                vec.at(i).avg_sfr = avg_sfr;
            }
        }

//        double centerX = vec[0].x; double centerY = vec[0].y;

//        for (size_t i = 0; i < vec.size(); i++) {
//            qInfo("A Layer: %d px: %f py: %f sfr: %f 1: %f 2: %f 3: %f 4: %f area: %f", vec[i].layer,
//                  vec[i].x, vec[i].y, vec[i].avg_sfr, vec[i].t_sfr, vec[i].r_sfr, vec[i].b_sfr, vec[i].l_sfr, vec[i].area);
//        }

//        vector<int> layers = sfr::classifyLayers(vec);

//        for (size_t i = 0; i < vec.size(); i++) {
//            qInfo("B Layer: %d px: %f py: %f sfr: %f 1: %f 2: %f 3: %f 4: %f area: %f", vec[i].layer,
//                  vec[i].x, vec[i].y, vec[i].avg_sfr, vec[i].t_sfr, vec[i].r_sfr, vec[i].b_sfr, vec[i].l_sfr, vec[i].area);
//        }
//        int layers = 4;
//        if (layers >= 1) {
//            //qInfo("[calculateSfr] Layer 0: px: %f py: %f sfr: %f 1: %f 2: %f 3: %f 4: %f area: %f",
//            //      vec[0].x, vec[0].y, vec[0].avg_sfr, vec[0].t_sfr, vec[0].r_sfr, vec[0].b_sfr, vec[0].l_sfr, vec[0].area);
            Sfr_entry entry = Sfr_entry(vec[0].x, vec[0].y, z, vec[0].avg_sfr, vec[0].area,
                                        vec[0].t_sfr, vec[0].r_sfr, vec[0].b_sfr, vec[0].l_sfr, vec[0].layer, 0);
            sv_result.push_back(entry);
//        }
//        if (layers >= 2) {
            for (size_t i = 1; i < vec.size(); i++) {
                int location = 1;
                if ( (vec[i].x < imageCenterX) && (vec[i].y < imageCenterY))
                {
                    location = 1; //UL
                    qInfo("UL Layer %d :  px: %f py: %f sfr: %f 1: %f 2: %f 3: %f 4: %f area: %f", ((i-1)/4) + 1,
                          vec[i].x, vec[i].y, vec[i].avg_sfr, vec[i].t_sfr, vec[i].r_sfr, vec[i].b_sfr, vec[i].l_sfr, vec[i].area);
                } else if ((vec[i].x > imageCenterX) && (vec[i].y < imageCenterY))
                {
                    location = 2; //UR
                    qInfo("UR Layer %d :  px: %f py: %f sfr: %f 1: %f 2: %f 3: %f 4: %f area: %f", ((i-1)/4) + 1,
                          vec[i].x, vec[i].y, vec[i].avg_sfr, vec[i].t_sfr, vec[i].r_sfr, vec[i].b_sfr, vec[i].l_sfr, vec[i].area);
                } else if ((vec[i].x > imageCenterX) && (vec[i].y > imageCenterY))
                {
                    location = 3; //LR
                    qInfo("LR Layer %d :  px: %f py: %f sfr: %f 1: %f 2: %f 3: %f 4: %f area: %f", ((i-1)/4) + 1,
                          vec[i].x, vec[i].y, vec[i].avg_sfr, vec[i].t_sfr, vec[i].r_sfr, vec[i].b_sfr, vec[i].l_sfr, vec[i].area);
                } else if ((vec[i].x < imageCenterX) && (vec[i].y > imageCenterY))
                {
                    location = 4; //LL
                    qInfo("LL Layer %d :  px: %f py: %f sfr: %f 1: %f 2: %f 3: %f 4: %f area: %f", ((i-1)/4) + 1,
                          vec[i].x, vec[i].y, vec[i].avg_sfr, vec[i].t_sfr, vec[i].r_sfr, vec[i].b_sfr, vec[i].l_sfr, vec[i].area);
                }
                Sfr_entry entry = Sfr_entry(vec[i].x, vec[i].y, z, vec[i].avg_sfr, vec[i].area,
                                           vec[i].t_sfr, vec[i].r_sfr, vec[i].b_sfr, vec[i].l_sfr, vec[i].layer, location);
                sv_result.push_back(entry);
            }
//        }
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
