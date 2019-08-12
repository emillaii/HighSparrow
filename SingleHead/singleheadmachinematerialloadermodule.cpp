#include "SingleHead/singleheadmachinematerialloadermodule.h"
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

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayPos(int index, int tray_index)
{
    qInfo("moveToTrayPos index %d tray_index %d",index,tray_index);
    bool result = pick_arm->move_XY_Synic(lensTray->getPositionByIndex(index,tray_index));
    if(!result)
        AppendError(QString(u8"移动到%1盘%1号位置失败").arg(index));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayPos(int tray_index)
{
    qInfo("moveToTrayPos tray_index %d",tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(lensTray->getCurrentPosition(tray_index),false);
    if(!result)
        AppendError(QString(u8"移动到%1盘当前位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayStartPos(int tray_index)
{
    qInfo("moveToStartPos%d",tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(lensTray->getStartPosition(tray_index),false);
    if(!result)
        AppendError(QString(u8"移动到%1盘起始位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayEndPos()
{
    qInfo("moveToTrayEndPos");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(lensTray->getEndPosition(),false);
    if(!result)
        AppendError(QString(u8"移动到sensor盘结束位置失败"));
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
        AppendError(QString(u8"移动到%1盘下一个位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSensorTrayPos(int index, int tray_index)
{
    qInfo("moveToTrayPos index %d tray_index %d",index,tray_index);
    bool result = pick_arm->move_XY_Synic(sensorTray->getPositionByIndex(index,tray_index));
    if(!result)
        AppendError(QString(u8"移动到%1盘%1号位置失败").arg(tray_index == 0?"sensor":"成品").arg(index));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSensorTrayPos(int tray_index)
{
    qInfo("moveToTrayPos tray_index %d",tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(sensorTray->getCurrentPosition(tray_index),false);
    if(!result)
        AppendError(QString(u8"移动到%1盘当前位置失败").arg(tray_index == 0?"sensor":"成品"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSensorTrayStartPos(int tray_index)
{
    qInfo("moveToStartPos%d",tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(sensorTray->getStartPosition(tray_index),false);
    if(!result)
        AppendError(QString(u8"移动到%1盘起始位置失败").arg(tray_index == 0?"sensor":"成品"));
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
        AppendError(QString(u8"移动到%1盘下一个位置失败").arg(tray_index == 0?"sensor":"成品"));
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
    double theta = pr_offset.Theta;
    if (pick_arm->parameters.placeLensTheta() < 0) theta = -pr_offset.Theta;
    pr_offset.Theta = 0; //Reset the last pr result
    double target_t = pick_arm->motor_th1->GetFeedbackPos() + theta;
    return  pick_arm->move_XmYT1_Synic(lut_pr_position.X(),lut_pr_position.Y(),target_t,check_softlanding);
}

bool SingleHeadMachineMaterialLoaderModule::moveToSUTPRPos(bool is_local, bool check_softlanding)
{
    qInfo("moveToSUTPRPos is_local %d",is_local);
    bool result;
    result =  pick_arm->move_XmY_Synic(sut_pr_position.ToPointF(),check_softlanding);
    if(!result)
        AppendError(QString("移动SPA到SUT位置失败%1").arg(is_local));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToPicker1WorkPos(bool check_softlanding)
{
    PrOffset temp(camera_to_picker1_offset.X() - pr_offset.X,camera_to_picker1_offset.Y() - pr_offset.Y,pr_offset.Theta);
    bool result = pick_arm->stepMove_XmYT1_Synic(temp.X,temp.Y,temp.Theta, check_softlanding);
    return  result;
}
bool SingleHeadMachineMaterialLoaderModule::moveToPicker2WorkPos(bool check_softlanding)
{
    PrOffset temp(camera_to_picker2_offset.X() - pr_offset.X,camera_to_picker2_offset.Y() - pr_offset.Y,pr_offset.Theta);
    qInfo("moveToPicker2WorkPos offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XmYT2_Synic(temp.X,temp.Y,temp.Theta,check_softlanding);
    if(result)
        AppendError(QString(u8"去2号吸头工作位置(step x %1,y %2,t %3)失败!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    return  result;
}



bool SingleHeadMachineMaterialLoaderModule::performSensorPR()
{
    qInfo("performSensorPR");
    bool result = sensor_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行料盘sensor视觉失败!"));
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::performSensorVacancyPR()
{
    qInfo("performVacancyPR");
    bool result = sensor_vacancy_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行料盘空位视觉失败!"));
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::performSUTPR()
{
    qInfo("performSUTPR");
    bool result = sut_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行SUT视觉失败!"));
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::performSUTSensorPR()
{
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
    Sleep(500); //Position settling
    return  lens_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::performLensVacancyPR()
{
    return  lens_vacancy_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::performLUTPR()
{
    return lut_vision->performPR(pr_offset);
}


bool SingleHeadMachineMaterialLoaderModule::performLUTLensPR()
{
    return lut_lens_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::picker2PickSensorFormTray(int time_out)
{
    qInfo("pickTraySensor time_out %d",time_out);
    pick_arm->motor_th2->MoveToPosSync(0);
    bool result = picker2SearchZ(pick_arm->parameters.pickSensorZ(),true,time_out, 1);
    if(result) {
        pick_arm->motor_th2->MoveToPosSync(90);
    }
    if(!result)
        AppendError(QString(u8"从sensor盘取sensor失败"));
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker1PickSensorFormTray(int time_out)
{
    qInfo("pickTraySensor time_out %d",time_out);
    pick_arm->motor_th1->MoveToPosSync(0);
    bool result = picker1SearchZ(pick_arm->parameters.pickSensorZ(),true,time_out, 0);
    if(result) {
        pick_arm->motor_th1->MoveToPosSync(90);
    }
    if(!result)
        AppendError(QString(u8"从sensor盘取sensor失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceSensorToSUT(QString dest, int time_out)
{
    qInfo("placeSensorToSUT dest %s time_out %d",dest.toStdString().c_str(),time_out);
   bool result = picker1SearchSutZ(pick_arm->parameters.placeSensorZ(),dest,"vacuumOnReq",false,time_out);
    if(!result)
        AppendError(QString(u8"放sensor到SUT%1失败").arg(dest=="remote"?1:2));
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker2PlaceSensorToSUT(QString dest, int time_out)
{
    qInfo("placeSensorToSUT dest %s time_out %d",dest.toStdString().c_str(),time_out);
   bool result = picker2SearchSutZ(pick_arm->parameters.placeSensorZ(),dest,"vacuumOnReq",false,time_out);
    if(!result)
        AppendError(QString(u8"放sensor到SUT%1失败").arg(dest=="remote"?1:2));
    return result;
}


bool SingleHeadMachineMaterialLoaderModule::picker2PickNGSensorFormSUT(int time_out)
{
    qInfo("picker2PickNGSensorFormSUT time_out %d",time_out);
    bool result = picker2SearchSutZ2(pick_arm->parameters.placeSensorZ(),"dummy","vacuumOffReq",true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT取NGsenor失败"));
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker1PickNGSensorFormSUT(QString dest, int time_out)
{
    qInfo("pickSUTSensor dest %s time_out %d",dest.toStdString().c_str(),time_out);
    bool result = picker1SearchSutZ2(pick_arm->parameters.placeSensorZ(),dest,"vacuumOffReq",true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT%1取NGsenor失败").arg(dest=="remote"?1:2));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PickProductFormSUT(QString dest, int time_out)
{
    qInfo("pickSUTProduct dest %s time_out %d",dest.toStdString().c_str(),time_out);
    bool result = picker1SearchSutZ2(pick_arm->parameters.pickProductZ(),dest,"vacuumOffReq",true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT%1取成品失败").arg(dest=="remote"?1:2));
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker2PickProductFormSUT(QString dest, int time_out)
{
    qInfo("pickSUTProduct dest %s time_out %d",dest.toStdString().c_str(),time_out);
    bool result = picker2SearchSutZ2(pick_arm->parameters.pickProductZ(),dest,"vacuumOffReq",true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT%1取成品失败").arg(dest=="remote"?1:2));
    return result;
}


bool SingleHeadMachineMaterialLoaderModule::picker1PlaceNGSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d",time_out);
    bool result = picker1SearchZ(pick_arm->parameters.placeSensorZ(),false,time_out, 0);
    if(!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PlaceSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d",time_out);
    bool result = picker2SearchZ(pick_arm->parameters.placeSensorZ(),false,time_out, 1);
    if(!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PlaceNGSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d",time_out);
    bool result = picker2SearchZ(pick_arm->parameters.placeSensorZ(),false,time_out, 1);
    if(!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceProductToTray(int time_out)
{
    qInfo("placeProductToTray time_out %d",time_out);
    bool result = picker1SearchZ(pick_arm->parameters.placeProductZ(),false,time_out,0);
    if(!result)
        AppendError(QString(u8"将成品放入成品盘失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PlaceProductToTray(int time_out)
{
    qInfo("placeProductToTray time_out %d",time_out);
    bool result = picker2SearchZ(pick_arm->parameters.placeProductZ(),false,time_out,0);
    if(!result)
        AppendError(QString(u8"将成品放入成品盘失败"));
    return result;
}




bool SingleHeadMachineMaterialLoaderModule::picker1PickLensFormTray(int time_out)
{
    qInfo("pickTrayLens");
    bool result = picker1SearchZ(pick_arm->parameters.pickLensZ(),true,time_out,0);
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker2PickLensFormTray(int time_out)
{
    qInfo("pickTrayLens");
    bool result = picker2SearchZ(pick_arm->parameters.pickLensZ(),true, time_out,1);
    return result;
}


bool SingleHeadMachineMaterialLoaderModule::picker1PlaceLensToLUT(int time_out)
{
    qInfo("placeLensToLUT");
    bool result = picker1SearchZ(pick_arm->parameters.placeLensZ(), false, time_out, 0);
    this->lut_vacuum->Set(true);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PickNGLensFormLUT(int time_out)
{
    qInfo("pickLUTLens");
    return picker2SearchZ(pick_arm->parameters.placeLensZ(),true,time_out,1);
}
bool SingleHeadMachineMaterialLoaderModule::picker1PickNGLensFormLUT(int time_out)
{
    QElapsedTimer timer; timer.start();
    qInfo("pickLUTLens");
    this->lut_vacuum->Set(false);
    qInfo("cp1: %d", timer.elapsed());
    bool ret = picker1SearchZ(pick_arm->parameters.placeLensZ(),true, time_out,0);
    qInfo("cp2: %d", timer.elapsed());
    return ret;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceNGLensToTray(int time_out)
{
    qInfo("placeLensToTray");
    return picker1SearchZ(pick_arm->parameters.pickLensZ(),false,time_out,0);
}
bool SingleHeadMachineMaterialLoaderModule::picker1PlaceLensToTray(int time_out)
{
    qInfo("placeLensToTray");
    return picker1SearchZ(pick_arm->parameters.pickLensZ(),false,time_out, 0);
}
bool SingleHeadMachineMaterialLoaderModule::picker2PlaceNGLensToTray(int time_out)
{
    qInfo("placeLensToTray");
    return picker2SearchZ(pick_arm->parameters.pickLensZ(),false,time_out,1);
}
bool SingleHeadMachineMaterialLoaderModule::picker2PlaceLensToTray(int time_out )
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

bool SingleHeadMachineMaterialLoaderModule::picker2SearchSutZ(double z, QString dest, QString cmd, bool is_open, int time_out)
{
    qInfo("picker2SearchSutZ z %f dest %s cmd %s is_open %d time_out %d",z,dest.toStdString().c_str(),cmd.toStdString().c_str(),is_open,time_out);
    bool result = pick_arm->move_XmY_Z2_XmY(z - parameters.escapeHeight(),parameters.escapeX(),parameters.escapeY());
    if(result)
    {
        result = pick_arm->ZSerchByForce(1,parameters.vcm2Svel(),parameters.vcm2PickForce(),z,parameters.vcm2Margin(),parameters.vcm2FinishDelay(),is_open,false,time_out);

        //sut_vacuum
        sut_vacuum->Set(1);
        QThread::msleep(200);
        result &= pick_arm->ZSerchReturn(1,time_out);
    }
    result &= pick_arm->motor_vcm2->MoveToPosSync(0);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker2SearchSutZ2(double z, QString dest, QString cmd, bool is_open, int time_out)
{
    qInfo("picker2SearchSutZ2 z %f dest %s cmd %s is_open %d time_out %d",z,dest.toStdString().c_str(),cmd.toStdString().c_str(),is_open,time_out);
    bool result = pick_arm->motor_vcm2->MoveToPosSync(z-parameters.escapeHeight());
    if(result)
    {
        result = pick_arm->ZSerchByForce(1,parameters.vcm2Svel(),parameters.vcm2PickForce(),z,parameters.vcm2Margin(),parameters.vcm2FinishDelay(),is_open,false,time_out);
        //sut_vacuum
        sut_vacuum->Set(0);
        QThread::msleep(200);
        result &= pick_arm->ZSerchReturn(1,time_out);
    }
    result &= pick_arm->move_XmY_Z2_XmY(0,parameters.escapeX(),parameters.escapeY());
    return result;
}
bool SingleHeadMachineMaterialLoaderModule::picker1SearchSutZ2(double z, QString dest, QString cmd, bool is_open, int time_out)
{
    qInfo("picker2SearchSutZ2 z %f dest %s cmd %s is_open %d time_out %d",z,dest.toStdString().c_str(),cmd.toStdString().c_str(),is_open,time_out);
    bool result = pick_arm->motor_vcm2->MoveToPosSync(z-parameters.escapeHeight());
    if(result)
    {
        result = pick_arm->ZSerchByForce(0,parameters.vcm1Svel(),parameters.vcm1PickForce(),z,parameters.vcm1Margin(),parameters.vcm1FinishDelay(),is_open,false,time_out);
        //sut_vacuum
        sut_vacuum->Set(0);
        QThread::msleep(200);
        result &= pick_arm->ZSerchReturn(0,time_out);
    }
    result &= pick_arm->move_XmY_Z1_XmY(0,parameters.escapeX(),parameters.escapeY());
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::picker1SearchSutZ(double z, QString dest, QString cmd, bool is_open, int time_out)
{
    qInfo("picker1SearchSutZ z %f dest %s cmd %s is_open %d time_out %d",z,dest.toStdString().c_str(),cmd.toStdString().c_str(),is_open,time_out);
    bool result = pick_arm->move_XmY_Z1_XmY(z - parameters.escapeHeight(),parameters.escapeX(),parameters.escapeY());
    if(result)
    {
        result = pick_arm->ZSerchByForce(0,parameters.vcm1Svel(),parameters.vcm1PickForce(),z,parameters.vcm1Margin(),parameters.vcm1FinishDelay(),is_open,false,time_out);

        //sut_vacuum
        sut_vacuum->Set(0);
        QThread::msleep(200);
        result &= pick_arm->ZSerchReturn(0,time_out);
    }
    result &= pick_arm->motor_vcm1->MoveToPosSync(0);
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



void SingleHeadMachineMaterialLoaderModule::receiveLoadMaterialRequest(bool need_sensor, bool need_lens, bool has_ng_sensor,
                                                                       bool has_ng_lens, bool has_product)
{
    qInfo("Receive material request, need_sensor: %d need_lens: %d  has_ng_sensor: %d has_ng_lens: %d has_product: %d",
          need_sensor, need_lens, has_ng_sensor, has_ng_lens, has_product);
    this->states.setNeedLoadSensor(need_sensor);
    this->states.setSutHasNgSensor(has_ng_sensor);
    this->states.setNeedLoadLens(need_lens);
    this->states.setLutHasNgLens(has_ng_lens);
    this->states.setSutHasProduct(has_product);
}

void SingleHeadMachineMaterialLoaderModule::run(bool has_material)
{
    is_run = true;
    while (is_run) {
        if (!states.needLoadSensor() && !states.needLoadLens()){  // Waiting for load material
            //May finish the loading, make decision here
            if (this->states.hasPickedLens())    //Place lens to LUT
            {
                moveToLUTPRPos();
                //performLUTLensPR();
                pr_offset.ReSet();               //use this in fast mode
                moveToPicker1WorkPos();
                picker1PlaceLensToLUT();
                states.setHasPickedLens(false);
                states.setLutHasLens(true);
            }

            if (this->states.hasPickedSensor())   //Place sensor to SUT
            {
                moveToSUTPRPos();
                pr_offset.ReSet();
                moveToPicker2WorkPos();           //use this in fast mode
                picker2PlaceSensorToSUT("");
                states.setHasPickedSensor(false);
                states.setSutHasSensor(true);
            }

            if (states.sutHasSensor() && states.lutHasLens())  //Both sensor and lens have already loaded into sut and lut
            {
                states.setSutHasSensor(false);
                states.setLutHasLens(false);
                emit sendLoadMaterialResponse( sensorTray->getCurrentIndex() , lensTray->getCurrentIndex());
            }
            QThread::msleep(100);
            continue;
        }

        if (this->states.lutHasNgLens()) {   //Pick the ng lens from lut
           moveToLUTPRPos();
           moveToPicker1WorkPos();
           picker1PickNGLensFormLUT();
           this->states.setHasPickedNgLens(true);
        }

        if (this->states.sutHasNgSensor()){  //Pick the ng sensor from sut
            moveToSUTPRPos();
            moveToPicker2WorkPos();
            picker2PickNGSensorFormSUT();
            this->states.setHasPickedNgSensor(true);
        }

        if (this->states.hasPickedNgLens())  // Place the ng lens to tray
        {
            moveToLensTrayPos(states.currentLensTray());
            moveToPicker1WorkPos();
            picker1PlaceLensToTray();
            states.setHasPickedNgLens(false);
            lensTray->setCurrentMaterialState(MaterialState::IsNg, states.currentLensTray());
        }

        if (this->states.needLoadLens())
        {
            moveToNextLensTrayPos(states.currentLensTray());
            performLensPR();
            moveToPicker1WorkPos();
            picker1PickLensFormTray();
            states.setHasPickedLens(true);
            states.setNeedLoadLens(false);
            lensTray->setCurrentMaterialState(MaterialState::IsInUse, states.currentLensTray());
        }

        if (this->states.hasPickedNgSensor()) // Place the ng sensor to tray
        {
            moveToSensorTrayPos(states.currentSensorTray());
            moveToPicker2WorkPos();
            picker2PlaceSensorToTray();
            states.setHasPickedNgSensor(false);
            sensorTray->setCurrentMaterialState(MaterialState::IsNg, states.currentSensorTray());
        }

        if (this->states.needLoadSensor())
        {
            moveToNextSensorTrayPos(0);
            performSensorPR();
            moveToPicker2WorkPos();
            picker2PickSensorFormTray();
            states.setHasPickedSensor(true);
            states.setNeedLoadSensor(false);
            sensorTray->setCurrentMaterialState(MaterialState::IsInUse, states.currentSensorTray());
        }
    }
}

bool SingleHeadMachineMaterialLoaderModule::loadLensFromTrayAndPlaceToLUT()
{
    bool ret = false;
    ret = moveToNextLensTrayPos(states.currentLensTray());
    ret &= performLensPR();
    ret &= moveToPicker1WorkPos();
    ret &= picker1PickLensFormTray();
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
    ret &= moveToPicker1WorkPos();
    ret &= picker1PickNGLensFormLUT();
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
    lensTray->resetTrayState();
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
        result = picker2PickSensorFormTray();
    }
        break;
    case PICKER1_PICK_SENSOR_FROM_TRAY:
    {
        qInfo("pick sensor from tray,cmd: %d",PICKER1_PICK_SENSOR_FROM_TRAY);
        result = picker1PickSensorFormTray();
    }
        break;
    case PICKER1_PLACE_SENSOR_TO_SUT:
    {
        qInfo("place sensor to SUT,cmd: %d",PICKER1_PLACE_SENSOR_TO_SUT);
         result = picker1PlaceSensorToSUT("remote");
    }
        break;
    case PICKER2_PLACE_SENSOR_TO_SUT:
    {
        qInfo("place sensor to SUT,cmd: %d",PICKER2_PLACE_SENSOR_TO_SUT);
        result = picker2PlaceSensorToSUT("remote");
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
        result = picker2PickNGSensorFormSUT();
    }
        break;
    case PICKER1_PICK_NG_SENSOR_FROM_SUT:
    {
        qInfo("pick ng Sensor from SUT,cmd: %d",PICKER1_PICK_NG_SENSOR_FROM_SUT);
        result = picker1PickNGSensorFormSUT("remote");
    }
        break;
    case PICKER1_PLACE_NG_SENSOR_TO_TRAY:
    {
        qInfo("place ng sensor to tray,cmd: %d",PICKER1_PLACE_NG_SENSOR_TO_TRAY);
        result = picker1PlaceNGSensorToTray();
    };
        break;
    case PICKER2_PLACE_NG_SENSOR_TO_TRAY:
    {
        qInfo("move to place sensor to tray,cmd: %d",PICKER2_PLACE_NG_SENSOR_TO_TRAY);
        result = picker2PlaceNGSensorToTray();
    }
        break;
    case PICKER2_PICK_PRODUCT_FROM_SUT:
    {
        qInfo("pick product form sut,cmd: %d",PICKER2_PICK_PRODUCT_FROM_SUT);
        result = picker2PickProductFormSUT("remote");
    }
        break;
    case PICKER2_PLACE_PRODUCT_TO_TRAY:
    {
        qInfo("place product to tray,cmd: %d",PICKER2_PLACE_PRODUCT_TO_TRAY);
        result = picker2PlaceProductToTray();
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
        result = picker1PickLensFormTray();
    }
        break;
    case PICKER1_PLACE_LENS_TO_LUT:
    {
        qInfo("place lens to SUT,cmd: %d",PICKER1_PLACE_LENS_TO_LUT);
        result = picker1PlaceLensToLUT();
    }
        break;
       case PICKER1_PICK_PRODUCT_FORM_SUT:
    {
        qInfo("pick product form SUT,cmd: %d",PICKER1_PICK_PRODUCT_FORM_SUT);
        result = picker1PickProductFormSUT("remote");
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
        result = picker1PickNGLensFormLUT();
    }
        break;
    case PICKER1_PLACE_NG_LENS_TO_TRAY:
    {
        qInfo("place ng lens to tray,cmd: %d",PICKER1_PLACE_NG_LENS_TO_TRAY);
        result = picker1PlaceLensToTray();
    }
        break;
    case PICKER2_PICKER_NG_LENS_FORM_LUT:
    {
        qInfo("pick ng lens form lut,cmd: %d",PICKER2_PICKER_NG_LENS_FORM_LUT);
        result = picker2PickNGLensFormLUT();
    }
        break;
    case PICKER2_PLACE_NG_LENS_TO_TRAY:
    {
        qInfo("place ng lens to tray,cmd: %d",PICKER2_PLACE_NG_LENS_TO_TRAY);
        result = picker2PlaceLensToTray();
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

