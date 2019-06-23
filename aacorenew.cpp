#include "aacorenew.h"
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

AACoreNew::AACoreNew(QString name, QObject *parent):ThreadWorkerBase (name)
{
}

void AACoreNew::Init(AAHeadModule *aa_head, LutClient *lut, SutModule *sut, Dothinkey *dk, ChartCalibration *chartCalibration,
                     DispenseModule *dispense, ImageGrabbingWorkerThread *imageThread, Unitlog *unitlog)
{
    this->aa_head = aa_head;
    this->lut = lut;
    this->dk = dk;
    this->chartCalibration = chartCalibration;
    this->dispense = dispense;
    this->imageThread = imageThread;
    this->sut = sut;
    this->unitlog = unitlog;
    ocImageProvider_1 = new ImageProvider();
    sfrImageProvider = new ImageProvider();
    connect(this, &AACoreNew::sfrResultsReady, this, &AACoreNew::storeSfrResults, Qt::DirectConnection);
    connect(this, &AACoreNew::sfrResultsDetectFinished, this, &AACoreNew::stopZScan, Qt::DirectConnection);
}

void AACoreNew::loadJsonConfig(QString file_name)
{
    QMap<QString, PropertyBase*> temp_map;
    temp_map.insert("AA_CORE_PARAMS", &parameters);
    PropertyBase::loadJsonConfig(file_name, temp_map);
}

void AACoreNew::saveJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("AA_CORE_PARAMS", &this->parameters);
    PropertyBase::saveJsonConfig(file_name,temp_map);
}

void AACoreNew::setSfrWorkerController(SfrWorkerController * sfrWorkerController){
    this->sfrWorkerController = sfrWorkerController;
}

void AACoreNew::run(bool has_material)
{
    qInfo("Start AACore Thread");
    is_run = true;
    while(is_run) {
        qInfo("AACore is running");
        runningUnit = this->unitlog->createUnit();
        runFlowchartTest();
        emit postDataToELK(this->runningUnit);
        QThread::msleep(100);
    }
    qInfo("End of thread");
}

void AACoreNew::LogicNg(int &ng_time)
{
    if(has_product)
    {
        has_ng_product = true;
        has_product = false;
        return;
    }
    if(parameters.firstRejectSensor())
    {
        if(ng_time >= parameters.rejectTimes())
        {
            ng_time = 0;
            has_ng_lens = true;
            has_lens = false;
        }
        else
        {
            has_ng_sensor = true;
            has_sensor = false;
            ng_time++;
        }
    }
    else
    {
        if(ng_time >= parameters.rejectTimes())
        {
            ng_time = 0;
            has_ng_sensor = true;
            has_sensor = false;
        }
        else
        {
            has_ng_lens = true;
            has_lens = false;
            ng_time++;
        }
    }
}

void AACoreNew::NgLens()
{
    has_lens = false;
    has_ng_lens = true;
    if(parameters.firstRejectSensor())
    {
        current_aa_ng_time = 0;
        current_oc_ng_time = 0;
        current_mtf_ng_time = 0;
    }
}

void AACoreNew::NgSensor()
{
    has_sensor = false;
    has_ng_sensor = true;
    if(!parameters.firstRejectSensor())
    {
        current_aa_ng_time = 0;
        current_oc_ng_time = 0;
        current_mtf_ng_time = 0;
    }

}

void AACoreNew::NgProduct()
{
    has_product = false;
    has_ng_product = true;
}
void AACoreNew::startWork( int run_mode)
{
    if (run_mode == RunMode::Normal) run(true);
    else if (run_mode == RunMode::NoMaterial) {
        run(false);
    } else if (run_mode == RunMode::VibrationTest) {
        is_run = true;
        mtf_log.clear();
        loopTestResult = "";
        loopTestResult.append("CC, UL,UR,LL,LR,\n");
        while (is_run) {
            performMTF(true,true);
            QThread::msleep(200);
        }
        writeFile(loopTestResult, MTF_DEBUG_DIR, "mtf_loop_test.csv");
    } else if (run_mode == RunMode::AAFlowChartTest) {
        runningUnit = this->unitlog->createUnit();
        runFlowchartTest();
        emit postDataToELK(this->runningUnit);
    }
    else if(run_mode == RunMode::OnllyLeftAA&&aa_head->parameters.moduleName().contains("1"))
    {
        run(true);
    }
    else if(run_mode == RunMode::OnlyRightAA&&aa_head->parameters.moduleName().contains("2"))
    {
        run(true);
    }
}

void AACoreNew::stopWork(bool wait_finish)
{
    qInfo("AACorenew stop work");
    is_run = false;
    return;
}

void AACoreNew::performHandlingOperation(int cmd)
{
    qInfo("AACore perform command: %d", cmd);
    if(cmd == 1)
    {
        performDispense();
    }
    else if(cmd == 2)
    {
        has_sensor = true;
        has_lens = true;
        performPRToBond();
    }
    return;
}

void AACoreNew::resetLogic()
{
    if(is_run)return;
    has_product = false;
    has_ng_product = false;
    has_ng_lens = false;
    has_ng_sensor = false;
    has_sensor = false;
    has_lens = false;
    aa_head->receive_sensor = false;
    aa_head->waiting_sensor = false;
    aa_head->receive_lens = false;
    aa_head->waiting_lens = false;
    current_aa_ng_time = 0;
    current_oc_ng_time = 0;
    current_mtf_ng_time = 0;
}

bool AACoreNew::runFlowchartTest()
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
                       ErrorCodeStruct ret_error = performTest(currentPointer.toStdString().c_str(), op["properties"]);
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
                           performReject();
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
                       qInfo("Performing Reject");
                       performReject();
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

