﻿#include "XtVcMotor.h"
#include "basemodulemanager.h"
#include "xtvcmotorparameter.h"

#include <QMessageBox>
#include <qcoreapplication.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qjsonarray.h>
#include <qjsondocument.h>

wchar_t BaseModuleManager::ip[] =  L"192.168.8.251";
wchar_t BaseModuleManager::profile_path1[] = L".\\config\\";
wchar_t BaseModuleManager::profile_path2[] = L"..\\config\\xt_motion_config.csv";

BaseModuleManager::BaseModuleManager(QObject *parent)
    : PropertyBase (parent),
    ErrorBase ("BaseModuleManager")
{
    this->moveToThread(&work_thread);
    this->work_thread.start();
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("BASE_MODULE_PARAMS", this);
    PropertyBase::loadJsonConfig(BASE_MODULE_JSON,temp_map);
    qInfo("Server Mode: %d", ServerMode());
    is_init = false;
    profile_loaded = false;
    if(!QDir(".//notopencamera").exists())
    {
        qInfo("Init BaslerPylonCamera--------------------------------------");
        pylonUplookCamera = new BaslerPylonCamera(CAMERA_SH_UT_UL);
        pylonDownlookCamera = new BaslerPylonCamera(CAMERA_SH_AA_DL);
        pylonPickarmCamera = new BaslerPylonCamera(CAMERA_SH_PA_DL);
    }

    lightingModule = new WordopLight();
    visionModule = new VisionModule(pylonDownlookCamera, pylonUplookCamera, pylonPickarmCamera);
    dothinkey = new Dothinkey();
    imageGrabberThread = new ImageGrabbingWorkerThread(dothinkey);

    connect(&aaCoreNew, &AACoreNew::callQmlRefeshImg, this, &BaseModuleManager::receiveImageFromAACore);
    connect(&aaCoreNew, &AACoreNew::pushDataToUnit, &unitlog, &Unitlog::pushDataToUnit);
    connect(&aaCoreNew, &AACoreNew::postDataToELK, &unitlog, &Unitlog::postDataToELK);
    if(!QDir(".//notopencamera").exists())
    {
        if(pylonUplookCamera) pylonUplookCamera->start();
        if(pylonDownlookCamera) pylonDownlookCamera->start();
        if(pylonPickarmCamera) pylonPickarmCamera->start();
    }
    material_tray.standards_parameters.setTrayCount(2);
    lens_tray.standards_parameters.setTrayCount(2);
    sensor_tray.standards_parameters.setTrayCount(2);
    unitlog.setServerAddress(configs.dataServerURL());
    setHomeState(false);
    connect(this,&BaseModuleManager::sendMsgSignal,this,&BaseModuleManager::sendMessageTest,Qt::BlockingQueuedConnection);
    connect(&timer, &QTimer::timeout, this, &BaseModuleManager::alarmChecking);
    connect(this,&BaseModuleManager::sendHandlingOperation,this,&BaseModuleManager::performHandlingOperation);
}

BaseModuleManager::~BaseModuleManager()
{
    this->work_thread.quit();
    this->work_thread.wait();
}

void BaseModuleManager::alarmChecking()
{
    bool checked_alarm = false;
    foreach (XtMotor* temp_motor, motors)
    {
        if(temp_motor->states.seekedOrigin()&&temp_motor->checkAlarm())
        {
            checked_alarm = true;
            AppendError(temp_motor->GetCurrentError());
            temp_motor->states.setSeekedOrigin(false);
            states.setSeekedOrigin(false);
            setHomeState(false);
        }
    }
    if(checked_alarm)
        emit sendAlarm(0,ErrorLevel::ErrorMustStop,GetCurrentError());
}

bool BaseModuleManager::sendMessageTest(QString title, QString content)
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,title,content,QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::Yes){
        return true;
    }else{
        return false;
    }
}

bool BaseModuleManager::loadParameters()
{
    configs.loadJsonConfig(QString(SYSTERM_PARAM_DIR).append(SYSTERM_CONGIF_FILE),"systermConfig");
    if(!this->paramers.loadJsonConfig(QString(CONFIG_DIR).append(SYSTERM_PARAM_FILE),SYSTERM_PARAMETER))return false;

    material_tray.loadJsonConfig(getCurrentParameterDir().append(MATERIAL_TRAY_FILE));
    if(ServerMode() ==2){
        lens_tray.loadJsonConfig(getCurrentParameterDir().append(SH_LENS_TRAY_FILE));
        sensor_tray.loadJsonConfig(getCurrentParameterDir().append(SH_SENSOR_TRAY_FILE));
    }
    aa_head_module.loadJsonConfig(getCurrentParameterDir().append(AA_HEAD_FILE));
    dothinkey->loadParams(getCurrentParameterDir().append(DOTHINGKEY_FILE));
    dispenser.parameters.loadJsonConfig(getCurrentParameterDir().append(DISPENSER_FILE),DISPENSER_PARAMETER);
    dispense_module.parameters.loadJsonConfig(getCurrentParameterDir().append(DISPENSE_MODULE_FILE),DISPENSER_MODULE_PARAMETER);

    if(ServerMode()==1)
    {
        sensor_loader_module.loadJsonConfig(getCurrentParameterDir().append(SENSOR_LOADER_FILE));
        sensor_pickarm.parameters.loadJsonConfig(getCurrentParameterDir().append(SENSOR_PICKARM_FILE),SENSOR_PICKARM_PARAMETER);

        QMap<QString,PropertyBase*> temp_map;
        temp_map.insert("sensor_tray_loader", &sensor_tray_loder_module.parameters);
        temp_map.insert("sensor_clip_stand", &sensor_clip_stand);
        temp_map.insert("entance_clip", &entrance_clip.parameters);
        temp_map.insert("exit_clip", &exit_clip.parameters);
        PropertyBase::loadJsonConfig(getCurrentParameterDir().append(SENSOR_TRAY_LOADER_FILE), temp_map);
    }
    else if(ServerMode()==0)
    {
        lut_module.loadJsonConfig(getCurrentParameterDir().append(LUT_FILE));
        lut_carrier.parameters.loadJsonConfig(getCurrentParameterDir().append(LUT_CARRIER_FILE),LUT_CARRIER_PARAMETER);
        lens_loader_module.loadJsonConfig(getCurrentParameterDir().append(LENS_LOADER_MODULE_FILE));
        lens_pick_arm.parameters.loadJsonConfig(getCurrentParameterDir().append(LENS_PICKARM_FILE),LENS_PICKARM_PARAMETER);
        tray_loader_module.parameters.loadJsonConfig(getCurrentParameterDir().append(TRAY_LOADER_FILE),TRAY_LOADER_PARAMETER);
        trayClipIn.standards_parameters.loadJsonConfig(getCurrentParameterDir().append(TRAY_CLIPIN_FILE),TRAY_CLIPIN_PARAMETER);
        trayClipOut.standards_parameters.loadJsonConfig(getCurrentParameterDir().append(TRAY_CLIPOUT_FILE),TRAY_CLIPOUT_PARAMETER);
     }
    else{
        //single_station_material_pickarm.parameters.loadJsonConfig(getCurrentParameterDir().append(MATERIAL_PICKARM_FILE),MATERIAL_PICKARM_PARAMETER);
        QMap<QString,PropertyBase*> temp_map;
        temp_map.insert("material_pickarm",&single_station_material_pickarm.parameters);
        temp_map.insert("pick_lens_base_position",&single_station_material_pickarm.pick_lens_base_position);
        temp_map.insert("pick_cmos_base_position",&single_station_material_pickarm.pick_cmos_base_position);
        temp_map.insert("pick_product_base_position",&single_station_material_pickarm.pick_product_base_position);
        temp_map.insert("place_lens_base_position",&single_station_material_pickarm.place_lens_base_position);
        temp_map.insert("place_cmos_base_position",&single_station_material_pickarm.place_cmos_base_position);
        temp_map.insert("place_product_base_position",&single_station_material_pickarm.place_product_base_position);
        temp_map.insert("cmos_escape_offset_position",&single_station_material_pickarm.cmos_escape_offset_position);
        temp_map.insert("lens_to_pr_distance_position",&single_station_material_pickarm.lens_to_pr_distance_position);
        temp_map.insert("cmos_to_pr_distance_position",&single_station_material_pickarm.cmos_to_pr_distance_position);
        PropertyBase::loadJsonConfig(getCurrentParameterDir().append(MATERIAL_PICKARM_FILE), temp_map);
        //single_station_material_loader_module.parameters.loadJsonConfig(getCurrentParameterDir().append(MATERIAL_LOADER_FILE),MATERIAL_LOADER_PARAMETER);
        QMap<QString,PropertyBase*> map;
        map.insert("material_loader",&single_station_material_loader_module.parameters);
        map.insert("sut_pr_position",&single_station_material_loader_module.sut_pr_position);
        map.insert("lut_pr_position",&single_station_material_loader_module.lut_pr_position);
        map.insert("updownlook_up_position",&single_station_material_loader_module.updownlook_up_position);
        map.insert("updownlook_down_position",&single_station_material_loader_module.updownlook_down_position);
        map.insert("lens_suction_offset",&single_station_material_loader_module.lens_suction_offset);
        map.insert("sensor_suction_offset",&single_station_material_loader_module.sensor_suction_offset);
        PropertyBase::loadJsonConfig(getCurrentParameterDir().append(MATERIAL_LOADER_FILE),map);
        lut_carrier.parameters.loadJsonConfig(getCurrentParameterDir().append(LUT_CARRIER_FILE),LUT_CARRIER_PARAMETER);
        sh_lsut_module.loadParams(getCurrentParameterDir().append(SH_LSUT_FILE));

    }
    aaCoreNew.loadJsonConfig(getCurrentParameterDir().append(AA_CORE_MODULE_FILE));
     qInfo("start  loadVcmFile(getCurrentParameterDir().append(VCM_PARAMETER_FILE));");
    loadVcmFile(getCurrentParameterDir().append(VCM_PARAMETER_FILE));
    loadMotorFile(getCurrentParameterDir().append(MOTOR_PARAMETER_FILE));
    loadCylinderFiles(getCurrentParameterDir().append(CYLINDER_PARAMETER_FILE));
    loadVacuumFiles(getCurrentParameterDir().append(VACUUM_PARAMETER_FILE));
    loadCalibrationFiles(getCurrentParameterDir().append(CALIBRATION_PARAMETER_FILE));
    loadVisionLoactionFiles(getCurrentParameterDir().append(VISION_LOCATION_PARAMETER_FILE));
    loadMotorLimitFiles(getCurrentParameterDir().append(LIMIT_PARAMETER_FILE));
    return true;
}

