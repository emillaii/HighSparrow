#include "aacore.h"
#include <QVariantMap>
#include <QImage>
#include <QElapsedTimer>
#include <visionavadaptor.h>
#include <config.h>
#include <QThread>
#include <stdlib.h>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include "aa_util.h"
#include "commonutils.h"
#define PI  3.14159265

typedef enum {
    AA_ZSCAN_NORMAL
} ZSCAN_MODE;

AACore::AACore(AAHeadModule* aa_head,LutModule* lut,SutModule* sut,Dothinkey* dk, ChartCalibration * chartCalibration,DispenseModule* dispense,QObject *parent) : QThread(parent)
{
    this->aa_head = aa_head;
    this->lut = lut;
    this->sut = sut;
    this->dk = dk;
    this->chartCalibration = chartCalibration;
    this->dispense = dispense;

    ocImageProvider_1 = new ImageProvider();
    sfrImageProvider = new ImageProvider();
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
    runFlowchartTest();
    //performAAOffline();
    //performOC(true, true);
    qInfo("End");
}

bool AACore::runFlowchartTest()
{
    qInfo("aaAutoTest Started");
    QVariantMap jsonMap = flowchartDocument.object().toVariantMap();
    QJsonObject links = jsonMap["links"].toJsonObject();
    QJsonObject operators = jsonMap["operators"].toJsonObject();
    bool end = false;
    QString currentPointer = "start";
    while (!end)
    {
       end = true;
       foreach(const QString &key, links.keys()){
           QJsonValue value = links.value(key);
           if (value["fromOperator"].toString() == currentPointer
            && value["fromConnector"].toString() == "success") {
               if (currentPointer == "start") {
                   qInfo(QString("Move from " + currentPointer + " to " + value["toOperator"].toString()).toStdString().c_str());
                   currentPointer = value["toOperator"].toString();
                   if (links.size() == 1) {
                       QJsonValue op = operators[currentPointer.toStdString().c_str()];
                       performTest(currentPointer.toStdString().c_str(), op["properties"]);
                   }
                   end = false;
                   break;
               }
               else {
                   qInfo(QString("Do Test:" + currentPointer).toStdString().c_str());
                   QJsonValue op = operators[currentPointer.toStdString().c_str()];\
                   //Choose Path base on the result
                   ErrorCodeStruct ret_error = performTest(currentPointer.toStdString().c_str(), op["properties"]);
                   bool ret = true;
                   if (ret_error.code != ErrorCode::OK) ret = false;
                   if (ret) {
                       currentPointer = value["toOperator"].toString();
                   } else {
                       //Find fail path
                       bool hasFailPath = false;
                       foreach(const QString &key, links.keys()){
                            QJsonValue value = links.value(key);
                            if (value["fromOperator"].toString() == currentPointer
                             && value["fromConnector"].toString() == "fail") {
                                currentPointer = value["toOperator"].toString();
                                hasFailPath = true;
                                break;
                            }
                       }
                       if (!hasFailPath) {
                           //qInfo() << "Missing fail path, will put to reject test item";
                           // qInfo() << "Reject! -> To Reject Tray";
                           //qInfo() << "End of graph";
                           end = true;
                           break;
                       }
                   }
                   if (currentPointer.contains("Accept")) {
                      // qInfo() << "Accept! -> To Good Tray";
                      // qInfo() << "End of graph";
                       end = true;
                       break;
                   } else if (currentPointer.contains("Reject")) {
                       //material_state.SetLensNg(true);
                       //material_state.SetCmosNg(true);
                       //qInfo() << "Reject! -> To Reject Tray";
                       //qInfo() << "End of graph";
                       end = true;
                       break;
                   }
                   end = false;
                   break;
               }
           } else if ( value["fromOperator"].toString() == currentPointer
                       && value["fromConnector"].toString() == "thread_1" ) {
               qInfo("Found Parallel Test Item");
               vector<QString> thread_1_test_list, thread_2_test_list;
               QString current_thread_1 = currentPointer, current_thread_2 = currentPointer;
               //Find the head first
               bool isFoundThread_1 = false, isFoundThread_2 = false;
               foreach(const QString &key, links.keys()){
                    QJsonValue value = links.value(key);
                    if (value["fromOperator"].toString() == current_thread_1 && value["fromConnector"] == "thread_1") {
                        current_thread_1 = value["toOperator"].toString();
                        isFoundThread_1 = true;
                    } else if (value["fromOperator"].toString() == current_thread_2 && value["fromConnector"] == "thread_2") {
                        current_thread_2 = value["toOperator"].toString();
                        isFoundThread_2 = true;
                    }
                    if (isFoundThread_1 && isFoundThread_2) {
                        thread_1_test_list.push_back(current_thread_1);
                        thread_2_test_list.push_back(current_thread_2);
                        break;
                    }
               }
               //Traverse the thread
               bool thread1End = false;
               while (!thread1End)
               {
                   thread1End = true;
                   foreach(const QString &key, links.keys()){
                       QJsonValue value = links.value(key);
                       if (value["fromOperator"].toString() == current_thread_1) {
                           thread_1_test_list.push_back(value["toOperator"].toString());
                           current_thread_1 = value["toOperator"].toString();
                           thread1End = false;
                           break;
                       }
                   }
                   if (current_thread_1.contains("Join")) {
                       thread1End = true;
                   }
               }
               bool thread2End = false;
               while (!thread2End)
               {
                   thread2End = true;
                   foreach(const QString &key, links.keys()){
                       QJsonValue value = links.value(key);
                       if (value["fromOperator"].toString() == current_thread_2) {
                           thread_2_test_list.push_back(value["toOperator"].toString());
                           current_thread_2 = value["toOperator"].toString();
                           thread2End = false;
                           if (current_thread_2.contains("Join")) {
                               currentPointer = current_thread_2;
                               thread2End = true;
                           }
                           break;
                       }
                   }
               }
               qInfo("End of traversal");
               //performParallelTest(thread_1_test_list, thread_2_test_list);
               //Perform Parallel Test
           }
       }
    }
    return true;
}

