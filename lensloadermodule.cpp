#include "lensloadermodule.h"
#include "tcpmessager.h"

LensLoaderModule::LensLoaderModule(QString name):ThreadWorkerBase (name)
{

}

void LensLoaderModule::Init(LensPickArm *pick_arm, MaterialTray *lens_tray, MaterialCarrier *lut_carrier,XtVacuum* load_vacuum,
                            XtVacuum* unload_vacuum, VisionLocation *lens_vision, VisionLocation *vacancy_vision, VisionLocation *lut_vision,
                            VisionLocation *lut_lens_vision,VisionLocation *lpa_picker_vision,VisionLocation *lpa_updownlook_up_vision,
                            VisionLocation *lpa_updownlook_down_vision, VisionLocation *lut_ng_slot_vision, LutModule *lut)
{
    parts.clear();
    this->pick_arm = pick_arm;
    parts.append(this->pick_arm);
    this->tray = lens_tray;
    parts.append(this->tray);
    this->lut_carrier = lut_carrier;
    parts.append(this->lut_carrier);
    this->load_vacuum = load_vacuum;
    parts.append(this->load_vacuum);
    this->unload_vacuum = unload_vacuum;
    parts.append(this->unload_vacuum);
    this->lens_vision = lens_vision;
    parts.append(this->lens_vision);
    this->lut_ng_slot_vision = lut_ng_slot_vision;
    parts.append(this->lut_ng_slot_vision);
    this->vacancy_vision = vacancy_vision;
    parts.append(this->vacancy_vision);
    this->lut_vision = lut_vision;
    parts.append(this->lut_vision);
    this->lut_lens_vision = lut_lens_vision;
    parts.append(this->lut_lens_vision);
    this->lpa_picker_vision = lpa_picker_vision;
    parts.append(this->lpa_picker_vision);
    this->lpa_updownlook_up_vision = lpa_updownlook_up_vision;
    parts.append(this->lpa_updownlook_up_vision);
    this->lpa_updownlook_down_vision = lpa_updownlook_down_vision;
    parts.append(this->lpa_updownlook_down_vision);
    //Some pickarm parameter need to sync with lensLoaderModule
    if (pick_arm->picker->vacuum)
    {
        parameters.setPickarmVaccumName(this->pick_arm->picker->vacuum->parameters.outIoName());
        parameters.setPickarmVaccumSensorName(this->pick_arm->picker->vacuum->parameters.inIoName());
    }
    //Align the motor name to lens loader module
    if (pick_arm->picker->motor_t)
       parameters.setMotorTName(this->pick_arm->picker->motor_t->Name());
    if (pick_arm->motor_x_tray)
        parameters.setMotorTrayName(this->pick_arm->motor_x_tray->Name());
    if (pick_arm->motor_x)
        parameters.setMotorXName(this->pick_arm->motor_x->Name());
    if (pick_arm->motor_y)
        parameters.setMotorYName(this->pick_arm->motor_y->Name());
    if (pick_arm->picker->motor_z)
        parameters.setMotorZName(this->pick_arm->picker->motor_z->Name());
    this->lut = lut;
}

void LensLoaderModule::loadJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LENS_PICKARM_PARAMS", &parameters);
    temp_map.insert("LUT_PR_POSITION1", &lut_pr_position1);
    temp_map.insert("LUT_PR_POSITION2", &lut_pr_position2);
    temp_map.insert("LUT_CAMERA_POSITION1", &lut_camera_position);
    temp_map.insert("LUT_PICKE_POSITION1", &lut_picker_position);
    temp_map.insert("LENS_UPDNLOOK_OFFSET", &lens_updnlook_offset);
    temp_map.insert("CAMERA_TO_PICKER_OFFSET", &camera_to_picker_offset);
    PropertyBase::loadJsonConfig(file_name, temp_map);
}

void LensLoaderModule::saveJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LENS_PICKARM_PARAMS", &parameters);
    temp_map.insert("LUT_PR_POSITION1", &lut_pr_position1);
    temp_map.insert("LUT_PR_POSITION2", &lut_pr_position2);
    temp_map.insert("LUT_CAMERA_POSITION1", &lut_camera_position);
    temp_map.insert("LUT_PICKE_POSITION1", &lut_picker_position);
    temp_map.insert("LENS_UPDNLOOK_OFFSET", &lens_updnlook_offset);
    temp_map.insert("CAMERA_TO_PICKER_OFFSET", &camera_to_picker_offset);
    PropertyBase::saveJsonConfig(file_name, temp_map);
}
void LensLoaderModule::receiveLoadLensRequst(bool need_lens,int ng_lens,int ng_len_tray)
{
    qInfo("receiveLensRequst need_lens %d ng_lens %d ng_len_tray %d",need_lens,ng_lens,ng_len_tray);
    QMutexLocker temp_locker(&lut_mutex);
    if(states.loadingLens())
        return;
    states.setNeedLoadLens(need_lens);
    if(ng_lens >= 0)
    {
        states.setLutHasNgLens(true);
        states.setLutNgLensID(ng_lens);
        states.setLutNgTrayID(ng_len_tray);
    }
    states.setLoadingLens(true);
    qInfo("lens requst take effect NeedLoadLens %d LutHasNgLens %d LutNgLensID %d LutNgTrayID %d",
          states.needLoadLens(),states.lutHasNgLens(),states.lutNgLensID(),states.lutNgTrayID());
}

void LensLoaderModule::receiveChangeTrayFinish()
{
    qInfo("LensLoaderModule receiveChangeTrayFinish");
    QMutexLocker temp_locker(&tray_mutex);
    if(states.waitingChangeTray())
    {
        states.setFinishChangeTray(true);
    }
    else
    {
        qInfo("LensLoaderModule receiveChangeTrayFinish but not effect");
    }
}

