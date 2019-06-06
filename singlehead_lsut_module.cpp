#include "singlehead_lsut_module.h"

SingleheadLSutModule::SingleheadLSutModule(QString name, QObject * parent):ThreadWorkerBase (name)
{

}

void SingleheadLSutModule::Init(MaterialCarrier *_lut_carrier, MaterialCarrier *_sut_carrier, XtCylinder *_pogopin)
{
    this->lut_carrier = _lut_carrier;
    this->sut_carrier = _sut_carrier;
    this->pogopin = _pogopin;
}

void SingleheadLSutModule::loadParams(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LSUT_PARAMS",&parameters);
    temp_map.insert("load_uplook_position",&load_uplook_position);
    temp_map.insert("lpa_camera_to_picker_offset",&lpa_camera_to_picker_offset);
    temp_map.insert("lut_load_position",&lut_load_position);
    temp_map.insert("lut_downlook_load_position",&lut_downlook_load_position);
    temp_map.insert("lut_downlook_unload_position",&lut_downlook_unload_position);
    temp_map.insert("aa_updownlook_position",&aa_updownlook_position);
    temp_map.insert("aa_picklens_position",&aa_picklens_position);
    temp_map.insert("aa_unpicklens_position",&aa_unpicklens_position);
    temp_map.insert("aa_uplook_position",&aa_uplook_position);
    temp_map.insert("aa_mushroom_position",&aa_mushroom_position);
    temp_map.insert("sut_load_position",&sut_load_position);
    temp_map.insert("downlook_position",&downlook_position);
    temp_map.insert("tool_downlook_position",&tool_downlook_position);
    temp_map.insert("mushroom_positon",&mushroom_positon);
    temp_map.insert("tool_uplook_positon",&tool_uplook_positon);
    temp_map.insert("up_downlook_offset",&up_downlook_offset);
    PropertyBase::loadJsonConfig(file_name,temp_map);
}

void SingleheadLSutModule::saveParams(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LSUT_PARAMS",&parameters);
    temp_map.insert("load_uplook_position",&load_uplook_position);
    temp_map.insert("lpa_camera_to_picker_offset",&lpa_camera_to_picker_offset);
    temp_map.insert("lut_load_position",&lut_load_position);
    temp_map.insert("lut_downlook_load_position",&lut_downlook_load_position);
    temp_map.insert("lut_downlook_unload_position",&lut_downlook_unload_position);
    temp_map.insert("aa_updownlook_position",&aa_updownlook_position);
    temp_map.insert("aa_picklens_position",&aa_picklens_position);
    temp_map.insert("aa_unpicklens_position",&aa_unpicklens_position);
    temp_map.insert("aa_uplook_position",&aa_uplook_position);
    temp_map.insert("aa_mushroom_position",&aa_mushroom_position);
    temp_map.insert("sut_load_position",&sut_load_position);
    temp_map.insert("downlook_position",&downlook_position);
    temp_map.insert("tool_downlook_position",&tool_downlook_position);
    temp_map.insert("mushroom_positon",&mushroom_positon);
    temp_map.insert("tool_uplook_positon",&tool_uplook_positon);
    temp_map.insert("up_downlook_offset",&up_downlook_offset);
    PropertyBase::saveJsonConfig(file_name,temp_map);
}

void SingleheadLSutModule::startWork(int run_mode)
{

}

void SingleheadLSutModule::stopWork(bool wait_finish)
{

}

void SingleheadLSutModule::resetLogic()
{

}

void SingleheadLSutModule::performHandlingOperation(int cmd)
{

}
