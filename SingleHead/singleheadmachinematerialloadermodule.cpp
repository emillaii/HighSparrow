#include "SingleHead/singleheadmachinematerialloadermodule.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QMessageBox>
#include <qdebug.h>

SingleHeadMachineMaterialLoaderModule::SingleHeadMachineMaterialLoaderModule(QString name) : ThreadWorkerBase(name) {}

void SingleHeadMachineMaterialLoaderModule::Init(SingleHeadMachineMaterialPickArm *pick_arm,
                                                 LSutState *lsutState,
                                                 MaterialTray *sensorTray,
                                                 MaterialTray *lensTray,
                                                 MaterialTray *rejectTray,
                                                 VisionLocation *sensor_vision,
                                                 VisionLocation *sensor_vacancy_vision,
                                                 VisionLocation *sut_vision,
                                                 VisionLocation *sut_sensor_vision,
                                                 VisionLocation *sut_product_vision,
                                                 VisionLocation *lens_vision,
                                                 VisionLocation *lens_vacancy_vision,
                                                 VisionLocation *lut_vision,
                                                 VisionLocation *lut_lens_vision,
                                                 XtVacuum *sutVacuum,
                                                 XtVacuum *lutVacuum,
                                                 TowerLightBuzzer *towerLightBuzzer)
{
    this->pick_arm = pick_arm;
    this->lsutState = lsutState;
    this->sensorTray = sensorTray;
    this->lensTray = lensTray;
    this->rejectTray = rejectTray;
    this->sensor_vision = sensor_vision;
    this->sensor_vacancy_vision = sensor_vacancy_vision;
    this->sut_vision = sut_vision;
    this->sut_sensor_vision = sut_sensor_vision;
    this->sut_product_vision = sut_product_vision;
    this->lens_vision = lens_vision;
    this->lens_vacancy_vision = lens_vacancy_vision;
    this->lut_vision = lut_vision;
    this->lut_lens_vision = lut_lens_vision;
    this->sut_vacuum = sutVacuum;
    this->lut_vacuum = lutVacuum;
    this->towerLightBuzzer = towerLightBuzzer;
}

void SingleHeadMachineMaterialLoaderModule::loadJsonConfig(QString file_name)
{
    QMap<QString, PropertyBase *> temp_map;
    temp_map.insert("material_loader", &parameters);
    temp_map.insert("sut_pr_position", &sut_pr_position);
    temp_map.insert("lut_pr_position", &lut_pr_position);
    temp_map.insert("camera_to_picker1_offset", &camera_to_picker1_offset);
    temp_map.insert("camera_to_picker2_offset", &camera_to_picker2_offset);
    temp_map.insert("place_ng_sensor_to_tray_offset", &placeNgSensorToTrayOffset);
    temp_map.insert("place_ng_product_to_tray_offset", &placeNgProductToTrayOffset);
    temp_map.insert("place_ok_product_to_tray_offset", &placeOkProductToTrayOffset);
    PropertyBase::loadJsonConfig(file_name, temp_map);
}

void SingleHeadMachineMaterialLoaderModule::saveJsonConfig(QString file_name)
{
    QMap<QString, PropertyBase *> temp_map;
    temp_map.insert("material_loader", &parameters);
    temp_map.insert("sut_pr_position", &sut_pr_position);
    temp_map.insert("lut_pr_position", &lut_pr_position);
    temp_map.insert("camera_to_picker1_offset", &camera_to_picker1_offset);
    temp_map.insert("camera_to_picker2_offset", &camera_to_picker2_offset);
    temp_map.insert("place_ng_sensor_to_tray_offset", &placeNgSensorToTrayOffset);
    temp_map.insert("place_ng_product_to_tray_offset", &placeNgProductToTrayOffset);
    temp_map.insert("place_ok_product_to_tray_offset", &placeOkProductToTrayOffset);
    PropertyBase::saveJsonConfig(file_name, temp_map);
}

