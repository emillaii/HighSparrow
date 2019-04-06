#include "logicmanager.h"

LogicManager::LogicManager(QObject *parent)
    : QThread (parent)
{
    aaCore = new AACore();
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
