#include "sensorloadermodule.h"
#include "utils/commonutils.h"
//#include "logicmanager.h"
#include "basemodulemanager.h"
#include <QMessageBox>
#include "utils/commonutils.h"
#include "utils/uiHelper/uioperation.h"
#include "utils/singletoninstances.h"
#define PI 3.1415926535898
SensorLoaderModule::SensorLoaderModule():ThreadWorkerBase ("SensorLoaderModule")
{

}

void SensorLoaderModule::Init(SensorPickArm *pick_arm,
                              MaterialTray *sensor_tray,
                              VisionLocation *sensor_vision,
                              VisionLocation *vacancy_vision,
                              VisionLocation *sut_vision,
                              VisionLocation *sut_sensor_vision,
                              VisionLocation *sut_product_vision,
                              VisionLocation *sensor_pickarm_calibration_glass_vision,
                              int thread_id)
{
    this->thread_id = thread_id;
    this->pick_arm = pick_arm;
    parts.append(this->pick_arm);
    this->tray = sensor_tray;
    parts.append(this->tray);
    this->tray_sensor_location = sensor_vision;
    parts.append(this->tray_sensor_location);
    this->tray_empty_location = vacancy_vision;
    parts.append(this->tray_empty_location);
    this->sut_empty_location = sut_vision;
    parts.append(this->sut_empty_location);
    this->sut_sensor_location = sut_sensor_vision;
    parts.append(this->sut_sensor_location);
    this->sut_product_location = sut_product_vision;
    parts.append(this->sut_product_location);
    this->sensor_pickarm_calibration_glass_location = sensor_pickarm_calibration_glass_vision;
    parts.append(this->sensor_pickarm_calibration_glass_location);
}

void SensorLoaderModule::startWork(int run_mode)
{
    if(run_mode == RunMode::AAFlowChartTest || run_mode == RunMode::UNLOAD_ALL_LENS) return;
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
    if(run_params.contains("DisableStation"))
    {
        QVariantMap disable_map = run_params["DisableStation"].toMap();
        states.setDisableStation1(disable_map["0"].toBool());
        states.setDisableStation2(disable_map["1"].toBool());
        qInfo("disableStation1:%d,disableStation2:%d",states.disableStation1(),states.disableStation2());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数DisableStation缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(run_params.contains("StationTask"))
    {
        QVariantMap task_map = run_params["StationTask"].toMap();
        states.setTaskOfStation1(task_map["0"].toInt());
        states.setTaskOfStation2(task_map["1"].toInt());
        qInfo("taskOfStation1:%d,taskOfStation2:%d",states.taskOfStation1(),states.taskOfStation2());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数StationTask缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(run_params.contains("HandlyChangeSensorTray"))
    {
        states.setHandlyChangeTray(run_params["HandlyChangeSensorTray"].toBool());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数HandlyChangeSensorTray缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(run_params.contains("HandlyChangeSensor"))
    {
        states.setHandlyChangeSensor(run_params["HandlyChangeSensor"].toBool());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数HandlyChangeSensor缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(states.handlyChangeSensor())
        return;


    qInfo("SensorLoader start run_mode :%d in %d",run_mode,QThread::currentThreadId());
    if(run_mode == RunMode::Normal)run();
    else if(run_mode == RunMode::NoMaterial)run();
    else if (run_mode == RunMode::VibrationTest) {
        is_run = true;
        while(is_run) {
            moveCameraToSUTPRPos(false,true);
            QThread::msleep(2000);
            moveCameraToSUTPRPos(true,true);
            QThread::msleep(2000);
        }
    }
}

void SensorLoaderModule::stopWork(bool wait_finish)
{
    is_run = false;
}

void SensorLoaderModule::performHandlingOperation(int cmd,QVariant param)
{
    qInfo("performHandling %d",cmd);
    bool result = false;
    int temp_value = TIMES_1;
    if(cmd%temp_value == HandleCameraPosition::SUT_POS1){
        result = moveCameraToSUTPRPos(false,true);
    }
    else if(cmd%temp_value == HandleCameraPosition::SUT_POS2){
        result = moveCameraToSUTPRPos(true,true);
    }
    else if(cmd%temp_value == HandleCameraPosition::SENSOR_TRAY_1_POS){
        result = moveCameraToTrayCurrentPos(SensorPosition::SENSOR_TRAY_1);
    }
    else if(cmd%temp_value == HandleCameraPosition::SENSOR_TRAY_2_POS){
        result = moveCameraToTrayCurrentPos(SensorPosition::SENSOR_TRAY_2);
    }
    else if(cmd%temp_value == HandleCameraPosition::SENSOR_TRAY_1_START_POS){
        result = moveToStartPos(SensorPosition::SENSOR_TRAY_1);
    }
    else if(cmd%temp_value == HandleCameraPosition::SENSOR_TRAY_2_START_POS){
        result = moveToStartPos(SensorPosition::SENSOR_TRAY_2);
    }
    else if(cmd%temp_value == HandleCameraPosition::NG_SENSOR_TRAY_POS){
        result = moveCameraToTrayCurrentPos(SensorPosition::NG_SENSOR_TRAY);
    }
    else if(cmd%temp_value == HandleCameraPosition::NG_SENSOR_TRAY_START_POS){
        result = moveToStartPos(SensorPosition::NG_SENSOR_TRAY);
    }
    else if(cmd%temp_value == HandleCameraPosition::BUFFER_TRAY_POS){
        result = moveCameraToTrayCurrentPos(SensorPosition::BUFFER_TRAY);
    }
    else if(cmd%temp_value == HandleCameraPosition::BUFFER_TRAY_START_POS){
        result = moveToStartPos(SensorPosition::BUFFER_TRAY);
    }
    else if(cmd%temp_value == HandleCameraPosition::SENSOR_TRAY_1_END_POS){
        result = moveToTray1EndPos();
    }
    else if(cmd%temp_value == HandleCameraPosition::SPA_STANDBY_POS){
        result = moveCameraToStandbyPos(true,true);
    }
    else
        result =true;
    cmd =cmd/temp_value*temp_value;
    if(!result)
    {
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    temp_value = TIMES_2;
    PrOffset temp_offset;
    if(cmd%temp_value == HandlePR::TRAY_SENSOR_PR){
        sut_sensor_location->OpenLight();
        QThread::msleep(200);
        states.setRunMode(0);
        result = performTraySensorPR();
        temp_offset = sut_sensor_location->getCurrentResult();
    }
    else if(cmd%temp_value == HandlePR::TRAY_EMPTY_PR){
        tray_empty_location->OpenLight();
        QThread::msleep(200);
        states.setRunMode(0);
        result = performTrayEmptyPR();
        temp_offset = tray_empty_location->getCurrentResult();
    }
    else if(cmd%temp_value == HandlePR::SUT_EMPTY_PR){
        sut_empty_location->OpenLight();
        QThread::msleep(200);
        states.setRunMode(0);
        result = performSUTEmptyPR();
        temp_offset = sut_empty_location->getCurrentResult();
    }
    else if(cmd%temp_value == HandlePR::SUT_SENSOR_PR){
        if(parameters.enableNgSensorPr())
        {
            sut_sensor_location->OpenLight();
            QThread::msleep(200);
            states.setRunMode(0);
            result = performSUTSensorPR();
            temp_offset = sut_sensor_location->getCurrentResult();
        }
    }
    else if(cmd%temp_value == HandlePR::SUT_PRODUCT_PR){
        if(parameters.enableProductPr())
        {
            sut_product_location->OpenLight();
            QThread::msleep(200);
            states.setRunMode(0);
            result = performSUTProductPR();
            temp_offset = sut_product_location->getCurrentResult();
        }
    }
    else if(cmd%temp_value == HandlePR::TRAY_SENSOR_OFFSET){
        sut_sensor_location->OpenLight();
        QThread::msleep(200);
        states.setRunMode(0);
        result = performTraySensorPR();
        if(result)
        {
            QPointF offset = tray_sensor_location->getCurrentResultOffset();
            parameters.setSensorOffsetX(offset.x());
            parameters.setSensorOffsetY(offset.y());
        }
    }
    else
        result = true;
    if(!result)
    {
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    cmd =cmd/temp_value*temp_value;
    temp_value = TIMES_3;
    if(cmd%temp_value == HandlePickerPos::TO_PICK_SENSOR_POS1)
    {
        result = movePicker1ToTrayCurrentPos(SensorPosition::SENSOR_TRAY_1,true);
    }
    if(cmd%temp_value == HandlePickerPos::TO_PICK_SENSOR_POS2)
    {
        result = movePicker1ToTrayCurrentPos(SensorPosition::SENSOR_TRAY_2,true);
    }
    else if(cmd%temp_value == HandlePickerPos::TO_PLACE_SENSOR_POS1)
    {
        sut_empty_location->resetResult();
        result = movePicker1ToSUTPos(false,true);
    }
    else if(cmd%temp_value == HandlePickerPos::TO_PLACE_SENSOR_POS2)
    {
        sut_empty_location->resetResult();
        result = movePicker1ToSUTPos(true,true);
    }
    else if(cmd%temp_value == HandlePickerPos::TO_PICK_PRODUCT_POS1)
    {
        result = movePicker2ToSUTPos(false,true,true);
    }
    else if(cmd%temp_value == HandlePickerPos::TO_PICK_PRODUCT_POS2)
    {
        result = movePicker2ToSUTPos(true,true,true);
    }
    else if(cmd%temp_value == HandlePickerPos::TO_PLACE_PRODUCT_POS1)
    {
        PrOffset pr_offset = tray->getTrayCurrentPrOffset(SensorPosition::SENSOR_TRAY_1);
        tray_empty_location->setCurrentResult(pr_offset);
        result = movePicker2ToTrayCurrentPos(SensorPosition::SENSOR_TRAY_1,true);
    }
    else if(cmd%temp_value == HandlePickerPos::TO_PLACE_PRODUCT_POS2)
    {
        PrOffset pr_offset = tray->getTrayCurrentPrOffset(SensorPosition::SENSOR_TRAY_2);
        tray_empty_location->setCurrentResult(pr_offset);
        result = movePicker2ToTrayCurrentPos(SensorPosition::SENSOR_TRAY_2,true);
    }
    else if(cmd%temp_value == HandlePickerPos::TO_PLACE_BUFFER_POS)
    {
        result = movePicker2ToTrayCurrentPos(SensorPosition::BUFFER_TRAY,true);
    }
    else if(cmd%temp_value == HandlePickerPos::TO_PICK_NG_SENSOR_POS1)
    {
        result = movePicker2ToSUTPos(false,false,true);
    }
    else if(cmd%temp_value == HandlePickerPos::TO_PICK_NG_SENSOR_POS2)
    {
        result = movePicker2ToSUTPos(true,false,true);
    }
    else if(cmd%temp_value == HandlePickerPos::TO_PLACE_NG_POS)
    {
        result = movePicker2ToTrayCurrentPos(SensorPosition::NG_SENSOR_TRAY,true);
    }
    else if(cmd%temp_value == HandlePickerPos::TO_PR_OFFSET)
    {
        result = moveToPRResultPos(temp_offset,true);
    }
    if(!result)
    {
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    cmd =cmd/temp_value*temp_value;
    temp_value = TIMES_4;
    qInfo("cmd : %d", cmd);
    if(cmd%temp_value == handlePickerAction::PICK_SENSOR_FROM_TRAY1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickSensorFromTray(SensorPosition::SENSOR_TRAY_1);
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_SENSOR_FROM_TRAY2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickSensorFromTray(SensorPosition::SENSOR_TRAY_2);
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_SENSOR_TO_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeSensorToSUT(false);
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_SENSOR_TO_SUT2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeSensorToSUT(true);
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_NG_SENSOR_FROM_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickNgSensorFromSut(false);
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_NG_SENSOR_FROM_SUT2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickNgSensorFromSut(true);
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_NG_SENSOR_TO_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeNgSensorToTray();
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_SENSOR_BACK_TO_TRAY1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = backSensorToTray(SensorPosition::SENSOR_TRAY_1);
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_SENSOR_BACK_TO_TRAY2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = backSensorToTray(SensorPosition::SENSOR_TRAY_2);
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_PRODUCT_FROM_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickProductFromSut(false);
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PICK_PRODUCT_FROM_SUT2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = pickProductFromSut(true);
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_PRODUCT_TO_TRAY1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeProductToTray(SensorPosition::SENSOR_TRAY_1);
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_PRODUCT_TO_TRAY2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeProductToTray(SensorPosition::SENSOR_TRAY_2);
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_PRODUCT_TO_BUFFER_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeProductToTray(SensorPosition::BUFFER_TRAY);
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::PLACE_NG_PRODUCT_TO_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = placeNgProductToTray();
            pr_offset.ReSet();
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_SENSOR_IN_TRAY1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker1MeasureHight(SensorPosition::SENSOR_TRAY_1);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_SENSOR_IN_TRAY2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker1MeasureHight(SensorPosition::SENSOR_TRAY_2);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_SENSOR_IN_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker1MeasureHight(SensorPosition::SUT1_SENSOR);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_SENSOR_IN_SUT2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker1MeasureHight(SensorPosition::SUT2_SENSOR);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_NG_SENSOR_IN_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(SensorPosition::SUT1_SENSOR);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_NG_SENSOR_IN_SUT2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(SensorPosition::SUT2_SENSOR);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_NG_SENSOR_IN_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(SensorPosition::NG_SENSOR_TRAY);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_NG_PRODUCT_IN_TRAY){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(SensorPosition::TRAY_NG_PRODUCT);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_PRODUCT_IN_SUT1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(SensorPosition::SUT1_PRODUCT);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_PRODUCT_IN_SUT2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(SensorPosition::SUT2_PRODUCT);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_PRODUCT_IN_TRAY1){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(SENSOR_TRAY_1);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_PRODUCT_IN_TRAY2){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(SENSOR_TRAY_2);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_PRODUCT_IN_BUFFER){
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作"))){
            result = picker2MeasureHight(BUFFER_TRAY);
        }
    }
    else if(cmd%temp_value == handlePickerAction::MEASURE_Z_OFFSET)
        result = measureZOffset();
    else if(cmd%temp_value == handlePickerAction::CLEARANCE)
    {
        result = unloadAllSensor();
        sendAlarmMessage(OK_OPERATION,"Sensor Clearance Done",ErrorLevel::TipNonblock);
        is_handling = false;
        return;
   }
    else
        result = true;
    if(!result)
    {
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    is_handling = false;
}

void SensorLoaderModule::changeBufferTray()
{
    allowChangeBufferTray = true;
    if (is_run == false)
    {
        moveToStartPos(SensorPosition::NG_SENSOR_TRAY);
    }
}

void SensorLoaderModule::changeNgTray()
{
    allowChangeNgTray = true;
    if (is_run == false)
    {
        moveToStartPos(SensorPosition::BUFFER_TRAY);
    }
}

void SensorTrayLoaderModule::receiveChangeTray()
{
    qInfo("receiveChangeTray");
    if(states.changingTray())
        return;
    states.setNeedChangeTray(true);
    if(states.hasVacancyTray())
        states.setHasProductTray(true);
    states.setChangingTray(true);
}

//void SensorLoaderModule::receiveRequestMessage(QString message, QString client_ip)
//{
//    qInfo("Sut Module receive command:%s from ip: %s", message.toStdString().c_str(), client_ip.toStdString().c_str());
//    QJsonObject obj = getJsonObjectFromString(message);
//    QString cmd = obj["cmd"].toString("");
//    obj.insert("client_ip",client_ip);
//    if (cmd == "sensorReq") {
//        qInfo("Enqueue the sensor request command in request quene");
//        QMutexLocker locker(&tcp_mutex);
//        requestQueue.enqueue(obj);
//    }
//    else if (cmd.length() > 0)
//    {
//        qInfo("Enqueue the %s in action queue", cmd.toStdString().c_str());
//        QMutexLocker locker(&tcp_mutex);
//        actionQueue.enqueue(obj);
//    }
//}

//void SensorLoaderModule::receiveChangeTrayFinish()
//{
//    qInfo("SensorLoaderModule receiveChangeTrayFInish");
//    QMutexLocker temp_locker(&tray_mutex);
//    if(states.waitingChangeTray())
//    {
//        states.setFinishChangeTray(true);
//    }
//    else
//    {
//        qInfo("SensorLoaderModule receiveChangeTrayFInish but not effective");
//    }
//}

void SensorLoaderModule::receivceModuleMessage(QVariantMap message)
{
    qInfo("receive module message %s",TcpMessager::getStringFromQvariantMap(message).toStdString().c_str());
    if(!message.contains("OriginModule"))
    {
        qInfo("message error! has no OriginModule.");
        return;
    }
    if(message["OriginModule"].toString().contains("LogicManager"))
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString().contains("SutVacuumSuccess"))
            {
                states.setVacuumOperationResult(1);
            }
            else if(message["Message"].toString().contains("SutVacuumFail"))
            {
                states.setVacuumOperationResult(-1);
            }
        }
    }
    else if(message["OriginModule"].toString()=="AA1CoreNew")
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString()=="LoadSensorRequest")
            {
                if(message.contains("MaterialState"))
                    states.setSut1MaterialState(MaterialTray::getMaterialStateFromName(message["MaterialState"].toString()));
                else
                    qInfo("message MaterialState miss.");
                if(message.contains("TaskNumber"))
                    states.setTaskOfStation1(message["TaskNumber"].toInt());
                if(!states.station1Unload())
                    states.setStation2NeedSensor(true);
                states.setStation1HasRequest(true);
            }
            else if(message["Message"].toString()=="UnloadMode")
            {
                states.setStation1Unload(true);
            }
            else if(message["Message"].toString()=="NormalMode")
            {
                states.setStation1Unload(false);
            }
            else
            {
                qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            }
        }
    }
    else if(message["OriginModule"].toString()=="AA2CoreNew")
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString()=="LoadSensorRequest")
            {
                if(message.contains("MaterialState"))
                    states.setSut2MaterialState(MaterialTray::getMaterialStateFromName(message["MaterialState"].toString()));
                else
                    qInfo("message MaterialState miss.");
                if(message.contains("TaskNumber"))
                    states.setTaskOfStation2(message["TaskNumber"].toInt());
                if(!states.station2Unload())
                    states.setStation2NeedSensor(true);
                states.setStation2HasRequest(true);
            }
            else if(message["Message"].toString()=="UnloadMode")
            {
                states.setStation2Unload(true);
            }
            else if(message["Message"].toString()=="NormalMode")
            {
                states.setStation2Unload(false);
            }
            else
            {
                qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            }
        }
    }
    else if(message["OriginModule"].toString()=="SensorTrayLoaderModule")
    {
        if(message["Message"].toString()=="FinishChangeTray2")
        {
            if(states.waitingChangeTray())
            {
                tray->copyTrayState(SensorPosition::SENSOR_TRAY_1,SensorPosition::SENSOR_TRAY_2);
                states.setHasSensorTray2(true);
                states.setHasSensorTray1(false);
                states.setFinishChangeTray(true);
            }
            else
            {
                qInfo("%s receivceModuleMessage but not effective",Name().toStdString().c_str());
            }
        }
        else if(message["Message"].toString()=="FinishChangeTray1")
        {
            tray->resetTrayState(SensorPosition::SENSOR_TRAY_1);
            states.setHasSensorTray1(true);
        }
        else if(message["Message"].toString()=="FinishChangeTray")
        {
            tray->resetTrayState(SensorPosition::SENSOR_TRAY_1);
            tray->resetTrayState(SensorPosition::SENSOR_TRAY_2);
            states.setHasSensorTray2(true);
            states.setHasSensorTray1(true);
            states.setFinishChangeTray(true);
        }
        else if (message["Message"].toString()=="NoSensorTray")
        {
            tray->resetTrayState(SensorPosition::SENSOR_TRAY_1);
            states.setHasSensorTray1(true);
            tray->setTrayCurrentNg(SensorPosition::SENSOR_TRAY_1);
            states.setIsLastTray(true);
        }
        else
        {
            qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
        }
    }
    else if (message["OriginModule"].toString()=="Sut1Module")
    {
        if(message["Message"].toString()=="SutReady")
            states.setSut1Ready(true);
        else
        {
            qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            return;
        }

    }
    else if (message["OriginModule"].toString()=="Sut2Module")
    {
        if(message["Message"].toString()=="SutReady")
            states.setSut2Ready(true);
        else
        {
            qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            return;
        }
    }
}

