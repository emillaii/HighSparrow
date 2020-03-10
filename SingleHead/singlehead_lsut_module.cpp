#include "SingleHead/singlehead_lsut_module.h"

SingleheadLSutModule::SingleheadLSutModule(QString name, QObject * parent) : ThreadWorkerBase (name)
{
    Q_UNUSED(parent);
}

void SingleheadLSutModule::Init(MaterialCarrier *sut_carrier,
                                VisionLocation *downlook_location,
                                VisionLocation *mushroom_location,
                                VisionLocation * gripper_location,
                                XtVacuum *sutVacuum,
                                XtVacuum *lutVacuum,
                                XtGeneralOutput *pogopin,
                                AAHeadModule * aa_head,
                                LSutState* lsutState)
{
    qInfo("SingleheadLSutModule Init");
    this->aa_head = aa_head;
    this->sut_carrier = sut_carrier;
    this->lsutState = lsutState;
    this->vision_downlook_location = downlook_location;
    this->vision_mushroom_location = mushroom_location;
    this->vision_uplook_location = gripper_location;
    this->vacuum_sut = sutVacuum;
    this->vacuum_lut = lutVacuum;
    this->pogopin = pogopin;
}

void SingleheadLSutModule::loadParams(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;

    temp_map.insert("LSUT_PARAMS",&parameters);
    //    temp_map.insert("load_position",&load_position);
    temp_map.insert("load_sensor_position",&load_sensor_position);
    temp_map.insert("load_lens_position",&load_lens_position);
    temp_map.insert("downlook_position",&downlook_position);
    temp_map.insert("mushroom_position",&mushroom_position);
    temp_map.insert("uplook_position",&uplook_position);
    temp_map.insert("calibration_position",&calibration_position);
    temp_map.insert("safety_position", &safety_position);
    temp_map.insert("pick_lens_position",&pick_lens_position);
    temp_map.insert("unpick_lens_position",&unpick_lens_position);
    temp_map.insert("lens_offset",&lens_offset);
    PropertyBase::loadJsonConfig(file_name,temp_map);
}

void SingleheadLSutModule::saveParams(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LSUT_PARAMS",&parameters);
    //    temp_map.insert("load_position",&load_position);
    temp_map.insert("load_sensor_position",&load_sensor_position);
    temp_map.insert("load_lens_position",&load_lens_position);
    temp_map.insert("downlook_position",&downlook_position);
    temp_map.insert("mushroom_position",&mushroom_position);
    temp_map.insert("uplook_position",&uplook_position);
    temp_map.insert("calibration_position",&calibration_position);
    temp_map.insert("safety_position",&safety_position);
    temp_map.insert("pick_lens_position",&pick_lens_position);
    temp_map.insert("unpick_lens_position",&unpick_lens_position);
    temp_map.insert("lens_offset",&lens_offset);
    PropertyBase::saveJsonConfig(file_name,temp_map);
}

void SingleheadLSutModule::receiveLoadMaterialFinishResponse()
{
    QMutexLocker tmpLocker(&locker);
    qInfo("Receive material from loader");
    pogopin->Set(true);
    lsutState->setWaitLoading(false);
    lsutState->setWaitAAProcess(true);
    qInfo(QString(tr("emit start aa process request")).toStdString().c_str());
    emit sendStartAAProcessRequestSignal(lsutState->currentSensorIndex(), lsutState->currentLensIndex(), lsutState->aaHeadHasLens());
}