bool BaseModuleManager::loadconfig()
{
    loadMotorLimitFiles(getCurrentParameterDir().append(LIMIT_PARAMETER_FILE));
    return true;
}

bool BaseModuleManager::saveParameters()
{
    //pr文件拷贝
    this->paramers.saveJsonConfig(QString(CONFIG_DIR).append(SYSTERM_PARAM_FILE),SYSTERM_PARAMETER);
    material_tray.saveJsonConfig(getCurrentParameterDir().append(MATERIAL_TRAY_FILE));
    if(ServerMode() ==2){
        lens_tray.saveJsonConfig(getCurrentParameterDir().append(SH_LENS_TRAY_FILE));
        sensor_tray.saveJsonConfig(getCurrentParameterDir().append(SH_SENSOR_TRAY_FILE));
    }
    aa_head_module.saveJsonConfig(getCurrentParameterDir().append(AA_HEAD_FILE));
    sut_module.saveJsonConfig(getCurrentParameterDir().append(SUT_FILE));
    dothinkey->saveJsonConfig(getCurrentParameterDir().append(DOTHINGKEY_FILE));
    dispenser.parameters.saveJsonConfig(getCurrentParameterDir().append(DISPENSER_FILE),DISPENSER_PARAMETER);
    dispense_module.parameters.saveJsonConfig(getCurrentParameterDir().append(DISPENSE_MODULE_FILE),DISPENSER_MODULE_PARAMETER);
    sut_carrier.parameters.saveJsonConfig(getCurrentParameterDir().append(SUT_CARRIER_FILE),"sut");
    if(ServerMode()==1)
    {
        sensor_loader_module.saveJsonConfig(getCurrentParameterDir().append(SENSOR_LOADER_FILE));
        sensor_pickarm.parameters.saveJsonConfig(getCurrentParameterDir().append(SENSOR_PICKARM_FILE),SENSOR_PICKARM_PARAMETER);

        QMap<QString,PropertyBase*> temp_map;
        temp_map.insert("sensor_tray_loader", &sensor_tray_loder_module.parameters);
        temp_map.insert("sensor_clip_stand", &sensor_clip_stand);
        temp_map.insert("entance_clip", &entrance_clip.parameters);
        temp_map.insert("exit_clip", &exit_clip.parameters);
        PropertyBase::saveJsonConfig(getCurrentParameterDir().append(SENSOR_TRAY_LOADER_FILE), temp_map);
    }
    else if(ServerMode()==0)
    {
        lut_module.saveJsonConfig(getCurrentParameterDir().append(LUT_FILE));
        lut_carrier.parameters.saveJsonConfig(getCurrentParameterDir().append(LUT_CARRIER_FILE),LUT_CARRIER_PARAMETER);
        lens_loader_module.saveJsonConfig(getCurrentParameterDir().append(LENS_LOADER_MODULE_FILE));
        lens_pick_arm.parameters.saveJsonConfig(getCurrentParameterDir().append(LENS_PICKARM_FILE),LENS_PICKARM_PARAMETER);
        tray_loader_module.parameters.saveJsonConfig(getCurrentParameterDir().append(TRAY_LOADER_FILE),TRAY_LOADER_PARAMETER);
        trayClipIn.standards_parameters.saveJsonConfig(getCurrentParameterDir().append(TRAY_CLIPIN_FILE),TRAY_CLIPIN_PARAMETER);
        trayClipOut.standards_parameters.saveJsonConfig(getCurrentParameterDir().append(TRAY_CLIPOUT_FILE),TRAY_CLIPOUT_PARAMETER);
    }
    else
    {
        //single_station_material_pickarm.parameters.saveJsonConfig(getCurrentParameterDir().append(MATERIAL_PICKARM_FILE),MATERIAL_PICKARM_PARAMETER);
        QMap<QString,PropertyBase*> temp_map;
        temp_map.insert("material_pickarm",&single_station_material_pickarm.parameters);
        temp_map.insert("pick_lens_base_position",&single_station_material_pickarm.pick_lens_base_position);
        temp_map.insert("pick_cmos_base_position",&single_station_material_pickarm.pick_cmos_base_position);
        temp_map.insert("pick_product_base_position",&single_station_material_pickarm.pick_product_base_position);
        temp_map.insert("place_lens_base_position",&single_station_material_pickarm.place_lens_base_position);
        temp_map.insert("place_cmos_base_position",&single_station_material_pickarm.place_cmos_base_position);
        temp_map.insert("place_product_base_position",&single_station_material_pickarm.place_product_base_position);
        temp_map.insert("cmos_escape_offset_position",&single_station_material_pickarm.cmos_escape_offset_position);
        temp_map.insert("lens_to_pr_distance_position",&single_station_material_pickarm.lens_to_pr_distance_position);
        temp_map.insert("cmos_to_pr_distance_position",&single_station_material_pickarm.cmos_to_pr_distance_position);
        PropertyBase::saveJsonConfig(getCurrentParameterDir().append(MATERIAL_PICKARM_FILE), temp_map);
        //single_station_material_loader_module.parameters.saveJsonConfig(getCurrentParameterDir().append(MATERIAL_LOADER_FILE),MATERIAL_LOADER_PARAMETER);
        QMap<QString,PropertyBase*> map;
        map.insert("material_loader",&single_station_material_loader_module.parameters);
        map.insert("sut_pr_position",&single_station_material_loader_module.sut_pr_position);
        map.insert("lut_pr_position",&single_station_material_loader_module.lut_pr_position);
        map.insert("updownlook_up_position",&single_station_material_loader_module.updownlook_up_position);
        map.insert("updownlook_down_position",&single_station_material_loader_module.updownlook_down_position);
        map.insert("lens_suction_offset",&single_station_material_loader_module.lens_suction_offset);
        map.insert("sensor_suction_offset",&single_station_material_loader_module.sensor_suction_offset);
        PropertyBase::saveJsonConfig(getCurrentParameterDir().append(MATERIAL_LOADER_FILE),map);
        sh_lsut_module.saveParams(getCurrentParameterDir().append(SH_LSUT_FILE));
    }
    aaCoreNew.saveJsonConfig(getCurrentParameterDir().append(AA_CORE_MODULE_FILE));
    saveMotorFile(getCurrentParameterDir().append(MOTOR_PARAMETER_FILE));
    saveCylinderFiles(getCurrentParameterDir().append(CYLINDER_PARAMETER_FILE));
    saveCalibrationFiles(getCurrentParameterDir().append(CALIBRATION_PARAMETER_FILE));
    saveVisionLoactionFiles(getCurrentParameterDir().append(VISION_LOCATION_PARAMETER_FILE));
    return true;
}

