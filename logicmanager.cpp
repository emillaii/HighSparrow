#include "logicmanager.h"
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QMessageBox>

LogicManager::LogicManager(QObject *parent):QObject (parent)
{
    this->setObjectName("LogicManager");
    this->moveToThread(&work_thread);
    work_thread.start();
    connect(this,&LogicManager::sendPerformHandling,this,&LogicManager::performHandlingOperation);
    connect(this,&LogicManager::sendPerformTcp,this,&LogicManager::performTcpOperation);
}

void LogicManager::init(BaseModuleManager *device_manager)
{
    baseModuleManage = device_manager;
}

//bool LogicManager::registerWorker(ThreadWorkerBase *worker)
//{
//    if(!workers.contains(worker->Name()))
//    {
//        workers.insert(worker->Name(),worker);
//        qInfo("registerWorker :%s",worker->Name().toStdString().c_str());
//        return true;
//    }
//    return false;
//}

void LogicManager::performHandling(QString module_name,int cmd,QVariant param)
{
    qInfo("performHandling in module: %s cmd: %d params: %s", module_name.toStdString().c_str(), cmd, param.toString().toStdString().c_str());
    emit sendPerformHandling(module_name,cmd,param);
}

void LogicManager::performHandling(int cmd,QVariant param)
{
    emit sendPerformHandling("",cmd,param);
}

void LogicManager::allMotorsSeekOrigin()
{

}

void LogicManager::run() {
    qInfo("Logic manager is running");
    is_handling = true;
    QString uuid = baseModuleManage->unitlog.createUnit();
    if (m_currentMode == CommandType::MOTION_STOP_HOME) {
        baseModuleManage->stopSeeking();
    }
    else if (m_currentMode == CommandType::MODE_AUTO_RUN) {
    }
    else if (m_currentMode == CommandType::PERFORM_LOOP_TEST) {
        return;
    }
    else if (m_currentMode == CommandType::PERFORM_OC) {
    }
    else if (m_currentMode == CommandType::MOTION_HOME) {
        qInfo("CommandType::MOTION_HOME");
        if(baseModuleManage->ServerMode() == 0)
        {
            if(baseModuleManage->allMotorsSeekOrigin())
            {
                QVariantMap message;
                message.insert("MotorName","AllMotor");
                sendCmdMessage(message,CommandType::MOTION_HOME);
                waitReturnMessage();
            }
        }
        else
        {
            QVariantMap message;
            message.insert("MotorName","AllMotor");
            sendCmdMessage(message,CommandType::MOTION_HOME);
            if(waitReturnMessage())
            {
                baseModuleManage->allMotorsSeekOrigin();
            }
        }
    } else if (m_currentMode == CommandType::MOTION_INIT) {
        qInfo("CommandType::MOTION_INIT");
        QVariantMap message;
        sendCmdMessage(message,CommandType::MOTION_INIT);
        baseModuleManage->initialDevice();
        waitReturnMessage();
    }
    else if (m_currentMode == CommandType::MOTION_HOME_ONLYSELF) {
        result =  baseModuleManage->allMotorsSeekOrigin();
        QVariantMap  return_message;
        return_message.insert("TargetModule","LogicManager");
        sendRespMessage(return_message,result);
    } else if (m_currentMode == CommandType::MOTION_INIT_ONLYSELF) {
        result = baseModuleManage->initialDevice();
        QVariantMap  return_message;
        return_message.insert("TargetModule","LogicManager");
        sendRespMessage(return_message,result);
    }
    else if (m_currentMode == CommandType::AA_MOVETO_MUSHROOM_CMD)
    {
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            baseModuleManage->aa_head_module.moveToMushroomPosition();
        }
    } else if (m_currentMode == CommandType::AA_MOVETO_PICK_LENS_CMD)
    {
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            baseModuleManage->aa_head_module.moveToPickLensPosition();
        }
    } else if (m_currentMode == CommandType::AA_MOVETO_OC_CMD)
    {
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            baseModuleManage->aa_head_module.moveToOCPosition();
        }
    } else if (m_currentMode == CommandType::SUT_MOVETO_PR_CMD)
    {
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            baseModuleManage->sut_module.moveToDownlookPos();
        }
    } else if (m_currentMode == CommandType::SUT_MOVETO_MUSHROOM_CMD)
    {
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            baseModuleManage->sut_module.moveToMushroomPos();
        }
    } else if (m_currentMode == CommandType::LUT_MOVETO_LOAD_CMD)
    {
        baseModuleManage->lut_module.moveToLoadPos();
    } else if (m_currentMode == CommandType::LUT_MOVETO_UNLOAD_CMD)
    {
        baseModuleManage->lut_module.moveToLutDownlookUnloadPos();
    } else if (m_currentMode == CommandType::LUT_MOVETO_LOAD_UPLOOK_CMD)
    {
        baseModuleManage->lut_module.moveToLoadUplookPos();
    } else if (m_currentMode == CommandType::LUT_MOVETO_AA1_UPLOOK_CMD)
    {
        baseModuleManage->lut_module.moveToAA1UplookPos();
    } else if (m_currentMode == CommandType::LUT_MOVETO_AA2_UPLOOK_CMD)
    {
        baseModuleManage->lut_module.moveToAA2UplookPos();
    }
    else if (m_currentMode == CommandType::PERFORM_CALIBRATION)
    {
        QVariantMap message;
        message["calibration_name"] = calibration_name;
        sendCmdMessage(message,CommandType::PERFORM_CALIBRATION);
        waitReturnMessage();

        baseModuleManage->performCalibration(calibration_name);
        qInfo("calibration End");
    }
    else if (m_currentMode == CommandType::PERFORM_UPDNLOOK_CALIBRATION)
    {
        baseModuleManage->performUpDnLookCalibration();
        qInfo("UpDnlook Calibration End");
    }
    else if (m_currentMode == CommandType::PERFORM_LENS_UPDNLOOK_CALIBRATION)
    {
        baseModuleManage->performLensUpDnLookCalibration();
        qInfo("Lens UpDnlook Calibration End");
    }
    else if (m_currentMode == CommandType::PERFORM_SENSOR_PICKHEAD_CALIBRATION)
    {
        qInfo("Sensor Pickhead calibration End");
    }
    else if (m_currentMode == CommandType::LUT_PICK_LENS_TO_AA1_CMD)
    {
        baseModuleManage->lut_module.moveToAA1PickLens(false,false,true);
    }
    else if (m_currentMode == CommandType::LUT_PICK_LENS_TO_AA1_CMD1)
    {
        baseModuleManage->lut_module.moveToAA1PickLens(true,false,true);
    }
    else if (m_currentMode == CommandType::LUT_PICK_LENS_TO_AA2_CMD)
    {
        qInfo("Move To AA2 PickLens");
        baseModuleManage->lut_module.moveToAA2PickLens(false,false,true);
    }
    else if (m_currentMode == CommandType::PERFORM_LOCATION)
    {
        baseModuleManage->performLocation(location_name,use_origin);
        qInfo("location End");
    }

    m_currentMode = CommandType::IDLE;
    qInfo("Logic Manager End");
    is_handling = false;
}

