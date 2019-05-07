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
    PERFORM_LOCATION,
    PERFORM_OC,
    PERFORM_LOOP_TEST
};

LogicManager::LogicManager(BaseModuleManager* device_manager,QObject *parent)
    : QThread (parent), m_currentMode(CommandType::IDLE)
{
    aaCore = new AACore(&device_manager->aa_head_module,device_manager->lutClient,&device_manager->sut_module,
                        device_manager->dothinkey, device_manager->chart_calibration,&device_manager->dispense_module,
                        device_manager->imageGrabberThread);
    baseModuleManage = device_manager;
}

void LogicManager::updateParams()
{
    aaCore->updateParams();
}

void LogicManager::run() {
    qInfo("Logic manager is running");
    QString uuid = baseModuleManage->unitlog.createUnit();
    if (m_currentMode == CommandType::MOTION_STOP_HOME) {
        baseModuleManage->stopSeeking();
    }
    else if (m_currentMode == CommandType::MODE_AUTO_RUN) {
//        aaCore->performLoopTest(AA_DIGNOSTICS_MODE::AA_AUTO_MODE, uuid);
//        aaCore->wait();
    }
    else if (m_currentMode == CommandType::PERFORM_LOOP_TEST) {
        aaCore->performLoopTest(AA_DIGNOSTICS_MODE::AA_MTF_TEST_MODE, uuid);
        return;
    }
    else if (m_currentMode == CommandType::PERFORM_OC) {
        aaCore->performOC(true, true);
    }
    else if (m_currentMode == CommandType::MOTION_HOME) {
        baseModuleManage->allMotorsSeekOrigin();
    } else if (m_currentMode == CommandType::MOTION_INIT) {
        baseModuleManage->initialDevice();
    }
    else if (m_currentMode == CommandType::AA_MOVETO_MUSHROOM_CMD)
    {
        baseModuleManage->aa_head_module.moveToMushroomPosition();
    } else if (m_currentMode == CommandType::AA_MOVETO_PICK_LENS_CMD)
    {
        baseModuleManage->aa_head_module.moveToPickLensPosition();
    } else if (m_currentMode == CommandType::AA_MOVETO_OC_CMD)
    {
        baseModuleManage->aa_head_module.moveToOCPosition();
    } else if (m_currentMode == CommandType::SUT_MOVETO_PR_CMD)
    {
        baseModuleManage->sut_module.moveToDownlookPos();
    } else if (m_currentMode == CommandType::SUT_MOVETO_MUSHROOM_CMD)
    {
        baseModuleManage->sut_module.moveToMushroomPos();
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
        aaCore->performLoopTest(AA_DIGNOSTICS_MODE::AA_IDLE_MODE, "");
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
    aaCore->setFlowchartDocument(json);
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

void LogicManager::performLocation(QString location_name)
{
    setStateMessage(__FUNCTION__);
    this->location_name = location_name;
    moveToCmd(CommandType::PERFORM_LOCATION);
}


void LogicManager::autoRun(QString json){
    aaCore->setFlowchartDocument(json);
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
    baseModuleManage->tray_loader_module.LTIEMovetoColumnIndex(0);
}

void LogicManager::trayLoaderModuleLTIEMovetoLastPos()
{
    int col = baseModuleManage->tray_loader_module.tray_clip->standards_parameters.columnCount()-1;
    baseModuleManage->tray_loader_module.LTIEMovetoColumnIndex(col);
}

void LogicManager::trayLoaderModuleLTOEMovetoFirstPos()
{
    baseModuleManage->tray_loader_module.LTOEMovetoColumnIndex(0);
}

void LogicManager::trayLoaderModuleLTOEMovetoLastPos()
{
    int col = baseModuleManage->tray_loader_module.tray_clip->standards_parameters.columnCount()-1;
    baseModuleManage->tray_loader_module.LTOEMovetoColumnIndex(col);
}

void LogicManager::trayLoaderModuleLTKX1MovetoGetPos()
{
    baseModuleManage->tray_loader_module.moveToLtkx1GetPos();
}

void LogicManager::trayLoaderModuleLTKX1MovetoSetPos()
{
    baseModuleManage->tray_loader_module.moveToLtkx1SetPos();
}

void LogicManager::trayLoaderModuleLTKX2MovetoGetPos()
{
    baseModuleManage->tray_loader_module.moveToLtkx2GetPos();
}

void LogicManager::trayLoaderModuleLTKX2MovetoSetPos()
{
    baseModuleManage->tray_loader_module.moveToLtkx2SetPos();
}

void LogicManager::trayLoaderModuleLTLMovetoGetPos()
{
    baseModuleManage->tray_loader_module.moveToLtlGetPos();
}

void LogicManager::trayLoaderModuleLTLMovetoSetPos()
{
    baseModuleManage->tray_loader_module.moveToLtlSetPos();
}

void LogicManager::trayLoaderModuleLTIEMovetoColumnIndex(int idx)
{
    baseModuleManage->tray_loader_module.LTIEMovetoColumnIndex(idx);
}

void LogicManager::trayLoaderModuleLTOEMovetoColumnIndex(int idx)
{
    baseModuleManage->tray_loader_module.LTOEMovetoColumnIndex(idx);
}

void LogicManager::trayLoaderModuleEjectTray()
{
    baseModuleManage->tray_loader_module.ejectTray();
}

void LogicManager::trayLoaderModuleLTKX1CylinderOn()
{
    baseModuleManage->tray_loader_module.motorInPress();
}

void LogicManager::trayLoaderModuleLTKX1CylinderOff()
{
    baseModuleManage->tray_loader_module.motorInRelease();
}

void LogicManager::trayLoaderModuleLTKX2CylinderOn()
{
    baseModuleManage->tray_loader_module.motorOutPress();
}

void LogicManager::trayLoaderModuleLTKX2CylinderOff()
{
    baseModuleManage->tray_loader_module.motorOutRelease();
}

void LogicManager::trayLoaderModuleLTLXCylinderOn()
{
    baseModuleManage->tray_loader_module.motorWorkPress();
}

void LogicManager::trayLoaderModuleLTLXCylinderOff()
{
    baseModuleManage->tray_loader_module.motorWorkRelease();
}

void LogicManager::trayLoaderModuleStartup()
{
    baseModuleManage->tray_loader_module.startUp();
    emit baseModuleManage->tray_loader_module.parameters.trayReady();
    //emit baseModuleManage->tray_loader_module.parameters.trayReady();
}

void LogicManager::trayLoaderModuleEmitTestTrayUsed(){
    emit baseModuleManage->tray_loader_module.testTrayUsed();
}

void LogicManager::trayLoaderModuleLTKX1MoveToRelayPos()
{
    baseModuleManage->tray_loader_module.moveToLtkx1RelayPos();
}