void SensorLoaderModule::run()
{
    //逻辑状态与实际状态检测
    is_run = true;

    int pr_times = parameters.autoPrTime();
    int change_tray_time_out = parameters.changeTrayTimeOut();

    time_label = QTime::currentTime();
    while (is_run)
    {
        QThread::msleep(1);
        if(!is_run)break;

        //取料
        if((!states.allowChangeTray())&&checkNeedPickSensor()&&(MaterialState::IsEmpty == states.picker1MaterialState())&&findTrayNextSensorPos(false))
        {
            if(!is_run)break;
            //sensor视觉
            tray_sensor_location->OpenLight();
            if(!moveCameraToTrayCurrentPos(states.currentTrayID()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            if((!performTraySensorPR()))
            {
                if(pr_times > 0)
                {
                    pr_times--;
                    tray->setCurrentMaterialState(MaterialState::IsNgSensor,states.currentTrayID());
//                    AppendError(u8"自动重试.");
//                    sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
                    GetCurrentError();
                    continue;
                }
                else
                {
                    pr_times = parameters.autoPrTime();
                    AppendError(u8"pr连续失败五次");
                    int alarm_id = sendAlarmMessage(CONTINUE_BLIND_SKIPTRAY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(CONTINUE_OPERATION == operation)
                    {
                        tray->setCurrentMaterialState(MaterialState::IsNgProduct,states.currentTrayID());
                        continue;
                    }
                    else if (SKIPTRAY_OPERATION == operation)
                    {
                        tray->setTrayCurrentNg(states.currentTrayID());
                        continue;
                    }
                }
            }
            else
            {
                pr_times = parameters.autoPrTime();
                //sendAlarmMessage(ErrorLevel::TipNonblock,"");
            }
            //走偏移值
            checkPicker1HasMateril();
            if(!movePicker1ToTrayCurrentPos(states.currentTrayID()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            if(!waitPicker1CheckResult(false))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setPicker1MaterialState(MaterialState::IsRawSensor);
                    continue;
                }
                if(!is_run)break;
            }
            //取sensor
            if((!pickSensorFromTray(states.currentTrayID())))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    tray->setCurrentMaterialState(MaterialState::IsNgSensor,states.currentTrayID());
                    continue;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }
            tray->setCurrentMaterialState(MaterialState::IsEmpty,states.currentTrayID());
            states.setPicker1MaterialState(MaterialState::IsRawSensor);
            if(!is_run)break;
        }

        //放成品
        if((!states.allowChangeTray())&&(MaterialState::IsGoodProduct == states.picker2MaterialState())&&findTrayNextEmptyPos())
        {
            bool enable_pr = parameters.enablePlaceProdcutPr();
            bool is_buffer_tray = states.currentTrayID() == SensorPosition::BUFFER_TRAY;
            if(is_buffer_tray)
                enable_pr = parameters.enableBufferProductPr()||(!states.hasBufferTrayPrOffset());
            //视觉定位
            if(enable_pr)
            {
                tray_empty_location->OpenLight();
                if(!moveCameraToTrayCurrentPos(states.currentTrayID()))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
                if((!performTrayEmptyPR()))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_NEXT_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(NEXT_OPERATION == operation)
                    {
                        tray->setCurrentMaterialState(MaterialState::IsNgSensor,states.currentTrayID());
                        continue;
                    }
                    else if(RETRY_OPERATION == operation)
                        continue;
                }
                if(is_buffer_tray)
                {
                    tray->setTrayPrOffset(tray_empty_location->getCurrentResult(),0,SensorPosition::BUFFER_TRAY);
                    states.setHasBufferTrayPrOffset(true);
                }
            }
            else
            {
                if(is_buffer_tray)
                    tray_empty_location->setCurrentResult(tray->getTrayPrOffset(0,SensorPosition::BUFFER_TRAY));
                else
                {
                    PrOffset pr_offset = tray->getTrayCurrentPrOffset(states.currentTrayID());
                    tray_empty_location->setCurrentResult(pr_offset);
                }
            }
            if(!is_run)break;
            //走偏移值
            if(!movePicker2ToTrayCurrentPos(states.currentTrayID()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
                if(!is_run)break;
            }
            if(!is_run)break;
            //检查是否有料
            if(!checkPicker2HasMaterialSync())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setPicker2MaterialState(MaterialState::IsEmpty);
                    continue;
                }
            }
            if(!is_run)break;
            //放下成品
            if((!placeProductToTray(states.currentTrayID())))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    picker2_senseor_data["WorkStation"] = BusyState::IDLE;
                    tray->setCurrentMaterialState(MaterialState::IsNgSensor,states.currentTrayID());
                    states.setPicker2MaterialState(MaterialState::IsEmpty);
                    continue;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }
            picker2_senseor_data["WorkStation"] = BusyState::IDLE;
            tray->setCurrentMaterialState(MaterialState::IsGoodProduct,states.currentTrayID());
            states.setPicker2MaterialState(MaterialState::IsEmpty);
            if(!is_run)break;
        }

        //检测是否更换Buffer盘
        if(!findTrayNextInitStatePos(SensorPosition::BUFFER_TRAY) || allowChangeBufferTray)
        {
            if(!moveToStartPos(SensorPosition::NG_SENSOR_TRAY))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            AppendError(u8"请更换缓存盘后继续！");
            int alarm_id = sendAlarmMessage(u8"已换盘",GetCurrentError());
            waitMessageReturn(is_run,alarm_id);
            if(!is_run)break;
            tray->resetTrayState(SensorPosition::BUFFER_TRAY);
            states.setHasBufferTrayPrOffset(false);
        }

        //检测NG盘是否已满
        if(!findTrayNextInitStatePos(SensorPosition::NG_SENSOR_TRAY) || allowChangeNgTray)
        {
            if(!moveCameraToStandbyPos(true))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            AppendError(u8"请更换Ng盘后继续！");
            int alarm_id = sendAlarmMessage(u8"已换盘",GetCurrentError());
            waitMessageReturn(is_run,alarm_id);
            if(!is_run)break;
            tray->resetTrayState(SensorPosition::NG_SENSOR_TRAY);
            findTrayNextInitStatePos(SensorPosition::NG_SENSOR_TRAY);
            states.setHasNgTrayPrOffset(false);
        }

        //放Ng成品/NgSensor
        if(((MaterialState::IsNgProduct == states.picker2MaterialState())||(MaterialState::IsNgSensor == states.picker2MaterialState())))
        {
            //检查是否有料
            if(!checkPicker2HasMaterialSync())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setPicker2MaterialState(MaterialState::IsEmpty);
                    continue;
                }
                if(!is_run)break;
            }
            //NG盘视觉
            if(parameters.enablePlaceNgProductPr()||(!states.hasNgTrayPrOffset()))
            {
                tray_empty_location->OpenLight();
                if(!moveCameraToTrayCurrentPos(SensorPosition::NG_SENSOR_TRAY))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
                if((!performTrayEmptyPR()))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_NEXT_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(NEXT_OPERATION == operation)
                    {
                        tray->setCurrentMaterialState(MaterialState::IsNgProduct,states.currentTrayID());
                        continue;
                    }
                    else if(RETRY_OPERATION == operation)
                        continue;
                    if(!is_run)break;
                }
                if(!parameters.enablePlaceNgProductPr())
                {
                    tray->setTrayPrOffset(tray_empty_location->getCurrentResult(),0,SensorPosition::NG_SENSOR_TRAY);
                    states.setHasNgTrayPrOffset(true);
                }
            }
            else
            {
                tray_empty_location->setCurrentResult(tray->getTrayPrOffset(0,SensorPosition::NG_SENSOR_TRAY));
            }
            //走偏移值
            if(!movePicker2ToTrayCurrentPos(SensorPosition::NG_SENSOR_TRAY))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            //放NG成品/NgSensor
            if(states.picker2MaterialState() == MaterialState::IsNgSensor)
            {
                if((!placeNgProductToTray()))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
                tray->setCurrentMaterialState(MaterialState::IsNgSensor,states.currentTrayID());
            }
            else
            {
                if((!placeNgSensorToTray()))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
                tray->setCurrentMaterialState(MaterialState::IsNgProduct,states.currentTrayID());
            }
            picker2_senseor_data["WorkStation"] = BusyState::IDLE;
            states.setPicker2MaterialState(MaterialState::IsEmpty);
            if(!is_run)break;
        }

        //第一盘取空再取料
        if((!states.allowChangeTray())&&checkNeedPickSensor()&&(MaterialState::IsEmpty == states.picker1MaterialState())&&findTrayNextSensorPos(true))
        {
            //sensor视觉
            tray_sensor_location->OpenLight();
            if(!moveCameraToTrayCurrentPos(states.currentTrayID()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            if((!performTraySensorPR()))
            {
                if(pr_times > 0)
                {
                    pr_times--;
                    tray->setCurrentMaterialState(MaterialState::IsNgSensor,states.currentTrayID());
//                    AppendError(u8"自动重试.");
//                    sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
                    GetCurrentError();
                    continue;
                }
                else
                {
                    pr_times = parameters.autoPrTime();
                    AppendError(u8"pr连续失败五次");
                    int alarm_id = sendAlarmMessage(CONTINUE_BLIND_SKIPTRAY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(CONTINUE_OPERATION == operation)
                    {
                        tray->setCurrentMaterialState(MaterialState::IsEmpty,states.currentTrayID());
                        continue;
                    }
                    else if (SKIPTRAY_OPERATION == operation)
                    {
                        tray->setTrayCurrentNg(states.currentTrayID());
                        continue;
                    }
                }
            }
            else
            {
                pr_times = parameters.autoPrTime();
                //sendAlarmMessage(ErrorLevel::TipNonblock,"");
            }
            //走偏移值
            checkPicker1HasMateril();
            if(!movePicker1ToTrayCurrentPos(states.currentTrayID()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            if(!waitPicker1CheckResult(false))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setPicker1MaterialState(MaterialState::IsRawSensor);
                    continue;
                }
            }
            //取sensor
            if((!pickSensorFromTray(states.currentTrayID())))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    tray->setCurrentMaterialState(MaterialState::IsNgSensor,states.currentTrayID());
                    continue;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }
            tray->setCurrentMaterialState(MaterialState::IsEmpty,states.currentTrayID());
            states.setPicker1MaterialState(MaterialState::IsRawSensor);
            if(!is_run)break;
        }

        //sut任务分配
        if(states.busyState() == BusyState::IDLE)
        {
            if(states.station1HasRequest())
            {
                states.setBusyState(BusyState::SUT1);
                states.setLastState(states.busyState());
            }
            else if(states.station2HasRequest())
            {
                states.setBusyState(BusyState::SUT2);
                states.setLastState(states.busyState());
            }
        }

        //去等待位置
        if(states.picker2MaterialState() == MaterialState::IsEmpty){    // Picker2 有料，应先处理完再去等待位置
            if(checkSut1WaitCondition())
            {
                if((states.picker1MaterialState() == MaterialState::IsRawSensor)&&(states.sut1MaterialState() == MaterialState::IsEmpty))
                {
                    if(!movePicker1ToSUTPos(false))
                    {
                        int alarm_id = sendAlarmMessage(CONTINUE_OPERATION,GetCurrentError());
                        waitMessageReturn(is_run,alarm_id);
                        if(!is_run)break;
                    }
                }
                else if(((states.sut1MaterialState() == MaterialState::IsNgProduct)&&(!parameters.enableNgProductPr()))||
                        ((states.sut1MaterialState() == MaterialState::IsGoodProduct)&&(!parameters.enableProductPr()))||
                        ((states.sut1MaterialState() == MaterialState::IsNgSensor)&&(!parameters.enableNgSensorPr()))||
                        ((states.sut1MaterialState() == MaterialState::IsRawSensor)&&(!parameters.enableProductPr())))
                {
                    if(!movePicker2ToSUTPos(false,states.sut1MaterialState() != MaterialState::IsNgSensor))
                    {
                        int alarm_id = sendAlarmMessage(CONTINUE_OPERATION,GetCurrentError());
                        waitMessageReturn(is_run,alarm_id);
                        if(!is_run)break;
                    }
                }
                else
                {
                    if(((states.sut1MaterialState() == MaterialState::IsNgProduct)&&parameters.enableNgProductPr())||
                                        ((states.sut1MaterialState() == MaterialState::IsGoodProduct)&&parameters.enableProductPr()))
                    {
                        sut_product_location->OpenLight();
                    }
                    else if(((states.sut1MaterialState() == MaterialState::IsNgSensor)&&parameters.enableNgSensorPr()))
                    {
                        sut_sensor_location->OpenLight();
                    }
                    if(!moveCameraToSUTPRPos(false,true))
                    {
                        int alarm_id = sendAlarmMessage(CONTINUE_OPERATION,GetCurrentError());
                        waitMessageReturn(is_run,alarm_id);
                        if(!is_run)break;
                    }
                }
            }
            else if(checkSut2WaitCondition())
            {
                if((states.picker1MaterialState() == MaterialState::IsRawSensor)&&(states.sut2MaterialState() == MaterialState::IsEmpty))
                {
                    if(!movePicker1ToSUTPos(true))
                    {
                        int alarm_id = sendAlarmMessage(CONTINUE_OPERATION,GetCurrentError());
                        waitMessageReturn(is_run,alarm_id);
                        if(!is_run)break;
                    }
                }
                else if(((states.sut2MaterialState() == MaterialState::IsNgProduct)&&(!parameters.enableNgProductPr()))||
                        ((states.sut2MaterialState() == MaterialState::IsGoodProduct)&&(!parameters.enableProductPr()))||
                        ((states.sut2MaterialState() == MaterialState::IsNgSensor)&&(!parameters.enableNgSensorPr()))||
                        ((states.sut2MaterialState() == MaterialState::IsRawSensor)&&(!parameters.enableProductPr())))
                {
                    if(!movePicker2ToSUTPos(true,states.sut1MaterialState() != MaterialState::IsNgSensor))
                    {
                        int alarm_id = sendAlarmMessage(CONTINUE_OPERATION,GetCurrentError());
                        waitMessageReturn(is_run,alarm_id);
                        if(!is_run)break;
                    }
                }
                else
                {
                    if(((states.sut2MaterialState() == MaterialState::IsNgProduct)&&parameters.enableNgProductPr())||
                            ((states.sut2MaterialState() == MaterialState::IsGoodProduct)&&parameters.enableProductPr()))
                    {
                        sut_product_location->OpenLight();
                    }
                    else if(((states.sut2MaterialState() == MaterialState::IsNgSensor)&&parameters.enableNgSensorPr()))
                    {
                        sut_sensor_location->OpenLight();
                    }
                    if(!moveCameraToSUTPRPos(true,true))
                    {
                        int alarm_id = sendAlarmMessage(CONTINUE_OPERATION,GetCurrentError());
                        waitMessageReturn(is_run,alarm_id);
                        if(!is_run)break;
                    }
                }
            }
            else
            {
                if(!moveCameraToStandbyPos(true))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_OPERATION,GetCurrentError());
                    waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                }
            }
        }

        //检测是否需要换盘
        if(!states.allowChangeTray()&&(states.picker2MaterialState() == MaterialState::IsEmpty))
        {
            if((!states.hasSensorTray2())||checkTrayNeedChange())
                states.setAllowChangeTray(true);
            else
            {
                int force_state = checkForceChageStation();
                if(force_state>0)
                    states.setBusyState(force_state);
            }
        }

        //无料无任务停机
        if(states.station1Unload()&&states.station2Unload()
                &&(states.sut1MaterialState()== MaterialState::IsEmpty)
                &&(states.sut2MaterialState()== MaterialState::IsEmpty)
                &&(states.picker1MaterialState()== MaterialState::IsEmpty)
                &&(states.picker2MaterialState()== MaterialState::IsEmpty))
        {
            sendAlarmMessage(OK_OPERATION,u8"打料任务完成，自动停机！",ErrorLevel::ErrorMustStop);
            is_run = false;
            break;
        }
        //执行换盘
        if(!is_run)break;
        if(states.allowChangeTray())
        {
            if(states.waitingChangeTray())
            {
                if(states.finishChangeTray())
                {
                    if(states.handlyChangeTray())
                        states.setCrossTrayPlaceProduct(false);
                    else
                        states.setCrossTrayPlaceProduct(!states.crossTrayPlaceProduct());
                    states.setWaitingChangeTray(false);
                    states.setFinishChangeTray(false);
                    states.setAllowChangeTray(false);
                    qInfo("finishChangeTray");
                }
                else
                {
                    QThread::msleep(100);
                    change_tray_time_out -=100;
                    if(change_tray_time_out<=0)
                    {
                        AppendError(QString(u8"等待换盘超时，超时时间%d,请选择是继续等待或者重新换盘").arg(parameters.changeTrayTimeOut()));

                        int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                        QString operation = waitMessageReturn(is_run,alarm_id);
                        if(!is_run)break;
                        if(RETRY_OPERATION == operation)
                        {
                            change_tray_time_out = parameters.changeTrayTimeOut();
                            continue;
                        }
                        states.setWaitingChangeTray(false);
                    }
                }
            }
            else
            {
                states.setFinishChangeTray(false);
                sendMessageToModule("SensorTrayLoaderModule","ChangeTrayResquest");
                change_tray_time_out = parameters.changeTrayTimeOut();
                states.setWaitingChangeTray(true);
                qInfo("sendChangeTray");
            }

            if(!is_run)break;
        }

        //SUT操作
        if((states.picker2MaterialState() == MaterialState::IsEmpty)&&(((states.busyState() == BusyState::SUT1)&&states.sut1Ready())||((states.busyState() == BusyState::SUT2)&&states.sut2Ready())))
        {
            bool is_local = states.busyState() == BusyState::SUT2;
            //取成品/NG成品
            bool has_product = is_local?states.sut2MaterialState()==MaterialState::IsGoodProduct:states.sut1MaterialState()==MaterialState::IsGoodProduct;
            bool has_ng_product = is_local?states.sut2MaterialState()==MaterialState::IsNgProduct:states.sut1MaterialState()==MaterialState::IsNgProduct;
            if((has_product||has_ng_product)&&(states.picker2MaterialState() == MaterialState::IsEmpty))
            {
                //SUT成品视觉
                bool enable_pr = has_product?parameters.enableProductPr():parameters.enableNgProductPr();
                if(enable_pr)
                {
                    sut_product_location->OpenLight();
                    if(!moveCameraToSUTPRPos(is_local))
                    {
                        int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                        QString operation = waitMessageReturn(is_run,alarm_id);
                        if(!is_run)break;
                        if(RETRY_OPERATION == operation)
                            continue;
                    }
                    if((!performSUTProductPR()))
                    {
                        int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                        QString operation = waitMessageReturn(is_run,alarm_id);
                        if(!is_run)break;
                        if(REJECT_OPERATION == operation)
                        {
                            if(is_local)
                                states.setSut2MaterialState(MaterialState::IsEmpty);
                            else
                                states.setSut1MaterialState(MaterialState::IsEmpty);
                            //uph统计
                            product_uph.addCurrentNumber(updateAccumulatedHour());
                            if(has_product)
                            {
                                product_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                                if(is_local)
                                {
                                    right_product_uph.addCurrentNumber(updateAccumulatedHour(false));
                                    if(has_product)
                                        right_product_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                                    right_comprehensive_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                                }
                                else
                                {
                                    left_product_uph.addCurrentNumber(updateAccumulatedHour(false));
                                    if(has_product)
                                        left_product_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                                    left_comprehensive_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                                }
                            }
                            comprehensive_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                            continue;
                        }
                        else if(RETRY_OPERATION == operation)
                            continue;
                    }
                }
                else
                {
                    sut_product_location->resetResult();
                }
                //走偏移值
                checkPicker2HasMateril();
                if(!movePicker2ToSUTPos(is_local,true))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                    if(!is_run)break;
                }
                if(!waitPicker2CheckResult(false))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
                if(!pickProductFromSut(is_local))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(CONTINUE_OPERATION == operation)
                    {
                        if(has_product)
                            states.setPicker2MaterialState(MaterialState::IsGoodProduct);
                        else
                            states.setPicker2MaterialState(MaterialState::IsNgProduct);
                        picker2_senseor_data["WorkStation"] = states.busyState();
                    }
                    else if(RETRY_OPERATION == operation)
                        continue;
                }
                else
                {
                    if(has_product)
                        states.setPicker2MaterialState(MaterialState::IsGoodProduct);
                    else
                        states.setPicker2MaterialState(MaterialState::IsNgProduct);
                    picker2_senseor_data["WorkStation"] = states.busyState();
                }
                if(states.crossTrayPlaceProduct()&&(!is_local))
                {
                    if(!sut1_sensor_data.contains("SensorCount"))
                        sut1_sensor_data.insert("SensorCount",0);
                    sut1_sensor_data["SensorCount"] = sut1_sensor_data["SensorCount"].toInt() + 1;
                    qInfo("sut SensorCount %d",sut1_sensor_data["SensorCount"].toInt());
                }

                if(is_local)
                    states.setSut2MaterialState(MaterialState::IsEmpty);
                else
                    states.setSut1MaterialState(MaterialState::IsEmpty);
                //uph统计
                product_uph.addCurrentNumber(updateAccumulatedHour());
                if(has_product)
                {
                    product_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    if(is_local)
                    {
                        right_product_uph.addCurrentNumber(updateAccumulatedHour(false));
                        if(has_product)
                            right_product_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                        right_comprehensive_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    }
                    else
                    {
                        left_product_uph.addCurrentNumber(updateAccumulatedHour(false));
                        if(has_product)
                            left_product_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                        left_comprehensive_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    }
                }
                comprehensive_uph.addCurrentReslutNumber(updateAccumulatedHour(false));

                if(!is_run)break;
            }
            //取NGSensor
            bool has_ng_sensor = is_local?states.sut2MaterialState()==MaterialState::IsNgSensor:states.sut1MaterialState()==MaterialState::IsNgSensor;
            if(has_ng_sensor&&(states.picker2MaterialState() == MaterialState::IsEmpty))
            {
                //SUT Sensor视觉
                if(parameters.enableNgSensorPr())
                {
                    sut_sensor_location->OpenLight();
                    if(!moveCameraToSUTPRPos(is_local))
                    {
                        int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                        QString operation = waitMessageReturn(is_run,alarm_id);
                        if(!is_run)break;
                        if(RETRY_OPERATION == operation)
                            continue;
                    }
                    if((!performSUTSensorPR()))
                    {
                        int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                        QString operation = waitMessageReturn(is_run,alarm_id);
                        if(!is_run)break;
                        if(REJECT_OPERATION == operation)
                        {
                            if(is_local)
                                states.setSut2MaterialState(MaterialState::IsEmpty);
                            else
                                states.setSut1MaterialState(MaterialState::IsEmpty);
                            //uph统计
                            sensor_uph.addCurrentReslutNumber(updateAccumulatedHour());
                            if(isLocalHost)
                                right_sensor_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                            else
                                left_sensor_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                            continue;
                        }
                        if(RETRY_OPERATION == operation)
                            continue;
                    }
                }
                else
                {
                    sut_sensor_location->resetResult();
                }
                //走偏移值
                checkPicker2HasMateril();
                if(!movePicker2ToSUTPos(is_local,true))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
                if(!waitPicker2CheckResult(false))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
                if(!pickNgSensorFromSut(is_local))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(CONTINUE_OPERATION == operation)
                    {
                        picker2_senseor_data["WorkStation"] = states.busyState();
                        states.setPicker2MaterialState(MaterialState::IsNgSensor);
                    }
                    else if(RETRY_OPERATION == operation)
                        continue;
                }
                else
                {
                    picker2_senseor_data["WorkStation"] = states.busyState();
                    states.setPicker2MaterialState(MaterialState::IsNgSensor);
                }
                if(states.crossTrayPlaceProduct()&&(!is_local))
                {
                    if(!sut1_sensor_data.contains("SensorCount"))
                        sut1_sensor_data.insert("SensorCount",0);
                    sut1_sensor_data["SensorCount"] = sut1_sensor_data["SensorCount"].toInt() + 1;
                    qInfo("sut SensorCount %d",sut1_sensor_data["SensorCount"].toInt());
                }
                if(is_local)
                    states.setSut2MaterialState(MaterialState::IsEmpty);
                else
                    states.setSut1MaterialState(MaterialState::IsEmpty);
                //uph统计
                sensor_uph.addCurrentReslutNumber(updateAccumulatedHour());
                if(isLocalHost)
                    right_sensor_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                else
                    left_sensor_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                if(!is_run)break;
            }
            //放sensor
            bool need_sensor = is_local?(!states.station2Unload()):(!states.station1Unload());
            if(!need_sensor)
            {
                if(is_local)
                {
                    states.setStation2HasRequest(false);
                    states.setSut2Ready(false);
                    QJsonObject param;
                    param.insert("MaterialState",MaterialTray::getMaterialStateName(states.sut2MaterialState()));
                    sendMessageToModule("Sut2Module","FinishLoadSensor",param);
                }
                else
                {
                    states.setStation1HasRequest(false);
                    states.setSut1Ready(false);
                    QJsonObject param;
                    param.insert("MaterialState",MaterialTray::getMaterialStateName(states.sut1MaterialState()));
                    sendMessageToModule("Sut1Module","FinishLoadSensor",param);
                }
                states.setBusyState(BusyState::IDLE);
                continue;
            }
            bool sut_empty = is_local?states.sut2MaterialState()==MaterialState::IsEmpty:states.sut1MaterialState()==MaterialState::IsEmpty;
            if(sut_empty&&(states.picker1MaterialState() == MaterialState::IsRawSensor))
            {
                //放sensor到SUT
                sut_empty_location->resetResult();
                if(!movePicker1ToSUTPos(is_local))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
                if(!placeSensorToSUT(is_local))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(REJECT_OPERATION == operation)
                    {
                        states.setPicker1MaterialState(MaterialState::IsEmpty);
                        continue;
                    }
                    else if(RETRY_OPERATION == operation)
                        continue;
                }
                states.setPicker1MaterialState(MaterialState::IsEmpty);
                if(is_local)
                {
                    states.setSut2MaterialState(MaterialState::IsRawSensor);
                    states.setStation2NeedSensor(false);
                    states.setStation2HasRequest(false);
                    states.setSut2Ready(false);
                    QJsonObject param;
                    param.insert("MaterialState",MaterialTray::getMaterialStateName(states.sut2MaterialState()));
                    qInfo("sut2MaterialState %s",MaterialTray::getMaterialStateName(states.sut2MaterialState()).toStdString().c_str());
                    sendMessageToModule("Sut2Module","FinishLoadSensor",param);
                }
                else
                {
                    states.setSut1MaterialState(MaterialState::IsRawSensor);
                    states.setStation1NeedSensor(false);
                    states.setStation1HasRequest(false);
                    states.setSut1Ready(false);
                    QJsonObject param;
                    param.insert("MaterialState",MaterialTray::getMaterialStateName(states.sut1MaterialState()));
                    qInfo("sut1MaterialState %s",MaterialTray::getMaterialStateName(states.sut2MaterialState()).toStdString().c_str());
                    sendMessageToModule("Sut1Module","FinishLoadSensor",param);
                }
                states.setBusyState(BusyState::IDLE);
                //uph 统计
                sensor_uph.addCurrentNumber(updateAccumulatedHour());
                comprehensive_uph.addCurrentNumber(updateAccumulatedHour(false));
                if(is_local)
                {
                    right_sensor_uph.addCurrentNumber(updateAccumulatedHour(false));
                    right_comprehensive_uph.addCurrentNumber(updateAccumulatedHour(false));
                }
                else
                {
                    left_sensor_uph.addCurrentNumber(updateAccumulatedHour(false));
                    left_comprehensive_uph.addCurrentNumber(updateAccumulatedHour(false));
                }
                if(!is_run)break;
            }
        }
    }
    states.setRunMode(RunMode::Normal);
    qInfo("sensor Loader Module stoped");
}