bool LogicManager::motorSeekOrigin(QString motor_name)
{
    if(motor_name == "AllMotor")
    {
        return baseModuleManage->allMotorsSeekOrigin();
    }
    else if(baseModuleManage->motors.contains(motor_name))
    {
        return baseModuleManage->GetMotorByName(motor_name)->SeekOrigin();
    }
    else
    {
        qInfo("Motor Name not exist!");
        return false;
    }
}

void LogicManager::sendCmdMessage(QVariantMap message,int cmd)
{
    message.insert("TargetModule","LogicManager");
    message.insert("performHandling",cmd);
    message.insert("OriginModule",parameters.moduleName());
    emit sendMessageToWorkerManger(message);
}

void LogicManager::sendRespMessage(QVariantMap message, bool result)
{
    message.insert("performHandlingResp",result);
    emit sendMessageToWorkerManger(message);
}

void LogicManager::sendMessageToModule(QString module_name, QString message)
{
    QVariantMap message_map;
    message_map.insert("TargetModule",module_name);
    message_map.insert("OriginModule",parameters.moduleName());
    message_map.insert("Message",message);
    emit sendMessageToWorkerManger(message_map);
}

void LogicManager::sendLogicManangerName()
{
    QVariantMap message_map;
    message_map.insert("TargetModule","WorksManager");
    message_map.insert("Message","SendLogicManagerName");
    message_map.insert("LogicManagerName",parameters.moduleName());
    emit sendMessageToWorkerManger(message_map);
}

bool LogicManager::waitReturnMessage()
{
    while (is_handling) {
        if(return_message.contains("performHandlingResp"))
        {
            bool result = return_message["performHandlingResp"].toBool();
            return_message.remove("performHandlingResp");
            return result;
        }
        Sleep(10);
    }
    return false;
}

bool LogicManager::performLocation(QString location_name, bool use_origin)
{
    qInfo("performCalibration of %s ",location_name.toStdString().c_str());
    VisionLocation* temp_location = baseModuleManage->GetVisionLocationByName(location_name);
    if(temp_location == nullptr)
    {
        qInfo("cant not find location %s",location_name.toStdString().c_str());
        AppendError(QString("cant not find location %1").arg(location_name));
        return  false;
    }
    Calibration* temp_caliration = baseModuleManage->GetCalibrationByName(temp_location->parameters.calibrationName());
    if(temp_caliration == nullptr)
    {
        qInfo("cant not find calibration %s",temp_location->parameters.calibrationName().toStdString().c_str());
        AppendError(QString("cant not find calibration %1").arg(temp_location->parameters.calibrationName()));
        return  false;
    }
    PrOffset offset;
    if(temp_location->parameters.calibrationName().contains("chart_calibration"))
    {
        double x,y;
        if(!temp_caliration->GetPixelPoint(x,y))
            return false;
        QPointF result_point;
        temp_caliration->getDeltaDistanceFromCenter(QPointF(x,y),result_point);
        offset.X = result_point.x(); offset.Y = result_point.y();
    }
    else
    {
        if(!temp_location->performPR())
        {
            AppendError(temp_location->GetCurrentError());
            return false;
        }
        offset = temp_location->getCurrentResult(use_origin);
    }
    if(temp_location->parameters.canMotion())
    {
       return temp_caliration->performPRResult(offset);
    }
    return true;
}

bool LogicManager::performCalibration(QString location_name)
{
    qInfo("performCalibration of %s ",location_name.toStdString().c_str());
    VisionLocation* temp_location = baseModuleManage->GetVisionLocationByName(location_name);
    if(temp_location == nullptr)
    {
        qInfo("cant not find location %s",location_name.toStdString().c_str());
        AppendError(QString("cant not find location %1").arg(location_name));
        return  false;
    }
    Calibration* temp_caliration = baseModuleManage->GetCalibrationByName(temp_location->parameters.calibrationName());
    if(temp_caliration == nullptr)
    {
        qInfo("cant not find calibration %s",temp_location->parameters.calibrationName().toStdString().c_str());
        AppendError(QString("cant not find calibration %1").arg(temp_location->parameters.calibrationName()));
        return  false;
    }
    return  temp_caliration->performCalibration();
}

bool LogicManager::performCylinderOperation(QString cylinder_name, bool state)
{
    qInfo("performCylinderOperation of %s ",cylinder_name.toStdString().c_str());
    XtCylinder* temp_cylinder = baseModuleManage->GetCylinderByName(cylinder_name);
    if(temp_cylinder == nullptr)
    {
        qInfo("cant not find cylinder %s",cylinder_name.toStdString().c_str());
        AppendError(QString("cant not find cylinder %1").arg(cylinder_name));
        return  false;
    }
    return  temp_cylinder->Set(state);
}

bool LogicManager::performOutputOperation(QString output_name, bool state)
{
    qInfo("performOutputOperation of %s ",output_name.toStdString().c_str());
    XtGeneralOutput* temp_output = baseModuleManage->GetOutputIoByName(output_name);
    if(temp_output == nullptr)
    {
        qInfo("cant not find output io %s",output_name.toStdString().c_str());
        AppendError(QString("cant not find output io %1").arg(output_name));
        return  false;
    }
    return  temp_output->Set(state);
}

bool LogicManager::performVacuumOperation(QString vacuum_name, bool state)
{
    qInfo("performVacuumOperation of %s ",vacuum_name.toStdString().c_str());
    XtVacuum* temp_vacuum = baseModuleManage->GetVacuumByName(vacuum_name);
    if(temp_vacuum == nullptr)
    {
        qInfo("cant not find vacuum %s",vacuum_name.toStdString().c_str());
        AppendError(QString("cant not find vacuum %1").arg(vacuum_name));
        return  false;
    }
    return  temp_vacuum->Set(state);
}






