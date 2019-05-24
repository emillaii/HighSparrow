#ifndef SINGLEHEADMACHINEMATERIALLOADERMODULE_H
#define SINGLEHEADMACHINEMATERIALLOADERMODULE_H

#include "singleheadmachinematerialloadermoduleparameter.h"
#include "singleheadmachinematerialpickarm.h"
#include "thread_worker_base.h"



class SingleHeadMachineMaterialLoaderModule:public ThreadWorkerBase
{
public:
    SingleHeadMachineMaterialLoaderModule(QString name = "SingleHeadMachineMaterialLoaderModule");
    void Init(SingleHeadMachineMaterialPickArm* pick_arm);
    void loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);
    SingleHeadMachineMaterialLoaderModuleParameter parameter;


public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
private:
    SingleHeadMachineMaterialPickArm* pick_arm;
};

#endif // SINGLEHEADMACHINEMATERIALLOADERMODULE_H
