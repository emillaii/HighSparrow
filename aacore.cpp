#include "aacore.h"
#include <QVariantMap>
#include <QImage>
#include <QElapsedTimer>
#include <visionavadaptor.h>
#include <config.h>
#include <QThread>
#include <windows.h>
#include <stdlib.h>

AACore::AACore(QObject *parent) : QThread(parent)
{
    connect(this, &AACore::sfrResultsReady, this, &AACore::storeSfrResults);
    connect(this, &AACore::sfrResultsDetectFinished, this, &AACore::stopZScan);
}

AACore::~AACore()
{

}

void AACore::setSfrWorkerController(SfrWorkerController * sfrWorkerController){
    this->sfrWorkerController = sfrWorkerController;
}

void AACore::run(){

}

void AACore::testAAOffline()
{
    qInfo("Testing AA Offline");
    performAAOffline();
}

void AACore::performAAOffline()
{
    ErrorCodeStruct ret = { OK, ""};
    QVariantMap map, stepTimerMap, dFovMap, sfrTimeElapsedMap;
    QElapsedTimer timer, stepTimer;
    timer.start(); stepTimer.start();
    int sfrCount = 0;
    double step_size = 0.01, start = -0.5;
    int imageWidth, imageHeight;
    double xsum=0,x2sum=0,ysum=0,xysum=0;
    double estimated_fov_slope = 15;
    isZScanNeedToStop = false;
    QString foldername = AA_DEBUG_DIR;
    int inputImageCount = 17;
    for (int i = 0; i < inputImageCount; i++)
    {
        if (isZScanNeedToStop) {
            qInfo("All peak passed, stop zscan");
            break;
        }
        //QString filename = "aa_log\\aa_log_bug\\2018-11-10T14-42-55-918Z\\zscan_" + QString::number(i) + ".bmp";
        //QString filename = "aa_log\\aa_log_bug\\2018-11-10T14-42-55-918Z\\zscan_" + QString::number(i) + ".bmp";
        QString filename = "C:\\Users\\emil\\Desktop\\Test\\Samsung\\debug\\debug\\zscan_" + QString::number(i) + ".bmp";
        cv::Mat img = cv::imread(filename.toStdString());
        stepTimerMap.insert(QString("image_grab_").append(QString::number(i)), stepTimer.elapsed());
        stepTimer.restart();
        double dfov = calculateDFOV(img);

        if (i == 0)
        {
            double target_z = (49 - dfov)/estimated_fov_slope + start;
            qInfo("target_z : %f", target_z);
        }
        double currZ = start+i*step_size;
        //FOV Fitting
        xsum=xsum+currZ;                        //calculate sigma(xi)
        ysum=ysum+dfov;                         //calculate sigma(yi)
        x2sum=x2sum+pow(currZ,2);               //calculate sigma(x^2i)
        xysum=xysum+currZ*dfov;                 //calculate sigma(xi*yi)
        dFovMap.insert(QString::number(i), dfov);
        imageWidth = img.cols; imageHeight = img.rows;
        stepTimerMap.insert(QString("z_scan_move_").append(QString::number(i)), stepTimer.elapsed());
        stepTimer.restart();
        emit sfrWorkerController->calculate(i, start+i*step_size, img, false, sfr::EdgeFilter::VERTICAL_ONLY);
        img.release();
        sfrCount++;
    }
    int timeout=10000;
    while(this->clustered_sfr_map.size() != sfrCount && timeout >0) {
        qInfo("....  %u", this->clustered_sfr_map.size());
        Sleep(1);
        //QThread::msleep(1);
        timeout--;
    }
    qInfo("finished");
    if (timeout <= 0) {
        qInfo("Error in performing AA Offline: %d", timeout);
        return;
    }
//    for (int i = 0; i < sfrCount; i++) {
//        sfrTimeElapsedMap.insert(QString("sfr_time_elapsed_").append(QString::number(i)), this->sfr_timeElapsed_map[i]);
//    }
    double fov_slope     = (20*xysum-xsum*ysum)/(20*x2sum-xsum*xsum);               //calculate slope
    double fov_intercept = (x2sum*ysum-xsum*xysum)/(x2sum*20-xsum*xsum);            //calculate intercept

    stepTimerMap.insert("sfr_wait", stepTimer.elapsed());
    stepTimer.restart();

    double xTilt, yTilt, zPeak, ul_zPeak, ur_zPeak, ll_zPeak, lr_zPeak;
//    sfrFitCurve_Advance(imageWidth, imageHeight, xTilt, yTilt, zPeak, ul_zPeak, ur_zPeak, ll_zPeak, lr_zPeak);
//    if (isnan(xTilt) || isnan(yTilt)) {
//        qInfo("if (isnan(xTilt) || isnan(yTilt)) failed");
//        return;
//    }
//    double corner_deviation = AA_Helper::calculateAACornerDeviation(ul_zPeak, ur_zPeak, ll_zPeak, lr_zPeak);
    clustered_sfr_map.clear();
    qInfo("[performAAOffline] Finished xTilt: %f yTilt: %f zPeak: %f ulPeak: %f", xTilt, yTilt, zPeak, ul_zPeak);
    map.insert("fov_slope", fov_slope);
    map.insert("fov_intercept", fov_intercept);
    map.insert("imageWidth", imageWidth);
    map.insert("imageHeight", imageHeight);
    map.insert("z_scan_steps", inputImageCount);
    map.insert("xTilt", xTilt);
    map.insert("yTilt", yTilt);
    map.insert("zPeak", zPeak);
    map.insert("dfov", dFovMap);
    map.insert("ur_zPeak", ur_zPeak);
    map.insert("ul_zPeak", ul_zPeak);
    map.insert("lr_zPeak", lr_zPeak);
    map.insert("ll_zPeak", ll_zPeak);
 //   map.insert("corner_deviation", corner_deviation);
    map.insert("result", "PASS");
    map.insert("time_breakdown", stepTimerMap);
    map.insert("sfr_breakdown", sfrTimeElapsedMap);
    map.insert("timeElapsed", timer.elapsed());
    qInfo("time elapsed: %d", timer.elapsed());
    return;
}

double AACore::calculateDFOV(cv::Mat img)
{
    return 100;
}

void AACore::storeSfrResults(unsigned int index, vector<Sfr_entry> sfrs, int timeElapsed)
{
    qInfo("Received sfr result from index: %d timeElapsed: %d", index, timeElapsed);
    clustered_sfr_map[index] = std::move(sfrs);
}

void AACore::stopZScan()
{
    qInfo("stop z scan");
    isZScanNeedToStop = true;
}