ErrorCodeStruct AACore::performTest(QString testItemName, QJsonValue properties)
{
    ErrorCodeStruct ret = { ErrorCode::OK, "" };
    QString testName = properties["title"].toString();
    QJsonValue params = properties["params"];
    int retry_count = params["retry"].toInt(0);
    QJsonValue delay_in_ms_qjv = params["delay_in_ms"];
    unsigned int delay_in_ms = delay_in_ms_qjv.toInt(0);
    for (int i = 0; i <= retry_count; i++) {
        if (testName.contains(AA_PIECE_START)) { qInfo("Performing Start"); }
        else if (testItemName.contains(AA_PIECE_LOAD_CAMERA)) {
            qInfo("Performing load camera");
        }
        else if (testItemName.contains(AA_PIECE_INIT_CAMERA)) {
            qInfo("Performing init camera");
        }
        else if (testItemName.contains(AA_PIECE_PR_TO_BOND)) {
            qInfo("Performing PR To Bond");
            ret = performPRToBond();
        }
        else if (testItemName.contains(AA_PIECE_INITIAL_TILT)) {
            qInfo("Performing Initial Tilt");
            //double initial_roll = params["roll"].toDouble();
            //double initial_pitch = params["pitch"].toDouble();
        }
        else if (testItemName.contains(AA_PIECE_Z_OFFSET)) {
            qInfo("Performaing Z Offset");
            //int type = params["type"].toInt(0);
            double z_offset_in_um = params["z_offset_in_um"].toDouble(0);
            performZOffset(z_offset_in_um);
        }
        else if (testItemName.contains(AA_PIECE_PICK_LENS)) {
            qInfo("Performing AA pick lens");

        }
        else if (testItemName.contains(AA_PIECE_UNLOAD_LENS)) {
            qInfo("Performing AA unload lens");
        }
        else if (testItemName.contains(AA_UNLOAD_CAMERA)) {
            qInfo("AA Unload Camera");
        }
        else if (testItemName.contains(AA_PIECE_OC)) {
            qInfo("Performing OC");
            bool enable_motion = params["enable_motion"].toInt();
            bool fast_mode = params["fast_mode"].toInt();
            performOC(enable_motion, fast_mode);
            qInfo("End of perform OC");
        }
        else if (testItemName.contains(AA_PIECE_AA)) {
            qInfo("Performing AA");
            int mode = params["mode"].toInt();
            double start_pos = params["start_pos"].toDouble();
            double stop_pos = params["stop_pos"].toDouble();
            double offset_in_um = params["offset_in_um"].toDouble()/1000;
            double step_size = params["step_size"].toDouble()/1000;
            int delay_z_in_ms = params["delay_Z_in_ms"].toInt();
            int wait_tilt = params["wait_tilt"].toInt();
            int edge_filter_mode = params["edge_filter"].toInt();
            int is_debug = params["is_debug"].toInt();
            double estimated_aa_fov = params["estimated_aa_fov"].toDouble();
            double estimated_fov_slope = params["estimated_fov_slope"].toDouble();
            sfr::EdgeFilter edgeFilter = sfr::EdgeFilter::NO_FILTER;
            if (edge_filter_mode == 1) {
                edgeFilter = sfr::EdgeFilter::VERTICAL_ONLY;
            } else if (edge_filter_mode == 2) {
                edgeFilter = sfr::EdgeFilter::HORIZONTAL_ONLY;
            }
            ret = performAA(start_pos, stop_pos, step_size, true, delay_z_in_ms, wait_tilt, mode,
                            estimated_aa_fov, is_debug, edgeFilter, estimated_fov_slope, offset_in_um);
            qInfo("End of perform AA");
        }
        else if (testItemName.contains(AA_PIECE_MTF)) {
            qInfo("Performing MTF");
            ret = performMTF();
            qInfo("End of perform MTF");
        }
        else if (testItemName.contains(AA_PIECE_UV)) {
            qInfo("Performing UV");
        }
        else if (testItemName.contains(AA_PIECE_DISPENSE)) {
            qInfo("Performing Dispense");
            int enable_save_image = params["enable_save_image"].toInt();
            int lighting = params["lighting"].toInt();
        }
        else if (testItemName.contains(AA_PIECE_DELAY)) {
            qInfo("Performing Delay");
            performDelay(delay_in_ms);
        }
        else if (testItemName.contains(AA_PIECE_ACCEPT))
        {
            qInfo("Performing Accept");
        }
        else if (testItemName.contains(AA_PIECE_REJECT))
        {
            qInfo("Performing Reject");
        }
        else if (testItemName.contains(AA_PIECE_JOIN))
        {
            qInfo("Performing Join");
        }
        else if (testItemName.contains(AA_PIECE_SAVE_IMAGE))
        {
            qInfo("Performing Save Image");
            int cameraChannel = params["type"].toInt();
            int lighting = params["lighting"].toInt();
        }
    }
    return ret;
}

