#include "lensloadermodule.h"

LensLoaderModule::LensLoaderModule(QString name):ThreadWorkerBase (name)
{

}

void LensLoaderModule::Init(LensPickArm *pick_arm, MaterialTray *lens_tray, MaterialCarrier *lut_carrier,XtVacuum* load_vacuum, XtVacuum* unload_vacuum, VisionLocation *lens_vision, VisionLocation *vacancy_vision, VisionLocation *lut_vision, VisionLocation *lut_lens_vision,VisionLocation *lpa_picker_vision,VisionLocation *lpa_updownlook_up_vision, VisionLocation *lpa_updownlook_down_vision)
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
    int pr_times = 5;
    bool has_task = true;
    bool need_load_lens;
    bool lut_has_ng_lens;

    bool waiting_change_tray = false;
    bool finish_change_tray = false;
    int change_tray_time_out = parameters.changeTrayTimeOut();
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
        if(!has_task)
            QThread::msleep(1000);
        //取料
        if((!finish_stop)&&states.hasTray()&&(!states.allowChangeTray())&&(!states.hasPickedNgLens())&&(!states.hasPickedLens()))
        {
            has_task = true;
            if(!moveToNextTrayPos(states.currentTray()))
            {
                AppendError("moveToTray Pos fail!");
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!performLensPR())&&has_material)
            {
                if(pr_times > 0)
                {
                    pr_times--;
                    tray->setCurrentMaterialState(MaterialState::IsEmpty,states.currentTray());
                    states.setPickedTrayID(states.currentTray());
                    states.setPickedLensID(tray->getCurrentIndex(states.currentTray()));
                    continue;
                }
                else
                {
                    pr_times = 5;
                    AppendError("performLensPR fial 5 times!");
                    sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                    waitMessageReturn(is_run);
                    if(is_run)break;
                }
            }
            if(!moveToWorkPos())
            {
                AppendError("moveToWorkPos fail!");
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }

            if((!pickTrayLens())&&has_material)
            {
                AppendError("pickTrayLens fail!");
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(!waitMessageReturn(is_run))
                    states.setHasPickedLens(true);
            }
            else
                states.setHasPickedLens(true);
            tray->setCurrentMaterialState(MaterialState::IsEmpty,states.currentTray());
            states.setPickedTrayID(states.currentTray());
            states.setPickedLensID(tray->getCurrentIndex(states.currentTray()));
            if(!is_run)break;
        }
        //放NGLens
        if(states.hasTray()&&(!states.allowChangeTray())&&states.hasPickedNgLens())
        {
            has_task = true;
            if(!moveToTrayEmptyPos(states.pickedLensID(),states.pickedTrayID()))
            {
                AppendError("去放NG Lens位置失败，请手动拿走LPA 上的NG Lens!");
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                waitMessageReturn(is_run);
                //todo加物料检测
                states.setHasPickedNgLens(false);
                continue;
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
                AppendError("moveToWorkPos fail!");
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!placeLensToTray())&&has_material)
            {
                AppendError("placeLensToTray fail!");
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                waitMessageReturn(is_run);
            }
            tray->setCurrentMaterialState(MaterialState::IsNg,states.currentTray());
            states.setHasPickedNgLens(false);
            if(!is_run)break;
        }
        //检测是否需要换盘
        if((!states.allowChangeTray()))
        {
            if((!states.hasTray())||checkNeedChangeTray())
                states.setAllowChangeTray(true);
        }
        //等待位置
        if(!moveToLUTPRPos1())
        {
            sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
            is_run = false;
            break;
        }
        //执行换盘
        if(states.allowChangeTray())
        {
            if(waiting_change_tray)
            {
                if(finish_change_tray)
                {
                    {
                        QMutexLocker temp_locker(&tray_mutex);
                        states.setFinishChangeTray(false);
                        states.setWaitingChangeTray(false);
                    }
                    tray->resetTrayState(0);
                    tray->resetTrayState(1);
                    states.setNeedChangTray(false);
                    states.setAllowChangeTray(false);
                    qInfo("finishChangeTray");
                }
                else
                {
                    if(!has_task)
                        QThread::msleep(1000);
                    qInfo("waitingChangeTray");
                    change_tray_time_out -=1000;
                    if(change_tray_time_out<=0)
                    {
                        AppendError(QString(u8"等待换盘超时，超时时间%d,请选择是继续等待或者重新换盘").arg(parameters.changeTrayTimeOut()));
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,GetCurrentError());
                        if(waitMessageReturn(is_run))
                            states.setWaitingChangeTray(false);
                        else
                            change_tray_time_out = parameters.changeTrayTimeOut();
                    }
                }
            }
            else
            {
                {
                    QMutexLocker temp_locker(&tray_mutex);
                    states.setFinishChangeTray(false);
                    emit sendChangeTrayRequst();
                    states.setWaitingChangeTray(true);
                }
                change_tray_time_out = parameters.changeTrayTimeOut();
                qInfo("sendChangeTray");
            }
        }
        //放料到LUT
        if(need_load_lens&&states.hasPickedLens())
        {
            has_task = true;
            if(!moveToLUTPRPos1())
            {
                AppendError("moveToLUTPRPos1 fail!");
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
            if((!placeLensToLUT())&&has_material)
            {
                AppendError("placeLensToLUT fail!");
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                    states.setHasPickedLens(false);
                else
                {
                    need_load_lens = false;
                    states.setHasPickedLens(false);
                }
            }
            else
            {
                need_load_lens = false;
                states.setHasPickedLens(false);
            }
            {
                QMutexLocker temp_locker(&lut_mutex);
                states.setNeedLoadLens(need_load_lens);
                states.setLutTrayID(states.pickedTrayID());
                states.setLutLensID(states.pickedLensID());
            }
        }
        //取NGlens
        if(lut_has_ng_lens&&(!states.hasPickedLens())&&(!states.hasPickedNgLens()))
        {
            has_task = true;
            if(!moveToLUTPRPos2())
            {
                AppendError("moveToLUTPRPos2 fail!");
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
                AppendError("moveToWorkPos fail!");
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!pickLUTLens())&&has_material)
            {
                AppendError("pickLUTLens fail!");
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                    lut_has_ng_lens = false;
                else
                    states.setHasPickedNgLens(true);
            }
            else
            {
                lut_has_ng_lens = false;
                states.setHasPickedNgLens(true);
            }
            {
                QMutexLocker temp_locker(&lut_mutex);
                states.setLutHasNgLens(lut_has_ng_lens);
                states.setPickedTrayID(states.lutNgTrayID());
                states.setPickedLensID(states.lutNgLensID());
            }
        }
        //判断是否完成
        if((!states.lutHasNgLens())&&(!states.needLoadLens()))
        {
            if(states.loadingLens())
            {
                QMutexLocker temp_locker(&lut_mutex);
                emit sendLoadLensFinish(states.lutLensID(),states.lutTrayID());
                states.setLoadingLens(false);
            }
        }
    }
    qInfo("LensLoader stoped");
}

