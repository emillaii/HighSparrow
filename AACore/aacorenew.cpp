#include "aacorenew.h"
#include <QVariantMap>
#include <QImage>
#include <QElapsedTimer>
#include <visionavadaptor.h>
#include "Utils/config.h"
#include <QThread>
#include <stdlib.h>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QPainter>
#include "aa_util.h"
#include "Utils/commonutils.h"
#include "Vision/visionmodule.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#define PI  3.14159265
#include <math.h>

vector<double> fitCurve(const vector<double> & x, const vector<double> & y, int order, double & localMaxX, double & localMaxY) {
    size_t n = x.size();

    double minX = 999999;
    double maxX = -999999;

    for (size_t i = 0; i < n; i++) {
        minX = std::min(minX, x[i]);
        maxX = std::max(maxX, x[i]);
    }

    Eigen::MatrixXd X(n, order + 1);
    for (size_t i = 0; i < n; ++i) {
        double tmp = 1;
        for (int j = 0; j <= order; ++j) {
            X(i, j) = tmp;
            tmp *= x[i];
        }
    }
    Eigen::MatrixXd Y(n, 1);
    for (size_t i = 0; i < n; ++i) {
        Y(i, 0) = y[i];
    }
    Eigen::MatrixXd Xt(order + 1, n);
    Xt = X.transpose();

    Eigen::MatrixXd XtX(order + 1, order + 1);
    XtX = Xt * X;
    Eigen::MatrixXd XtX_inverse(order + 1, order + 1);
    XtX_inverse = XtX.inverse();

    Eigen::MatrixXd A(order + 1, 1);
    A = XtX_inverse * Xt * Y;

    Eigen::MatrixXd B(order + 1, 1);
    B = X * A;

    Eigen::MatrixXd Ans(n, 1);
    Ans = X * A;

//    qInfo("Y | Prediction | Error");
//    for (size_t i = 0; i < n; ++i) {
//       qInfo("X: %f Y: %f  Ans: %f Error: %f", X(i,0), Y(i, 0), Ans(i, 0), Y(i, 0) - Ans(i, 0));
//    }

//    double error = 0;
//    for (size_t i = 0; i < n; ++i) {
//        error += (B(i, 0) - Y(i, 0)) * (B(i, 0) - Y(i, 0));
//    }

//    qInfo("Error: %f", error);

    vector<double> ans;
    for (int i = 0; i <= order; ++i) {
        ans.push_back(A(i, 0));
        //qInfo("i: %f", A(i, 0));
    }

    double delta = (maxX - minX) / 300;
    localMaxY = -999999;
    localMaxX = minX;
    for (double z = minX; z <= maxX; z += delta)
    {
        double tmp = 1; double ey = 0;
        for (int i = 0; i <= order; ++i) {
            ey += A(i,0)*tmp;
            tmp *= z;
        }
        if (ey > localMaxY) {
            localMaxX = z;
            localMaxY = ey;
        }
    }
    qInfo("Local Maxima X : %f local maxima Y : %f", localMaxX, localMaxY);
    return ans;
}

typedef enum {
    AA_ZSCAN_NORMAL,
    AA_DFOV_MODE,
    AA_STATIONARY_SCAN_MODE
} ZSCAN_MODE;

AACoreNew * that;

AACoreNew::AACoreNew(QString name, QObject *parent):ThreadWorkerBase (name)
{
    Q_UNUSED(name)
    Q_UNUSED(parent)
    that = this;
    ocImageProvider_1 = new ImageProvider();
    sfrImageProvider = new ImageProvider();
    dispenseImageProvider = new ImageProvider();
    connect(this, &AACoreNew::sfrResultsReady, this, &AACoreNew::storeSfrResults, Qt::DirectConnection);
    connect(this, &AACoreNew::sfrResultsDetectFinished, this, &AACoreNew::stopZScan, Qt::DirectConnection);
}

void AACoreNew::Init(AAHeadModule *aa_head, SingleheadLSutModule *lsut, Dothinkey *dk, ChartCalibration *chartCalibration,
                     DispenseModule *dispense, ImageGrabbingWorkerThread *imageThread, Unitlog *unitlog, VisionModule *visionModule, SingleHeadMachineMaterialLoaderModule *materialLoader)
{
    this->aa_head = aa_head;
    this->lsut = lsut;
    this->dk = dk;
    this->chartCalibration = chartCalibration;
    this->dispense = dispense;
    this->imageThread = imageThread;
    this->unitlog = unitlog;
    this->visionModule = visionModule;
    this->materialLoader = materialLoader;
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

void AACoreNew::receiveStartAAProcessRequestResponse(int sensorIndex, int lensIndex, bool isAAPickedLens)
{
    qInfo(__FUNCTION__);
    currentSensorIndex = sensorIndex;
    currentLensIndex = lensIndex;
    this->has_sensor = true;
    this->has_ng_sensor = false;
    this->has_lens = true;
    this->has_ng_lens = false;
    this->has_product = false;
    this->has_ng_product = false;
    this->isAAHeadPickedLens = isAAPickedLens;
    start_process = true;
}

void AACoreNew::run(bool has_material)
{
    qInfo("Start AACore Thread");
    is_run = true;
    QElapsedTimer timer;timer.start();
    while(is_run) {
		timer.restart();
        if (start_process)
        {
            runningUnit = this->unitlog->createUnit();
            hasDispense = false;
            hasUV = false;
            runFlowchartTest();
            if(hasDispense && !hasUV)
            {
                qCritical("Has executed glue dispense, but not executed UV!");
                performUV(4000);
                aa_head->openGripper();
            }
            start_process = false;
            emit postDataToELK(this->runningUnit);
			QThread::msleep(100);
			double temp_time = timer.elapsed();
			temp_time/=1000;
			qInfo("cycle_time :%f",temp_time);
            emit sendAAProcessFinishSignal(has_ng_sensor, has_ng_lens, has_product, has_ng_product, currentSensorIndex, currentLensIndex);
        }
        QThread::msleep(20);
    }
    qInfo("End of thread");
}

void AACoreNew::LogicNg(int &ng_time)
{
    //  Todo 为华为AA演示临时更改
    has_ng_lens = true;
    has_lens = false;
    has_ng_sensor = true;
    has_sensor = false;
    return;


    qInfo("LogicNg ng_time:%d",ng_time);
    if(has_product)
    {
        has_ng_product = true;
        has_product = false;

        has_lens = false;
        has_ng_lens = false;
        has_sensor = false;
        has_ng_sensor = false;
        return;
    }

    has_product = false;
    has_ng_product = false;
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
    qInfo("NgLens");
    has_lens = false;
    has_ng_lens = true;
    has_product = false;
    has_ng_product = false;
    if(parameters.firstRejectSensor())
    {
        current_aa_ng_time = 0;
//        current_oc_ng_time = 0;
//        current_mtf_ng_time = 0;
    }
}

void AACoreNew::NgSensorOrProduct()
{
    if(hasDispense)
    {
        NgProduct();
    }
    else
    {
        NgSensor();
    }
}

void AACoreNew::NgSensor()
{
    qInfo("NgSensor");
    has_sensor = false;
    has_ng_sensor = true;
    has_product = false;
    has_ng_product = false;
    if(!parameters.firstRejectSensor())
    {
        current_aa_ng_time = 0;
//        current_oc_ng_time = 0;
//        current_mtf_ng_time = 0;
    }
}

bool AACoreNew::HasLens()
{
    return has_lens||has_ng_lens;
}

bool AACoreNew::HasSensorOrProduct()
{
    return has_sensor||has_ng_sensor||has_product||has_ng_product;
}

void AACoreNew::NgProduct()
{
    has_ng_product = true;
    has_product = false;

    has_lens = false;
    has_ng_lens = false;
    has_sensor = false;
    has_ng_sensor = false;

    current_aa_ng_time = 0;
}

void AACoreNew::SetLens()
{
    has_lens = true;
    has_ng_lens = false;
}

void AACoreNew::SetSensor()
{
    has_sensor = true;
    has_ng_sensor = false;
    has_product = false;
    has_ng_product = false;
}

void AACoreNew::SetProduct()
{
    has_sensor = false;
    has_ng_sensor = false;
    has_product = true;
    has_ng_product = false;
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
            performMTF(true);
            QThread::msleep(200);
        }
        writeFile(loopTestResult, MTF_DEBUG_DIR, "mtf_loop_test.csv");
    } else if (run_mode == RunMode::AAFlowChartTest) {
        QElapsedTimer timer;timer.start();
        runningUnit = this->unitlog->createUnit();
        runFlowchartTest();
        emit postDataToELK(this->runningUnit);
		double temp_time = timer.elapsed();
        temp_time/=1000;
        qInfo("circle_time :%f",temp_time);
    }
    else if(run_mode == RunMode::OnlyLeftAA&&aa_head->parameters.moduleName().contains("1"))
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
    qInfo("AACore perform command: %d parmas :%s", cmd, handlingParams.toStdString().c_str());
    QJsonDocument jsonDoc = QJsonDocument::fromJson(handlingParams.toLocal8Bit().data());
    QJsonValue params = jsonDoc.object();

    runningUnit = this->unitlog->createUnit();
    if (cmd == HandleTest::Dispense)
    {
        if(!lsut->moveToDownlookPR(sensorDownlookOffset))
        {
            qCritical("moveToDownlookPR failed!");
            return;
        }
        performDispense();
    }
    else if (cmd == HandleTest::PR_To_Bond)
    {
        SetSensor();
        SetLens();
        performPRToBond();
    }
    else if (cmd == HandleTest::MTF) {
        performMTF(params, true);
        //performMTFOffline(params);
    }
    else if (cmd == HandleTest::OC) {
        performOC(params);
    }
    else if (cmd == HandleTest::AA) {
        if (currentChartDisplayChannel == 0) {
            aaData_1.setInProgress(true);
        } else {
            aaData_2.setInProgress(true);
        }
        //performAAOffline();
        performAA(params);
        aaData_1.setInProgress(false);
        aaData_2.setInProgress(false);
    }
    else if (cmd == HandleTest::INIT_CAMERA) {
        performInitSensor();
    }
    else if (cmd == HandleTest::Y_LEVEL) {
        performYLevelTest(params);
    }
    else if (cmd == HandleTest::UV) {
        performUV(params["delay_in_ms"].toInt());
    }
    else if (cmd == HandleTest::TOF) {
        performTOF(params);
    }
    else if (cmd == HandleTest::HW_CAMERA_CAPTURE) {
        performIRCameraCapture(params);
    }
    else if (cmd == HandleTest::Motion_Move) {
        performMotionMove(params);
    }
    else if (cmd == HandleTest::Command) {
        performCommand(params);
    }
    else if (cmd == HandleTest::AA_LOAD_LENS) {
        performLoadLens();
    }
    else if (cmd == HandleTest::AA_UNLOAD_LENS) {
        performUnloadLens();
    }
    else if (cmd == HandleTest::AA_LOAD_SENSOR) {
        performLoadSensor();
    }
    else if (cmd == HandleTest::AA_UNLOAD_SENSOR) {
        performUnloadSensor();
    }
    else if (cmd == HandleTest::AA_UNLOAD_PRODUT) {
        performUnloadProduct();
    }

    handlingParams = "";
    emit postDataToELK(this->runningUnit);
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
    isAAHeadPickedLens = false;
    send_lens_request = false;
    send_sensor_request = false;
    aa_head->waiting_sensor = false;
    aa_head->waiting_lens = false;
    current_aa_ng_time = 0;
