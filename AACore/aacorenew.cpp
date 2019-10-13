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
                     DispenseModule *dispense, ImageGrabbingWorkerThread *imageThread, Unitlog *unitlog)
{
    this->aa_head = aa_head;
    this->lsut = lsut;
    this->dk = dk;
    this->chartCalibration = chartCalibration;
    this->dispense = dispense;
    this->imageThread = imageThread;
    this->unitlog = unitlog;
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

void AACoreNew::receiveStartAAProcessRequest()
{
    qInfo(__FUNCTION__);
    this->has_sensor = true;
    this->has_ng_sensor = true;
    this->has_lens = true;
    this->has_ng_lens = true;
    this->has_product = false;
    this->has_ng_product = false;
    start_process = true;
}

void AACoreNew::run(bool has_material)
{
    qInfo("Start AACore Thread");
    is_run = true;
    while(is_run) {
        if (start_process)
        {
            runningUnit = this->unitlog->createUnit();
            runFlowchartTest();
            start_process = false;
            emit postDataToELK(this->runningUnit);
            emit sendAAProcessResponse(has_ng_sensor, has_ng_lens, has_product, has_ng_product);
        }
        QThread::msleep(100);
    }
    qInfo("End of thread");
}

void AACoreNew::LogicNg(int &ng_time)
{
    qInfo("LogicNg ng_time:%d",ng_time);
    if(has_product)
    {
        has_ng_product = true;
        has_product = false;
        return;
    }
    has_ng_lens = true;
    has_ng_sensor = true;
    has_lens = false;
    has_sensor = false;
//    if(parameters.firstRejectSensor())
//    {
//        if(ng_time >= parameters.rejectTimes())
//        {
//            ng_time = 0;
//            has_ng_lens = true;
//            has_lens = false;
//        }
//        else
//        {
//            has_ng_sensor = true;
//            has_sensor = false;
//            ng_time++;
//        }
//    }
//    else
//    {
//        if(ng_time >= parameters.rejectTimes())
//        {
//            ng_time = 0;
//            has_ng_sensor = true;
//            has_sensor = false;
//        }
//        else
//        {
//            has_ng_lens = true;
//            has_lens = false;
//            ng_time++;
//        }
//    }
}

void AACoreNew::NgLens()
{
    qInfo("NgLens");
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
    qInfo("NgSensor");
    has_sensor = false;
    has_ng_sensor = true;
    has_product = false;
    has_ng_product = false;
    if(!parameters.firstRejectSensor())
    {
        current_aa_ng_time = 0;
        current_oc_ng_time = 0;
        current_mtf_ng_time = 0;
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
    has_product = false;
    has_ng_product = true;
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
        runningUnit = this->unitlog->createUnit();
        runFlowchartTest();
        emit postDataToELK(this->runningUnit);
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
    send_lens_request = false;
    send_sensor_request = false;
    aa_head->waiting_sensor = false;
    aa_head->waiting_lens = false;
    current_aa_ng_time = 0;
    current_oc_ng_time = 0;
    current_mtf_ng_time = 0;
    grr_repeat_time = 0;
    grr_change_time = 0;
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
        NgSensor();
        NgLens();
        return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, ""};
    }
}

