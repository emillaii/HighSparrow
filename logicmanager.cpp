#include "logicmanager.h"

LogicManager::LogicManager(BaseModuleManager* device_manager,QObject *parent)
    : QThread (parent)
{
    aaCore = new AACore(&device_manager->aa_head_module,&device_manager->lut_module,&device_manager->sut_module);
    sfrWorkerController = new SfrWorkerController(aaCore);
    aaCore->setSfrWorkerController(sfrWorkerController);
}

void LogicManager::run() {
    qInfo("Logic manager is running");
    aaCore->start();
    aaCore->wait();
    qInfo("End");
}

void LogicManager::loadFlowchart(QString json)
{
    aaCore->setFlowchartDocument(json);
}