void LensLoaderModule::run(bool has_material)
{
    is_run = true;
    finish_stop = false;
    int pr_times = 5;
    bool has_task = true;
    bool need_load_lens;
    bool lut_has_ng_lens;

    bool waiting_change_tray = false;
    bool finish_change_tray = false;
    int change_tray_time_out = parameters.changeTrayTimeOut();
    time_label = QTime::currentTime();

    int current_time = parameters.repeatTime();
    int current_count = parameters.testLensCount();
    while (is_run)
    {
        has_task = false;
        {
            QMutexLocker temp_locker(&lut_mutex);
            need_load_lens = states.needLoadLens();
            lut_has_ng_lens = states.lutHasNgLens();
        }

        {
            QMutexLocker temp_locker(&tray_mutex);
            waiting_change_tray = states.waitingChangeTray();
            finish_change_tray = states.finishChangeTray();
        }
        //        if(!has_task)
        {
            QThread::msleep(1);
            if(finish_stop)
            {
                is_run = false;
                break;
            }
        }
        //放NGLens
        if(states.hasTray()&&(!states.allowChangeTray())&&states.hasPickedNgLens())
        {
            has_task = true;
            vacancy_vision->OpenLight();
            int result_tray = 0;
            if(!moveToTrayEmptyPos(states.pickerLensData()["LensId"].toInt(),states.pickerLensData()["TrayId"].toInt(),result_tray))
            {
                AppendError(u8"请手动拿走LPA上的NG Lens后继续!");
                int alarm_id = sendAlarmMessage(u8"已拿走",GetCurrentError());
                waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                states.setHasPickedNgLens(false);
                continue;
            }
            if(has_material&&(!performVacancyPR()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            //vacancy_vision->CloseLight();
            if(!moveToWorkPos(true))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setHasPickedNgLens(false);
                    continue;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }
            if((!placeLensToTray())&&has_material)
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setHasPickedNgLens(false);
                    continue;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }
            tray->setCurrentMaterialState(MaterialState::IsNgLens,result_tray);
            states.setHasPickedNgLens(false);
        }
        if(!is_run)break;
        //检测是否需要换盘
        if((!states.allowChangeTray())&&(!states.hasPickedLens())&&(!states.lutHasNgLens()))
        {
            if((!states.hasTray())||checkNeedChangeTray())
                states.setAllowChangeTray(true);
        }
        //取料
        if(states.hasTray()&&(!states.allowChangeTray())&&checkNeedPickLens()&&(!states.lutHasNgLens())&&(!states.hasPickedNgLens())&&(!states.hasPickedLens()))
        {
            has_task = true;
            if(tray->isTrayNeedChange(states.currentTray()))
            {
                if(states.currentTray() == 0)
                    states.setCurrentTray(1);
                else
                {
                    AppendError(u8"逻辑错误，无可用lens盘");
                    sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::ErrorMustStop);
                    is_run = false;
                    break;
                }
            }
            if((parameters.readyTrayPercent()>0)&&(states.currentTray() == 1)&&tray->checkFinishPercent(1,parameters.readyTrayPercent()))
            {
                sendMessageToModule("LensTrayLoaderModule","ReadyTrayResquest");
            }
            lens_vision->OpenLight();
            if(!moveToNextTrayPos(states.currentTray()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            if(has_material&&(!performLensPR()))
            {
                if(pr_times > 0)
                {
                    pr_times--;
                    tray->setCurrentMaterialState(MaterialState::IsEmpty,states.currentTray());
//                    AppendError(u8"自动重试.");
//                    sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
                    GetCurrentError();
                    continue;
                }
                else
                {
                    pr_times = 5;
                    AppendError(u8"执行lens视觉连续失败5次!");
                    int alarm_id = sendAlarmMessage(CONTINUE_BLIND_SKIPTRAY_OPERATION,GetCurrentError());
                    QString operation = waitMessageReturn(is_run,alarm_id);
                    if(!is_run)break;
                    if(CONTINUE_OPERATION == operation)
                    {
                        tray->setCurrentMaterialState(MaterialState::IsEmpty,states.currentTray());
                        continue;
                    }
                    else if (SKIPTRAY_OPERATION == operation)
                    {
                        tray->setTrayCurrentNg(states.currentTray());
                        continue;
                    }
                }
            }
            else
            {
                //sendAlarmMessage(ErrorLevel::TipNonblock,"");
                // TODO: Add autoprtime same as sensor loader module
            }
            //lens_vision->CloseLight();
            pr_times = 5;
            if(!moveToWorkPos(false))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }

            if((!pickTrayLens())&&has_material)
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                    continue;
                else if(RETRY_OPERATION == operation)
                    continue;
            }
            states.setHasPickedLens(true);
            addCurrentNumber();
            tray->setCurrentMaterialState(MaterialState::IsEmpty,states.currentTray());
            states.setPickerLensData("TrayId",states.currentTray());
            states.setPickerLensData("TrayNumber",tray->getTrayNumber(states.currentTray()));
            states.setPickerLensData("LensId",tray->getCurrentIndex(states.currentTray()));
            qInfo("picked lens index %d, tray_index %d",states.pickedLensID(),states.pickedTrayID());
        }
        if(!is_run)break;
        //等待位置
        if(!movePickerToLUTPos1(true))
        {
            int alarm_id = sendAlarmMessage(CONTINUE_OPERATION,GetCurrentError());
            waitMessageReturn(is_run,alarm_id);
            if(!is_run)break;
        }
        if(!is_run)break;
        //执行换盘
        if(states.allowChangeTray())
        {
            has_task = true;
            if(waiting_change_tray)
            {
                if(finish_change_tray)
                {
                    {
                        QMutexLocker temp_locker(&tray_mutex);
                        states.setFinishChangeTray(false);
                        states.setWaitingChangeTray(false);
                    }
                    states.setHasTray(true);
                    tray->resetTrayState(0);
                    tray->resetTrayState(1);
                    states.setCurrentTray(0);
                    states.setNeedChangTray(false);
                    states.setAllowChangeTray(false);
                    qInfo("finishChangeTray");
                }
                else
                {
                    QThread::msleep(1000);
                    qInfo("waitingChangeTray %d",change_tray_time_out);
                    change_tray_time_out -=1000;
                    if(change_tray_time_out <= 0)
                    {
                        AppendError(QString(u8"等待换盘超时，超时时间%d,请选择是继续等待或者重新换盘").arg(parameters.changeTrayTimeOut()));

                        int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                        QString operation = waitMessageReturn(is_run,alarm_id);
                        if(!is_run)break;
                        if(RETRY_OPERATION == operation)
                        {
                            change_tray_time_out = parameters.changeTrayTimeOut();
                            continue;
                        }
                        states.setWaitingChangeTray(false);
                    }
                }
            }
            else
            {
                {
                    QMutexLocker temp_locker(&tray_mutex);
                    states.setFinishChangeTray(false);
                    sendMessageToModule("LensTrayLoaderModule","ChangeTrayResquest");
                    states.setWaitingChangeTray(true);
                }
                change_tray_time_out = parameters.changeTrayTimeOut();
                qInfo("sendChangeTray");
            }
        }
        if(!is_run)break;
        //放料到LUT
        if(need_load_lens&&states.hasPickedLens())
        {
            qInfo(u8"放料到LUT");
            has_task = true;
            if(!movePickerToLUTPos1(true))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            if((!placeLensToLUT())&&has_material)
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setHasPickedLens(false);
                    continue;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }
                states.setHasPickedLens(false);
                states.setNeedLoadLens(false);
                states.setLutHasLens(true);
                states.copyInLutLensData(states.pickerLensData());
        }
        if(!is_run)break;
        //取NGlens
        if(lut_has_ng_lens&&(!states.hasPickedLens())&&(!states.hasPickedNgLens()))
        {
            addCurrentNgNumber();
            has_task = true;
            lut_lens_vision->OpenLight();
            if(!moveToLUTPRPos2())
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            if(has_material&&(!performLUTLensPR()))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setLutHasNgLens(false);
                    continue;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }
            //lut_lens_vision->CloseLight();
            if(!moveToWorkPos(false))
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(RETRY_OPERATION == operation)
                    continue;
            }
            if((!pickLUTLens())&&has_material)
            {
                int alarm_id = sendAlarmMessage(CONTINUE_RETRY_REJECT_OPERATION,GetCurrentError());
                QString operation = waitMessageReturn(is_run,alarm_id);
                if(!is_run)break;
                if(REJECT_OPERATION == operation)
                {
                    states.setLutHasNgLens(false);
                    continue;
                }
                else if(RETRY_OPERATION == operation)
                    continue;
            }
                states.setHasPickedNgLens(true);
            {
                QMutexLocker temp_locker(&lut_mutex);
                states.setLutHasNgLens(false);
                states.copyInPickerLensData(states.lutNgLensData());
                qInfo("picked ng lens index %d, tray_id %d",states.pickedLensID(),states.pickedTrayID());
            }
        }
        if(!is_run)break;
        //判断是否完成
        if((!states.lutHasNgLens())&&(!states.needLoadLens()))
        {
            if(states.loadingLens())
            {
                QMutexLocker temp_locker(&lut_mutex);
                QJsonObject param;
                param.insert("LutHasLens",states.lutHasLens());
                param.insert("LutLensData",QJsonObject::fromVariantMap(states.lutLensData()));
                sendMessageToModule("LUTModule","FinishLoadLens",param);
                states.setLoadingLens(false);
            }
        }
        if(!is_run)break;
    }
    states.setRunMode(RunMode::Normal);
    qInfo("Lens Loader module end of thread");
}

