#include "XtVcMotor.h"
#include "basemodulemanager.h"

wchar_t BaseModuleManager::ip[] =  L"192.168.0.251";
wchar_t BaseModuleManager::profile_path[] = L"./xt_motion_config.csv";
BaseModuleManager::BaseModuleManager(QObject *parent)
    : PropertyBase (parent)
{
    is_init = false;
    profile_loaded = false;
    pylonUplookCamera = new BaslerPylonCamera(UPLOOK_VISION_CAMERA);
    pylonDownlookCamera = new BaslerPylonCamera(DOWNLOOK_VISION_CAMERA);
    lightingModule = new WordopLight();
    visionModule = new VisionModule(pylonDownlookCamera, pylonUplookCamera, Q_NULLPTR);
    dothinkey = new Dothinkey();
    imageGrabberThread = new ImageGrabbingWorkerThread(dothinkey);
    pylonUplookCamera->start();
    pylonDownlookCamera->start();
}

BaseModuleManager::~BaseModuleManager()
{}

bool BaseModuleManager::ReadParameters()
{
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
        XtVcMotor* motor_lut_vcm = new XtVcMotor();
        motor_lut_vcm->Init("LUT_Z",lut_vcm_parameters);
        motors.insert("LUT_Z",motor_lut_vcm);

        XtVcMotor* motor_sut_vcm = new XtVcMotor();
        motor_sut_vcm->Init("SUT_Z",sut_vcm_parameters);
        motors.insert("SUT_Z",motor_sut_vcm);
        if(!InitStruct())return false;
        profile_loaded = true;
        return true;
    }
    InitStruct();
    return false;
}

