#include "XtVcMotor.h"
#include "basemodulemanager.h"
#include "xtvcmotorparameter.h"

#include <qcoreapplication.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qjsonarray.h>
#include <qjsondocument.h>

wchar_t BaseModuleManager::ip[] =  L"192.168.8.251";
wchar_t BaseModuleManager::profile_path1[] = L".\\config\\xt_motion_config.csv";
wchar_t BaseModuleManager::profile_path2[] = L"..\\config\\xt_motion_config.csv";
//wchar_t BaseModuleManager::profile_path2[] = L".\\xt_motion_config.csv";
BaseModuleManager::BaseModuleManager(QObject *parent)
    : PropertyBase (parent)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("BASE_MODULE_PARAMS", this);
    PropertyBase::loadJsonConfig(BASE_MODULE_JSON,temp_map);
    qInfo("Server Mode: %d", ServerMode());
    is_init = false;
    profile_loaded = false;
    if(!QDir(".//notopencamera").exists())
    {
        if (ServerMode() == 0) {
            pylonUplookCamera = new BaslerPylonCamera(UPLOOK_VISION_CAMERA);
            pylonDownlookCamera = new BaslerPylonCamera(DOWNLOOK_VISION_CAMERA);
            pylonPickarmCamera = new BaslerPylonCamera(PICKARM_VISION_CAMERA);
        } else{
            pylonDownlookCamera = new BaslerPylonCamera(CAMERA_AA2_DL);
            pylonPickarmCamera = new BaslerPylonCamera(CAMERA_SPA_DL);
        }
    }

    if (ServerMode() == 0) {
        qInfo("This sparrow is in Master mode");
        sparrowQServer = new SparrowQServer(ServerPort());
        sparrowQClient = new SparrowClient(QUrl("ws://localhost:9999"), true);
    } else {
        sparrowQClient = new SparrowClient(QUrl(ServerURL()), true);
    }
    lightingModule = new WordopLight(ServerMode());
    visionModule = new VisionModule(pylonDownlookCamera, pylonUplookCamera, pylonPickarmCamera);
    dothinkey = new Dothinkey();
    imageGrabberThread = new ImageGrabbingWorkerThread(dothinkey);

    if(!QDir(".//notopencamera").exists())
    {
        if(pylonUplookCamera) pylonUplookCamera->start();
        if(pylonDownlookCamera) pylonDownlookCamera->start();
        if(pylonPickarmCamera) pylonPickarmCamera->start();
    }

//    calibrations.insert(AA1_UPLOOK_CALIBRATION,new Calibration(AA1_UPLOOK_CALIBRATION,CALIBRATION_RESULT_PATH));
//    calibrations.insert(AA1_DOWNLOOK_CALIBRATION,new Calibration(AA1_DOWNLOOK_CALIBRATION,CALIBRATION_RESULT_PATH));
//    calibrations.insert(AA1_UPDownLOOK_UP_CALIBRATION,new Calibration(AA1_UPDownLOOK_UP_CALIBRATION,CALIBRATION_RESULT_PATH));
//    calibrations.insert(AA1_UPDownLOOK_DOWN_CALIBRATION,new Calibration(AA1_UPDownLOOK_DOWN_CALIBRATION,CALIBRATION_RESULT_PATH));
//    calibrations.insert(AA1_MUSHROOMHEAD_CALIBRATION,new Calibration(AA1_MUSHROOMHEAD_CALIBRATION,CALIBRATION_RESULT_PATH));
//    calibrations.insert(LPA_LENS_CALIBRATION,new Calibration(LPA_LENS_CALIBRATION,CALIBRATION_RESULT_PATH));

    chart_calibration = new ChartCalibration(dothinkey, AA_MAX_INTENSITY, AA_MIN_AREA, AA_MAX_AREA, CHART_CALIBRATION, CALIBRATION_RESULT_PATH);

    material_tray.standards_parameters.setTrayCount(2);
}

