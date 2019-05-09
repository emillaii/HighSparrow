#include "logicmanager.h"
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QMessageBox>

enum CommandType{
    IDLE,
    STOP,
    MOTION_INIT,
    MOTION_HOME,
    MOTION_STOP_HOME,     // Interrupt
    MODE_AUTO_RUN,
    AA_MOVETO_MUSHROOM_CMD,
    AA_MOVETO_PICK_LENS_CMD,
    AA_MOVETO_OC_CMD,
    SUT_MOVETO_MUSHROOM_CMD,
    SUT_MOVETO_PR_CMD,
    LUT_MOVETO_LOAD_CMD,
    LUT_MOVETO_UNLOAD_CMD,
    LUT_MOVETO_LOAD_UPLOOK_CMD,
    LUT_MOVETO_AA1_UPLOOK_CMD,
    LUT_MOVETO_AA2_UPLOOK_CMD,
    LUT_PICK_LENS_TO_AA2_CMD,
    LUT_PICK_LENS_TO_AA1_CMD,
    PERFORM_CALIBRATION,
    PERFORM_UPDNLOOK_CALIBRATION,
    PERFORM_LENS_UPDNLOOK_CALIBRATION,
    PERFORM_SENSOR_PICKHEAD_CALIBRATION,
    PERFORM_LOCATION,
    PERFORM_OC,
    PERFORM_LOOP_TEST
};

LogicManager::LogicManager(BaseModuleManager* device_manager,QObject *parent)
    : QThread (parent), m_currentMode(CommandType::IDLE)
{
//    aaCore = new AACore(&device_manager->aa_head_module,device_manager->lutClient,&device_manager->sut_module,
//                        device_manager->dothinkey, device_manager->chart_calibration,&device_manager->dispense_module,
//                        device_manager->imageGrabberThread);
    baseModuleManage = device_manager;
}

void LogicManager::updateParams()
{
    //aaCore->updateParams();
}

void LogicManager::run() {
    qInfo("Logic manager is running");
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
        baseModuleManage->allMotorsSeekOrigin();
    } else if (m_currentMode == CommandType::MOTION_INIT) {
        baseModuleManage->initialDevice();
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
        baseModuleManage->lut_module.moveToUnloadPos();
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
    else if (m_currentMode == CommandType::LUT_PICK_LENS_TO_AA2_CMD)
    {
        qInfo("Move To AA2 PickLens");
        baseModuleManage->lut_module.moveToAA2PickLens(false,true);
    }
    else if (m_currentMode == CommandType::PERFORM_LOCATION)
    {
        baseModuleManage->performLocation(location_name);
        qInfo("location End");
    }

    m_currentMode = CommandType::IDLE;
    qInfo("Logic Manager End");
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

void LogicManager::loadFlowchart(QString json)
{
}

void LogicManager::home(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::MOTION_HOME);}
void LogicManager::init(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::MOTION_INIT);}
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

void LogicManager::performLocation(QString location_name)
{
    setStateMessage(__FUNCTION__);
    this->location_name = location_name;
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
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LENS_TRAY1);
}

void LogicManager::lensPickArmMoveToTray2Pos()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LENS_TRAY2);
}

void LogicManager::lensPickArmMoveToTray1StartPos()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LENS_TRAY1_START_POS);
}

void LogicManager::lensPickArmMoveToTray2StartPos()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LENS_TRAY2_START_POS);
}

void LogicManager::lensPickArmMoveToTray1EndPos()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LENS_TRAY1_END_POS);
}

void LogicManager::lensPickArmMoveToLutPos1()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LUT_POS1);
}

void LogicManager::lensPickArmMoveToLutPos2()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LUT_POS2);
}

void LogicManager::lensPickArmMoveToPickLensFromTray1()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LENS_TRAY1+
                                                           HandlePR::LENS_PR+
                                                           HandleToWorkPos::ToWork+
                                                           handlePickerAction::PICK_LENS_FROM_TRAY);
}

void LogicManager::lensPickArmMoveToPickLensFromTray2()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LENS_TRAY2+
                                                           HandlePR::LENS_PR+
                                                           HandleToWorkPos::ToWork+
                                                           handlePickerAction::PICK_LENS_FROM_TRAY);
}

void LogicManager::lensPickArmMoveToPickLensFromLut()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LUT_POS2+
                                                           HandlePR::LUT_PR+
                                                           HandleToWorkPos::ToWork+
                                                           handlePickerAction::PICK_NG_LENS_FROM_LUT);
}

void LogicManager::lensPickArmMoveToPlaceLensToTray1()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LENS_TRAY1+
                                                           HandlePR::VACANCY_PR+
                                                           HandleToWorkPos::ToWork+
                                                           handlePickerAction::PLACE_NG_LENS_TO_TRAY);
}

void LogicManager::lensPickArmMoveToPlaceLensToTray2()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LENS_TRAY2+
                                                           HandlePR::VACANCY_PR+
                                                           HandleToWorkPos::ToWork+
                                                           handlePickerAction::PLACE_NG_LENS_TO_TRAY);
}

void LogicManager::lensPickArmMoveToPlaceLensToLut()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::LUT_POS1+
                                                           HandlePR::RESET_PR+
                                                           HandleToWorkPos::ToWork+
                                                           handlePickerAction::PLACE_LENS_TO_LUT);
}

void LogicManager::lensPickArmLensPR()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePR::LENS_PR+HandleToWorkPos::ToWork);
}

void LogicManager::lensPickArmVacancyTrayPR()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePR::VACANCY_PR+HandleToWorkPos::ToWork);
}

void LogicManager::lensPickArmLUTPR()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePR::LUT_PR+HandleToWorkPos::ToWork);
}

void LogicManager::lensPickArmLensMeasureHeight()
{
 baseModuleManage->lens_loader_module.performHandling( handlePickerAction::MeasureLensInTray);
}

void LogicManager::lensPickArmLUTMeasureHeight()
{
    baseModuleManage->lens_loader_module.performHandling( handlePickerAction::MeasureLensInLUT);
}

void LogicManager::lensPickArmMoveToUpdownlookDownPos()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::UPDOWNLOOK_DOWN_POS);
}

void LogicManager::lensPickArmMoveToUpdownlookUpPos()
{
    baseModuleManage->lens_loader_module.performHandling(HandlePosition::UPDOWNLOOK_UP_POS);
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

