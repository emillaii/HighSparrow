#include "xtmotion.h"
#include "XT_MotionControler_Client_Lib.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include "utils/logger.h"
#include <QtMath>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

using namespace XT_Controler_Extend;

wchar_t XtMotion::ip[] =  L"192.168.0.251";
wchar_t XtMotion::profile_path[] = L"./xt_motion_config.csv";
QVector<XtMotor *> XtMotion::AllMotors;
QVector<XtGeneralInput *> XtMotion::AllInputs;
int XtMotion::in_num_in_profile = 0;
QVector<XtGeneralOutput *> XtMotion::AllOutputs;
bool XtMotion::profile_loaded = false;
bool XtMotion::is_init = false;
bool XtMotion::is_home = false;
int XtMotion::curve_resource = 0;
int XtMotion::thread_resource = 0;
int XtMotion::axis_id_resource = 0;

XtMotor XtMotion::motor_aa_x;
XtMotor XtMotion::motor_aa_y;
XtMotor XtMotion::motor_aa_z;
XtMotor XtMotion::motor_aa_roll;
XtMotor XtMotion::motor_aa_yaw;
XtMotor XtMotion::motor_aa_pitch;
XtMotor XtMotion::motor_sut_x;
XtMotor XtMotion::motor_sut_y;
XtVcMotor XtMotion::motor_sut_z;
XtMotor XtMotion::motor_pa_x;
XtMotor XtMotion::motor_pa_y;
XtMotor XtMotion::motor_pa_z;
XtMotor XtMotion::motor_pa_theta1;
XtMotor XtMotion::motor_pa_theta2;
XtVcMotor XtMotion::motor_pa_vcm1;
XtVcMotor XtMotion::motor_pa_vcm2;
VCM_Parameter_struct XtMotion::pa_vcm1_parameters = {
    5000/*MaxVel*/,20000/*MaxAcc*/,200000/*MaxJerk*/,19/*MaxRange*/,0/*MinRange*/,7/*CanID*/,1/*dir*/,1024/*scale*/};
VCM_Parameter_struct XtMotion::pa_vcm2_parameters = {
    5000/*MaxVel*/,20000/*MaxAcc*/,200000/*MaxJerk*/,19/*MaxRange*/,0/*MinRange*/,10/*CanID*/,0/*dir*/,1024/*scale*/};
VCM_Parameter_struct XtMotion::sut_vcm_parameters = {
    500/*MaxVel*/,10000/*MaxAcc*/,200000/*MaxJerk*/,33/*MaxRange*/,0/*MinRange*/,9/*CanID*/,0/*dir*/,5000/*scale*/};
XtMotor XtMotion::motor_tray_x;

QJsonObject XtMotion::motor_extend_param;

XtGeneralInput XtMotion::xtin_gel_up_sensor;
XtGeneralInput XtMotion::xtin_gel_down_sensor;
XtGeneralInput XtMotion::xtin_gel_contact_sensor;
XtGeneralInput XtMotion::xtin_tray_ent;
XtGeneralInput XtMotion::xtin_tray_exit;
XtGeneralInput XtMotion::xtin_tray_in;
XtGeneralInput XtMotion::xtin_tray_out;
XtGeneralInput XtMotion::xtin_tray_s;
XtGeneralInput XtMotion::xtin_cmos_vacuum_s;
XtGeneralInput XtMotion::xtin_lens_vacuum_s;
XtGeneralInput XtMotion::xtin_pick_cmos_s;
XtGeneralInput XtMotion::xtin_pick_lens_s;
XtGeneralInput XtMotion::xtin_tray_ban_unban;
XtGeneralInput XtMotion::xtin_tray_ban_ban;
XtGeneralInput XtMotion::xtin_tray_lock1_up;
XtGeneralInput XtMotion::xtin_tray_lock1_down;
XtGeneralInput XtMotion::xtin_tray_lock2_up;
XtGeneralInput XtMotion::xtin_tray_lock2_down;
XtGeneralInput XtMotion::xtin_tray_up_up;
XtGeneralInput XtMotion::xtin_tray_up_down;