BaseModuleManager::~BaseModuleManager()
{
    QString temp;
    for (int i = 0; i < motors.size(); ++i) {
        temp = motors.keys()[i];
        if(temp == "SUT1_Z"||temp == "LUT_Z")
            delete  GetVcMotorByName(temp);
        else
            delete  GetMotorByName(temp);
    }
    for (int i = 0; i < output_ios.size(); ++i)
        delete  GetOutputIoByName(output_ios.keys()[i]);
    for (int i = 0; i < input_ios.size(); ++i)
        delete  GetInputIoByName(input_ios.keys()[i]);
    for (int i = 0; i < calibrations.size(); ++i)
        delete  calibrations[calibrations.keys()[i]];
    delete chart_calibration;
}

bool BaseModuleManager::loadParameters()
{
//    QMap<QString,PropertyBase*> temp_map;
//    temp_map.insert("BASE_MODULE_PARAMS", this);
//    PropertyBase::loadJsonConfig(BASE_MODULE_JSON,temp_map);

    material_tray.loadJsonConfig();
    aa_head_module.loadJsonConfig();
    sut_module.loadParams();
    lut_module.loadParams();
    dothinkey->loadParams();
    dispenser.parameters.loadJsonConfig(DISPENSER_PARAMETER_PATH,DISPENSER_PARAMETER);
    dispense_module.parameters.loadJsonConfig(DISPENSE_MODULE_PARAMETER_PATH,DISPENSER_MODULE_PARAMETER);
    material_tray.loadJsonConfig();
    lens_loader_module.loadJsonConfig();
    lut_carrier.parameters.loadJsonConfig(LUT_CARRIER_FILE_NAME,"lut");
    sut_carrier.parameters.loadJsonConfig(SUT_CARRIER_FILE_NAME,"sut");
    LoadVcmFile(VCM_PARAMETER_FILENAME);
    loadCylinderFiles(CYLINDER_PARAMETER_FILENAME);
    loadVacuumFiles(VACUUM_PARAMETER_FILENAME);
    loadVisionLoactionFiles(VISION_LOCATION_PARAMETER_FILENAME);
    loadCalibrationFiles(CALIBRATION_PARAMETER_FILENAME);
    if(!InitStruct())
        return false;
    return true;
}

bool BaseModuleManager::SaveParameters()
{
    material_tray.saveJsonConfig();
    aa_head_module.saveJsonConfig();
    sut_module.saveJsonConfig();
    lut_module.saveJsonConfig();
    dothinkey->saveJsonConfig();
    dispenser.parameters.saveJsonConfig(DISPENSER_PARAMETER_PATH,DISPENSER_PARAMETER);
    dispense_module.parameters.saveJsonConfig(DISPENSE_MODULE_PARAMETER_PATH,DISPENSER_MODULE_PARAMETER);
    material_tray.saveJsonConfig();
    lens_loader_module.saveJsonConfig();
    lut_carrier.parameters.saveJsonConfig(LUT_CARRIER_FILE_NAME,"lut");
    sut_carrier.parameters.saveJsonConfig(SUT_CARRIER_FILE_NAME,"sut");
    saveCalibrationFiles(CALIBRATION_PARAMETER_FILENAME);
    saveVisionLoactionFiles(VISION_LOCATION_PARAMETER_FILENAME);
    return true;
}

bool BaseModuleManager::LoadProfile()
{
    if(profile_loaded)
        return true;
    profile_loaded = false;
    int res;
    QString temp = QCoreApplication::applicationDirPath();
    if(temp == QDir::currentPath())
    {
        LPWSTR path = profile_path1;
        res = XT_Controler_Extend::Profile_Load(path);
    }
    else
    {
        LPWSTR path = profile_path2;
        res = XT_Controler_Extend::Profile_Load(path);
    }
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
        if(loadParameters())return false;
        profile_loaded = true;
        return true;
    }
    InitStruct();
    return false;
}