void SensorLoaderModule::resetLogic()
{
    if(is_run)
    {
        sendAlarmMessage(OK_OPERATION,u8"程序自动模式运行中，禁止重置逻辑.",ErrorLevel::TipNonblock);
        return;
    }
    tray->resetAllTrayState();
    states.reset();
    qInfo("SensorLoaderModule resetLogic");
}

void SensorLoaderModule::cameraTipOffsetCalibration(int pickhead)
{
    qInfo("Start camera tip offset calibration: %d", pickhead);
//    moveToStartPos(1);
//    QThread::msleep(1000);

    PrOffset offset;
    this->sensor_pickarm_calibration_glass_location->performPR(offset);
    std::vector<cv::Point2d> points;
    if (pickhead == 1)
    {
        this->pick_arm->stepMove_XYT1_Synic(-offset.X,-offset.Y, -50);
        for (int i = 0; i < 5; i++)
        {
            PrOffset offset;
            QThread::msleep(1000);
            this->sensor_pickarm_calibration_glass_location->performPR(offset);
            qInfo("PR offset: %f %f", offset.X, offset.Y);
            points.push_back(cv::Point2d(offset.X, offset.Y));
            this->pick_arm->stepMove_XYT1_Synic(picker1_offset.X(), picker1_offset.Y(), 0);
            bool result = pick_arm->Z1MeasureHeight(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),parameters.holdTime());
            QThread::msleep(1000);
            result &= pick_arm->Z1SerchReturn(30000);
            this->pick_arm->stepMove_XYT1_Synic(0, 0, 20);
            result = pick_arm->Z1MeasureHeight(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),parameters.holdTime());
            QThread::msleep(1000);
            result &= pick_arm->Z1SerchReturn(30000);
            this->pick_arm->stepMove_XYT1_Synic(-picker1_offset.X(), -picker1_offset.Y(), 0);
        }
        {//放回去
            this->pick_arm->stepMove_XYT1_Synic(picker1_offset.X(), picker1_offset.Y(), 0);
            bool result = pick_arm->Z1MeasureHeight(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),parameters.holdTime());
            QThread::msleep(1000);
            result &= pick_arm->Z1SerchReturn(30000);
            this->pick_arm->stepMove_XYT1_Synic(0, 0, -50);
            result = pick_arm->Z1MeasureHeight(parameters.vcmWorkSpeed(),parameters.vcmWorkForce());
            QThread::msleep(1000);
            result &= pick_arm->Z1SerchReturn(30000);
            this->pick_arm->stepMove_XYT2_Synic(-picker1_offset.X(), -picker1_offset.Y(), 0);
        }
        cv::Point2d center; double radius;
        fitCircle(points, center, radius);
        qInfo("Fit cicle: x: %f y: %f r:%f", center.x, center.y, radius);
        this->picker1_offset.setX(round(1000*(picker1_offset.X() + center.x))/1000);
        this->picker1_offset.setY(round(1000*(picker1_offset.Y() + center.y))/1000);
    } else {
        this->pick_arm->stepMove_XYT2_Synic(-offset.X,-offset.Y, -50);
        for (int i = 0; i < 5; i++)
        {
            PrOffset offset;
            QThread::msleep(1000);
            this->sensor_pickarm_calibration_glass_location->performPR(offset);
            qInfo("PR offset: %f %f", offset.X, offset.Y);
            points.push_back(cv::Point2d(offset.X, offset.Y));
            this->pick_arm->stepMove_XYT2_Synic(picker2_offset.X(), picker2_offset.Y(), 0);
            bool result = pick_arm->Z2MeasureHeight(parameters.vcmWorkSpeed(),parameters.pickProductForce(),parameters.holdTime());
            QThread::msleep(1000);
            result &= pick_arm->Z2SerchReturn(30000);
            this->pick_arm->stepMove_XYT2_Synic(0, 0, 20);
            result = pick_arm->Z2MeasureHeight(parameters.vcmWorkSpeed(),parameters.pickProductForce(),parameters.holdTime());
            QThread::msleep(1000);
            result &= pick_arm->Z2SerchReturn(30000);
            this->pick_arm->stepMove_XYT2_Synic(-picker2_offset.X(), -picker2_offset.Y(), 0);
        }
        {//放回去
            this->pick_arm->stepMove_XYT2_Synic(picker2_offset.X(), picker2_offset.Y(), 0);
            bool result = pick_arm->Z2MeasureHeight(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),parameters.holdTime());
            QThread::msleep(1000);
            result &= pick_arm->Z1SerchReturn(30000);
            this->pick_arm->stepMove_XYT2_Synic(0, 0, -50);
            result = pick_arm->Z2MeasureHeight(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),parameters.holdTime());
            QThread::msleep(1000);
            result &= pick_arm->Z1SerchReturn(30000);
            this->pick_arm->stepMove_XYT2_Synic(-picker2_offset.X(), -picker2_offset.Y(), 0);
        }
        cv::Point2d center; double radius;
        fitCircle(points, center, radius);
        qInfo("Fit cicle: x: %f y: %f r:%f", center.x, center.y, radius);
        this->picker2_offset.setX(round(1000*(picker2_offset.X() + center.x))/1000);
        this->picker2_offset.setY(round(1000*(picker2_offset.Y() + center.y))/1000);
    }
}

