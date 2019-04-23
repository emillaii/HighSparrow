#include "logicmanager.h"
#include <QtConcurrent/QtConcurrent>
#include <QFuture>

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
    CALIBRATION_CHART,
    CALIBRATION_MUSHROOM,
    CALIBRATION_UPLOOK,
    CALIBRATION_DOWNLOOK,
    PERFORM_OC,
    PERFORM_LOOP_TEST
};

LogicManager::LogicManager(BaseModuleManager* device_manager,QObject *parent)
    : QThread (parent), m_currentMode(CommandType::IDLE)
{
    aaCore = new AACore(&device_manager->aa_head_module,&device_manager->lut_module,&device_manager->sut_module, device_manager->dothinkey, device_manager->chartCalibration,&device_manager->dispense_module);
    sfrWorkerController = new SfrWorkerController(aaCore);
    aaCore->setSfrWorkerController(sfrWorkerController);
    baseModuleManage = device_manager;
    //Connections
    connect(aaCore, &AACore::pushDataToUnit, &unitlog, &Unitlog::pushDataToUnit);
    connect(aaCore, &AACore::postDataToELK, &unitlog, &Unitlog::postDataToELK);
}

void LogicManager::updateParams()
{
    aaCore->updateParams();
}

void LogicManager::run() {
    qInfo("Logic manager is running");
    QString uuid = unitlog.createUnit();
    if (m_currentMode == CommandType::MOTION_STOP_HOME) {
        baseModuleManage->stopSeeking();
    }
    else if (m_currentMode == CommandType::MODE_AUTO_RUN) {
        aaCore->performLoopTest(AA_DIGNOSTICS_MODE::AA_AUTO_MODE, uuid);
        aaCore->wait();
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
    else if (m_currentMode == CommandType::CALIBRATION_CHART)
    {
        baseModuleManage->performChartCalibration();
    }
    m_currentMode = CommandType::IDLE;
    qInfo("End");
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

void LogicManager::performChartCalibration(){setStateMessage(__FUNCTION__);moveToCmd(CommandType::CALIBRATION_CHART);}


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
    baseModuleManage->lens_pick_arm_module.performHandling(HandlePosition::LENS_TRAY1,m_currentMode);
}

void LogicManager::lensPickArmMoveToTray2Pos()
{
    baseModuleManage->lens_pick_arm_module.performHandling(HandlePosition::LENS_TRAY2,m_currentMode);
}

void LogicManager::lensPickArmMoveToTray1StartPos()
{
    baseModuleManage->lens_pick_arm_module.performHandling(HandlePosition::LENS_TRAY1_START_POS,m_currentMode);
}

void LogicManager::lensPickArmMoveToTray2StartPos()
{
    baseModuleManage->lens_pick_arm_module.performHandling(HandlePosition::LENS_TRAY2_START_POS,m_currentMode);
}

void LogicManager::lensPickArmMoveToTray1EndPos()
{
    baseModuleManage->lens_pick_arm_module.performHandling(HandlePosition::LENS_TRAY2_START_POS,m_currentMode);
}

void LogicManager::lensPickArmMoveToLutPos1()
{

}

void LogicManager::lensPickArmMoveToLutPos2()
{

}

void LogicManager::lensPickArmMoveToPickLensFromTray1()
{

}

void LogicManager::lensPickArmMoveToPickLensFromTray2()
{

}

void LogicManager::lensPickArmMoveToPickLensFromLut()
{

}

void LogicManager::lensPickArmMoveToPlaceLensToTray1()
{

}

void LogicManager::lensPickArmMoveToPlaceLensToTray2()
{

}

void LogicManager::lensPickArmMoveToPlaceLensToLut()
{

}