bool BaseModuleManager::LoadVcmFile(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
        saveVcmfile(file_name);
        return false;
    }
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
    else
    {
        VcMotorParameter temp_param;
        QString motor_name = temp_param.motorName();
        QJsonArray json;
        for (int i = 0; i < 4; ++i) {
            QJsonObject temp_object;
            QString temp_name = motor_name;
            temp_name.append(QString::number(i));
            temp_param.setMotorName(temp_name);
            temp_param.write(temp_object);
            json.append(temp_object);
        }
        return  saveJsonArray(file_name,json);
    }
}

bool BaseModuleManager::loadVacuumFiles(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
        saveVacuumFiles(file_name);
        return false;
    }
    for (int i = 0; i < array.count(); i++)
    {
        XtVacuum* temp_vacuum = new XtVacuum();
        temp_vacuum->parameters.read(array.at(i).toObject());
        QJsonObject temp_object;
        temp_vacuum->parameters.write(temp_object);
        if(!motors.contains(temp_vacuum->parameters.vacuumName()))
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
    else
    {
        XtVacuumParameter temp_param;
        QString vcauum_name = temp_param.vacuumName();
        QJsonArray json;
        for (int i = 0; i < 6; ++i) {
            QJsonObject temp_object;
            QString temp_name = vcauum_name;
            temp_name.append(QString::number(i));
            temp_param.setVacuumName(temp_name);
            temp_param.write(temp_object);
            json.append(temp_object);
        }
        return  saveJsonArray(file_name,json);
    }
}