bool SensorLoaderModule::checkTrayNeedChange()
{
    if(states.handlyChangeTray())
    {
        if(tray->isTrayNeedChange(SensorPosition::SENSOR_TRAY_1)&&
                tray->isTrayNeedChange(SensorPosition::SENSOR_TRAY_2)&&
                (states.picker2MaterialState()==MaterialState::IsEmpty)&&
                (states.picker1MaterialState()==MaterialState::IsEmpty)&&
                (states.sut1MaterialState()==MaterialState::IsEmpty)&&
                (states.sut2MaterialState()==MaterialState::IsEmpty))
            return true;
    }
    else
    {
        if(states.crossTrayPlaceProduct())
        {
            if(sut1_sensor_data["SensorCount"].toInt() >= 2 &&
                    states.picker2MaterialState()==MaterialState::IsEmpty&&states.hasSensorTray1())
            {
                qInfo("sut SensorCount %d",sut1_sensor_data["SensorCount"].toInt());
                sut1_sensor_data["SensorCount"] = 0;
                return true;
            }
            if((!tray->findLastPositionOfState(MaterialState::IsEmpty,SensorPosition::SENSOR_TRAY_2))&&states.hasSensorTray1())
            {
                qInfo("sut SensorCount %d",sut1_sensor_data["SensorCount"].toInt());
                return true;
            }
        }
        else
        {
            if(states.hasSensorTray1()&&
                    tray->isTrayNeedChange(SensorPosition::SENSOR_TRAY_1)&&
                    tray->isTrayNeedChange(SensorPosition::SENSOR_TRAY_2)&&
                    (states.picker1MaterialState() != MaterialState::IsGoodProduct))
                return true;
        }

    }
    return false;
}

int SensorLoaderModule::checkForceChageStation()
{
    if(states.handlyChangeTray())
    {
        if(tray->isTrayNeedChange(SensorPosition::SENSOR_TRAY_1)&&
                tray->isTrayNeedChange(SensorPosition::SENSOR_TRAY_2)&&
                (states.picker2MaterialState()==MaterialState::IsEmpty)&&
                (states.picker1MaterialState()==MaterialState::IsEmpty))
        {
            if((states.busyState() == BusyState::SUT1)&&states.sut2Ready()&&states.station2HasRequest()&&(states.sut2MaterialState()!=MaterialState::IsEmpty)&&(states.sut2MaterialState()!=MaterialState::IsRawSensor))
                return BusyState::SUT2;
            if((states.busyState() == BusyState::SUT2)&&states.sut1Ready()&&states.station1HasRequest()&&(states.sut1MaterialState()!=MaterialState::IsEmpty)&&(states.sut1MaterialState()!=MaterialState::IsRawSensor))
                return BusyState::SUT1;
        }
    }
    return BusyState::IDLE;
}

bool SensorLoaderModule::checkNeedPickSensor()
{
    if(states.station1Unload()&&states.station2Unload())
        return false;
    if(states.disableStation1()&&(states.taskOfStation2() == 1)&&(!states.station2NeedSensor()))
        return false;
    if(states.disableStation2()&&(states.taskOfStation1() == 1)&&(!states.station1NeedSensor()))
        return false;
    if((states.taskOfStation1() == 1)&&(!states.station1NeedSensor())&&(states.taskOfStation2() == 1)&&(!states.station2NeedSensor()))
        return false;
    return true;
}

bool SensorLoaderModule::checkSut1WaitCondition()
{
    if(states.disableStation1()||(states.allowChangeTray()&&states.handlyChangeTray()))
        return false;
    if(states.picker1MaterialState() == MaterialState::IsEmpty&&(states.sut1MaterialState() == MaterialState::IsEmpty))
        return false;
    if((states.busyState() == BusyState::IDLE)&&(states.lastState() == BusyState::SUT2))
        return true;
//todo    if((!states.hasSensorTray1())&&states.sut2MaterialState() != MaterialState::IsEmpty)
    if(states.busyState() == BusyState::SUT1)
        return true;
    if((states.busyState() == BusyState::IDLE)&&states.disableStation2())
        return true;
    return false;
}

