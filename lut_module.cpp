#include "lut_module.h"
#include "commonutils.h"

LutModule::LutModule(QString name, QObject *parent):ThreadWorkerBase (name)
{
}

LUTState LutModule::getLUTState()
{
    return state;
}

void LutModule::openServer(int port)
{
    server = new SparrowQServer(port);
    connect(server, &SparrowQServer::receiveRequestMessage, this, &LutModule::receiveRequestMessage, Qt::DirectConnection);
    connect(this, &LutModule::sendMessageToClient, this->server, &SparrowQServer::sendMessageToClient);
}

void LutModule::receiveRequestMessage(QString message, QString client_ip)
{
    qInfo("Lut Module receive command:%s from ip: %s", message.toStdString().c_str(), client_ip.toStdString().c_str());
    QJsonObject obj = getJsonObjectFromString(message);
    obj.insert("client_ip",client_ip);
    if (obj["cmd"] == "lensReq")
        requestQueue.enqueue(obj);
    else {
        actionQueue.enqueue(obj);
    }
}

void LutModule::run(bool has_material)
{
    qInfo("Start Lut Module Thread");
    state = HAS_LENS;
    is_run = true;
    while(is_run){
        if (requestQueue.size()>0 && state == HAS_LENS) {
            state = BUSY;
            QJsonObject obj = requestQueue.dequeue();
            QJsonObject result;
            qInfo("Start to consume request: %s", getStringFromJsonObject(obj).toStdString().c_str());
            QString client_ip = obj["client_ip"].toString("");
            QString cmd = obj["cmd"].toString("");
            bool isLocalHost = false;
            if(client_ip == "::1") {
                qInfo("This command come from localhost");
                isLocalHost = true;
            }
            if (cmd == "lensReq") {
                //isLocalHost ? moveToAA1UplookPos() : moveToAA2UplookPos();
                result.insert("event", "lensResp");
            }
            emit sendMessageToClient(obj["client_ip"].toString(), getStringFromJsonObject(result));
        }
        if (actionQueue.size()>0 && state == BUSY) {
            QJsonObject obj = actionQueue.dequeue();
            QJsonObject result;
            qInfo("Start to consume action request: %s", getStringFromJsonObject(obj).toStdString().c_str());
            QString client_ip = obj["client_ip"].toString("");
            QString cmd = obj["cmd"].toString("");
            bool isLocalHost = false;
            if(client_ip == "::1") {
                qInfo("This command come from localhost");
                isLocalHost = true;
            }
            if (cmd == "prReq") {
                result.insert("event", "prResp");
            } else if (cmd == "lutLeaveReq") {
                //moveToUnloadPos();
                result.insert("event", "lutLeaveResp");
                //state = NO_LENS;
                state = HAS_LENS;
            }
            emit sendMessageToClient(obj["client_ip"].toString(), getStringFromJsonObject(result));
        }
        qInfo("Working in state : %d", state);
        QThread::msleep(500);
    }
    qInfo("LUT Module end of thread");
}


void LutModule::startWork(bool reset_logic, int run_mode)
{
    qInfo("Lut Module start work in run mode: %d", run_mode);
    //if(reset_logic)ResetLogic();
    if(run_mode == RunMode::Normal)run(true);
    else if(run_mode == RunMode::NoMaterial)run(false);
    return;
}

void LutModule::stopWork(bool wait_finish)
{
    qInfo("Lut Module stop work");
    is_run = false;
    return;
}

void LutModule::performHandlingOperation(int cmd)
{
    qInfo("Lut Module perform command: %d", cmd);
    return;
}

void LutModule::Init(MaterialCarrier *carrier, VisionLocation* uplook_location,VisionLocation* load_location,VisionLocation* mushroom_location, XtVacuum *load_vacuum, XtVacuum *unload_vacuum,XtGeneralOutput *gripper)
{
    this->carrier = carrier;
    this->uplook_location = uplook_location;
    this->load_location = load_location;
    this->load_vacuum = load_vacuum;
    this->unload_vacuum = unload_vacuum;
    this->mushroom_location = mushroom_location;
    this->gripper = gripper;
}

void LutModule::saveJsonConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LUT_PARAMS", &parameters);
//    temp_map.insert("LUT_CARRIER_PARAMS", &this->carrier->parameters);
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
//    temp_map.insert("VISION_UPLOOK_LOCATION", &this->uplook_location->parameters);
//    temp_map.insert("VISION_UPDOWNLOOK_LOCATION", &this->updownlook_location->parameters);
//    temp_map.insert("VISION_LOAD_LOCATION", &this->load_location->parameters);
//    temp_map.insert("VISION_MUSHROOM_LOCATION", &this->mushroom_location->parameters);
    PropertyBase::saveJsonConfig("config//lutConfig.json", temp_map);
}

void LutModule::loadParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LUT_PARAMS", &parameters);
//    temp_map.insert("LUT_CARRIER_PARAMS", &this->carrier->parameters);
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
//    temp_map.insert("VISION_UPLOOK_LOCATION", &this->uplook_location->parameters);
//    temp_map.insert("VISION_UPDOWNLOOK_LOCATION", &this->updownlook_location->parameters);
//    temp_map.insert("VISION_LOAD_LOCATION", &this->load_location->parameters);
//    temp_map.insert("VISION_MUSHROOM_LOCATION", &this->mushroom_location->parameters);
    PropertyBase::loadJsonConfig("config//lutConfig.json", temp_map);
}

