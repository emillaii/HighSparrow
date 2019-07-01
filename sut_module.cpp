#include "sut_module.h"

#include <QMessageBox>

SutModule::SutModule()
{
    setName("SutModule");
    gui_thread_id = QThread::currentThreadId();
}

void SutModule::Init(MaterialCarrier *carrier,SutClient* sut_cilent, VisionLocation* downlook_location,VisionLocation* updownlook_down_location,VisionLocation* updownlook_up_locationn, XtVacuum *vacuum,XtCylinder* popgpin)
{
    this->carrier = carrier;
    parts.append(carrier);
    this->sut_cilent = sut_cilent;
//    parts.append(sut_cilent);
    this->vision_downlook_location = downlook_location;
    parts.append(vision_downlook_location);
    this->vision_updownlook_down_location = updownlook_down_location;
    parts.append(vision_updownlook_down_location);
    this->vision_updownlook_up_location = updownlook_up_locationn;
    parts.append(vision_updownlook_up_location);
    this->vacuum = vacuum;
    parts.append(vacuum);
    this->popgpin = popgpin;
    parts.append(popgpin);
    setName(parameters.moduleName());
}

void SutModule::saveJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("SUT_PARAMS", &this->parameters);
//    temp_map.insert("SUT_CARRIER_PARAMS", &this->carrier->parameters);
    temp_map.insert("LOAD_POSITION", &this->load_position);
    temp_map.insert("DOWNLOOK_POSITION", &this->downlook_position);
    temp_map.insert("TOOLUPLOOK_POSITION", &this->tool_uplook_positon);
    temp_map.insert("TOOLDOWNLOOK_POSITION", &this->tool_downlook_position);
    temp_map.insert("MUSHROOM_POSITION", &this->mushroom_positon);
    temp_map.insert("UP_DOWNLOOK_POSITION", &this->up_downlook_offset);
//    temp_map.insert("VISION_DOWNLOOK_LOCATION", &this->vision_downlook_location->parameters);
//    temp_map.insert("VISION_UPDOWNLOOK_LOCATION", &this->vision_updownlook_location->parameters);
    PropertyBase::saveJsonConfig(file_name, temp_map);
}

bool SutModule::checkSutSensorOrProduct(bool check_state)
{
    bool result = vacuum->checkHasMateriel();
    if(result == check_state)
        return true;
    QString error = QString(u8"SUT上逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

void SutModule::resetLogic()
{
    if(is_run)return;
    states.setSutHasSensor(false);
    states.setSutHasNgSensor(false);
    states.setSutHasProduct(false);
    states.setSutHasNgProduct(false);
    states.setAllowLoadSensor(false);
    states.setLoadingSensor(false);
    states.setWaitLoading(false);
    qInfo("resetLogic");
}

void SutModule::loadParams(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("SUT_PARAMS", &parameters);
//    temp_map.insert("SUT_CARRIER_PARAMS", &this->carrier->parameters);
    temp_map.insert("LOAD_POSITION", &this->load_position);
    temp_map.insert("DOWNLOOK_POSITION", &this->downlook_position);
    temp_map.insert("TOOLUPLOOK_POSITION", &this->tool_uplook_positon);
    temp_map.insert("TOOLDOWNLOOK_POSITION", &this->tool_downlook_position);
    temp_map.insert("MUSHROOM_POSITION", &this->mushroom_positon);
    temp_map.insert("UP_DOWNLOOK_POSITION", &this->up_downlook_offset);
//    temp_map.insert("VISION_DOWNLOOK_LOCATION", &this->vision_downlook_location->parameters);
//    temp_map.insert("VISION_UPDOWNLOOK_LOCATION", &this->vision_updownlook_location->parameters);
    PropertyBase::loadJsonConfig(file_name, temp_map);
}

bool SutModule::moveToDownlookPR(PrOffset &offset,bool close_lighting,bool check_autochthonous)
{
    vision_downlook_location->OpenLight();
    bool result = moveToDownlookPos(check_autochthonous);
    if(result)
    {
        if(has_material)
        result = vision_downlook_location->performPR(offset);
    }
    if(close_lighting)
        vision_downlook_location->CloseLight();
    return result;
}

bool SutModule::moveToDownlookPR(bool close_lighting, bool check_autochthonous)
{
    PrOffset offset;
    if((moveToDownlookPR(offset,close_lighting,check_autochthonous)))
    {
        emit sendLoadSensorFinish(offset.X,offset.Y,offset.Theta);
        return true;
    }
    AppendError(u8"执行downlook pr 失败");
    return false;
}

bool SutModule::moveToLoadPos(bool check_autochthonous)
{
    qInfo("moveToLoadPos");
//    popgpin->Set(true);
    return carrier->Move_SZ_SX_Y_X_Z_Sync(load_position.X(),load_position.Y(),load_position.Z(),check_autochthonous);
}

bool SutModule::moveToDownlookPos(bool check_autochthonous)
{
    qInfo("moveToDownlookPos");
    popgpin->Set(true);
    return carrier->Move_SZ_SX_Y_X_Z_Sync(downlook_position.X(),downlook_position.Y(),downlook_position.Z(),check_autochthonous);
}

bool SutModule::moveToReadyPos()
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(load_position.X(),load_position.Y(),load_position.Z(),true);
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
    return true;
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
    return true;
}

bool SutModule::toolDownlookPR(bool close_lighting, bool motion)
{
    PrOffset offset;
    return toolDownlookPR(offset,close_lighting,motion);
}

bool SutModule::moveToToolDownlookPos(bool check_autochthonous)
{
    popgpin->Set(true);
    if(QThread::currentThreadId()==gui_thread_id){
        QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"提示"),tr(u8"是否移动"),QMessageBox::Yes|QMessageBox::No);
        if(rb == QMessageBox::Yes){
            return carrier->Move_SZ_SX_Y_X_Z_Sync(tool_downlook_position.X(),tool_downlook_position.Y(),tool_downlook_position.Z(),check_autochthonous);
        }else{
            return false;
        }
    }
    if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
        return carrier->Move_SZ_SX_Y_X_Z_Sync(tool_downlook_position.X(),tool_downlook_position.Y(),tool_downlook_position.Z(),check_autochthonous);
    //}else{
     //   return true;
    }
}