XtGeneralOutput XtMotion::xtout_gel_up;
XtGeneralOutput XtMotion::xtout_gel_down;
XtGeneralOutput XtMotion::xtout_cmos_vacuum;
XtGeneralOutput XtMotion::xtout_lens_vacuum;
XtGeneralOutput XtMotion::xtout_pogopin;
XtGeneralOutput XtMotion::xtout_dispense;
XtGeneralOutput XtMotion::xtout_gripper;
XtGeneralOutput XtMotion::xtout_pick_cmos;
XtGeneralOutput XtMotion::xtout_pick_lens;
XtGeneralOutput XtMotion::xtout_pick_cmos_r;
XtGeneralOutput XtMotion::xtout_pick_lens_r;
XtGeneralOutput XtMotion::xtout_uv_1;
XtGeneralOutput XtMotion::xtout_uv_2;
XtGeneralOutput XtMotion::xtout_uv_3;
XtGeneralOutput XtMotion::xtout_uv_4;
XtGeneralOutput XtMotion::xtout_tray_ban;
XtGeneralOutput XtMotion::xtout_tray_lock1;
XtGeneralOutput XtMotion::xtout_tray_lock2;
XtGeneralOutput XtMotion::xtout_tray_up;

XtVacuum XtMotion::xtv_pick_cmos(&XtMotion::xtout_pick_cmos, &XtMotion::xtin_pick_cmos_s, &XtMotion::xtout_pick_cmos_r, "PA取CMOS真空");
XtVacuum XtMotion::xtv_pick_lens(&XtMotion::xtout_pick_lens, &XtMotion::xtin_pick_lens_s, &XtMotion::xtout_pick_lens_r, "PA取Lens真空");
XtVacuum XtMotion::xtv_cmos_hold(&XtMotion::xtout_cmos_vacuum, &XtMotion::xtin_cmos_vacuum_s,nullptr, "SUT真空");
XtVacuum XtMotion::xtv_lens_hold(&XtMotion::xtout_lens_vacuum, &XtMotion::xtin_lens_vacuum_s,nullptr, "LUT真空");

XtCylinder XtMotion::xtc_pogopin(&XtMotion::xtout_pogopin,nullptr,nullptr,"CMOS压料");
XtCylinder XtMotion::xtc_tray_ban(&XtMotion::xtout_tray_ban,&XtMotion::xtin_tray_ban_ban,&XtMotion::xtin_tray_ban_unban,"送料盘阻挡");
XtCylinder XtMotion::xtc_tray_lock1(&XtMotion::xtout_tray_lock1,&XtMotion::xtin_tray_lock1_up,&XtMotion::xtin_tray_lock1_down,"送料盘钩1");
XtCylinder XtMotion::xtc_tray_lock2(&XtMotion::xtout_tray_lock2,&XtMotion::xtin_tray_lock2_up,&XtMotion::xtin_tray_lock2_down,"送料盘钩2");
XtCylinder XtMotion::xtc_tray_up(&XtMotion::xtout_tray_up,&XtMotion::xtin_tray_up_up,&xtin_tray_up_down,"送料盘定起");

//double XtMotion::sut_safty_z_pos = -15;
double XtMotion::sut_safty_z_pos = 0;
double XtMotion::feed_lens_target = 0;
PickArm XtMotion::pick_arm(&motor_pa_x, &motor_pa_y, &motor_pa_z, &motor_pa_theta1, &motor_pa_theta2, &motor_pa_vcm1, &motor_pa_vcm2,&xtv_pick_lens,&xtv_pick_cmos,&xtv_lens_hold,&xtv_cmos_hold,&xtc_pogopin);
TrayLoader XtMotion::tray_loader(&motor_tray_x, &xtc_tray_ban, &xtc_tray_lock1, &xtc_tray_lock2, &xtc_tray_up,&xtin_tray_ent,&xtin_tray_exit,&xtin_tray_s,&xtin_tray_in,&xtin_tray_out);
AAHeadControler XtMotion::aa_head(&motor_aa_pitch,&motor_aa_roll,&motor_aa_yaw,&motor_aa_x,&motor_aa_y,&motor_aa_z);

QVector<SixAxisPosition*> XtMotion::AAHeadPosModify;