bool BaseModuleManager::registerWorkers(WorkersManager *manager)
{
    bool result = true;
    result &= manager->registerWorker(&sh_lsut_module);
    result &= manager->registerWorker(&single_station_material_loader_module);
   // result &= manager->registerWorker(&aaCoreNew);
    return result;
}

void BaseModuleManager::performHandling(int cmd)
{
    emit sendHandlingOperation(cmd);
}

void BaseModuleManager::performHandlingOperation(int cmd)
{
    qInfo("performHandlingOperation cmd: ", cmd);
}

bool BaseModuleManager::loadProfile()
{
    if(profile_loaded)
        return true;
    profile_loaded = false;
    if(loadStructConfig(SYSTERM_PARAM_DIR))
        if(loadParameters())
        {
            InitStruct();
            profile_loaded = true;
            return true;
        }
    qInfo("loadStructConfig(SYSTERM_PARAM_DIR) failed！");
    return false;
}

bool BaseModuleManager::loadStructConfig(QString file_dir)
{
   bool result = loadMachineConfig(QString(file_dir).append(MACHINE_PARAM_FILE));
   if(!result)return false;
   //todo将和打料的型号无关的参数移入该函数
//   configs.loadJsonConfig()
    return true;
}

bool BaseModuleManager::loadMachineConfig(QString file_path)
{
    if(QCoreApplication::applicationDirPath()!=QDir::currentPath())
        file_path = QString("..//").append(file_path);
    int res = XT_Controler_Extend::Profile_Load((LPWSTR)file_path.toStdWString().c_str());
    if(res!=0)
    {
        QString temp_name;
        int input_count = XT_Controler_Extend::Profile_Get_IoIn_Count();
        XtGeneralInput::count = input_count;
        for (int i = 0; i < input_count; ++i) {
            temp_name = QString::fromStdWString(XT_Controler_Extend::Profile_Get_IoIn_Name(i));
            if(temp_name == ""||input_ios.contains(temp_name))
            {
                qInfo("this already has an input io : %s",temp_name.toStdString().c_str());
                continue;
            }
            XtGeneralInput* input_io = new XtGeneralInput(temp_name,i);
            input_ios.insert(temp_name,input_io);
        }
        int output_count = XT_Controler_Extend::Profile_Get_IoOut_Count();
        XtGeneralOutput::count = output_count;
        for (int i = 0; i < output_count; ++i) {
            temp_name = QString::fromStdWString(XT_Controler_Extend::Profile_Get_IoOut_Name(i));
            if(temp_name == ""||output_ios.contains(temp_name))
            {
                qInfo("this already has an output io : %s",temp_name.toStdString().c_str());
                continue;
            }
            XtGeneralOutput* output_io = new XtGeneralOutput(temp_name,i);
            output_ios.insert(temp_name,output_io);
        }
        int motor_count = XT_Controler_Extend::Profile_Get_Axis_Count();
        XtMotor::axis_id_resource = motor_count;
        for (int i = 0; i < motor_count; ++i) {
            temp_name = QString::fromStdWString(XT_Controler_Extend::Profile_Get_Axis_Name(i));
            if(temp_name == ""||motors.contains(temp_name))
            {
                qInfo("this already has an motor io : %s",temp_name.toStdString().c_str());
                continue;
            }
            XtMotor* motor = new XtMotor();
            motor->Init(temp_name);

            motors.insert(temp_name,motor);
        }
        return true;
    }
}

bool BaseModuleManager::loadVcmFile(QString file_name)
{
    QJsonArray array;
     qInfo("loadVcmFile：%s", file_name.toStdString().c_str());
    if(!loadJsonArray(file_name,array))return false;
     qInfo("VCM num in json file：%d", array.count());
    for (int i = 0; i < array.count(); i++)
    {
        XtVcMotor* temp_motor = new XtVcMotor();
        temp_motor->parameters.read(array.at(i).toObject());
        temp_motor->Init();
        QJsonObject temp_object;
        temp_motor->parameters.write(temp_object);
        if(!motors.contains(temp_motor->parameters.motorName()))
            motors.insert(temp_motor->parameters.motorName(),temp_motor);
        else
        {
            qInfo("vcm motor param name(%s)repeat!",temp_motor->parameters.motorName().toStdString().c_str());
            delete temp_motor;
        }
    }
    return true;
}

bool BaseModuleManager::saveVcmfile(QString file_name)
{
    QJsonArray array;
    foreach (QString motor_name, motors.keys()) {
        XtVcMotor* temp_motor = GetVcMotorByName(motor_name);
        if(temp_motor != nullptr)
        {
            QJsonObject object;
            temp_motor->parameters.write(object);
            array.append(object);
        }
    }
    if(array.size() > 0)
        return  saveJsonArray(file_name,array);
    return  false;
}

bool BaseModuleManager::loadMotorFile(QString file_name)
{
    QJsonObject param_object;
    if(!loadJsonObject(file_name,param_object))return false;
    foreach (XtMotor* temp_motor, motors)
        if(param_object.contains(temp_motor->Name()))
                temp_motor->parameters.read(param_object[temp_motor->Name()].toObject());
    return true;
}

bool BaseModuleManager::saveMotorFile(QString file_name)
{
    QJsonObject param_object;
    foreach (XtMotor* temp_motor, motors)
    {
        QJsonObject temp_object;
        temp_motor->parameters.write(temp_object);
        param_object[temp_motor->Name()] = temp_object;
    }
    return saveJsonObject(file_name,param_object);
}

bool BaseModuleManager::loadVacuumFiles(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
//        saveVacuumFiles(file_name);
        return false;
    }
    for (int i = 0; i < array.count(); i++)
    {
        XtVacuum* temp_vacuum = new XtVacuum();
        temp_vacuum->parameters.read(array.at(i).toObject());
        QJsonObject temp_object;
        temp_vacuum->parameters.write(temp_object);
        if(!vacuums.contains(temp_vacuum->parameters.vacuumName()))
            vacuums.insert(temp_vacuum->parameters.vacuumName(),temp_vacuum);
        else
        {
            qInfo("vcm motor param name(%s)repeat!",temp_vacuum->parameters.vacuumName().toStdString().c_str());
            delete temp_vacuum;
        }
    }
    return true;
}

bool BaseModuleManager::saveVacuumFiles(QString file_name)
{
    QJsonArray array;
    foreach (QString temp_name, vacuums.keys()) {
        XtVacuum* temp_vacuum = GetVacuumByName(temp_name);
        if(temp_vacuum != nullptr)
        {
            QJsonObject object;
            temp_vacuum->parameters.write(object);
            array.append(object);
        }
    }
    if(array.size() > 0)
        return  saveJsonArray(file_name,array);
//    else
//    {
//        XtVacuumParameter temp_param;
//        QString vcauum_name = temp_param.vacuumName();
//        QJsonArray json;
//        for (int i = 0; i < 6; ++i) {
//            QJsonObject temp_object;
//            QString temp_name = vcauum_name;
//            temp_name.append(QString::number(i));
//            temp_param.setVacuumName(temp_name);
//            temp_param.write(temp_object);
//            json.append(temp_object);
//        }
//        return  saveJsonArray(file_name,json);
//    }
    return  false;
}

bool BaseModuleManager::loadCylinderFiles(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
//        saveCylinderFiles(file_name);
        return false;
    }
    for (int i = 0; i < array.count(); i++)
    {
        XtCylinder* temp_cylinder = new XtCylinder();
        temp_cylinder->parameters.read(array.at(i).toObject());
        QJsonObject temp_object;
        temp_cylinder->parameters.write(temp_object);
        if(!cylinder.contains(temp_cylinder->parameters.cylinderName()))
            cylinder.insert(temp_cylinder->parameters.cylinderName(),temp_cylinder);
        else
        {
            qInfo("vcm motor param name(%s)repeat!",temp_cylinder->parameters.cylinderName().toStdString().c_str());
            delete temp_cylinder;
        }
    }
    return true;
}

