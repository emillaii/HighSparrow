#include "lut_module.h"

LutModule::LutModule()
{
    this->loadParams();
    //connect(&this->parameters, &LutParameter::paramsChanged, this, &LutModule::updateParams);
    //connect(&this->carrier->parameters, &MaterialCarrierParameter::paramsChanged, this, &LutModule::updateParams);
}


void LutModule::Init(MaterialCarrier *carrier, Calibration *updown_calibration, WordopLight *lighting, VisionModule *vision, XtVacuum *load_vacuum, XtVacuum *unload_vacuum)
{
    this->carrier = carrier;
    this->updown_calibration = updown_calibration;
    this->lighting =lighting;
    this->vision = vision;
    this->load_vacuum = load_vacuum;
    this->unload_vacuum = unload_vacuum;
}

void LutModule::updateParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LUT_PARAMS", &this->parameters);
    temp_map.insert("LUT_CARRIER_PARAMS", &this->carrier->parameters);
    temp_map.insert("UPDOWNLOOK_POSITION", &this->updownlook_position);
    PropertyBase::saveJsonConfig("config//lutConfig.json", temp_map);
}

void LutModule::loadParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LUT_PARAMS", &parameters);
    temp_map.insert("LUT_CARRIER_PARAMS", &this->carrier->parameters);
    temp_map.insert("UPDOWNLOOK_POSITION", &updownlook_position);
    PropertyBase::loadJsonConfig("config//lutConfig.json", temp_map);
}

bool LutModule::moveToUpdownlookPos()
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(updownlook_position.X(),updownlook_position.Y(),updownlook_position.Z());
}

bool LutModule::moveToUpDwonlookPR(PrOffset &offset, bool close_lighting)
{
    lighting->ChangeBrightnessSignal(LIGHTING_DOWNLOOK,parameters.UpDnLookLighting());
    bool result = moveToUpdownlookPos();
    if(result)
    {
      ErrorCodeStruct temp =  vision->PR_Generic_NCC_Template_Matching(DOWNLOOK_VISION_CAMERA, parameters.upDownLookPrName(), pr_result);
      if(ErrorCode::OK == temp.code)
      {
          QPointF mech;
          if(updown_calibration->getDeltaDistanceFromCenter(QPointF(pr_result.x,pr_result.y),mech))
          {
             offset.X = mech.x();
             offset.Y = mech.y();
             offset.Theta = pr_result.theta;
             return true;
          }
      }
    }
    if(close_lighting)
        lighting->ChangeBrightnessSignal(LIGHTING_DOWNLOOK,0);
    return false;
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