bool XtMotion::LoadProfile()
{
    long long pContainer_ID[256];
    int container_num = 0;

    if(profile_loaded)
        return true;

    double r = qDegreesToRadians((double)90.0);
    RotateMatix rm = aa_head.GenRotateMatrix(r,1,0,0);
    NineDPoint r_aixs;
    r_aixs.position[0]=0;
    r_aixs.position[1]=0;
    r_aixs.position[2]=-1;
    double point[]={0,0,-1};
    aa_head.Rotate(rm,r_aixs,point,1);


    LoadPosModifyData("./AAHeadPosModifyData.json");

    LPWSTR path = profile_path;
    int res = Profile_Load(path);
    if(res!=0)
    {
        LoadMotorExtendParams();
        in_num_in_profile = Profile_Get_IoIn_Count();
        axis_id_resource = Profile_Get_Axis_Count();
        AddIOs();
        AddMotors();

        bool b_res = aa_head.InitInvParam("./AAHeadPosModifyData.json");
        if(!b_res) return false;
        NineDPoint p;
        p.position[0] = 0;
        p.position[1] = 20;
        p.position[2] = 0;
        aa_head.SetLensCenter(p);

        PickArmPos pos = {204,5,18,0};
        pick_arm.SetPickLensBase(pos);
        pos = {130,8,18,90};
        pick_arm.SetPickCMOSBase(pos);
        pos = {482,2,18,0};
        pick_arm.SetPlaceLensBase(pos);
        pos = {414,6,18,0};
        pick_arm.SetPlaceCMOSBase(pos);
        pos = {0,4.5,2,0};
        pick_arm.SetCMOSEscapeOffset(pos);
        pos = {414,6,18,0};
        pick_arm.SetPickProductBase(pos);
        pos = {130,8,18,90};
        pick_arm.SetPlaceProductBase(pos);
        //ToDo: Camera Offset
        QPointF distance = {1, 0.1};
        pick_arm.SetLensDistance(distance);
        distance = {1, 0.1};
        pick_arm.SetCMOSDistance(distance);

        profile_loaded = true;
        return true;
    }
    profile_loaded = false;
    return false;
}

bool XtMotion::Init()
{
    if(IsInit())
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
    motor_pa_vcm1.ConfigVCM();
	motor_pa_vcm2.ConfigVCM();
    motor_sut_z.ConfigVCM();
    //motor_sut_x.SetEncoderFeedback(4,0,-0.0005);
    //motor_sut_y.SetEncoderFeedback(4,1,-0.0002);
    //motor_sut_z.SetEncoderFeedback(4,2,-0.0002);
    //motor_sut_z.SetADC(6,0);
    XT_Controler_Extend::Start_Buffer_Sync(-1);

    is_init = true;
    //this must after "is_init = true;!!!"
    foreach (XtMotor *m, AllMotors) {
        m->GetMasterAxisID();
    }
    return true;
}

bool XtMotion::IsInit()
{
    return is_init;
}

bool XtMotion::IsHomeFinished()
{
    return is_home;
}

bool XtMotion::AllMotorsSeekOrigin()
{
    bool res;
    if(is_init!=true)
        return false;
    motor_sut_z.SeekOrigin();
    res = motor_sut_z.WaitSeekDone();
    if(!res)
        return false;
    motor_pa_z.SeekOrigin();
    motor_pa_vcm1.SeekOrigin();
    motor_pa_vcm2.SeekOrigin();
    res = motor_pa_z.WaitSeekDone();
    if(!res)
        return false;
    res = motor_pa_vcm1.WaitSeekDone();
    if(!res)
        return false;
    res = motor_pa_vcm2.WaitSeekDone();
    if(!res)
        return false;
    motor_sut_x.SeekOrigin();
    motor_sut_y.SeekOrigin();
    motor_pa_x.SeekOrigin();
    motor_pa_y.SeekOrigin();
    motor_pa_theta1.SeekOrigin();
    motor_pa_theta2.SeekOrigin();
    motor_tray_x.SeekOrigin();

    motor_aa_z.SeekOrigin();
    res = motor_aa_z.WaitSeekDone();
    if(!res)
        return false;
    motor_aa_x.SeekOrigin();
    motor_aa_y.SeekOrigin();
    motor_aa_pitch.SeekOrigin();
    motor_aa_roll.SeekOrigin();
    motor_aa_yaw.SeekOrigin();
    motor_aa_pitch.Home();
    motor_aa_roll.Home();
    motor_aa_yaw.Home();

    for(int i=0;i<AllMotors.length();i++ )
    {
        res = AllMotors[i]->WaitSeekDone();
        if(!res)
        {
            qInfo("%s home fail!",AllMotors[i]->Name().toStdString().c_str());
            return false;}
        AllMotors[i]->SetVel(AllMotors[i]->GetMaxVel());
    }

//    QThread::msleep(500);
//    for(int i=0;i<AllMotors.length();i++ )
//    {
//        AllMotors[i]->SetAxisDelay();
//    }
//    motor_sut_y.SetAxisDelay();
    is_home = true;
    return true;
}

