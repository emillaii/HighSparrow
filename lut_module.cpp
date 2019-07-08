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
        tcp_mutex.lock();
        requestQueue.enqueue(obj);
        tcp_mutex.unlock();
        qInfo("Enqueue the lens request command in request quene finish");
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
        QMutexLocker locker(&tcp_mutex);
        actionQueue.enqueue(obj);
    }
}

void LutModule::receiveLoadLensRequstFinish(int lens, int lens_tray)
{
    qInfo("receiveLensRequstFinish lens: %d lens_tray: %d",lens,lens_tray);
    QMutexLocker temp_locker(&loader_mutext);
    if(!states.waitingLens()) {
        qInfo("LutModule not need lens...");
        return;
    }
    qInfo("LutModule set has lens");
    states.setLutLensID(lens);
    states.setLutTrayID(lens_tray);
    qInfo("LutModule loaded lens id: %d tray id: %d", lens, lens_tray);
    states.setFinishWaitLens(true);
}

void LutModule::run(bool has_material)
{
    qInfo("Start Lut Module Thread");
    is_run = true;
    bool has_task = true;
    time_label = QTime::currentTime();
    while(is_run){
        has_task = false;
        if(!has_task)
            QThread::msleep(1);
        has_task = false;
        if (state == HAS_LENS) {
            tcp_mutex.lock();
            if((!requestQueue.isEmpty())) {
                has_task = true;
                state = BUSY;
                QJsonObject obj = requestQueue.dequeue();
                tcp_mutex.unlock();
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
                    //                actionQueue.clear();
                    sendEvent("lensResp");
                }
            }
            else
            {
                tcp_mutex.unlock();
            }
        }
        else if ((!isActionEmpty())&&state == BUSY) {
            has_task = true;
            while(state == BUSY)
            {
                if(isActionEmpty())
                {
                    QThread::msleep(1);
                    if(states.cmd() == "")
                        continue;
                    if(!is_run)
                        break;
                }
                if(states.cmd() == "")
                {
                    QMutexLocker locker(&tcp_mutex);
                    QJsonObject obj = actionQueue.dequeue();
                    qInfo("Start to consume action request: %s", getStringFromJsonObject(obj).toStdString().c_str());
                    QString client_ip = obj["client_ip"].toString("");
                    states.setCmd(obj["cmd"].toString(""));
                }

                if(states.cmd() == "unpickNgLensReq"&&(!states.unpickedNgLens()))
                {
                    qInfo("start to unpickNgLensReq");
                    if(!checkLutNgLens(false))
                    {
                        AppendError("请人工拿走多LUT上NG位置多余物料并检测真空状态！");
                        sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                        waitMessageReturn(is_run);
                        if(!is_run)break;
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
                        states.setUnpickedNgLens(true);
                        lens_uph.addCurrentReslutNumber(updateAccumulatedHour());
                        if(isLocalHost)
                            left_lens_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                        else
                            right_lens_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                        qInfo("unpicked ng lens index %d tary_index %d",states.lutNgLensID(),states.lutNgTrayID());
                    }
                }
                else if (states.cmd() == "unpickNgLensReq"&&states.unpickedNgLens())
                {
                    sendEvent("unpickNgLensResp");
                    states.setCmd("");
                    states.setLutNgLensID(isLocalHost?states.aa1LensID():states.aa2LensID());
                    states.setLutNgTrayID(isLocalHost?states.aa1TrayID():states.aa2TrayID());
                    lens_uph.addCurrentReslutNumber(updateAccumulatedHour());
                    if(isLocalHost)
                        left_lens_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    else
                        right_lens_uph.addCurrentReslutNumber(updateAccumulatedHour(false));
                    qInfo("unpicked ng lens index %d tary_index %d",states.lutNgLensID(),states.lutNgTrayID());
                }
                else if (states.cmd() == "pickLensReq"&&(!states.pickedLens())) {
                    qInfo("states.pickedLens %d",states.pickedLens());
                    if(!checkLutLens(true))
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            state = NO_LENS;
                            break;
                        }
                        if(!is_run)break;
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
                        states.setPickedLens(true);
                        isLocalHost?states.setAa1LensID(states.lutLensID()):states.setAa2LensID(states.lutLensID());
                        isLocalHost?states.setAa1TrayID(states.lutTrayID()):states.setAa2TrayID(states.lutTrayID());
                        lens_uph.addCurrentNumber(updateAccumulatedHour());
                        if(isLocalHost)
                            left_lens_uph.addCurrentNumber(updateAccumulatedHour(false));
                        else
                            right_lens_uph.addCurrentNumber(updateAccumulatedHour(false));
                    }
                }
                else if (states.cmd() == "pickLensReq"&&states.pickedLens())
                {
                    qInfo("states.pickedLens %d",states.pickedLens());
                    sendEvent("pickLensResp");
                    states.setCmd("");
                    isLocalHost?states.setAa1LensID(states.lutLensID()):states.setAa2LensID(states.lutLensID());
                    isLocalHost?states.setAa1TrayID(states.lutTrayID()):states.setAa2TrayID(states.lutTrayID());
                    lens_uph.addCurrentNumber(updateAccumulatedHour());
                    if(isLocalHost)
                        left_lens_uph.addCurrentNumber(updateAccumulatedHour(false));
                    else
                        right_lens_uph.addCurrentNumber(updateAccumulatedHour(false));
                }
                else if (states.cmd() == "prReq") {
                    qInfo("perform PR start");
                    bool action_result;
                    PrOffset pr_offset;
                    isLocalHost ?action_result = moveToAA1UplookPR(pr_offset): action_result = moveToAA2UplookPR(pr_offset);
                    qInfo("prReq Uplook PR Result %d", action_result);
                    if((!action_result)&&has_material)
                    {
                        sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            is_run = false;
                        }
                        else
                        {
                            continue;
                        }
                        if(!is_run)break;
                    }
                    else
                    {
                        sendPrEvent(pr_offset);
                        states.setCmd("");
                    }
                }
                else if (states.cmd() == "lutLeaveReq") {
                    bool action_result;
                    isLocalHost ?action_result = moveToAA1ReturnPos(): action_result = moveToAA2ReturnPos();
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
                        states.setPickedLens(false);
                        states.setUnpickedNgLens(false);
                        state = NO_LENS;
                    }
                }
            }
        }
        else  if (state == NO_LENS)
        {
            has_task = true;
            if(states.waitingLens()&&is_run)
            {
                if(states.finishWaitLens())
                {
                    if(!(checkLutLens(true)&&checkLutNgLens(false)))
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                        if(waitMessageReturn(is_run))
                        {
                            states.setWaitingLens(false);
                            continue;
                        }
                        if(!is_run)break;
                    }
                    qInfo("LUT Module has lens");
                    {
                        QMutexLocker temp_locker(&loader_mutext);
                        states.setWaitingLens(false);
                        states.setFinishWaitLens(false);
                    }
                    state = HAS_LENS;
                    bool action_result;
                    isLocalHost ?action_result = moveToAA2ReadyPos(): action_result = moveToAA1ReadyPos();
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
                states.setWaitingLens(true);
            }
        }
    }
    qInfo("LUT Module end of thread");
}

