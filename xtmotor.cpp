#include "XtGeneralInput.h"
#include "XtGeneralOutput.h"
#include "xtmotor.h"
#include "XT_MotionControler_Client_Lib.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include "logger.h"
#include "config.h"

using namespace XT_Controler_Extend;
int XtMotor::curve_resource = 0;
int XtMotor::thread_resource = 0;
int XtMotor::axis_id_resource = 0;
XtMotor::XtMotor()
{
    axis_id = -1;
    name = "";
    is_init = false;
}

void XtMotor::WaitSync(int thread)
{
    XT_Controler::WaitForAllInsFinish(thread);
}

void XtMotor::SetAxisDelay()
{
//    QJsonObject json = XtMotion::motor_extend_param[name].toObject();
//    qInfo("%s set delay",name.toStdString().c_str());
//    if(json.isEmpty()!=true)
//    {
//        qInfo("%s motor_extend_param not empty",name.toStdString().c_str());
//        if(axis_sub_id!=axis_id)
//        {XtMotorExtendParameters p;
//            p.read(json);
//            double delay_in_sec = p.Delay()/1000;

//            Set_Axis_Delay(axis_sub_id,delay_in_sec);
//            qInfo("%s set delay to %f second",name.toStdString().c_str(), delay_in_sec);
//        }
//    }
}

void XtMotor::ClearAxisDelay()
{
    if(axis_sub_id!=axis_id)
    {
        //Set_Axis_Delay(axis_sub_id,0);
    }
}

void XtMotor::GetMasterAxisID()
{
    int temp = Profile_Get_Axis_Master(axis_sub_id);
    if(temp<0||temp == axis_sub_id)
    {
        return;
    }
    else {
        axis_id = temp;
        qInfo("Axis %s vid is %d, pid is %d",name.toStdString().c_str(),axis_id,axis_sub_id);
        SetVel(max_vel);
        SetAcc(max_acc);
        SetJerk(max_jerk);
        Set_Axis_Bind(axis_sub_id,axis_id,1,1);
        XT_Controler::WaitForAllInsFinish(default_using_thread);
    }
}

void XtMotor::Init(const QString& moter_name)
{
    if(is_init)
        return;
    this->name = moter_name;


    axis_sub_id = Profile_Find_Axis_Name((LPWSTR)name.utf16());
    axis_id = axis_sub_id;

    default_using_thread = axis_sub_id + 20;

    out_en_id = Profile_Get_Axis_Enable_Output_ID(axis_sub_id);
    out_clr_id = Profile_Get_Axis_ClrAlm_Output_ID(axis_sub_id);
    en.Init(out_en_id);
    clr.Init(out_clr_id);

    in_inp_id = Profile_Get_Axis_InPos_Input_ID(axis_sub_id);
    in_alm_id = Profile_Get_Axis_Alarm_Input_ID(axis_sub_id);
    in_rdy_id = Profile_Get_Axis_Ready_Input_ID(axis_sub_id);
    inp.Init(in_inp_id);
    alm.Init(in_alm_id);
    rdy.Init(in_rdy_id);

    in_origin_id = -1;

    max_vel = Profile_Get_Axis_Vel(axis_id);
    max_acc = Profile_Get_Axis_Acc(axis_id);
    max_jerk = Profile_Get_Axis_Jerk(axis_id);

    adc = nullptr;
    encoder_id = -1;
    encoder_ratio = 1.0;
    max_range = Profile_Get_Axis_MaxPos(axis_id);
    min_range = Profile_Get_Axis_MinPos(axis_id);
    is_enable = false;
    is_init = true;

}


void XtMotor::SetADC(int can_id, int data_ch)
{
    adc = new XtADCModule();
    adc->InitResource(can_id,data_ch);
}

void XtMotor::SetEncoderFeedback(int can_id, int data_ch, double ratio)
{
    if(!is_init)
        return;
    if(encoder_id>0)
        return;
    int temp = default_using_thread;
    Encoder_Init(can_id,data_ch+20,temp);
    encoder_ratio = ratio;
    encoder_id = temp;//set encoder_id in last to ensure no read conflict
}

