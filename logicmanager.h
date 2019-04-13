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

    void performMotionThread();
    Q_INVOKABLE void aaMoveToMushroomPos();
    Q_INVOKABLE void aaMoveToPickLensPos();
    Q_INVOKABLE void aaMoveToOCPos();

    Q_INVOKABLE void sutMoveToMushroomPos();
    Q_INVOKABLE void sutMoveToPRPos();

    Q_INVOKABLE void lutMoveToLoadPos();
    Q_INVOKABLE void lutMoveToUnloadPos();
    Q_INVOKABLE void lutMoveToLoadUplookPos();
    Q_INVOKABLE void lutMoveToAA1UplookPos();
    Q_INVOKABLE void lutMoveToAA2UplookPos();

private:
    BaseModuleManager * baseModuleManage;
    SfrWorkerController * sfrWorkerController = Q_NULLPTR;
    void moveToCmd(int);
    int currentMode;
protected:
    void run();
};

#endif // LOGICMANAGER_H
