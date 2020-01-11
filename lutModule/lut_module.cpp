#include "lutModule/lut_module.h"
#include "utils/commonutils.h"
#include "materialtray.h"
#include <tcpmessager.h>

enum BusyState
{
    IDLE,
    STATION1,
    STATION2
};
LutModule::LutModule(QString name, QObject *parent):ThreadWorkerBase (name,parent)
{
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
        QThread::msleep(10);
        //分配任务
        if(states.busyState() == BusyState::IDLE)
        {
            if(states.station1HasRequest())
            {
                states.setBusyState(BusyState::STATION1);
                states.setLastState(states.busyState());
            }
            else if(states.station2HasRequest())
            {
                states.setBusyState(BusyState::STATION2);
                states.setLastState(states.busyState());
            }
        }
        //AA1卸NG料
        if((!states.waitingLens())&&(states.busyState() == BusyState::STATION1)&&(states.aa1HeadMaterialState() == MaterialState::IsNgLens)&&(!states.lutHasNgLens()))
        {
            if(!moveToAA1UnPickLens())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setLutHasNgLens(true);
            states.setAa1HeadMaterialState(MaterialState::IsEmpty);
            states.copyInNgLensData(states.aa1LensData());
        }
        //AA2卸NG料
        if((!states.waitingLens())&&(states.busyState() == BusyState::STATION2)&&(states.aa2HeadMaterialState() == MaterialState::IsNgLens)&&(!states.lutHasNgLens()))
        {
            if(!moveToAA2UnPickLens())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setLutHasNgLens(true);
            states.setAa2HeadMaterialState(MaterialState::IsEmpty);
            states.copyInNgLensData(states.aa2LensData());
        }
        //AA1上料
        if((!states.waitingLens())&&(states.busyState() == BusyState::STATION1)&&(states.aa1HeadMaterialState() == MaterialState::IsEmpty)&&states.station1NeedLens()&&states.lutHasLens())
        {
            if(!checkLutLensSync(true))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setLutHasLens(false);
                    break;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }
            if(!moveToAA1PickLens())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setLutHasLens(false);
            states.setStation1NeedLens(false);
            states.setAa1HeadMaterialState(MaterialState::IsRawLens);
            states.copyInAa1LensData(states.lensData());
        }
        //AA2上料
        if((!states.waitingLens())&&(states.busyState() == BusyState::STATION2)&&(states.aa2HeadMaterialState() == MaterialState::IsEmpty)&&states.station2NeedLens()&&states.lutHasLens())
        {
            if(!checkLutLensSync(true))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setLutHasLens(false);
                    break;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }
            if(!moveToAA2PickLens())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setLutHasLens(false);
            states.setStation2NeedLens(false);
            states.setAa2HeadMaterialState(MaterialState::IsRawLens);
            states.copyInAa2LensData(states.lensData());
        }
        //AA1视觉
        if((!states.waitingLens())&&states.station1HasRequest()&&(!states.station1NeedLens())&&(states.aa1HeadMaterialState() != MaterialState::IsNgLens))
        {
            if((states.aa1HeadMaterialState() == MaterialState::IsRawLens)&&(!moveToAA1UplookPR()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_AUTOREJECT_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(AUTOREJECT_OPERATION == operation)
                {
                    states.setAa1HeadMaterialState(MaterialState::IsNgLens);
                    states.setStation1NeedLens(true);
                    continue;
                }
                else if(REJECT_OPERATION == operation)
                {
                    states.setAa1HeadMaterialState(MaterialState::IsEmpty);
                    states.setStation1NeedLens(true);
                    continue;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setStation1HasRequest(false);
            states.setBusyState(BusyState::IDLE);
            QJsonObject param;
            param.insert("AAMaterialState",MaterialTray::getMaterialStateName(states.aa1HeadMaterialState()));
            param.insert("MaterialData",QJsonObject::fromVariantMap(states.aa1LensData()));
            sendMessageToModule("AA1CoreNew","FinishLoadLens",param);
        }
        //AA2视觉
        if((!states.waitingLens())&&states.station2HasRequest()&&(!states.station2NeedLens())&&(states.aa2HeadMaterialState() != MaterialState::IsNgLens))
        {
            if((states.aa2HeadMaterialState() == MaterialState::IsRawLens)&&(!moveToAA2UplookPR()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_AUTOREJECT_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(AUTOREJECT_OPERATION == operation)
                {
                    states.setAa2HeadMaterialState(MaterialState::IsNgLens);
                    states.setStation2NeedLens(true);
                    continue;
                }
                else if(REJECT_OPERATION == operation)
                {
                    states.setAa2HeadMaterialState(MaterialState::IsEmpty);
                    states.setStation2NeedLens(true);
                    continue;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setStation2HasRequest(false);
            states.setBusyState(BusyState::IDLE);
            QJsonObject param;
            param.insert("AAMaterialState",MaterialTray::getMaterialStateName(states.aa2HeadMaterialState()));
            param.insert("MaterialData",QJsonObject::fromVariantMap(states.aa2LensData()));
            sendMessageToModule("AA2CoreNew","FinishLoadLens",param);
        }
        //无任务无料去等料位置
        if((!states.waitingLens())&&(!states.lutHasLens())&&(!checkNeedLens())&&(!states.lutHasNgLens())&&(!states.waitingTask()))
        {
            if(!moveToLoadPosAndCheckMaterial(states.lutHasLens(),states.lutHasNgLens(),true))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setWaitingTask(true);
        }
        //请求上下料
        if((!states.waitingLens())&&(states.lutHasNgLens()||checkNeedLens()))
        {
            if(states.station1Unload()&&states.station2Unload())
                sendMessageToModule("LensLoaderModule","UnloadMode");
            if(!moveToLoadPosAndCheckMaterial(states.lutHasLens(),states.lutHasNgLens(),true))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            qInfo("LUT Module is waiting lens");
            QMutexLocker temp_locker(&loader_mutext);
            QJsonObject param;
            if(states.lutHasNgLens())
                param.insert("LutNgLensData",QJsonObject::fromVariantMap(states.ngLensData()));
            param.insert("LutHasLens",states.lutHasLens());
            param.insert("NeedLoadLens",checkNeedLens());
            int temp_number = 0;
            if (!states.disableStation1())
                temp_number += states.taskOfStation1();
            if (!states.disableStation2())
                temp_number += states.taskOfStation2();
            if((!states.disableStation1())&&(!states.station1NeedLens()))
                temp_number--;
            if((!states.disableStation2())&&(!states.station2NeedLens()))
                temp_number--;
            if(temp_number > 0)
                param.insert("TaskNumber",temp_number);
            sendMessageToModule("LensLoaderModule","LoadLensRequest",param);
            states.setWaitingLens(true);
            states.setWaitingTask(false);
        }
        //等待Lens
        if(states.waitingLens()&&states.finishWaitLens())
        {
            if(!checkLutLensSync(states.lutHasLens()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                {
                    continue;
                }
            }
            if(!checkLutNgLensSync(states.lutHasNgLens()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                {
                    continue;
                }
            }

            bool action_result;

            if((states.busyState() == BusyState::STATION2)
                    ||((!states.disableStation2())&&(states.busyState() == BusyState::IDLE)&&(states.lastState() == BusyState::STATION1))
                    ||states.disableStation1())
                action_result = moveToAA2ReadyPos();
            else
                action_result = action_result = moveToAA1ReadyPos();
            if(!action_result)
            {
                AppendError("move to ready pos fail");
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setWaitingLens(false);
            states.setFinishWaitLens(false);
        }
    }
    states.setRunMode(RunMode::Normal);
    qInfo("LUT Module end of thread");
}

//void LutModule::runTest()
//{
//    qInfo("Start Lut Module Thread");
//    is_run = true;
//    bool has_task = true;
//    int current_time = parameters.repeatTime();
//    int current_count = parameters.testLensCount();
//    bool finished = false;
//    isLocalHost = true;
//    while(is_run){
////        if(!has_task)
//            QThread::msleep(1);
//        has_task = false;
//        //取lens请求
//        if((!states.waitingLens())&&(!states.lutHasLens())&&(!states.lutLoadReady()))
//        {
//            has_task = true;
//            if(!moveToLoadPosAndCheckMaterial(false,states.lutNgLensID()>=0,true))
//            {
//                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
//                waitMessageReturn(is_run);
//                if(!is_run)break;
//            }
//            qInfo("LUT Module is waiting lens");
//            emit sendLoadLensRequst(true,states.lutNgLensID(),states.lutNgTrayID());
//            states.setWaitingLens(true);
//        }
//        //等待lens
//        if(states.waitingLens()&&states.finishWaitLens())
//        {
//            if(!(checkLutLensSync(true)&&checkLutNgLensSync(false)))
//            {
//                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
//                if(waitMessageReturn(is_run))
//                {
//                    states.setWaitingLens(false);
//                    continue;
//                }
//                if(!is_run)break;
//            }
//            qInfo("LUT Module has lens");
//            bool action_result;
//            isLocalHost ?action_result = moveToAA1ReadyPos():action_result = moveToAA2ReadyPos();
//            if(!action_result)
//            {
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            states.setWaitingLens(false);
//            states.setFinishWaitLens(false);
//            states.setLutHasLens(true);
//            states.setLutLoadReady(true);
//            states.setLutNgLensID(-1);
//            states.setLutNgTrayID(-1);
//            states.setLutHasNgLens(false);
//        }
//        //AA头上lens
//        if(states.lutHasLens()&&(!states.aaPickedLens()))
//        {
//            has_task = true;
//            if(!checkLutLensSync(true))
//            {
//                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
//                if(waitMessageReturn(is_run))
//                {
//                    state = NO_LENS;
//                    break;
//                }
//                if(!is_run)break;
//            }
//            bool action_result;
//            isLocalHost ?action_result = moveToAA1PickLens() : action_result = moveToAA2PickLens();
//            if((!action_result))
//            {
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            isLocalHost?states.setAa1LensID(states.lutLensID()):states.setAa2LensID(states.lutLensID());
//            isLocalHost?states.setAa1TrayID(states.lutTrayID()):states.setAa2TrayID(states.lutTrayID());
//            states.setAaPickedLens(true);
//            states.setNeedUplookPr(true);
//        }
//        //uplookpr
//        if(states.aaPickedLens()&&states.needUplookPr())
//        {
//            bool action_result;
//            if(parameters.staticTest())
//                current_time = parameters.repeatTime();
//            while (current_time>0)
//            {
//                isLocalHost ?action_result = moveToAA1UplookPR(pr_offset,false) : action_result = moveToAA2UplookPR(pr_offset,false);
//                if((!action_result))
//                {
//                    sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
//                    if(waitMessageReturn(is_run))
//                    {
//                        continue;
//                    }
//                    if(!is_run)break;
//                }
//                recordAALensPr(getUuid(finished,current_count,current_time));
//                if(!parameters.staticTest())
//                    break;
//                current_time--;
//            }
//            uplook_location->CloseLight();
//            if(!is_run)break;
//            states.setNeedUplookPr(false);
//        }
//        //AA头卸Lens
//        if(states.aaPickedLens()&&(!states.needUplookPr())&&(!states.lutHasNgLens()))
//        {
//            has_task = true;
//            if(!checkLutNgLensSync(false))
//            {
//                AppendError("请人工拿走多LUT上NG位置多余物料并检测真空状态！");
//                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
//                waitMessageReturn(is_run);
//                if(!is_run)break;
//            }
//            bool action_result;
//            isLocalHost ?action_result = moveToAA1UnPickLens() : action_result = moveToAA2UnPickLens();
//            if(!action_result)
//            {
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            else
//            {
//                states.setLutNgLensID(isLocalHost?states.aa1LensID():states.aa2LensID());
//                states.setLutNgTrayID(isLocalHost?states.aa1TrayID():states.aa2TrayID());
//                states.setAaPickedLens(false);
//                states.setLutHasNgLens(true);
//                states.setLutLoadReady(false);
//                states.setLutHasLens(false);
//                if(parameters.staticTest())
//                {
//                    current_count--;
//                    if(current_count<=0)
//                    {
//                        if(finished)
//                            break;
//                        else
//                        {
//                            finished  = true;
//                            isLocalHost = !isLocalHost;
//                            current_count = parameters.testLensCount();
//                        }
//                    }
//                }
//                else
//                {
//                    current_count--;
//                    if(current_count<=0)
//                    {
//                        current_count = parameters.testLensCount();
//                        current_time--;
//                        if(current_time<=0)
//                        {
//                            if(finished)
//                            {
//                                break;
//                            }
//                            else
//                            {
//                                isLocalHost = !isLocalHost;
//                                current_time = parameters.repeatTime();
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//    qInfo("LUT Module end of thread");
//}

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
    if(run_mode == RunMode::AAFlowChartTest) return;
    QVariantMap run_params = inquirRunParameters();
    if(run_params.isEmpty())
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数为空.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(run_params.contains("RunMode"))
    {
        states.setRunMode(run_params["RunMode"].toInt());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数RunMode缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(run_params.contains("DisableStation"))
    {
        QVariantMap disable_map = run_params["DisableStation"].toMap();
        states.setDisableStation1(disable_map["0"].toBool());
        states.setDisableStation2(disable_map["1"].toBool());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数DisableStation缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(run_params.contains("HandlyChangeLens"))
    {
        states.setHandlyChangeLens(run_params["HandlyChangLens"].toBool());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数HandlyChangeLens缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }

    this->has_material = true;
    qInfo("Lut Module start work in run mode: %d", run_mode);
    if(run_mode == RunMode::Normal)
        run(true);
    else if(run_mode == RunMode::NoMaterial)
    {
        this->has_material = false;
        run(false);
    }
//    else if(run_mode == RunMode::MachineTest)
//    {
//        runTest();
//    }
}

void LutModule::stopWork(bool wait_finish)
{
    qInfo("Lut Module stop work");
    is_run = false;
    return;
}

void LutModule::performHandlingOperation(int cmd,QVariant param)
{
    if(is_run)
    {
        is_handling = false;
        return;
    }
    qInfo("Lut Module perform command: %d", cmd);
    bool result = true;
    int temp_value = 100;
    if (cmd%temp_value == HandlePosition::LOAD_POS) {
        result = moveToLoadPos();
    }
    else if(cmd%temp_value == HandlePosition::AA1_UPLOOK_POS) {
        result = moveToAA1UplookPR(false,true,true);
    }
    else if(cmd%temp_value == HandlePosition::AA2_UPLOOK_POS) {
        result = moveToAA2UplookPR(false,true,true);
    }
    else if(cmd%temp_value == HandlePosition::AA1_UNPICK_LENS_POS) {
        result = moveToAA1UnPickLensPos(true,true);
    }
    else if(cmd%temp_value == HandlePosition::AA2_UNPICK_LENS_POS) {
        result = moveToAA2UnPickLensPos(true,true);
    }
    else if(cmd%temp_value == HandlePosition::AA1_PICK_LENS_POS) {
        result = moveToAA1PickLensPos(true,true);
    }
    else if(cmd%temp_value == HandlePosition::AA2_PICK_LENS_POS) {
        result = moveToAA2PickLensPos(true,true);
    } else {
        result = true;
    }
    cmd = cmd/temp_value*temp_value;
    temp_value = 1000;
    if(!result)
    {
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    //Handle PR
    if (cmd%temp_value == HandlePR::AA1_UPLOOK_PR || cmd%temp_value == HandlePR::AA2_UPLOOK_PR) {
        result = performUpLookPR();
    } else {
        result = true;
    }
    if(!result)
    {
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    //Action
    cmd = cmd/temp_value*temp_value;
    temp_value = 10000;
    if (cmd%temp_value == HandlerAction::Measure_AA1_PICK_POS) {
        result = moveToAAMeasurePickHeight(true);
    }
    else if (cmd%temp_value == HandlerAction::Measure_AA2_PICK_POS) {
        result = moveToAAMeasurePickHeight(false);
    }
    else if (cmd%temp_value == HandlerAction::AA1_PICK_LENS) {
        result = moveToAA1PickLens();
    }
    else if (cmd%temp_value == HandlerAction::AA2_PICK_LENS) {
        result = moveToAA2PickLens();
    }
    else if (cmd%temp_value == HandlerAction::AA1_UNPICK_LENS) {
        result = moveToAA1UnPickLens();
    }
    else if (cmd%temp_value == HandlerAction::AA2_UNPICK_LENS) {
        result = moveToAA2UnPickLens();
    }
    else {
        result = true;
    }
    if(!result)
    {
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    is_handling = false;
}

void LutModule::resetLogic()
{
    if(is_run)return;
    states.reset();
}

void LutModule::Init(MaterialCarrier *carrier, VisionLocation* uplook_location,VisionLocation* load_location,VisionLocation* mushroom_location, XtVacuum *load_vacuum, XtVacuum *unload_vacuum,XtGeneralOutput *gripper, SutModule *sut,int check_thread)
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
    this->check_thread = check_thread;
    this->gripper = gripper;
    //Align some parameters name in lut params for the ease of access
    if (load_vacuum)
    {
        this->parameters.setTcpLutVacuum1Name(load_vacuum->parameters.outIoName());
        this->parameters.setTcpLutVacuumSensor1Name(load_vacuum->parameters.inIoName());
    }
    if (unload_vacuum)
    {
        this->parameters.setTcpLutVacuum2Name(unload_vacuum->parameters.outIoName());
        this->parameters.setTcpLutVacuumSensor2Name(unload_vacuum->parameters.inIoName());
    }
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

bool LutModule::moveToAA1UplookPos(bool check_autochthonous,bool check_softlanding)
{
    qInfo("moveToAA1UplookPos");
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_uplook_position.X(),aa1_uplook_position.Y(),aa1_uplook_position.Z(),check_autochthonous, check_softlanding);
}

bool LutModule::moveToAA1UplookPR(PrOffset &offset, bool close_lighting,bool check_autochthonous,bool check_softlanding)
{
    qInfo("moveToAA1UplookPR");
    uplook_location->OpenLight();
    bool result = moveToAA1UplookPos(check_autochthonous, check_softlanding);
    if(result)
    {
       result = performUpLookPR();
       offset = uplook_location->getCurrentResult();
    }
    if(close_lighting)
        uplook_location->CloseLight();
    return result;
}

bool LutModule::moveToAA1UplookPR(bool close_lighting, bool check_autochthonous,bool check_softlanding)
{
    qInfo("moveToAA1UplookPR");
    PrOffset pr_offset;
    if(moveToAA1UplookPR(pr_offset,close_lighting,check_autochthonous, check_softlanding))
    {
        QJsonObject result;
        result.insert("prOffsetX", pr_offset.X);
        result.insert("prOffsetY", pr_offset.Y);
        result.insert("prOffsetT", pr_offset.Theta);
        result.insert("event", "prResp");
        this->sendMessageToModule("AA1CoreNew", "UplookPrResult", result);
        //emit sendMessageToClient("::1", getStringFromJsonObject(result));
        return true;
    }
    AppendError(u8"执行AA1uplook PR失败");
    return false;
}

bool LutModule::moveToAA2UplookPos(bool check_autochthonous,bool check_softlanding)
{
    qInfo("moveToAA2UplookPos");
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_uplook_position.X(),aa2_uplook_position.Y(),aa2_uplook_position.Z(),check_autochthonous,check_softlanding);
}

bool LutModule::moveToAA2UplookPR(PrOffset &offset, bool close_lighting,bool check_autochthonous,bool check_softlanding)
{
    qInfo("moveToAA2UplookPR");
    uplook_location->OpenLight();
    bool result = moveToAA2UplookPos(check_autochthonous,check_softlanding);
    if(result)
    {
        result = performUpLookPR();
        offset = uplook_location->getCurrentResult();
    }
    if(close_lighting)
        uplook_location->CloseLight();
    return result;
}

bool LutModule::moveToAA2UplookPR(bool close_lighting, bool check_autochthonous,bool check_softlanding)
{
    qInfo("moveToAA2UplookPR");
    PrOffset pr_offset;
    if(moveToAA2UplookPR(pr_offset,close_lighting,check_autochthonous,check_softlanding))
    {
        QJsonObject result;
        result.insert("prOffsetX", pr_offset.X);
        result.insert("prOffsetY", pr_offset.Y);
        result.insert("prOffsetT", pr_offset.Theta);
        result.insert("event", "prResp");
        this->sendMessageToModule("AA2CoreNew", "UplookPrResult", result);
        //emit sendMessageToClient("remote", getStringFromJsonObject(result));
        return true;
    }
    AppendError(u8"执行AA2 uplook PR失败");
    return false;
}

bool LutModule::moveToLoadPos(bool check_autochthonous,bool check_softlanding)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(load_position.X(),load_position.Y(),load_position.Z(),check_autochthonous,check_softlanding);
}

bool LutModule::moveToLoadPosAndCheckMaterial(bool has_lens,bool has_ng_lens,bool check_autochthonous,bool check_softlanding)
{
    bool result = load_vacuum->checkHasMateriel(check_thread);
    result &= unload_vacuum->checkHasMateriel(check_thread);
    result &= carrier->Move_SZ_SY_X_Y_Z_Sync(load_position.X(),load_position.Y(),load_position.Z(),check_autochthonous,check_softlanding);
    result &=  waitLutLensCheckResult(has_lens);
    result &=  waitLutNgLensCheckResult(has_ng_lens);
    return result;
}

bool LutModule::moveToLutDownlookloadPos(bool check_autochthonous,bool check_softlanding)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(lut_downlook_load_position.X(),lut_downlook_load_position.Y(),lut_downlook_load_position.Z(),check_autochthonous,check_softlanding);
}

bool LutModule::moveToLutDownlookUnloadPos(bool check_autochthonous,bool check_softlanding)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(lut_downlook_unload_position.X(),lut_downlook_unload_position.Y(),lut_downlook_unload_position.Z(),check_autochthonous,check_softlanding);
}

bool LutModule::moveToLoadUplookPos(bool check_autochthonous,bool check_softlanding)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(load_uplook_position.X(),load_uplook_position.Y(),load_uplook_position.Z(),check_autochthonous,check_softlanding);
}

bool LutModule::moveToLoadUplookPR(bool check_autochthonous,bool check_softlanding)
{
    return moveToLoadUplookPos(check_autochthonous,check_softlanding);
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

bool LutModule::moveToAA1PickLens(bool need_return,bool check_autochthonous,bool check_softlanding)
{
    openAA1Griper();
    bool result = true;
    if(parameters.enablePickForce())
    {
        result = carrier->Move_SZ_SY_X_YS_Z_Sync(aa1_picklens_position.X(),aa1_picklens_position.Y(),carrier->parameters.SafetyZ(),check_autochthonous,check_softlanding);
    }
    else
    {
        result = carrier->Move_SZ_SY_X_YS_Z_Sync(aa1_picklens_position.X(),aa1_picklens_position.Y(),aa1_picklens_position.Z() - parameters.lensHeight(),check_autochthonous,check_softlanding);
    }
    if(result)
    {
        if(parameters.enablePickForce())
        {
            qInfo("moveToAA1PickLens Start ZSerchByForce");
            result = carrier->motor_z->SearchPosByForce(parameters.pickSpeed(),parameters.pickForce(),aa1_picklens_position.Z(),parameters.lensHeight());
            qInfo("moveToAA1PickLens Finish ZSerchByForce");
        }
        closeAA1Griper();
        result &= closeLoadVacuum();
        Sleep(parameters.gripperDelay());
        if(need_return)
            result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::vcmReturn()
{
    return  carrier->motor_z->resetSoftLanding();
}

bool LutModule::moveToAA1PickLensPos(bool check_autochthonous,bool check_softlanding)
{
    //if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
    return carrier->Move_SZ_SY_X_YS_Z_Sync(aa1_picklens_position.X(),aa1_picklens_position.Y(),aa1_picklens_position.Z() - parameters.lensHeight(),check_autochthonous,check_softlanding);
    //}else{
    //    return true;
    //}
}

bool LutModule::moveToAAMeasurePickHeight(bool ishost, bool check_autochthonous,bool check_softlanding)
{
    if(ishost)
        openAA1Griper();
    else
        openAA2Griper();
    bool result = true;
    if(ishost)
        result &= carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_picklens_position.X(),aa1_picklens_position.Y(),0,check_autochthonous,check_softlanding);
    else
        result &=  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_picklens_position.X(),aa2_picklens_position.Y(),0,check_autochthonous,check_softlanding);

    if(result)
        result &= carrier->motor_z->SearchPosByForce(parameters.pickSpeed(),parameters.pickForce());
    return result;
}

bool LutModule::moveToAA1UnPickLensPos(bool check_autochthonous, bool check_softlanding)
{
    return carrier->Move_SZ_SY_X_YS_Z_Sync(aa1_unpicklens_position.X(),aa1_unpicklens_position.Y(),carrier->parameters.SafetyZ(),check_autochthonous,check_softlanding);
}

bool LutModule::moveToAA1UnPickLens(bool check_autochthonous,bool check_softlanding)
{
    bool result = carrier->Move_SZ_SY_X_YS_Z_Sync(aa1_unpicklens_position.X(),aa1_unpicklens_position.Y(),carrier->parameters.SafetyZ(),check_autochthonous,check_softlanding);
    if(result)
    {
        result = carrier->motor_z->SearchPosByForce(parameters.pickSpeed(),parameters.pickForce(),aa1_picklens_position.Z(),parameters.lensHeight());
        openAA1Griper();
        result &= openUnloadVacuum();
        Sleep(parameters.gripperDelay());
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToAA2PickLensPos(bool check_autochthonous,bool check_softlanding)
{
    return carrier->Move_SZ_SY_X_YS_Z_Sync(aa2_picklens_position.X(),aa2_picklens_position.Y(),aa2_picklens_position.Z() - parameters.lensHeight(),check_autochthonous,check_softlanding);
}

bool LutModule::moveToAA2PickLens(bool need_return, bool check_autochthonous,bool check_softlanding)
{
    qInfo("moveToAA2PickLens");
    openAA2Griper();
    bool result = true;
    if(parameters.enablePickForce())
    {
        result = carrier->Move_SZ_SY_X_YS_Z_Sync(aa2_picklens_position.X(),aa2_picklens_position.Y(),carrier->parameters.SafetyZ(),check_autochthonous,check_softlanding);
    }
    else
    {
        result = carrier->Move_SZ_SY_X_YS_Z_Sync(aa2_picklens_position.X(),aa2_picklens_position.Y(),aa2_picklens_position.Z() - parameters.lensHeight(),check_autochthonous,check_softlanding);
    }
        if(result)
    {
        if(parameters.enablePickForce())
        {
            qInfo("moveToAA2PickLens Start ZSerchByForce");
//            result = carrier->ZSerchByForce(parameters.pickSpeed(),parameters.pickForce(),-1,0,load_vacuum);
            result = carrier->motor_z->SearchPosByForce(parameters.pickSpeed(),parameters.pickForce(),aa2_picklens_position.Z(),parameters.lensHeight());
            qInfo("moveToAA2PickLens Finish ZSerchByForce");
        }
        closeAA2Griper();
        result &= closeLoadVacuum();
        Sleep(parameters.gripperDelay());
        if(need_return)
            result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::moveToAA2UnPickLensPos(bool check_autochthonous, bool check_softlanding)
{
    return carrier->Move_SZ_SY_X_YS_Z_Sync(aa2_unpicklens_position.X(),aa2_unpicklens_position.Y(),carrier->parameters.SafetyZ(),check_autochthonous,check_softlanding);
}

bool LutModule::moveToAA2UnPickLens(bool check_autochthonous,bool check_softlanding)
{
    qInfo("moveToAA2UnPickLens Start to aa2 unpickLens");
    bool result = carrier->Move_SZ_SY_X_YS_Z_Sync(aa2_unpicklens_position.X(),aa2_unpicklens_position.Y(),carrier->parameters.SafetyZ(),check_autochthonous,check_softlanding);
    if(result)
    {
        qInfo("moveToAA2UnPickLens Start ZSerchByForce");
//        result = carrier->ZSerchByForce(10,parameters.pickForce(),-1,0,load_vacuum);
        result = carrier->motor_z->SearchPosByForce(parameters.pickSpeed(),parameters.pickForce(),aa2_picklens_position.Z(),parameters.lensHeight());
        qInfo("moveToAA2UnPickLens Finish ZSerchByForce");
        openAA2Griper();
        result &= openUnloadVacuum();
        Sleep(parameters.gripperDelay());
        result &= carrier->motor_z->resetSoftLanding();
    }
    return result;
}

bool LutModule::moveToAA1MushroomLens(bool check_autochthonous,bool check_softlanding)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_mushroom_position.X(),aa1_mushroom_position.Y(),aa1_mushroom_position.Z(),check_autochthonous,check_softlanding);
}

bool LutModule::moveToAA2MushroomLens(bool check_autochthonous,bool check_softlanding)
{
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_mushroom_position.X(),aa2_mushroom_position.Y(),aa2_mushroom_position.Z(),check_autochthonous,check_softlanding);
}

bool LutModule::moveToAA1ReturnPos(bool check_autochthonous,bool check_softlanding)
{
    qInfo("moveToAA1readyPos(%f,%f,%f)",aa1_uplook_position.X(),0,0);
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_uplook_position.X(),0,0,check_autochthonous,check_softlanding);
}

bool LutModule::moveToAA2ReturnPos(bool check_autochthonous,bool check_softlanding)
{
    qInfo("moveToAA2readyPos(%f,%f,%f)",aa2_uplook_position.X(),0,0);
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_uplook_position.X(),0,0,check_autochthonous,check_softlanding);
}

bool LutModule::moveToAA1ReadyPos(bool check_autochthonous,bool check_softlanding)
{
    qInfo("moveToAA1readyPos(%f,%f,%f)",aa1_unpicklens_position.X(),0,0);
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa1_unpicklens_position.X(),0,0,check_autochthonous,check_softlanding);
}

bool LutModule::moveToAA2ReadyPos(bool check_autochthonous,bool check_softlanding)
{
    qInfo("moveToAA2readyPos(%f,%f,%f)",aa2_unpicklens_position.X(),0,0);
    return  carrier->Move_SZ_SY_X_Y_Z_Sync(aa2_unpicklens_position.X(),0,0,check_autochthonous,check_softlanding);
}

bool LutModule::checkLutLensSync(bool check_state)
{
    if(!has_material)
        return true;
    bool result = load_vacuum->checkHasMaterielSync();
    if(result == check_state||states.runMode() == RunMode::NoMaterial)
        return true;
    QString error = QString(u8"LUT放Lens位置上逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool LutModule::checkLutNgLensSync(bool check_state)
{
    if(!has_material)
        return true;
    bool result = unload_vacuum->checkHasMaterielSync();
    if(result == check_state||states.runMode() == RunMode::NoMaterial)
        return true;
    QString error = QString(u8"LUT放NgLens位置逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool LutModule::waitLutLensCheckResult(bool check_state)
{
    bool result = load_vacuum->getHasMateriel(check_thread);
    if(result == check_state||states.runMode() == RunMode::NoMaterial)
        return true;
    QString error = QString(u8"LUT放Lens位置上逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool LutModule::waitLutNgLensCheckResult(bool check_state)
{
    bool result = unload_vacuum->getHasMateriel(check_thread);
    if(result == check_state||states.runMode() == RunMode::NoMaterial)
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

bool LutModule::performUpLookPR()
{
    qInfo("performUpLookPR");
    bool result;
    if(states.runMode() == RunMode::NoMaterial)
        result = uplook_location->performNoMaterialPR();
    else
        result= uplook_location->performPR();
    if(!result)
        AppendError(QString(u8"执行uplook视觉失败!"));
    return  result;
}

bool LutModule::openLoadVacuum()
{
    if(states.runMode() == RunMode::NoMaterial)
        return load_vacuum->SetSimulation(true);
    else
        return load_vacuum->Set(true);
}

bool LutModule::closeLoadVacuum()
{
    if(states.runMode() == RunMode::NoMaterial)
        return load_vacuum->SetSimulation(false);
    else
        return load_vacuum->Set(false);
}

bool LutModule::openUnloadVacuum()
{
    if(states.runMode() == RunMode::NoMaterial)
        return unload_vacuum->SetSimulation(true);
    else
        return unload_vacuum->Set(true);
}

bool LutModule::closeUnloadVacuum()
{
    if(states.runMode() == RunMode::NoMaterial)
        return unload_vacuum->SetSimulation(false);
    else
        return unload_vacuum->Set(false);
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

bool LutModule::checkNeedLens()
{
    if(states.lutHasLens())
        return false;
    if(states.disableStation1()&&(states.taskOfStation2() == 1)&&(!states.station2NeedLens()))
        return false;
    if(states.disableStation2()&&(states.taskOfStation1() == 1)&&(!states.station1NeedLens()))
        return false;
    if((states.taskOfStation1() == 1)&&(!states.station1NeedLens())&&(states.taskOfStation2() == 1)&&(!states.station2NeedLens()))
        return false;
    return true;
}

void LutModule::openAA1Griper()
{
    gripper->Set(true);
    states.setGriperOperationResult(0);
    //sendMessageToModule("LogicManager1","OpenGripper");
}

void LutModule::closeAA1Griper()
{
    gripper->Set(false);
    states.setGriperOperationResult(0);
    //sendMessageToModule("LogicManager1","CloseGripper");
}

void LutModule::openAA2Griper()
{
    states.setGriperOperationResult(0);
    sendMessageToModule("LogicManager2","OpenGripper");
}

void LutModule::closeAA2Griper()
{

    states.setGriperOperationResult(0);
    sendMessageToModule("LogicManager2","CloseGripper");
}

bool LutModule::waitGripermFinish()
{
    int current_time = 0;
    while (current_time < parameters.griperOperationOutTime())
    {
        if(states.griperOperationResult() != 0)
        {
            qInfo("waitSutVacuumFinish time %d",current_time);
            return (states.griperOperationResult() > 0||states.runMode()== RunMode::NoMaterial);
        }
        current_time +=10;
        QThread::msleep(10);
    }
    qInfo("waitGripermFinish fail ,out time %d",parameters.griperOperationOutTime());
    return false;
}

QString LutModule::getUuid(bool is_right, int current_count, int current_time)
{
    QString uuid = "";
    if(is_right)
        uuid.append("right_");
    else
        uuid.append("left_");
    uuid.append(QString::number(parameters.testLensCount() - current_count + 1));
    uuid.append("_");
    uuid.append(QString::number(parameters.repeatTime() - current_time + 1));
    return uuid;
}

void LutModule::recordAALensPr(QString uuid)
{
    qInfo("recordAALensPr");
//    QString lensId = QString::number(states.lutLensID());
    QVariantMap temp_map;
    temp_map.insert("4_uplook_offset.x",pr_offset.X);
    temp_map.insert("4_uplook_offset.y",pr_offset.Y);
    temp_map.insert("4_uplook_offset.t",pr_offset.Theta);

    postCSVDataToUnit(uuid,temp_map);
}

void LutModule::receivceModuleMessage(QVariantMap message)
{
    qInfo("receive module message %s",TcpMessager::getStringFromQvariantMap(message).toStdString().c_str());
    if(!message.contains("OriginModule"))
    {
        qInfo("message error! has no OriginModule.");
        return;
    }
    QString origin_module = message["OriginModule"].toString();

    if(message["OriginModule"].toString().contains("LogicManager"))
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString().contains("GripperSuccess"))
            {
                states.setGriperOperationResult(1);
            }
            else if(message["Message"].toString().contains("GripperFail"))
            {
                states.setGriperOperationResult(-1);
            }
        }
    }
    else if(origin_module == "AA1CoreNew")
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString() == "LoadLensRequest")
            {
                if(message.contains("MaterialState"))
                    states.setAa1HeadMaterialState(MaterialTray::getMaterialStateFromName(message["MaterialState"].toString()));
                else
                    qInfo("message MaterialState miss");
                if(message.contains("TaskNumber"))
                    states.setTaskOfStation1(message["TaskNumber"].toInt());
                if(!states.station1Unload())
                    states.setStation1NeedLens(true);
                states.setStation1HasRequest(true);
            }
            else if(message["Message"].toString()=="UnloadMode")
            {
                states.setStation1Unload(true);
            }
            else if(message["Message"].toString()=="NormalMode")
            {
                states.setStation1Unload(false);
            }
            else
            {
                qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            }
        }
    }
    else if(origin_module == "AA2CoreNew")
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString() == "LoadLensRequest")
            {
                if(message.contains("MaterialState"))
                    states.setAa2HeadMaterialState(MaterialTray::getMaterialStateFromName(message["MaterialState"].toString()));
                else
                    qInfo("message MaterialState miss");
                if(message.contains("TaskNumber"))
                    states.setTaskOfStation2(message["TaskNumber"].toInt());
                if(!states.station2Unload())
                    states.setStation2NeedLens(true);
                states.setStation2HasRequest(true);
            }
            else if(message["Message"].toString()=="UnloadMode")
            {
                states.setStation2Unload(true);
            }
            else if(message["Message"].toString()=="NormalMode")
            {
                states.setStation2Unload(false);
            }
            else
            {
                qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            }
        }
    }
    else if(origin_module == "LensLoaderModule")
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString().contains("FinishLoadLens"))
            {
                if(message.contains("LutHasLens"))
                    states.setLutHasLens(message["LutHasLens"].toBool());
                else
                    qInfo("message LutHasLens miss");
                if(message.contains("LutLensData"))
                    states.copyInLensData(message["LutLensData"].toMap());
                else
                    qInfo("message LutLensData miss");
                states.setLutHasNgLens(false);
                states.setFinishWaitLens(true);
            }
        }
    }
}

PropertyBase *LutModule::getModuleState()
{
    return &states;
}

QMap<QString, PropertyBase *> LutModule::getModuleParameter()
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
    return temp_map;
}

void LutModule::setModuleParameter(QMap<QString, PropertyBase *>)
{

}

void LutModule::receiveToolUpPRRequest(PrOffset &offset)
{
    sut->toolUplookPR(offset, true, false);
}

