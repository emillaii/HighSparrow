#include "XtMotion/XtVcMotor.h"
#include "XT_MotionControler_Client_Lib.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include "C_SoftLanding_dll.h"
#include <QElapsedTimer>
int XtVcMotor::vcm_count = 0;
QVector<VCM_Resource_struct> XtVcMotor::all_parameter;
XtVcMotor::XtVcMotor()
{
    axis_id = -1;
    name = "";
    is_init = false;
    direction_is_opposite = false;
    vcm_id = vcm_count+1;
    vcm_count+=1;
}

void XtVcMotor::ConfigVCM()
{
    if(parameters.direction()!=0)
        direction_is_opposite = true;
    SetRunDirect(vcm_id, parameters.direction(), parameters.scale());
    SetPosModeSpeed(vcm_id,max_vel);
    SetPosModeAcc(vcm_id,max_acc);
    SetPosModejerk(vcm_id,max_jerk);
    SetPosLimit(vcm_id,max_range,min_range);
    qInfo("ConfigVCM::vcm_id:%d,max_range:%f min_range:%f",vcm_id,max_range,min_range);

    SetSoftlandingSlot(vcm_id,GetCurveResource());
    double current[4]{0.1,1,2,3};
    double force[4]{100,1000,2000,3000};
    //MapCurrent2Force(vcm_id,current,force,4);
    is_init = true;
    is_enable = true;
    error_code = get_motor_error(vcm_id);
}

void XtVcMotor::ChangeDiretion(bool befor_seek)
{
    int direction = parameters.direntionAfterSeek();
    if(befor_seek)direction =  parameters.direction();
    if(direction!=0)
        direction_is_opposite = true;
    else
        direction_is_opposite = false;
    qInfo("VCM %d:dirention:%d",vcm_id,direction);
    SetRunDirect(vcm_id, direction, parameters.scale());
}

void XtVcMotor::Init()
{
    name = parameters.motorName();
    origin.Init(name+"_O");
    origin2.Init(name+"_O2");
    max_vel = 100;

    axis_id = axis_id_resource++;
    default_using_thread = axis_id + 20;
    max_vel = parameters.maxVel();
    max_acc = parameters.maxAcc();
    max_jerk = parameters.maxJerk();
    max_range = parameters.maxRange();
    min_range = parameters.minRange();

    vcm_resource.CanID = parameters.canId();
    vcm_resource.iAxis = axis_id;
    vcm_resource.IO_ID = origin.ID();
    vcm_resource.Z_Index_ID = origin2.ID();
    vcm_resource.iThread = default_using_thread;
    vcm_resource.iThread_Curve = GetThreadResource();
    vcm_resource.Connet_Rebuild = 0;
    all_parameter.append(vcm_resource);
}