ErrorCodeStruct AACore::performDispense()
{
    sut->recordCurrentPos();
    if(!dispense->performDispense()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "dispense fail"};}
    if(!sut->movetoRecordPos()){return  ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "sut move to record pos fail"};}
    return ErrorCodeStruct {ErrorCode::OK, ""};
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
    int timeout=1000;
    while(this->clustered_sfr_map.size() != sfrCount && timeout >0) {
        Sleep(10);
        timeout--;
    }
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
    sfrFitCurve_Advance(imageWidth, imageHeight, xTilt, yTilt, zPeak, ul_zPeak, ur_zPeak, ll_zPeak, lr_zPeak);
    if (isnan(xTilt) || isnan(yTilt)) {
        qInfo("if (isnan(xTilt) || isnan(yTilt)) failed");
        return;
    }
    clustered_sfr_map.clear();
    qInfo("[performAAOffline] Finished xTilt: %f yTilt: %f zPeak: %f ulPeak: %f", xTilt, yTilt, zPeak, ul_zPeak);
    qInfo("time elapsed: %d", timer.elapsed());
    //unitlog->addVariantMap("AA", map);
}

ErrorCodeStruct AACore::performPRToBond()
{
    if (!this->aa_head->moveToMushroomPosition()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA cannot move to mushroom Pos"};}
    if (!this->lut->moveToUnloadPos()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "LUT cannot move to unload Pos"};}
    if (!this->sut->moveToMushroomPos()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "SUT cannot move to mushroom Pos"};}
    return ErrorCodeStruct {ErrorCode::OK, ""};
}