void XtMotor::SetFeedbackZero(double new_value)
{
    if(!is_init)
        return;
    if(encoder_id<1)
        return;
    if(encoder_ratio == 0)
        return;
    long long temp = new_value / encoder_ratio;
    XT_Controler_Extend::Encoder_Write_Value(encoder_id, temp);
}

void XtMotor::Enable()
{
    if(!is_init)
        return;
    XT_Controler::SetCurIoOutput(out_en_id,1);
    if(GetFeedbackPos()!=GetOutpuPos())
        ChangeCurPos(GetFeedbackPos());
    is_enable = true;
}

void XtMotor::Disable()
{
    if(!is_init)
        return;
    XT_Controler::SetCurIoOutput(out_en_id,0);
    is_enable = false;
}

QString XtMotor::Name() const
{
    return name;
}

double XtMotor::GetOutpuPos() const
{
    if(!is_init)
        return 0;
    double val;
    int res = XT_Controler_Extend::Get_Cur_Axis_Pos(axis_id,val);
    if(res!=0)
        return val;
    return 0;
}

double XtMotor::GetFeedbackPos() const
{
    if(encoder_id>0)
    {
        long long val;
        int res = XT_Controler_Extend::Encoder_Read_Value(encoder_id,val);
        if(res!=0)
            return val * encoder_ratio;
    }
    return GetOutpuPos();
}

double XtMotor::GetCurVel() const
{
    if(!is_init)
        return 0;
    double val;
    int res = XT_Controler_Extend::Get_Cur_Axis_Vel(axis_id,val);
    if(res!=0)
        return val;
    return 0;
}

double XtMotor::GetCurAcc() const
{
    if(!is_init)
        return 0;
    double val;
    int res = XT_Controler_Extend::Get_Cur_Axis_Acc(axis_id,val);
    if(res!=0)
        return val;
    return 0;
}

double XtMotor::GetCurADC() const
{
    if(adc!= nullptr)
        return adc->GetValue();
    return 0;
}

bool XtMotor::IsRunning() const
{
    if(!is_init)
        return false;
    int be_run;
    XT_Controler_Extend::Get_Cur_Axis_State(axis_id,be_run);
    return be_run == 1;
}

double XtMotor::GetPostiveRange() const
{
    return max_range;
}

double XtMotor::GetNegativeRange() const
{
    return min_range;
}

int XtMotor::AxisId() const
{
    return axis_id;
}

double XtMotor::GetMaxVel() const
{
    return max_vel;
}

double XtMotor::GetMaxAcc() const
{
    return max_acc;
}

double XtMotor::GetMaxJerk() const
{
    return max_jerk;
}

void XtMotor::SetVel(double vel, int thread)
{
    if(!is_init)
        return;
    if(thread==-1)
        thread = default_using_thread;
    XT_Controler::SET_MAX_VEL(thread,axis_id,vel);
}

void XtMotor::SetAcc(double vel, int thread)
{
    if(!is_init)
        return;
    if(thread==-1)
        thread = default_using_thread;
    XT_Controler::SET_MAX_ACC(thread,axis_id,vel);
}

void XtMotor::SetJerk(double jerk, int thread)
{
    if(!is_init)
        return;
    if(thread==-1)
        thread = default_using_thread;
    XT_Controler::SET_MAX_JERK(thread,axis_id,jerk);
    XT_Controler::WaitForAllInsFinish(thread);
}

void XtMotor::SetPostiveRange(double range)
{
    max_range = range;
}

void XtMotor::SetNegativeRange(double range)
{
    min_range = range;
}

void XtMotor::Home(int thread)
{
    MoveToPos(0, thread);
}

void XtMotor::CheckLimit(double &pos)
{
    if(pos>max_range)
    {
        pos=max_range;
    }

    if(pos<min_range)
    {
        pos=min_range;
    }
}