//    current_oc_ng_time = 0;
//    current_mtf_ng_time = 0;
    grr_repeat_time = 0;
    grr_change_time = 0;
    autoRunDispenseTimes = 0;
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
                   QVariantMap map;
                   map.insert("Time", getCurrentTimeString());
                   emit pushDataToUnit(runningUnit, "FlowChart_StartTime", map);    //Add a_ to make it first in map sorting

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
                   QJsonValue op = operators[currentPointer.toStdString().c_str()];
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
                           qInfo("Finished With Auto Reject");
                           performReject();
                           end = true;
                           break;
                       }
                   }
                   if (currentPointer.contains("Accept")
                           ||currentPointer.contains("Reject")
                           ||currentPointer.contains("Terminate")) {
                       QJsonValue op = operators[currentPointer.toStdString().c_str()];
                       ErrorCodeStruct ret_error = performTest(currentPointer.toStdString().c_str(), op["properties"]);
                       qInfo("Finished With %s",currentPointer.toStdString().c_str());
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
               qInfo("End of traversal: %s", jsonMap["operators"].toString().toStdString().c_str());
               QJsonValue op1 = operators[thread_1_test_list[0].toStdString().c_str()];
               QJsonValue op2 = operators[thread_2_test_list[0].toStdString().c_str()];
               ErrorCodeStruct ret = performParallelTest(thread_1_test_list, thread_2_test_list, op1["properties"], op2["properties"]);
               if (ret.code != ErrorCode::OK) {
                   qInfo("Finished With Auto Reject");
                   performReject();
                   end = true;
                   break;
               }
               //Perform Parallel Test
           }
       }
    }

    QVariantMap map;
    map.insert("Time", getCurrentTimeString());
    emit pushDataToUnit(runningUnit, "FlowChart_TerminateTime", map);   //Add a_ to make it the first one in map sorting

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
            qInfo("End of init camera %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_PR_TO_BOND)) {
            int finish_delay = params["delay_in_ms"].toInt();
            qInfo("Performing PR To Bond :%d",finish_delay);
            ret = performPRToBond();
            qInfo("End of perform PR To Bond %s",ret.errorMessage.toStdString().c_str());
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
        else if (testItemName.contains(AA_PIECE_LOAD_MATERIAL)) {
            qInfo("Performing Load Material");
            ret = performAAPickLens();
            qInfo("End of perform Load Material %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_UNLOAD_LENS)) {
            qInfo("Performing AA unload lens");
        }
        else if (testItemName.contains(AA_UNLOAD_CAMERA)) {
            qInfo("AA Unload Camera");
            ret = performCameraUnload();
            qInfo("End of perform unload camera %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_OC)) {
            ret = performOC(params);
            qInfo("End of perform OC %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_Y_LEVEL)) {
            qInfo("Performing Y Level");
            ret = performYLevelTest(params);
            qInfo("End of perform Y Level %s", ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_AA)) {
            qInfo("Performing AA");
            if (currentChartDisplayChannel == 0) {
                aaData_1.setInProgress(true);
            } else {
                aaData_2.setInProgress(true);
            }
            ret = performAA(params);
            aaData_1.setInProgress(false);
            aaData_2.setInProgress(false);
            qInfo("End of perform AA %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_MTF)) {
            qInfo("Performing MTF");
            ret = performMTF(params);
            qInfo("End of perform MTF %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_UV)) {
            qInfo("Performing UV");
            int uv_time = params["delay_in_ms"].toInt();
            ret = performUV(uv_time);
            qInfo("End of perform UV %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_DISPENSE)) {
            qInfo("Performing Dispense");
            int enable_save_image = params["enable_save_image"].toInt();
            int lighting = params["lighting"].toInt();
            ret = performDispense();
            qInfo("End of perform UV %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_DELAY)) {
            int delay_in_ms = params["delay_in_ms"].toInt();
            qInfo("Performing Delay : %d", delay_in_ms);
            performDelay(delay_in_ms);
            qInfo("End of perform Delay");
        }
        else if (testItemName.contains(AA_PIECE_ACCEPT))
        {
            qInfo("Performing Accept");
            performAccept();
            qInfo("End of perform Accept");
        }
        else if (testItemName.contains(AA_PIECE_REJECT))
        {
            qInfo("Performing Reject");
            performReject();
            qInfo("End of perform Reject");
        }
        else if (testItemName.contains(AA_PIECE_TERMINATE))
        {
            qInfo("Performing Terminate");
            performTerminate();
            qInfo("End of perform Terminate");
        }
        else if (testItemName.contains(AA_PIECE_GRR))
        {
            bool change_lens = params["change_lens"].toInt();
            bool change_sensor = params["change_sensor"].toInt();
            int repeat_time = params["repeat_time"].toInt();
            int change_time = params["change_time"].toInt();
            qInfo("Performing GRR change_lens:%d change_sensor:%d repeat_time:%d change_time:%d",change_lens,change_sensor,repeat_time,change_time);
            performGRR(change_lens,change_sensor,repeat_time,change_time);
            qInfo("End of perform GRR");
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
        else if (testItemName.contains(AA_PIECE_TOF))
        {
            qInfo("Performing TOF");
            performTOF(params);
            qInfo("End of perform TOF");
        }
        else if (testItemName.contains(AA_PIECE_HW_CAMERA_CAPTURE))
        {
            qInfo("Perform HW Camera Capture");
            performIRCameraCapture(params);
            qInfo("End of perform HW Camera Capture");
        }
        else if (testItemName.contains(AA_PIECE_COMMAND))
        {
            qInfo("Performing Command");
            performCommand(params);
            qInfo("End of perform Command");
        }
        else if (testItemName.contains(AA_PIECE_TOF_MOTION_MOVE))
        {
            qInfo("Performing TOF Motion Move");
            performMotionMove(params);
            qInfo("End of perform TOF Motion Move");
        }
    }
    return ret;
}

bool AACoreNew::performThreadTest(vector<QString> testList, QJsonValue params)
{
    ErrorCodeStruct ret = ErrorCodeStruct {ErrorCode::OK, ""};
    foreach(QString testName, testList) {
        qInfo() << "Perform Test in thread : " << testName;
        if (testName.contains(AA_PIECE_DELAY)) {
            int delay_in_ms = params["delay_in_ms"].toInt();
            that->performDelay(delay_in_ms);
        } else if (testName.contains(AA_PIECE_Y_LEVEL)) {
            ret = that->performYLevelTest(params);
        } else if (testName.contains(AA_PIECE_UV)) {
            int uv_time = params["delay_in_ms"].toInt();
            ret = that->performUV(uv_time);
        } else if (testName.contains(AA_PIECE_INIT_CAMERA)) {
            qInfo("Performing init camera");
            ret = that->performInitSensor();
            qInfo("End performInitSensor %s",ret.errorMessage.toStdString().c_str());
        } else if (testName.contains(AA_PIECE_PR_TO_BOND)) {
            qInfo("Performing PR To Bond");
            ret = that->performPRToBond();
            qInfo("End performPRToBond %s",ret.errorMessage.toStdString().c_str());
        }
    }
    if (ret.code == ErrorCode::OK)
        return true;
    else
        return false;
}

ErrorCodeStruct AACoreNew::performParallelTest(vector<QString> testList1, vector<QString> testList2, QJsonValue prop1, QJsonValue prop2)
{
    QJsonValue params1 = prop1["params"];
    QJsonValue params2 = prop2["params"];
    QFuture<bool> f1;
    QFuture<bool> f2;
    f1 = QtConcurrent::run(performThreadTest, testList1, params1);
    f2 = QtConcurrent::run(performThreadTest, testList2, params2);
    f1.waitForFinished();
    f2.waitForFinished();
    qInfo("Finish parallel test");
    bool ret1 = f1.result();
    bool ret2 = f2.result();
    bool ret = (ret1 && ret2);
    if (ret) {
        return ErrorCodeStruct {ErrorCode::OK, ""};
    } else {
        return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, ""};
    }
}

ErrorCodeStruct AACoreNew::performDispense()
{
    qInfo("Performing Dispense");
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    lsut->recordCurrentPos();
    dispense->setMapPosition(lsut->downlook_position.X(),lsut->downlook_position.Y());
    dispense->setPRPosition(sensorDownlookOffset.X,sensorDownlookOffset.Y,sensorDownlookOffset.Theta);

    lsut->moveToZPos(0);
    hasDispense = true;
    if(!dispense->performDispense()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "dispense fail"};}

    bool checkGlueRes = true;
    if(is_run)   //自动模式
    {
        autoRunDispenseTimes ++;
        if(autoRunDispenseTimes == 1)
        {
            QString imageNameAfterDispense;
            imageNameAfterDispense.append(getDispensePrLogDir())
                    .append(getCurrentTimeString())
                    .append("_")
                    .append("_after_dispense.jpg");
            lsut->moveToDownlookSaveImage(imageNameAfterDispense); // For save image only
            QImage image(imageNameAfterDispense);
            dispenseImageProvider->setImage(image);
            emit callQmlRefeshImg(3);  //Emit dispense image to QML

            auto rsp = UIOperation::getIns()->getUIResponse("Check Glue", "Please check glue!", MsgBoxIcon::Question, PassFailBtns);
            if(rsp == FailBtn)
            {
                autoRunDispenseTimes = 0;
                checkGlueRes = false;
            }
        }
        if(autoRunDispenseTimes >= parameters.FrequencyForCheckGlue())
        {
            autoRunDispenseTimes = 0;
        }
    }

    if(!lsut->movetoRecordPos()){return  ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "sut move to record pos fail"};}

    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "Dispense", map);
    qInfo("Finish Dispense");
    has_product = true;
    has_lens = false;
    has_sensor = false;
    has_ng_lens = false;
    has_ng_sensor = false;
    if(checkGlueRes)
    {
        return ErrorCodeStruct {ErrorCode::OK, ""};
    }
    else {
        LogicNg(current_aa_ng_time);
        return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "Check glue failed"};
    }
}

ErrorCodeStruct AACoreNew::performAA(QJsonValue params)
{
    QVariantMap map;
    map.insert("Result","OK");
    clustered_sfr_map.clear();
    int zScanMode = params["mode"].toInt();
    double start = params["start_pos"].toDouble();
    double stop = params["stop_pos"].toDouble();
    double step_size = params["step_size"].toDouble()/1000;
    unsigned int zSleepInMs = params["delay_Z_in_ms"].toInt();
    double estimated_aa_fov = parameters.EstimatedAAFOV();
    double estimated_fov_slope = parameters.EstimatedFOVSlope();
    double offset_in_um = params["offset_in_um"].toDouble()/1000;
    int enableTilt = params["enable_tilt"].toInt();
    int imageCount = params["image_count"].toInt();
    int position_checking = params["position_checking"].toInt();
    double xsum=0,x2sum=0,ysum=0,xysum=0;
    qInfo("start : %f stop: %f enable_tilt: %d", start, stop, enableTilt);
    unsigned int zScanCount = 0;
    QElapsedTimer timer; timer.start();
    int resize_factor = parameters.aaScanOversampling()+1;
    vector<double> fov_y; vector<double> fov_x;
    QPointF prev_point = {0, 0};
    double prev_fov_slope = 0;
    bool grabRet = true;
    int grab_time = 0;
    int step_move_time = 0;
    int sfr_wait_time = 0;
    int wait_tilt_time = 0;
    QElapsedTimer step_move_timer;
    QElapsedTimer grab_timer;
    double estimated_aa_z = 0;
    if(zScanMode == ZSCAN_MODE::AA_ZSCAN_NORMAL) {
        qInfo("z scan mode");
        unsigned int count = (int)fabs((start - stop)/step_size);
        for (unsigned int i = 0; i < count; i++)
        {
           step_move_timer.start();
           double target = start + i*step_size;
           qInfo("target: %f",target);
           this->lsut->sut_carrier->Move_Z_Sync(start+(i*step_size));
           QThread::msleep(zSleepInMs);
           qInfo("Getting feedback position from lsut");
           double realZ = lsut->sut_carrier->GetFeedBackPos().Z;
           qInfo("Z scan start from %f, real: %f", start+(i*step_size), realZ);
           grab_timer.start();
           cv::Mat img = dk->DothinkeyGrabImageCVWithAutoRetry(0, grabRet);
           qInfo("Grab time elapsed: %d", grab_timer.elapsed());
           if (!grabRet) {
               qInfo("AA Cannot grab image.");
               map["Result"] = QString("AA Cannot grab image.i:%1").arg(i);
               emit pushDataToUnit(runningUnit, "AA", map);
               NgSensorOrProduct();
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Cannot Grab Image"};
           }
           if (!blackScreenCheck(img)) {
               LogicNg(current_aa_ng_time);
               map["Result"] = "Detect black screen";
               emit pushDataToUnit(runningUnit, "AA", map);
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Detect BlackScreen"};
           }
           cv::Mat dst;
           cv::Size size(img.cols/resize_factor, img.rows/resize_factor);
           cv::resize(img, dst, size);
           QString imageName;
           imageName.append(getGrabberLogDir())
                           .append(getCurrentTimeString())
                           .append(".bmp");
//           cv::imwrite(imageName.toStdString().c_str(), img);
           double dfov = calculateDFOV(img);
           if(current_dfov.contains(QString::number(i)))
                current_dfov[QString::number(i)] = dfov;
           else
                current_dfov.insert(QString::number(i),dfov);
           qInfo("fov: %f  sut_z: %f", dfov, lsut->sut_carrier->GetFeedBackPos().Z);
           xsum=xsum+realZ;
           ysum=ysum+dfov;
           x2sum=x2sum+pow(realZ,2);
           xysum=xysum+realZ*dfov;
           zScanCount++;
           emit sfrWorkerController->calculate(i, start+i*step_size, dst, parameters.MaxIntensity(), parameters.MaxIntensity2(), parameters.MinArea(), parameters.MaxArea(), parameters.aaScanMTFFrequency()+1);
           img.release();
           dst.release();
         }
      } else if (zScanMode == ZSCAN_MODE::AA_DFOV_MODE){
           step_move_timer.start();
           lsut->sut_carrier->Move_Z_Sync(start);
           QThread::msleep(zSleepInMs);
           step_move_time += step_move_timer.elapsed();
           grab_timer.start();
           cv::Mat img = dk->DothinkeyGrabImageCVWithAutoRetry(0, grabRet);
           if (!grabRet) {
               qInfo("AA Cannot grab image.");
               map["Result"] = "AA Cannot grab image.";
               emit pushDataToUnit(runningUnit, "AA", map);
               NgSensorOrProduct();
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Cannot Grab Image"};
           }
           if (!blackScreenCheck(img)) {
               LogicNg(current_aa_ng_time);
               map["Result"] = "AA Detect black screen";
               emit pushDataToUnit(runningUnit, "AA", map);
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Detect BlackScreen"};
           }
           double dfov = calculateDFOV(img);
           if (dfov <= -1) {
               qInfo("Cannot find the target FOV!");
               LogicNg(current_aa_ng_time);
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
           }
           estimated_aa_z = (estimated_aa_fov - dfov)/estimated_fov_slope + start;
           double target_z = estimated_aa_z + offset_in_um;
           qInfo("The estimated target z is: %f dfov is %f", target_z, dfov);
           if (target_z >= stop) {
               qInfo("The estimated target is too large. value: %f", target_z);
               LogicNg(current_aa_ng_time);
               map["Result"] = QString("The estimated target is too large. value:%1 target:%2").arg(target_z).arg(stop);
               emit pushDataToUnit(runningUnit, "AA", map);
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
           }
           for (unsigned int i = 0; i < imageCount; i++) {
                step_move_timer.start();
                lsut->sut_carrier->Move_Z_Sync(target_z+(i*step_size));
                QThread::msleep(zSleepInMs);
                step_move_time += step_move_timer.elapsed();
                grab_timer.start();
                cv::Mat img = dk->DothinkeyGrabImageCVWithAutoRetry(0, grabRet);
                grab_time += grab_timer.elapsed();
                if (!grabRet) {
                    qInfo("AA Cannot grab image.");
                    NgSensorOrProduct();
                    map["Result"] = QString("AA Cannot grab image.i:%1").arg(i);
                    emit pushDataToUnit(runningUnit, "AA", map);
                    return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Cannot Grab Image"};
                }
                if (!blackScreenCheck(img)) {
                    LogicNg(current_aa_ng_time);
                    map["Result"] = QString("AA Detect BlackScreen.i:%1").arg(i);
                    emit pushDataToUnit(runningUnit, "AA", map);
                    return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Detect BlackScreen"};
                }
                double realZ = lsut->sut_carrier->GetFeedBackPos().Z;
                double dfov = calculateDFOV(img);

                if (i > 1) {
                   double slope = (dfov - prev_point.y()) / (realZ - prev_point.x());
                   double error = 0;
                   if (prev_fov_slope != 0) {
                       error = (slope - prev_fov_slope) / prev_fov_slope;
                   }
                   qInfo("current slope %f  prev_slope %f error %f", slope, prev_fov_slope, error);
                   if (fabs(error) > 0.2) {
                       qInfo("Crash detection is triggered");
//                       LogicNg(current_aa_ng_time);
//                       map["Result"] = QString("Crash detection is triggered. prev_fov_slope:%1 now_fov_slope:%2 error:%3").arg(prev_fov_slope).arg(slope).arg(error);
//                       emit pushDataToUnit(runningUnit, "AA", map);
//                       return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
                   }
                   prev_fov_slope = slope;
                }
                prev_point.setX(realZ); prev_point.setY(dfov);

                if(current_dfov.contains(QString::number(i)))
                     current_dfov[QString::number(i)] = dfov;
                else
                     current_dfov.insert(QString::number(i),dfov);
                qInfo("fov: %f  sut_z: %f", dfov, lsut->sut_carrier->GetFeedBackPos().Z);
                xsum=xsum+realZ;
                ysum=ysum+dfov;
                x2sum=x2sum+pow(realZ,2);
                xysum=xysum+realZ*dfov;
                cv::Mat dst;
                cv::Size size(img.cols/resize_factor, img.rows/resize_factor);
                cv::resize(img, dst, size);
                emit sfrWorkerController->calculate(i, realZ, dst, parameters.MaxIntensity(), parameters.MaxIntensity2(), parameters.MinArea(), parameters.MaxArea(), parameters.aaScanMTFFrequency()+1);
                img.release();
                dst.release();
                zScanCount++;
           }
    } else if (zScanMode == ZSCAN_MODE::AA_STATIONARY_SCAN_MODE){
         double currentZ = lsut->sut_carrier->GetFeedBackPos().Z;
         double target_z = currentZ + offset_in_um;
         for (unsigned int i = 0; i < imageCount; i++) {
             step_move_timer.start();
             lsut->sut_carrier->Move_Z_Sync(target_z+(i*step_size));
             QThread::msleep(zSleepInMs);
             step_move_time += step_move_timer.elapsed();
             grab_timer.start();
             cv::Mat img = dk->DothinkeyGrabImageCVWithAutoRetry(0,grabRet);
             grab_time += grab_timer.elapsed();
             if (!grabRet) {
                 qInfo("AA Cannot grab image.");
                 NgSensorOrProduct();
                 map["Result"] = QString("AA Cannot grab image.i:%1").arg(i);
                 emit pushDataToUnit(runningUnit, "AA", map);
                 return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "AA Cannot grab image"};
             }
             if (!blackScreenCheck(img)) {
                 LogicNg(current_aa_ng_time);
                 map["Result"] = QString("AA Detect BlackScreen.i:%1").arg(i);
                 emit pushDataToUnit(runningUnit, "AA", map);
                 return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Detect BlackScreen"};
             }
             double realZ = lsut->sut_carrier->GetFeedBackPos().Z;
             double dfov = calculateDFOV(img);
             if(current_dfov.contains(QString::number(i)))
                  current_dfov[QString::number(i)] = dfov;
             else
                  current_dfov.insert(QString::number(i),dfov);
             qInfo("fov: %f  sut_z: %f", dfov, lsut->sut_carrier->GetFeedBackPos().Z);
             xsum=xsum+realZ;
             ysum=ysum+dfov;
             x2sum=x2sum+pow(realZ,2);
             xysum=xysum+realZ*dfov;
             cv::Mat dst;
             cv::Size size(img.cols/resize_factor, img.rows/resize_factor);
             cv::resize(img, dst, size);
             emit sfrWorkerController->calculate(i, realZ, dst, parameters.MaxIntensity(), parameters.MaxIntensity2(), parameters.MinArea(), parameters.MaxArea(), parameters.aaScanMTFFrequency()+1);
             img.release();
             dst.release();
             zScanCount++;
         }
    }
    int timeout=1000;
    QElapsedTimer sfr_wait_timer; sfr_wait_timer.start();
    while(this->clustered_sfr_map.size() != zScanCount && timeout >0) {
          Sleep(10);
          timeout--;
    }
    sfr_wait_time += sfr_wait_timer.elapsed();
    double fov_slope     = (zScanCount*xysum-xsum*ysum)/(zScanCount*x2sum-xsum*xsum);       //calculate slope
    double fov_intercept = (x2sum*ysum-xsum*xysum)/(x2sum*zScanCount-xsum*xsum);            //calculate intercept
    current_fov_slope = fov_slope;
    qInfo("fov_slope: %f fov_intercept: %f", fov_slope, fov_intercept);

    QVariantMap aa_result = sfrFitCurve_Advance(resize_factor, start);

    qInfo("Layer 1 xTilt : %f yTilt: %f ", aa_result["xTilt_1"].toDouble(), aa_result["yTilt_1"].toDouble());
    qInfo("Layer 2 xTilt : %f yTilt: %f ", aa_result["xTilt_2"].toDouble(), aa_result["yTilt_2"].toDouble());
    qInfo("Layer 3 xTilt : %f yTilt: %f ", aa_result["xTilt_3"].toDouble(), aa_result["yTilt_3"].toDouble());
    map.insert("Mode", zScanMode);
    map.insert("START_POS", start);
    map.insert("STOP_POS", stop);
    map.insert("STEP_SIZE", step_size);
    map.insert("IMAGE_COUNT", imageCount);
    map.insert("STEP_MOVE_TIME", step_move_time);
    map.insert("GRAB_TIME", grab_time);
    map.insert("SFR_WAIT_TIME", sfr_wait_time);
    map.insert("X_TILT", round(aa_result["xTilt"].toDouble()*1000)/1000);
    map.insert("Y_TILT", round(aa_result["yTilt"].toDouble()*1000)/1000);
    map.insert("FOV_SLOPE", round(fov_slope*1000)/1000);
    map.insert("FOV_INTERCEPT", round(fov_intercept*1000)/1000);
    bool aaResult = aa_result["OK"].toBool();
    if (!aaResult) {
        LogicNg(current_aa_ng_time);
        map["Result"] = QString("sfrFitCurve_Advance fail");
        emit pushDataToUnit(runningUnit, "AA", map);
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Perform AA fail"};
    }
    step_move_timer.start();
    lsut->sut_carrier->Move_Z_Sync(aa_result["zPeak"].toDouble());
    qInfo("Expected Z Peak: %f SUT Actual Z: %f", aa_result["zPeak"].toDouble(),lsut->sut_carrier->GetFeedBackPos().Z);
    step_move_time += step_move_timer.elapsed();
    if (enableTilt == 0) {
        qInfo("Disable tilt...");
    } else {
        qInfo("Enable tilt...xTilt: %f yTilt: %f", aa_result["xTilt"].toDouble(), aa_result["yTilt"].toDouble());
        step_move_timer.start();
        //aa_head->stepInterpolation_AB_Sync(-aa_result["yTilt"].toDouble(), aa_result["xTilt"].toDouble());
        //aa_head->stepInterpolation_AB_Sync(-aa_result["xTilt"].toDouble(), -aa_result["yTilt"].toDouble());
        double tilt_a, tilt_b;
        int index;
        if(parameters.tiltRelationship() < 4)
        {
            tilt_a = aa_result["xTilt"].toDouble();
            tilt_b = aa_result["yTilt"].toDouble();
            index = parameters.tiltRelationship();
        }
        else
        {
            tilt_a = aa_result["yTilt"].toDouble();
            tilt_b = aa_result["xTilt"].toDouble();
            index = parameters.tiltRelationship() - 4;
        }
        if(index > 1)
            tilt_a = -tilt_a;
        if(parameters.tiltRelationship()%2 == 1)
            tilt_b = -tilt_b;
        qInfo("xTilt %f yTilt %f aTilt %f bTilt %f ",aa_result["xTilt"].toDouble(),aa_result["yTilt"].toDouble(),tilt_a,tilt_b);
        aa_head->stepInterpolation_AB_Sync(tilt_a, tilt_b);

        step_move_timer.start();
        wait_tilt_time += step_move_timer.elapsed();
        map.insert("WAIT_TILT_TIME", wait_tilt_time);
    }
    if (position_checking == 1){
        QThread::msleep(zSleepInMs);
        cv::Mat img = dk->DothinkeyGrabImageCVWithAutoRetry(0, grabRet);
        double beforeZ = lsut->sut_carrier->GetFeedBackPos().Z;
        double expected_fov = fov_slope*aa_result["zPeak"].toDouble() + fov_intercept;
        double dfov = calculateDFOV(img);
        double diff_z = (dfov - expected_fov)/fov_slope;
        double afterZ = lsut->sut_carrier->GetFeedBackPos().Z;
        map.insert("Final_X", lsut->sut_carrier->GetFeedBackPos().X);
        map.insert("Final_Y", lsut->sut_carrier->GetFeedBackPos().Y);
        map.insert("Final_Z", lsut->sut_carrier->GetFeedBackPos().Z);
        qInfo("before z: %f after z: %f now fov: %f expected fov: %f fov slope: %f fov intercept: %f", beforeZ, afterZ, dfov, expected_fov, fov_slope, fov_intercept);
    }
    clustered_sfr_map.clear();
    qInfo("AA time elapsed: %d", timer.elapsed());
    map.insert("Z_PEAK_CC", aa_result["zPeak"].toDouble());
//    map.insert("Z_PEAK_UL", aa_result["zPeak"].toDouble());
//    map.insert("Z_PEAK_UR", ur_zPeak);
//    map.insert("Z_PEAK_LL", ll_zPeak);
//    map.insert("Z_PEAK_LR", lr_zPeak);
//    map.insert("FOV_SLOPE", fov_slope);
//    map.insert("FOV_INTERCEPT", fov_intercept);
//    map.insert("DEV", dev);
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "AA", map);

//    qInfo("Finish AA");
    return ErrorCodeStruct{ ErrorCode::OK, ""};
}

