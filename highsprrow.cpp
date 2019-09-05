#include "highsprrow.h"
#include <QDir>
#include <QProcess>
#include <qmessagebox.h>

HighSprrow::HighSprrow()
{
    if (!QDir(CONFIG_DIR).exists()) {QDir().mkdir(CONFIG_DIR);}
    if (!QDir(PR_CONFIG_DIR).exists()) {QDir().mkdir(PR_CONFIG_DIR);}
    if (!QDir(SYSTERM_PARAM_DIR).exists()) {QDir().mkdir(SYSTERM_PARAM_DIR);}
    worker_manager = new WorkersManager(this);
    baseModuleManager = new BaseModuleManager();
    baseModuleManager->loadProfile();
    logicManager = new LogicManager(baseModuleManager);
    baseModuleManager->registerWorkers(worker_manager);
    connect(logicManager,&LogicManager::sendMsgSignal,worker_manager,&WorkersManager::sendMessageTest,Qt::BlockingQueuedConnection);
    connect(&baseModuleManager->aaCoreNew, &AACoreNew::callQmlRefeshImg, this, &HighSprrow::receiveImageFromAACore);
}

HighSprrow::~HighSprrow()
{
}

ErrorCodeStruct HighSprrow::InitComponent()
{
    ErrorCodeStruct ret = { OK, "" };
    return ret;
}

void HighSprrow::performLUTMushroomPR()
{
    PrOffset offset;
    if (baseModuleManager->vision_locations[PR_AA1_MUSHROOMHEAD]->performPR(offset))
    {
        baseModuleManager->lut_carrier.StepMove_XY_Sync(-offset.X, -offset.Y);
    } else {
        qInfo("Perform LUT Mushroom PR fail");
    }
}

void HighSprrow::performSUTDownlookPR()
{
    PrOffset offset;
    baseModuleManager->dispense_module.setMapPosition(baseModuleManager->sh_lsut_module.downlook_position.X(),
                                                      baseModuleManager->sh_lsut_module.downlook_position.Y());
    if (baseModuleManager->sh_lsut_module.moveToDownlookPR(offset, false,true))
    {
        qInfo("Perform SUT Downlook PR OffsetX %f OffsetY %f", offset.X, offset.Y);
        baseModuleManager->dispense_module.setPRPosition(offset.X, offset.Y,offset.Theta);
        baseModuleManager->sh_lsut_module.stepMove_XY_Sync(-offset.X, -offset.Y);
    } else {
        qInfo("Perform SUT Downlook PR Fail");
    }
}

void HighSprrow::performOnlySUTDownlookPR()
{
    PrOffset offset;
    baseModuleManager->dispense_module.setMapPosition(baseModuleManager->sh_lsut_module.downlook_position.X(),
                                                      baseModuleManager->sh_lsut_module.downlook_position.Y());
    if (baseModuleManager->vision_locations[PR_SUT_DOWNLOOK]->performPR(offset))
    {
        qInfo("Perform SUT Downlook PR OffsetX %f OffsetY %f", offset.X, offset.Y);
        baseModuleManager->dispense_module.setPRPosition(offset.X, offset.Y,offset.Theta);
        baseModuleManager->sh_lsut_module.stepMove_XY_Sync(-offset.X, -offset.Y);
    } else {
        qInfo("Perform SUT Downlook PR Fail");
    }
}

void HighSprrow::performLUTUplookPR()
{
    PrOffset offset;
    if(baseModuleManager->vision_locations[PR_AA1_LUT_UPLOOK]->performPR(offset))
    {
        baseModuleManager->aa_head_module.stepMove_XYC_Sync(-offset.X, -offset.Y,-offset.Theta);
    }
}

void HighSprrow::performLPALensPR()
{
    PrOffset offset;
    if(baseModuleManager->vision_locations[PR_LENS_LPALOOK]->performPR(offset))
    {
        baseModuleManager->single_station_material_pickarm.stepMove_XY_Synic(offset.X,offset.Y);//to do
    }
}

void HighSprrow::performLUTLensPR()
{
    PrOffset offset;
    if(baseModuleManager->vision_locations[PR_LENS_LUTLOOK]->performPR(offset))
    {
        baseModuleManager->single_station_material_pickarm.stepMove_XY_Synic(offset.X,offset.Y);//to do
    }
}

void HighSprrow::performVacancyLensPR()
{
    PrOffset offset;
    if(baseModuleManager->vision_locations[PR_VACANCY_LPALOOK]->performPR(offset))
    {
        baseModuleManager->single_station_material_pickarm.stepMove_XY_Synic(offset.X,offset.Y);//to do
    }
}

void HighSprrow::callQProcess(QString arg)
{
    QStringList arguments;
    arguments << "/c" << arg.toStdString().c_str();
    QProcess process;
    process.setWorkingDirectory(QDir::currentPath());
    qInfo("Learn PR %s,current directory:%s", arg.toStdString().c_str(),QDir::currentPath().toStdString().c_str());
    process.startDetached("cmd.exe", arguments);
    process.waitForStarted();
}

void HighSprrow::homeAllMotors()
{
    qInfo("Home All Motors");
}

void HighSprrow::aa_A_Inter(double step)
{
    baseModuleManager->aa_head_module.stepInterpolation_AB_Sync(step,0);
}

void HighSprrow::aa_B_Inter(double step)
{
    baseModuleManager->aa_head_module.stepInterpolation_AB_Sync(0,step);
}