bool LensLoaderModule::moveToNextTrayPos(int tray_index)
{
    qInfo("moveToNextTrayPos:%d",tray_index);
    if(tray->findNextPositionOfInitState(tray_index))
    {
//        return  pick_arm->move_XtXY_Synic(tray->getCurrentPosition(tray_index),parameters.visonPositionX());
        return  pick_arm->move_XtXYT_Synic(tray->getCurrentPosition(tray_index),parameters.visonPositionX(),parameters.pickTheta());
    }
    return  false;
}

bool LensLoaderModule::moveToLUTPRPos1(bool check_softlanding)
{
    return  pick_arm->move_XYT_Synic(lut_pr_position1.X(),lut_pr_position1.Y(),parameters.placeTheta(),check_softlanding);

}

bool LensLoaderModule::moveToLUTPRPos2(bool check_softlanding)
{
    return  pick_arm->move_XYT_Synic(lut_pr_position2.X(),lut_pr_position2.Y(),parameters.placeTheta(),check_softlanding);
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
    Sleep(500); //Position settling
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

bool LensLoaderModule::performPickerPR()
{
    return lpa_picker_vision->performPR(pr_offset);
}

bool LensLoaderModule::performUpDownlookDownPR(PrOffset &offset)
{
    return lpa_updownlook_down_vision->performPR(offset, false);
}

bool LensLoaderModule::performUpdowlookUpPR(PrOffset &offset)
{
    return lpa_updownlook_up_vision->performPR(offset, false);
}
//lut_camera_position = lut_picker_position - camera_to_picker_offset
//void LensLoaderModule::calculateCameraToPickerOffset()
//{
//    lut_camera_position.setX(lut_picker_position.X() - camera_to_picker_offset.X());
//    lut_camera_position.setY(lut_picker_position.Y() - camera_to_picker_offset.Y());
//    qInfo("Camera to picker offset x: %f y:%f", lut_camera_position.X(), lut_camera_position.Y());
//}
bool LensLoaderModule::moveToWorkPos(bool check_softlanding)
{
    PrOffset temp(camera_to_picker_offset.X() - pr_offset.X,camera_to_picker_offset.Y() - pr_offset.Y,pr_offset.Theta);
    bool result = pick_arm->stepMove_XYTp_Synic(temp,check_softlanding);
    return  result;
}

bool LensLoaderModule::moveToPrOffset(bool check_softlanding)
{
    PrOffset temp(- pr_offset.X, - pr_offset.Y,pr_offset.Theta);
    bool result = pick_arm->stepMove_XYTp_Synic(temp,check_softlanding);
    return  result;
}

bool LensLoaderModule::vcmSearchZ(double z,bool is_open)
{
    return pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),parameters.finishDelay(),is_open);
}

