#include "sut_module.h"

SutModule::SutModule()
{
    setName("SutModule");
}

void SutModule::Init(MaterialCarrier *carrier,SutClient* sut_cilent, VisionLocation* downlook_location,VisionLocation* updownlook_down_location,VisionLocation* updownlook_up_locationn, XtVacuum *vacuum,XtCylinder* popgpin)
{
    this->carrier = carrier;
    this->sut_cilent = sut_cilent;
    this->vision_downlook_location = downlook_location;
    this->vision_updownlook_down_location = updownlook_down_location;
    this->vision_updownlook_up_location = updownlook_up_locationn;
    this->vacuum = vacuum;
    this->popgpin = popgpin;
//    loadParams();
}

void SutModule::saveJsonConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("SUT_PARAMS", &this->parameters);
//    temp_map.insert("SUT_CARRIER_PARAMS", &this->carrier->parameters);
    temp_map.insert("LOAD_POSITION", &this->load_position);
    temp_map.insert("DOWNLOOK_POSITION", &this->downlook_position);
    temp_map.insert("TOOLUPLOOK_POSITION", &this->tool_uplook_positon);
    temp_map.insert("TOOLDOWNLOOK_POSITION", &this->tool_downlook_position);
    temp_map.insert("MUSHROOM_POSITION", &this->mushroom_positon);
//    temp_map.insert("VISION_DOWNLOOK_LOCATION", &this->vision_downlook_location->parameters);
//    temp_map.insert("VISION_UPDOWNLOOK_LOCATION", &this->vision_updownlook_location->parameters);
    PropertyBase::saveJsonConfig("config//sutConfig.json", temp_map);
}

void SutModule::loadParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("SUT_PARAMS", &parameters);
//    temp_map.insert("SUT_CARRIER_PARAMS", &this->carrier->parameters);
    temp_map.insert("LOAD_POSITION", &this->load_position);
    temp_map.insert("DOWNLOOK_POSITION", &this->downlook_position);
    temp_map.insert("TOOLUPLOOK_POSITION", &this->tool_uplook_positon);
    temp_map.insert("TOOLDOWNLOOK_POSITION", &this->tool_downlook_position);
    temp_map.insert("MUSHROOM_POSITION", &this->mushroom_positon);
//    temp_map.insert("VISION_DOWNLOOK_LOCATION", &this->vision_downlook_location->parameters);
//    temp_map.insert("VISION_UPDOWNLOOK_LOCATION", &this->vision_updownlook_location->parameters);
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
    qInfo("moveToLoadPos");
    return carrier->Move_SZ_SX_Y_X_Z_Sync(load_position.X(),load_position.Y(),load_position.Z(),check_autochthonous);
}

bool SutModule::moveToDownlookPos(bool check_autochthonous)
{
    qInfo("moveToDownlookPos");
    return carrier->Move_SZ_SX_Y_X_Z_Sync(downlook_position.X(),downlook_position.Y(),downlook_position.Z(),check_autochthonous);
}

bool SutModule::moveToUpDwonlookPR(PrOffset &offset,bool close_lighting,bool check_autochthonous)
{
    vision_updownlook_down_location->OpenLight();
    bool result = moveToToolDownlookPos(check_autochthonous);
    if(result)
    {
        vision_updownlook_down_location->performPR(offset);
    }
    if(close_lighting)
        vision_updownlook_down_location->CloseLight();
    return false;
}

bool SutModule::toolDownlookPR(PrOffset &offset, bool close_lighting, bool motion)
{
    vision_updownlook_down_location->OpenLight();
    if(!vision_updownlook_down_location->performPR(offset))
        return false;
    if(close_lighting)
        vision_updownlook_down_location->CloseLight();
    if(motion)
        carrier->StepMove_XY_Sync(-offset.X,-offset.Y);
    return false;
}

bool SutModule::toolUplookPR(PrOffset &offset, bool close_lighting, bool motion)
{
    vision_updownlook_up_location->OpenLight();
    if(!vision_updownlook_up_location->performPR(offset))
        return false;
    if(close_lighting)
        vision_updownlook_up_location->CloseLight();
    if(motion)
        carrier->StepMove_XY_Sync(-offset.X,-offset.Y);
    return false;
}

bool SutModule::toolDownlookPR(bool close_lighting, bool motion)
{
    PrOffset offset;
    return toolDownlookPR(offset,close_lighting,motion);
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
    qInfo("targert : %f",z);
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

void SutModule::receiveLoadSensorRequst(int sut_state)
{
    qInfo("receiveLoadSensorRequst %d in %d",sut_state,QThread::currentThreadId());
    if(states.allowLoadSensor())
        return;
    if(sut_state == SUT_STATE::NO_MATERIAL)
    {
        states.setSutHasSensor(false);
        states.setSutHasNgSensor(false);
        states.setSutHasProduct(false);
    }
    else if(sut_state == SUT_STATE::HAS_PRODUCT)
    {
        states.setSutHasSensor(false);
        states.setSutHasNgSensor(false);
        states.setSutHasProduct(true);
    }
    else if(sut_state == SUT_STATE::HAS_NG_SENSOR)
    {
        states.setSutHasSensor(false);
        states.setSutHasNgSensor(true);
        states.setSutHasProduct(false);
    }
    states.setAllowLoadSensor(true);
    qInfo("excute LoadSensorRequst");
}

void SutModule::run(bool has_material)
{
    is_run = true;
    while (is_run)
    {
        if(!states.allowLoadSensor())
        {
            QThread::msleep(1000);
            continue;
        }
        if((!states.sutHasSensor()))
        {
            if(!moveToLoadPos())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run =false;
                break;
            }
            if(!popgpin->Set(false))
            {
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                waitMessageReturn(is_run);
                continue;
            }
            QThread::msleep(100);
            if(! sut_cilent->sendSensorRequest(states.sutHasProduct(),states.sutHasNgSensor()))
            {
               sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
               waitMessageReturn(is_run);
               continue;
           }
           else
           {
               states.setSutHasSensor(true);
           }
        }
        if(states.sutHasSensor())
        {
            if(!popgpin->Set(true))
            {
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                waitMessageReturn(is_run);
                continue;
            }
            QThread::msleep(100);
            if((!moveToDownlookPR(offset))&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            else
            {
                emit sendLoadSensorFinish(offset.X,offset.Y,offset.Theta+parameters.cameraTheta());
                states.setAllowLoadSensor(false);
            }
        }
    }
}

void SutModule::startWork(bool reset_logic, int run_mode)
{
    qInfo("sut Module start reset:%d run_mode :%d in %d",reset_logic,run_mode,QThread::currentThreadId());
    if(run_mode == RunMode::Normal)run(true);
    else if(run_mode == RunMode::NoMaterial)run(false);

}

void SutModule::stopWork(bool wait_finish)
{
  is_run = false;
}

void SutModule::performHandlingOperation(int cmd)
{

}