bool LensLoaderModule::moveToNextTrayPos(int tray_index)
{
    qInfo("moveToNextTrayPos:%d",tray_index);
    bool result = false;
    if(tray->findNextPositionOfInitState(tray_index))
        result = pick_arm->move_XtXYT_Synic(tray->getCurrentPosition(tray_index),parameters.visonPositionX(),parameters.pickTheta());
    if(!result)
        AppendError(QString(u8"移动到%1号盘下一个位置失败").arg(tray_index));
    qInfo(u8"移动到%d号盘下一个位置,返回值%d",tray_index,result);
    return  result;
}

bool LensLoaderModule::moveToLUTPRPos1(bool check_softlanding)
{
    bool result =  pick_arm->move_XYT_Synic(lut_pr_position1.X(),lut_pr_position1.Y(),parameters.placeTheta(),false,check_softlanding);
    if(!result)
        AppendError(QString(u8"移动相机到LUT放Lens位置失败"));
    qInfo(u8"移动相机到LUT放Lens位置,返回值%d",result);
    return result;
}

bool LensLoaderModule::movePickerToLUTPos1(bool check_arrived,bool check_softlanding)
{
//    if(parameters.openTimeLog())
//        qInfo("movePickerToLUTPos1 check_arrived %d check_softlanding %d",check_arrived,check_softlanding);
    bool result =  pick_arm->move_XYT_Synic(lut_pr_position1.X() + camera_to_picker_offset.X(),lut_pr_position1.Y() + camera_to_picker_offset.Y(),parameters.placeTheta(),check_arrived,check_softlanding);
    if(!result)
        AppendError(QString(u8"移动吸头到LUT放Lens位置失败"));
//    if(parameters.openTimeLog())
//        qInfo("movePickerToLUTPos1 result %d",result);
    return result;
}

bool LensLoaderModule::moveToLUTPRPos2(bool check_softlanding)
{
    if(parameters.openTimeLog())
        qInfo("moveToLUTPRPos2 check_softlanding %d",check_softlanding);
    bool result = pick_arm->move_XYT_Synic(lut_pr_position2.X(),lut_pr_position2.Y(),parameters.placeTheta(),false,check_softlanding);
    if(!result)
        AppendError(QString(u8"移动到相机LUT取NgLens位置失败"));
    if(parameters.openTimeLog())
        qInfo(u8"移动相机到LUT取NgLens位置 result  %d",result);
    return result;
}

bool LensLoaderModule::movePickerToLUTPos2(bool check_arrived,bool check_softlanding)
{
    if(parameters.openTimeLog())
        qInfo(u8"移动到吸头LUT取NgLens位置 check_arrived %d check_softlanding %d",check_arrived,check_softlanding);
    bool result = pick_arm->move_XYT_Synic(lut_pr_position2.X() + camera_to_picker_offset.X(),lut_pr_position2.Y()+ camera_to_picker_offset.Y(),parameters.placeTheta(),check_arrived,check_softlanding);
    if(!result)
        AppendError(QString(u8"移动吸头到LUT取NgLens位置失败"));
    if(parameters.openTimeLog())
        qInfo(u8"移动到吸头LUT取NgLens位置 result %",result);
    return result;
}

bool LensLoaderModule::checkNeedChangeTray()
{
    if(tray->isTrayNeedChange(0)&&tray->isTrayNeedChange(1))
        return true;
    return false;
}

bool LensLoaderModule::performLensPR()
{
    qInfo("performLensPR");
    bool result;
    if(states.runMode() == RunMode::NoMaterial)
        result= lens_vision->performNoMaterialPR();
    else
        result = lens_vision->performPR();
    pr_offset = lens_vision->getCurrentResult();
    if(!result)
        AppendError(QString(u8"执行lens视觉失败"));
    qInfo(u8"执行lens视觉,返回值%d",result);
    return result;
}

bool LensLoaderModule::performVacancyPR()
{
    qInfo("performVacancyPR");
    bool result;
    if(states.runMode() == RunMode::NoMaterial)
        result= vacancy_vision->performNoMaterialPR();
    else
        result = vacancy_vision->performPR();
    pr_offset = vacancy_vision->getCurrentResult();
    if(!result)
        AppendError(QString(u8"执行lens料盘空位视觉失败"));
    qInfo(u8"执行lens料盘空位视觉,返回值%d",result);
    return result;
}

bool LensLoaderModule::performLUTPR()
{
    qInfo("performLUTPR");
    bool result;
    if(states.runMode() == RunMode::NoMaterial)
        result= lut_vision->performNoMaterialPR();
    else
        result = lut_vision->performPR();
    pr_offset = lut_vision->getCurrentResult();

    if(!result)
        AppendError(QString(u8"执行LUT空位视觉失败"));
    qInfo(u8"执行LUT空位视觉,返回值%d",result);
    return result;
}

