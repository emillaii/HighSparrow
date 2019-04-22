#include "lenspickarmmodule.h"

LensPickArmModule::LensPickArmModule(QString name):ThreadWorkerBase (name)
{

}

QString LensPickArmModule::GetCurrentError()
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

ErrorLevel LensPickArmModule::GetCurrentErrorLevel()
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

void LensPickArmModule::ResetLogic()
{

}

void LensPickArmModule::loadJsonConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LENS_PICKARM_PARAMS", &module_parameters);
    temp_map.insert("LUT_PR_POSITION1", &lut_pr_position1);
    temp_map.insert("LUT_PR_POSITION2", &lut_pr_position2);
    temp_map.insert("LUT_CAMERA_POSITION1", &lut_camera_position);
    PropertyBase::loadJsonConfig("config//lensPickArmModule.json", temp_map);
}

void LensPickArmModule::saveJsonConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LENS_PICKARM_PARAMS", &module_parameters);
    temp_map.insert("LUT_PR_POSITION1", &lut_pr_position1);
    temp_map.insert("LUT_PR_POSITION2", &lut_pr_position2);
    temp_map.insert("LUT_CAMERA_POSITION1", &lut_camera_position);
    PropertyBase::saveJsonConfig("config//lensPickArmModule.json", temp_map);
}

void LensPickArmModule::run(bool has_material)
{
    is_run = true;
    while (is_run)
    {
        //放NGLens
        if(module_states.hasPickedNgLens())
        {
            if(moveToTrayPos(picked_material.material_id,picked_material.tray_id)&&
                    performVacancyPR()&&
                    moveToWorkPos()&&
                    placeLensToTray())
            {
                module_states.setHasPickedNgLens(false);
            }
            else
            {
                ErrorLevel temp_level = GetCurrentErrorLevel();
                sendAlarmMessage(GetCurrentErrorLevel(),GetCurrentError());
                //todo
            }
        }
        //取料
        if((!module_states.hasPickedNgLens())&&(!module_states.hasPickedLens()))
        {
            if(moveToNextTrayPos(module_states.currentTray())&&
                    performLensPR()&&
                    moveToWorkPos()&&
                    pickTrayLens())
            {
                picked_material.tray_id = module_states.currentTray();
                picked_material.material_id = lens_tray->getCurrentIndex(module_states.currentTray());
                module_states.setHasPickedLens(true);

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
        if(module_states.needloadLens()&&module_states.hasPickedLens())
        {
            if(moveToWorkPos()&&placeLensToLUT())
            {
                lut_material.tray_id = picked_material.tray_id;
                lut_material.material_id = picked_material.material_id;
                module_states.setneedloadLens(false);
                module_states.setHasPickedLens(false);
            }
            else
            {
                ErrorLevel temp_level = GetCurrentErrorLevel();
                sendAlarmMessage(GetCurrentErrorLevel(),GetCurrentError());
                //todo
            }
        }
        //取NGlens
        if(module_states.lutHasNgLens()&&(!module_states.hasPickedLens())&&(!module_states.hasPickedNgLens()))
        {
            if(moveToLUTPRPos2()&&
                    performLUTPR()&&
                    moveToWorkPos()&&
                    placeLensToLUT())
            {
                picked_material.tray_id = picked_material.tray_id;
                picked_material.material_id = picked_material.material_id;
                module_states.setLutHasNgLens(false);
                module_states.setHasPickedNgLens(true);
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

bool LensPickArmModule::moveToNextTrayPos(int tray_index)
{
    if(lens_tray->findNextPositionOfInitState(tray_index))
    {
        return  pick_arm->move_XtXY_Synic(lens_tray->getCurrentPosition(tray_index),module_parameters.visonPositionX());
    }
    return  false;
}

bool LensPickArmModule::moveToLUTPRPos1()
{
    return  pick_arm->move_XtXY_Synic(lut_pr_position1.ToPointF(),module_parameters.visonPositionX());
}

bool LensPickArmModule::moveToLUTPRPos2()
{
    return  pick_arm->move_XtXY_Synic(lut_pr_position2.ToPointF(),module_parameters.visonPositionX());
}

bool LensPickArmModule::performLensPR()
{
    return  lens_vision->performPR(pr_offset);
}

bool LensPickArmModule::performVacancyPR()
{
    return  vacancy_vision->performPR(pr_offset);
}

bool LensPickArmModule::performLUTPR()
{
    return  lut_vision->performPR(pr_offset);
}

bool LensPickArmModule::moveToWorkPos()
{
    mPositionT temp(lut_camera_position.X() - lut_picker_position.X()- pr_offset.X,
                    lut_camera_position.X() - lut_picker_position.X()- pr_offset.X,pr_offset.Theta);
    return  pick_arm->stepMove_XYTp_Synic(temp);
}

bool LensPickArmModule::vcmSearchZ(double z)
{
    return pick_arm->ZSerchByForce(module_parameters.vcmWorkSpeed(),module_parameters.vcmWorkForce(),z,module_parameters.vcmMargin());
}

bool LensPickArmModule::pickTrayLens()
{
    return vcmSearchZ(module_parameters.pickLensZ());
}

bool LensPickArmModule::placeLensToLUT()
{
    return vcmSearchZ(module_parameters.placeLensZ());
}

bool LensPickArmModule::pickLUTLens()
{
    return vcmSearchZ(module_parameters.placeLensZ());
}

bool LensPickArmModule::placeLensToTray()
{
    return vcmSearchZ(module_parameters.pickLensZ());
}

bool LensPickArmModule::moveToTrayPos(int index, int tray_index)
{
    return true;
}

bool LensPickArmModule::moveToTrayPos(int column, int row, int tray_index)
{
    return true;
}

bool LensPickArmModule::moveToPerformPR(int selected_pr, bool back_position, mPositionT &result)
{
    return true;
}

bool LensPickArmModule::moveToPRResult(const mPositionT pr_result)
{
    return true;
}

bool LensPickArmModule::moveToPerformAction(int selected_action, bool do_vision)
{
    return true;
}

bool LensPickArmModule::isRunning()
{
    return is_run;
}

void LensPickArmModule::startWork(bool reset_logic, int run_mode)
{
    if(reset_logic)ResetLogic();
    if(run_mode&RunMode::Normal)run(true);
    else if(run_mode&RunMode::NoMaterial)run(false);

}

void LensPickArmModule::stopWork(bool wait_finish)
{

}
