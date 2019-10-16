﻿#include "SingleHead/singleheadmachinematerialloadermodule.h"
#include <QMessageBox>
#include <QElapsedTimer>


SingleHeadMachineMaterialLoaderModule::SingleHeadMachineMaterialLoaderModule(QString name)
    :ThreadWorkerBase (name)
{

}

void SingleHeadMachineMaterialLoaderModule::Init(SingleHeadMachineMaterialPickArm *pick_arm,
                                                 MaterialTray* sensorTray,
                                                 MaterialTray* lensTray,
                                                 MaterialTray* rejectTray,
                                                 VisionLocation* sensor_vision,
                                                 VisionLocation* sensor_vacancy_vision,
                                                 VisionLocation* sut_vision,
                                                 VisionLocation* sut_sensor_vision,
                                                 VisionLocation* sut_product_vision,
                                                 VisionLocation* lens_vision,
                                                 VisionLocation* lens_vacancy_vision,
                                                 VisionLocation* lut_vision,
                                                 VisionLocation* lut_lens_vision,
                                                 VisionLocation* camera_to_picker_offest_vision,
                                                 XtVacuum* sutVacuum,
                                                 XtVacuum* lutVacuum)
{
    this->pick_arm = pick_arm;
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
    this->camera_to_picker_offest_vision = camera_to_picker_offest_vision;
    this->sut_vacuum = sutVacuum;
    this->lut_vacuum = lutVacuum;
}

void SingleHeadMachineMaterialLoaderModule::loadJsonConfig(QString file_name)
{

}

void SingleHeadMachineMaterialLoaderModule::saveJsonConfig(QString file_name)
{

}