ErrorCodeStruct AACore::performAAPickLens()
{
    if (!this->aa_head->moveToMushroomPosition()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA cannot move to mushroom Pos"};}
    if(!this->lut->moveToAA1PickLens()){return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "LUT AA1 ccan not move to pick lens"};}
    if(!this->lut->moveToLoadPos()){return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "LUT ccan not move to lut load pos"};}

    return ErrorCodeStruct {ErrorCode::OK, ""};
}

ErrorCodeStruct AACore::performAA(double start, double stop, double step_size,
                                   bool enableMotion, int zSleepInMs, bool isWaitTiltMotion,
                                   int zScanMode, double estimated_aa_fov,
                                   bool is_debug, sfr::EdgeFilter edgeFilter,
                                   double estimated_fov_slope, double zOffset)
{
    qInfo("start: %f stop: %f step_size: %f", start, stop, step_size);
    int imageWidth = 0, imageHeight = 0;
    double xTilt, yTilt, zPeak, ul_zPeak, ur_zPeak, ll_zPeak, lr_zPeak;
    double corner_deviation = 0;
    unsigned int zScanCount = 0;
    double xsum=0,x2sum=0,ysum=0,xysum=0;
    vector<cv::Mat> images;
    sut->moveToZPos(start);
    mPoint3D start_pos = sut->carrier->GetFeedBackPos();
    int count = 0;
    if (zScanMode == ZSCAN_MODE::AA_ZSCAN_NORMAL) {
        count = (int)fabs((start - stop)/step_size);
        for (int i = 0; i < count; i++)
        {
           sut->moveToZPos(start+(i*step_size));
           msleep(zSleepInMs);
           double realZ = sut->carrier->GetFeedBackPos().Z;
           qInfo("Z scan start from %f to %f, real: %f", start_pos.Z, realZ);
           cv::Mat img = dk->DothinkeyGrabImageCV(0);
           imageWidth = img.cols; imageHeight = img.rows;
           QString imageName;
           imageName.append(getGrabberLogDir())
                           .append(getCurrentTimeString())
                           .append(".jpg");
           //cv::imwrite(imageName.toStdString().c_str(), img);
           double dfov = calculateDFOV(img);
           xsum=xsum+realZ;
           ysum=ysum+dfov;
           x2sum=x2sum+pow(realZ,2);
           xysum=xysum+realZ*dfov;
           qInfo("fov: %f  syt_z: %f", dfov,sut->carrier->GetFeedBackPos().Z);
           imageWidth = img.cols;
           imageHeight = img.rows;
           images.push_back(std::move(img));
           zScanCount++;
           emit sfrWorkerController->calculate(i, start+i*step_size, images[i], false, sfr::EdgeFilter::NO_FILTER);
       }
    }
    qInfo("End of zscan motion");
    int timeout=1000;
    while(this->clustered_sfr_map.size() != zScanCount && timeout >0) {
        Sleep(10);
        timeout--;
    }
    if (timeout <= 0) {
        qInfo("Error in performing AA Offline: %d", timeout);
        return ErrorCodeStruct{ ErrorCode::GENERIC_ERROR, ""};
    }

    double fov_slope     = (zScanCount*xysum-xsum*ysum)/(zScanCount*x2sum-xsum*xsum);       //calculate slope
    double fov_intercept = (x2sum*ysum-xsum*xysum)/(x2sum*zScanCount-xsum*xsum);            //calculate intercept
    qInfo("fov_slope: %f fov_intercept: %f", fov_slope, fov_intercept);

    sfrFitCurve_Advance(imageWidth, imageHeight, xTilt, yTilt, zPeak, ul_zPeak, ur_zPeak, ll_zPeak, lr_zPeak);
    clustered_sfr_map.clear();
    qInfo("xTilt: %f yTilt: %f zPeak: %f", xTilt, yTilt, zPeak);
    sut->stepMove_Z_Sync(-0.2);
    msleep(zSleepInMs);
    aa_head->stepInterpolation_AB_Sync(xTilt,yTilt);
    msleep(zSleepInMs);
    performOC(true, true);
    msleep(zSleepInMs);
    sut->moveToZPos(zPeak);
    return ErrorCodeStruct{ ErrorCode::OK, ""};
}