void SingleheadLSutModule::receiveAAProcessFinishResponse(bool has_ng_sensor, bool has_ng_lens, bool has_product, bool has_ng_product, int productOrSensorIndex, int lensIndex)
{
    QMutexLocker tmpLocker(&locker);
    currentProductIndex = productOrSensorIndex;
    currentLensIndex = lensIndex;

    qInfo("Receive AA process response has_ng_sensor: %d has_ng_lens: %d has_product: %d has_ng_product: %d",
          has_ng_sensor, has_ng_lens, has_product, has_ng_product);

    bool needCheckWhereIsTheOkLens = false;  //检查OkLens在LUT上还是在夹爪上

    lsutState->setHasProduct(has_product || has_ng_product);
    lsutState->setIsProductOk(has_product && !has_ng_product);
    if(has_product || has_ng_product)
    {
        lsutState->setLutHasLens(false);
        lsutState->setSutHasSensor(false);
        lsutState->setLutHasNgLens(false);
        lsutState->setSutHasNgSensor(false);
        lsutState->setAAHeadHasLens(false);
    }
    else {
        lsutState->setSutHasNgSensor(has_ng_sensor);
        lsutState->setSutHasSensor(!has_ng_sensor);

        if(has_ng_lens)
        {
            lsutState->setLutHasLens(false);
            this->unpickLens();
//            sut_carrier->Move_SZ_SX_Y_X_Z_Sync(-2, this->unpick_lens_position.Y(), this->unpick_lens_position.Z());
//            aa_head->openGripper();
//            QThread::msleep(200);
            lsutState->setLutHasNgLens(true);
            lsutState->setAAHeadHasLens(false);
        }
        else {
            lsutState->setLutHasNgLens(false);
            needCheckWhereIsTheOkLens = true;
        }
    }

    pogopin->Set(false);
    moveToLoadSensorPosition();

    if(needCheckWhereIsTheOkLens)
    {
        if(!vacuum_lut->Set(true, true, VACUUM_FINISH_DELAY, 300))
        {
            vacuum_lut->Set(false, false);
            lsutState->setLutHasLens(false);
        }
        else
        {
            lsutState->setLutHasLens(true);
        }
        qInfo("AA finished, Lut has ok Lens: %d", lsutState->lutHasLens());

        lsutState->setAAHeadHasLens(!lsutState->lutHasLens());
    }

    lsutState->setWaitAAProcess(false);
    lsutState->setWaitLoading(true);

    emit sendLoadMaterialRequestSignal(true, currentProductIndex, currentLensIndex);
}

void SingleheadLSutModule::run(){
    is_run = true;
    while (is_run) {
        if(locker.tryLock(200))
        {
            if(!lsutState->waitLoading() && !lsutState->waitAAProcess())
            {
                qDebug() << "LSUT is idle. So move to load position!";
                pogopin->Set(false);
                moveToLoadSensorPosition();

                lsutState->setWaitAAProcess(false);
                lsutState->setWaitLoading(true);

                emit sendLoadMaterialRequestSignal(true, currentProductIndex, currentLensIndex);
            }
            locker.unlock();
        }
        QThread::msleep(200);
    }
}

void SingleheadLSutModule::startWork(int run_mode)
{
    if (run_mode == RunMode::Normal) {
        qInfo("LSUT start work");
        is_run = true;
        while(is_run) {
            run();
        }
    }
}

void SingleheadLSutModule::stopWork(bool wait_finish)
{
    qInfo("LSUT module stop Work");
    is_run = false;
}

