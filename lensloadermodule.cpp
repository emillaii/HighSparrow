#include "lensloadermodule.h"

LensLoaderModule::LensLoaderModule(QString name):ThreadWorkerBase (name)
{

}

void LensLoaderModule::Init(LensPickArm *pick_arm, MaterialTray *lens_tray, MaterialCarrier *lut_carrier, VisionLocation *lens_vision, VisionLocation *vacancy_vision, VisionLocation *lut_vision, VisionLocation *lut_lens_vision)
{
    this->pick_arm = pick_arm;
    this->lens_tray = lens_tray;
    this->lut_carrier = lut_carrier;
    this->lens_vision = lens_vision;
    this->vacancy_vision = vacancy_vision;
    this->lut_vision = lut_vision;
    this->lut_lens_vision = lut_lens_vision;
}

QString LensLoaderModule::GetCurrentError()
{
    AppendLineError(pick_arm->GetCurrentError()/*&
                                                                                                                    lens_tray->GetCurrentError()&
                                                                                                                    lut_carrier->GetCurrentError()&
                                                                                                                    lens_vision->GetCurrentError()&
                                                                                                                    vacancy_vision->GetCurrentError()&
                                                                                                                    lut_vision->GetCurrentError()*/
                    );
    return  ErrorBase::GetCurrentError();
}

ErrorLevel LensLoaderModule::GetCurrentErrorLevel()
{
    setCurrentErrorLevel(pick_arm->GetCurrentErrorLevel()/*&
                                                                                                                                                      lens_tray->GetCurrentErrorLevel()&
                                                                                                                                                      lut_carrier->GetCurrentErrorLevel()&
                                                                                                                                                      lens_vision->GetCurrentErrorLevel()&
                                                                                                                                                      vacancy_vision->GetCurrentErrorLevel()&
                                                                                                                                                      lut_vision->GetCurrentErrorLevel()*/
                         );
    return  ErrorBase::GetCurrentErrorLevel();
}

void LensLoaderModule::ResetLogic()
{

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

void LensLoaderModule::performHandling(int cmd, int &finished_type)
{
    emit sendHandlingOperation(cmd,finished_type);
}

void LensLoaderModule::run(bool has_material)
{
    is_run = true;
    while (is_run)
    {
        //放NGLens
        if(states.hasPickedNgLens())
        {
            if(moveToTrayPos(picked_material.material_id,picked_material.tray_id)&&
                    performVacancyPR()&&
                    moveToWorkPos()&&
                    placeLensToTray())
            {
                states.setHasPickedNgLens(false);
            }
            else
            {
                ErrorLevel temp_level = GetCurrentErrorLevel();
                sendAlarmMessage(GetCurrentErrorLevel(),GetCurrentError());
                //todo
            }
        }
        //取料
        if((!states.hasPickedNgLens())&&(!states.hasPickedLens()))
        {
            if(moveToNextTrayPos(states.currentTray())&&
                    performLensPR()&&
                    moveToWorkPos()&&
                    pickTrayLens())
            {
                picked_material.tray_id = states.currentTray();
                picked_material.material_id = lens_tray->getCurrentIndex(states.currentTray());
                states.setHasPickedLens(true);

            }
            else {
                ErrorLevel temp_level = GetCurrentErrorLevel();
                sendAlarmMessage(GetCurrentErrorLevel(),GetCurrentError());
                //todo
            }
        }
        //等待
        if(!moveToLUTPRPos1())
        {
            ErrorLevel temp_level = GetCurrentErrorLevel();
            sendAlarmMessage(GetCurrentErrorLevel(),GetCurrentError());
            //todo
        }
        //放料到LUT
        if(states.needloadLens()&&states.hasPickedLens())
        {
            if(moveToWorkPos()&&placeLensToLUT())
            {
                lut_material.tray_id = picked_material.tray_id;
                lut_material.material_id = picked_material.material_id;
                states.setneedloadLens(false);
                states.setHasPickedLens(false);
            }
            else
            {
                ErrorLevel temp_level = GetCurrentErrorLevel();
                sendAlarmMessage(GetCurrentErrorLevel(),GetCurrentError());
                //todo
            }
        }
        //取NGlens
        if(states.lutHasNgLens()&&(!states.hasPickedLens())&&(!states.hasPickedNgLens()))
        {
            if(moveToLUTPRPos2()&&
                    performLUTPR()&&
                    moveToWorkPos()&&
                    placeLensToLUT())
            {
                picked_material.tray_id = picked_material.tray_id;
                picked_material.material_id = picked_material.material_id;
                states.setLutHasNgLens(false);
                states.setHasPickedNgLens(true);
            }
            else
            {
                ErrorLevel temp_level = GetCurrentErrorLevel();
                sendAlarmMessage(GetCurrentErrorLevel(),GetCurrentError());
                //todo
            }
        }
    }
}

bool LensLoaderModule::moveToNextTrayPos(int tray_index)
{
    if(lens_tray->findNextPositionOfInitState(tray_index))
    {
        return  pick_arm->move_XtXY_Synic(lens_tray->getCurrentPosition(tray_index),parameters.visonPositionX());
    }
    return  false;
}

bool LensLoaderModule::moveToLUTPRPos1(bool check_softlanding)
{
    return  pick_arm->move_XtXY_Synic(lut_pr_position1.ToPointF(),parameters.visonPositionX(),check_softlanding);
}

bool LensLoaderModule::moveToLUTPRPos2(bool check_softlanding)
{
    return  pick_arm->move_XtXY_Synic(lut_pr_position2.ToPointF(),parameters.visonPositionX(),check_softlanding);
}

bool LensLoaderModule::performLensPR()
{
    return  lens_vision->performPR(pr_offset);
}

bool LensLoaderModule::performVacancyPR()
{
    return  vacancy_vision->performPR(pr_offset);
}

bool LensLoaderModule::performLUTPR()
{
    return lut_vision->performPR(pr_offset);
}

void LensLoaderModule::resetPR()
{
    pr_offset.X = 0;
    pr_offset.Y = 0;
    pr_offset.Theta = 0;
}

bool LensLoaderModule::moveToWorkPos(bool check_softlanding)
{
    PrOffset temp(lut_camera_position.X() - lut_picker_position.X()- pr_offset.X,
                    lut_camera_position.X() - lut_picker_position.X()- pr_offset.X,pr_offset.Theta);
    return  pick_arm->stepMove_XYTp_Synic(temp,check_softlanding);
}

bool LensLoaderModule::vcmSearchZ(double z,bool check_softlanding)
{
    return pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),check_softlanding);
}