bool XtMotion::WaitAllMotorsTillStop(int timeout)
{
    bool res;
    for(int i=0;i<AllMotors.length();i++ )
    {
        res = AllMotors[i]->WaitMoveStop(timeout);
        if(!res)
            return false;
    }
    return true;
}

int XtMotion::GetCurveResource()
{
    int ret = curve_resource;
    curve_resource++;
    return ret;
}

int XtMotion::GetThreadResource()
{
    int ret = thread_resource;
    thread_resource++;
    return ret;
}

int XtMotion::GetAxisIdResource()
{
    int ret = axis_id_resource;
    axis_id_resource++;
    return ret;
}

bool XtMotion::Sut_Z_XY_Z_MoveSync(double x, double y, double z, int timeout)
{
    bool res;
    int using_thread = motor_sut_z.default_using_thread;
    res = XtMotion::motor_sut_z.MoveToPosSync(XtMotion::sut_safty_z_pos,using_thread);
    if(!res)
        return false;

    XtMotion::motor_sut_x.SGO(x,using_thread);
    XtMotion::motor_sut_y.SGO(y,using_thread);
    XtMotion::motor_sut_x.TILLSTOP(using_thread);
    XtMotion::motor_sut_y.TILLSTOP(using_thread);
    res = XtMotion::motor_sut_z.WaitMoveStop(timeout);
    if(!res)
        return false;
//    XtMotion::motor_sut_z.MoveToPos(z,using_thread);
    res = XtMotion::motor_sut_z.MoveToPosSync(z,using_thread);
//    res = XtMotion::motor_sut_z.WaitMoveStop(timeout);
    return res;
}

bool XtMotion::Sut_Z0_Y0_X_YZ_MoveSync(double x, double y, double z, int timeout)
{
    bool res;
    int using_thread = motor_sut_z.default_using_thread;

    if((abs(XtMotion::motor_sut_x.GetFeedbackPos()-x)>0.1)||(abs(XtMotion::motor_sut_y.GetFeedbackPos()-y)>0.1))
    {
        // z to 0
        res = XtMotion::motor_sut_z.MoveToPosSync(XtMotion::sut_safty_z_pos,using_thread);
        if(!res)
            return false;
        // y to 0
        XtMotion::motor_sut_y.SGO(0,using_thread);
        XtMotion::motor_sut_y.TILLSTOP(using_thread);
    }

    // x to pos
    XtMotion::motor_sut_x.SGO(x,using_thread);
    XtMotion::motor_sut_x.TILLSTOP(using_thread);

    // yz to pos
    XtMotion::motor_sut_y.SGO(y,using_thread);
    XtMotion::motor_sut_z.MoveToPos(z,using_thread);
    XtMotion::motor_sut_y.TILLSTOP(using_thread);
    res = XtMotion::motor_sut_z.WaitMoveStop(timeout);
    return res;
}


bool XtMotion::Sut_Z_XYZ_MoveSync(double x, double y, double z, int timeout)
{
    bool res;
    int using_thread = motor_sut_z.default_using_thread;
    if((abs(XtMotion::motor_sut_x.GetFeedbackPos()-x)>0.001)||(abs(XtMotion::motor_sut_y.GetFeedbackPos()-y)>0.001))
    {
        res = XtMotion::motor_sut_z.MoveToPosSync(XtMotion::sut_safty_z_pos,using_thread);
        if(!res)
        return false;
    }
    XtMotion::motor_sut_x.SGO(x,using_thread);
    XtMotion::motor_sut_y.SGO(y,using_thread);
    XtMotion::motor_sut_z.MoveToPos(z,using_thread);
    XtMotion::motor_sut_x.TILLSTOP(using_thread);
    XtMotion::motor_sut_y.TILLSTOP(using_thread);
    res = XtMotion::motor_sut_z.WaitMoveStop(timeout);
    return res;
}

bool XtMotion::AA_RollPitch_MoveSync(double roll,double pitch, int timeout)
{
    bool res;
    int using_thread = motor_aa_roll.default_using_thread;
    XtMotion::motor_aa_roll.SGO(roll,using_thread);
    XtMotion::motor_aa_pitch.SGO(pitch,using_thread);
    XtMotion::motor_aa_roll.TILLSTOP(using_thread);
    XtMotion::motor_aa_pitch.TILLSTOP(using_thread);
    res = XtMotion::motor_aa_roll.WaitMoveStop(timeout);
    return res;
}