ErrorCodeStruct AACoreNew::performDispense()
{
    qInfo("Performing Dispense");
    has_product = true;
    has_lens = false;
    has_sensor = false;
    has_ng_lens = false;
    has_ng_sensor = false;
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    lsut->recordCurrentPos();
    PrOffset offset;
    dispense->setMapPosition(lsut->downlook_position.X(),lsut->downlook_position.Y());
    if(!lsut->moveToDownlookPR(offset)){ return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "downlook pr fail"};}
    dispense->setPRPosition(offset.X,offset.Y,offset.Theta);
    lsut->moveToZPos(0);
    if(!dispense->performDispense()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "dispense fail"};}
    QString imageNameAfterDispense;
    imageNameAfterDispense.append(getDispensePrLogDir())
                   .append(getCurrentTimeString())
                   .append("_")
                   .append("_after_dispense.jpg");
    lsut->moveToDownlookSaveImage(imageNameAfterDispense); // For save image only
    QImage image(imageNameAfterDispense);
    dispenseImageProvider->setImage(image);
    emit callQmlRefeshImg(3);  //Emit dispense image to QML

    if(!lsut->movetoRecordPos()){return  ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "sut move to record pos fail"};}

    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "Dispense", map);
    qInfo("Finish Dispense");
    return ErrorCodeStruct {ErrorCode::OK, ""};
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
    int resize_factor = 2;
    vector<double> fov_y; vector<double> fov_x;
    QPointF prev_point = {0, 0};
    double prev_fov_slope = 0;
    bool grabRet = true;
    if(zScanMode == ZSCAN_MODE::AA_ZSCAN_NORMAL) {
        qInfo("z scan mode");
        unsigned int count = (int)fabs((start - stop)/step_size);
        for (unsigned int i = 0; i < count; i++)
        {
           double target = start + i*step_size;
           qInfo("target: %f",target);
           this->lsut->sut_carrier->Move_Z_Sync(start+(i*step_size));
           QThread::msleep(zSleepInMs);
           qInfo("Getting feedback position from lsut");
           double realZ = lsut->sut_carrier->GetFeedBackPos().Z;
           qInfo("Z scan start from %f, real: %f", start+(i*step_size), realZ);
           QElapsedTimer grabTimer; grabTimer.start();
           cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
           qInfo("Grab time elapsed: %d", grabTimer.elapsed());
           if (!grabRet) {
               qInfo("AA Cannot grab image.");
               map["Result"] = "AA Cannot grab image.";
               emit pushDataToUnit(runningUnit, "AA", map);
               LogicNg(current_aa_ng_time);
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
           emit sfrWorkerController->calculate(i, start+i*step_size, dst, false, resize_factor);
           img.release();
           dst.release();
         }
      } else if (zScanMode == ZSCAN_MODE::AA_DFOV_MODE){
           lsut->sut_carrier->Move_Z_Sync(start);
           QThread::msleep(zSleepInMs);
           cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
           if (!grabRet) {
               qInfo("AA Cannot grab image.");
               LogicNg(current_aa_ng_time);
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Cannot Grab Image"};
           }
           if (!blackScreenCheck(img)) {
               LogicNg(current_aa_ng_time);
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Detect BlackScreen"};
           }
           double dfov = calculateDFOV(img);
           if (dfov <= -1) {
               qInfo("Cannot find the target FOV!");
               LogicNg(current_aa_ng_time);
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
           }
           double estimated_aa_z = (estimated_aa_fov - dfov)/estimated_fov_slope + start;
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
                lsut->sut_carrier->Move_Z_Sync(target_z+(i*step_size));
                QThread::msleep(zSleepInMs);
                cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
                if (!grabRet) {
                    qInfo("AA Cannot grab image.");
                    LogicNg(current_aa_ng_time);
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
                emit sfrWorkerController->calculate(i, realZ, dst, false, resize_factor);
                img.release();
                dst.release();
                zScanCount++;
           }
    } else if (zScanMode == ZSCAN_MODE::AA_STATIONARY_SCAN_MODE){
         double currentZ = lsut->sut_carrier->GetFeedBackPos().Z;
         double target_z = currentZ + offset_in_um;
         for (unsigned int i = 0; i < imageCount; i++) {
             lsut->sut_carrier->Move_Z_Sync(target_z+(i*step_size));
             QThread::msleep(zSleepInMs);
             cv::Mat img = dk->DothinkeyGrabImageCV(0,grabRet);
             if (!grabRet) {
                 qInfo("AA Cannot grab image.");
                 LogicNg(current_aa_ng_time);
                 return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "AA Cannot grab image"};
             }
             if (!blackScreenCheck(img)) {
                 LogicNg(current_aa_ng_time);
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
             emit sfrWorkerController->calculate(i, realZ, dst, false, resize_factor);
             img.release();
             dst.release();
             zScanCount++;
         }
    }
    int timeout=1000;
    while(this->clustered_sfr_map.size() != zScanCount && timeout >0) {
          Sleep(10);
          timeout--;
    }
    double fov_slope     = (zScanCount*xysum-xsum*ysum)/(zScanCount*x2sum-xsum*xsum);       //calculate slope
    double fov_intercept = (x2sum*ysum-xsum*xysum)/(x2sum*zScanCount-xsum*xsum);            //calculate intercept
    current_fov_slope = fov_slope;
    qInfo("fov_slope: %f fov_intercept: %f", fov_slope, fov_intercept);

    QVariantMap aa_result = sfrFitCurve_Advance(resize_factor, start);

    qInfo("Layer 1 xTilt : %f yTilt: %f ", aa_result["xTilt_1"].toDouble(), aa_result["yTilt_1"].toDouble());
    qInfo("Layer 2 xTilt : %f yTilt: %f ", aa_result["xTilt_2"].toDouble(), aa_result["yTilt_2"].toDouble());
    qInfo("Layer 3 xTilt : %f yTilt: %f ", aa_result["xTilt_3"].toDouble(), aa_result["yTilt_3"].toDouble());
    bool aaResult = aa_result["OK"].toBool();
    if (!aaResult) {
        LogicNg(current_aa_ng_time);
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Perform AA fail"};
    }
    lsut->sut_carrier->Move_Z_Sync(aa_result["zPeak"].toDouble());
    qInfo("Expected Z Peak: %f SUT Actual Z: %f", aa_result["zPeak"].toDouble(),lsut->sut_carrier->GetFeedBackPos().Z);
    if (enableTilt == 0) {
        qInfo("Disable tilt...");
    } else {
        qInfo("Enable tilt...xTilt: %f yTilt: %f", aa_result["xTilt"].toDouble(), aa_result["yTilt"].toDouble());
        aa_head->stepInterpolation_AB_Sync(-aa_result["yTilt"].toDouble(), aa_result["xTilt"].toDouble());
        //aa_head->stepInterpolation_AB_Sync(-aa_result["xTilt"].toDouble(), -aa_result["yTilt"].toDouble());
    }
    if (position_checking == 1){
        QThread::msleep(zSleepInMs);
        cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
        double beforeZ = lsut->sut_carrier->GetFeedBackPos().Z;
        double expected_fov = fov_slope*aa_result["zPeak"].toDouble() + fov_intercept;
        double dfov = calculateDFOV(img);
        double diff_z = (dfov - expected_fov)/fov_slope;
//        sut->moveToZPos(beforeZ - diff_z);
        double afterZ = lsut->sut_carrier->GetFeedBackPos().Z;
        qInfo("before z: %f after z: %f now fov: %f expected fov: %f fov slope: %f fov intercept: %f", beforeZ, afterZ, dfov, expected_fov, fov_slope, fov_intercept);
    }
    clustered_sfr_map.clear();
    qInfo("AA time elapsed: %d", timer.elapsed());
    map.insert("X_TILT", aa_result["xTilt"].toDouble());
    map.insert("Y_TILT", aa_result["yTilt"].toDouble());
    map.insert("Z_PEAK_CC", aa_result["zPeak"].toDouble());
//    map.insert("Z_PEAK_UL", aa_result["zPeak"].toDouble());
//    map.insert("Z_PEAK_UR", ur_zPeak);
//    map.insert("Z_PEAK_LL", ll_zPeak);
//    map.insert("Z_PEAK_LR", lr_zPeak);
    map.insert("FOV_SLOPE", fov_slope);
    map.insert("FOV_INTERCEPT", fov_intercept);
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

        emit sfrWorkerController->calculate(i, start+i*step_size, dst, false, resize_factor);
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
    int fitOrder = 4;
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
    double cc_min_sfr = params["CC"].toDouble(-1);
    double ul_min_sfr = params["UL"].toDouble(-1);
    double ur_min_sfr = params["UR"].toDouble(-1);
    double ll_min_sfr = params["LL"].toDouble(-1);
    double lr_min_sfr = params["LR"].toDouble(-1);
    double sfr_dev_tol = params["SFR_DEV_TOL"].toDouble(100);
    qInfo("%f %f %f %f %f", cc_min_sfr, ul_min_sfr, ur_min_sfr, ll_min_sfr, lr_min_sfr);
    clustered_sfr_map.clear();
    QJsonValue aaPrams;
    this->sfrWorkerController->setSfrWorkerParams(aaPrams);
    QElapsedTimer timer;
    QVariantMap map;
    cv::Mat img = cv::imread("C:\\Users\\emil\\Desktop\\jiexi\\3.jpg");
    //cv::Mat img = cv::imread("C:\\Users\\emil\\Desktop\\Test\\Samsung\\debug\\debug\\zscan_6.bmp");
    //cv::Mat img = cv::imread("C:\\Users\\emil\\share\\20-05-24-622.bmp");
    double dfov = calculateDFOV(img);
    qInfo("%f %d %d %d", dfov, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea() );
    AA_Helper::AAA_Search_MTF_Pattern_Ex(img, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea());
    cv::Mat dst;
    cv::Size size(img.cols, img.rows);
    timer.start();
    cv::resize(img, dst, size);
    qInfo("img resize: %d %d time elapsed: %d", dst.cols, dst.rows, timer.elapsed());
    timer.restart();

    emit sfrWorkerController->calculate(0, 0, dst, false);
    int timeout=1000;
    while(this->clustered_sfr_map.size() != 1 && timeout >0) {
        Sleep(10);
        timeout--;
    }
    qInfo("clustered sfr map size: %d", clustered_sfr_map.size());
    vector<Sfr_entry> sv = clustered_sfr_map[0];
    int max_layer = 0;
    for (unsigned int i = 0; i < sv.size(); i++)
    {
        qInfo("%f %f %f %f %f %f %d %d", sv.at(i).px, sv.at(i).py,
              sv.at(i).t_sfr, sv.at(i).r_sfr, sv.at(i).b_sfr, sv.at(i).l_sfr,
              sv.at(i).layer, sv.at(i).location);
        if (sv.at(i).layer > max_layer) {
            max_layer = sv.at(i).layer - 1;
        }
    }
    bool sfr_check = true;
    std::vector<double> sfr_check_list;

    for (size_t ii = 1; ii <= 4; ii++) {
        sfr_check_list.push_back(sv[max_layer*4 + ii].t_sfr);
        sfr_check_list.push_back(sv[max_layer*4 + ii].r_sfr);
        sfr_check_list.push_back(sv[max_layer*4 + ii].b_sfr);
        sfr_check_list.push_back(sv[max_layer*4 + ii].l_sfr);
        qInfo("Outer layer: %d t_sfr: %f l_sfr: %f b_sfr: %f r_sfr: %f", max_layer,
              sv[max_layer*4 + ii].t_sfr, sv[max_layer*4 + ii].l_sfr,
              sv[max_layer*4 + ii].b_sfr, sv[max_layer*4 + ii].r_sfr);
    }

    std::sort(sfr_check_list.begin(), sfr_check_list.end());
    double max_sfr_deviation = fabs(sfr_check_list[0] - sfr_check_list[sfr_check_list.size()-1]);
    qInfo("Max sfr deviation : %f", max_sfr_deviation);
    if (max_sfr_deviation >= sfr_dev_tol) {
        qInfo("max_sfr_deviation cannot pass");
        sfr_check = false;
    }

    if (sv[0].t_sfr < cc_min_sfr || sv[0].r_sfr < cc_min_sfr || sv[0].b_sfr < cc_min_sfr || sv[0].l_sfr < cc_min_sfr) {
       qInfo("cc cannot pass");
       sfr_check = false;
    }
    if (sv[max_layer*4 + 1].t_sfr < ul_min_sfr || sv[max_layer*4 + 1].r_sfr < ul_min_sfr ||
        sv[max_layer*4 + 1].b_sfr < ul_min_sfr || sv[max_layer*4 + 1].l_sfr < ul_min_sfr) {
        qInfo("ul cannot pass");
        sfr_check = false;
    }
    if (sv[max_layer*4 + 2].t_sfr < ll_min_sfr || sv[max_layer*4 + 2].r_sfr < ll_min_sfr ||
        sv[max_layer*4 + 2].b_sfr < ll_min_sfr || sv[max_layer*4 + 2].l_sfr < ll_min_sfr) {
        qInfo("ll cannot pass");
        sfr_check = false;
    }
    if (sv[max_layer*4 + 3].t_sfr < lr_min_sfr || sv[max_layer*4 + 3].r_sfr < lr_min_sfr ||
        sv[max_layer*4 + 3].b_sfr < lr_min_sfr || sv[max_layer*4 + 3].l_sfr < lr_min_sfr) {
        qInfo("lr cannot pass");
        sfr_check = false;
    }
    if (sv[max_layer*4 + 4].t_sfr < ur_min_sfr || sv[max_layer*4 + 4].r_sfr < ur_min_sfr ||
        sv[max_layer*4 + 4].b_sfr < ur_min_sfr || sv[max_layer*4 + 4].l_sfr < ur_min_sfr) {
        qInfo("ur cannot pass");
        sfr_check = false;
    }

    if (true) {
        double display_factor = img.cols/CONSTANT_REFERENCE;
        int roi_width = sqrt(sv[0].area)*this->parameters.ROIRatio();
        QImage qImage = ImageGrabbingWorkerThread::cvMat2QImage(img);
        QPainter qPainter(&qImage);
        qPainter.setBrush(Qt::NoBrush);
        qPainter.setFont(QFont("Times",75*display_factor, QFont::Light));
        for (Sfr_entry sfr_entry : sv) {
            qPainter.setPen(QPen(Qt::blue, 4.0));
            if(sfr_entry.layer == (max_layer +1)) {
                double min_sfr = 0;
                if(sfr_entry.location == 1) { min_sfr = ul_min_sfr; }
                if(sfr_entry.location == 2) { min_sfr = ur_min_sfr; }
                if(sfr_entry.location == 3) { min_sfr = lr_min_sfr; }
                if(sfr_entry.location == 4) { min_sfr = ll_min_sfr; }
                    if(sfr_entry.t_sfr < min_sfr) {
                        qPainter.setPen(QPen(Qt::red, 4.0));
                    }else {
                        qPainter.setPen(QPen(Qt::blue, 4.0));
                    }
                    qPainter.drawText(sfr_entry.px - 50 , sfr_entry.py - roi_width/2, QString::number(sfr_entry.t_sfr, 'g', 4));
                    if(sfr_entry.r_sfr < min_sfr) {
                        qPainter.setPen(QPen(Qt::red, 4.0));
                    }else {
                        qPainter.setPen(QPen(Qt::blue, 4.0));
                    }
                    qPainter.drawText(sfr_entry.px + roi_width/2, sfr_entry.py,  QString::number(sfr_entry.r_sfr, 'g', 4));
                    if(sfr_entry.b_sfr < min_sfr) {
                        qPainter.setPen(QPen(Qt::red, 4.0));
                    }else {
                        qPainter.setPen(QPen(Qt::blue, 4.0));
                    }
                    qPainter.drawText(sfr_entry.px - 50, sfr_entry.py + roi_width/2,  QString::number(sfr_entry.b_sfr, 'g', 4));
                    if(sfr_entry.l_sfr < min_sfr) {
                        qPainter.setPen(QPen(Qt::red, 4.0));
                    }else {
                        qPainter.setPen(QPen(Qt::blue, 4.0));
                    }
                    qPainter.drawText(sfr_entry.px - roi_width/2 - 100, sfr_entry.py,  QString::number(sfr_entry.l_sfr, 'g', 4));
            } else {
                qPainter.drawText(sfr_entry.px - 50 , sfr_entry.py - roi_width/2, QString::number(sfr_entry.t_sfr, 'g', 4));
                qPainter.drawText(sfr_entry.px + roi_width/2, sfr_entry.py,  QString::number(sfr_entry.r_sfr, 'g', 4));
                qPainter.drawText(sfr_entry.px - 50, sfr_entry.py + roi_width/2,  QString::number(sfr_entry.b_sfr, 'g', 4));
                qPainter.drawText(sfr_entry.px - roi_width/2 - 100, sfr_entry.py,  QString::number(sfr_entry.l_sfr, 'g', 4));
            }
        }
        qPainter.end();
        sfrImageReady(std::move(qImage));
    }

    clustered_sfr_map.clear();
    qInfo("Time elapsed : %d sv size: %d", timer.elapsed(), sv.size());
    if (sfr_check) {
       return ErrorCodeStruct{ErrorCode::OK, ""};
    } else {
       LogicNg(current_mtf_ng_time);
       return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
}

ErrorCodeStruct AACoreNew::performMTF(QJsonValue params, bool write_log)
{
    double cc_min_sfr = params["CC"].toDouble(-1);
    double ul_min_sfr = params["UL"].toDouble(-1);
    double ur_min_sfr = params["UR"].toDouble(-1);
    double ll_min_sfr = params["LL"].toDouble(-1);
    double lr_min_sfr = params["LR"].toDouble(-1);
    double sfr_dev_tol = params["SFR_DEV_TOL"].toDouble(100);
    clustered_sfr_map.clear();
    QJsonValue aaPrams;
    this->sfrWorkerController->setSfrWorkerParams(aaPrams);
    QElapsedTimer timer;timer.start();
    QVariantMap map;
    bool grabRet = false;
    cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
    if (!grabRet) {
        qInfo("MTF Cannot grab image.");
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
    double fov = calculateDFOV(img);
    cv::Mat dst;
    cv::Size size(img.cols, img.rows);
//    timer.start();
    qint64 start_time = timer.elapsed();
    cv::resize(img, dst, size);
    qInfo("FOV: %f img resize: %d %d time elapsed: %d", fov, dst.cols, dst.rows, timer.elapsed() - start_time);
//    timer.restart();
    if (fov == -1) {
        qCritical("Cannot calculate FOV from the grabbed image.");
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
    start_time = timer.elapsed();
    emit sfrWorkerController->calculate(0, 0, dst, true);
    int timeout=1000;
    while(this->clustered_sfr_map.size() != 1 && timeout >0) {
        Sleep(10);
        timeout--;
    }
    vector<Sfr_entry> sv = clustered_sfr_map[0];
    int max_layer = 0;
    if(sv.size() == 0) {
        qCritical("Cannot find any MTF");
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
    for (unsigned int i = 0; i < sv.size(); i++)
    {
        qInfo("%f %f %f %f %f %f %d %d", sv.at(i).px, sv.at(i).py,
              sv.at(i).t_sfr, sv.at(i).r_sfr, sv.at(i).b_sfr, sv.at(i).l_sfr,
              sv.at(i).layer, sv.at(i).location);
        if (sv.at(i).layer > max_layer) {
            max_layer = sv.at(i).layer - 1;
        }
    }
    bool sfr_check = true;
    std::vector<double> sfr_check_list;

    for (size_t ii = 1; ii <= 4; ii++) {
        sfr_check_list.push_back(sv[max_layer*4 + ii].t_sfr);
        sfr_check_list.push_back(sv[max_layer*4 + ii].r_sfr);
        sfr_check_list.push_back(sv[max_layer*4 + ii].b_sfr);
        sfr_check_list.push_back(sv[max_layer*4 + ii].l_sfr);
        qInfo("Outer layer: %d t_sfr: %f l_sfr: %f b_sfr: %f r_sfr: %f", max_layer,
              sv[max_layer*4 + ii].t_sfr, sv[max_layer*4 + ii].l_sfr,
              sv[max_layer*4 + ii].b_sfr, sv[max_layer*4 + ii].r_sfr);
    }

    std::sort(sfr_check_list.begin(), sfr_check_list.end());
    double max_sfr_deviation = fabs(sfr_check_list[0] - sfr_check_list[sfr_check_list.size()-1]);
    qInfo("Max sfr deviation : %f", max_sfr_deviation);
    if (max_sfr_deviation >= sfr_dev_tol) {
        qInfo("max_sfr_deviation cannot pass");
        sfr_check = false;
    }

    if (sv[0].t_sfr < cc_min_sfr || sv[0].r_sfr < cc_min_sfr || sv[0].b_sfr < cc_min_sfr || sv[0].l_sfr < cc_min_sfr) {
       qInfo("cc cannot pass");
       sfr_check = false;
    }
    if (sv[max_layer*4 + 1].t_sfr < ul_min_sfr || sv[max_layer*4 + 1].r_sfr < ul_min_sfr ||
        sv[max_layer*4 + 1].b_sfr < ul_min_sfr || sv[max_layer*4 + 1].l_sfr < ul_min_sfr) {
        qInfo("ul cannot pass");
        sfr_check = false;
    }
    if (sv[max_layer*4 + 2].t_sfr < ll_min_sfr || sv[max_layer*4 + 2].r_sfr < ll_min_sfr ||
        sv[max_layer*4 + 2].b_sfr < ll_min_sfr || sv[max_layer*4 + 2].l_sfr < ll_min_sfr) {
        qInfo("ll cannot pass");
        sfr_check = false;
    }
    if (sv[max_layer*4 + 3].t_sfr < lr_min_sfr || sv[max_layer*4 + 3].r_sfr < lr_min_sfr ||
        sv[max_layer*4 + 3].b_sfr < lr_min_sfr || sv[max_layer*4 + 3].l_sfr < lr_min_sfr) {
        qInfo("lr cannot pass");
        sfr_check = false;
    }
    if (sv[max_layer*4 + 4].t_sfr < ur_min_sfr || sv[max_layer*4 + 4].r_sfr < ur_min_sfr ||
        sv[max_layer*4 + 4].b_sfr < ur_min_sfr || sv[max_layer*4 + 4].l_sfr < ur_min_sfr) {
        qInfo("ur cannot pass");
        sfr_check = false;
    }

    if (true) {
        double display_factor = img.cols/CONSTANT_REFERENCE;
        int roi_width = sqrt(sv[0].area)*this->parameters.ROIRatio();
        QImage qImage = ImageGrabbingWorkerThread::cvMat2QImage(img);
        QPainter qPainter(&qImage);
        qPainter.setBrush(Qt::NoBrush);
        qPainter.setFont(QFont("Times",75*display_factor, QFont::Light));
        for (Sfr_entry sfr_entry : sv) {
            qPainter.setPen(QPen(Qt::blue, 4.0));
            if(sfr_entry.layer == (max_layer +1)) {
                double min_sfr = 0;
                if(sfr_entry.location == 1) { min_sfr = ul_min_sfr; }
                if(sfr_entry.location == 2) { min_sfr = ur_min_sfr; }
                if(sfr_entry.location == 3) { min_sfr = lr_min_sfr; }
                if(sfr_entry.location == 4) { min_sfr = ll_min_sfr; }
                    if(sfr_entry.t_sfr < min_sfr) {
                        qPainter.setPen(QPen(Qt::red, 4.0));
                    }else {
                        qPainter.setPen(QPen(Qt::blue, 4.0));
                    }
                    qPainter.drawText(sfr_entry.px - 50 , sfr_entry.py - roi_width/2, QString::number(sfr_entry.t_sfr, 'g', 4));
                    if(sfr_entry.r_sfr < min_sfr) {
                        qPainter.setPen(QPen(Qt::red, 4.0));
                    }else {
                        qPainter.setPen(QPen(Qt::blue, 4.0));
                    }
                    qPainter.drawText(sfr_entry.px + roi_width/2, sfr_entry.py,  QString::number(sfr_entry.r_sfr, 'g', 4));
                    if(sfr_entry.b_sfr < min_sfr) {
                        qPainter.setPen(QPen(Qt::red, 4.0));
                    }else {
                        qPainter.setPen(QPen(Qt::blue, 4.0));
                    }
                    qPainter.drawText(sfr_entry.px - 50, sfr_entry.py + roi_width/2,  QString::number(sfr_entry.b_sfr, 'g', 4));
                    if(sfr_entry.l_sfr < min_sfr) {
                        qPainter.setPen(QPen(Qt::red, 4.0));
                    }else {
                        qPainter.setPen(QPen(Qt::blue, 4.0));
                    }
                    qPainter.drawText(sfr_entry.px - roi_width/2 - 100, sfr_entry.py,  QString::number(sfr_entry.l_sfr, 'g', 4));
            } else {
                qPainter.drawText(sfr_entry.px - 50 , sfr_entry.py - roi_width/2, QString::number(sfr_entry.t_sfr, 'g', 4));
                qPainter.drawText(sfr_entry.px + roi_width/2, sfr_entry.py,  QString::number(sfr_entry.r_sfr, 'g', 4));
                qPainter.drawText(sfr_entry.px - 50, sfr_entry.py + roi_width/2,  QString::number(sfr_entry.b_sfr, 'g', 4));
                qPainter.drawText(sfr_entry.px - roi_width/2 - 100, sfr_entry.py,  QString::number(sfr_entry.l_sfr, 'g', 4));
            }
        }
        qPainter.end();
        sfrImageReady(std::move(qImage));
    }

    clustered_sfr_map.clear();
    qInfo("Time elapsed : %d sv size: %d", timer.elapsed() - start_time, sv.size());
    map.insert("FOV",fov);
    map.insert("zPeak",lsut->sut_carrier->GetFeedBackPos().Z);
    map.insert("CC_T_SFR", sv[0].t_sfr);
    map.insert("CC_R_SFR", sv[0].r_sfr);
    map.insert("CC_B_SFR", sv[0].b_sfr);
    map.insert("CC_L_SFR", sv[0].l_sfr);
    map.insert("CC_SFR", (sv[0].t_sfr + sv[0].r_sfr + sv[0].b_sfr + sv[0].l_sfr)/4);
    map.insert("UL_T_SFR", sv[max_layer*4 + 1].t_sfr);
    map.insert("UL_R_SFR", sv[max_layer*4 + 1].r_sfr);
    map.insert("UL_B_SFR", sv[max_layer*4 + 1].b_sfr);
    map.insert("UL_L_SFR", sv[max_layer*4 + 1].l_sfr);
    map.insert("UL_SFR", (sv[max_layer*4 + 1].t_sfr + sv[max_layer*4 + 1].r_sfr + sv[max_layer*4 + 1].b_sfr + sv[max_layer*4 + 1].l_sfr)/4);
    map.insert("LL_T_SFR", sv[max_layer*4 + 2].t_sfr);
    map.insert("LL_R_SFR", sv[max_layer*4 + 2].r_sfr);
    map.insert("LL_B_SFR", sv[max_layer*4 + 2].b_sfr);
    map.insert("LL_L_SFR", sv[max_layer*4 + 2].l_sfr);
    map.insert("LL_SFR", (sv[max_layer*4 + 2].t_sfr + sv[max_layer*4 + 2].r_sfr + sv[max_layer*4 + 2].b_sfr + sv[max_layer*4 + 2].l_sfr)/4);
    map.insert("LR_T_SFR", sv[max_layer*4 + 3].t_sfr);
    map.insert("LR_R_SFR", sv[max_layer*4 + 3].r_sfr);
    map.insert("LR_B_SFR", sv[max_layer*4 + 3].b_sfr);
    map.insert("LR_L_SFR", sv[max_layer*4 + 3].l_sfr);
    map.insert("LR_SFR", (sv[max_layer*4 + 3].t_sfr + sv[max_layer*4 + 3].r_sfr + sv[max_layer*4 + 3].b_sfr + sv[max_layer*4 + 3].l_sfr)/4);
    map.insert("UR_T_SFR", sv[max_layer*4 + 4].t_sfr);
    map.insert("UR_R_SFR", sv[max_layer*4 + 4].r_sfr);
    map.insert("UR_B_SFR", sv[max_layer*4 + 4].b_sfr);
    map.insert("UR_L_SFR", sv[max_layer*4 + 4].l_sfr);
    map.insert("UR_SFR", (sv[max_layer*4 + 4].t_sfr + sv[max_layer*4 + 4].r_sfr + sv[max_layer*4 + 4].b_sfr + sv[max_layer*4 + 4].l_sfr)/4);
    map.insert("OC_OFFSET_X_IN_PIXEL", mtf_oc_x);
    map.insert("OC_OFFSET_Y_IN_PIXEL", mtf_oc_y);
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "MTF", map);

    if (sfr_check) {
       return ErrorCodeStruct{ErrorCode::OK, ""};
    } else {
      // LogicNg(current_mtf_ng_time);
        NgLens();
        NgSensor();
       return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
}

//ErrorCodeStruct AACoreNew::performMTF(QJsonValue params, bool write_log)
//{
//    QElapsedTimer timer; timer.start();
//    QVariantMap map;
//    //cv::Mat img = cv::imread("C:\\Users\\emil\\Desktop\\Test\\Samsung\\debug\\debug\\zscan_6.bmp");
//    cv::Mat img = dk->DothinkeyGrabImageCV(0);
//    int imageWidth = img.cols;
//    int imageHeight = img.rows;
//    double fov = this->calculateDFOV(img);
//    map.insert("DFOV", fov);
//    if (fov == -1) {
//        qInfo("Error in calculating fov");
//        LogicNg(current_mtf_ng_time);
//        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
//    } else {
//        qInfo("DFOV :%f", fov);
//    }
//    emit sfrWorkerController->calculate(0, 0, img, true);
//    int timeout=1000;
//    while(this->clustered_sfr_map.size() != 1 && timeout >0) {
//        Sleep(10);
//        timeout--;
//    }
//    if (timeout <= 0) {
//        qInfo("Error in performing MTF: %d", timeout);
//        LogicNg(current_mtf_ng_time);
//        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
//    }
//    vector<Sfr_entry> sfr_entry = clustered_sfr_map.at(0);
//    double cc_min_d = 999999, ul_min_d = 999999, ur_min_d = 999999, lr_min_d = 999999, ll_min_d = 999999;
//    unsigned int ccROIIndex = 0 , ulROIIndex = 0, urROIIndex = 0, llROIIndex = 0, lrROIIndex = 0;
//    for (unsigned int i = 0; i < sfr_entry.size(); i++)
//    {
//        double cc_d = sqrt(pow(sfr_entry.at(i).px - imageWidth/2, 2) + pow(sfr_entry.at(i).py - imageHeight/2, 2));
//        double ul_d = sqrt(pow(sfr_entry.at(i).px, 2) + pow(sfr_entry.at(i).py, 2));
//        double ur_d = sqrt(pow(sfr_entry.at(i).px - imageWidth, 2) + pow(sfr_entry.at(i).py, 2));
//        double ll_d = sqrt(pow(sfr_entry.at(i).px, 2) + pow(sfr_entry.at(i).py - imageHeight, 2));
//        double lr_d = sqrt(pow(sfr_entry.at(i).px - imageWidth, 2) + pow(sfr_entry.at(i).py - imageHeight, 2));
//        if (cc_d < cc_min_d) {
//              cc_min_d = cc_d;
//              ccROIIndex = i;
//        }
//        if (ul_d < ul_min_d) {
//             ul_min_d = ul_d;
//             ulROIIndex = i;
//        }
//        if (ur_d < ur_min_d) {
//             ur_min_d = ur_d;
//             urROIIndex = i;
//        }
//        if (ll_d < ll_min_d) {
//             ll_min_d = ll_d;
//             llROIIndex = i;
//        }
//        if (lr_d < lr_min_d) {
//            lr_min_d = lr_d;
//            lrROIIndex = i;
//        }
//    }

//    double cc_min_sfr = params["CC"].toDouble(-1);
//    double ul_min_sfr = params["UL"].toDouble(-1);
//    double ur_min_sfr = params["UR"].toDouble(-1);
//    double ll_min_sfr = params["LL"].toDouble(-1);
//    double lr_min_sfr = params["LR"].toDouble(-1);
//    double sfr_dev_tol = params["SFR_DEV_TOL"].toDouble(100);

//    map.insert("CC_CHECK", cc_min_sfr);
//    map.insert("UR_CHECK", ul_min_sfr);
//    map.insert("UL_CHECK", ur_min_sfr);
//    map.insert("LR_CHECK", lr_min_sfr);
//    map.insert("LL_CHECK", ll_min_sfr);
//    map.insert("SFR_MAX_TOL", sfr_dev_tol);

//    bool sfr_check = true;
//    if (sfr_entry[ccROIIndex].t_sfr < cc_min_sfr ||
//        sfr_entry[ccROIIndex].r_sfr < cc_min_sfr ||
//        sfr_entry[ccROIIndex].b_sfr < cc_min_sfr ||
//        sfr_entry[ccROIIndex].l_sfr < cc_min_sfr) {
//       qInfo("cc cannot pass");
//       sfr_check = false;
//    }
//    if (sfr_entry[urROIIndex].t_sfr < ur_min_sfr ||
//        sfr_entry[urROIIndex].r_sfr < ur_min_sfr ||
//        sfr_entry[urROIIndex].b_sfr < ur_min_sfr ||
//        sfr_entry[urROIIndex].l_sfr < ur_min_sfr) {
//       qInfo("ur cannot pass");
//       sfr_check = false;
//    }
//    if (sfr_entry[ulROIIndex].t_sfr < ul_min_sfr ||
//        sfr_entry[ulROIIndex].r_sfr < ul_min_sfr ||
//        sfr_entry[ulROIIndex].b_sfr < ul_min_sfr ||
//        sfr_entry[ulROIIndex].l_sfr < ul_min_sfr) {
//       qInfo("ul cannot pass");
//       sfr_check = false;
//    }
//    if (sfr_entry[lrROIIndex].t_sfr < lr_min_sfr ||
//        sfr_entry[lrROIIndex].r_sfr < lr_min_sfr ||
//        sfr_entry[lrROIIndex].b_sfr < lr_min_sfr ||
//        sfr_entry[lrROIIndex].l_sfr < lr_min_sfr) {
//       qInfo("lr cannot pass");
//       sfr_check = false;
//    }
//    if (sfr_entry[llROIIndex].t_sfr < ll_min_sfr ||
//        sfr_entry[llROIIndex].r_sfr < ll_min_sfr ||
//        sfr_entry[llROIIndex].b_sfr < ll_min_sfr ||
//        sfr_entry[llROIIndex].l_sfr < ll_min_sfr) {
//       qInfo("ll cannot pass");
//       sfr_check = false;
//    }

//    std::vector<double> sfr_v;
//    for (int i = 0; i < 4; i++) {
//        unsigned int index = 0;
//        if (i == 0) index = urROIIndex;
//        if (i == 1) index = ulROIIndex;
//        if (i == 2) index = lrROIIndex;
//        if (i == 3) index = llROIIndex;
//        sfr_v.push_back(sfr_entry[index].sfr);
//        sfr_v.push_back(sfr_entry[index].sfr);
//        sfr_v.push_back(sfr_entry[index].sfr);
//        sfr_v.push_back(sfr_entry[index].sfr);
//        sfr_v.push_back(sfr_entry[index].sfr);
//    }

//    std::sort(sfr_v.begin(), sfr_v.end());
//    double max_sfr_deviation = fabs(sfr_v[0] - sfr_v[sfr_v.size()-1]);
//    if (max_sfr_deviation >= sfr_dev_tol) {
//        qInfo("max_sfr_deviation cannot pass");
//        sfr_check = false;
//    }

//    qInfo("Read the aahead and sut carrier feedback");
//    mPoint6D motorsPosition = this->aa_head->GetFeedBack();
//    mPoint3D sutPosition = this->sut->carrier->GetFeedBackPos();
//    qInfo("inset data to map ccROIIndex %d urROIIndex %d ulROIInde %dx lrROIIndex %d llROIIndex %d size %d",ccROIIndex,urROIIndex,ulROIIndex,lrROIIndex,llROIIndex,sfr_entry.size());
//    map.insert("AA_X", motorsPosition.X);
//    map.insert("AA_Y", motorsPosition.Y);
//    map.insert("AA_Z", motorsPosition.Z);
//    map.insert("AA_A", motorsPosition.A);
//    map.insert("AA_B", motorsPosition.B);
//    map.insert("AA_C", motorsPosition.C);
//    map.insert("AA_A", motorsPosition.A);
//    map.insert("AA_B", motorsPosition.B);
//    map.insert("AA_C", motorsPosition.C);
//    map.insert("SUT_X", sutPosition.X);
//    map.insert("SUT_Y", sutPosition.Y);
//    map.insert("SUT_Z", sutPosition.Z);
//    map.insert("OC_X", sfr_entry[ccROIIndex].px - imageWidth/2);
//    map.insert("OC_Y", sfr_entry[ccROIIndex].py - imageHeight/2);

//    map.insert("CC_SFR", sfr_entry[ccROIIndex].sfr);
//    map.insert("CC_SFR_1", sfr_entry[ccROIIndex].t_sfr);
//    map.insert("CC_SFR_2", sfr_entry[ccROIIndex].r_sfr);
//    map.insert("CC_SFR_3", sfr_entry[ccROIIndex].b_sfr);
//    map.insert("CC_SFR_4", sfr_entry[ccROIIndex].l_sfr);

//    map.insert("UR_SFR", sfr_entry[urROIIndex].sfr);
//    map.insert("UR_SFR_1", sfr_entry[urROIIndex].t_sfr);
//    map.insert("UR_SFR_2", sfr_entry[urROIIndex].r_sfr);
//    map.insert("UR_SFR_3", sfr_entry[urROIIndex].b_sfr);
//    map.insert("UR_SFR_4", sfr_entry[urROIIndex].l_sfr);

//    map.insert("UL_SFR", sfr_entry[ulROIIndex].sfr);
//    map.insert("UL_SFR_1", sfr_entry[ulROIIndex].t_sfr);
//    map.insert("UL_SFR_2", sfr_entry[ulROIIndex].r_sfr);
//    map.insert("UL_SFR_3", sfr_entry[ulROIIndex].b_sfr);
//    map.insert("UL_SFR_4", sfr_entry[ulROIIndex].l_sfr);

//    map.insert("LR_SFR", sfr_entry[lrROIIndex].sfr);
//    map.insert("LR_SFR_1", sfr_entry[lrROIIndex].t_sfr);
//    map.insert("LR_SFR_2", sfr_entry[lrROIIndex].r_sfr);
//    map.insert("LR_SFR_3", sfr_entry[lrROIIndex].b_sfr);
//    map.insert("LR_SFR_4", sfr_entry[lrROIIndex].l_sfr);

//    map.insert("LL_SFR", sfr_entry[llROIIndex].sfr);
//    map.insert("LL_SFR_1", sfr_entry[llROIIndex].t_sfr);
//    map.insert("LL_SFR_2", sfr_entry[llROIIndex].r_sfr);
//    map.insert("LL_SFR_3", sfr_entry[llROIIndex].b_sfr);
//    map.insert("LL_SFR_4", sfr_entry[llROIIndex].l_sfr);

//    map.insert("Sensor_ID", dk->readSensorID());
//    map.insert("SFR_CHECK", sfr_check);
//    map.insert("DFOV", fov);
//    map.insert("timeElapsed", timer.elapsed());
//    qInfo("CC_X :%f CC_Y: %f", sfr_entry[ccROIIndex].px, sfr_entry[ccROIIndex].py);
//    clustered_sfr_map.clear();
//    emit pushDataToUnit(this->runningUnit, "MTF", map);
//    if (write_log) {
//        this->loopTestResult.append(QString::number(sfr_entry[ccROIIndex].sfr))
//                            .append(",")
//                            .append(QString::number(sfr_entry[ulROIIndex].sfr))
//                            .append(",")
//                            .append(QString::number(sfr_entry[urROIIndex].sfr))
//                            .append(",")
//                            .append(QString::number(sfr_entry[llROIIndex].sfr))
//                            .append(",")
//                            .append(QString::number(sfr_entry[lrROIIndex].sfr))
//                            .append(",\n");
//        this->mtf_log.incrementData(sfr_entry[ccROIIndex].sfr, sfr_entry[ulROIIndex].sfr, sfr_entry[urROIIndex].sfr, sfr_entry[llROIIndex].sfr,sfr_entry[lrROIIndex].sfr);
//    }
//    qInfo("MTF done");
//    if (sfr_check) {
//        return ErrorCodeStruct{ErrorCode::OK, ""};
//    } else {
//        LogicNg(current_mtf_ng_time);
//        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
//    }
//}

ErrorCodeStruct AACoreNew::performUV(int uv_time)
{
    qInfo("Perform UV uv time: %d", uv_time);
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
//    NgLens();
//    NgSensor();
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

ErrorCodeStruct AACoreNew::performTerminate()
{
    imageThread->stop();
    Sleep(100);
    imageThread->exit();
    dk->DothinkeyClose();
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
    cv::Mat inputImage = dk->DothinkeyGrabImageCV(0, grabRet);
    if (!grabRet) {
        qInfo("Cannot grab image.");
        NgSensor();
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "Y Level Test Fail. Cannot grab image"};
    }
    float min_i = 0;
    float max_i = 0;
    vector<float> intensityProfile;
    QElapsedTimer timer; timer.start();
    bool ret = AA_Helper::calculateImageIntensityProfile(inputImage, min_i, max_i, intensityProfile);
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
    vector<float> intensityProfile; float min_i = 0; float max_i = 0;
    bool ret = AA_Helper::calculateImageIntensityProfile(inImage, min_i, max_i, intensityProfile);
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
    cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
    if (!grabRet) {
        qInfo("AA Cannot grab image.");
        LogicNg(current_aa_ng_time);
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

ErrorCodeStruct AACoreNew::performInitSensor()
{
    //if(!has_sensor) return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "has no sensor"};
    if (dk->DothinkeyIsGrabbing()) {
        qInfo("Sensor have already init");
        return ErrorCodeStruct {ErrorCode::OK, ""};
    }
    QElapsedTimer timer, stepTimer; timer.start(); stepTimer.start();
    QVariantMap map;
    const int channel = 0;
    bool res = dk->DothinkeyEnum();
    if (!res) { qCritical("Cannot find dothinkey");NgSensor();return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "1"}; }
    res = dk->DothinkeyOpen();
    map.insert("dothinkeyOpen", stepTimer.elapsed()); stepTimer.restart();
    if (!res) { qCritical("Cannot open dothinkey"); NgSensor();return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "2"}; }
    res = dk->DothinkeyLoadIniFile(channel);
    map.insert("dothinkeyLoadIniFile", stepTimer.elapsed()); stepTimer.restart();
    if (!res) { qCritical("Cannot load dothinkey ini file");NgSensor(); return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "3"}; }
    res = dk->DothinkeyStartCamera(channel);
    map.insert("dothinkeyStartCamera", stepTimer.elapsed()); stepTimer.restart();
    if (!res) { qCritical("Cannot start camera");NgSensor(); return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "4"}; }

//    QString sensorID = dk->readSensorID();
//    qInfo("performInitSensor sensor ID: %s", sensorID.toStdString().c_str());
//    map.insert("sensorID", sensorID);
    if (!imageThread->isRunning())
        imageThread->start();
    map.insert("timeElapsed", timer.elapsed());
    map.insert("success", res);
    emit pushDataToUnit(runningUnit, "InitSensor", map);
    return ErrorCodeStruct {ErrorCode::OK, ""};
}

// TO BE MODIFIED
ErrorCodeStruct AACoreNew::performPRToBond()
{
    QElapsedTimer timer, stepTimer; timer.start(); stepTimer.start();
    QVariantMap map;
    if (!lsut->gripLens()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA pick lens fail"};}
    map.insert("aa_gripLens", stepTimer.elapsed()); stepTimer.restart();
    if (!this->aa_head->moveToMushroomPosition(true)) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA cannot move to mushroom Pos"};}
    map.insert("aa_head_moveToMushroomPosition", stepTimer.elapsed()); stepTimer.restart();
//    PrOffset downlookPrOffset;
//    if(!lsut->moveToDownlookPR(downlookPrOffset)) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "moveToDownlookPR"}; }
//    qInfo("downlook Pr Offset x: %f y: %f t: %f", downlookPrOffset.X, downlookPrOffset.Y, downlookPrOffset.Theta);
//    double theta = lsut->up_downlook_offset.Theta() + aa_head->uplook_theta + aa_head->offset_theta;
//    this->aa_head->moveToSync(0, 0, 0, theta);
    if(!this->lsut->moveToMushroomPosition(true)) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "moveToMushroomPosition"}; }
    map.insert("lsut_moveToMushroomPosition", stepTimer.elapsed()); stepTimer.restart();
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "PrToBond", map);
    return ErrorCodeStruct {ErrorCode::OK, ""};
}

