#include "singleheadmachinematerialloadermodule.h"

SingleHeadMachineMaterialLoaderModule::SingleHeadMachineMaterialLoaderModule(QString name)
:ThreadWorkerBase (name)
{

}

void SingleHeadMachineMaterialLoaderModule::Init(SingleHeadMachineMaterialPickArm *_pick_arm,
                                                 MaterialTray* _sensorTray,
                                                 MaterialTray* _lensTray,
                                                 VisionLocation* _sensor_vision,
                                                 VisionLocation* _sensor_vacancy_vision,
                                                 VisionLocation* _sut_vision,
                                                 VisionLocation* _sut_sensor_vision,
                                                 VisionLocation* _sut_product_vision,
                                                 VisionLocation* _lens_vision,
                                                 VisionLocation* _lens_vacancy_vision,
                                                 VisionLocation* _lut_vision,
                                                 VisionLocation* _lut_lens_vision,
                                                 XtVacuum* _sutVacuum,
                                                 XtVacuum* _lutVacuum)
{
    this->pick_arm = _pick_arm;
    this->pick_arm->parent = this;
    this->sensorTray = _sensorTray;
    this->lensTray = _lensTray;
    this->sensor_vision = _sensor_vision;
    this->sensor_vacancy_vision = _sensor_vacancy_vision;
    this->sut_vision = _sut_vision;
    this->sut_sensor_vision = _sut_sensor_vision;
    this->sut_product_vision = _sut_product_vision;
    this->lens_vision = _lens_vision;
    this->lens_vacancy_vision = _lens_vacancy_vision;
    this->lut_vision = _lut_vision;
    this->lut_lens_vision = _lut_lens_vision;
    this->sut_vacuum = _sutVacuum;
    this->lut_vacuum = _lutVacuum;
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

bool SingleHeadMachineMaterialLoaderModule::ToPickCmosPosition()
{
    //check pickarm at pick safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_cmos_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at pick safe height failed");
        return false;
    }
//    qInfo("pick cmos x:%f y:%f",pick_cmos_base_pos.x,pick_cmos_base_pos.y);
    double cmosx,cmosy,cmosc;
    cmosx = pick_arm->pick_cmos_base_position.x()+pick_arm->cmos_to_pr_distance_position.X();
    cmosy = pick_arm->pick_cmos_base_position.y()+pick_arm->cmos_to_pr_distance_position.Y();
    cmosc = pick_arm->pick_cmos_base_position.th() ;//todo退迴的料暫時未記錄角度誤差
    //z down to pick coms
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->pick_cmos_base_position.z());
    if(!res)
    {
        qInfo("z down to pick cmos failed!");
        return false;
    }
    //move to pick coms
    res = pick_arm->XYC2SyncMove(cmosx,cmosy,cmosc);
    if(!res)
    {
        qInfo("move to pick coms failed!");
        return false;
    }
    return true;
}

bool SingleHeadMachineMaterialLoaderModule::ToPickLensPosition()
{
    //check pickarm at pick safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_lens_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at pick safe height failed");
        return false;
    }
    double lensx,lensy;
    lensx = pick_arm->pick_lens_base_position.x()+pick_arm->lens_to_pr_distance_position.X();
    lensy = pick_arm->pick_lens_base_position.y()+pick_arm->lens_to_pr_distance_position.Y();

    //go to pick coms
    res = pick_arm->XYZC1SyncMove(lensx,lensy,pick_arm->pick_lens_base_position.z(),pick_arm->pick_lens_base_position.th());////todo退迴的料暫時未記錄角度誤差
    if(!res)
    {
        qInfo("go to pick cmos failed!");
        return false;
    }
    return true;
}

bool SingleHeadMachineMaterialLoaderModule::PickLens(double force)
{
    //softlanding to pick cmos
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->pick_lens_base_position.v(),pick_arm->pick_lens_base_position.z());
    bool res = pick_arm->vcm1SoftLanding(force,vcm1_pos);
    if(!res)
    {
        qInfo("softlanding to pick cmos failed!");
        return false;
    }

    bool res_v = pick_arm->vacuum_lens_suction->Set(true);
    if(!res_v)
        qInfo("vacuum1 set true failed!");
    //pick cmos softlanding reutrn
    res = pick_arm->vcm1SoftLandingReturn();
    if(!res)
    {
        qInfo("pick cmos softlanding reutrn failed!");
        return false;
    }
    return res&&res_v;
}

bool SingleHeadMachineMaterialLoaderModule::ToSUTPosition()
{
    //check pickarm at place safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->place_cmos_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
    //to pos above place coms
    double xpos = pick_arm->place_cmos_base_position.x() + pick_arm->cmos_to_pr_distance_position.X() + pick_arm->cmos_escape_offset_position.x();
    double ypos = pick_arm->place_cmos_base_position.y() + pick_arm->cmos_to_pr_distance_position.Y() + pick_arm->cmos_escape_offset_position.y();
    double cpos = pick_arm->place_cmos_base_position.th() + pick_arm->cmos_escape_offset_position.th();
    res = pick_arm->XYC2SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("to pos above place coms failed");
        return false;
    }
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::CheckSUTExistCmos()
{
    return pick_arm->vacuum_sut->IsVacuum();
}

bool SingleHeadMachineMaterialLoaderModule::ToLUTPosition()
{
    //check pickarm at place safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->place_lens_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }

    //to pos above place lens
    double xpos = pick_arm->place_lens_base_position.x();
    double ypos = pick_arm->place_lens_base_position.y();
    double cpos = pick_arm->place_lens_base_position.th();
    res = pick_arm->XYC1SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("to pos above place lens failed");
        return false;
    }
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::picker1ToLUTPosition()
{
    //check pickarm at place safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->place_lens_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }

    //to pos above place lens
    double xpos = pick_arm->place_lens_base_position.x() + pick_arm->lens_to_pr_distance_position.X();
    double ypos = pick_arm->place_lens_base_position.y() + pick_arm->lens_to_pr_distance_position.Y();
    double cpos = pick_arm->place_lens_base_position.th();
    res = pick_arm->XYC1SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("to pos above place lens failed");
        return false;
    }
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::CheckLUTExistLens()
{
    return pick_arm->vacuum_lut->IsVacuum();
}