void AACoreNew::performAAOffline()
{
    clustered_sfr_map.clear();
    ErrorCodeStruct ret = { OK, ""};
    QVariantMap map, stepTimerMap, dFovMap, sfrTimeElapsedMap;
    QElapsedTimer timer;
    timer.start();
    int resize_factor = 2;
    int sfrCount = 0;
    double step_size = 0.01, start = -20.5;
    double xsum=0,x2sum=0,ysum=0,xysum=0;
    double estimated_fov_slope = 15;
    isZScanNeedToStop = false;
    QString foldername = AA_DEBUG_DIR;
    int inputImageCount = 15;
    for (int i = 0; i < inputImageCount -1; i++)
    {
        if (isZScanNeedToStop) {
            qInfo("All peak passed, stop zscan");
            break;
        }
        //QString filename = "aa_log\\aa_log_bug\\2018-11-10T14-42-55-918Z\\zscan_" + QString::number(i) + ".bmp";
        //QString filename = "aa_log\\aa_log_bug\\2018-11-10T14-42-55-918Z\\zscan_" + QString::number(i) + ".bmp";
        QString filename = "C:\\Users\\emil\\Desktop\\Test\\Samsung\\debug\\debug\\zscan_" + QString::number(i) + ".bmp";
        //QString filename = "1\\" + QString::number(i+1) + ".bmp";
        cv::Mat img = cv::imread(filename.toStdString());
        if (!blackScreenCheck(img)) {
            return;
        }
        cv::Mat dst;
        cv::Size size(img.cols/resize_factor, img.rows/resize_factor);
        cv::resize(img, dst, size);

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

        emit sfrWorkerController->calculate(i, start+i*step_size, dst, parameters.MaxIntensity(), parameters.MaxIntensity2(), parameters.MinArea(), parameters.MaxArea(), parameters.aaScanMTFFrequency()+1);
        img.release();
        dst.release();
        sfrCount++;
    }
    int timeout=1000;
    while(this->clustered_sfr_map.size() != sfrCount && timeout > 0) {
        Sleep(10);
        timeout--;
    }
    if (timeout <= 0) {
        qInfo("Error in performing AA Offline: %d", timeout);
        return;
    }
    qInfo("clustered sfr map pattern size: %d clustered_sfr_map size: %d", clustered_sfr_map[0].size(), clustered_sfr_map.size());
    QVariantMap aa_result = sfrFitCurve_Advance(resize_factor, start);
    clustered_sfr_map.clear();
    qInfo("[PerformAAOffline] time elapsed: %d", timer.elapsed());
}

void AACoreNew::performHandling(int cmd, QString params)
{
    qInfo("performHandling: %d %s", cmd, params.toStdString().c_str());
    handlingParams = params;
    emit sendHandlingOperation(cmd);
}

bool zPeakComp(const threeDPoint & p1, const threeDPoint & p2)
{
    return p1.z < p2.z;
}

