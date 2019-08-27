#include "logicmanager.h"
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QMessageBox>

LogicManager::LogicManager(BaseModuleManager* device_manager,QObject *parent)
    : QThread (parent), m_currentMode(CommandType::IDLE)
{
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

void LogicManager::autoRun(QString json)
{
    setStateMessage(__FUNCTION__);moveToCmd(CommandType::MODE_AUTO_RUN);
}

void LogicManager::performOC(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::PERFORM_OC);}
void LogicManager::performLoopTest(int mode)
{
    qInfo("Loop Test Mode: %d", mode);
    setStateMessage(__FUNCTION__);
    moveToCmd(CommandType::PERFORM_LOOP_TEST);
}

void LogicManager::pickArmMoveToLensTray1Pos()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePosition::LENS_TRAY1);
}

void LogicManager::pickArmMoveToLensTray2Pos()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePosition::LENS_TRAY2);
}

void LogicManager::pickArmMoveToLensTray1StartPos()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePosition::LENS_TRAY1_START_POS);
}

void LogicManager::pickArmMoveToLensTray2StartPos()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePosition::LENS_TRAY2_START_POS);
}

void LogicManager::pickArmMoveToLensTray1EndPos()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePosition::LENS_TRAY1_END_POS);
}

void LogicManager::pickArmMoveToLutPos()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePosition::LUT_POS1);
}


void LogicManager::pickArmMoveToPickLensFromTray1()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePosition::LENS_TRAY1+
                                                           HandlePR::LENS_PR+
                                                           HandleToWorkPos::ToWork+
                                                           handlePickerAction::PICK_LENS_FROM_TRAY);
}

void LogicManager::pickArmMoveToPickLensFromTray2()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePosition::LENS_TRAY2+
                                                           HandlePR::LENS_PR+
                                                           HandleToWorkPos::ToWork+
                                                           handlePickerAction::PICK_LENS_FROM_TRAY);
}

void LogicManager::pickArmMoveToPickLensFromLut()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePosition::LUT_POS2+
                                                           HandlePR::LUT_PR+
                                                           HandleToWorkPos::ToWork+
                                                           handlePickerAction::PICK_NG_LENS_FROM_LUT);
}

void LogicManager::pickArmMoveToPlaceLensToTray1()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePosition::LENS_TRAY1+
                                                           HandlePR::VACANCY_PR+
                                                           HandleToWorkPos::ToWork+
                                                           handlePickerAction::PLACE_NG_LENS_TO_TRAY);
}

void LogicManager::pickArmMoveToPlaceLensToTray2()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePosition::LENS_TRAY2+
                                                           HandlePR::VACANCY_PR+
                                                           HandleToWorkPos::ToWork+
                                                           handlePickerAction::PLACE_NG_LENS_TO_TRAY);
}

void LogicManager::pickArmMoveToPlaceLensToLut()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePosition::LUT_POS1+
                                                           HandlePR::RESET_PR+
                                                           HandleToWorkPos::ToWork+
                                                           handlePickerAction::PLACE_LENS_TO_LUT);
}

void LogicManager::pickArmLensTrayLensPR()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePR::LENS_PR+HandleToWorkPos::ToWork);
}

void LogicManager::pickArmLensTrayVacancyPR()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePR::VACANCY_PR+HandleToWorkPos::ToWork);
}

void LogicManager::pickArmLUTVancyPR()
{
    baseModuleManage->single_station_material_loader_module.performHandling(HandlePR::LUT_PR+HandleToWorkPos::ToWork);
}

void LogicManager::pickArmLensMeasureHeight()
{
 baseModuleManage->single_station_material_loader_module.performHandling( handlePickerAction::MeasureLensInTray);
}

void LogicManager::pickArmLUTMeasureHeight()
{
    baseModuleManage->single_station_material_loader_module.performHandling( handlePickerAction::MeasureLensInLUT);
}

void LogicManager::receiveCommand(int cmd)
{
    qInfo("receive command: %d", cmd);
    if (cmd == CommandType::MOTION_INIT) {
        init();
    }
}