bool XtMotion::FeedLensToAAHead(double speed, double start_pos, double targetPos)
{
    bool res;
    double result;
    xtout_gripper = true;
    qInfo("FeedLensToAAHead Z start position: %f", start_pos);
    motor_sut_z.MoveToPosSync(start_pos);
    QThread::msleep(100);
    //res = motor_sut_z.SearchPosByADC(speed,start_pos+range,-6,false,result);
    double temp = targetPos;
    if(temp<0)
        temp = feed_lens_target;
    res = motor_sut_z.SearchPosByForce(speed, temp, 500, 1.0, result);
    if(res)
    {
        xtout_gripper = false;
        xtout_lens_vacuum = false;
    }
    qInfo("FeedLensToAAHead Z target: %f result: %f",temp, result);
    QThread::msleep(500);
    bool res2 = motor_sut_z.DoSoftLandingReturn();
    if(!res2)
    {
        qInfo("motor_sut_z.DoSoftLandingReturn() failed");
    }
    res2 = motor_sut_z.WaitSoftLandingDone();
    if(!res2)
    {
        qInfo("motor_sut_z.WaitSoftLandingDone() failed");
    }
    return res;
}

bool XtMotion::GetLensFromAAHead(double speed, double start_pos, double targetPos)
{
    bool res;
    double result;
    double temp = targetPos;
    if(temp<0)
        temp = feed_lens_target;

    qInfo("GetLensFromAAHead Z start position: %f", start_pos);
    motor_sut_z.MoveToPosSync(start_pos);
    QThread::msleep(100);
    res = motor_sut_z.SearchPosByForce(speed, temp, 500, 1.0, result);
    bool res_v = true;
    if(res)
    {
        xtout_gripper = true;
//        xtout_lens_vacuum = true;
       res_v = xtv_lens_hold.Set(true);
    }
    qInfo("FeedLensToAAHead Z result: %f", result);
    QThread::msleep(500);
    res = motor_sut_z.DoSoftLandingReturn();
    if(!res)
    {
        qInfo("motor_sut_z.DoSoftLandingReturn() failed");
    }
    res = motor_sut_z.WaitSoftLandingDone();
    if(!res)
    {
        qInfo("motor_sut_z.WaitSoftLandingDone() failed");
    }
    xtout_gripper = false;
    return res&&res_v;
}

void XtMotion::SetFeedLensTarget(double value)
{
    feed_lens_target = value;
    qInfo("Now Feed Lens Target Height is %f", feed_lens_target);
}

XtMotor *XtMotion::GetMotor(QString name)
{
    for(int i = 0;i<AllMotors.length();i++)
    {
        if(AllMotors[i]->Name() == name)
            return AllMotors[i];
    }
    return nullptr;
}




void XtMotion::AddMotors()
{
    motor_aa_x.Init("AA_X");
    motor_aa_y.Init("AA_Y");
    motor_aa_z.Init("AA_Z");
    motor_aa_roll.Init("AA_ROLL");
    motor_aa_yaw.Init("AA_YAW");
    motor_aa_pitch.Init("AA_PITCH");
    motor_sut_x.Init("SUT_X");
    motor_sut_y.Init("SUT_Y");
    //motor_sut_z.Init("SUT_Z");
    motor_sut_z.Init("SUT_Z",sut_vcm_parameters);
    motor_pa_x.Init("PA_X");
    motor_pa_y.Init("PA_Y");
    motor_pa_z.Init("PA_Z");
    motor_pa_theta1.Init("PA_TH1");
    motor_pa_theta2.Init("PA_TH2");
    motor_tray_x.Init("TRAY_X");
    motor_pa_vcm1.Init("PA_VCM1",pa_vcm1_parameters);
    motor_pa_vcm2.Init("PA_VCM2",pa_vcm2_parameters);


}