void XtMotor::MoveToPos(double pos,int thread)
{
    if(!is_init)
        return;
    if(!is_enable)
        return;
    CheckLimit(pos);
    if(thread==-1)
        thread = default_using_thread;
    XT_Controler::SGO(thread, axis_id, pos);
    XT_Controler::TILLSTOP(thread, axis_id);
}

void XtMotor::SlowMoveToPos(double pos, double vel_ratio, int thread)
{
    if(!is_init)
        return;
    if(!is_enable)
        return;
    CheckLimit(pos);
    if(thread==-1)
        thread = default_using_thread;
    if(vel_ratio > 1||vel_ratio <= POS_ERROR)
        vel_ratio = 1;
    XT_Controler::SET_MAX_VEL(thread,axis_id,max_vel*vel_ratio);
    XT_Controler::SGO(thread, axis_id, pos);
    XT_Controler::TILLSTOP(thread, axis_id);
    XT_Controler::SET_MAX_VEL(thread,axis_id,max_vel);
}

void XtMotor::SGO(double pos,int thread)
{
    if(!is_init)
        return;
    if(!is_enable)
        return;
    CheckLimit(pos);
    if(thread==-1)
        thread = default_using_thread;
    XT_Controler::SGO(thread, axis_id, pos);
}
void XtMotor::TILLSTOP(int thread)
{
    if(!is_init)
        return;
    if(thread==-1)
        thread = default_using_thread;
    XT_Controler::TILLSTOP(thread, axis_id);
}

void XtMotor::TILLTIME(int ms, int thread)
{
    if(!is_init)
        return;
    if(thread==-1)
        thread = default_using_thread;
    XT_Controler::TILLTIME(thread, ms);
}

bool XtMotor::WaitMoveStop(int timeout)
{
    if(!is_init)
        return false;
    if(!is_enable)
        return false;

    int thread = default_using_thread;
    int buffer_len, finish;
    while(timeout>0)
    {
        XT_Controler::GetThreadInsBufferRemainCount(thread, buffer_len, finish);
        if (buffer_len == 0 && finish == 1)
        {
            return true;
        }
        timeout-=1;
        QThread::msleep(1);
    }
    XT_Controler::ClearInsBuffer(thread);
    XT_Controler::STOP_S(thread, axis_id);
    return false;
}

bool XtMotor::WaitArrivedTargetPos(double target_position, int timeout)
{

    if(!is_init)
        return false;
    if(!is_enable)
        return false;
    if(fabs(GetFeedbackPos() - target_position) < POS_ERROR)return true;
    while (timeout>0) {

        if(fabs(GetFeedbackPos() - target_position) < POS_ERROR)
        {
            Sleep(10);//todo根据不同的轴到位等待时间
            return true;
        }
        Sleep(10);
        timeout--;

    }
    return false;
}


bool XtMotor::MoveToPosSync(double pos, int thread)
{
    if(!is_init)
        return false;
    if(!is_enable)
        return false;
    double currPos = GetFeedbackPos();
    double targetPos = pos;
    MoveToPos(pos,thread);
    int count = 3000;
    while ( fabs(currPos - targetPos) >= POS_ERROR)
    {
        currPos = GetFeedbackPos();
        Sleep(10);
        count-=10;
        if (count < 1) {
            qInfo("Motion Timeout.target pos:%f current pos:%f",pos,currPos);
            return false;
        }
    }
    return true;
}

bool XtMotor::SlowMoveToPosSync(double pos, double vel_ratio, int thread)
{
    if(!is_init)
        return false;
    if(!is_enable)
        return false;
    double currPos = GetFeedbackPos();
    double targetPos = pos;
    SlowMoveToPos(pos,vel_ratio,thread);
    int count = 10000;
    while ( fabs(currPos - targetPos) >= POS_ERROR)
    {
        currPos = GetFeedbackPos();
        Sleep(10);
        count-=10;
        if (count < 1) {
            qInfo("Motion Timeout.");
            return false;
        }
    }
    return true;
}