// TO BE MODIFIED
ErrorCodeStruct AACoreNew::performAAPickLens()
{
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    if (!lsut->gripLens()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA pick lens fail"};}
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "AAPickLens", map);
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
    std::vector<AA_Helper::patternAttr> vector = AA_Helper::AAA_Search_MTF_Pattern_Ex(img, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea());
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
        return;
    }
    bool grabRet = false;
    cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
    if (!grabRet) {
        qInfo("AA Cannot grab image.");
        return;
    }
    cv::imwrite("livePhoto.bmp", img);
}

void AACoreNew::aaCoreParametersChanged()
{
    qInfo("AA Core parameters changed");
    QVariantMap map;
    cv::Mat img = cv::imread("livePhoto.bmp");
    QImage outImage = imageThread->cvMat2QImage(img);
    double dfov = calculateDFOV(img);
    double imageCenterX = img.cols/2;
    double imageCenterY = img.rows/2;
    QPainter qPainter(&outImage);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setPen(QPen(Qt::red, 4.0));
    emit sfrWorkerController->calculate(0, 0, img, true);
    int timeout=1000;
    while(this->clustered_sfr_map.size() != 1 && timeout >0) {
        Sleep(10);
        timeout--;
    }
    vector<Sfr_entry> sv = clustered_sfr_map[0];
    double r1 = sqrt(imageCenterX*imageCenterX + imageCenterY*imageCenterY);

    qPainter.setFont(QFont("Times",50, QFont::Bold));
    qPainter.drawText(imageCenterX/2 , 100 , QString("DFOV: ").append(QString::number(dfov)));
    for (unsigned int i = 0; i < sv.size(); i++)
    {
        double roi_width = sqrt(sv[i].area)*this->parameters.ROIRatio();
        qInfo("%f %f %f %f %f %f %d %d", sv.at(i).px, sv.at(i).py,
              sv.at(i).t_sfr, sv.at(i).r_sfr, sv.at(i).b_sfr, sv.at(i).l_sfr,
              sv.at(i).layer, sv.at(i).location);
        double radius = sqrt(pow(sv[i].px - imageCenterX, 2) + pow(sv[i].py - imageCenterY, 2));

        if (sv[i].layer == 0) {
            qPainter.setPen(QPen(Qt::red, 4.0));
        }
        else if (sv[i].layer == 1) {
            qPainter.setPen(QPen(QColor(102, 0, 204), 4.0)); //Purple
        }
        else if (sv[i].layer == 2) {
            qPainter.setPen(QPen(Qt::blue, 4.0));
        }
        else if (sv[i].layer == 3) {
            qPainter.setPen(QPen(Qt::yellow, 4.0));
        }
        double w_t = parameters.WeightList()[0 + sv[i].layer*4].toDouble();
        double w_r = parameters.WeightList()[1 + sv[i].layer*4].toDouble();
        double w_b = parameters.WeightList()[2 + sv[i].layer*4].toDouble();
        double w_l = parameters.WeightList()[3 + sv[i].layer*4].toDouble();

        qPainter.drawRect(QRectF(sv[i].px-roi_width/2, sv[i].py-roi_width/2, roi_width, roi_width));
        qPainter.drawEllipse(QPoint(imageCenterX, imageCenterY), (int)(radius), (int)(radius));

        qPainter.drawText(sv[i].px - 50 , sv[i].py - roi_width/2, QString("").append(QString::number(w_t)));
        qPainter.drawText(sv[i].px - 50 + roi_width/2, sv[i].py,  QString("").append(QString::number(w_r)));
        qPainter.drawText(sv[i].px - 50, sv[i].py + roi_width/2,  QString("").append(QString::number(w_b)));
        qPainter.drawText(sv[i].px - roi_width/2 - 50, sv[i].py,  QString("").append(QString::number(w_l)));
    }
    clustered_sfr_map.clear();
    qPainter.end();
    aaCoreTuningProvider->setImage(outImage);
    emit callQmlRefeshImg(2);
}
