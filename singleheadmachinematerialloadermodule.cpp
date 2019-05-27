#include "singleheadmachinematerialloadermodule.h"

SingleHeadMachineMaterialLoaderModule::SingleHeadMachineMaterialLoaderModule(QString name)
:ThreadWorkerBase (name)
{

}

void SingleHeadMachineMaterialLoaderModule::Init(SingleHeadMachineMaterialPickArm *pick_arm)
{
    this->pick_arm = pick_arm;
    this->pick_arm->parent = this;

}

void SingleHeadMachineMaterialLoaderModule::loadJsonConfig(QString file_name)
{

}

void SingleHeadMachineMaterialLoaderModule::saveJsonConfig(QString file_name)
{

}

bool SingleHeadMachineMaterialLoaderModule::ToPickCmosPosition()
{
    //check pickarm at pick safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_cmos_base_pos.z);
    if(!res)
    {
        qInfo("check pickarm at pick safe height failed");
        return false;
    }
//    qInfo("pick cmos x:%f y:%f",pick_cmos_base_pos.x,pick_cmos_base_pos.y);
    double cmosx,cmosy,cmosc;
    cmosx = pick_arm->pick_cmos_base_pos.x+pick_arm->cmos_to_pr_distance.x();
    cmosy = pick_arm->pick_cmos_base_pos.y+pick_arm->cmos_to_pr_distance.y();
    cmosc = pick_arm->pick_cmos_base_pos.th ;//todo退迴的料暫時未記錄角度誤差
    //z down to pick coms
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->pick_cmos_base_pos.z);
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

bool SingleHeadMachineMaterialLoaderModule::PickCMOS(double force)
{
    //softlanding to pick cmos
    double vcm2_pos=pick_arm->vcm2GetMotorPos(pick_arm->pick_cmos_base_pos.v,pick_arm->pick_cmos_base_pos.z);
    bool res = pick_arm->vcm2SoftLanding(force,vcm2_pos);
    if(!res)
    {
        qInfo("softlanding to pick cmos failed!");
        return false;
    }
    bool res_v = pick_arm->vacuum_sensor_suction->Set(true);
    if(!res_v)
        qInfo("vacuum2 set true fail");
//    v2->Set(true);
//    QThread::msleep(200);
    //pick cmos softlanding reutrn
    res = pick_arm->vcm2SoftLandingReturn();
    if(!res)
    {
        qInfo("pick cmos softlanding reutrn failed!");
        return false;
    }
    return res&&res_v;
}

bool SingleHeadMachineMaterialLoaderModule::ToPickLensPosition()
{
    //check pickarm at pick safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_lens_base_pos.z);
    if(!res)
    {
        qInfo("check pickarm at pick safe height failed");
        return false;
    }
    double lensx,lensy;
    lensx = pick_arm->pick_lens_base_pos.x+pick_arm->lens_to_pr_distance.x();
    lensy = pick_arm->pick_lens_base_pos.y+pick_arm->lens_to_pr_distance.y();

    //go to pick coms
    res = pick_arm->XYZC1SyncMove(lensx,lensy,pick_arm->pick_lens_base_pos.z,pick_arm->pick_lens_base_pos.th);////todo退迴的料暫時未記錄角度誤差
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
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->pick_lens_base_pos.v,pick_arm->pick_lens_base_pos.z);
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
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->place_cmos_base_pos.z);
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
    //to pos above place coms
    double xpos = pick_arm->place_cmos_base_pos.x + pick_arm->cmos_to_pr_distance.x() + pick_arm->cmos_escape_offset.x;
    double ypos = pick_arm->place_cmos_base_pos.y + pick_arm->cmos_to_pr_distance.y() + pick_arm->cmos_escape_offset.y;
    double cpos = pick_arm->place_cmos_base_pos.th + pick_arm->cmos_escape_offset.th;
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

