#include "XtVcMotor.h"
#include "basemodulemanager.h"
#include "xtvcmotorparameter.h"

#include <QMessageBox>
#include <qcoreapplication.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qjsonarray.h>
#include <qjsondocument.h>

wchar_t BaseModuleManager::ip[] =  L"192.168.8.251";
wchar_t BaseModuleManager::profile_path1[] = L".\\config\\xt_motion_config.csv";
wchar_t BaseModuleManager::profile_path2[] = L"..\\config\\xt_motion_config.csv";

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

    lightingModule = new WordopLight(ServerMode());
    visionModule = new VisionModule(pylonDownlookCamera, pylonUplookCamera, pylonPickarmCamera);
    dothinkey = new Dothinkey();
    imageGrabberThread = new ImageGrabbingWorkerThread(dothinkey);

    if (ServerMode() == 0) {
        qInfo("This sparrow is in Master mode");
        this->lut_module.openServer(19998);
        lutClient = new LutClient(&this->aa_head_module, "ws://localhost:19998");
        sut_clitent = new SutClient("ws://192.168.0.251:19999");
        connect(&lut_module,&LutModule::sendLoadLensRequst,&lens_loader_module,&LensLoaderModule::receiveLoadLensRequst,Qt::DirectConnection);
        connect(&lens_loader_module,&LensLoaderModule::sendLoadLensRequstFinish,&lut_module,&LutModule::receiveLoadLensRequstFinish,Qt::DirectConnection);

    } else {
        qInfo("This sparrow is in Slave mode");
        this->sensor_loader_module.openServer(19999);
        lutClient = new LutClient(&this->aa_head_module, "ws://192.168.0.250:19998");
        sut_clitent = new SutClient("ws://localhost:19999");
    }
    connect(&sut_module,&SutModule::sendLoadSensorFinish,&aa_head_module,&AAHeadModule::receiveSensorFromSut,Qt::DirectConnection);
    connect(&aa_head_module,&AAHeadModule::sendSensrRequestToSut,&sut_module,&SutModule::receiveLoadSensorRequst,Qt::DirectConnection);

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
    unitlog.setServerAddress(DataServerURL());
    connect(this,&BaseModuleManager::sendMsgSignal,this,&BaseModuleManager::sendMessageTest);
}

BaseModuleManager::~BaseModuleManager()
{
    //    QString temp;
    //    for (int i = 0; i < motors.size(); ++i) {
    //        temp = motors.keys()[i];
    //        if(temp == "SUT1_Z"||temp == "LUT_Z")
    //            delete  GetVcMotorByName(temp);
    //        else
    //            delete  GetMotorByName(temp);
    //    }
    //    for (int i = 0; i < output_ios.size(); ++i)
    //        delete  GetOutputIoByName(output_ios.keys()[i]);
    //    for (int i = 0; i < input_ios.size(); ++i)
    //        delete  GetInputIoByName(input_ios.keys()[i]);
    //    for (int i = 0; i < calibrations.size(); ++i)
    //        delete  calibrations[calibrations.keys()[i]];
    //    delete chart_calibration;
}

bool BaseModuleManager::sendMessageTest(QString title, QString content)
{
    QMessageBox::StandardButton rb = QMessageBox::information(nullptr,"title","content",QMessageBox::Yes|QMessageBox::No);
    if(rb==QMessageBox::Yes){
        return true;
    }else{
        return false;
    }
}

