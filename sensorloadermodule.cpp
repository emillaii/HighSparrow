#include "sensorloadermodule.h"
#include "utils/commonutils.h"
//#include "logicmanager.h"
#include "basemodulemanager.h"
#include <QMessageBox>
#include "utils/commonutils.h"
#define PI 3.1415926535898
SensorLoaderModule::SensorLoaderModule():ThreadWorkerBase ("SensorLoaderModule")
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
    temp_map.insert("spa_standby_position", &spa_standby_position);
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
    temp_map.insert("spa_standby_position", &spa_standby_position);
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
            moveToSUTPRPos(false,true,true);
            QThread::msleep(2000);
            moveToSUTPRPos(true,true,true);
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
        result = moveToSUTPRPos(false,true,true);
    }
    else if(cmd%temp_value == HandlePosition::SUT_POS2){
        qInfo(u8"移动SPA到SUT2位置");
        result = moveToSUTPRPos(true,true,true);
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY1){
        qInfo(u8"移动SPA到sensor料盘当前位置");
        result = moveToTrayPos(0);
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY2){
        qInfo(u8"移动SPA到成品料盘当前位置");
        result = moveToTrayPos(1);
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY1_START_POS){
        qInfo(u8"移动SPA到sensor料盘起始位置");
        result = moveToStartPos(0);
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY2_START_POS){
        qInfo(u8"移动SPA到成品料盘起始位置");
        result = moveToStartPos(1);
    }
    else if(cmd%temp_value == HandlePosition::SENSOR_TRAY1_END_POS){
        qInfo(u8"移动SPA到sensor料盘起始位置");
        result = moveToTray1EndPos();
    }
    else if(cmd%temp_value == HandlePosition::SPA_STANDBY_POS){
        qInfo(u8"移动SPA到standby位置");
        result = moveToStandbyPos(true,true);
    }
    else
        result =true;
    cmd =cmd/temp_value*temp_value;
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        is_handling = false;
        return;
    }
    temp_value = 100;
    if(cmd%temp_value == HandlePR::RESET_PR){
        qInfo(u8"重置PR结果");
        pr_offset.ReSet();
    }
    else if(cmd%temp_value == HandlePR::SENSOR_PR){
        qInfo(u8"执行sensorPR");
        result = performSensorPR();
        if(result)
        {
            getPicker1SensorOffset();
            getPicker2SensorOffset();
        }
    }
    else if(cmd%temp_value == HandlePR::VACANCY_PR){
        qInfo(u8"执行料盘空位PR");
        result = performVacancyPR();
    }
    else if(cmd%temp_value == HandlePR::SUT_PR){
        qInfo(u8"执行SUT定位PR");
        result = performSUTPR();
    }
    else if(cmd%temp_value == HandlePR::NG_SENSOR_PR){
        qInfo(u8"执行NG sensor PR");
        result = performSUTSensorPR();
        if(result)
        {
            getPicker2SensorOffset();
        }
    }
    else if(cmd%temp_value == HandlePR::PRODUCT_PR){
        qInfo(u8"执行成品PR");
        sut_product_vision->OpenLight();
        result = performSUTProductPR();
        if(result)
        {
            getPicker2SensorOffset();
        }
    }
    else
        result = true;
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        is_handling = false;
        return;
    }
    cmd =cmd/temp_value*temp_value;
    temp_value = 1000;
    if(cmd%temp_value == HandleToWorkPos::TO_PICK_SENSOR_OFFSET){
        qInfo(u8"移动吸头1到视觉位置");
            result = moveToWorkPos(false,true);
    }
    else if(cmd%temp_value == HandleToWorkPos::TO_PICK_PRODUCT_OFFSET){
        qInfo(u8"移动吸头2到视觉位置");
        if(pick_arm->motor_x->GetFeedbackPos()>400)
            result = movePicker2ToSUTPos(true,true,true);
        else
            result = movePicker2ToSUTPos(false,true,true);
    }
    if(cmd%temp_value == HandleToWorkPos::TO_PLACE_SENSOR_OFFSET){
        qInfo(u8"移动吸头1到视觉位置");
        if(pick_arm->motor_x->GetFeedbackPos()>400)
            result = movePicker1ToSUTPos(true,true,true);
        else
            result = movePicker1ToSUTPos(false,true,true);
    }
    else if(cmd%temp_value == HandleToWorkPos::TO_PLACE_PRODUCT_OFFSET){
        qInfo(u8"移动吸头2到视觉位置");
        result = movePicker2ToTrayPos(1);
    }
    else if(cmd%temp_value == HandleToWorkPos::TO_PR_OFFSET){
        qInfo(u8"移动SA到当前视觉结果位置");
        result = moveToPRResultPos(true);
    }
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        is_handling = false;
        return;
    }
    cmd =cmd/temp_value*temp_value;
    temp_value = 100000;
    qInfo("cmd : %d", cmd);
    if(cmd%temp_value == handlePickerAction::PICK_SENSOR_FROM_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickTraySensor();
            setPicker1SensorOffset();
            setPicker2SensorOffset();
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_SENSOR_TO_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeSensorToSUT("remote");
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_SENSOR_TO_SUT2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeSensorToSUT("::1");
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_NG_SENSOR_FROM_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickSUTSensor("remote");
            if(fabs(pr_offset.Theta)>0.01)
                setPicker2SensorOffset();
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_NG_SENSOR_FROM_SUT2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickSUTSensor("::1");
            if(fabs(pr_offset.Theta)>0.01)
                setPicker2SensorOffset();
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_NG_SENSOR_TO_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeSensorToTray();
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_PRODUCT_FROM_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickSUTProduct("remote");
            if(fabs(pr_offset.Theta)>0.01)
                setPicker2SensorOffset();
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_PRODUCT_FROM_SUT2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickSUTProduct("::1");
            if(fabs(pr_offset.Theta)>0.01)
                setPicker2SensorOffset();
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_PRODUCT_TO_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeProductToTray();
            pr_offset.ReSet();
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
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        is_handling = false;
        return;
    }
    is_handling = false;
}