bool SensorLoaderModule::checkSut2WaitCondition()
{
    if(states.disableStation2()||(states.allowChangeTray()&&states.handlyChangeTray()))
        return false;
    if(states.picker1MaterialState() == MaterialState::IsEmpty&&(states.sut2MaterialState() == MaterialState::IsEmpty))
        return false;
    if((states.busyState() == BusyState::IDLE)&&(states.lastState() != BusyState::SUT2))
        return true;
    if(states.busyState() == BusyState::SUT2)
        return true;
    if((states.busyState() == BusyState::IDLE)&&states.disableStation1())
        return true;
    return false;
}

int SensorLoaderModule::getTrayIndex()
{
    int work_station = picker2_senseor_data["WorkStation"].toInt();
    //qInfo("work_station : %d",work_station);
    if(states.crossTrayPlaceProduct())
    {
        if(work_station == BusyState::SUT2)
            return SensorPosition::SENSOR_TRAY_1;
        else
            return SensorPosition::SENSOR_TRAY_2;
    }
    else
    {
        if(work_station == BusyState::SUT1)
            return SensorPosition::SENSOR_TRAY_1;
        else
            return SensorPosition::SENSOR_TRAY_2;
    }
}

//bool SensorLoaderModule::moveToSensorTrayNextPos()
//{
//    bool result = tray->findNextPositionOfInitState();
//    QPointF tem_pos = tray->getCurrentPosition(0);
//    if(result)
//        result &=  pick_arm->move_XYT1_Synic(tem_pos.x(),tem_pos.y(),parameters.picker1ThetaOffset());
//    if(!result)
//        AppendError(QString(u8"移动到sensor盘下一个位置失败"));
//    qInfo(u8"移动到sensor号盘下一个位置");
//    return result;
//}

//bool SensorLoaderModule::moveToProductTrayNextPos()
//{
//    bool result = tray->findNextPositionOfInitState(1);
//    if(result)
//        result &=  pick_arm->move_XY_Synic(tray->getCurrentPosition(1));
//    if(!result)
//        AppendError(QString(u8"移动到成品盘下一个位置失败"));
//    qInfo(u8"移动到成品盘下一个位置");
//    return result;
//}

//bool SensorLoaderModule::moveToNgTrayNextPos()
//{
//    bool result_tray = tray->findNextPositionOfInitState(2);
//    bool result = true;
//    if(result_tray)
//        result &=  pick_arm->move_XY_Synic(tray->getCurrentPosition(2));
//    if(!result)
//        AppendError(QString(u8"移动到Ng盘下一个位置失败"));
//    qInfo(u8"移动到Ng盘下一个位置");
//    return result&&result_tray;
//}

bool SensorLoaderModule::findTrayNextSensorPos(bool allow_change_tray)
{
    int tray_index = getTrayIndex();
    if(SensorPosition::SENSOR_TRAY_1 == tray_index)
    {
        if (states.isLastTray())
        {
            if(states.hasSensorTray2()&&tray->findNextPositionOfInitState(SensorPosition::SENSOR_TRAY_2))
            {
                states.setCurrentTrayID(SensorPosition::SENSOR_TRAY_2);
                return true;
            }
            if(!allow_change_tray)
                return false;
        }
        else
        {
            if(states.hasSensorTray1()&&tray->findNextPositionOfInitState(tray_index))
            {
                states.setCurrentTrayID(tray_index);
                return true;
            }
            if(!allow_change_tray)
                return false;
            if(states.hasSensorTray2()&&tray->findNextPositionOfInitState(SensorPosition::SENSOR_TRAY_2))
            {
                states.setCurrentTrayID(SensorPosition::SENSOR_TRAY_2);
                return true;
            }
        }
    }
    else
    {
        if(states.hasSensorTray2()&&tray->findNextPositionOfInitState(tray_index))
        {
            states.setCurrentTrayID(tray_index);
            return true;
        }
        if(!allow_change_tray)
            return false;
        if(states.hasSensorTray1()&&tray->findNextPositionOfInitState(SensorPosition::SENSOR_TRAY_1))
        {
            states.setCurrentTrayID(SensorPosition::SENSOR_TRAY_1);
            return true;
        }
    }
    return false;
}

bool SensorLoaderModule::findTrayNextEmptyPos()
{
    int tray_index = getTrayIndex();
    if(SensorPosition::SENSOR_TRAY_1 == tray_index && (!states.hasSensorTray1()))
        return false;
    if(SensorPosition::SENSOR_TRAY_2 == tray_index && (!states.hasSensorTray2()))
        return false;
    if(tray->findLastPositionOfState(MaterialState::IsEmpty,tray_index))
    {
        if((!states.handlyChangeTray())&&(SensorPosition::SENSOR_TRAY_1 == tray_index)&&(tray->getCurrentIndex(tray_index) > (tray->getLastIndex() - 2)))//tray1最后两个
        {
            if(tray->findLastPositionOfState(MaterialState::IsEmpty,SensorPosition::SENSOR_TRAY_2))
            {
                states.setCurrentTrayID(SensorPosition::SENSOR_TRAY_2);
                return true;
            }
        }
        states.setCurrentTrayID(tray_index);
        return true;
    }
    if(((states.picker1MaterialState() != MaterialState::IsEmpty)
        ||((SensorPosition::SENSOR_TRAY_1 == tray_index)&&states.station1Unload())
        ||((SensorPosition::SENSOR_TRAY_2 == tray_index)&&states.station2Unload()))
            &&findTrayNextInitStatePos(SensorPosition::BUFFER_TRAY))
    {
        states.setCurrentTrayID(SensorPosition::BUFFER_TRAY);
        return true;
    }
    return false;
}

bool SensorLoaderModule::findTrayNextInitStatePos(int tray_index)
{
    if(tray->findNextPositionOfInitState(tray_index))
    {
        states.setCurrentTrayID(tray_index);
        return true;
    }
    return false;
}


bool SensorLoaderModule::moveCameraToTrayCurrentPos(int tray_index, bool check_softlanding)
{
    QElapsedTimer timer; timer.start();
    QString temp;
    pick_arm->setCallerName(__FUNCTION__);
    bool result = pick_arm->move_XY_Synic(tray->getCurrentPosition(tray_index),false,check_softlanding);
    temp.append(" move_XY_Synic ").append(QString::number(timer.elapsed()));
    pick_arm->setCallerName("");
    if(result){
        qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
        return true;
    }
    QString tray_name = tray->getTrayName(tray_index);
    AppendError(QString(u8"移动到盘下一个位置失败").arg(tray_name));
    qInfo(u8"移动到%s下一个位置",tray_name.toStdString().c_str());
    //qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    QString log = QString("[Timelog] ").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
    return false;
}

bool SensorLoaderModule::moveCameraToSUTPRPos(bool is_local,bool check_softlanding)
{
    QElapsedTimer timer; timer.start();
    bool result;
    QPointF temp_pos;
    if(is_local)
        temp_pos =  sut2_pr_position.ToPointF();
    else
        temp_pos =  sut1_pr_position.ToPointF();
    result = pick_arm->move_XYT1T2_Synic(temp_pos.x(),temp_pos.y(),parameters.picker1PlaceTheta(),parameters.picker2PickTheta(),true,check_softlanding);
    if(result) {
        qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
        return true;
    }
    int sut_i = is_local?2:1;
    AppendError(QString(u8"移动SPA相机SUT%1位置失败").arg(sut_i));
    qInfo(u8"移动SPA相机SUT%d位置失败",sut_i);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return false;
}

bool SensorLoaderModule::movePicker1ToSUTPos(bool is_local,bool check_softlanding)
{
    QElapsedTimer timer; timer.start();
    bool result;
    QPointF temp_pos;
    if(is_local)
        temp_pos =  sut2_pr_position.ToPointF() + sut2PlaceOffset.ToPointF();
    else
        temp_pos =  sut1_pr_position.ToPointF() + sut1PlaceOffset.ToPointF();
    double x = temp_pos.x() + picker1_offset.X();
    double y = temp_pos.y() + picker1_offset.Y();
    if(sut_empty_location->parameters.useOrigin())
    {
        QPointF temp_offset = getPickerResultOffset(sut_empty_location->getCurrentResult().Theta);
        x-=temp_offset.x();
        y-=temp_offset.y();
    }
    pick_arm->setCallerName(__FUNCTION__);
    result = pick_arm->move_XYT1_Synic(x,y,parameters.picker1PlaceTheta(),true,check_softlanding);
    pick_arm->setCallerName("");
    if(result) {
        qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
        return true;
    }
    int sut_i = is_local?2:1;
    AppendError(QString(u8"移动sensor吸头到SUT%1位置失败").arg(sut_i));
    qInfo(u8"移动sensor吸头到SUT%d位置失败",sut_i);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return false;
}

bool SensorLoaderModule::movePicker2ToSUTPos(bool is_local,bool is_product,bool check_softlanding)
{
    QElapsedTimer timer; timer.start();
    bool result;
    QPointF temp_pos;
    if(is_local)
        temp_pos =  sut2_pr_position.ToPointF() + sut2PickOffset.ToPointF();
    else
        temp_pos =  sut1_pr_position.ToPointF() + sut1PickOffset.ToPointF();
    PrOffset temp_pr = is_product?sut_product_location->getCurrentResult():sut_sensor_location->getCurrentResult();
    double x = temp_pos.x() + picker2_offset.X() - temp_pr.X;
    double y = temp_pos.y() + picker2_offset.Y() - temp_pr.Y;
    double theta = parameters.picker2PickTheta()  -  temp_pr.Theta;
    bool temp_origin = is_product?sut_product_location->parameters.useOrigin():sut_sensor_location->parameters.useOrigin();
    if(temp_origin)
    {
        QPointF temp_offset = getPickerResultOffset(temp_pr.Theta);
        x-=temp_offset.x();
        y-=temp_offset.y();
    }
    pick_arm->setCallerName(__FUNCTION__);
    result = pick_arm->move_XYT2_Synic(x,y,theta,true,check_softlanding);
    pick_arm->setCallerName("");
    if(result) {
        if (timer.elapsed() > 0)
            qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
        return true;
    }
    int sut_i = is_local?2:1;
    AppendError(QString(u8"移动成品吸头到SUT%1位置失败").arg(sut_i));
    qInfo(u8"移动成品吸头到SUT%d位置失败",sut_i);
    if (timer.elapsed() > 0)
        qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return false;
}

bool SensorLoaderModule::performTraySensorPR()
{
    QElapsedTimer timer; timer.start();
    bool result;
    if(states.runMode() == RunMode::NoMaterial)
        result = tray_sensor_location->performNoMaterialPR();
    else
        result= tray_sensor_location->performPR();
    if(result) //ToDo: Set the boolean for this pr using offset or not
        tray->setTrayCurrentPrOffset(tray_sensor_location->getCurrentResult(false),states.currentTrayID());
    else
        AppendError(QString(u8"执行料盘sensor视觉失败!"));
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return  result;
}

bool SensorLoaderModule::performTrayEmptyPR()
{
    QElapsedTimer timer; timer.start();
    bool result;
    if(states.runMode() == RunMode::NoMaterial)
        result= tray_empty_location->performNoMaterialPR();
    else
        result = tray_empty_location->performPR();
    if(!result)
        AppendError(QString(u8"执行料盘空位视觉失败!"));
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return  result;
}

bool SensorLoaderModule::performSUTEmptyPR()
{
    qInfo("performSUTEmptyPR");
    bool result;
    if(states.runMode() == RunMode::NoMaterial)
        result= sut_empty_location->performNoMaterialPR();
    else
        result = sut_empty_location->performPR();
    if(!result)
        AppendError(QString(u8"执行SUT视觉失败!"));
    return  result;
}

bool SensorLoaderModule::performSUTSensorPR()
{
    qInfo("performSUTSensorPR");
    bool result;
    if(states.runMode() == RunMode::NoMaterial)
        result= sut_sensor_location->performNoMaterialPR();
    else
        result = sut_sensor_location->performPR();
    if(!result)
        AppendError(QString(u8"执行SUT上的sensor视觉失败!"));
    return  result;
}

bool SensorLoaderModule::performSUTProductPR()
{
    QElapsedTimer timer; timer.start();
    qInfo("performSUTProductPR");
    bool result;
    if(states.runMode() == RunMode::NoMaterial)
        result= sut_product_location->performNoMaterialPR();
    else
        result =  sut_product_location->performPR();
    if(!result)
        AppendError(QString(u8"执行SUT视觉失败!"));
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return  result;
}

QPointF SensorLoaderModule::getPickerResultOffset(double theta)
{
    double temp_theta = theta*PI/180;
    double x = parameters.sensorOffsetX()*cos(temp_theta) + parameters.sensorOffsetY()*sin(temp_theta);
    double y = parameters.sensorOffsetY()*cos(temp_theta) - parameters.sensorOffsetX()*sin(temp_theta);
    return QPointF(x,y);
}

void SensorLoaderModule::setSensorOffset()
{
    QPointF temp_offset = tray_sensor_location->getCurrentResultOffset();
    parameters.setSensorOffsetX(temp_offset.x());
    parameters.setSensorOffsetY(temp_offset.y());
}

bool SensorLoaderModule::moveToPRResultPos(PrOffset pr_result,bool check_softlanding)
{
    qInfo("moveToPRResultPos offset:(%f,%f,%f)",-pr_result.X,-pr_result.Y,-pr_result.Theta);
    bool result = pick_arm->stepMove_XY_Synic(-pr_result.X,-pr_result.Y,check_softlanding);
    if(!result)
        AppendError(QString(u8"去视觉结果位置(step x %1,y %2,t %3)失败!").arg(pr_result.X).arg(pr_result.Y).arg(pr_result.Theta));
    return  result;
}

bool SensorLoaderModule::movePicker1ToTrayCurrentPos(int tray_index,bool check_softlanding)
{
    QElapsedTimer timer; timer.start();
    QString temp;
    QPointF next_pos = tray->getCurrentPosition(tray_index);
    PrOffset temp_pr = tray_sensor_location->getCurrentResult();
    double x = next_pos.x() + picker1_offset.X() - temp_pr.X;
    double y = next_pos.y() + picker1_offset.Y() - temp_pr.Y;
    double t = parameters.picker1PickTheta() - temp_pr.Theta;
    if(tray_sensor_location->parameters.useOrigin())
    {
        QPointF temp_offset = getPickerResultOffset(temp_pr.Theta);
        x-=temp_offset.x();
        y-=temp_offset.y();
    }
    if (tray_index == SensorPosition::SENSOR_TRAY_1)
    {
        x += sensorTray1PickOffset.X();
        y += sensorTray1PickOffset.Y();
    }
    else if (tray_index == SensorPosition::SENSOR_TRAY_2)
    {
        x += sensorTray2PickOffset.X();
        y += sensorTray2PickOffset.Y();
    }
    pick_arm->setCallerName(__FUNCTION__);
    bool result = pick_arm->move_XYT1_Synic(x,y,t,false,check_softlanding);
    temp.append(" move_XYT1_Synic ").append(QString::number(timer.elapsed()));
    pick_arm->setCallerName("");
    if(result) {
        qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
        return true;
    }
    AppendError(QString(u8"移动1号吸头到位置(x %1,y %2,t %3)失败!").arg(x).arg(y).arg(t));
    qInfo(u8"移动1号吸头到位置(x %f,y %f,t %f)失败!",x,y,t);
    //qWarning("[Timelog] %s %d %f", __FUNCTION__, timer.elapsed());
    QString log = QString("[Timelog] ").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
    return  false;
}