bool BaseModuleManager::loadParameters()
{

    material_tray.loadJsonConfig();
    aa_head_module.loadJsonConfig();
    sut_module.loadParams();
    dothinkey->loadParams();
    dispenser.parameters.loadJsonConfig(DISPENSER_PARAMETER_PATH,DISPENSER_PARAMETER);
    dispense_module.parameters.loadJsonConfig(DISPENSE_MODULE_PARAMETER_PATH,DISPENSER_MODULE_PARAMETER);

    tray_loader_module.parameters.loadJsonConfig(TRAY_LOADER_PATH,TRAY_LOADER_PARAMETER);
    trayClipIn.standards_parameters.loadJsonConfig(TRAY_CLIPIN_PATH,TRAY_CLIPIN_PARAMETER);
    trayClipOut.standards_parameters.loadJsonConfig(TRAY_CLIPOUT_PATH,TRAY_CLIPOUT_PARAMETER);

    material_tray.loadJsonConfig();
    if(ServerMode())
    {
        sensor_loader_module.loadJsonConfig();
        sensor_pickarm.parameters.loadJsonConfig(SENSOR_PICKARM_FILE_NAME,"sensor_pickarm");
    }
    else
    {
        lut_module.loadParams();
        lut_carrier.parameters.loadJsonConfig(LUT_CARRIER_FILE_NAME,"lut");
        lens_loader_module.loadJsonConfig();
        lens_pick_arm.parameters.loadJsonConfig(LENS_PICKARM_FILE_NAME,"lens_pickarm");
    }
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
    lens_pick_arm.parameters.saveJsonConfig(LENS_PICKARM_FILE_NAME,"lens_pickarm");
    lut_carrier.parameters.saveJsonConfig(LUT_CARRIER_FILE_NAME,"lut");
    sut_carrier.parameters.saveJsonConfig(SUT_CARRIER_FILE_NAME,"sut");
    if(ServerMode())
    {
        sensor_loader_module.saveJsonConfig();
        sensor_pickarm.parameters.saveJsonConfig(SENSOR_PICKARM_FILE_NAME,"sensor_pickarm");
    }
    else
    {
        lut_module.saveJsonConfig();
        lut_carrier.parameters.saveJsonConfig(LUT_CARRIER_FILE_NAME,"lut");
        lens_loader_module.saveJsonConfig();
        lens_pick_arm.parameters.saveJsonConfig(LENS_PICKARM_FILE_NAME,"lens_pickarm");
    }
    tray_loader_module.parameters.saveJsonConfig(TRAY_LOADER_PATH,TRAY_LOADER_PARAMETER);
    trayClipIn.standards_parameters.saveJsonConfig(TRAY_CLIPIN_PATH,TRAY_CLIPIN_PARAMETER);
    trayClipOut.standards_parameters.saveJsonConfig(TRAY_CLIPOUT_PATH,TRAY_CLIPOUT_PARAMETER);

    saveCalibrationFiles(CALIBRATION_PARAMETER_FILENAME);
    saveVisionLoactionFiles(VISION_LOCATION_PARAMETER_FILENAME);
    return true;
}