bool SutModule::moveToToolUplookPos(bool check_autochthonous)
{
    popgpin->Set(true);
    if(QThread::currentThreadId()==gui_thread_id){
        QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"提示"),tr(u8"是否移动"),QMessageBox::Yes|QMessageBox::No);
        if(rb == QMessageBox::Yes){
            return carrier->Move_SZ_SX_Y_X_Z_Sync(tool_uplook_positon.X(),tool_uplook_positon.Y(),tool_uplook_positon.Z(),check_autochthonous);
        }else{
            return false;
        }
    }
    if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
        return carrier->Move_SZ_SX_Y_X_Z_Sync(tool_uplook_positon.X(),tool_uplook_positon.Y(),tool_uplook_positon.Z(),check_autochthonous);
    //}else{
    //    return true;
    }
}

bool SutModule::moveToMushroomPos(bool check_autochthonous)
{
    return carrier->Move_SZ_SX_YS_X_Z_Sync(mushroom_positon.X(),mushroom_positon.Y(),mushroom_positon.Z(),check_autochthonous);
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
        states.setSutHasNgProduct(false);
    }
    else if(sut_state == SUT_STATE::HAS_NG_SENSOR)
    {
        states.setSutHasSensor(false);
        states.setSutHasNgSensor(true);
        states.setSutHasProduct(false);
        states.setSutHasNgProduct(false);
    }
    else if(sut_state == SUT_STATE::HAS_PRODUCT)
    {
        states.setSutHasSensor(false);
        states.setSutHasNgSensor(false);
        states.setSutHasProduct(true);
        states.setSutHasNgProduct(false);
    }
    else if(sut_state == SUT_STATE::HAS_NG_PRODUCT)
    {
        states.setSutHasSensor(false);
        states.setSutHasNgSensor(false);
        states.setSutHasProduct(false);
        states.setSutHasNgProduct(true);
    }
    this->sut_state = sut_state;
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
        if((!states.sutHasSensor())&&(!states.loadingSensor()))
        {
            if(!moveToLoadPos(true))
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
            states.setLoadingSensor(true);
        }
        if((!states.sutHasSensor())&&states.loadingSensor())
        {
            if(states.sutHasProduct()||states.sutHasNgSensor()||states.sutHasNgProduct())
            {
                if(!checkSutSensorOrProduct(true))
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                    if(waitMessageReturn(is_run))
                    {
                        states.setSutHasProduct(false);
                        states.setSutHasNgSensor(false);
                        states.setSutHasNgProduct(false);
                        continue;
                    }
                    if(!is_run)break;
                }
            }
            else
            {
                if(!checkSutSensorOrProduct(false))
                {
                    sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                    waitMessageReturn(is_run);
                    if(!is_run)break;
                }
            }
            if(! sut_cilent->sendSensorRequest(is_run,sut_state))
            {
               AppendError(u8"等待sensor超时");
               sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
               waitMessageReturn(is_run);
               continue;
           }
           else
           {
               states.setSutHasSensor(true);
               states.setLoadingSensor(false);
           }
        }
        if(states.sutHasSensor())
        {
            if(!checkSutSensorOrProduct(true))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setSutHasSensor(false);
                    continue;
                }
                if(!is_run)break;
            }

            if(!popgpin->Set(true))
            {
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                waitMessageReturn(is_run);
                continue;
            }
            QThread::msleep(100);
            if((!moveToDownlookPR(offset))&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    continue;
                }
                emit sendLoadSensorFinish(offset.X,offset.Y,offset.Theta);
                states.setAllowLoadSensor(false);
            }
            else
            {
                emit sendLoadSensorFinish(offset.X,offset.Y,offset.Theta);
                states.setAllowLoadSensor(false);
            }
        }
    }
    qInfo("sut module end of thread");
}

void SutModule::startWork(int run_mode)
{
    has_material = true;
    qInfo("sut Module start run_mode :%d in %d",run_mode,QThread::currentThreadId());
    if(run_mode == RunMode::Normal)
        run(true);
    else if(Name().contains("1")&&run_mode == RunMode::OnllyLeftAA)
        run(true);
    else if(Name().contains("2")&&run_mode == RunMode::OnlyRightAA)
        run(true);
    else if(run_mode == RunMode::NoMaterial)
    {
        has_material = false;
        run(false);
    }

}

void SutModule::stopWork(bool wait_finish)
{
  is_run = false;
}

void SutModule::performHandlingOperation(int cmd)
{
    if(cmd == 1)
        if(!moveToDownlookPR(false,true))
            sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
}