bool SensorLoaderModule::movePicker2ToTrayCurrentPos(int tray_index,bool check_softlanding)
{
    QElapsedTimer timer; timer.start();
    QPointF next_pos = tray->getCurrentPosition(tray_index);
    PrOffset temp_pr = tray_empty_location->getCurrentResult();
    double x = next_pos.x() + picker2_offset.X() - temp_pr.X;
    double y = next_pos.y() + picker2_offset.Y() - temp_pr.Y;
    double t = parameters.picker2PlaceTheta() - temp_pr.Theta;
    if (tray_index == SensorPosition::SENSOR_TRAY_1)
    {
        x += sensorTray1PlaceOffset.X();
        y += sensorTray1PlaceOffset.Y();
    }
    else if (tray_index == SensorPosition::SENSOR_TRAY_2)
    {
        x += sensorTray2PlaceOffset.X();
        y += sensorTray2PlaceOffset.Y();
    }
    else if (tray_index == SensorPosition::NG_SENSOR_TRAY)
    {
        x += ngTrayPlaceOffset.X();
        y += ngTrayPlaceOffset.Y();
    }
    else if (SensorPosition::BUFFER_TRAY)
    {
        x += bufferTrayPlaceOffset.X();
        y += bufferTrayPlaceOffset.Y();
    }
    qInfo("movePicker2ToTrayCurrentPos x: %f y: %f temp_pr_x: %f temp_pr_y: %f", x, y, temp_pr.X, temp_pr.Y);
    if(tray_empty_location->parameters.useOrigin())
    {
        qInfo("tray_empty_location->parameters.useOrigin()");
        QPointF temp_offset = getPickerResultOffset(temp_pr.Theta);
        x-=temp_offset.x();
        y-=temp_offset.y();
    }
    pick_arm->setCallerName(__FUNCTION__);
    bool result = pick_arm->move_XYT2_Synic(x,y,t,false,check_softlanding);
    pick_arm->setCallerName("");
    if(result) {
        qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
        return true;
    }
    AppendError(QString(u8"移动成品吸头到位置(x %1,y %2,t %3)失败!").arg(x).arg(y).arg(t));
    qInfo(u8"移动成品吸头到位置(x %f,y %f,t %f)失败!",x,y,t);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return false;
}

bool SensorLoaderModule:: picker1PickFromTray(double z, int time_out)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    QString temp;
    if(parameters.openTimeLog())
        qInfo("picker1PickFromTray z %f time_out %d",z,time_out);
    bool result = true;
    if(parameters.disablePickFromTrayForceLimit())
    {
        smallTimer.restart();
        double dist_z = fabs(pick_arm->picker1->motor_z->GetFeedbackPos() - z + parameters.pickFromTrayMargin());
        result = pick_arm->MoveZ1Synic(z - parameters.pickFromTrayMargin(),time_out);
        temp.append(" dist_z ").append(QString::number(dist_z))
            .append(" MoveZ1Synic ").append(QString::number(smallTimer.elapsed()));
    }
    else
    {
        smallTimer.restart();
        double dist_z = fabs(pick_arm->picker1->motor_z->GetFeedbackPos() - z + parameters.pickFromTrayMargin());
        result = pick_arm->Z1SearchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),time_out);
        temp.append(" dist_z ").append(QString::number(dist_z))
            .append(" Z1SearchByForce ").append(QString::number(smallTimer.elapsed()));
    }
    if(parameters.openTimeLog())
        qInfo("picker2PlaceToTray pick result %d",result);
    smallTimer.restart();
    setPicker1Vacuum(true);
    temp.append(" setPicker1Vacuum ").append(QString::number(smallTimer.elapsed()));
    if(parameters.disablePickFromTrayForceLimit())
    {
        smallTimer.restart();
        double dist_z = fabs(pick_arm->picker1->motor_z->GetFeedbackPos() - pick_arm->parameters.motor1SafeHeight());
        result &= pick_arm->MoveZ1ToSafeHeighSync();
        temp.append(" dist_z ").append(QString::number(dist_z))
            .append(" MoveZ1ToSafeHeighSync ").append(QString::number(smallTimer.elapsed()));
    }
    else {
        smallTimer.restart();
        result &= pick_arm->Z1SerchReturn(time_out);
        temp.append(" Z1SerchReturn ").append(QString::number(smallTimer.elapsed()));
    }
    if(parameters.openTimeLog())
        qInfo("picker1PickFromTray return result %d",result);
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
    return result;
}

bool SensorLoaderModule::picker1BackToTray(double z, int time_out)
{
    if(parameters.openTimeLog())
        qInfo("picker1BackToTray z %f time_out %d",z,time_out);
    bool result = true;
    if(parameters.disableBackToTrayForceLimit())
        result = pick_arm->MoveZ1Synic(z - parameters.backToTrayMargin(),time_out);
        else
        result = pick_arm->Z1SearchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),time_out);
    if(parameters.openTimeLog())
        qInfo("picker1BackToTray place result %d",result);
    setPicker1Vacuum(false);
    if(parameters.disableBackToTrayForceLimit())
        result &= pick_arm->MoveZ1ToSafeHeighSync();
    else
        result &= pick_arm->Z1SerchReturn(time_out);
    if(parameters.openTimeLog())
        qInfo("picker1BackToTray return result %d",result);
    return result;
}

bool SensorLoaderModule::picker1PlaceToSut(double z, bool is_local, int time_out)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    QString temp;
    if(parameters.openTimeLog())
        qInfo("picker1PlaceToSut z %f is_local %d time_out %d",z,is_local,time_out);
    bool result = true;
    if(parameters.enableEscape())
    {
        pick_arm->setCallerName(__FUNCTION__);
        smallTimer.restart();
        result = pick_arm->move_XeYe_Z1_XY(z - parameters.escapeHeight(),parameters.escapeX(),parameters.escapeY());
        pick_arm->setCallerName("");
        temp.append(" move_XeYe_Z1_XY ").append(QString::number(smallTimer.elapsed()));
        if(parameters.openTimeLog())
            qInfo("picker1PlaceToSut escape result %d",result);
    }
    if(result)
    {
        if(parameters.disablePlaceToSutForceLimmit())
        {
            double margin = is_local?parameters.pickFromSut2Margin():parameters.pickFromSut1Margin();
            smallTimer.restart();
            double dist_z = fabs( pick_arm->picker1->motor_z->GetFeedbackPos() - margin);
            result = pick_arm->MoveZ1Synic(z - margin,time_out);
            temp.append(" dist_z ").append(QString::number(dist_z))
                .append(" move_z1 ").append(QString::number(smallTimer.elapsed()));
        }
        else
        {
            double dist_z = fabs( pick_arm->picker1->motor_z->GetFeedbackPos() - parameters.vcmMargin());
            smallTimer.restart();
            result = pick_arm->Z1SearchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),time_out);
            temp.append(" dist_z ").append(QString::number(dist_z))
                .append(" search_z1 ").append(QString::number(smallTimer.elapsed()));
        }
        if(parameters.openTimeLog())
            qInfo("picker1PlaceToSut pick result %d",result);
        is_local?openSut2Vacuum():openSut1Vacuum();
        smallTimer.restart();
        setPicker1Vacuum(false);
        result &= waitSutVacuumFinish();
        temp.append(" sut_vacuum_finish ").append(QString::number(smallTimer.elapsed()));
        if(parameters.openTimeLog())
            qInfo("picker1PlaceToSut vacuum result %d",result);
        if(parameters.disablePlaceToSutForceLimmit()&&(!parameters.enableEscape()))
        {
            smallTimer.restart();
            result &= pick_arm->MoveZ1ToSafeHeighSync();
            temp.append(" MoveZ1ToSafeHeighSync ").append(QString::number(smallTimer.elapsed()));
        }
        else
        {
            smallTimer.restart();
            result &= pick_arm->Z1SerchReturn(time_out);
            temp.append(" Z1SerchReturn ").append(QString::number(smallTimer.elapsed()));
        }
        if(parameters.openTimeLog())
            qInfo("picker1PlaceToSut return result %d",result);
    }
    if(parameters.enableEscape())
    {
        smallTimer.restart();
        result &= pick_arm->MoveZ1ToSafeHeighSync();
        temp.append(" MoveZ1ToSafeHeighSync ").append(QString::number(smallTimer.elapsed()));
        if(parameters.openTimeLog())
            qInfo("picker1PlaceToSut escape return result %d",result);
    }
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
    return result;
}

bool SensorLoaderModule::picker2PickFromSut(double z,double force, bool is_local, int time_out)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    QString temp;

    if(parameters.openTimeLog())
        qInfo("picker2PickFromSut z %f force %f is_local %d is_open %d time_out %d",z,force,is_local,true,time_out);
    is_local?closeSut2Vacuum():closeSut1Vacuum();
    smallTimer.restart();
    pick_arm->picker2->vacuum->Set(true,false);
    temp.append(" picker_2_vacuum ").append(QString::number(smallTimer.elapsed()));
    bool result = true;
    if(parameters.enableEscape()&&(!parameters.disablePickFromSutForceLimit()))
    {
        smallTimer.restart();
        double dist_z = fabs(pick_arm->picker2->motor_z->GetFeedbackPos() - parameters.escapeHeight());
        result = pick_arm->picker2->motor_z->MoveToPosSync(z - parameters.escapeHeight());
        temp.append(" dist_z ").append(QString::number(dist_z))
            .append(" z_move ").append(QString::number(smallTimer.elapsed()));
        if(parameters.openTimeLog())
            qInfo("motor_z move  to escape %f result %d",z,result);
    }
    if(result)
    {
        if(parameters.disablePickFromSutForceLimit())
        {
            double margin = is_local?parameters.pickFromSut2Margin():parameters.pickFromSut1Margin();
            double dist_z = fabs(pick_arm->picker2->motor_z->GetFeedbackPos() - (z-margin));
            smallTimer.restart();
            result &= pick_arm->MoveZ2Synic(z - margin,time_out);
            temp.append(" dist_z ").append(QString::number(dist_z))
                .append(" z_move ").append(QString::number(smallTimer.elapsed()));
        }
        else
        {
            double dist_z = fabs(pick_arm->picker2->motor_z->GetFeedbackPos() - (z-parameters.vcmMargin()));
            smallTimer.restart();
            result &= pick_arm->Z2SerchByForce(parameters.vcmWorkSpeed(),force,z,parameters.vcmMargin(),time_out);
            temp.append(" dist_z ").append(QString::number(dist_z))
                .append(" z_search ").append(QString::number(smallTimer.elapsed()));
        }
        smallTimer.restart();
        result &= waitSutVacuumFinish();
        temp.append(" waitSutVacuumFinish ").append(QString::number(smallTimer.elapsed()));

        if(parameters.openTimeLog())
            qInfo("motor_z move to %f result %d",z,result);
        smallTimer.restart();
        setPicker2Vacuum(true);
        temp.append(" setPicker2Vacuum ").append(QString::number(smallTimer.elapsed()));

        smallTimer.restart();
        if(parameters.disablePickFromSutForceLimit())
        {
            if(parameters.enableEscape()) {
                double dist_z = fabs(pick_arm->picker2->motor_z->GetFeedbackPos() - (z-parameters.escapeHeight()));
                result = pick_arm->picker2->motor_z->MoveToPosSync(z - parameters.escapeHeight());
                temp.append(" dist_z ").append(QString::number(dist_z))
                    .append(" z_move ").append(QString::number(smallTimer.elapsed()));
            }
            else {
                result &= pick_arm->MoveZ2ToSafeHeighSync();
                temp.append(" MoveZ2ToSafeHeighSync ").append(QString::number(smallTimer.elapsed()));
            }
        }
        else {
            result &= pick_arm->Z2SerchReturn(time_out);
            temp.append(" Z2SerchReturn ").append(QString::number(smallTimer.elapsed()));
        }
        if(parameters.openTimeLog())
            qInfo("motor_z move return result %d",result);
    }
    if(parameters.enableEscape()){
        smallTimer.restart();
        pick_arm->setCallerName(__FUNCTION__);
        result &= pick_arm->move_XeYe_Z2(pick_arm->parameters.motor2SafeHeight(),parameters.escapeX(),parameters.escapeY());
        pick_arm->setCallerName("");
        temp.append(" move_XeYe_Z2 ").append(QString::number(smallTimer.elapsed()));
    }
    if(parameters.openTimeLog())
        qInfo("picker2PickFromSut result %d",result);
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
    return result;
}

bool SensorLoaderModule::picker2PlaceToTray(double z,double force,bool is_product, int time_out)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    QString temp;
    if(parameters.openTimeLog())
        qInfo("picker2PlaceToTray z %f time_out %d",z,time_out);
    bool result = true;
    bool disable = is_product?parameters.disablePlaceToGoodTrayForceLimit():parameters.disablePlaceToNgTrayForceLimit();
    double margin = is_product?parameters.placeToGoodTrayMargin():parameters.placeToNgTrayMargin();
    if(disable) {
        smallTimer.restart();
        double dist_z = fabs(pick_arm->picker2->motor_z->GetFeedbackPos() - (z-margin));
        result = pick_arm->MoveZ2Synic(z - margin,time_out);
        temp.append(" dist_z ").append(QString::number(dist_z))
            .append(" z_move ").append(QString::number(smallTimer.elapsed()));
    }
    else {
        smallTimer.restart();
        double dist_z = fabs(pick_arm->picker2->motor_z->GetFeedbackPos() - (z-parameters.vcmMargin()));
        result = pick_arm->Z2SerchByForce(parameters.vcmWorkSpeed(),force,z,parameters.vcmMargin(),time_out);
        temp.append(" dist_z ").append(QString::number(dist_z))
            .append(" z_search ").append(QString::number(smallTimer.elapsed()));
    }
    if(parameters.openTimeLog())
        qInfo("picker2PlaceToTray pick result %d",result);
    smallTimer.restart();
    setPicker2Vacuum(false);
    temp.append(" setPicker2Vacuum ").append(QString::number(smallTimer.elapsed()));
    if(disable)
    {
        smallTimer.restart();
        result &= pick_arm->MoveZ2ToSafeHeighSync();
        temp.append(" MoveZ2ToSafeHeighSync ").append(QString::number(smallTimer.elapsed()));
    }
    else
    {
        smallTimer.restart();
        result &= pick_arm->Z2SerchReturn(time_out);
        temp.append(" Z2SerchReturn ").append(QString::number(smallTimer.elapsed()));
    }
    if(parameters.openTimeLog())
        qInfo("picker2PlaceToTray return result %d",result);
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
    return result;
}

