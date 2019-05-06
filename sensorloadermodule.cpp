#include "sensorloadermodule.h"
#include "commonutils.h"

SensorLoaderModule::SensorLoaderModule():ThreadWorkerBase ("SensorLoader")
{

}

void SensorLoaderModule::Init(SensorPickArm *pick_arm, MaterialTray *sensor_tray,
                              VisionLocation *sensor_vision, VisionLocation *vacancy_vision,
                              VisionLocation *sut_vision, VisionLocation *sut_sensor_vision,
                              VisionLocation *sut_product_vision)
{
    this->pick_arm = pick_arm;
    parts.append(this->pick_arm);
    this->tray = sensor_tray;
    parts.append(this->tray);
    this->sensor_vision = sensor_vision;
    parts.append(this->sensor_vision);
    this->vacancy_vision = vacancy_vision;
    parts.append(this->vacancy_vision);
    this->sut_vision = sut_vision;
    parts.append(this->sut_vision);
    this->sut_sensor_vision = sut_sensor_vision;
    parts.append(this->sut_sensor_vision);
    this->sut_product_vision = sut_product_vision;
    parts.append(this->sut_product_vision);
    this->spa_updownlook_up_vision = spa_updownlook_up_vision;
    parts.append(this->spa_updownlook_up_vision);
    this->spa_updownlook_down_vision = spa_updownlook_down_vision;
    parts.append(this->spa_updownlook_down_vision);
}

bool SensorLoaderModule::loadJsonConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("parameters", &parameters);
    temp_map.insert("sut1_pr_position", &sut1_pr_position);
    temp_map.insert("sut2_pr_position", &sut1_pr_position);
    temp_map.insert("picker1_offset", &picker1_offset);
    temp_map.insert("picker2_offset", &picker2_offset);
    PropertyBase::loadJsonConfig("config//SensorLoaderModule.json", temp_map);
    return true;
}

void SensorLoaderModule::saveJsonConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("parameters", &parameters);
    temp_map.insert("sut1_pr_position", &sut1_pr_position);
    temp_map.insert("sut2_pr_position", &sut1_pr_position);
    temp_map.insert("picker1_offset", &picker1_offset);
    temp_map.insert("picker2_offset", &picker2_offset);
    PropertyBase::saveJsonConfig("config//SensorLoaderModule.json", temp_map);
}

void SensorLoaderModule::startWork(bool reset_logic, int run_mode)
{
    qInfo("SensorLoader start reset:%d run_mode :%d",reset_logic,run_mode);
    if(reset_logic)resetLogic();
    if(run_mode == RunMode::Normal)run(true);
    else if(run_mode == RunMode::NoMaterial)run(false);
}

void SensorLoaderModule::stopWork(bool wait_finish)
{

}

void SensorLoaderModule::performHandlingOperation(int cmd)
{

}

void SensorLoaderModule::receiveRequestMessage(QString message, QString client_ip)
{
    qInfo("Lut Module receive command:%s from ip: %s", message.toStdString().c_str(), client_ip.toStdString().c_str());
    QJsonObject obj = getJsonObjectFromString(message);
    QString cmd = obj["cmd"].toString("");
    obj.insert("client_ip",client_ip);
    if (cmd == "lensReq") {
        qInfo("Enqueue the lens request command in request quene");
        requestQueue.enqueue(obj);
    }
    else if (cmd.length() > 0)
    {
        qInfo("Enqueue the %s in action queue", cmd.toStdString().c_str());
        actionQueue.enqueue(obj);
    }
}

void SensorLoaderModule::resetLogic()
{

}

void SensorLoaderModule::openServer(int port)
{
    server = new SparrowQServer(port);
    connect(server, &SparrowQServer::receiveRequestMessage, this, &SensorLoaderModule::receiveRequestMessage, Qt::DirectConnection);
    connect(this, &SensorLoaderModule::sendMessageToClient, this->server, &SparrowQServer::sendMessageToClient);
}