void SensorLoaderModule::receiveRequestMessage(QString message, QString client_ip)
{
    qInfo("Sut Module receive command:%s from ip: %s", message.toStdString().c_str(), client_ip.toStdString().c_str());
    QJsonObject obj = getJsonObjectFromString(message);
    QString cmd = obj["cmd"].toString("");
    obj.insert("client_ip",client_ip);
    if (cmd == "sensorReq") {
        qInfo("Enqueue the sensor request command in request quene");
        QMutexLocker locker(&tcp_mutex);
        requestQueue.enqueue(obj);
    }
    else if (cmd.length() > 0)
    {
        qInfo("Enqueue the %s in action queue", cmd.toStdString().c_str());
        QMutexLocker locker(&tcp_mutex);
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

void SensorLoaderModule::receivceModuleMessage(QVariantMap message)
{
    qInfo("receive module message %s",message["Message"].toString().toStdString().c_str());
    QMutexLocker temp_locker(&message_mutex);
    this->message = message;
    if(message.contains("OriginModule")&&message["OriginModule"].toString()=="AA1CoreNew")
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString()=="NoSensor")
            {
                states.setSutHasSensor(false);
                states.setSutHasNgSensor(false);
                states.setSutHasProduct(false);
                states.setSutHasNgProduct(false);
            }
            if(message["Message"].toString()=="NgSensor")
            {
                states.setSutHasSensor(false);
                states.setSutHasNgSensor(true);
                states.setSutHasProduct(false);
                states.setSutHasNgProduct(false);
            }
            else if(message["Message"].toString()=="NgProduct")
            {
                states.setSutHasSensor(false);
                states.setSutHasNgSensor(false);
                states.setSutHasProduct(false);
                states.setSutHasNgProduct(true);
            }
            else if(message["Message"].toString()=="GoodProduct")
            {
                states.setSutHasSensor(false);
                states.setSutHasNgSensor(false);
                states.setSutHasProduct(true);
                states.setSutHasNgProduct(false);
            }
        }
    }
    else if(message.contains("OriginModule")&&message["OriginModule"].toString()=="AA2CoreNew")
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString()=="Sensor")
            {
                states.setSut2HasSensor(true);
                states.setSut2HasNgSensor(false);
                states.setSut2HasProduct(false);
                states.setSut2HasNgProduct(false);
            }
            if(message["Message"].toString()=="NgSensor")
            {
                states.setSut2HasSensor(false);
                states.setSut2HasNgSensor(true);
                states.setSut2HasProduct(false);
                states.setSut2HasNgProduct(false);
            }
            else if(message["Message"].toString()=="NgProduct")
            {
                states.setSut2HasSensor(false);
                states.setSut2HasNgSensor(false);
                states.setSut2HasProduct(false);
                states.setSut2HasNgProduct(true);
            }
            else if(message["Message"].toString()=="GoodProduct")
            {
                states.setSut2HasSensor(false);
                states.setSut2HasNgSensor(false);
                states.setSut2HasProduct(true);
                states.setSut2HasNgProduct(false);
            }
        }
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
    states.setHasUnpickedProduct(false);
    states.setHasUnpickedNgProduct(false);
    states.setHasUnpickedNgSensor(false);

    requestQueue.clear();
    actionQueue.clear();
    tray->resetTrayState();
    tray->resetTrayState(1);
    tray->resetTrayState(2);
    states.reset();
    qInfo("resetLogic");
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
            bool result = pick_arm->Z1SerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),15,parameters.vcmMargin(),parameters.finishDelay(),true,true,30000);
            QThread::msleep(1000);
            result &= pick_arm->ZSerchReturn(30000);
            this->pick_arm->stepMove_XYT1_Synic(0, 0, 20);
            result = pick_arm->Z1SerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),15,parameters.vcmMargin(),parameters.finishDelay(),false,true,30000);
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
            bool result = pick_arm->Z2SerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),15,parameters.vcmMargin(),parameters.finishDelay(),true,true,30000);
            QThread::msleep(1000);
            result &= pick_arm->ZSerchReturn2(30000);
            this->pick_arm->stepMove_XYT2_Synic(0, 0, 20);
            result = pick_arm->Z2SerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),15,parameters.vcmMargin(),parameters.finishDelay(),false,true,30000);
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
    int ng_tray_index = 2;
    is_run = true;
    finish_stop = false;
    int pr_times = parameters.autoPrTime();
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
//        if(!has_task)
            QThread::msleep(1);
        has_task = false;
        //放成品
        if((!states.allowChangeTray())&&states.hasTray()&&states.hasPickedProduct())
        {
            has_task = true;
            if(parameters.enableProdcutTrayPr())
            {
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
                        tray->setCurrentMaterialState(MaterialState::IsProduct,product_tray_index);
                        continue;
                    }
                    if(!is_run)break;
                }
                vacancy_vision->CloseLight();
            }
            else
            {
                pr_offset.ReSet();
            }
            if(!movePicker2ToTrayPos(product_tray_index))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
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
        }
        //放Ng成品
        if((!states.allowChangeTray())&&states.hasTray()&&states.hasPickedNgProduct())
        {
            has_task = true;
            if(parameters.enableNgProductTrayPr())
            {
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
                        tray->setCurrentMaterialState(MaterialState::IsNgProduct,ng_tray_index);
                        continue;
                    }
                    if(!is_run)break;
                }
                vacancy_vision->CloseLight();
            }
            else
            {
                pr_offset.ReSet();
            }
            if(!movePicker2ToTrayPos(ng_tray_index))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
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
        //放NGSensor
        if((!states.allowChangeTray())&&states.hasTray()&&states.hasPickedNgSensor())
        {
            has_task = true;
            if(parameters.enableNgSensorTrayPr())
            {
                vacancy_vision->OpenLight();
                if(!moveToNgTrayNextPos())
                {
                    AppendError("请更换Ng盘后继续！");
                    sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                    waitMessageReturn(is_run);
                    tray->resetTrayState(2);
                }
                if((!performVacancyPR())&&has_material)
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                    if(waitMessageReturn(is_run))
                    {
                        tray->setCurrentMaterialState(MaterialState::IsNg,ng_tray_index);
                        continue;
                    }
                    if(!is_run)break;
                }
                vacancy_vision->CloseLight();
            }
            else
            {
                pr_offset.ReSet();
            }
            if(!movePicker2ToTrayPos(ng_tray_index))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
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
        //取料
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

                    AppendError(u8"自动重试.");
                    sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
                    continue;
                }
                else
                {
                    pr_times = parameters.autoPrTime();
                    AppendError(u8"pr连续失败五次");
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
            getPicker1SensorOffset();
            getPicker2SensorOffset();
            pr_times = parameters.autoPrTime();
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
            setPicker1SensorOffset();
            setPicker2SensorOffset();
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
        //sut操作
        if ((!states.allowChangeTray())&&(!requestQueue.isEmpty()) && (!states.beExchangeMaterial())&&states.hasPickedSensor()&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct())&&(!states.hasPickedNgProduct()))
        {
            has_task = true;
            QMutexLocker locker(&tcp_mutex);
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

        //等待位置

        if(states.allowChangeTray())
        {
            if(!moveToStandbyPos(true))
            {
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                is_run = false;
                break;
            }
        }
        else if(states.beExchangeMaterial()?isLocalHost:(!isLocalHost))
        {
            if(states.hasPickedSensor()&&(!states.sut2HasNgProduct())&&(!states.sut2HasProduct())&&(!states.sut2HasNgSensor()))
            {
                if(!movePicker1ToSUTPos(true,true))
                {
                    sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                    waitMessageReturn(is_run);
                    if(!is_run)break;
                }
            }
            else if((states.sut2HasNgProduct()&&(!parameters.enableNgProductPr()))||
                    (states.sut2HasProduct()&&(!parameters.enableProductPr()))||
                    (states.sut2HasNgSensor()&&(!parameters.enableNgSensorPr())))
            {
                if(!movePicker2ToSUTPos(true,true))
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                    waitMessageReturn(is_run);
                    if(!is_run)break;
                }
            }
            else
            {
                if((states.sut2HasNgProduct()&&parameters.enableNgProductPr())||(states.sut2HasProduct()&&parameters.enableProductPr()))
                {
                    sut_product_vision->OpenLight();
                }
                else if((states.sut2HasNgSensor()&&parameters.enableNgSensorPr()))
                {
                    sut_sensor_vision->OpenLight();
                }
                if(!moveToSUTPRPos(true,true))
                {
                    sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                    is_run = false;
                    break;
                }
            }
        }
        else
        {
            if(!(states.hasPickedSensor()||states.sutHasNgProduct()||states.sutHasProduct()||states.sutHasNgSensor()))
            {
                if(!movePicker1ToSUTPos(false,true))
                {
                    sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                    waitMessageReturn(is_run);
                    if(!is_run)break;
                }
            }
            else if((states.sutHasNgProduct()&&(!parameters.enableNgProductPr()))||
                    (states.sutHasProduct()&&(!parameters.enableProductPr()))||
                    (states.sutHasNgSensor()&&(!parameters.enableNgSensorPr())))
            {
                if(!movePicker2ToSUTPos(false,true))
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                    waitMessageReturn(is_run);
                    if(!is_run)break;
                }
            }
            else
            {
                if(!moveToSUTPRPos(false,true))
                {
                    sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                    is_run = false;
                    break;
                }
            }
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
                    states.setHasTray(true);
                    tray->resetTrayState(0);
                    tray->resetTrayState(1);
                    states.setNeedChangTray(false);
                    states.setAllowChangeTray(false);
                    qInfo("finishChangeTray");
                }
                else
                {
//                    if(!has_task)
                        QThread::msleep(100);
                    qInfo("waitingChangeTray");
                    change_tray_time_out -=100;
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

        if ((!states.allowChangeTray())&& states.beExchangeMaterial())
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
                    QMutexLocker locker(&tcp_mutex);
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
                    if(parameters.enableProductPr())
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
                            AppendError(u8"成品视觉失败！");
                            sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                            int result = waitMessageReturn(is_run);
                            if(result)is_run = false;
                            if(!is_run)break;
                            if(result)
                                continue;
                        }
                        sut_product_vision->CloseLight();
                        getPicker2SensorOffset();
                    }
                    else
                    {
                        pr_offset.ReSet();
                    }
                    if(!movePicker2ToSUTPos(isLocalHost))
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            continue;
                        }
                        if(!is_run)break;
                    }
                    if((!pickSUTProduct(isLocalHost?"::1":"remote"))&&has_material)
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            continue;
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
                    setPicker2SensorOffset();
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
                    if(parameters.enableNgProductPr())
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
                            AppendError(u8"成品视觉失败！");
                            sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                            int result = waitMessageReturn(is_run);
                            if(result)is_run = false;
                            if(!is_run)break;
                            if(result)
                                continue;
                        }
                        sut_product_vision->CloseLight();
                        getPicker2SensorOffset();
                    }
                    else
                    {
                        pr_offset.ReSet();
                    }
                    if(!movePicker2ToSUTPos(isLocalHost))
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            continue;
                        }
                        if(!is_run)break;
                    }
                    if((!pickSUTProduct(isLocalHost?"::1":"remote"))&&has_material)
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            continue;
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
                    setPicker2SensorOffset();
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
                    //取NG sensor
                    if(parameters.enableNgSensorPr())
                    {
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
                            AppendError(u8"NG视觉失败！");
                            sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                            int result = waitMessageReturn(is_run);
                            if(result)
                                is_run = false;
                            else
                                continue;
                            if(!is_run)break;
                        }
                        sut_sensor_vision->CloseLight();
                        getPicker2SensorOffset();
                    }
                    else
                    {
                        pr_offset.ReSet();
                    }
                    if(!movePicker2ToSUTPos(isLocalHost))
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            continue;
                        }
                        if(!is_run)break;
                    }
                    if((!pickSUTSensor(isLocalHost?"::1":"remote"))&&has_material)
                    {
                        AppendError("pickSUTSensor fail!");
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
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
                    setPicker2SensorOffset();
                    sensor_uph.addCurrentReslutNumber(updateAccumulatedHour());
                    if(isLocalHost)
                        right_sensor_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    else
                        left_sensor_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                }
                else if ((states.cmd() == "loadSensorReq")&&states.hasPickedSensor())
                {
                    //放料到SUT
                    pr_offset.ReSet();
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
                        states.setSut2HasSensor(true);
                        right_sensor_uph.addCurrentNumber(updateAccumulatedHour(false));
                        right_comprehensive_uph.addCurrentNumber(updateAccumulatedHour(false));
                    }
                    else
                    {
                        states.setSutHasSensor(true);
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
    qInfo("sensor Loader Module stoped");
}