void SensorLoaderModule::openSut1Vacuum()
{
    states.setVacuumOperationResult(0);
    sendMessageToModule("LogicManager1","OpenSutVacuum");
}

void SensorLoaderModule::closeSut1Vacuum()
{
    states.setVacuumOperationResult(0);
    sendMessageToModule("LogicManager1","CloseSutVacuum");
}

bool SensorLoaderModule::waitSutVacuumFinish()
{
    int current_time = 0;
    while (current_time < parameters.vacuumOperationOutTime())
    {
        if(states.vacuumOperationResult() != 0)
        {
            qInfo("waitSutVacuumFinish time %d",current_time);
            return (states.vacuumOperationResult() > 0||states.runMode()== RunMode::NoMaterial);
        }
        current_time +=10;
        QThread::msleep(10);
    }
    qInfo("waitSutVacuumFinish fail ,out time %d",parameters.vacuumOperationOutTime());
    return false;
}

void SensorLoaderModule::openSut2Vacuum()
{
    states.setVacuumOperationResult(0);
    sendMessageToModule("LogicManager2","OpenSutVacuum");
}

void SensorLoaderModule::closeSut2Vacuum()
{
    states.setVacuumOperationResult(0);
    sendMessageToModule("LogicManager2","CloseSutVacuum");
}

//bool SensorLoaderModule::checkPickedSensor(bool check_state)
//{
//    bool result;
//    result = pick_arm->picker1->vacuum->checkHasMaterielSync();
//    if(result == check_state)
//        return true;
//    QString error = QString(u8"sensor吸头上逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
//    AppendError(error);
//    qInfo(error.toStdString().c_str());
//    return false;
//}

//bool SensorLoaderModule::checkPickedNgOrProduct(bool check_state)
//{
//    if(parameters.openTimeLog())
//        qInfo("checkPickedNgOrProduct check_state %d",check_state);
//    bool result;
//    result = pick_arm->picker2->vacuum->checkHasMaterielSync();
//    if(parameters.openTimeLog())
//        qInfo("checkPickedNgOrProduct result %d",result);
//    if(result == check_state)
//        return true;
//    QString error = QString(u8"成品吸头上逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
//    AppendError(error);
//    qInfo(error.toStdString().c_str());
//    return false;
//}