void SensorLoaderModule::run(bool has_material)
{
    int sensor_tray_index = 0;
    int product_tray_index = 1;
    is_run = true;
    int pr_times = 5;
    bool has_task = true;
    while (is_run)
    {
//        if(!has_task)
            QThread::msleep(1000);
        has_task = false;
        //放成品
        if(states.hasPickedProduct())
        {
            has_task = true;
            if(!moveToNextTrayPos(product_tray_index))
            {
                AppendError(u8"moveToTrayPos fail");
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!performVacancyPR())&&has_material)
            {
                AppendError(u8"空盘视觉失败！");
                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                int result = waitMessageReturn(is_run);
                if(!result)is_run = false;
                if(!is_run)break;
                if(result)
                    continue;
            }
            if(!moveToWorkPos())
            {
                AppendError(u8"moveToWorkPos fail");
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!placeProductToTray())&&has_material)
            {
                AppendError(u8"placeProductToTray fail");
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                waitMessageReturn(is_run);
            }
            tray->setCurrentMaterialState(MaterialState::IsProduct,product_tray_index);
            states.setHasPickedProduct(false);
            if(!is_run)break;
        }
        //放NGSensor
        if(states.hasPickedNgSensor())
        {
            has_task = true;
            if(!moveToNextTrayPos(product_tray_index))
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!performVacancyPR())&&has_material)
            {
                AppendError(u8"空盘视觉失败！");
                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                int result = waitMessageReturn(is_run);
                if(!result)is_run = false;
                if(!is_run)break;
                if(result)
                    continue;
            }
            if(!moveToWorkPos())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!placeSensorToTray())&&has_material)
            {
                AppendError(u8"placeProductToTray fail");
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                waitMessageReturn(is_run);
            }
            tray->setCurrentMaterialState(MaterialState::IsNg,product_tray_index);
            states.setHasPickedProduct(false);
            if(!is_run)break;
        }
        //取料
        if((!finish_stop)&&(!states.hasPickedSensor())&&(!states.hasPickedProduct())&&(!states.hasPickedNgSensor()))
        {
            has_task = true;
            if(!moveToNextTrayPos(sensor_tray_index))
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!performSensorPR())&&has_material)
            {
                if(pr_times > 0)
                    continue;
                else
                {
                    pr_times = 5;
                    sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                    waitMessageReturn(is_run);
                    if(is_run)break;
                }
            }
            if(!moveToWorkPos())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }

            if((!pickTraySensor())&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(!waitMessageReturn(is_run))
                    states.setHasPickedSensor(true);
            }
            else
                states.setHasPickedSensor(true);
            tray->setCurrentMaterialState(MaterialState::IsEmpty,sensor_tray_index);
            picked_material = tray->getCurrentIndex(sensor_tray_index);
            if(!is_run)break;
        }
        //等待位置
        if(!moveToSUTPRPos())
        {
            sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
            is_run = false;
            break;
        }
        if (requestQueue.size()>0 && (!states.beExchangeMaterial())&&states.hasPickedSensor()&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct()))
        {
            has_task = true;
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

            if (cmd == "sensorReq")
            {
                qInfo("start commincate with %s",servingIP.toStdString().c_str());
                actionQueue.clear();
                states.setBeExchangeMaterial(true);
                sendEvent("sensorResp");
            }
        }

        if (actionQueue.size()>0 && states.beExchangeMaterial())
        {
            has_task = true;
            if(states.cmd() == "")
            {
                QJsonObject obj = actionQueue.dequeue();
                qInfo("Start to consume action request: %s", getStringFromJsonObject(obj).toStdString().c_str());
//                QString client_ip = obj["client_ip"].toString("");
                states.setCmd(obj["cmd"].toString(""));
            }

            if((states.cmd() == "unloadProductReq")&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct()))
            {
                if(!moveToSUTPRPos(isLocalHost))
                {
                    sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                    is_run = false;
                    break;
                }
                if((!performSUTProductPR())&&has_material)
                {
                    AppendError(u8"NG视觉失败！");
                    sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                    int result = waitMessageReturn(is_run);
                    if(!result)is_run = false;
                    if(!is_run)break;
                    if(result)
                        continue;
                }
                if(!moveToWorkPos2())
                {
                    sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                    is_run = false;
                    break;
                }
                if((!pickSUTProduct())&&has_material)
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                    if(waitMessageReturn(is_run))
                        states.setSutHasProduct(false);
                    else
                    {
                        states.setSutHasProduct(false);
                        states.setHasPickedProduct(true);
                    }
                }
                else
                {
                    states.setSutHasProduct(false);
                    states.setHasPickedProduct(true);
                }
                sendEvent("unloadProductResp");
                states.setCmd("");
            }
            else if((states.cmd() == "unloadNgSensorReq")&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct()))
            {
                //取NGsensor
                if(states.sutHasNgSensor()&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct()))
                {
                    has_task = true;
                    if(!moveToSUTPRPos())
                    {
                        AppendError("moveToSUTPRPos fail!");
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if((!performSUTSensorPR())&&has_material)
                    {
                        AppendError(u8"NG视觉失败！");
                        sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                        int result = waitMessageReturn(is_run);
                        if(!result)is_run = false;
                        if(!is_run)break;
                        if(result)
                            continue;
                    }
                    if(!moveToWorkPos2())
                    {
                        AppendError("moveToWorkPos2 fail!");
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                        is_run = false;
                        break;
                    }
                    if((!pickSUTSensor())&&has_material)
                    {
                        AppendError("pickSUTSensor fail!");
                        sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                        if(waitMessageReturn(is_run))
                            states.setSutHasNgSensor(false);
                        else
                        {
                            states.setSutHasNgSensor(false);
                            states.setHasPickedNgSensor(true);
                        }
                    }
                    else
                    {
                        states.setSutHasNgSensor(false);
                        states.setHasPickedNgSensor(true);
                    }
                }
                sendEvent("unloadNgSensorResp");
                states.setCmd("");
            }
            else if ((states.cmd() == "loadSensorReq")&&states.hasPickedSensor())
            {
                //放料到SUT
                if(!moveToSUTPRPos(isLocalHost))
                {
                    sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                    is_run = false;
                    break;
                }
                if(!moveToWorkPos())
                {
                    AppendError("moveToWorkPos fail!");
                    sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                    is_run = false;
                    break;
                }
                if((!placeSensorToSUT())&&has_material)
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                    if(waitMessageReturn(is_run))
                    {
                        states.setHasPickedSensor(false);
                    }
                    else
                    {
                        states.setNeedLoadSensor(false);
                        states.setHasPickedSensor(false);
                        sendEvent("loadSensorResp");
                        states.setCmd("");
                        states.setBeExchangeMaterial(false);
                    }
                }
                else
                {
                    states.setNeedLoadSensor(false);
                    states.setHasPickedSensor(false);
                    sendEvent("loadSensorResp");
                    states.setCmd("");
                    states.setBeExchangeMaterial(false);
                }
            }
        }
