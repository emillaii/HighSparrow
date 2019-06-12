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

int LutModule::getConnectedClient()
{
    return server->getConnectedClients();
}
//aa1_picklens_position = aa1_uplook_position - (load_uplook_position + lpa_updownlook_offset) + lpa_downlook_load_position
//aa1_unpicklens_position = aa1_uplook_position - (load_uplook_position + lpa_updownlook_offset) + lpa_downlook_unload_position
//aa2_picklens_position = aa2_uplook_position - (load_uplook_position + lpa_updownlook_offset) + lpa_downlook_load_position
//aa2_unpicklens_position = aa2_uplook_position - (load_uplook_position + lpa_updownlook_offset) + lpa_downlook_unload_position
void LutModule::calculcateRelativePosition()
{
    int decimal_digit = 4;
    double camera_x = load_uplook_position.X() + lpa_camera_to_picker_offset.X();
    double camera_y = load_uplook_position.Y() + lpa_camera_to_picker_offset.Y();

    double temp_value = aa1_uplook_position.X() - camera_x + lut_downlook_load_position.X();
    aa1_picklens_position.setX(round(temp_value*pow(10,decimal_digit))/pow(10,decimal_digit));

    temp_value = aa1_uplook_position.X() - camera_x + lut_downlook_load_position.X();
    aa1_picklens_position.setY(aa1_uplook_position.Y() - camera_y + lut_downlook_load_position.Y());

    temp_value = aa1_uplook_position.X() - camera_x + lut_downlook_unload_position.X();
    aa1_unpicklens_position.setX(round(temp_value*pow(10,decimal_digit))/pow(10,decimal_digit));

    temp_value = aa1_uplook_position.Y() - camera_y + lut_downlook_unload_position.Y();
    aa1_unpicklens_position.setY(round(temp_value*pow(10,decimal_digit))/pow(10,decimal_digit));

    temp_value = aa2_uplook_position.X() - camera_x + lut_downlook_load_position.X();
    aa2_picklens_position.setX(round(temp_value*pow(10,decimal_digit))/pow(10,decimal_digit));

    temp_value = aa2_uplook_position.Y() - camera_y + lut_downlook_load_position.Y();
    aa2_picklens_position.setY(round(temp_value*pow(10,decimal_digit))/pow(10,decimal_digit));

    temp_value = aa2_uplook_position.X() - camera_x + lut_downlook_unload_position.X();
    aa2_unpicklens_position.setX(round(temp_value*pow(10,decimal_digit))/pow(10,decimal_digit));

    temp_value = aa2_uplook_position.Y() - camera_y + lut_downlook_unload_position.Y();
    aa2_unpicklens_position.setY(round(temp_value*pow(10,decimal_digit))/pow(10,decimal_digit));
}

void LutModule::receiveRequestMessage(QString message, QString client_ip)
{
    qInfo("Lut Module receive command:%s from ip: %s", message.toStdString().c_str(), client_ip.toStdString().c_str());
    QJsonObject obj = getJsonObjectFromString(message);
    QString cmd = obj["cmd"].toString("");
    obj.insert("client_ip",client_ip);
    if (cmd == "lensReq") {
        qInfo("Enqueue the lens request command in request quene");
        requestQueue.enqueue(obj);
    }
    else if (cmd == "moveToUnloadPosReq") {
        servingIP = client_ip;
        this->moveToLutDownlookUnloadPos();
        sendEvent("moveToUnloadPosResp");
    }
    else if (cmd == "moveToAA1UplookPosReq") {
        servingIP = client_ip;
        this->moveToAA1UplookPos();
        sendEvent("moveToAA1UplookPosResp");
    }
    else if (cmd == "moveToAA2UplookPosReq") {
        servingIP = client_ip;
        this->moveToAA2UplookPos();
        sendEvent("moveToAA2UplookPosResp");
    }
    else if (cmd == "tooluplookPRReq") {
        servingIP = client_ip;
        PrOffset pr_offset;
        this->receiveToolUpPRRequest(pr_offset);
        QJsonObject result;
        result.insert("prOffsetX", pr_offset.X);
        result.insert("prOffsetY", pr_offset.Y);
        result.insert("prOffsetT", pr_offset.Theta);
        result.insert("event", "tooluplookPRResp");
        emit sendMessageToClient(servingIP, getStringFromJsonObject(result));
    }
    else if (cmd.length() > 0) {
        qInfo("Enqueue the %s in action queue", cmd.toStdString().c_str());
        actionQueue.enqueue(obj);
    }
}

