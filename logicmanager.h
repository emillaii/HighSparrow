#ifndef LOGICMANAGER_H
#define LOGICMANAGER_H

#include "basemodulemanager.h"

#include <QThread>
#include <aacore.h>

class LogicManager : public QThread
{
    Q_OBJECT
public:
    explicit LogicManager(BaseModuleManager* device_manager, QObject *parent = nullptr);
    Q_INVOKABLE void loadFlowchart(QString);
    AACore * aaCore = Q_NULLPTR;


private:
    SfrWorkerController * sfrWorkerController = Q_NULLPTR;

protected:
    void run();
};

#endif // LOGICMANAGER_H