void SingleHeadMachineMaterialLoaderModule::performHandling(int cmd)
{
    emit sendHandlingOperation(cmd);
    qInfo("emit performHandling %d",cmd);
}
void SingleHeadMachineMaterialLoaderModule::cameraTipOffsetCalibration(int pickhead)
{
    qInfo("Start camera tip offset calibration: %d", pickhead);
    //    moveToStartPos(1);
    moveToSensorTrayStartPos(0);
    QThread::msleep(1000);
    PrOffset offset;
    //    this->sensor_pickarm_calibration_glass_location->performPR(offset);
    this->camera_to_picker_offest_vision->performPR(offset);
    std::vector<cv::Point2d> points;
    if (pickhead == 0)
    {
        this->pick_arm->stepMove_XmYT1_Synic(-offset.X,-offset.Y, -50);
        for (int i = 0; i < 5; i++)
        {
            PrOffset offset;
            QThread::msleep(1000);
            //            this->sensor_pickarm_calibration_glass_location->performPR(offset);
            this->camera_to_picker_offest_vision->performPR(offset);

            qInfo("PR offset: %f %f", offset.X, offset.Y);
            points.push_back(cv::Point2d(offset.X, offset.Y));
            this->pick_arm->stepMove_XmYT1_Synic(camera_to_picker1_offset.X(), camera_to_picker1_offset.Y(), 0);
            //            bool result = pick_arm->Z1SerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),15,parameters.vcmMargin(),parameters.finishDelay(),true,true,30000);
            bool result = pick_arm->ZSerchByForce(0,parameters.vcm1Svel(),parameters.vcm1PickForce(),15,parameters.vcm1Margin(),parameters.vcm1FinishDelay(),true,true,30000);

            QThread::msleep(1000);
            result &= pick_arm->ZSerchReturn(30000);
            this->pick_arm->stepMove_XmYT1_Synic(0, 0, 20);
            //            result = pick_arm->Z1SerchByForce(parameters.vcmWorkSpeed(),parameters.vcmWorkForce(),15,parameters.vcmMargin(),parameters.finishDelay(),false,true,30000);
            result = pick_arm->ZSerchByForce(0,parameters.vcm1Svel(),parameters.vcm1PickForce(),15,parameters.vcm1Margin(),parameters.vcm1FinishDelay(),true,true,30000);

            QThread::msleep(1000);
            result &= pick_arm->ZSerchReturn(30000);
            this->pick_arm->stepMove_XmYT1_Synic(-camera_to_picker1_offset.X(), -camera_to_picker1_offset.X(), 0);
        }
        this->pick_arm->stepMove_XmYT1_Synic(0,0, -100);
        cv::Point2d center; double radius;
        fitCircle(points, center, radius);
        qInfo("Fit cicle: x: %f y: %f r:%f", center.x, center.y, radius);
        this->camera_to_picker1_offset.setX(camera_to_picker1_offset.X() + center.x);
        this->camera_to_picker1_offset.setY(camera_to_picker1_offset.Y() + center.y);
    } else {
        this->pick_arm->stepMove_XmYT2_Synic(-offset.X,-offset.Y, -50);
        for (int i = 0; i < 5; i++)
        {
            PrOffset offset;
            QThread::msleep(1000);
            //            this->sensor_pickarm_calibration_glass_location->performPR(offset);
            this->camera_to_picker_offest_vision->performPR(offset);
            qInfo("PR offset: %f %f", offset.X, offset.Y);
            points.push_back(cv::Point2d(offset.X, offset.Y));
            this->pick_arm->stepMove_XmYT2_Synic(camera_to_picker2_offset.X(), camera_to_picker2_offset.Y(), 0);
            //            bool result = pick_arm->Z2SerchByForce(parameters.vcmWorkSpeed(),parameters.pickProductForce(),15,parameters.vcmMargin(),parameters.finishDelay(),true,true,30000);
            bool result = pick_arm->ZSerchByForce(1,parameters.vcm2Svel(),parameters.vcm2PickForce(),15,parameters.vcm2Margin(),parameters.vcm2FinishDelay(),true,true,30000);

            QThread::msleep(1000);
            result &= pick_arm->ZSerchReturn(30000);
            this->pick_arm->stepMove_XmYT2_Synic(0, 0, 20);
            //            result = pick_arm->Z2SerchByForce(parameters.vcmWorkSpeed(),parameters.pickProductForce(),15,parameters.vcmMargin(),parameters.finishDelay(),false,true,30000);
            result = pick_arm->ZSerchByForce(1,parameters.vcm2Svel(),parameters.vcm2PickForce(),15,parameters.vcm2Margin(),parameters.vcm2FinishDelay(),true,true,30000);

            QThread::msleep(1000);
            result &= pick_arm->ZSerchReturn(30000);
            this->pick_arm->stepMove_XmYT2_Synic(-camera_to_picker2_offset.X(), -camera_to_picker2_offset.Y(), 0);
        }
        this->pick_arm->stepMove_XmYT2_Synic(0,0, -100);
        cv::Point2d center; double radius;
        fitCircle(points, center, radius);
        qInfo("Fit cicle: x: %f y: %f r:%f", center.x, center.y, radius);
        this->camera_to_picker2_offset.setX(camera_to_picker2_offset.X() + center.x);
        this->camera_to_picker2_offset.setY(camera_to_picker2_offset.Y() + center.y);
    }
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayPos(int index, int tray_index)
{
    qInfo("moveToTrayPos index %d tray_index %d",index,tray_index);
    bool result = pick_arm->move_XY_Synic(lensTray->getPositionByIndex(index,tray_index));
    if(!result)
        AppendError(QString(u8"移动到lens盘%1号位置失败").arg(index));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayPos(int tray_index)
{
    qInfo("moveToTrayPos tray_index %d",tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(lensTray->getCurrentPosition(tray_index),false);
    if(!result)
        AppendError(QString(u8"移动到lens盘当前位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayStartPos(int tray_index)
{
    qInfo("moveToStartPos%d",tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(lensTray->getStartPosition(tray_index),false);
    if(!result)
        AppendError(QString(u8"移动到lens盘起始位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayEndPos()
{
    qInfo("moveToTrayEndPos");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(lensTray->getEndPosition(),false);
    if(!result)
        AppendError(QString(u8"移动到lens盘结束位置失败"));
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayEmptyPos(int index, int tray_index)
{
    if(index >= 0 && tray_index >= 0 && lensTray->getMaterialState(index,tray_index) == MaterialState::IsEmpty)
        return moveToLensTrayPos(index,tray_index);
    if(lensTray->findLastPositionOfState(MaterialState::IsEmpty,tray_index))
        return moveToLensTrayPos(tray_index);
    if(tray_index == 0&&lensTray->findLastPositionOfState(MaterialState::IsEmpty,1))
        return moveToLensTrayPos(1);
    return false;
}
bool SingleHeadMachineMaterialLoaderModule::moveToNextLensTrayPos(int tray_index)
{
    qInfo("moveToNextTrayPos tray_index %d",tray_index);
    bool result = lensTray->findNextPositionOfInitState(tray_index);
    if(result)
        result &=  pick_arm->move_XY_Synic(lensTray->getCurrentPosition(tray_index));
    if(!result)
        AppendError(QString(u8"移动到lens盘下一个位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSensorTrayPos(int index, int tray_index)
{
    qInfo("moveToTrayPos index %d tray_index %d",index,tray_index);
    bool result = pick_arm->move_XY_Synic(sensorTray->getPositionByIndex(index,tray_index));
    if(!result)
        AppendError(QString(u8"移动到sensor盘%1号位置失败").arg(index));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSensorTrayPos(int tray_index)
{
    qInfo("moveToSensorTrayPos");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(sensorTray->getCurrentPosition(tray_index),false);
    if(!result)
        AppendError(QString(u8"移动到sensor盘当前位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSensorTrayStartPos(int tray_index)
{
    qInfo("moveToStartPos%d");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(sensorTray->getStartPosition(tray_index),false);
    if(!result)
        AppendError(QString(u8"移动到%1盘起始位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSensorTrayEndPos()
{
    qInfo("moveToTray1EndPos");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(sensorTray->getEndPosition(),false);
    if(!result)
        AppendError(QString(u8"移动到sensor盘结束位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToNextSensorTrayPos(int tray_index)
{
    qInfo("moveToNextTrayPos tray_index %d",tray_index);
    bool result = sensorTray->findNextPositionOfInitState(tray_index);
    if(result)
        result &=  pick_arm->move_XY_Synic(sensorTray->getCurrentPosition(tray_index));
    if(!result)
        AppendError(QString(u8"移动到sensor盘下一个位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToRejectTrayPos(int index, int tray_index)
{
    qInfo("moveToRejectTrayPos index %d tray_index %d",index,tray_index);
    bool result = pick_arm->move_XY_Synic(rejectTray->getPositionByIndex(index,tray_index));
    if(!result)
        AppendError(QString(u8"移动到reject盘%1号位置失败").arg(index));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToRejectTrayPos(int tray_index)
{
    qInfo("moveToRejectTrayPos tray_index %d",tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(rejectTray->getCurrentPosition(tray_index),true);
    if(!result)
        AppendError(QString(u8"移动到reject盘当前位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToRejectTrayStartPos(int tray_index)
{
    qInfo("moveToRejectTrayStartPos%d",tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(rejectTray->getStartPosition(tray_index),true);
    if(!result)
        AppendError(QString(u8"移动到reject盘起始位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToRejectTrayEndPos()
{
    qInfo("moveToRejectTrayEndPos");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(rejectTray->getEndPosition(),true);
    if(!result)
        AppendError(QString(u8"移动到reject盘结束位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToNextRejectTrayPos(int tray_index)
{
    qInfo("moveToNextRejectTrayPos tray_index %d",tray_index);
    bool result = rejectTray->findNextPositionOfInitState(tray_index);
    if(result)
        result &=  pick_arm->move_XY_Synic(rejectTray->getCurrentPosition(tray_index));
    if(!result)
        AppendError(QString(u8"移动到reject盘下一个位置失败"));
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::moveToLUTPRPos(bool check_softlanding)
{
    qInfo("moveToLUTPRPos %d");
    bool result =  pick_arm->move_XmY_Synic(lut_pr_position.ToPointF(),check_softlanding);
    if(!result)
        AppendError(QString("移动LPA到LUT位置失败%1"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSUTPRPos(bool check_softlanding)
{
    qInfo("moveToSUTPRPos %d");
    bool result =  pick_arm->move_XmY_Synic(sut_pr_position.ToPointF(),check_softlanding);
    if(!result)
        AppendError(QString("移动SPA到SUT位置失败%1"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToPicker1WorkPos(bool check_softlanding)
{
    PrOffset temp(camera_to_picker1_offset.X() - pr_offset.X,camera_to_picker1_offset.Y() - pr_offset.Y,pr_offset.Theta);
    bool result = pick_arm->stepMove_XmYT1_Synic(temp.X,temp.Y,temp.Theta, check_softlanding);
    if(!result)
        AppendError(QString(u8"去1号吸头工作位置(step x %1,y %2,t %3)失败!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    return  result;
}
bool SingleHeadMachineMaterialLoaderModule::moveToPicker2WorkPos(bool check_softlanding)
{
    PrOffset temp(camera_to_picker2_offset.X() - pr_offset.X,camera_to_picker2_offset.Y() - pr_offset.Y,pr_offset.Theta);
    qInfo("moveToPicker2WorkPos offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XmYT2_Synic(temp.X,temp.Y,temp.Theta,check_softlanding);
    if(!result)
        AppendError(QString(u8"去2号吸头工作位置(step x %1,y %2,t %3)失败!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::performSensorPR()
{
    Sleep(100); //Position settling
    qInfo("performSensorPR");
    bool result = sensor_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行料盘sensor视觉失败!"));
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::performSensorVacancyPR()
{
    Sleep(100); //Position settling
    qInfo("performVacancyPR");
    bool result = sensor_vacancy_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行料盘空位视觉失败!"));
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::performSUTPR()
{
    Sleep(100); //Position settling
    qInfo("performSUTPR");
    bool result = sut_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行SUT视觉失败!"));
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::performSUTSensorPR()
{
    Sleep(100); //Position settling
    qInfo("performSUTSensorPR");
    bool result = sut_sensor_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行SUT上的sensor视觉失败!"));
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::performSUTProductPR()
{
    qInfo("performSUTProductPR");
    bool result = sut_product_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行SUT视觉失败!"));
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::performLensPR()
{
    qInfo("performLensPR");
    Sleep(100); //Position settling
    return  lens_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::performLensVacancyPR()
{
    Sleep(100); //Position settling
    return  lens_vacancy_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::performLUTPR()
{
    Sleep(100); //Position settling
    return lut_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::performLUTLensPR()
{
    Sleep(100); //Position settling
    return lut_lens_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::picker2PickSensorFromTray(int time_out)
{
    qInfo("pickTraySensor time_out %d",time_out);
    bool result = picker2SearchZ(pick_arm->parameters.pickSensorZ(),true,time_out, 1);
    if(!result)
        AppendError(QString(u8"从sensor盘取sensor失败"));
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker2PlaceSensorToSUT(int time_out)
{
    qInfo("placeSensorToSUT time_out %d",time_out);
    bool result = picker2SearchSutZ2(pick_arm->parameters.placeSensorZ(),false,time_out);
    if(!result)
        AppendError(QString(u8"放sensor到SUT%1失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PickNgSensorFromSUT(int time_out)
{
    qInfo("picker2PickNgSensorFromSUT time_out %d",time_out);
    bool result = picker2SearchSutZ(pick_arm->parameters.placeSensorZ(),true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT取NGsenor失败"));
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker1PickNgSensorFromSUT(int time_out)
{
    qInfo("pickSUTSensor  time_out %d",time_out);
    bool result = picker1SearchSutZ(pick_arm->parameters.placeSensorZ(),true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT%1取NGsenor失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PickProductFormSUT(int time_out)
{
    qInfo("pickSUTProduct time_out %d",time_out);
    bool result = picker1SearchSutZ(pick_arm->parameters.pickProductZ(),true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT%1取成品失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceNgSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d",time_out);
    bool result = picker1SearchZ(pick_arm->parameters.pickSensorZ(),false,time_out, 0);
    if(result)
    {
        result &= pick_arm->motor_th1->MoveToPosSync(0);
    }
    if(!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PlaceSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d",time_out);
    bool result = picker2SearchZ(pick_arm->parameters.pickSensorZ(),false,time_out, 1);

    if(!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PlaceNgSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d",time_out);
    bool result = picker2SearchZ(pick_arm->parameters.pickSensorZ(),false,time_out, 1);
    if(result)
    {
        result &= pick_arm->motor_th2->MoveToPosSync(0);
    }
    if(!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceProductToTray(int time_out)
{
    qInfo("placeProductToTray time_out %d",time_out);
    bool result = picker1SearchZ(pick_arm->parameters.placeProductZ(),false,time_out,0);
    if(result)
    {
        result &= pick_arm->motor_th1->MoveToPosSync(0);
    }
    if(!result)
        AppendError(QString(u8"将成品放入成品盘失败"));
    return result;
}


bool SingleHeadMachineMaterialLoaderModule::picker1PickLensFromTray(int time_out)
{
    qInfo("pickTrayLens");
    bool result = picker1SearchZ(pick_arm->parameters.pickLensZ(),true,time_out,0);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceLensToLUT(int time_out)
{
    qInfo("placeLensToLUT");
    bool result = picker1SearchZ(pick_arm->parameters.placeLensZ(), false, time_out, 0);
    this->lut_vacuum->Set(true, false);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PickNgLensFromLUT(int time_out)
{
    QElapsedTimer timer; timer.start();
    qInfo("pickLUTLens");
    this->lut_vacuum->Set(false);
    qInfo("cp1: %d", timer.elapsed());
    bool ret = picker2SearchZ(pick_arm->parameters.placeLensZ(),true, time_out,1);
    qInfo("cp2: %d", timer.elapsed());
    return ret;
}
bool SingleHeadMachineMaterialLoaderModule::picker1PickNgLensFromLUT(int time_out)
{
    QElapsedTimer timer; timer.start();
    qInfo("pickLUTLens");
    this->lut_vacuum->Set(false, false);
    qInfo("cp1: %d", timer.elapsed());
    bool ret = picker1SearchZ(pick_arm->parameters.placeLensZ(),true, time_out,0);
    qInfo("cp2: %d", timer.elapsed());
    return ret;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceNgLensToTray(int time_out)
{
    qInfo("placeLensToTray");
    return picker1SearchZ(pick_arm->parameters.pickLensZ(),false,time_out,0);
}
bool SingleHeadMachineMaterialLoaderModule::picker1PlaceLensToTray(int time_out)
{
    qInfo("placeLensToTray");
    return picker1SearchZ(pick_arm->parameters.pickLensZ(),false,time_out, 0);
}
bool SingleHeadMachineMaterialLoaderModule::picker2PlaceNgLensToTray(int time_out)
{
    qInfo("placeLensToTray");
    return picker2SearchZ(pick_arm->parameters.pickLensZ(),false,time_out,1);
}

bool SingleHeadMachineMaterialLoaderModule::picker2SearchZ(double z, bool is_open, int time_out,int picker)
{
    qInfo("picker2SearchZ z %f is_open %d timeout %d",z,is_open,time_out);
    bool result = pick_arm->ZSerchByForce(picker,parameters.vcm2Svel(),parameters.vcm2PickForce(),z,parameters.vcm2Margin(),parameters.vcm2FinishDelay(),is_open,false,time_out);
    result &= pick_arm->ZSerchReturn(picker,time_out);
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker1SearchZ(double z, bool is_open, int time_out,int picker)
{
    qInfo("picker1SearchZ z %f is_open %d timeout %d",z,is_open,time_out);
    QElapsedTimer timer; timer.start();
    bool result = pick_arm->ZSerchByForce(picker,parameters.vcm1Svel(),parameters.vcm1PickForce(),z,parameters.vcm1Margin(),parameters.vcm1FinishDelay(),is_open,false,time_out);
    qInfo("ZSerchByForce: %d", timer.elapsed()); timer.restart();
    result &= pick_arm->ZSerchReturn(picker,time_out);
    qInfo("ZSerchReturn: %d", timer.elapsed()); timer.restart();
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2SearchSutZ(double z, bool is_open, int time_out)
{
    qInfo("picker2SearchSutZ2 z %f is_open %d time_out %d",z,is_open,time_out);
    bool result = pick_arm->motor_vcm2->MoveToPosSync(z-parameters.escapeHeight());

    if(result)
    {
        sut_vacuum->Set(0);
        result = pick_arm->ZSerchByForce(1,parameters.vcm2Svel(),parameters.vcm2PickForce(),z,parameters.vcm2Margin(),parameters.vcm2FinishDelay(),is_open,false,time_out);
        result &= pick_arm->ZSerchReturn(1,time_out);
        QThread::msleep(200);
        result &=pick_arm->motor_th2->MoveToPosSync(-90);
        QThread::msleep(200);
        result &= pick_arm->motor_vcm2->MoveToPosSync(0);
    }
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2SearchSutZ2(double z,  bool is_open, int time_out)
{
    qInfo("picker2SearchSutZ z %f is_open %d time_out %d sutPlaceSensorAngle: %f",z,is_open,time_out, parameters.sutPlaceSensorAngle());
    bool result = pick_arm->motor_vcm2->MoveToPosSync(z-parameters.escapeHeight());

    if(result)
    {
        result =pick_arm->motor_th2->MoveToPosSync(parameters.sutPlaceSensorAngle());
        result &= pick_arm->ZSerchByForce(1,parameters.vcm2Svel(),parameters.vcm2PickForce(),z,parameters.vcm2Margin(),parameters.vcm2FinishDelay(),is_open,false,time_out);
        sut_vacuum->Set(true);
        //QThread::msleep(200);
        result &= pick_arm->ZSerchReturn(1,time_out);
        result &= pick_arm->motor_vcm2->MoveToPosSync(0);
    }
    result &= pick_arm ->motor_th2->MoveToPos(0);
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker1SearchSutZ2(double z,bool is_open, int time_out)
{
    qInfo("picker1SearchSutZ2 z %f dest %s cmd %s is_open %d time_out %d",z,is_open,time_out);
    bool result = pick_arm->motor_vcm1->MoveToPosSync(z-parameters.escapeHeight());
    if(result)
    {
        result = pick_arm->ZSerchByForce(0,parameters.vcm1Svel(),parameters.vcm1PickForce(),z,parameters.vcm1Margin(),parameters.vcm1FinishDelay(),is_open,false,time_out);
        sut_vacuum->Set(1);
        QThread::msleep(100);
        result &= pick_arm->ZSerchReturn(0,time_out);
    }
    result &= pick_arm->move_XmY_Z1_XmY(0,parameters.escapeX(),parameters.escapeY());
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1SearchSutZ(double z,bool is_open, int time_out)
{
    qInfo("picker1SearchSutZ z %f is_open %d time_out %d",z,is_open,time_out);
    bool result = pick_arm->motor_vcm1->MoveToPosSync(z-parameters.escapeHeight());

    if(result)
    {
        sut_vacuum->Set(0);
        QThread::msleep(100);
        result = pick_arm->ZSerchByForce(0,parameters.vcm1Svel(),parameters.vcm1PickForce(),z,parameters.vcm1Margin(),parameters.vcm1FinishDelay(),is_open,false,time_out);
        QThread::msleep(100);
        result &= pick_arm->ZSerchReturn(0,time_out);
        QThread::msleep(200);
        result &=pick_arm->motor_th1->MoveToPosSync(-90);
        //QThread::msleep(200);
        result &= pick_arm->motor_vcm1->MoveToPosSync(0);
    }
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker1MeasureHight(bool is_tray,bool is_product)
{
    qInfo("measureHight speed: %f force: %f", parameters.vcm1Svel(), parameters.vcm1PickForce());
    if(pick_arm->ZSerchByForce(0,parameters.vcm1Svel(),parameters.vcm1PickForce(),true))
    {
        QThread::msleep(100);
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否应用此高度:%1？").arg(pick_arm->GetSoftladngPosition()))){
            return true;
        }
        if(is_product){
            if(is_tray)
                pick_arm->parameters.setPlaceProductZ(pick_arm->GetSoftladngPosition());
            else
                pick_arm->parameters.setPickProductZ(pick_arm->GetSoftladngPosition());
        }else{
            if(is_tray)
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
    qInfo("picker2MeasureHight is_tray %d is_product %d",is_tray,is_product);
    if(pick_arm->ZSerchByForce(1,parameters.vcm2Svel(),parameters.vcm2PickForce(),true))
    {
        QThread::msleep(100);
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否应用此高度:%1").arg(pick_arm->GetSoftladngPosition(false, 1)))){
            return true;
        }
        if(is_tray)
        {
            if(is_product)
                pick_arm->parameters.setPlaceProductZ(pick_arm->GetSoftladngPosition(false, 1));
            else{

                pick_arm->parameters.setPickSensorZ(pick_arm->GetSoftladngPosition(false, 1));
                
            }
        }
        else
        {
            if(is_product)
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

bool SingleHeadMachineMaterialLoaderModule::checkNeedChangeLensTray()
{
    if(lensTray->isTrayNeedChange(0)&&lensTray->isTrayNeedChange(1))
        return true;
    return false;
}

bool SingleHeadMachineMaterialLoaderModule::checkNeedChangeSensorTray()
{
    if(sensorTray->isTrayNeedChange(0)&&sensorTray->isTrayNeedChange(1))
        return true;
    return false;
}
bool SingleHeadMachineMaterialLoaderModule::checkNeedChangeRejectTray()
{
    if(rejectTray->isTrayNeedChange(0)&&rejectTray->isTrayNeedChange(1))
        return true;
    return false;
}

bool SingleHeadMachineMaterialLoaderModule::checkNeedChangeTray()
{
    if(sensorTray->isTrayNeedChange(0))
    {
        sensorTray->resetTrayState(0);
        states.setAllowChangeSensorTray(true);
        return true;
    }

    if(  lensTray->isTrayNeedChange(0))
    {
        lensTray->resetTrayState(0);
        states.setAllowChangeLensTray(true);
        return true;
    }
    if(rejectTray ->isTrayNeedChange(0))
    {
        rejectTray ->resetTrayState(0);
        states.setAllowChangeRejectTray(true);
        return true;
    }
    return false;
}

bool SingleHeadMachineMaterialLoaderModule::moveToChangeTrayPos()
{
    qInfo("moveToChangeTrayPos %d tray_load %d");
    bool result = pick_arm->motor_x-> MoveToPosSync(0);
    if(!result)
        AppendError(QString(u8"移动到换盘位置位置失败"));
    return result;
}

void SingleHeadMachineMaterialLoaderModule::receiveLoadMaterialRequest(bool need_sensor, bool need_lens, bool has_ng_sensor,
                                                                       bool has_ng_lens, bool has_product,  bool isSutReadyToLoadMaterial)
{
    qInfo("Receive material request, need_sensor: %d need_lens: %d  has_ng_sensor: %d has_ng_lens: %d has_product: %d",
          need_sensor, need_lens, has_ng_sensor, has_ng_lens, has_product);
    this->states.setNeedLoadSensor(need_sensor);
    this->states.setSutHasNgSensor(has_ng_sensor);
    this->states.setNeedLoadLens(need_lens);
    this->states.setLutHasNgLens(has_ng_lens);
    this->states.setSutHasProduct(has_product);
    this->states.setSutIsReadyToLoadMaterial(isSutReadyToLoadMaterial);
}

void SingleHeadMachineMaterialLoaderModule::receiveChangeLensTrayRequest()
{
    QMutexLocker temp_locker(&materialLoader_mutex);
    if(states.waitingChangeLensTray())
    {
        states.setFinishChangeLensTray(true);
    }
    else {
        qInfo("LensLoaderModule receiveChangeTrayFinish but not effect");
    }
}
void SingleHeadMachineMaterialLoaderModule::receiveChangeSensorTrayRequest()
{
    QMutexLocker temp_locker(&materialLoader_mutex);
    if(states.waitingChangeSensorTray())
    {
        states.setFinishChangeSensorTray(true);
    }
    else {
        qInfo("SensorLoaderModule receiveChangeTrayFinish but not effect");
    }
}

void SingleHeadMachineMaterialLoaderModule::run(bool has_material)
{
    is_run = true;
    while (is_run) {
        if (!states.needLoadSensor() && !states.needLoadLens()){  // Waiting for load material
            //May finish the loading, make decision here
            if (this->states.hasPickedLens() && this->states.sutIsReadyToLoadMaterial())    //Place lens to LUT
            {
                qInfo("Place lens to LUT");
                QPointF point = lut_pr_position.ToPointF();
                point.setX(point.x()+camera_to_picker1_offset.X());
                point.setY(point.y()+camera_to_picker1_offset.Y());
                pick_arm->move_XmY_Synic(point);
                picker1PlaceLensToLUT();
                states.setHasPickedLens(false);
                states.setLutHasLens(true);
                if(!is_run)break;
            }

            if (this->states.hasPickedSensor() && this->states.sutIsReadyToLoadMaterial() && !states.sutHasNgSensor())   //Place sensor to SUT
            {
                qInfo("Place sensor to SUT");
                moveToSUTPRPos();
                pr_offset.ReSet();
                if(!performSUTPR())
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Perform SUT PR fail, please check");
                    int operation = waitMessageReturn(is_run);
                    qInfo("user operation: %d", operation);
                    performSUTPR();
                    continue;
                }else{
                    moveToPicker2WorkPos();           //use this in fast mode
                    picker2PlaceSensorToSUT();
                    states.setHasPickedSensor(false);
                    states.setSutHasSensor(true);
                }
                if(!is_run)break;
            }

            if (states.sutHasSensor() && states.lutHasLens())  //Both sensor and lens have already loaded into sut and lut
            {
                qInfo("Both sensor and lens have already loaded into sut and lut");
                states.setSutHasSensor(false);
                states.setLutHasLens(false);
                emit sendLoadMaterialResponse( sensorTray->getCurrentIndex() , lensTray->getCurrentIndex());
            }
            QThread::msleep(100);
            continue;
        }

        if (this->states.lutHasNgLens()) {   //Pick the ng lens from lut
            qInfo("Pick the ng lens from lut");
            this->states.setLutHasNgLens(false);
            this->states.setHasPickedNgLens(false);
            if(!is_run)break;
        }

        if (this->states.sutHasNgSensor()){  //Pick the ng sensor from sut
            if (this->states.hasPickedLens()) {   //If picker 1 has lens, need unload the lens to lut first
                qInfo("Place lens to LUT");
                QPointF point = lut_pr_position.ToPointF();
                point.setX(point.x()+camera_to_picker1_offset.X());
                point.setY(point.y()+camera_to_picker1_offset.Y());
                pick_arm->move_XmY_Synic(point);
                picker1PlaceLensToLUT();
                states.setHasPickedLens(false);
                states.setLutHasLens(true);
            }
            qInfo("Pick the ng sensor from sut");
            moveToSUTPRPos();
            pr_offset.ReSet();
            performSUTSensorPR();
            if(!performSUTSensorPR())
            {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Perform SUTSensor PR fail, please check");
                int operation = waitMessageReturn(is_run);
                qInfo("user operation: %d", operation);
                this->states.setSutHasNgSensor(false);   //user removed the ng sensor
                this->states.setHasPickedNgSensor(false);
                continue;
            }else{
                moveToPicker1WorkPos();
                picker1PickNgSensorFromSUT();
                qInfo("pick_arm->vacuum_lens_suction name: %s", pick_arm->vacuum_lens_suction->GetName().toStdString().c_str());
                if(pick_arm->vacuum_lens_suction->GetVacuumState()) {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Pick ng sensor Fail.Please check");
                    int operation = waitMessageReturn(is_run);
                    qInfo("user operation: %d", operation);
                    //rejectTray->setCurrentMaterialState(MaterialState::IsNg, states.currentRejectTray());
                    this->states.setSutHasNgSensor(false);
                    this->states.setHasPickedNgSensor(false);
                    //Testing the reset theta motor position
                    pick_arm->motor_th1->MoveToPos(0);
                } else {
                    this->states.setSutHasNgSensor(false);
                    this->states.setHasPickedNgSensor(true);
                }
            }
            //Place Sensor to SUT
            qInfo("Place sensor to SUT");
            if (states.hasPickedSensor() && !states.sutHasSensor()) {
                moveToSUTPRPos();
                pr_offset.ReSet();
                if(!performSUTPR())
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Perform SUT PR fail, please check");
                    int operation = waitMessageReturn(is_run);
                    qInfo("user operation: %d", operation);
                    performSUTPR();
                    continue;
                }else{
                    moveToPicker2WorkPos();           //use this in fast mode
                    picker2PlaceSensorToSUT();
                    states.setHasPickedSensor(false);
                    states.setSutHasSensor(true);
                    states.setNeedLoadSensor(false);
                    states.setNeedLoadLens(false);
                }
            }
            if(!is_run)break;
            continue;
        }

        if (this->states.hasPickedNgSensor()){ // Place the ng sensor to tray
            qInfo("Place the ng sensor to tray");
            if(moveToNextRejectTrayPos(states.currentRejectTray()))
            {
                pr_offset.ReSet();
                performSensorVacancyPR();
                moveToPicker1WorkPos();
                picker1PlaceNgSensorToTray();
                states.setHasPickedNgSensor(false);
                rejectTray->setCurrentMaterialState(MaterialState::IsNg, states.currentRejectTray());
            } else {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Reject Tray Full. Please clear the reject tray");
                int operation = waitMessageReturn(is_run);
                qInfo("user operation: %d", operation);
                rejectTray->resetTrayState(states.currentRejectTray());

                if(moveToNextRejectTrayPos(states.currentRejectTray()))
                {
                    pr_offset.ReSet();
                    performSensorVacancyPR();
                    moveToPicker1WorkPos();
                    picker1PlaceNgSensorToTray();
                    states.setHasPickedNgSensor(false);
                    rejectTray->setCurrentMaterialState(MaterialState::IsNg, states.currentRejectTray());
                }

                continue;
            }
            if(!is_run)break;
        }

        if (this->states.sutHasProduct()){  //Pick product form sut
            if (this->states.hasPickedLens()) {   //If picker 1 has lens, need unload the lens to lut first
                qInfo("Place lens to LUT");
                QPointF point = lut_pr_position.ToPointF();
                point.setX(point.x()+camera_to_picker1_offset.X());
                point.setY(point.y()+camera_to_picker1_offset.Y());
                pick_arm->move_XmY_Synic(point);
                picker1PlaceLensToLUT();
                states.setHasPickedLens(false);
                states.setLutHasLens(true);
            }
            qInfo("Pick product from sut ");
            moveToSUTPRPos();
            pr_offset.ReSet();
            performSUTProductPR();
            moveToPicker1WorkPos();
            picker1PickProductFormSUT();
            if(pick_arm->vacuum_lens_suction->GetVacuumState()) {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Pick Product Fail.Please check");
                int operation = waitMessageReturn(is_run);
                qInfo("user operation: %d", operation);
                //sensorTray->setCurrentMaterialState(MaterialState::IsProduct, states.currentSensorTray());
                this->states.setSutHasProduct(false);
                this->states.setHasPickedProduct(false);
                pick_arm->motor_th1->MoveToPos(0);
            } else {
                this->states.setSutHasProduct(false);
                this->states.setHasPickedProduct(true);
            }
            //Place Sensor to SUT
            if (states.hasPickedSensor() && !states.sutHasSensor()) {
                qInfo("Place sensor to SUT");
                moveToSUTPRPos();
                pr_offset.ReSet();
                if(!performSUTPR())
                {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Perform SUT PR fail, please check");
                    int operation = waitMessageReturn(is_run);
                    qInfo("user operation: %d", operation);
                    performSUTPR();
                    continue;
                }else{
                    moveToPicker2WorkPos();           //use this in fast mode
                    picker2PlaceSensorToSUT();
                    states.setHasPickedSensor(false);
                    states.setSutHasSensor(true);
                    states.setNeedLoadSensor(false);
                    states.setNeedLoadLens(false);
                }
            }
            continue;
            if(!is_run)break;
        }

        if(this->states.hasPickedProduct()){  //Place product to tray
            qInfo("Place Product to Sensor Tray");
            if(moveToSensorTrayPos(states.currentSensorTray()))
            {
                pr_offset.ReSet();
                performSensorVacancyPR();
                moveToPicker1WorkPos();
                picker1PlaceProductToTray();
                states.setHasPickedProduct(false);
                sensorTray->setCurrentMaterialState(MaterialState::IsProduct, states.currentSensorTray());
            } else {
                sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Prodcut Tray Error!");
                int operation = waitMessageReturn(is_run);
                qInfo("user operation: %d", operation);
                sensorTray->resetTrayState(states.currentSensorTray());
                continue;
            }
            if(!is_run)break;
        }

        if (this->states.hasPickedNgLens()){  // Place the ng lens to tray
            qInfo("{Place the ng lens to tray ");
            moveToLensTrayPos(states.currentLensTray());
            pr_offset.ReSet();
            performLensVacancyPR();
            moveToPicker1WorkPos();
            picker1PlaceLensToTray();
            states.setHasPickedNgLens(false);
            lensTray->setCurrentMaterialState(MaterialState::IsNg, states.currentLensTray());
            if(!is_run)break;
        }

        if (this->states.needLoadLens() && !this->states.lutHasLens()){ //Pick lens form tray
            qInfo("picke lens from tray");
            if(moveToNextLensTrayPos(states.currentLensTray())){
                pr_offset.ReSet();
                if(!performLensPR()){
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Perform Lens PR fail");
                    int operation = waitMessageReturn(is_run);
                    qInfo("user operation: %d", operation);
                    lensTray->setCurrentMaterialState(MaterialState::IsNg, states.currentLensTray());
                    continue;
                } else {
                    moveToPicker1WorkPos();
                    picker1PickLensFromTray();
                    if(pick_arm->vacuum_lens_suction->GetVacuumState()) {
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Pick lens from lens tray fail");
                        int operation = waitMessageReturn(is_run);
                        qInfo("user operation: %d", operation);
//                        states.setHasPickedLens(true);
//                        states.setNeedLoadLens(false);
                        lensTray->setCurrentMaterialState(MaterialState::IsInUse, states.currentLensTray());
                    }else {
                        states.setHasPickedLens(true);
                        states.setNeedLoadLens(false);
                        lensTray->setCurrentMaterialState(MaterialState::IsInUse, states.currentLensTray());
                    }
                }
            }else {
                moveToChangeTrayPos();
                sendAlarmMessage(ErrorLevel::ContinueOrRetry, "lens tray is empty. Please change the lens tray");
                int operation = waitMessageReturn(is_run);
                qInfo("user operation: %d", operation);
                lensTray->resetTrayState(states.currentLensTray());
                continue;
            }
            if(!is_run)break;
        }

        if (this->states.needLoadSensor()){ //Pick sensor form tray
            qInfo("Pick sensor form tray");
            if(moveToNextSensorTrayPos(states.currentSensorTray())){
                pr_offset.ReSet();
                if (!performSensorPR()) {
                    sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Perform Sensor PR fail");
                    int operation = waitMessageReturn(is_run);
                    qInfo("user operation: %d", operation);
                    sensorTray->setCurrentMaterialState(MaterialState::IsNg, states.currentSensorTray());
                    continue;
                } else {
                    moveToPicker2WorkPos();
                    picker2PickSensorFromTray();
                    if(!pick_arm->vacuum_sensor_suction->GetVacuumState()) {
                        sendAlarmMessage(ErrorLevel::ContinueOrRetry, "Pick sensor from sensor tray fail");
                        int operation = waitMessageReturn(is_run);
                        qInfo("user operation: %d", operation);
                        sensorTray->setCurrentMaterialState(MaterialState::IsNg, states.currentSensorTray());
                    } else {
                        states.setHasPickedSensor(true);
                        states.setNeedLoadSensor(false);
                        sensorTray->setCurrentMaterialState(MaterialState::IsInUse, states.currentSensorTray());
                        moveToLUTPRPos();
                    }
                }
            }else {
                moveToChangeTrayPos();
                sendAlarmMessage(ErrorLevel::ContinueOrRetry, "sensor tray is empty. Please change the sensor tray");
                int operation = waitMessageReturn(is_run);
                qInfo("user operation: %d", operation);
                sensorTray->resetTrayState(states.currentSensorTray());
                continue;
            }
            if(!is_run)break;
        }

        if(this->states.lutHasLens() && this->states.hasPickedSensor())
        {
            this->states.setNeedLoadLens(false);
        }


//        if(this->states.allowChangeLensTray()){
//            bool result= moveToChangeTrayPos();
//            if (result)
//            {
//                sendAlarmMessage(ErrorLevel::ContinueOrRetry,"please change lens tray handlely");
//                states.setNeedLoadLens(true);
//            }
//        }
//        if(this->states.allowChangeSensorTray()){
//            bool result= moveToChangeTrayPos();
//            if (result)
//            {
//                sendAlarmMessage(ErrorLevel::ContinueOrRetry,"please change sensor tray handlely");
//                states.setNeedLoadSensor(true);
//            }
//        }
//        if(this->states.allowChangeRejectTray()){
//            bool result= moveToChangeTrayPos();
//            if (result)
//            {
//                sendAlarmMessage(ErrorLevel::ContinueOrRetry,"please change reject tray handlely");
//            }
//        }
    }
}

bool SingleHeadMachineMaterialLoaderModule::loadLensFromTrayAndPlaceToLUT()
{
    bool ret = false;
    ret = moveToNextLensTrayPos(states.currentLensTray());
    ret &= performLensPR();
    ret &= moveToPicker1WorkPos();
    ret &= picker1PickLensFromTray();
    if (ret) {
        this->states.setHasPickedNgLens(true);
    }
    ret &= moveToLUTPRPos();
    ret &= moveToPicker1WorkPos();
    ret &= picker1PlaceLensToLUT();
    if (ret) {
        this->states.setHasPickedNgLens(false);
    }
    return ret;
}

bool SingleHeadMachineMaterialLoaderModule::pickNgLensFromLUTAndReturnToTray()
{
    bool ret = false;
    ret = moveToLUTPRPos();
    ret &= performLUTLensPR();
    ret &= moveToPicker1WorkPos();
    ret &= picker1PickNgLensFromLUT();
    ret &= moveToNextLensTrayPos(states.currentLensTray());
    //performLensVacancyPR())
    ret &= moveToPicker1WorkPos();
    ret &= picker1PlaceLensToTray();
    return ret;
}

void SingleHeadMachineMaterialLoaderModule::startWork(int run_mode)
{
    qInfo("MaterailLoader start run_mode :%d in %d", run_mode, QThread::currentThreadId());
    if (run_mode == RunMode::Normal)
        run(true);
}

void SingleHeadMachineMaterialLoaderModule::stopWork(bool wait_finish)
{
    qInfo("MaterailLoader stop Work");
    is_run = false;
}

void SingleHeadMachineMaterialLoaderModule::resetLogic()
{
    qInfo("resetLogic is called");
    this->states.setLutHasLens(false);
    this->states.setLutHasNgLens(false);
    this->states.setLoadingLens(false);
    sensorTray->resetTrayState(sensorTray->getCurrentIndex());
    lensTray->resetTrayState(lensTray->getCurrentIndex());
    rejectTray->resetTrayState(rejectTray->getCurrentIndex());
    //    sensorTray->resetTrayState(0);
    //    lensTray->resetTrayState(0);
    //    rejectTray->resetTrayState(0);


    this->states.setHasSensorTray(false);
    this->states.setSutHasSensor(false);
    this->states.setSutHasNgSensor(false);
    this->states.setSutHasProduct(false);
    this->states.setNeedLoadSensor(false);
    this->states.setNeedChangeSensorTray(false);
    this->states.setAllowChangeSensorTray(false);
    this->states.setHasPickedSensor(false);
    this->states.setHasPickedProduct(false);
    this->states.setHasPickedNgSensor(false);
    this->states.setBeExchangeMaterial(false);
    this->states.setWaitingChangeSensorTray(false);
    this->states.setFinishChangeSensorTray(false);
    this->states.setHasLensTray(true);

    this->states.setLutHasLens(false);
    this->states.setLutHasNgLens(false);
    this->states.setNeedLoadLens(false);
    this->states.setCurrentSensorTray(0);
    this->states.setCurrentLensTray(0);
    this->states.setCurrentRejectTray(0);

    this->states.setNeedChangeLensTray(false);
    this->states.setAllowChangeSensorTray(false);
    this->states.setHasPickedLens(false);
    this->states.setHasPickedNgLens(false);
    this->states.setLoadingLens(false);
    this->states.setWaitingChangeLensTray(false);

    //    int m_lutTrayID = -1;
    //    int m_lutLensID = -1;
    //    int m_pickedLensTrayID = -1;
    //    int m_pickedLensID = -1;
    //    int m_lutNgTrayID = -1;
    //    int m_lutNgLensID = -1;
    //    bool m_loadingLens = false;
    //    bool m_waitingChangeLensTray = false;
    //    bool m_finishChangeLensTray = false;
}

void SingleHeadMachineMaterialLoaderModule::performHandlingOperation(int cmd)
{
    qInfo("performHandling %d",cmd);
    bool result;
    int handlePosition = cmd&HANDLE_POSITION;
    switch (handlePosition){
    case LENS_TRAY1:
    {
        qInfo("pick arm move to lens Tray1 current position, cmd: %d",LENS_TRAY1);
        result = moveToLensTrayPos(0);
    }
        break;
    case LENS_TRAY2:
    {
        qInfo("pick arm move to lens Tray2 current position, cmd: %d",LENS_TRAY2);
        result = moveToLensTrayPos(1);
    }
        break;
    case LUT_PR_POS:
    {
        qInfo("pick arm move to LUT pr position,cmd: %d",LUT_PR_POS);
        result = moveToLUTPRPos();
    }
        break;
    case LENS_TRAY1_START_POS:
    {
        qInfo("pick arm move to lens tray1 start position,cmd: %d",LENS_TRAY1_START_POS);
        result = moveToLensTrayStartPos(0);
    }
        break;
    case LENS_TRAY2_START_POS:
    {
        qInfo("pick arm move to lens tray2 start position,cmd: %d",LENS_TRAY2_START_POS);
        result = moveToLensTrayStartPos(1);
    }
        break;
    case LENS_TRAY1_END_POS:
    {
        qInfo("pick arm move to lens tray1 end position,cmd: %d",LENS_TRAY1_END_POS);
        result = moveToLensTrayEndPos();
    }
        break;
    case SENSOR_TRAY:
    {
        qInfo("pick arm move to sensor tray current position,cmd: %d",SENSOR_TRAY);
        result = moveToSensorTrayPos(0);
    }
        break;
    case SUT_PR_POS:
    {
        qInfo("pick arm move to sut pr position,cmd: %d",SUT_PR_POS);
        result = moveToSUTPRPos();
    }
        break;
    case SENSOR_TRAY_START_POS:
    {
        qInfo("pick arm move to sensor tray start position,cmd: %d",SENSOR_TRAY_START_POS);
        result = moveToSensorTrayStartPos(0);
    }
        break;
    case SENSOR_TRAY_END_POS:
    {
        qInfo("pick armmove to snesor tray end position,cmd: %d",SENSOR_TRAY_END_POS);
        result = moveToSensorTrayEndPos();
    }
        break;
    case REJECT_TRAY:
    {
        qInfo("pick arm move to reject tray current position,cmd: %d",REJECT_TRAY);
        result = moveToRejectTrayPos(0);
    }
        break;
    case REJECT_TRAY_START_POS:
    {
        qInfo("pick arm move to reject tray start position,cmd: %d",REJECT_TRAY_START_POS);
        result = moveToRejectTrayStartPos(0);
    }
        break;
    case REJECT_TRAY_END_POS:
    {
        qInfo("pick arm move to reject tray end position,cmd: %d",REJECT_TRAY_END_POS);
        result = moveToRejectTrayEndPos();
    }
        break;
    default:
        result = true;
        break;
    }
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        return;
    }
    int handlePR = cmd&HANDLE_PR;
    switch (handlePR) {
    case RESET_PR:
    {
        qInfo("reset PR result,cmd: %d",RESET_PR);
        pr_offset.ReSet();
    }
        break;
    case SENSOR_PR:
    {
        qInfo("perform sensor PR,cmd: %d",SENSOR_PR);
        result = performSensorPR();
    }
        break;
    case SENSOR_VACANCY_PR:
    {
        qInfo("perform sensor tray vacancy PR, cmd: %d",SENSOR_VACANCY_PR);
        result = performSensorVacancyPR();
    }
        break;
    case SUT_PR:
    {
        qInfo("perform SUT vacancy PR,cmd: %d",SUT_PR);
        result = performSUTPR();
    }
        break;
    case NG_SENSOR_PR:
    {
        qInfo("perform SUT Ng Sensor PR,cmd: %d",NG_SENSOR_PR);
        result = performSUTSensorPR();
    }
        break;
    case PRODUCT_PR:
    {
        qInfo("perform SUT product PR,cmd: %d",PRODUCT_PR);
        result = performSUTProductPR();
    }
        break;
    case LENS_PR:
    {
        qInfo("perform lens PR,cmd: %d",LENS_PR);
        result = performLensPR();
    }
        break;
    case NG_LENS_PR:
    {
        qInfo("perform ng lens PR, cmd: %d",NG_LENS_PR);
        result = performLUTLensPR();
    }
        break;
    case LENS_VACANCY_PR:
    {
        qInfo("perform lens tray vacancy PR,cmd: %d",LENS_VACANCY_PR);
        result = performLensVacancyPR();
    }
        break;
    case LUT_PR:
    {
        qInfo("perform LUT vacancy PR,cmd: %d",LUT_PR);
        result = performLUTPR();
    }
        break;

    default:
        result = true;
        break;
    }
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock,GetCurrentError());
        return;
    }
    int handleToWorkPos = cmd&HANDLE_TO_WORKPOS;
    switch (handleToWorkPos) {
    case PICKER1_TO_WORKPOS:
    {
        qInfo("lens suction move offset,cmd: %d",PICKER1_TO_WORKPOS);
        result = moveToPicker1WorkPos();
    }
        break;
    case PICKER2_TO_WORKPOS:
    {
        qInfo("sensor suction move offset,cmd: %d",PICKER2_TO_WORKPOS);
        result = moveToPicker2WorkPos();
    }
        break;

    default:
        result = true;
        break;
    }
    if(!result)
    {
        qInfo("Move To Work Pos fail");
        return;
    }
    int handlePickerAction = cmd&HANDLE_PICKER_ACTION;
    switch (handlePickerAction) {
    case PICKER2_PICK_SENSOR_FROM_TRAY:
    {
        qInfo("pick sensor from tray,cmd: %d",PICKER2_PICK_SENSOR_FROM_TRAY);
        result = picker2PickSensorFromTray();
    }
        break;
    case PICKER2_PLACE_SENSOR_TO_SUT:
    {
        qInfo("place sensor to SUT,cmd: %d",PICKER2_PLACE_SENSOR_TO_SUT);
        result = picker2PlaceSensorToSUT();
    }
        break;
    case PICKER1_PLACE_PRODUCT_TO_TRAY:
    {
        qInfo("place product to tray,cmd: %d",PICKER1_PLACE_PRODUCT_TO_TRAY);
        result = picker1PlaceProductToTray();
    }
        break;
    case PICKER1_PLACE_NG_PRODUCT_TO_TRAY:
    {
        qInfo("place ng product to tray,cmd: %d",PICKER1_PLACE_NG_PRODUCT_TO_TRAY);
        result = picker1PlaceProductToTray();// use place product ation for NG
    }
        break;

    case PICKER2_PICK_NG_SENSOR_FROM_SUT:
    {
        qInfo("pick ng Sensor from SUT,cmd: %d",PICKER2_PICK_NG_SENSOR_FROM_SUT);
        result = picker2PickNgSensorFromSUT();
    }
        break;
    case PICKER1_PICK_NG_SENSOR_FROM_SUT:
    {
        qInfo("pick ng Sensor from SUT,cmd: %d",PICKER1_PICK_NG_SENSOR_FROM_SUT);
        result = picker1PickNgSensorFromSUT();
    }
        break;
    case PICKER1_PLACE_NG_SENSOR_TO_TRAY:
    {
        qInfo("place ng sensor to tray,cmd: %d",PICKER1_PLACE_NG_SENSOR_TO_TRAY);
        result = picker1PlaceNgSensorToTray();
    };
        break;
    case PICKER2_PLACE_NG_SENSOR_TO_TRAY:
    {
        qInfo("move to place sensor to tray,cmd: %d",PICKER2_PLACE_NG_SENSOR_TO_TRAY);
        result = picker2PlaceNgSensorToTray();
    }
        break;
    case PICKER2_MEASURE_SENSOR_IN_TRAY:
    {
        qInfo("mesure pick senso from tray height, cmd: %d",PICKER2_MEASURE_SENSOR_IN_TRAY);

        result = picker2MeasureHight(true,false);
    }
        break;
    case PICKER1_MEASURE_PRODUCT_IN_TRAY:
    {
        qInfo("measure place product to tray height, cmd: %d ",PICKER1_MEASURE_PRODUCT_IN_TRAY);
        result = picker1MeasureHight(true,true);
    }
        break;
    case PICKER2_MEASURE_SENSOR_IN_SUT:
    {
        qInfo("measure place sensor to SUT,cmd: %d",PICKER2_MEASURE_SENSOR_IN_SUT);

        result = picker2MeasureHight(false,false);
    }
        break;
    case PICKER1_MEASURE_PRODUCT_IN_SUT:
    {
        qInfo("measure pick product form SUT, cmd: %d",PICKER1_MEASURE_PRODUCT_IN_SUT);
        result = picker1MeasureHight(false,true);
    }
        break;
    case PICKER1_PICK_LENS_FROM_TRAY:
    {
        qInfo("pick lens form tray, cmd: %d",PICKER1_PICK_LENS_FROM_TRAY);
        result = picker1PickLensFromTray();
    }
        break;
    case PICKER1_PLACE_LENS_TO_LUT:
    {
        qInfo("place lens to SUT,cmd: %d",PICKER1_PLACE_LENS_TO_LUT);
        result = picker1PlaceLensToLUT();
    }
        break;
    case PICKER1_PICK_PRODUCT_FROM_SUT:
    {
        qInfo("pick product form SUT,cmd: %d",PICKER1_PICK_PRODUCT_FROM_SUT);
        result = picker1PickProductFormSUT();
    }
        break;
    case  PICKER1_PLACE_LENS_TO_TRAY:
    {
        qInfo("place lens to tray,cmd: %d",PICKER1_PLACE_LENS_TO_TRAY);
        result = picker1PlaceLensToTray();
    }
        break;
    case PICKER2_PLACE_SENSOR_TO_TRAY:
    {
        qInfo("place sensor to tray,cmd: %d",PICKER2_PLACE_SENSOR_TO_TRAY);
        result = picker2PlaceSensorToTray();
    }
        break;
    case PICKER1_PICK_NG_LENS_FROM_LUT:
    {
        qInfo("pick ng lens from LUT,cmd: %d",PICKER1_PICK_NG_LENS_FROM_LUT);
        result = picker1PickNgLensFromLUT();
    }
        break;
    case PICKER1_PLACE_NG_LENS_TO_TRAY:
    {
        qInfo("place ng lens to tray,cmd: %d",PICKER1_PLACE_NG_LENS_TO_TRAY);
        result = picker1PlaceNgLensToTray();
    }
        break;
    case PICKER2_PLACE_NG_LENS_TO_TRAY:
    {
        qInfo("place ng lens to tray,cmd: %d",PICKER2_PLACE_NG_LENS_TO_TRAY);
        result = picker2PlaceNgLensToTray();
    }
        break;
    case PICKER2_PICK_NG_LENS_FROM_LUT:
    {
        qInfo("pick ng lens form lut,cmd: %d",PICKER2_PICK_NG_LENS_FROM_LUT);
        result = picker2PickNgLensFromLUT();
    }
        break;
    case PICKER1_MEASURE_LENS_IN_TRAY:
    {
        qInfo("measure pick lens from tray height,cmd: %d",PICKER1_MEASURE_LENS_IN_TRAY);
        result = picker1MeasureHight(true);
    }
        break;
    case PICKER1_MEASURE_LENS_IN_LUT:
    {
        qInfo("measuer place lens to LUT height,cmd: %d",PICKER1_MEASURE_LENS_IN_LUT);
        result = picker1MeasureHight(false);
    }
        break;
    default:
        result = true;
        break;
    }
    return;
}