//        //放料到SUT
//        if(states.needLoadSensor()&&states.hasPickedSensor())
//        {
//            has_task = true;
//            if(!moveToSUTPRPos())
//            {
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            if(!moveToWorkPos())
//            {
//                AppendError("moveToWorkPos fail!");
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            if((!placeSensorToSUT())&&has_material)
//            {
//                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
//                if(waitMessageReturn(is_run))
//                    states.setHasPickedSensor(false);
//                else
//                {
//                    states.setNeedLoadSensor(false);
//                    states.setHasPickedSensor(false);
//                }
//            }
//            else
//            {
//                states.setNeedLoadSensor(false);
//                states.setHasPickedSensor(false);
//            }
//            sut_raw_material = picked_material;
//        }
//        //取成品
//        if(states.sutHasProduct()&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct()))
//        {
//            has_task = true;
//            if(!moveToSUTPRPos())
//            {
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            if((!performSUTProductPR())&&has_material)
//            {
//                AppendError(u8"NG视觉失败！");
//                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
//                int result = waitMessageReturn(is_run);
//                if(!result)is_run = false;
//                if(!is_run)break;
//                if(result)
//                    continue;
//            }
//            if(!moveToWorkPos2())
//            {
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            if((!pickSUTProduct())&&has_material)
//            {
//                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
//                if(waitMessageReturn(is_run))
//                    states.setSutHasProduct(false);
//                else
//                {
//                    states.setSutHasProduct(false);
//                    states.setHasPickedProduct(true);
//                }
//            }
//            else
//            {
//                states.setSutHasProduct(false);
//                states.setHasPickedProduct(true);
//            }