ErrorCodeStruct AACore::performOC(bool enableMotion, bool fastMode)
{
    ErrorCodeStruct ret = { ErrorCode::OK, "" };
    QVariantMap map;
    QVariantMap stepTimerMap;
    QElapsedTimer timer, stepTimer;
    timer.start(); stepTimer.start();
    /*
    this->lightingController->SetBrightness(LIGHTING_UPLOOK, 0);
    this->lightingController->SetBrightness(LIGHTING_DOWNLOOK, 0);*/
    cv::Mat img = dk->DothinkeyGrabImageCV(0);
    QString imageName;
    imageName.append(getGrabberLogDir())
                    .append(getCurrentTimeString())
                    .append(".jpg");
    cv::imwrite(imageName.toStdString().c_str(), img);
    //cv::Mat img = cv::imread("C:\\Users\\emil\\Desktop\\Test\\Samsung\\debug\\debug\\zscan_10.bmp");
    QImage outImage;
    double offsetX, offsetY;
    unsigned int ccIndex = 10000, ulIndex = 0, urIndex = 0, lrIndex = 0, llIndex = 0;
    int method = 1;  //1: Pattern ; else : Mass center
    if (method == 1)
    {
        std::vector<AA_Helper::patternAttr> vector = search_mtf_pattern(img, outImage, false,
                                                                        ccIndex, ulIndex, urIndex,
                                                                        llIndex, lrIndex);
        stepTimerMap.insert("search_pattern", stepTimer.elapsed());
        stepTimer.restart();
        ocImageProvider_1->setImage(outImage);
        emit callQmlRefeshImg();
        if( vector.size()<1 || ccIndex > 9 ) return ErrorCodeStruct { ErrorCode::GENERIC_ERROR, "Cannot find enough pattern" };
        offsetX = vector[ccIndex].center.x() - (vector[ccIndex].width/2);
        offsetY = vector[ccIndex].center.y() - (vector[ccIndex].height/2);
        qInfo("OC OffsetX: %f %f", offsetX, offsetY);
    } else {
        QImage outImage; QPointF center;
        if (!AA_Helper::calculateOC(img, center, outImage)) return ErrorCodeStruct { ErrorCode::GENERIC_ERROR, "Cannot calculate OC"};
        ocImageProvider_1->setImage(outImage);
        emit callQmlRefeshImg();
        offsetX = center.x() - img.cols/2;
        offsetY = center.y() - img.rows/2;
    }
    if (enableMotion)
    {
        QPointF x_ratio = chartCalibration->getOneXPxielDistance();
        qInfo("x pixel Ratio: %f %f ", x_ratio.x(), x_ratio.y());
        QPointF y_ratio = chartCalibration->getOneYPxielDistance();
        qInfo("y pixel Ratio: %f %f ", y_ratio.x(), y_ratio.y());
        double stepX = offsetX * x_ratio.x() + offsetY * y_ratio.x();
        double stepY = offsetX * x_ratio.y() + offsetY * y_ratio.y();
        qInfo("xy step: %f %f ", stepX, stepY);
        if(abs(stepX)>0.5||abs(stepY)>0.5)
        {
            qInfo("OC result too big (x:%f,y:%f) pixel：(%f,%f) cmosPixelToMM (x:)%f,%f) ",stepY,stepY,offsetX,offsetY,x_ratio.x(),x_ratio.y());
            return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "OC step too large" };
        }
        this->sut->stepMove_XY_Sync(-stepX, -stepY);
    }
    return ret;
}