void LutModule::receiveLoadLensRequstFinish(int lens, int lens_tray)
{
    qInfo("receiveLensRequstFinish lens: %d lens_tray: %d",lens,lens_tray);
    QMutexLocker temp_locker(&loader_mutext);
    if(states.pickingLens()) {
        qInfo("LutModule is picking lens...");
        return;
    }
    states.setLutHasLens(true);
    qInfo("LutModule set has lens");
    states.setLutLensID(lens);
    states.setLutTrayID(lens_tray);
    states.setLutNgLensID(-1);
    states.setLutNgTrayID(-1);
    qInfo("LutModule current tray id lens: %d tray: %d", lens, lens_tray);
    states.setPickingLens(true);
}

void LutModule::run(bool has_material)
{
    qInfo("Start Lut Module Thread");
    is_run = true;
    bool isLocalHost = false;
    bool has_task = true;
    while(is_run){
        has_task = false;
        if(!has_task)
            QThread::msleep(100);
        has_task = false;
        if (requestQueue.size()>0 && state == HAS_LENS) {
            has_task = true;
            state = BUSY;
            QJsonObject obj = requestQueue.dequeue();
            qInfo("Start to consume request: %s", getStringFromJsonObject(obj).toStdString().c_str());
            QString client_ip = obj["client_ip"].toString("");
            servingIP = client_ip;
            QString cmd = obj["cmd"].toString("");
            if(client_ip == "::1")
            {
                qInfo("This command come from localhost");
                isLocalHost = true;
            }
            else
            {
                qInfo("This command come from anther computer");
                isLocalHost = false;
            }

            if (cmd == "lensReq")
            {
                qInfo("start commincate whit %s",servingIP.toStdString().c_str());
                actionQueue.clear();
                sendEvent("lensResp");
            }
        }
        else if (actionQueue.size()>0 && state == BUSY) {
            has_task = true;
            while(state == BUSY)
            {
                if(actionQueue.size()<=0)
                {
                    QThread::msleep(100);
                    if(is_run)
                        continue;
                    else
                        break;
                }
                if(states.cmd() == "")
                {
                    QJsonObject obj = actionQueue.dequeue();
                    qInfo("Start to consume action request: %s", getStringFromJsonObject(obj).toStdString().c_str());
                    QString client_ip = obj["client_ip"].toString("");
                    states.setCmd(obj["cmd"].toString(""));
                }

                if(states.cmd() == "unpickNgLensReq")
                {
                    if(!checkLutNgLens(false))
                    {
                        sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                        waitMessageReturn(is_run);
                        if(is_run)break;
                    }
                    bool action_result;
                    isLocalHost ?action_result = moveToAA1UnPickLens() : action_result = moveToAA2UnPickLens();
                    if((!action_result)&&has_material)
                    {
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    else
                    {
                        sendEvent("unpickNgLensResp");
                        states.setCmd("");
                        states.setLutNgLensID(isLocalHost?states.aa1LensID():states.aa2LensID());
                        states.setLutNgTrayID(isLocalHost?states.aa1TrayID():states.aa2TrayID());
                        qInfo("unpicked ng lens index %d tary_index %d",states.lutNgLensID(),states.lutNgTrayID());
                    }
                }
                else if (states.cmd() == "pickLensReq") {

                    if(!checkLutLens(true))
                    {
                        sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                        waitMessageReturn(is_run);
                        if(is_run)break;
                    }
                    bool action_result;
                    isLocalHost ?action_result = moveToAA1PickLens() : action_result = moveToAA2PickLens();
                    if((!action_result)&&has_material)
                    {
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    else
                    {
                        sendEvent("pickLensResp");
                        states.setCmd("");
                        isLocalHost?states.setAa1LensID(states.lutLensID()):states.setAa2LensID(states.lutLensID());
                        isLocalHost?states.setAa1TrayID(states.lutTrayID()):states.setAa2TrayID(states.lutTrayID());
                        if(isLocalHost)
                            qInfo("aa1 pick lens index %d ,tray_index %d",states.aa1LensID(),states.aa1TrayID());
                        else
                            qInfo("aa2 pick lens index %d ,tray_index %d",states.aa2LensID(),states.aa2TrayID());
                    }
                }
                else if (states.cmd() == "prReq") {
                    qInfo("perform PR start");
                    bool action_result;
                    PrOffset pr_offset;
                    isLocalHost ?action_result = moveToAA1UplookPR(pr_offset): action_result = moveToAA2UplookPR(pr_offset);
                    qInfo("prReq Uplook PR Result %d", action_result);
                    if((!action_result)&&has_material)
                    {
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    else
                    {
                        sendPrEvent(pr_offset);
                        states.setCmd("");
                    }
                } else if (states.cmd() == "lutLeaveReq") {
                    bool action_result;
                    isLocalHost ?action_result = moveToAA1readyPos(): action_result = moveToAA2readyPos();
                    if((!action_result)&&has_material)
                    {
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    else
                    {
                        sendEvent("lutLeaveResp");
                        states.setCmd("");
                        state = NO_LENS;
                    }
                }
            }

        }
        else  if (state == NO_LENS)
        {
            has_task = true;
            if(states.waitLens()&&is_run)
            {
                qInfo("LUT Module is waiting lens");
                bool lut_has_lens = false;
                {
                    QMutexLocker temp_locker(&loader_mutext);
                    lut_has_lens = states.lutHasLens();
                }
                if(lut_has_lens)
                {
                    if(!(checkLutLens(true)&&checkLutNgLens(false)))
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            states.setWaitLens(false);
                            continue;
                        }
                        if(!is_run)break;
                    }
                    qInfo("LUT Module has lens");
                    {
                        QMutexLocker temp_locker(&loader_mutext);
                        states.setWaitLens(false);
                        states.setPickingLens(false);
                    }
                    state = HAS_LENS;
                    bool action_result;
                    isLocalHost ?action_result = moveToAA2readyPos(): action_result = moveToAA1readyPos();
                    if(!action_result)
                    {
                        AppendError("move to ready pos fail");
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                }
                else
                {
                    QThread::msleep(100);
                }
            }
            else
            {
                if(!moveToLoadPos(true))
                {
                    AppendError("move to load pos fail");
                    sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                    is_run = false;
                    break;
                }
                if(!(checkLutLens(false)&&checkLutNgLens(states.lutNgLensID()>=0)))
                {
                    sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                    waitMessageReturn(is_run);
                    if(!is_run)break;
                }
                qInfo("LUT Module is not waiting lens");
                QMutexLocker temp_locker(&loader_mutext);
                states.setLutHasLens(false);
                emit sendLoadLensRequst(true,states.lutNgLensID(),states.lutNgTrayID());
                states.setWaitLens(true);
            }
        }
       }
    qInfo("LUT Module end of thread");
}

void LutModule::sendEvent(const QString event)
{
    QJsonObject result;
    result.insert("event", event);
    emit sendMessageToClient(servingIP, getStringFromJsonObject(result));
}

void LutModule::sendCmd(QString serving_ip,const QString cmd)
{
    QJsonObject result;
    result.insert("cmd", cmd);
    emit sendMessageToClient(serving_ip, getStringFromJsonObject(result));
}

void LutModule::sendPrEvent(const PrOffset pr_offset)
{
    QJsonObject result;
    result.insert("prOffsetX", pr_offset.X);
    result.insert("prOffsetY", pr_offset.Y);
    result.insert("prOffsetT", pr_offset.Theta);
    result.insert("event", "prResp");
    emit sendMessageToClient(servingIP, getStringFromJsonObject(result));
}
void LutModule::startWork(int run_mode)
{
    qInfo("Lut Module start work in run mode: %d", run_mode);
    if(run_mode == RunMode::Normal||run_mode == RunMode::OnllyLeftAA||run_mode == RunMode::OnlyRightAA)
        run(true);
    else if(run_mode == RunMode::NoMaterial)
        run(false);
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
    bool result = true;
    if(cmd == 1)
      result =  moveToAA1UplookPR(false,true);
    else if(cmd == 2)
      result =  moveToAA2UplookPR(false,true);
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        return;
    }
}

void LutModule::resetLogic()
{
    if(is_run)return;
    actionQueue.clear();
    requestQueue.clear();
    state = LUTState::NO_LENS;
    states.setLutTrayID(-1);
    states.setLutLensID(-1);
    states.setLutNgTrayID(-1);
    states.setLutNgLensID(-1);
    states.setAa1TrayID(-1);
    states.setAa1LensID(-1);
    states.setAa2TrayID(-1);
    states.setAa2LensID(-1);
    states.setWaitLens(false);
    states.setServingIP("");
    states.setLutHasLens(false);
    states.setPickingLens(false);
    states.setCmd("");
}

void LutModule::Init(MaterialCarrier *carrier, VisionLocation* uplook_location,VisionLocation* load_location,VisionLocation* mushroom_location, XtVacuum *load_vacuum, XtVacuum *unload_vacuum,XtGeneralOutput *gripper, SutModule *sut)
{
    this->carrier = carrier;
    parts.append(carrier);
    this->uplook_location = uplook_location;
    parts.append(uplook_location);
    this->load_location = load_location;
    parts.append(load_location);
    this->load_vacuum = load_vacuum;
    parts.append(load_vacuum);
    this->unload_vacuum = unload_vacuum;
    parts.append(unload_vacuum);
    this->mushroom_location = mushroom_location;
    parts.append(mushroom_location);
    this->sut = sut;
}

void LutModule::saveJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LUT_PARAMS", &parameters);
    temp_map.insert("LOAD_POSITION", &load_position);
    temp_map.insert("LUT_DOWNLOOK_LOAD_POSITION", &lut_downlook_load_position);
    temp_map.insert("LUT_DOWNLOOK_UNLOAD_POSITION", &lut_downlook_unload_position);
    temp_map.insert("LOAD_UPLOOK_POSITION", &load_uplook_position);
    temp_map.insert("AA1_UPDOWNLOOK_POSITION", &aa1_updownlook_position);
    temp_map.insert("AA1_PICKLENS_POSITION", &aa1_picklens_position);
    temp_map.insert("AA1_UNPICKLENS_POSITION", &aa1_unpicklens_position);
    temp_map.insert("AA1_UPLOOK_POSITION", &aa1_uplook_position);
    temp_map.insert("AA1_Ready_POSITION", &aa1_ready_position);
    temp_map.insert("AA2_UPDOWNLOOK_POSITION", &aa2_updownlook_position);
    temp_map.insert("AA2_PICKLENS_POSITION", &aa2_picklens_position);
    temp_map.insert("AA2_UNPICKLENS_POSITION", &aa2_unpicklens_position);
    temp_map.insert("AA2_UPLOOK_POSITION", &aa2_uplook_position);
    temp_map.insert("AA2_Ready_POSITION", &aa2_ready_position);
    temp_map.insert("LPA_CAMERA_TO_PICKER_POSITION", &lpa_camera_to_picker_offset);
    PropertyBase::saveJsonConfig(file_name, temp_map);
}

void LutModule::loadJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LUT_PARAMS", &parameters);
    temp_map.insert("LOAD_POSITION", &load_position);
    temp_map.insert("LUT_DOWNLOOK_LOAD_POSITION", &lut_downlook_load_position);
    temp_map.insert("LUT_DOWNLOOK_UNLOAD_POSITION", &lut_downlook_unload_position);
    temp_map.insert("LOAD_UPLOOK_POSITION", &load_uplook_position);
    temp_map.insert("AA1_UPDOWNLOOK_POSITION", &aa1_updownlook_position);
    temp_map.insert("AA1_PICKLENS_POSITION", &aa1_picklens_position);
    temp_map.insert("AA1_UNPICKLENS_POSITION", &aa1_unpicklens_position);
    temp_map.insert("AA1_UPLOOK_POSITION", &aa1_uplook_position);
    temp_map.insert("AA1_Ready_POSITION", &aa1_ready_position);
    temp_map.insert("AA2_UPDOWNLOOK_POSITION", &aa2_updownlook_position);
    temp_map.insert("AA2_PICKLENS_POSITION", &aa2_picklens_position);
    temp_map.insert("AA2_UNPICKLENS_POSITION", &aa2_unpicklens_position);
    temp_map.insert("AA2_UPLOOK_POSITION", &aa2_uplook_position);
    temp_map.insert("AA2_Ready_POSITION", &aa2_ready_position);
    temp_map.insert("LPA_CAMERA_TO_PICKER_POSITION", &lpa_camera_to_picker_offset);
    PropertyBase::loadJsonConfig(file_name, temp_map);
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

bool LutModule::moveToAA1UplookPR(bool close_lighting, bool check_autochthonous)
{
    PrOffset pr_offset;
    if(moveToAA1UplookPR(pr_offset,close_lighting,check_autochthonous))
    {
        QJsonObject result;
        result.insert("prOffsetX", pr_offset.X);
        result.insert("prOffsetY", pr_offset.Y);
        result.insert("prOffsetT", pr_offset.Theta);
        result.insert("event", "prResp");
        emit sendMessageToClient("::1", getStringFromJsonObject(result));
        return true;
    }
    AppendError(u8"执行AA1uplook PR失败");
    return false;
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
        result = uplook_location->performPR(offset);
    }
    if(close_lighting)
        uplook_location->CloseLight();
    return result;
}