bool BaseModuleManager::registerWorkers(WorkersManager *manager)
{
    bool result = true;
    if (this->ServerMode() == 0)
    {
        result &= manager->registerWorker(&lens_loader_module);
        result &= manager->registerWorker(&lut_module);
    }
    else {
        result &= manager->registerWorker(&sensor_loader_module);
    }
    result &= manager->registerWorker(&sut_module);
    result &= manager->registerWorker(&aaCoreNew);
    return result;
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
        if(!vision_locations.contains(temp_location->parameters.locationName()))
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
        temp_cylinder->Init(GetOutputIoByName(temp_cylinder->parameters.oneOutName()),
                            GetInputIoByName(temp_cylinder->parameters.oneInName()),
                            GetInputIoByName(temp_cylinder->parameters.zeroInName()),
                            GetOutputIoByName(temp_cylinder->parameters.zeroOutName()));
    }
    sut_carrier.Init("sut_carrier",GetMotorByName(sut_module.parameters.motorXName()),
                     GetMotorByName(sut_module.parameters.motorYName()),
                     GetVcMotorByName(sut_module.parameters.motorZName()),
                     GetVacuumByName(sut_module.parameters.vacuumName()));
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
    if (chart_calibration) {
        chart_calibration->Init(GetMotorByName(chart_calibration->parameters.motorXName()),
                                GetMotorByName(chart_calibration->parameters.motorYName()),
                                nullptr);
    }
    foreach (VisionLocation* temp_vision, vision_locations.values()) {
        temp_vision->Init(visionModule,GetPixel2MechByName(temp_vision->parameters.calibrationName()),lightingModule);
    }
    sut_clitent->Init(GetVacuumByName(sut_module.parameters.vacuumName()));
    sut_module.Init(&sut_carrier,sut_clitent,
                    GetVisionLocationByName(sut_module.parameters.downlookLocationName()),
                    GetVisionLocationByName(sut_module.parameters.updownlookDownLocationName()),
                    GetVisionLocationByName(sut_module.parameters.updownlookUpLocationName()),
                    GetVacuumByName(sut_module.parameters.vacuumName()),
                    GetCylinderByName(sut_module.parameters.cylinderName()));
    QVector<XtMotor *> executive_motors;
    executive_motors.push_back(GetMotorByName(sut_module.parameters.motorXName()));
    executive_motors.push_back(GetMotorByName(sut_module.parameters.motorYName()));
    executive_motors.push_back(GetMotorByName(sut_module.parameters.motorZName()));
    dispenser.Init(XtMotor::GetCurveResource(),XtMotor::GetThreadResource(),XtMotor::GetThreadResource(),executive_motors,
                   GetOutputIoByName(dispenser.parameters.dispenseIo()));
    dispense_module.Init(DISPENSER_PARAMETER_PATH,"dispense_module",
                         GetCalibrationByName(GetVisionLocationByName(sut_module.parameters.downlookLocationName())->parameters.calibrationName()),
                         &dispenser,visionModule,&sut_carrier,
                         GetOutputIoByName(dispenser.parameters.dispenseIo()));
    dispense_module.setMapPosition(sut_module.downlook_position.X(),sut_module.downlook_position.Y());

    if(ServerMode())
    {
        sensor_picker1.Init(GetVcMotorByName(sensor_pickarm.parameters.motorZName()),
                            GetMotorByName(sensor_pickarm.parameters.motorTName()),
                            GetVacuumByName(sensor_pickarm.parameters.vacuumName()));
        sensor_picker2.Init(GetVcMotorByName(sensor_pickarm.parameters.motorZ2Name()),
                            GetMotorByName(sensor_pickarm.parameters.motorT2Name()),
                            GetVacuumByName(sensor_pickarm.parameters.vacuum2Name()));
        sensor_pickarm.Init(GetMotorByName(sensor_pickarm.parameters.motorXName()),
                            GetMotorByName(sensor_pickarm.parameters.motorYName()),
                            &sensor_picker1,&sensor_picker2);
        sensor_loader_module.Init(&sensor_pickarm,&material_tray,
                                  GetVisionLocationByName(sensor_loader_module.parameters.sensorLocationName()),
                                  GetVisionLocationByName(sensor_loader_module.parameters.vacancyLocationName()),
                                  GetVisionLocationByName(sensor_loader_module.parameters.sutLocationName()),
                                  GetVisionLocationByName(sensor_loader_module.parameters.sutSensorLocationName()),
                                  GetVisionLocationByName(sensor_loader_module.parameters.sutProductLocationName()));
        //connect(&sensor_loader_module,&SensorLoaderModule::sendMsgSignal,this,&BaseModuleManager::sendMessageTest);
    }
    else
    {
        lut_carrier.Init("lut_carrier",GetMotorByName(lut_module.parameters.motorXName()),
                         GetMotorByName(lut_module.parameters.motorYName()),
                         GetVcMotorByName(lut_module.parameters.motorZName()),
                         GetVacuumByName(lut_module.parameters.vacuum1Name()));
        lut_module.Init(&lut_carrier,GetVisionLocationByName(lut_module.parameters.uplookLocationName()),
                        GetVisionLocationByName(lut_module.parameters.loadlookLocationName()),
                        GetVisionLocationByName(lut_module.parameters.mushroomLocationName()),
                        GetVacuumByName(lut_module.parameters.vacuum1Name()),
                        GetVacuumByName(lut_module.parameters.vacuum1Name()),
                        GetOutputIoByName(aa_head_module.parameters.gripperName()));
        lens_picker.Init(GetVcMotorByName(lens_pick_arm.parameters.motorZName()),
                         GetMotorByName(lens_pick_arm.parameters.motorTName()),
                         GetVacuumByName(lens_pick_arm.parameters.vacuumName()));
        lens_pick_arm.Init(GetMotorByName(lens_pick_arm.parameters.motorTrayName()),
                           GetMotorByName(lens_pick_arm.parameters.motorXName()),
                           GetMotorByName(lens_pick_arm.parameters.motorYName()),
                           &lens_picker);
        lens_loader_module.Init(&lens_pick_arm,&material_tray,&lut_carrier,
                                GetVacuumByName(lut_module.parameters.vacuum1Name()),
                                GetVacuumByName(lut_module.parameters.vacuum2Name()),
                                GetVisionLocationByName(lens_loader_module.parameters.lensLocationName()),
                                GetVisionLocationByName(lens_loader_module.parameters.vacancyLocationName()),
                                GetVisionLocationByName(lens_loader_module.parameters.lutLocationName()),
                                GetVisionLocationByName(lens_loader_module.parameters.lutLensLocationName()),
                                GetVisionLocationByName(lens_loader_module.parameters.lpaUpdownlookUpLocationName()),
                                GetVisionLocationByName(lens_loader_module.parameters.lpaUpdownlookDownLocationName()));

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
                            GetInputIoByName(tray_loader_module.parameters.clipoutInputName()));

    sfrWorkerController = new SfrWorkerController(&aaCoreNew);
    aaCoreNew.setSfrWorkerController(sfrWorkerController);
    aaCoreNew.Init(&aa_head_module, lutClient, &sut_module, dothinkey, chart_calibration, &dispense_module, imageGrabberThread, &unitlog);
    //todo
    material_tray.resetTrayState(0);
    material_tray.resetTrayState(1);
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
    if (ServerMode() == 0) {
        return allMotorsSeekOriginal1();
    } else {
        return allMotorsSeekOriginal2();
    }
}

