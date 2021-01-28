#include "sutModule/sut_module.h"

#include <QMessageBox>
#include <tcpmessager.h>
#include "materialtray.h"

SutModule::SutModule()
{
    gui_thread_id = QThread::currentThreadId();
}

void SutModule::Init(MaterialCarrier *carrier, VisionLocation* downlook_location,VisionLocation* updownlook_down_location,VisionLocation* updownlook_up_locationn, XtVacuum *vacuum,XtCylinder* pogopin,XtGeneralOutput* camera_trig,int thread_id)
{
    this->carrier = carrier;
    parts.append(carrier);
    this->vision_downlook_location = downlook_location;
    parts.append(vision_downlook_location);
    this->vision_updownlook_down_location = updownlook_down_location;
    parts.append(vision_updownlook_down_location);
    this->vision_updownlook_up_location = updownlook_up_locationn;
    parts.append(vision_updownlook_up_location);
    this->vacuum = vacuum;
    parts.append(vacuum);
    this->popgpin = pogopin;
    parts.append(popgpin);
    this->camera_trig = camera_trig;
    parts.append(camera_trig);
    setName(parameters.moduleName());
    this->thread_id = thread_id;
    if (vacuum)
        this->parameters.setSutVaccumName(vacuum->parameters.outIoName());
    if (pogopin)
        this->parameters.setSutPOGOPINName(pogopin->parameters.oneOutName());
}

void SutModule::saveJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map = getModuleParameter();
    PropertyBase::saveJsonConfig(file_name, temp_map);
}

//bool SutModule::checkSutSensorOrProduct(bool check_state)
//{
//    bool result = vacuum->checkHasMaterielSync();
//    if(result == check_state)
//        return true;
//    QString error = QString(u8"SUT上逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
//    AppendError(error);
//    qInfo(error.toStdString().c_str());
//    return false;
//}

bool SutModule::checkSutHasMaterialSynic()
{
    QElapsedTimer timer; timer.start();
    bool result;
    result = vacuum->checkHasMaterielSync();
    if(result||RunMode::NoMaterial == states.runMode())
    {
        qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
        return true;
    }
    QString error = QString(u8"sut上应有料，但真空检测到无料。");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return false;
}

bool SutModule::checkSutHasMaterial()
{
    QElapsedTimer timer; timer.start();
    bool result;
    result = vacuum->checkHasMateriel(thread_id);
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
    return result;
}

bool SutModule::waitSutCheckResult(bool check_state)
{
    QElapsedTimer timer; timer.start();
    bool result;
    result = vacuum->getHasMateriel(thread_id);
    if(result == check_state||RunMode::NoMaterial == states.runMode())
    {
        qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
        return true;
    }
    QString error;
    if(check_state)
        error = QString(u8"sut上应有料，但真空检测到无料。");
    else
        error = QString(u8"sut上应无料，但真空检测到有料。");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    qWarning("[Timelog] %s %d", __FUNCTION__, timer.elapsed());
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
    QMap<QString,PropertyBase*> temp_map = getModuleParameter();
    PropertyBase::loadJsonConfig(file_name, temp_map);
}

bool SutModule::moveToDownlookPR(PrOffset &offset,bool close_lighting,bool check_autochthonous)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer; smallTimer.start();
    QString temp;
    vision_downlook_location->OpenLight();
    bool result = moveToDownlookPos(check_autochthonous);
    temp.append(" moveToDownlookPos ").append(QString::number(smallTimer.elapsed()));
    if(result)
    {
        smallTimer.restart();
        result = performDownLookPR();
        temp.append(" performDownLookPR ").append(QString::number(smallTimer.elapsed()));
        offset = vision_downlook_location->getCurrentResult();
    }
    if(close_lighting)
    {
        vision_downlook_location->CloseLight();
    }
    if(!result)
        AppendError(u8"执行downlook pr 失败");
    QString log = QString("[Timelog] ").append(__FUNCTION__).append(" ")
                                      .append(QString::number(timer.elapsed()))
                                      .append(temp);
    qInfo(log.toStdString().c_str());
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
        result = vision_downlook_location->saveImage(imageName);
    }
    if(close_lighting)
        vision_downlook_location->CloseLight();
    return result;
}