ErrorCodeStruct AACore::performMTF()
{
    cv::Mat img = cv::imread("C:\\Users\\emil\\Desktop\\Test\\Samsung\\debug\\debug\\zscan_6.bmp");
    //cv::Mat img = dk->DothinkeyGrabImageCV(0);
    int imageWidth = img.cols;
    int imageHeight = img.rows;
    double fov = this->calculateDFOV(img);
    if (fov == -1) {
        qInfo("Error in calculating fov");
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
    emit sfrWorkerController->calculate(0, 0, img, true);
    int timeout=1000;
    while(this->clustered_sfr_map.size() != 1 && timeout >0) {
        Sleep(10);
        timeout--;
    }
    if (timeout <= 0) {
        qInfo("Error in performing MTF: %d", timeout);

        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
    vector<Sfr_entry> sfr_entry = clustered_sfr_map.at(0);
    double cc_min_d = 999999, ul_min_d = 999999, ur_min_d = 999999, lr_min_d = 999999, ll_min_d = 999999;
    unsigned int ccROIIndex = 0 , ulROIIndex = 0, urROIIndex = 0, llROIIndex = 0, lrROIIndex = 0;
    for (unsigned int i = 0; i < sfr_entry.size(); i++)
    {
        double cc_d = sqrt(pow(sfr_entry.at(i).px - imageWidth/2, 2) + pow(sfr_entry.at(i).py - imageHeight/2, 2));
        double ul_d = sqrt(pow(sfr_entry.at(i).px, 2) + pow(sfr_entry.at(i).py, 2));
        double ur_d = sqrt(pow(sfr_entry.at(i).px - imageWidth, 2) + pow(sfr_entry.at(i).py, 2));
        double ll_d = sqrt(pow(sfr_entry.at(i).px, 2) + pow(sfr_entry.at(i).py - imageHeight, 2));
        double lr_d = sqrt(pow(sfr_entry.at(i).px - imageWidth, 2) + pow(sfr_entry.at(i).py - imageHeight, 2));
        if (cc_d < cc_min_d) {
              cc_min_d = cc_d;
              ccROIIndex = i;
        }
        if (ul_d < ul_min_d) {
             ul_min_d = ul_d;
             ulROIIndex = i;
        }
        if (ur_d < ur_min_d) {
             ur_min_d = ur_d;
             urROIIndex = i;
        }
        if (ll_d < ll_min_d) {
             ll_min_d = ll_d;
             llROIIndex = i;
        }
        if (lr_d < lr_min_d) {
            lr_min_d = lr_d;
            lrROIIndex = i;
        }
    }
    std::vector<double> sfr_v;
    sfr_v.push_back(sfr_entry[ccROIIndex].sfr);
    sfr_v.push_back(sfr_entry[urROIIndex].sfr);
    sfr_v.push_back(sfr_entry[ulROIIndex].sfr);
    sfr_v.push_back(sfr_entry[lrROIIndex].sfr);
    sfr_v.push_back(sfr_entry[llROIIndex].sfr);
    std::sort(sfr_v.begin(), sfr_v.end());
    clustered_sfr_map.clear();
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACore::performDelay(int delay_in_ms)
{
    QVariantMap map;
    Sleep(delay_in_ms);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACore::performZOffset(double zOffset)
{
    sut->stepMove_Z_Sync(zOffset);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

void AACore::sfrImageReady(QImage img)
{
    sfrImageProvider->setImage(img);
    emit callQmlRefeshSfrImg();
}

double AACore::calculateDFOV(cv::Mat img)
{
    QImage outImage;
    unsigned int ccIndex = 0, ulIndex = 0, urIndex = 0, lrIndex = 0, llIndex = 0;
    std::vector<AA_Helper::patternAttr> vector = search_mtf_pattern(img, outImage, true,
                                                                    ccIndex, ulIndex, urIndex,
                                                                    llIndex, lrIndex);
    if (vector.size() >= 5)
    {
        double d1 = sqrt(pow((vector[ulIndex].center.x() - vector[lrIndex].center.x()), 2) + pow((vector[ulIndex].center.y() - vector[lrIndex].center.y()), 2));
        double d2 = sqrt(pow((vector[urIndex].center.x() - vector[llIndex].center.x()), 2) + pow((vector[urIndex].center.y() - vector[llIndex].center.y()), 2));
        double f = 1.98;
        double dfov1 = 2*atan(d1/(2*this->chartCalibration->getOneXPxielDistance().x()*f))*180/PI;
        double dfov2 = 2*atan(d2/(2*this->chartCalibration->getOneXPxielDistance().y()*cmosPixelToMM_Y*f))*180/PI;
        double dfov = (dfov1 + dfov2)/2;
        return dfov;
    }
    return -1;
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

void AACore::sfrFitCurve_Advance(double imageWidth, double imageHeight, double &xTilt, double &yTilt,
                                  double &zPeak, double &ul_zPeak, double &ur_zPeak, double &ll_zPeak, double &lr_zPeak)
{
    std::vector<aa_util::aaCurve> aaCurves;
    std::vector<std::vector<Sfr_entry>> clustered_sfr;

    for (unsigned int i = 0; i < clustered_sfr_map.size(); ++i) {
        if (clustered_sfr_map.find(i) != clustered_sfr_map.end()) {
            clustered_sfr.push_back(std::move(clustered_sfr_map[i]));
        }
    }
    clustered_sfr = sfr_curve_analysis(clustered_sfr);
    vector<threeDPoint> points;
    int principle_center_x = imageWidth/2, principle_center_y = imageHeight/2;
    double cc_peak_z = 0, ul_peak_z = 0, ur_peak_z = 0, ll_peak_z = 0, lr_peak_z = 0;
    int cc_curve_index = 0;
    double g_x_min = 99999;
    double g_x_max = -99999;

    //sfrFitAllCurves(clustered_sfr, aaCurves, points, g_x_min, g_x_max, cc_peak_z, cc_curve_index, principle_center_x, principle_center_y, cmosPixelToMM, cmosPixelToMM);
    sfrFitAllCurves(clustered_sfr, aaCurves, points, g_x_min, g_x_max, cc_peak_z, cc_curve_index, principle_center_x, principle_center_y, 892, 892);
    //ToDo: Check whether the points result is determinitics
    if (points.size() == 5) {
       ul_peak_z = points[0].z;
       ur_peak_z = points[1].z;
       ll_peak_z = points[3].z;
       lr_peak_z = points[4].z;
    }
    threeDPoint weighted_vector = planeFitting(points);

    unsigned int ccIndex = 0, ulIndex = 0, urIndex = 0, llIndex = 0, lrIndex = 0;
    AA_Helper::AA_Find_Charactertistics_Pattern(clustered_sfr, imageWidth, imageHeight,
                                                ccIndex, ulIndex, urIndex, llIndex, lrIndex);
    for(unsigned i = 0; i < clustered_sfr.size(); i++) {
        if (i == ccIndex || i == ulIndex || i == urIndex || i == llIndex || i == lrIndex)
        {
            QString indexString;
            QVariantMap sfrMap;
            if (i == ccIndex) indexString = "CC";
            else if (i == ulIndex) indexString = "UL";
            else if (i == urIndex) indexString = "UR";
            else if (i == llIndex) indexString = "LL";
            else if (i == lrIndex) indexString = "LR";
            for (unsigned int j = 0; j < clustered_sfr[i].size(); j++) {
                QVariantMap s;
                s.insert("index", j);
                s.insert("position", indexString);
                s.insert("px", clustered_sfr[i][j].px);
                s.insert("py", clustered_sfr[i][j].py);
                s.insert("area", clustered_sfr[i][j].area);
                s.insert("sfr", clustered_sfr[i][j].sfr);
                sfrMap.insert(indexString, s);
                sfrMap.insert("pz", clustered_sfr[i][j].pz);
                //unitlog->postSfrDataToELK(sfrMap);
            }
        }
    }
    xTilt = weighted_vector.z * weighted_vector.x;
    yTilt = weighted_vector.z * weighted_vector.y;
    double corner_deviation = AA_Helper::calculateAACornerDeviation(ul_peak_z, ur_peak_z, ll_peak_z, lr_peak_z);
    qInfo("corner_deviation: %f", corner_deviation);
    if (currentChartDisplayChannel == 0) {
        currentChartDisplayChannel = 1;
        aaData_1.clear();
        aaData_1.setDev(round(corner_deviation*1000)/1000);
        aaData_1.setWCCPeakZ(round(cc_peak_z*1000));
        aaData_1.setWULPeakZ(round(ul_peak_z*1000));
        aaData_1.setWURPeakZ(round(ur_peak_z*1000));
        aaData_1.setWLLPeakZ(round(ll_peak_z*1000));
        aaData_1.setWLRPeakZ(round(lr_peak_z*1000));

        for (unsigned int i = 0; i < clustered_sfr.size(); i++)
        {
            for (unsigned int j = 0; j < clustered_sfr.at(i).size(); j++) {
                if (i == ccIndex) this->aaData_1.addData(0, clustered_sfr.at(i).at(j).pz*100000, clustered_sfr.at(i).at(j).sfr);
                else if (i == ulIndex) this->aaData_1.addData(1, clustered_sfr.at(i).at(j).pz*100000, clustered_sfr.at(i).at(j).sfr);
                else if (i == urIndex) this->aaData_1.addData(2, clustered_sfr.at(i).at(j).pz*100000, clustered_sfr.at(i).at(j).sfr);
                else if (i == llIndex) this->aaData_1.addData(3, clustered_sfr.at(i).at(j).pz*100000, clustered_sfr.at(i).at(j).sfr);
                else if (i == lrIndex) this->aaData_1.addData(4, clustered_sfr.at(i).at(j).pz*100000, clustered_sfr.at(i).at(j).sfr);
            }
        }
        aaData_1.plot();
    } else {
        currentChartDisplayChannel = 0;
        aaData_2.clear();
        aaData_2.setDev(round(corner_deviation*1000)/1000);
        aaData_2.setWCCPeakZ(round(cc_peak_z*1000));
        aaData_2.setWULPeakZ(round(ul_peak_z*1000));
        aaData_2.setWURPeakZ(round(ur_peak_z*1000));
        aaData_2.setWLLPeakZ(round(ll_peak_z*1000));
        aaData_2.setWLRPeakZ(round(lr_peak_z*1000));
        for (unsigned int i = 0; i < clustered_sfr.size(); i++)
        {
            for (unsigned int j = 0; j < clustered_sfr.at(i).size(); j++) {
                if (i == ccIndex) this->aaData_2.addData(0, clustered_sfr.at(i).at(j).pz*100000, clustered_sfr.at(i).at(j).sfr);
                else if (i == ulIndex) this->aaData_2.addData(1, clustered_sfr.at(i).at(j).pz*100000, clustered_sfr.at(i).at(j).sfr);
                else if (i == urIndex) this->aaData_2.addData(2, clustered_sfr.at(i).at(j).pz*100000, clustered_sfr.at(i).at(j).sfr);
                else if (i == llIndex) this->aaData_2.addData(3, clustered_sfr.at(i).at(j).pz*100000, clustered_sfr.at(i).at(j).sfr);
                else if (i == lrIndex) this->aaData_2.addData(4, clustered_sfr.at(i).at(j).pz*100000, clustered_sfr.at(i).at(j).sfr);
            }
        }
        aaData_2.plot();
    }
    zPeak = cc_peak_z;
    ul_zPeak = ul_peak_z;
    ur_zPeak = ur_peak_z;
    ll_zPeak = ll_peak_z;
    lr_zPeak = lr_peak_z;

}

std::vector<AA_Helper::patternAttr> AACore::search_mtf_pattern(cv::Mat inImage, QImage &image, bool isFastMode, unsigned int &ccROIIndex, unsigned int &ulROIIndex, unsigned int &urROIIndex, unsigned int &llROIIndex, unsigned int &lrROIIndex)
{
    int max_intensity = 50;
    int min_area = 10000; int max_area = 90000;

    return AA_Helper::AA_Search_MTF_Pattern(inImage, image, isFastMode, ccROIIndex, ulROIIndex, urROIIndex, llROIIndex, lrROIIndex, max_intensity, min_area, max_area);
}