bool LensLoaderModule::performLUTNGSlotPR()
{
    qInfo("performLUTNGSlotPR");
    bool result;
    if(states.runMode() == RunMode::NoMaterial)
        result= lut_ng_slot_vision->performNoMaterialPR();
    else
        result = lut_ng_slot_vision->performPR();
    pr_offset = lut_ng_slot_vision->getCurrentResult();

    if(!result)
        AppendError(QString(u8"执行LUT NG SLOT 空位视觉失败"));
    qInfo(u8"执行LUT NG SLOT 空位视觉,返回值%d",result);
    return result;
}

bool LensLoaderModule::performLUTLensPR()
{
    qInfo("performLUTLensPR");
    bool result;
    if(states.runMode() == RunMode::NoMaterial)
        result= lut_lens_vision->performNoMaterialPR();
    else
        result = lut_lens_vision->performPR();
    pr_offset = lut_lens_vision->getCurrentResult();
    if(!result)
        AppendError(QString(u8"执行LUT上Lens视觉失败"));
    qInfo(u8"执行LUT上Lens视觉,返回值%d",result);
    return result;
}

bool LensLoaderModule::performPickerPR()
{
    bool result = lpa_picker_vision->performPR();
    pr_offset = lpa_picker_vision->getCurrentResult();
    if(!result)
        AppendError(QString(u8"执行吸头视觉失败"));
    qInfo(u8"执行吸头视觉,返回值%d",result);
    return result;
}

bool LensLoaderModule::performUpDownlookDownPR(PrOffset &offset)
{
    bool result = lpa_updownlook_down_vision->performPR(offset, false);
    if(!result)
        AppendError(QString(u8"执行updn downlook视觉失败"));
    qInfo(u8"执行updn downlook视觉,返回值%d",result);
    return result;
}

bool LensLoaderModule::performUpdowlookUpPR(PrOffset &offset)
{
    bool result = lpa_updownlook_up_vision->performPR(offset, false);
    if(!result)
        AppendError(QString(u8"执行updn uplook视觉失败"));
    qInfo(u8"执行updn uplook视觉,返回值%d",result);
    return result;
}

bool LensLoaderModule:: moveToWorkPos(bool check_state)
{
    PrOffset temp(camera_to_picker_offset.X() - pr_offset.X,camera_to_picker_offset.Y() - pr_offset.Y,-pr_offset.Theta);
    bool result = pick_arm->stepMove_XYTp_Pos(temp,false);
    bool check_result = checkPickedLensOrNg(check_state);
    result &= pick_arm->waitStepMove_XYTp();
    if(!result)
        AppendError(QString(u8"移动吸头和视觉偏移失败"));
    qInfo(u8"移动吸头和视觉偏移,返回值%d",result);
    result &= check_result;
    return  result;
}

bool LensLoaderModule::moveToWorkPos()
{
    PrOffset temp(camera_to_picker_offset.X() - pr_offset.X,camera_to_picker_offset.Y() - pr_offset.Y,-pr_offset.Theta);
    bool result = pick_arm->stepMove_XYTp_Pos(temp,true);
    result &= pick_arm->waitStepMove_XYTp();
    if(!result)
        AppendError(QString(u8"移动吸头和视觉偏移失败"));
    qInfo(u8"移动吸头和视觉偏移,返回值%d",result);
    return  result;
}

bool LensLoaderModule::checkPickedLensOrNg(bool check_state)
{
    if(!has_material)
        return true;
    bool result =pick_arm->picker->vacuum->checkHasMaterielSync();
    if(result == check_state||states.runMode() == RunMode::NoMaterial)
        return true;
    QString error = QString(u8"lens吸头上逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool LensLoaderModule::checkLutLens(bool check_state)
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

bool LensLoaderModule::checkLutNgLens(bool check_state)
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

bool LensLoaderModule::moveToPrOffset(bool check_softlanding)
{
    PrOffset temp(- pr_offset.X, - pr_offset.Y,-pr_offset.Theta);
    bool result = pick_arm->stepMove_XYTp_Synic(temp,check_softlanding);
    if(!result)
        AppendError(QString(u8"移动视觉偏移失败"));
    qInfo(u8"移动视觉偏移,返回值%d",result);
    return  result;
}

bool LensLoaderModule::vcmSearchZ(double z,bool is_open)
{
    return pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),parameters.finishDelay(),is_open);
}

bool LensLoaderModule::vcmSearchLUTZ(double z, bool is_open)
{
    return pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),parameters.finishDelay(),is_open,false);
}

bool LensLoaderModule::vcmSearchReturn()
{
    return pick_arm->picker->motor_z->resetSoftLanding();
}

bool LensLoaderModule::pickTrayLens()
{
    qInfo("pickTrayLens");
    bool result = vcmSearchZ(parameters.pickLensZ(),true);
    if(!result)
        AppendError(QString(u8"从lens料盘当前位置取lens失败"));
    qInfo(u8"从lens料盘当前位置取lens,返回值%d",result);
    if(result)
        result &= checkPickedLensOrNg(true);
    return result;
}

bool LensLoaderModule::placeLensToLUT()
{
    if(parameters.openTimeLog())
        qInfo(u8"从当前位置放lens到LUT");
    bool result = vcmSearchLUTZ(parameters.placeLensZ(), false);
    result &= openLoadVacuum();
    result &= vcmSearchReturn();
    if(!result)
        AppendError(QString(u8"从当前位置放lens到LUT失败"));
    if(parameters.openTimeLog())
        qInfo(u8"从当前位置放lens到LUT result %d ",result);
    return result;
}

bool LensLoaderModule::placeLensToLUT2()
{
    if(parameters.openTimeLog())
        qInfo(u8"从当前位置放lens到LUT2");
    bool result = vcmSearchLUTZ(parameters.placeLensZ(), false);
    result &= openUnloadVacuum();
    result &= vcmSearchReturn();
    if(!result)
        AppendError(QString(u8"从当前位置放lens到LUT2失败"));
    if(parameters.openTimeLog())
        qInfo(u8"从当前位置放lens到LUT2 result %d ",result);
    return result;
}

bool LensLoaderModule::pickLUT1Lens()
{
    bool result = vcmSearchLUTZ(parameters.placeLensZ(), true);
    result &= closeLoadVacuum();
    result &= vcmSearchReturn();
    if(!result)
        AppendError(QString(u8"从LUT1当前位置取NgLens失败"));
    qInfo(u8"从LUT1当前位置取NgLens,返回值%d",result);
    if(result)
        result &= checkPickedLensOrNg(true);
    return result;
}

