#include "lut_module.h"

LutModule::LutModule(MaterialCarrier* carrier, BaslerPylonCamera* camera,WordopLight* lighting,VisionModule* vision,XtVacuum* load_vacuum,XtVacuum* unload_vacuun)
{
    this->carrier = carrier;
    this->camera = camera;
    this->lighting =lighting;
    this->vision = vision;
    this->load_vacuum = load_vacuum;
    this->unload_vacuum = unload_vacuun;
    this->loadParams();
    connect(&this->parameters, &LutParameter::paramsChanged, this, &LutModule::updateParams);
    connect(&this->carrier->parameters, &MaterialCarrierParameter::paramsChanged, this, &LutModule::updateParams);
}


void LutModule::updateParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LUT_PARAMS", &this->parameters);
    temp_map.insert("LUT_CARRIER_PARAMS", &this->carrier->parameters);
    PropertyBase::saveJsonConfig("config//lutConfig.json", temp_map);
}

void LutModule::loadParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LUT_PARAMS", &parameters);
    temp_map.insert("LUT_CARRIER_PARAMS", &this->carrier->parameters);
    PropertyBase::loadJsonConfig("config//lutConfig.json", temp_map);
}

bool LutModule::moveToPR()
{
    lighting->ChangeBrightnessSignal(LIGHTING_UPLOOK,parameters.Lighting());
    bool result = carrier->Move_Vision_Sync();
    if(result)
        vision->PR_Generic_NCC_Template_Matching(DOWNLOOK_VISION_CAMERA, "",pr_result);
    return result;
}

bool LutModule::moveToLoadPos()
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(parameters.LoadX(),parameters.LoadY(),parameters.LoadZ());
}

bool LutModule::moveToPick1Lens()
{
    bool result = grabber->Set(false);
    if(result)
        result = carrier->Move_SZ_SY_X_Y_Z_Sync(parameters.Pick1X(),parameters.Pick1Y(),parameters.Pick1Z());
    if(result)
    {
        //todo one fuction
        double reuslt_pos;
        result = carrier->ZSerchByForce(reuslt_pos,parameters.PickForce(),-1,0,load_vacuum);
        if(result)
            grabber->Set(false);
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToUnPick1Lens()
{
    bool result = carrier->Move_SZ_SY_X_Y_Z_Sync(parameters.UnPick1X(),parameters.UnPick1Y(),parameters.UnPick1Z());
    if(result)
    {
        //todo one fuction
        double reuslt_pos;
        result = carrier->ZSerchByForce(reuslt_pos,parameters.PickForce(),-1,1,unload_vacuum);
        if(result)
            grabber->Set(true);
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToPick2Lens()
{
    bool result = grabber->Set(false);
    if(result)
        result = carrier->Move_SZ_SY_X_Y_Z_Sync(parameters.Pick2X(),parameters.Pick2Y(),parameters.Pick2Z());
    if(result)
    {
        double reuslt_pos;
        result = carrier->ZSerchByForce(reuslt_pos,parameters.PickForce(),-1,0,load_vacuum);
        if(result)
            grabber->Set(false);
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToUnPick2Lens()
{
    bool result = carrier->Move_SZ_SY_X_Y_Z_Sync(parameters.UnPick2X(),parameters.UnPick2Y(),parameters.UnPick2Z());
    if(result)
    {
        double reuslt_pos;
        result = carrier->ZSerchByForce(reuslt_pos,parameters.PickForce(),-1,1,unload_vacuum);
        if(result)
            grabber->Set(true);
        result &= carrier->ZSerchReturn();
    }
    return result;
}
