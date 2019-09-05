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
            baseModuleManage->sh_lsut_module.moveToPRPosition();
        }
    } else if (m_currentMode == CommandType::SUT_MOVETO_MUSHROOM_CMD)
    {
        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            baseModuleManage->sh_lsut_module.moveToMushroomPosition();
        }
    } else if (m_currentMode == CommandType::LUT_MOVETO_LOAD_CMD)
    {
        baseModuleManage->sh_lsut_module.moveToLoadSensorPosition();
    }
    else if (m_currentMode == CommandType::LUT_MOVETO_LOAD_UPLOOK_CMD)
    {
    } else if (m_currentMode == CommandType::LUT_MOVETO_AA_UPLOOK_CMD)
    {
        baseModuleManage->sh_lsut_module.moveToGripperPosition();
    }
    else if (m_currentMode == CommandType::PERFORM_CALIBRATION)
    {
        baseModuleManage->performCalibration(calibration_name);
        qInfo("calibration End");
    }
    else if (m_currentMode == CommandType::LUT_PICK_LENS_TO_AA_CMD)
    {
        baseModuleManage->sh_lsut_module.moveToPickLensPosition();
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
void LogicManager::lutMoveToLoadUplookPos(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::LUT_MOVETO_LOAD_UPLOOK_CMD);}
void LogicManager::lutMoveToAAUplookPos(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::LUT_MOVETO_AA_UPLOOK_CMD);}

void LogicManager::lutPickLensToAA(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::LUT_PICK_LENS_TO_AA_CMD);}

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
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::LENS_TRAY1);
}

void LogicManager::pickArmMoveToLensTray2Pos()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::LENS_TRAY2);
}

void LogicManager::pickArmMoveToLutPos()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::LUT_PR_POS);
}


void LogicManager::pickArmMoveToPickLensFromTray1()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::LENS_TRAY1+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePR::LENS_PR+
                                                                            SingleHeadMachineMaterialLoaderModule::HandleToWorkPos::PICKER1_TO_WORKPOS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePickerAction::PICKER1_PICK_LENS_FROM_TRAY);
}

void LogicManager::pickArmMoveToPickLensFromTray2()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::LENS_TRAY2+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePR::LENS_PR+
                                                                            SingleHeadMachineMaterialLoaderModule::HandleToWorkPos::PICKER1_TO_WORKPOS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePickerAction::PICKER1_PICK_LENS_FROM_TRAY);
}

void LogicManager::pickArmMoveToPickLensFromLut()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::LUT_PR_POS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePR::NG_LENS_PR+
                                                                            SingleHeadMachineMaterialLoaderModule::HandleToWorkPos::PICKER1_TO_WORKPOS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePickerAction::PICKER1_PICK_NG_LENS_FROM_LUT);
}

void LogicManager::pickArmMoveToPlaceLensToTray1()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::LENS_TRAY1+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePR::LENS_VACANCY_PR+
                                                                            SingleHeadMachineMaterialLoaderModule::HandleToWorkPos::PICKER1_TO_WORKPOS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePickerAction::PICKER1_PLACE_LENS_TO_TRAY);
}

void LogicManager::pickArmMoveToPlaceLensToTray2()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::LENS_TRAY2+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePR::LENS_VACANCY_PR+
                                                                            SingleHeadMachineMaterialLoaderModule::HandleToWorkPos::PICKER1_TO_WORKPOS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePickerAction::PICKER1_PLACE_LENS_TO_TRAY);
}

void LogicManager::pickArmMoveToPlaceLensToLut()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::LUT_PR_POS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePR::LUT_PR+
                                                                            SingleHeadMachineMaterialLoaderModule::HandleToWorkPos::PICKER1_TO_WORKPOS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePickerAction::PICKER1_PLACE_LENS_TO_LUT);
}

void LogicManager::pickArmMoveToSensorTrayPos()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::SENSOR_TRAY);
}

void LogicManager::pickArmMoveToRejectTrayPos()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::REJECT_TRAY);
}

void LogicManager::pickArmMoveToSutPos()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::SUT_PR_POS);
}

void LogicManager::pickArmMoveToPickSensorFromTray()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::SENSOR_TRAY+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePR::SENSOR_PR+
                                                                            SingleHeadMachineMaterialLoaderModule::HandleToWorkPos::PICKER2_TO_WORKPOS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePickerAction::PICKER2_PICK_SENSOR_FROM_TRAY);
}

void LogicManager::pickArmMoveToPickSensorFromSut()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::SUT_PR_POS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePR::NG_SENSOR_PR+
                                                                            SingleHeadMachineMaterialLoaderModule::HandleToWorkPos::PICKER2_TO_WORKPOS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePickerAction::PICKER2_PICK_NG_SENSOR_FROM_SUT);
}

void LogicManager::pickArmMoveToPlaceSensorToTray()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::REJECT_TRAY+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePR::SENSOR_VACANCY_PR+
                                                                            SingleHeadMachineMaterialLoaderModule::HandleToWorkPos::PICKER2_TO_WORKPOS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePickerAction::PICKER2_PLACE_NG_SENSOR_TO_TRAY);
}

void LogicManager::pickArmMoveToPlaceSensorToSut()
{
    baseModuleManage->single_station_material_loader_module.performHandling(SingleHeadMachineMaterialLoaderModule::HandlePosition::SUT_PR_POS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePR::SUT_PR+
                                                                            SingleHeadMachineMaterialLoaderModule::HandleToWorkPos::PICKER2_TO_WORKPOS+
                                                                            SingleHeadMachineMaterialLoaderModule::HandlePickerAction::PICKER2_PLACE_SENSOR_TO_SUT);
}
void LogicManager::receiveCommand(int cmd)
{
    qInfo("receive command: %d", cmd);
    if (cmd == CommandType::MOTION_INIT) {
        init();
    }
}
