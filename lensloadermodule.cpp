#include "lensloadermodule.h"

LensLoaderModule::LensLoaderModule(QString name):ThreadWorkerBase (name)
{

}

void LensLoaderModule::Init(LensPickArm *pick_arm, MaterialTray *lens_tray, MaterialCarrier *lut_carrier, VisionLocation *lens_vision, VisionLocation *vacancy_vision, VisionLocation *lut_vision, VisionLocation *lut_lens_vision,VisionLocation *lpa_updownlook_up_vision, VisionLocation *lpa_updownlook_down_vision)
{
    parts.clear();
    this->pick_arm = pick_arm;
    parts.append(this->pick_arm);
    this->tray = lens_tray;
    parts.append(this->tray);
    this->lut_carrier = lut_carrier;
    parts.append(this->lut_carrier);
    this->lens_vision = lens_vision;
    parts.append(this->lens_vision);
    this->vacancy_vision = vacancy_vision;
    parts.append(this->vacancy_vision);
    this->lut_vision = lut_vision;
    parts.append(this->lut_vision);
    this->lut_lens_vision = lut_lens_vision;
    parts.append(this->lut_lens_vision);
    this->lpa_updownlook_up_vision = lpa_updownlook_up_vision;
    parts.append(this->lpa_updownlook_up_vision);
    this->lpa_updownlook_down_vision = lpa_updownlook_down_vision;
    parts.append(this->lpa_updownlook_down_vision);
    lens_tray->resetTrayState(0);
    lens_tray->resetTrayState(1);
}

void LensLoaderModule::resetLogic()
{
    receiveLensRequst(true,0,0);
}

void LensLoaderModule::loadJsonConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LENS_PICKARM_PARAMS", &parameters);
    temp_map.insert("LUT_PR_POSITION1", &lut_pr_position1);
    temp_map.insert("LUT_PR_POSITION2", &lut_pr_position2);
    temp_map.insert("LUT_CAMERA_POSITION1", &lut_camera_position);
    temp_map.insert("LUT_PICKE_POSITION1", &lut_picker_position);
    PropertyBase::loadJsonConfig("config//lensLoaderModule.json", temp_map);
}

void LensLoaderModule::saveJsonConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LENS_PICKARM_PARAMS", &parameters);
    temp_map.insert("LUT_PR_POSITION1", &lut_pr_position1);
    temp_map.insert("LUT_PR_POSITION2", &lut_pr_position2);
    temp_map.insert("LUT_CAMERA_POSITION1", &lut_camera_position);
    temp_map.insert("LUT_PICKE_POSITION1", &lut_picker_position);
    PropertyBase::saveJsonConfig("config//lensLoaderModule.json", temp_map);
}

void LensLoaderModule::performHandling(int cmd)
{
    emit sendHandlingOperation(cmd);
    qInfo("emit performHandling %d",cmd);
}

void LensLoaderModule::receiveLensRequst(bool need_lens,int ng_lens,int ng_len_tray)
{
    states.setneedloadLens(need_lens);
    if(ng_lens >= 0)
    {
        states.setLutHasNgLens(true);
        lut_ng_material.tray_id = ng_len_tray;
        lut_ng_material.material_id = ng_lens;
    }

}

void LensLoaderModule::run(bool has_material)
{
    return ;
    is_run = true;
    int pr_times = 5;
    bool has_task = true;
    while (is_run)
    {
//        if(!has_task)
            QThread::msleep(1000);
        has_task = false;
        //放NGLens
        if(states.hasPickedNgLens())
        {
            has_task = true;
            if(!moveToTrayPos(picked_material.material_id,picked_material.tray_id))
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
            if(!placeLensToTray())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                waitMessageReturn(is_run);
            }
            tray->setCurrentMaterialState(MaterialState::IsNg,states.currentTray());
            states.setHasPickedNgLens(false);
            if(!is_run)break;
        }
        //取料
        if((!finish_stop)&&(!states.hasPickedNgLens())&&(!states.hasPickedLens()))
        {
            has_task = true;
            if(!moveToNextTrayPos(states.currentTray()))
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!performLensPR())&&has_material)
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

            if(!pickTrayLens())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(!waitMessageReturn(is_run))
                    states.setHasPickedLens(true);
            }
            else
                states.setHasPickedLens(true);
            tray->setCurrentMaterialState(MaterialState::IsEmpty,states.currentTray());
            picked_material.tray_id = states.currentTray();
            picked_material.material_id = tray->getCurrentIndex(states.currentTray());
            if(!is_run)break;
        }
        //等待位置
        if(!moveToLUTPRPos1())
        {
            sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
            is_run = false;
            break;
        }
        //放料到LUT
        if(states.needloadLens()&&states.hasPickedLens())
        {
            has_task = true;
            if(!moveToLUTPRPos1())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if(!moveToWorkPos())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }


            if(!placeLensToLUT())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                    states.setHasPickedLens(false);
                else
                {
                    states.setneedloadLens(false);
                    states.setHasPickedLens(false);
                }
            }
            else
            {
                states.setneedloadLens(false);
                states.setHasPickedLens(false);
            }
            lut_material.tray_id = picked_material.tray_id;
            lut_material.material_id = picked_material.material_id;
        }
        //取NGlens
        if(states.lutHasNgLens()&&(!states.hasPickedLens())&&(!states.hasPickedNgLens()))
        {
            has_task = true;
            if(!moveToLUTPRPos2())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!performLUTPR())&&has_material)
            {
                AppendError(u8"NG视觉失败！");
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
            if(!pickLUTLens())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                    states.setLutHasNgLens(false);
                else
                {
                    states.setLutHasNgLens(false);
                    states.setHasPickedNgLens(true);
                }
            }
            else
            {
                states.setLutHasNgLens(false);
                states.setHasPickedNgLens(true);
            }
            picked_material.tray_id = lut_ng_material.tray_id;
            picked_material.material_id = lut_ng_material.material_id;
        }
    }
    qInfo("LensLoader stoped");
}