bool SensorLoaderModule::checkPicker1HasMaterialSync()
{
    if(parameters.openTimeLog())
        qInfo("checkPicker1HasMaterialSync start");
    bool result;
    result = pick_arm->picker1->vacuum->checkHasMaterielSync();
    if(parameters.openTimeLog())
        qInfo("checkPicker1HasMaterialSync result %d",result);
    if(result||RunMode::NoMaterial == states.runMode())
        return true;
    QString error = QString(u8"sensor吸头上应有料，但真空检测未通过。");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool SensorLoaderModule::checkPicker2HasMaterialSync()
{
    QElapsedTimer timer; timer.start();
    if(parameters.openTimeLog())
        qInfo("checkPicker2HasMaterialSync start");
    bool result;
    result = pick_arm->picker2->vacuum->checkHasMaterielSync();
    if(parameters.openTimeLog())
        qInfo("checkPicker2HasMaterialSync result %d",result);
    if(result||RunMode::NoMaterial == states.runMode())
    {
        qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
        return true;
    }
    QString error = QString(u8"成品吸头上应有料，但真空检测未通过。");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return false;
}

bool SensorLoaderModule::checkPicker2HasMateril()
{
    QElapsedTimer timer; timer.start();
    if(parameters.openTimeLog())
        qInfo("checkPicker2HasMateril start");
    bool result;
    result = pick_arm->picker2->vacuum->checkHasMateriel(thread_id);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool SensorLoaderModule::waitPicker2CheckResult(bool check_state)
{
    QElapsedTimer timer; timer.start();
    if(parameters.openTimeLog())
        qInfo("waitPicker1CheckResult start");
    bool result;
    result = pick_arm->picker2->vacuum->getHasMateriel(thread_id);
    if(parameters.openTimeLog())
        qInfo("waitPicker1CheckResult result %d",result);
    if(result == check_state||RunMode::NoMaterial == states.runMode())
    {
        qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
        return true;
    }
    QString error = QString(u8"成品吸头上应无料，但真空检测能通过。");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return false;
}

bool SensorLoaderModule::setPicker1Vacuum(bool state)
{
    if(states.runMode() == RunMode::NoMaterial)
        return pick_arm->picker1->vacuum->SetSimulation(state);
    else
        return pick_arm->picker1->vacuum->Set(state);
}

bool SensorLoaderModule::setPicker2Vacuum(bool state)
{
    if(states.runMode() == RunMode::NoMaterial)
        return pick_arm->picker2->vacuum->SetSimulation(state);
    else
        return pick_arm->picker2->vacuum->Set(state);
}

bool SensorLoaderModule::checkPicker1HasMateril()
{
    QElapsedTimer timer; timer.start();
    if(parameters.openTimeLog())
        qInfo("checkPicker1HasNoMateril start");
    bool result;
    result = pick_arm->picker1->vacuum->checkHasMateriel(thread_id);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool SensorLoaderModule::waitPicker1CheckResult(bool check_state)
{
    QElapsedTimer timer; timer.start();
    if(parameters.openTimeLog())
        qInfo("waitPicker1CheckResult start");
    bool result;
    result = pick_arm->picker1->vacuum->getHasMateriel(thread_id);
    if(parameters.openTimeLog())
        qInfo("waitPicker1CheckResult result %d",result);
    if(result == check_state||RunMode::NoMaterial == states.runMode())
    {
        qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
        return true;
    }
    QString error = QString(u8"Sensor吸头上应无料，但真空检测能通过。");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return false;
}

bool SensorLoaderModule::pickSensorFromTray(int tray_id,int time_out)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    QString temp;
    double temp_z = SensorPosition::SENSOR_TRAY_1 == tray_id?parameters.pickSensorZ():parameters.pickSensorZ2();
    this->setCallerName(__FUNCTION__);
    smallTimer.restart();
    bool result = picker1PickFromTray(temp_z,time_out);
    temp.append(" picker1PickFromTray ").append(QString::number(smallTimer.elapsed()));
    this->setCallerName("");
    if(!result)
        AppendError(QString(u8"从%1号sensor盘取sensor失败.").arg(tray_id + 1));
    if(result)
    {
        smallTimer.restart();
        result &= checkPicker1HasMaterialSync();
        temp.append(" checkPicker1HasMaterialSync ").append(QString::number(smallTimer.elapsed()));
    }
    //qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    QString log = QString("[Timelog] ").append(callerName).append(":").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
    return result;
}

bool SensorLoaderModule::backSensorToTray(int tray_id,int time_out)
{
    double temp_z = SensorPosition::SENSOR_TRAY_1 == tray_id?parameters.pickSensorZ():parameters.pickSensorZ2();
    bool result = picker1BackToTray(temp_z,time_out);
    if(!result)
        AppendError(QString(u8"放回sensor到%1号Sensor盘失败.").arg(tray_id + 1));
    return result;

}

bool SensorLoaderModule::placeSensorToSUT(bool is_local,int time_out)
{
    QElapsedTimer timer; timer.start();
    double placeSensorZ = is_local?parameters.placeSUT2SensorZ():parameters.placeSUT1SensorZ();
    setCallerName(__FUNCTION__);
    bool result = picker1PlaceToSut(placeSensorZ,is_local,time_out);
    setCallerName("");
    if(!result)
        AppendError(QString(u8"放sensor到SUT%1失败").arg(is_local?2:1));
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool SensorLoaderModule::pickNgSensorFromSut(bool is_local,int time_out)
{
    QElapsedTimer timer; timer.start();
    double pickNgSensorZ = is_local?parameters.pickSUT2NgSensorZ():parameters.pickSUT1NgSensorZ();
    setCallerName(__FUNCTION__);
    bool result = picker2PickFromSut(pickNgSensorZ,parameters.pickProductForce(),is_local,time_out);
    setCallerName("");
    if(!result)
        AppendError(QString(u8"从SUT%1取NGsenor失败").arg(is_local?2:1));
    if(result)
        result &= checkPicker2HasMaterialSync();
    if(parameters.openTimeLog())
        qInfo("pickSUTSensor resilt %d",result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool SensorLoaderModule::pickProductFromSut(bool is_local,int time_out)
{
    QElapsedTimer timer; timer.start();
    double pickProductZ = is_local?parameters.pickSUT2ProductZ():parameters.pickSUT1ProductZ();
    bool result = picker2PickFromSut(pickProductZ,parameters.pickProductForce(),is_local,time_out);
    if(!result)
        AppendError(QString(u8"从SUT%1取成品失败").arg(is_local?2:1));
    if(result)
        result &= checkPicker2HasMaterialSync();
    if(parameters.openTimeLog())
        qInfo("pickSUTProduct result %d",result);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool SensorLoaderModule::placeNgSensorToTray(int time_out)
{
    QElapsedTimer timer; timer.start();
    bool result = picker2PlaceToTray(parameters.placeNgSensorZ(),parameters.pickProductForce(),false,time_out);
    if(!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool SensorLoaderModule::placeNgProductToTray(int time_out)
{
    QElapsedTimer timer; timer.start();
    setCallerName(__FUNCTION__);
    bool result = picker2PlaceToTray(parameters.placeNgProductZ(),parameters.pickProductForce(),false,time_out);
    setCallerName("");
    if(!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool SensorLoaderModule::placeProductToTray(int tray_id,int time_out)
{
    QElapsedTimer timer; timer.start();
    qInfo("place product to tray");
    double temp_z = parameters.placeProductZ();
    if(SensorPosition::SENSOR_TRAY_2 == tray_id)
        temp_z = parameters.placeProductZ2();
    else if(SensorPosition::BUFFER_TRAY == tray_id)
        temp_z = parameters.placeBufferProductZ();
    bool result = picker2PlaceToTray(temp_z,parameters.pickProductForce(),false,time_out);
    if(!result)
        AppendError(QString(u8"将成品放入%1号成品盘失败").arg(tray_id + 1));
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool SensorLoaderModule::placeProductToBuferr(int time_out)
{
    bool result = picker2PlaceToTray(parameters.placeBufferProductZ(),parameters.pickProductForce(),false,time_out);
    if(!result)
        AppendError(QString(u8"将成品放入备用盘失败"));
    return result;
}

bool SensorLoaderModule::picker1MeasureHight(int tray_id)
{
    qInfo("picker1MeasureHight tray_id %d",tray_id);
    if(pick_arm->Z1MeasureHeight(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),parameters.holdTime()))
    {
        QThread::msleep(100);
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否应用此高度:%1").arg(pick_arm->GetZ1MeasuredHeight()))){
            return true;
        }
        if(tray_id == SensorPosition::SUT1_SENSOR)
            parameters.setPlaceSUT1SensorZ(pick_arm->GetZ1MeasuredHeight());
        else if (tray_id == SensorPosition::SUT2_SENSOR)
        {
            parameters.setPlaceSUT2SensorZ(pick_arm->GetZ1MeasuredHeight());
        }
        else if(tray_id == SensorPosition::SENSOR_TRAY_1)
            parameters.setPickSensorZ(pick_arm->GetZ1MeasuredHeight());
        else if(tray_id == SensorPosition::SENSOR_TRAY_2)
            parameters.setPickSensorZ2(pick_arm->GetZ1MeasuredHeight());
        else
        {
            AppendError("测量类型错误！");
            return false;
        }
        return true;
    }
    AppendError(QString(u8"1号吸头测高失败"));
    return false;
}

bool SensorLoaderModule::picker2MeasureHight(int tray_id)
{
    qInfo("picker2MeasureHight tray_id %d",tray_id);
    if(pick_arm->Z2MeasureHeight(parameters.vcmWorkSpeed(),parameters.pickProductForce(),parameters.holdTime()))
    {
        QThread::msleep(100);
        double temp_z = pick_arm->GetZ2MeasuredHeight();
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否应用此高度:%1").arg(temp_z))){
            return true;
        }
        if(tray_id == SensorPosition::SUT1_SENSOR)
            parameters.setPickSUT1NgSensorZ(temp_z);
        else if (tray_id == SensorPosition::SUT2_SENSOR)
            parameters.setPickSUT2NgSensorZ(temp_z);
        else if(tray_id == SensorPosition::SUT1_PRODUCT)
            parameters.setPickSUT1ProductZ(temp_z);
        else if(tray_id == SensorPosition::SUT2_PRODUCT)
            parameters.setPickSUT2ProductZ(temp_z);
        else if(tray_id == SensorPosition::SENSOR_TRAY_1)
            parameters.setPlaceProductZ(temp_z);
        else if(tray_id == SensorPosition::SENSOR_TRAY_2)
            parameters.setPlaceProductZ2(temp_z);
        else if(tray_id == SensorPosition::NG_SENSOR_TRAY)
            parameters.setPlaceNgSensorZ(temp_z);
        else if(tray_id == SensorPosition::TRAY_NG_PRODUCT)
            parameters.setPlaceNgProductZ(temp_z);
        else if(tray_id == SensorPosition::BUFFER_TRAY)
            parameters.setPlaceBufferProductZ(temp_z);
        else
        {
            AppendError("测量类型错误！");
            return false;
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
    if(!pick_arm->Z1MeasureHeight(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),parameters.holdTime()))
    {
        AppendError(QString(u8"测量SUT台高度差失败"));
        return false;
    }
    sut1_height = pick_arm->GetZ1MeasuredHeight();
    temp_point.setX(sut2_pr_position.X()+picker1_offset.X());
    temp_point.setY(sut2_pr_position.Y()+picker1_offset.Y());
    if(!pick_arm->move_XY_Synic(temp_point,true))
    {
        AppendError(QString(u8"测量SUT台高度差失败"));
        return false;
    }
    if(!pick_arm->Z1MeasureHeight(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),parameters.holdTime()))

    {
        AppendError(QString(u8"测量SUT台高度差失败"));
        return false;
    }
    double z_offset = pick_arm->GetZ1MeasuredHeight()-sut1_height;
    if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否应用此数值:%1").arg(z_offset))){
        return true;
    }
    QJsonObject param;
    param.insert("ZOffset",z_offset);
    sendMessageToModule("Sut1Module","ZOffset",param);
    return true;
}

bool SensorLoaderModule::unloadAllSensor()
{
    qInfo("Unload all sensor");

    sendMessageToModule("Sut1Module", "MoveToLoadPos");
    sendMessageToModule("Sut2Module", "MoveToLoadPos");
    int i = 0;
    //Checking 1: SPA has product already
    if (this->pick_arm->picker2->vacuum->checkHasMaterielSync())
    {
        QList<QString> trayIndexSelection; trayIndexSelection.append("Tray 1"); trayIndexSelection.append("Tray 2"); trayIndexSelection.append("Buffer Tray");
        int tray_index = SensorPosition::SENSOR_TRAY_1;
        QString resp = SI::ui.getUIResponse(this->Name(), "Detect Product! Please select the target tray.", MsgBoxIcon::Question, trayIndexSelection);
        if(resp == "Tray 1") tray_index = SensorPosition::SENSOR_TRAY_1;
        else if (resp == "Tray 2") tray_index = SensorPosition::SENSOR_TRAY_2;
        else if (resp == "Buffer Tray") tray_index = SensorPosition::BUFFER_TRAY;

        do{
            moveToTrayPos(i, tray_index); i++;
            if(performTrayEmptyPR()) {
               qInfo("Detect Empty Pos");
               PrOffset pr_offset = tray_empty_location->getCurrentResult();
               tray_empty_location->setCurrentResult(pr_offset);
               movePicker2ToTrayCurrentPos(tray_index,true);
               placeProductToTray(tray_index);
               break;
            }
            if (i%10 == 0) {
                QString resp = SI::ui.getUIResponse(this->Name(), "Cannot find empty position. Continue?", MsgBoxIcon::Question, SI::ui.yesNoButtons);
                if(resp ==  SI::ui.No) {
                    return false;
                }
            }
        } while(i<99);
    }
    //Checking 2: SPA has sensor already
    if (this->pick_arm->picker1->vacuum->checkHasMaterielSync())
    {
        QList<QString> trayIndexSelection; trayIndexSelection.append("Tray 1"); trayIndexSelection.append("Tray 2"); trayIndexSelection.append("Buffer Tray");
        int tray_index = SensorPosition::SENSOR_TRAY_1;
        QString resp = SI::ui.getUIResponse(this->Name(), "Detect Sensor! Please select the target tray.", MsgBoxIcon::Question, trayIndexSelection);
        if(resp == "Tray 1") tray_index = SensorPosition::SENSOR_TRAY_1;
        else if (resp == "Tray 2") tray_index = SensorPosition::SENSOR_TRAY_2;
        else if (resp == "Buffer Tray") tray_index = SensorPosition::BUFFER_TRAY;
        do{
            moveToTrayPos(i, tray_index); i++;
            if(performTrayEmptyPR()) {
               qInfo("Detect Empty Pos");
               PrOffset pr_offset = tray_empty_location->getCurrentResult();
               tray_empty_location->setCurrentResult(pr_offset);
               movePicker1ToTrayCurrentPos(tray_index,true);
               backSensorToTray(tray_index);
               break;
            }
            if (i%10 == 0) {
                QString resp = SI::ui.getUIResponse(this->Name(), "Cannot find empty position. Continue?", MsgBoxIcon::Question, SI::ui.yesNoButtons);
                if(resp ==  SI::ui.No) {
                    return false;
                }
            }
        } while(i<99);
    }
    //Checking 3: Look at SUT 1 whether this has sensor, if yes, unload that
    if(moveCameraToSUTPRPos(false,true)){
        if(!performSUTEmptyPR()){
            if(performSUTSensorPR()){   //This is ng sensor
                movePicker2ToSUTPos(false, false);
                pickNgSensorFromSut(false);
                QList<QString> trayIndexSelection; trayIndexSelection.append("Tray 1"); trayIndexSelection.append("Tray 2"); trayIndexSelection.append("Buffer Tray");
                int tray_index = SensorPosition::SENSOR_TRAY_1;
                QString resp = SI::ui.getUIResponse(this->Name(), "Detect Ng Sensor! Please select the target tray.", MsgBoxIcon::Question, trayIndexSelection);
                if(resp == "Tray 1") tray_index = SensorPosition::SENSOR_TRAY_1;
                else if (resp == "Tray 2") tray_index = SensorPosition::SENSOR_TRAY_2;
                else if (resp == "Buffer Tray") tray_index = SensorPosition::BUFFER_TRAY;
                do{
                    moveToTrayPos(i, tray_index); i++;
                    if(performTrayEmptyPR()) {
                       qInfo("Detect Empty Pos");
                       PrOffset pr_offset = tray_empty_location->getCurrentResult();
                       tray_empty_location->setCurrentResult(pr_offset);
                       movePicker2ToTrayCurrentPos(tray_index,true);
                       placeProductToTray(tray_index);
                       break;
                    }
                    if (i%10 == 0) {
                        QString resp = SI::ui.getUIResponse(this->Name(), "Cannot find empty position. Continue?", MsgBoxIcon::Question, SI::ui.yesNoButtons);
                        if(resp ==  SI::ui.No) {
                            return false;
                        }
                    }
                } while(i<99);
            }
        }
    }
    //Checking 4: Look at SUT 2 whether this has sensor
    if(moveCameraToSUTPRPos(true,true)){
        if(!performSUTEmptyPR()){
            if(performSUTSensorPR()){   //This is not product
                movePicker2ToSUTPos(false, false);
                pickNgSensorFromSut(false);
                QList<QString> trayIndexSelection; trayIndexSelection.append("Tray 1"); trayIndexSelection.append("Tray 2"); trayIndexSelection.append("Buffer Tray");
                int tray_index = SensorPosition::SENSOR_TRAY_1;
                QString resp = SI::ui.getUIResponse(this->Name(), "Detect Product! Please select the target tray.", MsgBoxIcon::Question, trayIndexSelection);
                if(resp == "Tray 1") tray_index = SensorPosition::SENSOR_TRAY_1;
                else if (resp == "Tray 2") tray_index = SensorPosition::SENSOR_TRAY_2;
                else if (resp == "Buffer Tray") tray_index = SensorPosition::BUFFER_TRAY;

                do{
                    moveToTrayPos(i, tray_index); i++;
                    if(performTrayEmptyPR()) {
                       qInfo("Detect Empty Pos");
                       PrOffset pr_offset = tray_empty_location->getCurrentResult();
                       tray_empty_location->setCurrentResult(pr_offset);
                       movePicker2ToTrayCurrentPos(tray_index,true);
                       placeProductToTray(tray_index);
                       break;
                    }
                    if (i%10 == 0) {
                        QString resp = SI::ui.getUIResponse(this->Name(), "Cannot find empty position. Continue?", MsgBoxIcon::Question, SI::ui.yesNoButtons);
                        if(resp ==  SI::ui.No) {
                            return false;
                        }
                    }
                } while(i<99);
            }
        }
    }
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

//bool SensorLoaderModule::moveToTrayPos(int tray_index)
//{
//    qInfo("moveToTrayPos tray_index %d",tray_index);
//    QPointF tem_pos = tray->getCurrentPosition(tray_index);
//    bool result = pick_arm->move_XYT1_Synic(tem_pos.x(),tem_pos.y(),parameters.picker1ThetaOffset(),true);
//    if(!result)
//        AppendError(QString(u8"移动到%1盘当前位置失败").arg(tray_index == 0?u8"sensor":u8"成品"));
//    return result;
//}

bool SensorLoaderModule::moveToStartPos(int tray_index)
{
    QElapsedTimer timer; timer.start();
    bool result = pick_arm->move_XY_Synic(tray->getStartPosition(tray_index),true);
    if(!result)
        AppendError(QString(u8"移动到%1盘起始位置失败").arg(tray_index == 0?"sensor":"成品"));
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
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

bool SensorLoaderModule::moveCameraToStandbyPos(bool check_arrived,bool check_softlanding)
{
    QElapsedTimer timer; timer.start();
    QString temp;
    pick_arm->setCallerName(__FUNCTION__);
    bool result = pick_arm->move_XY_Synic(QPointF(spa_standby_position.X(), spa_standby_position.Y()),check_arrived,check_softlanding);
    temp.append(" move_XY_Synic ").append(QString::number(timer.elapsed()));
    pick_arm->setCallerName("");
    if(!result)
        AppendError(QString(u8"移动SPA到standby位置失败"));
    QString log = QString("[Timelog] ").append(__FUNCTION__).append(" ")
                                       .append(QString::number(timer.elapsed()))
                                       .append(temp);
    qWarning(log.toStdString().c_str());
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

//void SensorLoaderModule::sendEvent(const QString event)
//{
//    QJsonObject result;
//    result.insert("event", event);
//    emit sendMessageToClient(servingIP, getStringFromJsonObject(result));
//}

//void SensorLoaderModule::sendCmd(QString serving_ip, const QString cmd)
//{
//    QJsonObject result;
//    result.insert("cmd", cmd);
//    emit sendMessageToClient(serving_ip, getStringFromJsonObject(result));
//}

//void SensorLoaderModule::getPicker1SensorOffset()
//{
//    if(parameters.useSensorOffset())
//    {
//        double temp_theta = pr_offset.Theta*PI/180;
//        double x = parameters.sensorOffsetX()*cos(temp_theta) + parameters.sensorOffsetY()*sin(temp_theta) ;
//        double y = parameters.sensorOffsetY()*cos(temp_theta) - parameters.sensorOffsetX()*sin(temp_theta) ;
//        states.setPicker1SensorOffsetX(x);
//        states.setPicker1SensorOffsetY(y);
//    }
//    else
//    {
//        states.setPicker1SensorOffsetX(pr_offset.O_X);
//        states.setPicker1SensorOffsetY(pr_offset.O_Y);
//    }
//}

//void SensorLoaderModule::getPicker2SensorOffset()
//{
//    if(parameters.useSensorOffset())
//    {
//        double temp_theta = pr_offset.Theta*PI/180;
//        double x = parameters.sensorOffsetX()*cos(temp_theta) + parameters.sensorOffsetY()*sin(temp_theta) ;
//        double y = parameters.sensorOffsetY()*cos(temp_theta) - parameters.sensorOffsetX()*sin(temp_theta) ;
//        states.setPicker2SensorOffsetX(x);
//        states.setPicker2SensorOffsetY(y);
//    }
//    else
//    {
//        states.setPicker2SensorOffsetX(pr_offset.O_X);
//        states.setPicker2SensorOffsetY(pr_offset.O_Y);
//    }
//}

//void SensorLoaderModule::setPicker1SensorOffset()
//{
//    if(parameters.useSensorOffset())
//    {
//        states.setPicker1SensorOffsetX(parameters.sensorOffsetX());
//        states.setPicker1SensorOffsetY(parameters.sensorOffsetY());
//    }
//    else
//    {
//        double temp_theta = -pr_offset.Theta*PI/180;
//        double x = pr_offset.O_X*cos(temp_theta) + pr_offset.O_Y*sin(temp_theta) ;
//        double y = pr_offset.O_Y*cos(temp_theta) - pr_offset.O_X*sin(temp_theta) ;
//        states.setPicker1SensorOffsetX(x);
//        states.setPicker1SensorOffsetY(y);
//    }
//}

//void SensorLoaderModule::setPicker2SensorOffset()
//{
//    if(parameters.useSensorOffset())
//    {
//        states.setPicker2SensorOffsetX(parameters.sensorOffsetX());
//        states.setPicker2SensorOffsetY(parameters.sensorOffsetY());
//    }
//    else
//    {
//        double temp_theta = -pr_offset.Theta*PI/180;
//        double x = pr_offset.O_X*cos(temp_theta) + pr_offset.O_Y*sin(temp_theta) ;
//        double y = pr_offset.O_Y*cos(temp_theta) - pr_offset.O_X*sin(temp_theta) ;
//        states.setPicker2SensorOffsetX(x);
//        states.setPicker2SensorOffsetY(y);
//    }
//}

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

QMap<QString,PropertyBase*> SensorLoaderModule::getModuleParameter()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("parameters", &parameters);
    temp_map.insert("sut1_pr_position", &sut1_pr_position);
    temp_map.insert("sut2_pr_position", &sut2_pr_position);
    temp_map.insert("spa_standby_position", &spa_standby_position);
    temp_map.insert("picker1_offset", &picker1_offset);
    temp_map.insert("picker2_offset", &picker2_offset);
    temp_map.insert("sensorTray1PickOffset", &sensorTray1PickOffset);
    temp_map.insert("sensorTray2PickOffset", &sensorTray2PickOffset);
    temp_map.insert("sut1PickOffset", &sut1PickOffset);
    temp_map.insert("sut2PickOffset", &sut2PickOffset);
    temp_map.insert("sut1PlaceOffset", &sut1PlaceOffset);
    temp_map.insert("sut2PlaceOffset", &sut2PlaceOffset);
	temp_map.insert("sensorTray1PlaceOffset", &sensorTray1PlaceOffset);
    temp_map.insert("sensorTray2PlaceOffset", &sensorTray2PlaceOffset);
    temp_map.insert("ngTrayPlaceOffset", &ngTrayPlaceOffset);
    temp_map.insert("bufferTrayPlaceOffset", &bufferTrayPlaceOffset);
    temp_map.insert("sensor_uph",&sensor_uph);
    temp_map.insert("left_sensor_uph",&left_sensor_uph);
    temp_map.insert("right_sensor_uph",&right_sensor_uph);
    temp_map.insert("right_sensor_uph",&product_uph);
    temp_map.insert("left_product_uph",&left_product_uph);
    temp_map.insert("right_product_uph",&right_product_uph);
    temp_map.insert("comprehensive_uph",&comprehensive_uph);
    temp_map.insert("left_comprehensive_uph",&left_comprehensive_uph);
    temp_map.insert("right_comprehensive_uph",&right_comprehensive_uph);
    return temp_map;
}

void SensorLoaderModule::setModuleParameter(QMap<QString, PropertyBase *>)
{

}
