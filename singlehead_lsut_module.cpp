#include "singlehead_lsut_module.h"

SingleheadLSutModule::SingleheadLSutModule(QString name, QObject * parent) : ThreadWorkerBase (name)
{
    Q_UNUSED(parent);
    connect(this,&SingleheadLSutModule::sendHandlingOperation,this,&SingleheadLSutModule::performHandlingOperation);
}

void SingleheadLSutModule::Init(MaterialCarrier *_sut_carrier,
                                VisionLocation *_downlook_location,
                                VisionLocation *_updownlook_down_location,
                                VisionLocation *_updownlook_up_location,
                                VisionLocation *_mushroom_location,
                                VisionLocation * _gripper_location,
                                XtVacuum *_sutVacuum,
                                XtVacuum *_lutVacuum,
                                XtCylinder *_pogopin)
{
    qInfo("SingleheadLSutModule Init");
    this->sut_carrier = _sut_carrier;
    this->vision_downlook_location = _downlook_location;
    this->vision_updownlook_down_location = _updownlook_down_location;
    this->vision_updownlook_up_location = _updownlook_up_location;
    this->vision_mushroom_location = _mushroom_location;
    this->vision_gripper_location = _gripper_location;
    this->vacuum_sut = _sutVacuum;
    this->vacuum_lut = _lutVacuum;
    this->pogopin = _pogopin;
}

void SingleheadLSutModule::loadParams(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LSUT_PARAMS",&parameters);
    temp_map.insert("load_position",&load_position);
    temp_map.insert("downlook_position",&downlook_position);
    temp_map.insert("mushroom_position",&mushroom_position);
    temp_map.insert("gripper_position",&gripper_position);
    temp_map.insert("calibration_position",&calibration_position);
    temp_map.insert("safety_position", &safety_position);
    temp_map.insert("pick_lens_position",&pick_lens_position);
    temp_map.insert("unpick_lens_position",&unpick_lens_position);
    temp_map.insert("lens_offset",&lens_offset);
    temp_map.insert("sensor_offset",&sensor_offset);
    temp_map.insert("up_downlook_offset", &up_downlook_offset);
    PropertyBase::loadJsonConfig(file_name,temp_map);
}

void SingleheadLSutModule::saveParams(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LSUT_PARAMS",&parameters);
    temp_map.insert("load_position",&load_position);
    temp_map.insert("downlook_position",&downlook_position);
    temp_map.insert("mushroom_position",&mushroom_position);
    temp_map.insert("gripper_position",&gripper_position);
    temp_map.insert("calibration_position",&calibration_position);
    temp_map.insert("safety_position",&safety_position);
    temp_map.insert("pick_lens_position",&pick_lens_position);
    temp_map.insert("unpick_lens_position",&unpick_lens_position);
    temp_map.insert("lens_offset",&lens_offset);
    temp_map.insert("sensor_offset",&sensor_offset);
    temp_map.insert("up_downlook_offset", &up_downlook_offset);
    PropertyBase::saveJsonConfig(file_name,temp_map);
}

// TODO
void SingleheadLSutModule::run(bool isProduct)
{

}


void SingleheadLSutModule::startWork(int run_mode)
{
    qInfo("LSUT start work");
    is_run = true;
    while(is_run) {
        run(true);
    }
}

void SingleheadLSutModule::stopWork(bool wait_finish)
{
    qInfo("LSUT module stop Work");
    is_run = false;
}

void SingleheadLSutModule::resetLogic()
{
    if(is_run)
    {
        return;
    }
    // TODO Reset state to false
}

void SingleheadLSutModule::performHandling(int cmd)
{
    emit sendHandlingOperation(cmd);
    qInfo("emit performHandling %d",cmd);
}

