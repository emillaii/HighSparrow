#include "singleheadmachinematerialloadermodule.h"

SingleHeadMachineMaterialLoaderModule::SingleHeadMachineMaterialLoaderModule(QString name)
:ThreadWorkerBase (name)
{

}

void SingleHeadMachineMaterialLoaderModule::Init(SingleHeadMachineMaterialPickArm *pick_arm)
{
    this->pick_arm = pick_arm;
}

void SingleHeadMachineMaterialLoaderModule::loadJsonConfig(QString file_name)
{

}

void SingleHeadMachineMaterialLoaderModule::saveJsonConfig(QString file_name)
{

}

void SingleHeadMachineMaterialLoaderModule::startWork(int run_mode)
{

}

void SingleHeadMachineMaterialLoaderModule::stopWork(bool wait_finish)
{

}

void SingleHeadMachineMaterialLoaderModule::resetLogic()
{

}

void SingleHeadMachineMaterialLoaderModule::performHandlingOperation(int cmd)
{

}
