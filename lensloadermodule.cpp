﻿#include "lensloadermodule.h"

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
            if(!moveToTrayEmptyPos(states.pickedLensID(),states.pickedTrayID(),result_tray))
            {
                AppendError(" 请手动拿走LPA上的NG Lens后继续!");
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                waitMessageReturn(is_run);
                //todo加物料检测
                states.setHasPickedNgLens(false);
                continue;
            }
            if(has_material&&(!performVacancyPR()))
            {
                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                int result = waitMessageReturn(is_run);
                if(result)
                    is_run = false;
                else
                    continue;
                if(!is_run)break;
            }
            vacancy_vision->CloseLight();
            if(!moveToWorkPos(true))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setHasPickedNgLens(false);
                    continue;
                }
                if(!is_run)break;
            }
            if((!placeLensToTray())&&has_material)
            {
                AppendError(u8" 如果NG Lens未放好请手动拿走！");
                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    //todo检测料已拿走
                }
                if(!is_run)break;
            }
            tray->setCurrentMaterialState(MaterialState::IsNg,result_tray);
            states.setHasPickedNgLens(false);
            if(parameters.staticTest()&&finish_stop&&(tray->getCurrentIndex() == parameters.testLensCount() -1))
            {
                current_time--;
                if(current_time <= 0 &&tray->getCurrentIndex() == parameters.testLensCount() -1)
                {
                    is_run = false;
                    sendAlarmMessage(ErrorLevel::ErrorMustStop,"测试完成,自动停止！");
                    break;
                }
                else
                {
                    current_count = parameters.testLensCount();
                    tray->resetTrayState(0);
                }
                finish_stop = false;
            }
        }
        //检测是否需要换盘
        if((!states.allowChangeTray())&&(!states.hasPickedLens())&&(!states.lutHasNgLens()))
        {
            if((!states.hasTray())||checkNeedChangeTray())
                states.setAllowChangeTray(true);
        }
        //取料
        if((!finish_stop)&&states.hasTray()&&(!states.allowChangeTray())&&(!states.lutHasNgLens())&&(!states.hasPickedNgLens())&&(!states.hasPickedLens()))
        {
            has_task = true;
            if(tray->isTrayNeedChange(states.currentTray()))
            {
                if(states.currentTray() == 0)
                    states.setCurrentTray(1);
                else
                {
                    AppendError(u8"逻辑错误，无可用lens");
                    sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                    is_run = false;
                    break;
                }
            }
            lens_vision->OpenLight();
            if(!moveToNextTrayPos(states.currentTray()))
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if(has_material&&(!performLensPR()))
            {
                if(pr_times > 0)
                {
                    pr_times--;
                    tray->setCurrentMaterialState(MaterialState::IsEmpty,states.currentTray());
                    states.setPickedTrayID(states.currentTray());
                    states.setPickedLensID(tray->getCurrentIndex(states.currentTray()));

                    AppendError(u8"自动重试.");
                    sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
                    continue;
                }
                else
                {
                    pr_times = 5;
                    AppendError(u8"执行lens视觉连续失败5次!");
                    sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                    waitMessageReturn(is_run);
                    if(!is_run)break;
                    continue;
                }
            }
            else
            {
                sendAlarmMessage(ErrorLevel::TipNonblock,"");
            }
            lens_vision->CloseLight();
            pr_times = 5;
            if(!moveToWorkPos(false))
            {
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                if(waitMessageReturn(is_run))
                    continue;
                if(!is_run)break;
            }

            if((!pickTrayLens())&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
                if(!waitMessageReturn(is_run))
                    states.setHasPickedLens(true);
                if(!is_run)break;
            }
            else
                states.setHasPickedLens(true);
            addCurrentNumber();
            tray->setCurrentMaterialState(MaterialState::IsEmpty,states.currentTray());
            states.setPickedTrayID(states.currentTray());
            states.setPickedLensID(tray->getCurrentIndex(states.currentTray()));
            qInfo("picked lens index %d, tray_index %d",states.pickedLensID(),states.pickedTrayID());

            current_count--;
            if(current_count<=0)
                finish_stop = true;
        }
        //等待位置
        if(!movePickerToLUTPos1(true))
        {
            sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
            is_run = false;
            break;
        }
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
                    states.setNeedChangTray(false);
                    states.setAllowChangeTray(false);
                    qInfo("finishChangeTray");
                }
                else
                {
                    QThread::msleep(1000);
                    qInfo("waitingChangeTray %d",change_tray_time_out);
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
            qInfo(u8"放料到LUT");
            has_task = true;
            if(!movePickerToLUTPos1(true))
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if((!placeLensToLUT())&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
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
            addCurrentNgNumber();
            has_task = true;
            lut_lens_vision->OpenLight();
            if(!moveToLUTPRPos2())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if(has_material&&(!performLUTLensPR()))
            {
                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                int result = waitMessageReturn(is_run);
                if(result)
                    is_run = false;
                else
                    continue;
                if(!is_run)break;
            }
            lut_lens_vision->CloseLight();
            if(!moveToWorkPos(false))
            {
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                if(waitMessageReturn(is_run))
                    continue;
                if(!is_run)break;
            }
            if((!pickLUTLens())&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
                if(!waitMessageReturn(is_run))
                    states.setHasPickedNgLens(true);
                if(!is_run)break;
            }
            else
            {
                states.setHasPickedNgLens(true);
            }
            {
                QMutexLocker temp_locker(&lut_mutex);
                states.setLutHasNgLens(false);
                states.setPickedTrayID(states.lutNgTrayID());
                states.setPickedLensID(states.lutNgLensID());
                qInfo("picked ng lens index %d, tray_id %d",states.pickedLensID(),states.pickedTrayID());
            }
        }
        //判断是否完成
        if((!states.lutHasNgLens())&&(!states.needLoadLens()))
        {
            if(states.loadingLens())
            {
                has_task = true;
                QMutexLocker temp_locker(&lut_mutex);
                emit sendLoadLensFinish(states.lutLensID(),states.lutTrayID());
                qInfo("sendLoadLensFinish lutLensID %d lutTrayID %d",states.lutLensID(),states.lutTrayID());
                states.setLoadingLens(false);
            }
        }
    }
    qInfo("Lens Loader module end of thread");
}

