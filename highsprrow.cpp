#include "highsprrow.h"
#include <QDir>

HighSprrow::HighSprrow()
{
    if (!QDir(CONFIG_DIR).exists()) {QDir().mkdir(CONFIG_DIR);}
    pylonUplookCamera = new BaslerPylonCamera(UPLOOK_VISION_CAMERA);
   // pylonDownlookCamera = new BaslerPylonCamera(DOWNLOOK_VISION_CAMERA);
    visionModule = new VisionModule();
    aaHeadModule = new AAHeadModule();
    aaCore = new AACore();
    sfrWorkerController = new SfrWorkerController(aaCore);
    aaCore->setSfrWorkerController(sfrWorkerController);
}

HighSprrow::~HighSprrow()
{

}

ErrorCodeStruct HighSprrow::InitComponent()
{
    ErrorCodeStruct ret = { OK, "" };
    qInfo("Init Component");
    //pylonDownlookCamera->start();
    //pylonUplookCamera->start();
    return ret;
}

void HighSprrow::init()
{
    InitComponent();
}

void HighSprrow::home()
{
    qInfo("Home...");
    pylonDownlookCamera->start();
    pylonUplookCamera->start();
    PRResultStruct prResult;
    visionModule->PR_Generic_NCC_Template_Matching(UPLOOK_VISION_CAMERA, prResult);
    qInfo("%f %f %f", prResult.x, prResult.y, prResult.theta);
}

void HighSprrow::homeAllMotors()
{
    qInfo("Home All Motors");
}