bool LensLoaderModule::pickLUTLens()
{
    bool result = vcmSearchLUTZ(parameters.placeLensZ(), true);
    result &= closeUnloadVacuum();
    result &= vcmSearchReturn();
    if(!result)
        AppendError(QString(u8"从LUT当前位置取NgLens失败"));
    qInfo(u8"从LUT当前位置取NgLens,返回值%d",result);
    if(result)
        result &= checkPickedLensOrNg(true);
    return result;
}

bool LensLoaderModule::placeLensToTray()
{
    bool result =  vcmSearchZ(parameters.pickLensZ(),false);
    if(!result)
        AppendError(QString(u8"从当前位置放NgLens到Lens料盘失败"));
    qInfo(u8"从当前位置放NgLens到Lens料盘,返回值%d",result);
    return result;
}

bool LensLoaderModule::measureHight(bool is_tray)
{
    if(pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),true))
    {
        QThread::msleep(100);
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否应用此高度:%1？").arg(pick_arm->GetSoftladngPosition()))){
            return true;
        }
        if(is_tray)
            parameters.setPickLensZ(pick_arm->GetSoftladngPosition());
        else
            parameters.setPlaceLensZ(pick_arm->GetSoftladngPosition());
        return true;
    }
    AppendError(QString(u8"%1测高失败,测量速度:%2,测量力:%3").arg(is_tray?"Lens料盘":"LUT台").arg(parameters.vcmWorkSpeed()).arg(parameters.vcmWorkForce()));
    qInfo("测高失败 料盘测高:%d 测量速度: %f 测量力: %f",is_tray, parameters.vcmWorkSpeed(), parameters.vcmWorkForce());
    return false;
}

bool LensLoaderModule::moveToTrayEmptyPos(int index, int tray_index,int& result_tray)
{
    qInfo("moveToTrayEmptyPos index %d tray_index %d",index,tray_index);
    result_tray = tray_index;
    if(index < 0 ||tray_index < 0)
    {
        return false;
    }
    bool result = false;
    if(index >= 0 && tray_index >= 0 && tray->getMaterialState(index,tray_index) == MaterialState::IsEmpty)
    {
        tray->setTrayCurrent(index,tray_index);
        result = moveToTrayPos(tray_index);
    }
    else if(tray->findLastPositionOfState(MaterialState::IsEmpty,tray_index))
        result = moveToTrayPos(tray_index);
    else if(tray->findLastPositionOfState(MaterialState::IsEmpty,tray_index == 0?1:0))
    {
        result_tray = tray_index == 0?1:0;
        result = moveToTrayPos(result_tray);
    }
    if(!result)
        AppendError(QString(u8"移动到lens料盘空位失败,起始位置:%1,lens料盘:%2").arg(index).arg(tray_index));
    qInfo(u8"移动到lens料盘空位,起始位置:%d,lens料盘:%d,返回值:%d",index,tray_index,result);
    return result;
}

bool LensLoaderModule::moveToTrayPos(int index, int tray_index)
{
    bool result = pick_arm->move_XtXYT_Synic(tray->getPositionByIndex(index,tray_index),parameters.visonPositionX(),parameters.pickTheta());
    if(!result)
        AppendError(QString(u8"移动到Lens料盘位置失败,位置%1,盘%2").arg(index).arg(tray_index));
    qInfo(u8"移动到lens料盘位置,位置%d,料盘%d,返回值:%d",index,tray_index,result);
    return result;
}

bool LensLoaderModule::moveToTrayPos(int tray_index)
{
    bool result = pick_arm->move_XtXYT_Synic(tray->getCurrentPosition(tray_index),parameters.visonPositionX(),parameters.pickTheta(),false);
    if(!result)
        AppendError(QString(u8"移动到Lens料盘当前位置失败,盘%2").arg(tray_index));
    qInfo(u8"移动到lens料盘当前位置,料盘:%d,返回值:%d",tray_index,result);
    return result;
}

bool LensLoaderModule::moveToStartPos(int tray_index)
{
    bool result = pick_arm->move_XtXY_Synic(tray->getStartPosition(tray_index),parameters.visonPositionX(),true);
    if(!result)
        AppendError(QString(u8"移动到Lens料盘起点位置失败,盘%2").arg(tray_index));
    qInfo(u8"移动到lens料盘起点位置,料盘:%d,返回值:%d",tray_index,result);
    return result;
}

bool LensLoaderModule::moveToTray1EndPos()
{
    bool result = pick_arm->move_XtXY_Synic(tray->getEndPosition(),parameters.visonPositionX(),true);
    if(!result)
        AppendError(QString(u8"移动到1号Lens料盘终点位置失败"));
    qInfo(u8"移动到1号lens料盘终点位置,返回值:%d",result);
    return result;
}

bool LensLoaderModule::moveToUpdownlookDownPos()
{
    bool result = pick_arm->move_XY_Synic(lut_camera_position.X(),lut_camera_position.Y(),true);
    if(!result)
        AppendError(QString(u8"移动到UpDn Downlook位置失败"));
    qInfo(u8"移动到UpDn Downlook位置,返回值:%d",result);
    return result;
}

bool LensLoaderModule::moveToUpdownlookUpPos()
{
    bool result = pick_arm->move_XY_Synic(lut_picker_position.X(),lut_picker_position.Y(),true);
    if(!result)
        AppendError(QString(u8"移动到UpDn Uplook位置失败"));
    qInfo(u8"移动到UpDn Uplook位置,返回值:%d",result);
    return result;
}

bool LensLoaderModule::loadOneLensToLUT()
{
    sendMessageToModule("Sut1Module", "MoveToLoadPos");
    sendMessageToModule("Sut2Module", "MoveToLoadPos");
    lut->moveToLoadPos();
    //Checking 1: LPA has lens already
    if (this->pick_arm->picker->vacuum->checkHasMaterielSync())
    {
        int i = 0;
        do{
            moveToTrayPos(i, 0); i++;
            if(performVacancyPR()) {
                moveToWorkPos(true);
                break;
            }
            if( i == 20) { qInfo("Fail"); return false; }
        } while (i<20);
        placeLensToTray();
    }
    int i = 0;
    do{
        moveToTrayPos(i, 0); i++;
        if(performLensPR()) {
            moveToWorkPos(true);
            break;
        }
        if( i == 20) { qInfo("Fail"); return false; }
    } while (i<20);
    bool ret = pickTrayLens();
    ret &= moveToLUTPRPos1(true);
    ret &= moveToWorkPos();
    ret &= placeLensToLUT();
    return true;
}

