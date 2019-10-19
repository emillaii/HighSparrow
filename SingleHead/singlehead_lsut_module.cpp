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
                                AAHeadModule * aa_head)
{
    qInfo("SingleheadLSutModule Init");
    this->aa_head = aa_head;
    this->sut_carrier = sut_carrier;
    this->vision_downlook_location = downlook_location;
    this->vision_mushroom_location = mushroom_location;
    this->vision_gripper_location = gripper_location;
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
    temp_map.insert("gripper_position",&gripper_position);
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
    temp_map.insert("gripper_position",&gripper_position);
    temp_map.insert("calibration_position",&calibration_position);
    temp_map.insert("safety_position",&safety_position);
    temp_map.insert("pick_lens_position",&pick_lens_position);
    temp_map.insert("unpick_lens_position",&unpick_lens_position);
    temp_map.insert("lens_offset",&lens_offset);
    PropertyBase::saveJsonConfig(file_name,temp_map);
}

void SingleheadLSutModule::receiveMaterialResponse(int sensor_index, int lens_index)
{
    qInfo("Receive material from loader with sensor_index: %d lens_index: %d", sensor_index, lens_index);
    pogopin->Set(true);
    if(sensor_index >= 0) {
        this->states.setSutHasSensor(true);
        this->states.setSutHasNgSensor(false);
    }
    if(lens_index >= 0) {
        this->states.setLutHasLens(true);
        this->states.setLutHasNgLens(false);
        //gripLens();    // Pick lens from lut to aa head
        //moveToPRPosition();
        //moveToGripperPosition();
    }
    this->states.setWaitLoading(false);

    //Start AA Process
    if (states.sutHasSensor() && !states.sutHasNgSensor() &&
        states.lutHasLens() && !states.lutHasNgLens())
    {
        states.setWaitAAProcess(true);
        QThread::msleep(100);// Wait for the while loop trapping in checking the aa process state
        qInfo("emit start aa process request");
        emit sendStartAAProcessRequest();
    }
}

void SingleheadLSutModule::receiveAAProcessResponse(bool has_ng_sensor, bool has_ng_lens, bool has_product, bool has_ng_product)
{
    qInfo("Receive AA process response has_ng_sensor: %d has_ng_lens: %d has_product: %d has_ng_product: %d",
          has_ng_sensor, has_ng_lens, has_product, has_ng_product);
    this->states.setSutHasNgSensor(has_ng_sensor);
    this->states.setLutHasNgLens(has_ng_lens);
    this->states.setHasProduct(has_product);
    this->states.setHasNgProduct(has_ng_product);
    if (has_ng_lens) {
        //unpickLens();   // unpick the lens from aa head to lut
        //Go to lens reject bin
        sut_carrier->Move_SZ_SX_Y_X_Z_Sync(30, this->unpick_lens_position.Y(), this->unpick_lens_position.Z());
        aa_head->openGripper();
        QThread::msleep(200);
    }
    this->states.setWaitAAProcess(false);
    pogopin->Set(false);
    moveToLoadSensorPosition();
    emit sendLoadMaterialRequest(states.allowLoadSensor(), states.allowLoadLens(),
                                 states.sutHasNgSensor(), states.lutHasNgLens(),
                                 states.hasProduct(), true);
}

void SingleheadLSutModule::run(bool has_material){
    is_run = true;
    while (is_run) {
        if (states.waitLoading() || states.waitAAProcess()) { //Waiting Material or AA Process
            if (!states.waitLoading()) { //Pickarm is very free
                states.setWaitLoading(true);
                states.setAllowLoadLens(true);
                states.setAllowLoadSensor(true);
                emit sendLoadMaterialRequest(states.allowLoadSensor(), states.allowLoadLens(),
                                             false, false, false, false);
            }
            QThread::msleep(100);
            continue;
        }
        if (!this->states.lutHasLens())          //LUT No lens
        {
            states.setWaitLoading(true);
            states.setAllowLoadLens(true);
            states.setLutHasNgLens(false);
            states.setLutHasLens(false);
            if(!is_run) break;
        }

        if (!this->states.sutHasSensor())
        {
            states.setWaitLoading(true);
            states.setAllowLoadSensor(true);
            states.setSutHasSensor(false);
            states.setSutHasNgSensor(false);
            if(!is_run) break;
        }

        if (states.allowLoadLens() || states.allowLoadSensor())
        {
            pogopin->Set(false);
            moveToLoadSensorPosition();
            states.setWaitLoading(true);
            emit sendLoadMaterialRequest(states.allowLoadSensor(), states.allowLoadLens(),
                                         states.sutHasNgSensor(), states.lutHasNgLens(),
                                         states.hasProduct(), true);
        }
    }
}

