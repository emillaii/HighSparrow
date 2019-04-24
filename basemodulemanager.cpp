#include "XtVcMotor.h"
#include "basemodulemanager.h"
#include "xtvcmotorparameter.h"

#include <qdir.h>
#include <qjsonarray.h>
#include <qjsondocument.h>

wchar_t BaseModuleManager::ip[] =  L"192.168.8.251";
wchar_t BaseModuleManager::profile_path[] = L"./xt_motion_config.csv";
BaseModuleManager::BaseModuleManager(QObject *parent)
    : PropertyBase (parent)
{
    is_init = false;
    profile_loaded = false;
    if(!QDir(".//notopencamera").exists())
    {
        pylonUplookCamera = new BaslerPylonCamera(UPLOOK_VISION_CAMERA);
        pylonDownlookCamera = new BaslerPylonCamera(DOWNLOOK_VISION_CAMERA);
        pylonPickarmCamera = new BaslerPylonCamera(PICKARM_VISION_CAMERA);
    }

    if (ServerMode() == 0) {
        qInfo("This sparrow is in Master mode");
        sparrowQServer = new SparrowQServer(ServerPort());
        sparrowQClient = new SparrowClient(QUrl("ws://localhost:9999"), true);
    } else {
        sparrowQClient = new SparrowClient(QUrl(ServerURL()), true);
    }
    lightingModule = new WordopLight();
    visionModule = new VisionModule(pylonDownlookCamera, pylonUplookCamera, pylonPickarmCamera);
    dothinkey = new Dothinkey();
    imageGrabberThread = new ImageGrabbingWorkerThread(dothinkey);

    if(!QDir(".//notopencamera").exists())
    {
        pylonUplookCamera->start();
        pylonDownlookCamera->start();
        pylonPickarmCamera->start();
    }
    vision_locations.insert(PR_AA1_TOOL_UPLOOK,new VisionLocation());
    vision_locations.insert(PR_AA1_TOOL_DOWNLOOK,new VisionLocation());
    vision_locations.insert(PR_AA1_LUT_UPLOOK,new VisionLocation());
//    vision_locations.insert(PR_AA2_TOOL_UPLOOK,new VisionLocation());
//    vision_locations.insert(PR_AA2_TOOL_DOWNLOOK,new VisionLocation());
//    vision_locations.insert(PR_AA2_LUT_UPLOOK,new VisionLocation());
    vision_locations.insert(PR_SUT_DOWNLOOK,new VisionLocation());
    vision_locations.insert(PR_LOAD_LUT_UPLOOK,new VisionLocation());
    vision_locations.insert(PR_AA1_MUSHROOMHEAD,new VisionLocation());
    vision_locations.insert(PR_LENS_LPALOOK,new VisionLocation());
    vision_locations.insert(PR_VACANCY_LPALOOK,new VisionLocation());
    vision_locations.insert(PR_LENS_LUTLOOK,new VisionLocation());
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
    delete chartCalibration;
}

bool BaseModuleManager::ReadParameters()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("BASE_MODULE_PARAMS", this);
    PropertyBase::loadJsonConfig(BASE_MODULE_JSON,temp_map);

    aa_head_module.loadParams();
    sut_module.loadParams();
    lut_module.loadParams();
    dothinkey->loadParams();
    return true;
}

bool BaseModuleManager::SaveParameters()
{
return true;
}

bool BaseModuleManager::LoadProfile()
{
    if(profile_loaded)
        return true;
    profile_loaded = false;
    LPWSTR path = profile_path;
    int res = XT_Controler_Extend::Profile_Load(path);
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
//        XtVcMotor* motor_lut_vcm = new XtVcMotor();
//        motor_lut_vcm->Init("LUT_Z",lut_vcm_parameters);
//        motors.insert("LUT_Z",motor_lut_vcm);

//        XtVcMotor* motor_sut_vcm = new XtVcMotor();
//        motor_sut_vcm->Init("SUT_Z",sut_vcm_parameters);
//        motors.insert("SUT_Z",motor_sut_vcm);
        LoadVcmFile();

        vacuums.insert("LUT_V",new XtVacuum(GetOutputIoByName(u8"LUT吸真空"),GetInputIoByName(u8"LUT真空检测"),GetOutputIoByName(u8"LUT破真空"),u8"LUT_V"));
        vacuums.insert("SUT_V",new XtVacuum(GetOutputIoByName(u8"SUT1吸真空"),GetInputIoByName(u8"SUT1真空检测"),GetOutputIoByName(u8"SUT1破真空"),u8"LUT_V"));
        if(!InitStruct())return false;
        profile_loaded = true;
        return true;
    }
    InitStruct();
    return false;
}