bool SingleHeadMachineMaterialLoaderModule::PlaceToLUT(double force)
{
    //first check if xyzvc pos is right
    PickArmPos target_pos;
    target_pos.x = pick_arm->place_lens_base_position.x() + pick_arm->lens_to_pr_distance_position.X();
    target_pos.y = pick_arm->place_lens_base_position.y() + pick_arm->lens_to_pr_distance_position.Y();
    target_pos.z = pick_arm->place_lens_base_position.z();
    target_pos.v = parameters.vcm1BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->place_lens_base_position.th();
    bool res = pick_arm->sucker1getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker1当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down to place lens
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->place_lens_base_position.z());
    if(!res)
    {
        qInfo("z down to place lens failed!");
        return false;
    }
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->place_lens_base_position.v() ,pick_arm->place_lens_base_position.z());
    res = pick_arm->vcm1SoftLanding(force,vcm1_pos);
    if(!res)
    {
        qInfo("vcm1 soft landing to place lens failed!");
        return false;
    }

//    XtMotion::xtout_lens_vacuum = true;
    bool res_v = pick_arm->vacuum_lut->Set(true,false);
    res_v &= pick_arm->vacuum_lens_suction->Set(false);
    res_v &= pick_arm->vacuum_lut->Wait(true);
    if(!res_v)
        qInfo("vacuum1 set false failed!");

    //leave
    res = pick_arm->vcm1SoftLandingReturn();
    if(!res)
    {
        qInfo("vcm1 soft landing return failed!");
        return false;
    }

    return res&&res_v;
}

bool SingleHeadMachineMaterialLoaderModule::CheckVcm1NoWorkPeice()
{
    return pick_arm->vacuum_lens_suction->IsVacuum();
}

bool SingleHeadMachineMaterialLoaderModule::CheckVcm2NoWorkPeice()
{
    return pick_arm->vacuum_sensor_suction->IsVacuum();
}

bool SingleHeadMachineMaterialLoaderModule::ToPickProductPosition()
{
    //check pickarm at place safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_product_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }

    //to pos above pick product
    double xpos = pick_arm->pick_product_base_position.x() + pick_arm->lens_to_pr_distance_position.X();
    double ypos = pick_arm->pick_product_base_position.y() + pick_arm->lens_to_pr_distance_position.Y();
    double cpos = pick_arm->pick_product_base_position.th();
    res = pick_arm->XYC1SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("to pos above pick product failed");
        return false;
    }
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::PickProduct(double force)
{
    //first check if xyzvc pos is right
    PickArmPos target_pos;
    target_pos.x = pick_arm->pick_product_base_position.x() + pick_arm->lens_to_pr_distance_position.X();
    target_pos.y = pick_arm->pick_product_base_position.y() + pick_arm->lens_to_pr_distance_position.Y();
    target_pos.z = pick_arm->pick_product_base_position.z();
    target_pos.v = parameters.vcm1BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->pick_product_base_position.th();
    bool res = pick_arm->sucker1getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker1当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down for pick product
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->pick_product_base_position.v() + pick_arm->cmos_escape_offset_position.v(),pick_arm->pick_product_base_position.z());
    res = pick_arm->ZV1MoveSync(pick_arm->pick_product_base_position.z(),vcm1_pos);
    if(!res)
    {
        qInfo("sucker1 down for pick product failed!");
        return false;
    }

    //vcm1 soft landing to pick product
    res = pick_arm->vcm1SoftLanding(force,vcm1_pos + pick_arm->cmos_escape_offset_position.v());
    if(!res)
    {
        qInfo("vcm1 soft landing to pick product failed!");
        return false;
    }

    bool res_v = pick_arm->pogopin->Set(false,false);
    res_v &= pick_arm->vacuum_sut->Set(false,false);
    res_v &= pick_arm->vacuum_lens_suction->Set(true);
    res_v &= pick_arm->pogopin->Wait(false);
    res_v &= pick_arm->vacuum_sut->Wait(false);
    if(!res_v)
        qInfo("vacuum1 sut vacuum or pogopin failed!");
//    XtMotion::xtout_pogopin = false;
//    XtMotion::xtout_cmos_vacuum = false;
//    v1->Set(true);
//    QThread::msleep(200);

    //list cmos_escape_offset.v
    res = pick_arm->vcm1SoftLandingReturn();
    if(!res)
    {
        qInfo("vcm1 soft landing return failed!");
        return false;
    }

    //vcm1 avoid obstacle
    double xpos = pick_arm->pick_product_base_position.x()+pick_arm->lens_to_pr_distance_position.X()+ pick_arm->cmos_escape_offset_position.x();
    double ypos = pick_arm->pick_product_base_position.y()+pick_arm->lens_to_pr_distance_position.Y()+ pick_arm->cmos_escape_offset_position.y();
    double cpos = pick_arm->pick_product_base_position.th() + pick_arm->cmos_escape_offset_position.th();
    res = pick_arm->XYC1SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("vcm1 avoid obstacle failed!");
        return false;
    }

    //vcm1 to saft height
    res = pick_arm->motor_vcm1->MoveToPosSync(parameters.vcm1BaseHeight() - parameters.vcmSafeHeight());
    if(!res)
    {
        qInfo("vcm1 to saft height failed!");
        return false;
    }

    return res&&res_v;
}

bool SingleHeadMachineMaterialLoaderModule::picker1ToSUTPosition()
{
    //check pickarm at place safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->place_cmos_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
//    res = XYZSyncMove(place_cmos_base_pos.x,place_cmos_base_pos.y,place_cmos_base_pos.z);
//    if(!res)
//    {
//        qInfo("to pos above pr ng coms failed");
//        return false;
//    }
    //to pos above pick ng coms
    double xpos = pick_arm->place_cmos_base_position.x() + pick_arm->lens_to_pr_distance_position.X();
    double ypos = pick_arm->place_cmos_base_position.y() + pick_arm->lens_to_pr_distance_position.Y();
    double cpos = pick_arm->place_cmos_base_position.th();
    res = pick_arm->XYC2SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("to pos above place ng coms failed");
        return false;
    }
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PickFromSUT(double force)
{
    //first check if xyzvc pos is right
    PickArmPos target_pos;
    target_pos.x = pick_arm->place_cmos_base_position.x() + pick_arm->lens_to_pr_distance_position.X();
    target_pos.y = pick_arm->place_cmos_base_position.y() + pick_arm->lens_to_pr_distance_position.Y();
    target_pos.z = pick_arm->place_cmos_base_position.z();
    target_pos.v = parameters.vcm1BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->place_cmos_base_position.th();
    bool res = pick_arm->sucker1getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker1当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down for pick ng coms
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->place_cmos_base_position.v() + pick_arm->cmos_escape_offset_position.v(),pick_arm->place_cmos_base_position.z());
    res = pick_arm->ZV1MoveSync(pick_arm->place_cmos_base_position.z(),vcm1_pos);
    if(!res)
    {
        qInfo("sucker1 down for pick ng coms failed!");
        return false;
    }

    //vcm1 soft landing to pick ng coms
    res = pick_arm->vcm1SoftLanding(force, vcm1_pos + pick_arm->cmos_escape_offset_position.v());
    if(!res)
    {
        qInfo("vcm1 soft landing to pick ng coms failed!");
        return false;
    }

    bool res_v = pick_arm->pogopin->Set(false,false);
    res_v &= pick_arm->vacuum_sut->Set(false,false);
    res_v &= pick_arm->vacuum_lens_suction->Set(true);
    res_v &= pick_arm->pogopin->Wait(false);
    res_v &= pick_arm->vacuum_sut->Wait(false);
    if(!res_v)
        qInfo("vacuum1 sut vacuum or pogopin failed!");