bool LensLoaderModule::unloadAllLens()
{
    sendMessageToModule("Sut1Module", "MoveToLoadPos");
    sendMessageToModule("Sut2Module", "MoveToLoadPos");
    lut->moveToLoadPos();
    int i = 0;
    //Checking 1: LPA has lens already
    if (this->pick_arm->picker->vacuum->checkHasMaterielSync())
    {   //ToDo：how to do the indexing tray
        do{
            moveToTrayPos(i, 0); i++;
            if(performVacancyPR()) {
                moveToWorkPos(true);
                break;
            }
            if( i == 19) { qInfo("Fail"); return false; }
        } while (i<19);
        placeLensToTray();
    }
    //Checking 2: LUT1 has lens
    if (lut->load_vacuum->checkHasMaterielSync())
    {
        moveToLUTPRPos1();
        performLUTLensPR();
        moveToWorkPos(true);
        pickLUT1Lens();
        do{
            moveToTrayPos(i, 0); i++;
            if(performVacancyPR()) {
                moveToWorkPos(true);
                break;
            }
            if( i == 19) { qInfo("Fail"); return false; }
        }while(i<19);
        placeLensToTray();
    }

    //Checking 3: LUT2 has lens
    if (lut->unload_vacuum->checkHasMaterielSync())
    {
        moveToLUTPRPos2();
        performLUTLensPR();
        moveToWorkPos(true);
        pickLUTLens();
        do{
            moveToTrayPos(i, 0); i++;
            if(performVacancyPR()) {
                moveToWorkPos(true);
                break;
            }
            if( i == 19) { qInfo("Fail"); return false; }
        }while (i<19);
        placeLensToTray();
    }

    //Checking 4: LUT Go To AA1 Unpick
    lut->moveToAA1UnPickLens();
    lut->moveToLoadPos();
    if (lut->unload_vacuum->checkHasMaterielSync())
    {
        moveToLUTPRPos2();
        performLUTLensPR();
        moveToWorkPos(true);
        pickLUTLens();
        do{
            moveToTrayPos(i, 0); i++;
            if(performVacancyPR()) {
                moveToWorkPos(true);
                break;
            }
            if( i == 19) { qInfo("Fail"); return false; }
        }while (i<19);
        placeLensToTray();
    }
    //Checking 5: LUT Go To AA2 Unpick
    lut->moveToAA2UnPickLens();
    lut->moveToLoadPos();
    if (lut->unload_vacuum->checkHasMaterielSync())
    {
        moveToLUTPRPos2();
        performLUTLensPR();
        moveToWorkPos(true);
        pickLUTLens();
        do{
            moveToTrayPos(i, 0); i++;
            if(performVacancyPR()) {
                moveToWorkPos(true);
                break;
            }
            if( i == 19) { qInfo("Fail"); return false; }
        }while (i<19);
        placeLensToTray();
    }
    lut->closeLoadVacuum();
    lut->closeUnloadVacuum();
    return true;
}

void LensLoaderModule::addCurrentNumber()
{
    parameters.setCurrentNumber(parameters.currentNumber() + 1);
    parameters.setLensYield((parameters.currentNumber() - parameters.currentNgNumber())/parameters.currentNumber());
    parameters.setAccumulatredHour(parameters.accumulatredHour() + getHourSpace(time_label));

}

void LensLoaderModule::addCurrentNgNumber()
{
    parameters.setCurrentNgNumber(parameters.currentNgNumber() + 1);
}

void LensLoaderModule::clearNumber()
{
    parameters.setCurrentNumber(0);
    parameters.setCurrentNgNumber(0);
    parameters.setAccumulatredHour(0);
    parameters.setLensYield(0);
    parameters.setLensUPH(0);
}

void LensLoaderModule::updateNumber()
{
    double lens_yield = parameters.currentNumber() - parameters.currentNgNumber();
    lens_yield /= parameters.currentNumber();
    lens_yield = round(lens_yield*1000)/1000;
    parameters.setLensYield(lens_yield);
    parameters.setAccumulatredHour(parameters.accumulatredHour() + getHourSpace(time_label));
    int uph = 0;
    if(parameters.accumulatredHour()>0)
        uph = qRound(parameters.currentNumber()/parameters.accumulatredHour());
    parameters.setLensUPH(uph);
}

double LensLoaderModule::getHourSpace(QTime time_label)
{
    int temp_minute = QTime::currentTime().minute() - time_label.minute();
    if(temp_minute < 0)temp_minute = 60 - temp_minute;
    double space = temp_minute/60.0;
    int temp_second =   QTime::currentTime().second() - time_label.second();
    space += temp_second/3600.0;
    return space;
}

bool LensLoaderModule::openLoadVacuum()
{
    if(states.runMode() == RunMode::NoMaterial)
        return load_vacuum->SetSimulation(true);
    else
        return load_vacuum->Set(true);
}

bool LensLoaderModule::closeLoadVacuum()
{
    if(states.runMode() == RunMode::NoMaterial)
        return load_vacuum->SetSimulation(false);
    else
        return load_vacuum->Set(false);
}

bool LensLoaderModule::openUnloadVacuum()
{
    if(states.runMode() == RunMode::NoMaterial)
        return unload_vacuum->SetSimulation(true);
    else
        return unload_vacuum->Set(true);
}

bool LensLoaderModule::closeUnloadVacuum()
{
    if(states.runMode() == RunMode::NoMaterial)
        return unload_vacuum->SetSimulation(false);
    else
        return unload_vacuum->Set(false);
}

bool LensLoaderModule::isRunning()
{
    return is_run;
}

void LensLoaderModule::startWork(int run_mode)
{
    if(run_mode == RunMode::AAFlowChartTest) return;
    if(run_mode == RunMode::UNLOAD_ALL_LENS) {
        this->unloadAllLens();
        return;
    }

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

    if(run_params.contains("HandlyChangeLens"))
    {
        states.setHandlyChangeLens(run_params["HandlyChangeLens"].toBool());
    }
    else
    {
        sendAlarmMessage(OK_OPERATION,u8"启动参数HandlyChangeLens缺失.启动失败.",ErrorLevel::ErrorMustStop);
        return;
    }
    if(states.handlyChangeLens())
        return;

    has_material = true;
    qInfo("Lensloader start run_mode :%d",run_mode);
    if(run_mode == RunMode::Normal)
        run(true);
    else if(run_mode == RunMode::NoMaterial)
    {
        has_material = false;
        run(false);
    }
    else if(run_mode == RunMode::VibrationTest)
    {
        is_run = true;
        while(is_run) {
            moveToLUTPRPos1();
            QThread::msleep(2000);
            moveToTrayPos(0,0);
            QThread::msleep(2000);
        }
    }
}


