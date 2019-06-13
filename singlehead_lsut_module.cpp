#include "singlehead_lsut_module.h"

SingleheadLSutModule::SingleheadLSutModule(QString name, QObject * parent) : ThreadWorkerBase (name)
{
    Q_UNUSED(parent);
    connect(this,&SingleheadLSutModule::sendHandlingOperation,this,&SingleheadLSutModule::performHandlingOperation);
}

void SingleheadLSutModule::Init(MaterialCarrier *_sut_carrier, XtCylinder *_pogopin)
{
    qInfo("SingleheadLSutModule Init");
    this->sut_carrier = _sut_carrier;
    this->pogopin = _pogopin;
}

void SingleheadLSutModule::loadParams(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LSUT_PARAMS",&parameters);
    temp_map.insert("load_position",&load_position);
    temp_map.insert("downlook_position",&downlook_position);
    temp_map.insert("mushroom_position",&mushroom_position);
    temp_map.insert("calibration_position",&calibration_position);
    temp_map.insert("safety_position", &safety_position);
    PropertyBase::loadJsonConfig(file_name,temp_map);
}

void SingleheadLSutModule::saveParams(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LSUT_PARAMS",&parameters);
    temp_map.insert("load_position",&load_position);
    temp_map.insert("downlook_position",&downlook_position);
    temp_map.insert("mushroom_position",&mushroom_position);
    temp_map.insert("calibration_position",&calibration_position);
    temp_map.insert("safety_position",&safety_position);
    PropertyBase::saveJsonConfig(file_name,temp_map);
}

void SingleheadLSutModule::startWork(int run_mode)
{
    is_run = true;
    while(is_run) {
        qInfo("I am running ");
        QThread::msleep(1000);
    }
}

void SingleheadLSutModule::stopWork(bool wait_finish)
{
    qInfo("Stop Work");
    is_run = false;
}

void SingleheadLSutModule::resetLogic()
{

}

void SingleheadLSutModule::performHandling(int cmd)
{
    emit sendHandlingOperation(cmd);
    qInfo("emit performHandling %d",cmd);
}

void SingleheadLSutModule::performHandlingOperation(int cmd)
{
    if (cmd == MOVE_TO_MUSHROOM_POSITION) {
        moveToMushroomPosition(true);
    } else if (cmd == MOVE_TO_LOAD_POSITION) {
        moveToLoadPosition(true);
    }
    int temp_value = 10;
    if(cmd % temp_value == HandleMoveToPosition::MOVE_TO_MUSHROOM_POSITION) {
        moveToMushroomPosition(true);
    }
    else if (cmd % temp_value == HandleMoveToPosition::MOVE_TO_LOAD_POSITION) {
        moveToLoadPosition(true);
    }
    else if (cmd % temp_value == HandleMoveToPosition::MOVE_TO_CALIBRATION_POSITION) {
        moveToCalibrationPosition(true);
    }
    else if (cmd % temp_value == HandleMoveToPosition::MOVE_TO_PR_POSITION) {
        moveToPRPosition(true);
    }
    else if (cmd % temp_value == HandleMoveToPosition::MOVE_TO_SAFETY_POSITION) {
        moveToSafetyPosition(true);
    }
}

bool SingleheadLSutModule::moveToMushroomPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToMushroomPos");
    bool ret = sut_carrier->motor_y->MoveToPosSync(mushroom_position.Y());
    qInfo(ret?"true":"false");
    return true;
    //return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(mushroom_position.X(),mushroom_position.Y(),mushroom_position.Z(),check_autochthonous);
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
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(calibration_position.X(),calibration_position.Y(),calibration_position.Z(),check_autochthonous);
}

bool SingleheadLSutModule::moveToSafetyPosition(bool check_autochthonous)
{
    qInfo("SUT module moveToSafetyPos");
    return sut_carrier->Move_SZ_SX_Y_X_Z_Sync(safety_position.X(),safety_position.Y(),safety_position.Z(),check_autochthonous);
}