void SingleHeadMachineMaterialLoaderModule::performHandling(int cmd)
{
    emit sendHandlingOperation(cmd);
    qInfo("emit performHandling %d", cmd);
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayPos(int tray_index)
{
    qInfo("moveToTrayPos tray_index %d", tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(lensTray->getCurrentPosition(tray_index), false);
    if (!result)
        AppendError(QString(u8"移动到lens盘当前位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayStartPos(int tray_index)
{
    qInfo("moveToStartPos%d", tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(lensTray->getStartPosition(tray_index), false);
    if (!result)
        AppendError(QString(u8"移动到lens盘起始位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayEndPos()
{
    qInfo("moveToTrayEndPos");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(lensTray->getEndPosition(), false);
    if (!result)
        AppendError(QString(u8"移动到lens盘结束位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToNextLensTrayPos(int tray_index)
{
    qInfo("moveToNextTrayPos tray_index %d", tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= lensTray->findNextPositionOfInitState(tray_index);
    if (result)
        result &= pick_arm->move_XY_Synic(lensTray->getCurrentPosition(tray_index));
    if (!result)
        AppendError(QString(u8"移动到lens盘下一个位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSensorTrayPos(int tray_index)
{
    qInfo("moveToSensorTrayPos");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(sensorTray->getCurrentPosition(tray_index), false);
    if (!result)
        AppendError(QString(u8"移动到sensor盘当前位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSensorTrayStartPos(int tray_index)
{
    qInfo("moveToStartPos%d");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(sensorTray->getStartPosition(tray_index), false);
    if (!result)
        AppendError(QString(u8"移动到%1盘起始位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSensorTrayEndPos()
{
    qInfo("moveToTray1EndPos");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(sensorTray->getEndPosition(), false);
    if (!result)
        AppendError(QString(u8"移动到sensor盘结束位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToNextSensorTrayPos(int tray_index)
{
    qInfo("moveToNextTrayPos tray_index %d", tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= sensorTray->findNextPositionOfInitState(tray_index);
    if (result)
        result &= pick_arm->move_XY_Synic(sensorTray->getCurrentPosition(tray_index));
    if (!result)
        AppendError(QString(u8"移动到sensor盘下一个位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToRejectTrayPos(int tray_index)
{
    qInfo("moveToRejectTrayPos tray_index %d", tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(rejectTray->getCurrentPosition(tray_index), true);
    if (!result)
        AppendError(QString(u8"移动到reject盘当前位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToRejectTrayStartPos(int tray_index)
{
    qInfo("moveToRejectTrayStartPos%d", tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(rejectTray->getStartPosition(tray_index), true);
    if (!result)
        AppendError(QString(u8"移动到reject盘起始位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToRejectTrayEndPos()
{
    qInfo("moveToRejectTrayEndPos");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(rejectTray->getEndPosition(), true);
    if (!result)
        AppendError(QString(u8"移动到reject盘结束位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToNextRejectTrayPos(int tray_index)
{
    qInfo("moveToNextRejectTrayPos tray_index %d", tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= rejectTray->findNextPositionOfInitState(tray_index);
    if (result)
        result &= pick_arm->move_XY_Synic(rejectTray->getCurrentPosition(tray_index));
    if (!result)
        AppendError(QString(u8"移动到reject盘下一个位置失败"));
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::moveToLUTPRPos(bool check_softlanding)
{
    qInfo("moveToLUTPRPos %d");
    bool result = pick_arm->move_XmY_Synic(lut_pr_position.ToPointF(), check_softlanding);
    if (!result)
        AppendError(QString("移动LPA到LUT位置失败%1"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSUTPRPos(bool check_softlanding)
{
    qInfo("moveToSUTPRPos %d");
    bool result = pick_arm->move_XmY_Synic(sut_pr_position.ToPointF(), check_softlanding);
    if (!result)
        AppendError(QString("移动SPA到SUT位置失败%1"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToPicker1WorkPos(bool check_softlanding)
{
    PrOffset temp(camera_to_picker1_offset.X() - pr_offset.X, camera_to_picker1_offset.Y() - pr_offset.Y,
                  pr_offset.Theta);
    bool result = pick_arm->stepMove_XmYT1_Synic(temp.X, temp.Y, temp.Theta, check_softlanding);
    if (!result)
        AppendError(QString(u8"去1号吸头工作位置(step x %1,y %2,t %3)失败!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::moveToPicker2WorkPos(bool check_softlanding)
{
    PrOffset temp(camera_to_picker2_offset.X() - pr_offset.X, camera_to_picker2_offset.Y() - pr_offset.Y,
                  pr_offset.Theta);
    qInfo("moveToPicker2WorkPos offset:(%f,%f,%f)", temp.X, temp.Y, temp.Theta);
    bool result = pick_arm->stepMove_XmYT2_Synic(temp.X, temp.Y, temp.Theta, check_softlanding);
    if (!result)
        AppendError(QString(u8"去2号吸头工作位置(step x %1,y %2,t %3)失败!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::performSensorPR()
{
    sensor_vision->OpenLight();
    Sleep(100);    // Position settling
    qInfo("performSensorPR");
    bool result = sensor_vision->performPR(pr_offset);
    if (result)
        AppendError(QString(u8"执行料盘sensor视觉失败!"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::performSensorVacancyPR()
{
    sensor_vacancy_vision->OpenLight();
    Sleep(100);    // Position settling
    qInfo("performVacancyPR");
    bool result = sensor_vacancy_vision->performPR(pr_offset);
    if (result)
        AppendError(QString(u8"执行料盘空位视觉失败!"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::performSUTPR()
{
    sut_vision->OpenLight();
    Sleep(100);    // Position settling
    qInfo("performSUTPR");
    bool result = sut_vision->performPR(pr_offset);
    if (result)
        AppendError(QString(u8"执行SUT视觉失败!"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::performSUTSensorPR()
{
    sut_sensor_vision->OpenLight();
    Sleep(100);    // Position settling
    qInfo("performSUTSensorPR");
    bool result = sut_sensor_vision->performPR(pr_offset);
    if (result)
        AppendError(QString(u8"执行SUT上的sensor视觉失败!"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::performSUTProductPR()
{
    sut_product_vision->OpenLight();
    qInfo("performSUTProductPR");
    Sleep(100);    // Position settling
    bool result = sut_product_vision->performPR(pr_offset);
    if (result)
        AppendError(QString(u8"执行SUT视觉失败!"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::performLensPR()
{
    qInfo("performLensPR");
    lens_vision->OpenLight();
    Sleep(100);    // Position settling
    return lens_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::performLensVacancyPR()
{
    lens_vacancy_vision->OpenLight();
    Sleep(100);    // Position settling
    return lens_vacancy_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::performLUTPR()
{
    lut_vision->OpenLight();
    Sleep(100);    // Position settling
    return lut_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::performLUTLensPR()
{
    lut_lens_vision->OpenLight();
    Sleep(100);    // Position settling
    return lut_lens_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::picker2PickSensorFromTray(int time_out)
{
    qInfo("pickTraySensor time_out %d", time_out);
    bool result = picker2SearchZ(pick_arm->parameters.pickSensorZ(), true, time_out, 1);
    if (!result)
        AppendError(QString(u8"从sensor盘取sensor失败"));
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker2PlaceSensorToSUT(int time_out)
{
    qInfo("placeSensorToSUT time_out %d", time_out);
    bool result = picker2SearchSutZ2(pick_arm->parameters.placeSensorZ(), false, time_out);
    if (!result)
        AppendError(QString(u8"放sensor到SUT%1失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PickNgSensorFromSUT(int time_out)
{
    qInfo("picker2PickNgSensorFromSUT time_out %d", time_out);
    bool result = picker2SearchSutZ(pick_arm->parameters.placeSensorZ(), true, time_out);
    if (!result)
        AppendError(QString(u8"从SUT取NGsenor失败"));
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker1PickNgSensorFromSUT(int time_out)
{
    qInfo("pickSUTSensor  time_out %d", time_out);
    bool result = picker1SearchSutZ(pick_arm->parameters.placeSensorZ(), true, time_out);
    if (!result)
        AppendError(QString(u8"从SUT%1取NGsenor失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PickProductFormSUT(int time_out)
{
    qInfo("pickSUTProduct time_out %d", time_out);
    bool result = picker1SearchSutZ(pick_arm->parameters.pickProductZ(), true, time_out);
    if (!result)
        AppendError(QString(u8"从SUT%1取成品失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceNgSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d", time_out);
    bool result = picker1SearchZ(pick_arm->parameters.pickSensorZ(), false, time_out, 0);
    if (result)
    {
        result &= pick_arm->motor_th1->MoveToPosSync(0);
    }
    if (!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PlaceSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d", time_out);
    bool result = picker2SearchZ(pick_arm->parameters.pickSensorZ(), false, time_out, 1);

    if (!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PlaceNgSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d", time_out);
    bool result = picker2SearchZ(pick_arm->parameters.pickSensorZ(), false, time_out, 1);
    if (result)
    {
        result &= pick_arm->motor_th2->MoveToPosSync(0);
    }
    if (!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceProductToTray(int time_out)
{
    qInfo("placeProductToTray time_out %d", time_out);
    bool result = picker1SearchZ(pick_arm->parameters.placeProductZ(), false, time_out, 0);
    if (result)
    {
        result &= pick_arm->motor_th1->MoveToPosSync(0);
    }
    if (!result)
        AppendError(QString(u8"将成品放入成品盘失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PickLensFromTray(int time_out)
{
    qInfo("pickTrayLens");
    bool result = picker1SearchZ(pick_arm->parameters.pickLensZ(), true, time_out, 0);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceLensToLUT(int time_out)
{
    qInfo("placeLensToLUT");
    this->lut_vacuum->Set(true, false);
    bool result = picker1SearchZ(pick_arm->parameters.placeLensZ(), false, time_out, 0);
    //    this->lut_vacuum->Set(true, false);
    //    result &= pick_arm->vacuum_sensor_suction->Set(false, false);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PickNgLensFromLUT(int time_out)
{
    QElapsedTimer timer;
    timer.start();
    qInfo("pickLUTLens");
    this->lut_vacuum->Set(false);
    qInfo("cp1: %d", timer.elapsed());
    bool ret = picker2SearchZ(pick_arm->parameters.placeLensZ(), true, time_out, 1);
    qInfo("cp2: %d", timer.elapsed());
    return ret;
}
bool SingleHeadMachineMaterialLoaderModule::picker1PickNgLensFromLUT(int time_out)
{
    QElapsedTimer timer;
    timer.start();
    qInfo("pickLUTLens");
    this->lut_vacuum->Set(false, false);
    qInfo("cp1: %d", timer.elapsed());
    bool ret = picker1SearchZ(pick_arm->parameters.placeLensZ(), true, time_out, 0);
    qInfo("cp2: %d", timer.elapsed());
    return ret;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceNgLensToTray(int time_out)
{
    qInfo("placeLensToTray");
    //    return picker1SearchZ(pick_arm->parameters.pickLensZ(),false,time_out,0);
    bool result = picker1SearchZ(pick_arm->parameters.pickLensZ(), false, time_out, 0);
    //    result &= pick_arm->vacuum_sensor_suction->Set(false,false);
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker1PlaceLensToTray(int time_out)
{
    qInfo("placeLensToTray");
    //    return picker1SearchZ(pick_arm->parameters.pickLensZ(),false,time_out, 0);
    bool result = picker1SearchZ(pick_arm->parameters.pickLensZ(), false, time_out, 0);
    //    result &= pick_arm->vacuum_sensor_suction->Set(false,false);
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker2PlaceNgLensToTray(int time_out)
{
    qInfo("placeLensToTray");
    return picker2SearchZ(pick_arm->parameters.pickLensZ(), false, time_out, 1);
}

bool SingleHeadMachineMaterialLoaderModule::picker2SearchZ(double z, bool is_open, int time_out, int picker)
{
    qInfo("picker2SearchZ z %f is_open %d timeout %d", z, is_open, time_out);
    bool result
            = pick_arm->ZSerchByForce(picker, parameters.vcm2Svel(), parameters.vcm2PickForce(), z, parameters.vcm2Margin(),
                                      parameters.vcm2FinishDelay(), is_open, false, time_out);
    result &= pick_arm->ZSerchReturn(picker, time_out);
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker1SearchZ(double z, bool is_open, int time_out, int picker)
{
    qInfo("picker1SearchZ z %f is_open %d timeout %d", z, is_open, time_out);
    QElapsedTimer timer;
    timer.start();
    bool result
            = pick_arm->ZSerchByForce(picker, parameters.vcm1Svel(), parameters.vcm1PickForce(), z, parameters.vcm1Margin(),
                                      parameters.vcm1FinishDelay(), is_open, false, time_out);
    qInfo("ZSerchByForce: %d", timer.elapsed());
    timer.restart();
    result &= pick_arm->ZSerchReturn(picker, time_out);
    qInfo("ZSerchReturn: %d", timer.elapsed());
    timer.restart();
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2SearchSutZ(double z, bool is_open, int time_out)
{
    qInfo("picker2SearchSutZ2 z %f is_open %d time_out %d", z, is_open, time_out);
    bool result = pick_arm->motor_vcm2->MoveToPosSync(z - parameters.escapeHeight());
    if (result)
    {
        sut_vacuum->Set(true);
        result
                = pick_arm->ZSerchByForce(1, parameters.vcm2Svel(), parameters.vcm2PickForce(), z, parameters.vcm2Margin(),
                                          parameters.vcm2FinishDelay(), is_open, false, time_out);
        result &= pick_arm->ZSerchReturn(1, time_out);
        QThread::msleep(100);
        result = pick_arm->motor_y->StepMoveSync(parameters.escapeY());
        QThread::msleep(100);
        result &= pick_arm->motor_vcm2->MoveToPosSync(0);
    }
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2SearchSutZ2(double z, bool is_open, int time_out)
{
    qInfo("picker2SearchSutZ z %f is_open %d time_out %d sutPlaceSensorAngle: %f", z, is_open, time_out,
          parameters.sutPlaceSensorAngle());
    bool result = pick_arm->motor_y->StepMoveSync(parameters.escapeY());
    result &= pick_arm->motor_vcm2->MoveToPosSync(z - parameters.escapeHeight());

    if (result)
    {
        result = pick_arm->motor_th2->MoveToPosSync(parameters.sutPlaceSensorAngle());
        //        result = pick_arm->motor_vcmx->StepMoveSync(parameters.escapeX());
        result &= pick_arm->motor_y->StepMoveSync(-parameters.escapeY());
        result
                &= pick_arm->ZSerchByForce(1, parameters.vcm2Svel(), parameters.vcm2PickForce(), z, parameters.vcm2Margin(),
                                           parameters.vcm2FinishDelay(), is_open, false, time_out);
        sut_vacuum->Set(true, false);    // Sensor放到SUT表面不平整，真空吸经常检测不到
        result &= pick_arm->ZSerchReturn(1, time_out);
        //        result &= pick_arm->motor_vcm2->MoveToPosSync(0);
    }
    result &= pick_arm->motor_vcm2->MoveToPosSync(0);
    result &= pick_arm->motor_th2->MoveToPos(0);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2SearchSutZ2Revert(double z, bool is_open, int time_out)
{
    qInfo("picker2SearchSutZ2Revert z %f is_open %d time_out %d sutPlaceSensorAngle: %f", z, is_open, time_out,
          parameters.sutPlaceSensorAngle());
    bool result = pick_arm->motor_vcm2->MoveToPosSync(z - parameters.escapeHeight());

    if (result)
    {
        sut_vacuum->Set(false);
        result
                &= pick_arm->ZSerchByForce(1, parameters.vcm2Svel(), parameters.vcm2PickForce(), z, parameters.vcm2Margin(),
                                           parameters.vcm2FinishDelay(), is_open, false, time_out);
        result &= pick_arm->ZSerchReturn(1, time_out);
        pick_arm->motor_vcm2->MoveToPosSync(z - parameters.escapeHeight());
        QThread::msleep(200);
        result = pick_arm->motor_vcmx->StepMoveSync(-parameters.escapeX());
        result &= pick_arm->motor_vcm2->MoveToPosSync(0);
    }
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker1SearchSutZ2(double z, bool is_open, int time_out)
{
    qInfo("picker1SearchSutZ2 z %f is_open %d time_out %d", z, is_open, time_out);
    bool result = pick_arm->motor_vcm1->MoveToPosSync(z - parameters.escapeHeight());

    if (result)
    {
        sut_vacuum->Set(false);
        result
                = pick_arm->ZSerchByForce(0, parameters.vcm1Svel(), parameters.vcm1PickForce(), z, parameters.vcm1Margin(),
                                          parameters.vcm1FinishDelay(), is_open, false, time_out);
        QThread::msleep(100);
        result &= pick_arm->ZSerchReturn(0, time_out);
        QThread::msleep(200);
        pick_arm->motor_vcm1->MoveToPosSync(z - parameters.escapeHeight());
        QThread::msleep(200);
        result = pick_arm->motor_vcmx->StepMoveSync(-parameters.escapeX());
        result &= pick_arm->motor_vcm1->MoveToPosSync(0);
    }
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1SearchSutZ(double z, bool is_open, int time_out)
{
    qInfo("picker1SearchSutZ2 z %f is_open %d time_out %d", z, is_open, time_out);
    bool result = pick_arm->motor_vcm1->MoveToPosSync(z - parameters.escapeHeight());

    if (result)
    {
        sut_vacuum->Set(false);
        result
                = pick_arm->ZSerchByForce(0, parameters.vcm1Svel(), parameters.vcm1PickForce(), z, parameters.vcm1Margin(),
                                          parameters.vcm1FinishDelay(), is_open, false, time_out);
        QThread::msleep(300);
        result &= pick_arm->ZSerchReturn(0, time_out);
        QThread::msleep(100);
        result &= pick_arm->motor_th1->MoveToPosSync(-parameters.sutPlaceSensorAngle());
        result &= pick_arm->motor_y->StepMoveSync(parameters.escapeY());
        QThread::msleep(100);
        result &= pick_arm->motor_vcm1->MoveToPosSync(0);
        //        result = pick_arm->motor_th1->MoveToPos(-parameters.sutPlaceSensorAngle());
    }
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker1MeasureHight(bool is_tray, bool is_product)
{
    qInfo("measureHight speed: %f force: %f", parameters.vcm1Svel(), parameters.vcm1PickForce());
    if (pick_arm->ZSerchByForce(0, parameters.vcm1Svel(), parameters.vcm1PickForce(), true))
    {
        QThread::msleep(100);
        if (!emit sendMsgSignal(tr(u8"提示"), tr(u8"是否应用此高度:%1？").arg(pick_arm->GetSoftladngPosition())))
        {
            return true;
        }
        if (is_product)
        {
            if (is_tray)
                pick_arm->parameters.setPlaceProductZ(pick_arm->GetSoftladngPosition());
            else
                pick_arm->parameters.setPickProductZ(pick_arm->GetSoftladngPosition());
        }
        else
        {
            if (is_tray)
                pick_arm->parameters.setPickLensZ(pick_arm->GetSoftladngPosition());
            else
                pick_arm->parameters.setPlaceLensZ(pick_arm->GetSoftladngPosition());
        }
        return true;
    }
    return false;
}
bool SingleHeadMachineMaterialLoaderModule::picker2MeasureHight(bool is_tray, bool is_product)
{
    qInfo("picker2MeasureHight is_tray %d is_product %d", is_tray, is_product);
    if (pick_arm->ZSerchByForce(1, parameters.vcm2Svel(), parameters.vcm2PickForce(), true))
    {
        QThread::msleep(100);
        if (!emit sendMsgSignal(tr(u8"提示"), tr(u8"是否应用此高度:%1").arg(pick_arm->GetSoftladngPosition(false, 1))))
        {
            return true;
        }
        if (is_tray)
        {
            if (is_product)
                pick_arm->parameters.setPlaceProductZ(pick_arm->GetSoftladngPosition(false, 1));
            else
            {

                pick_arm->parameters.setPickSensorZ(pick_arm->GetSoftladngPosition(false, 1));
            }
        }
        else
        {
            if (is_product)
                pick_arm->parameters.setPickProductZ(pick_arm->GetSoftladngPosition(false, 1));
            else
            {
                pick_arm->parameters.setPlaceSensorZ(pick_arm->GetSoftladngPosition(false, 1));
            }
        }
        return true;
    }
    AppendError(QString(u8"2号吸头测高失败"));
    return false;
}

void SingleHeadMachineMaterialLoaderModule::applyPrOffset(PositionT &offset)
{
    qInfo("Apply pr offst: x, y, theta: %f %f %f", offset.X(), offset.Y(), offset.Theta());
    pr_offset.X += offset.X();
    pr_offset.Y += offset.Y();
    pr_offset.Theta += offset.Theta();
}

bool SingleHeadMachineMaterialLoaderModule::moveToChangeTrayPos()
{
    qInfo("moveToChangeTrayPos %d tray_load %d");
    bool result = pick_arm->motor_x->MoveToPosSync(0);
    if (!result)
        AppendError(QString(u8"移动到换盘位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::towerLightBuzzerTest()
{
    towerLightBuzzer->blinkBuzzer();
    return true;
}

bool SingleHeadMachineMaterialLoaderModule::closeAllTowerLightBuzzer()
{
    towerLightBuzzer->closeBuzzer();
    return true;
}


void SingleHeadMachineMaterialLoaderModule::receiveLoadMaterialRequestResponse(bool isSutReadyToLoadMaterial,
                                                                               int productIndex)
{
    currentProductIndex = productIndex;
    this->states.setSutIsReadyToLoadMaterial(isSutReadyToLoadMaterial);
    completeLoad = false;
}

void SingleHeadMachineMaterialLoaderModule::run()
{
    is_run = true;
    int waitLastProductPlaceToTrayTimes = 0;
    while (is_run)
    {
        if (states.sutIsReadyToLoadMaterial())
        {
            if (lsutState->lutHasNgLens())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Please remove the LUT NG lens!");
                towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                int operation = waitMessageReturn(is_run);
                towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                qInfo("user operation: %d", operation);
                lsutState->setLutHasNgLens(false);
            }

            if (this->states.hasPickedLens() && lsutState->lutIsEmply()
                    && !lsutState->aaHeadHasLens())    // Place lens to LUT
            {
                qInfo("Place lens to LUT");
                QPointF point = lut_pr_position.ToPointF();
                point.setX(point.x() + camera_to_picker1_offset.X());
                point.setY(point.y() + camera_to_picker1_offset.Y());
                pick_arm->move_XmY_Synic(point);
                picker1PlaceLensToLUT();
                lsutState->setCurrentLensIndex(states.currentLensIndexInPicker());
                states.setHasPickedLens(false);
                lsutState->setLutHasLens(true);
                qInfo("Place lens to LUT Finished");
                if (!is_run)
                    break;
            }

            if (lsutState->sutHasNgSensor() && states.picker1IsIdle())
            {
                qInfo("Pick the ng sensor from sut");
                moveToSUTPRPos();
                pr_offset.ReSet();
                if (!performSUTSensorPR())
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Perform SUTSensor PR fail, please check");
                    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                    int operation = waitMessageReturn(is_run);
                    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                    qInfo("user operation: %d", operation);
                    lsutState->setSutHasNgSensor(false);    // user removed the ng sensor
                }
                else
                {
                    moveToPicker1WorkPos();
                    picker1PickNgSensorFromSUT();
                    qInfo("pick_arm->vacuum_lens_suction name: %s",
                          pick_arm->vacuum_lens_suction->GetName().toStdString().c_str());
                    if (!pick_arm->vacuum_lens_suction->GetVacuumState())
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry,
                                         "Pick ng sensor Fail.Please remove the ng sensor!");
                        towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                        int operation = waitMessageReturn(is_run);
                        towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                        qInfo("user operation: %d", operation);
                        lsutState->setSutHasNgSensor(false);
                        // Testing the reset theta motor position
                        pick_arm->motor_th1->MoveToPos(0);
                    }
                    else
                    {
                        lsutState->setSutHasNgSensor(false);
                        this->states.setHasPickedNgSensor(true);
                    }
                }
                if (!is_run)
                    break;
            }

            if (lsutState->hasProduct() && states.picker1IsIdle())
            {    // Pick product form sut
                qInfo("Pick product from sut ");
                moveToSUTPRPos();
                pr_offset.ReSet();
                if (!performSUTProductPR())
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry,
                                     "Perform SUTProduct PR fail, please remove the product!");
                    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                    int operation = waitMessageReturn(is_run);
                    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                    qInfo("user operation: %d", operation);
                    lsutState->setHasProduct(false);
                }
                else
                {
                    pr_offset.Theta = 0;    // ignore angle
                    moveToPicker1WorkPos();
                    picker1PickProductFormSUT();
                    if (!pick_arm->vacuum_lens_suction->GetVacuumState())
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Pick Product Fail. Please remove the product");
                        towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                        int operation = waitMessageReturn(is_run);
                        towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                        qInfo("user operation: %d", operation);
                        lsutState->setHasProduct(false);
                        pick_arm->motor_th1->MoveToPos(0);
                    }
                    else
                    {
                        lsutState->setHasProduct(false);
                        states.setHasPickedProduct(true);
                        states.setIsPickedProductOk(lsutState->isProductOk());
                    }
                }
                if (!is_run)
                    break;
            }

            if (states.hasPickedSensor() && lsutState->sutIsEmply())
            {    // Place Sensor to SUT
                qInfo("Place sensor to SUT");
                moveToSUTPRPos();
                pr_offset.ReSet();
                if (!performSUTPR())
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Perform SUT PR fail, please check");
                    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                    int operation = waitMessageReturn(is_run);
                    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                    qInfo("user operation: %d", operation);
                    continue;
                }
                else
                {
                    moveToPicker2WorkPos();    // use this in fast mode
                    picker2PlaceSensorToSUT();
                    lsutState->setCurrentSensorIndex(states.currentSensorIndexInPicker());
                    states.setHasPickedSensor(false);
                    lsutState->setSutHasSensor(true);
                }
                if (!is_run)
                    break;
            }

            if (states.hasPickedLens() && lsutState->hasOkLens()
                    && picker1ShouldUnloadDutOnLSutFirst())    // place lens to tray
            {
                moveToLensTrayPos(states.currentLensTray());
                performLensVacancyPR();
                moveToPicker1WorkPos();
                picker1PlaceLensToTray();
                lensTray->setCurrentMaterailStateWithInit(states.currentLensTray());
                states.setHasPickedLens(false);
                continue;
            }

            if (lsutState->sutHasSensor() && lsutState->hasOkLens() && !lsutState->lutHasNgLens())
            {
                qInfo("Both sensor and lens have already loaded into sut and lut");
                emit sendLoadMaterialFinishSignal();
                states.setSutIsReadyToLoadMaterial(false);
            }
        }

        if (this->states.hasPickedNgSensor())
        {    // Place the ng sensor to tray
            qInfo("Place the ng sensor to tray");
            if (moveToNextRejectTrayPos(states.currentRejectTray()))
            {
                pr_offset.ReSet();
                if (parameters.performPrAsPlacingNgDut())
                {
                    performSensorVacancyPR();
                }
                else
                {
                    applyPrOffset(placeNgSensorToTrayOffset);
                }
                moveToPicker1WorkPos();
                picker1PlaceNgSensorToTray();
                states.setHasPickedNgSensor(false);
                rejectTray->setCurrentMaterialState(MaterialState::IsNg, states.currentRejectTray());
            }
            else
            {
                moveToChangeTrayPos();
                sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Reject Tray Full. Please clear the reject tray");
                towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                int operation = waitMessageReturn(is_run);
                towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                qInfo("user operation: %d", operation);
                rejectTray->resetTrayState(states.currentRejectTray());
                continue;
            }
            if (!is_run)
                break;
        }

        if (this->states.hasPickedProduct() && states.isPickedProductOk())
        {    // Place product to tray
            qInfo("Place Product to Sensor Tray");

            if (!pickSensorPoses.contains(currentProductIndex))
            {
                qCritical("Did not find picking sensor position. Index: %d", currentProductIndex);
                sendAlarmMessage(ErrorLevel::ContinueOrRetry,
                                 "Did not know where the product should be placed to! Please remove the product!");
                towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                int operation = waitMessageReturn(is_run);
                towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                qInfo("user operation: %d", operation);
                states.setHasPickedProduct(false);
            }
            else
            {
                PickArmPos recordPickSensorPos = pickSensorPoses.value(currentProductIndex);
                recordPickSensorPos.TL_X
                        += camera_to_picker1_offset.X() - camera_to_picker2_offset.X() + placeOkProductToTrayOffset.X();
                recordPickSensorPos.PA_Y
                        += camera_to_picker1_offset.Y() - camera_to_picker2_offset.Y() + placeOkProductToTrayOffset.Y();
                if (pick_arm->move_XYXm_Sync(recordPickSensorPos))
                {
                    picker1PlaceProductToTray();
                    states.setHasPickedProduct(false);
                    sensorTray->setCurrentMaterialState(MaterialState::IsProduct, states.currentSensorTray());
                }
                else
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry,
                                     "Move failed! Please remove the product, then home the motor!");
                    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                    int operation = waitMessageReturn(is_run);
                    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                    qInfo("user operation: %d", operation);
                    states.setHasPickedProduct(false);
                }
            }

            if (!is_run)
                break;
        }

        if (this->states.hasPickedProduct() && !states.isPickedProductOk())
        {    // Place the ng product to tray
            qInfo("Place the ng product to tray");
            if (moveToNextRejectTrayPos(states.currentRejectTray()))
            {
                pr_offset.ReSet();
                if (parameters.performPrAsPlacingNgDut())
                {
                    performSensorVacancyPR();
                }
                else
                {
                    applyPrOffset(placeNgProductToTrayOffset);
                }
                moveToPicker1WorkPos();
                picker1PlaceProductToTray();
                states.setHasPickedProduct(false);
                rejectTray->setCurrentMaterialState(MaterialState::IsNg, states.currentRejectTray());
            }
            else
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Reject Tray Full. Please clear the reject tray");
                towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                int operation = waitMessageReturn(is_run);
                towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                qInfo("user operation: %d", operation);
                rejectTray->resetTrayState(states.currentRejectTray());
                continue;
            }
            if (!is_run)
                break;
        }

        if (states.picker1IsIdle() && !picker1ShouldUnloadDutOnLSutFirst() && !shouldLoadSensorToLSutFirst())
        {
            qInfo("picke lens from tray");
            if (moveToNextLensTrayPos(states.currentLensTray()))
            {
                pr_offset.ReSet();

                QElapsedTimer timer;
                timer.start();
                bool res = performLensPR();
                int elapsed = timer.elapsed();
                if (elapsed > 1000)
                {
                    qCritical("Perform lens pr cost too long time. %d ms", elapsed);
                }

                if (!res)
                {
                    lensPrFailedTimes++;
                    if (lensPrFailedTimes >= MaxPickDutFailedTimes)
                    {
                        lensPrFailedTimes = 0;
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Perform Lens PR fail");
                        towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                        int operation = waitMessageReturn(is_run);
                        towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                        qInfo("user operation: %d", operation);
                    }
                    else
                    {
                        qCritical("Perform lens PR failed! Auto skip this lens. Continuous lens pr failed times: %d.",
                                  lensPrFailedTimes);
                    }
                    lensTray->setCurrentMaterialState(MaterialState::IsNg, states.currentLensTray());
                    continue;
                }
                else
                {
                    lensPrFailedTimes = 0;
                    moveToPicker1WorkPos();
                    picker1PickLensFromTray();
                    if (!pick_arm->vacuum_lens_suction->GetVacuumState())
                    {
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Pick lens from lens tray fail");
                        towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                        int operation = waitMessageReturn(is_run);
                        towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                        qInfo("user operation: %d", operation);
                        lensTray->setCurrentMaterialState(MaterialState::IsNg, states.currentLensTray());
                        continue;
                    }
                    else
                    {
                        states.setHasPickedLens(true);
                        states.setCurrentLensIndexInPicker(lensTray->getCurrentIndex());
                        lensTray->setCurrentMaterialState(MaterialState::IsInUse, states.currentLensTray());
                    }
                }
            }
            else
            {
                moveToChangeTrayPos();
                sendAlarmMessage(ErrorLevel::ContinueOrRetry, "lens tray is empty. Please change the lens tray");
                towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                int operation = waitMessageReturn(is_run);
                towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                qInfo("user operation: %d", operation);
                lensTray->resetTrayState(states.currentLensTray());
                continue;
            }
            if (!is_run)
                break;
        }

        if (!states.hasPickedSensor() && !picker1ShouldUnloadDutOnLSutFirst())
        {    // Pick sensor form tray
            if (sensorTray->findNextPositionOfInitState(states.currentSensorTray()))
            {
                qInfo("Pick sensor form tray");
                if (moveToNextSensorTrayPos(states.currentSensorTray()))
                {
                    pr_offset.ReSet();
                    if (!performSensorPR())
                    {
                        sensorPrFailedTimes++;
                        if (sensorPrFailedTimes >= MaxPickDutFailedTimes)
                        {
                            sensorPrFailedTimes = 0;
                            sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Perform Sensor PR fail");
                            towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                            int operation = waitMessageReturn(is_run);
                            towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                            qInfo("user operation: %d", operation);
                        }
                        else
                        {
                            qCritical("Perform sensor PR failed! Auto skip this sensor. Continuous sensor pr failed "
                                      "times: %d.",
                                      sensorPrFailedTimes);
                        }
                        sensorTray->setCurrentMaterialState(MaterialState::IsNg, states.currentSensorTray());
                        continue;
                    }
                    else
                    {
                        sensorPrFailedTimes = 0;
                        moveToPicker2WorkPos();
                        PickArmPos tmpPickSensorPos;
                        tmpPickSensorPos.TL_X = pick_arm->motor_x->GetFeedbackPos();
                        tmpPickSensorPos.PA_X = pick_arm->motor_vcmx->GetFeedbackPos();
                        tmpPickSensorPos.PA_Y = pick_arm->motor_y->GetFeedbackPos();
                        picker2PickSensorFromTray();
                        if (!pick_arm->vacuum_sensor_suction->GetVacuumState())
                        {
                            pickSensorFailedTimes++;
                            if (pickSensorFailedTimes >= MaxPickDutFailedTimes)
                            {
                                pickSensorFailedTimes = 0;
                                SI::ui.getUIResponse("Error", "Pick sensor from tray failed!", MsgBoxIcon::Error,
                                                     SI::ui.Retry);
                            }
                            else
                            {
                                qInfo("Pick sensor failed, Auto skipped!");
                            }

                            sensorTray->setCurrentMaterialState(MaterialState::IsNg, states.currentSensorTray());
                            continue;
                        }
                        else
                        {
                            pickSensorFailedTimes = 0;
                            pickSensorPoses[sensorTray->getCurrentIndex()] = tmpPickSensorPos;
                            states.setHasPickedSensor(true);
                            states.setCurrentSensorIndexInPicker(sensorTray->getCurrentIndex());
                            sensorTray->setCurrentMaterialState(MaterialState::IsInUse, states.currentSensorTray());
                        }
                    }
                }
                else
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry, "moveToNextSensorTrayPos fail");
                    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                    waitMessageReturn(is_run);
                    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                }
            }
            else    // need to change tray
            {
                if (states.sutIsReadyToLoadMaterial() && !states.hasPickedProduct() && !lsutState->hasProduct()
                        && !lsutState->sutHasSensor())
                {
                    waitLastProductPlaceToTrayTimes = 0;

                    moveToChangeTrayPos();
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry,
                                     "sensor tray is empty. Please change the sensor tray");
                    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Red);
                    int operation = waitMessageReturn(is_run);
                    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
                    qInfo("user operation: %d", operation);
                    sensorTray->resetTrayState(states.currentSensorTray());
                    pickSensorPoses.clear();
                    continue;
                }
                else    // 本盘Sensor，还有Sensor/Product还在SUT上，未放回SensorTray盘
                {
                    QThread::msleep(50);
                    waitLastProductPlaceToTrayTimes++;
                    if (waitLastProductPlaceToTrayTimes > 20)    //加额外Log，以免程序死循环难以发现。
                    {
                        qInfo("Waiting last product being placed to sensor tray!");
                        waitLastProductPlaceToTrayTimes = 0;
                    }
                    continue;
                }
            }
            if (!is_run)
                break;
        }

        if (!states.sutIsReadyToLoadMaterial() && states.hasPickedLens() && states.hasPickedSensor())
        {
            if (!completeLoad)
            {
                qDebug() << "MaterialLoader is idle, so moveToLUTPRPos";
                moveToLUTPRPos();
                completeLoad = true;
            }

            if (!is_run)
                break;
        }

        if (completeLoad)
        {
            QThread::msleep(50);
        }
    }
}

void SingleHeadMachineMaterialLoaderModule::startWork(int run_mode)
{
    qInfo("MaterailLoader start run_mode :%d in %d", run_mode, QThread::currentThreadId());
    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Green);
    if (run_mode == RunMode::Normal)
        run();
    else if (run_mode == RunMode::NoMaterial)
    {
        qInfo("Run in burn in mode");
        is_run = true;
        while (is_run)
        {
            moveToLensTrayStartPos(0);
            QThread::msleep(1000);
            moveToLensTrayEndPos();
            QThread::msleep(1000);
        }
    }

}

void SingleHeadMachineMaterialLoaderModule::stopWork(bool wait_finish)
{
    qInfo("MaterailLoader stop Work");
    is_run = false;
    towerLightBuzzer->switchColor(TowerLightBuzzer::TowerLightColor::Yellow);
}

void SingleHeadMachineMaterialLoaderModule::resetLogic()
{
    qInfo("resetLogic is called");
    sensorTray->resetTrayState(sensorTray->getCurrentIndex());
    lensTray->resetTrayState(lensTray->getCurrentIndex());
    rejectTray->resetTrayState(rejectTray->getCurrentIndex());
    this->states.setSutIsReadyToLoadMaterial(false);
    this->states.setHasPickedSensor(false);
    this->states.setHasPickedProduct(false);
    this->states.setHasPickedNgSensor(false);
    this->states.setHasPickedLens(false);
    this->states.setHasPickedNgLens(false);
    this->states.setIsPickedProductOk(false);

    this->states.setCurrentSensorTray(0);
    this->states.setCurrentLensTray(0);
    this->states.setCurrentRejectTray(0);

    completeLoad = false;
}

void SingleHeadMachineMaterialLoaderModule::performHandlingOperation(int cmd)
{
    qInfo("performHandling %d", cmd);
    bool result;
    int handlePosition = cmd & HANDLE_POSITION;
    switch (handlePosition)
    {
    case LENS_TRAY1:
    {
        qInfo("pick arm move to lens Tray1 current position, cmd: %d", LENS_TRAY1);
        result = moveToLensTrayPos(0);
    }
        break;
    case LENS_TRAY2:
    {
        qInfo("pick arm move to lens Tray2 current position, cmd: %d", LENS_TRAY2);
        result = moveToLensTrayPos(1);
    }
        break;
    case LUT_PR_POS:
    {
        qInfo("pick arm move to LUT pr position,cmd: %d", LUT_PR_POS);
        result = moveToLUTPRPos();
    }
        break;
    case LENS_TRAY1_START_POS:
    {
        qInfo("pick arm move to lens tray1 start position,cmd: %d", LENS_TRAY1_START_POS);
        result = moveToLensTrayStartPos(0);
    }
        break;
    case LENS_TRAY2_START_POS:
    {
        qInfo("pick arm move to lens tray2 start position,cmd: %d", LENS_TRAY2_START_POS);
        result = moveToLensTrayStartPos(1);
    }
        break;
    case LENS_TRAY1_END_POS:
    {
        qInfo("pick arm move to lens tray1 end position,cmd: %d", LENS_TRAY1_END_POS);
        result = moveToLensTrayEndPos();
    }
        break;
    case SENSOR_TRAY:
    {
        qInfo("pick arm move to sensor tray current position,cmd: %d", SENSOR_TRAY);
        result = moveToSensorTrayPos(0);
    }
        break;
    case SUT_PR_POS:
    {
        qInfo("pick arm move to sut pr position,cmd: %d", SUT_PR_POS);
        result = moveToSUTPRPos();
    }
        break;
    case SENSOR_TRAY_START_POS:
    {
        qInfo("pick arm move to sensor tray start position,cmd: %d", SENSOR_TRAY_START_POS);
        result = moveToSensorTrayStartPos(0);
    }
        break;
    case SENSOR_TRAY_END_POS:
    {
        qInfo("pick armmove to snesor tray end position,cmd: %d", SENSOR_TRAY_END_POS);
        result = moveToSensorTrayEndPos();
    }
        break;
    case REJECT_TRAY:
    {
        qInfo("pick arm move to reject tray current position,cmd: %d", REJECT_TRAY);
        result = moveToRejectTrayPos(0);
    }
        break;
    case REJECT_TRAY_START_POS:
    {
        qInfo("pick arm move to reject tray start position,cmd: %d", REJECT_TRAY_START_POS);
        result = moveToRejectTrayStartPos(0);
    }
        break;
    case REJECT_TRAY_END_POS:
    {
        qInfo("pick arm move to reject tray end position,cmd: %d", REJECT_TRAY_END_POS);
        result = moveToRejectTrayEndPos();
    }
        break;
    default:
        result = true;
        break;
    }
    if (!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock, GetCurrentError());
        return;
    }
    int handlePR = cmd & HANDLE_PR;
    switch (handlePR)
    {
    case RESET_PR:
    {
        qInfo("reset PR result,cmd: %d", RESET_PR);
        pr_offset.ReSet();
    }
        break;
    case SENSOR_PR:
    {
        qInfo("perform sensor PR,cmd: %d", SENSOR_PR);
        result = performSensorPR();
    }
        break;
    case SENSOR_VACANCY_PR:
    {
        qInfo("perform sensor tray vacancy PR, cmd: %d", SENSOR_VACANCY_PR);
        result = performSensorVacancyPR();
    }
        break;
    case SUT_PR:
    {
        qInfo("perform SUT vacancy PR,cmd: %d", SUT_PR);
        result = performSUTPR();
    }
        break;
    case NG_SENSOR_PR:
    {
        qInfo("perform SUT Ng Sensor PR,cmd: %d", NG_SENSOR_PR);
        result = performSUTSensorPR();
    }
        break;
    case PRODUCT_PR:
    {
        qInfo("perform SUT product PR,cmd: %d", PRODUCT_PR);
        result = performSUTProductPR();
    }
        break;
    case LENS_PR:
    {
        qInfo("perform lens PR,cmd: %d", LENS_PR);
        result = performLensPR();
    }
        break;
    case NG_LENS_PR:
    {
        qInfo("perform ng lens PR, cmd: %d", NG_LENS_PR);
        result = performLUTLensPR();
    }
        break;
    case LENS_VACANCY_PR:
    {
        qInfo("perform lens tray vacancy PR,cmd: %d", LENS_VACANCY_PR);
        result = performLensVacancyPR();
    }
        break;
    case LUT_PR:
    {
        qInfo("perform LUT vacancy PR,cmd: %d", LUT_PR);
        result = performLUTPR();
    }
        break;

    default:
        result = true;
        break;
    }
    if (!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock, GetCurrentError());
        return;
    }
    int handleApplyOffset = cmd & HANDLE_PR_OFFSET;
    switch (handleApplyOffset)
    {
    case APPLY_PLACE_NG_SENSOR_TO_TRAY_OFFSET:
    {
        applyPrOffset(placeNgSensorToTrayOffset);
        break;
    }
    case APPLY_PLACE_NG_PRODUCT_TO_TRAY_OFFSET:
    {
        applyPrOffset(placeNgProductToTrayOffset);
        break;
    }
    }
    int handleToWorkPos = cmd & HANDLE_TO_WORKPOS;
    switch (handleToWorkPos)
    {
    case PICKER1_TO_WORKPOS:
    {
        qInfo("lens suction move offset,cmd: %d", PICKER1_TO_WORKPOS);
        result = moveToPicker1WorkPos();
    }
        break;
    case PICKER2_TO_WORKPOS:
    {
        qInfo("sensor suction move offset,cmd: %d", PICKER2_TO_WORKPOS);
        result = moveToPicker2WorkPos();
    }
        break;

    default:
        result = true;
        break;
    }
    if (!result)
    {
        qInfo("Move To Work Pos fail");
        return;
    }
    int handlePickerAction = cmd & HANDLE_PICKER_ACTION;
    switch (handlePickerAction)
    {
    case PICKER2_PICK_SENSOR_FROM_TRAY:
    {
        qInfo("pick sensor from tray,cmd: %d", PICKER2_PICK_SENSOR_FROM_TRAY);
        result = picker2PickSensorFromTray();
    }
        break;
    case PICKER2_PLACE_SENSOR_TO_SUT:
    {
        qInfo("place sensor to SUT,cmd: %d", PICKER2_PLACE_SENSOR_TO_SUT);
        result = picker2PlaceSensorToSUT();
    }
        break;
    case PICKER1_PLACE_PRODUCT_TO_TRAY:
    {
        qInfo("place product to tray,cmd: %d", PICKER1_PLACE_PRODUCT_TO_TRAY);
        result = picker1PlaceProductToTray();
    }
        break;
    case PICKER1_PLACE_NG_PRODUCT_TO_TRAY:
    {
        qInfo("place ng product to tray,cmd: %d", PICKER1_PLACE_NG_PRODUCT_TO_TRAY);
        result = picker1PlaceProductToTray();    // use place product ation for NG
    }
        break;

    case PICKER2_PICK_NG_SENSOR_FROM_SUT:
    {
        qInfo("pick ng Sensor from SUT,cmd: %d", PICKER2_PICK_NG_SENSOR_FROM_SUT);
        result = picker2PickNgSensorFromSUT();
    }
        break;
    case PICKER1_PICK_NG_SENSOR_FROM_SUT:
    {
        qInfo("pick ng Sensor from SUT,cmd: %d", PICKER1_PICK_NG_SENSOR_FROM_SUT);
        result = picker1PickNgSensorFromSUT();
    }
        break;
    case PICKER1_PLACE_NG_SENSOR_TO_TRAY:
    {
        qInfo("place ng sensor to tray,cmd: %d", PICKER1_PLACE_NG_SENSOR_TO_TRAY);
        result = picker1PlaceNgSensorToTray();
    };
        break;
    case PICKER2_PLACE_NG_SENSOR_TO_TRAY:
    {
        qInfo("move to place sensor to tray,cmd: %d", PICKER2_PLACE_NG_SENSOR_TO_TRAY);
        result = picker2PlaceNgSensorToTray();
    }
        break;
    case PICKER2_MEASURE_SENSOR_IN_TRAY:
    {
        qInfo("mesure pick senso from tray height, cmd: %d", PICKER2_MEASURE_SENSOR_IN_TRAY);

        result = picker2MeasureHight(true, false);
    }
        break;
    case PICKER1_MEASURE_PRODUCT_IN_TRAY:
    {
        qInfo("measure place product to tray height, cmd: %d ", PICKER1_MEASURE_PRODUCT_IN_TRAY);
        result = picker1MeasureHight(true, true);
    }
        break;
    case PICKER2_MEASURE_SENSOR_IN_SUT:
    {
        qInfo("measure place sensor to SUT,cmd: %d", PICKER2_MEASURE_SENSOR_IN_SUT);

        result = picker2MeasureHight(false, false);
    }
        break;
    case PICKER1_MEASURE_PRODUCT_IN_SUT:
    {
        qInfo("measure pick product form SUT, cmd: %d", PICKER1_MEASURE_PRODUCT_IN_SUT);
        result = picker1MeasureHight(false, true);
    }
        break;
    case PICKER1_PICK_LENS_FROM_TRAY:
    {
        qInfo("pick lens form tray, cmd: %d", PICKER1_PICK_LENS_FROM_TRAY);
        result = picker1PickLensFromTray();
    }
        break;
    case PICKER1_PLACE_LENS_TO_LUT:
    {
        qInfo("place lens to SUT,cmd: %d", PICKER1_PLACE_LENS_TO_LUT);
        result = picker1PlaceLensToLUT();
    }
        break;
    case PICKER1_PICK_PRODUCT_FROM_SUT:
    {
        qInfo("pick product form SUT,cmd: %d", PICKER1_PICK_PRODUCT_FROM_SUT);
        result = picker1PickProductFormSUT();
    }
        break;
    case PICKER1_PLACE_LENS_TO_TRAY:
    {
        qInfo("place lens to tray,cmd: %d", PICKER1_PLACE_LENS_TO_TRAY);
        result = picker1PlaceLensToTray();
    }
        break;
    case PICKER2_PLACE_SENSOR_TO_TRAY:
    {
        qInfo("place sensor to tray,cmd: %d", PICKER2_PLACE_SENSOR_TO_TRAY);
        result = picker2PlaceSensorToTray();
    }
        break;
    case PICKER1_PICK_NG_LENS_FROM_LUT:
    {
        qInfo("pick ng lens from LUT,cmd: %d", PICKER1_PICK_NG_LENS_FROM_LUT);
        result = picker1PickNgLensFromLUT();
    }
        break;
    case PICKER1_PLACE_NG_LENS_TO_TRAY:
    {
        qInfo("place ng lens to tray,cmd: %d", PICKER1_PLACE_NG_LENS_TO_TRAY);
        result = picker1PlaceNgLensToTray();
    }
        break;
    case PICKER2_PLACE_NG_LENS_TO_TRAY:
    {
        qInfo("place ng lens to tray,cmd: %d", PICKER2_PLACE_NG_LENS_TO_TRAY);
        result = picker2PlaceNgLensToTray();
    }
        break;
    case PICKER2_PICK_NG_LENS_FROM_LUT:
    {
        qInfo("pick ng lens form lut,cmd: %d", PICKER2_PICK_NG_LENS_FROM_LUT);
        result = picker2PickNgLensFromLUT();
    }
        break;
    case PICKER1_MEASURE_LENS_IN_TRAY:
    {
        qInfo("measure pick lens from tray height,cmd: %d", PICKER1_MEASURE_LENS_IN_TRAY);
        result = picker1MeasureHight(true);
    }
        break;
    case PICKER1_MEASURE_LENS_IN_LUT:
    {
        qInfo("measuer place lens to LUT height,cmd: %d", PICKER1_MEASURE_LENS_IN_LUT);
        result = picker1MeasureHight(false);
    }
        break;
    default:
        result = true;
        break;
    }
    return;
}