bool BaseModuleManager::allMotorsSeekOriginal1()
{
    bool result;
    GetVcMotorByName(this->lut_module.parameters.motorZName())->resetSoftLanding();

    if(!GetCylinderByName(this->tray_loader_module.parameters.cylinderLTK2Name())->Set(1))
        return false;
    GetMotorByName(this->lut_module.parameters.motorYName())->SeekOrigin();//LUT_Y
    GetMotorByName(this->aa_head_module.parameters.motorYName())->SeekOrigin();//AA_Y
    GetMotorByName(this->sut_module.parameters.motorZName())->SeekOrigin();//SUT_Z
    GetMotorByName(this->lens_pick_arm.parameters.motorZName())->SeekOrigin();//LPA_Z

    result = GetMotorByName(sut_module.parameters.motorZName())->WaitSeekDone();
    if (!result) return false;

    GetMotorByName(this->sut_module.parameters.motorXName())->SeekOrigin();//SUT_X
    GetMotorByName(this->sut_module.parameters.motorYName())->SeekOrigin();//SUT_Y

    result = GetMotorByName(this->lens_pick_arm.parameters.motorZName())->WaitSeekDone();
    if(!result)return false;

    GetMotorByName(this->lens_pick_arm.parameters.motorXName())->SeekOrigin();//LPA_X
    GetMotorByName(this->lens_pick_arm.parameters.motorYName())->SeekOrigin();//LPA_Y

    result = GetMotorByName(this->aa_head_module.parameters.motorYName())->WaitSeekDone();
    if(!result)return false;

    GetMotorByName(this->aa_head_module.parameters.motorXName())->SeekOrigin();//AA_X
    GetMotorByName(this->aa_head_module.parameters.motorZName())->SeekOrigin();//AA_Z
    GetMotorByName(this->aa_head_module.parameters.motorAName())->SeekOrigin();//AA_A
    GetMotorByName(this->aa_head_module.parameters.motorBName())->SeekOrigin();//AA_B
    GetMotorByName(this->aa_head_module.parameters.motorCName())->SeekOrigin();//AA_C

    //缩回气缸
    GetCylinderByName(this->tray_loader_module.parameters.cylinderClipName())->Set(0);

    GetMotorByName(this->tray_loader_module.parameters.motorLTIEName())->SeekOrigin();//LTIE

    GetMotorByName(this->tray_loader_module.parameters.motorLTOEName())->SeekOrigin();//LTOE

    result = GetMotorByName(this->lut_module.parameters.motorYName())->WaitSeekDone();
    if(!result)return false;
    GetMotorByName(this->lut_module.parameters.motorXName())->SeekOrigin();//LUT_X
    GetMotorByName(this->lut_module.parameters.motorZName())->SeekOrigin();//LUT_Z


    result = GetMotorByName(this->lens_pick_arm.parameters.motorYName())->WaitSeekDone();
    if(!result)return false;
    //升起气缸,降下托盘
    //
    GetMotorByName(this->lens_pick_arm.parameters.motorTrayName())->SeekOrigin();//LTL


    GetCylinderByName(this->tray_loader_module.parameters.cylinderLTK1Name())->Set(0);
    GetCylinderByName(this->tray_loader_module.parameters.cylinderLTK2Name())->Set(0);
    GetMotorByName(this->tray_loader_module.parameters.motorLTKX1Name())->SeekOrigin();//LTK1
    GetMotorByName(this->tray_loader_module.parameters.motorLTKX2Name())->SeekOrigin();//LTK2


    result &= GetMotorByName(this->sut_module.parameters.motorXName())->WaitSeekDone();
    result &= GetMotorByName(this->sut_module.parameters.motorYName())->WaitSeekDone();

    result &= GetMotorByName(this->lens_pick_arm.parameters.motorXName())->WaitSeekDone();


    result &= GetMotorByName(this->aa_head_module.parameters.motorXName())->WaitSeekDone();
    result &= GetMotorByName(this->aa_head_module.parameters.motorZName())->WaitSeekDone();
    result &= GetMotorByName(this->aa_head_module.parameters.motorAName())->WaitSeekDone();
    result &= GetMotorByName(this->aa_head_module.parameters.motorBName())->WaitSeekDone();
    result &= GetMotorByName(this->aa_head_module.parameters.motorCName())->WaitSeekDone();

    result &= GetMotorByName(this->lut_module.parameters.motorZName())->WaitSeekDone();
    result &= GetMotorByName(this->lut_module.parameters.motorXName())->WaitSeekDone();

    result &= GetMotorByName(this->lens_pick_arm.parameters.motorTrayName())->WaitSeekDone();

    result &= GetMotorByName(this->tray_loader_module.parameters.motorLTKX1Name())->WaitSeekDone();
    result &= GetMotorByName(this->tray_loader_module.parameters.motorLTKX2Name())->WaitSeekDone();
    result &= GetMotorByName(this->tray_loader_module.parameters.motorLTIEName())->WaitSeekDone();
    result &= GetMotorByName(this->tray_loader_module.parameters.motorLTOEName())->WaitSeekDone();
    if(result)
    {
        qInfo("all motors seeked origin");
        setHomeState(true);
        return true;
    }
    return false;
}