bool BaseModuleManager::saveCylinderFiles(QString file_name)
{
    QJsonArray array;
    foreach (QString temp_name, cylinder.keys()) {
        XtCylinder* temp_cylinder = GetCylinderByName(temp_name);
        if(temp_cylinder != nullptr)
        {
            QJsonObject object;
            temp_cylinder->parameters.write(object);
            array.append(object);
        }
    }
    if(array.size() > 0)
        return  saveJsonArray(file_name,array);
//    else
//    {
//        XtCylinderParameter temp_param;
//        QString vcauum_name = temp_param.cylinderName();
//        QJsonArray json;
//        for (int i = 0; i < 6; ++i) {
//            QJsonObject temp_object;
//            QString temp_name = vcauum_name;
//            temp_name.append(QString::number(i));
//            temp_param.setCylinderName(temp_name);
//            temp_param.write(temp_object);
//            json.append(temp_object);
//        }
//        return  saveJsonArray(file_name,json);
//    }
    return  false;
}

bool BaseModuleManager::loadVisionLoactionFiles(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
//        saveVisionLoactionFiles(file_name);
        return false;
    }
    for (int i = 0; i < array.count(); i++)
    {
        VisionLocation* temp_location = new VisionLocation();
        temp_location->parameters.read(array.at(i).toObject());
        QJsonObject temp_object;
        temp_location->parameters.write(temp_object);
        if(!vision_locations.contains(temp_location->parameters.locationName()))
            vision_locations.insert(temp_location->parameters.locationName(),temp_location);
        else
        {
            qInfo("vision location param name(%s)repeat!",temp_location->parameters.locationName().toStdString().c_str());
            delete temp_location;
        }
    }
    return true;
}

bool BaseModuleManager::saveVisionLoactionFiles(QString file_name)
{
    QJsonArray array;
    foreach (QString temp_name, vision_locations.keys()) {
        VisionLocation* temp_location = GetVisionLocationByName(temp_name);
        if(temp_location != nullptr)
        {
            QJsonObject object;
            temp_location->parameters.write(object);
            array.append(object);
        }
    }
    if(array.size() > 0)
        return  saveJsonArray(file_name,array);
//    else
//    {
//        VisionLocationParameter temp_param;
//        QString location_name = temp_param.locationName();
//        QJsonArray json;
//        for (int i = 0; i < 6; ++i) {
//            QJsonObject temp_object;
//            QString temp_name = location_name;
//            temp_name.append(QString::number(i));
//            temp_param.setLocationName(temp_name);
//            temp_param.write(temp_object);
//            json.append(temp_object);
//        }
//        return  saveJsonArray(file_name,json);
//    }
    return  false;
}

bool BaseModuleManager::loadCalibrationFiles(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
//        saveCalibrationFiles(file_name);
        return false;
    }
    for (int i = 0; i < array.count(); i++)
    {
        QJsonObject data_object =array.at(i).toObject();
        Calibration* temp_calibration;
        if(data_object["calibrationName"].toString().contains("chart_calibration"))
        {
            qInfo("get chart calibration");
            temp_calibration = chart_calibration = new ChartCalibration(dothinkey, AA_MAX_INTENSITY, AA_MIN_AREA, AA_MAX_AREA, CHART_CALIBRATION, CALIBRATION_RESULT_PATH);
        }
        else
            temp_calibration = new Calibration();
        temp_calibration->parameters.read(data_object);
        QJsonObject temp_object;
        temp_calibration->parameters.write(temp_object);
        if(!calibrations.contains(temp_calibration->parameters.calibrationName()))
            calibrations.insert(temp_calibration->parameters.calibrationName(),temp_calibration);
        else
        {
            qInfo("vcm motor param name(%s)repeat!",temp_calibration->parameters.calibrationName().toStdString().c_str());
            delete temp_calibration;
        }
    }
    return true;
}

bool BaseModuleManager::saveCalibrationFiles(QString file_name)
{
    QJsonArray array;
    foreach (QString temp_name, calibrations.keys()) {
        Calibration* temp_calibration = GetCalibrationByName(temp_name);
        if(temp_calibration != nullptr)
        {
            QJsonObject object;
            temp_calibration->parameters.write(object);
            array.append(object);
        }
    }
    if(array.size() > 0)
        return  saveJsonArray(file_name,array);
//    else
//    {
//        CalibrationParameter temp_param;
//        QString caibration_name = temp_param.calibrationName();
//        QJsonArray json;
//        for (int i = 0; i < 6; ++i) {
//            QJsonObject temp_object;
//            QString temp_name = caibration_name;
//            temp_name.append(QString::number(i));
//            temp_param.setCalibrationName(temp_name);
//            temp_param.write(temp_object);
//            json.append(temp_object);
//        }
//        return  saveJsonArray(file_name,json);
//    }
    return  false;
}

bool BaseModuleManager::loadMotorLimitFiles(QString file_name)
{
    QJsonObject param_object;
    if(!loadJsonObject(file_name,param_object))
        return false;
    foreach (XtMotor* temp_motor, motors)
    {
        temp_motor->vertical_limit_parameters.clear();
        temp_motor->parallel_limit_parameters.clear();
        temp_motor->io_limit_parameters.clear();
        if(param_object.contains(temp_motor->Name()))
        {
           QJsonObject temp_object = param_object[temp_motor->Name()].toObject();
           for (int i = 0; i < temp_object.size(); ++i)
           {
               QString temp_name = temp_object.keys()[i];
               if(temp_name.contains("VerticalMotorLimit"))
               {
                   VerticalLimitParameter* temp_param = new VerticalLimitParameter();
                   temp_param->read(temp_object[temp_name].toObject());
                   temp_motor-> vertical_limit_parameters.append(temp_param);
                   continue;
               }
               if(temp_name.contains("ParallelMotorLimit"))
               {
                   ParallelLimitParameter* temp_param = new ParallelLimitParameter();
                   temp_param->read(temp_object[temp_name].toObject());
                   temp_motor->parallel_limit_parameters.append(temp_param);
                   continue;
               }
               if(temp_name.contains("IOLimit"))
               {
                   IOLimitParameter* temp_param = new IOLimitParameter();
                   temp_param->read(temp_object[temp_name].toObject());
                   temp_motor->io_limit_parameters.append(temp_param);
                   continue;
               }
           }
        }
        qInfo("%s loadmotorlimit motors %d limit %d",temp_motor->Name().toStdString().c_str(),temp_motor->parallel_limit_motors.count(),temp_motor->parallel_limit_parameters.count());
    }
    return true;
}

bool BaseModuleManager::saveMotorLimitFiles(QString file_name)
{
    QJsonObject parameters_object;
    foreach (QString temp_name, motors.keys())
    {
        XtMotor* temp_motor = GetMotorByName(temp_name);
        if(temp_motor != nullptr)
        {
            QJsonObject temp_object;
            int i = 0;
            foreach(VerticalLimitParameter* limit,temp_motor->vertical_limit_parameters)
            {
                i++;
                QJsonObject object;
                limit->write(object);
                temp_object[QString("VerticalMotorLimit").append(QString::number(i))] = object;
            }
            i = 0;
            foreach(ParallelLimitParameter* limit,temp_motor->parallel_limit_parameters)
            {
                i++;
                QJsonObject object;
                limit->write(object);
                temp_object[QString("ParallelMotorLimit").append(QString::number(i))] = object;
            }
            i = 0;
            foreach(IOLimitParameter* io_limit,temp_motor->io_limit_parameters)
            {
                i++;
                QJsonObject object;
                io_limit->write(object);
                temp_object[QString("IOLimit").append(QString::number(i))] = object;
            }
            if(temp_object.count()>0)
                parameters_object[temp_name] = temp_object;
        }
    }
   return saveJsonObject(file_name,parameters_object);
}