//    XtMotion::xtout_pogopin = false;
//    XtMotion::xtout_cmos_vacuum = false;
//    v1->Set(true);
//    QThread::msleep(200);

    //list cmos_escape_offset.v
    res = pick_arm->vcm1SoftLandingReturn();
    if(!res)
    {
        qInfo("vcm1 soft landing return failed!");
        return false;
    }

    //vcm1 avoid obstacle
    double xpos = pick_arm->place_cmos_base_position.x() + pick_arm->lens_to_pr_distance_position.X() + pick_arm->cmos_escape_offset_position.x();
    double ypos = pick_arm->place_cmos_base_position.y() + pick_arm->lens_to_pr_distance_position.Y() + pick_arm->cmos_escape_offset_position.y();
    double cpos = pick_arm->place_cmos_base_position.th() + pick_arm->cmos_escape_offset_position.th();
    res = pick_arm->XYC1SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("vcm1 avoid obstacle failed!");
        return false;
    }

    //vcm1 to saft height
    res = pick_arm->motor_vcm1->MoveToPosSync(parameters.vcm1BaseHeight() - parameters.vcmSafeHeight());
    if(!res)
    {
        qInfo("vcm1 to saft height failed!");
        return false;
    }

    return res&&res_v;
}

bool SingleHeadMachineMaterialLoaderModule::picker2ToLUTPosition()
{
    //check pickarm at place safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->place_lens_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
    //to pos above pick ng lens
    double xpos = pick_arm->place_lens_base_position.x() + pick_arm->cmos_to_pr_distance_position.X();
    double ypos = pick_arm->place_lens_base_position.y() + pick_arm->cmos_to_pr_distance_position.Y();
    double cpos = pick_arm->place_lens_base_position.th();
    res = pick_arm->XYC2SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("to pos above pick ng lens failed");
        return false;
    }
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PickFromLUT(double force)
{
    //first check if xyzvc pos is right
    PickArmPos target_pos;
    target_pos.x = pick_arm->place_lens_base_position.x() + pick_arm->cmos_to_pr_distance_position.X();
    target_pos.y = pick_arm->place_lens_base_position.y() + pick_arm->cmos_to_pr_distance_position.Y();
    target_pos.z = pick_arm->place_lens_base_position.z();
    target_pos.v = parameters.vcm2BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->place_lens_base_position.th();
    bool res = pick_arm->sucker2getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker2当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //z down to pick ng lens
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->place_lens_base_position.z());
    if(!res)
    {
        qInfo("z down to pick ng lens failed!");
        return false;
    }
    //vcm2 soft landing to pick ng lens
    double vcm2_pos = pick_arm->vcm2GetMotorPos(pick_arm->place_lens_base_position.v() ,pick_arm->place_lens_base_position.z());
    res = pick_arm->vcm2SoftLanding(force,vcm2_pos);
    if(!res)
    {
        qInfo("vcm2 soft landing to pick ng lens failed!");
        return false;
    }
    bool res_v = pick_arm->vacuum_lut->Set(false,false);
    res_v &= pick_arm->vacuum_sensor_suction->Set(true);
    res_v &= pick_arm->vacuum_lut->Wait(false);
    if(!res_v)
        qInfo("vacumm2 set true fail");
//    v2->Set(true);
//    QThread::msleep(500);

    //leave
    res = pick_arm->vcm2SoftLandingReturn();
    if(!res)
    {
        qInfo("vcm2 soft landing return failed!");
        return false;
    }

    return res&&res_v;
}

bool SingleHeadMachineMaterialLoaderModule::ToPlaceProductPosition()
{
    //check pickarm at place safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->place_product_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at pr height failed");
        return false;
    }
    //to pos above place product
    double xpos = pick_arm->place_product_base_position.x() + pick_arm->lens_to_pr_distance_position.X();
    double ypos = pick_arm->place_product_base_position.y() + pick_arm->lens_to_pr_distance_position.Y();
    double cpos = pick_arm->place_product_base_position.th();
    res = pick_arm->XYC1SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("to pos above place product failed");
        return false;
    }
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::PlaceProductToTray(double force)
{
    //first check if xyzc pos is right
    PickArmPos target_pos;
    target_pos.x = pick_arm->place_product_base_position.x() + pick_arm->lens_to_pr_distance_position.X();
    target_pos.y = pick_arm->place_product_base_position.y() + pick_arm->lens_to_pr_distance_position.Y();
    target_pos.z = pick_arm->place_product_base_position.z();
    target_pos.v = parameters.vcm1BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->place_product_base_position.th();
    bool res = pick_arm->sucker1getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker1当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }
    //down to place ng cmos
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->place_product_base_position.z());
    if(!res)
    {
        qInfo("z down to place product failed!");
        return false;
    }
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->place_product_base_position.v() ,pick_arm->place_product_base_position.z());
    res = pick_arm->vcm1SoftLanding(force,vcm1_pos);
    if(!res)
    {
        qInfo("vcm1 soft landing to place product failed!");
        return false;
    }
    bool res_v = pick_arm->vacuum_lens_suction->Set(false,true,parameters.vcm1FinishDelay());
    if(!res_v)
        qInfo("vacuum1 set false fail");
//    QThread::msleep(1200);

    //leave
    //pick_arm->vacuum_lens_suction->Jet(parameters.vcm1FinishDelay(),DELAY_JET);
    res = pick_arm->vcm1SoftLandingReturn();

    //v1->UnJet();
    if(!res)
    {
        qInfo("vcm1 soft landing return failed!");
        return false;
    }

    return res&&res_v;
}

