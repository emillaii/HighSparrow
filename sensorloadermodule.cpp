#include "sensorloadermodule.h"
#include "commonutils.h"
//#include "logicmanager.h"
#include "basemodulemanager.h"
#include <QMessageBox>
#include "commonutils.h"

SensorLoaderModule::SensorLoaderModule():ThreadWorkerBase ("SensorLoader")
{

}

void SensorLoaderModule::Init(SensorPickArm *pick_arm, MaterialTray *sensor_tray,
                              VisionLocation *sensor_vision, VisionLocation *vacancy_vision,
                              VisionLocation *sut_vision, VisionLocation *sut_sensor_vision,
                              VisionLocation *sut_product_vision, VisionLocation *sensor_pickarm_calibration_glass_vision)
{
    this->pick_arm = pick_arm;
    parts.append(this->pick_arm);
    this->tray = sensor_tray;
    parts.append(this->tray);
    this->sensor_vision = sensor_vision;
    parts.append(this->sensor_vision);
    this->vacancy_vision = vacancy_vision;
    parts.append(this->vacancy_vision);
    this->sut_vision = sut_vision;
    parts.append(this->sut_vision);
    this->sut_sensor_vision = sut_sensor_vision;
    parts.append(this->sut_sensor_vision);
    this->sut_product_vision = sut_product_vision;
    parts.append(this->sut_product_vision);
    this->sensor_pickarm_calibration_glass_vision = sensor_pickarm_calibration_glass_vision;
    parts.append(this->sensor_pickarm_calibration_glass_vision);
}

bool SensorLoaderModule::loadJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("parameters", &parameters);
    temp_map.insert("sut1_pr_position", &sut1_pr_position);
    temp_map.insert("sut2_pr_position", &sut2_pr_position);
    temp_map.insert("picker1_offset", &picker1_offset);
    temp_map.insert("picker2_offset", &picker2_offset);
    temp_map.insert("sensor_uph",&sensor_uph);
    temp_map.insert("left_sensor_uph",&left_sensor_uph);
    temp_map.insert("right_sensor_uph",&right_sensor_uph);
    temp_map.insert("right_sensor_uph",&product_uph);
    temp_map.insert("left_product_uph",&left_product_uph);
    temp_map.insert("right_product_uph",&right_product_uph);
    temp_map.insert("comprehensive_uph",&comprehensive_uph);
    temp_map.insert("left_comprehensive_uph",&left_comprehensive_uph);
    temp_map.insert("right_comprehensive_uph",&right_comprehensive_uph);
    PropertyBase::loadJsonConfig(file_name, temp_map);
    return true;
}

void SensorLoaderModule::saveJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("parameters", &parameters);
    temp_map.insert("sut1_pr_position", &sut1_pr_position);
    temp_map.insert("sut2_pr_position", &sut2_pr_position);
    temp_map.insert("picker1_offset", &picker1_offset);
    temp_map.insert("picker2_offset", &picker2_offset);
    temp_map.insert("sensor_uph",&sensor_uph);
    temp_map.insert("left_sensor_uph",&left_sensor_uph);
    temp_map.insert("right_sensor_uph",&right_sensor_uph);
    temp_map.insert("right_sensor_uph",&product_uph);
    temp_map.insert("left_product_uph",&left_product_uph);
    temp_map.insert("right_product_uph",&right_product_uph);
    temp_map.insert("comprehensive_uph",&comprehensive_uph);
    temp_map.insert("left_comprehensive_uph",&left_comprehensive_uph);
    temp_map.insert("right_comprehensive_uph",&right_comprehensive_uph);
    PropertyBase::saveJsonConfig(file_name, temp_map);
}

void SensorLoaderModule::startWork(int run_mode)
{
    qInfo("SensorLoader start run_mode :%d in %d",run_mode,QThread::currentThreadId());
    if(run_mode == RunMode::Normal)run(true);
    else if(run_mode == RunMode::NoMaterial)run(false);
    else if (run_mode == RunMode::VibrationTest) {
        is_run = true;
        while(is_run) {
            moveToSUTPRPos(false,true);
            QThread::msleep(2000);
            moveToSUTPRPos(true,true);
            QThread::msleep(2000);
        }
    }
}

void SensorLoaderModule::stopWork(bool wait_finish)
{
    is_run = false;
}