void LensLoaderModule::runTest()
{
    is_run = true;
    finish_stop = false;
    states.setHasTray(true);
    states.setAllowChangeTray(false);

    bool has_task = true;
    bool need_load_lens;
    bool lut_has_ng_lens;
    int current_time = parameters.repeatTime();
    int current_count = parameters.testLensCount();
    bool finished = false;
    while (is_run)
    {
        has_task = false;
        {
            QMutexLocker temp_locker(&lut_mutex);
            need_load_lens = states.needLoadLens();
            lut_has_ng_lens = states.lutHasNgLens();
        }
        //        if(!has_task)
        {
            QThread::msleep(100);
        }
        //放NGLens
        if(states.hasTray()&&(!states.allowChangeTray())&&states.hasPickedNgLens())
        {
            has_task = true;
            int result_tray = 0;
            vacancy_vision->OpenLight();
            if(!moveToTrayEmptyPos(states.pickedLensID(),states.pickedTrayID(),result_tray))
            {
                AppendError(" 请手动拿走LPA上的NG Lens后继续!");
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                waitMessageReturn(is_run);
                //todo加物料检测
                states.setHasPickedNgLens(false);
                continue;
            }
            //            if(parameters.staticTest())
            //                current_time = parameters.repeatTime();
            //            while(current_time > 0)
            //            {
            if(!performVacancyPR())
            {
                sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                if(waitMessageReturn(is_run))
                    is_run = false;
                else
                    continue;
                if(!is_run)break;
            }
            //                recordTrayVacancyPr(getUuid(finished,states.pickedLensID(),current_time));
            //                if(!parameters.staticTest())
            //                    break;
            //                current_time--;
            //            }
            vacancy_vision->CloseLight();
            if(!moveToWorkPos(true))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setHasPickedNgLens(false);
                    continue;
                }
                if(!is_run)break;
            }
            if((!placeLensToTray()))
            {
                AppendError(u8" 如果NG Lens未放好请手动拿走！");
                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    //todo检测料已拿走
                }
                if(!is_run)break;
            }
            tray->setCurrentMaterialState(MaterialState::IsNg,result_tray);
            states.setHasPickedNgLens(false);
            if(parameters.staticTest())
            {
                if(finish_stop&&(tray->getCurrentIndex() == parameters.testLensCount() -1))
                {
                    if(finished)
                    {
                        is_run = false;
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,"测试完成,自动停止！");
                        break;
                    }
                    else
                    {
                        finished = true;
                        current_count = parameters.testLensCount();
                        tray->resetTrayState(0);
                    }
                    finish_stop = false;
                }
            }
            else
            {
                if(finish_stop)
                    current_time--;
                if(current_time <= 0)
                {
                    if(finished&&(tray->getCurrentIndex() == parameters.testLensCount() -1))
                    {
                        is_run = false;
                        sendAlarmMessage(ErrorLevel::ErrorMustStop,"测试完成,自动停止！");
                        break;
                    }
                    else
                    {
                        finished = true;
                        current_time = parameters.repeatTime();
                        current_count = parameters.testLensCount();
                        tray->resetTrayState(0);
                    }
                }
                else
                {
                    current_count = parameters.testLensCount();
                    tray->resetTrayState(0);
                }
                finish_stop = false;
            }
        }
        //取料
        if((!finish_stop)&&states.hasTray()&&(!states.allowChangeTray())&&(!states.lutHasNgLens())&&(!states.hasPickedNgLens())&&(!states.hasPickedLens()))
        {
            has_task = true;
            if(tray->isTrayNeedChange(states.currentTray()))
            {
                if(states.currentTray() == 0)
                    states.setCurrentTray(1);
                else
                {
                    AppendError(u8"逻辑错误，无可用lens盘");
                    sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                    is_run = false;
                    break;
                }
            }
            lens_vision->OpenLight();
            if(!moveToNextTrayPos(states.currentTray()))
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if(parameters.staticTest())
                current_time = parameters.repeatTime();
            while(current_time > 0)
            {
                if(!performLensPR())
                {
                    sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                    if(waitMessageReturn(is_run))
                    {
                        is_run = false;
                    }
                    else
                    {
                        qInfo("continue performLensPR");
                        QThread::msleep(1000);
                        continue;
                    }
                    if(!is_run)break;
                }
                recordTrayLensPr(getUuid(finished,tray->getCurrentIndex(states.currentTray()),current_time));
                if(!parameters.staticTest())
                    break;
                current_time--;
            }
            lens_vision->CloseLight();
            if(!is_run)break;

            if(!moveToWorkPos(false))
            {
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                if(waitMessageReturn(is_run))
                    continue;
                if(!is_run)break;
            }

            if(!pickTrayLens())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
                if(!waitMessageReturn(is_run))
                    states.setHasPickedLens(true);
                if(!is_run)break;
            }
            else
            {
                current_count--;
                if(current_count<=0)
                    finish_stop = true;
                tray->setCurrentMaterialState(MaterialState::IsEmpty,states.currentTray());
                states.setHasPickedLens(true);
                states.setPickedTrayID(states.currentTray());
                states.setPickedLensID(tray->getCurrentIndex(states.currentTray()));
            }

        }
        //放料到LUT
        if(need_load_lens&&states.hasPickedLens())
        {
            has_task = true;
            lut_vision->OpenLight();
            if(!moveToLUTPRPos1())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if(parameters.staticTest())
                current_time = parameters.repeatTime();
            while(current_time > 0)
            {
                if(!performLUTPR())
                {
                    sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                    if(waitMessageReturn(is_run))
                        is_run = false;
                    else
                        continue;
                    if(!is_run)break;
                }
                recordLutVacancyPr(getUuid(finished,states.pickedLensID(),current_time));
                if(!parameters.staticTest())
                    break;
                current_time--;
            }
            lut_vision->CloseLight();
            if(!is_run)break;
            if(!moveToWorkPos(true))
            {
                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
                if(waitMessageReturn(is_run))
                {
                    states.setHasPickedNgLens(false);
                    continue;
                }
                if(!is_run)break;
            }
            if(!placeLensToLUT())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
                if(waitMessageReturn(is_run))
                    states.setHasPickedLens(false);
                else
                {
                    need_load_lens = false;
                    states.setHasPickedLens(false);
                }
                if(!is_run)break;
            }
            else
            {
                need_load_lens = false;
                states.setHasPickedLens(false);
            }
            lut_lens_vision->OpenLight();
            if(!moveToLUTPRPos1())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if(parameters.staticTest())
                current_time = parameters.repeatTime();
            while(current_time > 0)
            {
                if(!performLUTLensPR())
                {
                    sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                    if(waitMessageReturn(is_run))
                        is_run = false;
                    else
                        continue;
                    if(!is_run)break;
                }
                recordLutLensPr(getUuid(finished,states.pickedLensID(),current_time));
                if(!parameters.staticTest())
                    break;
                current_time--;
            }
            lut_lens_vision->CloseLight();
            if(!is_run)break;
            QMutexLocker temp_locker(&lut_mutex);
            states.setNeedLoadLens(need_load_lens);
            states.setLutTrayID(states.pickedTrayID());
            states.setLutLensID(states.pickedLensID());
        }
        //取NGlens
        if(lut_has_ng_lens&&(!states.hasPickedLens())&&(!states.hasPickedNgLens()))
        {
            has_task = true;
            lut_lens_vision->OpenLight();
            if(!moveToLUTPRPos2())
            {
                sendAlarmMessage(ErrorLevel::ErrorMustStop,GetCurrentError());
                is_run = false;
                break;
            }
            if(parameters.staticTest())
                current_time = parameters.repeatTime();
            while(current_time > 0)
            {
                if(!performLUTLensPR())
                {
                    sendAlarmMessage(ErrorLevel::RetryOrStop,GetCurrentError());
                    if(waitMessageReturn(is_run))
                        is_run = false;
                    else
                        continue;
                    if(!is_run)break;
                }
                recordNgLensPr(getUuid(finished,states.lutNgLensID(),current_time));
                if(!parameters.staticTest())
                    break;
                current_time--;
            }
            lut_lens_vision->CloseLight();
            if(!is_run)break;
            if(!moveToWorkPos(false))
            {
                sendAlarmMessage(ErrorLevel::WarningBlock,GetCurrentError());
                if(waitMessageReturn(is_run))
                    continue;
                if(!is_run)break;
            }
            if((!pickLUTLens())&&has_material)
            {
                sendAlarmMessage(ErrorLevel::ContinueOrReject,GetCurrentError());
                if(!waitMessageReturn(is_run))
                    states.setHasPickedNgLens(true);
                if(!is_run)break;
            }
            else
            {
                states.setHasPickedNgLens(true);
            }
            {
                QMutexLocker temp_locker(&lut_mutex);
                states.setLutHasNgLens(false);
                states.setPickedTrayID(states.lutNgTrayID());
                states.setPickedLensID(states.lutNgLensID());
                qInfo("picked ng lens index %d, tray_id %d",states.pickedLensID(),states.pickedTrayID());
            }
        }
        //判断是否完成
        if((!states.lutHasNgLens())&&(!states.needLoadLens()))
        {
            if(states.loadingLens())
            {
                has_task = true;
                QMutexLocker temp_locker(&lut_mutex);
                emit sendLoadLensFinish(states.lutLensID(),states.lutTrayID());
                qInfo("sendLoadLensFinish lutLensID %d lutTrayID %d",states.lutLensID(),states.lutTrayID());
                states.setLoadingLens(false);
            }
        }
    }
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
    bool result = lens_vision->performPR();
    pr_offset = lens_vision->getCurrentResult();
    if(!result)
        AppendError(QString(u8"执行lens视觉失败"));
    qInfo(u8"执行lens视觉,返回值%d",result);
    return result;
}