bool BaseModuleManager::loadJsonArray(QString file_name,QJsonArray &array)
{
    QString val;
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        AppendError(QString(u8"加载参数文件失败，文件名 %1").arg(file_name));
        qWarning("load parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    val = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doucment = QJsonDocument::fromJson(val.toUtf8(), &error);
    if (!doucment.isNull() && (error.error == QJsonParseError::NoError)) { //解析否出现错误
        if (doucment.isArray()) { // 数组判断
            array = doucment.array(); // 转数组
        }
    }
    if(array.size()>0)
        return true;
    else
    {
        AppendError(QString(u8"解析参数文件失败，文件名 %1").arg(file_name));
        qInfo("load parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
}

bool BaseModuleManager::saveJsonArray(QString file_name,QJsonArray &array)
{
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QFile::WriteOnly)){
        AppendError(QString(u8"保存参数文件失败，文件名 %1").arg(file_name));
        qWarning("save parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    QJsonDocument document;
    document.setArray(array);
    file.write(document.toJson());
    file.close();
    return true;
}

bool BaseModuleManager::loadJsonObject(QString file_name, QJsonObject &object)
{
    QString val;
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("load parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    val = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doucment = QJsonDocument::fromJson(val.toUtf8(), &error);
    if (!doucment.isNull() && (error.error == QJsonParseError::NoError)) { //解析否出现错误
        if (doucment.isObject()) { // 数组判断
            object = doucment.object(); // 转数组
            return true;
        }
    }
    qInfo("load parameters to %s failed, load file error:%s.",file_name.toStdString().data(),error.errorString().toStdString().c_str());
    return false;
}

bool BaseModuleManager::saveJsonObject(QString file_name, QJsonObject &object)
{
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QFile::WriteOnly)){
        qWarning("save parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    QJsonDocument document;
    document.setObject(object);
    file.write(document.toJson());
    file.close();
    return true;
}

QString BaseModuleManager::getCurrentParameterDir()
{
    QString dir = QString(CONFIG_DIR).append(paramers.materialType()).append("//");
    if(!QDir(dir).exists())
        QDir().mkdir(dir);
    return dir;
}

bool BaseModuleManager::InitStruct()
{
    foreach (XtMotor* temp_motor, motors)
    {
        for (int i = 0; i < temp_motor->vertical_limit_parameters.size(); ++i) {
            XtMotor* limit_motor = GetMotorByName(temp_motor->vertical_limit_parameters[i]->motorName());
            temp_motor->vertical_limit_motors.append(limit_motor);
        }
        for (int i = 0; i < temp_motor->parallel_limit_parameters.size(); ++i) {
            XtMotor* limit_motor = GetMotorByName(temp_motor->parallel_limit_parameters[i]->motorName());
            temp_motor->parallel_limit_motors.append(limit_motor);
            limit_motor = GetMotorByName(temp_motor->parallel_limit_parameters[i]->effectMotorXName());
            temp_motor->parallel_limit_motors.append(limit_motor);
            limit_motor = GetMotorByName(temp_motor->parallel_limit_parameters[i]->effectMotorYName());
            temp_motor->parallel_limit_motors.append(limit_motor);
        }
        for (int i = 0; i < temp_motor->io_limit_parameters.size(); ++i) {
            XtGeneralInput* limit_io = GetInputIoByName(temp_motor->io_limit_parameters[i]->inputIOName());
            temp_motor->limit_ios.append(limit_io);
        }
    }
    foreach (XtVacuum* temp_vacuum, vacuums.values()) {
        temp_vacuum->Init(GetOutputIoByName(temp_vacuum->parameters.outIoName()),
                          GetInputIoByName(temp_vacuum->parameters.inIoName()),
                          GetOutputIoByName(temp_vacuum->parameters.breakIoName()));
    }

    foreach (XtCylinder* temp_cylinder, cylinder.values()) {
        temp_cylinder->Init(GetOutputIoByName(temp_cylinder->parameters.oneOutName()),
                            GetInputIoByName(temp_cylinder->parameters.oneInName()),
                            GetInputIoByName(temp_cylinder->parameters.zeroInName()),
                            GetOutputIoByName(temp_cylinder->parameters.zeroOutName()));
    }

    sut_carrier.Init("sut_carrier",GetMotorByName(sh_lsut_module.parameters.motorXName()),
                     GetMotorByName(sh_lsut_module.parameters.motorYName()),
                     GetVcMotorByName(sh_lsut_module.parameters.motorZName()),
                     GetVacuumByName(sh_lsut_module.parameters.sutVacuumName()));

    aa_head_module.Init("AAHead",GetMotorByName(aa_head_module.parameters.motorXName()),
                        GetMotorByName(aa_head_module.parameters.motorYName()),
                        GetMotorByName(aa_head_module.parameters.motorZName()),
                        GetMotorByName(aa_head_module.parameters.motorAName()),
                        GetMotorByName(aa_head_module.parameters.motorBName()),
                        GetMotorByName(aa_head_module.parameters.motorCName()),
                        GetOutputIoByName(aa_head_module.parameters.gripperName()),
                        GetOutputIoByName(aa_head_module.parameters.uv1Name()),
                        GetOutputIoByName(aa_head_module.parameters.uv2Name()),
                        GetOutputIoByName(aa_head_module.parameters.uv3Name()),
                        GetOutputIoByName(aa_head_module.parameters.uv4Name()),
                        XtMotor::GetThreadResource(),
                        &sut_carrier);

    foreach (Calibration* temp_calibraion, calibrations) {
        temp_calibraion->Init(GetMotorByName(temp_calibraion->parameters.motorXName()),
                              GetMotorByName(temp_calibraion->parameters.motorYName()),
                              GetVisionLocationByName(temp_calibraion->parameters.locationName()));
    }
    if (chart_calibration) {
        chart_calibration->Init(GetMotorByName(chart_calibration->parameters.motorXName()),
                                GetMotorByName(chart_calibration->parameters.motorYName()),
                                nullptr);
    }
    foreach (VisionLocation* temp_vision, vision_locations.values()) {
        temp_vision->Init(visionModule,GetPixel2MechByName(temp_vision->parameters.calibrationName()),lightingModule);
    }

    QVector<XtMotor *> executive_motors;
    executive_motors.push_back(GetMotorByName(sut_module.parameters.motorXName()));
    executive_motors.push_back(GetMotorByName(sut_module.parameters.motorYName()));
    executive_motors.push_back(GetMotorByName(sut_module.parameters.motorZName()));
    dispenser.Init(XtMotor::GetCurveResource(),XtMotor::GetThreadResource(),XtMotor::GetThreadResource(),executive_motors,
                   GetOutputIoByName(dispenser.parameters.dispenseIo()));

    dispense_module.Init(DISPENSER_PARAMETER_PATH,"dispense_module",
                         GetCalibrationByName(GetVisionLocationByName(sh_lsut_module.parameters.sutDownLookLocationName())->parameters.calibrationName()),
                         &dispenser,visionModule,&sut_carrier,
                         GetOutputIoByName(dispenser.parameters.dispenseIo()));
    dispense_module.setMapPosition(sut_module.downlook_position.X(),sut_module.downlook_position.Y());

    if(ServerMode()==2)
    {
        //single_station_material_pickarm.Init();
        single_station_material_pickarm.Init(GetMotorByName(single_station_material_pickarm.parameters.motorXName()),
                                             GetMotorByName(single_station_material_pickarm.parameters.motorYName()),
                                             GetMotorByName(single_station_material_pickarm.parameters.motorTh1Name()),
                                             GetMotorByName(single_station_material_pickarm.parameters.motorTh2Name()),
                                             GetVcMotorByName(single_station_material_pickarm.parameters.motorVcm1Name()),
                                             GetVcMotorByName(single_station_material_pickarm.parameters.motorVcm2Name()),
                                             GetVcMotorByName(single_station_material_pickarm.parameters.motorVcmXName()),
                                             GetVacuumByName(single_station_material_pickarm.parameters.vacuumLensSuctionName()),
                                             GetVacuumByName(single_station_material_pickarm.parameters.vacuumSensorSuctionName()),
                                             GetVacuumByName(single_station_material_pickarm.parameters.vacuumLUTName()),
                                             GetVacuumByName(single_station_material_pickarm.parameters.vacuumSUTName()),
                                             GetCylinderByName(single_station_material_pickarm.parameters.cylinderName()));
        single_station_material_loader_module.Init(&single_station_material_pickarm,&sensor_tray,&lens_tray,
                                                   GetVisionLocationByName(single_station_material_loader_module.parameters.sensorVisionName()),
                                                   GetVisionLocationByName(single_station_material_loader_module.parameters.sensorVacancyVisionName()),
                                                   GetVisionLocationByName(single_station_material_loader_module.parameters.sutVisionName()),
                                                   GetVisionLocationByName(single_station_material_loader_module.parameters.sutSensorVisionName()),
                                                   GetVisionLocationByName(single_station_material_loader_module.parameters.sutProductVisionName()),
                                                   GetVisionLocationByName(single_station_material_loader_module.parameters.lensVisionName()),
                                                   GetVisionLocationByName(single_station_material_loader_module.parameters.lensVacancyVisionName()),
                                                   GetVisionLocationByName(single_station_material_loader_module.parameters.lutVisionName()),
                                                   GetVisionLocationByName(single_station_material_loader_module.parameters.lutLensVision()),
                                                   GetVacuumByName(single_station_material_loader_module.parameters.sutVacuumName()),
                                                   GetVacuumByName(single_station_material_loader_module.parameters.lutVacuumName()));
        sensor_tray.resetTrayState(0);
        sensor_tray.resetTrayState(1);
        lens_tray.resetTrayState(0);
        lens_tray.resetTrayState(1);

        sh_lsut_module.Init(&sut_carrier,
                            GetVisionLocationByName(sh_lsut_module.parameters.sutDownLookLocationName()),
                            GetVisionLocationByName(sh_lsut_module.parameters.updownlookDownLocationName()),
                            GetVisionLocationByName(sh_lsut_module.parameters.updownlookUpLocationName()),
                            GetVisionLocationByName(sh_lsut_module.parameters.mushroomLocationName()),
                            GetVisionLocationByName(sh_lsut_module.parameters.lutGripperLoactionName()),
                            GetVacuumByName(sh_lsut_module.parameters.sutVacuumName()),
                            GetVacuumByName(sh_lsut_module.parameters.lutVacuumName()),
                            GetCylinderByName(sh_lsut_module.parameters.cylinderName()));

    }else
    {
        lut_carrier.Init("lut_carrier",GetMotorByName(sh_lsut_module.parameters.motorXName()),
                         GetMotorByName(sh_lsut_module.parameters.motorYName()),
                         GetVcMotorByName(sh_lsut_module.parameters.motorZName()),
                         GetVacuumByName(sh_lsut_module.parameters.lutVacuumName()));
    }
    tray_loader_module.Init(GetMotorByName(tray_loader_module.parameters.motorLTIEName()),
                            GetMotorByName(tray_loader_module.parameters.motorLTKX1Name()),
                            GetMotorByName(tray_loader_module.parameters.motorLTLXName()),
                            GetMotorByName(tray_loader_module.parameters.motorLTKX2Name()),
                            GetMotorByName(tray_loader_module.parameters.motorLTOEName()),
                            GetCylinderByName(tray_loader_module.parameters.cylinderClipName()),
                            GetCylinderByName(tray_loader_module.parameters.cylinderLTK1Name()),
                            GetCylinderByName(tray_loader_module.parameters.cylinderLTK2Name()),
                            GetCylinderByName(tray_loader_module.parameters.cylinderTrayName()),
                            &trayClipIn,&trayClipOut,
                            GetInputIoByName(tray_loader_module.parameters.clipinInputName()),
                            GetInputIoByName(tray_loader_module.parameters.clipoutInputName()),
                            GetInputIoByName(tray_loader_module.parameters.trayInputIoName()));

    sfrWorkerController = new SfrWorkerController(&aaCoreNew);
    aaCoreNew.setSfrWorkerController(sfrWorkerController);
    aaCoreNew.Init(&aa_head_module, lutClient, &sut_module, dothinkey, chart_calibration, &dispense_module, imageGrabberThread, &unitlog);
    entrance_clip.Init(u8"Sensor进料盘弹夹",&sensor_clip_stand);
    exit_clip.Init(u8"Sensor出料盘弹夹",&sensor_clip_stand);
    sensor_tray_loder_module.Init(GetMotorByName(sensor_tray_loder_module.parameters.motorTrayName()),
                                  GetMotorByName(sensor_tray_loder_module.parameters.motorSTKName()),
                                  GetMotorByName(sensor_tray_loder_module.parameters.motorSTIEName()),
                                  GetMotorByName(sensor_tray_loder_module.parameters.motorSTOEName()),
                                  GetMotorByName(sensor_tray_loder_module.parameters.motorSPOName()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderSTK1Name()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderSTK2Name()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderHoldTrayName()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderVacancyTrayName()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderEntanceClipPushName()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderExitClipPushName()),
                                  GetCylinderByName(sensor_tray_loder_module.parameters.cylinderGripperName()),
                                  &entrance_clip,&exit_clip);

    //todo
    material_tray.resetTrayState(0);
    material_tray.resetTrayState(1);
    return true;
}

bool BaseModuleManager::initialDevice()
{
    if(is_init)
        return true;
    if(!profile_loaded)
        return false;
    timer.stop();
    qInfo("Init module manager");
    LPWSTR pTarget = ip;
    XT_Controler::InitDevice_PC_Local_Controler(0);
    int res = XT_Controler::beCurConnectServerAndInterfaceBoard();
    if (1 != res) {
        res = XT_Controler::ConnectControlServer(pTarget, 0, 0);
        if (1 != res)
        {
            qInfo("Motion control server cannot connect");
            return false;
        }
    }

    XT_Controler::ReBuildSystem();

    QTime tic;
    tic.start();
    while(tic.elapsed()<300);

    res = XT_Controler::ConnectControlServer(pTarget, 0, 0);

    if (1 != res)
    {
        qInfo("Motion control server cannot connect");
        return false;
    }

    res = XT_Controler_Extend::Profile_Init_Controller(1);

    if (1 != res)
    {
        qInfo("Motion control server cannot connect");
        return false;
    }
    XT_Controler_Extend::Stop_Buffer_Sync();

    XtVcMotor::InitAllVCM();
    XtVcMotor* temp_motor;
    foreach (QString mator_name, motors.keys()) {
        temp_motor = GetVcMotorByName(mator_name);
        if(temp_motor!= nullptr)
            temp_motor->ConfigVCM();
    }
    XT_Controler_Extend::Start_Buffer_Sync(-1);

    is_init = true;
    //this must after "is_init = true;!!!"
    foreach (XtMotor *m, motors.values()) {
        m->GetMasterAxisID();
    }
    EnableMotors();
//    timer.start(1000);
    return true;
}

bool BaseModuleManager::generateConfigFiles()
{
    foreach (XtMotor* temp_motor, motors) {
        temp_motor->parallel_limit_parameters.append(new ParallelLimitParameter());
        temp_motor->vertical_limit_parameters.append(new VerticalLimitParameter());
        QVariantList temp_space;temp_space.append(0);temp_space.append(1);
        temp_motor->vertical_limit_parameters[0]->setMoveSpance(temp_space);
        temp_motor->vertical_limit_parameters[0]->setLimitSpance(temp_space);
        temp_motor->io_limit_parameters.append(new IOLimitParameter());
        temp_motor->io_limit_parameters[0]->setMoveSpance(temp_space);
    }
    bool result = saveMotorLimitFiles(LIMIT_PARAMETER_MODE_FILENAME);

    return result;
}

void BaseModuleManager::EnableMotors()
{
    if(is_init) {
        foreach (XtMotor *m, motors.values()) {
            m->Enable();
        }
    }
}

void BaseModuleManager::DisableAllMotors()
{
    if(is_init) {
        foreach (XtMotor *m, motors.values()) {
            m->Disable();
        }
    }
}

bool BaseModuleManager::allMotorsSeekOrigin()
{
    return allMotorsSeekOriginal3();
}

bool BaseModuleManager::allMotorsSeekOriginal3()
{
    qInfo("allMotorsSeekOriginal3 Start");
    bool result = false;

    //Home SUT Z First
    GetVcMotorByName(sh_lsut_module.parameters.motorZName())->SeekOrigin();
    result &= GetVcMotorByName(sh_lsut_module.parameters.motorZName())->WaitSeekDone();

    GetVcMotorByName(single_station_material_pickarm.parameters.motorVcmXName())->SeekOrigin();
    GetMotorByName(sh_lsut_module.parameters.motorXName())->SeekOrigin();
    GetMotorByName(sh_lsut_module.parameters.motorYName())->SeekOrigin();

//    GetMotorByName(aa_head_module.parameters.motorAName())->SeekOrigin();
//    GetMotorByName(aa_head_module.parameters.motorBName())->SeekOrigin();
//    GetMotorByName(aa_head_module.parameters.motorCName())->SeekOrigin();

    GetVcMotorByName(single_station_material_pickarm.parameters.motorVcm1Name())->SeekOrigin();
    GetVcMotorByName(single_station_material_pickarm.parameters.motorVcm2Name())->SeekOrigin();
    result &= GetVcMotorByName(single_station_material_pickarm.parameters.motorVcm1Name())->WaitSeekDone();
    result &= GetVcMotorByName(single_station_material_pickarm.parameters.motorVcm2Name())->WaitSeekDone();
    result &= GetVcMotorByName(single_station_material_pickarm.parameters.motorVcmXName())->WaitSeekDone();
    GetMotorByName(single_station_material_pickarm.parameters.motorXName())->SeekOrigin();
    GetMotorByName(single_station_material_pickarm.parameters.motorYName())->SeekOrigin();
    //GetMotorByName(single_station_material_pickarm.parameters.motorZName())->SeekOrigin();
    //result &= GetMotorByName(single_station_material_pickarm.parameters.motorZName())->WaitSeekDone();

    GetMotorByName(single_station_material_pickarm.parameters.motorTh1Name())->SeekOrigin();
    GetMotorByName(single_station_material_pickarm.parameters.motorTh2Name())->SeekOrigin();

    result = GetMotorByName(single_station_material_pickarm.parameters.motorXName())->WaitSeekDone();
    result &= GetMotorByName(single_station_material_pickarm.parameters.motorYName())->WaitSeekDone();
    result &= GetMotorByName(single_station_material_pickarm.parameters.motorTh1Name())->WaitSeekDone();
    result &= GetMotorByName(single_station_material_pickarm.parameters.motorTh2Name())->WaitSeekDone();

    result &= GetMotorByName(sh_lsut_module.parameters.motorXName())->WaitSeekDone();
    result &= GetMotorByName(sh_lsut_module.parameters.motorYName())->WaitSeekDone();

    result &= GetMotorByName(aa_head_module.parameters.motorAName())->WaitSeekDone();
    result &= GetMotorByName(aa_head_module.parameters.motorBName())->WaitSeekDone();
    result &= GetMotorByName(aa_head_module.parameters.motorCName())->WaitSeekDone();

    if(result){
        qInfo("all motor seeked origin successed!");
        setHomeState(true);
        return true;
    }

    return false;
}

void BaseModuleManager::stopSeeking()
{
    if(is_init)
        foreach (XtMotor *m, motors.values()) {
            m->StopSeeking();
            m->Disable();
        }
}

int BaseModuleManager::getNumberOfMotors()
{
    return motors.size();
}

QString BaseModuleManager::getMotorsName(int index)
{
    return motors.keys()[index];
}

void BaseModuleManager::updateParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("BASE_MODULE_PARAMS", this);
    PropertyBase::saveJsonConfig(BASE_MODULE_JSON,temp_map);
    saveParameters();
    //    loadParameters();
}

void BaseModuleManager::loadFlowchart(QString json)
{
    qInfo("Load flowchart: %s", json.toStdString().c_str());
    aaCoreNew.setFlowchartDocument(json);
}

void BaseModuleManager::loadSensorTrayLoaderMuduleParameter()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("sensor_tray_loader", &sensor_tray_loder_module.parameters);
    temp_map.insert("sensor_clip_stand", &sensor_clip_stand);
    temp_map.insert("entance_clip", &entrance_clip.parameters);
    temp_map.insert("exit_clip", &exit_clip.parameters);
    PropertyBase::loadJsonConfig(SENSOR_TRAY_LOADER_FILE_NAME, temp_map);
}

XtMotor *BaseModuleManager::GetMotorByName(QString name)
{
    if(name == "")return nullptr;
    if(motors.contains(name))
        return motors[name];
    else
        qInfo("can not find motor io %s",name.toStdString().c_str());
    return nullptr;
}

XtVcMotor *BaseModuleManager::GetVcMotorByName(QString name)
{
    if(name == "")return nullptr;
    if(motors.contains(name))
        return  dynamic_cast<XtVcMotor*>(motors[name]);
    else
        qInfo("can not find vcm motor io %s",name.toStdString().c_str());
    return nullptr;
}

XtGeneralOutput *BaseModuleManager::GetOutputIoByName(QString name)
{
    if(name == "")return nullptr;
    if(output_ios.contains(name))
        return output_ios[name];
    else
        qInfo("can not find output io %s",name.toStdString().c_str());
    return nullptr;
}

XtGeneralInput *BaseModuleManager::GetInputIoByName(QString name)
{
    if(name == "")return nullptr;
    if(input_ios.contains(name))
        return input_ios[name];
    else
        qInfo("can not find input io %s",name.toStdString().c_str());
    return nullptr;
}

XtVacuum *BaseModuleManager::GetVacuumByName(QString name)
{
    if(name == "")return nullptr;
    if(vacuums.contains(name))
        return vacuums[name];
    else
        qInfo("can not find vacuum io %s",name.toStdString().c_str());
    return nullptr;
}

XtCylinder *BaseModuleManager::GetCylinderByName(QString name)
{
    if(name == "")return nullptr;
    if(cylinder.contains(name))
        return cylinder[name];
    else
        qInfo("can not find cylinder io %s",name.toStdString().c_str());
    return nullptr;
}

VisionLocation *BaseModuleManager::GetVisionLocationByName(QString name)
{
    if(name == "")return nullptr;
    if(vision_locations.contains(name))
        return vision_locations[name];
    else
        qInfo("can not find vision location: %s",name.toStdString().c_str());
    return nullptr;
}

Pixel2Mech *BaseModuleManager::GetPixel2MechByName(QString name)
{
    if(name == "")return nullptr;
    if(calibrations.contains(name))
        return calibrations[name]->getCaliMapping();
    else
        qInfo("can not find calibration pixel2mech io %s",name.toStdString().c_str());
    return nullptr;
}

Calibration *BaseModuleManager::GetCalibrationByName(QString name)
{
    if(name == "")return nullptr;
    if(calibrations.contains(name))
        return calibrations[name];
    else
        qInfo("can not find calibration io %s",name.toStdString().c_str());
    return nullptr;
}

bool BaseModuleManager::stepMove(QString name, double step, bool isPositive)
{
    if (!motors.contains(name)) {
        qInfo("Selected motor does not exist");
        return false;
    }
    XtMotor* temp_motor = motors[name];
    qInfo("Step move: %s %f %d %f", temp_motor->Name().toStdString().c_str(), step, isPositive, temp_motor->GetFeedbackPos());
    if (isPositive)
        temp_motor->StepMove(step);
    else {
        temp_motor->StepMove(-step);
    }
    return true;
}


bool BaseModuleManager::stepMove(int index, double step, bool isPositive)
{
    XtMotor* temp_motor = motors.values()[index];
    qInfo("Step move: %s %f %d %f", temp_motor->Name().toStdString().c_str(), step, isPositive, temp_motor->GetFeedbackPos());
    if (isPositive)
        temp_motor->StepMove(step);
    else {
        temp_motor->StepMove(-step);
    }
    return true;
}

void BaseModuleManager::setMotorParamByName(QString name, double vel, double acc, double jert)
{
    qInfo("setMotorParamByName %f, %f, %f",vel,acc,jert);
  XtMotor* temp_motor = GetMotorByName(name);
  if(temp_motor != nullptr)
  {
      temp_motor->SetVel(vel);
      temp_motor->SetAcc(acc);
      temp_motor->SetJerk(jert);
  }
}

bool BaseModuleManager::performCalibration(QString calibration_name)
{
    qInfo("perform %s",calibration_name.toStdString().c_str());
    if(calibration_name.contains("chart_calibration"))
        return  chart_calibration->performCalibration();
    Calibration* temp_caliration = GetCalibrationByName(calibration_name);
    if(temp_caliration == nullptr)
    {
        qInfo("not calibration %s",calibration_name.toStdString().c_str());
        return  false;
    }
    return  temp_caliration->performCalibration();
}

bool BaseModuleManager::performUpDnLookCalibration()
{
    qInfo("performUpDnLookCalibration");
    //ToDo: Move the lut movement in LUT Client.
    lutClient->sendLUTMoveToPos(0); //Unload Pos
    PrOffset offset1, offset2;
    sut_module.moveToToolDownlookPos(true);
    if (!sut_module.toolDownlookPR(offset1,true,false)) {
        qInfo("SUT Cannot do the tool downlook PR");
        return false;
    }
    qInfo("UpDnlook Down PR: %f %f %f", offset1.X, offset1.Y, offset1.Theta);
    sut_module.moveToToolUplookPos(true);
    if (ServerMode() == 0) {
        lutClient->sendLUTMoveToPos(1); //AA 1 Pos
        lutClient->requestToolUpPRResult(offset2); //Do uplook PR
    } else {
        lutClient->sendLUTMoveToPos(2); //AA 2 Pos
    }
    double offsetT = offset1.Theta - offset2.Theta;
    qInfo("UpDnlook Up PR: %f %f %f", offset2.X, offset2.Y, offset2.Theta);
    qInfo("UpDnlook Camera Offset downlook - uplook = %f", offsetT);
    sut_module.up_downlook_offset.setX(sut_module.tool_uplook_positon.X() - offset2.X - sut_module.tool_downlook_position.X() + offset1.X);
    sut_module.up_downlook_offset.setY(sut_module.tool_uplook_positon.Y() - offset2.Y - sut_module.tool_downlook_position.Y() + offset1.Y);
    sut_module.up_downlook_offset.setTheta(offset1.Theta - offset2.Theta);
    qInfo("updownlook offset %f,%f,%f",sut_module.up_downlook_offset.X(),sut_module.up_downlook_offset.Y(),sut_module.up_downlook_offset.Theta());
    sut_module.parameters.setCameraTheta(offsetT);
    lutClient->sendLUTMoveToPos(0); //AA 1 Pos
    return true;
}
bool BaseModuleManager::performLensUpDnLookCalibration()
{
    this->lens_loader_module.calculateCameraToPickerOffset();
    this->lut_module.lpa_camera_to_picker_offset.setX(-lens_loader_module.camera_to_picker_offset.X());
    this->lut_module.lpa_camera_to_picker_offset.setY(-lens_loader_module.camera_to_picker_offset.Y());
    qInfo("Lens UpDnlook Calibration result offsetX : %f offsetY: %f", lens_loader_module.camera_to_picker_offset.X() ,lens_loader_module.camera_to_picker_offset.Y());
    return true;
}

bool BaseModuleManager::performLocation(QString location_name)
{
//    if(!emit sendMsgSignal("title","content")){
//        return true;
//    }
    qInfo("perform %s",location_name.toStdString().c_str());
    VisionLocation* temp_location = GetVisionLocationByName(location_name);
    if(temp_location == nullptr)
    {
        qInfo("location(%s) is null",location_name.toStdString().c_str());
        return false;
    }
    Calibration* temp_caliration = GetCalibrationByName(temp_location->parameters.calibrationName());
    if(temp_caliration == nullptr)
    {
        qInfo("calibraion(%s)is null",temp_location->parameters.calibrationName().toStdString().c_str());
        return  false;
    }
    PrOffset offset;
    if(temp_location->parameters.calibrationName().contains("chart_calibration"))
    {
        double x,y;
        if(!temp_caliration->GetPixelPoint(x,y))
            return false;
        QPointF result_point;
        temp_caliration->getDeltaDistanceFromCenter(QPointF(x,y),result_point);
        offset.X = result_point.x(); offset.Y = result_point.y();
    }
    else
    {
        if(!temp_location->performPR(offset))return false;
    }
    if(temp_location->parameters.canMotion())
    {
//        if(!emit sendMsgSignal(tr(u8"提示"),tr(u8"x: %1,y:%2,theta:%3,是否移动").arg(offset.X).arg(offset.Y).arg(offset.Theta))){
//            return true;
//        }
        temp_caliration->performPRResult(offset);
    }
    return true;
}

QString BaseModuleManager::getCalibrationParam(QString calibration_name)
{
    Calibration* temp_caliration = GetCalibrationByName(calibration_name);
    if(temp_caliration == nullptr)return  "no calibration";
    QString temp_value = "(";
    QPointF temp_point = temp_caliration->getOnePxielDistance();
    temp_value.append(QString::number(round(temp_point.x()*10000)/10000));
    temp_value.append(",");
    temp_value.append(QString::number(round(temp_point.y()*10000)/10000));
    temp_value.append(",");
    temp_value.append(QString::number(round(temp_caliration->getRotationAngle()*100)/100));
    temp_value.append(")");
    return temp_value;
}

void BaseModuleManager::setOutput(QString name, bool on)
{
    if(name=="SUT1吸真空")
        on = !on;
    if (this->output_ios.contains(name)){
        qInfo("set output : %s %d", name.toStdString().c_str(), on);
        output_ios[name]->Set(on);
    }
}

bool BaseModuleManager::getOutput(QString name)
{
    if (this->output_ios.contains(name)){
        if(name=="SUT1吸真空")
            return !output_ios[name]->Value();
        return  output_ios[name]->Value();
    }
    return  false;
}

bool BaseModuleManager::getInput(QString name)
{
    if (this->input_ios.contains(name)){
        return  input_ios[name]->Value();
    }
    return  false;
}

void BaseModuleManager::motorSeekOrigin(QString name)
{
    qInfo("motorSeekOrigin: %s", name.toStdString().c_str());
    if (motors.contains(name)) {
        qInfo("Start motorSeekOrigin: %s", name.toStdString().c_str());
        motors[name]->SeekOrigin();
    }
}

double BaseModuleManager::getPROffsetX(QString location_name)
{
    return  GetVisionLocationByName(location_name)->getCurrentOffset().x();
}

double BaseModuleManager::getPROffsetY(QString location_name)
{
    return  GetVisionLocationByName(location_name)->getCurrentOffset().y();
}

double BaseModuleManager::getMotorFeedbackPos(QString name)
{
    if (motors.contains(name)) {
        return motors[name]->GetFeedbackPos();
    }
    return 0;
}

double BaseModuleManager::getMotorFeedbackPos(int index)
{
    return motors.values()[index]->GetFeedbackPos();
}

void BaseModuleManager::setLightingBrightness(QString location_name)
{
    GetVisionLocationByName(location_name)->OpenLight();
}

void BaseModuleManager::sendLoadLens(bool has_ng)
{
    aa_head_module.moveToPickLensPosition();
    if(has_ng)
        emit lut_module.sendLoadLensRequst(true,0,0);
    else
        emit lut_module.sendLoadLensRequst(true,-1,-1);
}

void BaseModuleManager::sendLoadSensor(bool has_product, bool has_ng)
{
    if(has_product)
    {
        emit  aa_head_module.sendSensrRequestToSut(SUT_STATE::HAS_PRODUCT);
        qInfo("sendSensrRequestToSut 2 in %d",QThread::currentThreadId());
    }
    else if(has_ng)
    {
        emit  aa_head_module.sendSensrRequestToSut(SUT_STATE::HAS_NG_SENSOR);
        qInfo("sendSensrRequestToSut 1 in %d",QThread::currentThreadId());
    }
    else
    {
        emit  aa_head_module.sendSensrRequestToSut(SUT_STATE::NO_MATERIAL);
        qInfo("sendSensrRequestToSut 0 in %d",QThread::currentThreadId());
    }
}

void BaseModuleManager::sendChangeSensorTray()
{
   emit sensor_loader_module.sendChangeTrayRequst();
}

bool BaseModuleManager::initSensor()
{
    const int channel = 0;
    bool res = dothinkey->DothinkeyEnum();
    if (!res) { qCritical("Cannot find dothinkey"); return false; }
    dothinkey->DothinkeyOpen();
    if (!res) { qCritical("Cannot open dothinkey"); return false; }
    dothinkey->DothinkeyLoadIniFile(channel);
    if (!res) { qCritical("Cannot load dothinkey ini file"); return false; }
    dothinkey->DothinkeyStartCamera(channel);
    if (!res) { qCritical("Cannot start camera"); return false; }
    imageGrabberThread->start();
    return true;
}

bool BaseModuleManager::closeSensor()
{
    imageGrabberThread->stop();
    Sleep(100);
    imageGrabberThread->exit();
    return dothinkey->DothinkeyClose();
}

void BaseModuleManager::loadSensorLoaderParameter()
{
    material_tray.loadJsonConfig(getCurrentParameterDir().append(MATERIAL_TRAY_FILE));
    sensor_loader_module.loadJsonConfig(getCurrentParameterDir().append(SENSOR_LOADER_FILE));
}

double BaseModuleManager::showChartCalibrationRotation()
{
    return chart_calibration->getRotationAngle();
}