void SensorLoaderModule::performHandlingOperation(int cmd)
{
    qInfo("performHandling %d",cmd);
    bool result = false;
    int temp_value = 10;
    if(cmd%temp_value == HandlePosition::SUT_POS1){
        qInfo(u8"??SPA?SUT1??");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"????"))){
            result = moveToSUTPRPos(false,true);
//        }
    }
    else if(cmd%temp_value == HandlePosition::SUT_POS2){
        qInfo(u8"??SPA?SUT2??");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"????"))){
            result = moveToSUTPRPos(true,true);
//        }
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY1){
        qInfo(u8"??SPA?sensor??????");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"????"))){
            result = moveToTrayPos(0);
//        }
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY2){
        qInfo(u8"??SPA?????????");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"????"))){
            result = moveToTrayPos(2);
//        }
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY1_START_POS){
        qInfo(u8"??SPA?sensor??????");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"????"))){
            result = moveToStartPos(0);
//        }
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY2_START_POS){
        qInfo(u8"??SPA?????????");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"????"))){
            result = moveToStartPos(1);
//        }
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY1_END_POS){
        qInfo(u8"??SPA?sensor??????");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"????"))){
            result = moveToTray1EndPos();
//        }
    }
    else
        result =true;
    cmd =cmd/temp_value*temp_value;
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        return;
    }
    temp_value = 100;
    if(cmd%temp_value == HandlePR::RESET_PR){
        qInfo(u8"??PR??");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            pr_offset.ReSet();
//        }
    }
    else if(cmd%temp_value == HandlePR::SENSOR_PR){
        qInfo(u8"??sensorPR");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = performSensorPR();
            if(result)
            {
                states.setPicker1OffsetX(pr_offset.O_X);
                states.setPicker1OffsetY(pr_offset.O_Y);
            }
//        }
    }
    else if(cmd%temp_value == HandlePR::VACANCY_PR){
        qInfo(u8"??????PR");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = performVacancyPR();
//        }
    }
    else if(cmd%temp_value == HandlePR::SUT_PR){
        qInfo(u8"??SUT??PR");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = performSUTPR();
//        }
    }
    else if(cmd%temp_value == HandlePR::NG_SENSOR_PR){
        qInfo(u8"??NG sensor PR");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = performSUTSensorPR();
            if(result)
            {
                states.setPicker2OffsetX(pr_offset.O_X);
                states.setPicker2OffsetY(pr_offset.O_Y);
            }
//        }
    }
    else if(cmd%temp_value == HandlePR::PRODUCT_PR){
        qInfo(u8"????PR");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = performSUTProductPR();
            if(result)
            {
                states.setPicker2OffsetX(pr_offset.O_X);
                states.setPicker2OffsetY(pr_offset.O_Y);
            }
//        }
    }
    else
        result = true;
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        return;
    }
    cmd =cmd/temp_value*temp_value;
    temp_value = 1000;
    if(cmd%temp_value == HandleToWorkPos::TO_PICK_SENSOR_OFFSET){
        qInfo(u8"????1?????");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"????"))){
            result = moveToWorkPos(false);
//        }
    }
    else if(cmd%temp_value == HandleToWorkPos::TO_PICK_PRODUCT_OFFSET){
        qInfo(u8"????2?????");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"????"))){
            result = moveToWorkPos2(false);
//        }
    }
    if(cmd%temp_value == HandleToWorkPos::TO_PLACE_SENSOR_OFFSET){
        qInfo(u8"????1?????");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"????"))){
            result = moveToWorkPos(true);
//        }
    }
    else if(cmd%temp_value == HandleToWorkPos::TO_PLACE_PRODUCT_OFFSET){
        qInfo(u8"????2?????");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"????"))){
            result = moveToWorkPos2(true);
//        }
    }
    else if(cmd%temp_value == HandleToWorkPos::TO_PR_OFFSET){
        qInfo(u8"??SA?????????");
//        if(emit sendMsgSignal(tr(u8"??"),tr(u8"????"))){
            result = moveToPRResultPos(true);
//        }
    }
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        return;
    }
    cmd =cmd/temp_value*temp_value;
    temp_value = 100000;
    qInfo("cmd : %d", cmd);
    if(cmd%temp_value == handlePickerAction::PICK_SENSOR_FROM_TRAY){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = pickTraySensor();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_SENSOR_TO_SUT1){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = placeSensorToSUT("remote");
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_SENSOR_TO_SUT2){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = placeSensorToSUT("::1");
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_NG_SENSOR_FROM_SUT1){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = pickSUTSensor("remote");
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_NG_SENSOR_FROM_SUT2){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = pickSUTSensor("::1");
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_NG_SENSOR_TO_TRAY){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = placeSensorToTray();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_PRODUCT_FROM_SUT1){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = pickSUTProduct("remote");
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_PRODUCT_FROM_SUT2){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = pickSUTProduct("::1");
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_PRODUCT_TO_TRAY){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = placeProductToTray();
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_SENSOR_IN_TRAY){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = picker1MeasureHight(true);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_SENSOR_IN_SUT1){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = picker1MeasureHight(false);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_NG_SENSOR_IN_SUT1){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = picker2MeasureHight(false,false);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_NG_SENSOR_IN_TRAY){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = picker2MeasureHight(true,false);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_PRODUCT_IN_SUT1){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = picker2MeasureHight(false,true);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_PRODUCT_IN_TRAY){
        if(emit sendMsgSignal(tr(u8"??"),tr(u8"??????"))){
            result = picker2MeasureHight(true,true);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_Z_OFFSET)
        result = measureZOffset();
    else
        result = true;
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        return;
    }
}

void SensorLoaderModule::receiveRequestMessage(QString message, QString client_ip)
{
    qInfo("Sut Module receive command:%s from ip: %s", message.toStdString().c_str(), client_ip.toStdString().c_str());
    QJsonObject obj = getJsonObjectFromString(message);
    QString cmd = obj["cmd"].toString("");
    obj.insert("client_ip",client_ip);
    if (cmd == "sensorReq") {
        qInfo("Enqueue the sensor request command in request quene");
        requestQueue.enqueue(obj);
    }
    else if (cmd.length() > 0)
    {
        qInfo("Enqueue the %s in action queue", cmd.toStdString().c_str());
        actionQueue.enqueue(obj);
    }
}

void SensorLoaderModule::receiveChangeTrayFinish()
{
    qInfo("SensorLoaderModule receiveChangeTrayFInish");
    QMutexLocker temp_locker(&tray_mutex);
    if(states.waitingChangeTray())
    {
        states.setFinishChangeTray(true);
    }
    else
    {
        qInfo("SensorLoaderModule receiveChangeTrayFInish but not effective");
    }
}


void SensorLoaderModule::resetLogic()
{
    if(is_run)return;
    states.setHasTray(false);//sensor tray ready

    states.setSutHasSensor(false);
    states.setSutHasNgSensor(false);
    states.setSutHasProduct(false);
    states.setSutHasNgProduct(false);
    states.setNeedLoadSensor(false);
    states.setNeedChangTray(false);
    states.setAllowChangeTray(false);
    states.setHasPickedSensor(false);
    states.setHasPickedProduct(false);
    states.setHasPickedNgProduct(false);
    states.setHasPickedNgSensor(false);
    states.setBeExchangeMaterial(false);
    states.setCmd("");
    states.setWaitingChangeTray(false);
    states.setFinishChangeTray(false);
    requestQueue.clear();
    actionQueue.clear();
    tray->resetTrayState();
    tray->resetTrayState(1);
    tray->resetTrayState(2);
    states.setHasUnpickedProduct(false);
    states.setHasUnpickedNgProduct(false);
    states.setHasUnpickedNgSensor(false);
}

void SensorLoaderModule::openServer(int port)
{
    server = new SparrowQServer(port);
    connect(server, &SparrowQServer::receiveRequestMessage, this, &SensorLoaderModule::receiveRequestMessage, Qt::DirectConnection);
    connect(this, &SensorLoaderModule::sendMessageToClient, this->server, &SparrowQServer::sendMessageToClient);
}