QVariantMap AACoreNew::sfrFitCurve_Advance(int resize_factor, double start_pos)
{
    QVariantMap result, map;
    vector<vector<Sfr_entry>> sorted_sfr_map;
    for (size_t i = 0; i < clustered_sfr_map[0].size(); ++i)
    {
        vector<Sfr_entry> sfr_map;
        for (size_t ii = 0; ii < clustered_sfr_map.size(); ++ii)
        {
            sfr_map.push_back(clustered_sfr_map[ii][i]);
        }
        sorted_sfr_map.push_back(sfr_map);
    }
    qInfo("clustered sfr map pattern size: %d sorted_sfr_map size: %d", clustered_sfr_map[0].size(), sorted_sfr_map.size());
    if (clustered_sfr_map[0].size() == 0 || clustered_sfr_map[0].size() < 4) {
        qInfo("AA Scan Fail. Not enough data points for data fitting");
        result.insert("OK", false);
        return result;
    }
    int fitOrder = parameters.aaScanCurveFitOrder();
    if (clustered_sfr_map[0].size() == 6) {
        qInfo("Down the curve fitting to 5 order");
        fitOrder = 5;
    } else if (clustered_sfr_map[0].size() == 5) {
        qInfo("Down the curve fitting to 4 order");
        fitOrder = 4;
    }
    threeDPoint point_0;
    vector<threeDPoint> points_1, points_11;
    vector<threeDPoint> points_2, points_22;
    vector<threeDPoint> points_3, points_33;
    for (size_t i = 0; i < sorted_sfr_map.size(); i++) {
        vector<double> sfr, z;
        double ex = 0; double ey = 0;
        for (size_t ii=0; ii < sorted_sfr_map[i].size(); ii++) {
            sfr.push_back(sorted_sfr_map[i][ii].sfr);
            z.push_back(sorted_sfr_map[i][ii].pz-start_pos);
            ex += sorted_sfr_map[i][ii].px*resize_factor;
            ey += sorted_sfr_map[i][ii].py*resize_factor;
        }
        ex /= (sorted_sfr_map[i].size()*parameters.SensorXRatio());
        ey /= (sorted_sfr_map[i].size()*parameters.SensorYRatio());

        double peak_sfr, peak_z;
        fitCurve(z, sfr, fitOrder, peak_z, peak_sfr);
        if (i==0) {
            point_0.x = ex; point_0.y = ey; point_0.z = peak_z + start_pos;
        } else if ( i >= 1 && i <= 4) {
            points_1.emplace_back(ex, ey, peak_z + start_pos);
            points_11.emplace_back(ex, ey, peak_z + start_pos);
        } else if ( i >= 5 && i <= 8) {
            points_2.emplace_back(ex, ey, peak_z + start_pos);
            points_22.emplace_back(ex, ey, peak_z + start_pos);
        } else if ( i >= 9 && i <= 12) {
            points_3.emplace_back(ex, ey, peak_z + start_pos);
            points_33.emplace_back(ex, ey, peak_z + start_pos);
        }
    }
    sort(points_11.begin(), points_11.end(), zPeakComp);
    sort(points_22.begin(), points_22.end(), zPeakComp);
    sort(points_33.begin(), points_33.end(), zPeakComp);

    qInfo("Layer 0: x: %f y: %f z: %f", point_0.x, point_0.y, point_0.z);
    int layerDetected = 0; int validLayer = 0;
    if (points_1.size()==4) { validLayer++; }
    if (points_2.size()==4) { validLayer++; }
    if (points_3.size()==4) { validLayer++; }
    for (size_t i = 0; i < points_1.size(); i++) {
        qInfo("Layer 1: x: %f y: %f z: %f", points_1[i].x, points_1[i].y, points_1[i].z);
        layerDetected++;
    }

    for (size_t i = 0; i < points_2.size(); i++) {
        qInfo("Layer 2: x: %f y: %f z: %f", points_2[i].x, points_2[i].y, points_2[i].z);
        layerDetected++;
    }

    for (size_t i = 0; i < points_3.size(); i++) {
        qInfo("Layer 3: x: %f y: %f z: %f", points_3[i].x, points_3[i].y, points_3[i].z);
        layerDetected++;
    }

    //Layer checking
    if ((points_1.size() > 0 && points_1.size() < 4) ||
        (points_2.size() > 0 && points_2.size() < 4) ||
        (points_3.size() > 0 && points_3.size() < 4))
    {
        qCritical("AA pattern layer checking fail.");
        result.insert("OK", false);
        emit postSfrDataToELK(runningUnit, map);
        return result;
    }

    threeDPoint weighted_vector_1 = planeFitting(points_1);
    threeDPoint weighted_vector_2 = planeFitting(points_2);
    threeDPoint weighted_vector_3 = planeFitting(points_3);
    double dev_1 = 0, dev_2 = 0, dev_3 =0;
    if (points_11.size()>0) dev_1 = fabs(points_11[0].z - points_11[points_11.size()-1].z)*1000;
    if (points_22.size()>0) dev_2 = fabs(points_22[0].z - points_22[points_22.size()-1].z)*1000;
    if (points_33.size()>0) dev_3 = fabs(points_33[0].z - points_33[points_33.size()-1].z)*1000;
    double xTilt_1 = weighted_vector_1.z * weighted_vector_1.x;
    double yTilt_1 = weighted_vector_1.z * weighted_vector_1.y;
    double xTilt_2 = weighted_vector_2.z * weighted_vector_2.x;
    double yTilt_2 = weighted_vector_2.z * weighted_vector_2.y;
    double xTilt_3 = weighted_vector_3.z * weighted_vector_3.x;
    double yTilt_3 = weighted_vector_3.z * weighted_vector_3.y;

    qInfo("Layer 1: xTilt: %f yTilt: %f dev: %f", xTilt_1, yTilt_1, dev_1);
    qInfo("Layer 2: xTilt: %f yTilt: %f dev: %f", xTilt_2, yTilt_2, dev_2);
    qInfo("Layer 3: xTilt: %f yTilt: %f dev: %f", xTilt_3, yTilt_3, dev_3);
    result.insert("xTilt_1", xTilt_1); map.insert("xTilt_1", xTilt_1);
    result.insert("yTilt_1", yTilt_1); map.insert("yTilt_1", yTilt_1);
    result.insert("xTilt_2", xTilt_2); map.insert("xTilt_2", xTilt_2);
    result.insert("yTilt_2", yTilt_2); map.insert("yTilt_2", yTilt_2);
    result.insert("xTilt_3", xTilt_3); map.insert("xTilt_3", xTilt_3);
    result.insert("yTilt_3", yTilt_3); map.insert("yTilt_3", yTilt_3);
    result.insert("zPeak", point_0.z); map.insert("zPeak", point_0.z);
    result.insert("OK", true);
    if (validLayer == 1) {
        result.insert("xTilt", xTilt_1);
        result.insert("yTilt", yTilt_1);
    } else if (validLayer == 2) {
        result.insert("xTilt", xTilt_2);
        result.insert("yTilt", yTilt_2);
    } else if (validLayer == 3) {
        result.insert("xTilt", xTilt_3);
        result.insert("yTilt", yTilt_3);
    }
    AAData *data;

    if (currentChartDisplayChannel == 0) {
        data = &aaData_1;
        currentChartDisplayChannel = 1;
    } else {
        data = &aaData_2;
        currentChartDisplayChannel = 0;
    }
    data->clear();
    data->setDev(round(dev_1*1000)/1000);
    data->setWCCPeakZ(round(point_0.z*1000));
    data->setXTilt(round(xTilt_1*10000)/10000);
    data->setYTilt(round(yTilt_1*10000)/10000);
    //Layer 0:
    data->setLayer0(QString("L0 -- CC:")
                    .append(QString::number(point_0.z, 'g', 6))
    );
    //Layer 1:
    if (points_1.size() > 0) {
        data->setLayer1(QString("L1- XT:")
                           .append(QString::number(xTilt_1,'g',3))
                           .append(" YT:")
                           .append(QString::number(yTilt_1,'g',3))
                           .append(" UL:")
                           .append(QString::number(points_1[0].z,'g',6))
                           .append(" UR:")
                           .append(QString::number(points_1[3].z,'g',6))
                           .append(" LL:")
                           .append(QString::number(points_1[1].z,'g',6))
                           .append(" LR:")
                           .append(QString::number(points_1[2].z,'g',6))
                           .append(" DEV:")
                           .append(QString::number(dev_1,'g',3))
        );
    }
    //Layer 2:
    if (points_2.size() > 0) {
        data->setLayer2(QString("L2- XT:")
                           .append(QString::number(xTilt_2,'g',3))
                           .append(" YT:")
                           .append(QString::number(yTilt_2,'g',3))
                           .append(" UL:")
                           .append(QString::number(points_2[0].z,'g',6))
                           .append(" UR:")
                           .append(QString::number(points_2[3].z,'g',6))
                           .append(" LL:")
                           .append(QString::number(points_2[1].z,'g',6))
                           .append(" LR:")
                           .append(QString::number(points_2[2].z,'g',6))
                           .append(" DEV:")
                           .append(QString::number(dev_2,'g',3))
        );
    }
    //Layer 3
    if (points_3.size() > 0) {
        data->setLayer3(QString("L3- XT:")
                           .append(QString::number(xTilt_3,'g',3))
                           .append(" YT:")
                           .append(QString::number(yTilt_3,'g',3))
                           .append(" UL:")
                           .append(QString::number(points_3[0].z,'g',6))
                           .append(" UR:")
                           .append(QString::number(points_3[3].z,'g',6))
                           .append(" LL:")
                           .append(QString::number(points_3[1].z,'g',6))
                           .append(" LR:")
                           .append(QString::number(points_3[2].z,'g',6))
                           .append(" DEV:")
                           .append(QString::number(dev_3,'g',3))
        );
    }

    int display_layer = validLayer-1;
    QVariantMap sfrMap;
    for(size_t i = 0; i < sorted_sfr_map[0].size(); i++)
    {
        data->addData(0, sorted_sfr_map[0][i].pz*1000, sorted_sfr_map[0][i].sfr, 0);
        QVariantMap s;
        s.insert("index", i);
        s.insert("px", sorted_sfr_map[0][i].px);
        s.insert("py", sorted_sfr_map[0][i].py);
        s.insert("area", sorted_sfr_map[0][i].area);
        s.insert("sfr", sorted_sfr_map[0][i].sfr);
        s.insert("dfov", current_dfov[QString::number(i)]);
        s.insert("t_sfr", sorted_sfr_map[0][i].t_sfr);
        s.insert("b_sfr", sorted_sfr_map[0][i].b_sfr);
        s.insert("l_sfr", sorted_sfr_map[0][i].l_sfr);
        s.insert("r_sfr", sorted_sfr_map[0][i].r_sfr);
        s.insert("pz", sorted_sfr_map[0][i].pz);
        sfrMap.insert(QString::number(i), s);

        if (points_1.size() > 0) {
            data->addData(1, sorted_sfr_map[1+4*display_layer][i].pz*1000, sorted_sfr_map[1+4*display_layer][i].sfr, 0);
            data->addData(2, sorted_sfr_map[4+4*display_layer][i].pz*1000, sorted_sfr_map[4+4*display_layer][i].sfr, 0);
            data->addData(3, sorted_sfr_map[3+4*display_layer][i].pz*1000, sorted_sfr_map[3+4*display_layer][i].sfr, 0);
            data->addData(4, sorted_sfr_map[2+4*display_layer][i].pz*1000, sorted_sfr_map[2+4*display_layer][i].sfr, 0);
        }
    }
    data->plot();
    map.insert("CC", sfrMap);
    if (validLayer>=1) {
        for (size_t j = 0; j < 4; j++){
            QVariantMap sfrMap;
            for(size_t i = 0; i < sorted_sfr_map[1].size(); i++)
            {
                QVariantMap s;
                s.insert("index", i);
                s.insert("px", sorted_sfr_map[1+j][i].px);
                s.insert("py", sorted_sfr_map[1+j][i].py);
                s.insert("area", sorted_sfr_map[1+j][i].area);
                s.insert("sfr", sorted_sfr_map[1+j][i].sfr);
                s.insert("dfov", current_dfov[QString::number(i)]);
                s.insert("t_sfr", sorted_sfr_map[1+j][i].t_sfr);
                s.insert("b_sfr", sorted_sfr_map[1+j][i].b_sfr);
                s.insert("l_sfr", sorted_sfr_map[1+j][i].l_sfr);
                s.insert("r_sfr", sorted_sfr_map[1+j][i].r_sfr);
                s.insert("pz", sorted_sfr_map[1+j][i].pz);
                sfrMap.insert(QString::number(i), s);
            }
            QString indexString = "";
            if (j==0) indexString = "UL_1";
            if (j==1) indexString = "LL_1";
            if (j==2) indexString = "LR_1";
            if (j==3) indexString = "UR_1";
            map.insert(indexString, sfrMap);
        }
        map.insert("xPeak_1_UL", points_1[0].x);
        map.insert("xPeak_1_LL", points_1[1].x);
        map.insert("xPeak_1_LR", points_1[2].x);
        map.insert("xPeak_1_UR", points_1[3].x);
        map.insert("yPeak_1_UL", points_1[0].y);
        map.insert("yPeak_1_LL", points_1[1].y);
        map.insert("yPeak_1_LR", points_1[2].y);
        map.insert("yPeak_1_UR", points_1[3].y);
        map.insert("zPeak_1_UL", points_1[0].z);
        map.insert("zPeak_1_LL", points_1[1].z);
        map.insert("zPeak_1_LR", points_1[2].z);
        map.insert("zPeak_1_UR", points_1[3].z);
    }
    if (validLayer>=2) {
        for (size_t j = 0; j < 4; j++){
            QVariantMap sfrMap;
            for(size_t i = 0; i < sorted_sfr_map[5].size(); i++)
            {
                QVariantMap s;
                s.insert("index", i);
                s.insert("px", sorted_sfr_map[5+j][i].px);
                s.insert("py", sorted_sfr_map[5+j][i].py);
                s.insert("area", sorted_sfr_map[5+j][i].area);
                s.insert("sfr", sorted_sfr_map[5+j][i].sfr);
                s.insert("dfov", current_dfov[QString::number(i)]);
                s.insert("t_sfr", sorted_sfr_map[5+j][i].t_sfr);
                s.insert("b_sfr", sorted_sfr_map[5+j][i].b_sfr);
                s.insert("l_sfr", sorted_sfr_map[5+j][i].l_sfr);
                s.insert("r_sfr", sorted_sfr_map[5+j][i].r_sfr);
                s.insert("pz", sorted_sfr_map[5+j][i].pz);
                sfrMap.insert(QString::number(i), s);
            }
            QString indexString = "";
            if (j==0) indexString = "UL_2";
            if (j==1) indexString = "LL_2";
            if (j==2) indexString = "LR_2";
            if (j==3) indexString = "UR_2";
            map.insert(indexString, sfrMap);
        }
        map.insert("xPeak_2_UL", points_2[0].x);
        map.insert("xPeak_2_LL", points_2[1].x);
        map.insert("xPeak_2_LR", points_2[2].x);
        map.insert("xPeak_2_UR", points_2[3].x);
        map.insert("yPeak_2_UL", points_2[0].y);
        map.insert("yPeak_2_LL", points_2[1].y);
        map.insert("yPeak_2_LR", points_2[2].y);
        map.insert("yPeak_2_UR", points_2[3].y);
        map.insert("zPeak_2_UL", points_2[0].z);
        map.insert("zPeak_2_LL", points_2[1].z);
        map.insert("zPeak_2_LR", points_2[2].z);
        map.insert("zPeak_2_UR", points_2[3].z);
    }
    if (validLayer>=3) {
        for (size_t j = 0; j < 4; j++){
            QVariantMap sfrMap;
            for(size_t i = 0; i < sorted_sfr_map[1].size(); i++)
            {
                QVariantMap s;
                s.insert("index", i);
                s.insert("px", sorted_sfr_map[9+j][i].px);
                s.insert("py", sorted_sfr_map[9+j][i].py);
                s.insert("area", sorted_sfr_map[9+j][i].area);
                s.insert("sfr", sorted_sfr_map[9+j][i].sfr);
                s.insert("dfov", current_dfov[QString::number(i)]);
                s.insert("t_sfr", sorted_sfr_map[9+j][i].t_sfr);
                s.insert("b_sfr", sorted_sfr_map[9+j][i].b_sfr);
                s.insert("l_sfr", sorted_sfr_map[9+j][i].l_sfr);
                s.insert("r_sfr", sorted_sfr_map[9+j][i].r_sfr);
                s.insert("pz", sorted_sfr_map[9+j][i].pz);
                sfrMap.insert(QString::number(i), s);
            }
            QString indexString = "";
            if (j==0) indexString = "UL_3";
            if (j==1) indexString = "LL_3";
            if (j==2) indexString = "LR_3";
            if (j==3) indexString = "UR_3";
            map.insert(indexString, sfrMap);
        }
        map.insert("xPeak_3_UL", points_3[0].x);
        map.insert("xPeak_3_LL", points_3[1].x);
        map.insert("xPeak_3_LR", points_3[2].x);
        map.insert("xPeak_3_UR", points_3[3].x);
        map.insert("yPeak_3_UL", points_3[0].y);
        map.insert("yPeak_3_LL", points_3[1].y);
        map.insert("yPeak_3_LR", points_3[2].y);
        map.insert("yPeak_3_UR", points_3[3].y);
        map.insert("zPeak_3_UL", points_3[0].z);
        map.insert("zPeak_3_LL", points_3[1].z);
        map.insert("zPeak_3_LR", points_3[2].z);
        map.insert("zPeak_3_UR", points_3[3].z);
        map.insert("fov_slope", current_fov_slope);
    }
    emit postSfrDataToELK(runningUnit, map);
    return result;
}