void LutModule::runTest()
{
    qInfo("Start Lut Module Thread");
    is_run = true;
    bool has_task = true;
    int current_time = parameters.repeatTime();
    int current_count = parameters.testLensCount();
    bool finished = false;
    isLocalHost = true;
    while(is_run){
        if(!has_task)
            QThread::msleep(1);
        has_task = false;
        //取lens请求
        if((!states.waitingLens())&&(!states.lutHasLens())&&(!states.lutLoadReady()))
        {
            has_task = true;
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
            qInfo("LUT Module is waiting lens");
            emit sendLoadLensRequst(true,states.lutNgLensID(),states.lutNgTrayID());
            states.setWaitingLens(true);
        }
        //等待lens
        if(states.waitingLens()&&states.finishWaitLens())
        {
            if(!(checkLutLens(true)&&checkLutNgLens(false)))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setWaitingLens(false);
                    continue;
                }
                if(!is_run)break;
            }
            qInfo("LUT Module has lens");
            bool action_result;
            isLocalHost ?action_result = moveToAA1ReadyPos():action_result = moveToAA2ReadyPos();
            if(!action_result)
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            states.setWaitingLens(false);
            states.setLutHasLens(true);
            states.setLutLoadReady(true);
            states.setLutNgLensID(-1);
            states.setLutNgTrayID(-1);
            states.setLutHasNgLens(false);
        }
        //AA头上lens
        if(states.lutHasLens()&&(!states.aaPickedLens()))
        {
            has_task = true;
            if(!checkLutLens(true))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    state = NO_LENS;
                    break;
                }
                if(!is_run)break;
            }
            bool action_result;
            isLocalHost ?action_result = moveToAA1PickLens() : action_result = moveToAA2PickLens();
            if((!action_result))
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            isLocalHost?states.setAa1LensID(states.lutLensID()):states.setAa2LensID(states.lutLensID());
            isLocalHost?states.setAa1TrayID(states.lutTrayID()):states.setAa2TrayID(states.lutTrayID());
            states.setAaPickedLens(true);
            states.setNeedUplookPr(true);
        }
        //uplookpr
        if(states.aaPickedLens()&&states.needUplookPr())
        {
            bool action_result;
            if(parameters.staticTest())
                current_time = parameters.repeatTime();
            while (current_time>0)
            {
                isLocalHost ?action_result = moveToAA1UplookPR(pr_offset) : action_result = moveToAA2UplookPR(pr_offset);
                if((!action_result))
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                    if(waitMessageReturn(is_run))
                    {
                        continue;
                    }
                    if(!is_run)break;
                }
                recordAALensPr(getUuid(finished,current_count,current_time));
                if(!parameters.staticTest())
                    break;
                current_time--;
            }
            if(!is_run)break;
            states.setNeedUplookPr(false);
        }
        //AA头卸Lens
        if(states.aaPickedLens()&&(!states.needUplookPr())&&(!states.lutHasNgLens()))
        {
            has_task = true;
            if(!checkLutNgLens(false))
            {
                AppendError("请人工拿走多LUT上NG位置多余物料并检测真空状态！");
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                waitMessageReturn(is_run);
                if(!is_run)break;
            }
            bool action_result;
            isLocalHost ?action_result = moveToAA1UnPickLens() : action_result = moveToAA2UnPickLens();
            if(!action_result)
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            else
            {
                states.setLutNgLensID(isLocalHost?states.aa1LensID():states.aa2LensID());
                states.setLutNgTrayID(isLocalHost?states.aa1TrayID():states.aa2TrayID());
                states.setAaPickedLens(false);
                states.setLutHasNgLens(true);
                states.setLutLoadReady(false);
                if(parameters.staticTest())
                {
                    current_count--;
                    if(current_count<=0)
                    {
                        if(finished)
                            break;
                        else
                        {
                            finished  = true;
                            isLocalHost = !isLocalHost;
                            current_count = parameters.testLensCount();
                        }
                    }
                }
                else
                {
                    current_count--;
                    if(current_count<=0)
                    {
                        current_count = parameters.testLensCount();
                        current_time--;
                        if(current_time<=0)
                        {
                            if(finished)
                            {
                                break;
                            }
                            else
                            {
                                isLocalHost = !isLocalHost;
                                current_time = parameters.repeatTime();
                            }
                        }
                    }
                }
            }
        }
    }
    qInfo("LUT Module end of thread");
}