void SensorLoaderModule::cameraTipOffsetCalibration(int pickhead)
{
    qInfo("Start camera tip offset calibration: %d", pickhead);
    moveToStartPos(1);
    QThread::msleep(1000);
    PrOffset offset;
    this->sensor_pickarm_calibration_glass_vision->performPR(offset);
    std::vector<cv::Point2d> points;
    if (pickhead == 1)
    {
        this->pick_arm->stepMove_XYT1_Synic(-offset.X,-offset.Y, -50);
        for (int i = 0; i < 5; i++)
        {
            PrOffset offset;
            QThread::msleep(1000);
            this->sensor_pickarm_calibration_glass_vision->performPR(offset);
            qInfo("PR offset: %f %f", offset.X, offset.Y);
            points.push_back(cv::Point2d(offset.X, offset.Y));
            this->pick_arm->stepMove_XYT1_Synic(picker1_offset.X(), picker1_offset.Y(), 0);
            bool result = pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),15,parameters.vcmMargin(),parameters.finishDelay(),true,true,30000);
            QThread::msleep(1000);
            result &= pick_arm->ZSerchReturn(30000);
            this->pick_arm->stepMove_XYT1_Synic(0, 0, 20);
            result = pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),15,parameters.vcmMargin(),parameters.finishDelay(),false,true,30000);
            QThread::msleep(1000);
            result &= pick_arm->ZSerchReturn(30000);
            this->pick_arm->stepMove_XYT1_Synic(-picker1_offset.X(), -picker1_offset.Y(), 0);
        }
        this->pick_arm->stepMove_XYT1_Synic(0,0, -100);
        cv::Point2d center; double radius;
        fitCircle(points, center, radius);
        qInfo("Fit cicle: x: %f y: %f r:%f", center.x, center.y, radius);
        this->picker1_offset.setX(picker1_offset.X() + center.x);
        this->picker1_offset.setY(picker1_offset.Y() + center.y);
    } else {
        this->pick_arm->stepMove_XYT2_Synic(-offset.X,-offset.Y, -50);
        for (int i = 0; i < 5; i++)
        {
            PrOffset offset;
            QThread::msleep(1000);
            this->sensor_pickarm_calibration_glass_vision->performPR(offset);
            qInfo("PR offset: %f %f", offset.X, offset.Y);
            points.push_back(cv::Point2d(offset.X, offset.Y));
            this->pick_arm->stepMove_XYT2_Synic(picker2_offset.X(), picker2_offset.Y(), 0);
            bool result = pick_arm->ZSerchByForce2(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),15,parameters.vcmMargin(),parameters.finishDelay(),true,true,30000);
            QThread::msleep(1000);
            result &= pick_arm->ZSerchReturn2(30000);
            this->pick_arm->stepMove_XYT2_Synic(0, 0, 20);
            result = pick_arm->ZSerchByForce2(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),15,parameters.vcmMargin(),parameters.finishDelay(),false,true,30000);
            QThread::msleep(1000);
            result &= pick_arm->ZSerchReturn2(30000);
            this->pick_arm->stepMove_XYT2_Synic(-picker2_offset.X(), -picker2_offset.Y(), 0);
        }
        this->pick_arm->stepMove_XYT2_Synic(0,0, -100);
        cv::Point2d center; double radius;
        fitCircle(points, center, radius);
        qInfo("Fit cicle: x: %f y: %f r:%f", center.x, center.y, radius);
        this->picker2_offset.setX(picker2_offset.X() + center.x);
        this->picker2_offset.setY(picker2_offset.Y() + center.y);
    }
}

