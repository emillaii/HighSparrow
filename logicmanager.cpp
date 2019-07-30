#include "logicmanager.h"
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QMessageBox>

LogicManager::LogicManager(BaseModuleManager* device_manager,QObject *parent)
    : QThread (parent), m_currentMode(CommandType::IDLE)
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

void LogicManager::performHandling(int cmd)
{

}

void LogicManager::updateParams()
{
    //aaCore->updateParams();
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
        sendRespMessage(return_message,result);
    } else if (m_currentMode == CommandType::MOTION_INIT_ONLYSELF) {
        result = baseModuleManage->initialDevice();
        QVariantMap  return_message;
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
        baseModuleManage->lut_module.moveToAA1PickLens(false,true);
    }
    else if (m_currentMode == CommandType::LUT_PICK_LENS_TO_AA1_CMD1)
    {
        baseModuleManage->lut_module.moveToAA1PickLens(true,true);
    }
    else if (m_currentMode == CommandType::LUT_PICK_LENS_TO_AA2_CMD)
    {
        qInfo("Move To AA2 PickLens");
        baseModuleManage->lut_module.moveToAA2PickLens(false,true);
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
        home_only_self();
//        waitReturnMessage();
        return result;
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
    emit sendMessageToWorkerManger(message);
}

void LogicManager::sendRespMessage(QVariantMap message, bool result)
{
    message.insert("TargetModule","LogicManager");
    message.insert("performHandlingResp",result);
    emit sendMessageToWorkerManger(message);
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
void LogicManager::moveToCmd(int cmd) {
    if (cmd == CommandType::STOP)
    {
        m_currentMode = CommandType::IDLE;
        return;
    }

    if (m_currentMode == CommandType::IDLE || cmd == CommandType::MOTION_STOP_HOME)
    {
        setCurrentMode(cmd);
        this->start();
    }
    else {
        qInfo("%s Fail ! Due to previous command is running", __FUNCTION__);
    }
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

void LogicManager::performCalibration(QString calibration_name)
{
    setStateMessage(__FUNCTION__);
    this->calibration_name = calibration_name;
    moveToCmd(CommandType::PERFORM_CALIBRATION);
}

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

void LogicManager::performLocation(QString location_name,bool use_origin)
{
    setStateMessage(__FUNCTION__);
    this->location_name = location_name;
    this->use_origin = use_origin;
    moveToCmd(CommandType::PERFORM_LOCATION);
}

void LogicManager::autoRun(QString json){
    setStateMessage(__FUNCTION__);moveToCmd(CommandType::MODE_AUTO_RUN);
}

void LogicManager::performOC(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::PERFORM_OC);}
void LogicManager::performLoopTest(int mode){
    qInfo("Loop Test Mode: %d", mode);
    setStateMessage(__FUNCTION__);
    moveToCmd(CommandType::PERFORM_LOOP_TEST);
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
                                                         LensLoaderModule::HandlePickerAction::PICK_NG_LENS_FROM_LUT);
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
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::UPDOWNLOOK_DOWN_POS);
}

void LogicManager::lensPickArmMoveToUpdownlookUpPos()
{
    baseModuleManage->lens_loader_module.performHandling(LensLoaderModule::HandlePosition::UPDOWNLOOK_UP_POS);
}

void LogicManager::trayLoaderModuleLTIEMovetoFirstPos()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.LTIEMovetoColumnIndex(0);
}

void LogicManager::trayLoaderModuleLTIEMovetoLastPos()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    int col = baseModuleManage->tray_loader_module.tray_clip->standards_parameters.columnCount()-1;
    baseModuleManage->tray_loader_module.LTIEMovetoColumnIndex(col);
}

void LogicManager::trayLoaderModuleLTOEMovetoFirstPos()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.LTOEMovetoColumnIndex(0);
}

void LogicManager::trayLoaderModuleLTOEMovetoLastPos()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    int col = baseModuleManage->tray_loader_module.tray_clip->standards_parameters.columnCount()-1;
    baseModuleManage->tray_loader_module.LTOEMovetoColumnIndex(col);
}

void LogicManager::trayLoaderModuleLTKX1MovetoGetPos()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.moveToLtkx1GetPos();
}

void LogicManager::trayLoaderModuleLTKX1MovetoSetPos()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.moveToLtkx1SetPos();
}

void LogicManager::trayLoaderModuleLTKX2MovetoGetPos()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.moveToLtkx2GetPos();
}

void LogicManager::trayLoaderModuleLTKX2MovetoSetPos()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.moveToLtkx2SetPos();
}