bool SutModule::moveToLoadPos(bool check_autochthonous)
{
    QElapsedTimer timer; timer.start();
    QElapsedTimer smallTimer;

    double dist_x = fabs(carrier->motor_x->GetFeedbackPos() - load_position.X());
    double dist_y = fabs(carrier->motor_y->GetFeedbackPos() - load_position.Y());

    smallTimer.start();
    carrier->setCallerFunctionName(__FUNCTION__);
    bool result = carrier->Move_SZ_SX_Y_X_Sync(load_position.X(),load_position.Y(),parameters.loadPosArrivedY(),check_autochthonous);
    double dist_z = fabs(carrier->motor_z->GetFeedbackPos() - load_position.Z());
    int t1 = smallTimer.elapsed();
    //result &= popgpin->Set(false,false);
    if(result)
    {
        smallTimer.restart();
        result &= popgpin->Set(false,false);
        int t2 = smallTimer.elapsed();
        smallTimer.restart();
        result &= carrier->motor_z->MoveToPosSync(load_position.Z(),0.1);
        int t3 = smallTimer.elapsed();
        smallTimer.restart();
        result &= carrier->motor_y->WaitArrivedTargetPos(load_position.Y());
        int t4 = smallTimer.elapsed();
        smallTimer.restart();
        QThread::msleep(parameters.delayBeforeVaccum());
        int t5 = smallTimer.elapsed();
        smallTimer.restart();
        result &= vacuum->Set(false,false,false);
        int t6 = smallTimer.elapsed();
        QString log = QString("[Timelog] ").append(__FUNCTION__).append(" ")
                                           .append(QString::number(timer.elapsed()))
                                           .append(" dist_x ")
                                           .append(QString::number(dist_x))
                                           .append(" dist_y ")
                                           .append(QString::number(dist_y))
                                           .append(" dist_z ")
                                           .append(QString::number(dist_z))
                                           .append(" move_x_y_z ")
                                           .append(QString::number(t1))
                                           .append(" pogo ")
                                           .append(QString::number(t2))
                                           .append(" z ")
                                           .append(QString::number(t3))
                                           .append(" y ")
                                           .append(QString::number(t4))
                                           .append(" delayBeforeVaccum ")
                                           .append(QString::number(t5))
                                           .append(" vaccumSet ")
                                           .append(QString::number(t6));

        qWarning(log.toStdString().c_str());
    }
    carrier->setCallerFunctionName("");
    return result;
}