void SingleheadLSutModule::performHandlingOperation(int cmd)
{
    qInfo("Single head LSUT performHandling %d",cmd);
    bool result;

    int temp_value = 10;
    if(cmd % temp_value == HandlePosition::MOVE_TO_MUSHROOM_POSITION) {
        qInfo("LSUT move to mushroom position, cmd: %d", MOVE_TO_MUSHROOM_POSITION);
        result = moveToMushroomPosition(true);
    }
    else if(cmd % temp_value == HandlePosition::MOVE_TO_GRIPPER_POSITION) {
        qInfo("LSUT move to gripper position, cmd: %d", MOVE_TO_GRIPPER_POSITION);
        result = moveToGripperPosition(true);
    }
    else if (cmd % temp_value == HandlePosition::MOVE_TO_LOAD_POSITION) {
        qInfo("LSUT move to load position, cmd: %d", MOVE_TO_LOAD_POSITION);
        result = moveToLoadPosition(true);
    }
    else if (cmd % temp_value == HandlePosition::MOVE_TO_CALIBRATION_POSITION) {
        qInfo("LSUT move to updown downlook calibration position, cmd: %d", MOVE_TO_CALIBRATION_POSITION);
        result = moveToCalibrationPosition(true);
    }
    else if (cmd % temp_value == HandlePosition::MOVE_TO_PR_POSITION) {
        qInfo("LSUT move to sensor PR position, cmd: %d", MOVE_TO_PR_POSITION);
        result = moveToPRPosition(true);
    }
    else if (cmd % temp_value == HandlePosition::MOVE_TO_SAFETY_POSITION) {
        qInfo("LSUT move to safety position, cmd: %d", MOVE_TO_SAFETY_POSITION);
        result = moveToSafetyPosition(true);
    }
    else {
        result = true;
    }
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock, GetCurrentError());
        return;
    }
    cmd =cmd/temp_value*temp_value;

    temp_value = 100;
    if(cmd % temp_value == HandlePR::RESET_PR) {
        qInfo("Reset PR result,cmd: %d", RESET_PR);
        pr_offset.ReSet();
    }
    else if (cmd % temp_value == HandlePR::DOWNLOOK_SENSOR_PR) {
        qInfo("LSUT perform sensor PR, cmd: %d", DOWNLOOK_SENSOR_PR);
        result = performDownlookSensorPR();
    }
    else if (cmd % temp_value == HandlePR::UPLOOK_LENS_PR) {
        qInfo("LSUT perform lens PR, cmd: %d", UPLOOK_LENS_PR);
        result = performUplookLensPR();
    }
    else if (cmd % temp_value == HandlePR::UPLOOK_GRIPPER_PR) {
        qInfo("LSUT perform gripper PR, cmd: %d", UPLOOK_GRIPPER_PR);
        result = performUplookGripperPR();
    }
    else {
        result = true;
    }
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock, GetCurrentError());
        return;
    }
    cmd =cmd/temp_value*temp_value;

    temp_value = 1000;
    if(cmd % temp_value == HandleToWorkPos::SENSOR_TO_BOND) {
        qInfo("Move sensor to bond, cmd: %d", SENSOR_TO_BOND);
        //MoveSenorToBond();
    }
    else if (cmd % temp_value == HandleToWorkPos::LENS_TO_GRIPPER) {
        qInfo("Move lens to gripper, cmd: %d", LENS_TO_GRIPPER);
        result = moveLensToGripper();
    }
    else {
        result = true;
    }
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock, GetCurrentError());
        return;
    }
    cmd =cmd/temp_value*temp_value;

    temp_value = 10000;
    if (cmd % temp_value == HandlePick::LENS_GRIPPER_MEASURE_HEIGHT) {
        qInfo("Measure height from lens to gripper, cmd: %d", LENS_GRIPPER_MEASURE_HEIGHT);
        result = lensGripperMeasureHight();
    }
//    else if (cmd % temp_value == HandlePick::GRAB_LENS_TO_GRIPPER) {
//        qInfo("Grab lens to gripper, cmd: %d", GRAB_LENS_TO_GRIPPER);
//        result = grabLensToGripper();
//    }
    else {
        result = true;
    }
    if(!result)
    {
        sendAlarmMessage(ErrorLevel::TipNonblock, GetCurrentError());
        return;
    }

    return;
}