bool LensLoaderModule::performVacancyPR()
{
    bool result = vacancy_vision->performPR();
    pr_offset = vacancy_vision->getCurrentResult();
    if(!result)
        AppendError(QString(u8"执行lens料盘空位视觉失败"));
    qInfo(u8"执行lens料盘空位视觉,返回值%d",result);
    return result;
}

bool LensLoaderModule::performLUTPR()
{
    bool result = lut_vision->performPR();
    pr_offset = lut_vision->getCurrentResult();

    if(!result)
        AppendError(QString(u8"执行LUT空位视觉失败"));
    qInfo(u8"执行LUT空位视觉,返回值%d",result);
    return result;
}

bool LensLoaderModule::performLUTLensPR()
{
    bool result = lut_lens_vision->performPR();
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
    if(result == check_state)
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
    if(result == check_state)
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
    if(result == check_state)
        return true;
    QString error = QString(u8"LUT放NgLens位置逻辑%1料，但检测到%2料。").arg(check_state?u8"有":u8"无").arg(result?u8"有":u8"无");
    AppendError(error);
    qInfo(error.toStdString().c_str());
    return false;
}

bool LensLoaderModule::moveToPrOffset(bool check_softlanding)
{
    PrOffset temp(- pr_offset.X, - pr_offset.Y,pr_offset.Theta);
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
    result &= load_vacuum->Set(true);
    result &= vcmSearchReturn();
    if(!result)
        AppendError(QString(u8"从当前位置放lens到LUT失败"));
    if(parameters.openTimeLog())
        qInfo(u8"从当前位置放lens到LUT result %d ",result);
    return result;
}