bool SingleHeadMachineMaterialLoaderModule::picker1ToCMOSPosition()
{
    //check pickarm at pr safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_cmos_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
    //to pos above place ng coms
    qInfo("place used cmos x:%f y:%f",pick_arm->pick_cmos_base_position.x(),pick_arm->pick_cmos_base_position.y());
    double xpos = pick_arm->pick_cmos_base_position.x() + pick_arm->lens_to_pr_distance_position.X();
    double ypos = pick_arm->pick_cmos_base_position.y() + pick_arm->lens_to_pr_distance_position.Y();
    double cpos = pick_arm->pick_cmos_base_position.th();
    res = pick_arm->XYC2SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("to pos above place ng coms failed");
        return false;
    }
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceCMOSToTray(double force)
{
    //first check if xyzvc pos is right
    PickArmPos target_pos;
    target_pos.x = pick_arm->pick_cmos_base_position.x() + pick_arm->lens_to_pr_distance_position.X();
    target_pos.y = pick_arm->pick_cmos_base_position.y() + pick_arm->lens_to_pr_distance_position.Y();
    target_pos.z = pick_arm->pick_cmos_base_position.z();
    target_pos.v = parameters.vcm1BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->pick_cmos_base_position.th();
    bool res = pick_arm->sucker1getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker1当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down to place ng cmos
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->pick_cmos_base_position.z());
    if(!res)
    {
        qInfo("z down to place ng cmos failed!");
        return false;
    }
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->pick_cmos_base_position.v() ,pick_arm->pick_cmos_base_position.z());
    res = pick_arm->vcm1SoftLanding(force,vcm1_pos);
    if(!res)
    {
        qInfo("vcm1 soft landing to place ng cmos failed!");
        return false;
    }

    bool res_v = pick_arm->vacuum_lens_suction->Set(false,true,parameters.vcm1FinishDelay());
    if(!res_v)
        qInfo("vacuum1 set false fail");
//    v1->Set(false);
//    QThread::msleep(1200);

    //leave
    //pick_arm->vacuum_lens_suction->Jet(vcm1_finish_delay,DELAY_JET);
    res = pick_arm->vcm1SoftLandingReturn();
    //v1->UnJet();
    if(!res)
    {
        qInfo("vcm1 soft landing return failed!");
        return false;
    }

    return res&&res_v;
}

bool SingleHeadMachineMaterialLoaderModule::picker2ToLensPosition()
{
    //check pickarm at place pr height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_lens_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
    //to pos above pick ng coms
    double xpos = pick_arm->pick_lens_base_position.x() + pick_arm->cmos_to_pr_distance_position.X();
    double ypos = pick_arm->pick_lens_base_position.y() + pick_arm->cmos_to_pr_distance_position.Y();
    double cpos = pick_arm->pick_lens_base_position.th();
    res = pick_arm->XYC2SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("to pos above place ng coms failed");
        return false;
    }
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PlacelensToTray(double force)
{
    //first check if xyzvc pos is right
    PickArmPos target_pos;
    target_pos.x = pick_arm->pick_lens_base_position.x() + pick_arm->cmos_to_pr_distance_position.X();
    target_pos.y = pick_arm->pick_lens_base_position.y() + pick_arm->cmos_to_pr_distance_position.Y();
    target_pos.z = pick_arm->pick_lens_base_position.z();
    target_pos.v = parameters.vcm2BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->pick_lens_base_position.th();
    bool res = pick_arm->sucker2getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker2当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down to place ng cmos
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->pick_lens_base_position.z());
    if(!res)
    {
        qInfo("z down to place ng cmos failed!");
        return false;
    }
    double vcm2_pos = pick_arm->vcm2GetMotorPos(pick_arm->pick_lens_base_position.v() ,pick_arm->pick_lens_base_position.z());
    res = pick_arm->vcm2SoftLanding(force,vcm2_pos);
    if(!res)
    {
        qInfo("vcm2 soft landing to place ng cmos failed!");
        return false;
    }

    bool res_v = pick_arm->vacuum_sensor_suction->Set(false,true,parameters.vcm2FinishDelay());
    if(!res_v)
        qInfo("vacumm2 set false fail");
//    v2->Set(false);
//    QThread::msleep(1200);

    //leave
    //v2->Jet(vcm2_finish_delay,DELAY_JET);
    res = pick_arm->vcm2SoftLandingReturn();
    //v2->UnJet();
    if(!res)
    {
        qInfo("vcm2 soft landing return failed!");
        return false;
    }

    return res&&res_v;
}

bool SingleHeadMachineMaterialLoaderModule::picker1ToLensPosition()
{
    //check pickarm at pr safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_lens_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
    //to pos above place ng coms
    double xpos = pick_arm->pick_lens_base_position.x() + pick_arm->lens_to_pr_distance_position.X();
    double ypos = pick_arm->pick_lens_base_position.y() + pick_arm->lens_to_pr_distance_position.Y();
    double cpos = pick_arm->pick_lens_base_position.th();
    res = pick_arm->XYC2SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("to pos above place ng coms failed");
        return false;
    }
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::picker1PlaceLensToTray(double force)
{
    //first check if xyzvc pos is right
    PickArmPos target_pos;
    target_pos.x = pick_arm->pick_lens_base_position.x() + pick_arm->lens_to_pr_distance_position.X();
    target_pos.y = pick_arm->pick_lens_base_position.y() + pick_arm->lens_to_pr_distance_position.Y();
    target_pos.z = pick_arm->pick_lens_base_position.z();
    target_pos.v = parameters.vcm1BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->pick_lens_base_position.th();
    bool res = pick_arm->sucker1getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker1当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down to place ng cmos
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->pick_lens_base_position.z());
    if(!res)
    {
        qInfo("z down to place ng cmos failed!");
        return false;
    }
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->pick_lens_base_position.v() ,pick_arm->pick_lens_base_position.z());
    res = pick_arm->vcm1SoftLanding(force,vcm1_pos);
    if(!res)
    {
        qInfo("vcm1 soft landing to place ng cmos failed!");
        return false;
    }

    bool res_v = pick_arm->vacuum_lens_suction->Set(false,true,parameters.vcm1FinishDelay());
    if(!res_v)
        qInfo("vacuum1 set false fail");
//    v1->Set(false);
//    QThread::msleep(1200);

    //leave
    //v1->Jet(vcm1_finish_delay,DELAY_JET);
    res = pick_arm->vcm1SoftLandingReturn();
    //v1->UnJet();
    //v1->WaitUnJet();
    if(!res)
    {
        qInfo("vcm1 soft landing return failed!");
        return false;
    }

    return res&&res_v;
}