//        }
//        //取NGsensor
//        if(states.sutHasNgSensor()&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct()))
//        {
//            has_task = true;
//            if(!moveToSUTPRPos())
//            {
//                AppendError("moveToSUTPRPos fail!");
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            if((!performSUTSensorPR())&&has_material)
//            {
//                AppendError(u8"NG视觉失败！");
//                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
//                int result = waitMessageReturn(is_run);
//                if(!result)is_run = false;
//                if(!is_run)break;
//                if(result)
//                    continue;
//            }
//            if(!moveToWorkPos2())
//            {
//                AppendError("moveToWorkPos2 fail!");
//                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
//                is_run = false;
//                break;
//            }
//            if((!pickSUTSensor())&&has_material)
//            {
//                AppendError("pickSUTSensor fail!");
//                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
//                if(waitMessageReturn(is_run))
//                    states.setSutHasNgSensor(false);
//                else
//                {
//                    states.setSutHasNgSensor(false);
//                    states.setHasPickedNgSensor(true);
//                }
//            }
//            else
//            {
//                states.setSutHasNgSensor(false);
//                states.setHasPickedNgSensor(true);
//            }
//        }
    }
    qInfo("LensLoader stoped");
}

bool SensorLoaderModule::moveToNextTrayPos(int tray_index)
{
    qInfo("moveToNextTrayPos:%d",tray_index);
    if(tray->findNextPositionOfInitState(tray_index))
    {
        return  pick_arm->move_XY_Synic(tray->getCurrentPosition(tray_index));
    }
    return  false;
}

bool SensorLoaderModule::moveToSUTPRPos(bool is_local,bool check_softlanding)
{
    qInfo("moveToSUTPRPos %d",is_local);
    if(is_local)
        return  pick_arm->move_XY_Synic(sut2_pr_position.ToPointF(),check_softlanding);
    else
        return  pick_arm->move_XY_Synic(sut1_pr_position.ToPointF(),check_softlanding);
}

bool SensorLoaderModule::performSensorPR()
{
    qInfo("performSensorPR");
    return  sensor_vision->performPR(pr_offset);
}

bool SensorLoaderModule::performVacancyPR()
{
    qInfo("performSensorPR");
    return  vacancy_vision->performPR(pr_offset);

}

bool SensorLoaderModule::performSUTSensorPR()
{
    qInfo("performSUTSensorPR");
    return  sut_sensor_vision->performPR(pr_offset);
}

bool SensorLoaderModule::performSUTProductPR()
{
    qInfo("performSUTProductPR");
    return  sut_product_vision->performPR(pr_offset);
}

bool SensorLoaderModule::moveToWorkPos(bool check_softlanding)
{
    qInfo("moveToWorkPos");
    PrOffset temp(picker1_offset.X() - pr_offset.X,picker1_offset.Y() - pr_offset.Y,pr_offset.Theta);
    qInfo("offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XYT1_Synic(temp.X,temp.Y,temp.Theta,check_softlanding);
    pr_offset.ReSet();
    return  result;
}

bool SensorLoaderModule::moveToWorkPos2(bool check_softlanding)
{
    qInfo("moveToWorkPos2");
    PrOffset temp(picker2_offset.X() - pr_offset.X,picker2_offset.Y() - pr_offset.Y,pr_offset.Theta);
    qInfo("offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XYT1_Synic(temp.X,temp.Y,temp.Theta,check_softlanding);
    pr_offset.ReSet();
    return  result;
}

bool SensorLoaderModule::picker1SearchZ(double z,bool is_open, int time_out)
{
    qInfo("picker1SearchZ");
    bool result = pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),parameters.finishDelay(),is_open,false,time_out);
    if(is_open)
        sendCmd("remot","vacuumOnReq");
    else
        sendCmd("remot","vacuumOffReq");
    QThread::msleep(200);
    result &= pick_arm->ZSerchReturn(time_out);
    return result;
}