bool LutModule::moveToAA2UplookPR(bool close_lighting, bool check_autochthonous)
{
    PrOffset pr_offset;
    if(moveToAA2UplookPR(pr_offset,close_lighting,check_autochthonous))
    {
        QJsonObject result;
        result.insert("prOffsetX", pr_offset.X);
        result.insert("prOffsetY", pr_offset.Y);
        result.insert("prOffsetT", pr_offset.Theta);
        result.insert("event", "prResp");
        emit sendMessageToClient("remote", getStringFromJsonObject(result));
        return true;
    }
    AppendError(u8"执行AA2 uplook PR失败");
    return false;
}

bool LutModule::moveToLoadPos(bool check_autochthonous)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(load_position.X(),load_position.Y(),load_position.Z(),check_autochthonous);
}

bool LutModule::moveToLutDownlookloadPos(bool check_autochthonous)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(lut_downlook_load_position.X(),lut_downlook_load_position.Y(),lut_downlook_load_position.Z(),check_autochthonous);
}

bool LutModule::moveToLutDownlookUnloadPos(bool check_autochthonous)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(lut_downlook_unload_position.X(),lut_downlook_unload_position.Y(),lut_downlook_unload_position.Z(),check_autochthonous);
}

bool LutModule::moveToLoadUplookPos(bool check_autochthonous)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(load_uplook_position.X(),load_uplook_position.Y(),load_uplook_position.Z(),check_autochthonous);
}

