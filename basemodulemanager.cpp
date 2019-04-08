#include "XtVcMotor.h"
#include "basemodulemanager.h"

wchar_t BaseModuleManager::ip[] =  L"192.168.0.251";
wchar_t BaseModuleManager::profile_path[] = L"./xt_motion_config.csv";
BaseModuleManager::BaseModuleManager(QObject *parent)
    : PropertyBase (parent),
      m_downlookLighting(0), m_uplookLighting(0)
{
    is_init = false;
    profile_loaded = false;
    pylonUplookCamera = new BaslerPylonCamera(CAMERA_LUT_DL);
    pylonDownlookCamera = new BaslerPylonCamera(CAMERA_AA1_DL);
    lightingModule = new WordopLight();
    lightingModule->Init("com1");
    lightingModule->OnOff(2, true);
    lightingModule->OnOff(0, true);
    visionModule = new VisionModule();
    pylonUplookCamera->start();
    pylonDownlookCamera->start();
}

BaseModuleManager::~BaseModuleManager()
{}

void BaseModuleManager::testVision()
{
    PRResultStruct prResult;
    visionModule->PR_Generic_NCC_Template_Matching(DOWNLOOK_VISION_CAMERA, prResult);
    qInfo("%f %f %f", prResult.x, prResult.y, prResult.theta);
}

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
    LPWSTR path = profile_path;
    int res = XT_Controler_Extend::Profile_Load(path);
    if(res!=0)
    {
        QString temp_name;
        XtGeneralInput::count = XT_Controler_Extend::Profile_Get_IoIn_Count();
        for (int i = 0; i < XtGeneralInput::count; ++i) {
           temp_name = QString::fromStdWString(XT_Controler_Extend::Profile_Get_IoIn_Name(i));
           if(temp_name == ""||input_ios.contains(temp_name))
           {
               qInfo("this already has an input io : %s",temp_name.toStdString().c_str());
               continue;
           }
           XtGeneralInput* input_io = new XtGeneralInput(temp_name,i);
           input_ios.insert(temp_name,input_io);
        }
        XtGeneralOutput::count = XT_Controler_Extend::Profile_Get_IoOut_Count();
        for (int i = 0; i < XtGeneralOutput::count; ++i) {
           temp_name = QString::fromStdWString(XT_Controler_Extend::Profile_Get_IoOut_Name(i));
           if(temp_name == ""||output_ios.contains(temp_name))
           {
               qInfo("this already has an output io : %s",temp_name.toStdString().c_str());
               continue;
           }
           XtGeneralOutput* output_io = new XtGeneralOutput(temp_name,i);
           output_ios.insert(temp_name,output_io);
        }
        XtMotor::axis_id_resource = XT_Controler_Extend::Profile_Get_Axis_Count();
        for (int i = 0; i <  XtMotor::axis_id_resource; ++i) {
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

        profile_loaded = true;
        return true;
    }
    profile_loaded = false;
    return false;
}

bool BaseModuleManager::initialDevice()
{
    if(is_init)
        return true;
    if(!LoadProfile())
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
    motors["AA1_Y"]->SeekOrigin();
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
    result &= motors["AA1_Y"]->WaitSeekDone();
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