void SensorLoaderModule::run(bool has_material)
{
    //???????????

    int sensor_tray_index = 0;
    int product_tray_index = 1;
    int ng_tray_index = 2;
    is_run = true;
    finish_stop = false;
    int pr_times = 5;
    bool has_task = true;
    int change_tray_time_out = parameters.changeTrayTimeOut();

    bool waiting_change_tray = false;
    bool finish_change_tray = false;
    time_label = QTime::currentTime();
    while (is_run)
    {
        {
            QMutexLocker temp_locker(&tray_mutex);
            waiting_change_tray = states.waitingChangeTray();
            finish_change_tray = states.finishChangeTray();
        }
        if(!has_task)
            QThread::msleep(1000);
        has_task = false;
        //???
        if((!states.allowChangeTray())&&states.hasTray()&&states.hasPickedProduct())
        {
            has_task = true;
            vacancy_vision->OpenLight();
            if(!moveToProductTrayNextPos())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!performVacancyPR())&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setHasPickedProduct(false);
                    continue;
                }
                if(!is_run)break;
            }
            vacancy_vision->CloseLight();
            if(!moveToWorkPos2(true))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setHasPickedProduct(false);
                    continue;
                }
                if(!is_run)break;
            }
            if((!placeProductToTray())&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                    continue;
            }
            tray->setCurrentMaterialState(MaterialState::IsProduct,product_tray_index);
            states.setHasPickedProduct(false);
            if(!is_run)break;
        }
        //?Ng??
        if((!states.allowChangeTray())&&states.hasTray()&&states.hasPickedNgProduct())
        {
            has_task = true;
            vacancy_vision->OpenLight();
            if(!moveToNgTrayNextPos())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!performVacancyPR())&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setHasPickedNgProduct(false);
                    continue;
                }
                if(!is_run)break;
            }
            vacancy_vision->CloseLight();
            if(!moveToWorkPos2(true))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setHasPickedNgProduct(false);
                    continue;
                }
                if(!is_run)break;
            }
            if((!placeProductToTray())&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                    continue;
            }
            tray->setCurrentMaterialState(MaterialState::IsNgProduct,ng_tray_index);
            states.setHasPickedNgProduct(false);
            if(!is_run)break;
        }
        //?NGSensor
        if((!states.allowChangeTray())&&states.hasTray()&&states.hasPickedNgSensor())
        {
            has_task = true;
            vacancy_vision->OpenLight();
            if(!moveToNgTrayNextPos())
            {
                AppendError("???Nguyen????!");
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                waitMessageReturn(is_run);
                tray->resetTrayState(2);
            }
            if((!performVacancyPR())&&has_material)
            {
                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                if(waitMessageReturn(is_run))
                    is_run = false;
                else
                    continue;
                if(!is_run)break;
            }
            vacancy_vision->CloseLight();
            if(!moveToWorkPos2(true))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setHasPickedNgSensor(false);
                    continue;
                }
                if(!is_run)break;
            }
            if((!placeSensorToTray())&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                    continue;
            }
            tray->setCurrentMaterialState(MaterialState::IsNg,ng_tray_index);
            states.setHasPickedNgSensor(false);
            if(!is_run)break;
        }
        //????????
        if((!states.allowChangeTray()))
        {
            if((!states.hasTray())||checkTrayNeedChange())
                states.setAllowChangeTray(true);
        }
        //??
        if((!finish_stop)&&(!states.allowChangeTray())&&states.hasTray()&&(!states.hasPickedSensor())&&(!states.hasPickedProduct())&&(!states.hasPickedNgProduct())&&(!states.hasPickedNgSensor()))
        {
            has_task = true;
            sensor_vision->OpenLight();
            if(!moveToSensorTrayNextPos())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!performSensorPR())&&has_material)
            {
                if(pr_times > 0)
                {
                    pr_times--;
                    tray->setCurrentMaterialState(MaterialState::IsNg,sensor_tray_index);
                    picked_material = tray->getCurrentIndex(sensor_tray_index);

                    AppendError(u8"????.");
                    sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
                    continue;
                }
                else
                {
                    pr_times = 5;
                    AppendError(u8"pr??????");
                    sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                    waitMessageReturn(is_run);
                    if(!is_run)break;
                    continue;
                }
            }
            else
            {
                sendAlarmMessage(ErrorLevel::TipNonblock,"");
            }
            sensor_vision->CloseLight();
            states.setPicker1OffsetX(pr_offset.O_X);
            states.setPicker1OffsetY(pr_offset.O_Y);
            pr_times = 5;
            if(!moveToWorkPos(false))
            {
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                waitMessageReturn(is_run);
                if(!is_run)break;
            }
            if((!pickTraySensor())&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(!waitMessageReturn(is_run))
                    states.setHasPickedSensor(true);
            }
            else
                states.setHasPickedSensor(true);
            tray->setCurrentMaterialState(MaterialState::IsEmpty,sensor_tray_index);
            picked_material = tray->getCurrentIndex(sensor_tray_index);
            if(!is_run)break;
        }
        //????
        if(!moveToSUTPRPos(states.beExchangeMaterial()?isLocalHost:(!isLocalHost)))
        {
            sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
            is_run = false;
            break;
        }
        //????
        if(states.allowChangeTray())
        {
            if(waiting_change_tray)
            {
                if(finish_change_tray)
                {
                    {
                        QMutexLocker temp_locker(&tray_mutex);
                        states.setFinishChangeTray(false);
                        states.setWaitingChangeTray(false);
                    }
                    states.setHasTray(true);
                    tray->resetTrayState(0);
                    tray->resetTrayState(1);
                    states.setNeedChangTray(false);
                    states.setAllowChangeTray(false);
                    qInfo("finishChangeTray");
                }
                else
                {
                    if(!has_task)
                        QThread::msleep(1000);
                    qInfo("waitingChangeTray");
                    change_tray_time_out -=1000;
                    if(change_tray_time_out<=0)
                    {
                        AppendError(QString(u8"??????,????%d,??????????????").arg(parameters.changeTrayTimeOut()));
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                        if(waitMessageReturn(is_run))
                            states.setWaitingChangeTray(false);
                        else
                            change_tray_time_out = parameters.changeTrayTimeOut();
                    }
                }
            }
            else
            {
                {
                    QMutexLocker temp_locker(&tray_mutex);
                    states.setFinishChangeTray(false);
                    emit sendChangeTrayRequst();
                    states.setWaitingChangeTray(true);
                }
                change_tray_time_out = parameters.changeTrayTimeOut();
                qInfo("sendChangeTray");
            }
        }
        //sut??
        if ((!states.allowChangeTray())&&(!requestQueue.isEmpty()) && (!states.beExchangeMaterial())&&states.hasPickedSensor()&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct())&&(!states.hasPickedNgProduct()))
        {
            has_task = true;
            QJsonObject obj = requestQueue.dequeue();
            qInfo("Start to consume request: %s", getStringFromJsonObject(obj).toStdString().c_str());
            QString client_ip = obj["client_ip"].toString("");
            servingIP = client_ip;
            QString cmd = obj["cmd"].toString("");
            if(client_ip == "::1")
            {
                qInfo("This command come from localhost");
                isLocalHost = true;
            }
            else
            {
                qInfo("This command come from anther computer");
                isLocalHost = false;
            }

            if (cmd == "sensorReq")
            {
                qInfo("start commincate with %s",servingIP.toStdString().c_str());
//                actionQueue.clear();
                states.setBeExchangeMaterial(true);
                sendEvent("sensorResp");
            }
        }

        if ((!states.allowChangeTray())&&(!actionQueue.isEmpty()) && states.beExchangeMaterial())
        {
            has_task = true;
            while(states.beExchangeMaterial())
            {
                if((actionQueue.isEmpty()))
                {
                    QThread::msleep(1);
                    if(states.cmd() == "")
                        continue;
                    if(!is_run)
                        break;
                }
                if(states.cmd() == "")
                {
                    QJsonObject obj = actionQueue.dequeue();
                    qInfo("Start to consume action request: %s", getStringFromJsonObject(obj).toStdString().c_str());
                    states.setCmd(obj["cmd"].toString(""));
                }
                if((states.cmd() == "unloadProductReq")&&states.hasUnpickedProduct())
                {
                    sendEvent("unloadProductResp");
                    states.setCmd("");
                }
                else if((states.cmd() == "unloadProductReq")&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct())&&(!states.hasUnpickedProduct()))
                {
                    sut_product_vision->OpenLight();
                    if(!moveToSUTPRPos(isLocalHost))
                    {
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if((!performSUTProductPR())&&has_material)
                    {
                        AppendError(u8"??????!");
                        sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                        int result = waitMessageReturn(is_run);
                        if(result)is_run = false;
                        if(!is_run)break;
                        if(result)
                            continue;
                    }
                    sut_product_vision->CloseLight();
                    if(!moveToWorkPos2(false))
                    {
                        sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                        waitMessageReturn(is_run);
                        if(!is_run)break;
                    }
                    if((!pickSUTProduct(isLocalHost?"::1":"remote"))&&has_material)
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            states.setSutHasProduct(false);
                        }
                        else
                        {
                            states.setHasUnpickedProduct(true);
                            states.setSutHasProduct(false);
                            states.setHasPickedProduct(true);
                        }
                    }
                    else
                    {
                        states.setHasUnpickedProduct(true);
                        states.setSutHasProduct(false);
                        states.setHasPickedProduct(true);
                    }
                    sendEvent("unloadProductResp");
                    states.setCmd("");
                    product_uph.addCurrentNumber(updateAccumulatedHour());
                    product_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    comprehensive_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    if(isLocalHost)
                    {
                        right_product_uph.addCurrentNumber(updateAccumulatedHour(false));
                        right_product_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                        right_comprehensive_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    }
                    else
                    {
                        left_product_uph.addCurrentNumber(updateAccumulatedHour(false));
                        left_product_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                        left_comprehensive_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    }
                }
                else if((states.cmd() == "unloadNgProductReq")&&states.hasUnpickedNgProduct())
                {
                    sendEvent("unloadNgProductResp");
                    states.setCmd("");
                }
                else if((states.cmd() == "unloadNgProductReq")&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct())&&(!states.hasUnpickedNgProduct()))
                {
                    sut_product_vision->OpenLight();
                    if(!moveToSUTPRPos(isLocalHost))
                    {
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if((!performSUTProductPR())&&has_material)
                    {
                        AppendError(u8"??????!");
                        sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                        int result = waitMessageReturn(is_run);
                        if(result)is_run = false;
                        if(!is_run)break;
                        if(result)
                            continue;
                    }
                    sut_product_vision->CloseLight();
                    if(!moveToWorkPos2(false))
                    {
                        sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                        waitMessageReturn(is_run);
                        if(!is_run)break;
                    }
                    if((!pickSUTProduct(isLocalHost?"::1":"remote"))&&has_material)
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            states.setSutHasNgProduct(false);
                        }
                        else
                        {
                            states.setHasUnpickedNgProduct(true);
                            states.setSutHasNgProduct(false);
                            states.setHasPickedNgProduct(true);
                        }
                    }
                    else
                    {
                        states.setHasUnpickedNgProduct(true);
                        states.setSutHasNgProduct(false);
                        states.setHasPickedNgProduct(true);
                    }
                    sendEvent("unloadNgProductResp");
                    states.setCmd("");
                    product_uph.addCurrentNumber(updateAccumulatedHour());
                    comprehensive_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    if(isLocalHost)
                    {
                        right_product_uph.addCurrentNumber(updateAccumulatedHour(false));
                        right_comprehensive_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    }
                    else
                    {
                        left_product_uph.addCurrentNumber(updateAccumulatedHour(false));
                        left_comprehensive_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    }
                }

                else if((states.cmd() == "unloadNgSensorReq")&&states.hasUpickedNgSensor())
                {
                    sendEvent("unloadNgSensorResp");
                    states.setCmd("");
                }
                else if((states.cmd() == "unloadNgSensorReq")&&(!states.hasPickedNgSensor())&&(!states.hasUpickedNgSensor()))
                {
                    //?NG sensor
                    sut_sensor_vision->OpenLight();
                    if(!moveToSUTPRPos(isLocalHost))
                    {
                        AppendError("moveToSUTPRPos fail!");
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if((!performSUTSensorPR())&&has_material)
                    {
                        AppendError(u8"NG????!");
                        sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                        int result = waitMessageReturn(is_run);
                        if(result)
                            is_run = false;
                        else
                            continue;
                        if(!is_run)break;
                    }
                    sut_sensor_vision->CloseLight();
                    states.setPicker2OffsetX(pr_offset.O_X);
                    states.setPicker2OffsetY(pr_offset.O_Y);
                    if(!moveToWorkPos2(false))
                    {
                        sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                        waitMessageReturn(is_run);
                        if(!is_run)break;
                    }
                    if((!pickSUTSensor(isLocalHost?"::1":"remote"))&&has_material)
                    {
                        AppendError("pickSUTSensor fail!");
                        sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            states.setSutHasNgSensor(false);
                            continue;
                        }
                        else
                        {
                            states.setHasUnpickedNgSensor(true);
                            states.setSutHasNgSensor(false);
                            states.setHasPickedNgSensor(true);
                        }
                    }
                    else
                    {
                        states.setHasUnpickedNgSensor(true);
                        states.setSutHasNgSensor(false);
                        states.setHasPickedNgSensor(true);
                    }
                    sendEvent("unloadNgSensorResp");
                    states.setCmd("");
                    sensor_uph.addCurrentReslutNumber(updateAccumulatedHour());
                    if(isLocalHost)
                        right_sensor_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    else
                        left_sensor_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                }
                else if ((states.cmd() == "loadSensorReq")&&states.hasPickedSensor())
                {
                    //???SUT
                    if(!movePicker1ToSUTPos(isLocalHost))
                    {
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if((!placeSensorToSUT(isLocalHost?"::1":"remote"))&&has_material)
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            states.setHasPickedSensor(false);
                            break;
                        }
                    }
                    states.setNeedLoadSensor(false);
                    states.setHasPickedSensor(false);
                    sendEvent("loadSensorResp");
                    states.setCmd("");
                    sensor_uph.addCurrentNumber(updateAccumulatedHour());
                    comprehensive_uph.addCurrentNumber(updateAccumulatedHour(false));
                    if(isLocalHost)
                    {
                        right_sensor_uph.addCurrentNumber(updateAccumulatedHour(false));
                        right_comprehensive_uph.addCurrentNumber(updateAccumulatedHour(false));
                    }
                    else
                    {
                        left_sensor_uph.addCurrentNumber(updateAccumulatedHour(false));
                        left_comprehensive_uph.addCurrentNumber(updateAccumulatedHour(false));
                    }
                    states.setHasUnpickedNgSensor(false);
                    states.setHasUnpickedProduct(false);
                    states.setHasUnpickedNgProduct(false);
                    states.setBeExchangeMaterial(false);
                }
                else if((((states.cmd() == "unloadProductReq")||(states.cmd() == "unloadNgSensorReq"))&&(states.hasPickedNgSensor()||states.hasPickedProduct()))
                        ||((states.cmd() == "loadSensorReq")&&(!states.hasPickedSensor())))
                {
                    break;
                }
            }
            }

        //        //???SUT