bool LutModule::moveToLoadUplookPR(bool check_autochthonous)
{
    return moveToLoadUplookPos(check_autochthonous);
//    if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
//        load_location->OpenLight();
//        if(moveToLoadUplookPos(check_autochthonous))
//            //        return  load_location->performPR();
//            return true;
//        return false;
//    }
//    return true;
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
    sendCmd("::1","gripperOnReq");
    bool result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_picklens_position.X(),aa1_picklens_position.Y(),aa1_picklens_position.Z(),check_autochthonous);
    if(result)
    {
        //todo one fuction
        result = carrier->ZSerchByForce(10,parameters.pickForce(),-1,0,load_vacuum);
        if(result)
        {
            sendCmd("::1","gripperOffReq");
            //            gripper->Set(false);
            //            Sleep(180);
            load_vacuum->Set(false);
            Sleep(500);
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
    //if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
        return carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_picklens_position.X(),aa1_picklens_position.Y(),aa1_picklens_position.Z(),check_autochthonous);
    //}else{
    //    return true;
    //}
}

bool LutModule::moveToAA1UnPickLens(bool check_autochthonous)
{
    bool result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_unpicklens_position.X(),aa1_unpicklens_position.Y(),aa1_unpicklens_position.Z(),check_autochthonous);
    if(result)
    {
        result = carrier->ZSerchByForce(10,parameters.pickForce(),-1,1,unload_vacuum);
        if(result)
        {
            sendCmd("::1","gripperOnReq");
            this->unload_vacuum->Set(true);
            Sleep(500);
        }
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToAA2PickLensPos(bool check_autochthonous)
{
//    if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
//        return true;
//    }
    return carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_picklens_position.X(),aa2_picklens_position.Y(),aa2_picklens_position.Z(),check_autochthonous);
}