bool SensorLoaderModule::picker2SearchZ(double z,bool is_open, int time_out)
{
    qInfo("picker2SearchZ");
    bool result = pick_arm->ZSerchByForce2(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),parameters.finishDelay(),is_open,false,time_out);
    if(is_open)
        sendCmd("::1","vacuumOnReq");
    else
        sendCmd("::1","vacuumOffReq");
    QThread::msleep(200);
    result &= pick_arm->ZSerchReturn2(time_out);
    return result;

}

bool SensorLoaderModule::pickTraySensor(bool check_softlanding)
{
    qInfo("pickTraySensor");
    return picker1SearchZ(parameters.pickLensZ(),true,check_softlanding);
}

bool SensorLoaderModule::placeSensorToSUT(bool check_softlanding)
{
    qInfo("pickTrplaceSensorToSUTaySensor");
    return picker1SearchZ(parameters.placeLensZ(),false,check_softlanding);
}

bool SensorLoaderModule::pickSUTSensor(bool check_softlanding)
{
    qInfo("pickSUTSensor");
    return picker2SearchZ(parameters.placeLensZ(),true,check_softlanding);
}

bool SensorLoaderModule::pickSUTProduct(bool check_softlanding)
{
    qInfo("pickSUTProduct");
    return picker2SearchZ(parameters.pickProductZ(),true,check_softlanding);
}

bool SensorLoaderModule::placeSensorToTray(bool check_softlanding)
{
    qInfo("placeSensorToTray");
    return picker2SearchZ(parameters.pickLensZ(),false,check_softlanding);
}

bool SensorLoaderModule::placeProductToTray(bool check_softlanding)
{
    qInfo("placeProductToTray");
    return picker2SearchZ(parameters.placeProductZ(),false,check_softlanding);
}

bool SensorLoaderModule::measureHight(bool is_tray)
{
    qInfo("measureHight");
    if(pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))
    {
        if(is_tray)
            parameters.setPickLensZ(pick_arm->GetSoftladngPosition());
        else
            parameters.setPlaceLensZ(pick_arm->GetSoftladngPosition());
        return true;
    }
    return false;
}

bool SensorLoaderModule::moveToTrayPos(int index, int tray_index)
{
    qInfo("moveToTrayPos");
    return pick_arm->move_XY_Synic(tray->getPositionByIndex(index,tray_index));
}

bool SensorLoaderModule::moveToTrayPos(int tray_index)
{
    qInfo("moveToTrayPos%d",tray_index);
    return  pick_arm->move_XY_Synic(tray->getCurrentPosition(tray_index),true);
}

bool SensorLoaderModule::moveToStartPos(int tray_index)
{
    qInfo("moveToStartPos%d",tray_index);
    return pick_arm->move_XY_Synic(tray->getStartPosition(tray_index),true);
}

bool SensorLoaderModule::moveToTray1EndPos()
{
    qInfo("moveToTray1EndPos");
    return pick_arm->move_XY_Synic(tray->getEndPosition(),true);
}

void SensorLoaderModule::sendEvent(const QString event)
{
    QJsonObject result;
    result.insert("event", event);
    emit sendMessageToClient(servingIP, getStringFromJsonObject(result));
}

void SensorLoaderModule::sendCmd(QString serving_ip, const QString cmd)
{
    QJsonObject result;
    result.insert("cmd", cmd);
    emit sendMessageToClient(serving_ip, getStringFromJsonObject(result));
}