//        if(states.needLoadSensor()&&states.hasPickedSensor())
//        {
//            has_task = true;
//            if(!moveToSUTPRPos())
//            {
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            if(!moveToWorkPos())
//            {
//                AppendError("moveToWorkPos fail!");
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            if((!placeSensorToSUT())&&has_material)
//            {
//                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
//                if(waitMessageReturn(is_run))
//                    states.setHasPickedSensor(false);
//                else
//                {
//                    states.setNeedLoadSensor(false);
//                    states.setHasPickedSensor(false);
//                }
//            }
//            else
//            {
//                states.setNeedLoadSensor(false);
//                states.setHasPickedSensor(false);
//            }
//            sut_raw_material = picked_material;
//        }
//        //???
//        if(states.sutHasProduct()&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct()))
//        {
//            has_task = true;
//            if(!moveToSUTPRPos())
//            {
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            if((!performSUTProductPR())&&has_material)
//            {
//                AppendError(u8"NG????!");
//                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
//                int result = waitMessageReturn(is_run);
//                if(result)is_run = false;
//                if(!is_run)break;
//                if(result)
//                    continue;
//            }
//            if(!moveToWorkPos2())
//            {
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            if((!pickSUTProduct())&&has_material)
//            {
//                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
//                if(waitMessageReturn(is_run))
//                    states.setSutHasProduct(false);
//                else
//                {
//                    states.setSutHasProduct(false);
//                    states.setHasPickedProduct(true);
//                }
//            }
//            else
//            {
//                states.setSutHasProduct(false);
//                states.setHasPickedProduct(true);
//            }

