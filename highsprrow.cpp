#include "highsprrow.h"
#include <QDir>
#include <QProcess>
#include <qmessagebox.h>

HighSprrow::HighSprrow()
{
    if (!QDir(CONFIG_DIR).exists()) {QDir().mkdir(CONFIG_DIR);}
    if (!QDir(PR_CONFIG_DIR).exists()) {QDir().mkdir(PR_CONFIG_DIR);}
    baseModuleManager = new BaseModuleManager(this);
    baseModuleManager->LoadProfile();
   logicManager = new LogicManager(baseModuleManager);
   WorkersManager threadWorkerManager(this);
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
