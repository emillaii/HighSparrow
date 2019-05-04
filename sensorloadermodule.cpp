#include "sensorloadermodule.h"

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
    temp_map.insert("sut_pr_position", &sut_pr_position);
    temp_map.insert("picker1_offset", &picker1_offset);
    temp_map.insert("picker2_offset", &picker2_offset);
    PropertyBase::loadJsonConfig("config//SensorLoaderModule.json", temp_map);
    return true;
}

void SensorLoaderModule::saveJsonConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("parameters", &parameters);
    temp_map.insert("sut_pr_position", &sut_pr_position);
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

void SensorLoaderModule::resetLogic()
{

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
            if(!moveToTrayPos(sut_used_material,product_tray_index))
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
            if(!placeProductToTray())
            {
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
            if(!moveToTrayPos(sut_used_material,product_tray_index))
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
            if(!placeSensorToTray())
            {
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

            if(!pickTraySensor())
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
        //放料到LUT
        if(states.needLoadSensor()&&states.hasPickedSensor())
        {
            has_task = true;
            if(!moveToSUTPRPos())
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
            if(!placeSensorToSUT())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrGiveUp,GetCurrentError());
                if(waitMessageReturn(is_run))
                    states.setHasPickedSensor(false);
                else
                {
                    states.setNeedLoadSensor(false);
                    states.setHasPickedSensor(false);
                }
            }
            else
            {
                states.setNeedLoadSensor(false);
                states.setHasPickedSensor(false);
            }
            sut_raw_material = picked_material;
        }
        //取成品
        if(states.sutHasProduct()&&(!states.hasPickedNgSensor())&&(!states.hasPickedProduct()))
        {
            has_task = true;
            if(!moveToSUTPRPos())
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
            if(!pickSUTProduct())
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

        }
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
            if(!pickSUTSensor())
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

bool SensorLoaderModule::moveToSUTPRPos(bool check_softlanding)
{
    qInfo("moveToSUTPRPos");
    return  pick_arm->move_XY_Synic(sut_pr_position.ToPointF(),check_softlanding);
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

bool SensorLoaderModule::picker1SearchZ(double z,bool is_open, bool check_softlanding)
{
    qInfo("picker1SearchZ");
    return pick_arm->ZSerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),parameters.finishDelay(),is_open,check_softlanding);
}

bool SensorLoaderModule::picker2SearchZ(double z,bool is_open, bool check_softlanding)
{
    qInfo("picker2SearchZ");
    return pick_arm->ZSerchByForce2(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),z,parameters.vcmMargin(),parameters.finishDelay(),is_open,check_softlanding);

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