bool LensLoaderModule::moveToNextTrayPos(int tray_index)
{
    qInfo("moveToNextTrayPos:%d",tray_index);
    if(tray->findNextPositionOfInitState(tray_index))
    {
        return  pick_arm->move_XtXY_Synic(tray->getCurrentPosition(tray_index),parameters.visonPositionX());
    }
    return  false;
}

bool LensLoaderModule::moveToLUTPRPos1(bool check_softlanding)
{
    qInfo("moveToLUTPRPos1");
    return  pick_arm->move_XtXY_Synic(lut_pr_position1.ToPointF(),parameters.visonPositionX(),check_softlanding);
}

bool LensLoaderModule::moveToLUTPRPos2(bool check_softlanding)
{
    qInfo("moveToLUTPRPos2");
    return  pick_arm->move_XtXY_Synic(lut_pr_position2.ToPointF(),parameters.visonPositionX(),check_softlanding);
}

bool LensLoaderModule::performLensPR()
{
    qInfo("performLensPR");
    Sleep(500); //Position settling
    return  lens_vision->performPR(pr_offset);
}

bool LensLoaderModule::performVacancyPR()
{
    qInfo("performVacancyPR:%d");
    Sleep(500); //Position settling
    return  vacancy_vision->performPR(pr_offset);
}

bool LensLoaderModule::performLUTPR()
{
    qInfo("performLUTPR");
    return lut_vision->performPR(pr_offset);
}

bool LensLoaderModule::performUpDownlookDownPR()
{
    qInfo("performUpDownlookDownPR");
    return lpa_updownlook_down_vision->performPR(pr_offset);
}

bool LensLoaderModule::performUpdowlookUpPR()
{
    qInfo("performUpdowlookUpPR");
    return lpa_updownlook_up_vision->performPR(pr_offset);
}
bool LensLoaderModule::moveToWorkPos(bool check_softlanding)
{
    qInfo("moveToWorkPos");
    PrOffset temp(lut_picker_position.X() - lut_camera_position.X() - pr_offset.X,
                    lut_picker_position.Y() - lut_camera_position.Y() - pr_offset.Y,pr_offset.Theta);
    qInfo("offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XYTp_Synic(temp,check_softlanding);
    qInfo("Step move finished");
    pr_offset.ReSet();
    return  result;
}

bool LensLoaderModule::vcmSearchZ(double z,bool is_open,bool check_softlanding)
{
    qInfo("vcmSearchZ limit: %f timeout: %d", z, parameters.finishDelay());

    return pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),parameters.finishDelay(),is_open);
}

bool LensLoaderModule::pickTrayLens(bool check_softlanding)
{
    qInfo("pickTrayLens");
//    bool result = pick_arm->Move_SZ_Sync(parameters.pickLensZ());
//    result = pick_arm->pickarmVaccum(true);
//    Sleep(250);
//    pick_arm->Move_SZ_Sync(parameters.placeLensZ())
    bool result = vcmSearchZ(parameters.pickLensZ(),true,check_softlanding);
    return result;
}

bool LensLoaderModule::placeLensToLUT(bool check_softlanding)
{
    qInfo("placeLensToLUT");
    return vcmSearchZ(parameters.placeLensZ(), false,check_softlanding);
}

bool LensLoaderModule::pickLUTLens(bool check_softlanding)
{
    qInfo("pickLUTLens");
    return vcmSearchZ(parameters.placeLensZ(),false,check_softlanding);
}

bool LensLoaderModule::placeLensToTray(bool check_softlanding)
{
    qInfo("placeLensToTray");
    return vcmSearchZ(parameters.pickLensZ(),check_softlanding);
}

