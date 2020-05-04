#include "highsprrow.h"
#include <QDir>
#include <QProcess>
#include <qmessagebox.h>

HighSprrow::HighSprrow()
{
    if (!QDir(CONFIG_DIR).exists()) {QDir().mkdir(CONFIG_DIR);}
    if (!QDir(PR_CONFIG_DIR).exists()) {QDir().mkdir(PR_CONFIG_DIR);}
    if (!QDir(Y_LEVEL_PATH_DIR).exists()) {QDir().mkdir(Y_LEVEL_PATH_DIR);}
    if (!QDir(SYSTERM_PARAM_DIR).exists()) {QDir().mkdir(SYSTERM_PARAM_DIR);}
    worker_manager = new WorkersManager(this);
    baseModuleManager = new BaseModuleManager();
    baseModuleManager->loadProfile();
    worker_manager->parameters.loadJsonConfig(baseModuleManager->getSystermParameterDir().append("WorkManager.json"),"WorkManager");
    worker_manager->parameters.saveJsonConfig(baseModuleManager->getSystermParameterDir().append("WorkManager.json"),"WorkManager");
    logicManager = new LogicManager();
    logicManager->parameters.loadJsonConfig(baseModuleManager->getSystermParameterDir().append("LogicManager.json"),"logicManager");
    logicManager->parameters.saveJsonConfig(baseModuleManager->getSystermParameterDir().append("LogicManager.json"),"logicManager");
    logicManager->init(baseModuleManager);
    baseModuleManager->registerWorkers(worker_manager);
    worker_manager->Init(baseModuleManager->GetTcpMessagersByName(worker_manager->parameters.cmsMessagerNames()),
                         baseModuleManager->GetTcpMessagersByName(worker_manager->parameters.respMessagerNames()));
    connect(logicManager,&LogicManager::sendMsgSignal,worker_manager,&WorkersManager::sendMessageTest,Qt::BlockingQueuedConnection);
    connect(&baseModuleManager->aaCoreNew, &AACoreNew::callQmlRefeshImg, this, &HighSprrow::receiveImageFromAACore);
    connect(this->baseModuleManager->visionModule, &VisionModule::callQmlRefeshImg, this, &HighSprrow::receiveImageFromAACore);
    connect(&baseModuleManager->dispense_module, &DispenseModule::callQmlRefeshImg, this, &HighSprrow::receiveImageFromAACore);
    connect(worker_manager,&WorkersManager::sendMessageToLogicManager,logicManager,&LogicManager::receiveMessageFromWorkerManger,Qt::DirectConnection);
    connect(logicManager,&LogicManager::sendMessageToWorkerManger,worker_manager,&WorkersManager::receiveModuleMessage,Qt::DirectConnection);

    connect(worker_manager,&WorkersManager::sendMessageToDevicesManager,baseModuleManager,&BaseModuleManager::receiveMessageFromWorkerManger);
    connect(baseModuleManager,&BaseModuleManager::sendMessageToWorkerManger,worker_manager,&WorkersManager::receiveModuleMessage,Qt::DirectConnection);

    logicManager->sendLogicManangerName();
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
    baseModuleManager->dispense_module.setMapPosition(baseModuleManager->sut_module.downlook_position.X(),
                                                      baseModuleManager->sut_module.downlook_position.Y());
    if (baseModuleManager->sut_module.moveToDownlookPR(offset, false,true))
    {
        qInfo("Perform SUT Downlook PR OffsetX %f OffsetY %f", offset.X, offset.Y);
        baseModuleManager->dispense_module.setPRPosition(offset.X, offset.Y,offset.Theta);
        baseModuleManager->sut_module.stepMove_XY_Sync(-offset.X, -offset.Y);
    } else {
        qInfo("Perform SUT Downlook PR Fail");
    }
}

void HighSprrow::performOnlySUTDownlookPR()
{
    PrOffset offset;
    baseModuleManager->dispense_module.setMapPosition(baseModuleManager->sut_module.downlook_position.X(),
                                                      baseModuleManager->sut_module.downlook_position.Y());
    if (baseModuleManager->vision_locations[PR_SUT_DOWNLOOK]->performPR(offset))
    {
        qInfo("Perform SUT Downlook PR OffsetX %f OffsetY %f", offset.X, offset.Y);
        baseModuleManager->dispense_module.setPRPosition(offset.X, offset.Y,offset.Theta);
        baseModuleManager->sut_module.stepMove_XY_Sync(-offset.X, -offset.Y);
    } else {
        qInfo("Perform SUT Downlook PR Fail");
    }
}

void HighSprrow::performLUTUplookPR()
{
    PrOffset offset;
    if (baseModuleManager->lut_module.moveToAA1UplookPR(offset, false, true))
    {
        baseModuleManager->aa_head_module.stepMove_XYC_Sync(-offset.X, -offset.Y,-offset.Theta);
    }
}

void HighSprrow::performLPALensPR()
{
    PrOffset offset;
    if(baseModuleManager->vision_locations[PR_LENS_LPALOOK]->performPR(offset))
    {
        baseModuleManager->lens_pick_arm.stepMove_XYTp_Synic(offset,true);
    }
}

void HighSprrow::performLUTLensPR()
{
    PrOffset offset;
    if(baseModuleManager->vision_locations[PR_LENS_LUTLOOK]->performPR(offset))
    {
        baseModuleManager->lens_pick_arm.stepMove_XYTp_Synic(offset,true);
    }
}

void HighSprrow::performVacancyLensPR()
{
    PrOffset offset;
    if(baseModuleManager->vision_locations[PR_VACANCY_LPALOOK]->performPR(offset))
    {
        baseModuleManager->lens_pick_arm.stepMove_XYTp_Synic(offset,true);
    }
}

void HighSprrow::callQProcess(QString arg)
{
    QStringList arguments;
    arguments << "/c" << arg.toStdString().c_str();
    QProcess process;
    process.setWorkingDirectory(QDir::currentPath());
    qInfo("Start process %s,current directory:%s", arg.toStdString().c_str(),QDir::currentPath().toStdString().c_str());
    process.startDetached("cmd.exe", arguments);
    process.waitForStarted();
}

void HighSprrow::callAVSPRSettingProcess(QString exe_name, QString pr_name)
{
    QString filename = ".//config//prConfig//prconfig.txt";
    QFile file;
    file.setFileName(filename);
    if(!file.open(QFile::WriteOnly)){
        qWarning("save parameters to %s failed, Couldn't open save file.",filename.toStdString().data());
        return;
    }
    file.write(pr_name.toStdString().c_str());
    file.close();
    callQProcess(exe_name);
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

void HighSprrow::tcp_aa_A_Inter(double step)
{
    baseModuleManager->sendAAHeadInterMoveRequest(0, step);
}

void HighSprrow::tcp_aa_B_Inter(double step)
{
    baseModuleManager->sendAAHeadInterMoveRequest(1, step);
}