ErrorCodeStruct AACoreNew::performTest(QString testItemName, QJsonValue properties)
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
            ret = performInitSensor();
            qInfo("End of init camera");
        }
        else if (testItemName.contains(AA_PIECE_PR_TO_BOND)) {
            qInfo("Performing PR To Bond");
            ret = performPRToBond();
            qInfo("End of perform PR To Bond");
        }
        else if (testItemName.contains(AA_PIECE_INITIAL_TILT)) {
            qInfo("Performing Initial Tilt");
            double initial_roll = params["roll"].toDouble();
            double initial_pitch = params["pitch"].toDouble();
            aa_head->stepInterpolation_AB_Sync(initial_roll, initial_pitch);
            qInfo("End of perform initial tilt");
        }
        else if (testItemName.contains(AA_PIECE_Z_OFFSET)) {
            qInfo("Performing Z Offset");
            double z_offset_in_um = params["z_offset_in_um"].toDouble(0);
            z_offset_in_um /= 1000;
            performZOffset(z_offset_in_um);
            qInfo("End of perform z offset");
        }
        else if (testItemName.contains(AA_PIECE_XY_OFFSET)) {
            qInfo("Performing XY Offset");
            double x_offset_in_um = params["x_offset_in_um"].toDouble(0);
            double y_offset_in_um = params["y_offset_in_um"].toDouble(0);
            x_offset_in_um /= 1000;
            y_offset_in_um /= 1000;
            performXYOffset(x_offset_in_um, y_offset_in_um);
            qInfo("End of perform xy offset");
        }
        else if (testItemName.contains(AA_PIECE_PICK_LENS)) {
            qInfo("Performing AA pick lens");
            ret = performAAPickLens();
            qInfo("End of perform AA pick lens");
        }
        else if (testItemName.contains(AA_PIECE_UNLOAD_LENS)) {
            qInfo("Performing AA unload lens");
        }
        else if (testItemName.contains(AA_UNLOAD_CAMERA)) {
            qInfo("AA Unload Camera");
            ret = performCameraUnload();
            qInfo("End of perform unload camera");
        }
        else if (testItemName.contains(AA_PIECE_OC)) {
            qInfo("Performing OC");
            bool enable_motion = params["enable_motion"].toInt();
            bool fast_mode = params["fast_mode"].toInt();
            ret = performOC(enable_motion, fast_mode);
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
            int no_tilt = params["no_tilt"].toInt();
            sfr::EdgeFilter edgeFilter = sfr::EdgeFilter::NO_FILTER;
            if (edge_filter_mode == 1) {
                edgeFilter = sfr::EdgeFilter::VERTICAL_ONLY;
            } else if (edge_filter_mode == 2) {
                edgeFilter = sfr::EdgeFilter::HORIZONTAL_ONLY;
            }
            ret = performAA(start_pos, stop_pos, step_size, true, delay_z_in_ms, wait_tilt, mode,
                            estimated_aa_fov, is_debug, edgeFilter, estimated_fov_slope, offset_in_um,no_tilt);
            qInfo("End of perform AA");
        }
        else if (testItemName.contains(AA_PIECE_MTF)) {
            qInfo("Performing MTF");
            ret = performMTF(params);
            qInfo("End of perform MTF");
        }
        else if (testItemName.contains(AA_PIECE_UV)) {
            qInfo("Performing UV");
            //int uv_time_input = params["delay_in_ms"].toInt();
            int uv_time = 3000;
            performUV(uv_time);
            qInfo("End of perform UV");
        }
        else if (testItemName.contains(AA_PIECE_DISPENSE)) {
            qInfo("Performing Dispense");
            int enable_save_image = params["enable_save_image"].toInt();
            int lighting = params["lighting"].toInt();
            ret = performDispense();
        }
        else if (testItemName.contains(AA_PIECE_DELAY)) {
            int delay_in_ms = params["delay_in_ms"].toInt();
            qInfo("Performing Delay : %d", delay_in_ms);
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

ErrorCodeStruct AACoreNew::performDispense()
{
    qInfo("Performing Dispense");
    has_product = true;
    has_lens = false;
    has_sensor = false;
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    sut->recordCurrentPos();
    PrOffset offset;
    dispense->setMapPosition(sut->downlook_position.X(),sut->downlook_position.Y());
    if(!sut->moveToDownlookPR(offset)){ NgProduct(); return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "downlook pr fail"};}
    dispense->setPRPosition(offset.X,offset.Y,offset.Theta);
    if(!dispense->performDispense()) { NgProduct(); return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "dispense fail"};}
    sut->moveToDownlookPR(offset); // For save image only
    if(!sut->movetoRecordPos()){NgProduct(); return  ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "sut move to record pos fail"};}
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "Dispense", map);
    qInfo("Finish Dispense");
    return ErrorCodeStruct {ErrorCode::OK, ""};
}