void XtVcMotor::InitAllVCM()
{
    qInfo("VCM num：%d", all_parameter.length());//我在这里加了句把长度
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


double XtVcMotor::GetFeedbackPos(int decimal_digit) const
{
    if(!is_init)
        return 0;

    {
//        static int time = 0;
//        qInfo("spance:%d", QTime::currentTime().msec()-time);
//        time = QTime::currentTime().msec();
        double val;
        int times = 10;
        do{
            int res = GetNowPos(vcm_id,val);
            if(res==1)
                break;
            Sleep(1);
            qInfo("%s VCM GetNowPos failed! %d", this->Name().toStdString().c_str(), times);
        }
        while(--times>0);
        if(direction_is_opposite)
            val = -val;
        return round(val*pow(10,decimal_digit))/pow(10,decimal_digit);
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

bool XtVcMotor::getAlarmState()
{
    if(error_code == get_motor_error(vcm_id))
    {
        if(states.seekedOrigin())
            states.setSeekedOrigin(false);
        return true;
    }
    return false;
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

bool XtVcMotor::SeekOrigin(int thread)
{
    if(!checkState(false))return false;
    int result;
    qInfo("sadas");
    ChangeDiretion(true);
    if(parameters.findOriginCurrent() > 0)
        result = Touch_Go_Zero(vcm_id,parameters.findOriginCurrent(),parameters.touchDistance());
    else {

        SetZeroPos(vcm_id,0);
        result = Init_Go_Zero(vcm_id);
    }
    if(result == 1)
    {
        states.setSeekedOrigin(true);
        qInfo("axis %s seek origin succees!",name.toStdString().c_str());
    }
    else
    {
        states.setSeekedOrigin(false);
        AppendError(QString(u8"%1回零失败").arg(name));
        qInfo("axis %s seek origin fail!",name.toStdString().c_str());
    }
    ChangeDiretion();
    return states.seekedOrigin();
}



bool XtVcMotor::WaitSeekDone(int thread,int timeout)
{
    if(!checkState(false))return false;
    qInfo("WaitSeekDone %d",states.seekedOrigin());
    return states.seekedOrigin();
}

void XtVcMotor::GetMasterAxisID()
{
    axis_sub_id = axis_id;
}


bool XtVcMotor::SearchPosByADC(double vel, double search_limit, double threshold, bool search_above, double &result)
{

    return false;
}

bool XtVcMotor::SearchPosByForce(const double speed,const double force,const double limit,const double margin,const int timeout)
{
    if(is_debug)return true;
    if(!(checkState()&&checkLimit(limit)&&checkInterface(limit)))return false;
    double start_pos = GetOutpuPos();
    qInfo("start_pos: %f,force:%f,search_limit:%f speed:%f margin:%f",start_pos,force,limit,speed,margin);
    SetSoftLanding(speed, max_acc, force, start_pos, limit, margin);
    bool res;
    res = DoSoftLanding();
    res &= WaitSoftLandingDone(timeout);
    if(res)
        qInfo("sooftlanding pos:%f",GetFeedbackPos());
    else
        AppendError(QString(u8"搜索位置失败 开始位置 %1 力 %2 目标位置 %3 速度 %4 到位区间 %5").arg(start_pos).arg(force).arg(limit).arg(speed).arg(margin));
    return res;
}

bool XtVcMotor::SearchPosByForce(const double speed,const double force,const int timeout)
{
    if(is_debug)return true;
    if(!(checkState()&&checkLimit(max_range)&&checkInterface(max_range)))return false;
    double start_pos = GetOutpuPos();
    double limit = start_pos + (max_range - start_pos)/2;
    double margin = abs(max_range - start_pos)/2.1;
    SetSoftLanding(speed,max_acc, force, start_pos,start_pos + (max_range - start_pos)/2,abs(max_range - start_pos)/2.1);
    bool res;
    res = DoSoftLanding();
    res &= WaitSoftLandingDone(timeout);
    if(res)
        qInfo("sooftlanding pos:%f",GetFeedbackPos());
    else
        AppendError(QString(u8"搜索位置失败 开始位置 %1 力 %2 目标位置 %3 速度 %4 到位区间 %5").arg(start_pos).arg(force).arg(limit).arg(speed).arg(margin));
    return res;
}

double XtVcMotor::SearchPosByForceOnyDown(double speed, double force, int timeout)
{
    if(!is_init)
        return 0.0;
    double start_pos = GetOutpuPos();
    SetSoftLanding(speed,max_acc, force, start_pos,start_pos + (max_range - start_pos)/2,abs(max_range - start_pos)/2.1);
    bool res;
    res = DoSoftLanding();
    res &= WaitSoftLandingDone(timeout);
    if(res)
        return  GetFeedbackPos();
    return start_pos;
}

void XtVcMotor::RestoreForce()
{
    if(!is_init)
        return;
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
    qInfo("slow_speed:%f start_pos: %f,force:%f,target_pos:%f,margin:%f",slow_speed,start_pos,force,target_pos,margin);
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
    if(!is_init)
        return false;
    is_softlanding = true;
    int res = Do_SoftDown(vcm_id);
    if(res==0)
        return true;
    AppendError(QString(u8"%1 软着陆伸出失败 错误码 %2").arg(name).arg(res));
//    qInfo("VCM %d DoSoftLanding Failed! code: %d",vcm_id,res);
    return false;
}

bool XtVcMotor::DoSoftLandingReturn()
{
    if(is_debug)return true;
    if(!is_init)
        return false;
    is_returning = true;
    int res = Do_SoftUp(vcm_id);
    if(res==0)
        return true;
    AppendError(QString(u8"%1 软着陆返回失败 错误码 %2").arg(name).arg(res));
//    qInfo("VCM %d DoSoftLandingReturn Failed! code: %d",vcm_id,res);
    return false;
}

bool XtVcMotor::resetSoftLanding(int timeout)
{
    bool ret = true;
    if(is_softlanding|is_returning)
        if(!WaitSoftLandingDone(timeout))
            ret = false;
    if(is_softlanded)
        ret = DoSoftLandingReturn()&WaitSoftLandingDone(timeout);
    RestoreForce();
    if(!ret)
        AppendError(QString(u8"%1 软着陆复位失败").arg(name));
    return ret;
}

bool XtVcMotor::WaitSoftLandingDone(int timeout)
{
    if(is_debug)return true;
    if(!checkState(false))return false;
    while(timeout > 0)
    {
        int res = CheckPosReady(vcm_id);
        if (res == 1)
        {
            is_softlanded = is_softlanding;
            is_softlanding = false;
            is_returning = false;
            return true;
        }
        timeout-=10;
        QThread::msleep(10);
    }
    is_softlanded = is_softlanding;
    is_softlanding = false;
    is_returning = false;
    LONG64 temp_code = get_motor_error(vcm_id);
    if(error_code == temp_code)
    {
        qInfo("CheckPosReady fail but action success outtime %d",timeout);
        return true;
    }
    AppendError(QString(u8"%1 等待软着陆伸出/软着陆返回完成超时 错误码 %2 - %3").arg(name).arg(temp_code).arg(error_code));
    error_code = temp_code;
//    qInfo("WaitSoftLandingDone fail");
    return false;
}