bool LensLoaderModule::pickTrayLens()
{
    qInfo("pickTrayLens");
//    bool result = pick_arm->Move_SZ_Sync(parameters.pickLensZ());
//    result = pick_arm->pickarmVaccum(true);
//    Sleep(250);
//    pick_arm->Move_SZ_Sync(parameters.placeLensZ())
    bool result = vcmSearchZ(parameters.pickLensZ(),true);
    return result;
}

bool LensLoaderModule::placeLensToLUT()
{
    qInfo("placeLensToLUT");
//    bool result = pick_arm->stepMove_T_Syncic(parameters.placeTheta());
    this->lut_carrier->vacuum->Set(true);
    bool result = vcmSearchZ(parameters.placeLensZ(), false);
//    return result = pick_arm->stepMove_T_Syncic(-parameters.placeTheta());
    return result;
}

bool LensLoaderModule::pickLUTLens()
{
    qInfo("pickLUTLens");
    return vcmSearchZ(parameters.placeLensZ(),true);
}

bool LensLoaderModule::placeLensToTray()
{
    qInfo("placeLensToTray");
    return vcmSearchZ(parameters.pickLensZ(),false);
}

bool LensLoaderModule::measureHight(bool is_tray)
{
    qInfo("measureHight speed: %f force: %f", parameters.vcmWorkSpeed(), parameters.vcmWorkForce());
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
    return false;
}

bool LensLoaderModule::moveToTrayEmptyPos(int index, int tray_index)
{
    if(index >= 0 && tray_index >= 0 && tray->getMaterialState(index,tray_index) == MaterialState::IsEmpty)
        return moveToTrayPos(index,tray_index);
    if(tray->findLastPositionOfState(MaterialState::IsEmpty,tray_index))
        return moveToTrayPos(tray_index);
    if(tray_index == 0&&tray->findLastPositionOfState(MaterialState::IsEmpty,1))
        return moveToTrayPos(1);
    return false;
}

bool LensLoaderModule::moveToTrayPos(int index, int tray_index)
{
    qInfo("moveToTrayPos");
//    return pick_arm->move_XtXY_Synic(tray->getPositionByIndex(index,tray_index),parameters.visonPositionX());
    return pick_arm->move_XtXYT_Synic(tray->getPositionByIndex(index,tray_index),parameters.visonPositionX(),parameters.pickTheta());
}