void LogicManager::home(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::MOTION_HOME);}
void LogicManager::init(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::MOTION_INIT);}
void LogicManager::home_only_self(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::MOTION_HOME_ONLYSELF);}
void LogicManager::init_only_self(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::MOTION_INIT_ONLYSELF);}
void LogicManager::stopHome(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::MOTION_STOP_HOME);}
void LogicManager::stop(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::STOP);}

void LogicManager::aaMoveToMushroomPos(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::AA_MOVETO_MUSHROOM_CMD);}
void LogicManager::aaMoveToPickLensPos(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::AA_MOVETO_PICK_LENS_CMD);}
void LogicManager::aaMoveToOCPos(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::AA_MOVETO_OC_CMD);}

void LogicManager::sutMoveToMushroomPos(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::SUT_MOVETO_MUSHROOM_CMD);}
void LogicManager::sutMoveToPRPos(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::SUT_MOVETO_PR_CMD);}

void LogicManager::lutMoveToLoadPos(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::LUT_MOVETO_LOAD_CMD);}
void LogicManager::lutMoveToUnloadPos(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::LUT_MOVETO_UNLOAD_CMD);}
void LogicManager::lutMoveToLoadUplookPos(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::LUT_MOVETO_LOAD_UPLOOK_CMD);}
void LogicManager::lutMoveToAA1UplookPos(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::LUT_MOVETO_AA1_UPLOOK_CMD);}
void LogicManager::lutMoveToAA2UplookPos(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::LUT_MOVETO_AA2_UPLOOK_CMD);}

void LogicManager::lutPickLensToAA1(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::LUT_PICK_LENS_TO_AA1_CMD);}
void LogicManager::lutPickLensToAA11(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::LUT_PICK_LENS_TO_AA1_CMD1);}
void LogicManager::lutPickLensToAA2(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::LUT_PICK_LENS_TO_AA2_CMD);}

void LogicManager::performChartCalibration()
{
    setStateMessage(__FUNCTION__);
    calibration_name = baseModuleManage->chart_calibration->parameters.calibrationName();
    moveToCmd(CommandType::PERFORM_CALIBRATION);
}

//void LogicManager::performCalibration(QString calibration_name)
//{
//    setStateMessage(__FUNCTION__);
//    this->calibration_name = calibration_name;
//    moveToCmd(CommandType::PERFORM_CALIBRATION);
//}

void LogicManager::performUpDnLookCalibration()
{
    setStateMessage(__FUNCTION__);
    moveToCmd(CommandType::PERFORM_UPDNLOOK_CALIBRATION);
}

void LogicManager::performLensUpDnLookCalibration()
{
    setStateMessage(__FUNCTION__);
    moveToCmd(CommandType::PERFORM_LENS_UPDNLOOK_CALIBRATION);
}

void LogicManager::performSensorPickheadCalibration()
{
    setStateMessage(__FUNCTION__);
    moveToCmd(CommandType::PERFORM_SENSOR_PICKHEAD_CALIBRATION);
}

//void LogicManager::performLocation(QString location_name,bool use_origin)
//{
//    setStateMessage(__FUNCTION__);
//    this->location_name = location_name;
//    this->use_origin = use_origin;
//    moveToCmd(CommandType::PERFORM_LOCATION);
//}

void LogicManager::autoRun(QString json){
    setStateMessage(__FUNCTION__);moveToCmd(CommandType::MODE_AUTO_RUN);
}

void LogicManager::performOC(){
    setStateMessage(__FUNCTION__);moveToCmd(CommandType::PERFORM_OC);
}

void LogicManager::performLoopTest(int mode){
    qInfo("Loop Test Mode: %d", mode);
    setStateMessage(__FUNCTION__);
    moveToCmd(CommandType::PERFORM_LOOP_TEST);
}

void LogicManager::performUV()
{
    qInfo("perform UV in logic manager");
    if(baseModuleManage->sut_module.moveToMushroomPos(true))
    {
        int uvTime = baseModuleManage->dispense_module.parameters.uvTimeMs();
        baseModuleManage->aa_head_module.openUVTillTime(uvTime);
        baseModuleManage->aa_head_module.waitUVFinish();
    }
}

void LogicManager::lensPickArmMoveToTray1Pos()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LENS_TRAY1);
}

void LogicManager::lensPickArmMoveToTray2Pos()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LENS_TRAY2);
}

void LogicManager::lensPickArmMoveToTray1StartPos()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LENS_TRAY1_START_POS);
}

void LogicManager::lensPickArmMoveToTray2StartPos()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LENS_TRAY2_START_POS);
}

void LogicManager::lensPickArmMoveToTray1EndPos()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LENS_TRAY1_END_POS);
}

void LogicManager::lensPickArmMoveToLutPos1()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LUT_POS1);
}

void LogicManager::lensPickArmMoveToLutPos2()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LUT_POS2);
}

void LogicManager::lensPickArmMoveToPickLensFromTray1()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LENS_TRAY1+
                                                         LensLoaderModule::HandlePR::LENS_PR+
                                                         LensLoaderModule::HandleToWorkPos::ToWork+
                                                         LensLoaderModule::HandlePickerAction::PICK_LENS_FROM_TRAY);
}

void LogicManager::lensPickArmMoveToPickLensFromTray2()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LENS_TRAY2+
                                                         LensLoaderModule::HandlePR::LENS_PR+
                                                         LensLoaderModule::HandleToWorkPos::ToWork+
                                                         LensLoaderModule::HandlePickerAction::PICK_LENS_FROM_TRAY);
}

void LogicManager::lensPickArmMoveToPickLensFromLut()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LUT_POS2+
                                                         LensLoaderModule::HandlePR::LUT_LENS_PR+
                                                         LensLoaderModule::HandleToWorkPos::ToWork+
                                                         LensLoaderModule::HandlePickerAction::PICK_NG_LENS_FROM_LUT2);
}

void LogicManager::lensPickArmMoveToPlaceLensToTray1()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LENS_TRAY1+
                                                         LensLoaderModule::HandlePR::VACANCY_PR+
                                                         LensLoaderModule::HandleToWorkPos::ToWork+
                                                         LensLoaderModule::HandlePickerAction::PLACE_NG_LENS_TO_TRAY);
}

