#include "AACore/aacorenew.h"
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
#include <QPainter>
#include "aa_util.h"
#include "utils/commonutils.h"
#include "vision/visionmodule.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "sfr.h"
#define PI  3.14159265
#include <ipcclient.h>
#include <math.h>
#include "tcpmessager.h"
#include "materialtray.h"
vector<double> fitCurve(const vector<double> & x, const vector<double> & y, int order, double & localMaxX,
                        double & localMaxY, double & error_avg, double & error_dev, vector<double> & y_output) {
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
        //order次方
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

    double error = 0;
    double average = 0;
    for (size_t i = 0; i < n; ++i) {
        average += (B(i, 0) - Y(i, 0));
    }
    average = average/n;
    //qInfo("Error Average: %f", average);
    error_avg = average;

    for (size_t i = 0; i < n; ++i) {
        double diff = B(i, 0) - Y(i, 0) - average;
        error +=(diff * diff);
    }
    error_dev = error;
    vector<double> ans;
    for (int i = 0; i <= order; ++i) {
        ans.push_back(A(i, 0));
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

    for (size_t j = 0; j < x.size(); j++){
        double tmp = 1; double ey = 0;
        for (int i = 0; i <= order; ++i){
            ey += A(i,0)*tmp;
            tmp *= x[j];
        }
        y_output.push_back(ey);
    }
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
}

void AACoreNew::Init(AAHeadModule *aa_head, SutModule *sut, Dothinkey *dk, ChartCalibration *chartCalibration,
                     DispenseModule *dispense, ImageGrabbingWorkerThread *imageThread, Unitlog *unitlog, int serverMode)
{
    that = this;
    setName(parameters.moduleName());
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
    aaCoreTuningProvider = new ImageProvider();
    dispenseImageProvider = new ImageProvider();
    connect(this, &AACoreNew::sfrResultsReady, this, &AACoreNew::storeSfrResults, Qt::DirectConnection);
    connect(this, &AACoreNew::sfrResultsDetectFinished, this, &AACoreNew::stopZScan, Qt::DirectConnection);
    //connect(&this->parameters, &AACoreParameters::paramsChanged, this, &AACoreNew::aaCoreParametersChanged);
    this->serverMode = serverMode;
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
    performTerminate(); //Close camera first

    QElapsedTimer timer;timer.start();
    while(is_run) {
        qInfo("AACore is running");
        timer.restart();
        runningUnit = this->unitlog->createUnit();
        //Reset some previous result
        oc_fov = -1;
        runFlowchartTest();
        emit postDataToELK(this->runningUnit);
        QThread::msleep(100);
        double temp_time = timer.elapsed();
        temp_time/=1000;
        qInfo("cycle_time :%f",temp_time);
        states.setCircleTime(temp_time);
        if(states.circleTime() > parameters.minCircleTime() && states.circleTime() < parameters.maxCicleTime())
        {
            states.setCircleCount(states.circleCount()+1);
            double temp_average = states.circleAverageTime()*(states.circleCount()-1) + states.circleTime();
            temp_average /= states.circleCount();
            temp_average *= 1000;
            temp_average = round(temp_average)/1000;
            qInfo("CircleAverageTime :%f",temp_average);
            states.setCircleAverageTime(temp_average);
        }
    }
    states.setRunMode(RunMode::Normal);
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

void AACoreNew::SetNoLens()
{
    has_lens = false;
    has_ng_lens = false;
}

void AACoreNew::SetSensor()
{
    has_sensor = true;
    has_ng_sensor = false;
    has_product = false;
    has_ng_product = false;
}

void AACoreNew::SetNoSensor()
{
    has_sensor = false;
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
    has_lens = false;
    has_ng_lens = false;
}

double AACoreNew::getSFRDev_mm(int count,double numbers,...)
{
    va_list arg_ptr;
    va_start(arg_ptr,numbers);
    double max = numbers;
    double min = max;
    count--;
    while (count >0) {
        numbers = va_arg(arg_ptr,double);
        if(max < numbers)
        {
            max = numbers;
        }
        if(min > numbers)
        {
            min = numbers;
        }
        count--;
    }
    va_end(arg_ptr);
    return max - min;
}
double AACoreNew::getzPeakDev_um(int count,double numbers,...)
{
    va_list arg_ptr;
    va_start(arg_ptr,numbers);
    double max = numbers;
    double min = max;
    bool temp = true;
    count--;
    while (count>0) {
        numbers = va_arg(arg_ptr,double);
        if(max < numbers)
        {
            temp = false;
            max = numbers;
        }
        if(min > numbers)
        {
            temp = true;
            min = numbers;
        }
        count--;
    }
    va_end(arg_ptr);
    if(temp)
        return (max- min)*1000;
    else
        return -(max- min)*1000;
}

void AACoreNew::startWork( int run_mode)
{
    QVariantMap run_params = inquirRunParameters();
    if(run_params.isEmpty())
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数为空.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(run_params.contains("RunMode"))
    {
        states.setRunMode(run_params["RunMode"].toInt());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数RunMode缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(run_params.contains("StationNumber"))
    {
        QString local_station = run_params["StationNumber"].toString();
        states.setStationNumber(local_station.toInt());
        if(run_params.contains("DisableStation"))
        {
            QVariantMap disable_map = run_params["DisableStation"].toMap();
            states.setDisableStation(disable_map[local_station].toBool());
        }
        else
        {
            sendAlarmMessage(OK_OPERATION,u8"启动参数DisableStation缺失.启动失败.",ErrorLevel::ErrorMustStop);
            return;
        }
        if(run_params.contains("StationTask"))
        {
            QVariantMap task_map = run_params["StationTask"].toMap();
            states.setStationTask(task_map[local_station].toInt());
        }
        else
        {
            sendAlarmMessage(OK_OPERATION,u8"启动参数StationTask缺失.启动失败.",ErrorLevel::ErrorMustStop);
            return;
        }
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数StationNumber缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(states.disableStation())
        return;
    emit clearHeaders();
    qInfo("startWork clearHeaders");
    if (run_mode == RunMode::Normal) run(true);
    else if (run_mode == RunMode::NoMaterial) {
        run(false);
    } else if (run_mode == RunMode::VibrationTest) {
        is_run = true;
        mtf_log.clear();
        loopTestResult = "";
        loopTestResult.append("CC, UL,UR,LL,LR,\n");
        while (is_run) {
            QJsonObject  params;
            params["CC"] = 0;
            params["UL"] = 0;
            params["UR"] = 0;
            params["LL"] = 0;
            params["LR"] = 0;
            params["SFR_DEV_TOL"] = 100;
            performMTFNew(params,true);
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
        states.setCircleTime(temp_time);
    }
}

void AACoreNew::stopWork(bool wait_finish)
{
    qInfo("AACorenew stop work");
    is_run = false;
    return;
}

void AACoreNew::performHandlingOperation(int cmd,QVariant param)
{
    emit clearHeaders();
    qInfo("AACore perform command: %d parmas :%s", cmd, param.toString().toStdString().c_str());
    QJsonDocument jsonDoc = QJsonDocument::fromJson(param.toString().toLocal8Bit().data());
    QJsonValue params = jsonDoc.object();

    runningUnit = this->unitlog->createUnit();
    if (cmd == HandleTest::Dispense)
    {
        sut->DownlookPrDone = false;
        bool enable = parameters.enableCheckDispense();
        int check_count = parameters.checkDispenseCount();
        parameters.setEnableCheckDispense(true);
        parameters.setCheckDispenseCount(1);
        ErrorCodeStruct ret = performDispense(params);
        parameters.setEnableCheckDispense(enable);
        parameters.setCheckDispenseCount(check_count);
        sut->vision_downlook_location->OpenLight();
        qInfo("End of perform Dispense %s",ret.errorMessage.toStdString().c_str());
    }
    else if (cmd == HandleTest::PR_To_Bond)
    {
        SetSensor();
        SetLens();
        performPRToBond(0);
    }
    else if (cmd == HandleTest::MTF) {
        performMTFNew(params);
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
        //performAAOffline();
        aaData_1.setInProgress(false);
        aaData_2.setInProgress(false);
    }
    else if (cmd == HandleTest::INIT_CAMERA) {
        SetSensor();
        int finish_delay = params["delay_in_ms"].toInt(0);
        performInitSensor(finish_delay);
    }
    else if (cmd == HandleTest::Y_LEVEL) {
        performYLevelTest(params);
    }
    else if (cmd == HandleTest::Z_OFFSET) {
        performZOffset(params);
    }
    else if (cmd == HandleTest::UV) {
        performUV(params);
    }
    else if (cmd == HandleTest::OTP) {
        performOTP(params);
    }
    else if (cmd == HandleTest::UNLOAD_CAMERA) {
        int finish_delay = params["delay_in_ms"].toInt(0);
        performCameraUnload(finish_delay);
    }
    else if (cmd == HandleTest::MOVE_LENS) {
        performVCMInit(params);
    }
    else if (cmd == HandleTest::INIT_VCM) {
        QJsonObject temp_params;
        temp_params["target_position"] = -1;
        performVCMInit(temp_params);
    }
    else if (cmd == HandleTest::LENS_VCM_POS) {
        QJsonObject temp_params;
        temp_params["target_position"] = parameters.lensVcmWorkPosition();
        performVCMInit(temp_params);
    }
    else if (cmd == HandleTest::XY_OFFSET) {
        performXYOffset(params);
    }
    else if (cmd == HandleTest::HOME_TILT) {
        this->aa_head->homeTilt();
    }
    else if (cmd == HandleTest::AA_HEAD_MOVE_TO_MUSHROOM) {
        this->aa_head->moveToMushroomPosition();
    }
    else if (cmd == HandleTest::AA_HEAD_MOVE_TO_PICK_LENS) {
        this->aa_head->moveToPickLensPosition();
    }
    else if (cmd == HandleTest::DISPENSE_XY_OFFSET_TEST) {
        this->dispense->moveToDispenseDot(false);
    }
    else if (cmd == HandleTest::DISPENSE_Z_TEST) {
        this->dispense->moveToDispenseDot();
        emit needUpdateParameterInTcpModule();
    }
    emit postDataToELK(this->runningUnit);
    is_handling = false;
}

void AACoreNew::resetLogic()
{
    if(is_run)return;
    states.reset();
    has_product = false;
    has_ng_product = false;
    has_ng_lens = false;
    has_ng_sensor = false;
    has_sensor = false;
    has_lens = false;
    send_lens_request = false;
    finish_lens_request = false;
    send_sensor_request = false;
    finish_sensor_request = false;
    aa_head->receive_sensor = false;
    aa_head->waiting_sensor = false;
    aa_head->receive_lens = false;
    aa_head->waiting_lens = false;
    current_aa_ng_time = 0;
    current_oc_ng_time = 0;
    current_mtf_ng_time = 0;
    grr_repeat_time = 0;
    grr_change_time = 0;
    current_dispense = 0;
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
    runningTestName = testName;
    QJsonValue params = properties["params"];
    int retry_count = params["retry"].toInt(0);
    QJsonValue delay_in_ms_qjv = params["delay_in_ms"];
    unsigned int delay_in_ms = delay_in_ms_qjv.toInt(0);
    for (int i = 0; i <= retry_count; i++) {
        parameters.setAACoreRunningTest("Running test: " + testItemName);
        if (testItemName.contains(AA_PIECE_START)) { qInfo("Performing Start"); }
        else if (testItemName.contains(AA_PIECE_LOAD_CAMERA)) {
            qInfo("Performing load camera");
        }
        else if (testItemName.contains(AA_PIECE_INIT_LENS)) {
            int finish_delay = params["delay_in_ms"].toInt(0);
            double target_position = params["target_position"].toDouble();
            qInfo("Performing init lens :%d position %f",finish_delay,target_position);
            ret = performVCMInit(params);
            qInfo("End of init camera %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_INIT_CAMERA)) {
            int finish_delay = params["delay_in_ms"].toInt(0);
            qInfo("Performing init camera :%d",finish_delay);
            ret = performInitSensor(finish_delay);
            qInfo("End of init camera %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_PR_TO_BOND)) {
            int finish_delay = params["delay_in_ms"].toInt(0);
            qInfo("Performing PR To Bond :%d",finish_delay);
            ret = performPRToBond(finish_delay);
            qInfo("End of perform PR To Bond %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_INITIAL_TILT)) {
            qInfo("Performing Initial Tilt");
            double initial_roll = params["roll"].toDouble(0);
            double initial_pitch = params["pitch"].toDouble(0);
            int finish_delay = params["delay_in_ms"].toInt(0);
            aa_head->stepInterpolation_AB_Sync(initial_roll, initial_pitch);
            if(finish_delay>0)
                Sleep(finish_delay);
            qInfo("End of perform initial tilt");
        }
        else if (testItemName.contains(AA_PIECE_Z_OFFSET)) {
            qInfo("Performing Z Offset");
            performZOffset(params);
            qInfo("End of perform z offset");
        }
        else if (testItemName.contains(AA_PIECE_XY_OFFSET)) {
            qInfo("Performing XY Offset");
            performXYOffset(params);
            qInfo("End of perform xy offset");
        }
        else if (testItemName.contains(AA_PIECE_LOAD_MATERIAL)) {
            int finish_delay = params["delay_in_ms"].toInt(0);
            qInfo("Performing Load Material :%d",finish_delay);
            ret = performLoadMaterial(finish_delay);
            qInfo("End of perform Load Material %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_UNLOAD_LENS)) {
            qInfo("Performing AA unload lens");
        }
        else if (testItemName.contains(AA_UNLOAD_CAMERA)) {

            int finish_delay = params["delay_in_ms"].toInt(0);
            qInfo("AA Unload Camera delay %d",finish_delay);
            ret = performCameraUnload(finish_delay);
            qInfo("End of perform unload camera %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_OC)) {
            qInfo("Performing OC %s",params.toString().toStdString().c_str());
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
            ret = performMTFNew(params);
            qInfo("End of perform MTF %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_UV)) {
            qInfo("Performing UV");
            ret = performUV(params);
            qInfo("End of perform UV %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_OTP)) {
            qInfo("Performing OTP");
            ret = performOTP(params);
            qInfo("End of perform OTP %s",ret.errorMessage.toStdString().c_str());
        }
        else if (testItemName.contains(AA_PIECE_DISPENSE)) {
            qInfo("Performing Dispense",params.toString().toStdString().c_str());
            ret = performDispense(params);
            qInfo("End of perform Dispense %s",ret.errorMessage.toStdString().c_str());
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
    parameters.setAACoreRunningTest("");
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
            ret = that->performUV(params);
        } else if (testName.contains(AA_PIECE_PR_TO_BOND)) {
            int finish_delay = params["delay_in_ms"].toInt();
            qInfo("start performPRToBond :%d",finish_delay);
            ret = that->performPRToBond(finish_delay);
            qInfo("End performPRToBond %s",ret.errorMessage.toStdString().c_str());
        } else if (testName.contains(AA_PIECE_INIT_CAMERA)) {

            int finish_delay = params["delay_in_ms"].toInt(0);
            qInfo("Performing init camera :%d",finish_delay);
            ret = that->performInitSensor(finish_delay);
            qInfo("End performInitSensor %s",ret.errorMessage.toStdString().c_str());
        } else if (testName.contains(AA_PIECE_OTP)) {
            ret = that->performOTP(params);
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

ErrorCodeStruct AACoreNew::performDispense(QJsonValue params)
{
    double x_offset_in_um = params["x_offset_in_um"].toDouble(0);
    double y_offset_in_um = params["y_offset_in_um"].toDouble(0);
    double z_offset_in_um = params["z_offset_in_um"].toDouble(0);
    int finish_delay = params["delay_in_ms"].toInt(0);
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    sut->recordCurrentPos();
    dispense->setMapPosition(sut->downlook_position.X(),sut->downlook_position.Y());
    // Check if downlook sensor pr is done or not
    if (!sut->DownlookPrDone) {
        PrOffset offset;
        if(!sut->moveToDownlookPR(offset)){ NgProduct(); return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "downlook pr fail"};}
        dispense->setPRPosition(offset.X, offset.Y, offset.Theta);

    }
    else {
        dispense->setPRPosition(this->aa_head->offset_x,this->aa_head->offset_y,this->aa_head->offset_theta);
    }

    // Perform dispense
    if(!dispense->performDispense()) { NgProduct(); return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "dispense fail"};}

    current_dispense--;
    if(parameters.enableCheckDispense())
    {
        if(current_dispense<=0)
        {
            // Capture image after dispense
            QString imageNameAfterDispense;
            imageNameAfterDispense.append(getDispensePrLogDir())
                            .append(getCurrentTimeString())
                            .append("_")
                            .append(dk->readSensorID())
                            .append("_after_dispense.jpg");

            sut->moveToDownlookSaveImage(imageNameAfterDispense); // For save image only
            //ToDo: return QImage from this function
            QImage image(imageNameAfterDispense);
            dispenseImageProvider->setImage(image);
            emit callQmlRefeshImg(3);  //Emit dispense image to QML

            int alarm_id = sendAlarmMessage(CONTINUE_REJECT_OPERATION, u8"画胶检查");
            QString operation = waitMessageReturn(is_run,alarm_id);
            if (u8"抛料" == operation)
            {
                NgSensor();
                return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, u8"画胶检查"};
            }
            current_dispense = parameters.checkDispenseCount();
        }
    }
    SetProduct();
    if(!sut->movetoRecordPosAddOffset(x_offset_in_um/1000,y_offset_in_um/1000,z_offset_in_um/1000)){NgProduct(); return  ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "sut move to record pos fail"};}
    if(finish_delay>0)
        Sleep(finish_delay);
    map.insert("x_offset_in_um",x_offset_in_um);
    map.insert("y_offset_in_um",y_offset_in_um);
    map.insert("z_offset_in_um",z_offset_in_um);
    map.insert("z_peak",sut->carrier->GetFeedBackPos().Z);
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
//    int is_debug = params["is_debug"].toInt();
    //int is_debug = 0;
    int finish_delay = params["delay_in_ms"].toInt();
    double xsum=0,x2sum=0,ysum=0,xysum=0;
    qInfo("start : %f stop: %f enable_tilt: %d", start, stop, enableTilt);
    unsigned int zScanCount = 0;
    QElapsedTimer timer; timer.start();
    int resize_factor = 2;
    vector<double> fov_y; vector<double> fov_x;
    QPointF prev_point = {0, 0};
    double prev_fov_slope = 0;
    bool grabRet = true;
    int grab_time = 0;
    int step_move_time = 0;
    int sfr_wait_time = 0;
    int wait_tilt_time = 0;
    double zScanStopPosition = 0;
    QElapsedTimer step_move_timer;
    QElapsedTimer grab_timer;
    double estimated_aa_z = 0;
    if(zScanMode == ZSCAN_MODE::AA_ZSCAN_NORMAL) {
        unsigned int count = (int)fabs((start - stop)/step_size);
        for (unsigned int i = 0; i < count; i++)
        {
           step_move_timer.start();
           sut->moveToZPos(start+(i*step_size));
           zScanStopPosition = start+(i*step_size);
           QThread::msleep(zSleepInMs);
           step_move_time += step_move_timer.elapsed();
           double realZ = sut->carrier->GetFeedBackPos().Z;
           qInfo("Z scan start from %f, real: %f", start+(i*step_size), realZ);
           grab_timer.start();
           cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
           grab_time += grab_timer.elapsed();
           if (!grabRet) {
               qInfo("AA Cannot grab image.");
               NgSensor();
               map["Result"] = QString("AA Cannot grab image.i:%1").arg(i);
               emit pushDataToUnit(runningUnit, "AA", map);
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "AA Cannot Grab Image"};
           }
           if (!blackScreenCheck(img)) {
               NgSensor();
               map["Result"] = QString("AA Detect BlackScreen.i:%1").arg(i);
               emit pushDataToUnit(runningUnit, "AA", map);
               return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "AA Detect BlackScreen"};
           }
           cv::Mat dst;
           cv::Size size(img.cols/resize_factor, img.rows/resize_factor);
           cv::resize(img, dst, size);
           if(parameters.isDebug() == true)
           {
               QString imageName;
               imageName.append(getGrabberLogDir())
                       .append(sensorID)
                       .append("_")
                       .append(getCurrentTimeString())
                       .append(".bmp");
               cv::imwrite(imageName.toStdString().c_str(), img);
           }
           double dfov = calculateDFOV(img);
           if(current_dfov.contains(QString::number(i)))
                current_dfov[QString::number(i)] = dfov;
           else
                current_dfov.insert(QString::number(i),dfov);
           qInfo("fov: %f  sut_z: %f", dfov, sut->carrier->GetFeedBackPos().Z);
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
           step_move_timer.start();
           double dfov = -1;
           if (oc_fov < 0) {
               sut->moveToZPos(start);
               QThread::msleep(zSleepInMs);
               step_move_time += step_move_timer.elapsed();
               grab_timer.start();
               cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
               grab_time += grab_timer.elapsed();

               if (!grabRet) {
                   qInfo("AA Cannot grab image.");
                   map["Result"] = "AA Cannot grab image.";
                   emit pushDataToUnit(runningUnit, "AA", map);
                   NgSensor();
                   return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "AA Cannot Grab Image"};
               }
               if (!blackScreenCheck(img)) {
                   NgSensor();
                   map["Result"] = "AA Detect black screen";
                   emit pushDataToUnit(runningUnit, "AA", map);
                   return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "AA Detect BlackScreen"};
               }
               dfov = calculateDFOV(img);
               if (dfov <= -1) {
                   qInfo("Cannot find the target FOV!");
                   LogicNg(current_aa_ng_time);
                   return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
               }
               estimated_aa_z = (estimated_aa_fov - dfov)/estimated_fov_slope + start;
           } else {
               qInfo("Use the result FOV in previous OC. FOV = %f", oc_fov);
               dfov = this->oc_fov;
               estimated_aa_z = (estimated_aa_fov - dfov)/estimated_fov_slope + oc_z;
               oc_fov = -1;
               oc_z = 0;
           }
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
                sut->moveToZPos(target_z+(i*step_size));
                zScanStopPosition = start+(i*step_size);
                QThread::msleep(zSleepInMs);
                step_move_time += step_move_timer.elapsed();
                qInfo("Current Z: %f", sut->carrier->GetFeedBackPos().Z);
                grab_timer.start();
                cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
                grab_time += grab_timer.elapsed();
                if (!grabRet) {
                    qInfo("AA Cannot grab image.");
                    NgSensor();
                    map["Result"] = QString("AA Cannot grab image.i:%1").arg(i);
                    emit pushDataToUnit(runningUnit, "AA", map);
                    return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "AA Cannot Grab Image"};
                }
                if (!blackScreenCheck(img)) {
                    NgSensor();
                    map["Result"] = QString("AA Detect BlackScreen.i:%1").arg(i);
                    emit pushDataToUnit(runningUnit, "AA", map);
                    return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "AA Detect BlackScreen"};
                }

                if(parameters.isDebug() == true) {
                    QString imageName;
                    imageName.append(getGrabberLogDir())
                            .append(sensorID)
                            .append("_")
                            .append(getCurrentTimeString())
                            .append(".bmp");
                    cv::imwrite(imageName.toStdString().c_str(), img);
                }

                double realZ = sut->carrier->GetFeedBackPos().Z;
                double dfov = calculateDFOV(img);

                if (i > 1) {
                   double slope = (dfov - prev_point.y()) / (realZ - prev_point.x());
                   double error = 0;
                   if (prev_fov_slope != 0) {
                       error = (slope - prev_fov_slope) / prev_fov_slope;
                   }
                   qInfo("current slope %f  prev_slope %f error %f", slope, prev_fov_slope, error);
//                   if (fabs(error) > 0.5) {
//                       qInfo("Crash detection is triggered");
//                       LogicNg(current_aa_ng_time);
//                       map["Result"] = QString("Crash detection is triggered. prev_fov_slope:%1 now_fov_slope:%2 error:%3").arg(prev_fov_slope).arg(slope).arg(error);
//                       emit pushDataToUnit(runningUnit, "AA", map);
//                       return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
//                   }
                   prev_fov_slope = slope;
                }
                prev_point.setX(realZ); prev_point.setY(dfov);

                if(current_dfov.contains(QString::number(i)))
                     current_dfov[QString::number(i)] = dfov;
                else
                     current_dfov.insert(QString::number(i),dfov);
                qInfo("fov: %f  sut_z: %f", dfov, sut->carrier->GetFeedBackPos().Z);
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
         double currentZ = sut->carrier->GetFeedBackPos().Z;
         double target_z = currentZ + offset_in_um;
         for (unsigned int i = 0; i < imageCount; i++) {
             step_move_timer.start();
             sut->moveToZPos(target_z+(i*step_size));
             zScanStopPosition = start+(i*step_size);
             QThread::msleep(zSleepInMs);
             step_move_time += step_move_timer.elapsed();
             grab_timer.start();
             cv::Mat img = dk->DothinkeyGrabImageCV(0,grabRet);
             grab_time += grab_timer.elapsed();
             if (!grabRet) {
                 qInfo("AA Cannot grab image.");
                 NgSensor();
                 map["Result"] = QString("AA Cannot grab image.i:%1").arg(i);
                 emit pushDataToUnit(runningUnit, "AA", map);
                 return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "AA Cannot Grab Image"};
             }
             if (!blackScreenCheck(img)) {
                 NgSensor();
                 map["Result"] = QString("AA Detect BlackScreen.i:%1").arg(i);
                 emit pushDataToUnit(runningUnit, "AA", map);
                 return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "AA Detect BlackScreen"};
             }
             double realZ = sut->carrier->GetFeedBackPos().Z;
             double dfov = calculateDFOV(img);
             if(current_dfov.contains(QString::number(i)))
                  current_dfov[QString::number(i)] = dfov;
             else
                  current_dfov.insert(QString::number(i),dfov);
             qInfo("fov: %f  sut_z: %f", dfov, sut->carrier->GetFeedBackPos().Z);
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
    map.insert("CC_Zpeak_Dev", round(aa_result["CC_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("UL_03F_Zpeak_Dev", round(aa_result["UL_03F_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("UR_03F_Zpeak_Dev", round(aa_result["UR_03F_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("LR_03F_Zpeak_Dev", round(aa_result["LR_03F_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("LL_03F_Zpeak_Dev", round(aa_result["LL_03F_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("UL_05F_Zpeak_Dev", round(aa_result["UL_05F_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("UR_05F_Zpeak_Dev", round(aa_result["UR_05F_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("LR_05F_Zpeak_Dev", round(aa_result["LR_05F_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("LL_05F_Zpeak_Dev", round(aa_result["LL_05F_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("UL_08F_Zpeak_Dev", round(aa_result["UL_08F_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("UR_08F_Zpeak_Dev", round(aa_result["UR_08F_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("LR_08F_Zpeak_Dev", round(aa_result["LR_08F_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("LL_08F_Zpeak_Dev", round(aa_result["LL_08F_Zpeak_Dev"].toDouble()*1000)/1000);
    map.insert("Mode", zScanMode);
    map.insert("START_POS", start);
    map.insert("STOP_POS", stop);
    map.insert("STEP_SIZE", step_size);
    map.insert("IMAGE_COUNT", imageCount);
    map.insert("STEP_MOVE_TIME", step_move_time);
    map.insert("GRAB_TIME", grab_time);
    map.insert("SFR_WAIT_TIME", sfr_wait_time);
    map.insert("TILT_WAIT_TIME", wait_tilt_time);
    map.insert("X_TILT", round(aa_result["xTilt"].toDouble()*1000)/1000);
    map.insert("Y_TILT", round(aa_result["yTilt"].toDouble()*1000)/1000);
    map.insert("Z_PEAK_CC_um", round((aa_result["zPeak_cc"].toDouble()*1000)*1000)/1000);
    map.insert("Z_PEAK_03_um", round((aa_result["zPeak_03"].toDouble()*1000)*1000)/1000);
    map.insert("Z_PEAK_05_um", round((aa_result["zPeak_05"].toDouble()*1000)*1000)/1000);
    map.insert("Z_PEAK_08_um", round((aa_result["zPeak_08"].toDouble()*1000)*1000)/1000);
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
    double z_peak = aa_result["zPeak"].toDouble();
    sut->moveToZPos(z_peak);
    qInfo("zpeak: %f",z_peak);
    step_move_time += step_move_timer.elapsed();
    if (enableTilt == 0) {
        qInfo("Disable tilt...");
    } else {
        qInfo("Enable tilt...xTilt: %f yTilt: %f", aa_result["xTilt"].toDouble(), aa_result["yTilt"].toDouble());
        step_move_timer.start();
        double tilt_a,tilt_b;
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
        wait_tilt_time += step_move_timer.elapsed();
    }
    double zpeak_dev = getzPeakDev_um(3,aa_result["zPeak_cc"].toDouble(),aa_result["zPeak_05"].toDouble(),aa_result["zPeak_08"].toDouble());
    qInfo("zpeak_dev: %f",zpeak_dev);
    double zpeak_dev_cc_05 = getzPeakDev_um(2,aa_result["zPeak_cc"].toDouble()-aa_result["zPeak_05"].toDouble());
    qInfo("zpeak_dev_cc_05: %f",zpeak_dev);
    double zpeak_dev_cc_08 = getzPeakDev_um(2,aa_result["zPeak_cc"].toDouble()-aa_result["zPeak_08"].toDouble());
    qInfo("zpeak_dev_cc_08: %f",zpeak_dev);

    double zPeakDiff05Max = parameters.zPeakDiff05Max();
    double zPeakDiff08Max = parameters.zPeakDiff08Max();
    double zPeakDiff05F = 1000 * fabs((aa_result["zPeak_2_UL"].toDouble()+aa_result["zPeak_2_LR"].toDouble())/2 - (aa_result["zPeak_2_LL"].toDouble()+aa_result["zPeak_2_UR"].toDouble())/2);
    double zPeakDiff08F = 1000 * fabs((aa_result["zPeak_3_UL"].toDouble()+aa_result["zPeak_3_LR"].toDouble())/2 - (aa_result["zPeak_3_LL"].toDouble()+aa_result["zPeak_3_UR"].toDouble())/2);
    map["zPeak_05F_Diff_um"] = round(zPeakDiff05F*1000)/1000;
    map["zPeak_08F_Diff_um"] = round(zPeakDiff08F*1000)/1000;
    qInfo("Check 05F zPeak %f", zPeakDiff05F);
    qInfo("Check 08F zPeak %f", zPeakDiff08F);

    map.insert("Z_PEAK_um", round((z_peak*1000)*1000)/1000);
    map.insert("Z_PEAK_DEV_CC_05_um", round(zpeak_dev_cc_05*1000)/1000);
    map.insert("Z_PEAK_DEV_CC_08_um", round(zpeak_dev_cc_08*1000)/1000);
    map.insert("Z_PEAK_DEV_um", round(zpeak_dev*1000)/1000);
    if (position_checking == 1){
        double maxZPeak = aa_result["maxZPeak"].toDouble();
        if ( fabs(zScanStopPosition - maxZPeak) < 0.001 ) {
            LogicNg(current_aa_ng_time);
            map["Result"] = QString("one of the zpeak too close to z scan boundary.(%1)").arg(maxZPeak);
            emit pushDataToUnit(runningUnit, "AA", map);
            return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, map["Result"].toString()};
        }
        if(zpeak_dev > parameters.maxDev()|| zpeak_dev < parameters.minDev())
        {
            LogicNg(current_aa_ng_time);
            map["Result"] = QString("zpeak dev(%1) fail.").arg(zpeak_dev);
            emit pushDataToUnit(runningUnit, "AA", map);
            return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, map["Result"].toString()};
        }

        if(zpeak_dev_cc_05 > parameters.CC05MaxDev() || zpeak_dev_cc_05 < parameters.CC05MinDev())
        {
            LogicNg(current_aa_ng_time);
            map["Result"] = QString("zpeak dev CC_05(%1) fail.").arg(zpeak_dev_cc_05);
            emit pushDataToUnit(runningUnit, "AA", map);
            return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, map["Result"].toString()};
        }
        if(zpeak_dev_cc_08 > parameters.CC08MaxDev() || zpeak_dev_cc_08 < parameters.CC08MinDev())
        {
            LogicNg(current_aa_ng_time);
            map["Result"] = QString("zpeak dev CC_08(%1) fail.").arg(zpeak_dev_cc_08);
            emit pushDataToUnit(runningUnit, "AA", map);
            return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, map["Result"].toString()};
        }

        // Check 05F&08F ROI ZPeak
        if (zPeakDiff05F > zPeakDiff05Max)
        {
            qInfo("Check 05F zPeak diff fail with %f", zPeakDiff05F);
            LogicNg(current_aa_ng_time);
            map["Result"] = QString("05F zPeak diff (%1) fail.").arg(zPeakDiff05F);
            emit pushDataToUnit(runningUnit, "AA", map);
            return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, map["Result"].toString()};
        }
        if (zPeakDiff08F > zPeakDiff08Max)
        {
            qInfo("Check 08F zPeak diff fail with %f", zPeakDiff08F);
            LogicNg(current_aa_ng_time);
            map["Result"] = QString("08F zPeak diff (%1) fail.").arg(zPeakDiff08F);
            emit pushDataToUnit(runningUnit, "AA", map);
            return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, map["Result"].toString()};
        }

        QThread::msleep(zSleepInMs);
        cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
        double beforeZ = sut->carrier->GetFeedBackPos().Z;
        double expected_fov = fov_slope*z_peak + fov_intercept;
        double dfov = calculateDFOV(img);
        double diff_z = (dfov - expected_fov)/fov_slope;
        sut->moveToZPos(beforeZ - diff_z);
        double afterZ = sut->carrier->GetFeedBackPos().Z;
        map.insert("Z_PEAK_Checked",round(-diff_z*1000)/1000);
        map.insert("Final_X", sut->carrier->GetFeedBackPos().X);
        map.insert("Final_Y", sut->carrier->GetFeedBackPos().Y);
        map.insert("Final_Z", sut->carrier->GetFeedBackPos().Z);
        qInfo("before z: %f after z: %f now fov: %f expected fov: %f fov slope: %f fov intercept: %f", beforeZ, afterZ, dfov, expected_fov, fov_slope, fov_intercept);
    }
    else {
        map.insert("Z_PEAK_Checked",0);
    }
    clustered_sfr_map.clear();
    qInfo("AA time elapsed: %d", timer.elapsed());
    if(finish_delay>0)
        Sleep(finish_delay);
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "AA", map);
    return ErrorCodeStruct{ ErrorCode::OK, ""};
}

void AACoreNew::performAAOffline()
{
    clustered_sfr_map.clear();
    ErrorCodeStruct ret = { OK, ""};
    QVariantMap map, stepTimerMap, dFovMap, sfrTimeElapsedMap;
    QElapsedTimer timer;
    timer.start();
    int resize_factor = 1;
    int sfrCount = 0;
    double step_size = 0.01, start = -20.5;
    double xsum=0,x2sum=0,ysum=0,xysum=0;
    double estimated_fov_slope = 15;
    isZScanNeedToStop = false;
    QString foldername = AA_DEBUG_DIR;
    int inputImageCount = 17;
    for (int i = 0; i < inputImageCount -1; i++)
    {
        if (isZScanNeedToStop) {
            qInfo("All peak passed, stop zscan");
            break;
        }
        //QString filename = "aa_log\\aa_log_bug\\2018-11-10T14-42-55-918Z\\zscan_" + QString::number(i) + ".bmp";
        //QString filename = "aa_log\\aa_log_bug\\2018-11-10T14-42-55-918Z\\zscan_" + QString::number(i) + ".bmp";
        QString filename = "C:\\Users\\emil\\Desktop\\Test\\Samsung\\debug\\debug\\zscan_" + QString::number(i) + ".bmp";
        //QString filename = "offline\\" + QString::number(i) + ".bmp";
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
    int timeout=2000;
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
    map.insert("X_TILT", round(aa_result["xTilt"].toDouble()*1000)/1000);
    map.insert("Y_TILT", round(aa_result["yTilt"].toDouble()*1000)/1000);
    map.insert("Z_PEAK_CC_um", round((aa_result["zPeak_cc"].toDouble()*1000)*1000)/1000);
    map.insert("Z_PEAK_03_um", round((aa_result["zPeak_03"].toDouble()*1000)*1000)/1000);
    map.insert("Z_PEAK_05_um", round((aa_result["zPeak_05"].toDouble()*1000)*1000)/1000);
    map.insert("Z_PEAK_08_um", round((aa_result["zPeak_08"].toDouble()*1000)*1000)/1000);
    qInfo("MaxPeakZ: %f", aa_result["maxPeakZ"].toDouble());
    clustered_sfr_map.clear();
    qInfo("[PerformAAOffline] time elapsed: %d", timer.elapsed());
    emit pushDataToUnit(runningUnit, "AA", map);
}

void AACoreNew::performHandling(int cmd, QString params)
{
    qInfo("performHandling: %d %s", cmd, params.toStdString().c_str());
//    handlingParams = params;
    emit sendHandlingOperation(cmd,params);
}

bool zPeakComp(const threeDPoint & p1, const threeDPoint & p2)
{
    return p1.z < p2.z;
}

QVariantMap AACoreNew::sfrFitCurve_Advance(int resize_factor, double start_pos)
{
    QVariantMap result, map;
    map.insert("SensorID",sensorID);
    vector<vector<Sfr_entry>> sorted_sfr_map;
    vector<vector<double>> sorted_sfr_fit_map;
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
    threeDPoint point_0;
    vector<threeDPoint> points_1, points_11;
    vector<threeDPoint> points_2, points_22;
    vector<threeDPoint> points_3, points_33;
    double maxPeakZ = -99999;
    for (size_t i = 0; i < sorted_sfr_map.size(); i++) {
        vector<double> sfr,b_sfr,t_sfr,l_sfr,r_sfr, z;
        vector<double> sfr_fit, b_sfr_fit, t_sfr_fit, l_sfr_fit, r_sfr_fit;
        double ex = 0; double ey = 0;
        for (size_t ii=0; ii < sorted_sfr_map[i].size(); ii++) {
            //double avg_sfr = sorted_sfr_map[i][ii].sfr;
            qInfo("sorted_sfr_map[%d][%d]: location:%d, px:%f ,py:%f",i,ii,sorted_sfr_map[i][ii].location,sorted_sfr_map[i][ii].px,sorted_sfr_map[i][ii].py);
            double avg_sfr = 0;
            if (sorted_sfr_map[i][ii].location == 1) {
                //UL 1
                avg_sfr = parameters.WeightList().at(0).toDouble()*sorted_sfr_map[i][ii].t_sfr + parameters.WeightList().at(1).toDouble()*sorted_sfr_map[i][ii].r_sfr
                        + parameters.WeightList().at(2).toDouble()*sorted_sfr_map[i][ii].b_sfr + parameters.WeightList().at(3).toDouble()*sorted_sfr_map[i][ii].l_sfr;
            }
            else if (sorted_sfr_map[i][ii].location == 4) {
                //LL 2
                avg_sfr = parameters.WeightList().at(4).toDouble()*sorted_sfr_map[i][ii].t_sfr + parameters.WeightList().at(5).toDouble()*sorted_sfr_map[i][ii].r_sfr
                        + parameters.WeightList().at(6).toDouble()*sorted_sfr_map[i][ii].b_sfr + parameters.WeightList().at(7).toDouble()*sorted_sfr_map[i][ii].l_sfr;
            }
            else if (sorted_sfr_map[i][ii].location == 3) {
                //LR 3
                avg_sfr = parameters.WeightList().at(8).toDouble()*sorted_sfr_map[i][ii].t_sfr + parameters.WeightList().at(9).toDouble()*sorted_sfr_map[i][ii].r_sfr
                        + parameters.WeightList().at(10).toDouble()*sorted_sfr_map[i][ii].b_sfr + parameters.WeightList().at(11).toDouble()*sorted_sfr_map[i][ii].l_sfr;
            }
            else if (sorted_sfr_map[i][ii].location == 2) {
                //UR 4
                avg_sfr = parameters.WeightList().at(12).toDouble()*sorted_sfr_map[i][ii].t_sfr + parameters.WeightList().at(13).toDouble()*sorted_sfr_map[i][ii].r_sfr
                        + parameters.WeightList().at(14).toDouble()*sorted_sfr_map[i][ii].b_sfr + parameters.WeightList().at(15).toDouble()*sorted_sfr_map[i][ii].l_sfr;
            } else {
                avg_sfr = (sorted_sfr_map[i][ii].t_sfr + sorted_sfr_map[i][ii].r_sfr + sorted_sfr_map[i][ii].b_sfr + sorted_sfr_map[i][ii].l_sfr)/4;
            }
            sfr.push_back(avg_sfr);
            b_sfr.push_back(sorted_sfr_map[i][ii].b_sfr);
            t_sfr.push_back(sorted_sfr_map[i][ii].t_sfr);
            l_sfr.push_back(sorted_sfr_map[i][ii].l_sfr);
            r_sfr.push_back(sorted_sfr_map[i][ii].r_sfr);

            z.push_back(sorted_sfr_map[i][ii].pz-start_pos);
            ex += sorted_sfr_map[i][ii].px*resize_factor;
            ey += sorted_sfr_map[i][ii].py*resize_factor;
        }
        ex /= (sorted_sfr_map[i].size()*parameters.SensorXRatio());
        ey /= (sorted_sfr_map[i].size()*parameters.SensorYRatio());

        double peak_sfr, peak_z,b_peak_z,t_peak_z,l_peak_z,r_peak_z;

        double error_avg,error_dev;
        fitCurve(z, b_sfr, fitOrder, b_peak_z, peak_sfr,error_avg,error_dev, b_sfr_fit);
        fitCurve(z, t_sfr, fitOrder, t_peak_z, peak_sfr,error_avg,error_dev, t_sfr_fit);
        fitCurve(z, l_sfr, fitOrder, l_peak_z, peak_sfr,error_avg,error_dev, l_sfr_fit);
        fitCurve(z, r_sfr, fitOrder, r_peak_z, peak_sfr,error_avg,error_dev, r_sfr_fit);

        if (b_peak_z > maxPeakZ) maxPeakZ = b_peak_z;
        if (t_peak_z > maxPeakZ) maxPeakZ = t_peak_z;
        if (l_peak_z > maxPeakZ) maxPeakZ = l_peak_z;
        if (r_peak_z > maxPeakZ) maxPeakZ = r_peak_z;

        qInfo("%i b_peak_z %f ",i,b_peak_z + start_pos);
        qInfo("%i t_peak_z %f ",i,t_peak_z + start_pos);
        qInfo("%i l_peak_z %f ",i,l_peak_z + start_pos);
        qInfo("%i r_peak_z %f ",i,r_peak_z + start_pos);
        double dev = abs(getzPeakDev_um(4,b_peak_z,t_peak_z,l_peak_z,r_peak_z));
        qInfo("%i peak_z_dev %f ",i,dev);
        switch (i) {
        case 0:
            result.insert("CC_Zpeak_Dev", dev); map.insert("CC_Zpeak_Dev", dev);
            break;
        case 1:
            result.insert("UL_03F_Zpeak_Dev", dev); map.insert("UL_03F_Zpeak_Dev", dev);
            break;
        case 2:
            result.insert("UR_03F_Zpeak_Dev", dev); map.insert("UR_03F_Zpeak_Dev", dev);
            break;
        case 3:
            result.insert("LR_03F_Zpeak_Dev", dev); map.insert("LR_03F_Zpeak_Dev", dev);
            break;
        case 4:
            result.insert("LL_03F_Zpeak_Dev", dev); map.insert("LL_03F_Zpeak_Dev", dev);
            break;
        case 5:
            result.insert("UL_05F_Zpeak_Dev", dev); map.insert("UL_05F_Zpeak_Dev", dev);
            break;
        case 6:
            result.insert("UR_05F_Zpeak_Dev", dev); map.insert("UR_05F_Zpeak_Dev", dev);
            break;
        case 7:
            result.insert("LR_05F_Zpeak_Dev", dev); map.insert("LR_05F_Zpeak_Dev", dev);
            break;
        case 8:
            result.insert("LL_05F_Zpeak_Dev", dev); map.insert("LL_05F_Zpeak_Dev", dev);
            break;
        case 9:
            result.insert("UL_08F_Zpeak_Dev", dev); map.insert("UL_08F_Zpeak_Dev", dev);
            break;
        case 10:
            result.insert("UR_08F_Zpeak_Dev", dev); map.insert("UR_08F_Zpeak_Dev", dev);
            break;
        case 11:
            result.insert("LR_08F_Zpeak_Dev", dev); map.insert("LR_08F_Zpeak_Dev", dev);
            break;
        case 12:
            result.insert("LL_08F_Zpeak_Dev", dev); map.insert("LL_08F_Zpeak_Dev", dev);
            break;
        default:
            break;
        }
        fitCurve(z, sfr, fitOrder, peak_z, peak_sfr,error_avg,error_dev, sfr_fit);
        sorted_sfr_fit_map.push_back(sfr_fit); //Used to display the curve with fitting result
        if (i==0) {
            point_0.x = ex; point_0.y = ey; point_0.z = peak_z + start_pos;
            result.insert("fitCurveErrorDevCC", error_dev); map.insert("fitCurveErrorDevCC", error_dev);
            qInfo("fitCurveErrorDevCC:avg:%f,dev:%f",error_avg,error_dev);
        } else if ( i >= 1 && i <= 4) {
            points_1.emplace_back(ex, ey, peak_z + start_pos);
            points_11.emplace_back(ex, ey, peak_z + start_pos);
        } else if ( i >= 5 && i <= 8) {
            points_2.emplace_back(ex, ey, peak_z + start_pos);
            points_22.emplace_back(ex, ey, peak_z + start_pos);
            if(i==5)
            {
                result.insert("fitCurveErrorDevUL", error_dev); map.insert("fitCurveErrorDevUL", error_dev);
                qInfo("fitCurveErrorDev05UL:avg%f,dev:%f",error_avg,error_dev);
            }
            else if(i==6)
            {
                result.insert("fitCurveErrorDevUR", error_dev); map.insert("fitCurveErrorDevUR", error_dev);
                qInfo("fitCurveErrorDev05UR:avg%f,dev:%f",error_avg,error_dev);
            }
            else if(i==7)
                {
                result.insert("fitCurveErrorDevLR", error_dev); map.insert("fitCurveErrorDevLR", error_dev);
                qInfo("fitCurveErrorDev05LR:avg%f,dev:%f",error_avg,error_dev);
            }
            else if(i==8)
                {
                result.insert("fitCurveErrorDevLL", error_dev); map.insert("fitCurveErrorDevLL", error_dev);
                qInfo("fitCurveErrorDev05UL:avg%f,dev:%f",error_avg,error_dev);
            }
        } else if ( i >= 9 && i <= 12) {
            points_3.emplace_back(ex, ey, peak_z + start_pos);
            points_33.emplace_back(ex, ey, peak_z + start_pos);
            if(i==9)
            {
                result.insert("fitCurveErrorDevUL", error_dev); map.insert("fitCurveErrorDevUL", error_dev);
                qInfo("fitCurveErrorDev08UL:avg%f,dev:%f",error_avg,error_dev);
            }
            else if(i==10)
            {
                result.insert("fitCurveErrorDevUR", error_dev); map.insert("fitCurveErrorDevUR", error_dev);
                qInfo("fitCurveErrorDev08UR:avg%f,dev:%f",error_avg,error_dev);
            }
            else if(i==11)
                {
                result.insert("fitCurveErrorDevLR", error_dev); map.insert("fitCurveErrorDevLR", error_dev);
                qInfo("fitCurveErrorDev08LR:avg%f,dev:%f",error_avg,error_dev);
            }
            else if(i==12)
                {
                result.insert("fitCurveErrorDevLL", error_dev); map.insert("fitCurveErrorDevLL", error_dev);
                qInfo("fitCurveErrorDev08LL:avg%f,dev:%f",error_avg,error_dev);
            }
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
    double peak_03  = 0;
    for (size_t i = 0; i < points_1.size(); i++) {
        qInfo("Layer 1: x: %f y: %f z: %f", points_1[i].x, points_1[i].y, points_1[i].z);
        layerDetected++;
        peak_03 += points_1[i].z;
    }
    peak_03 /= points_1.size();
    qInfo("Peak 0.3F : %f", peak_03);
    double peak_05  = 0;
    for (size_t i = 0; i < points_2.size(); i++) {
        qInfo("Layer 2: x: %f y: %f z: %f", points_2[i].x, points_2[i].y, points_2[i].z);
        layerDetected++;
        peak_05 += points_2[i].z;
    }
    peak_05 /= points_2.size();
    qInfo("Peak 0.5F : %f", peak_05);
    double peak_08  = 0;
    for (size_t i = 0; i < points_3.size(); i++) {
        qInfo("Layer 3: x: %f y: %f z: %f", points_3[i].x, points_3[i].y, points_3[i].z);
        layerDetected++;
        peak_08 += points_3[i].z;
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
    peak_08 /= points_3.size();
    qInfo("Peak 0.8F : %f", peak_05);
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
    map.insert("cczPeak", point_0.z);

    result.insert("zPeak_03", peak_03); map.insert("zPeak_03", peak_03);
    result.insert("zPeak_05", peak_05); map.insert("zPeak_05", peak_05);
    result.insert("zPeak_08", peak_08); map.insert("zPeak_08", peak_08);
    result.insert("zPeak_cc", point_0.z); map.insert("zPeak_cc", point_0.z);
    result.insert("maxPeakZ", maxPeakZ + start_pos); map.insert("maxPeakZ", maxPeakZ + start_pos);
    double all_coefficient = parameters.zpeakccCoefficient() +parameters.zpeak03Coefficient() +parameters.zpeak05Coefficient() +parameters.zpeak08Coefficient();

    if(parameters.enableZpeakCoefficient()&&all_coefficient>0)
    {
        double z_peak = (parameters.zpeakccCoefficient()*map["zPeak_cc"].toDouble()+
                parameters.zpeak03Coefficient()*map["zPeak_03"].toDouble()+
                parameters.zpeak05Coefficient()*map["zPeak_05"].toDouble()+
                parameters.zpeak08Coefficient()*map["zPeak_08"].toDouble())/all_coefficient;
        result.insert("zPeak", z_peak); map.insert("zPeak", z_peak);
    }
    else {
        if (parameters.PeakProfile() == 1) {
            result.insert("zPeak", peak_03); map.insert("zPeak", peak_03);
        } else if (parameters.PeakProfile() == 2) {
            result.insert("zPeak", peak_05); map.insert("zPeak", peak_05);
        } else if (parameters.PeakProfile() == 3) {
            result.insert("zPeak", peak_08); map.insert("zPeak", peak_08);
        } else {
            result.insert("zPeak", point_0.z); map.insert("zPeak", point_0.z);
        }
    }

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
    data->setZPeak(map["zPeak"].toDouble()*1000);
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

        data->addData(0, sorted_sfr_map[0][i].pz*1000, sorted_sfr_fit_map[0][i], sorted_sfr_map[0][i].sfr);
        if (points_1.size() > 0) {
            for (int j = 1; j < 5; ++j) {
                double avg_sfr = parameters.WeightList().at(4*j-4+0).toDouble()*sorted_sfr_map[j+4*display_layer][i].t_sfr + parameters.WeightList().at(4*j-4+1).toDouble()*sorted_sfr_map[j+4*display_layer][i].r_sfr
                        + parameters.WeightList().at(4*j-4+2).toDouble()*sorted_sfr_map[j+4*display_layer][i].b_sfr + parameters.WeightList().at(4*j-4+3).toDouble()*sorted_sfr_map[j+4*display_layer][i].l_sfr;
                data->addData(j,sorted_sfr_map[j+4*display_layer][i].pz*1000, sorted_sfr_fit_map[j+4*display_layer][i],avg_sfr);
            }
        }
    }
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
        map.insert("zPeak_1_UL", points_1[0].z);result.insert("zPeak_1_UL", points_1[0].z);
        map.insert("zPeak_1_LL", points_1[1].z);result.insert("zPeak_1_LL", points_1[1].z);
        map.insert("zPeak_1_LR", points_1[2].z);result.insert("zPeak_1_LR", points_1[2].z);
        map.insert("zPeak_1_UR", points_1[3].z);result.insert("zPeak_1_UR", points_1[3].z);
        map.insert("dev_1", dev_1);
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
        map.insert("zPeak_2_UL", points_2[0].z);result.insert("zPeak_2_UL", points_2[0].z);
        map.insert("zPeak_2_LL", points_2[1].z);result.insert("zPeak_2_LL", points_2[1].z);
        map.insert("zPeak_2_LR", points_2[2].z);result.insert("zPeak_2_LR", points_2[2].z);
        map.insert("zPeak_2_UR", points_2[3].z);result.insert("zPeak_2_UR", points_2[3].z);
        map.insert("dev_2", dev_2);
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
        map.insert("zPeak_3_UL", points_3[0].z);result.insert("zPeak_3_UL", points_3[0].z);
        map.insert("zPeak_3_LL", points_3[1].z);result.insert("zPeak_3_LL", points_3[1].z);
        map.insert("zPeak_3_LR", points_3[2].z);result.insert("zPeak_3_LR", points_3[2].z);
        map.insert("zPeak_3_UR", points_3[3].z);result.insert("zPeak_3_UR", points_3[3].z);
        map.insert("dev_3", dev_3);
    }
    map.insert("fov_slope", current_fov_slope);
    //emit pushDataToUnit(runningUnit, "SFR", map);
	emit postSfrDataToELK(runningUnit, map);
    data->plot(runningTestName);
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
    double sfr_dev_tol = params["SFR_DEV_TOL"].toDouble(100);
    double sfr_tol[4] = {0};
    sfr_tol[0] = params["CC_TOL"].toDouble(-1);
    sfr_tol[1] = params["03F_TOL"].toDouble(-1);
    sfr_tol[2] = params["05F_TOL"].toDouble(-1);
    sfr_tol[3] = params["08F_TOL"].toDouble(-1);

    cv::Mat input_img = cv::imread("C:\\Users\\emil\\Desktop\\aatest\\low.bmp");
    std::vector<AA_Helper::patternAttr> patterns = AA_Helper::AAA_Search_MTF_Pattern_Ex(input_img, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea(), -1);
    vector<double> sfr_l_v, sfr_r_v, sfr_t_v, sfr_b_v;
    cv::Rect roi; roi.width = 32; roi.height = 32;
    double rect_width = 0;
    std::vector<MTF_Pattern_Position> vec;
    double imageCenterX = input_img.cols/2;
    double imageCenterY = input_img.rows/2;
    double r1 = sqrt(imageCenterX*imageCenterX + imageCenterY*imageCenterY);
    for (uint i = 0; i < patterns.size(); i++) {
        qInfo("Pattern width : %f height: %f", patterns[i].width, patterns[i].height);
        cv::Mat cropped_l_img, cropped_r_img, cropped_t_img, cropped_b_img;
        rect_width = sqrt(patterns[i].area)/2;
        //Left ROI
        roi.x = patterns[i].center.x() - rect_width - roi.width/2;
        roi.y = patterns[i].center.y() - roi.width/2;
        input_img(roi).copyTo(cropped_l_img);
        //Right ROI
        roi.x = patterns[i].center.x() + rect_width - roi.width/2;
        roi.y = patterns[i].center.y() - roi.width/2;
        input_img(roi).copyTo(cropped_r_img);
        //Top ROI
        roi.x = patterns[i].center.x() - roi.width/2;
        roi.y = patterns[i].center.y() - rect_width - roi.width/2;
        input_img(roi).copyTo(cropped_t_img);
        //Bottom ROI
        roi.x = patterns[i].center.x() - roi.width/2;
        roi.y = patterns[i].center.y() + rect_width - roi.width/2;
        input_img(roi).copyTo(cropped_b_img);

        QString filename_t = "t_";
        filename_t.append(QString::number(i)).append(".bmp");
        cv::imwrite(filename_t.toStdString(), cropped_t_img);
        QString filename_b = "b_";
        filename_b.append(QString::number(i)).append(".bmp");;
        cv::imwrite(filename_b.toStdString(), cropped_b_img);
        QString filename_l = "l_";
        filename_l.append(QString::number(i)).append(".bmp");;
        cv::imwrite(filename_l.toStdString(), cropped_l_img);
        QString filename_r = "r_";
        filename_r.append(QString::number(i)).append(".bmp");;
        cv::imwrite(filename_r.toStdString(), cropped_r_img);

        double sfr_l = sfr::calculateSfrWithSingleRoi(cropped_l_img,1);
        double sfr_r = sfr::calculateSfrWithSingleRoi(cropped_r_img,1);
        double sfr_t = sfr::calculateSfrWithSingleRoi(cropped_t_img,1);
        double sfr_b = sfr::calculateSfrWithSingleRoi(cropped_b_img,1);
        sfr_l_v.push_back(sfr_l);
        sfr_r_v.push_back(sfr_r);
        sfr_t_v.push_back(sfr_t);
        sfr_b_v.push_back(sfr_b);
        double radius = sqrt(pow(patterns[i].center.x() - imageCenterX, 2) + pow(patterns[i].center.y() - imageCenterY, 2));
        double f = radius/r1;
        double avg_sfr = (sfr_t + sfr_r + sfr_b + sfr_l)/4;
        qInfo("x: %f y: %f sfr_l: %f sfr_r: %f sfr_t: %f sfr_b: %f",
              patterns[i].center.x(), patterns[i].center.y(), sfr_l, sfr_r, sfr_t, sfr_b);
        vec.emplace_back(patterns[i].center.x(), patterns[i].center.y(),
                         f, sfr_t, sfr_r, sfr_b, sfr_l, patterns[i].area, avg_sfr);

    }
    vector<int> layers = sfr::classifyLayers(vec);

    double display_factor = input_img.cols/CONSTANT_REFERENCE;
    QImage qImage = ImageGrabbingWorkerThread::cvMat2QImage(input_img);
    QPainter qPainter(&qImage);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setFont(QFont("Times",75*display_factor, QFont::Light));
    for (size_t i = 0; i < vec.size(); i++) {
        qInfo("Layer %d :  px: %f py: %f sfr: %f 1: %f 2: %f 3: %f 4: %f area: %f", ((i-1)/4) + 1,
              vec[i].x, vec[i].y, vec[i].avg_sfr, vec[i].t_sfr, vec[i].r_sfr, vec[i].b_sfr, vec[i].l_sfr, vec[i].area);
        qPainter.setPen(QPen(Qt::blue, 4.0));
        qPainter.drawText(vec[i].x - rect_width/2, vec[i].y - rect_width*2, QString::number(vec[i].t_sfr, 'g', 4));
        qPainter.drawText(vec[i].x + 150, vec[i].y,  QString::number(vec[i].r_sfr, 'g', 4));
        qPainter.drawText(vec[i].x - 50, vec[i].y+ 120,  QString::number(vec[i].b_sfr, 'g', 4));
        qPainter.drawText(vec[i].x - 250, vec[i].y,  QString::number(vec[i].l_sfr, 'g', 4));
    }
    qPainter.end();
    sfrImageReady(std::move(qImage));
    int max_layer = 0;
    map.insert("CC_T_SFR", round(vec[0].t_sfr*1000)/1000);
    map.insert("CC_R_SFR", round(vec[0].r_sfr*1000)/1000);
    map.insert("CC_B_SFR", round(vec[0].b_sfr*1000)/1000);
    map.insert("CC_L_SFR", round(vec[0].l_sfr*1000)/1000);
    map.insert("CC_SFR", round(((vec[0].t_sfr + vec[0].r_sfr + vec[0].b_sfr + vec[0].l_sfr)/4)*1000)/1000);
    map.insert("UL_T_SFR", round(vec[max_layer*4 + 1].t_sfr*1000)/1000);
    map.insert("UL_R_SFR", round(vec[max_layer*4 + 1].r_sfr*1000)/1000);
    map.insert("UL_B_SFR", round(vec[max_layer*4 + 1].b_sfr*1000)/1000);
    map.insert("UL_L_SFR", round(vec[max_layer*4 + 1].l_sfr*1000)/1000);
    map.insert("UL_SFR", round(vec[max_layer*4 + 1].avg_sfr*1000)/1000);
    map.insert("LL_T_SFR", round(vec[max_layer*4 + 2].t_sfr*1000)/1000);
    map.insert("LL_R_SFR", round(vec[max_layer*4 + 2].r_sfr*1000)/1000);
    map.insert("LL_B_SFR", round(vec[max_layer*4 + 2].b_sfr*1000)/1000);
    map.insert("LL_L_SFR", round(vec[max_layer*4 + 2].l_sfr*1000)/1000);
    map.insert("LL_SFR", round(vec[max_layer*4 + 2].avg_sfr*1000)/1000);
    map.insert("LR_T_SFR", round(vec[max_layer*4 + 3].t_sfr*1000)/1000);
    map.insert("LR_R_SFR", round(vec[max_layer*4 + 3].r_sfr*1000)/1000);
    map.insert("LR_B_SFR", round(vec[max_layer*4 + 3].b_sfr*1000)/1000);
    map.insert("LR_L_SFR", round(vec[max_layer*4 + 3].l_sfr*1000)/1000);
    map.insert("LR_SFR", round(vec[max_layer*4 + 3].avg_sfr*1000)/1000);
    map.insert("UR_T_SFR", round(vec[max_layer*4 + 4].t_sfr*1000)/1000);
    map.insert("UR_R_SFR", round(vec[max_layer*4 + 4].r_sfr*1000)/1000);
    map.insert("UR_B_SFR", round(vec[max_layer*4 + 4].b_sfr*1000)/1000);
    map.insert("UR_L_SFR", round(vec[max_layer*4 + 4].l_sfr*1000)/1000);
    map.insert("UR_SFR", round(vec[max_layer*4 + 4].avg_sfr*1000)/1000);

    emit pushDataToUnit(this->runningUnit, "MTF", map);
    return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
}

double AACoreNew::performMTFInThread( cv::Mat input, int freq )
{
    double sfr = sfr::calculateSfrWithSingleRoi(input, freq);
    return sfr;
}

ErrorCodeStruct AACoreNew::performMTFNew(QJsonValue params, bool write_log)
{
//    double cc_min_sfr = params["CC"].toDouble(-1);
//    double ul_min_sfr = params["UL"].toDouble(-1);
//    double ur_min_sfr = params["UR"].toDouble(-1);
//    double ll_min_sfr = params["LL"].toDouble(-1);
//    double lr_min_sfr = params["LR"].toDouble(-1);
    double sfr_dev_tol = params["SFR_DEV_TOL"].toDouble(100);
    double sfr_tol[8] = {0};
    sfr_tol[0] = params["CC_MIN"].toDouble(0);
    sfr_tol[1] = params["03F_MIN"].toDouble(0);
    sfr_tol[2] = params["05F_MIN"].toDouble(0);
    sfr_tol[3] = params["08F_MIN"].toDouble(0);
    sfr_tol[4] = params["CC_MAX"].toDouble(100);
    sfr_tol[5] = params["03F_MAX"].toDouble(100);
    sfr_tol[6] = params["05F_MAX"].toDouble(100);
    sfr_tol[7] = params["08F_MAX"].toDouble(100);
    QString error = "";
    QElapsedTimer timer;timer.start();
    QVariantMap map;
    bool grabRet = false;
    cv::Mat input_img = dk->DothinkeyGrabImageCV(0, grabRet);
    //cv::Mat input_img = cv::imread("C:\\Users\\emil\\Desktop\\mtf_test\\18-45-31-211.bmp");
    if (!grabRet) {
        qInfo("MTF Cannot grab image.");
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, ""};
    }
    double fov = calculateDFOV(input_img);
    std::vector<AA_Helper::patternAttr> patterns = AA_Helper::AAA_Search_MTF_Pattern_Ex(input_img, parameters.MaxIntensity(), parameters.MinArea(), parameters.MaxArea(), -1);
    vector<double> sfr_l_v, sfr_r_v, sfr_t_v, sfr_b_v;
    cv::Rect roi; roi.width = 32; roi.height = 32;
    double rect_width = 0;
    bool sfr_check = true;
    std::vector<MTF_Pattern_Position> vec;
    double imageCenterX = input_img.cols/2;
    double imageCenterY = input_img.rows/2;
    double r1 = sqrt(imageCenterX*imageCenterX + imageCenterY*imageCenterY);
    for (uint i = 0; i < patterns.size(); i++) {
        cv::Mat cropped_l_img, cropped_r_img, cropped_t_img, cropped_b_img;
        rect_width = sqrt(patterns[i].area)/2;
        //Left ROI
        roi.x = patterns[i].center.x() - rect_width - roi.width/2;
        roi.y = patterns[i].center.y() - roi.width/2;
        input_img(roi).copyTo(cropped_l_img);
        //Right ROI
        roi.x = patterns[i].center.x() + rect_width - roi.width/2;
        roi.y = patterns[i].center.y() - roi.width/2;
        input_img(roi).copyTo(cropped_r_img);
        //Top ROI
        roi.x = patterns[i].center.x() - roi.width/2;
        roi.y = patterns[i].center.y() - rect_width - roi.width/2;
        input_img(roi).copyTo(cropped_t_img);
        //Bottom ROI
        roi.x = patterns[i].center.x() - roi.width/2;
        roi.y = patterns[i].center.y() + rect_width - roi.width/2;
        input_img(roi).copyTo(cropped_b_img);

        QFuture<double> f1, f2, f3, f4;
        f1 = QtConcurrent::run(performMTFInThread, cropped_l_img, 1);
        f2 = QtConcurrent::run(performMTFInThread, cropped_r_img, 1);
        f3 = QtConcurrent::run(performMTFInThread, cropped_t_img, 1);
        f4 = QtConcurrent::run(performMTFInThread, cropped_b_img, 1);
        f1.waitForFinished();
        f2.waitForFinished();
        f3.waitForFinished();
        f4.waitForFinished();
        double sfr_l = f1.result();
        double sfr_r = f2.result();
        double sfr_t = f3.result();
        double sfr_b = f4.result();
        sfr_l_v.push_back(sfr_l);
        sfr_r_v.push_back(sfr_r);
        sfr_t_v.push_back(sfr_t);
        sfr_b_v.push_back(sfr_b);
        double radius = sqrt(pow(patterns[i].center.x() - imageCenterX, 2) + pow(patterns[i].center.y() - imageCenterY, 2));
        double f = radius/r1;
        double avg_sfr = (sfr_t + sfr_r + sfr_b + sfr_l)/4;
        qDebug("x: %f y: %f sfr_l: %f sfr_r: %f sfr_t: %f sfr_b: %f",
              patterns[i].center.x(), patterns[i].center.y(), sfr_l, sfr_r, sfr_t, sfr_b);
        vec.emplace_back(patterns[i].center.x(), patterns[i].center.y(),
                         f, sfr_t, sfr_r, sfr_b, sfr_l, patterns[i].area, avg_sfr);

    }
    vector<int> layers = sfr::classifyLayers(vec);
    double display_factor = input_img.cols/CONSTANT_REFERENCE;
    QImage qImage = ImageGrabbingWorkerThread::cvMat2QImage(input_img);
    QPainter qPainter(&qImage);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setFont(QFont("Times",75*display_factor, QFont::Light));
    int max_layer = 0;
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        if (vec.at(i).layer > max_layer) {
            max_layer = vec.at(i).layer - 1;
        }
    }
    for (size_t i = 0; i < vec.size(); i++) {
        qPainter.setPen(QPen(Qt::blue, 4.0));
        qPainter.drawText(vec[i].x - rect_width/2, vec[i].y - rect_width*2, QString::number(vec[i].t_sfr, 'g', 4));
        qPainter.drawText(vec[i].x + 150, vec[i].y,  QString::number(vec[i].r_sfr, 'g', 4));
        qPainter.drawText(vec[i].x - 50, vec[i].y+ 120,  QString::number(vec[i].b_sfr, 'g', 4));
        qPainter.drawText(vec[i].x - 250, vec[i].y,  QString::number(vec[i].l_sfr, 'g', 4));
    }
    qPainter.end();
    sfrImageReady(std::move(qImage));

    // Check 4 lines in CC if each SFR score is lower than min or larger than max
    if (vec[0].t_sfr < sfr_tol[0] || vec[0].r_sfr < sfr_tol[0] || vec[0].b_sfr < sfr_tol[0] || vec[0].l_sfr < sfr_tol[0]
             || vec[0].t_sfr > sfr_tol[4] || vec[0].r_sfr > sfr_tol[4] || vec[0].b_sfr > sfr_tol[4] || vec[0].l_sfr > sfr_tol[4])
    {
        error.append("CC fail.");
        sfr_check = false;
    }
    for(int i = 0; i <= max_layer; i++) {
        map.insert(QString("UL_T_SFR_").append(QString::number(i+1)), round(vec[i*4 + 1].t_sfr*1000)/1000);
        map.insert(QString("UL_R_SFR_").append(QString::number(i+1)), round(vec[i*4 + 1].r_sfr*1000)/1000);
        map.insert(QString("UL_B_SFR_").append(QString::number(i+1)), round(vec[i*4 + 1].b_sfr*1000)/1000);
        map.insert(QString("UL_L_SFR_").append(QString::number(i+1)), round(vec[i*4 + 1].l_sfr*1000)/1000);
        map.insert(QString("UL_SFR_").append(QString::number(i+1)), round(vec[i*4 + 1].avg_sfr*1000)/1000);
        map.insert(QString("LL_T_SFR_").append(QString::number(i+1)), round(vec[i*4 + 2].t_sfr*1000)/1000);
        map.insert(QString("LL_R_SFR_").append(QString::number(i+1)), round(vec[i*4 + 2].r_sfr*1000)/1000);
        map.insert(QString("LL_B_SFR_").append(QString::number(i+1)), round(vec[i*4 + 2].b_sfr*1000)/1000);
        map.insert(QString("LL_L_SFR_").append(QString::number(i+1)), round(vec[i*4 + 2].l_sfr*1000)/1000);
        map.insert(QString("LL_SFR_").append(QString::number(i+1)), round(vec[i*4 + 2].avg_sfr*1000)/1000);
        map.insert(QString("LR_T_SFR_").append(QString::number(i+1)), round(vec[i*4 + 3].t_sfr*1000)/1000);
        map.insert(QString("LR_R_SFR_").append(QString::number(i+1)), round(vec[i*4 + 3].r_sfr*1000)/1000);
        map.insert(QString("LR_B_SFR_").append(QString::number(i+1)), round(vec[i*4 + 3].b_sfr*1000)/1000);
        map.insert(QString("LR_L_SFR_").append(QString::number(i+1)), round(vec[i*4 + 3].l_sfr*1000)/1000);
        map.insert(QString("LR_SFR_").append(QString::number(i+1)), round(vec[i*4 + 3].avg_sfr*1000)/1000);
        map.insert(QString("UR_T_SFR_").append(QString::number(i+1)), round(vec[i*4 + 4].t_sfr*1000)/1000);
        map.insert(QString("UR_R_SFR_").append(QString::number(i+1)), round(vec[i*4 + 4].r_sfr*1000)/1000);
        map.insert(QString("UR_B_SFR_").append(QString::number(i+1)), round(vec[i*4 + 4].b_sfr*1000)/1000);
        map.insert(QString("UR_L_SFR_").append(QString::number(i+1)), round(vec[i*4 + 4].l_sfr*1000)/1000);
        map.insert(QString("UR_SFR_").append(QString::number(i+1)), round(vec[i*4 + 4].avg_sfr*1000)/1000);
        //Check each 4 ROI in 03F, 05F, 08F if each 4 SFR score is lower than min or larger than max
        if (vec[i*4+1].t_sfr < sfr_tol[i+1] || vec[i*4+1].r_sfr < sfr_tol[i+1] || vec[i*4+1].b_sfr < sfr_tol[i+1] || vec[i*4+1].l_sfr < sfr_tol[i+1]
                || vec[i*4+2].t_sfr < sfr_tol[i+1] || vec[i*4+2].r_sfr < sfr_tol[i+1] || vec[i*4+2].b_sfr < sfr_tol[i+1] || vec[i*4+2].l_sfr < sfr_tol[i+1]
                || vec[i*4+3].t_sfr < sfr_tol[i+1] || vec[i*4+3].r_sfr < sfr_tol[i+1] || vec[i*4+3].b_sfr < sfr_tol[i+1] || vec[i*4+3].l_sfr < sfr_tol[i+1]
                || vec[i*4+4].t_sfr < sfr_tol[i+1] || vec[i*4+4].r_sfr < sfr_tol[i+1] || vec[i*4+4].b_sfr < sfr_tol[i+1] || vec[i*4+4].l_sfr < sfr_tol[i+1]
                || vec[i*4+1].t_sfr > sfr_tol[i+5] || vec[i*4+1].r_sfr > sfr_tol[i+5] || vec[i*4+1].b_sfr > sfr_tol[i+5] || vec[i*4+1].l_sfr > sfr_tol[i+5]
                || vec[i*4+2].t_sfr > sfr_tol[i+5] || vec[i*4+2].r_sfr > sfr_tol[i+5] || vec[i*4+2].b_sfr > sfr_tol[i+5] || vec[i*4+2].l_sfr > sfr_tol[i+5]
                || vec[i*4+3].t_sfr > sfr_tol[i+5] || vec[i*4+3].r_sfr > sfr_tol[i+5] || vec[i*4+3].b_sfr > sfr_tol[i+5] || vec[i*4+3].l_sfr > sfr_tol[i+5]
                || vec[i*4+4].t_sfr > sfr_tol[i+5] || vec[i*4+4].r_sfr > sfr_tol[i+5] || vec[i*4+4].b_sfr > sfr_tol[i+5] || vec[i*4+4].l_sfr > sfr_tol[i+5])
        {
            if (i == 0)
            {
                qInfo("03F check SFR fail with %f", sfr_tol[1]);
                error.append("03F fail.");
            }
            else if (i == 1)
            {
                qInfo("05F check SFR fail with %f", sfr_tol[2]);
                error.append("05F fail.");
            }
            else if (i == 2)
            {
                qInfo("08F check SFR fail with %f", sfr_tol[3]);
                error.append("08F fail.");
            }
            sfr_check = false;
        }
        qInfo("UL %f %f %f %f", vec[i*4 + 1].t_sfr, vec[i*4 + 1].r_sfr, vec[i*4 + 1].b_sfr, vec[i*4 + 1].l_sfr);
        qInfo("UR %f %f %f %f", vec[i*4 + 2].t_sfr, vec[i*4 + 2].r_sfr, vec[i*4 + 2].b_sfr, vec[i*4 + 2].l_sfr);
        qInfo("LR %f %f %f %f", vec[i*4 + 3].t_sfr, vec[i*4 + 3].r_sfr, vec[i*4 + 3].b_sfr, vec[i*4 + 3].l_sfr);
        qInfo("LL %f %f %f %f", vec[i*4 + 4].t_sfr, vec[i*4 + 4].r_sfr, vec[i*4 + 4].b_sfr, vec[i*4 + 4].l_sfr);
        qInfo("MIN %f", sfr_tol[i+1]);
        qInfo("MAX %f", sfr_tol[i+5]);
    }
    double ul_08f_sfr_dev = getSFRDev_mm(4,vec[max_layer*4 + 1].t_sfr,vec[max_layer*4 + 1].r_sfr,vec[max_layer*4 + 1].b_sfr,vec[max_layer*4 + 1].l_sfr);
    double ll_08f_sfr_dev = getSFRDev_mm(4,vec[max_layer*4 + 2].t_sfr,vec[max_layer*4 + 2].r_sfr,vec[max_layer*4 + 2].b_sfr,vec[max_layer*4 + 2].l_sfr);
    double lr_08f_sfr_dev = getSFRDev_mm(4,vec[max_layer*4 + 3].t_sfr,vec[max_layer*4 + 3].r_sfr,vec[max_layer*4 + 3].b_sfr,vec[max_layer*4 + 3].l_sfr);
    double ur_08f_sfr_dev = getSFRDev_mm(4,vec[max_layer*4 + 4].t_sfr,vec[max_layer*4 + 4].r_sfr,vec[max_layer*4 + 4].b_sfr,vec[max_layer*4 + 4].l_sfr);
    qInfo("ul_08f_sfr_dev : %f ll_08f_sfr_dev : %f lr_08f_sfr_dev : %f ur_08f_sfr_dev : %f", ul_08f_sfr_dev,ll_08f_sfr_dev,lr_08f_sfr_dev,ur_08f_sfr_dev);
    if (ul_08f_sfr_dev >= sfr_dev_tol || ll_08f_sfr_dev >= sfr_dev_tol || lr_08f_sfr_dev >= sfr_dev_tol || ur_08f_sfr_dev >= sfr_dev_tol) {
        qInfo("08f_sfr_corner_dev cannot pass");
        sfr_check = false;
        error.append("08F SFR corner dev fail.");
    }
    map.insert("SensorID", dk->readSensorID());
    map.insert("FOV",round(fov*1000)/1000);
    map.insert("zPeak",round(sut->carrier->GetFeedBackPos().Z*1000)/1000);
    map.insert("CC_T_SFR", round(vec[0].t_sfr*1000)/1000);
    map.insert("CC_R_SFR", round(vec[0].r_sfr*1000)/1000);
    map.insert("CC_B_SFR", round(vec[0].b_sfr*1000)/1000);
    map.insert("CC_L_SFR", round(vec[0].l_sfr*1000)/1000);
    map.insert("CC_SFR", round(((vec[0].t_sfr + vec[0].r_sfr + vec[0].b_sfr + vec[0].l_sfr)/4)*1000)/1000);
    map.insert("UL_T_SFR", round(vec[max_layer*4 + 1].t_sfr*1000)/1000);
    map.insert("UL_R_SFR", round(vec[max_layer*4 + 1].r_sfr*1000)/1000);
    map.insert("UL_B_SFR", round(vec[max_layer*4 + 1].b_sfr*1000)/1000);
    map.insert("UL_L_SFR", round(vec[max_layer*4 + 1].l_sfr*1000)/1000);
    map.insert("UL_SFR", round(vec[max_layer*4 + 1].avg_sfr*1000)/1000);
    map.insert("LL_T_SFR", round(vec[max_layer*4 + 2].t_sfr*1000)/1000);
    map.insert("LL_R_SFR", round(vec[max_layer*4 + 2].r_sfr*1000)/1000);
    map.insert("LL_B_SFR", round(vec[max_layer*4 + 2].b_sfr*1000)/1000);
    map.insert("LL_L_SFR", round(vec[max_layer*4 + 2].l_sfr*1000)/1000);
    map.insert("LL_SFR", round(vec[max_layer*4 + 2].avg_sfr*1000)/1000);
    map.insert("LR_T_SFR", round(vec[max_layer*4 + 3].t_sfr*1000)/1000);
    map.insert("LR_R_SFR", round(vec[max_layer*4 + 3].r_sfr*1000)/1000);
    map.insert("LR_B_SFR", round(vec[max_layer*4 + 3].b_sfr*1000)/1000);
    map.insert("LR_L_SFR", round(vec[max_layer*4 + 3].l_sfr*1000)/1000);
    map.insert("LR_SFR", round(vec[max_layer*4 + 3].avg_sfr*1000)/1000);
    map.insert("UR_T_SFR", round(vec[max_layer*4 + 4].t_sfr*1000)/1000);
    map.insert("UR_R_SFR", round(vec[max_layer*4 + 4].r_sfr*1000)/1000);
    map.insert("UR_B_SFR", round(vec[max_layer*4 + 4].b_sfr*1000)/1000);
    map.insert("UR_L_SFR", round(vec[max_layer*4 + 4].l_sfr*1000)/1000);
    map.insert("UR_SFR", round(vec[max_layer*4 + 4].avg_sfr*1000)/1000);
    map.insert("OC_OFFSET_X_IN_PIXEL", round(mtf_oc_x*1000)/1000);
    map.insert("OC_OFFSET_Y_IN_PIXEL", round(mtf_oc_y*1000)/1000);
//    map.insert("SFR_DEV",max_sfr_deviation);
    map.insert("UL_08F_SFR_DEV",round(ul_08f_sfr_dev*1000)/1000);
    map.insert("LL_08F_SFR_DEV",round(ll_08f_sfr_dev*1000)/1000);
    map.insert("LR_08F_SFR_DEV",round(lr_08f_sfr_dev*1000)/1000);
    map.insert("UR_08F_SFR_DEV",round(ur_08f_sfr_dev*1000)/1000);
    map.insert("timeElapsed", timer.elapsed());
    qDebug("Time Elapsed: %d", timer.elapsed());
    if (write_log) {
        this->loopTestResult.append(QString::number(vec[0].avg_sfr))
                            .append(",")
                            .append(QString::number(vec[max_layer*4 + 1].avg_sfr))
                            .append(",")
                            .append(QString::number(vec[max_layer*4 + 4].avg_sfr))
                            .append(",")
                            .append(QString::number(vec[max_layer*4 + 2].avg_sfr))
                            .append(",")
                            .append(QString::number(vec[max_layer*4 + 3].avg_sfr))
                            .append(",\n");
        this->mtf_log.incrementData(vec[0].avg_sfr,
                                    vec[max_layer*4 + 1].avg_sfr,
                                    vec[max_layer*4 + 4].avg_sfr,
                                    vec[max_layer*4 + 2].avg_sfr,
                                    vec[max_layer*4 + 3].avg_sfr);
    }
    if (sfr_check) {
       map.insert("result", "Pass");
       emit pushDataToUnit(runningUnit, "MTF", map);
       return ErrorCodeStruct{ErrorCode::OK, ""};
    } else {
       map.insert("result", error);
       emit pushDataToUnit(runningUnit, "MTF", map);
       LogicNg(current_mtf_ng_time);
       return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, error};
    }
}

ErrorCodeStruct AACoreNew::performMTF(QJsonValue params)
{
    int resize_factor = 2;
    double cc_min_sfr = params["CC"].toDouble(-1);
    double ul_min_sfr = params["UL"].toDouble(-1);
    double ur_min_sfr = params["UR"].toDouble(-1);
    double ll_min_sfr = params["LL"].toDouble(-1);
    double lr_min_sfr = params["LR"].toDouble(-1);
    double sfr_dev_tol = params["SFR_DEV_TOL"].toDouble(100);
    double sfr_tol[4] = {0};
    sfr_tol[0] = params["CC_TOL"].toDouble(-1);
    sfr_tol[1] = params["03F_TOL"].toDouble(-1);
    sfr_tol[2] = params["05F_TOL"].toDouble(-1);
    sfr_tol[3] = params["08F_TOL"].toDouble(-1);
    QString error = "";
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
    cv::Size size(img.cols/resize_factor, img.rows/resize_factor);
    qint64 start_time = timer.elapsed();
    cv::resize(img, dst, size);
    qInfo("FOV: %f img resize: %d %d time elapsed: %d", fov, dst.cols, dst.rows, timer.elapsed() - start_time);
    start_time = timer.elapsed();
    emit sfrWorkerController->calculate(0, 0, dst, true, resize_factor);
    int timeout=1000;
    while(this->clustered_sfr_map.size() != 1 && timeout >0) {
        Sleep(10);
        timeout--;
    }
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
    double ul_08f_sfr_dev = getSFRDev_mm(4,sv[max_layer*4 + 1].t_sfr,sv[max_layer*4 + 1].r_sfr,sv[max_layer*4 + 1].b_sfr,sv[max_layer*4 + 1].l_sfr);
    double ll_08f_sfr_dev = getSFRDev_mm(4,sv[max_layer*4 + 2].t_sfr,sv[max_layer*4 + 2].r_sfr,sv[max_layer*4 + 2].b_sfr,sv[max_layer*4 + 2].l_sfr);
    double lr_08f_sfr_dev = getSFRDev_mm(4,sv[max_layer*4 + 3].t_sfr,sv[max_layer*4 + 3].r_sfr,sv[max_layer*4 + 3].b_sfr,sv[max_layer*4 + 3].l_sfr);
    double ur_08f_sfr_dev = getSFRDev_mm(4,sv[max_layer*4 + 4].t_sfr,sv[max_layer*4 + 4].r_sfr,sv[max_layer*4 + 4].b_sfr,sv[max_layer*4 + 4].l_sfr);
    qInfo("ul_08f_sfr_dev : %f ll_08f_sfr_dev : %f lr_08f_sfr_dev : %f ur_08f_sfr_dev : %f", ul_08f_sfr_dev,ll_08f_sfr_dev,lr_08f_sfr_dev,ur_08f_sfr_dev);
    double max_sfr_deviation = fabs(sfr_check_list[0] - sfr_check_list[sfr_check_list.size()-1]);
    mtf_oc_x = sv[0].px - dst.cols/2; mtf_oc_y = sv[0].py - dst.rows/2;
    qInfo("Max sfr deviation : %f", max_sfr_deviation);
    if (ul_08f_sfr_dev >= sfr_dev_tol || ll_08f_sfr_dev >= sfr_dev_tol || lr_08f_sfr_dev >= sfr_dev_tol || ur_08f_sfr_dev >= sfr_dev_tol) {
        qInfo("08f_sfr_corner_dev cannot pass");
        sfr_check = false;
        error.append("08F SFR corner dev fail.");
    }
    if (true) {
        double display_factor = img.cols/CONSTANT_REFERENCE;
        int roi_width = sqrt(sv[0].area)*this->parameters.ROIRatio();
        QImage qImage = ImageGrabbingWorkerThread::cvMat2QImage(dst);
        QPainter qPainter(&qImage);
        qPainter.setBrush(Qt::NoBrush);
        qPainter.setFont(QFont("Times",40*display_factor, QFont::Light));
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
    map.insert("SensorID", dk->readSensorID());
    map.insert("FOV",fov);
    map.insert("zPeak",sut->carrier->GetFeedBackPos().Z);
    map.insert("CC_T_SFR", sv[0].t_sfr);
    map.insert("CC_R_SFR", sv[0].r_sfr);
    map.insert("CC_B_SFR", sv[0].b_sfr);
    map.insert("CC_L_SFR", sv[0].l_sfr);
    map.insert("CC_SFR", (sv[0].t_sfr + sv[0].r_sfr + sv[0].b_sfr + sv[0].l_sfr)/4);
    // Check 4 lines in CC if each SFR score is lower than tolerance
    if (sv[0].t_sfr < sfr_tol[0] || sv[0].r_sfr < sfr_tol[0] || sv[0].b_sfr < sfr_tol[0] || sv[0].l_sfr < sfr_tol[0])
    {
        error.append("CC fail.");
        sfr_check = false;
    }
    for(unsigned i = 0; i <= max_layer; i++) {
        map.insert(QString("UL_T_SFR_").append(QString::number(i+1)), sv[i*4 + 1].t_sfr);
        map.insert(QString("UL_R_SFR_").append(QString::number(i+1)), sv[i*4 + 1].r_sfr);
        map.insert(QString("UL_B_SFR_").append(QString::number(i+1)), sv[i*4 + 1].b_sfr);
        map.insert(QString("UL_L_SFR_").append(QString::number(i+1)), sv[i*4 + 1].l_sfr);
        map.insert(QString("UL_SFR_").append(QString::number(i+1)), (sv[i*4 + 1].t_sfr + sv[i*4 + 1].r_sfr + sv[i*4 + 1].b_sfr + sv[i*4 + 1].l_sfr)/4);
        map.insert(QString("LL_T_SFR_").append(QString::number(i+1)), sv[i*4 + 2].t_sfr);
        map.insert(QString("LL_R_SFR_").append(QString::number(i+1)), sv[i*4 + 2].r_sfr);
        map.insert(QString("LL_B_SFR_").append(QString::number(i+1)), sv[i*4 + 2].b_sfr);
        map.insert(QString("LL_L_SFR_").append(QString::number(i+1)), sv[i*4 + 2].l_sfr);
        map.insert(QString("LL_SFR_").append(QString::number(i+1)), (sv[i*4 + 2].t_sfr + sv[i*4 + 2].r_sfr + sv[i*4 + 2].b_sfr + sv[i*4 + 2].l_sfr)/4);
        map.insert(QString("LR_T_SFR_").append(QString::number(i+1)), sv[i*4 + 3].t_sfr);
        map.insert(QString("LR_R_SFR_").append(QString::number(i+1)), sv[i*4 + 3].r_sfr);
        map.insert(QString("LR_B_SFR_").append(QString::number(i+1)), sv[i*4 + 3].b_sfr);
        map.insert(QString("LR_L_SFR_").append(QString::number(i+1)), sv[i*4 + 3].l_sfr);
        map.insert(QString("LR_SFR_").append(QString::number(i+1)), (sv[i*4 + 3].t_sfr + sv[i*4 + 3].r_sfr + sv[i*4 + 3].b_sfr + sv[i*4 + 3].l_sfr)/4);
        map.insert(QString("UR_T_SFR_").append(QString::number(i+1)), sv[i*4 + 4].t_sfr);
        map.insert(QString("UR_R_SFR_").append(QString::number(i+1)), sv[i*4 + 4].r_sfr);
        map.insert(QString("UR_B_SFR_").append(QString::number(i+1)), sv[i*4 + 4].b_sfr);
        map.insert(QString("UR_L_SFR_").append(QString::number(i+1)), sv[i*4 + 4].l_sfr);
        map.insert(QString("UR_SFR_").append(QString::number(i+1)), (sv[i*4 + 4].t_sfr + sv[i*4 + 4].r_sfr + sv[i*4 + 4].b_sfr + sv[i*4 + 4].l_sfr)/4);
        //Check each 4 ROI in 03F, 05F, 08F if each 4 SFR score is lower than tolerance
        if (sv[i*4+1].t_sfr < sfr_tol[i+1] || sv[i*4+1].r_sfr < sfr_tol[i+1] || sv[i*4+1].b_sfr < sfr_tol[i+1] || sv[i*4+1].l_sfr < sfr_tol[i+1]
                || sv[i*4+2].t_sfr < sfr_tol[i+1] || sv[i*4+2].r_sfr < sfr_tol[i+1] || sv[i*4+2].b_sfr < sfr_tol[i+1] || sv[i*4+2].l_sfr < sfr_tol[i+1]
                || sv[i*4+3].t_sfr < sfr_tol[i+1] || sv[i*4+3].r_sfr < sfr_tol[i+1] || sv[i*4+3].b_sfr < sfr_tol[i+1] || sv[i*4+3].l_sfr < sfr_tol[i+1]
                || sv[i*4+4].t_sfr < sfr_tol[i+1] || sv[i*4+4].r_sfr < sfr_tol[i+1] || sv[i*4+4].b_sfr < sfr_tol[i+1] || sv[i*4+4].l_sfr < sfr_tol[i+1])
        {
            if (i == 0)
            {
                qInfo("03F check SFR fail with %f", sfr_tol[1]);
                error.append("03F fail.");
            }
            else if (i == 1)
            {
                qInfo("05F check SFR fail with %f", sfr_tol[2]);
                error.append("05F fail.");
            }
            else if (i == 2)
            {
                qInfo("08F check SFR fail with %f", sfr_tol[3]);
                error.append("08F fail.");
            }
            sfr_check = false;
        }
    }
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
    map.insert("OC_OFFSET_X_IN_PIXEL", round(mtf_oc_x*1000)/1000);
    map.insert("OC_OFFSET_Y_IN_PIXEL", round(mtf_oc_y*1000)/1000);
    map.insert("SFR_DEV",max_sfr_deviation);
    map.insert("UL_08F_SFR_DEV",ul_08f_sfr_dev);
    map.insert("LL_08F_SFR_DEV",ll_08f_sfr_dev);
    map.insert("LR_08F_SFR_DEV",lr_08f_sfr_dev);
    map.insert("UR_08F_SFR_DEV",ur_08f_sfr_dev);
    map.insert("timeElapsed", timer.elapsed());

    if (sfr_check) {
       map.insert("result", "Pass");
       emit pushDataToUnit(runningUnit, "MTF", map);
       return ErrorCodeStruct{ErrorCode::OK, ""};
    } else {
       map.insert("result", error);
       emit pushDataToUnit(runningUnit, "MTF", map);
       LogicNg(current_mtf_ng_time);
       return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, error};
    }
}

ErrorCodeStruct AACoreNew::performUV(QJsonValue params)
{
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    int uv_time = params["time_in_ms"].toInt(3000);
    bool enable_OTP = params["enable_OTP"].toInt(0);
    aa_head->openUVTillTime(uv_time);
    bool result = true;
    if (enable_OTP)
    {
        // OTP
        result = dk->DothinkeyOTP(serverMode);
        if (result != true)
        {
            result = dk->DothinkeyOTP(serverMode);  //retry OTP
        }
    }
    aa_head->waitUVFinish();
    map.insert("timeElapsed", timer.elapsed());
    if(result)
    {
        map.insert("result", "ok");
        emit pushDataToUnit(this->runningUnit, "UV", map);
        return ErrorCodeStruct{ErrorCode::OK, ""};
    }
    else
    {
        NgProduct();
        map.insert("result", "otp fail");
        emit pushDataToUnit(this->runningUnit, "UV", map);
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "otp fail"};
    }
}

ErrorCodeStruct AACoreNew::performReject()
{
    QVariantMap map;
    imageThread->stop();
    Sleep(100);
    imageThread->exit();
    dk->DothinkeyClose();
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
    parameters.setCurrentTask(parameters.currentTask() + 1);
    states.setCurrentTask(states.currentTask() + 1);
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
   qInfo("Perform GRR : %d %d %d %d", change_lens, change_sensor, repeat_time, change_time);
   if(!change_lens) {
       SetLens();
   }
   if(!change_sensor) {
       SetSensor();
   }
   if(grr_repeat_time >= repeat_time)
   {
       grr_repeat_time = 0;
       grr_change_time++;
       if(change_time>0&&grr_change_time >= change_time)
       {
           grr_change_time = 0;
           sendAlarmMessage(OK_OPERATION,u8"GRR finish.",ErrorLevel::ErrorMustStop);
       }
       if(change_lens&&HasLens())
           NgLens();
       if(change_sensor&&HasSensorOrProduct())
           SetProduct();
   }
   if(!change_lens) {
       SetLens();
   }
   if(!change_sensor) {
       SetSensor();
   }
   grr_repeat_time++;
   return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performOTP(QJsonValue params)
{
    qInfo("Performing OTP");
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performYLevelTest(QJsonValue params)
{
    int enable_plot = params["enable_plot"].toInt();
    //cv::Mat inputImage = cv::imread("1/blackScreen.bmp");
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
        if ((max_i - min_i) < parameters.minIntensityDiff()) {
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
    bool enable_motion = params["enable_motion"].toInt();
    bool fast_mode = params["fast_mode"].toInt();
    int finish_delay = params["delay_in_ms"].toInt();
    ErrorCodeStruct ret = { ErrorCode::OK, "" };
    QVariantMap map;
    QElapsedTimer timer;
    timer.start();
    bool grabRet;
    cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
    if (!grabRet) {
        qInfo("OC Cannot grab image.");
        map["Result"] = "OC Cannot grab image.";
        emit pushDataToUnit(runningUnit, "OC", map);
        NgSensor();
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "OC Cannot Grab Image"};
    }
    oc_fov = calculateDFOV(img);
    oc_z = aa_head->GetFeedBack().Z;
    qInfo("DFOV in OC: %f", oc_fov);
    map.insert("DFOV", oc_fov);
    if (!blackScreenCheck(img)) {
        NgSensor();
        map["Result"] = "OC Detect black screen";
        emit pushDataToUnit(runningUnit, "OC", map);
        return ErrorCodeStruct{ErrorCode::GENERIC_ERROR, "OC Detect BlackScreen"};
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
            NgSensor();
            map.insert("result", "OC Cannot find enough pattern");
            map.insert("timeElapsed", timer.elapsed());
            emit pushDataToUnit(this->runningUnit, "OC", map);
            return ErrorCodeStruct { ErrorCode::GENERIC_ERROR, "Cannot find enough pattern" };
        }
        offsetX = vector[ccIndex].center.x() - (vector[ccIndex].width/2);
        offsetY = vector[ccIndex].center.y() - (vector[ccIndex].height/2);
        qInfo("OC OffsetX: %f %f", offsetX, offsetY);
        map.insert("OC_OFFSET_X_IN_PIXEL", round(offsetX*1000)/1000);
        map.insert("OC_OFFSET_Y_IN_PIXEL", round(offsetY*1000)/1000);
    } else {
        QImage outImage; QPointF center;
        if (!AA_Helper::calculateOC(img, center, outImage))
        {
            NgSensor();
            map.insert("result", "OC Cannot calculate OC");
            map.insert("timeElapsed", timer.elapsed());
            emit pushDataToUnit(this->runningUnit, "OC", map);
            return ErrorCodeStruct { ErrorCode::GENERIC_ERROR, "Cannot calculate OC"};
        }
        ocImageProvider_1->setImage(outImage);
        emit callQmlRefeshImg(1);
        offsetX = center.x() - img.cols/2;
        offsetY = center.y() - img.rows/2;
    }
    if (enable_motion)
    {
        QPointF x_ratio = chartCalibration->getOneXPxielDistance();
        qInfo("x pixel Ratio: %f %f ", x_ratio.x(), x_ratio.y());
        QPointF y_ratio = chartCalibration->getOneYPxielDistance();
        qInfo("y pixel Ratio: %f %f ", y_ratio.x(), y_ratio.y());
        double stepX = offsetX * x_ratio.x() + offsetY * y_ratio.x();
        double stepY = offsetX * x_ratio.y() + offsetY * y_ratio.y();
        map.insert("OC_OFFSET_X_IN_UM", round(stepX*1000*1000)/1000);
        map.insert("OC_OFFSET_Y_IN_UM", round(stepY*1000*1000)/1000);
        qInfo("xy step: %f %f ", stepX, stepY);
        if(abs(stepX)>0.5||abs(stepY)>0.5)
        {
            NgSensor();
            qInfo("OC result too big (x:%f,y:%f) pixel：(%f,%f) cmosPixelToMM (x:)%f,%f) ",stepY,stepY,offsetX,offsetY,x_ratio.x(),x_ratio.y());
            map.insert("result", "OC result too big");
            map.insert("timeElapsed", timer.elapsed());
            emit pushDataToUnit(this->runningUnit, "OC", map);
            return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "OC result too big" };
        }
        this->sut->stepMove_XY_Sync(-stepX, -stepY);
    }
    if(finish_delay>0)
        Sleep(finish_delay);
    map.insert("result", "OK");
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "OC", map);
    qInfo("Finish OC");
    return ret;
}

ErrorCodeStruct AACoreNew::performVCMInit(QJsonValue params)
{
    QElapsedTimer timer;timer.start();
    QVariantMap map;
    double target_position = params["target_position"].toDouble();
    int delay = params["delay_in_ms"].toInt();
    map.insert("target_position", target_position);
    if (!this->vcmCmdServer.isOpen()) {
        QStringList arguments;
        arguments << "/c" << ".\\config\\i2c_test\\i2c_test.exe";
        vcmCmdServer.setWorkingDirectory(QDir::currentPath());
        vcmCmdServer.start("cmd.exe", arguments);
        vcmCmdServer.waitForStarted();
        QThread::msleep(1000);
    }
    CClient client;
    QVariantMap json;
    if(target_position < 0)
    {
        json["cmd"] = "init";
        json["delay"] = delay;
    }
    else
    {
        json["cmd"] = "move";
        json["pos"] = target_position;
        json["delay"] = delay;
    }

    QString jsonString = QString(QJsonDocument(QJsonObject::fromVariantMap(json)).toJson());
    qInfo(jsonString.toStdString().c_str());
    if (!client.ConnectToServer("localserver-test")) {   //If the i2c client cannot connect the i2c server
        QString respond = "i2c server connect fail";
        NgLens();
        map.insert("result", respond);
        emit pushDataToUnit(runningUnit, "VCMInit", map);
        return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, respond};
    }
    client.sendMessage(jsonString);

    QString respond = client.waitRespond();
    if(respond.contains("success"))
        qInfo(respond.toStdString().c_str());
//    else
//    {
//       int alarm_id = sendAlarmMessage(CONTINUE_REJECT_OPERATION,respond);
//       QString opertion = waitMessageReturn(is_run,alarm_id);
//       if(opertion == REJECT_OPERATION)
//       {
//           NgLens();
//           map.insert("result", respond);
//           emit pushDataToUnit(runningUnit, "VCMInit", map);
//           return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, respond};
//       }
//    }
//    if(delay>0)
//        QThread::msleep(delay);
    map.insert("timeElapsed", timer.elapsed());
    map.insert("result", "OK");
    emit pushDataToUnit(runningUnit, "VCMInit", map);
    return ErrorCodeStruct {ErrorCode::OK, ""};
}
ErrorCodeStruct AACoreNew::performInitSensor(int finish_delay,bool check_map)
{
    if (dk->DothinkeyIsGrabbing()) {
        qInfo("Dothinkey is grabbing image already, init sensor pass");
        return ErrorCodeStruct {ErrorCode::OK, ""};
    }
    if(!has_sensor) return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "has no sensor"};
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

    sensorID = dk->readSensorID();
    qInfo("performInitSensor sensor ID: %s", sensorID.toStdString().c_str());
    map.insert("sensorID", sensorID);
    if (!imageThread->isRunning())
        imageThread->start();

    if(check_map)
    {
        bool grabRet = true;
        cv::Mat img = dk->DothinkeyGrabImageCV(0, grabRet);
        if(!grabRet)
        {
            NgSensor();
            map.insert("result", "InitSensor Grab Image fail");
            emit pushDataToUnit(runningUnit, "InitSensor", map);
            return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "InitSensor Grab Image fail"};
        }
        if(!blackScreenCheck(img))
        {
            NgSensor();
            map.insert("result", "InitSensor Detect black screen");
            emit pushDataToUnit(runningUnit, "InitSensor", map);
            return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "InitSensor Detect black screen"};
        }
    }
    if(finish_delay>0)
        Sleep(finish_delay);
    map.insert("timeElapsed", timer.elapsed());
    map.insert("result", "OK");
    emit pushDataToUnit(runningUnit, "InitSensor", map);
    return ErrorCodeStruct {ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performPRToBond(int finish_delay)
{
    if((!has_sensor)||(!has_lens)){ return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA no sensor or no lens"};}
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
    if (!sut->moveZToSafety()) {return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA cannot move to SUT_Z safety position"};};
    if (!this->aa_head->moveToSZ_XYSC_Z_Sync(x,y,z,theta)) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA cannot move to PRToBond Position"};}
    if(finish_delay>0)
        Sleep(finish_delay);
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(runningUnit, "PrToBond", map);
    return ErrorCodeStruct {ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performLoadMaterial(int finish_delay)
{
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    ErrorCodeStruct result = ErrorCodeStruct {ErrorCode::OK, ""};

    if(states.stationTask() > 0)
    {
        if((!states.finishStationTask())&&(states.currentTask() >= states.stationTask()))
        {
            states.setFinishSensorTask(true);
            sendMessageToModule("SensorLoaderModule","UnloadMode");
            sendMessageToModule(states.stationNumber()==0?"Sut1Module":"Sut2Module","UnloadMode");
            sendMessageToModule("LUTModule","UnloadMode");
        }
    }

    if(!(has_sensor&&has_lens))
    {
        if (!this->sut->moveZToSaftyInMushroom()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "SUT cannot move to saft Pos"};}

    }
    if((!has_sensor)&&(!send_sensor_request))
    {
        qInfo("need sensor has_product %d  has_ng_product %d has_ng_sensor %d",has_product,has_ng_product,has_ng_sensor);
        QString sut_module_name = states.stationNumber()==0?"Sut1Module":"Sut2Module";
        int sensor_state;
        if(has_product)
            sensor_state = MaterialState::IsGoodProduct;
        else if(has_ng_product)
            sensor_state = MaterialState::IsNgProduct;
        else if(has_ng_sensor)
            sensor_state = MaterialState::IsNgSensor;
        else
            sensor_state = MaterialState::IsEmpty;
        QJsonObject param;
        param.insert("MaterialState",MaterialTray::getMaterialStateName(sensor_state));
        if((states.stationTask() > 0) && (states.stationTask() > states.currentTask()))
            param.insert("TaskNumber",states.stationTask() - states.currentTask());
        sendMessageToModule(sut_module_name,"LoadSensorRequest",param);
        sendMessageToModule("SensorLoaderModule","LoadSensorRequest",param);
        finish_sensor_request = false;
        send_sensor_request = true;
        if(has_lens)
            if (!this->aa_head->moveToPickLensPosition()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA cannot move to picklens Pos"};}
    }

    if((!states.finishStationTask())&&(!has_lens)&&(!send_lens_request))
    {
        int lens_state;
        if(has_ng_lens)
            lens_state = MaterialState::IsNgLens;
        else if(has_lens)
            lens_state = MaterialState::IsRawLens;
        else
            lens_state = MaterialState::IsEmpty;
        QJsonObject param;
        param.insert("MaterialState",MaterialTray::getMaterialStateName(lens_state));
        if((states.stationTask() > 0) && (states.stationTask() > states.currentTask()))
            param.insert("TaskNumber",states.stationTask() - states.currentTask());
        sendMessageToModule("LUTModule","LoadLensRequest",param);

        qInfo("need lens has_ng_lens %d",has_ng_lens);
        if (!this->aa_head->moveToPickLensPosition()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "AA cannot move to picklens Pos"};}
        finish_lens_request = false;
        send_lens_request = true;
        if(has_sensor)
            if (!this->sut->moveToDownlookPos()) { return ErrorCodeStruct {ErrorCode::GENERIC_ERROR, "SUT cannot move to downlook Pos"};}
    }
    if((!states.finishStationTask())&&send_lens_request)
    {
        int time_out = 0;
        while (is_run) {
            if(finish_lens_request)
            {
                finish_lens_request = false;
                send_lens_request = false;
                qInfo("wait For LoadLens succeed.");
                if(parameters.enableLensVcm())
                {
                    QJsonObject temp_params;
                    temp_params["target_position"] = -1;
                    ErrorCodeStruct temp_result = performVCMInit(temp_params);
                    if(temp_result.code != ErrorCode::OK)
                    {
                        int alarm_id = sendAlarmMessage(CONTINUE_REJECT_OPERATION,temp_result.errorMessage);
                        QString opertion = waitMessageReturn(is_run,alarm_id);
                        if(opertion == REJECT_OPERATION)
                        {
                            NgLens();
                            result.code = temp_result.code;
                            result.errorMessage.append(temp_result.errorMessage);
                        }
                    }
                    temp_params["target_position"] = parameters.lensVcmWorkPosition();
                    temp_result = performVCMInit(temp_params);
                    if(temp_result.code != ErrorCode::OK)
                    {
                        int alarm_id = sendAlarmMessage(CONTINUE_REJECT_OPERATION,temp_result.errorMessage);
                        QString opertion = waitMessageReturn(is_run,alarm_id);
                        if(opertion == REJECT_OPERATION)
                        {
                            NgLens();
                            result.code = temp_result.code;
                            result.errorMessage.append(temp_result.errorMessage);
                        }
                    }
                }
                break;
            }
            QThread::msleep(10);
            time_out ++;
        }
        if(!is_run)
        {
            qInfo("wait For load lens interrupt.");
            result.code = ErrorCode::GENERIC_ERROR;
            result.errorMessage.append("sensor request fail.");
        }
    }
    if(send_sensor_request)
    {
        qInfo("wait sensor has_product %d has_ng_sensor %d",has_product,has_ng_sensor);

        int time_out = 0;
        while (is_run) {
            if(finish_sensor_request)
            {
                finish_sensor_request = false;
                send_sensor_request = false;
                qInfo("wait For LoadSensor succeed.");
                if(states.finishStationTask())
                {
                    while (is_run&&states.finishStationTask()) {
                        QThread::msleep(1000);
                    }
                    states.setFinishSensorTask(false);
                    states.setCurrentTask(0);
                }
                break;
            }
            QThread::msleep(10);
            time_out ++;
        }
        if(!is_run)
        {
            qInfo("wait For LoadSensor interrupt.");
            result.code = ErrorCode::GENERIC_ERROR;
            result.errorMessage.append("sensor request fail.");
        }
    }
    if(finish_delay > 0)
        Sleep(finish_delay);
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "AA_Load_Material", map);
    qInfo("Done Load Material");
    return result;
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

ErrorCodeStruct AACoreNew::performCameraUnload(int finish_delay)
{
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    aa_head->openGripper();
    if(finish_delay>0)
        Sleep(finish_delay);
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "Camera_Unload", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performZOffset(QJsonValue params)
{
    double z_offset_in_um = params["z_offset_in_um"].toDouble(0);
    int finish_delay = params["delay_in_ms"].toInt(0);
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    double curr_z = sut->carrier->GetFeedBackPos().Z;
    double target_z = sut->carrier->GetFeedBackPos().Z + z_offset_in_um/1000;
    sut->moveToZPos(target_z);
    QThread::msleep(50);
    double final_z = sut->carrier->GetFeedBackPos().Z;
    map.insert("z_offset_in_um", z_offset_in_um);
    map.insert("ori_z_pos", curr_z);
    map.insert("final_z_pos", final_z);
    if(finish_delay>0)
        Sleep(finish_delay);
    map.insert("timeElapsed", timer.elapsed());
    emit pushDataToUnit(this->runningUnit, "Z_Offset", map);
    return ErrorCodeStruct{ErrorCode::OK, ""};
}

ErrorCodeStruct AACoreNew::performXYOffset(QJsonValue params)
{
    double x_offset_in_um = params["x_offset_in_um"].toDouble(0);
    double y_offset_in_um = params["y_offset_in_um"].toDouble(0);
    int finish_delay = params["delay_in_ms"].toInt(0);
    QElapsedTimer timer; timer.start();
    QVariantMap map;
    mPoint3D ori_pos = sut->carrier->GetFeedBackPos();
    sut->stepMove_XY_Sync(x_offset_in_um/1000, y_offset_in_um/1000);
    QThread::msleep(200);
    mPoint3D final_pos = sut->carrier->GetFeedBackPos();
    map.insert("x_offset_in_um", x_offset_in_um);
    map.insert("y_offset_in_um", y_offset_in_um);
    map.insert("ori_x_pos", ori_pos.X);
    map.insert("ori_y_pos", ori_pos.Y);
    map.insert("final_x_pos", final_pos.X);
    map.insert("final_y_pos", final_pos.Y);
    if(finish_delay>0)
        Sleep(finish_delay);
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
        qInfo("d1: %f d2 %f f: %f dfov1: %f dfov2: %f", d1, d2, f, dfov1, dfov2);
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

void AACoreNew::updateAACoreSensorParameters(double scaleX, double scaleY, double angle)
{
    qInfo("AACoreNew update aa core sensor parameters is called. scaleX: %f scaleY: %f angle: %f", scaleX, scaleY, angle);
    this->parameters.setSensorXRatio(scaleX);
    this->parameters.setSensorYRatio(scaleY);
    this->parameters.setSensorOrientation(angle);
    //ToDo: Expect the sensor orientation is related to the aa compensation
}

PropertyBase *AACoreNew::getModuleState()
{
    return &states;
}

void AACoreNew::receivceModuleMessage(QVariantMap message)
{
    qInfo("receive module message %s",TcpMessager::getStringFromQvariantMap(message).toStdString().c_str());
    if(!message.contains("OriginModule"))
    {
        qInfo("message error! has no OriginModule.");
        return;
    }
    QString sut_module_name = states.stationNumber() == 0?"Sut1Module":"Sut2Module";
    if(message["OriginModule"].toString() == sut_module_name)
    {
        if(!message.contains("Message"))
        {
            qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            return;
        }

        if(message["Message"].toString()=="FinishLoadSensor")
        {
            int temp_state = MaterialTray::getMaterialStateFromName(message["SutMaterialState"].toString());
            if(temp_state == MaterialState::IsRawSensor)
                SetSensor();
            else
                SetNoSensor();
            //todo MaterialData
            finish_sensor_request = true;
        }
        else
        {
            qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            return;
        }
    }
    else if(message["OriginModule"].toString() == "LUTModule")
    {
        if(!message.contains("Message"))
        {
            qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            return;
        }

        if(message["Message"].toString() == "FinishLoadLens")
        {
            int temp_state = MaterialTray::getMaterialStateFromName(message["AAMaterialState"].toString());
            if(temp_state == MaterialState::IsRawLens)
                SetLens();
            else
                SetNoLens();
//            message["MaterialState"]
            finish_lens_request = true;
        }
        else
        {
            qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            return;
        }
    }
    else
    {
        qInfo("module name error %s",message["OriginModule"].toString().toStdString().c_str());
        return;
    }
}

QMap<QString, PropertyBase *> AACoreNew::getModuleParameter()
{
    QMap<QString, PropertyBase *> temp_map;
    temp_map.insert("AA_HEAD_PARAMS", &aa_head->parameters);
    temp_map.insert("AA_HEAD_POSITION", &aa_head->mushroom_position);
    temp_map.insert("AA_PICK_LENS_POSITION", &aa_head->pick_lens_position);
    temp_map.insert("AA_CORE_PARAMS", &this->parameters);
    temp_map.insert(DISPENSER_MODULE_PARAMETER,  &this->dispense->parameters);
    temp_map.insert(DISPENSER_PARAMETER, &this->dispense->dispenser->parameters);
    return temp_map;
}

void AACoreNew::setModuleParameter(QMap<QString, PropertyBase *> parameters)
{

}