void SingleheadLSutModule::resetLogic()
{
    qInfo("resetLogic is called");
    //    states.setWaitLens(false);
    lsutState->setAAHeadHasLens(false);
    lsutState->setLutHasLens(false);
    lsutState->setLutHasNgLens(false);
    lsutState->setSutHasSensor(false);
    lsutState->setSutHasNgSensor(false);
    lsutState->setHasProduct(false);
    lsutState->setIsProductOk(false);
    lsutState->setWaitLoading(false);
    lsutState->setWaitAAProcess(false);
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
    else if(cmd % temp_value == HandlePosition::MOVE_TO_UPLOOK_POSITION) {
        qInfo("LSUT move to gripper position, cmd: %d", MOVE_TO_UPLOOK_POSITION);
        result = moveToUplookPosition(true);
    }
    else if (cmd % temp_value == HandlePosition::MOVE_TO_LOAD_SENSOR_POSITION) {
        qInfo("LSUT move to load sensor position, cmd: %d", MOVE_TO_LOAD_SENSOR_POSITION);
        result = moveToLoadSensorPosition(true);
    }
    else if (cmd % temp_value == HandlePosition::MOVE_TO_PR_POSITION) {
        qInfo("LSUT move to sensor PR position, cmd: %d", MOVE_TO_PR_POSITION);
        result = moveToPRPosition(true);
    }
    else if (cmd % temp_value == HandlePosition::MOVE_TO_PICK_LENS_POSITION) {
        qInfo("LSUT move to safety position, cmd: %d", MOVE_TO_PICK_LENS_POSITION);
        result = moveToPickLensPosition(true);
    }
    else if (cmd % temp_value == HandlePosition::MOVE_TO_UNPICK_LENS_POSITION) {
        qInfo("LSUT move to safety position, cmd: %d", MOVE_TO_UNPICK_LENS_POSITION);
        result = moveToUnpickLensPosition(true);
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

    if (cmd % temp_value == HandleToWorkPos::LENS_TO_GRIPPER_CENTER) {
        qInfo("Move lens to gripper center, cmd: %d", LENS_TO_GRIPPER_CENTER);
        result = moveLensToGripperCenter();
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
    else if (cmd % temp_value == HandlePick::GRAB_LENS_TO_GRIPPER) {
        qInfo("Grab lens to gripper, cmd: %d", GRAB_LENS_TO_GRIPPER);
        result = gripLens();
    }
    else if (cmd % temp_value == HandlePick::UNPICK_LENS_TO_LUT) {
        qInfo("UnPick lens to lut, cmd: %d", UNPICK_LENS_TO_LUT);
        result = unpickLens();
    }
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

bool SingleheadLSutModule::moveToDownlookPR(PrOffset &offset,bool close_lighting,bool check_autochthonous)
{

    vision_downlook_location->OpenLight();
    bool result = moveToPRPosition(check_autochthonous);
    QThread::msleep(200);
    if(result)
    {
        result = vision_downlook_location->performPR(offset);
    }
    if(close_lighting)
        vision_downlook_location->CloseLight();
    return result;
}

bool SingleheadLSutModule::moveToUplookPR(PrOffset &offset, bool close_lighting, bool check_autochthonous)
{
    vision_uplook_location->OpenLight();
    bool result = moveToUplookPosition(check_autochthonous);
    QThread::msleep(200);
    if(result)
    {
        result = vision_uplook_location->performPR(offset);
    }
    if(close_lighting)
        vision_uplook_location->CloseLight();
    return result;
}

bool SingleheadLSutModule::moveToDownlookSaveImage(QString filename)
{
    vision_downlook_location->OpenLight();
    bool result = moveToPRPosition(true);
    QThread::msleep(300);
    if(result)
    {
        this->vision_downlook_location->saveImage(filename);
    }
    return result;
}

bool SingleheadLSutModule::moveToMushroomPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToMushroomPos");

    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(mushroom_position.X(),mushroom_position.Y(),mushroom_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToUplookPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToGripperPosition");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(uplook_position.X(),uplook_position.Y(),uplook_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToLoadSensorPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToLoadPos");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(load_sensor_position.X(),load_sensor_position.Y(),load_sensor_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToPRPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToPRPosition");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(downlook_position.X(),downlook_position.Y(),downlook_position.Z(),check_autochthonous);
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

bool SingleheadLSutModule::moveLensToGripperCenter()
{
    qInfo("moveLensToGripper position");
    return sut_carrier->StepMove_XY_Sync(lens_offset.X()-pr_offset.X, lens_offset.Y()-pr_offset.Y);

}

bool SingleheadLSutModule::stepMove_XY_Sync(double x,double y)
{
    qInfo("Move to target position, X: %f, Y: %f", x, y);
    return sut_carrier->StepMove_XY_Sync(x,y);
}

bool SingleheadLSutModule::moveToZPos(double z)
{
    qInfo("Motor Z : %s Target : %f", sut_carrier->motor_z->Name().toStdString().c_str(),z);
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

bool SingleheadLSutModule::lensGripperMeasureHight()
{
    qInfo("Measure height from lens to gripper with low speed");
    if (sut_carrier->ZSerchByForce(parameters.LensSoftlandingVel(), parameters.LensSoftlandingForce(), true))
    {
        QThread::msleep(100);
        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"Use this Z height:%1").arg(sut_carrier->GetSoftladngPosition()))){
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

bool SingleheadLSutModule::gripLens()
{
    qInfo("GripLens Start SUT is going to zOffset: %f", parameters.ZOffset());
    aa_head->openGripper();
    aa_head->moveToPikLensPositionAsync();
    if(!moveToPickLensPosition() || !aa_head->waitArrivedPickLesPosition()) {
        qInfo("Move to pick lens pos fail");
        this->vacuum_lut->Set(false, false);  //First do not check the state.
        aa_head->closeGripper();
        return false;
    }
    if(parameters.delayBeforGripLens() > 0)
    {
        QThread::msleep(parameters.delayBeforGripLens());
    }
    this->vacuum_lut->Set(false, false);  //First do not check the state.
    aa_head->closeGripper();
    if(parameters.delayAfterGripLens() > 0)
    {
        QThread::msleep(parameters.delayAfterGripLens());
    }
    qInfo("GripLens finished");
    return true;
}

bool SingleheadLSutModule::unpickLens()
{
    qInfo("Unpick Lens start");
    if (!moveToUnpickLensPosition(true)) {
        qInfo("Move to unpick lens position fail");
        return false;
    }
    if (!this->vacuum_lut->Set(true)) {
        qInfo("vacuum_lut set to true error");
        return false;
    }
    aa_head->openGripper();
    moveToLoadSensorPosition();
    qInfo("Unpick Lens finished");

    return true;
}