//        }
//        //?NGsensor
//        if(states.sutHasNgSensor()&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct()))
//        {
//            has_task = true;
//            if(!moveToSUTPRPos())
//            {
//                AppendError("moveToSUTPRPos fail!");
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            if((!performSUTSensorPR())&&has_material)
//            {
//                AppendError(u8"NG????!");
//                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
//                int result = waitMessageReturn(is_run);
//                if(result)is_run = false;
//                if(!is_run)break;
//                if(result)
//                    continue;
//            }
//            if(!moveToWorkPos2())
//            {
//                AppendError("moveToWorkPos2 fail!");
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            if((!pickSUTSensor())&&has_material)
//            {
//                AppendError("pickSUTSensor fail!");
//                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
//                if(waitMessageReturn(is_run))
//                    states.setSutHasNgSensor(false);
//                else
//                {
//                    states.setSutHasNgSensor(false);
//                    states.setHasPickedNgSensor(true);
//                }
//            }
//            else
//            {
//                states.setSutHasNgSensor(false);
//                states.setHasPickedNgSensor(true);
//            }
//        }
    }
    qInfo("sensor Loader Module stoped");
}

bool SensorLoaderModule::checkTrayNeedChange()
{
    if(tray->isTrayNeedChange(0)||tray->isTrayNeedChange(1))
        return true;
    return false;
}

bool SensorLoaderModule::moveToSensorTrayNextPos()
{
    bool result = tray->findNextPositionOfInitState();
    QPointF tem_pos = tray->getCurrentPosition(0);
    if(result)
        result &=  pick_arm->move_XYT1_Synic(tem_pos.x(),tem_pos.y(),parameters.picker1ThetaOffset());
    if(!result)
        AppendError(QString(u8"???sensor????????"));
    qInfo(u8"???sensor???????");
    return result;
}

bool SensorLoaderModule::moveToProductTrayNextPos()
{
    bool result = tray->findNextPositionOfInitState(1);
    if(result)
        result &=  pick_arm->move_XY_Synic(tray->getCurrentPosition(1));
    if(!result)
        AppendError(QString(u8"?????????????"));
    qInfo(u8"???????????");
    return result;
}

bool SensorLoaderModule::moveToNgTrayNextPos()
{
    bool result_tray = tray->findNextPositionOfInitState(2);
    bool result = true;
    if(result_tray)
        result &=  pick_arm->move_XY_Synic(tray->getCurrentPosition(2));
    if(!result)
        AppendError(QString(u8"???Ng????????"));
    qInfo(u8"???Ng??????");
    return result&&result_tray;
}

bool SensorLoaderModule::moveToSUTPRPos(bool is_local,bool check_softlanding)
{
    qInfo("moveToSUTPRPos is_local %d",is_local);
    bool result;
    QPointF temp_pos;
    if(is_local)
        temp_pos =  sut2_pr_position.ToPointF();
    else
        temp_pos =  sut1_pr_position.ToPointF();
    result = pick_arm->move_XYT2_Synic(temp_pos.x(),temp_pos.y(),parameters.picker2ThetaOffset(),check_softlanding);
    if(!result)
        AppendError(QString(u8"??SPA??SUT????%1").arg(is_local));
    qInfo(u8"??SPA???SUT%d??,???%d",is_local,result);
    return result;
}

bool SensorLoaderModule::movePicker1ToSUTPos(bool is_local)
{
    bool result;
    QPointF temp_pos;
    if(is_local)
        temp_pos =  sut2_pr_position.ToPointF();
    else
        temp_pos =  sut1_pr_position.ToPointF();
    result = pick_arm->move_XYT2_Synic(temp_pos.x() + picker1_offset.X() - states.picker1OffsetX(),temp_pos.y() + picker1_offset.Y() - states.picker1OffsetY(),parameters.picker2ThetaOffset());
    if(!result)
        AppendError(QString(u8"??SPA???SUT????%1").arg(is_local));
    qInfo(u8"??SPA???SUT????%d,???%d",is_local,result);
    return result;
}

bool SensorLoaderModule::performSensorPR()
{
    qInfo("performSensorPR");
    bool result = sensor_vision->performPR(pr_offset);
    if(!result)
        AppendError(QString(u8"????sensor????!"));
    return  result;
}

bool SensorLoaderModule::performVacancyPR()
{
    qInfo("performVacancyPR");
    bool result = vacancy_vision->performPR(pr_offset);
    if(!result)
        AppendError(QString(u8"??????????!"));
    return  result;
}

bool SensorLoaderModule::performSUTPR()
{
    qInfo("performSUTPR");
    bool result = sut_vision->performPR(pr_offset);
    if(!result)
        AppendError(QString(u8"??SUT????!"));
    return  result;
}

bool SensorLoaderModule::performSUTSensorPR()
{
    qInfo("performSUTSensorPR");
    bool result = sut_sensor_vision->performPR(pr_offset);
    if(!result)
        AppendError(QString(u8"??SUT??sensor????!"));
    return  result;
}

bool SensorLoaderModule::performSUTProductPR()
{
    qInfo("performSUTProductPR");
    bool result =  sut_product_vision->performPR(pr_offset);
    if(!result)
        AppendError(QString(u8"??SUT????!"));
    return  result;
}

