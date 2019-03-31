#include "highsprrow.h"
#include <visionmodule.h>

HighSprrow::HighSprrow()
{
    pylonUplookCamera = new BaslerPylonCamera(UPLOOK_VISION_CAMERA);
    pylonDownlookCamera = new BaslerPylonCamera(DOWNLOOK_VISION_CAMERA);
    visionModule = new VisionModule();
}

HighSprrow::~HighSprrow()
{
    pylonDownlookCamera->Close();
    pylonUplookCamera->Close();
}

ErrorCodeStruct HighSprrow::InitComponent()
{
    ErrorCodeStruct ret = { OK, "" };
    qInfo("Init Component");
    pylonDownlookCamera->start();
    pylonUplookCamera->start();
    return ret;
}

void HighSprrow::init()
{
    InitComponent();
}

void HighSprrow::home()
{
    qInfo("Home...");
    PRResultStruct prResult;
    visionModule->PR_Generic_NCC_Template_Matching(UPLOOK_VISION_CAMERA, prResult);
    qInfo("%f %f %f", prResult.x, prResult.y, prResult.theta);
}

void HighSprrow::homeAllMotors()
{
    qInfo("Home All Motors");
}