void LogicManager::lensPickArmMoveToPlaceLensToTray2()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LENS_TRAY2+
                                                         LensLoaderModule::HandlePR::VACANCY_PR+
                                                         LensLoaderModule::HandleToWorkPos::ToWork+
                                                         LensLoaderModule::HandlePickerAction::PLACE_NG_LENS_TO_TRAY);
}

void LogicManager::lensPickArmMoveToPlaceLensToLut()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::LUT_POS1+
                                                         LensLoaderModule::HandlePR::RESET_PR+
                                                         LensLoaderModule::HandleToWorkPos::ToWork+
                                                         LensLoaderModule::HandlePickerAction::PLACE_LENS_TO_LUT);
}

void LogicManager::lensPickArmLensPR()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePR::LENS_PR+LensLoaderModule::HandleToWorkPos::ToWork);
}

void LogicManager::lensPickArmVacancyTrayPR()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePR::VACANCY_PR+LensLoaderModule::HandleToWorkPos::ToWork);
}

void LogicManager::lensPickArmLUTPR()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePR::LUT_PR+LensLoaderModule::HandleToWorkPos::ToWork);
}

void LogicManager::lensPickArmLensMeasureHeight()
{
    baseModuleManage->lens_loader_module.performHandling( LensLoaderModule::HandlePickerAction::MeasureLensInTray);
}

void LogicManager::lensPickArmLUTMeasureHeight()
{
    baseModuleManage->lens_loader_module.performHandling( LensLoaderModule::HandlePickerAction::MeasureLensInLUT);
}

void LogicManager::lensPickArmMoveToUpdownlookDownPos()
{
    //    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::UPDOWNLOOK_DOWN_POS);
}

void LogicManager::lensPickArmMoveToUpdownlookUpPos()
{
    //    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::UPDOWNLOOK_UP_POS);
}

void LogicManager::trayLoaderModuleLTIEMovetoFirstPos()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.LTIEMovetoColumnIndex(0);
}

void LogicManager::trayLoaderModuleLTIEMovetoLastPos()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    int col = baseModuleManage->tray_loader_module.tray_clip->standards_parameters.columnCount()-1;
    //    baseModuleManage->tray_loader_module.LTIEMovetoColumnIndex(col);
}

void LogicManager::trayLoaderModuleLTOEMovetoFirstPos()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.LTOEMovetoColumnIndex(0);
}

void LogicManager::trayLoaderModuleLTOEMovetoLastPos()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    int col = baseModuleManage->tray_loader_module.tray_clip->standards_parameters.columnCount()-1;
    //    baseModuleManage->tray_loader_module.LTOEMovetoColumnIndex(col);
}

void LogicManager::trayLoaderModuleLTKX1MovetoGetPos()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.moveToLtkx1GetPos();
}

void LogicManager::trayLoaderModuleLTKX1MovetoSetPos()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.moveToLtkx1SetPos();
}

void LogicManager::trayLoaderModuleLTKX2MovetoGetPos()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.moveToLtkx2GetPos();
}

void LogicManager::trayLoaderModuleLTKX2MovetoSetPos()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.moveToLtkx2SetPos();
}

void LogicManager::trayLoaderModuleLTLMovetoGetPos()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.moveToLtlGetPos();
}

void LogicManager::trayLoaderModuleLTLMovetoSetPos()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.moveToLtlSetPos();
}

void LogicManager::trayLoaderModuleLTIEMovetoColumnIndex(int idx)
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.LTIEMovetoColumnIndex(idx);
}

void LogicManager::trayLoaderModuleLTOEMovetoColumnIndex(int idx)
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.LTOEMovetoColumnIndex(idx);
}

void LogicManager::trayLoaderModuleEjectTray()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.ejectTray();
}

void LogicManager::trayLoaderModuleLTKX1CylinderOn()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.motorInPress();
}

void LogicManager::trayLoaderModuleLTKX1CylinderOff()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.motorInRelease();
}

void LogicManager::trayLoaderModuleLTKX2CylinderOn()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.motorOutPress();
}

void LogicManager::trayLoaderModuleLTKX2CylinderOff()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.motorOutRelease();
}

void LogicManager::trayLoaderModuleLTLXCylinderOn()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.motorWorkPress();
}

void LogicManager::trayLoaderModuleLTLXCylinderOff()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.motorWorkRelease();
}

void LogicManager::trayLoaderModuleStartup()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.startUp();
    //    emit baseModuleManage->tray_loader_module.parameters.trayReady();
    //    //emit baseModuleManage->tray_loader_module.parameters.trayReady();
}

void LogicManager::trayLoaderModuleEmitTestTrayUsed(){
    //    emit baseModuleManage->tray_loader_module.testTrayUsed();
}

void LogicManager::trayLoaderModuleLTKX1MoveToRelayPos()
{
    //    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    //    if(rb==QMessageBox::No){
    //        return;
    //    }
    //    baseModuleManage->tray_loader_module.moveToLtkx1RelayPos();
}

bool LogicManager::trayLoaderModuleCheckLTLXGetPos(double x)
{
    //    double max_range = baseModuleManage->GetMotorByName(baseModuleManage->tray_loader_module.parameters.motorLTKX1Name())->GetPostiveRange();
    //    double min_range = baseModuleManage->GetMotorByName(baseModuleManage->tray_loader_module.parameters.motorLTKX1Name())->GetNegativeRange();
    //    if(x+166.7538>max_range||x+166.7538-310<min_range){
    //        QMessageBox::warning(nullptr,tr(u8"提示"),tr(u8"LTK_X1无法移动到此位置，请重新选择"),QMessageBox::Ok);
    //        return false;
    //    }
    return true;
}

bool LogicManager::trayLoaderModuleCheckLTLXSetPos(double x)
{
    //    double min_range = baseModuleManage->GetMotorByName(baseModuleManage->tray_loader_module.parameters.motorLTKX2Name())->GetNegativeRange();
    //    if(x-653<min_range){
    //        QMessageBox::warning(nullptr,tr(u8"提示"),tr(u8"LTK_X2无法移动到此位置，请重新选择"),QMessageBox::Ok);
    //        return false;
    //    }
    return true;
}

void LogicManager::trayLoaderModuleLTLXPickUpTray()
{
    //    baseModuleManage->tray_loader_module.onTestLTLXPickUpTray();
}