bool LutModule::isActionEmpty()
{
    QMutexLocker locker(&tcp_mutex);
    return  actionQueue.isEmpty();
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
    this->has_material = true;
    qInfo("Lut Module start work in run mode: %d", run_mode);
    if(run_mode == RunMode::Normal||run_mode == RunMode::OnllyLeftAA||run_mode == RunMode::OnlyRightAA)
        run(true);
    else if(run_mode == RunMode::NoMaterial)
    {
        this->has_material = false;
        run(false);
    }
    else if(run_mode == RunMode::MachineTest)
    {
        runTest();
    }
}

void LutModule::stopWork(bool wait_finish)
{
    qInfo("Lut Module stop work");
    is_run = false;
    return;
}

void LutModule::performHandlingOperation(int cmd)
{
    if(is_run)return;
    qInfo("Lut Module perform command: %d", cmd);
    bool result = true;
    if(cmd == 1)
        result =  moveToAA1UplookPR(false,true);
    else if(cmd == 2)
        result =  moveToAA2UplookPR(false,true);
    else if(cmd == 3)
        result =  moveToAA1UnPickLens(true);
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
    states.reset();
    states.setLutTrayID(-1);
    states.setLutLensID(-1);
    states.setLutNgTrayID(-1);
    states.setLutNgLensID(-1);
    states.setAa1TrayID(-1);
    states.setAa1LensID(-1);
    states.setAa2TrayID(-1);
    states.setAa2LensID(-1);
    states.setWaitingLens(false);
    states.setServingIP("");
    states.setLutHasLens(false);
    states.setPickingLens(false);
    states.setCmd("");
    states.setUnpickedNgLens(false);
    states.setPickedLens(false);
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
    temp_map.insert("lens_uph", &lens_uph);
    temp_map.insert("left_lens_uph", &left_lens_uph);
    temp_map.insert("right_lens_uph", &right_lens_uph);
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
    temp_map.insert("lens_uph", &lens_uph);
    temp_map.insert("left_lens_uph", &left_lens_uph);
    temp_map.insert("right_lens_uph", &right_lens_uph);
    PropertyBase::loadJsonConfig(file_name, temp_map);
}