void XtMotion::AddIOs()
{
    xtin_gel_up_sensor.Init("GEL_UP_S");
    xtin_gel_down_sensor.Init("GEL_DOWN_S");
    xtin_gel_contact_sensor.Init("GEL_CON_S");
    xtin_tray_ent.Init("TRAY_ENT_S");
    xtin_tray_exit.Init("TRAY_EXIT_S");
    xtin_tray_in.Init("TRAY_IN_S");
    xtin_tray_out.Init("TRAY_OUT_S");
    xtin_tray_s.Init("TRAY_S");
    xtin_cmos_vacuum_s.Init("CMOS_VAC_S");
    xtin_lens_vacuum_s.Init("LENS_VAC_S");
    xtin_pick_cmos_s.Init("PA_VAC2_S");
    xtin_pick_lens_s.Init("PA_VAC1_S");
    xtin_tray_ban_unban.Init("TRAY_BAN_UNBAN");
    xtin_tray_ban_ban.Init("TRAY_BAN_BAN");
    xtin_tray_lock1_up.Init("TRAY_LOCK1_UP");
    xtin_tray_lock1_down.Init("TRAY_LOCK1_DOWN");
    xtin_tray_lock2_up.Init("TRAY_LOCK2_UP");
    xtin_tray_lock2_down.Init("TRAY_LOCK2_DOWN");
    xtin_tray_up_up.Init("TRAY_UP_UP");
    xtin_tray_up_down.Init("TRAY_UP_DOWN");

    xtout_gel_up.Init("GEL_UP");
    xtout_gel_down.Init("GEL_DOWN");
    xtout_cmos_vacuum.Init("CMOS_VAC");
    xtout_lens_vacuum.Init("LENS_VAC");
    xtout_pogopin.Init("POGOPIN");
    xtout_dispense.Init("DISPENSE");
    xtout_gripper.Init("GRIPPER");
    xtout_uv_1.Init("UV1");
    xtout_uv_2.Init("UV2");
    xtout_uv_3.Init("UV3");
    xtout_uv_4.Init("UV4");
    xtout_pick_lens.Init("PA_VAC1");
    xtout_pick_cmos.Init("PA_VAC2");
    xtout_pick_lens_r.Init("PA_VAC1R");
    xtout_pick_cmos_r.Init("PA_VAC2R");
    xtout_tray_ban.Init("TRAY_BAN");
    xtout_tray_lock1.Init("TRAY_LOCK1");
    xtout_tray_lock2.Init("TRAY_LOCK2");
    xtout_tray_up.Init("TRAY_UP");
}

void XtMotion::EnableAllMotors()
{
    for(int i=0;i<AllMotors.length();i++ )
    {
        AllMotors[i]->Enable();
    }
}

bool XtMotion::LoadPosModifyData(QString path)
{
    QFile loadFile(path);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("LoadPosModifyData failed.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    const QJsonObject& json = loadDoc.object();
    if(json.contains("PosModifyData") && json["PosModifyData"].isArray())
    {
        QJsonArray data_array = json["PosModifyData"].toArray();
        CleanPosModifyData();
        AAHeadPosModify.reserve(data_array.size());
        for(int i = 0;i<data_array.size();i++)
        {
            QJsonObject data_object = data_array[i].toObject();
            SixAxisPosition *data = new SixAxisPosition(0,0,0,0,0,0);
            data->read(data_object);
            AAHeadPosModify.append(data);
        }
    }

    return true;
}

void XtMotion::LoadMotorExtendParams(QString path)
{
    QFile loadFile(path);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("LoadMotorExtendParams failed.");
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    motor_extend_param = std::move(loadDoc.object());
    for (int i = 0; i < AllMotors.length(); ++i)
    {
        QJsonObject json = XtMotion::motor_extend_param[AllMotors[i]->Name()].toObject();
        if(json.isEmpty()!=true)
        {
            AllMotors[i]->extend_parameters.read(json);
        }
    }
}

void XtMotion::SaveMotorExtendParams(QString path)
{
    QFile saveFile(path);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("SaveMotorExtendParams failed, Couldn't open save file.");
        return;
    }

    QJsonObject json;
    foreach (const XtMotor* m, AllMotors) {
        QJsonObject data_object;
        m->extend_parameters.write(data_object);
        json[m->Name()] = data_object;
    }
    json["Timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());
}

void XtMotion::CleanPosModifyData()
{
    for(int i = 0;i<AAHeadPosModify.length();i++)
    {
        delete AAHeadPosModify[i];
    }
    AAHeadPosModify.clear();
}

bool XtMotion::SavePosModifyData(QString path)
{
    QFile saveFile(path);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("SavePosModifyData failed, Couldn't open save file.");
        return false;
    }

    QJsonObject json;
    QJsonArray data_array;
    foreach (const SixAxisPosition* data, AAHeadPosModify) {
        QJsonObject data_object;
        data->write(data_object);
        data_array.append(data_object);
    }
    json["PosModifyData"] = data_array;
    json["Timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());

    return true;
}