void LogicManager::trayLoaderModuleLTLXPutDownTray()
{
    //    baseModuleManage->tray_loader_module.onTestLTLXPutDownTray();
}

void LogicManager::sensorTrayLoaderModuleSTIEMovetoColumnIndex(int n)
{
    baseModuleManage->sensor_tray_loder_module.movetoSTIEColumnIndex(n);
}

void LogicManager::sensorTrayLoaderModuleSTOEMovetoColumnIndex(int n)
{
    baseModuleManage->sensor_tray_loder_module.movetoSTOEColumnIndex(n);
}

void LogicManager::sensorTrayLoaderModuleMovetoPushMotorSafePosotion()
{
    baseModuleManage->sensor_tray_loder_module.movetoPushMotorSafePosotion();
}

void LogicManager::sensorTrayLoaderModuleMovetoDownTrayPosition()
{
    baseModuleManage->sensor_tray_loder_module.movetoDownTrayPosition();
}

void LogicManager::sensorTrayLoaderModuleMovetoGetTrayPosition()
{
    baseModuleManage->sensor_tray_loder_module.movetoGetTrayPosition();
}

void LogicManager::sensorTrayLoaderModuleMovetoFinishKickTrayPosition()
{
    baseModuleManage->sensor_tray_loder_module.movetoFinishKickTrayPosition();
}

void LogicManager::sensorTrayLoaderModuleMovetoPushoutPosition()
{
    baseModuleManage->sensor_tray_loder_module.movetoPushoutPosition();
}

void LogicManager::sensorTrayLoaderModuleMovetoPutTrayPosition()
{
    baseModuleManage->sensor_tray_loder_module.movetoPutTrayPosition();
}

void LogicManager::sensorTrayLoaderModuleMovetoStartKickTrayPosition()
{
    baseModuleManage->sensor_tray_loder_module.movetoStartKickTrayPosition();
}

void LogicManager::sensorTrayLoaderModuleMovetoTrayWorkPosition()
{
    baseModuleManage->sensor_tray_loder_module.movetoTrayWorkPosition();
}

void LogicManager::sensorTrayLoaderModuleMovetoVacancyTrayPosition()
{
    baseModuleManage->sensor_tray_loder_module.movetoVacancyTrayPosition();
}

