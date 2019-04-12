#include "lut_module.h"

LutModule::LutModule()
{
    //connect(&this->parameters, &LutParameter::paramsChanged, this, &LutModule::updateParams);
    //connect(&this->carrier->parameters, &MaterialCarrierParameter::paramsChanged, this, &LutModule::updateParams);
}


void LutModule::Init(MaterialCarrier *carrier, VisionLocation* uplook_location,VisionLocation* updownlook_location,VisionLocation* load_location, XtVacuum *load_vacuum, XtVacuum *unload_vacuum)
{
    this->carrier = carrier;
    this->uplook_location = uplook_location;
    this->updownlook_location = updownlook_location;
    this->load_location = load_location;
    this->load_vacuum = load_vacuum;
    this->unload_vacuum = unload_vacuum;
    loadParams();
}

void LutModule::updateParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LUT_PARAMS", &parameters);
    temp_map.insert("LUT_CARRIER_PARAMS", &this->carrier->parameters);
    temp_map.insert("LOAD_POSITION", &load_position);
    temp_map.insert("UNLOAD_POSITION", &unload_position);
    temp_map.insert("LOAD_UPLOOK_POSITION", &load_uplook_position);
    temp_map.insert("AA1_UPDOWNLOOK_POSITION", &aa1_updownlook_position);
    temp_map.insert("AA1_PICKLENS_POSITION", &aa1_picklens_position);
    temp_map.insert("AA1_UNPICKLENS_POSITION", &aa1_unpicklens_position);
    temp_map.insert("AA1_UPLOOK_POSITION", &aa1_uplook_position);
    temp_map.insert("AA2_UPDOWNLOOK_POSITION", &aa2_updownlook_position);
    temp_map.insert("AA2_PICKLENS_POSITION", &aa2_picklens_position);
    temp_map.insert("AA2_UNPICKLENS_POSITION", &aa2_unpicklens_position);
    temp_map.insert("AA2_UPLOOK_POSITION", &aa2_uplook_position);
    PropertyBase::saveJsonConfig("config//lutConfig.json", temp_map);
}

void LutModule::loadParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LUT_PARAMS", &parameters);
    temp_map.insert("LUT_CARRIER_PARAMS", &this->carrier->parameters);
    temp_map.insert("LOAD_POSITION", &load_position);
    temp_map.insert("UNLOAD_POSITION", &unload_position);
    temp_map.insert("LOAD_UPLOOK_POSITION", &load_uplook_position);
    temp_map.insert("AA1_UPDOWNLOOK_POSITION", &aa1_updownlook_position);
    temp_map.insert("AA1_PICKLENS_POSITION", &aa1_picklens_position);
    temp_map.insert("AA1_UNPICKLENS_POSITION", &aa1_unpicklens_position);
    temp_map.insert("AA1_UPLOOK_POSITION", &aa1_uplook_position);
    temp_map.insert("AA2_UPDOWNLOOK_POSITION", &aa2_updownlook_position);
    temp_map.insert("AA2_PICKLENS_POSITION", &aa2_picklens_position);
    temp_map.insert("AA2_UNPICKLENS_POSITION", &aa2_unpicklens_position);
    temp_map.insert("AA2_UPLOOK_POSITION", &aa2_uplook_position);
    PropertyBase::loadJsonConfig("config//lutConfig.json", temp_map);
}

bool LutModule::moveToAA1UplookPos()
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_uplook_position.X(),aa1_uplook_position.Y(),aa1_uplook_position.Z());
}

bool LutModule::moveToAA1UplookPR(PrOffset &offset, bool close_lighting)
{

    uplook_location->OpenLight();
    bool result = moveToAA1UplookPos();
    if(result)
    {
      uplook_location->performPR(offset);
    }
    if(close_lighting)
        uplook_location->CloseLight();
    return false;
}

bool LutModule::moveToAA2UplookPos()
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_uplook_position.X(),aa2_uplook_position.Y(),aa2_uplook_position.Z());
}

bool LutModule::moveToAA2UplookPR(PrOffset &offset, bool close_lighting)
{
    uplook_location->OpenLight();
    bool result = moveToAA2UplookPos();
    if(result)
    {
      uplook_location->performPR(offset);
    }
    if(close_lighting)
        uplook_location->CloseLight();
    return false;
}

bool LutModule::moveToAA1UpdownlookPos()
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_updownlook_position.X(),aa1_updownlook_position.Y(),aa1_updownlook_position.Z());
}

bool LutModule::moveToAA1UpDwonlookPR(PrOffset &offset, bool close_lighting)
{
    updownlook_location->OpenLight();
    bool result = moveToAA1UpdownlookPos();
    if(result)
    {
      updownlook_location->performPR(offset);
    }
    if(close_lighting)
        updownlook_location->CloseLight();
    return false;
}

bool LutModule::moveToAA2UpdownlookPos()
{
     return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_updownlook_position.X(),aa2_updownlook_position.Y(),aa2_updownlook_position.Z());
}

bool LutModule::moveToAA2UpDwonlookPR(PrOffset &offset, bool close_lighting)
{
    updownlook_location->OpenLight();
    bool result = moveToAA2UpdownlookPos();
    if(result)
    {
      updownlook_location->performPR(offset);
    }
    if(close_lighting)
        updownlook_location->CloseLight();
    return false;
}

bool LutModule::moveToLoadPos()
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(load_position.X(),load_position.Y(),load_position.Z());
}

bool LutModule::moveToUnloadPos()
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(unload_position.X(),unload_position.Y(),unload_position.Z());
}

bool LutModule::moveToLoadUplookPos()
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(load_uplook_position.X(),load_uplook_position.Y(),load_uplook_position.Z());
}

bool LutModule::moveToAA1PickLens()
{
    bool result = grabber->Set(false);
    if(result)
        result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_picklens_position.X(),aa1_picklens_position.Y(),aa1_picklens_position.Z());
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

bool LutModule::moveToAA1UnPickLens()
{
    bool result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_unpicklens_position.X(),aa1_unpicklens_position.Y(),aa1_unpicklens_position.Z());
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

bool LutModule::moveToAA2PickLens()
{
    bool result = grabber->Set(false);
    if(result)
        result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_picklens_position.X(),aa2_picklens_position.Y(),aa2_picklens_position.Z());
    if(result)
    {
        double reuslt_pos;
        result = carrier->ZSerchByForce(reuslt_pos,parameters.PickForce(),-1,0,load_vacuum);
        if(result)
//   todo        grabber->Set(false);
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToAA2UnPickLens()
{
    bool result = grabber->Set(false);
    if(result)
        result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_unpicklens_position.X(),aa2_unpicklens_position.Y(),aa2_unpicklens_position.Z());
    if(result)
    {
        double reuslt_pos;
        result = carrier->ZSerchByForce(reuslt_pos,parameters.PickForce(),-1,0,load_vacuum);
        if(result)
//  todo          grabber->Set(false);
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::stepMove_XY_Sync(double x, double y)
{
    return carrier->StepMove_XY_Sync(x,y);
}