bool BaseModuleManager::allMotorsSeekOriginal2()
{
    qInfo("allMotorsSeekOriginal2 Start");
    //推料氣缸復位
    bool result;
    GetMotorByName(sut_module.parameters.motorZName())->SeekOrigin();
    GetMotorByName(sensor_pickarm.parameters.motorZName())->SeekOrigin();
    GetMotorByName(sensor_pickarm.parameters.motorZ2Name())->SeekOrigin();
    result = GetMotorByName(sut_module.parameters.motorZName())->WaitSeekDone();
    if(!result) return false;
    GetMotorByName(sut_module.parameters.motorYName())->SeekOrigin();

    result = GetMotorByName(sensor_pickarm.parameters.motorZName())->WaitSeekDone();
    result &= GetMotorByName(sensor_pickarm.parameters.motorZ2Name())->WaitSeekDone();
    if(!result) return false;
    //KicK復位
    GetMotorByName(this->sensor_pickarm.parameters.motorXName())->SeekOrigin();
    GetMotorByName(this->sensor_pickarm.parameters.motorYName())->SeekOrigin();
    GetMotorByName(this->sensor_pickarm.parameters.motorTName())->SeekOrigin();
    GetMotorByName(this->sensor_pickarm.parameters.motorT2Name())->SeekOrigin();

    result &= GetMotorByName(sut_module.parameters.motorYName())->WaitSeekDone();
    GetMotorByName(this->aa_head_module.parameters.motorYName())->SeekOrigin();

    result &= GetMotorByName(this->aa_head_module.parameters.motorYName())->WaitSeekDone();
    if(!result)return false;
    GetMotorByName(this->aa_head_module.parameters.motorXName())->SeekOrigin();
    GetMotorByName(this->aa_head_module.parameters.motorZName())->SeekOrigin();
    GetMotorByName(this->aa_head_module.parameters.motorAName())->SeekOrigin();
    GetMotorByName(this->aa_head_module.parameters.motorBName())->SeekOrigin();
    GetMotorByName(this->aa_head_module.parameters.motorCName())->SeekOrigin();
    GetMotorByName(this->sut_module.parameters.motorXName())->SeekOrigin();

    result &= GetMotorByName(this->sensor_pickarm.parameters.motorYName())->WaitSeekDone();
    if(!result)return false;
//    GetMotorByName(this->sensor_pickarm.parameters.motorTrayName())->SeekOrigin();


    result &= GetMotorByName(this->aa_head_module.parameters.motorXName())->WaitSeekDone();
    result &= GetMotorByName(this->aa_head_module.parameters.motorZName())->WaitSeekDone();
    result &= GetMotorByName(this->aa_head_module.parameters.motorAName())->WaitSeekDone();
    result &= GetMotorByName(this->aa_head_module.parameters.motorBName())->WaitSeekDone();
    result &= GetMotorByName(this->aa_head_module.parameters.motorCName())->WaitSeekDone();
    result &= GetMotorByName(this->sut_module.parameters.motorXName())->WaitSeekDone();
    result &= GetMotorByName(this->sensor_pickarm.parameters.motorXName())->WaitSeekDone();
    result &= GetMotorByName(this->sensor_pickarm.parameters.motorTName())->WaitSeekDone();
    result &= GetMotorByName(this->sensor_pickarm.parameters.motorT2Name())->WaitSeekDone();
//    result &= GetMotorByName(this->sensor_pickarm.parameters.motorTrayName())->WaitSeekDone();

    if(result)
    {
        qInfo("all motor seeked origin successed!");
        setHomeState(true);
        return  true;
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
    SaveParameters();
    //    loadParameters();
}

void BaseModuleManager::loadFlowchart(QString json)
{
    qInfo("Load flowchart: %s", json.toStdString().c_str());
    aaCoreNew.setFlowchartDocument(json);
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

bool BaseModuleManager::performCalibration(QString calibration_name)
{
    if(calibration_name.contains("chart_calibration"))
        return   chart_calibration->performCalibration();
    Calibration* temp_caliration = GetCalibrationByName(calibration_name);
    if(temp_caliration == nullptr)
    {
        qInfo("not calibration %s",calibration_name.toStdString().c_str());
        return  false;
    }
    return  temp_caliration->performCalibration();
}

bool BaseModuleManager::performLocation(QString location_name)
{
//    if(!emit sendMsgSignal("title","content")){
//        return true;
//    }
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
        if(!emit sendMsgSignal(tr("无标题"),tr("x: %1,y:%2,theta:%3,是否移动").arg(offset.X).arg(offset.Y).arg(offset.Theta))){
            return true;
        }
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
    if (motors.contains(name)) {
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
    material_tray.loadJsonConfig();
    sensor_loader_module.loadJsonConfig();
}