bool SingleHeadMachineMaterialLoaderModule::PlaceToSUT(double force)
{
    //first check if xyzvc pos is right
    PickArmPos target_pos;
    target_pos.x = pick_arm->place_cmos_base_pos.x + pick_arm->cmos_to_pr_distance.x() + pick_arm->cmos_escape_offset.x;
    target_pos.y = pick_arm->place_cmos_base_pos.y + pick_arm->cmos_to_pr_distance.y() + pick_arm->cmos_escape_offset.y;
    target_pos.z = pick_arm->place_cmos_base_pos.z;
    target_pos.v = parameters.vcm2BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->place_cmos_base_pos.th + pick_arm->cmos_escape_offset.th;
    bool res = pick_arm->sucker2getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker2当前电机位置不在目标位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }
    //down for feed
    double vcm2_pos = pick_arm->vcm2GetMotorPos(pick_arm->place_cmos_base_pos.v + pick_arm->cmos_escape_offset.v,pick_arm->place_cmos_base_pos.z);
    res = pick_arm->ZV2MoveSync(pick_arm->place_cmos_base_pos.z,vcm2_pos);
    if(!res)
    {
        qInfo("sucker2 down for feed failed!");
        return false;
    }

    //feed in
    res = pick_arm->XYC2SyncMove(pick_arm->place_cmos_base_pos.x + pick_arm->cmos_to_pr_distance.x(), pick_arm->place_cmos_base_pos.y + pick_arm->cmos_to_pr_distance.y(), pick_arm->place_cmos_base_pos.th);
    if(!res)
    {
        qInfo("sucker2 feed in failed!");
        return false;
    }

    //down to place
    res = pick_arm->vcm2SoftLanding(force,vcm2_pos + pick_arm->cmos_escape_offset.v);
    if(!res)
    {
        qInfo("vcm2  soft landing  place coms failed!");
        return false;
    }

    //place
    bool res_v = pick_arm->pogopin->Set(true,false);
    res_v &= pick_arm->vacuum_sut->Set(true,false);
    res_v &= pick_arm->vacuum_sensor_suction->Set(false);
    res_v &= pick_arm->pogopin->Wait(true,3000);
    res_v &= pick_arm->vacuum_sut->Wait(true);
    if(!res_v)
        qInfo("vacuum2 sut vacuum or pogopin failed!");
//    XtMotion::xtout_cmos_vacuum = true;
//    XtMotion::xtout_pogopin = true;
//    v2->Set(false);
//    QThread::msleep(500);

    //leave
    res = pick_arm->vcm2SoftLandingReturn();
    if(!res)
    {
        qInfo("vcm2 soft landing return failed!");
        return false;
    }
    res = pick_arm->motor_vcm2->MoveToPosSync(parameters.vcm2BaseHeight() - parameters.vcmSafeHeight());
    if(!res)
    {
        qInfo("leave place coms failed!");
        return false;
    }
    return res&&res_v;
}