bool BaseModuleManager::InitStruct()
{
    XtMotor *x,*y,*z,*a,*b,*c;
    XtVcMotor *z_v;
    XtVacuum *v,*v_u;
    XtGeneralOutput *o;
    x = GetMotorByName("LUT_X");
    y = GetMotorByName("LUT_Y");
    z_v = GetVcMotorByName("LUT_Z");
    v = GetVacuumByName("LUT_V");
    //    if(x == nullptr||y == nullptr||z_v == nullptr||v == nullptr)return false;
    lut_carrier.Init("LUT",x,y,z_v,v);
    calibrations.insert(AA1_UPDownLOOK_UP_CALIBRATION,new Calibration(AA1_UPDownLOOK_UP_CALIBRATION,CALIBRATION_RESULT_PATH,x,y,lightingModule,0,lut_module.parameters.UpDnLookLighting(),UPLOOK_VISION_CAMERA,lut_module.parameters.upDownLookPrName()));
    v_u = GetVacuumByName("LUT_V_U");
    //    if(lut_carrier == nullptr||pylonUplookCamera == nullptr||lightingModule == nullptr||visionModule == nullptr || v_u == nullptr)return false;
    lut_module.Init(&lut_carrier,calibrations[AA1_UPDownLOOK_UP_CALIBRATION],lightingModule,visionModule,v,v_u);
    x = GetMotorByName("SUT1_X");
    y = GetMotorByName("SUT1_Y");
    z_v = GetVcMotorByName("SUT_Z");
    v = GetVacuumByName("SUT_V");
    //if(x == nullptr||y == nullptr||z_v == nullptr||v == nullptr)return false;
    sut_carrier.Init("SUT",x,y,z_v,v);
    //if(sut_carrier == nullptr||pylonDownlookCamera == nullptr||lightingModule == nullptr||visionModule == nullptr)return false;

    sut_module.Init(&sut_carrier,calibrations[AA1_DOWNLOOK_CALIBRATION],calibrations[AA1_UPDownLOOK_DOWN_CALIBRATION],lightingModule,visionModule);

    //if(x == nullptr||y == nullptr)return false;
    calibrations.insert(AA1_DOWNLOOK_CALIBRATION,new Calibration(AA1_DOWNLOOK_CALIBRATION,CALIBRATION_RESULT_PATH,x,y,lightingModule,0,sut_module.parameters.Lighting(),DOWNLOOK_VISION_CAMERA,sut_module.parameters.prName()));
    calibrations.insert(AA1_UPDownLOOK_DOWN_CALIBRATION,new Calibration(AA1_UPDownLOOK_DOWN_CALIBRATION,CALIBRATION_RESULT_PATH,x,y,lightingModule,0,sut_module.parameters.UpDnLookLighting(),DOWNLOOK_VISION_CAMERA,sut_module.parameters.upDownLookPrName()));

    QVector<XtMotor *> executive_motors;
    executive_motors.push_back(x);
    executive_motors.push_back(y);
    executive_motors.push_back(z);
    o = GetOutputIoByName("Dispense");
    dispenser.Init(DISPENSER_PARAMETER_PATH,AA1_DISPENSER,XtMotor::GetCurveResource(),XtMotor::GetThreadResource(),XtMotor::GetThreadResource(),executive_motors,o);
    dispense_module.Init(calibrations[AA1_DOWNLOOK_CALIBRATION],&dispenser);
    x = GetMotorByName("AA1_X");
    y = GetMotorByName("AA1_Y");
    z = GetMotorByName("AA1_Z");
    a = GetMotorByName("AA1_A");
    b = GetMotorByName("AA1_B");
    c = GetMotorByName("AA1_C");
    v = GetVacuumByName("AA_V");
    //if(x == nullptr||y == nullptr||z == nullptr||a == nullptr||b == nullptr||c == nullptr||v == nullptr)return false;
    aa_head_module.Init("AAHead",x,y,z,a,b,c,v);

    //if(x == nullptr||y == nullptr)return false;
    calibrations.insert(AA1_UPLOOK_CALIBRATION,new Calibration(AA1_UPLOOK_CALIBRATION,CALIBRATION_RESULT_PATH,x,y,lightingModule,0,lut_module.parameters.Lighting(),UPLOOK_VISION_CAMERA,lut_module.parameters.prName()));
    x = GetMotorByName("SUT1_X");
    y = GetMotorByName("SUT1_Y");


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
    XtVcMotor* temp_motor = dynamic_cast<XtVcMotor*>(motors["LUT_Z"]);
    if(temp_motor==nullptr)
        qInfo("motor LUT_Z is null");
    else
        temp_motor->ConfigVCM();

    temp_motor = dynamic_cast<XtVcMotor*>(motors["SUT_Z"]);
    if(temp_motor==nullptr)
        qInfo("motor SUT_Z is null");
    else
        temp_motor->ConfigVCM();

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
    motors["LUT_Z"]->SeekOrigin();
    result = motors["SUT_Z"]->WaitSeekDone();
    result &= motors["LUT_Z"]->WaitSeekDone();
    if(!result)return false;
    motors["AA1_Y"]->SeekOrigin();
    result = motors["AA1_Y"]->WaitSeekDone();
    if(!result)return false;
    motors["LUT_Y"]->SeekOrigin();
    motors["AA1_X"]->SeekOrigin();
    motors["AA1_Z"]->SeekOrigin();
    motors["AA1_A"]->SeekOrigin();
    motors["AA1_B"]->SeekOrigin();
    motors["AA1_C"]->SeekOrigin();
    motors["SUT1_X"]->SeekOrigin();
    motors["SUT1_Y"]->SeekOrigin();
    result = motors["LUT_Y"]->WaitSeekDone();
    if(result)
    {
        motors["LUT_X"]->SeekOrigin();
        result &= motors["LUT_X"]->WaitSeekDone();
    }
    result &= motors["AA1_X"]->WaitSeekDone();
    result &= motors["AA1_Z"]->WaitSeekDone();
    result &= motors["AA1_A"]->WaitSeekDone();
    result &= motors["AA1_B"]->WaitSeekDone();
    result &= motors["AA1_C"]->WaitSeekDone();
    result &= motors["SUT1_X"]->WaitSeekDone();
    result &= motors["SUT1_Y"]->WaitSeekDone();
    if(!result)return false;
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
    if(calibrations.contains(AA1_UPDownLOOK_UP_CALIBRATION))
        if(calibrations[AA1_UPDownLOOK_UP_CALIBRATION]->performCalibration())
            if(calibrations.contains(AA1_UPDownLOOK_DOWN_CALIBRATION))
                calibrations[AA1_UPDownLOOK_DOWN_CALIBRATION]->performCalibration();
}

void BaseModuleManager::UpdateCalibrationParameters()
{

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
    return nullptr;
}

bool BaseModuleManager::stepMove(QString name, double step, bool isPositive)
{
    if (motors.contains(name)) {
        qInfo("Step move: %s %f %d %f", name.toStdString().c_str(), step, isPositive, motors[name]->GetFeedbackPos());
        if (isPositive)
            motors[name]->StepMove(step);
        else {
            motors[name]->StepMove(-step);
        }
        return true;
    } else
        return false;
}

double BaseModuleManager::getMotorFeedbackPos(QString name)
{
     if (motors.contains(name)) {
          return motors[name]->GetFeedbackPos();
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
