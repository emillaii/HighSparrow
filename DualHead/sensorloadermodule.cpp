#include "DualHead/sensorloadermodule.h"
#include "Utils/commonutils.h"
//#include "logicmanager.h"
#include "basemodulemanager.h"
#include <QMessageBox>
#include "Utils/commonutils.h"

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
        qInfo(u8"移动SPA到SUT1位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动"))){
            result = moveToSUTPRPos(false,true);
//        }
    }
    else if(cmd%temp_value == HandlePosition::SUT_POS2){
        qInfo(u8"移动SPA到SUT2位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动"))){
            result = moveToSUTPRPos(true,true);
//        }
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY1){
        qInfo(u8"移动SPA到sensor料盘当前位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动"))){
            result = moveToTrayPos(0);
//        }
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY2){
        qInfo(u8"移动SPA到成品料盘当前位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动"))){
            result = moveToTrayPos(1);
//        }
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY1_START_POS){
        qInfo(u8"移动SPA到sensor料盘起始位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动"))){
            result = moveToStartPos(0);
//        }
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY2_START_POS){
        qInfo(u8"移动SPA到成品料盘起始位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动"))){
            result = moveToStartPos(1);
//        }
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY1_END_POS){
        qInfo(u8"移动SPA到sensor料盘起始位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动"))){
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
        qInfo(u8"重置PR结果");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            pr_offset.ReSet();
//        }
    }
    else if(cmd%temp_value == HandlePR::SENSOR_PR){
        qInfo(u8"执行sensorPR");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = performSensorPR();
//        }
    }
    else if(cmd%temp_value == HandlePR::VACANCY_PR){
        qInfo(u8"执行料盘空位PR");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = performVacancyPR();
//        }
    }
    else if(cmd%temp_value == HandlePR::SUT_PR){
        qInfo(u8"执行SUT定位PR");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = performSUTPR();
//        }
    }
    else if(cmd%temp_value == HandlePR::NG_SENSOR_PR){
        qInfo(u8"执行NG sensor PR");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = performSUTSensorPR();
//        }
    }
    else if(cmd%temp_value == HandlePR::PRODUCT_PR){
        qInfo(u8"执行成品PR");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = performSUTProductPR();
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
    if(cmd%temp_value == HandleToWorkPos::TO_PICK1){
        qInfo(u8"移动吸头1到视觉位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动"))){
            result = moveToWorkPos();
//        }
    }
    else if(cmd%temp_value == HandleToWorkPos::TO_PICK2){
        qInfo(u8"移动吸头2到视觉位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动"))){
            result = moveToWorkPos2();
//        }
    }
    else if(cmd%temp_value == HandleToWorkPos::TO_PR_OFFSET){
        qInfo(u8"移动LPA到当前视觉结果位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动"))){
            result = moveToWorkPos2();
//        }
    }
    if(!result)
    {
        //        finished_type = FinishedType::Alarm;
        qInfo("Move To Work Pos fail");
        return;
    }
    cmd =cmd/temp_value*temp_value;
    temp_value = 100000;
    qInfo("cmd : %d", cmd);
    if(cmd%temp_value == handlePickerAction::PICK_SENSOR_FROM_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickTraySensor();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_SENSOR_TO_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeSensorToSUT("remote");
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_SENSOR_TO_SUT2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeSensorToSUT("::1");
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_NG_SENSOR_FROM_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickSUTSensor("remote");
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_NG_SENSOR_FROM_SUT2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickSUTSensor("::1");
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_NG_SENSOR_TO_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeSensorToTray();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_PRODUCT_FROM_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickSUTProduct("remote");
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_PRODUCT_FROM_SUT2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickSUTProduct("::1");
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_PRODUCT_TO_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeProductToTray();
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_SENSOR_IN_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker1MeasureHight(true);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_SENSOR_IN_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker1MeasureHight(false);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_NG_SENSOR_IN_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(false,false);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_NG_SENSOR_IN_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(true,false);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_PRODUCT_IN_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(false,true);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_PRODUCT_IN_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(true,true);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_Z_OFFSET)
        result = measureZOffset();
    else
        result = true;
    if(!result)
    {
        //        finished_type = FinishedType::Alarm;
    return;
}
    //    finished_type = FinishedType::Success;
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
    states.setHasTray(false);
    states.setSutHasSensor(false);
    states.setSutHasNgSensor(false);
    states.setSutHasProduct(false);
    states.setNeedLoadSensor(false);
    states.setNeedChangTray(false);
    states.setAllowChangeTray(false);
    states.setHasPickedSensor(false);
    states.setHasPickedProduct(false);
    states.setHasPickedNgSensor(false);
    states.setBeExchangeMaterial(false);
    states.setCmd("");
    states.setWaitingChangeTray(false);
    states.setFinishChangeTray(false);
    requestQueue.clear();
    actionQueue.clear();
    tray->resetTrayState();
    tray->resetTrayState(1);
}

