#include "logicmanager.h"
#include <QtConcurrent/QtConcurrent>
#include <QFuture>

enum CommandType{
    IDLE,
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
    : QThread (parent), currentMode(CommandType::IDLE)
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
    if (currentMode == CommandType::AA_MOVETO_MUSHROOM_CMD)
    {
        baseModuleManage->aa_head_module.moveToMushroomPosition();
    } else if (currentMode == CommandType::AA_MOVETO_PICK_LENS_CMD)
    {
        baseModuleManage->aa_head_module.moveToPickLensPosition();
    } else if (currentMode == CommandType::AA_MOVETO_OC_CMD)
    {
        baseModuleManage->aa_head_module.moveToOCPosition();
    } else if (currentMode == CommandType::SUT_MOVETO_PR_CMD)
    {
        baseModuleManage->sut_module.moveToDownlookPos();
    } else if (currentMode == CommandType::SUT_MOVETO_MUSHROOM_CMD)
    {
        baseModuleManage->sut_module.moveToMushroomPos();
    } else if (currentMode == CommandType::LUT_MOVETO_LOAD_CMD)
    {
        baseModuleManage->lut_module.moveToLoadPos();
    } else if (currentMode == CommandType::LUT_MOVETO_UNLOAD_CMD)
    {
        baseModuleManage->lut_module.moveToUnloadPos();
    } else if (currentMode == CommandType::LUT_MOVETO_LOAD_UPLOOK_CMD)
    {
        baseModuleManage->lut_module.moveToLoadUplookPos();
    } else if (currentMode == CommandType::LUT_MOVETO_AA1_UPLOOK_CMD)
    {
        baseModuleManage->lut_module.moveToAA1UplookPos();
    } else if (currentMode == CommandType::LUT_MOVETO_AA2_UPLOOK_CMD)
    {
        baseModuleManage->lut_module.moveToAA2UplookPos();
    }
    currentMode = CommandType::IDLE;
    qInfo("End");
}

void LogicManager::moveToCmd(int cmd) {
    if (currentMode == CommandType::IDLE)
    {
        currentMode = cmd;
        this->start();
    } else {
        qInfo("%s Fail ! Due to previous command is running", __FUNCTION__);
    }
}

void LogicManager::loadFlowchart(QString json)
{
    aaCore->setFlowchartDocument(json);
}

void LogicManager::aaMoveToMushroomPos(){moveToCmd(CommandType::AA_MOVETO_MUSHROOM_CMD);}
void LogicManager::aaMoveToPickLensPos(){moveToCmd(CommandType::AA_MOVETO_PICK_LENS_CMD);}
void LogicManager::aaMoveToOCPos(){moveToCmd(CommandType::AA_MOVETO_OC_CMD);}

void LogicManager::sutMoveToMushroomPos(){moveToCmd(CommandType::SUT_MOVETO_MUSHROOM_CMD);}
void LogicManager::sutMoveToPRPos(){moveToCmd(CommandType::SUT_MOVETO_PR_CMD);}

void LogicManager::lutMoveToLoadPos(){moveToCmd(CommandType::LUT_MOVETO_LOAD_CMD);}
void LogicManager::lutMoveToUnloadPos(){moveToCmd(CommandType::LUT_MOVETO_UNLOAD_CMD);}
void LogicManager::lutMoveToLoadUplookPos(){moveToCmd(CommandType::LUT_MOVETO_LOAD_UPLOOK_CMD);}
void LogicManager::lutMoveToAA1UplookPos(){moveToCmd(CommandType::LUT_MOVETO_AA1_UPLOOK_CMD);}
void LogicManager::lutMoveToAA2UplookPos(){moveToCmd(CommandType::LUT_MOVETO_AA2_UPLOOK_CMD);}