void XtMotor::StepMove(double step, int thread)
{
    if(!is_init)
        return;
    if(!is_enable)
        return;
    if(thread==-1)
        thread = default_using_thread;
    if(step>0)
        XT_Controler_Extend::SGO_INCREASE_LIMIT(thread, axis_id, step, max_range);
    else
        XT_Controler_Extend::SGO_INCREASE_LIMIT(thread, axis_id, step, min_range);
}

bool XtMotor::StepMoveSync(double step, int thread)
{
    if(!is_init)
        return false;
    if(!is_enable)
        return false;
    double currPos = GetFeedbackPos();
    double targetPos = currPos+step;
    StepMove(step, thread);
    int count = 10000;
    while ( fabs(currPos - targetPos) >= 0.001)
    {
        currPos = GetFeedbackPos();
        Sleep(10);
        count-=10;
        if (count == 0) {
            qInfo("Motion Timeout.");
            return false;
        }
    }
    return true;
}

bool XtMotor::StepMoveSync(double step, bool dir, int thread)
{
    double temp_step = step;
    if(!dir)
        temp_step = - step;
    return StepMoveSync(temp_step,thread);
}

void XtMotor::SeekOrigin(int thread)
{
    if(!is_init)
        return;
    if(!is_enable)
        return;
    if(thread==-1)
        thread = default_using_thread;
    ClearAxisDelay();
    XT_Controler_Extend::PROFILE_AXIS_SEEK_ORIGIN(thread,axis_sub_id);
}

void XtMotor::StopSeeking(int thread)
{
    if(!is_init)
        return;
    if(!is_enable)
        return;
    if(thread==-1)
        thread = default_using_thread;
    XT_Controler::ClearInsBuffer(thread);
    XT_Controler::STOP_S(thread, axis_id);
}



bool XtMotor::WaitSeekDone(int thread,int timeout)
{
    if(!is_init)
        return false;
    if(!is_enable)
        return false;
    if(thread==-1)
        thread = default_using_thread;
    int buffer_len, finish;
    while(timeout>0)
    {
        XT_Controler::GetThreadInsBufferRemainCount(thread, buffer_len, finish);
        if (buffer_len == 0 && finish == 1)
        {
            SetFeedbackZero(GetOutpuPos());
            return true;
        }
        timeout-=10;
        QThread::msleep(10);
    }
    XT_Controler::ClearInsBuffer(thread);
    XT_Controler::STOP_S(thread, axis_id);
    qInfo("axis %s seek origin fail！",name.toStdString().c_str());
    return false;
}

bool XtMotor::WaitStop(int timeout)
{
    if(!is_init)
        return true;
    if(!is_enable)
        return true;
    int be_run = 1;

    while( timeout > 0 )
    {
        XT_Controler_Extend::Get_Cur_Axis_State(axis_id,be_run);
        if (be_run == 0)
        {
            return true;
        }
        timeout-=10;
        QThread::msleep(10);
    }
    return false;
}

bool XtMotor::SearchPosByADC(double vel, double search_limit, double threshold, bool search_above, double &result)
{
    if(!is_init)
        return false;
    if(!is_enable)
        return false;
    int timeout = 100;
    double adc_value;
    int thread = default_using_thread;
    if(search_limit > max_range)
    {
        search_limit = max_range;
    }

    if(search_limit < min_range)
    {
        search_limit = min_range;
    }
    double old_vel = GetMaxVel();
    XT_Controler::SET_MAX_VEL(thread, axis_id, vel);
    XT_Controler::SGO(thread, axis_id, search_limit);
    while(timeout>0)
    {
        adc_value = adc->GetValue();
        result = GetFeedbackPos();
        if(search_above)//need value heigher than threshld
        {
            if(adc_value>threshold)
            {
                XT_Controler::STOP_T(thread, axis_id);
                XT_Controler::WaitForAllInsFinish(thread);
                //SetVel(old_vel, thread);
                qInfo("SearchPosByADC true");
                return true;
            }
        }
        else//need value lower than threshld
        {
            if(adc_value<threshold)
            {
                XT_Controler::STOP_T(thread, axis_id);
                XT_Controler::WaitForAllInsFinish(thread);
                //SetVel(old_vel, thread);
                qInfo("SearchPosByADC true");
                return true;
            }
        }
        if(fabs(result - search_limit) < 0.001)
        {
            //SetVel(old_vel, thread);
            qInfo("SearchPosByADC over search_limit false");
            return false;
        }
            timeout--;
        QThread::msleep(10);
    }
    //SetVel(old_vel, thread);
    qInfo("SearchPosByADC timeout false");
    return false;
}