void SensorLoaderModule::openServer(int port)
{
    server = new SparrowQServer(port);
    connect(server, &SparrowQServer::receiveRequestMessage, this, &SensorLoaderModule::receiveRequestMessage, Qt::DirectConnection);
    connect(this, &SensorLoaderModule::sendMessageToClient, this->server, &SparrowQServer::sendMessageToClient);
}

void SensorLoaderModule::performHandling(int cmd)
{
    emit sendHandlingOperation(cmd);
    qInfo("emit performHandling %d",cmd);
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
    //逻辑状态与实际状态检测

    int sensor_tray_index = 0;
    int product_tray_index = 1;
    is_run = true;
    int pr_times = 5;
    bool has_task = true;
    int change_tray_time_out = parameters.changeTrayTimeOut();

    bool waiting_change_tray = false;
    bool finish_change_tray = false;
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
        //放成品
        if((!states.allowChangeTray())&&states.hasTray()&&states.hasPickedProduct())
        {
            has_task = true;
            if(!moveToNextTrayPos(product_tray_index))
            {
                AppendError(u8"moveToTrayPos fail");
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!performVacancyPR())&&has_material)
            {
                AppendError(u8"空盘视觉失败！");
                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                int result = waitMessageReturn(is_run);
                if(!result)is_run = false;
                if(!is_run)break;
                if(result)
                    continue;
            }
            if(!moveToWorkPos2())
            {
                AppendError(u8"moveToWorkPos fail");
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!placeProductToTray())&&has_material)
            {
                AppendError(u8"placeProductToTray fail");
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                waitMessageReturn(is_run);
            }
            tray->setCurrentMaterialState(MaterialState::IsProduct,product_tray_index);
            states.setHasPickedProduct(false);
            if(!is_run)break;
        }
        //放NGSensor
        if((!states.allowChangeTray())&&states.hasTray()&&states.hasPickedNgSensor())
        {
            has_task = true;
            if(!moveToNextTrayPos(product_tray_index))
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!performVacancyPR())&&has_material)
            {
                AppendError(u8"空盘视觉失败！");
                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                int result = waitMessageReturn(is_run);
                if(!result)is_run = false;
                if(!is_run)break;
                if(result)
                    continue;
            }
            if(!moveToWorkPos2())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!placeSensorToTray())&&has_material)
            {
                AppendError(u8"placeProductToTray fail");
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                waitMessageReturn(is_run);
            }
            tray->setCurrentMaterialState(MaterialState::IsNg,product_tray_index);
            states.setHasPickedNgSensor(false);
            if(!is_run)break;
        }
        //取料
        if((!finish_stop)&&(!states.allowChangeTray())&&states.hasTray()&&(!states.hasPickedSensor())&&(!states.hasPickedProduct())&&(!states.hasPickedNgSensor()))
        {
            has_task = true;
            if(!moveToNextTrayPos(sensor_tray_index))
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
                    tray->setCurrentMaterialState(MaterialState::IsEmpty,sensor_tray_index);
                    picked_material = tray->getCurrentIndex(sensor_tray_index);
                    continue;
                    }
                else
                {
                    pr_times = 5;
                    AppendError(u8"pr连续失败五次");
                    sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                    waitMessageReturn(is_run);
                    if(is_run)break;
                }
            }
            if(!moveToWorkPos())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
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
        //检测是否需要换盘
        if((!states.allowChangeTray()))
        {
            if((!states.hasTray())||checkTrayNeedChange())
                states.setAllowChangeTray(true);
        }
        //等待位置
        if(!moveToSUTPRPos(states.beExchangeMaterial()?isLocalHost:(!isLocalHost)))
        {
            sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
            is_run = false;
            break;
        }
        //执行换盘
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
                        AppendError(QString(u8"等待换盘超时，超时时间%d,请选择是继续等待或者重新换盘").arg(parameters.changeTrayTimeOut()));
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
        //sut操作
        if (requestQueue.size()>0 && (!states.beExchangeMaterial())&&states.hasPickedSensor()&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct()))
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
                actionQueue.clear();
                states.setBeExchangeMaterial(true);
                sendEvent("sensorResp");
            }
        }

        if (actionQueue.size()>0 && states.beExchangeMaterial())
        {
            has_task = true;
            while(states.beExchangeMaterial())
            {
                if(actionQueue.size()<=0)
                {
                    QThread::msleep(100);
                    if(is_run)
                        continue;
                    else
                        break;
                }
                if(states.cmd() == "")
                {
                    QJsonObject obj = actionQueue.dequeue();
                    qInfo("Start to consume action request: %s", getStringFromJsonObject(obj).toStdString().c_str());
                    states.setCmd(obj["cmd"].toString(""));
                }

                if((states.cmd() == "unloadProductReq")&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct()))
                {

                    if(!moveToSUTPRPos(isLocalHost))
                    {
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if((!performSUTProductPR())&&has_material)
                    {
                        AppendError(u8"成品视觉失败！");
                        sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                        int result = waitMessageReturn(is_run);
                        if(!result)is_run = false;
                        if(!is_run)break;
                        if(result)
                            continue;
                    }
                    if(!moveToWorkPos2())
                    {
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if((!pickSUTProduct(isLocalHost?"::1":"remote"))&&has_material)
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                        if(waitMessageReturn(is_run))
                            states.setSutHasProduct(false);
                        else
                        {
                            states.setSutHasProduct(false);
                            states.setHasPickedProduct(true);
                        }
                    }
                    else
                    {
                        states.setSutHasProduct(false);
                        states.setHasPickedProduct(true);
                    }
                    sendEvent("unloadProductResp");
                    states.setCmd("");
                }
                else if((states.cmd() == "unloadNgSensorReq")&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct()))
                {
                    //取NG sensor
                    if(!moveToSUTPRPos(isLocalHost))
                    {
                        AppendError("moveToSUTPRPos fail!");
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if((!performSUTSensorPR())&&has_material)
                    {
                        AppendError(u8"NG视觉失败！");
                        sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                        int result = waitMessageReturn(is_run);
                        if(!result)is_run = false;
                        if(!is_run)break;
                        if(result)
                            continue;
                    }
                    if(!moveToWorkPos2())
                    {
                        AppendError("moveToWorkPos2 fail!");
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if((!pickSUTSensor(isLocalHost?"::1":"remote"))&&has_material)
                    {
                        AppendError("pickSUTSensor fail!");
                        sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                        if(waitMessageReturn(is_run))
                            states.setSutHasNgSensor(false);
                        else
                        {
                            states.setSutHasNgSensor(false);
                            states.setHasPickedNgSensor(true);
                        }
                    }
                    else
                    {
                        states.setSutHasNgSensor(false);
                        states.setHasPickedNgSensor(true);
                    }
                    sendEvent("unloadNgSensorResp");
                    states.setCmd("");
                }
                else if ((states.cmd() == "loadSensorReq")&&states.hasPickedSensor())
                {
                    //放料到SUT
                    if(!moveToSUTPRPos(isLocalHost))
                    {
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if(!moveToWorkPos())
                    {
                        AppendError("moveToWorkPos fail!");
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
                        else
                        {
                            states.setNeedLoadSensor(false);
                            states.setHasPickedSensor(false);
                            sendEvent("loadSensorResp");
                            states.setCmd("");
                            states.setBeExchangeMaterial(false);
                        }
                    }
                    else
                    {
                        states.setNeedLoadSensor(false);
                        states.setHasPickedSensor(false);
                        sendEvent("loadSensorResp");
                        states.setCmd("");
                        states.setBeExchangeMaterial(false);
                    }
                }
                else if((((states.cmd() == "unloadProductReq")||(states.cmd() == "unloadNgSensorReq"))&&(states.hasPickedNgSensor()||states.hasPickedProduct()))
                        ||((states.cmd() == "loadSensorReq")&&(!states.hasPickedSensor())))
                {
                    break;
                }
            }
            }

        //        //放料到SUT
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
//        //取成品
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
//                AppendError(u8"NG视觉失败！");
//                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
//                int result = waitMessageReturn(is_run);
//                if(!result)is_run = false;
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
//        //取NGsensor
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
//                AppendError(u8"NG视觉失败！");
//                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
//                int result = waitMessageReturn(is_run);
//                if(!result)is_run = false;
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
    qInfo("LensLoader stoped");
}

bool SensorLoaderModule::checkTrayNeedChange()
{
    if(tray->isTrayNeedChange(0)||tray->isTrayNeedChange(1))
        return true;
    return false;
}

bool SensorLoaderModule::moveToNextTrayPos(int tray_index)
{
    qInfo("moveToNextTrayPos tray_index %d",tray_index);
    bool result = tray->findNextPositionOfInitState(tray_index);
    if(result)
        result &=  pick_arm->move_XY_Synic(tray->getCurrentPosition(tray_index));
    if(!result)
        AppendError(QString(u8"移动到%1盘下一个位置失败").arg(tray_index == 0?"sensor":"成品"));
    return result;
}

bool SensorLoaderModule::moveToSUTPRPos(bool is_local,bool check_softlanding)
{
    qInfo("moveToSUTPRPos is_local %d",is_local);
    bool result;
    if(is_local)
        result =  pick_arm->move_XY_Synic(sut2_pr_position.ToPointF(),check_softlanding);
    else
        result =  pick_arm->move_XY_Synic(sut1_pr_position.ToPointF(),check_softlanding);
    if(!result)
        AppendError(QString("移动SPA到SUT位置失败%1").arg(is_local));
    return result;
}

bool SensorLoaderModule::performSensorPR()
{
    qInfo("performSensorPR");
    bool result = sensor_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行料盘sensor视觉失败!"));
    return  result;
}

bool SensorLoaderModule::performVacancyPR()
{
    qInfo("performVacancyPR");
    bool result = vacancy_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行料盘空位视觉失败!"));
    return  result;
}

bool SensorLoaderModule::performSUTPR()
{
    qInfo("performSUTPR");
    bool result = sut_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行SUT视觉失败!"));
    return  result;
}

bool SensorLoaderModule::performSUTSensorPR()
{
    qInfo("performSUTSensorPR");
    bool result = sut_sensor_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行SUT上的sensor视觉失败!"));
    return  result;
}

bool SensorLoaderModule::performSUTProductPR()
{
    qInfo("performSUTProductPR");
    bool result =  sut_product_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行SUT视觉失败!"));
    return  result;
}

bool SensorLoaderModule::moveToWorkPos(bool check_softlanding)
{
    PrOffset temp(picker1_offset.X() - pr_offset.X,picker1_offset.Y() - pr_offset.Y,pr_offset.Theta);
    qInfo("moveToWorkPos offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XYT1_Synic(temp.X,temp.Y,temp.Theta,check_softlanding);
    if(result)
        AppendError(QString(u8"去1号吸头工作位置(step x %1,y %2,t %3)失败!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    return  result;
}

bool SensorLoaderModule::moveToWorkPos2(bool check_softlanding)
{
    PrOffset temp(picker2_offset.X() - pr_offset.X,picker2_offset.Y() - pr_offset.Y,pr_offset.Theta);
    qInfo("moveToWorkPos2 offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XYT2_Synic(temp.X,temp.Y,temp.Theta,check_softlanding);
    if(result)
        AppendError(QString(u8"去2号吸头工作作位置(step x %1,y %2,t %3)失败!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    return  result;
}

bool SensorLoaderModule::moveToPRResultPos(bool check_softlanding)
{
    PrOffset temp(picker2_offset.X() - pr_offset.X,picker2_offset.Y() - pr_offset.Y,pr_offset.Theta);
    qInfo("moveToPRResultPos offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XY_Synic(temp.X,temp.Y,check_softlanding);
    if(result)
        AppendError(QString(u8"去视觉结果位置(step x %1,y %2,t %3)失败!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
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
        if(result)
        {
            sendCmd(dest,cmd);
            QThread::msleep(200);
        }
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
        if(result)
        {
            sendCmd(dest,cmd);
            QThread::msleep(200);
        }
        result &= pick_arm->ZSerchReturn2(time_out);
    }
    result &= pick_arm->move_XeYe_Z2(0,parameters.escapeX(),parameters.escapeY());
    return result;
}

bool SensorLoaderModule::pickTraySensor(int time_out)
{
    qInfo("pickTraySensor time_out %d",time_out);
    bool result = picker1SearchZ(parameters.pickSensorZ(),true,time_out);
    if(!result)
        AppendError(QString(u8"从sensor盘取sensor失败"));
    return result;
}

bool SensorLoaderModule::placeSensorToSUT(QString dest,int time_out)
{
    qInfo("placeSensorToSUT dest %s time_out %d",dest.toStdString().c_str(),time_out);
   bool result = picker1SearchSutZ(parameters.placeSensorZ(),dest,"vacuumOnReq",false,time_out);
    if(!result)
        AppendError(QString(u8"放sensor到SUT%1失败").arg(dest=="remote"?1:2));
    return result;
}

bool SensorLoaderModule::pickSUTSensor(QString dest,int time_out)
{
    qInfo("pickSUTSensor dest %s time_out %d",dest.toStdString().c_str(),time_out);
    bool result = picker2SearchSutZ(parameters.pickNgSensorZ(),dest,"vacuumOffReq",true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT%1取NGsenor失败").arg(dest=="remote"?1:2));
    return result;
}

bool SensorLoaderModule::pickSUTProduct(QString dest,int time_out)
{
    qInfo("pickSUTProduct dest %s time_out %d",dest.toStdString().c_str(),time_out);
    bool result = picker2SearchSutZ(parameters.pickProductZ(),dest,"vacuumOffReq",true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT%1取成品失败").arg(dest=="remote"?1:2));
    return result;
}

bool SensorLoaderModule::placeSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d",time_out);
    bool result = picker2SearchZ(parameters.placeNgSensorZ(),false,time_out);
    if(!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    return result;
}

bool SensorLoaderModule::placeProductToTray(int time_out)
{
    qInfo("placeProductToTray time_out %d",time_out);
    bool result = picker2SearchZ(parameters.placeProductZ(),false,time_out);
    if(!result)
        AppendError(QString(u8"将成品放入成品盘失败"));
    return result;
}

bool SensorLoaderModule::picker1MeasureHight(bool is_tray)
{
    qInfo("picker1MeasureHight is_tray %d",is_tray);
    if(pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))
    {
        QThread::msleep(100);
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否应用此高度:%1").arg(pick_arm->GetSoftladngPosition()))){
            return true;
        }
        if(is_tray)
            parameters.setPickSensorZ(pick_arm->GetSoftladngPosition());
        else
            parameters.setPlaceSensorZ(pick_arm->GetSoftladngPosition());
        return true;
    }
    AppendError(QString(u8"1号吸头测高失败"));
    return false;
}

bool SensorLoaderModule::picker2MeasureHight(bool is_tray, bool is_product)
{
    qInfo("picker2MeasureHight is_tray %d is_product %d",is_tray,is_product);
    if(pick_arm->ZSerchByForce2(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))
    {
        QThread::msleep(100);
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否应用此高度:%1").arg(pick_arm->GetSoftladngPosition2()))){
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
    AppendError(QString(u8"2号吸头测高失败"));
    return false;
}

bool SensorLoaderModule::measureZOffset()
{
    QPointF temp_point(sut1_pr_position.X()+picker1_offset.X(),sut1_pr_position.Y()+picker1_offset.Y());
    double sut1_height = 0;
    if(!pick_arm->move_XY_Synic(temp_point,true))
    {
        AppendError(QString(u8"测量SUT台高度差失败"));
        return false;
    }
    if(!pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))
    {
        AppendError(QString(u8"测量SUT台高度差失败"));
        return false;
    }
    sut1_height = pick_arm->GetSoftladngPosition();
    temp_point.setX(sut2_pr_position.X()+picker1_offset.X());
    temp_point.setY(sut2_pr_position.Y()+picker1_offset.Y());
    if(!pick_arm->move_XY_Synic(temp_point,true))
    {
        AppendError(QString(u8"测量SUT台高度差失败"));
        return false;
    }
    if(!pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))

    {
        AppendError(QString(u8"测量SUT台高度差失败"));
        return false;
    }
    double zOffset = pick_arm->GetSoftladngPosition()-sut1_height;
    if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否应用此数值:%1").arg(zOffset))){
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
        AppendError(QString(u8"移动到%1盘%1号位置失败").arg(tray_index == 0?"sensor":"成品").arg(index));
    return result;
}

bool SensorLoaderModule::moveToTrayPos(int tray_index)
{
    qInfo("moveToTrayPos tray_index %d",tray_index);
    bool result = pick_arm->move_XY_Synic(tray->getCurrentPosition(tray_index),true);
    if(!result)
        AppendError(QString(u8"移动到%1盘当前位置失败").arg(tray_index == 0?"sensor":"成品"));
    return result;
}

bool SensorLoaderModule::moveToStartPos(int tray_index)
{
    qInfo("moveToStartPos%d",tray_index);
    bool result = pick_arm->move_XY_Synic(tray->getStartPosition(tray_index),true);
    if(!result)
        AppendError(QString(u8"移动到%1盘起始位置失败").arg(tray_index == 0?"sensor":"成品"));
    return result;
}

bool SensorLoaderModule::moveToTray1EndPos()
{
    qInfo("moveToTray1EndPos");
    bool result = pick_arm->move_XY_Synic(tray->getEndPosition(),true);
    if(!result)
        AppendError(QString(u8"移动到sensor盘结束位置失败"));
    return result;
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