void LensLoaderModule::stopWork(bool wait_finish)
{
    qInfo("LensLoader stop");
    //    if(wait_finish)
    //        finish_stop = true;
    //    else
    is_run = false;
}

void LensLoaderModule::resetLogic()
{
    if(is_run)return;
    qInfo("LensLoaderModule reset logic");
    states.setHasTray(false);//lens tray ready

    states.setLutHasLens(false);
    states.setLutHasNgLens(false);
    states.setNeedLoadLens(false);
    states.setCurrentTray(0);
    states.setNeedChangTray(false);
    states.setAllowChangeTray(false);
    states.setHasPickedLens(false);
    states.setHasPickedNgLens(false);
    states.setLutTrayID(-1);
    states.setLutLensID(-1);
    states.setPickedTrayID(-1);
    states.setPickedLensID(-1);
    states.setLutNgTrayID(-1);
    states.setLutNgLensID(-1);
    states.setLoadingLens(false);
    states.setWaitingChangeTray(false);
    tray->resetTrayState();
    tray->resetTrayState(1);
}

void LensLoaderModule::performHandlingOperation(int cmd,QVariant param)
{
    qInfo("performHandling %d %s",cmd, param.toString().toStdString().c_str());
    QJsonObject paramJson = getJsonObjectFromString(param.toString());
    int col = paramJson["col"].toInt(-1);
    int row = paramJson["row"].toInt(-1);
    int tray_index = paramJson["tray_index"].toInt(-1);
    bool skip_dialog = paramJson["skip_dialog"].toBool(false);
    if (tray_index >= 0 && col >=0 && row >= 0 )  {
        qInfo("Params col: %d row: %d tray_index:%d", col, row, tray_index);
        this->tray->setTrayCurrent(col, row, tray_index);
    }
    bool result;
    int temp_value = 10;
    if (cmd == HandleMarcoAction::CLEARANCE)
    {
        this->unloadAllLens();
        sendAlarmMessage(OK_OPERATION,"Lens Clearance Done",ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    if (cmd == HandleMarcoAction::LOAD_ONE_LENS_TO_LUT)
    {
        this->loadOneLensToLUT();
        sendAlarmMessage(OK_OPERATION,"Load lens to LUT Done",ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    if(cmd%temp_value == HandlePosition::LUT_POS1)
        result = moveToLUTPRPos1(true);
    else if(cmd%temp_value == HandlePosition::LUT_POS2)
        result = moveToLUTPRPos2(true);
    else if(cmd%temp_value == HandlePosition::LENS_TRAY1)
        result = moveToTrayPos(0);
    else if(cmd%temp_value == HandlePosition::LENS_TRAY2)
        result = moveToTrayPos(1);
    else if(cmd%temp_value == HandlePosition::LENS_TRAY1_START_POS)
        result = moveToStartPos(0);
    else if(cmd%temp_value == HandlePosition::LENS_TRAY2_START_POS)
        result = moveToStartPos(1);
    else if(cmd%temp_value == HandlePosition::LENS_TRAY1_END_POS)
        result = moveToTray1EndPos();
    else if(cmd%temp_value == HandlePosition::UPDOWNLOOK_DOWN_POS)
        result = moveToUpdownlookDownPos();
    else if(cmd%temp_value == HandlePosition::UPDOWNLOOK_UP_POS)
        result = moveToUpdownlookUpPos();
    else
        result = true;
    cmd =cmd/temp_value*temp_value;
    temp_value = 100;
    if(!result)
    {
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    if(cmd%temp_value == HandlePR::RESET_PR)
        pr_offset.ReSet();
    else if(cmd%temp_value == HandlePR::LENS_PR)
        result = performLensPR();
    else if(cmd%temp_value == HandlePR::VACANCY_PR)
        result = performVacancyPR();
    else if(cmd%temp_value == HandlePR::LUT_PR)
        result = performLUTPR();
    else if(cmd%temp_value == HandlePR::LUT_LENS_PR)
        result = performLUTLensPR();
    else if(cmd%temp_value == HandlePR::PICKER_PR)
        result = performPickerPR();
    else if(cmd%temp_value == HandlePR::LUT_NG_SLOT_PR)
        result = performLUTNGSlotPR();
    else
        result = true;
    if(!result)
    {
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    cmd =cmd/temp_value*temp_value;
    temp_value = 1000;
    if(cmd%temp_value == HandleToWorkPos::ToWork)
        result = moveToWorkPos();
    else if(cmd%temp_value == HandleToWorkPos::ToPrOffset){
        if(emit sendMsgSignal(tr(u8""),tr(u8"是否移动？"))){
            result = moveToPrOffset(true);
        }
    }
    if(!result)
    {
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    cmd =cmd/temp_value*temp_value;
    temp_value = 10000;
    if(cmd%temp_value == HandlePickerAction::PICK_LENS_FROM_TRAY){
        if (skip_dialog){
            result = pickTrayLens();
        }
        else if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = pickTrayLens();
        }
    }
    else if(cmd%temp_value == HandlePickerAction::PLACE_LENS_TO_LUT){
        if (skip_dialog){
            result = placeLensToLUT();
        }
        else if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = placeLensToLUT();
        }
    }
    else if(cmd%temp_value == HandlePickerAction::PLACE_LENS_TO_LUT2){
        if (skip_dialog){
            result = placeLensToLUT2();
        }
        else if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = placeLensToLUT2();
        }
    }
    else if(cmd%temp_value == HandlePickerAction::PICK_NG_LENS_FROM_LUT1){
        if (skip_dialog){
            result = pickLUT1Lens();
        }
        else if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = pickLUT1Lens();
        }
    }
    else if(cmd%temp_value == HandlePickerAction::PICK_NG_LENS_FROM_LUT2){
        if (skip_dialog){
            result = pickLUTLens();
        }
        else if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = pickLUTLens();
        }
    }
    else if(cmd%temp_value == HandlePickerAction::PLACE_NG_LENS_TO_TRAY){
        if (skip_dialog){
            result = placeLensToTray();
        }
        else if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = placeLensToTray();
        }
    }
    else if(cmd%temp_value == HandlePickerAction::MeasureLensInLUT)
        result = measureHight(false);
    else if(cmd%temp_value == HandlePickerAction::MeasureLensInTray)
        result = measureHight(true);
    else
        result = true;
    if(!result)
    {
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
        is_handling = false;
        return;
    }
    is_handling = false;
}

QString LensLoaderModule::getUuid(bool is_right, int current_count, int current_time)
{
    QString uuid = "";
    if(is_right)
        uuid.append("right_");
    else
        uuid.append("left_");
    uuid.append(QString::number(current_count + 1));
    uuid.append("_");
    uuid.append(QString::number(parameters.repeatTime() - current_time + 1));
    return uuid;
}

void LensLoaderModule::recordLutLensPr(QString uuid)
{
    qInfo("recordLutLensPr %s",uuid.toStdString().c_str());
    QVariantMap* temp = &prRecordMap[uuid];
    temp->insert("3_lut_lens_offset.x",pr_offset.X);
    temp->insert("3_lut_lens_offset.y",pr_offset.Y);
    temp->insert("3_lut_lens_offset.t",pr_offset.Theta);
}

void LensLoaderModule::recordLutVacancyPr(QString uuid)
{
    qInfo("recordLutVacancyPr %s",uuid.toStdString().c_str());
    QVariantMap* temp = &prRecordMap[uuid];
    temp->insert("2_lut_vacancy_offset.x",pr_offset.X);
    temp->insert("2_lut_vacancy_offset.y",pr_offset.Y);
    temp->insert("2_lut_vacancy_offset.t",pr_offset.Theta);
}

void LensLoaderModule::recordTrayLensPr(QString uuid)
{
    qInfo("recordTrayLensPr %s",uuid.toStdString().c_str());
    QStringList list = uuid.split('_');
    QVariantMap* temp = new QVariantMap();
    temp->insert("_station",list[0]);
    temp->insert("_lens_id",list[1]);
    temp->insert("_test_time",list[2]);
    temp->insert("1_tray_lens_offset.x",pr_offset.X);
    temp->insert("1_tray_lens_offset.y",pr_offset.Y);
    temp->insert("1_tray_lens_offset.t",pr_offset.Theta);
    prRecordMap.insert(uuid,*temp);
}

void LensLoaderModule::recordTrayVacancyPr(QString uuid)
{
    qInfo("recordTrayVacancyPr %s",uuid.toStdString().c_str());
    QVariantMap* temp = &prRecordMap[uuid];
    temp->insert("6_tray_vacancy_offset.x",pr_offset.X);
    temp->insert("6_tray_vacancy_offset.y",pr_offset.Y);
    temp->insert("6_tray_vacancy_offset.t",pr_offset.Theta);
    postCSVDataToUnit(uuid,prRecordMap[uuid]);
    saveUnitDataToCSV(uuid);
    prRecordMap.remove(uuid);
}

void LensLoaderModule::recordNgLensPr(QString uuid)
{
    qInfo("recordNgLensPr %s",uuid.toStdString().c_str());
    QVariantMap* temp = &prRecordMap[uuid];
    temp->insert("5_ng_lens_offset.x",pr_offset.X);
    temp->insert("5_ng_lens_offset.y",pr_offset.Y);
    temp->insert("5_ng_lens_offset.t",pr_offset.Theta);
    postCSVDataToUnit(uuid,prRecordMap[uuid]);
    saveUnitDataToCSV(uuid);
    prRecordMap.remove(uuid);
}

void LensLoaderModule::receivceModuleMessage(QVariantMap message)
{
    qInfo("receive module message %s",TcpMessager::getStringFromQvariantMap(message).toStdString().c_str());

    QString origin_module = message["OriginModule"].toString();
    if(origin_module == "LUTModule")
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString() == "LoadLensRequest")
            {
                if(states.loadingLens())
                {
                    qInfo("repeat lens request!");
                    return;
                }
                if(message.contains("LutHasLens"))
                    states.setLutHasLens(message["LutHasLens"].toBool());
                else
                    qInfo("message LutHasLens miss.");
                if(message.contains("NeedLoadLens"))
                    states.setNeedLoadLens(message["NeedLoadLens"].toBool());
                else
                    qInfo("message NeedLoadLens miss.");
                if(message.contains("LutNgLensData"))
                {
                    states.setLutHasNgLens(true);
                    states.copyInLutNgLensData(message["MaterialData"].toMap());
                }
                if(message.contains("TaskNumber"))
                    states.setTaskOfStations(message["TaskNumber"].toInt());
                states.setLoadingLens(true);
                qInfo("lens requst take effect NeedLoadLens %d LutHasNgLens %d LutNgLensID %d LutNgTrayID %d",
                      states.needLoadLens(),states.lutHasNgLens(),states.lutNgLensID(),states.lutNgTrayID());
            }
            else if(message["Message"].toString()=="UnloadMode")
            {
                states.setStationsUnload(true);
            }
            else if(message["Message"].toString()=="NormalMode")
            {
                states.setStationsUnload(false);
            }
            else
            {
                qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            }
        }
    }
    else if(origin_module == "LensTrayLoaderModule")
    {
        if(message.contains("Message"))
        {
            if(message["Message"].toString() == "FinishChangeTray")
            {
                if(!states.waitingChangeTray())
                {
                    qInfo("receive message FinishChangeTray when not changing tray.");
                    return;
                }
                states.setFinishChangeTray(true);
            }
            else if(message["Message"].toString()=="UnloadMode")
            {
                states.setStationsUnload(true);
            }
            else if(message["Message"].toString()=="NormalMode")
            {
                states.setStationsUnload(false);
            }
            else
            {
                qInfo("module message error %s",message["Message"].toString().toStdString().c_str());
            }
        }
    }
}