bool LensLoaderModule::measureHight(bool is_tray)
{
    qInfo("measureHight speed: %f force: %f", parameters.vcmWorkSpeed(), parameters.vcmWorkForce());
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

bool LensLoaderModule::moveToTrayPos(int index, int tray_index)
{
    qInfo("moveToTrayPos");
    return pick_arm->move_XtXY_Synic(tray->getPositionByIndex(index,tray_index),parameters.visonPositionX());
}

bool LensLoaderModule::moveToTrayPos(int tray_index)
{
    qInfo("moveToTrayPos %d",tray_index);
    return  pick_arm->move_XtXY_Synic(tray->getCurrentPosition(tray_index),parameters.visonPositionX(),false);
}

bool LensLoaderModule::moveToStartPos(int tray_index)
{
    qInfo("moveToStartPos%d",tray_index);
    return pick_arm->move_XtXY_Synic(tray->getStartPosition(tray_index),parameters.visonPositionX(),true);
}

bool LensLoaderModule::moveToTray1EndPos()
{
    qInfo("moveToTray1EndPos");
    return pick_arm->move_XtXY_Synic(tray->getEndPosition(),parameters.visonPositionX(),true);
}

bool LensLoaderModule::moveToUpdownlookDownPos()
{
    qInfo("moveToUpdownlookDownPos");
    return pick_arm->move_XY_Synic(lut_camera_position.X(),lut_camera_position.Y(),true);
}

bool LensLoaderModule::moveToUpdownlookUpPos()
{
    qInfo("moveToUpdownlookUpPos");
    return pick_arm->move_XY_Synic(lut_picker_position.X(),lut_picker_position.Y(),true);
}

bool LensLoaderModule::isRunning()
{
    return is_run;
}

void LensLoaderModule::startWork(bool reset_logic, int run_mode)
{
    qInfo("Lensloader start reset:%d run_mode :%d",reset_logic,run_mode);
    if(reset_logic)resetLogic();
    if(run_mode == RunMode::Normal)run(true);
    else if(run_mode == RunMode::NoMaterial)run(false);
}

void LensLoaderModule::stopWork(bool wait_finish)
{
    qInfo("LensLoader stop");
//    if(wait_finish)
//        finish_stop = true;
//    else
        is_run = false;
}

void LensLoaderModule::performHandlingOperation(int cmd)
{
    qInfo("performHandling %d",cmd);
    bool result;
    if(cmd%10 == HandlePosition::LUT_POS1)
        result = moveToLUTPRPos1(true);
    else if(cmd%10 == HandlePosition::LUT_POS2)
        result = moveToLUTPRPos2(true);
    else if(cmd%10 == HandlePosition::LENS_TRAY1)
        result = moveToTrayPos(0);
    else if(cmd%10 == HandlePosition::LENS_TRAY2)
        result = moveToTrayPos(1);
    else if(cmd%10 == HandlePosition::LENS_TRAY1_START_POS)
        result = moveToStartPos(0);
    else if(cmd%10 == HandlePosition::LENS_TRAY2_START_POS)
        result = moveToStartPos(1);
    else if(cmd%10 == HandlePosition::LENS_TRAY1_END_POS)
        result = moveToTray1EndPos();
    else if(cmd%10 == HandlePosition::UPDOWNLOOK_DOWN_POS)
        result = moveToUpdownlookDownPos();
    else if(cmd%10 == HandlePosition::UPDOWNLOOK_UP_POS)
        result = moveToUpdownlookUpPos();
    else
        result = true;
    cmd =cmd/10*10;
    if(!result)
    {
//        finished_type = FinishedType::Alarm;
        return;
    }
    if(cmd%100 == HandlePR::RESET_PR)
        pr_offset.ReSet();
    else if(cmd%100 == HandlePR::LENS_PR)
        result = performLensPR();
    else if(cmd%100 == HandlePR::VACANCY_PR)
        result = performVacancyPR();
    else if(cmd%100 == HandlePR::LUT_PR)
        result = performLUTPR();
    else if(cmd%100 == HandlePR::UPDOWNLOOK_DOWN_PR)
        result = performUpDownlookDownPR();
    else if(cmd%100 == HandlePR::UPDOWNLOOK_UP_PR)
        result = performUpdowlookUpPR();
    else
        result = true;
    if(!result)
    {
//        finished_type = FinishedType::Alarm;
        return;
    }
    cmd =cmd/100*100;
    if(cmd%1000 == HandleToWorkPos::ToWork)
        result = moveToWorkPos();
    if(!result)
    {
//        finished_type = FinishedType::Alarm;
        qInfo("Move To Work Pos fail");
        return;
    }
    cmd =cmd/1000*1000;
    qInfo("cmd : %d", cmd);
    if(cmd%10000 == handlePickerAction::PICK_LENS_FROM_TRAY)
        result = pickTrayLens(true);
    else if(cmd%10000 == handlePickerAction::PLACE_LENS_TO_LUT)
        result = placeLensToLUT(true);
    else if(cmd%10000 == handlePickerAction::PICK_NG_LENS_FROM_LUT)
        result = pickLUTLens(true);
    else if(cmd%10000 == handlePickerAction::PLACE_NG_LENS_TO_TRAY)
        result = placeLensToTray();
    else if(cmd%10000 == handlePickerAction::MeasureLensInLUT)
        result = measureHight(false);
    else if(cmd%10000 == handlePickerAction::MeasureLensInTray)
        result = measureHight(true);
    else
        result = true;
    if(!result)
    {
//        finished_type = FinishedType::Alarm;
        return;
    }
//    finished_type = FinishedType::Success;
}
