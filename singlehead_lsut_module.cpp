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
    temp_map.insert("mushroom_position",&mushroom_positon);
    temp_map.insert("calibration_position",&calibration_position);
    PropertyBase::loadJsonConfig(file_name,temp_map);
}

void SingleheadLSutModule::saveParams(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LSUT_PARAMS",&parameters);
    temp_map.insert("load_position",&load_position);
    temp_map.insert("downlook_position",&downlook_position);
    temp_map.insert("mushroom_position",&mushroom_positon);
    temp_map.insert("calibration_position",&calibration_position);
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
        qInfo("Move to mushroom position");
    } else if (cmd == MOVE_TO_LOAD_POSITION) {
        qInfo("Move to load position");
    }
}