bool LutModule::moveToAA1UplookPos(bool check_autochthonous)
{
    qInfo("moveToAA1UplookPos");
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_uplook_position.X(),aa1_uplook_position.Y(),aa1_uplook_position.Z(),check_autochthonous);
}

bool LutModule::moveToAA1UplookPR(PrOffset &offset, bool close_lighting,bool check_autochthonous)
{
    qInfo("moveToAA1UplookPR");
    uplook_location->OpenLight();
    bool result = moveToAA1UplookPos(check_autochthonous);
    if(result)
    {
        if(has_material)
            result = uplook_location->performPR(offset);
    }
    if(close_lighting)
        uplook_location->CloseLight();
    return result;
}

bool LutModule::moveToAA1UplookPR(bool close_lighting, bool check_autochthonous)
{
    qInfo("moveToAA1UplookPR");
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
    qInfo("moveToAA2UplookPos");
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_uplook_position.X(),aa2_uplook_position.Y(),aa2_uplook_position.Z(),check_autochthonous);
}

bool LutModule::moveToAA2UplookPR(PrOffset &offset, bool close_lighting,bool check_autochthonous)
{
    qInfo("moveToAA2UplookPR");
    uplook_location->OpenLight();
    bool result = moveToAA2UplookPos(check_autochthonous);
    if(result)
    {
        if(has_material)
            result = uplook_location->performPR(offset);
    }
    if(close_lighting)
        uplook_location->CloseLight();
    return result;
}

bool LutModule::moveToAA2UplookPR(bool close_lighting, bool check_autochthonous)
{
    qInfo("moveToAA2UplookPR");
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
    bool result = carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_picklens_position.X(),aa1_picklens_position.Y(),aa1_picklens_position.Z() - parameters.lensHeight(),check_autochthonous);
    if(result)
    {
        if(parameters.enablePickForce())
        {
            qInfo("moveToAA1PickLens Start ZSerchByForce");
            result = carrier->ZSerchByForce(parameters.pickSpeed(),parameters.pickForce(),-1,0,load_vacuum);
            qInfo("moveToAA1PickLens Finish ZSerchByForce");
        }
        sendCmd("::1","gripperOffReq");
        Sleep(parameters.gripperDelay());
        load_vacuum->Set(false);
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
    return carrier->Move_SZ_SY_X_YS_Z_Sync(aa1_picklens_position.X(),aa1_picklens_position.Y(),aa1_picklens_position.Z() - parameters.lensHeight(),check_autochthonous);
    //}else{
    //    return true;
    //}
}

bool LutModule::moveToAAMeasurePickHeight(bool ishost, bool check_autochthonous)
{
    bool result = true;
    if(ishost)
        result &= carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_picklens_position.X(),aa1_picklens_position.Y(),0,check_autochthonous);
    else
        result &=  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_picklens_position.X(),aa2_picklens_position.Y(),0,check_autochthonous);

    if(result)
        result &= carrier->motor_z-> SearchPosByForce(parameters.pickSpeed(),parameters.pickForce());
    return result;
}