void LogicManager::performHandlingOperation(QString module_name, int cmd,QVariant param)
{
    qInfo("performHandlingOperation module_name: %s cmd: %d param: %s", module_name.toStdString().c_str(), cmd, param.toString().toStdString().c_str());
    states.setIsHandling(true);
    is_handling = true;
    if(module_name == "")//非单一模块动作
    {
        if(cmd == CommandType::MOTION_INIT)
        {
            states.setHandlingMessage(u8"正在初始化……");
            QVariantMap message;
            sendCmdMessage(message,CommandType::MOTION_INIT);
            baseModuleManage->initialDevice();
            waitReturnMessage();
        }
        else if(cmd == CommandType::MOTION_HOME)
        {
            states.setHandlingMessage(u8"正在回零……");
            if(baseModuleManage->ServerMode() == 0)
            {
                if(baseModuleManage->allMotorsSeekOrigin())
                {
                    QVariantMap message;
                    message.insert("MotorName","AllMotor");
                    sendCmdMessage(message,CommandType::MOTION_HOME);
                    waitReturnMessage();
                }
            }
            else
            {
                QVariantMap message;
                message.insert("MotorName","AllMotor");
                sendCmdMessage(message,CommandType::MOTION_HOME);
                if(waitReturnMessage())
                    baseModuleManage->allMotorsSeekOrigin();
            }
        }
        else if (cmd == CommandType::MOTION_SINGLE_MOTOR_HOME)
        {
            states.setHandlingMessage(u8"正在执行单轴回零……");
            if(baseModuleManage->motors.contains(states.currentChannelName()))
            {
                baseModuleManage->GetMotorByName(states.currentChannelName())->SeekOrigin();
                baseModuleManage->GetMotorByName(states.currentChannelName())->WaitSeekDone();
            }
        }
        else if (cmd == CommandType::PERFORM_LOCATION)
        {
            states.setHandlingMessage(u8"正在执行视觉……");
            baseModuleManage->performLocation(states.currentLocationName(), states.useOriginPr());
        }
        else if(cmd == HANDLING_ORIGIN_LOCATION)
        {
            QString location_name = param.toString();
            states.setHandlingMessage(QString(u8"正在执行1视觉……").arg(location_name));
            if(baseModuleManage->vision_locations.contains(location_name))
            {
                performLocation(location_name,true);
            }
            else
            {
                QVariantMap message;
                message.insert("LocationName",location_name);
                message.insert("UseOrigin",true);
                sendCmdMessage(message,HANDLING_ORIGIN_LOCATION);
                waitReturnMessage();
            }
        }
        else if(cmd == HANDLING_OFFSET_LOCATION)
        {
            QString location_name = param.toString();
            states.setHandlingMessage(QString(u8"正在执行1视觉……").arg(location_name));
            if(baseModuleManage->vision_locations.contains(location_name))
            {
                performLocation(location_name,false);
            }
            else
            {
                QVariantMap message;
                message.insert("LocationName",location_name);
                message.insert("UseOrigin",false);
                sendCmdMessage(message,HANDLING_OFFSET_LOCATION);
                waitReturnMessage();
            }
        }
        else if(cmd == HANDLING_CALIBRATION)
        {
            QString location_name = param.toString();
            qInfo("Handling calibration: %s", location_name.toStdString().c_str());
            states.setHandlingMessage(QString(u8"正在执行1校正……").arg(location_name));
            if(baseModuleManage->vision_locations.contains(location_name))
            {
                performCalibration(location_name);
            }
            else
            {
                QVariantMap message;
                message.insert("LocationName",location_name);
                sendCmdMessage(message,CommandType::HANDLING_CALIBRATION);
                waitReturnMessage();
            }
        }
        else if(cmd == HANDLING_OPEN_CYLINDER)
        {
            QString cylinder_name = param.toString();
            states.setHandlingMessage(QString(u8"正在打开气缸1……").arg(cylinder_name));
            if(baseModuleManage->cylinder.contains(cylinder_name))
            {
                performCylinderOperation(cylinder_name,true);
            }
            else
            {
                QVariantMap message;
                message.insert("TargetName",cylinder_name);
                message.insert("TargetState",true);
                sendCmdMessage(message,HANDLING_OPEN_CYLINDER);
                waitReturnMessage();
            }
        }
        else if(cmd == HANDLING_CLOSE_CYLINDER)
        {
            QString cylinder_name = param.toString();
            states.setHandlingMessage(QString(u8"正在关闭气缸1……").arg(cylinder_name));
            if(baseModuleManage->cylinder.contains(cylinder_name))
            {
                performCylinderOperation(cylinder_name,false);
            }
            else
            {
                QVariantMap message;
                message.insert("TargetName",cylinder_name);
                message.insert("TargetState",false);
                sendCmdMessage(message,HANDLING_CLOSE_CYLINDER);
                waitReturnMessage();
            }
        }
        else if(cmd == HANDLING_OPEN_OUTPUT)
        {
            QString output_name = param.toString();
            states.setHandlingMessage(QString(u8"正在打开输出IO1……").arg(output_name));
            if(baseModuleManage->output_ios.contains(output_name))
            {
                performOutputOperation(output_name,true);
            }
            else
            {
                QVariantMap message;
                message.insert("TargetName",output_name);
                message.insert("TargetState",true);
                sendCmdMessage(message,HANDLING_OPEN_OUTPUT);
                waitReturnMessage();
            }
        }
        else if(cmd == HANDLING_CLOSE_OUTPUT)
        {
            QString output_name = param.toString();
            states.setHandlingMessage(QString(u8"正在关闭输出IO1……").arg(output_name));
            if(baseModuleManage->output_ios.contains(output_name))
            {
                performOutputOperation(output_name,false);
            }
            else
            {
                QVariantMap message;
                message.insert("TargetName",output_name);
                message.insert("TargetState",false);
                sendCmdMessage(message,HANDLING_CLOSE_OUTPUT);
                waitReturnMessage();
            }
        }
        else if(cmd == HANDLING_OPEN_VACUUM)
        {
            QString vacuum_name = param.toString();
            states.setHandlingMessage(QString(u8"正在打开真空1……").arg(vacuum_name));
            if(baseModuleManage->vacuums.contains(vacuum_name))
            {
                performVacuumOperation(vacuum_name,true);
            }
            else
            {
                QVariantMap message;
                message.insert("TargetName",vacuum_name);
                message.insert("TargetState",true);
                sendCmdMessage(message,HANDLING_OPEN_VACUUM);
                waitReturnMessage();
            }
        }
        else if(cmd == HANDLING_CLOSE_VACUUM)
        {
            QString vacuum_name = param.toString();
            states.setHandlingMessage(QString(u8"正在关闭真空1……").arg(vacuum_name));
            if(baseModuleManage->vacuums.contains(vacuum_name))
            {
                performVacuumOperation(vacuum_name,false);
            }
            else
            {
                QVariantMap message;
                message.insert("TargetName",vacuum_name);
                message.insert("TargetState",false);
                sendCmdMessage(message,HANDLING_CLOSE_VACUUM);
                waitReturnMessage();
            }
        }
        else if (cmd == CommandType::PERFORM_CALIBRATION)
        {
            QVariantMap message;
            message["calibration_name"] = states.currentCalibrationName();
            sendCmdMessage(message,CommandType::PERFORM_CALIBRATION);
            waitReturnMessage();

            qInfo("perform calibration name: %s", states.currentCalibrationName().toStdString().c_str());
            states.setHandlingMessage(u8"正在执行校正……");
            baseModuleManage->performCalibration(states.currentCalibrationName());
        }
        else if (cmd == CommandType::PERFORM_UPDNLOOK_CALIBRATION)
        {
            states.setHandlingMessage(u8"正在执行up&down相机校正……");
            baseModuleManage->performUpDnLookCalibration();
        }
        else if (cmd == CommandType::PERFORM_UV)
        {
            states.setHandlingMessage(u8"正在执行UV……");
            performUV();
        }
        else if (cmd == CommandType::LOAD_LENS_FROM_TRAY_1_AND_PICK_TO_AA1)
        {
            states.setHandlingMessage(u8"正在执行从盘1上Lens到AA1……");
            baseModuleManage->lut_module.moveToLoadPos(true, false);
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LENS_TRAY1+
                                                                 baseModuleManage->lens_loader_module.LENS_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PICK_LENS_FROM_TRAY);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LUT_POS1+
                                                                 baseModuleManage->lens_loader_module.RESET_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PLACE_LENS_TO_LUT);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            baseModuleManage->lut_module.moveToAA1PickLens(false,false,true);
            baseModuleManage->lut_module.moveToLoadPos(true, false);
            qInfo("LOAD_LENS_FROM_TRAY_1_AND_PICK_TO_AA1 complete!");
        }
        else if (cmd == CommandType::LOAD_LENS_FROM_TRAY_2_AND_PICK_TO_AA1)
        {
            states.setHandlingMessage(u8"正在执行从盘2上Lens到AA1……");
            baseModuleManage->lut_module.moveToLoadPos(true, false);
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LENS_TRAY2+
                                                                 baseModuleManage->lens_loader_module.LENS_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PICK_LENS_FROM_TRAY);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LUT_POS1+
                                                                 baseModuleManage->lens_loader_module.RESET_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PLACE_LENS_TO_LUT);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            baseModuleManage->lut_module.moveToAA1PickLens(false,false,true);
            baseModuleManage->lut_module.moveToLoadPos(true, false);
            qInfo("LOAD_LENS_FROM_TRAY_2_AND_PICK_TO_AA1 complete!");
        }
        else if (cmd == CommandType::LOAD_LENS_FROM_TRAY_1_AND_PICK_TO_AA2)
        {
            states.setHandlingMessage(u8"正在执行从盘1上Lens到AA2……");
            baseModuleManage->lut_module.moveToLoadPos(true, false);
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LENS_TRAY1+
                                                                 baseModuleManage->lens_loader_module.LENS_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PICK_LENS_FROM_TRAY);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LUT_POS1+
                                                                 baseModuleManage->lens_loader_module.RESET_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PLACE_LENS_TO_LUT);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            baseModuleManage->lut_module.moveToAA2PickLens(false,false,true);
            baseModuleManage->lut_module.moveToLoadPos(true, false);
            qInfo("LOAD_LENS_FROM_TRAY_1_AND_PICK_TO_AA2 complete!");
        }
        else if (cmd == CommandType::LOAD_LENS_FROM_TRAY_2_AND_PICK_TO_AA2)
        {
            states.setHandlingMessage(u8"正在执行从盘2上Lens到AA1……");
            baseModuleManage->lut_module.moveToLoadPos(true, false);
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LENS_TRAY2+
                                                                 baseModuleManage->lens_loader_module.LENS_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PICK_LENS_FROM_TRAY);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LUT_POS1+
                                                                 baseModuleManage->lens_loader_module.RESET_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PLACE_LENS_TO_LUT);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            baseModuleManage->lut_module.moveToAA2PickLens(false,false,true);
            baseModuleManage->lut_module.moveToLoadPos(true, false);
            qInfo("LOAD_LENS_FROM_TRAY_2_AND_PICK_TO_AA2 complete!");
        }
        else if (cmd == CommandType::UNPICK_LENS_FROM_AA1_AND_PLACE_TO_TRAY1)
        {
            states.setHandlingMessage(u8"正在执行从AA1取Lens到盘1……");
            baseModuleManage->lut_module.moveToAA1UnPickLens(true,true);
            baseModuleManage->lut_module.moveToLoadPos(true, false);
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LUT_POS2+
                                                                 baseModuleManage->lens_loader_module.LUT_LENS_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PICK_NG_LENS_FROM_LUT2);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LENS_TRAY1+
                                                                 baseModuleManage->lens_loader_module.VACANCY_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PLACE_NG_LENS_TO_TRAY);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            qInfo("UNPICK_LENS_FROM_AA1_AND_PLACE_TO_TRAY1 complete!");
        }
        else if (cmd == CommandType::UNPICK_LENS_FROM_AA2_AND_PLACE_TO_TRAY1)
        {
            states.setHandlingMessage(u8"正在执行从AA2取Lens到盘1……");
            baseModuleManage->lut_module.moveToAA2UnPickLens(true,true);
            baseModuleManage->lut_module.moveToLoadPos(true, false);
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LUT_POS2+
                                                                 baseModuleManage->lens_loader_module.LUT_LENS_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PICK_NG_LENS_FROM_LUT2);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LENS_TRAY1+
                                                                 baseModuleManage->lens_loader_module.VACANCY_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PLACE_NG_LENS_TO_TRAY);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            qInfo("UNPICK_LENS_FROM_AA2_AND_PLACE_TO_TRAY1 complete!");
        }
        else if (cmd == CommandType::UNPICK_LENS_FROM_AA1_AND_PLACE_TO_TRAY2)
        {
            states.setHandlingMessage(u8"正在执行从AA1取Lens到盘2……");
            baseModuleManage->lut_module.moveToAA1UnPickLens(true,true);
            baseModuleManage->lut_module.moveToLoadPos(true, false);
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LUT_POS2+
                                                                 baseModuleManage->lens_loader_module.LUT_LENS_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PICK_NG_LENS_FROM_LUT2);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LENS_TRAY2+
                                                                 baseModuleManage->lens_loader_module.VACANCY_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PLACE_NG_LENS_TO_TRAY);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            qInfo("UNPICK_LENS_FROM_AA1_AND_PLACE_TO_TRAY2 complete!");
        }
        else if (cmd == CommandType::UNPICK_LENS_FROM_AA2_AND_PLACE_TO_TRAY2)
        {
            states.setHandlingMessage(u8"正在执行从AA2取Lens到盘2……");
            baseModuleManage->lut_module.moveToAA2UnPickLens(true,true);
            baseModuleManage->lut_module.moveToLoadPos(true, false);
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LUT_POS2+
                                                                 baseModuleManage->lens_loader_module.LUT_LENS_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PICK_NG_LENS_FROM_LUT2);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            baseModuleManage->lens_loader_module.performHandling(baseModuleManage->lens_loader_module.LENS_TRAY2+
                                                                 baseModuleManage->lens_loader_module.VACANCY_PR+
                                                                 baseModuleManage->lens_loader_module.ToWork+
                                                                 baseModuleManage->lens_loader_module.PLACE_NG_LENS_TO_TRAY);
            baseModuleManage->lens_loader_module.waitPerformHandling();
            qInfo("UNPICK_LENS_FROM_AA2_AND_PLACE_TO_TRAY2 complete!");
        }
        else if (cmd == CommandType::SAVE_PARAMETERS)
        {
            states.setHandlingMessage(u8"正在保存参数");
            baseModuleManage->updateParams();
            QThread::msleep(500);
        }
        else if (cmd == CommandType::LOAD_PARAMETERS)
        {
            states.setHandlingMessage(u8"正在加载参数");
            baseModuleManage->loadconfig();
            QThread::msleep(500);
        }
    }
    else if(baseModuleManage->workers.contains(module_name))//单一模块动作
    {
        states.setHandlingMessage(module_name);
        baseModuleManage->workers[module_name]->performHandling(cmd,param);
    }
    else {
        states.setHandlingMessage(module_name);
        QVariantMap message;
        message.insert("ModuleName", module_name);
        message.insert("Params", param);
        qDebug("sendCmdMessage");
        sendCmdMessage(message,cmd);
        waitReturnMessage();
        qDebug("Wait return message success");
    }
    qInfo("LogicManager performHandlingOperation complete!");
    is_handling = false;
    states.setIsHandling(false);
}