bool LensLoaderModule::pickTrayLens(bool check_softlanding)
{
    return vcmSearchZ(parameters.pickLensZ(),check_softlanding);
}

bool LensLoaderModule::placeLensToLUT(bool check_softlanding)
{
    return vcmSearchZ(parameters.placeLensZ(),check_softlanding);
}

bool LensLoaderModule::pickLUTLens(bool check_softlanding)
{
    return vcmSearchZ(parameters.placeLensZ(),check_softlanding);
}

bool LensLoaderModule::placeLensToTray(bool check_softlanding)
{
    return vcmSearchZ(parameters.pickLensZ(),check_softlanding);
}

bool LensLoaderModule::measureHight(bool is_tray)
{
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
    return pick_arm->move_XtXY_Synic(lens_tray->getPositionByIndex(index,tray_index),parameters.visonPositionX());
}

bool LensLoaderModule::moveToTrayPos(int tray_index)
{
    return  pick_arm->move_XtXY_Synic(lens_tray->getCurrentPosition(tray_index),parameters.visonPositionX(),true);
}

bool LensLoaderModule::moveToStartPos(int tray_index)
{
    return pick_arm->move_XtXY_Synic(lens_tray->getStartPosition(tray_index),parameters.visonPositionX(),true);
}

bool LensLoaderModule::moveToTray1EndPos()
{
    return pick_arm->move_XtXY_Synic(lens_tray->getEndPosition(),parameters.visonPositionX(),true);
}

bool LensLoaderModule::isRunning()
{
    return is_run;
}

void LensLoaderModule::startWork(bool reset_logic, int run_mode)
{
    if(reset_logic)ResetLogic();
    if(run_mode&RunMode::Normal)run(true);
    else if(run_mode&RunMode::NoMaterial)run(false);
}

void LensLoaderModule::stopWork(bool wait_finish)
{
    is_run = false;
}

void LensLoaderModule::performHandlingOperation(int cmd, int &finished_type)
{
    bool result;
    if(cmd&HandlePosition::LUT_POS1)
        result = moveToLUTPRPos1(true);
    else if(cmd&HandlePosition::LUT_POS2)
        result = moveToLUTPRPos2(true);
    else if(cmd&HandlePosition::LENS_TRAY1)
        result = moveToTrayPos(0);
    else if(cmd&HandlePosition::LENS_TRAY2)
        result = moveToTrayPos(1);
    else if(cmd&HandlePosition::LENS_TRAY1_START_POS)
        result = moveToStartPos(0);
    else if(cmd&HandlePosition::LENS_TRAY2_START_POS)
        result = moveToStartPos(1);
    else if(cmd&HandlePosition::LENS_TRAY1_END_POS)
        result = moveToTray1EndPos();
    else
        result = true;
    if(!result)
    {
        finished_type = FinishedType::Alarm;
        return;
    }
    if(cmd&HandlePR::LUT_PR)
        resetPR();
    else if(cmd&HandlePR::LUT_PR)
        result = performLensPR();
    else if(cmd&HandlePR::VACANCY_PR)
        result = performVacancyPR();
    else if(cmd&HandlePR::LUT_PR)
        result = performLUTPR();
    else
        result = true;
    if(!result)
    {
        finished_type = FinishedType::Alarm;
        return;
    }
    if(cmd&HandleToWorkPos::ToWork)
        result = moveToWorkPos();
    if(!result)
    {
        finished_type = FinishedType::Alarm;
        return;
    }
    if(cmd&handlePickerAction::PICK_LENS_FROM_TRAY)
        result = pickTrayLens(true);
    else if(cmd&handlePickerAction::PLACE_LENS_TO_LUT)
        result = placeLensToLUT(true);
    else if(cmd&handlePickerAction::PICK_NG_LENS_FROM_LUT)
        result = pickLUTLens(true);
    else if(cmd&handlePickerAction::PLACE_NG_LENS_TO_TRAY)
        result = placeLensToTray();
    else if(cmd&handlePickerAction::MeasureLensInLUT)
        result = measureHight(false);
    else if(cmd&handlePickerAction::MeasureLensInTray)
        result = measureHight(true);
    else
        result = true;
    if(!result)
    {
        finished_type = FinishedType::Alarm;
        return;
    }
    finished_type = FinishedType::Success;
}