ErrorCodeStruct AACoreNew::performAA(double start, double stop, double step_size,
                                   bool enableMotion, int zSleepInMs, bool isWaitTiltMotion,
                                   int zScanMode, double estimated_aa_fov,
                                   bool is_debug, sfr::EdgeFilter edgeFilter,
                                   double estimated_fov_slope, double zOffset,int no_tilt)
{
    qInfo("Performing AA");
    QVariantMap map, dfovMap;
    QElapsedTimer timer; timer.start();
    qInfo("start: %f stop: %f step_size: %f", start, stop, step_size);
    int imageWidth = 0, imageHeight = 0;
    double xTilt, yTilt, zPeak, ul_zPeak, ur_zPeak, ll_zPeak, lr_zPeak;
    unsigned int zScanCount = 0;
    double xsum=0,x2sum=0,ysum=0,xysum=0;
    vector<cv::Mat> images;
    if (start > 0) sut->moveToZPos(start);
    mPoint3D start_pos = sut->carrier->GetFeedBackPos();
    int count = 0;
    QPointF prev_point = {0, 0};
    double prev_fov_slope = 0;
    if (zScanMode == ZSCAN_MODE::AA_ZSCAN_NORMAL) {
        if (start <= 0) {
            start = start_pos.Z;
            count = 6;
        } else {
            count = (int)fabs((start - stop)/step_size);
        }
        for (int i = 0; i < count; i++)
        {
           sut->moveToZPos(start+(i*step_size));
           QThread::msleep(zSleepInMs);
           double realZ = sut->carrier->GetFeedBackPos().Z;
           qInfo("Z scan start from %f, real: %f", start+(i*step_size), realZ);
           cv::Mat img = dk->DothinkeyGrabImageCV(0);
           imageWidth = img.cols; imageHeight = img.rows;
           QString imageName;
           imageName.append(getGrabberLogDir())
                           .append(getCurrentTimeString())
                           .append(".jpg");
           //cv::imwrite(imageName.toStdString().c_str(), img);
           double dfov = calculateDFOV(img);
           dfovMap.insert(QString::number(i), dfov);
           xsum=xsum+realZ;
           ysum=ysum+dfov;
           x2sum=x2sum+pow(realZ,2);
           xysum=xysum+realZ*dfov;
           qInfo("fov: %f  sut_z: %f", dfov,sut->carrier->GetFeedBackPos().Z);
           if(current_dfov.contains(QString::number(i)))
               current_dfov[QString::number(i)] = dfov;
           else
               current_dfov.insert(QString::number(i),dfov);
           imageWidth = img.cols;
           imageHeight = img.rows;
           images.push_back(std::move(img));
           zScanCount++;
           emit sfrWorkerController->calculate(i, start+i*step_size, images[i], false, sfr::EdgeFilter::NO_FILTER);
       }
    }else {
        isZScanNeedToStop = false;
        QThread::msleep(zSleepInMs);
        cv::Mat img = dk->DothinkeyGrabImageCV(0);
        double dfov = calculateDFOV(img);
        dfovMap.insert("-1", dfov);
        if (dfov <= -1) {
            qInfo("Cannot find the target FOV!");
            LogicNg(current_aa_ng_time);
            return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
        }
        double estimated_aa_z = (estimated_aa_fov - dfov)/estimated_fov_slope + start;
        double target_z = estimated_aa_z + zOffset;
        qInfo("The estimated target z is: %f dfov is%f", target_z, dfov);
        if (target_z >= stop) {
            qInfo("The estimated target is too large. value: %f", target_z);
            LogicNg(current_aa_ng_time);
            return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};;
        }
        if (target_z <= start-1) {
            qInfo("The estimated target is too small. value: %f", target_z);
            LogicNg(current_aa_ng_time);
            return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
        }
        sut->moveToZPos(target_z);
        for (unsigned int i = 0; i < 10; i++) {
            if (isZScanNeedToStop) {
                qInfo("z scan detected finished");
                break;
            }
            sut->moveToZPos(target_z+(i*step_size));
            QThread::msleep(zSleepInMs);
            mPoint3D currPos = sut->carrier->GetFeedBackPos();
            cv::Mat img = dk->DothinkeyGrabImageCV(0);
            double dfov = calculateDFOV(img);
            if(current_dfov.contains(QString::number(i)))
                current_dfov[QString::number(i)] = dfov;
            else
                current_dfov.insert(QString::number(i),dfov);
            dfovMap.insert(QString::number(i), dfov);
            bool isCrashDetected = false;
            if (i > 1) {
                double slope = (dfov - prev_point.y()) / (currPos.Z - prev_point.x());
                double error = 0;
                if (prev_fov_slope != 0) {
                    error = (slope - prev_fov_slope) / prev_fov_slope;
                }
                if (fabs(error) > 0.2) {
                    qInfo("Crash detection is triggered");
                    isCrashDetected = true;
                }
                qInfo("current slope %f  prev_slope %f error %f", slope, prev_fov_slope, error);
                prev_fov_slope = slope;
            }
            prev_point.setX(currPos.Z); prev_point.setY(dfov);

            if (dfov <= -1) {
                qInfo("Cannot find the target FOV!");
                LogicNg(current_aa_ng_time);
                return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
            }
            xsum=xsum+currPos.Z;                        //calculate sigma(xi)
            ysum=ysum+dfov;                             //calculate sigma(yi)
            x2sum=x2sum+pow(currPos.Z,2);               //calculate sigma(x^2i)
            xysum=xysum+currPos.Z*dfov;                 //calculate sigma(xi*yi)
            imageWidth = img.cols;
            imageHeight = img.rows;
            images.push_back(std::move(img));
            zScanCount++;
            emit sfrWorkerController->calculate(i, currPos.Z, images[i], false, sfr::EdgeFilter::NO_FILTER);
            if (isCrashDetected) {
                qInfo("Total zCount: %d", zScanCount);
                break;
            }
        }
    }
    int timeout=1000;
    while(this->clustered_sfr_map.size() != zScanCount && timeout >0) {
        Sleep(10);
        timeout--;
    }
    if (timeout <= 0) {
        qInfo("Error in performing AA: %d", timeout);
        clustered_sfr_map.clear();
        LogicNg(current_aa_ng_time);
        return ErrorCodeStruct{ ErrorCode::GENERIC_ERROR, ""};
    }

    if (zScanCount < 6) {
        qInfo("Error in performing AA due to insufficient.");
        clustered_sfr_map.clear();
        LogicNg(current_aa_ng_time);
        return ErrorCodeStruct{ ErrorCode::GENERIC_ERROR, ""};
    }

    double fov_slope     = (zScanCount*xysum-xsum*ysum)/(zScanCount*x2sum-xsum*xsum);       //calculate slope
    double fov_intercept = (x2sum*ysum-xsum*xysum)/(x2sum*zScanCount-xsum*xsum);            //calculate intercept
    qInfo("fov_slope: %f fov_intercept: %f", fov_slope, fov_intercept);
    double dev = 0;
    sfrFitCurve_Advance(imageWidth, imageHeight, xTilt, yTilt, zPeak, ul_zPeak, ur_zPeak, ll_zPeak, lr_zPeak, dev);
    clustered_sfr_map.clear();
    qInfo("xTilt: %f yTilt: %f zPeak: %f", xTilt, yTilt, zPeak);
//    QThread::msleep(zSleepInMs);
    qInfo("aa_head before: %f", aa_head->GetFeedBack().Z);
    //aa_head->stepInterpolation_AB_Sync(xTilt,yTilt);
    //aa_head->stepInterpolation_AB_Sync(-yTilt,xTilt);
    if(no_tilt == 0)
        aa_head->stepInterpolation_AB_Sync(-yTilt, xTilt);

    qInfo("aa_head after :%f", aa_head->GetFeedBack().Z);
    sut->moveToZPos(zPeak);
    map.insert("X_TILT", xTilt);
    map.insert("Y_TILT", yTilt);
    map.insert("Z_PEAK_CC", zPeak);
    map.insert("Z_PEAK_UL", ul_zPeak);
    map.insert("Z_PEAK_UR", ur_zPeak);
    map.insert("Z_PEAK_LL", ll_zPeak);
    map.insert("Z_PEAK_LR", lr_zPeak);
    map.insert("FOV_SLOPE", fov_slope);
    map.insert("FOV_INTERCEPT", fov_intercept);
    map.insert("DEV", dev);
    map.insert("timeElapsed", timer.elapsed());
    map.insert("DFOV", dfovMap);
    emit pushDataToUnit(runningUnit, "AA", map);

    qInfo("Finish AA");
    return ErrorCodeStruct{ ErrorCode::OK, ""};
}