bool LutModule::moveToAA1UnPickLens(bool check_autochthonous)
{
    bool result = carrier->Move_SZ_SY_X_YS_Z_Sync(aa1_unpicklens_position.X(),aa1_unpicklens_position.Y(),aa1_unpicklens_position.Z(),check_autochthonous);
    if(result)
    {
        qInfo("moveToAA1UnPickLens Start ZSerchByForce");
        result = carrier->ZSerchByForce(10,parameters.pickForce(),-1,1,unload_vacuum);
        qInfo("moveToAA1UnPickLens Start ZSerchByForce");
        //        if(result)
        //        {
        sendCmd("::1","gripperOnReq");
        this->unload_vacuum->Set(true);
        Sleep(parameters.gripperDelay());
        //        }
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToAA2PickLensPos(bool check_autochthonous)
{
    //    if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
    //        return true;
    //    }
    return carrier->Move_SZ_SY_X_YS_Z_Sync(aa2_picklens_position.X(),aa2_picklens_position.Y(),aa2_picklens_position.Z() - parameters.lensHeight(),check_autochthonous);
}

bool LutModule::moveToAA2PickLens(bool need_return, bool check_autochthonous)
{
    qInfo("moveToAA2PickLens");
    sendCmd("remote","gripperOnReq");
    bool result = carrier->Move_SZ_SY_X_YS_Z_Sync(aa2_picklens_position.X(),aa2_picklens_position.Y(),aa2_picklens_position.Z() - parameters.lensHeight(),check_autochthonous);
    if(result)
    {
        if(parameters.enablePickForce())
        {
            qInfo("moveToAA1PickLens Start ZSerchByForce");
            result = carrier->ZSerchByForce(parameters.pickSpeed(),parameters.pickForce(),-1,0,load_vacuum);
            qInfo("moveToAA1PickLens Finish ZSerchByForce");
        }
        sendCmd("remote","gripperOffReq");
        Sleep(parameters.gripperDelay());
        load_vacuum->Set(false);
        if(need_return)
            result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToAA2UnPickLens(bool check_autochthonous)
{
    qInfo("moveToAA2UnPickLens Start to aa2 unpickLens");
    bool result = carrier->Move_SZ_SY_X_YS_Z_Sync(aa2_unpicklens_position.X(),aa2_unpicklens_position.Y(),aa2_unpicklens_position.Z(),check_autochthonous);
    if(result)
    {
        qInfo("moveToAA2UnPickLens Start ZSerchByForce");
        result = carrier->ZSerchByForce(10,parameters.pickForce(),-1,0,load_vacuum);
        qInfo("moveToAA2UnPickLens Finish ZSerchByForce");
        //        if(result)
        //        {
        sendCmd("remote","gripperOnReq");
        this->unload_vacuum->Set(true);
        Sleep(parameters.gripperDelay());
        //        }
        result &= carrier->motor_z->resetSoftLanding();
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

bool LutModule::moveToAA1ReturnPos(bool check_autochthonous)
{
    qInfo("moveToAA1readyPos(%f,%f,%f)",aa1_uplook_position.X(),0,0);
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_uplook_position.X(),0,0,check_autochthonous);
}

bool LutModule::moveToAA2ReturnPos(bool check_autochthonous)
{
    qInfo("moveToAA2readyPos(%f,%f,%f)",aa2_uplook_position.X(),0,0);
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_uplook_position.X(),0,0,check_autochthonous);
}

bool LutModule::moveToAA1ReadyPos(bool check_autochthonous)
{
    qInfo("moveToAA1readyPos(%f,%f,%f)",aa1_unpicklens_position.X(),0,0);
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_unpicklens_position.X(),0,0,check_autochthonous);
}

bool LutModule::moveToAA2ReadyPos(bool check_autochthonous)
{
    qInfo("moveToAA2readyPos(%f,%f,%f)",aa2_unpicklens_position.X(),0,0);
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_unpicklens_position.X(),0,0,check_autochthonous);
}

bool LutModule::checkLutLens(bool check_state)
{
    if(!has_material)
        return true;
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
    if(!has_material)
        return true;
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

double LutModule::updateAccumulatedHour(bool calculate)
{
    if(calculate)
    {
        parameters.setAccumulatedHour(parameters.accumulatedHour() + getHourSpace(time_label));
        time_label = QTime::currentTime();
    }
    return parameters.accumulatedHour();
}

double LutModule::getHourSpace(QTime time_label)
{
    int temp_minute = QTime::currentTime().minute() - time_label.minute();
    if(temp_minute < 0)temp_minute = 60 - temp_minute;
    double space = temp_minute/60.0;
    int temp_second =   QTime::currentTime().second() - time_label.second();
    space += temp_second/3600.0;
    return space;
}

void LutModule::clearNumber()
{
    lens_uph.clearNumber();
    left_lens_uph.clearNumber();
    right_lens_uph.clearNumber();
    parameters.setAccumulatedHour(0);
}

QString LutModule::getUuid(bool is_right, int current_count, int current_time)
{
    QString uuid = "";
    if(is_right)
        uuid.append("right_");
    else
        uuid.append("left_");
    uuid.append(parameters.testLensCount() - current_count + 1);
    uuid.append("_");
    uuid.append(parameters.repeatTime() - current_time + 1);
    return uuid;
}

void LutModule::recordAALensPr(QString uuid)
{
    QString lensId = QString::number(states.lutLensID());
    QVariantMap temp_map;
    temp_map.insert("aa_offset.x",pr_offset.X);
    temp_map.insert("aa_offset.y",pr_offset.Y);
    temp_map.insert("aa_offset.th",pr_offset.Theta);

    postCSVDataToUnit(uuid,temp_map);
}

void LutModule::receiveToolUpPRRequest(PrOffset &offset)
{
    sut->toolUplookPR(offset, true, false);
}

