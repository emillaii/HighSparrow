#include "XtVcMotor.h"
#include "XT_MotionControler_Client_Lib.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include "C_SoftLanding_dll.h"

int XtVcMotor::vcm_count = 0;
QVector<VCM_Resource_struct> XtVcMotor::all_parameter;
XtVcMotor::XtVcMotor()
{
    axis_id = -1;
    name = "";
    is_init = false;
    origin_result = false;
    direction_is_opposite = false;
    vcm_id = vcm_count+1;
    vcm_count+=1;
}

void XtVcMotor::ConfigVCM()
{
    if(parameters.direction!=0)
        direction_is_opposite = true;
    SetRunDirect(vcm_id, parameters.direction, parameters.scale);
    SetPosModeSpeed(vcm_id,max_vel);
    SetPosModeAcc(vcm_id,max_acc);
    SetPosModejerk(vcm_id,max_jerk);
    SetPosLimit(vcm_id,max_range,min_range);
    SetSoftlandingSlot(vcm_id,curve_resource++);
    is_init = true;
}

void XtVcMotor::Init(const QString& motor_name,VCM_Parameter_struct parameters)
{
    name = motor_name;
    origin.Init(name+"_O");
    max_vel = 100;

    this->parameters = parameters;
    axis_id = axis_id_resource++;
    default_using_thread = axis_id + 20;
    max_vel = parameters.MaxVel;
    max_acc = parameters.MaxAcc;
    max_jerk = parameters.MaxJerk;
    max_range = parameters.MaxPos;
    min_range = parameters.MinPos;

    vcm_resource.CanID = parameters.CanID;
    vcm_resource.iAxis = axis_id;
    vcm_resource.IO_ID = origin.ID();
    vcm_resource.iThread = default_using_thread;
    vcm_resource.iThread_Curve = thread_resource++;
    vcm_resource.Connet_Rebuild = 0;
    all_parameter.append(vcm_resource);
}

void XtVcMotor::InitAllVCM()
{
    VCMT_resource_alloc(all_parameter.data(),all_parameter.length());
    Soft_landing_dll_init(1);

    int timeout = 30000;
    while(timeout>0)
    {
        int res = Get_Init_Ready();
        if(res == 1)
        {
            return;
        }
        if(res == -1)
        {
            qInfo("VCM on CAN ID is NOT exist!");
            return;
        }
        timeout-=10;
        QThread::msleep(10);
    }
    qInfo("VCM Soft_landing_dll_init failed!");
}

void XtVcMotor::Init(const QString& motor_name)
{
}



void XtVcMotor::SetADC(int can_id, int data_ch)
{

}

void XtVcMotor::SetEncoderFeedback(int can_id, int data_ch, double ratio)
{

}

void XtVcMotor::SetFeedbackZero(double new_value)
{
    if(!is_init)
        return;
}

void XtVcMotor::Enable()
{
    if(!is_init)
        return;
    is_enable = true;
}

void XtVcMotor::Disable()
{
    if(!is_init)
        return;
    is_enable = false;
}


double XtVcMotor::GetOutpuPos() const
{
    if(!is_init)
        return 0;
    {
        double val;
        int res = XT_Controler_Extend::Get_Cur_Axis_Pos(axis_id,val);
        if(res!=0)
        {
            //qInfo("vcm now output pos is %f",val);
            return val;
        }
    }
    return 0;
}

double XtVcMotor::GetFeedbackPos() const
{
    if(!is_init)
        return 0;

    {
        //return GetOutpuPos();
        double val;
        int times = 10;
        do{
            int res = GetNowPos(vcm_id,val);
            if(res==1)
                break;
            Sleep(1);
            qInfo("VCM GetNowPos failed! %d",times);
        }
        while(--times>0);
        if(direction_is_opposite)
            val = -val;
        return val;
    }
    return 0;
}




double XtVcMotor::GetCurADC() const
{
    if(adc!= nullptr)
        return adc->GetValue();
    return 0;
}

bool XtVcMotor::IsRunning() const
{
    if(!is_init)
        return false;
    int be_run;
    XT_Controler_Extend::Get_Cur_Axis_State(axis_id,be_run);
    return be_run == 1;
}

double XtVcMotor::GetPostiveRange() const
{
    return max_range;
}

double XtVcMotor::GetNegativeRange() const
{
    return min_range;
}

int XtVcMotor::AxisId() const
{
    return axis_id;
}

double XtVcMotor::GetMaxVel() const
{
    return max_vel;
}

double XtVcMotor::GetMaxAcc() const
{
    return max_acc;
}

double XtVcMotor::GetMaxJerk() const
{
    return max_jerk;
}