bool BaseModuleManager::loadCylinderFiles(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
        saveCylinderFiles(file_name);
        return false;
    }
    for (int i = 0; i < array.count(); i++)
    {
        XtCylinder* temp_cylinder = new XtCylinder();
        temp_cylinder->parameters.read(array.at(i).toObject());
        QJsonObject temp_object;
        temp_cylinder->parameters.write(temp_object);
        if(!motors.contains(temp_cylinder->parameters.cylinderName()))
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
    foreach (QString temp_name, vacuums.keys()) {
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
    else
    {
        XtCylinderParameter temp_param;
        QString vcauum_name = temp_param.cylinderName();
        QJsonArray json;
        for (int i = 0; i < 6; ++i) {
            QJsonObject temp_object;
            QString temp_name = vcauum_name;
            temp_name.append(QString::number(i));
            temp_param.setCylinderName(temp_name);
            temp_param.write(temp_object);
            json.append(temp_object);
        }
        return  saveJsonArray(file_name,json);
    }
}

bool BaseModuleManager::loadVisionLoactionFiles(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
        saveVisionLoactionFiles(file_name);
        return false;
    }
    for (int i = 0; i < array.count(); i++)
    {
        VisionLocation* temp_location = new VisionLocation();
        temp_location->parameters.read(array.at(i).toObject());
        QJsonObject temp_object;
        temp_location->parameters.write(temp_object);
        if(!motors.contains(temp_location->parameters.locationName()))
            vision_locations.insert(temp_location->parameters.locationName(),temp_location);
        else
        {
            qInfo("vcm motor param name(%s)repeat!",temp_location->parameters.locationName().toStdString().c_str());
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
    else
    {
        VisionLocationParameter temp_param;
        QString location_name = temp_param.locationName();
        QJsonArray json;
        for (int i = 0; i < 6; ++i) {
            QJsonObject temp_object;
            QString temp_name = location_name;
            temp_name.append(QString::number(i));
            temp_param.setLocationName(temp_name);
            temp_param.write(temp_object);
            json.append(temp_object);
        }
        return  saveJsonArray(file_name,json);
    }
}

bool BaseModuleManager::loadCalibrationFiles(QString file_name)
{
    QJsonArray array;
    if(!loadJsonArray(file_name,array))
    {
        saveCalibrationFiles(file_name);
        return false;
    }
    for (int i = 0; i < array.count(); i++)
    {
        QJsonObject data_object =array.at(i).toObject();
        Calibration* temp_calibration;
        if(data_object["calibrationName"].toString() == "aa2_chart_calibration")
        {
            qInfo("get chart calibration");
            temp_calibration = chart_calibration;
        }
        else
            temp_calibration = new Calibration();
        temp_calibration->parameters.read(data_object);
        QJsonObject temp_object;
        temp_calibration->parameters.write(temp_object);
        if(!motors.contains(temp_calibration->parameters.calibrationName()))
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
    else
    {
        CalibrationParameter temp_param;
        QString caibration_name = temp_param.calibrationName();
        QJsonArray json;
        for (int i = 0; i < 6; ++i) {
            QJsonObject temp_object;
            QString temp_name = caibration_name;
            temp_name.append(QString::number(i));
            temp_param.setCalibrationName(temp_name);
            temp_param.write(temp_object);
            json.append(temp_object);
        }
        return  saveJsonArray(file_name,json);
    }
}

//bool BaseModuleManager::LoadVcmFile()
//{
//    QString val;
//    QFile file;
//    file.setFileName(VCM_PARAMETER_FILENAME);
//    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        if(!file.open(QFile::ReadWrite)){
//            return false;
//        }
//        VcMotorParameter temp_param;
//        QString motor_name = temp_param.motorName();
//        QJsonArray json;
//        for (int i = 0; i < 4; ++i) {
//            QJsonObject temp_object;
//            QString temp_name = motor_name;
//            temp_name.append(QString::number(i));
//            temp_param.setMotorName(temp_name);
//            temp_param.write(temp_object);
//            json.append(temp_object);
//        }
//        QJsonDocument document;
//        document.setArray(json);
//        QJsonDocument saveDoc(json);
//        file.write(document.toJson());
//        file.close();
//        return false;
//    }
//    val = file.readAll();
//    file.close();

//    QJsonParseError error;
//    QJsonDocument doucment = QJsonDocument::fromJson(val.toUtf8(), &error);
//    if (!doucment.isNull() && (error.error == QJsonParseError::NoError)) { //解析否出现错误
//        if (doucment.isArray()) { // 数组判断
//            QJsonArray array = doucment.array(); // 转数组
//            QJsonArray array_new;
//            for (int i = 0; i < array.count(); i++)
//            {
//                XtVcMotor* temp_motor = new XtVcMotor();
//                temp_motor->parameters.read(array.at(i).toObject());
//                temp_motor->Init();
//                QJsonObject temp_object;
//                temp_motor->parameters.write(temp_object);
//                array_new.append(temp_object);
//                if(!motors.contains(temp_motor->parameters.motorName()))
//                    motors.insert(temp_motor->parameters.motorName(),temp_motor);
//                else
//                    delete temp_motor;
//            }

//            QFile saveFile(VCM_PARAMETER_FILENAME);
//            if (saveFile.open(QIODevice::WriteOnly)) {
//                QJsonDocument saveDoc(array_new);
//                saveFile.write(saveDoc.toJson());
//                saveFile.close();
//            }
//        }
//    }
//    return true;
//}

bool BaseModuleManager::loadJsonArray(QString file_name,QJsonArray &array)
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
        if (doucment.isArray()) { // 数组判断
            array = doucment.array(); // 转数组
        }
    }
    if(array.size()>0)
        return true;
    else
    {
         qInfo("load parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
}

bool BaseModuleManager::saveJsonArray(QString file_name,QJsonArray &array)
{
    QFile file;
    file.setFileName(file_name);
    if(!file.open(QFile::ReadWrite)){
        qWarning("save parameters to %s failed, Couldn't open save file.",file_name.toStdString().data());
        return false;
    }
    QJsonDocument document;
    document.setArray(array);
    file.write(document.toJson());
    file.close();
    return true;
}

bool BaseModuleManager::InitStruct()
{
    foreach (XtVacuum* temp_vacuum, vacuums.values()) {
        temp_vacuum->Init(GetOutputIoByName(temp_vacuum->parameters.outIoName()),
                          GetInputIoByName(temp_vacuum->parameters.inIoName()),
                          GetOutputIoByName(temp_vacuum->parameters.breakIoName()));
    }

    foreach (XtCylinder* temp_cylinder, cylinder.values()) {
        temp_cylinder->Init(GetOutputIoByName(temp_cylinder->parameters.oneInName()),
                            GetInputIoByName(temp_cylinder->parameters.oneInName()),
                            GetInputIoByName(temp_cylinder->parameters.zeroInName()),
                            GetOutputIoByName(temp_cylinder->parameters.zeroOutName()));
    }
    lut_carrier.Init("lut_carrier",GetMotorByName(lut_module.parameters.motorXName()),
                     GetMotorByName(lut_module.parameters.motorYName()),
                     GetVcMotorByName(lut_module.parameters.motorZName()),
                     GetVacuumByName(lut_module.parameters.vacuum1Name()));
    sut_carrier.Init("sut_carrier",GetMotorByName(sut_module.parameters.motorXName()),
                     GetMotorByName(sut_module.parameters.motorYName()),
                     GetVcMotorByName(sut_module.parameters.motorZName()),
                     GetVacuumByName(sut_module.parameters.vacuumName()));
    lens_picker.Init(GetVcMotorByName(lens_pick_arm.parameters.motorXName()),
                     GetMotorByName(lens_pick_arm.parameters.motorTName()),
                     GetVacuumByName(lens_pick_arm.parameters.vacuumName()));
    lens_pick_arm.Init(GetMotorByName(lens_pick_arm.parameters.motorTrayName()),
                       GetMotorByName(lens_pick_arm.parameters.motorXName()),
                       GetMotorByName(lens_pick_arm.parameters.motorYName()),
                       &lens_picker);
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
                        XtMotor::GetThreadResource());

    foreach (Calibration* temp_calibraion, calibrations) {
        temp_calibraion->Init(GetMotorByName(temp_calibraion->parameters.motorXName()),
                              GetMotorByName(temp_calibraion->parameters.motorYName()),
                              GetVisionLocationByName(temp_calibraion->parameters.locationName()));
    }
    chart_calibration->Init(GetMotorByName(chart_calibration->parameters.motorXName()),
                           GetMotorByName(chart_calibration->parameters.motorYName()),
                           nullptr);
    foreach (VisionLocation* temp_vision, vision_locations.values()) {
        temp_vision->Init(visionModule,GetPixel2MechByName(temp_vision->parameters.calibrationName()),lightingModule);
    }
//    vision_locations[PR_AA1_TOOL_UPLOOK]->Init(visionModule,calibrations[AA1_UPDownLOOK_UP_CALIBRATION]->getCaliMapping(),lightingModule);
//    vision_locations[PR_AA1_TOOL_DOWNLOOK]->Init(visionModule,calibrations[AA1_UPDownLOOK_DOWN_CALIBRATION]->getCaliMapping(),lightingModule);
//    vision_locations[PR_AA1_LUT_UPLOOK]->Init(visionModule,calibrations[AA1_UPLOOK_CALIBRATION]->getCaliMapping(),lightingModule);
//    vision_locations[PR_SUT_DOWNLOOK]->Init(visionModule,calibrations[AA1_DOWNLOOK_CALIBRATION]->getCaliMapping(),lightingModule);
//    vision_locations[PR_LOAD_LUT_UPLOOK]->Init(visionModule,calibrations[AA1_UPLOOK_CALIBRATION]->getCaliMapping(),lightingModule);
//    vision_locations[PR_AA1_MUSHROOMHEAD]->Init(visionModule,calibrations[AA1_MUSHROOMHEAD_CALIBRATION]->getCaliMapping(),lightingModule);
//    vision_locations[PR_LENS_LPALOOK]->Init(visionModule,calibrations[LPA_LENS_CALIBRATION]->getCaliMapping(),lightingModule);
//    vision_locations[PR_VACANCY_LPALOOK]->Init(visionModule,calibrations[LPA_LENS_CALIBRATION]->getCaliMapping(),lightingModule);
//    vision_locations[PR_LENS_LUTLOOK]->Init(visionModule,calibrations[LPA_LENS_CALIBRATION]->getCaliMapping(),lightingModule);

    lut_module.Init(&lut_carrier,GetVisionLocationByName(PR_AA1_LUT_UPLOOK),
                    GetVisionLocationByName(PR_AA1_TOOL_UPLOOK),
                    GetVisionLocationByName(PR_LOAD_LUT_UPLOOK),
                    GetVisionLocationByName(PR_AA1_MUSHROOMHEAD),
                    GetVacuumByName(lut_module.parameters.vacuum1Name()),
                    GetVacuumByName(lut_module.parameters.vacuum1Name()),
                    GetOutputIoByName(aa_head_module.parameters.gripperName()));
    sut_module.Init(&sut_carrier,GetVisionLocationByName(PR_SUT_DOWNLOOK),
                    GetVisionLocationByName(PR_AA1_TOOL_DOWNLOOK),
                    GetVacuumByName(sut_module.parameters.vacuumName()));
    QVector<XtMotor *> executive_motors;
    executive_motors.push_back(GetMotorByName(lut_module.parameters.motorXName()));
    executive_motors.push_back(GetMotorByName(lut_module.parameters.motorYName()));
    executive_motors.push_back(GetMotorByName(lut_module.parameters.motorZName()));
    dispenser.Init(XtMotor::GetCurveResource(),XtMotor::GetThreadResource(),XtMotor::GetThreadResource(),executive_motors,
                   GetOutputIoByName(dispenser.parameters.dispenseIo()));
    dispense_module.Init(DISPENSER_PARAMETER_PATH,"dispense_module",chart_calibration,&dispenser,visionModule,&sut_carrier,
                         GetOutputIoByName(dispenser.parameters.dispenseIo()));
    dispense_module.setMapPosition(sut_module.downlook_position.X(),sut_module.downlook_position.Y());

    lens_loader_module.Init(&lens_pick_arm,&material_tray,&lut_carrier,GetVisionLocationByName(PR_LENS_LPALOOK),
                            GetVisionLocationByName(PR_VACANCY_LPALOOK),GetVisionLocationByName(PR_LENS_LUTLOOK));
    profile_loaded = true;

    return true;
}

bool BaseModuleManager::initialDevice()
{
    if(is_init)
        return true;
    if(!profile_loaded)
        return false;

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
    return true;
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
    bool result;

    motors["SUT_Z"]->SeekOrigin();
    if (this->ServerMode() == 0) motors["LPA_Z"]->SeekOrigin();
    if (this->ServerMode() == 0) result = motors["LPA_Z"]->WaitSeekDone();
    if(!result)return false;
    if (this->ServerMode() == 0) motors["LPA_X"]->SeekOrigin();
    if (this->ServerMode() == 0) motors["LPA_Y"]->SeekOrigin();
    if (this->ServerMode() == 0) motors["LUT_Y"]->SeekOrigin();
    result = motors["SUT_Z"]->WaitSeekDone();
    if(!result)return false;
    motors["SUT1_Y"]->SeekOrigin();
    if (this->ServerMode() == 0)result &= motors["LUT_Y"]->WaitSeekDone();
    result &= motors["SUT1_Y"]->WaitSeekDone();
    if (this->ServerMode() == 0) result &= motors["LPA_Y"]->WaitSeekDone();
    if(!result)return false;
    //降下气缸
    if (this->ServerMode() == 0) motors["LTL_X"]->SeekOrigin();
    motors["AA1_Y"]->SeekOrigin();
    result = motors["AA1_Y"]->WaitSeekDone();
    if(!result)return false;
    if (this->ServerMode() == 0) motors["LUT_Z"]->SeekOrigin();
    motors["AA1_X"]->SeekOrigin();
    motors["AA1_Z"]->SeekOrigin();
    motors["AA1_A"]->SeekOrigin();
    motors["AA1_B"]->SeekOrigin();
    motors["AA1_C"]->SeekOrigin();
    motors["SUT1_X"]->SeekOrigin();
    if (this->ServerMode() == 0) motors["LUT_X"]->SeekOrigin();
    result &= motors["AA1_X"]->WaitSeekDone();
    result &= motors["AA1_Z"]->WaitSeekDone();
    result &= motors["AA1_A"]->WaitSeekDone();
    result &= motors["AA1_B"]->WaitSeekDone();
    result &= motors["AA1_C"]->WaitSeekDone();
    result &= motors["SUT1_X"]->WaitSeekDone();
    if (this->ServerMode() == 0) result &= motors["LUT_X"]->WaitSeekDone();
    if (this->ServerMode() == 0) result &= motors["LUT_Z"]->WaitSeekDone();
    if (this->ServerMode() == 0) result &= motors["LPA_X"]->WaitSeekDone();
    if (this->ServerMode() == 0) result &= motors["LTL_X"]->WaitSeekDone();
    if(!result)return false;
    if (this->ServerMode() == 0)GetVcMotorByName("LUT_Z")->ChangeDiretion();
    qInfo("all motor seeked origin successed!");
    return  true;
}

void BaseModuleManager::stopSeeking()
{
    if(is_init)
        foreach (XtMotor *m, motors.values()) {
            m->StopSeeking();
            m->Disable();
        }
}

void BaseModuleManager::performUplookCalibration()
{
    if(calibrations.contains(AA1_UPLOOK_CALIBRATION))
        calibrations[AA1_UPLOOK_CALIBRATION]->performCalibration();
}

void BaseModuleManager::performDownlookCalibration()
{
    if(calibrations.contains(AA1_DOWNLOOK_CALIBRATION))
        calibrations[AA1_DOWNLOOK_CALIBRATION]->performCalibration();
}

void BaseModuleManager::performUpDownlookCalibration()
{
    if(calibrations.contains(AA1_UPDownLOOK_DOWN_CALIBRATION))
        calibrations[AA1_UPDownLOOK_DOWN_CALIBRATION]->performCalibration();
}

void BaseModuleManager::performUpDownlookUpCalibration()
{
    if(calibrations.contains(AA1_UPDownLOOK_UP_CALIBRATION))
        calibrations[AA1_UPDownLOOK_UP_CALIBRATION]->performCalibration();
}

void BaseModuleManager::performAA1MushroomHeadCalibration()
{
    if(calibrations.contains(AA1_MUSHROOMHEAD_CALIBRATION))
        calibrations[AA1_MUSHROOMHEAD_CALIBRATION]->performCalibration();
}

void BaseModuleManager::performLPALensCalibration()
{
    if(calibrations.contains(LPA_LENS_CALIBRATION))
        calibrations[LPA_LENS_CALIBRATION]->performCalibration();
}

void BaseModuleManager::performChartCalibration()
{
    qInfo("perform Chart Calibration");
    if (chart_calibration)
        chart_calibration->performCalibration(0.1, 0.1);
}

void BaseModuleManager::UpdateCalibrationParameters()
{

}

int BaseModuleManager::getNumberOfMotors()
{
    return motors.size();
}

QString BaseModuleManager::getMotorsName(int index)
{
    return motors.keys()[index];
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
    if(output_ios.contains(name))
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
        qInfo("can not find vision location io %s",name.toStdString().c_str());
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

void BaseModuleManager::motorSeekOrigin(QString name)
{
    if (motors.contains(name)) {
        motors[name]->SeekOrigin();
    }
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

void BaseModuleManager::testCommand()
{
    QJsonObject obj = this->sparrowQServer->commandDequeue();
    qInfo("CMD: %s", obj["cmd"].toString().toStdString().c_str());
}