bool SingleheadLSutModule::moveToMushroomPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToMushroomPos");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(mushroom_position.X(),mushroom_position.Y(),mushroom_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToGripperPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToGripperPosition");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(gripper_position.X(),gripper_position.Y(),gripper_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToLoadPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToLoadPos");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(load_position.X(),load_position.Y(),load_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToPRPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToLoadPos");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(downlook_position.X(),downlook_position.Y(),downlook_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToCalibrationPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToCalibrationPos");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(calibration_position.X(), calibration_position.Y(), calibration_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToSafetyPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToSafetyPos");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(safety_position.X(),safety_position.Y(),safety_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToPickLensPosition(bool check_autochthonous)
{
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(pick_lens_position.X(),pick_lens_position.Y(),pick_lens_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToUnpickLensPosition(bool check_autochthonous)
{
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(unpick_lens_position.X(),unpick_lens_position.Y(),unpick_lens_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::performDownlookSensorPR()
{
    return vision_downlook_location->performPR(pr_offset);
}

// Do PR when LENS in gripper, so it should be in gripper vision location?
bool SingleheadLSutModule::performUplookLensPR()
{
    return vision_mushroom_location->performPR(pr_offset);
}

bool SingleheadLSutModule::performUplookGripperPR()
{
    return vision_gripper_location->performPR(pr_offset);
}

bool SingleheadLSutModule::moveToLensWorkPos()
{
    return sut_carrier->StepMove_XY_Sync(lens_offset.X()-pr_offset.X, lens_offset.Y()-pr_offset.Y);
}

bool SingleheadLSutModule::moveToSensorWorkPos()
{
    return sut_carrier->StepMove_XY_Sync(sensor_offset.X()-pr_offset.X, sensor_offset.Y()-pr_offset.Y);
}

bool SingleheadLSutModule::stepMove_XY_Sync(double x,double y)
{
    qInfo("Move to target position, X: %f, Y: %f", x, y);
    return sut_carrier->StepMove_XY_Sync(x,y);
}

bool SingleheadLSutModule::stepMove_Z_Sync(double step_z)
{
    qInfo("Target Z position: %f", step_z);
    return sut_carrier->StepMove_Z(step_z);
}

bool SingleheadLSutModule::moveToZPos(double z)
{
    qInfo("Targert : %f",z);
    return sut_carrier->Move_Z_Sync(z);
}

void SingleheadLSutModule::recordCurrentPos()
{
    qInfo("Record current position");
    record_position = sut_carrier->GetFeedBackPos();
}

bool SingleheadLSutModule::movetoRecordPos(bool check_autochthonous)
{
    qInfo("Move to record position X: %f, Y: %f, Z: %f", record_position.X, record_position.Y, record_position.Z);
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(record_position.X,record_position.Y,record_position.Z,check_autochthonous);
}

bool SingleheadLSutModule::moveToCamPos(double pixel_x, double pixel_y, int upDownLook)
{
    bool ret = false;
    qInfo("Move to camera view position offset pixel_x: %f, pixel_y: %f", pixel_x, pixel_y);

    // Calculate mechanical position
    QPointF meth;
    if (upDownLook == 0)
    {
        qInfo("Use uplook gripper vision location");
        ret = vision_gripper_location->mapping->CalcMechDistanceFromPixelCenter(pixel_x, -pixel_y, meth);
    }
    else if (upDownLook == 1)
    {
        qInfo("Use downlook vision location");
        ret = vision_downlook_location->mapping->CalcMechDistanceFromPixelCenter(pixel_x, pixel_y, meth);
    }
    else
    {
        return false;
    }
    qInfo("Move to camera position meth_x: %f, meth_y: %f", meth.x(), meth.y());

    // Move motors
    ret = stepMove_XY_Sync(meth.x(), meth.y());
    return ret;
}

bool SingleheadLSutModule::moveLensToGripper()
{
    qInfo("Move lens to gripper");
    double offsetX = lens_offset.X() - pr_offset.X;
    double offsetY = lens_offset.Y() - pr_offset.Y;
    qInfo("moveLensToGripper offset:(%f, %f)", offsetX, offsetY);
    bool result = sut_carrier->StepMove_XY_Sync(offsetX, offsetY);
    if(result)
        AppendError(QString(u8"移动Lens去Gripper位置(step x %1,y %2)失败!").arg(offsetX).arg(offsetY));
    return  result;
}

bool SingleheadLSutModule::lensGripperMeasureHight()
{
    qInfo("Measure height from lens to gripper with low speed");
    if (sut_carrier->ZSerchByForce(parameters.LensSoftlandingVel(), parameters.LensSoftlandingForce(), true))
    {
        QThread::msleep(100);
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"是否应用此高度:%1").arg(sut_carrier->GetSoftladngPosition()))){
            return true;
        }
        parameters.setZOffset(sut_carrier->GetSoftladngPosition());
    }
    else
    {
        AppendError(QString(u8"Lens-Gripper测高失败"));
        return false;
    }
}