bool SingleHeadMachineMaterialLoaderModule::picker2ToCmosPosition()
{
    //check pickarm at place pr height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_cmos_base_position.z());
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
    //to pos above pick ng coms
    double xpos = pick_arm->pick_cmos_base_position.x() + pick_arm->cmos_to_pr_distance_position.X();
    double ypos = pick_arm->pick_cmos_base_position.y() + pick_arm->cmos_to_pr_distance_position.Y();
    double cpos = pick_arm->pick_cmos_base_position.th();
    res = pick_arm->XYC2SyncMove(xpos,ypos,cpos);
    if(!res)
    {
        qInfo("to pos above place ng coms failed");
        return false;
    }
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::picker2PlaceCmosToTray(double force)
{
    //first check if xyzvc pos is right
    PickArmPos target_pos;
    target_pos.x = pick_arm->pick_cmos_base_position.x() + pick_arm->cmos_to_pr_distance_position.X();
    target_pos.y = pick_arm->pick_cmos_base_position.y() + pick_arm->cmos_to_pr_distance_position.Y();
    target_pos.z = pick_arm->pick_cmos_base_position.z();
    target_pos.v = parameters.vcm2BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->pick_cmos_base_position.th();
    bool res = pick_arm->sucker2getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker2当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down to place ng cmos
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->pick_cmos_base_position.z());
    if(!res)
    {
        qInfo("z down to place ng cmos failed!");
        return false;
    }
    double vcm2_pos = pick_arm->vcm2GetMotorPos(pick_arm->pick_cmos_base_position.v() ,pick_arm->pick_cmos_base_position.z());
    res = pick_arm->vcm2SoftLanding(force,vcm2_pos);
    if(!res)
    {
        qInfo("vcm2 soft landing to place ng cmos failed!");
        return false;
    }

    bool res_v = pick_arm->vacuum_sensor_suction->Set(false,true,parameters.vcm2FinishDelay());
    if(!res_v)
        qInfo("vacumm2 set false fail");
//    v2->Set(false);
//    QThread::msleep(1200);

    //leave
    //v2->Jet(vcm2_finish_delay,DELAY_JET);
    res = pick_arm->vcm2SoftLandingReturn();
    //v2->UnJet();
    //v2->WaitUnJet();
    if(!res)
    {
        qInfo("vcm2 soft landing return failed!");
        return false;
    }

    return res&&res_v;
}