bool BaseModuleManager::LoadVcmFile()
{
    QString val;
    QFile file;
    file.setFileName(".//config//vcm_parameter.json");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if(!file.open(QFile::ReadWrite)){
            return false;
        }
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
        QJsonDocument document;
        document.setArray(json);
        QJsonDocument saveDoc(json);
        file.write(document.toJson());
        file.close();
        return false;
    }
    val = file.readAll();
    file.close();

    QJsonParseError error;
        QJsonDocument doucment = QJsonDocument::fromJson(val.toUtf8(), &error);
        if (!doucment.isNull() && (error.error == QJsonParseError::NoError)) { //解析否出现错误
            if (doucment.isArray()) { // 数组判断
                QJsonArray array = doucment.array();  // 转数组
                VcMotorParameter temp_param;
                for (int i = 0; i < array.count(); i++)
                {
                    temp_param.read(array.at(i).toObject());
                    XtVcMotor* temp_motor = new XtVcMotor();
                    temp_motor->Init(temp_param.motorName(),temp_param.toparameter());
                    if(!motors.contains(temp_param.motorName()))
                        motors.insert(temp_param.motorName(),temp_motor);
                }
            }
        }
    return true;
}

bool BaseModuleManager::InitStruct()
{
    XtMotor *lut_x = GetMotorByName("LUT_X");
    XtMotor *lut_y = GetMotorByName("LUT_Y");
    XtVcMotor *lut_z = GetVcMotorByName("LUT_Z");
    XtMotor *sut_x = GetMotorByName("SUT1_X");
    XtMotor *sut_y = GetMotorByName("SUT1_Y");
    XtVcMotor *sut_z = GetVcMotorByName("SUT_Z");
    XtMotor *aa_x = GetMotorByName("AA1_X");
    XtMotor *aa_y = GetMotorByName("AA1_Y");
    XtMotor *aa_z = GetMotorByName("AA1_Z");
    XtMotor *aa_a = GetMotorByName("AA1_A");
    XtMotor *aa_b = GetMotorByName("AA1_B");
    XtMotor *aa_c = GetMotorByName("AA1_C");
    XtGeneralOutput * uv1 = GetOutputIoByName(u8"AA1_UV灯1");
    XtGeneralOutput * uv2 = GetOutputIoByName(u8"AA1_UV灯2");
    XtGeneralOutput * uv3 = GetOutputIoByName(u8"AA1_UV灯3");
    XtGeneralOutput * uv4 = GetOutputIoByName(u8"AA1_UV灯4");

    XtVacuum *lut_v = GetVacuumByName("LUT_V");
    XtVacuum *lut_v_u = GetVacuumByName("LUT_V_U");
    XtVacuum *sut_v = GetVacuumByName("SUT_V");
//    XtCylinder *sut_p = GetCylinderByName("SUT_P");
    XtCylinder *sut_j = GetCylinderByName("SUT_J");
    XtGeneralOutput *dispense_o = GetOutputIoByName(u8"SUT1点胶阀");
    XtGeneralOutput *gripper = GetOutputIoByName("AA1_GripON");

    lut_carrier.Init("LUT",lut_x,lut_y,lut_z,lut_v);
    sut_carrier.Init("SUT",sut_x,sut_y,sut_z,sut_v);
    lens_picker.Init(GetVcMotorByName(lens_pick_arm.parameters.motorXName()),
                     GetMotorByName(lens_pick_arm.parameters.motorTName()),
                     GetVacuumByName(lens_pick_arm.parameters.vacuumName()));
    lens_pick_arm.Init(GetMotorByName(lens_pick_arm.parameters.motorTrayName()),
                       GetMotorByName(lens_pick_arm.parameters.motorXName()),
                       GetMotorByName(lens_pick_arm.parameters.motorYName()),
                                      &lens_picker);
    aa_head_module.Init("AAHead",aa_x,aa_y,aa_z,aa_a,aa_b,aa_c,gripper,uv1,uv2,uv3,uv4,XtMotor::GetThreadResource());
    material_tray.Init();
    calibrations.insert(AA1_UPLOOK_CALIBRATION,new Calibration(AA1_UPLOOK_CALIBRATION,CALIBRATION_RESULT_PATH,aa_x,aa_y,vision_locations[PR_AA1_LUT_UPLOOK]));
    calibrations.insert(AA1_DOWNLOOK_CALIBRATION,new Calibration(AA1_DOWNLOOK_CALIBRATION,CALIBRATION_RESULT_PATH,sut_x,sut_y,vision_locations[PR_SUT_DOWNLOOK]));
    calibrations.insert(AA1_UPDownLOOK_UP_CALIBRATION,new Calibration(AA1_UPDownLOOK_UP_CALIBRATION,CALIBRATION_RESULT_PATH,lut_x,lut_y,vision_locations[PR_AA1_TOOL_UPLOOK]));
    calibrations.insert(AA1_UPDownLOOK_DOWN_CALIBRATION,new Calibration(AA1_UPDownLOOK_DOWN_CALIBRATION,CALIBRATION_RESULT_PATH,sut_x,sut_y,vision_locations[PR_AA1_TOOL_DOWNLOOK]));
    calibrations.insert(AA1_MUSHROOMHEAD_CALIBRATION,new Calibration(AA1_MUSHROOMHEAD_CALIBRATION,CALIBRATION_RESULT_PATH,lut_x,lut_y,vision_locations[PR_AA1_MUSHROOMHEAD]));
    calibrations.insert(LPA_LENS_CALIBRATION,new Calibration(LPA_LENS_CALIBRATION,CALIBRATION_RESULT_PATH,lut_x,lut_y,vision_locations[PR_LENS_LPALOOK]));

    chartCalibration = new ChartCalibration(dothinkey, AA_MAX_INTENSITY, AA_MIN_AREA, AA_MAX_AREA, CHART_CALIBRATION, CALIBRATION_RESULT_PATH, sut_x, sut_y);

    vision_locations[PR_AA1_TOOL_UPLOOK]->Init(visionModule,calibrations[AA1_UPDownLOOK_UP_CALIBRATION]->getCaliMapping(),lightingModule);
    vision_locations[PR_AA1_TOOL_DOWNLOOK]->Init(visionModule,calibrations[AA1_UPDownLOOK_DOWN_CALIBRATION]->getCaliMapping(),lightingModule);
    vision_locations[PR_AA1_LUT_UPLOOK]->Init(visionModule,calibrations[AA1_UPLOOK_CALIBRATION]->getCaliMapping(),lightingModule);
    vision_locations[PR_SUT_DOWNLOOK]->Init(visionModule,calibrations[AA1_DOWNLOOK_CALIBRATION]->getCaliMapping(),lightingModule);
    vision_locations[PR_LOAD_LUT_UPLOOK]->Init(visionModule,calibrations[AA1_UPLOOK_CALIBRATION]->getCaliMapping(),lightingModule);
    vision_locations[PR_AA1_MUSHROOMHEAD]->Init(visionModule,calibrations[AA1_MUSHROOMHEAD_CALIBRATION]->getCaliMapping(),lightingModule);
    vision_locations[PR_LENS_LPALOOK]->Init(visionModule,calibrations[LPA_LENS_CALIBRATION]->getCaliMapping(),lightingModule);
    vision_locations[PR_VACANCY_LPALOOK]->Init(visionModule,calibrations[LPA_LENS_CALIBRATION]->getCaliMapping(),lightingModule);
    vision_locations[PR_LENS_LUTLOOK]->Init(visionModule,calibrations[LPA_LENS_CALIBRATION]->getCaliMapping(),lightingModule);

    lut_module.Init(&lut_carrier,vision_locations[PR_AA1_LUT_UPLOOK],vision_locations[PR_AA1_TOOL_UPLOOK],vision_locations[PR_LOAD_LUT_UPLOOK],vision_locations[PR_AA1_MUSHROOMHEAD],lut_v,lut_v_u,gripper);
    sut_module.Init(&sut_carrier,vision_locations[PR_SUT_DOWNLOOK],vision_locations[PR_AA1_TOOL_DOWNLOOK],sut_v);
    QVector<XtMotor *> executive_motors;
    executive_motors.push_back(sut_x);
    executive_motors.push_back(sut_y);
    executive_motors.push_back(sut_z);
    dispenser.Init(XtMotor::GetCurveResource(),XtMotor::GetThreadResource(),XtMotor::GetThreadResource(),executive_motors,dispense_o);
    dispense_module.Init(DISPENSER_PARAMETER_PATH,AA1_DISPENSE,calibrations[AA1_DOWNLOOK_CALIBRATION],&dispenser,visionModule,&sut_carrier,dispense_o);
    dispense_module.setMapPosition(sut_module.downlook_position.X(),sut_module.downlook_position.Y());

    lens_loader_module.Init(&lens_pick_arm,&material_tray,&lut_carrier,vision_locations[PR_LENS_LPALOOK],vision_locations[PR_VACANCY_LPALOOK],vision_locations[PR_LENS_LUTLOOK]);
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
    motors["LUT_Y"]->SeekOrigin();
    result = motors["SUT_Z"]->WaitSeekDone();
    if(!result)return false;
    motors["SUT1_Y"]->SeekOrigin();
    result &= motors["LUT_Y"]->WaitSeekDone();
    result &= motors["SUT1_Y"]->WaitSeekDone();
    if(!result)return false;
    motors["AA1_Y"]->SeekOrigin();
    result = motors["AA1_Y"]->WaitSeekDone();
    if(!result)return false;
    motors["LUT_Z"]->SeekOrigin();
    motors["AA1_X"]->SeekOrigin();
    motors["AA1_Z"]->SeekOrigin();
    motors["AA1_A"]->SeekOrigin();
    motors["AA1_B"]->SeekOrigin();
    motors["AA1_C"]->SeekOrigin();
    motors["SUT1_X"]->SeekOrigin();
    motors["LUT_X"]->SeekOrigin();
    result &= motors["AA1_X"]->WaitSeekDone();
    result &= motors["AA1_Z"]->WaitSeekDone();
    result &= motors["AA1_A"]->WaitSeekDone();
    result &= motors["AA1_B"]->WaitSeekDone();
    result &= motors["AA1_C"]->WaitSeekDone();
    result &= motors["SUT1_X"]->WaitSeekDone();
    result &= motors["LUT_X"]->WaitSeekDone();
    result &= motors["LUT_Z"]->WaitSeekDone();
    if(!result)return false;
    GetVcMotorByName("LUT_Z")->ChangeDiretion();
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
    if (chartCalibration)
        chartCalibration->performCalibration(0.1, 0.1);
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
    if(motors.contains(name))
        return motors[name];
    return nullptr;
}

XtVcMotor *BaseModuleManager::GetVcMotorByName(QString name)
{
    if(motors.contains(name))
        return  dynamic_cast<XtVcMotor*>(motors[name]);
    return nullptr;
}

XtGeneralOutput *BaseModuleManager::GetOutputIoByName(QString name)
{
    if(output_ios.contains(name))
        return output_ios[name];
    return nullptr;
}

XtGeneralInput *BaseModuleManager::GetInputIoByName(QString name)
{
    if(output_ios.contains(name))
        return input_ios[name];
    return nullptr;
}

XtVacuum *BaseModuleManager::GetVacuumByName(QString name)
{
    if(vacuums.contains(name))
        return vacuums[name];
    return nullptr;
}

XtCylinder *BaseModuleManager::GetCylinderByName(QString name)
{
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