PropertyBase *LensLoaderModule::getModuleState()
{
    return &states;
}

bool LensLoaderModule::checkNeedPickLens()
{
    if(states.stationsUnload())
        return false;
    if((states.taskOfStations() == 1)&&(!states.needLoadLens()))
        return false;
    return true;
}

QMap<QString, PropertyBase *> LensLoaderModule::getModuleParameter()
{
    QMap<QString,PropertyBase*> temp_map;

    temp_map.insert("LENS_PICKARM_PARAMS", &parameters);
    temp_map.insert("LUT_PR_POSITION1", &lut_pr_position1);
    temp_map.insert("LUT_PR_POSITION2", &lut_pr_position2);
    temp_map.insert("LUT_CAMERA_POSITION1", &lut_camera_position);
    temp_map.insert("LUT_PICKE_POSITION1", &lut_picker_position);
    temp_map.insert("LENS_UPDNLOOK_OFFSET", &lens_updnlook_offset);
    temp_map.insert("CAMERA_TO_PICKER_OFFSET", &camera_to_picker_offset);
    temp_map.insert("END_POSITION", &this->tray->first_tray_end_position);
    for (int i = 0; i < this->tray->standards_parameters.trayCount(); ++i)
    {
        temp_map.insert(QString("TRAY").append(QString::number(i + 1)).append("_PARAMETER"), this->tray->parameters[i]);
        temp_map.insert(QString("TRAY").append(QString::number(i + 1)).append("_START_POSITION"), &this->tray->parameters[i]->tray_start_position);
    }

    return temp_map;
}

void LensLoaderModule::setModuleParameter(QMap<QString, PropertyBase *> parameters)
{

}