bool SingleHeadMachineMaterialLoaderModule::MoveSensorPicker2Cam()
{
    bool res = pick_arm->LiftToSafeHeight();
    if(!res) return false;
    res = pick_arm->motor_x->StepMoveSync(pick_arm->cmos_to_pr_distance_position.X());
    if(!res) return false;
    res = pick_arm->motor_y->StepMoveSync(pick_arm->cmos_to_pr_distance_position.Y());
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::MoveCam2SensorPicker()
{
    bool res = pick_arm->LiftToSafeHeight();
    if(!res) return false;
    res = pick_arm->motor_x->StepMoveSync(-pick_arm->cmos_to_pr_distance_position.X());
    if(!res) return false;
    res = pick_arm->motor_y->StepMoveSync(-pick_arm->cmos_to_pr_distance_position.Y());
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::MoveLensPikcer2Cam()
{
    bool res = pick_arm->LiftToSafeHeight();
    if(!res) return false;
    res = pick_arm->motor_x->StepMoveSync(pick_arm->lens_to_pr_distance_position.X());
    if(!res) return false;
    res = pick_arm->motor_y->StepMoveSync(pick_arm->lens_to_pr_distance_position.Y());
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::MoveCam2LensPicker()
{
    bool res = pick_arm->LiftToSafeHeight();
    if(!res) return false;
    res = pick_arm->motor_x->StepMoveSync(-pick_arm->lens_to_pr_distance_position.X());
    if(!res) return false;
    res = pick_arm->motor_y->StepMoveSync(-pick_arm->lens_to_pr_distance_position.Y());
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::XYZSyncMove(double xpos, double ypos, double zpos)
{
    bool res;
    //XtMotor::Move2AxisToPos(x,xpos,y,ypos);
    pick_arm->motor_x->MoveToPos(xpos);
    pick_arm->motor_y->MoveToPos(ypos);
    pick_arm->motor_z->MoveToPos(zpos);
    res = pick_arm->motor_x->WaitMoveStop();
    res &= pick_arm->motor_y->WaitMoveStop();
    res &= pick_arm->motor_z->WaitMoveStop();
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::ToSaftyHeight(double safty_height)
{
    if(pick_arm->CheckZV1V2AboveSafeHeight(safty_height))
        return true;
    return pick_arm->ZV1V2ToSafeHeight(safty_height);
}

bool SingleHeadMachineMaterialLoaderModule::checkTrayNeedChange()
{
    if(sensorTray->isTrayNeedChange(0))
        return true;
    return false;
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

bool SingleHeadMachineMaterialLoaderModule::moveToSUTPRPos(bool is_local, bool check_softlanding)
{
    qInfo("moveToSUTPRPos is_local %d",is_local);
    bool result;
    result =  pick_arm->move_XmY_Synic(sut_pr_position.ToPointF(),check_softlanding);
    if(!result)
        AppendError(QString("移动SPA到SUT位置失败%1").arg(is_local));
    return result;
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
    bool result =  sut_product_vision->performPR(pr_offset);
    if(result)
        AppendError(QString(u8"执行SUT视觉失败!"));
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSPAWorkPos(bool check_softlanding)
{
    PrOffset temp(sensor_suction_offset.X() - pr_offset.X,sensor_suction_offset.Y() - pr_offset.Y,pr_offset.Theta);
    qInfo("moveToWorkPos offset:(%f,%f,%f)",temp.X,temp.Y,temp.Theta);
    bool result = pick_arm->stepMove_XYT1_Synic(temp.X,temp.Y,temp.Theta,check_softlanding);
    if(result)
        AppendError(QString(u8"去1号吸头工作位置(step x %1,y %2,t %3)失败!").arg(temp.X).arg(temp.Y).arg(temp.Theta));
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::sensorPickerSearchZ(double z, bool is_open, int time_out,int picker)
{
    qInfo("picker1SearchZ z %f is_open %d timeout %d",z,is_open,time_out);
    bool result = pick_arm->ZSerchByForce(picker,parameters.vcm1Svel(),parameters.vcm1PickForce(),z,parameters.vcm1Margin(),parameters.vcm1FinishDelay(),is_open,false,time_out);
    result &= pick_arm->ZSerchReturn(0,time_out);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::sensorPickerSearchSutZ(double z, QString dest, QString cmd, bool is_open, int time_out)
{
    qInfo("picker1SearchSutZ z %f dest %s cmd %s is_open %d time_out %d",z,dest.toStdString().c_str(),cmd.toStdString().c_str(),is_open,time_out);
    bool result = pick_arm->move_XeYe_Z1_XY(z - parameters.escapeHeight(),parameters.escapeX(),parameters.escapeY());
    if(result)
    {
        result = pick_arm->ZSerchByForce(0,parameters.vcm1Svel(),parameters.vcm1PickForce(),z,parameters.vcm1Margin(),parameters.vcm1FinishDelay(),is_open,false,time_out);

        //sut_vacuum
        sut_vacuum->Set(0);
        QThread::msleep(200);
        result &= pick_arm->ZSerchReturn(time_out);
    }
    result &= pick_arm->motor_vcm1->MoveToPosSync(0);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::lensPickerSearchSutZ(double z, QString dest, QString cmd, bool is_open, int time_out)
{
    qInfo("picker1SearchSutZ z %f dest %s cmd %s is_open %d time_out %d",z,dest.toStdString().c_str(),cmd.toStdString().c_str(),is_open,time_out);
    bool result = pick_arm->move_XeYe_Z2_XY(z - parameters.escapeHeight(),parameters.escapeX(),parameters.escapeY());
    if(result)
    {
        result = pick_arm->ZSerchByForce(1,parameters.vcm1Svel(),parameters.vcm1PickForce(),z,parameters.vcm1Margin(),parameters.vcm1FinishDelay(),is_open,false,time_out);

        //sut_vacuum
        sut_vacuum->Set(0);
        QThread::msleep(200);
        result &= pick_arm->ZSerchReturn(time_out);
    }
    result &= pick_arm->motor_vcm1->MoveToPosSync(0);
    return result;
}
/*
 * 捡起tray盘上sensor，调用前保证吸嘴对准sensor中心
*/
bool SingleHeadMachineMaterialLoaderModule::pickTraySensor(int time_out)
{
    qInfo("pickTraySensor time_out %d",time_out);
    bool result = sensorPickerSearchZ(pick_arm->parameters.pickSensorZ(),true,time_out);
    if(!result)
        AppendError(QString(u8"从sensor盘取sensor失败"));
    return result;
}
/*
 * 放置sensor于SUT上，调用前保证吸嘴对准sut_vacuum中心
*/
bool SingleHeadMachineMaterialLoaderModule::placeSensorToSUT(QString dest, int time_out)
{
    qInfo("placeSensorToSUT dest %s time_out %d",dest.toStdString().c_str(),time_out);
   bool result = sensorPickerSearchSutZ(pick_arm->parameters.placeSensorZ(),dest,"vacuumOnReq",false,time_out);
    if(!result)
        AppendError(QString(u8"放sensor到SUT%1失败").arg(dest=="remote"?1:2));
    return result;
}
/*
 * 从SUT捡起NG sensor，调用前吸嘴到位对准
*/
bool SingleHeadMachineMaterialLoaderModule::pickSUTSensor(QString dest, int time_out)
{
    qInfo("pickSUTSensor dest %s time_out %d",dest.toStdString().c_str(),time_out);
    bool result = sensorPickerSearchSutZ(pick_arm->parameters.pickNgSensorZ(),dest,"vacuumOffReq",true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT%1取NGsenor失败").arg(dest=="remote"?1:2));
    return result;
}
/*
 * 从SUT捡起成品，调用前吸嘴到位对准
*/
bool SingleHeadMachineMaterialLoaderModule::pickSUTProduct(QString dest, int time_out)
{
    qInfo("pickSUTProduct dest %s time_out %d",dest.toStdString().c_str(),time_out);
    bool result = lensPickerSearchSutZ(pick_arm->parameters.pickProductZ(),dest,"vacuumOffReq",true,time_out);
    if(!result)
        AppendError(QString(u8"从SUT%1取成品失败").arg(dest=="remote"?1:2));
    return result;
}
/*
 * 放置NG sensor至tray盘上，调用前吸嘴到位对准
*/
bool SingleHeadMachineMaterialLoaderModule::placeSensorToTray(int time_out)
{
    qInfo("placeSensorToTray time_out %d",time_out);
    bool result = sensorPickerSearchZ(pick_arm->parameters.placeNgSensorZ(),false,time_out);
    if(!result)
        AppendError(QString(u8"将Ngsensor放入NG盘失败"));
    return result;
}
/*
 * 放置成品至tray盘上，调用前吸嘴到位对准
*/
bool SingleHeadMachineMaterialLoaderModule::placeProductToTray(int time_out)
{
    qInfo("placeProductToTray time_out %d",time_out);
    bool result = sensorPickerSearchZ(pick_arm->parameters.placeProductZ(),false,time_out,1);
    if(!result)
        AppendError(QString(u8"将成品放入成品盘失败"));
    return result;
}
/*
 * sensor吸嘴测高
*/
bool SingleHeadMachineMaterialLoaderModule::sensorPickerMeasureHight(bool is_tray, bool is_product,bool is_ng)
{
    qInfo("picker2MeasureHight is_tray %d is_product %d",is_tray,is_product);
    if(pick_arm->ZSerchByForce(0,parameters.vcm1Svel(),parameters.vcm1PickForce(),true))
    {
        QThread::msleep(100);
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否应用此高度:%1").arg(pick_arm->GetSoftladngPosition()))){
            return true;
        }
        if(is_tray)
        {
            if(is_product)
                pick_arm->parameters.setPlaceProductZ(pick_arm->GetSoftladngPosition());
            else{
                if(is_ng){
                    pick_arm->parameters.setPlaceNgSensorZ(pick_arm->GetSoftladngPosition());
                }
                else{
                    pick_arm->parameters.setPickSensorZ(pick_arm->GetSoftladngPosition());
                }
            }
        }
        else
        {
            if(is_product)
                pick_arm->parameters.setPickProductZ(pick_arm->GetSoftladngPosition());
            else
            {
                if(is_ng)
                    pick_arm->parameters.setPickNgSensorZ(pick_arm->GetSoftladngPosition());
                else
                    pick_arm->parameters.setPlaceSensorZ(pick_arm->GetSoftladngPosition());
            }

        }
        return true;
    }
    AppendError(QString(u8"2号吸头测高失败"));
    return false;
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
    result &= pick_arm->move_XY_Synic(sensorTray->getCurrentPosition(tray_index),true);
    if(!result)
        AppendError(QString(u8"移动到%1盘当前位置失败").arg(tray_index == 0?"sensor":"成品"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSensorStartPos(int tray_index)
{
    qInfo("moveToStartPos%d",tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(sensorTray->getStartPosition(tray_index),true);
    if(!result)
        AppendError(QString(u8"移动到%1盘起始位置失败").arg(tray_index == 0?"sensor":"成品"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToSensorTray1EndPos()
{
    qInfo("moveToTray1EndPos");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XY_Synic(sensorTray->getEndPosition(),true);
    if(!result)
        AppendError(QString(u8"移动到sensor盘结束位置失败"));
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToNextLensTrayPos(int tray_index)
{
    qInfo("moveToNextTrayPos:%d",tray_index);
    if(lensTray->findNextPositionOfInitState(tray_index))
    {
//        return  pick_arm->move_XtXY_Synic(tray->getCurrentPosition(tray_index),parameters.visonPositionX());
        return  pick_arm->move_XtXYT2_Synic(lensTray->getCurrentPosition(tray_index),parameters.visionPositionX(),pick_arm->parameters.pickLensTheta());
    }
    return  false;
}

bool SingleHeadMachineMaterialLoaderModule::moveToLUTPRPos(bool check_softlanding)
{
    double theta = pr_offset.Theta;
    if (pick_arm->parameters.placeLensTheta() < 0) theta = -pr_offset.Theta;
    pr_offset.Theta = 0; //Reset the last pr result
    double target_t = pick_arm->motor_th2->GetFeedbackPos() + theta;
    return  pick_arm->move_XYT2_Synic(lut_pr_position.X(),lut_pr_position.Y(),target_t,check_softlanding);
}

bool SingleHeadMachineMaterialLoaderModule::checkNeedChangeTray()
{
    if(lensTray->isTrayNeedChange(0)&&lensTray->isTrayNeedChange(1))
        return true;
    return false;
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

bool SingleHeadMachineMaterialLoaderModule::performLensPickerPR()
{
    return lpa_picker_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::performNGLensPR()
{
    return lut_lens_vision->performPR(pr_offset);
}

bool SingleHeadMachineMaterialLoaderModule::moveToLPAWorkPos(bool check_softlanding)
{
    PrOffset temp(lens_suction_offset.X() - pr_offset.X,lens_suction_offset.Y() - pr_offset.Y,0);
    bool result = pick_arm->stepMove_XYT2_Synic(temp,check_softlanding);
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToLPAPrOffset(bool check_softlanding)
{
    PrOffset temp(- pr_offset.X, - pr_offset.Y,pr_offset.Theta);
    bool result = pick_arm->stepMove_XYT2_Synic(temp,check_softlanding);
    return  result;
}

bool SingleHeadMachineMaterialLoaderModule::vcm2SearchZ(double z, bool is_open)
{
    return pick_arm->ZSerchByForce(1,parameters.vcm2Svel(),parameters.vcm2PickForce(),z,parameters.vcm2Margin(),parameters.vcm2FinishDelay(),is_open);
}

bool SingleHeadMachineMaterialLoaderModule::pickTrayLens()
{
    qInfo("pickTrayLens");
    bool result = vcm2SearchZ(pick_arm->parameters.pickLensZ(),true);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::placeLensToLUT()
{
    qInfo("placeLensToLUT");
    this->lut_vacuum->Set(true);
    bool result = vcm2SearchZ(pick_arm->parameters.placeLensZ(), false);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::pickLUTLens()
{
    qInfo("pickLUTLens");
    return vcm2SearchZ(pick_arm->parameters.placeLensZ(),true);
}

bool SingleHeadMachineMaterialLoaderModule::placeLensToTray()
{
    qInfo("placeLensToTray");
    return vcm2SearchZ(pick_arm->parameters.pickLensZ(),false);
}

bool SingleHeadMachineMaterialLoaderModule::lensPickerMeasureHight(bool is_tray,bool is_product)
{
    qInfo("measureHight speed: %f force: %f", parameters.vcm2Svel(), parameters.vcm2PickForce());
    if(pick_arm->ZSerchByForce(1,parameters.vcm2Svel(),parameters.vcm2PickForce(),true))
    {
        QThread::msleep(100);
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否应用此高度:%1？").arg(pick_arm->GetSoftladngPosition()))){
            return true;
        }
        if(is_product){
            if(is_tray)
                pick_arm->parameters.setPlaceProductZ(pick_arm->GetSoftladngPosition(false,1));
            else
                pick_arm->parameters.setPickProductZ(pick_arm->GetSoftladngPosition(false,1));
        }else{
            if(is_tray)
                pick_arm->parameters.setPickLensZ(pick_arm->GetSoftladngPosition(false,1));
            else
                pick_arm->parameters.setPlaceLensZ(pick_arm->GetSoftladngPosition(false,1));
        }
        return true;
    }
    return false;
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

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayPos(int index, int tray_index)
{
    qInfo("moveToTrayPos");
//    return pick_arm->move_XtXY_Synic(tray->getPositionByIndex(index,tray_index),parameters.visonPositionX());
    return pick_arm->move_XtXYT2_Synic(lensTray->getPositionByIndex(index,tray_index),parameters.visionPositionX(),pick_arm->parameters.pickLensTheta());
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensTrayPos(int tray_index)
{
    qInfo("moveToTrayPos %d",tray_index);
//    return  pick_arm->move_XtXY_Synic(tray->getCurrentPosition(tray_index),parameters.visonPositionX(),false);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XtXYT2_Synic(lensTray->getCurrentPosition(tray_index),parameters.visionPositionX(),pick_arm->parameters.pickLensTheta(),false);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensStartPos(int tray_index)
{
    qInfo("moveToStartPos%d",tray_index);
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XtXY_Synic(lensTray->getStartPosition(tray_index),parameters.visionPositionX(),true);
    return result;
}

bool SingleHeadMachineMaterialLoaderModule::moveToLensTray1EndPos()
{
    qInfo("moveToTray1EndPos");
    bool result = pick_arm->move_Xm_Origin();
    result &= pick_arm->move_XtXY_Synic(lensTray->getEndPosition(),parameters.visionPositionX(),true);
    return result;
}

void SingleHeadMachineMaterialLoaderModule::startWork(int run_mode)
{

}

void SingleHeadMachineMaterialLoaderModule::stopWork(bool wait_finish)
{

}

void SingleHeadMachineMaterialLoaderModule::resetLogic()
{

}

void SingleHeadMachineMaterialLoaderModule::performHandlingOperation(int cmd)
{
    qInfo("performHandling %d",cmd);
    bool result;
//    int curren
    int handlePosition = cmd&HANDLE_POSITION;
    switch (handlePosition){
    case LENS_TRAY1:
    {
        qInfo("lpa move to lensTray1's current position, cmd: %d",LENS_TRAY1);
        result = moveToLensTrayPos(0);
    }
        break;
    case LENS_TRAY2:
    {
        qInfo("lpa move to lensTray2's current position, cmd: %d",LENS_TRAY2);
        result = moveToLensTrayPos(1);
    }
        break;
    case LUT_POS:
    {
        qInfo("lpa move to LUT position,cmd: %d",LUT_POS);
        result = moveToLUTPRPos();
    }
        break;
    case LENS_TRAY1_START_POS:
    {
        qInfo("lpa move to tray1 start position,cmd: %d",LENS_TRAY1_START_POS);
        result = moveToLensStartPos(0);
    }
        break;
    case LENS_TRAY2_START_POS:
    {
        qInfo("lpa move to tray2 start position,cmd: %d",LENS_TRAY2_START_POS);
        result = moveToLensStartPos(1);
    }
        break;
    case LENS_TRAY1_END_POS:
    {
        qInfo("lpa move to tray1 end position,cmd: %d",LENS_TRAY1_END_POS);
        result = moveToLensTray1EndPos();
    }
        break;
    case UPDOWNLOOK_DOWN_POS:
    {
        qInfo("UPDOWNLOOK_DOWN_POS");
        result = true;
    }
        break;
    case UPDOWNLOOK_UP_POS:
    {
        qInfo("UNPDOWNLOOK_UP_POS");
        result = true;
    }
        break;
    case SENSOR_TRAY:
    {
        qInfo("spa move to sensor tray current position,cmd: %d",SENSOR_TRAY);
        result = moveToSensorTrayPos(0);
    }
        break;
    case SUT_POS:
    {
        qInfo("spa move to sut position,cmd: %d",SUT_POS);
        result = moveToSUTPRPos();
    }
        break;
    case SENSOR_TRAY_START_POS:
    {
        qInfo("spa move to sensor tray start position,cmd: %d",SENSOR_TRAY_START_POS);
        result = moveToSensorStartPos(0);
    }
        break;
    case SENSOR_TRAY_END_POS:
    {
        qInfo("spa move to snesor tray end position,cmd: %d",SENSOR_TRAY_END_POS);
        result = moveToSensorTray1EndPos();
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
        result = performNGLensPR();
    }
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
    case UPDOWNLOOK_DOWN_PR:
    {
        qInfo("UPDOWNLOOK_DOWN_PR");
    }
        break;
    case LENS_PICKER_PR:
    {
        qInfo("perform lens suction PR");
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
    case LPA_TO_WORK:
    {
        qInfo("lens suction move offset,cmd: %d",LPA_TO_WORK);
        result = moveToLPAWorkPos();
    }
        break;
    case LPA_TO_PROFFSET:
    {
        qInfo("LPA_TO_PROFFSET");
    }
        break;
    case SENSOR_TO_PICK:
    {
        qInfo("sensor suction move offset,cmd: %d",SENSOR_TO_PICK);
        result = moveToSPAWorkPos();
    }
        break;
    case SENSOR_TO_PR_OFFSET:
    {
        qInfo("SENSOR_TO_PR_OFFSET");
    }
        break;
    default:
        result = true;
        break;
    }
    if(!result)
    {
        //        finished_type = FinishedType::Alarm;
        qInfo("Move To Work Pos fail");
        return;
    }
    int handlePickerAction = cmd&HANDLE_PICKER_ACTION;
    switch (handlePickerAction) {
    case PICK_SENSOR_FROM_TRAY:
    {
        qInfo("move to pick sensor from tray,cmd: %d",PICK_SENSOR_FROM_TRAY);
        result = pickTraySensor();
    }
        break;
    case PLACE_SENSOR_TO_SUT:
    {
        qInfo("move to place sensor to SUT,cmd: %d",PLACE_SENSOR_TO_SUT);
        result = placeSensorToSUT("remote");
    }
        break;
    case PICK_NG_SENSOR_FROM_SUT:
    {
        qInfo("move to pick ngSensor from SUT,cmd: %d",PICK_NG_SENSOR_FROM_SUT);
        result = pickSUTSensor("remote");
    }
        break;
    case PLACE_NG_SENSOR_TO_TRAY:
    {
        qInfo("move to place sensor to tray,cmd: %d",PLACE_NG_SENSOR_TO_TRAY);
        result = placeSensorToTray();
    }
        break;
    case PICK_PRODUCT_FROM_SUT:
    {
        qInfo("move to pick product form sut,cmd: %d",PICK_PRODUCT_FROM_SUT);
        result = pickSUTProduct("remote");
    }
        break;
    case PLACE_PRODUCT_TO_TRAY:
    {
        qInfo("move to place product to tray,cmd: %d",PLACE_PRODUCT_TO_TRAY);
        result = placeProductToTray();
    }
        break;
    case MEASURE_SENSOR_IN_TRAY:
    {
        qInfo("mesure pick senso from tray height, cmd: %d",MEASURE_SENSOR_IN_TRAY);
        result = sensorPickerMeasureHight(true,false,false);

    }
        break;
    case MEASURE_NG_SENSOR_IN_TRAY:
    {
        qInfo("measure place ng sensor to tray height, cmd: %d",MEASURE_NG_SENSOR_IN_TRAY);
        result = sensorPickerMeasureHight(true,false);
    }
        break;
    case MEASURE_PRODUCT_IN_TRAY:
    {
        qInfo("measure place product to tray height, cmd: %d ",MEASURE_PRODUCT_IN_TRAY);
        result = lensPickerMeasureHight(true,true);
    }
        break;
    case MEASURE_SENSOR_IN_SUT:
    {
        qInfo("measure place sensor to SUT,cmd: %d",MEASURE_SENSOR_IN_SUT);
        result = sensorPickerMeasureHight(false,false,false);
    }
        break;
    case MEASURE_NG_SENSOR_IN_SUT:
    {
        qInfo("measure pick ng sensor from SUT, cmd: %d",MEASURE_NG_SENSOR_IN_SUT);
        result = sensorPickerMeasureHight(false,false);
    }
        break;
    case MEASURE_PRODUCT_IN_SUT:
    {
        qInfo("measure pick product form SUT, cmd: %d",MEASURE_PRODUCT_IN_SUT);
        result = lensPickerMeasureHight(false,true);
    }
        break;
    case PICK_LENS_FROM_TRAY:
    {
        qInfo("pick lens form tray, cmd: %d",PICK_LENS_FROM_TRAY);
        result = pickTrayLens();
    }
        break;
    case PLACE_LENS_TO_LUT:
    {
        qInfo("place lens to SUT,cmd: %d",PLACE_LENS_TO_LUT);
        result = placeLensToLUT();
    }
        break;
    case PICK_NG_LENS_FROM_LUT:
    {
        qInfo("pick ng lens from LUT,cmd: %d",PICK_NG_LENS_FROM_LUT);
        result = pickLUTLens();
    }
        break;
    case PLACE_NG_LENS_TO_TRAY:
    {
        qInfo("place ng lens to tray,cmd: %d",PLACE_NG_LENS_TO_TRAY);
        result = placeLensToTray();
    }
        break;
    case MEASURE_LENS_IN_TRAY:
    {
        qInfo("measure pick lens from tray height,cmd: %d",MEASURE_LENS_IN_TRAY);
        result = lensPickerMeasureHight(true);
    }
        break;
    case MEASURE_LENS_IN_LUT:
    {
        qInfo("measuer place lens to LUT height,cmd: %d",MEASURE_LENS_IN_LUT);
        result = lensPickerMeasureHight(false);
    }
        break;
    default:
        result = true;
        break;
    }
    return;
}