bool LutModule::moveToAA1UplookPos(bool check_autochthonous)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_uplook_position.X(),aa1_uplook_position.Y(),aa1_uplook_position.Z(),check_autochthonous);
}

bool LutModule::moveToAA1UplookPR(PrOffset &offset, bool close_lighting,bool check_autochthonous)
{
    uplook_location->OpenLight();
    bool result = moveToAA1UplookPos(check_autochthonous);
    if(result)
    {
      result = uplook_location->performPR(offset);
    }
    if(close_lighting)
        uplook_location->CloseLight();
    return result;
}

bool LutModule::moveToAA2UplookPos(bool check_autochthonous)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_uplook_position.X(),aa2_uplook_position.Y(),aa2_uplook_position.Z(),check_autochthonous);
}

bool LutModule::moveToAA2UplookPR(PrOffset &offset, bool close_lighting,bool check_autochthonous)
{
    uplook_location->OpenLight();
    bool result = moveToAA2UplookPos(check_autochthonous);
    if(result)
    {
      uplook_location->performPR(offset);
    }
    if(close_lighting)
        uplook_location->CloseLight();
    return false;
}

bool LutModule::moveToLoadPos(bool check_autochthonous)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(load_position.X(),load_position.Y(),load_position.Z(),check_autochthonous);
}

bool LutModule::moveToUnloadPos(bool check_autochthonous)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(unload_position.X(),unload_position.Y(),unload_position.Z(),check_autochthonous);
}

bool LutModule::moveToLoadUplookPos(bool check_autochthonous)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(load_uplook_position.X(),load_uplook_position.Y(),load_uplook_position.Z(),check_autochthonous);
}

bool LutModule::moveToLoadUplookPR(bool check_autochthonous)
{
    load_location->OpenLight();
    if(moveToLoadUplookPos(check_autochthonous))
        return  load_location->performPR();
    return false;
}

double LutModule::getLoadUplookPRX()
{
    return load_location->getCurrentOffset().x();
}

double LutModule::getLoadUplookPRY()
{
    return load_location->getCurrentOffset().y();
}

bool LutModule::moveToAA1PickLens(bool need_return,bool check_autochthonous)
{
    gripper->Set(true);
    bool result;
        result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_picklens_position.X(),aa1_picklens_position.Y(),aa1_picklens_position.Z(),check_autochthonous);
    if(result)
    {
        //todo one fuction
        double reuslt_pos;
        result = carrier->ZSerchByForce(10,parameters.pickForce(),-1,0,load_vacuum);
        if(result)
        {
            gripper->Set(false);
            Sleep(180);
            load_vacuum->Set(false);
            Sleep(20);
        }
        if(need_return)
            result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::vcmReturn()
{
    return  carrier->ZSerchReturn();
}

bool LutModule::moveToAA1PickLensPos(bool check_autochthonous)
{
    return carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_picklens_position.X(),aa1_picklens_position.Y(),aa1_picklens_position.Z(),check_autochthonous);
}

bool LutModule::moveToAA1UnPickLens(bool check_autochthonous)
{
    bool result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_unpicklens_position.X(),aa1_unpicklens_position.Y(),aa1_unpicklens_position.Z(),check_autochthonous);
    if(result)
    {
        result = carrier->ZSerchByForce(10,parameters.pickForce(),-1,1,unload_vacuum);
        if(result)
            gripper->Set(true);
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToAA2PickLensPos(bool check_autochthonous)
{
     return carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_picklens_position.X(),aa2_picklens_position.Y(),aa2_picklens_position.Z(),check_autochthonous);
}

bool LutModule::moveToAA2PickLens(bool need_return, bool check_autochthonous)
{
    qInfo("moveToAA2PickLens");
    QJsonObject gripperOnMessage;
    gripperOnMessage.insert("cmd", "gripperOnReq");
    emit sendMessageToClient("remote", getStringFromJsonObject(gripperOnMessage));
    bool result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_picklens_position.X(),aa2_picklens_position.Y(),aa2_picklens_position.Z(),check_autochthonous);
    if(result)
    {
        result = carrier->ZSerchByForce(10,parameters.pickForce(),-1,0,load_vacuum);
        QJsonObject gripperOffMessage;
        gripperOffMessage.insert("cmd", "gripperOffReq");
        emit sendMessageToClient("remote", getStringFromJsonObject(gripperOffMessage));
        Sleep(250);
        if(need_return)
            result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToAA2UnPickLens(bool check_autochthonous)
{
    bool result;
    gripper->Set(false);
    if(result)
        result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_unpicklens_position.X(),aa2_unpicklens_position.Y(),aa2_unpicklens_position.Z(),check_autochthonous);
    if(result)
    {
        double reuslt_pos;
        result = carrier->ZSerchByForce(reuslt_pos,parameters.pickForce(),-1,0,load_vacuum);
        if(result)
//  todo          grabber->Set(false);
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToAA1MushroomLens(bool check_autochthonous)
{
     return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_mushroom_position.X(),aa1_mushroom_position.Y(),aa1_mushroom_position.Z(),check_autochthonous);
}

bool LutModule::moveToAA2MushroomLens(bool check_autochthonous)
{
     return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_mushroom_position.X(),aa2_mushroom_position.Y(),aa2_mushroom_position.Z(),check_autochthonous);
}

bool LutModule::stepMove_XY_Sync(double x, double y)
{
    return carrier->StepMove_XY_Sync(x,y);
}