bool SutModule::moveToDownlookPos(bool check_autochthonous)
{
    qInfo("moveToDownlookPos");
    popgpin->Set(true,false);
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

bool SutModule::moveToDownLookFlyPr()
{
    if(carrier->motor_y->GetFeedbackPos()<parameters.downlookFlyStartPosition())
    {
        AppendError(u8"Sut_Y位于飞拍减速位置之前，无法进行飞拍");
        return false;
    }
    //开始移动
    bool result = carrier->motor_y->setTrig(true,parameters.downlookFlyPrPosition(),camera_trig->GetID(),true);
    result &= carrier->motor_y->setTrig(true,parameters.downlookFlyPrPosition()-1,camera_trig->GetID(),false);
    result &= carrier->motor_x->SGO(down_look_fly_end_position.X());
    result &= carrier->motor_y->SGO(down_look_fly_end_position.Y());
//    result &= carrier->Move_SZ_XY_ToPos(down_look_fly_end_position.X(),down_look_fly_end_position.Y());
    if(!result)
        return false;
    //到达飞拍起点变速
    result = carrier->motor_y->WaitLessThanTargetPos(parameters.downlookFlyStartPosition());
    if(!result)
        return false;
    carrier->motor_y->SetVel(parameters.downlookFlyVelocity());
    result = carrier->motor_y->SGO(down_look_fly_end_position.Y());
    if(!result)
    {
        carrier->motor_y->ResetVel();
        return false;
    }
    //到达飞排点进行飞拍
    result = carrier->motor_y->WaitArrivedTargetPos(parameters.downlookFlyPrPosition()+1);
    if(!result)
    {
        carrier->motor_y->ResetVel();
        return false;
    }
    carrier->motor_y->clearTrig();
    carrier->motor_y->ResetVel();
    return true;
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

bool SutModule::moveToParticalCheckPos(bool check_autochthonous)
{
    popgpin->Set(true);
    return carrier->Move_SZ_SX_YS_X_Z_Sync(partical_check_position.X(),partical_check_position.Y(),partical_check_position.Z(),check_autochthonous);
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
    qInfo("target : %f",z);
    return carrier->Move_Z_Sync(z);
}

bool SutModule::moveToXPos(double x)
{
    qInfo("target : %f",x);
    return carrier->Move_X_Sync(x);
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
    return carrier->Move_SZ_SX_YS_X_Z_Sync(carrier->parameters.SafetyX(),carrier->parameters.SafetyY(),carrier->parameters.SafetyZ(),check_autochthonous);
}

bool SutModule::performDownLookPR()
{
    qInfo("performDownLookPR");
    bool result;
    if(states.runMode() == RunMode::NoMaterial)
        result = vision_downlook_location->performNoMaterialPR();
    else
        result = vision_downlook_location->performPR();
    if(!result)
        AppendError(QString(u8"执行SUT DownLook视觉失败!"));
    return  result;
}

bool SutModule::OpenSutVacuum()
{
    if(states.runMode() == RunMode::NoMaterial)
        return vacuum->SetSimulation(true);
    else
        return vacuum->Set(true);
}

bool SutModule::CloseSutVacuum()
{
    if(states.runMode() == RunMode::NoMaterial)
        return vacuum->SetSimulation(false);
    else
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

void SutModule::run()
{
    is_run = true;
    QString aa_module_name = states.stationNumber() == 0?"AA1CoreNew":"AA2CoreNew";
    while (is_run)
    {
        QThread::msleep(10);
        if(!states.allowLoadSensor())
            continue;
        //去load位置
        if((states.sutMaterialState() != MaterialState::IsRawSensor)&&(!states.waitLoading()))
        {
            if(states.sutMaterialState() != MaterialState::IsEmpty)
            {
                if(!moveToLoadPos())
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
                if(!checkSutHasMaterialSynic())
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(REJECT_OPERATION == operation)
                    {
                        states.setSutMaterialState(MaterialState::IsEmpty);
                        continue;
                    }
                    else if(RETRY_OPERATION == operation)
                        continue;
                }
            }
            else
            {
                checkSutHasMaterial();
                if(!moveToLoadPos())
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
                if(!waitSutCheckResult(false))
                {
                    int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(RETRY_OPERATION == operation)
                        continue;
                }
            }
            sendMessageToModule("SensorLoaderModule","SutReady");
            states.setLoadingSensor(true);
            states.setWaitLoading(true);
        }
        //等待卸料完成
        if(states.waitLoading()&&(!states.loadingSensor())&&(states.sutMaterialState() == MaterialState::IsEmpty))
        {
            states.setAllowLoadSensor(false);
            states.setWaitLoading(false);
            QJsonObject param;
            param.insert("SutMaterialState",MaterialTray::getMaterialStateName(states.sutMaterialState()));
            sendMessageToModule(aa_module_name,"FinishLoadSensor",param);
        }
        //等待卸料+上料完成
        if(states.waitLoading()&&(!states.loadingSensor())&&(states.sutMaterialState() == MaterialState::IsRawSensor))
        {
            if(!checkSutHasMaterialSynic())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setSutHasSensor(false);
                    continue;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }

            if(!popgpin->Set(true))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            QThread::msleep(100);
            if(!moveToDownlookPR(offset))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                {
                    continue;
                }
                else if(REJECT_OPERATION == operation)
                {
                    states.setSutHasNgSensor(true);
                    states.setSutMaterialState(MaterialState::IsNgSensor);
                    DownlookPrDone = false;
                    //continue;
                }
            }
            else
            {
                DownlookPrDone = true;
            }
            emit sendLoadSensorFinish(offset.X,offset.Y,offset.Theta);
            states.setAllowLoadSensor(false);
            states.setWaitLoading(false);
            QJsonObject param;
            //            param.insert("OffsetX",offset.X);
            //            param.insert("OffsetY",offset.Y);
            //            param.insert("OffsetT",offset.Theta);
            param.insert("SutMaterialState",MaterialTray::getMaterialStateName(states.sutMaterialState()));
            sendMessageToModule(aa_module_name,"FinishLoadSensor",param);
        }

    }
    states.setRunMode(RunMode::Normal);
    qInfo("sut module end of thread");
}

