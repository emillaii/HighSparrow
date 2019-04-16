#include "sut_module.h"

SutModule::SutModule()
{
    //connect(&this->parameters, &SutParameter::paramsChanged, this, &SutModule::updateParams);
    //connect(&this->carrier->parameters, &MaterialCarrierParameter::paramsChanged, this, &SutModule::updateParams);
}

void SutModule::Init(MaterialCarrier *carrier, VisionLocation* downlook_location,VisionLocation* updownlook_location, XtVacuum *vacuum)
{
    this->carrier = carrier;
    this->vision_downlook_location = downlook_location;
    this->vision_updownlook_location = updownlook_location;
    this->vacuum = vacuum;
    loadParams();
}

void SutModule::updateParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("SUT_PARAMS", &this->parameters);
    temp_map.insert("SUT_CARRIER_PARAMS", &this->carrier->parameters);
    temp_map.insert("LOAD_POSITION", &this->load_position);
    temp_map.insert("DOWNLOOK_POSITION", &this->downlook_position);
    temp_map.insert("TOOLUPLOOK_POSITION", &this->tool_uplook_positon);
    temp_map.insert("TOOLDOWNLOOK_POSITION", &this->tool_downlook_position);
    temp_map.insert("MUSHROOM_POSITION", &this->mushroom_positon);
    temp_map.insert("VISION_DOWNLOOK_LOCATION", &this->vision_downlook_location->parameters);
    temp_map.insert("VISION_UPDOWNLOOK_LOCATION", &this->vision_updownlook_location->parameters);
    PropertyBase::saveJsonConfig("config//sutConfig.json", temp_map);
}

void SutModule::loadParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("SUT_PARAMS", &parameters);
    temp_map.insert("SUT_CARRIER_PARAMS", &this->carrier->parameters);
    temp_map.insert("LOAD_POSITION", &this->load_position);
    temp_map.insert("DOWNLOOK_POSITION", &this->downlook_position);
    temp_map.insert("TOOLUPLOOK_POSITION", &this->tool_uplook_positon);
    temp_map.insert("TOOLDOWNLOOK_POSITION", &this->tool_downlook_position);
    temp_map.insert("MUSHROOM_POSITION", &this->mushroom_positon);
    temp_map.insert("VISION_DOWNLOOK_LOCATION", &this->vision_downlook_location->parameters);
    temp_map.insert("VISION_UPDOWNLOOK_LOCATION", &this->vision_updownlook_location->parameters);
    PropertyBase::loadJsonConfig("config//sutConfig.json", temp_map);
}

bool SutModule::moveToDownlookPR(PrOffset &offset,bool close_lighting,bool check_autochthonous)
{
    vision_downlook_location->OpenLight();
    bool result = moveToDownlookPos(check_autochthonous);
    if(result)
    {
        result = vision_downlook_location->performPR(offset);
    }
    if(close_lighting)
        vision_downlook_location->CloseLight();
    return result;
}

bool SutModule::moveToLoadPos(bool check_autochthonous)
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(load_position.X(),load_position.Y(),load_position.Z(),check_autochthonous);
}

bool SutModule::moveToDownlookPos(bool check_autochthonous)
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(downlook_position.X(),downlook_position.Y(),downlook_position.Z(),check_autochthonous);
}

bool SutModule::moveToUpDwonlookPR(PrOffset &offset,bool close_lighting,bool check_autochthonous)
{
    vision_updownlook_location->OpenLight();
    bool result = moveToToolDownlookPos(check_autochthonous);
    if(result)
    {
        vision_updownlook_location->performPR(offset);
    }
    if(close_lighting)
        vision_updownlook_location->CloseLight();
    return false;
}

bool SutModule::moveToToolDownlookPos(bool check_autochthonous)
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(tool_downlook_position.X(),tool_downlook_position.Y(),tool_downlook_position.Z(),check_autochthonous);
}

bool SutModule::moveToToolUplookPos(bool check_autochthonous)
{
     return carrier->Move_SZ_SX_Y_X_Z_Sync(tool_uplook_positon.X(),tool_uplook_positon.Y(),tool_uplook_positon.Z(),check_autochthonous);
}

bool SutModule::moveToMushroomPos(bool check_autochthonous)
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(mushroom_positon.X(),mushroom_positon.Y(),mushroom_positon.Z(),check_autochthonous);
}

bool SutModule::stepMove_XY_Sync(double x, double y)
{
    return carrier->StepMove_XY_Sync(x,y);
}

bool SutModule::stepMove_Z_Sync(double step_z)
{
    return carrier->StepMove_Z(step_z);
}

bool SutModule::moveToZPos(double z)
{
    return carrier->Move_Z_Sync(z);
}

void SutModule::recordCurrentPos()
{
    record_position = carrier->GetFeedBackPos();
}

bool SutModule::movetoRecordPos(bool check_autochthonous)
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(record_position.X,record_position.Y,record_position.Z,check_autochthonous);
}


