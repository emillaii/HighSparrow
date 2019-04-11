#include "sut_module.h"

SutModule::SutModule()
{
    //connect(&this->parameters, &SutParameter::paramsChanged, this, &SutModule::updateParams);
    //connect(&this->carrier->parameters, &MaterialCarrierParameter::paramsChanged, this, &SutModule::updateParams);
}

void SutModule::Init(MaterialCarrier *carrier, Calibration *down_calibration, Calibration *updown_calibration, WordopLight *lighting, VisionModule *vision)
{
    this->carrier = carrier;
    this->down_calibration = down_calibration;
    this->updown_calibration = updown_calibration;
    this->lighting = lighting;
    this->vision = vision;
}

void SutModule::updateParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("SUT_PARAMS", &this->parameters);
    temp_map.insert("SUT_CARRIER_PARAMS", &this->carrier->parameters);
    temp_map.insert("LOAD_POSITION", &this->load_position);
    temp_map.insert("DOWNLOOK_POSITION", &this->downlook_position);
    temp_map.insert("TOOLLOOK_POSITION", &this->toollook_position);
    temp_map.insert("MUSHROOM_POSITION", &this->mushroom_positon);
    PropertyBase::saveJsonConfig("config//sutConfig.json", temp_map);
}

void SutModule::loadParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("SUT_PARAMS", &parameters);
    temp_map.insert("SUT_CARRIER_PARAMS", &this->carrier->parameters);
    temp_map.insert("LOAD_POSITION", &this->load_position);
    temp_map.insert("DOWNLOOK_POSITION", &this->downlook_position);
    temp_map.insert("TOOLLOOK_POSITION", &this->toollook_position);
    temp_map.insert("MUSHROOM_POSITION", &this->mushroom_positon);
    PropertyBase::loadJsonConfig("config//sutConfig.json", temp_map);
}

bool SutModule::moveToDownlookPR(PrOffset &offset,bool close_lighting)
{
    lighting->ChangeBrightnessSignal(LIGHTING_DOWNLOOK,parameters.Lighting());
    bool result = moveToDownlookPos();
    if(result)
    {
      ErrorCodeStruct temp =  vision->PR_Generic_NCC_Template_Matching(DOWNLOOK_VISION_CAMERA, parameters.prName(), pr_result);
      if(ErrorCode::OK == temp.code)
      {
          QPointF mech;
          if(down_calibration->getDeltaDistanceFromCenter(QPointF(pr_result.x,pr_result.y),mech))
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

bool SutModule::moveToLoadPos()
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(load_position.X(),load_position.Y(),load_position.Z());
}

bool SutModule::moveToDownlookPos()
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(downlook_position.X(),downlook_position.Y(),downlook_position.Z());
}

bool SutModule::moveToUpDwonlookPR(PrOffset &offset,bool close_lighting)
{
    lighting->ChangeBrightnessSignal(LIGHTING_DOWNLOOK,parameters.UpDnLookLighting());
    bool result = moveToDownlookPos();
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

bool SutModule::moveToToollookPos()
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(toollook_position.X(),toollook_position.Y(),toollook_position.Z());
}

bool SutModule::moveToMushroomPos()
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(mushroom_positon.X(),mushroom_positon.Y(),mushroom_positon.Z());
}

bool SutModule::stepMove_XY_Sync(double x, double y)
{
    return carrier->StepMove_XY_Sync(x,y);
}
