#include "sut_module.h"

SutModule::SutModule(MaterialCarrier* carrier , BaslerPylonCamera *camera, WordopLight *lighting, VisionModule *vision)
{
    this->carrier = carrier;
    this->camera = camera;
    this->lighting = lighting;
    this->vision = vision;
    this->loadParams();
    connect(&this->parameters, &SutParameter::paramsChanged, this, &SutModule::updateParams);
}

void SutModule::updateParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("SUT_PARAMS", &this->parameters);
    PropertyBase::saveJsonConfig(SUT_MODULE_JSON, temp_map);
}

void SutModule::loadParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("SUT_PARAMS", &parameters);
    PropertyBase::loadJsonConfig(SUT_MODULE_JSON, temp_map);
}

bool SutModule::moveToPR()
{
    lighting->ChangeBrightnessSignal(LIGHTING_DOWNLOOK,parameters.Lighting());
    bool result = carrier->Move_Vision_Sync();
    if(result)
    {
        vision->PR_Generic_NCC_Template_Matching(DOWNLOOK_VISION_CAMERA, "", pr_result);
    }
    return result;
}

bool SutModule::moveToLoadPos()
{
    return carrier->Move_SZ_SX_XY_Z_Sync(parameters.LoadX(),parameters.LoadY(),parameters.LoadZ());
}

bool SutModule::moveToOCPos()
{
    return carrier->Move_SZ_SX_XY_Z_Sync(parameters.OCX(),parameters.OCY(),parameters.OCZ());
}

bool SutModule::stepMove_XY_Sync(double x, double y)
{
    return carrier->StepMove_XY_Sync(x,y);
}