void SensorLoaderModule::runTest()
{
    int sensor_tray_index = 0;
    int ng_tray_index = 2;
    is_run = true;
    finish_stop = false;
    bool has_task = true;
    int current_time = parameters.repeatTime();
    int current_count = parameters.testSensorCount();
    bool finished = false;
    states.setHasTray(true);
    while (is_run)
    {
//        if(!has_task)
            QThread::msleep(100);
        has_task = false;
        //放Ng到成品盘
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
            if(parameters.staticTest())
                current_time = parameters.repeatTime();
            while (current_time>0)
            {
                if((!performVacancyPR()))
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                    if(waitMessageReturn(is_run))
                    {
                        continue;
                    }
                    if(!is_run)break;
                    if(!parameters.staticTest())
                        break;
                    current_time--;
                }
            }
            vacancy_vision->CloseLight();
            if(!movePicker2ToSUTPos(isLocalHost))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setHasPickedProduct(false);
                    continue;
                }
                if(!is_run)break;
            }
            if(!placeSensorToTray())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                    continue;
            }
            tray->setCurrentMaterialState(MaterialState::IsNg,ng_tray_index);
            states.setHasPickedNgSensor(false);
            if(!is_run)break;
            if(parameters.staticTest())
            {
                if(finish_stop)
                {
                    if(finished)
                    {
                        is_run = false;
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,"测试完成,自动停止！");
                        break;
                    }
                    else
                    {
                        finished = true;
                        current_count = parameters.testSensorCount();
                        tray->resetTrayState(0);
                    }
                    finish_stop = false;
                }
            }
            else
            {
                if(finish_stop)
                    current_time--;
                if(current_time <= 0)
                {
                    if(finished)
                    {
                        is_run = false;
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,"测试完成,自动停止！");
                        break;
                    }
                    else
                    {
                        finished = true;
                        current_time = parameters.repeatTime();
                        current_count = parameters.testSensorCount();
                        tray->resetTrayState(0);
                    }
                }
                else
                {
                    current_count = parameters.testSensorCount();
                    tray->resetTrayState(0);
                }
                finish_stop = false;
            }
        }
        //取料
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
            if(parameters.staticTest())
                current_time = parameters.repeatTime();
            while(current_time > 0)
            {
                if(!performSensorPR())
                {
                    sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                    if(waitMessageReturn(is_run))
                        is_run = false;
                    else
                        continue;
                    if(!is_run)break;
                }
                if(!parameters.staticTest())
                    break;
                current_time--;
            }
            sensor_vision->CloseLight();
            getPicker1SensorOffset();
            getPicker2SensorOffset();
            if(!moveToWorkPos(false))
            {
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                waitMessageReturn(is_run);
                if(!is_run)break;
            }
            if((!pickTraySensor()))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(!waitMessageReturn(is_run))
                    states.setHasPickedSensor(true);
            }
            else
                states.setHasPickedSensor(true);
            setPicker1SensorOffset();
            setPicker2SensorOffset();
            tray->setCurrentMaterialState(MaterialState::IsEmpty,sensor_tray_index);
            picked_material = tray->getCurrentIndex(sensor_tray_index);
            if(!is_run)break;
        }
        //等待位置
        if(!moveToSUTPRPos(isLocalHost,true))
        {
            sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
            is_run = false;
            break;
        }
        //sut操作
        if ((!states.allowChangeTray())&&(!requestQueue.isEmpty()) && (!states.beExchangeMaterial())&&states.hasPickedSensor()&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct())&&(!states.hasPickedNgProduct()))
        {
            has_task = true;
            QMutexLocker locker(&tcp_mutex);
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

        if ((!states.allowChangeTray())&& states.beExchangeMaterial())
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
                    QMutexLocker locker(&tcp_mutex);
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
                    if((!performSUTProductPR()))
                    {
                        AppendError(u8"成品视觉失败！");
                        sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                        int result = waitMessageReturn(is_run);
                        if(result)is_run = false;
                        if(!is_run)break;
                        if(result)
                            continue;
                    }
                    sut_product_vision->CloseLight();
                    getPicker2SensorOffset();
                    if(!movePicker2ToSUTPos(isLocalHost))
                    {
                        sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                        waitMessageReturn(is_run);
                        if(!is_run)break;
                    }
                    if((!pickSUTProduct(isLocalHost?"::1":"remote")))
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            continue;
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
                    setPicker2SensorOffset();
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
                    if((!performSUTProductPR()))
                    {
                        AppendError(u8"成品视觉失败！");
                        sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                        int result = waitMessageReturn(is_run);
                        if(result)is_run = false;
                        if(!is_run)break;
                        if(result)
                            continue;
                    }
                    sut_product_vision->CloseLight();
                    getPicker2SensorOffset();
                    //                    states.setPicker2OffsetX(pr_offset.O_X);
                    //                    states.setPicker2OffsetY(pr_offset.O_Y);
                    if(!movePicker2ToSUTPos(isLocalHost))
                    {
                        sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                        waitMessageReturn(is_run);
                        if(!is_run)break;
                    }
                    if((!pickSUTProduct(isLocalHost?"::1":"remote")))
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            continue;
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
                    setPicker2SensorOffset();
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
                    //取NG sensor
                    sut_sensor_vision->OpenLight();
                    if(!moveToSUTPRPos(isLocalHost))
                    {
                        AppendError("moveToSUTPRPos fail!");
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if((!performSUTSensorPR()))
                    {
                        AppendError(u8"NG视觉失败！");
                        sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                        int result = waitMessageReturn(is_run);
                        if(result)
                            is_run = false;
                        else
                            continue;
                        if(!is_run)break;
                    }
                    sut_sensor_vision->CloseLight();
                    getPicker2SensorOffset();
                    //                    states.setPicker2OffsetX(pr_offset.O_X);
                    //                    states.setPicker2OffsetY(pr_offset.O_Y);
                    if(!movePicker2ToSUTPos(isLocalHost))
                    {
                        sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                        waitMessageReturn(is_run);
                        if(!is_run)break;
                    }
                    if((!pickSUTSensor(isLocalHost?"::1":"remote")))
                    {
                        AppendError("pickSUTSensor fail!");
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
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
                    setPicker2SensorOffset();
                    sensor_uph.addCurrentReslutNumber(updateAccumulatedHour());
                    if(isLocalHost)
                        right_sensor_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    else
                        left_sensor_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                }
                else if ((states.cmd() == "loadSensorReq")&&states.hasPickedSensor())
                {
                    //放料到SUT
                    pr_offset.ReSet();
                    if(!movePicker1ToSUTPos(isLocalHost))
                    {
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if((!placeSensorToSUT(isLocalHost?"::1":"remote")))
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
        AppendError(QString(u8"移动到sensor盘下一个位置失败"));
    qInfo(u8"移动到sensor号盘下一个位置");
    return result;
}

bool SensorLoaderModule::moveToProductTrayNextPos()
{
    bool result = tray->findNextPositionOfInitState(1);
    if(result)
        result &=  pick_arm->move_XY_Synic(tray->getCurrentPosition(1));
    if(!result)
        AppendError(QString(u8"移动到成品盘下一个位置失败"));
    qInfo(u8"移动到成品盘下一个位置");
    return result;
}

bool SensorLoaderModule::moveToNgTrayNextPos()
{
    bool result_tray = tray->findNextPositionOfInitState(2);
    bool result = true;
    if(result_tray)
        result &=  pick_arm->move_XY_Synic(tray->getCurrentPosition(2));
    if(!result)
        AppendError(QString(u8"移动到Ng盘下一个位置失败"));
    qInfo(u8"移动到Ng盘下一个位置");
    return result&&result_tray;
}

bool SensorLoaderModule::moveToSUTPRPos(bool is_local,bool check_arrived,bool check_softlanding)
{
//    qInfo("moveToSUTPRPos is_local %d",is_local);
    bool result;
    QPointF temp_pos;
    if(is_local)
        temp_pos =  sut2_pr_position.ToPointF();
    else
        temp_pos =  sut1_pr_position.ToPointF();
    result = pick_arm->move_XYT2_Synic(temp_pos.x(),temp_pos.y(),parameters.picker2ThetaOffset(),check_arrived,check_softlanding);
    if(!result)
        AppendError(QString(u8"移动SPA相机SUT位置失败%1").arg(is_local));
    return result;
}

bool SensorLoaderModule::movePicker1ToSUTPos(bool is_local,bool check_arrived,bool check_softlanding)
{
    bool result;
    QPointF temp_pos;
    double theta = parameters.picker1ThetaOffset() + pr_offset.Theta;
    if(is_local)
    {
        temp_pos =  sut2_pr_position.ToPointF();
            theta += parameters.sut2Theta();
    }
    else
    {
        temp_pos =  sut1_pr_position.ToPointF();
        theta += parameters.sut1Theta();
    }
    result = pick_arm->move_XYT1_Synic(temp_pos.x() + picker1_offset.X() - pr_offset.X - states.picker1SensorOffsetX(),
                                       temp_pos.y() + picker1_offset.Y() - pr_offset.Y - states.picker1SensorOffsetY(),
                                       theta,
                                       check_arrived,check_softlanding);
    bool check_result = checkPickedSensor(true);
    result &= pick_arm->wait_XYT1_Arrived();
    if(!result)
        AppendError(QString(u8"移动SPA吸嘴1到SUT位置失败%1").arg(is_local));
    return result&&check_result;
}

bool SensorLoaderModule::movePicker2ToSUTPos(bool is_local,bool check_arrived,bool check_softlanding)
{
    bool result;
    QPointF temp_pos;
    if(is_local)
        temp_pos =  sut2_pr_position.ToPointF();
    else
        temp_pos =  sut1_pr_position.ToPointF();
    result = pick_arm->move_XYT2_Pos(temp_pos.x() + picker2_offset.X() - pr_offset.X - states.picker2SensorOffsetX(),
                                     temp_pos.y() + picker2_offset.Y() - pr_offset.Y - states.picker2SensorOffsetY(),
                                     parameters.picker2ThetaOffset() + pr_offset.Theta,
                                     check_arrived,check_softlanding);
    bool check_result = checkPickedNgOrProduct(false);
    result &= pick_arm->wait_XYT2_Arrived();
    if(!result)
        AppendError(QString(u8"移动SPA吸嘴2到SUT位置失败%1").arg(is_local));
    return result&&check_result;
}

bool SensorLoaderModule::performSensorPR()
{
    qInfo("performSensorPR");
    bool result = sensor_vision->performPR(pr_offset);
    if(!result)
        AppendError(QString(u8"执行料盘sensor视觉失败!"));
    return  result;
}

bool SensorLoaderModule::performVacancyPR()
{
    qInfo("performVacancyPR");
    bool result = vacancy_vision->performPR(pr_offset);
    if(!result)
        AppendError(QString(u8"执行料盘空位视觉失败!"));
    return  result;
}

bool SensorLoaderModule::performSUTPR()
{
    qInfo("performSUTPR");
    bool result = sut_vision->performPR(pr_offset);
    if(!result)
        AppendError(QString(u8"执行SUT视觉失败!"));
    return  result;
}

bool SensorLoaderModule::performSUTSensorPR()
{
    qInfo("performSUTSensorPR");
    bool result = sut_sensor_vision->performPR(pr_offset);
    if(!result)
        AppendError(QString(u8"执行SUT上的sensor视觉失败!"));
    return  result;
}

bool SensorLoaderModule::performSUTProductPR()
{
    qInfo("performSUTProductPR");
    bool result =  sut_product_vision->performPR(pr_offset);
    if(!result)
        AppendError(QString(u8"执行SUT视觉失败!"));
    return  result;
}

bool SensorLoaderModule::moveToWorkPos(bool check_state,bool check_softlanding)
{

    PrOffset temp(picker1_offset.X() - states.picker1SensorOffsetX() - pr_offset.X,picker1_offset.Y() - states.picker1SensorOffsetY() - pr_offset.Y,-pr_offset.Theta);
    qInfo("moveToWorkPos offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XYT1_Pos(temp.X,temp.Y,temp.Theta,check_softlanding);
    bool check_result = checkPickedSensor(check_state);
    result &= pick_arm->wait_XYT1_Arrived();
    if(!result)
        AppendError(QString(u8"去1号吸头工作位置(step x %1,y %2,t %3)失败!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    result &= check_result;
    return  result;
}

bool SensorLoaderModule::moveToWorkPos2(bool check_state,bool check_softlanding)
{
    PrOffset temp(picker2_offset.X() - states.picker2SensorOffsetX() - pr_offset.X,picker2_offset.Y() - states.picker2SensorOffsetY() - pr_offset.Y,-pr_offset.Theta);
    qInfo("moveToWorkPos2 offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XYT2_Pos(temp.X,temp.Y,temp.Theta,check_softlanding);
    bool check_result = checkPickedNgOrProduct(check_state);
    result &= pick_arm->wait_XYT2_Arrived();
    if(!result)
        AppendError(QString(u8"去2号吸头工作位置(step x %1,y %2,t %3)失败!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    result &= check_result;
    return  result;
}

bool SensorLoaderModule::moveToPRResultPos(bool check_softlanding)
{
    PrOffset temp(pr_offset.X,pr_offset.Y,pr_offset.Theta);
    qInfo("moveToPRResultPos offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XY_Synic(temp.X,temp.Y,check_softlanding);
    if(!result)
        AppendError(QString(u8"去视觉结果位置(step x %1,y %2,t %3)失败!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    return  result;
}

bool SensorLoaderModule::movePicker2ToTrayPos(int tray_index)
{
    bool result = tray->findNextPositionOfInitState(tray_index);
    if(!result)
    {
        AppendError(QString(u8"获取盘下一个位置失败"));
        qInfo("获取盘下一个位置失败");
        return false;
    }
    QPointF next_pos = tray->getCurrentPosition(tray_index);
    double x = next_pos.x() + picker2_offset.X() - states.picker2SensorOffsetX() - pr_offset.X;
    double y = next_pos.y() + picker2_offset.Y() - states.picker2SensorOffsetY() - pr_offset.Y;
    double t = parameters.picker2ThetaOffset() + pr_offset.Theta;
    result = pick_arm->move_XYT2_Pos(x,y,t);
    bool check_result = checkPickedNgOrProduct(true);
    result &= pick_arm->wait_XYT2_Arrived();
    if(!result)
        AppendError(QString(u8"去2号吸头盘工作位置(step x %1,y %2,t %3)失败!").arg(x).arg(y).arg(t));
    result &= check_result;
    return  result;
}

bool SensorLoaderModule::picker1SearchZ(double z,bool is_open, int time_out)
{
    qInfo("picker1SearchZ z %f is_open %d timeout %d",z,is_open,time_out);
    bool result = true;
    if(parameters.enableForceLimit())
        result = pick_arm->Z1SerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),parameters.finishDelay(),is_open,false,time_out);
    else
        result = pick_arm->Z1MoveToPick(z - parameters.vcmMargin(),is_open,false);
    if(parameters.enableForceLimit())
        result &= pick_arm->ZSerchReturn(time_out);
    else
        result &= pick_arm->picker1->motor_z->MoveToPosSync(0);
    return result;
}

bool SensorLoaderModule::picker1SearchSutZ(double z, QString dest, QString cmd, bool is_open, int time_out)
{
    qInfo("picker1SearchSutZ z %f dest %s cmd %s is_open %d time_out %d",z,dest.toStdString().c_str(),cmd.toStdString().c_str(),is_open,time_out);
    bool result = true;
    if(parameters.enablePlaceForce())
        result = pick_arm->move_XeYe_Z1_XY(z - parameters.escapeHeight(),parameters.escapeX(),parameters.escapeY());
    if(result)
    {
        if(parameters.enableSutForceLimit())
            result = pick_arm->Z1SerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),parameters.finishDelay(),is_open,false,time_out);
        else
            result = pick_arm->Z1MoveToPick(z - parameters.sutMargin(),is_open,false);
        sendCmd(dest,cmd);
        QThread::msleep(200);
        if(parameters.enableSutForceLimit())
            result &= pick_arm->ZSerchReturn(time_out);
        else
            result &= pick_arm->picker1->motor_z->MoveToPosSync(0);
    }
    result &= pick_arm->picker1->motor_z->MoveToPosSync(0);
    return result;
}
bool SensorLoaderModule::picker2SearchZ(double z,double force,bool is_open, int time_out)
{
    qInfo("picker2SearchZ z %f is_open %d time_out %d",z,is_open,time_out);
    bool result = true;
    if(parameters.enablePlaceForce())
        result = pick_arm->Z2SerchByForce(parameters.vcmWorkSpeed(),force,z + parameters.zOffset(),parameters.vcmMargin(),parameters.finishDelay(),is_open,false,time_out);
    else
        result = pick_arm->Z2MoveToPick(z - parameters.placeTrayMargin(),is_open,false);
    if(parameters.enablePlaceForce())
        result &= pick_arm->ZSerchReturn2(time_out);
    else
        result &= pick_arm->picker2->motor_z->MoveToPosSync(0);
    return result;
}

bool SensorLoaderModule::picker2SearchSutZ(double z,double force, QString dest, QString cmd, bool is_open, int time_out)
{
    qInfo("picker2SearchZ %s",dest.toStdString().c_str());
    sendCmd(dest,cmd);
    pick_arm->picker2->vacuum->Set(is_open,false);
    bool result = true;
    if(parameters.enableEscape())
        result = pick_arm->picker2->motor_z->MoveToPosSync(z - parameters.escapeHeight());
    if(result)
    {
        if(parameters.enableSutForceLimit())
            result = pick_arm->Z2SerchByForce(parameters.vcmWorkSpeed(),force,z + parameters.zOffset(),parameters.vcmMargin(),parameters.finishDelay(),is_open,false,time_out);
        else
            result = pick_arm->Z2MoveToPick(z - parameters.sutMargin(),is_open,false);
        if(parameters.enableSutForceLimit())
            result &= pick_arm->ZSerchReturn2(time_out);
        else
            result &= pick_arm->picker2->motor_z->MoveToPosSync(0);
    }
    if(parameters.enableEscape())
        result &= pick_arm->move_XeYe_Z2(0,parameters.escapeX(),parameters.escapeY());
    return result;
}

bool SensorLoaderModule::checkPickedSensor(bool check_state)
{
    bool result;
    result = pick_arm->picker1->vacuum->checkHasMaterielSync();
    if(result == check_state)
        return true;
    QString error = QString(u8"sensor吸头上逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool SensorLoaderModule::checkPickedNgOrProduct(bool check_state)
{
    bool result;
    result = pick_arm->picker2->vacuum->checkHasMaterielSync();
    if(result == check_state)
        return true;
    QString error = QString(u8"成品吸头上逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool SensorLoaderModule::pickTraySensor(int time_out)
{
    qInfo("pickTraySensor time_out %d",time_out);
    bool result = picker1SearchZ(parameters.pickSensorZ(),true,time_out);
    if(!result)
        AppendError(QString(u8"从sensor盘取sensor失败"));
    if(result)
        result &= checkPickedSensor(true);
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
    bool result = picker2SearchSutZ(parameters.pickNgSensorZ(),parameters.vcmWorkForce(),dest,"vacuumOffReq",true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT%1取NGsenor失败").arg(dest=="remote"?1:2));
    if(result)
        result &= checkPickedNgOrProduct(true);
    return result;
}

bool SensorLoaderModule::pickSUTProduct(QString dest,int time_out)
{
    qInfo("pickSUTProduct dest %s time_out %d",dest.toStdString().c_str(),time_out);
    bool result = picker2SearchSutZ(parameters.pickProductZ(),parameters.pickProductForce(),dest,"vacuumOffReq",true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT%1取成品失败").arg(dest=="remote"?1:2));
    if(result)
        result &= checkPickedNgOrProduct(true);
    return result;
}

bool SensorLoaderModule::placeSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d",time_out);
    bool result = picker2SearchZ(parameters.placeNgSensorZ(),parameters.vcmWorkForce(),false,time_out);
    if(!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    return result;
}

bool SensorLoaderModule::placeProductToTray(int time_out)
{
    qInfo("placeProductToTray time_out %d",time_out);
    bool result = picker2SearchZ(parameters.placeProductZ(),parameters.pickProductForce(),false,time_out);
    if(!result)
        AppendError(QString(u8"将成品放入成品盘失败"));
    return result;
}

bool SensorLoaderModule::picker1MeasureHight(bool is_tray)
{
    qInfo("picker1MeasureHight is_tray %d",is_tray);
    if(pick_arm->Z1SerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))
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
    double force = is_product?parameters.pickProductForce():parameters.vcmWorkForce();
    if(pick_arm->Z2SerchByForce(parameters.vcmWorkSpeed(),force,true))
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
    QPointF temp_point(sut1_pr_position.X()+picker1_offset.X(),sut2_pr_position.Y()+picker1_offset.Y());
    double sut1_height = 0;
    if(!pick_arm->move_XY_Synic(temp_point,true))
    {
        AppendError(QString(u8"测量SUT台高度差失败"));
        return false;
    }
    if(!pick_arm->Z1SerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))
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
    if(!pick_arm->Z1SerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))

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
    QPointF tem_pos = tray->getCurrentPosition(tray_index);
    bool result = pick_arm->move_XYT1_Synic(tem_pos.x(),tem_pos.y(),parameters.picker1ThetaOffset(),true);
    if(!result)
        AppendError(QString(u8"移动到%1盘当前位置失败").arg(tray_index == 0?u8"sensor":u8"成品"));
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

bool SensorLoaderModule::moveToStandbyPos(bool check_arrived,bool check_softlanding)
{
    qInfo("moveToStandbyPosition");
    bool result = pick_arm->move_XY_Synic(QPointF(spa_standby_position.X(), spa_standby_position.Y()),check_arrived,check_softlanding);
    if(!result)
         AppendError(QString(u8"移动SPA到standby位置失败"));
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

void SensorLoaderModule::getPicker1SensorOffset()
{
    if(parameters.useSensorOffset())
    {
        double temp_theta = pr_offset.Theta*PI/180;
        double x = parameters.sensorOffsetX()*cos(temp_theta) + parameters.sensorOffsetY()*sin(temp_theta) ;
        double y = parameters.sensorOffsetY()*cos(temp_theta) - parameters.sensorOffsetX()*sin(temp_theta) ;
        states.setPicker1SensorOffsetX(x);
        states.setPicker1SensorOffsetY(y);
    }
    else
    {
        states.setPicker1SensorOffsetX(pr_offset.O_X);
        states.setPicker1SensorOffsetY(pr_offset.O_Y);
    }
}

void SensorLoaderModule::getPicker2SensorOffset()
{
    if(parameters.useSensorOffset())
    {
        double temp_theta = pr_offset.Theta*PI/180;
        double x = parameters.sensorOffsetX()*cos(temp_theta) + parameters.sensorOffsetY()*sin(temp_theta) ;
        double y = parameters.sensorOffsetY()*cos(temp_theta) - parameters.sensorOffsetX()*sin(temp_theta) ;
        states.setPicker2SensorOffsetX(x);
        states.setPicker2SensorOffsetY(y);
    }
    else
    {
        states.setPicker2SensorOffsetX(pr_offset.O_X);
        states.setPicker2SensorOffsetY(pr_offset.O_Y);
    }
}

void SensorLoaderModule::setPicker1SensorOffset()
{
    if(parameters.useSensorOffset())
    {
        states.setPicker1SensorOffsetX(parameters.sensorOffsetX());
        states.setPicker1SensorOffsetY(parameters.sensorOffsetY());
    }
    else
    {
        double temp_theta = -pr_offset.Theta*PI/180;
        double x = pr_offset.O_X*cos(temp_theta) + pr_offset.O_Y*sin(temp_theta) ;
        double y = pr_offset.O_Y*cos(temp_theta) - pr_offset.O_X*sin(temp_theta) ;
        states.setPicker1SensorOffsetX(x);
        states.setPicker1SensorOffsetY(y);
    }
}

void SensorLoaderModule::setPicker2SensorOffset()
{
    if(parameters.useSensorOffset())
    {
        states.setPicker2SensorOffsetX(parameters.sensorOffsetX());
        states.setPicker2SensorOffsetY(parameters.sensorOffsetY());
    }
    else
    {
        double temp_theta = -pr_offset.Theta*PI/180;
        double x = pr_offset.O_X*cos(temp_theta) + pr_offset.O_Y*sin(temp_theta) ;
        double y = pr_offset.O_Y*cos(temp_theta) - pr_offset.O_X*sin(temp_theta) ;
        states.setPicker2SensorOffsetX(x);
        states.setPicker2SensorOffsetY(y);
    }
}

QString SensorLoaderModule::getUuid(int sensor_index)
{
    QString uuid = "";
    if(is_test_finish)
        uuid.append("left_");
    else
        uuid.append("right_");
    uuid.append(QString::number(sensor_index + 1));
    uuid.append("_");
    uuid.append(QString::number(parameters.repeatTime() - current_time + 1));
    return uuid;
}

void SensorLoaderModule::startRecord(QString uuid)
{
    qInfo("startRecord %s",uuid.toStdString().c_str());
    QStringList list = uuid.split('_');
    QVariantMap* temp = new QVariantMap();
    temp->insert("_station",list[0]);
    temp->insert("_lens_id",list[1]);
    temp->insert("_test_time",list[2]);
}

void SensorLoaderModule::recordPrTest(QString uuid,VisionLocation location)
{
    if(!location.parameters.enablePrTest())
        return;
    qInfo("record %s uuid: %s",location.parameters.locationName().toStdString().c_str(),uuid.toStdString().c_str());
    QVariantMap* temp_map = &prRecordMap[uuid];
    QString temp_name = QString::number(location.parameters.testIndex()).append("_").append(location.parameters.locationName().append("_offset"));
    temp_map->insert(temp_name.append(".X"),pr_offset.X);
    temp_map->insert(temp_name.append(".Y"),pr_offset.Y);
    temp_map->insert(temp_name.append(".T"),pr_offset.Theta);
}

void SensorLoaderModule::endRecord(QString uuid)
{
    postCSVDataToUnit(uuid,prRecordMap[uuid]);
    saveUnitDataToCSV(uuid);
    prRecordMap.remove(uuid);
}

PropertyBase *SensorLoaderModule::getModuleState()
{
    return &states;
}