void SutModule::startWork(int run_mode)
{
    if(run_mode == RunMode::AAFlowChartTest || run_mode == RunMode::UNLOAD_ALL_LENS) return;
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
    if(run_params.contains("HandlyChangeSensor"))
    {
        states.setHandlyChangeSensor(run_params["HandlyChangeSensor"].toBool());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数HandlyChangeSensor缺失.启动失败.",ErrorLevel::ErrorMustStop);
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
        sendAlarmMessage(OK_OPERATION,u8"启动参数StationNumber、DisableStation缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(states.disableStation())
        return;
    qInfo("sut Module start run_mode :%d in %d",run_mode,QThread::currentThreadId());
    if(states.runMode() == RunMode::Normal||states.runMode() == RunMode::NoMaterial)
        run();
}

void SutModule::stopWork(bool wait_finish)
{
  is_run = false;
}

void SutModule::performHandlingOperation(int cmd,QVariant param)
{
    bool result = true;
    int temp_value = 10;
    qInfo("performHandlingOperation cmd: %d", cmd);
    if(cmd%temp_value == HandlePosition::DOWNLOOK_PR_POS)
        result = moveToDownlookPos();
    else if(cmd%temp_value == HandlePosition::DOWNLOOK_ON_THE_FLY_POS)
        result = moveToDownLookFlyPr();
    else if(cmd%temp_value == HandlePosition::LOAD_POS)
        result = moveToLoadPos();
    else if(cmd%temp_value == HandlePosition::MUSHROOM_POS)
        result = moveToMushroomPos();
    else if(cmd%temp_value == HandlePosition::PARTICAL_CHECK_POS)
        result = moveToParticalCheckPos();
    if(!result)
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
    cmd = cmd/temp_value*temp_value;
    temp_value = 100;
    if(cmd%temp_value == HandlePR::DOWNLOOK_PR)
    {
//        PrOffset offset(0, 0, 0);
//        moveToDownlookPR(offset, false);
//        stepMove_XY_Sync(-offset.X, -offset.Y);
        result = moveToDownlookPR(true, false);
    }
    if(!result)
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
    is_handling = false;
}

void SutModule::receivceModuleMessage(QVariantMap message)
{
    qInfo("receive module message %s",TcpMessager::getStringFromQvariantMap(message).toStdString().c_str());
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
            if(message["Message"].toString()=="FinishLoadSensor")
            {
                states.setSutMaterialState(MaterialTray::getMaterialStateFromName(message["MaterialState"].toString()));
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
            if(message["Message"].toString()=="LoadSensorRequest")
            {
                states.setSutMaterialState(MaterialTray::getMaterialStateFromName(message["MaterialState"].toString()));
                states.setAllowLoadSensor(true);
            }
            else if(message["Message"].toString()=="UnloadMode")
            {
                states.setStationUnload(true);
            }
            else if(message["Message"].toString()=="NormalMode")
            {
                states.setStationUnload(false);
            }
            else
            {
                qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            }
        }
    } else if(message.contains("Message")){
        if(message["Message"].toString()=="MoveToLoadPos")
        {
            moveToLoadPos();
        }
    } else {
        qInfo("module name error %s",message["OriginModule"].toString().toStdString().c_str());
    }
}

PropertyBase *SutModule::getModuleState()
{
    return &states;
}

QMap<QString, PropertyBase *> SutModule::getModuleParameter()
{
    QMap<QString,PropertyBase*> temp_map;
//    temp_map.insert("sut", &this->carrier->parameters);
    temp_map.insert("SUT_PARAMS", &this->parameters);
    temp_map.insert("LOAD_POSITION", &this->load_position);
    temp_map.insert("DOWNLOOK_POSITION", &this->downlook_position);
    temp_map.insert("PARTICAL_CHECK_POSITION", &this->partical_check_position);
    temp_map.insert("TOOLUPLOOK_POSITION", &this->tool_uplook_positon);
    temp_map.insert("TOOLDOWNLOOK_POSITION", &this->tool_downlook_position);
    temp_map.insert("MUSHROOM_POSITION", &this->mushroom_positon);
    temp_map.insert("UP_DOWNLOOK_POSITION", &this->up_downlook_offset);
    temp_map.insert("DOWNLOOK_FLY_END_POSITION", &this->down_look_fly_end_position);
    return temp_map;
}

void SutModule::setModuleParameter(QMap<QString, PropertyBase *> parameters)
{

}