void SingleheadLSutModule::lens_logic()
{
}

void SingleheadLSutModule::sensor_logic()
{
}


void SingleheadLSutModule::startWork(int run_mode)
{
    if (run_mode == RunMode::Normal) {
        qInfo("LSUT start work");
        is_run = true;
        while(is_run) {
            run(true);
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
    states.setWaitLens(false);
    states.setLutHasLens(false);
    states.setSutHasSensor(false);
    states.setSutHasNgSensor(false);
    states.setAllowLoadSensor(false);
    states.setWaitLoading(false);
    states.setAllowLoadLens(false);
    states.setHasProduct(false);
    states.setLutHasNgLens(false);
    states.setHasNgProduct(false);
    states.setWaitAAProcess(false);
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
    else if (cmd % temp_value == HandlePosition::MOVE_TO_LOAD_SENSOR_POSITION) {
        qInfo("LSUT move to load sensor position, cmd: %d", MOVE_TO_LOAD_SENSOR_POSITION);
        result = moveToLoadSensorPosition(true);
    }
    else if (cmd % temp_value == HandlePosition::MOVE_TO_LOAD_LENS_POSITION) {
        qInfo("LSUT move to load lens position, cmd: %d", MOVE_TO_LOAD_LENS_POSITION);
        result = moveToLoadLensPosition(true);
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

   if (cmd % temp_value == HandleToWorkPos::LENS_TO_GRIPPER_CENTER) {
        qInfo("Move lens to gripper center, cmd: %d", LENS_TO_GRIPPER_CENTER);
        result = moveLensToGripperCenter();
    }
    else if (cmd % temp_value == HandleToWorkPos::LENS_TO_GRIPPER) {
        qInfo("Move lens to gripper, cmd: %d", LENS_TO_GRIPPER);
        result = moveLensToGripperPos();
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
    if(result)
    {
        result = vision_downlook_location->performPR(offset);
    }
    if(close_lighting)
        vision_downlook_location->CloseLight();
    return result;
}

bool SingleheadLSutModule::moveToDownlookSaveImage(QString filename)
{
    vision_downlook_location->OpenLight();
    bool result = moveToPRPosition(true);
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


bool SingleheadLSutModule::moveToGripperPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToGripperPosition");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(gripper_position.X(),gripper_position.Y(),gripper_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToLoadSensorPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToLoadPos");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(load_sensor_position.X(),load_sensor_position.Y(),load_sensor_position.Z(),check_autochthonous);
}
bool SingleheadLSutModule::moveToLoadLensPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToLoadPos");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(load_lens_position.X(),load_lens_position.Y(),load_lens_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToPRPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToPRPosition");
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

bool SingleheadLSutModule::stepMove_Z_Sync(double step_z)
{
    qInfo("Target Z position: %f", step_z);
    return sut_carrier->StepMove_Z(step_z);
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


bool SingleheadLSutModule::moveLensToGripperPos(double step_z)
{

    qInfo("moveLensToGripper position");
   return sut_carrier->StepMove_Z(parameters.ZOffset());

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
    if(!moveToPickLensPosition()) {
          qInfo("Move to pick lens pos fail");
          return false;
      }
    aa_head->closeGripper();
    this->vacuum_lut->Set(false, false);  //First do not check the state.
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
    qInfo("Unpick Lens finished");
    return true;
}