void LogicManager::performTcpOperation(QVariantMap message)
{
    qInfo("receiveMessageFromWorkerMangers");
    if(message.contains("Message"))
    {
        qInfo("receiveMessageFromWorkerManger Message %s",message["Message"].toString().toStdString().c_str());
        if(message["Message"].toString() == "OpenSutVacuum")
        {
            bool result = baseModuleManage->sut_module.OpenSutVacuum();
            if(message.contains("OriginModule"))
            {
                sendMessageToModule(message["OriginModule"].toString(),result?"OpenSutVacuumSuccess":"OpenSutVacuumFail");
            }
        }
        else if(message["Message"].toString() == "CloseSutVacuum")
        {
            bool result = baseModuleManage->sut_module.CloseSutVacuum();
            if(message.contains("OriginModule"))
            {
                sendMessageToModule(message["OriginModule"].toString(),result?"CloseSutVacuumSuccess":"CloseSutVacuumFail");
            }
        }
        else if(message["Message"].toString() == "AAHeadMoveToPickPos")
        {
            bool result = baseModuleManage->aa_head_module.moveToPickLensPosition();
            if(message.contains("OriginModule"))
            {
                sendMessageToModule(message["OriginModule"].toString(),result?"AAHeadMoveToPickPosSuccess":"AAHeadMoveToPickPosFail");
            }
        }
        else if(message["Message"].toString() == "OpenGripper")
        {
            bool result = baseModuleManage->aa_head_module.openGripper();
            if(message.contains("OriginModule"))
            {
                sendMessageToModule(message["OriginModule"].toString(),result?"OpenGripperSuccess":"OpenGripperFail");
            }
        }
        else if(message["Message"].toString() == "CloseGripper")
        {
            bool result = baseModuleManage->aa_head_module.closeGripper();
            if(message.contains("OriginModule"))
            {
                sendMessageToModule(message["OriginModule"].toString(),result?"CloseGripperSuccess":"CloseGripperFail");
            }
        }
        else if(message["Message"].toString() == "OpenAlarmLight")
        {
            baseModuleManage->setOutput(u8"三色报警指示灯_绿", false);
            baseModuleManage->setOutput(u8"三色报警指示灯_黄", false);
            baseModuleManage->setOutput(u8"三色报警指示灯_红", true);
            //baseModuleManage->setOutput(u8"三色报警指示灯_蜂鸣器",true);
        }
        else if(message["Message"].toString() == "CloseAlarmLight")
        {
            baseModuleManage->setOutput(u8"三色报警指示灯_绿", true);
            baseModuleManage->setOutput(u8"三色报警指示灯_黄", false);
            baseModuleManage->setOutput(u8"三色报警指示灯_红", false);
            //baseModuleManage->setOutput(u8"三色报警指示灯_蜂鸣器",false);
        }
        else if (message["Message"].toString() == "StopAllWorkers")
        {
            QVariantMap message_map;
            message_map.insert("TargetModule","WorksManager");
            message_map.insert("OriginModule",parameters.moduleName());
            message_map.insert("Message","StopAllWorkers");
            emit sendMessageToWorkerManger(message_map);
        }
        else if (message["Message"].toString() == "InquiryInputIOState")
        {
            qInfo("receive InquiryInputIOState message, ioName = %s", message["InputIOName"].toString().toStdString().c_str());
            QString ioName = message["InputIOName"].toString();
            XtGeneralInput *temp_io = baseModuleManage->GetInputIoByName(ioName);
            if (temp_io == nullptr)
            {
                sendMessageToModule(message["OriginModule"].toString(), "fail");
            }
            else
            {
                sendMessageToModule(message["OriginModule"].toString(), temp_io->Value()?"true":"false");
            }
        }
    }
    else if(message.contains("performHandling"))
    {
        states.setIsHandling(true);
        bool result = true;

        qInfo("receiveMessageFromWorkerManger performHandling %d Params: %s",message["performHandling"].toInt(), message["Paramsss"].toString().toStdString().c_str());
        QString module_name = message["ModuleName"].toString();
        if(baseModuleManage->workers.contains(module_name))
        {
            QString params = message["Params"].toString();
            baseModuleManage->workers[module_name]->performHandling(message["performHandling"].toInt(), QVariant(params));
            result = baseModuleManage->workers[module_name]->waitPerformHandling();
            QVariantMap  return_message;
            return_message.insert("TargetModule",message["OriginModule"]);
            sendRespMessage(return_message,result);
            states.setIsHandling(false);
            return;
        }
        else if(message["performHandling"].toInt() == CommandType::MOTION_HOME)
        {
            states.setHandlingMessage(u8"正在回零……");
            if(message.contains("MotorName"))
            {
                result = motorSeekOrigin(message["MotorName"].toString());
            }
        }
        else if (message["performHandling"].toInt() == CommandType::MOTION_INIT)
        {
            states.setHandlingMessage(u8"正在初始化……");
            result = baseModuleManage->initialDevice();
        }
        else if (message["performHandling"].toInt() == HANDLING_ORIGIN_LOCATION
                 ||message["performHandling"].toInt() == HANDLING_OFFSET_LOCATION)
        {
            QString location_name = message["LocationName"].toString();
            if(baseModuleManage->vision_locations.contains(location_name))
                result = performLocation(location_name,message["UseOrigin"].toBool());
        }
        else if (message["performHandling"].toInt() == HANDLING_CALIBRATION)
        {
            QString location_name = message["LocationName"].toString();
            if(baseModuleManage->vision_locations.contains(location_name))
                result = performCalibration(location_name);
        }
        else if(message["performHandling"].toInt() == HANDLING_OPEN_CYLINDER
                ||message["performHandling"].toInt() == HANDLING_CLOSE_CYLINDER)
        {
            QString target_name = message["TargetName"].toString();
            if(baseModuleManage->cylinder.contains(target_name))
                performCylinderOperation(target_name,message["TargetState"].toBool());
        }
        else if(message["performHandling"].toInt() == HANDLING_OPEN_OUTPUT
                ||message["performHandling"].toInt() == HANDLING_CLOSE_OUTPUT)
        {
            QString target_name = message["TargetName"].toString();
            if(baseModuleManage->output_ios.contains(target_name))
                performOutputOperation(target_name,message["TargetState"].toBool());
        }
        else if(message["performHandling"].toInt() == HANDLING_OPEN_VACUUM
                ||message["performHandling"].toInt() == HANDLING_CLOSE_VACUUM)
        {
            QString target_name = message["TargetName"].toString();
            if(baseModuleManage->vacuums.contains(target_name))
                performVacuumOperation(target_name,message["TargetState"].toBool());
        }

        return_message.insert("TargetModule","LogicManager");
        sendRespMessage(return_message,result);
        states.setIsHandling(false);
    }
}

void LogicManager::receiveCommand(int cmd)
{
    qInfo("receive command: %d", cmd);
    if (cmd == CommandType::MOTION_INIT) {
        init();
    }
}

void LogicManager::moveToCmd(int)
{

}

void LogicManager::receiveMessageFromWorkerManger(QVariantMap message)
{
    qInfo("receiveMessageFromWorkerManger");
    if(message.contains("performHandlingResp"))
    {
        qInfo("receiveMessageFromWorkerManger performHandlingResp %d",message["performHandlingResp"].toBool());
        if(!return_message.contains("performHandlingResp"))
            return_message.insert("performHandlingResp",message["performHandlingResp"].toBool());
    }
    else
    {
        emit sendPerformTcp(message);
        qInfo("sendPerformTcp");
    }
}