bool SensorLoaderModule::moveToWorkPos(bool check_state,bool check_softlanding)
{
    PrOffset temp(picker1_offset.X() - states.picker1OffsetX() - pr_offset.X,picker1_offset.Y() - states.picker1OffsetY() - pr_offset.Y,-pr_offset.Theta);
    qInfo("moveToWorkPos offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XYT1_Pos(temp.X,temp.Y,temp.Theta,check_softlanding);
    bool check_result = checkPickedSensor(check_state);
    result &= pick_arm->waitStepMove_XYT1();
    if(!result)
        AppendError(QString(u8"?1???????(step x %1,y %2,t %3)??!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    result &= check_result;
	return  result;
}

bool SensorLoaderModule::moveToWorkPos2(bool check_state,bool check_softlanding)
{
    PrOffset temp(picker2_offset.X() - states.picker2OffsetX() - pr_offset.X,picker2_offset.Y() - states.picker2OffsetY() - pr_offset.Y,-pr_offset.Theta);
    qInfo("moveToWorkPos2 offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XYT2_Pos(temp.X,temp.Y,temp.Theta,check_softlanding);
    bool check_result = checkPickedNgOrProduct(check_state);
    result &= pick_arm->waitStepMove_XYT2();
    if(!result)
        AppendError(QString(u8"?2????????(step x %1,y %2,t %3)??!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    result &= check_result;
    return  result;
}

bool SensorLoaderModule::moveToPRResultPos(bool check_softlanding)
{
    PrOffset temp(pr_offset.X,pr_offset.Y,pr_offset.Theta);
    qInfo("moveToPRResultPos offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XY_Synic(temp.X,temp.Y,check_softlanding);
    if(!result)
        AppendError(QString(u8"???????(step x %1,y %2,t %3)??!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    return  result;
}

bool SensorLoaderModule::picker1SearchZ(double z,bool is_open, int time_out)
{
    qInfo("picker1SearchZ z %f is_open %d timeout %d",z,is_open,time_out);
    bool result = pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),parameters.finishDelay(),is_open,false,time_out);
    result &= pick_arm->ZSerchReturn(time_out);
    return result;
}

bool SensorLoaderModule::picker1SearchSutZ(double z, QString dest, QString cmd, bool is_open, int time_out)
{
    qInfo("picker1SearchSutZ z %f dest %s cmd %s is_open %d time_out %d",z,dest.toStdString().c_str(),cmd.toStdString().c_str(),is_open,time_out);
    bool result = pick_arm->move_XeYe_Z1_XY(z - parameters.escapeHeight(),parameters.escapeX(),parameters.escapeY());
    if(result)
    {
        result = pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),parameters.finishDelay(),is_open,false,time_out);
        //        if(result)
        //        {
        sendCmd(dest,cmd);
        QThread::msleep(200);
        //        }
        result &= pick_arm->ZSerchReturn(time_out);
    }
    result &= pick_arm->picker1->motor_z->MoveToPosSync(0);
    return result;
}
bool SensorLoaderModule::picker2SearchZ(double z,bool is_open, int time_out)
{
    qInfo("picker2SearchZ z %f is_open %d time_out %d",z,is_open,time_out);
    bool result = pick_arm->ZSerchByForce2(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z + parameters.zOffset(),parameters.vcmMargin(),parameters.finishDelay(),is_open,false,time_out);
    result &= pick_arm->ZSerchReturn2(time_out);
    return result;
}

bool SensorLoaderModule::picker2SearchSutZ(double z, QString dest, QString cmd, bool is_open, int time_out)
{
    qInfo("picker2SearchZ %s",dest.toStdString().c_str());
    bool result = pick_arm->picker2->motor_z->MoveToPosSync(z - parameters.escapeHeight());
    if(result)
    {
        result = pick_arm->ZSerchByForce2(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z + parameters.zOffset(),parameters.vcmMargin(),parameters.finishDelay(),is_open,false,time_out);
//        if(result)
//        {
        sendCmd(dest,cmd);
        QThread::msleep(200);
//        }
        result &= pick_arm->ZSerchReturn2(time_out);
    }
    result &= pick_arm->move_XeYe_Z2(0,parameters.escapeX(),parameters.escapeY());
    return result;
}

bool SensorLoaderModule::checkPickedSensor(bool check_state)
{
    bool result;
    result = pick_arm->picker1->vacuum->checkHasMateriel();
    if(result == check_state)
        return true;
    QString error = QString(u8"sensor?????%1?,????%2??").arg(check_state?u8"?":u8"?").arg(result?u8"?":u8"?");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool SensorLoaderModule::checkPickedNgOrProduct(bool check_state)
{
    bool result;
    result = pick_arm->picker2->vacuum->checkHasMateriel();
    if(result == check_state)
        return true;
    QString error = QString(u8"???????%1?,????%2??").arg(check_state?u8"?":u8"?").arg(result?u8"?":u8"?");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool SensorLoaderModule::pickTraySensor(int time_out)
{
    qInfo("pickTraySensor time_out %d",time_out);
    bool result = picker1SearchZ(parameters.pickSensorZ(),true,time_out);
    if(!result)
        AppendError(QString(u8"?sensor??sensor??"));
    if(result)
        result &= checkPickedSensor(true);
    return result;
}

bool SensorLoaderModule::placeSensorToSUT(QString dest,int time_out)
{
    qInfo("placeSensorToSUT dest %s time_out %d",dest.toStdString().c_str(),time_out);
   bool result = picker1SearchSutZ(parameters.placeSensorZ(),dest,"vacuumOnReq",false,time_out);
    if(!result)
        AppendError(QString(u8"?sensor?SUT%1??").arg(dest=="remote"?1:2));
    return result;
}

bool SensorLoaderModule::pickSUTSensor(QString dest,int time_out)
{
    qInfo("pickSUTSensor dest %s time_out %d",dest.toStdString().c_str(),time_out);
    bool result = picker2SearchSutZ(parameters.pickNgSensorZ(),dest,"vacuumOffReq",true,time_out);
    if(!result)
        AppendError(QString(u8"?SUT%1?NGsenor??").arg(dest=="remote"?1:2));
    if(result)
        result &= checkPickedNgOrProduct(true);
    return result;
}

bool SensorLoaderModule::pickSUTProduct(QString dest,int time_out)
{
    qInfo("pickSUTProduct dest %s time_out %d",dest.toStdString().c_str(),time_out);
    bool result = picker2SearchSutZ(parameters.pickProductZ(),dest,"vacuumOffReq",true,time_out);
    if(!result)
        AppendError(QString(u8"?SUT%1?????").arg(dest=="remote"?1:2));
    if(result)
        result &= checkPickedNgOrProduct(true);
    return result;
}

bool SensorLoaderModule::placeSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d",time_out);
    bool result = picker2SearchZ(parameters.placeNgSensorZ(),false,time_out);
    if(!result)
        AppendError(QString(u8"?Ngsensor??NG???"));
    return result;
}

bool SensorLoaderModule::placeProductToTray(int time_out)
{
    qInfo("placeProductToTray time_out %d",time_out);
    bool result = picker2SearchZ(parameters.placeProductZ(),false,time_out);
    if(!result)
        AppendError(QString(u8"??????????"));
    return result;
}

bool SensorLoaderModule::picker1MeasureHight(bool is_tray)
{
    qInfo("picker1MeasureHight is_tray %d",is_tray);
    if(pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))
    {
        QThread::msleep(100);
        if(!emit sendMsgSignal(tr(u8"??"),tr(u8"???????:%1").arg(pick_arm->GetSoftladngPosition()))){
            return true;
        }
        if(is_tray)
            parameters.setPickSensorZ(pick_arm->GetSoftladngPosition());
        else
            parameters.setPlaceSensorZ(pick_arm->GetSoftladngPosition());
        return true;
    }
    AppendError(QString(u8"1???????"));
    return false;
}

bool SensorLoaderModule::picker2MeasureHight(bool is_tray, bool is_product)
{
    qInfo("picker2MeasureHight is_tray %d is_product %d",is_tray,is_product);
    if(pick_arm->ZSerchByForce2(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))
    {
        QThread::msleep(100);
        if(!emit sendMsgSignal(tr(u8"??"),tr(u8"???????:%1").arg(pick_arm->GetSoftladngPosition2()))){
            return true;
        }
        if(is_tray)
        {
            if(is_product)
                parameters.setPlaceProductZ(pick_arm->GetSoftladngPosition2());
            else
                parameters.setPlaceNgSensorZ(pick_arm->GetSoftladngPosition2());
        }
        else
        {
            if(is_product)
                parameters.setPickProductZ(pick_arm->GetSoftladngPosition2());
            else
                parameters.setPickNgSensorZ(pick_arm->GetSoftladngPosition2());
        }
        return true;
    }
    AppendError(QString(u8"2???????"));
    return false;
}

bool SensorLoaderModule::measureZOffset()
{
    QPointF temp_point(sut1_pr_position.X()+picker1_offset.X(),sut2_pr_position.Y()+picker1_offset.Y());
    double sut1_height = 0;
    if(!pick_arm->move_XY_Synic(temp_point,true))
    {
        AppendError(QString(u8"??SUT??????"));
        return false;
    }
    if(!pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))
    {
        AppendError(QString(u8"??SUT??????"));
        return false;
    }
    sut1_height = pick_arm->GetSoftladngPosition();
    temp_point.setX(sut2_pr_position.X()+picker1_offset.X());
    temp_point.setY(sut2_pr_position.Y()+picker1_offset.Y());
    if(!pick_arm->move_XY_Synic(temp_point,true))
    {
        AppendError(QString(u8"??SUT??????"));
        return false;
    }
    if(!pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))

    {
        AppendError(QString(u8"??SUT??????"));
        return false;
    }
    double zOffset = pick_arm->GetSoftladngPosition()-sut1_height;
    if(!emit sendMsgSignal(tr(u8"??"),tr(u8"???????:%1").arg(zOffset))){
        return true;
    }
    parameters.setZOffset(pick_arm->GetSoftladngPosition() - sut1_height);
    return true;
}

bool SensorLoaderModule::moveToTrayPos(int index, int tray_index)
{
    qInfo("moveToTrayPos index %d tray_index %d",index,tray_index);
    bool result = pick_arm->move_XY_Synic(tray->getPositionByIndex(index,tray_index));
    if(!result)
        AppendError(QString(u8"???%1?%1?????").arg(tray_index == 0?"sensor":"??").arg(index));
    return result;
}

bool SensorLoaderModule::moveToTrayPos(int tray_index)
{
    qInfo("moveToTrayPos tray_index %d",tray_index);
    QPointF tem_pos = tray->getCurrentPosition(tray_index);
    bool result = pick_arm->move_XYT1_Synic(tem_pos.x(),tem_pos.y(),parameters.picker1ThetaOffset(),true);
    if(!result)
        AppendError(QString(u8"???%1???????").arg(tray_index == 0?u8"sensor":u8"??"));
    return result;
}

bool SensorLoaderModule::moveToStartPos(int tray_index)
{
    qInfo("moveToStartPos%d",tray_index);
    bool result = pick_arm->move_XY_Synic(tray->getStartPosition(tray_index),true);
    if(!result)
        AppendError(QString(u8"???%1???????").arg(tray_index == 0?"sensor":"??"));
    return result;
}

bool SensorLoaderModule::moveToTray1EndPos()
{
    qInfo("moveToTray1EndPos");
    bool result = pick_arm->move_XY_Synic(tray->getEndPosition(),true);
    if(!result)
        AppendError(QString(u8"???sensor???????"));
    return result;
}

double SensorLoaderModule::updateAccumulatedHour(bool calculate)
{
    if(calculate)
    {
        parameters.setAccumulatedHour(parameters.accumulatedHour() + getHourSpace(time_label));
        time_label = QTime::currentTime();
    }
    return parameters.accumulatedHour();
}

double SensorLoaderModule::getHourSpace(QTime time_label)
{
    int temp_minute = QTime::currentTime().minute() - time_label.minute();
    if(temp_minute < 0)temp_minute = 60 - temp_minute;
    double space = temp_minute/60.0;
    int temp_second =   QTime::currentTime().second() - time_label.second();
    space += temp_second/3600.0;
    return space;
}

void SensorLoaderModule::clearNumber()
{
    sensor_uph.clearNumber();
    product_uph.clearNumber();
    comprehensive_uph.clearNumber();
    left_sensor_uph.clearNumber();
    left_product_uph.clearNumber();
    left_comprehensive_uph.clearNumber();
    right_sensor_uph.clearNumber();
    right_product_uph.clearNumber();
    right_comprehensive_uph.clearNumber();
    parameters.setAccumulatedHour(0);
}

void SensorLoaderModule::sendEvent(const QString event)
{
    QJsonObject result;
    result.insert("event", event);
    emit sendMessageToClient(servingIP, getStringFromJsonObject(result));
}

void SensorLoaderModule::sendCmd(QString serving_ip, const QString cmd)
{
    QJsonObject result;
    result.insert("cmd", cmd);
    emit sendMessageToClient(serving_ip, getStringFromJsonObject(result));
}
