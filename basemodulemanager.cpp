#include "basemodulemanager.h"

BaseModuleManager::BaseModuleManager(QObject *parent)
    : PropertyBase (parent),
      m_downlookLighting(0), m_uplookLighting(0)
{
    //pylonUplookCamera = new BaslerPylonCamera(UPLOOK_VISION_CAMERA);
    //pylonDownlookCamera = new BaslerPylonCamera(DOWNLOOK_VISION_CAMERA);
    lightingModule = new WordopLight();
    visionModule = new VisionModule();
}

BaseModuleManager::~BaseModuleManager()
{}

void BaseModuleManager::testVision()
{
    PRResultStruct prResult;
    visionModule->PR_Generic_NCC_Template_Matching(DOWNLOOK_VISION_CAMERA, prResult);
    qInfo("%f %f %f", prResult.x, prResult.y, prResult.theta);
}