bool LensLoaderModule::moveToTrayPos(int tray_index)
{
    qInfo("moveToTrayPos %d",tray_index);
//    return  pick_arm->move_XtXY_Synic(tray->getCurrentPosition(tray_index),parameters.visonPositionX(),false);
    return  pick_arm->move_XtXYT_Synic(tray->getCurrentPosition(tray_index),parameters.visonPositionX(),parameters.pickTheta(),false);
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

void LensLoaderModule::startWork(int run_mode)
{
    qInfo("Lensloader start run_mode :%d",run_mode);
    if(run_mode == RunMode::Normal||run_mode == RunMode::OnllyLeftAA||run_mode == RunMode::OnlyRightAA)run(true);
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

void LensLoaderModule::resetLogic()
{
    if(is_run)return;
    states.setHasTray(true);
    states.setLutHasLens(false);
    states.setLutHasNgLens(false);
    states.setNeedLoadLens(false);
    states.setCurrentTray(false);
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
    tray->resetTrayState();
    tray->resetTrayState(1);
}

void LensLoaderModule::performHandlingOperation(int cmd)
{
    qInfo("performHandling %d",cmd);
    bool result;
//    int curren
    if(cmd%10 == HandlePosition::LUT_POS1){
        qInfo(u8"移动LPA到LUT上放Lens位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            result = moveToLUTPRPos1(true);
//        }
    }
    else if(cmd%10 == HandlePosition::LUT_POS2){
        qInfo(u8"移动LPA到LUT上取NG Lens位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            result = moveToLUTPRPos2(true);
//        }
    }
    else if(cmd%10 == HandlePosition::LENS_TRAY1){
        qInfo(u8"移动LPA到1号Lens料盘当前lens位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            result = moveToTrayPos(0);
//        }
    }
    else if(cmd%10 == HandlePosition::LENS_TRAY2){
        qInfo(u8"移动LPA到2号Lens料盘当前lens位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            result = moveToTrayPos(1);
//        }
    }
    else if(cmd%10 == HandlePosition::LENS_TRAY1_START_POS){
        qInfo(u8"移动LPA到1号Lens料盘起始位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            result = moveToStartPos(0);
//        }
    }
    else if(cmd%10 == HandlePosition::LENS_TRAY2_START_POS){
        qInfo(u8"移动LPA到2号Lens料盘起始位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            result = moveToStartPos(1);
//        }
    }
    else if(cmd%10 == HandlePosition::LENS_TRAY1_END_POS){
        qInfo(u8"移动LPA到1号Lens料盘终点位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            result = moveToTray1EndPos();
//        }
    }
    else if(cmd%10 == HandlePosition::UPDOWNLOOK_DOWN_POS){
        qInfo(u8"移动LPA到updownlook位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            result = moveToUpdownlookDownPos();
//        }
    }
    else if(cmd%10 == HandlePosition::UPDOWNLOOK_UP_POS){
        qInfo(u8"移动LPA到吸头视觉位置");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否移动？"))){
            result = moveToUpdownlookUpPos();
//        }
    }
    else
        result = true;
    cmd =cmd/10*10;
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        return;
    }
    if(cmd%100 == HandlePR::RESET_PR){
        qInfo(u8"PR偏差置零");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作？"))){
            pr_offset.ReSet();
//        }
    }
    else if(cmd%100 == HandlePR::LENS_PR){
        qInfo(u8"执行Lens视觉");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作？"))){
            result = performLensPR();
//        }
    }
    else if(cmd%100 == HandlePR::VACANCY_PR){
        qInfo(u8"执行料盘空位视觉");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作？"))){
            result = performVacancyPR();
//        }
    }
    else if(cmd%100 == HandlePR::LUT_PR){
        qInfo(u8"执行LUT定位视觉");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作？"))){
            result = performLUTPR();
//        }
    }
    else if(cmd%100 == HandlePR::PICKER_PR){
        qInfo(u8"执行吸头视觉");
//        if(emit sendMsgSignal(tr(u8"提示"),tr(u8"是否执行操作？"))){
            result = performPickerPR();
//        }
    }
    else
        result = true;
    if(!result)
    {
//        finished_type = FinishedType::Alarm;
        return;
    }
    cmd =cmd/100*100;
    if(cmd%1000 == HandleToWorkPos::ToWork){
        if(emit sendMsgSignal(tr(u8""),tr(u8"是否移动？"))){
            result = moveToWorkPos();
        }
    }
    else if(cmd%1000)
    if(!result)
    {
//        finished_type = FinishedType::Alarm;
        qInfo("Move To Work Pos fail");
        return;
    }
    cmd =cmd/1000*1000;
    qInfo("cmd : %d", cmd);
    if(cmd%10000 == handlePickerAction::PICK_LENS_FROM_TRAY){
        if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = pickTrayLens();
        }
    }
    else if(cmd%10000 == handlePickerAction::PLACE_LENS_TO_LUT){
        if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = placeLensToLUT();
        }
    }
    else if(cmd%10000 == handlePickerAction::PICK_NG_LENS_FROM_LUT){
        if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = pickLUTLens();
        }
    }
    else if(cmd%10000 == handlePickerAction::PLACE_NG_LENS_TO_TRAY){
        if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = placeLensToTray();
        }
    }
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