void LogicManager::trayLoaderModuleLTLMovetoGetPos()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.moveToLtlGetPos();
}

void LogicManager::trayLoaderModuleLTLMovetoSetPos()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.moveToLtlSetPos();
}

void LogicManager::trayLoaderModuleLTIEMovetoColumnIndex(int idx)
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.LTIEMovetoColumnIndex(idx);
}

void LogicManager::trayLoaderModuleLTOEMovetoColumnIndex(int idx)
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.LTOEMovetoColumnIndex(idx);
}

void LogicManager::trayLoaderModuleEjectTray()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.ejectTray();
}

void LogicManager::trayLoaderModuleLTKX1CylinderOn()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.motorInPress();
}

void LogicManager::trayLoaderModuleLTKX1CylinderOff()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.motorInRelease();
}

void LogicManager::trayLoaderModuleLTKX2CylinderOn()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.motorOutPress();
}

void LogicManager::trayLoaderModuleLTKX2CylinderOff()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.motorOutRelease();
}

void LogicManager::trayLoaderModuleLTLXCylinderOn()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.motorWorkPress();
}

void LogicManager::trayLoaderModuleLTLXCylinderOff()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.motorWorkRelease();
}

void LogicManager::trayLoaderModuleStartup()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.startUp();
    emit baseModuleManage->tray_loader_module.parameters.trayReady();
    //emit baseModuleManage->tray_loader_module.parameters.trayReady();
}

void LogicManager::trayLoaderModuleEmitTestTrayUsed(){
    emit baseModuleManage->tray_loader_module.testTrayUsed();
}

void LogicManager::trayLoaderModuleLTKX1MoveToRelayPos()
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否移动？"),QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::No){
        return;
    }
    baseModuleManage->tray_loader_module.moveToLtkx1RelayPos();
}

bool LogicManager::trayLoaderModuleCheckLTLXGetPos(double x)
{
    double max_range = baseModuleManage->GetMotorByName(baseModuleManage->tray_loader_module.parameters.motorLTKX1Name())->GetPostiveRange();
    double min_range = baseModuleManage->GetMotorByName(baseModuleManage->tray_loader_module.parameters.motorLTKX1Name())->GetNegativeRange();
    if(x+166.7538>max_range||x+166.7538-310<min_range){
        QMessageBox::warning(nullptr,tr(u8"提示"),tr(u8"LTK_X1无法移动到此位置，请重新选择"),QMessageBox::Ok);
        return false;
    }
    return true;
}

bool LogicManager::trayLoaderModuleCheckLTLXSetPos(double x)
{
    double min_range = baseModuleManage->GetMotorByName(baseModuleManage->tray_loader_module.parameters.motorLTKX2Name())->GetNegativeRange();
    if(x-653<min_range){
        QMessageBox::warning(nullptr,tr(u8"提示"),tr(u8"LTK_X2无法移动到此位置，请重新选择"),QMessageBox::Ok);
        return false;
    }
    return true;
}

void LogicManager::trayLoaderModuleLTLXPickUpTray()
{
    baseModuleManage->tray_loader_module.onTestLTLXPickUpTray();
}

void LogicManager::trayLoaderModuleLTLXPutDownTray()
{
    baseModuleManage->tray_loader_module.onTestLTLXPutDownTray();
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

void LogicManager::receiveCommand(int cmd)
{
    qInfo("receive command: %d", cmd);
    if (cmd == CommandType::MOTION_INIT) {
        init();
    }
}

void LogicManager::receiveMessageFromWorkerManger(QVariantMap message)
{
    if(message.contains("performHandling"))
    {
        //is_handling = true;
        QString module_name = message["ModuleName"].toString();
        if(baseModuleManage->workers.contains(module_name))
        {
            baseModuleManage->workers[module_name]->performHandling(message["performHandling"].toInt());
            result = baseModuleManage->workers[module_name]->waitPerformHandling();
            QVariantMap  return_message;
            sendRespMessage(return_message,result);
        }
        else if(message["performHandling"].toInt() == CommandType::MOTION_HOME)
        {
            if(message.contains("MotorName"))
            {
                motorSeekOrigin(message["MotorName"].toString());
            }
        }
        else if (message["performHandling"].toInt() == CommandType::MOTION_INIT)
        {
            init_only_self();
//            waitReturnMessage();
//            QVariantMap  return_message;
//            sendRespMessage(return_message,result);
        }
        //is_handling = false;
    }
    else if(message.contains("performHandlingResp"))
    {
        return_message.insert("performHandlingResp",message["performHandlingResp"].toBool());
    }
}