bool LutModule::moveToAA2PickLens(bool need_return, bool check_autochthonous)
{
    qInfo("moveToAA2PickLens");
    sendCmd("remote","gripperOnReq");
    bool result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_picklens_position.X(),aa2_picklens_position.Y(),aa2_picklens_position.Z(),check_autochthonous);
    if(result)
    {
        result = carrier->ZSerchByForce(10,parameters.pickForce(),-1,0,load_vacuum);
        if (result) {
            sendCmd("remote","gripperOffReq");
            //            emit sendMessageToClient("remote", getStringFromJsonObject(gripperOffMessage));
            this->load_vacuum->Set(false);
            Sleep(500);  //ToDo: Put that in UI
        }
        if(need_return)
            result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToAA2UnPickLens(bool check_autochthonous)
{
    bool result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_unpicklens_position.X(),aa2_unpicklens_position.Y(),aa2_unpicklens_position.Z(),check_autochthonous);
    if(result)
    {
        result = carrier->ZSerchByForce(10,parameters.pickForce(),-1,0,load_vacuum);
        if(result)
        {
            sendCmd("remote","gripperOnReq");
            this->unload_vacuum->Set(true);
            Sleep(500);  //ToDo: Put that in UI
        }
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

bool LutModule::moveToAA1readyPos(bool check_autochthonous)
{
    qInfo("moveToAA1readyPos(%f,%f,%f)",aa1_ready_position.X(),aa1_ready_position.Y(),aa1_ready_position.Z());
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_ready_position.X(),aa1_ready_position.Y(),aa1_ready_position.Z(),check_autochthonous);
}

bool LutModule::moveToAA2readyPos(bool check_autochthonous)
{
    qInfo("moveToAA2readyPos(%f,%f,%f)",aa2_ready_position.X(),aa2_ready_position.Y(),aa2_ready_position.Z());
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_ready_position.X(),aa2_ready_position.Y(),aa2_ready_position.Z(),check_autochthonous);
}

bool LutModule::checkLutLens(bool check_state)
{
    bool result = load_vacuum->checkHasMateriel();
    if(result == check_state)
        return true;
    QString error = QString(u8"LUT放Lens位置上逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool LutModule::checkLutNgLens(bool check_state)
{
    bool result = unload_vacuum->checkHasMateriel();
    if(result == check_state)
        return true;
    QString error = QString(u8"LUT放NgLens位置逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool LutModule::stepMove_XY_Sync(double x, double y)
{
    return carrier->StepMove_XY_Sync(x,y);
}

void LutModule::receiveToolUpPRRequest(PrOffset &offset)
{
    sut->toolUplookPR(offset, true, false);
}

