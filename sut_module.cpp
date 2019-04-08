#include "sut_module.h"

SutModule::SutModule(MaterialCarrier* carrier , BaslerPylonCamera *camera, WordopLight *lighting, VisionModule *vision)
{
    this->carrier = carrier;
    this->camera = camera;
    this->lighting = lighting;
    this->vision = vision;
    this->loadParams();
    connect(&this->parameters, &SutParameter::paramsChanged, this, &SutModule::updateParams);
    connect(&this->carrier->parameters, &MaterialCarrierParameter::paramsChanged, this, &SutModule::updateParams);
}

void SutModule::updateParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("SUT_PARAMS", &this->parameters);
    temp_map.insert("SUT_CARRIER_PARAMS", &this->carrier->parameters);
    PropertyBase::saveJsonConfig("config//sutConfig.json", temp_map);
}

void SutModule::loadParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("SUT_PARAMS", &parameters);
    temp_map.insert("SUT_CARRIER_PARAMS", &this->carrier->parameters);
    PropertyBase::loadJsonConfig("config//sutConfig.json", temp_map);
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
    return carrier->Move_SZ_SX_Y_X_Z_Sync(parameters.LoadX(),parameters.LoadY(),parameters.LoadZ());
}

bool SutModule::moveToOCPos()
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(parameters.OCX(),parameters.OCY(),parameters.OCZ());
}

bool SutModule::stepMove_XY_Sync(double x, double y)
{
    return carrier->StepMove_XY_Sync(x,y);
}