void XtVcMotor::SetVel(double vel, int thread)
{
    if(!is_init)
        return;
    SetPosModeSpeed(vcm_id, vel);
}

void XtVcMotor::SetAcc(double vel, int thread)
{
    if(!is_init)
        return;
    SetPosModeAcc(vcm_id, vel);
}

void XtVcMotor::SetJerk(double jerk, int thread)
{
    if(!is_init)
        return;
    if(thread==-1)
        thread = default_using_thread;
    SetPosModejerk(vcm_id, jerk);
}

void XtVcMotor::SetPostiveRange(double range)
{
    max_range = range;
}

void XtVcMotor::SetNegativeRange(double range)
{
    min_range = range;
}

void XtVcMotor::Home(int thread)
{
    MoveToPos(0, thread);
}

void XtVcMotor::SeekOrigin(int thread)
{
    if(!is_init)
        return;
    if(!is_enable)
        return;
    SetZeroPos(vcm_id,0);
    int result = Init_Go_Zero(vcm_id);
    if(result == 1)
        origin_result = true;
    else
    {
        origin_result = false;
        qInfo("axis %s seek origin fail!",name.toStdString().c_str());
    }
}



bool XtVcMotor::WaitSeekDone(int thread,int timeout)
{
    if(!is_init)
        return false;
    if(!is_enable)
        return false;
    return origin_result;
}

void XtVcMotor::GetMasterAxisID()
{
    axis_sub_id = axis_id;
}


bool XtVcMotor::SearchPosByADC(double vel, double search_limit, double threshold, bool search_above, double &result)
{

    return false;
}

bool XtVcMotor::SearchPosByForce(double slow_speed, double search_limit, double force, double margin, double &result)
{
    if(!is_init)
        return false;
    double start_pos = GetOutpuPos();
    qInfo("start_pos: %f,force:%f,search_limit:%f",start_pos,force,search_limit);
    SetSoftLanding(slow_speed, slow_speed*10, force, start_pos, search_limit, margin);
    bool res;
    res = DoSoftLanding();
    res |= WaitSoftLandingDone();
    if(!res)
    {
        result = start_pos;
//        SetCurrentLimit(vcm_id,10,-10);
        return false;
    }
    result = GetFeedbackPos();

    return true;
}

bool XtVcMotor::SearchPosByForce(double &result, double force, double  search_limit)
{
    if(!is_init)
        return false;
    if(search_limit<0)search_limit = max_range;
    double start_pos = GetOutpuPos();
    qInfo("start_pos: %f,force:%f,search_limit:%f",start_pos,force,search_limit);
    SetSoftLanding(max_vel,max_acc, force, start_pos, search_limit, 1);
    bool res;
    res = DoSoftLanding();
    res &= WaitSoftLandingDone();
    if(!res)
    {
        result = start_pos;
        return false;
    }
    result = GetFeedbackPos();
    return  true;
}

void XtVcMotor::RestoreForce()
{
    SetCurrentLimit(vcm_id,10,-10);
}

void XtVcMotor::ShowSetting()
{
    if(!is_init)
        return;
    ShowSettingDlg();
}

void XtVcMotor::SetSoftLanding(double slow_speed, double slow_acc, double force, double start_pos, double target_pos, double margin)
{
    if(!is_init)
        return;
    SetFastSpeed(vcm_id,max_vel);
    SetFastAcc(vcm_id, max_acc);
    SetSlowSpeed(vcm_id, slow_speed);
    SetSlowAcc(vcm_id,slow_acc);
    SetForce(vcm_id,force);
    SetSoftPos(vcm_id,target_pos);
    SetSoftLandingMargin(vcm_id,margin);
    SetZeroPos(vcm_id,start_pos);
    UpdataParam(vcm_id);
}

bool XtVcMotor::DoSoftLanding()
{
    int res = Do_SoftDown(vcm_id);
    if(res==0)
        return true;
    qInfo("VCM %d DoSoftLanding Failed! code: %d",vcm_id,res);
    return false;
}

bool XtVcMotor::DoSoftLandingReturn()
{
    if(!is_init)
        return false;
    int res = Do_SoftUp(vcm_id);
    if(res==0)
        return true;
    qInfo("VCM %d DoSoftLandingReturn Failed! code: %d",vcm_id,res);
    return false;
}

bool XtVcMotor::WaitSoftLandingDone(int timeout)
{
    if(!is_init)
        return false;
    while(timeout>0)
    {
        int res = CheckPosReady(vcm_id);
        if (res == 1)
        {
            return true;
        }
        timeout-=10;
        QThread::msleep(10);
    }
    return false;
}