ErrorCodeStruct AACoreNew::performMTFOffline(QJsonValue params)
{
    QVariantMap map;
    double cc_min_sfr = params["CC"].toDouble(-1);
    double ul_min_sfr = params["UL"].toDouble(-1);
    double ur_min_sfr = params["UR"].toDouble(-1);
    double ll_min_sfr = params["LL"].toDouble(-1);
    double lr_min_sfr = params["LR"].toDouble(-1);
    map.insert("cc_min_sfr_spec", cc_min_sfr);
    map.insert("ul_min_sfr_spec", ul_min_sfr);
    map.insert("ur_min_sfr_spec", ur_min_sfr);
    map.insert("ll_min_sfr_spec", ll_min_sfr);
    map.insert("lr_min_sfr_spec", lr_min_sfr);
    qInfo("%f %f %f %f %f", cc_min_sfr, ul_min_sfr, ur_min_sfr, ll_min_sfr, lr_min_sfr);
    QJsonValue aaPrams;
    cv::Mat input_img = cv::imread("livePhoto.bmp");
    QElapsedTimer timer;
    if (!blackScreenCheck(input_img)) {  return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""}; }
    double dfov = calculateDFOV(input_img);
    qInfo("%f %d %d %d", dfov, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea() );
    std::vector<AA_Helper::patternAttr> patterns1 = AA_Helper::AAA_Search_MTF_Pattern_Ex(input_img, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea(), -1);
    std::vector<AA_Helper::patternAttr> patterns2 = AA_Helper::AAA_Search_MTF_Pattern_Ex(input_img, parameters.MaxIntensity2(), parameters.MinArea(), parameters.MaxArea(), -1);

    std::vector<AA_Helper::patternAttr> patterns;

    if (patterns2.size() < patterns1.size()) {
        patterns2.swap(patterns1);
    }

    if (patterns2.size() >= patterns1.size()) {
        for (uint i = 0; i < patterns2.size(); i++) {
            patterns.push_back(patterns2[i]);
        }
        for (uint i = 0; i < patterns1.size(); i++) {
            bool isDuplicated = false;
            for (uint j = 0; j < patterns2.size(); j++) {
                double positionDiff = sqrt(pow(patterns1[i].center.x()-patterns2[j].center.x(), 2) + pow(patterns1[i].center.y()-patterns2[j].center.y(),2));
                if (positionDiff < 20) { //threshold diff
                    isDuplicated = true;
                }
            }
            if (!isDuplicated) {
                patterns.push_back(patterns1[i]);
            }
        }
    }

    double rect_width = 0;
    double imageCenterX = input_img.cols/2;
    double imageCenterY = input_img.rows/2;
    double r1 = sqrt(imageCenterX*imageCenterX + imageCenterY*imageCenterY);
    std::vector<MTF_Pattern_Position> vec;
    timer.start();
    for (uint i = 0; i < patterns.size(); i++) {
        //Crop ROI
        {
            cv::Rect roi; cv::Mat copped_roi;
            double width = sqrt(patterns[i].area)/2;
            rect_width = width;
            roi.width = width*4; roi.height = width*4;
            roi.x = patterns[i].center.x() - width*2;
            roi.y = patterns[i].center.y() - width*2;
            if (roi.x < 0) roi.x = 0;
            if (roi.x + roi.width > input_img.cols) { roi.width = input_img.cols - roi.x; }
            if (roi.y < 0) roi.y = 0;
            if (roi.y + roi.height > input_img.rows) { roi.height = input_img.rows - roi.y; }
            input_img(roi).copyTo(copped_roi);
            QString imageName = QString::number(i).append(".bmp");
            cv::imwrite(imageName.toStdString().c_str(), copped_roi);
            double radius = sqrt(pow(patterns[i].center.x() - imageCenterX, 2) + pow(patterns[i].center.y() - imageCenterY, 2));
            double f = radius/r1;
            double t_sfr = 0, r_sfr = 0, b_sfr = 0, l_sfr = 0;
            bool ret = sfr::sfr_calculation_single_pattern(copped_roi, t_sfr, r_sfr, b_sfr, l_sfr, 8*(parameters.mtfFrequency()+1));
            if (!ret) {
                qWarning("Cannot calculate MTF in the detected pattern");
                return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
            }
            double avg_sfr = ( t_sfr + r_sfr + b_sfr + l_sfr)/4;
            vec.emplace_back(patterns[i].center.x(), patterns[i].center.y(),
                             f, t_sfr*100, r_sfr*100, b_sfr*100, l_sfr*100, patterns[i].area, avg_sfr);
         }
    }
    qInfo("time elapsed: %d", timer.elapsed());
    vector<int> layers = sfr::classifyLayers(vec);
    QImage qImage = ImageGrabbingWorkerThread::cvMat2QImage(input_img);
    QPainter qPainter(&qImage);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setFont(QFont("Times", parameters.drawTextSize(), QFont::Light));
    int max_layer = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec.at(i).layer > max_layer) {
            max_layer = vec.at(i).layer - 1;
        }
        qInfo("Layer %d :  px: %f py: %f sfr: %f 1: %f 2: %f 3: %f 4: %f area: %f", ((i-1)/4) + 1,
              vec[i].x, vec[i].y, vec[i].avg_sfr, vec[i].t_sfr, vec[i].r_sfr, vec[i].b_sfr, vec[i].l_sfr, vec[i].area);
        QVariantMap data;
        data.insert("x", vec[i].x);
        data.insert("y", vec[i].y);
        data.insert("sfr", vec[i].avg_sfr);
        qPainter.setPen(QPen(Qt::blue, 4.0));
        qPainter.drawText(vec[i].x - rect_width/2, vec[i].y - rect_width*2, QString::number(vec[i].t_sfr, 'g', 4));
        qPainter.drawText(vec[i].x + rect_width, vec[i].y,  QString::number(vec[i].r_sfr, 'g', 4));
        qPainter.drawText(vec[i].x - rect_width, vec[i].y + rect_width*3,  QString::number(vec[i].b_sfr, 'g', 4));
        qPainter.drawText(vec[i].x - rect_width*4, vec[i].y,  QString::number(vec[i].l_sfr, 'g', 4));
    }
    qPainter.end();
    sfrImageReady(std::move(qImage));

    qInfo("max layer: %d", max_layer);
    bool sfr_check = true;
    if (vec.size() > 0) {
        if (vec[0].t_sfr < cc_min_sfr || vec[0].r_sfr < cc_min_sfr || vec[0].b_sfr < cc_min_sfr || vec[0].l_sfr < cc_min_sfr) {
            qWarning("CC cannot pass");
            map.insert("error", "CC cannot pass");
            sfr_check = false;
        }
    } else {
        qWarning("cannot find CC");
        map.insert("error", "cannot find CC");
        sfr_check = false;
    }
    if (vec.size() >= max_layer*4 + 1) {
        if (vec[max_layer*4 + 1].t_sfr < ul_min_sfr || vec[max_layer*4 + 1].r_sfr < ul_min_sfr ||
            vec[max_layer*4 + 1].b_sfr < ul_min_sfr || vec[max_layer*4 + 1].l_sfr < ul_min_sfr) {
            qWarning("UL cannot pass");
            map.insert("error", "UL cannot pass");
            sfr_check = false;
        }
    } else {
        qWarning("cannot find UL");
        map.insert("error", "cannot find UL");
        sfr_check = false;
    }

    if (vec.size() >= max_layer*4 + 2) {
        if (vec[max_layer*4 + 2].t_sfr < ll_min_sfr || vec[max_layer*4 + 2].r_sfr < ll_min_sfr ||
            vec[max_layer*4 + 2].b_sfr < ll_min_sfr || vec[max_layer*4 + 2].l_sfr < ll_min_sfr) {
            qWarning("LL cannot pass");
            map.insert("error", "LL cannot pass");
            sfr_check = false;
        }
    } else {
        qWarning("cannot find LL");
        map.insert("error", "cannot find LL");
        sfr_check = false;
    }

    if (vec.size() >= max_layer*4 + 3) {
       if (vec[max_layer*4 + 3].t_sfr < lr_min_sfr || vec[max_layer*4 + 3].r_sfr < lr_min_sfr ||
           vec[max_layer*4 + 3].b_sfr < lr_min_sfr || vec[max_layer*4 + 3].l_sfr < lr_min_sfr) {
            qWarning("LR cannot pass");
            map.insert("error", "LR cannot pass");
            sfr_check = false;
        }
    } else {
        qWarning("cannot find LR");
        map.insert("error", "cannot find LR");
        sfr_check = false;
    }

    if (vec.size() >= max_layer*4 + 4) {
        if (vec[max_layer*4 + 4].t_sfr < ur_min_sfr || vec[max_layer*4 + 4].r_sfr < ur_min_sfr ||
            vec[max_layer*4 + 4].b_sfr < ur_min_sfr || vec[max_layer*4 + 4].l_sfr < ur_min_sfr) {
            qWarning("UR cannot pass");
            map.insert("error", "UR cannot pass");
            sfr_check = false;
        }
    }

    if (vec.size() > 0 ) {
        map.insert("CC_T_SFR", vec[0].t_sfr);
        map.insert("CC_R_SFR", vec[0].r_sfr);
        map.insert("CC_B_SFR", vec[0].b_sfr);
        map.insert("CC_L_SFR", vec[0].l_sfr);
        map.insert("CC_SFR", (vec[0].t_sfr + vec[0].r_sfr + vec[0].b_sfr + vec[0].l_sfr)/4);
    }
    if (vec.size() >= max_layer*4 + 1) {
        map.insert("UL_T_SFR", vec[max_layer*4 + 1].t_sfr);
        map.insert("UL_R_SFR", vec[max_layer*4 + 1].r_sfr);
        map.insert("UL_B_SFR", vec[max_layer*4 + 1].b_sfr);
        map.insert("UL_L_SFR", vec[max_layer*4 + 1].l_sfr);
        map.insert("UL_SFR", (vec[max_layer*4 + 1].t_sfr + vec[max_layer*4 + 1].r_sfr + vec[max_layer*4 + 1].b_sfr + vec[max_layer*4 + 1].l_sfr)/4);
    }
    if (vec.size() >= max_layer*4 + 2) {
        map.insert("LL_T_SFR", vec[max_layer*4 + 2].t_sfr);
        map.insert("LL_R_SFR", vec[max_layer*4 + 2].r_sfr);
        map.insert("LL_B_SFR", vec[max_layer*4 + 2].b_sfr);
        map.insert("LL_L_SFR", vec[max_layer*4 + 2].l_sfr);
        map.insert("LL_SFR", (vec[max_layer*4 + 2].t_sfr + vec[max_layer*4 + 2].r_sfr + vec[max_layer*4 + 2].b_sfr + vec[max_layer*4 + 2].l_sfr)/4);
    }
    if (vec.size() >= max_layer*4 + 3) {
        map.insert("LR_T_SFR", vec[max_layer*4 + 3].t_sfr);
        map.insert("LR_R_SFR", vec[max_layer*4 + 3].r_sfr);
        map.insert("LR_B_SFR", vec[max_layer*4 + 3].b_sfr);
        map.insert("LR_L_SFR", vec[max_layer*4 + 3].l_sfr);
        map.insert("LR_SFR", (vec[max_layer*4 + 3].t_sfr + vec[max_layer*4 + 3].r_sfr + vec[max_layer*4 + 3].b_sfr + vec[max_layer*4 + 3].l_sfr)/4);
    }
    if (vec.size() >= max_layer*4 + 4) {
        map.insert("UR_T_SFR", vec[max_layer*4 + 4].t_sfr);
        map.insert("UR_R_SFR", vec[max_layer*4 + 4].r_sfr);
        map.insert("UR_B_SFR", vec[max_layer*4 + 4].b_sfr);
        map.insert("UR_L_SFR", vec[max_layer*4 + 4].l_sfr);
        map.insert("UR_SFR", (vec[max_layer*4 + 4].t_sfr + vec[max_layer*4 + 4].r_sfr + vec[max_layer*4 + 4].b_sfr + vec[max_layer*4 + 4].l_sfr)/4);
    }
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "MTF", map);

    return ErrorCodeStruct{ErrorCode::OK, ""};
}