void AACoreNew::performAAOffline()
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
    double dev = 0;
    sfrFitCurve_Advance(imageWidth, imageHeight, xTilt, yTilt, zPeak, ul_zPeak, ur_zPeak, ll_zPeak, lr_zPeak, dev);
    if (isnan(xTilt) || isnan(yTilt)) {
        qInfo("if (isnan(xTilt) || isnan(yTilt)) failed");
        return;
    }
    clustered_sfr_map.clear();
    qInfo("[performAAOffline] Finished xTilt: %f yTilt: %f zPeak: %f ulPeak: %f", xTilt, yTilt, zPeak, ul_zPeak);
    qInfo("time elapsed: %d", timer.elapsed());
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "AAOffline", map);
}

void AACoreNew::performHandling(int cmd)
{
    emit sendHandlingOperation(cmd);
}

void AACoreNew::sfrFitCurve_Advance(double imageWidth, double imageHeight, double &xTilt, double &yTilt,
                                  double &zPeak, double &ul_zPeak, double &ur_zPeak, double &ll_zPeak, double &lr_zPeak, double &dev)
{
    std::vector<aa_util::aaCurve> aaCurves;
    std::vector<std::vector<Sfr_entry>> clustered_sfr;
    QVariantMap map;
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
    qInfo("X Ratio: %d Y Ratio: %d PCX : %d PCY : %d", parameters.SensorXRatio(), parameters.SensorYRatio(), principle_center_x, principle_center_y);
    sfrFitAllCurves(clustered_sfr, aaCurves, points, g_x_min, g_x_max, cc_peak_z, cc_curve_index, principle_center_x, principle_center_y, parameters.SensorXRatio(), parameters.SensorYRatio());
    double cc_min_d = 999999, ul_min_d = 999999, ur_min_d = 999999, lr_min_d = 999999, ll_min_d = 999999;
    unsigned int ccROIIndex = 0, ulROIIndex = 0, urROIIndex = 0, llROIIndex = 0, lrROIIndex = 0;
    if (points.size() == 5) {
        for (unsigned int i = 0; i < 5; i++) {
            double cc_d = sqrt(pow(points[i].x - imageWidth/2, 2) + pow(points[i].y - imageHeight/2, 2));
            double ul_d = sqrt(pow(points[i].x, 2) + pow(points[i].y, 2));
            double ur_d = sqrt(pow(points[i].x - imageWidth, 2) + pow(points[i].y, 2));
            double ll_d = sqrt(pow(points[i].x, 2) + pow(points[i].y - imageHeight, 2));
            double lr_d = sqrt(pow(points[i].x - imageWidth, 2) + pow(points[i].y - imageHeight, 2));
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
    } else { qInfo("Insufficient curve point"); return; }
    ul_peak_z = points[ulROIIndex].z;
    ur_peak_z = points[urROIIndex].z;
    ll_peak_z = points[llROIIndex].z;
    lr_peak_z = points[lrROIIndex].z;
    for (int i = 0; i < 5; i++) {
        points[i].x /= parameters.SensorXRatio(); points[i].y /= parameters.SensorYRatio();
    }
    threeDPoint weighted_vector = planeFitting(points);

    unsigned int ccIndex = 0, ulIndex = 0, urIndex = 0, llIndex = 0, lrIndex = 0;
    AA_Helper::AA_Find_Charactertistics_Pattern(clustered_sfr, imageWidth, imageHeight,
                                                ccIndex, ulIndex, urIndex, llIndex, lrIndex);
    map.insert("SensorID",dk->readSensorID());
    for(unsigned i = 0; i < clustered_sfr.size(); i++) {
        QVariantMap sfrMap;
        if (i == ccIndex || i == ulIndex || i == urIndex || i == llIndex || i == lrIndex)
        {
            QString indexString;
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
                s.insert("dfov",current_dfov[QString::number(j)]);
                s.insert("t_sfr", clustered_sfr[i][j].t_sfr);
                s.insert("b_sfr", clustered_sfr[i][j].b_sfr);
                s.insert("l_sfr", clustered_sfr[i][j].l_sfr);
                s.insert("r_sfr", clustered_sfr[i][j].r_sfr);

                s.insert("pz", clustered_sfr[i][j].pz);
                sfrMap.insert(QString::number(j), s);
//                emit postSfrDataToELK(runningUnit, sfrMap);
            }
            map.insert(indexString, sfrMap);
        }
    }
    xTilt = weighted_vector.z * weighted_vector.x;
    yTilt = weighted_vector.z * weighted_vector.y;
    double corner_deviation = AA_Helper::calculateAACornerDeviation(ul_peak_z, ur_peak_z, ll_peak_z, lr_peak_z);
    dev = corner_deviation;
    map.insert("ul_peak", ul_peak_z);
    map.insert("ur_peak", ur_peak_z);
    map.insert("ll_peak", ll_peak_z);
    map.insert("lr_peak", lr_peak_z);
    map.insert("cc_peak", cc_peak_z);
    map.insert("xTilt", xTilt);
    map.insert("yTilt", yTilt);
    map.insert("dev", dev);
    emit postSfrDataToELK(runningUnit, map);
    if (currentChartDisplayChannel == 0) {
        currentChartDisplayChannel = 1;
        aaData_1.clear();
        aaData_1.setDev(round(corner_deviation*1000)/1000);
        aaData_1.setWCCPeakZ(round(cc_peak_z*1000));
        aaData_1.setWULPeakZ(round(ul_peak_z*1000));
        aaData_1.setWURPeakZ(round(ur_peak_z*1000));
        aaData_1.setWLLPeakZ(round(ll_peak_z*1000));
        aaData_1.setWLRPeakZ(round(lr_peak_z*1000));
        aaData_1.setXTilt(round(xTilt*10000)/10000);
        aaData_1.setYTilt(round(yTilt*10000)/10000);

        for (unsigned int i = 0; i < clustered_sfr.size(); i++)
        {
            for (unsigned int j = 0; j < clustered_sfr.at(i).size(); j++) {
                if (i == ccIndex) this->aaData_1.addData(0, clustered_sfr.at(i).at(j).pz*1000, clustered_sfr.at(i).at(j).sfr);
                else if (i == ulIndex) this->aaData_1.addData(1, clustered_sfr.at(i).at(j).pz*1000, clustered_sfr.at(i).at(j).sfr);
                else if (i == urIndex) this->aaData_1.addData(2, clustered_sfr.at(i).at(j).pz*1000, clustered_sfr.at(i).at(j).sfr);
                else if (i == llIndex) this->aaData_1.addData(3, clustered_sfr.at(i).at(j).pz*1000, clustered_sfr.at(i).at(j).sfr);
                else if (i == lrIndex) this->aaData_1.addData(4, clustered_sfr.at(i).at(j).pz*1000, clustered_sfr.at(i).at(j).sfr);
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
        aaData_2.setXTilt(round(xTilt*10000)/10000);
        aaData_2.setYTilt(round(yTilt*10000)/10000);
        for (unsigned int i = 0; i < clustered_sfr.size(); i++)
        {
            for (unsigned int j = 0; j < clustered_sfr.at(i).size(); j++) {
                if (i == ccIndex) this->aaData_2.addData(0, clustered_sfr.at(i).at(j).pz*1000, clustered_sfr.at(i).at(j).sfr);
                else if (i == ulIndex) this->aaData_2.addData(1, clustered_sfr.at(i).at(j).pz*1000, clustered_sfr.at(i).at(j).sfr);
                else if (i == urIndex) this->aaData_2.addData(2, clustered_sfr.at(i).at(j).pz*1000, clustered_sfr.at(i).at(j).sfr);
                else if (i == llIndex) this->aaData_2.addData(3, clustered_sfr.at(i).at(j).pz*1000, clustered_sfr.at(i).at(j).sfr);
                else if (i == lrIndex) this->aaData_2.addData(4, clustered_sfr.at(i).at(j).pz*1000, clustered_sfr.at(i).at(j).sfr);
            }
        }
        aaData_2.plot();
    }
    zPeak = cc_peak_z;
    ul_zPeak = ul_peak_z;
    ur_zPeak = ur_peak_z;
    ll_zPeak = ll_peak_z;
    lr_zPeak = lr_peak_z;
    qInfo("End of sfrFitCurve");
}

ErrorCodeStruct AACoreNew::performMTF(QJsonValue params, bool write_log)
{
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    //cv::Mat img = cv::imread("C:\\Users\\emil\\Desktop\\Test\\Samsung\\debug\\debug\\zscan_6.bmp");
    cv::Mat img = dk->DothinkeyGrabImageCV(0);
    int imageWidth = img.cols;
    int imageHeight = img.rows;
    double fov = this->calculateDFOV(img);
    map.insert("DFOV", fov);
    if (fov == -1) {
        qInfo("Error in calculating fov");
        LogicNg(current_mtf_ng_time);
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    } else {
        qInfo("DFOV :%f", fov);
    }
    emit sfrWorkerController->calculate(0, 0, img, true);
    int timeout=1000;
    while(this->clustered_sfr_map.size() != 1 && timeout >0) {
        Sleep(10);
        timeout--;
    }
    if (timeout <= 0) {
        qInfo("Error in performing MTF: %d", timeout);
        LogicNg(current_mtf_ng_time);
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

    double cc_min_sfr = params["CC"].toDouble(-1);
    double ul_min_sfr = params["UL"].toDouble(-1);
    double ur_min_sfr = params["UR"].toDouble(-1);
    double ll_min_sfr = params["LL"].toDouble(-1);
    double lr_min_sfr = params["LR"].toDouble(-1);
    double sfr_dev_tol = params["SFR_DEV_TOL"].toDouble(100);

    map.insert("CC_CHECK", cc_min_sfr);
    map.insert("UR_CHECK", ul_min_sfr);
    map.insert("UL_CHECK", ur_min_sfr);
    map.insert("LR_CHECK", lr_min_sfr);
    map.insert("LL_CHECK", ll_min_sfr);
    map.insert("SFR_MAX_TOL", sfr_dev_tol);

    bool sfr_check = true;
    if (sfr_entry[ccROIIndex].t_sfr < cc_min_sfr ||
        sfr_entry[ccROIIndex].r_sfr < cc_min_sfr ||
        sfr_entry[ccROIIndex].b_sfr < cc_min_sfr ||
        sfr_entry[ccROIIndex].l_sfr < cc_min_sfr) {
       qInfo("cc cannot pass");
       sfr_check = false;
    }
    if (sfr_entry[urROIIndex].t_sfr < ur_min_sfr ||
        sfr_entry[urROIIndex].r_sfr < ur_min_sfr ||
        sfr_entry[urROIIndex].b_sfr < ur_min_sfr ||
        sfr_entry[urROIIndex].l_sfr < ur_min_sfr) {
       qInfo("ur cannot pass");
       sfr_check = false;
    }
    if (sfr_entry[ulROIIndex].t_sfr < ul_min_sfr ||
        sfr_entry[ulROIIndex].r_sfr < ul_min_sfr ||
        sfr_entry[ulROIIndex].b_sfr < ul_min_sfr ||
        sfr_entry[ulROIIndex].l_sfr < ul_min_sfr) {
       qInfo("ul cannot pass");
       sfr_check = false;
    }
    if (sfr_entry[lrROIIndex].t_sfr < lr_min_sfr ||
        sfr_entry[lrROIIndex].r_sfr < lr_min_sfr ||
        sfr_entry[lrROIIndex].b_sfr < lr_min_sfr ||
        sfr_entry[lrROIIndex].l_sfr < lr_min_sfr) {
       qInfo("lr cannot pass");
       sfr_check = false;
    }
    if (sfr_entry[llROIIndex].t_sfr < ll_min_sfr ||
        sfr_entry[llROIIndex].r_sfr < ll_min_sfr ||
        sfr_entry[llROIIndex].b_sfr < ll_min_sfr ||
        sfr_entry[llROIIndex].l_sfr < ll_min_sfr) {
       qInfo("ll cannot pass");
       sfr_check = false;
    }

    std::vector<double> sfr_v;
    for (int i = 0; i < 4; i++) {
        unsigned int index = 0;
        if (i == 0) index = urROIIndex;
        if (i == 1) index = ulROIIndex;
        if (i == 2) index = lrROIIndex;
        if (i == 3) index = llROIIndex;
        sfr_v.push_back(sfr_entry[index].sfr);
        sfr_v.push_back(sfr_entry[index].sfr);
        sfr_v.push_back(sfr_entry[index].sfr);
        sfr_v.push_back(sfr_entry[index].sfr);
        sfr_v.push_back(sfr_entry[index].sfr);
    }

    std::sort(sfr_v.begin(), sfr_v.end());
    double max_sfr_deviation = fabs(sfr_v[0] - sfr_v[sfr_v.size()-1]);
    if (max_sfr_deviation >= sfr_dev_tol) {
        qInfo("max_sfr_deviation cannot pass");
        sfr_check = false;
    }

    qInfo("Read the aahead and sut carrier feedback");
    mPoint6D motorsPosition = this->aa_head->GetFeedBack();
    mPoint3D sutPosition = this->sut->carrier->GetFeedBackPos();
    qInfo("inset data to map ccROIIndex %d urROIIndex %d ulROIInde %dx lrROIIndex %d llROIIndex %d size %d",ccROIIndex,urROIIndex,ulROIIndex,lrROIIndex,llROIIndex,sfr_entry.size());
    map.insert("AA_X", motorsPosition.X);
    map.insert("AA_Y", motorsPosition.Y);
    map.insert("AA_Z", motorsPosition.Z);
    map.insert("AA_A", motorsPosition.A);
    map.insert("AA_B", motorsPosition.B);
    map.insert("AA_C", motorsPosition.C);
    map.insert("AA_A", motorsPosition.A);
    map.insert("AA_B", motorsPosition.B);
    map.insert("AA_C", motorsPosition.C);
    map.insert("SUT_X", sutPosition.X);
    map.insert("SUT_Y", sutPosition.Y);
    map.insert("SUT_Z", sutPosition.Z);
    map.insert("OC_X", sfr_entry[ccROIIndex].px - imageWidth/2);
    map.insert("OC_Y", sfr_entry[ccROIIndex].py - imageHeight/2);

    map.insert("CC_SFR", sfr_entry[ccROIIndex].sfr);
    map.insert("CC_SFR_1", sfr_entry[ccROIIndex].t_sfr);
    map.insert("CC_SFR_2", sfr_entry[ccROIIndex].r_sfr);
    map.insert("CC_SFR_3", sfr_entry[ccROIIndex].b_sfr);
    map.insert("CC_SFR_4", sfr_entry[ccROIIndex].l_sfr);

    map.insert("UR_SFR", sfr_entry[urROIIndex].sfr);
    map.insert("UR_SFR_1", sfr_entry[urROIIndex].t_sfr);
    map.insert("UR_SFR_2", sfr_entry[urROIIndex].r_sfr);
    map.insert("UR_SFR_3", sfr_entry[urROIIndex].b_sfr);
    map.insert("UR_SFR_4", sfr_entry[urROIIndex].l_sfr);

    map.insert("UL_SFR", sfr_entry[ulROIIndex].sfr);
    map.insert("UL_SFR_1", sfr_entry[ulROIIndex].t_sfr);
    map.insert("UL_SFR_2", sfr_entry[ulROIIndex].r_sfr);
    map.insert("UL_SFR_3", sfr_entry[ulROIIndex].b_sfr);
    map.insert("UL_SFR_4", sfr_entry[ulROIIndex].l_sfr);

    map.insert("LR_SFR", sfr_entry[lrROIIndex].sfr);
    map.insert("LR_SFR_1", sfr_entry[lrROIIndex].t_sfr);
    map.insert("LR_SFR_2", sfr_entry[lrROIIndex].r_sfr);
    map.insert("LR_SFR_3", sfr_entry[lrROIIndex].b_sfr);
    map.insert("LR_SFR_4", sfr_entry[lrROIIndex].l_sfr);

    map.insert("LL_SFR", sfr_entry[llROIIndex].sfr);
    map.insert("LL_SFR_1", sfr_entry[llROIIndex].t_sfr);
    map.insert("LL_SFR_2", sfr_entry[llROIIndex].r_sfr);
    map.insert("LL_SFR_3", sfr_entry[llROIIndex].b_sfr);
    map.insert("LL_SFR_4", sfr_entry[llROIIndex].l_sfr);

    map.insert("Sensor_ID", dk->readSensorID());
    map.insert("SFR_CHECK", sfr_check);
    map.insert("DFOV", fov);
    map.insert("timeElapsed", timer.elapsed());
    qInfo("CC_X :%f CC_Y: %f", sfr_entry[ccROIIndex].px, sfr_entry[ccROIIndex].py);
    clustered_sfr_map.clear();
    emit pushDataToUnit(this->runningUnit, "MTF", map);
    if (write_log) {
        this->loopTestResult.append(QString::number(sfr_entry[ccROIIndex].sfr))
                            .append(",")
                            .append(QString::number(sfr_entry[ulROIIndex].sfr))
                            .append(",")
                            .append(QString::number(sfr_entry[urROIIndex].sfr))
                            .append(",")
                            .append(QString::number(sfr_entry[llROIIndex].sfr))
                            .append(",")
                            .append(QString::number(sfr_entry[lrROIIndex].sfr))
                            .append(",\n");
        this->mtf_log.incrementData(sfr_entry[ccROIIndex].sfr, sfr_entry[ulROIIndex].sfr, sfr_entry[urROIIndex].sfr, sfr_entry[llROIIndex].sfr,sfr_entry[lrROIIndex].sfr);
    }
    qInfo("MTF done");
    if (sfr_check) {
        return ErrorCodeStruct{ErrorCode::OK, ""};
    } else {
        LogicNg(current_mtf_ng_time);
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
}

ErrorCodeStruct AACoreNew::performUV(int uv_time)
{
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    aa_head->openUVTillTime(uv_time);
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "UV", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performReject()
{
    QVariantMap map;
    imageThread->stop();
    Sleep(100);
    imageThread->exit();
    dk->DothinkeyClose();
//    if(!has_product)
//    {
//        if(has_lens)
//            has_ng_lens = true;
//        if(has_sensor)
//            has_ng_sensor = true;
//    }
//    has_sensor = false;
//    has_lens = false;
//    map.insert("has_ng_lens", has_product);
//    map.insert("has_ng_sensor", has_ng_sensor);
//    map.insert("has_product", has_product);
//    map.insert("has_sensor", has_sensor);
//    map.insert("has_lens", has_lens);
//    emit pushDataToUnit(this->runningUnit, "Reject", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performAccept()
{
    imageThread->stop();
    Sleep(100);
    imageThread->exit();
    dk->DothinkeyClose();
    current_aa_ng_time = 0;
    current_oc_ng_time = 0;
    current_mtf_ng_time = 0;
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performOC(bool enableMotion, bool fastMode)
{
    qInfo("Perform OC");
    ErrorCodeStruct ret = { ErrorCode::OK, "" };
    QVariantMap map;
    QElapsedTimer timer;
    timer.start();
    cv::Mat img = dk->DothinkeyGrabImageCV(0);
    QString imageName;
    imageName.append(getGrabberLogDir())
                    .append(getCurrentTimeString())
                    .append(".jpg");
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
        ocImageProvider_1->setImage(outImage);
        emit callQmlRefeshImg(1);
        if( vector.size()<1 || ccIndex > 9 )
        {
            NgLens();
            NgSensor();
//            LogicNg(current_oc_ng_time);
            return ErrorCodeStruct { ErrorCode::GENERIC_ERROR, "Cannot find enough pattern" };
        }
        offsetX = vector[ccIndex].center.x() - (vector[ccIndex].width/2);
        offsetY = vector[ccIndex].center.y() - (vector[ccIndex].height/2);
        qInfo("OC OffsetX: %f %f", offsetX, offsetY);
        map.insert("OC_OFFSET_X_IN_PIXEL", offsetX);
        map.insert("OC_OFFSET_Y_IN_PIXEL", offsetY);
    } else {
        QImage outImage; QPointF center;
        if (!AA_Helper::calculateOC(img, center, outImage))
        {
            NgLens();
            NgSensor();
//            LogicNg(current_oc_ng_time);
            return ErrorCodeStruct { ErrorCode::GENERIC_ERROR, "Cannot calculate OC"};
        }
        ocImageProvider_1->setImage(outImage);
        emit callQmlRefeshImg(1);
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
        map.insert("OC_OFFSET_X_IN_MM", stepX);
        map.insert("OC_OFFSET_Y_IN_MM", stepY);
        qInfo("xy step: %f %f ", stepX, stepY);
        if(abs(stepX)>0.5||abs(stepY)>0.5)
        {
            NgLens();
            NgSensor();
//            LogicNg(current_oc_ng_time);
            qInfo("OC result too big (x:%f,y:%f) pixel：(%f,%f) cmosPixelToMM (x:)%f,%f) ",stepY,stepY,offsetX,offsetY,x_ratio.x(),x_ratio.y());
            return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "OC step too large" };
        }
        this->sut->stepMove_XY_Sync(-stepX, -stepY);
    }
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "OC", map);
    qInfo("Finish OC");
    return ret;
}

ErrorCodeStruct AACoreNew::performInitSensor()
{
    if(!has_sensor) return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, ""};
    QElapsedTimer timer, stepTimer; timer.start(); stepTimer.start();
    QVariantMap map;
    const int channel = 0;
    bool res = dk->DothinkeyEnum();
    if (!res) { qCritical("Cannot find dothinkey");NgSensor();return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, ""}; }
    res = dk->DothinkeyOpen();
    map.insert("dothinkeyOpen", stepTimer.elapsed()); stepTimer.restart();
    if (!res) { qCritical("Cannot open dothinkey"); NgSensor();return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, ""}; }
    res = dk->DothinkeyLoadIniFile(channel);
    map.insert("dothinkeyLoadIniFile", stepTimer.elapsed()); stepTimer.restart();
    if (!res) { qCritical("Cannot load dothinkey ini file");NgSensor(); return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, ""}; }
    res = dk->DothinkeyStartCamera(channel);
    map.insert("dothinkeyStartCamera", stepTimer.elapsed()); stepTimer.restart();
    if (!res) { qCritical("Cannot start camera");NgSensor(); return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, ""}; }

    QString sensorID = dk->readSensorID();
    qInfo("performInitSensor sensor ID: %s", sensorID.toStdString().c_str());
    map.insert("sensorID", sensorID);
    if (!imageThread->isRunning())
        imageThread->start();
    map.insert("timeElapsed", timer.elapsed());
    map.insert("success", res);
    emit pushDataToUnit(runningUnit, "InitSensor", map);
    return ErrorCodeStruct {ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performPRToBond()
{
    if((!has_sensor)||(!has_lens)){ return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA cannot move to mushroom Pos"};}
    //if (!this->lut->moveToUnloadPos()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "LUT cannot move to unload Pos"};}
    QElapsedTimer timer, stepTimer; timer.start(); stepTimer.start();
    QVariantMap map;

    map.insert("moveToDownlookPR", stepTimer.elapsed()); stepTimer.restart();
    if (!this->aa_head->moveToMushroomPosition(true)) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA cannot move to mushroom Pos"};}
    map.insert("aa_head_moveToMushroomPosition", stepTimer.elapsed()); stepTimer.restart();

    //    double x = sut->downlook_position.X() + sut->up_downlook_offset.X() + aa_head->uplook_x + aa_head->offset_x;
    //    double y = sut->downlook_position.Y() + sut->up_downlook_offset.Y() + aa_head->uplook_y + aa_head->offset_y;
    double x = sut->mushroom_positon.X();
    double y = sut->mushroom_positon.Y();
    double z = sut->mushroom_positon.Z();
    double theta = sut->up_downlook_offset.Theta() - aa_head->uplook_theta - aa_head->offset_theta;
    qInfo("downlook_offset(%f,%f)",aa_head->offset_x,aa_head->offset_y,aa_head->offset_theta);
    qInfo("uplook_offset(%f,%f,%f)",aa_head->uplook_x,aa_head->uplook_y,aa_head->uplook_theta);
    qInfo("up_downlook_offset(%f,%f,%f)",sut->up_downlook_offset.X(),sut->up_downlook_offset.Y(),sut->up_downlook_offset.Theta());
    if (!this->aa_head->moveToSZ_XYC_Z_Sync(x,y,z,theta)) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA cannot move to PRToBond Position"};}

    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "PrToBond", map);
    return ErrorCodeStruct {ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performAAPickLens()
{
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    if (!this->sut->moveToReadyPos()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "SUT cannot move to ready Pos"};}
    if (!this->aa_head->moveToPickLensPosition()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA cannot move to picklens Pos"};}
    if(aa_head->receive_sensor)
    {
        qInfo("has wait sensor suceess");
        has_sensor = true;
        has_ng_sensor = false;
        aa_head->receive_sensor = false;
    }
    if((!has_sensor))
    {
        qInfo("need sensor has_product %d  has_ng_product %d has_ng_sensor %d",has_product,has_ng_product,has_ng_sensor);
        if(has_product)
            aa_head->sendSensrRequest(SUT_STATE::HAS_PRODUCT);
        else if(has_ng_product)
            aa_head->sendSensrRequest(SUT_STATE::HAS_NG_PRODUCT);
        else if(has_ng_sensor)
            aa_head->sendSensrRequest(SUT_STATE::HAS_NG_SENSOR);
        else
            aa_head->sendSensrRequest(SUT_STATE::NO_MATERIAL);
    }
    if(aa_head->receive_lens)
    {
        qInfo("has wait lens suceess");
        has_lens = true;
        has_ng_lens = false;
        aa_head->receive_lens = false;
    }
    if(!has_lens)
    {
        qInfo("need lens has_ng_lens %d",has_ng_lens);
        this->lut->sendLensRequest(is_run,has_ng_lens,true);
        if (aa_head->receive_lens)
        {
            qInfo("wait lens suceess");
            has_lens = true;
            has_ng_lens = false;
            aa_head->receive_lens = false;
        }
        else{
            if(is_run)
            {
                AppendError("wait lens time_out");
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
            }
            is_run = false;
            return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "Lens lens request fail"};
        }
    }
    if(!has_sensor)
    {
        qInfo("wait sensor has_product %d has_ng_sensor %d",has_product,has_ng_sensor);
        aa_head->waitForLoadSensor(is_run);
        if(aa_head->receive_sensor)
        {
            qInfo("wait sensor suceess");
            has_sensor = true;
            has_ng_sensor = false;
            aa_head->receive_sensor = false;
        }
        else
        {
            if(is_run)
            {
                AppendError("wait sensor time_out");
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
            }
            is_run = false;
            return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "sensor request fail"};
        }
    }
    has_product = false;
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "AA_Load_Material", map);
    qInfo("Done Pick Lens");
    return ErrorCodeStruct {ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performDelay(int delay_in_ms)
{
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    Sleep(delay_in_ms);
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "delay", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performCameraUnload()
{
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    aa_head->openGripper();
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "Camera_Unload", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performZOffset(double zOffset)
{
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    double curr_z = sut->carrier->GetFeedBackPos().Z;
    double target_z = sut->carrier->GetFeedBackPos().Z + zOffset;
    sut->moveToZPos(target_z);
    QThread::msleep(200);
    double final_z = sut->carrier->GetFeedBackPos().Z;
    map.insert("zOffset", zOffset);
    map.insert("ori_z_pos", curr_z);
    map.insert("final_z_pos", final_z);
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "Z_Offset", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performXYOffset(double xOffset, double yOffset)
{
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    mPoint3D ori_pos = sut->carrier->GetFeedBackPos();
    sut->stepMove_XY_Sync(xOffset, yOffset);
    QThread::msleep(200);
    mPoint3D final_pos = sut->carrier->GetFeedBackPos();
    map.insert("xOffset", xOffset);
    map.insert("yOffset", yOffset);
    map.insert("ori_x_pos", ori_pos.X);
    map.insert("ori_y_pos", ori_pos.Y);
    map.insert("final_x_pos", final_pos.X);
    map.insert("final_y_pos", final_pos.Y);
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "XY_Offset", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

std::vector<AA_Helper::patternAttr> AACoreNew::search_mtf_pattern(cv::Mat inImage, QImage &image, bool isFastMode, unsigned int &ccROIIndex, unsigned int &ulROIIndex, unsigned int &urROIIndex, unsigned int &llROIIndex, unsigned int &lrROIIndex)
{
    return AA_Helper::AA_Search_MTF_Pattern(inImage, image, isFastMode, ccROIIndex, ulROIIndex, urROIIndex, llROIIndex, lrROIIndex, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea());
}

double AACoreNew::calculateDFOV(cv::Mat img)
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
        double f = parameters.EFL();
        double dfov1 = 2*atan(d1/(2*parameters.SensorXRatio()*f))*180/PI;
        double dfov2 = 2*atan(d2/(2*parameters.SensorYRatio()*f))*180/PI;
        double dfov = (dfov1 + dfov2)/2;
        return dfov;
    }
    return -1;
}

void AACoreNew::storeSfrResults(unsigned int index, vector<Sfr_entry> sfrs, int timeElapsed)
{
    qInfo("Received sfr result from index: %d timeElapsed: %d", index, timeElapsed);
    clustered_sfr_map[index] = std::move(sfrs);
}

void AACoreNew::stopZScan()
{
    qInfo("stop z scan");
    isZScanNeedToStop = true;
}

void AACoreNew::triggerGripperOn(bool isOn)
{
    qInfo("Trigger gripper : %d", isOn);
    if (isOn) aa_head->openGripper();
    else aa_head->closeGripper();
}

void AACoreNew::sfrImageReady(QImage img)
{
    qInfo("Sfr Image Ready");
    sfrImageProvider->setImage(img);
//    QString filename = "";
//    filename.append(getMTFLogDir())
//                    .append(getCurrentTimeString())
//                    .append(".jpg");
//    img.save(filename);
    emit callQmlRefeshImg(0);
}
