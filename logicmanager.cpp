#include "logicmanager.h"
#include <QtConcurrent/QtConcurrent>
#include <QFuture>

enum CommandType{
    IDLE,
    MOTION_INIT,
    MOTION_HOME,
    MOTION_STOP_HOME,     // Interrupt
    AA_MOVETO_MUSHROOM_CMD,
    AA_MOVETO_PICK_LENS_CMD,
    AA_MOVETO_OC_CMD,
    SUT_MOVETO_MUSHROOM_CMD,
    SUT_MOVETO_PR_CMD,
    LUT_MOVETO_LOAD_CMD,
    LUT_MOVETO_UNLOAD_CMD,
    LUT_MOVETO_LOAD_UPLOOK_CMD,
    LUT_MOVETO_AA1_UPLOOK_CMD,
    LUT_MOVETO_AA2_UPLOOK_CMD
};

LogicManager::LogicManager(BaseModuleManager* device_manager,QObject *parent)
    : QThread (parent), m_currentMode(CommandType::IDLE)
{
    aaCore = new AACore(&device_manager->aa_head_module,&device_manager->lut_module,&device_manager->sut_module, device_manager->dothinkey);
    sfrWorkerController = new SfrWorkerController(aaCore);
    aaCore->setSfrWorkerController(sfrWorkerController);
    baseModuleManage = device_manager;
}

void LogicManager::run() {
    qInfo("Logic manager is running");
//    aaCore->start();
//    aaCore->wait();

    if (m_currentMode == CommandType::MOTION_STOP_HOME) {
        baseModuleManage->stopSeeking();
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
    m_currentMode = CommandType::IDLE;
    qInfo("End");
}

void LogicManager::moveToCmd(int cmd) {
    if (m_currentMode == CommandType::IDLE)
    {
        setCurrentMode(cmd);
        this->start();
    } else {
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