QString convertFormulaFromTOFResult(QString input, TOFResult tofResult)
{
    QString formula = input;
    formula.replace("a", QString::number(tofResult.a));
    formula.replace("b", QString::number(tofResult.b));
    formula.replace("c", QString::number(tofResult.c));
    formula.replace("d", QString::number(tofResult.d));
    formula.replace("e", QString::number(tofResult.e));
    formula.replace("f", QString::number(tofResult.f));
    formula.replace("g", QString::number(tofResult.g));
    formula.replace("h", QString::number(tofResult.h));
    formula.replace("i", QString::number(tofResult.i));
    formula.replace("j", QString::number(tofResult.j));
    formula.replace("k", QString::number(tofResult.k));
    formula.replace("l", QString::number(tofResult.l));
    return formula;
}

ErrorCodeStruct AACoreNew::performMotionMove(QJsonValue params)
{
    QElapsedTimer timer;timer.start();

    QVariantMap map;

    map.insert("inital_sut_x", this->lsut->sut_carrier->GetFeedBackPos().X);
    map.insert("inital_sut_y", this->lsut->sut_carrier->GetFeedBackPos().Y);
    map.insert("inital_sut_z", this->lsut->sut_carrier->GetFeedBackPos().Z);

    map.insert("inital_aa_a", this->aa_head->GetFeedBack().A);
    map.insert("inital_aa_b", this->aa_head->GetFeedBack().B);
    map.insert("inital_aa_c", this->aa_head->GetFeedBack().C);

    qInfo("Perform Motion Move");
    if (lsut->sut_carrier->motor_x == Q_NULLPTR ||
        lsut->sut_carrier->motor_y == Q_NULLPTR) {
        qWarning("SUT carrier motors is null, motors cannot move");
    } else {
        if ( fabs(tof_x) > parameters.xLimit() ||
             fabs(tof_y) > parameters.yLimit() ||
             fabs(tof_z) > parameters.zLimit() ||
             fabs(tof_rx) > parameters.rxLimit() ||
             fabs(tof_ry) > parameters.ryLimit() ||
             fabs(tof_rz) > parameters.rzLimit()) {
             qWarning("The calculated movement is larger than spec, motors cannot move");
        } else {
            this->lsut->stepMove_XY_Sync(tof_x, tof_y);
            this->lsut->sut_carrier->StepMove_Z(tof_z);
            aa_head->stepInterpolation_AB_Sync(tof_rx, tof_ry);
            aa_head->step_C_Sync(tof_rz);
        }
    }
    map.insert("xLimit", parameters.xLimit());
    map.insert("ylimit", parameters.yLimit());
    map.insert("zLimit", parameters.zLimit());
    map.insert("rxLimit", parameters.rxLimit());
    map.insert("ryLimit", parameters.ryLimit());
    map.insert("rzLimit", parameters.rzLimit());

    map.insert("x", tof_x);
    map.insert("y", tof_y);
    map.insert("z", tof_z);
    map.insert("rx", tof_rx);
    map.insert("ry", tof_ry);
    map.insert("rz", tof_rz);

    map.insert("final_sut_x", this->lsut->sut_carrier->GetFeedBackPos().X);
    map.insert("final_sut_y", this->lsut->sut_carrier->GetFeedBackPos().Y);
    map.insert("final_sut_z", this->lsut->sut_carrier->GetFeedBackPos().Z);

    map.insert("final_aa_a", this->aa_head->GetFeedBack().A);
    map.insert("final_aa_b", this->aa_head->GetFeedBack().B);
    map.insert("final_aa_c", this->aa_head->GetFeedBack().C);

    map.insert("timeElapsed", timer.elapsed());
    qInfo("Finish motion move. Time elapsed: %d", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "Motion_Move", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performCommand(QJsonValue params)
{
    QElapsedTimer timer;timer.start();
    QString directory = params["directory"].toString();
    QString command = params["command"].toString();
    qInfo("Directory: %s Command: %s", directory.toStdString().c_str(), command.toStdString().c_str());
    QStringList arguments;
    arguments << "/c" << command.toStdString().c_str();
    QStringList args;
    QProcess *child = new QProcess();
    child->setWorkingDirectory(directory);
    child->start(directory + "/" + command, args);
    QVariantMap map;
    map.insert("directory", directory.toStdString().c_str());
    map.insert("command", command.toStdString().c_str());
    map.insert("timeElapsed", timer.elapsed());
    qInfo("Finish command. Time elapsed: %d", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "Command", map);

    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performTOF(QJsonValue params)
{
    QElapsedTimer timer;timer.start();
    QVariantMap map;
    qInfo("Start perform TOF");
    int method = params["method"].toInt(1);
    int imageProcessingMethod = params["image_processing_method"].toInt(-1);
    int enable_motion = params["enable_motion"].toInt(0);
    int exposureTime = params["exposure_time"].toInt(5000);
    int delayAfterSettingExposure = params["delay"].toInt(500);
    qInfo("enable motion: %d exposureTime: %d delay: %d", enable_motion, exposureTime, delayAfterSettingExposure);
    bool grabFromCamera = false;
    if (imageProcessingMethod != -1) {
        grabFromCamera = true;
        method = imageProcessingMethod;
        qInfo("perform TOF : %d", method);
        visionModule->setExposureTime(CAMERA_SH_HIK_CAM, exposureTime);
        QThread::msleep(delayAfterSettingExposure);
    }
    QString filename = params["filename"].toString();
    TOFResult tofResult;
    filename.replace("file:///", "");
    double x = 0;
    double y = 0;
    double z = 0;
    double rx = 0;
    double ry = 0;
    double rz = 0;
    if (method == 1) {
        tofResult = visionModule->imageProcessing1(filename, parameters.scanY1PixelLocation1(), parameters.scanY2PixelLocation1(), parameters.scanY4PixelLocation1(), parameters.scanY5PixelLocation1(), parameters.halfWidth1(), parameters.intensityCorrectionFactor1(), grabFromCamera);
        x = mathExpression(convertFormulaFromTOFResult(this->parameters.x1(), tofResult));
        y = mathExpression(convertFormulaFromTOFResult(this->parameters.y1(), tofResult));
        z = mathExpression(convertFormulaFromTOFResult(this->parameters.z1(), tofResult));
        rx = mathExpression(convertFormulaFromTOFResult(this->parameters.rx1(), tofResult));
        ry = mathExpression(convertFormulaFromTOFResult(this->parameters.ry1(), tofResult));
        rz = mathExpression(convertFormulaFromTOFResult(this->parameters.rz1(), tofResult));
    } else {
        tofResult = visionModule->imageProcessing2(filename, parameters.scanY1PixelLocation2(), parameters.scanY2PixelLocation2(), parameters.scanY3PixelLocation2(), parameters.halfWidth2(), parameters.intensityCorrectionFactor1(), grabFromCamera);
        x = mathExpression(convertFormulaFromTOFResult(this->parameters.x2(), tofResult));
        y = mathExpression(convertFormulaFromTOFResult(this->parameters.y2(), tofResult));
        z = mathExpression(convertFormulaFromTOFResult(this->parameters.z2(), tofResult));
        rx = mathExpression(convertFormulaFromTOFResult(this->parameters.rx2(), tofResult));
        ry = mathExpression(convertFormulaFromTOFResult(this->parameters.ry2(), tofResult));
        rz = mathExpression(convertFormulaFromTOFResult(this->parameters.rz2(), tofResult));
    }

    tof_x = x;
    tof_y = y;
    tof_z = z;
    tof_rx = rx;
    tof_ry = ry;
    tof_rz = rz;

    if (enable_motion) {
        if (lsut->sut_carrier->motor_x == Q_NULLPTR ||
            lsut->sut_carrier->motor_y == Q_NULLPTR) {
            qWarning("SUT carrier motors is null, motors cannot move");
        } else {
            if ( fabs(x) > parameters.xLimit() ||
                 fabs(y) > parameters.yLimit() ||
                 fabs(z) > parameters.zLimit() ||
                 fabs(rx) > parameters.rxLimit() ||
                 fabs(ry) > parameters.ryLimit() ||
                 fabs(rz) > parameters.rzLimit()) {
                qWarning("The calculated movement is larger than spec, motors cannot move");
            } else {
                this->lsut->stepMove_XY_Sync(x, y);
                this->lsut->sut_carrier->StepMove_Z(z);
                aa_head->stepInterpolation_AB_Sync(rx, ry);
            }
        }
    }

    map.insert("--timeStamp", getCurrentDateString().append("-").append(getCurrentTimeString()));

    map.insert("inital_sut_x", this->lsut->sut_carrier->GetFeedBackPos().X);
    map.insert("inital_sut_y", this->lsut->sut_carrier->GetFeedBackPos().Y);
    map.insert("inital_sut_z", this->lsut->sut_carrier->GetFeedBackPos().Z);
    map.insert("inital_aa_a", this->aa_head->GetFeedBack().A);
    map.insert("inital_aa_b", this->aa_head->GetFeedBack().B);
    map.insert("inital_aa_c", this->aa_head->GetFeedBack().C);

    map.insert("a", tofResult.a);
    map.insert("b", tofResult.b);
    map.insert("c", tofResult.c);
    map.insert("d", tofResult.d);
    map.insert("e", tofResult.e);
    map.insert("f", tofResult.f);
    map.insert("g", tofResult.g);
    map.insert("h", tofResult.h);
    map.insert("i", tofResult.i);
    map.insert("j", tofResult.j);
    map.insert("k", tofResult.k);
    map.insert("l", tofResult.l);

    map.insert("x", x);
    map.insert("y", y);
    map.insert("z", z);
    map.insert("rx", rx);
    map.insert("ry", ry);
    map.insert("rz", rz);

    map.insert("method", method);
    map.insert("result", tofResult.ret);
    map.insert("-imageName", tofResult.imageName);
    map.insert("timeElapsed", timer.elapsed());
    qInfo("Finish TOF. Time elapsed: %d", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "TOF", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performMTF(QJsonValue params, bool write_log)
{
    QVariantMap map;
    double cc_min_sfr = params["CC"].toDouble(-1);
    double ul_min_sfr = params["UL"].toDouble(-1);
    double ur_min_sfr = params["UR"].toDouble(-1);
    double ll_min_sfr = params["LL"].toDouble(-1);
    double lr_min_sfr = params["LR"].toDouble(-1);
    map.insert("cc_min_sfr_spec", cc_min_sfr);
    map.insert("ul_min_sfr_spec", ul_min_sfr);
    map.insert("ur_min_sfr_spec", ur_min_sfr);
    map.insert("ll_min_sfr_spec", ll_min_sfr);
    map.insert("lr_min_sfr_spec", lr_min_sfr);

    QElapsedTimer timer;timer.start();
    bool grabRet = false;
    cv::Mat input_img = dk->DothinkeyGrabImageCVWithAutoRetry(0, grabRet);
    if (!grabRet) {
        qInfo("MTF Cannot grab image.");
        NgSensorOrProduct();
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
    if (!blackScreenCheck(input_img)) {  return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""}; }
    double fov = calculateDFOV(input_img);
    qInfo("fov: %f max_I: %d min_area: %d max_area: %d", fov, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea() );
    if (fov == -1) {
        qCritical("Cannot calculate FOV from the grabbed image.");
        LogicNg(current_aa_ng_time);
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
    //std::vector<AA_Helper::patternAttr> patterns = AA_Helper::AAA_Search_MTF_Pattern_Ex(input_img, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea(), -1);
    std::vector<AA_Helper::patternAttr> patterns1 = AA_Helper::AAA_Search_MTF_Pattern_Ex(input_img, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea(), -1);
    std::vector<AA_Helper::patternAttr> patterns2 = AA_Helper::AAA_Search_MTF_Pattern_Ex(input_img, parameters.MaxIntensity2(), parameters.MinArea(), parameters.MaxArea(), -1);
    std::vector<AA_Helper::patternAttr> patterns;

    if (patterns2.size() < patterns1.size()) {
        patterns2.swap(patterns1);
    }

    if (patterns2.size() >= patterns1.size()) {
        for (uint i = 0; i < patterns2.size(); i++) {
            patterns.push_back(patterns2[i]);
        }
        for (uint i = 0; i < patterns1.size(); i++) {
            bool isDuplicated = false;
            for (uint j = 0; j < patterns2.size(); j++) {
                double positionDiff = sqrt(pow(patterns1[i].center.x()-patterns2[j].center.x(), 2) + pow(patterns1[i].center.y()-patterns2[j].center.y(),2));
                if (positionDiff < 20) { //threshold diff
                    isDuplicated = true;
                }
            }
            if (!isDuplicated) {
                patterns.push_back(patterns1[i]);
            }
        }
    }
    double rect_width = 0;
    double imageCenterX = input_img.cols/2;
    double imageCenterY = input_img.rows/2;
    double r1 = sqrt(imageCenterX*imageCenterX + imageCenterY*imageCenterY);
    std::vector<MTF_Pattern_Position> vec;
    for (uint i = 0; i < patterns.size(); i++) {
       //Crop ROI
       {
           cv::Rect roi; cv::Mat copped_roi;
           double width = sqrt(patterns[i].area)/2;
           rect_width = width;
           roi.width = width*4; roi.height = width*4;
           roi.x = patterns[i].center.x() - width*2;
           roi.y = patterns[i].center.y() - width*2;
           if (roi.x < 0) roi.x = 0;
           if (roi.x + roi.width > input_img.cols) { roi.width = input_img.cols - roi.x; }
           if (roi.y < 0) roi.y = 0;
           if (roi.y + roi.height > input_img.rows) { roi.height = input_img.rows - roi.y; }
           input_img(roi).copyTo(copped_roi);
           double radius = sqrt(pow(patterns[i].center.x() - imageCenterX, 2) + pow(patterns[i].center.y() - imageCenterY, 2));
           double f = radius/r1;
           double t_sfr = 0, r_sfr = 0, b_sfr = 0, l_sfr = 0;
           bool ret = sfr::sfr_calculation_single_pattern(copped_roi, t_sfr, r_sfr, b_sfr, l_sfr, 8*(parameters.mtfFrequency() + 1));
           if (!ret) {
               qWarning("Cannot calculate MTF in the detected pattern");
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
           }
           double avg_sfr = ( t_sfr + r_sfr + b_sfr + l_sfr)/4;
           vec.emplace_back(patterns[i].center.x(), patterns[i].center.y(),
                            f, t_sfr*100, r_sfr*100, b_sfr*100, l_sfr*100, patterns[i].area, avg_sfr);
        }
    }

    vector<int> layers = sfr::classifyLayers(vec);
    int max_layer = 0;
    QImage qImage = ImageGrabbingWorkerThread::cvMat2QImage(input_img);
    QPainter qPainter(&qImage);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setFont(QFont("Times", parameters.drawTextSize(), QFont::Light));

    for (size_t i = 0; i < vec.size(); i++) {
        if (vec.at(i).layer > max_layer) {
            max_layer = vec.at(i).layer - 1;
        }
        QVariantMap data;
        data.insert("x", vec[i].x);
        data.insert("y", vec[i].y);
        data.insert("sfr", vec[i].avg_sfr);
        qPainter.setPen(QPen(Qt::blue, 4.0));
        qPainter.drawText(vec[i].x - rect_width/2, vec[i].y - rect_width*2, QString::number(vec[i].t_sfr, 'g', 4));
        qPainter.drawText(vec[i].x + rect_width, vec[i].y,  QString::number(vec[i].r_sfr, 'g', 4));
        qPainter.drawText(vec[i].x - rect_width, vec[i].y + rect_width*3,  QString::number(vec[i].b_sfr, 'g', 4));
        qPainter.drawText(vec[i].x - rect_width*4, vec[i].y,  QString::number(vec[i].l_sfr, 'g', 4));
    }
    qPainter.end();
    sfrImageReady(std::move(qImage));
    bool sfr_check = true;
    if (vec.size() > 0) {
        if (vec[0].t_sfr < cc_min_sfr || vec[0].r_sfr < cc_min_sfr || vec[0].b_sfr < cc_min_sfr || vec[0].l_sfr < cc_min_sfr) {
            qWarning("CC cannot pass");
            map.insert("error", "CC cannot pass");
            sfr_check = false;
        }
    } else {
        qWarning("cannot find CC");
        map.insert("error", "cannot find CC");
        sfr_check = false;
    }
    if (vec.size() >= max_layer*4 + 1) {
        if (vec[max_layer*4 + 1].t_sfr < ul_min_sfr || vec[max_layer*4 + 1].r_sfr < ul_min_sfr ||
            vec[max_layer*4 + 1].b_sfr < ul_min_sfr || vec[max_layer*4 + 1].l_sfr < ul_min_sfr) {
            qWarning("UL cannot pass");
            map.insert("error", "UL cannot pass");
            sfr_check = false;
        }
    } else {
        qWarning("cannot find UL");
        map.insert("error", "cannot find UL");
        sfr_check = false;
    }

    if (vec.size() >= max_layer*4 + 2) {
        if (vec[max_layer*4 + 2].t_sfr < ll_min_sfr || vec[max_layer*4 + 2].r_sfr < ll_min_sfr ||
            vec[max_layer*4 + 2].b_sfr < ll_min_sfr || vec[max_layer*4 + 2].l_sfr < ll_min_sfr) {
            qWarning("LL cannot pass");
            map.insert("error", "LL cannot pass");
            sfr_check = false;
        }
    } else {
        qWarning("cannot find LL");
        map.insert("error", "cannot find LL");
        sfr_check = false;
    }

    if (vec.size() >= max_layer*4 + 3) {
       if (vec[max_layer*4 + 3].t_sfr < lr_min_sfr || vec[max_layer*4 + 3].r_sfr < lr_min_sfr ||
           vec[max_layer*4 + 3].b_sfr < lr_min_sfr || vec[max_layer*4 + 3].l_sfr < lr_min_sfr) {
            qWarning("LR cannot pass");
            map.insert("error", "LR cannot pass");
            sfr_check = false;
        }
    } else {
        qWarning("cannot find LR");
        map.insert("error", "cannot find LR");
        sfr_check = false;
    }

    if (vec.size() >= max_layer*4 + 4) {
        if (vec[max_layer*4 + 4].t_sfr < ur_min_sfr || vec[max_layer*4 + 4].r_sfr < ur_min_sfr ||
            vec[max_layer*4 + 4].b_sfr < ur_min_sfr || vec[max_layer*4 + 4].l_sfr < ur_min_sfr) {
            qWarning("UR cannot pass");
            map.insert("error", "UR cannot pass");
            sfr_check = false;
        }
    }

    map.insert("FOV",fov);
    map.insert("zPeak",lsut->sut_carrier->GetFeedBackPos().Z);
    if (vec.size() > 0) {
        map.insert("CC_T_SFR", vec[0].t_sfr);
        map.insert("CC_R_SFR", vec[0].r_sfr);
        map.insert("CC_B_SFR", vec[0].b_sfr);
        map.insert("CC_L_SFR", vec[0].l_sfr);
        map.insert("CC_SFR", (vec[0].t_sfr + vec[0].r_sfr + vec[0].b_sfr + vec[0].l_sfr)/4);
    }
    if (vec.size() >= max_layer*4 + 1) {
        map.insert("UL_T_SFR", vec[max_layer*4 + 1].t_sfr);
        map.insert("UL_R_SFR", vec[max_layer*4 + 1].r_sfr);
        map.insert("UL_B_SFR", vec[max_layer*4 + 1].b_sfr);
        map.insert("UL_L_SFR", vec[max_layer*4 + 1].l_sfr);
        map.insert("UL_SFR", (vec[max_layer*4 + 1].t_sfr + vec[max_layer*4 + 1].r_sfr + vec[max_layer*4 + 1].b_sfr + vec[max_layer*4 + 1].l_sfr)/4);
    }
    if (vec.size() >= max_layer*4 + 2) {
        map.insert("LL_T_SFR", vec[max_layer*4 + 2].t_sfr);
        map.insert("LL_R_SFR", vec[max_layer*4 + 2].r_sfr);
        map.insert("LL_B_SFR", vec[max_layer*4 + 2].b_sfr);
        map.insert("LL_L_SFR", vec[max_layer*4 + 2].l_sfr);
        map.insert("LL_SFR", (vec[max_layer*4 + 2].t_sfr + vec[max_layer*4 + 2].r_sfr + vec[max_layer*4 + 2].b_sfr + vec[max_layer*4 + 2].l_sfr)/4);
    }
    if (vec.size() >= max_layer*4 + 3) {
        map.insert("LR_T_SFR", vec[max_layer*4 + 3].t_sfr);
        map.insert("LR_R_SFR", vec[max_layer*4 + 3].r_sfr);
        map.insert("LR_B_SFR", vec[max_layer*4 + 3].b_sfr);
        map.insert("LR_L_SFR", vec[max_layer*4 + 3].l_sfr);
        map.insert("LR_SFR", (vec[max_layer*4 + 3].t_sfr + vec[max_layer*4 + 3].r_sfr + vec[max_layer*4 + 3].b_sfr + vec[max_layer*4 + 3].l_sfr)/4);
    }
    if (vec.size() >= max_layer*4 + 4) {
        map.insert("UR_T_SFR", vec[max_layer*4 + 4].t_sfr);
        map.insert("UR_R_SFR", vec[max_layer*4 + 4].r_sfr);
        map.insert("UR_B_SFR", vec[max_layer*4 + 4].b_sfr);
        map.insert("UR_L_SFR", vec[max_layer*4 + 4].l_sfr);
        map.insert("UR_SFR", (vec[max_layer*4 + 4].t_sfr + vec[max_layer*4 + 4].r_sfr + vec[max_layer*4 + 4].b_sfr + vec[max_layer*4 + 4].l_sfr)/4);
    }
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "MTF", map);

    if (sfr_check) {
       return ErrorCodeStruct{ErrorCode::OK, ""};
    } else {
       LogicNg(current_aa_ng_time);
       return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
}

ErrorCodeStruct AACoreNew::performUV(int uv_time)
{
    hasUV = true;
    qInfo("Perform UV uv time: %d", uv_time);
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    aa_head->openUVTillTime(uv_time);
    map.insert("timeElapsed", timer.elapsed());
    map.insert("result", "ok");
    emit pushDataToUnit(this->runningUnit, "UV", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performReject()
{
    QVariantMap map;
    imageThread->stop();
    imageThread->wait();
    imageThread->exit();
    dk->stopCamera();
    map.insert("has_ng_lens", has_ng_lens);
    map.insert("has_ng_sensor", has_ng_sensor);
    map.insert("has_product", has_product);
    map.insert("has_sensor", has_sensor);
    map.insert("has_lens", has_lens);
    emit pushDataToUnit(this->runningUnit, "Reject", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performAccept()
{
    imageThread->stop();
    imageThread->wait();
    imageThread->exit();
    dk->stopCamera();
    current_aa_ng_time = 0;
//    current_oc_ng_time = 0;
//    current_mtf_ng_time = 0;
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performTerminate()
{
    imageThread->stop();
    imageThread->wait();
    imageThread->exit();
    dk->stopCamera();

    QVariantMap map;
    map.insert("has_ng_lens", has_ng_lens);
    map.insert("has_ng_sensor", has_ng_sensor);
    map.insert("has_product", has_product);
    map.insert("has_sensor", has_sensor);
    map.insert("has_lens", has_lens);
    emit pushDataToUnit(this->runningUnit, "Termiate", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performGRR(bool change_lens,bool change_sensor,int repeat_time,int change_time)
{
    if(!change_lens)
        SetLens();
    if(!change_sensor)
        SetSensor();
   if(grr_repeat_time >= repeat_time)
   {
       grr_repeat_time = 0;
       grr_change_time++;
       if(grr_change_time >= change_time)
       {
           grr_change_time = 0;
           sendAlarmMessage(ErrorLevel::ErrorMustStop,"GRR Finished!");
       }
       if(change_lens&&HasLens())
           NgLens();
       if(change_sensor&&HasSensorOrProduct())
           NgSensor();
   }
   grr_repeat_time++;
   return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performYLevelTest(QJsonValue params)
{
    int enable_plot = params["enable_plot"].toInt();
//    cv::Mat inputImage = cv::imread("1/5.bmp");
    bool grabRet;
    cv::Mat inputImage = dk->DothinkeyGrabImageCVWithAutoRetry(0, grabRet);
    if (!grabRet) {
        qInfo("Cannot grab image.");
        NgSensorOrProduct();
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Y Level Test Fail. Cannot grab image"};
    }
    QElapsedTimer timer; timer.start();
    vector<float> intensityProfile; float min_i = 0; float max_i = 0; int detectedError = 0; float negative_di = 0; float positive_di = 0;
    bool ret = AA_Helper::calculateImageIntensityProfile(inputImage, min_i, max_i, intensityProfile, 0, 0, detectedError, negative_di, positive_di);
    if (ret) {
        qInfo("performYLevelTest Success. Min I: %f Max I: %f size: %d", min_i, max_i, intensityProfile.size());
        if (enable_plot == 1) {
            intensity_profile.clear();
            this->intensity_profile.plotIntensityProfile(min_i, max_i, intensityProfile);
        }
        if (max_i < 10) {
            qInfo("This is black screen.");
            return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Y Level Fail. Black screen detected"};
        }
        qInfo("Time elapsed: %d", timer.elapsed());
        return ErrorCodeStruct{ErrorCode::OK, ""};
    } else {
        qInfo("performYLevelTest Fail");
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Y Level Fail. Cannot grab image"};
    }
}

bool AACoreNew::blackScreenCheck(cv::Mat inImage)
{
    vector<float> intensityProfile; float min_i = 0; float max_i = 0; int detectedError = 0; float negative_di = 0; float positive_di = 0;
    bool ret = AA_Helper::calculateImageIntensityProfile(inImage, min_i, max_i, intensityProfile, 0, 0, detectedError, negative_di, positive_di);
    if (ret) {
        qInfo("[blackScreenCheck] Checking intensity...min: %f max: %f", min_i, max_i);
        if (max_i < 10) {
            qInfo("Detect black screen");
            return false;
        }
        return true;
    } else {
        qInfo("Check intensity fail");
        return false;
    }
}

ErrorCodeStruct AACoreNew::performOC(QJsonValue params)
{
    bool enableMotion = params["enable_motion"].toInt();
    bool fastMode = params["fast_mode"].toInt();
    ErrorCodeStruct ret = { ErrorCode::OK, "" };
    QVariantMap map;
    QElapsedTimer timer;
    timer.start();
    bool grabRet;
    cv::Mat img = dk->DothinkeyGrabImageCVWithAutoRetry(0, grabRet);
    if (!grabRet) {
        qInfo("AA Cannot grab image.");
        NgSensorOrProduct();
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
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
            LogicNg(current_aa_ng_time);
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
            LogicNg(current_aa_ng_time);
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
            LogicNg(current_aa_ng_time);
            qInfo("OC result too big (x:%f,y:%f) pixel:(%f,%f) cmosPixelToMM (x:)%f,%f) ",stepY,stepY,offsetX,offsetY,x_ratio.x(),x_ratio.y());
            return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "OC step too large" };
        }
        this->lsut->stepMove_XY_Sync(-stepX, -stepY);
    }
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "OC", map);
    qInfo("Finish OC");
    return ret;
}

ErrorCodeStruct AACoreNew::performIRCameraCapture(QJsonValue params)
{
    qInfo("Perform IR Camera Capture");
    QString directory = params["directory"].toString();
    QString command = params["command"].toString();
    int delay = params["delay"].toInt(0);
    int width = params["width"].toInt(0);
    int height = params["height"].toInt(0);
    QString dataFilename = params["filename"].toString();
    bool save_image = params["save_image"].toInt();
    QString imageFilename = params["image_file_name"].toString();
    performCommand(params);   // TO Be test
    QThread::msleep(delay);

    qInfo("Perform init sensor");
    QFile file;
    file.setFileName(dataFilename);
    cv::Mat mat(height, width, CV_8UC3);
    int rows = 0;
    if (file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&file);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList list = line.split(QRegExp(","), QString::SkipEmptyParts);
          for (int i = 0; i < list.size(); i++) {
              mat.at<cv::Vec3b>(rows, i)[0] = list[i].toInt();
              mat.at<cv::Vec3b>(rows, i)[1] = list[i].toInt();
              mat.at<cv::Vec3b>(rows, i)[2] = list[i].toInt();
          }
          rows++;
       }
       file.close();
    }
    QImage outputImage = ImageGrabbingWorkerThread::cvMat2QImage(mat);
    if (save_image)
    {
        cv::imwrite(imageFilename.toStdString(), mat);
    }

    ocImageProvider_1->setImage(outputImage);
    emit callQmlRefeshImg(1);
    return ErrorCodeStruct {ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performMTF_HW(QJsonValue params)
{
    QString imageFilename = params["image_file_name"].toString();

    QElapsedTimer timer;timer.start();
    bool grabRet = false;
    cv::Mat input_img = cv::imread(imageFilename.toStdString());

    if (!blackScreenCheck(input_img)) {  return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""}; }
    double fov = calculateDFOV(input_img);
    qInfo("fov: %f max_I: %d min_area: %d max_area: %d", fov, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea() );
    if (fov == -1) {
        qCritical("Cannot calculate FOV from the grabbed image.");
        LogicNg(current_aa_ng_time);
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
    //std::vector<AA_Helper::patternAttr> patterns = AA_Helper::AAA_Search_MTF_Pattern_Ex(input_img, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea(), -1);
    std::vector<AA_Helper::patternAttr> patterns1 = AA_Helper::AAA_Search_MTF_Pattern_Ex(input_img, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea(), -1);
    std::vector<AA_Helper::patternAttr> patterns2 = AA_Helper::AAA_Search_MTF_Pattern_Ex(input_img, parameters.MaxIntensity2(), parameters.MinArea(), parameters.MaxArea(), -1);
    std::vector<AA_Helper::patternAttr> patterns;

    if (patterns2.size() < patterns1.size()) {
        patterns2.swap(patterns1);
    }

    if (patterns2.size() >= patterns1.size()) {
        for (uint i = 0; i < patterns2.size(); i++) {
            patterns.push_back(patterns2[i]);
        }
        for (uint i = 0; i < patterns1.size(); i++) {
            bool isDuplicated = false;
            for (uint j = 0; j < patterns2.size(); j++) {
                double positionDiff = sqrt(pow(patterns1[i].center.x()-patterns2[j].center.x(), 2) + pow(patterns1[i].center.y()-patterns2[j].center.y(),2));
                if (positionDiff < 20) { //threshold diff
                    isDuplicated = true;
                }
            }
            if (!isDuplicated) {
                patterns.push_back(patterns1[i]);
            }
        }
    }
    double rect_width = 0;
    double imageCenterX = input_img.cols/2;
    double imageCenterY = input_img.rows/2;
    double r1 = sqrt(imageCenterX*imageCenterX + imageCenterY*imageCenterY);
    std::vector<MTF_Pattern_Position> vec;
    for (uint i = 0; i < patterns.size(); i++) {
       //Crop ROI
       {
           cv::Rect roi; cv::Mat copped_roi;
           double width = sqrt(patterns[i].area)/2;
           rect_width = width;
           roi.width = width*4; roi.height = width*4;
           roi.x = patterns[i].center.x() - width*2;
           roi.y = patterns[i].center.y() - width*2;
           if (roi.x < 0) roi.x = 0;
           if (roi.x + roi.width > input_img.cols) { roi.width = input_img.cols - roi.x; }
           if (roi.y < 0) roi.y = 0;
           if (roi.y + roi.height > input_img.rows) { roi.height = input_img.rows - roi.y; }
           input_img(roi).copyTo(copped_roi);
           double radius = sqrt(pow(patterns[i].center.x() - imageCenterX, 2) + pow(patterns[i].center.y() - imageCenterY, 2));
           double f = radius/r1;
           double t_sfr = 0, r_sfr = 0, b_sfr = 0, l_sfr = 0;
           bool ret = sfr::sfr_calculation_single_pattern(copped_roi, t_sfr, r_sfr, b_sfr, l_sfr, 8*(parameters.mtfFrequency() + 1));
           if (!ret) {
               qWarning("Cannot calculate MTF in the detected pattern");
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
           }
           double avg_sfr = ( t_sfr + r_sfr + b_sfr + l_sfr)/4;
           vec.emplace_back(patterns[i].center.x(), patterns[i].center.y(),
                            f, t_sfr*100, r_sfr*100, b_sfr*100, l_sfr*100, patterns[i].area, avg_sfr);
        }
    }

    vector<int> layers = sfr::classifyLayers(vec);
    int max_layer = 0;
    QImage qImage = ImageGrabbingWorkerThread::cvMat2QImage(input_img);
    QPainter qPainter(&qImage);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setFont(QFont("Times", parameters.drawTextSize(), QFont::Light));

    for (size_t i = 0; i < vec.size(); i++) {
        if (vec.at(i).layer > max_layer) {
            max_layer = vec.at(i).layer - 1;
        }
        QVariantMap data;
        data.insert("x", vec[i].x);
        data.insert("y", vec[i].y);
        data.insert("sfr", vec[i].avg_sfr);
        qPainter.setPen(QPen(Qt::blue, 4.0));
        qPainter.drawText(vec[i].x - rect_width/2, vec[i].y - rect_width*2, QString::number(vec[i].t_sfr, 'g', 4));
        qPainter.drawText(vec[i].x + rect_width, vec[i].y,  QString::number(vec[i].r_sfr, 'g', 4));
        qPainter.drawText(vec[i].x - rect_width, vec[i].y + rect_width*3,  QString::number(vec[i].b_sfr, 'g', 4));
        qPainter.drawText(vec[i].x - rect_width*4, vec[i].y,  QString::number(vec[i].l_sfr, 'g', 4));
    }
    qPainter.end();
    sfrImageReady(std::move(qImage));

    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performInitSensor()
{
    if(!has_sensor) return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "has no sensor"};
    if (dk->DothinkeyIsGrabbing()) {
        qInfo("Sensor have already init");
        return ErrorCodeStruct {ErrorCode::OK, ""};
    }
    QElapsedTimer timer, stepTimer; timer.start(); stepTimer.start();
    QVariantMap map;
    const int channel = 0;

    if(needReInitFrameGrabber)
    {
        if(!dk->initDevice())
        {
            UIOperation::getIns()->showMessage("Error", "Can not init frame grabber. Please check!", MsgBoxIcon::Error, OkBtn);
        }
        else {
            needReInitFrameGrabber = false;
        }
    }

    if(!dk->startCamera(channel))
    {
        qCritical("Cannot start camera");
        NgSensor();
        needReInitFrameGrabber = true;
        return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "StartCameraFailed"};
    }
    bool res = false;
    dk->DothinkeyGrabImageCV(channel, res);
    if(!res)
    {
        qCritical("Cannot grab image");
        NgSensor();
        needReInitFrameGrabber = true;
        return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "GrabImageFailed"};
    }
    map.insert("dothinkeyStartCamera", stepTimer.elapsed()); stepTimer.restart();

//    QString sensorID = dk->readSensorID();
//    qInfo("performInitSensor sensor ID: %s", sensorID.toStdString().c_str());
//    map.insert("sensorID", sensorID);
    if (!imageThread->isRunning())
        imageThread->start();
    map.insert("timeElapsed", timer.elapsed());
    map.insert("result", "OK");
    emit pushDataToUnit(runningUnit, "InitSensor", map);
    return ErrorCodeStruct {ErrorCode::OK, ""};
}

// TO BE MODIFIED
ErrorCodeStruct AACoreNew::performPRToBond()
{
    QElapsedTimer timer, stepTimer; timer.start(); stepTimer.start();
    QVariantMap map;
    sensorDownlookOffset.ReSet();
    if(!lsut->moveToDownlookPR(sensorDownlookOffset)) { NgSensor(); return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "moveToDownlookPR"}; }
    qInfo("downlook Pr Offset x: %f y: %f t: %f", sensorDownlookOffset.X, sensorDownlookOffset.Y, sensorDownlookOffset.Theta);
    map.insert("moveToDownlookPR", stepTimer.elapsed()); stepTimer.restart();
    if (!aaHeadPickLens()) { NgLens(); return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA pick lens fail"};}
    map.insert("aa_gripLens", stepTimer.elapsed()); stepTimer.restart();
    //華為AA暫時comment
//    PrOffset uplookPrOffset;
//    if(!lsut->moveToUplookPR(uplookPrOffset)){ NgLens(); return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "moveToUplookPR"}; }
//    map.insert("moveToUplookPR", stepTimer.elapsed()); stepTimer.restart();

//    double thetaOffset = -uplookPrOffset.Theta - sensorDownlookOffset.Theta + aa_head->bondOffset.Theta();

//    if (!this->aa_head->moveToMushroomPosWithCOffset(thetaOffset)) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA cannot move to mushroom Pos"};}
//    map.insert("aa_head_moveToMushroomPosition", stepTimer.elapsed()); stepTimer.restart();
    if(!this->lsut->moveToMushroomPosition(true)) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "moveToMushroomPosition"}; }
    map.insert("lsut_moveToMushroomPosition", stepTimer.elapsed()); stepTimer.restart();
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "PrToBond", map);
    return ErrorCodeStruct {ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performLoadLens()
{
    qInfo("perform load lens");
    QElapsedTimer timer; timer.start();
    QVariantMap map;

    this->lsut->moveToLoadSensorPosition(true);      //LSUT Move to load position

    this->materialLoader->semiAutoPickLensAndLoadLens(); // Pickarm pick lens + pickarm place lens to LSUT

    this->aa_head->moveToPickLensPositionSync();    // AA head move to pick lens position

    this->aa_head->moveAAHead_XYZToPos();   // AA head move to XYZ position

    this->aaHeadPickLens(); // LSUT move to pick lens position and AA gripper grip lens

    PrOffset uplookPrOffset;
    this->lsut->moveToUplookPR(uplookPrOffset,false,true);  //LSUT move to uplook pr position and perform uplook pr

    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "LoadLens", map);
    return  ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performUnloadLens()
{
    qInfo("perform unload lens");
    QElapsedTimer timer; timer.start();
    QVariantMap map;

    aa_head->moveToPickLensPositionSync();  // AA head move to unpick lens position

    lsut->unpickLens(); // LSUT and AA head unpick lens

    lsut->moveToLoadSensorPosition();  //LSUT move to load position

    this->materialLoader->semiAutoPickNgLensAndPlaceToTray();  //pick arm pick ng lens + pickarm place ng lens

    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "UnloadLens", map);
    return  ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performLoadSensor()
{
    qInfo("perform load Sensor");
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    this->lsut->moveToLoadSensorPosition();
    //pickarm pick sensor
    //pickarm place sensor to LSUT
    //sut perfrom downlook pr
    //sut move to mushroom position
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "LoadSensor", map);
    return  ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performUnloadSensor()
{
    qInfo("perform unload sensor");
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    lsut->moveToLoadSensorPosition(); //LSUT move to load position
    lsut->pogopin->Set(false); // Pogopin presser open
    //pickarm pick ng sensor + place ng sensor
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "UnloadSensor", map);
    return  ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performUnloadProduct()
{
    qInfo("perform unload product");
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    lsut->aa_head->openGripper();     //open gribber
    lsut->moveToLoadSensorPosition(); //Move to load sensor position
    lsut->pogopin->Set(false); // Pogopin presser open
    //pickarm pick product
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "UnloadProduct", map);
    return  ErrorCodeStruct{ErrorCode::OK, ""};
}

// TO BE MODIFIED
ErrorCodeStruct AACoreNew::performAAPickLens()
{
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    if (!aaHeadPickLens()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA pick lens fail"};}
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "AAPickLens", map);
    return  ErrorCodeStruct{ErrorCode::OK, ""};
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
    double curr_z = lsut->sut_carrier->GetFeedBackPos().Z;
    double target_z = lsut->sut_carrier->GetFeedBackPos().Z + zOffset;
    lsut->moveToZPos(target_z);
    QThread::msleep(200);
    double final_z = lsut->sut_carrier->GetFeedBackPos().Z;
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
    mPoint3D ori_pos(0, 1, 2);
    mPoint3D final_pos(3, 4, 5);
//    mPoint3D ori_pos = sut->carrier->GetFeedBackPos();
//    sut->stepMove_XY_Sync(xOffset, yOffset);
//    QThread::msleep(200);
//    mPoint3D final_pos = sut->carrier->GetFeedBackPos();
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
    std::vector<AA_Helper::patternAttr> vector = AA_Helper::AAA_Search_MTF_Pattern_Ex(img, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea(), 1);
    if (vector.size() == 4) {
        double d1 = sqrt(pow((vector[0].center.x() - vector[2].center.x()), 2) + pow((vector[0].center.y() - vector[2].center.y()), 2));
        double d2 = sqrt(pow((vector[3].center.x() - vector[1].center.x()), 2) + pow((vector[3].center.y() - vector[1].center.y()), 2));
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
    clustered_sfr_map[index] = std::move(sfrs);
    qInfo("Received sfr result from index: %d timeElapsed: %d size: %d", index, timeElapsed, clustered_sfr_map.size());

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
    emit callQmlRefeshImg(0);
}

void AACoreNew::captureLiveImage()
{
    if(!dk->DothinkeyIsGrabbing()) {
        qInfo("Image Grabber is not ON");
        UIOperation::getIns()->showMessage("AA Core", QString("Save Image Fail! Image Grabber is not open"), MsgBoxIcon::Error, "OK");
        return;
    }
    bool grabRet = false;
    cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
    if (!grabRet) {
        UIOperation::getIns()->showMessage("AA Core", QString("Save Image Fail! Image Grabber is not open"), MsgBoxIcon::Error, "OK");
        qWarning("AA Cannot grab image.");
        return;
    } else {
        cv::imwrite("livePhoto.bmp", img);
        UIOperation::getIns()->showMessage("AA Core", QString("Save Image Success! You can start AA Core parameter debug."), MsgBoxIcon::Information, "OK");
    }
}