bool LensLoaderModule::pickLUTLens()
{
    bool result = vcmSearchLUTZ(parameters.placeLensZ(), true);
    result &= unload_vacuum->Set(false);
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

bool LensLoaderModule::isRunning()
{
    return is_run;
}

void LensLoaderModule::startWork(int run_mode)
{
    has_material = true;
    qInfo("Lensloader start run_mode :%d",run_mode);
    if(run_mode == RunMode::Normal||run_mode == RunMode::OnllyLeftAA||run_mode == RunMode::OnlyRightAA)run(true);
    else if(run_mode == RunMode::NoMaterial)
    {
        has_material = false;
        run(false);
    }
    else if(run_mode == RunMode::MachineTest)
    {
        runTest();
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

void LensLoaderModule::performHandlingOperation(int cmd)
{
    qInfo("performHandling %d",cmd);
    bool result;
    int temp_value = 10;
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
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
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
    else
        result = true;
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
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
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        is_handling = false;
        return;
    }
    cmd =cmd/temp_value*temp_value;
    temp_value = 10000;
    if(cmd%temp_value == HandlePickerAction::PICK_LENS_FROM_TRAY){
        if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = pickTrayLens();
        }
    }
    else if(cmd%temp_value == HandlePickerAction::PLACE_LENS_TO_LUT){
        if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = placeLensToLUT();
        }
    }
    else if(cmd%temp_value == HandlePickerAction::PICK_NG_LENS_FROM_LUT){
        if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
            result = pickLUTLens();
        }
    }
    else if(cmd%temp_value == HandlePickerAction::PLACE_NG_LENS_TO_TRAY){
        if(emit sendMsgSignal(tr(u8""),tr(u8"是否执行操作？"))){
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
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
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

}

PropertyBase *LensLoaderModule::getModuleState()
{
    return &states;
}