bool XtMotor::SearchPosByIO(double vel, double search_limit, bool search_rise, int io_id, double &result)
{
    if(!is_init)
        return false;
    if(!is_enable)
        return false;
    int thread = default_using_thread;
    int timeout = 30000;
    double currPos = GetOutpuPos();
    double negative = min_range;
    double old_vel = GetMaxVel();
    qInfo("Tray Vel:%f",vel);
    SetVel(vel, thread);
    if(search_limit>max_range)
    {
        search_limit=max_range;
    }

    if(search_limit<min_range)
    {
        search_limit=min_range;
    }
    if(search_limit < currPos)
        negative = max_range;


    XT_Controler::USE_LOCAL_REG(thread);
    int axisPosReg=0;

    int	iAxisPosReg = 0;

    XT_Controler::GET_INPUT_IO(thread, io_id, axisPosReg);

    if (search_rise)//判断现在是不是触发状态1
        XT_Controler::CONDITION_NEG_JMP(thread, axisPosReg, 1, 2);//不是1就跳走
    else
        XT_Controler::CONDITION_NEG_JMP(thread, axisPosReg, 0, 2);

    XT_Controler::SGO(thread, axis_id, negative);//是的话先离开

    if (search_rise)
        XT_Controler::TILLIO_NEG(thread, io_id);//等到非1
    else
        XT_Controler::TILLIO(thread, io_id);

    XT_Controler::SGO(thread, axis_id, search_limit);//向目标搜索点进发

    if (search_rise)
    {
        XT_Controler::TILLIO(thread, io_id);//等到1
    }
    else
    {
        XT_Controler::TILLIO_NEG(thread, io_id);
    }

    XT_Controler::GET_OUTPUT_POS(thread, axis_id, iAxisPosReg);//记下当前位置

    XT_Controler::ADD_R_C(thread, iAxisPosReg, iAxisPosReg, 0);

    XT_Controler::SGO_R(thread, axis_id, iAxisPosReg);

    XT_Controler::TILLSTOP(thread, axis_id);

    int buffer_len, finish;
    while(timeout>0)
    {
        XT_Controler::GetThreadInsBufferRemainCount(thread, buffer_len, finish);
        if (buffer_len == 0 && finish == 1)
        {
            result = GetFeedbackPos();
            SetVel(old_vel, thread);
            return true;
        }
        timeout-=10;
        QThread::msleep(10);
    }
    XT_Controler::ClearInsBuffer(thread);
    SetVel(old_vel, thread);
    XT_Controler::STOP_S(thread, axis_id);
    result = 0;
    return false;

}

bool XtMotor::IsInsideRange(double target_pos, double error)
{
    return (target_pos > min_range - error)&&(target_pos < max_range + error);
}

void XtMotor::ChangeCurPos(double pos)
{
    if(!is_init)
        return;
    XT_Controler::SET_AXIS_ZEROS(0,axis_id,pos);
    XT_Controler::WaitForAllInsFinish(0);
}



XtMotorExtendParameters::XtMotorExtendParameters()
{
    m_Delay = 0;
}

double XtMotorExtendParameters::Delay() const
{
    return m_Delay;
}

void XtMotorExtendParameters::setDelay(double Delay)
{
    m_Delay = Delay;
}