bool SingleHeadMachineMaterialLoaderModule::ToLUTPosition()
{
    //check pickarm at place safe height
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->place_lens_base_pos.z);
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }

    //to pos above place lens
    double xpos = pick_arm->place_lens_base_pos.x;
    double ypos = pick_arm->place_lens_base_pos.y;
    double cpos = pick_arm->place_lens_base_pos.th;
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
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->place_lens_base_pos.z);
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }

    //to pos above place lens
    double xpos = pick_arm->place_lens_base_pos.x + pick_arm->lens_to_pr_distance.x();
    double ypos = pick_arm->place_lens_base_pos.y + pick_arm->lens_to_pr_distance.y();
    double cpos = pick_arm->place_lens_base_pos.th;
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
    target_pos.x = pick_arm->place_lens_base_pos.x + pick_arm->lens_to_pr_distance.x();
    target_pos.y = pick_arm->place_lens_base_pos.y + pick_arm->lens_to_pr_distance.y();
    target_pos.z = pick_arm->place_lens_base_pos.z;
    target_pos.v = parameters.vcm1BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->place_lens_base_pos.th;
    bool res = pick_arm->sucker1getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker1当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down to place lens
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->place_lens_base_pos.z);
    if(!res)
    {
        qInfo("z down to place lens failed!");
        return false;
    }
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->place_lens_base_pos.v ,pick_arm->place_lens_base_pos.z);
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
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_product_base_pos.z);
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }

    //to pos above pick product
    double xpos = pick_arm->pick_product_base_pos.x + pick_arm->lens_to_pr_distance.x();
    double ypos = pick_arm->pick_product_base_pos.y + pick_arm->lens_to_pr_distance.y();
    double cpos = pick_arm->pick_product_base_pos.th;
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
    target_pos.x = pick_arm->pick_product_base_pos.x + pick_arm->lens_to_pr_distance.x();
    target_pos.y = pick_arm->pick_product_base_pos.y + pick_arm->lens_to_pr_distance.y();
    target_pos.z = pick_arm->pick_product_base_pos.z;
    target_pos.v = parameters.vcm1BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->pick_product_base_pos.th;
    bool res = pick_arm->sucker1getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker1当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down for pick product
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->pick_product_base_pos.v + pick_arm->cmos_escape_offset.v,pick_arm->pick_product_base_pos.z);
    res = pick_arm->ZV1MoveSync(pick_arm->pick_product_base_pos.z,vcm1_pos);
    if(!res)
    {
        qInfo("sucker1 down for pick product failed!");
        return false;
    }

    //vcm1 soft landing to pick product
    res = pick_arm->vcm1SoftLanding(force,vcm1_pos + pick_arm->cmos_escape_offset.v);
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
    double xpos = pick_arm->pick_product_base_pos.x+pick_arm->lens_to_pr_distance.x()+ pick_arm->cmos_escape_offset.x;
    double ypos = pick_arm->pick_product_base_pos.y+pick_arm->lens_to_pr_distance.y()+ pick_arm->cmos_escape_offset.y;
    double cpos = pick_arm->pick_product_base_pos.th + pick_arm->cmos_escape_offset.th;
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
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->place_cmos_base_pos.z);
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
    double xpos = pick_arm->place_cmos_base_pos.x + pick_arm->lens_to_pr_distance.x();
    double ypos = pick_arm->place_cmos_base_pos.y + pick_arm->lens_to_pr_distance.y();
    double cpos = pick_arm->place_cmos_base_pos.th;
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
    target_pos.x = pick_arm->place_cmos_base_pos.x + pick_arm->lens_to_pr_distance.x();
    target_pos.y = pick_arm->place_cmos_base_pos.y + pick_arm->lens_to_pr_distance.y();
    target_pos.z = pick_arm->place_cmos_base_pos.z;
    target_pos.v = parameters.vcm1BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->place_cmos_base_pos.th;
    bool res = pick_arm->sucker1getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker1当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down for pick ng coms
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->place_cmos_base_pos.v + pick_arm->cmos_escape_offset.v,pick_arm->place_cmos_base_pos.z);
    res = pick_arm->ZV1MoveSync(pick_arm->place_cmos_base_pos.z,vcm1_pos);
    if(!res)
    {
        qInfo("sucker1 down for pick ng coms failed!");
        return false;
    }

    //vcm1 soft landing to pick ng coms
    res = pick_arm->vcm1SoftLanding(force, vcm1_pos + pick_arm->cmos_escape_offset.v);
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
    double xpos = pick_arm->place_cmos_base_pos.x + pick_arm->lens_to_pr_distance.x() + pick_arm->cmos_escape_offset.x;
    double ypos = pick_arm->place_cmos_base_pos.y + pick_arm->lens_to_pr_distance.y() + pick_arm->cmos_escape_offset.y;
    double cpos = pick_arm->place_cmos_base_pos.th + pick_arm->cmos_escape_offset.th;
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
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->place_lens_base_pos.z);
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
    //to pos above pick ng lens
    double xpos = pick_arm->place_lens_base_pos.x + pick_arm->cmos_to_pr_distance.x();
    double ypos = pick_arm->place_lens_base_pos.y + pick_arm->cmos_to_pr_distance.y();
    double cpos = pick_arm->place_lens_base_pos.th;
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
    target_pos.x = pick_arm->place_lens_base_pos.x + pick_arm->cmos_to_pr_distance.x();
    target_pos.y = pick_arm->place_lens_base_pos.y + pick_arm->cmos_to_pr_distance.y();
    target_pos.z = pick_arm->place_lens_base_pos.z;
    target_pos.v = parameters.vcm2BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->place_lens_base_pos.th;
    bool res = pick_arm->sucker2getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker2当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //z down to pick ng lens
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->place_lens_base_pos.z);
    if(!res)
    {
        qInfo("z down to pick ng lens failed!");
        return false;
    }
    //vcm2 soft landing to pick ng lens
    double vcm2_pos = pick_arm->vcm2GetMotorPos(pick_arm->place_lens_base_pos.v ,pick_arm->place_lens_base_pos.z);
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
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->place_product_base_pos.z);
    if(!res)
    {
        qInfo("check pickarm at pr height failed");
        return false;
    }
    //to pos above place product
    double xpos = pick_arm->place_product_base_pos.x + pick_arm->lens_to_pr_distance.x();
    double ypos = pick_arm->place_product_base_pos.y + pick_arm->lens_to_pr_distance.y();
    double cpos = pick_arm->place_product_base_pos.th;
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
    target_pos.x = pick_arm->place_product_base_pos.x + pick_arm->lens_to_pr_distance.x();
    target_pos.y = pick_arm->place_product_base_pos.y + pick_arm->lens_to_pr_distance.y();
    target_pos.z = pick_arm->place_product_base_pos.z;
    target_pos.v = parameters.vcm1BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->place_product_base_pos.th;
    bool res = pick_arm->sucker1getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker1当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }
    //down to place ng cmos
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->place_product_base_pos.z);
    if(!res)
    {
        qInfo("z down to place product failed!");
        return false;
    }
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->place_product_base_pos.v ,pick_arm->place_product_base_pos.z);
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
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_cmos_base_pos.z);
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
    //to pos above place ng coms
    qInfo("place used cmos x:%f y:%f",pick_arm->pick_cmos_base_pos.x,pick_arm->pick_cmos_base_pos.y);
    double xpos = pick_arm->pick_cmos_base_pos.x + pick_arm->lens_to_pr_distance.x();
    double ypos = pick_arm->pick_cmos_base_pos.y + pick_arm->lens_to_pr_distance.y();
    double cpos = pick_arm->pick_cmos_base_pos.th;
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
    target_pos.x = pick_arm->pick_cmos_base_pos.x + pick_arm->lens_to_pr_distance.x();
    target_pos.y = pick_arm->pick_cmos_base_pos.y + pick_arm->lens_to_pr_distance.y();
    target_pos.z = pick_arm->pick_cmos_base_pos.z;
    target_pos.v = parameters.vcm1BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->pick_cmos_base_pos.th;
    bool res = pick_arm->sucker1getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker1当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down to place ng cmos
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->pick_cmos_base_pos.z);
    if(!res)
    {
        qInfo("z down to place ng cmos failed!");
        return false;
    }
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->pick_cmos_base_pos.v ,pick_arm->pick_cmos_base_pos.z);
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
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_lens_base_pos.z);
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
    //to pos above pick ng coms
    double xpos = pick_arm->pick_lens_base_pos.x + pick_arm->cmos_to_pr_distance.x();
    double ypos = pick_arm->pick_lens_base_pos.y + pick_arm->cmos_to_pr_distance.y();
    double cpos = pick_arm->pick_lens_base_pos.th;
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
    target_pos.x = pick_arm->pick_lens_base_pos.x + pick_arm->cmos_to_pr_distance.x();
    target_pos.y = pick_arm->pick_lens_base_pos.y + pick_arm->cmos_to_pr_distance.y();
    target_pos.z = pick_arm->pick_lens_base_pos.z;
    target_pos.v = parameters.vcm2BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->pick_lens_base_pos.th;
    bool res = pick_arm->sucker2getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker2当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down to place ng cmos
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->pick_lens_base_pos.z);
    if(!res)
    {
        qInfo("z down to place ng cmos failed!");
        return false;
    }
    double vcm2_pos = pick_arm->vcm2GetMotorPos(pick_arm->pick_lens_base_pos.v ,pick_arm->pick_lens_base_pos.z);
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
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_lens_base_pos.z);
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
    //to pos above place ng coms
    double xpos = pick_arm->pick_lens_base_pos.x + pick_arm->lens_to_pr_distance.x();
    double ypos = pick_arm->pick_lens_base_pos.y + pick_arm->lens_to_pr_distance.y();
    double cpos = pick_arm->pick_lens_base_pos.th;
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
    target_pos.x = pick_arm->pick_lens_base_pos.x + pick_arm->lens_to_pr_distance.x();
    target_pos.y = pick_arm->pick_lens_base_pos.y + pick_arm->lens_to_pr_distance.y();
    target_pos.z = pick_arm->pick_lens_base_pos.z;
    target_pos.v = parameters.vcm1BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->pick_lens_base_pos.th;
    bool res = pick_arm->sucker1getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker1当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down to place ng cmos
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->pick_lens_base_pos.z);
    if(!res)
    {
        qInfo("z down to place ng cmos failed!");
        return false;
    }
    double vcm1_pos = pick_arm->vcm1GetMotorPos(pick_arm->pick_lens_base_pos.v ,pick_arm->pick_lens_base_pos.z);
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
    bool res = pick_arm->CheckZV1V2AboveSafeHeight(pick_arm->pick_cmos_base_pos.z);
    if(!res)
    {
        qInfo("check pickarm at place safe height failed");
        return false;
    }
    //to pos above pick ng coms
    double xpos = pick_arm->pick_cmos_base_pos.x + pick_arm->cmos_to_pr_distance.x();
    double ypos = pick_arm->pick_cmos_base_pos.y + pick_arm->cmos_to_pr_distance.y();
    double cpos = pick_arm->pick_cmos_base_pos.th;
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
    target_pos.x = pick_arm->pick_cmos_base_pos.x + pick_arm->cmos_to_pr_distance.x();
    target_pos.y = pick_arm->pick_cmos_base_pos.y + pick_arm->cmos_to_pr_distance.y();
    target_pos.z = pick_arm->pick_cmos_base_pos.z;
    target_pos.v = parameters.vcm2BaseHeight() - parameters.vcmSafeHeight();
    target_pos.th = pick_arm->pick_cmos_base_pos.th;
    bool res = pick_arm->sucker2getCurrentPos().isAbovePos(target_pos);
    if(!res)
    {
        qInfo("sucker2当前電機位置不在目標位置（%f,%f,%f,%f,%f）上方",target_pos.x,target_pos.y,target_pos.z,target_pos.v,target_pos.th);
        return false;
    }

    //down to place ng cmos
    res = pick_arm->motor_z->MoveToPosSync(pick_arm->pick_cmos_base_pos.z);
    if(!res)
    {
        qInfo("z down to place ng cmos failed!");
        return false;
    }
    double vcm2_pos = pick_arm->vcm2GetMotorPos(pick_arm->pick_cmos_base_pos.v ,pick_arm->pick_cmos_base_pos.z);
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
    res = pick_arm->motor_x->StepMoveSync(pick_arm->cmos_to_pr_distance.x());
    if(!res) return false;
    res = pick_arm->motor_y->StepMoveSync(pick_arm->cmos_to_pr_distance.y());
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::MoveCam2SensorPicker()
{
    bool res = pick_arm->LiftToSafeHeight();
    if(!res) return false;
    res = pick_arm->motor_x->StepMoveSync(-pick_arm->cmos_to_pr_distance.x());
    if(!res) return false;
    res = pick_arm->motor_y->StepMoveSync(-pick_arm->cmos_to_pr_distance.y());
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::MoveLensPikcer2Cam()
{
    bool res = pick_arm->LiftToSafeHeight();
    if(!res) return false;
    res = pick_arm->motor_x->StepMoveSync(pick_arm->lens_to_pr_distance.x());
    if(!res) return false;
    res = pick_arm->motor_y->StepMoveSync(pick_arm->lens_to_pr_distance.y());
    return res;
}

bool SingleHeadMachineMaterialLoaderModule::MoveCam2LensPicker()
{
    bool res = pick_arm->LiftToSafeHeight();
    if(!res) return false;
    res = pick_arm->motor_x->StepMoveSync(-pick_arm->lens_to_pr_distance.x());
    if(!res) return false;
    res = pick_arm->motor_y->StepMoveSync(-pick_arm->lens_to_pr_distance.y());
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

}
