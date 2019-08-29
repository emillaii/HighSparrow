﻿#include "sutModule/sut_module.h"

#include <QMessageBox>
#include <tcpmessager.h>
#include "materialtray.h"

SutModule::SutModule()
{
    setName("SutModule");
    gui_thread_id = QThread::currentThreadId();
}

void SutModule::Init(MaterialCarrier *carrier,SutClient* sut_cilent, VisionLocation* downlook_location,VisionLocation* updownlook_down_location,VisionLocation* updownlook_up_locationn, XtVacuum *vacuum,XtCylinder* popgpin,int thread_id)
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
    this->thread_id = thread_id;
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
    temp_map.insert("SAFETY_POSITION", &this->safety_positon);
//    temp_map.insert("VISION_DOWNLOOK_LOCATION", &this->vision_downlook_location->parameters);
//    temp_map.insert("VISION_UPDOWNLOOK_LOCATION", &this->vision_updownlook_location->parameters);
    PropertyBase::saveJsonConfig(file_name, temp_map);
}

bool SutModule::checkSutSensorOrProduct(bool check_state)
{
    bool result = vacuum->checkHasMaterielSync();
    if(result == check_state)
        return true;
    QString error = QString(u8"SUT上逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool SutModule::checkSutHasMaterialSynic()
{
    bool result;
    result = vacuum->checkHasMaterielSync();
    if(result||RunMode::NoMaterial == states.runMode())
        return true;
    QString error = QString(u8"sut上应有料，但真空检测到无料。");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool SutModule::checkSutHasMaterial()
{
    bool result;
    result = vacuum->checkHasMateriel(thread_id);
    return result;
}

bool SutModule::waitSutCheckResult(bool check_state)
{
    bool result;
    result = vacuum->getHasMateriel(thread_id);
    if(result == check_state||RunMode::NoMaterial == states.runMode())
        return true;
    QString error;
    if(check_state)
        error = QString(u8"sut上应有料，但真空检测到无料。");
    else
        error = QString(u8"sut上应无料，但真空检测到有料。");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

void SutModule::resetLogic()
{
    if(is_run)return;
    states.reset();
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
    temp_map.insert("SAFETY_POSITION", &this->safety_positon);
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
    if(!result)
        AppendError(u8"执行downlook pr 失败");
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

bool SutModule::moveToDownlookSaveImage(QString imageName,bool close_lighting,bool check_autochthonous)
{
    vision_downlook_location->OpenLight();
    bool result = moveToDownlookPos(check_autochthonous);
    if(result)
    {
        if(has_material)
            QThread::msleep(80);
            result = vision_downlook_location->saveImage(imageName);
    }
    if(close_lighting)
        vision_downlook_location->CloseLight();
    return result;
}

bool SutModule::moveToLoadPos(bool check_autochthonous)
{
    qInfo("moveToLoadPos");
    bool result = carrier->Move_SZ_SX_Y_X_Sync(load_position.X(),load_position.Y(),parameters.loadPosArrivedY(),check_autochthonous);
    //result &= popgpin->Set(false,false);
    if(result)
    {
        result &= popgpin->Set(false,false);
        qInfo("moveToLoadPos z");
        result &= carrier->motor_z->MoveToPosSync(load_position.Z(),0.1);
        qInfo("moveToLoadPos z");
        result &= carrier->motor_y->WaitArrivedTargetPos(load_position.Y());
        result &= vacuum->Set(false,false,false);
    }
    qInfo("moveToLoadPos");
    return result;
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
    }else{
        return true;
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
    }else{
        return true;
    }
}

bool SutModule::moveToMushroomPos(bool check_autochthonous)
{
    popgpin->Set(true);
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

bool SutModule::moveZToSaftyInMushroom()
{
    if(carrier->GetFeedBackPos().Y < downlook_position.Y())
        return carrier->Move_Z_Sync(carrier->parameters.SafetyZ());
    else
        return true;
}

bool SutModule::moveZToSafety()
{
    return carrier->Move_Z_Sync(carrier->parameters.SafetyZ());
}

void SutModule::recordCurrentPos()
{
    record_position = carrier->GetFeedBackPos();
}

bool SutModule::movetoRecordPos(bool check_autochthonous)
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(record_position.X,record_position.Y,record_position.Z,check_autochthonous);
}

bool SutModule::movetoRecordPosAddOffset(double x_offset, double y_offset, double z_offset, bool check_autochthonous)
{
    return carrier->Move_SZ_SX_Y_X_Z_Sync(record_position.X + x_offset,record_position.Y + y_offset,record_position.Z + z_offset,check_autochthonous);
}

bool SutModule::moveToSafetyPos(bool check_autochthonous)
{
    popgpin->Set(true);
    return carrier->Move_SZ_SX_YS_X_Z_Sync(safety_positon.X(),safety_positon.Y(),safety_positon.Z(),check_autochthonous);
}

bool SutModule::OpenSutVacuum()
{
    return vacuum->Set(true);
}

bool SutModule::CloseSutVacuum()
{
    return vacuum->Set(false);
}

void SutModule::receiveLoadSensorRequst(int sut_state)
{
    qInfo("receiveLoadSensorRequst %d in %d",sut_state,QThread::currentThreadId());
    if(states.allowLoadSensor())
        return;
//    if(sut_state == SUT_STATE::NO_MATERIAL)
//    {
//        states.setSutHasSensor(false);
//        states.setSutHasNgSensor(false);
//        states.setSutHasProduct(false);
//        states.setSutHasNgProduct(false);
//    }
//    else if(sut_state == SUT_STATE::HAS_NG_SENSOR)
//    {
//        states.setSutHasSensor(false);
//        states.setSutHasNgSensor(true);
//        states.setSutHasProduct(false);
//        states.setSutHasNgProduct(false);
//    }
//    else if(sut_state == SUT_STATE::HAS_PRODUCT)
//    {
//        states.setSutHasSensor(false);
//        states.setSutHasNgSensor(false);
//        states.setSutHasProduct(true);
//        states.setSutHasNgProduct(false);
//    }
//    else if(sut_state == SUT_STATE::HAS_NG_PRODUCT)
//    {
//        states.setSutHasSensor(false);
//        states.setSutHasNgSensor(false);
//        states.setSutHasProduct(false);
//        states.setSutHasNgProduct(true);
//    }
//    this->sut_state = sut_state;
    states.setAllowLoadSensor(true);
    qInfo("excute LoadSensorRequst");
}

void SutModule::run(bool has_material)
{
    is_run = true;
    QString aa_module_name = states.stationNumber() == 0?"AA1CoreNew":"AA2CoreNew";
    while (is_run)
    {
        QThread::msleep(10);
        if(!states.allowLoadSensor())
            continue;
        if((states.sutMaterialState() != MaterialState::IsRaw)&&(!states.waitLoading()))
        {
            if(states.sutMaterialState() != MaterialState::IsEmpty)
            {
                if(!moveToLoadPos())
                {
                    sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                    is_run =false;
                    break;
                }
                if(!checkSutHasMaterialSynic())
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
                    if(waitMessageReturn(is_run))
                    {
                        states.setSutMaterialState(MaterialState::IsEmpty);
                        continue;
                    }
                    if(!is_run)break;
                }
            }
            else
            {
                checkSutHasMaterial();
                if(!moveToLoadPos())
                {
                    sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                    is_run =false;
                    break;
                }
                if(!waitSutCheckResult(false))
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
                    if(waitMessageReturn(is_run))
                        continue;
                    if(!is_run)break;
                }
            }
            sendMessageToModule("SensorLoaderModule","SutReady");
            states.setLoadingSensor(true);
            states.setWaitLoading(true);
        }

        if(states.waitLoading()&&(!states.loadingSensor())&&(states.sutMaterialState() == MaterialState::IsEmpty))
        {
            states.setAllowLoadSensor(false);
            states.setWaitLoading(false);
            sendMessageToModule(aa_module_name,"FinishPickRequest");
        }

        if(states.waitLoading()&&(!states.loadingSensor())&&(states.sutMaterialState() == MaterialState::IsRaw))
        {
            if(!checkSutHasMaterialSynic())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
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
            }
            else
            {
                DownlookPrDone = true;
            }
            emit sendLoadSensorFinish(offset.X,offset.Y,offset.Theta);
//            QJsonObject param;
//            param.insert("OffsetX",offset.X);
//            param.insert("OffsetY",offset.Y);
//            param.insert("OffsetT",offset.Theta);
            states.setAllowLoadSensor(false);
            states.setWaitLoading(false);
            sendMessageToModule(aa_module_name,"FinishPlaceRequest");
        }

    }
    qInfo("sut module end of thread");
}

void SutModule::startWork(int run_mode)
{
    QVariantMap run_params = inquirRunParameters();
    if(run_params.isEmpty())
    {
        sendAlarmMessage(ErrorLevel::ErrorMustStop,u8"启动参数为空.启动失败.");
        return;
    }
    if(run_params.contains("RunMode"))
    {
        states.setRunMode(run_params["RunMode"].toInt());
    }
    else
    {
        sendAlarmMessage(ErrorLevel::ErrorMustStop,u8"启动参数RunMode缺失.启动失败.");
        return;
    }
    if(run_params.contains("HandlyChangeSensor"))
    {
        states.setHandlyChangeSensor(run_params["HandlyChangeSensor"].toBool());
    }
    else
    {
        sendAlarmMessage(ErrorLevel::ErrorMustStop,u8"启动参数HandlyChangeSensor缺失.启动失败.");
        return;
    }
    if(run_params.contains("StationNumber")&&run_params.contains("DisableStation"))
    {
        QString local_station = run_params["StationNumber"].toString();
        states.setStationNumber(local_station.toInt());
        QVariantMap disable_map = run_params["DisableStation"].toMap();
        states.setDisableStation(disable_map[local_station].toBool());
    }
    else
    {
        sendAlarmMessage(ErrorLevel::ErrorMustStop,u8"启动参数StationNumber、DisableStation缺失.启动失败.");
        return;
    }
    if(states.disableStation())
        return;
    has_material = true;
    qInfo("sut Module start run_mode :%d in %d",run_mode,QThread::currentThreadId());
    if(states.runMode() == RunMode::Normal)
        run(true);
    else if(states.runMode() == RunMode::NoMaterial)
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
    is_handling = false;
}

void SutModule::receivceModuleMessage(QVariantMap message)
{
    qInfo("receive module message %s",TcpMessager::getStringFromQvariantMap(message).toStdString().c_str());
    QMutexLocker temp_locker(&message_mutex);
    if(message.contains("TargetModule")&&message["TargetModule"].toString() == "WorksManager")
        this->module_message = message;
    if(!message.contains("OriginModule"))
    {
        qInfo("message error! has no OriginModule.");
        return;
    }
    QString aa_module_name = states.stationNumber() == 0?"AA1CoreNew":"AA2CoreNew";
    if(message["OriginModule"].toString() == "SensorLoaderModule")
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString()=="FinishPickRequest")
            {
                states.setSutMaterialState(MaterialState::IsEmpty);
                states.setLoadingSensor(false);
            }
            else if(message["Message"].toString()=="FinishPlaceRequest")
            {
                states.setSutMaterialState(MaterialState::IsRaw);
                states.setLoadingSensor(false);
            }
            else
            {
                qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            }
        }
    }
    else if(message["OriginModule"].toString()== aa_module_name)
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString()=="NoSensor")
            {
                states.setSutMaterialState(MaterialState::IsEmpty);
                states.setAllowLoadSensor(true);
            }
            else if(message["Message"].toString()=="NgSensor")
            {
                states.setSutMaterialState(MaterialState::IsNgSensor);
                states.setAllowLoadSensor(true);
            }
            else if(message["Message"].toString()=="NgProduct")
            {
                states.setSutMaterialState(MaterialState::IsNgProduct);
                states.setAllowLoadSensor(true);
            }
            else if(message["Message"].toString()=="GoodProduct")
            {
                states.setSutMaterialState(MaterialState::IsProduct);
                states.setAllowLoadSensor(true);
            }
            else
            {
                qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            }
        }
    }
    else {
         qInfo("module name error %s",message["OriginModule"].toString().toStdString().c_str());
    }
}

PropertyBase *SutModule::getModuleState()
{
    return &states;
}

QMap<QString, PropertyBase *> SutModule::getModuleParameter()
{
    QMap<QString, PropertyBase *> temp;
    return temp;
}

