#include "XtVcMotor.h"
#include "basemodulemanager.h"

wchar_t BaseModuleManager::ip[] =  L"192.168.0.251";
wchar_t BaseModuleManager::profile_path[] = L"./xt_motion_config.csv";
BaseModuleManager::BaseModuleManager(QObject *parent)
    : PropertyBase (parent),
      m_downlookLighting(0), m_uplookLighting(0)
{
    //pylonUplookCamera = new BaslerPylonCamera(UPLOOK_VISION_CAMERA);
    //pylonDownlookCamera = new BaslerPylonCamera(DOWNLOOK_VISION_CAMERA);
    lightingModule = new WordopLight();
    visionModule = new VisionModule();

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
        int in_num_in_profile = XT_Controler_Extend::Profile_Get_IoIn_Count();
        for (int i = 0; i < in_num_in_profile; ++i) {
           temp_name = QString::fromStdWString(XT_Controler_Extend::Profile_Get_IoIn_Name(i));
           if(temp_name == ""||input_ios.contains(temp_name))
           {
               qInfo("this already has an input io : %s",temp_name.toStdString().c_str());
               continue;
           }
           XtGeneralInput input_io = XtGeneralInput(temp_name,i);
           input_ios.insert(temp_name,&input_io);
        }
        int out_num_in_profile = XT_Controler_Extend::Profile_Get_IoIn_Count();
        for (int i = 0; i < out_num_in_profile; ++i) {
           temp_name = QString::fromStdWString(XT_Controler_Extend::Profile_Get_IoOut_Name(i));
           if(temp_name == ""||output_ios.contains(temp_name))
           {
               qInfo("this already has an input io : %s",temp_name.toStdString().c_str());
               continue;
           }
           XtGeneralOutput output_io = XtGeneralOutput(temp_name,i);
           output_ios.insert(temp_name,&output_io);
        }
        axis_id_resource = XT_Controler_Extend::Profile_Get_Axis_Count();
        for (int i = 0; i < axis_id_resource; ++i) {
           temp_name = QString::fromStdWString(XT_Controler_Extend::Profile_Get_IoIn_Name(i));
           if(temp_name == ""||input_ios.contains(temp_name))
           {
               qInfo("this already has an input io : %s",temp_name.toStdString().c_str());
               continue;
           }
           XtMotor motor;
           motor.Init(temp_name);
           motors.insert(temp_name,&motor);
        }
        XtVcMotor motor_lut_vcm,motor_sut_vcm;
        motor_lut_vcm.Init("LUT_VCM",lut_vcm_parameters);
        motors.insert("LUT_VCM",&motor_lut_vcm);
        motor_sut_vcm.Init("SUT1_VCM",sut_vcm_parameters);
        motors.insert("SUT1_VCM",&motor_sut_vcm);
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
    dynamic_cast<XtVcMotor*>(motors["LUT_VCM"])->ConfigVCM();
    dynamic_cast<XtVcMotor*>(motors["SUT1_VCM"])->ConfigVCM();

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

bool BaseModuleManager::AllMotorsSeekOrigin()
{
    bool result;
    motors["SUT1_Z"]->SeekOrigin();
    result = motors["SUT1_Z"]->WaitSeekDone();
    if(!result)return false;
    motors["LUT_Z"]->SeekOrigin();
    result = motors["LUT_Z"]->WaitSeekDone();
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
    if(!result)
    {
        qInfo("all motor seeked origin successed!");
        return false;
    }
    return  true;
}

void BaseModuleManager::StopSeeking()
{
    if(is_init)
            foreach (XtMotor *m, motors.values()) {
                m->StopSeeking();
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
