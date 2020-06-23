#include "XtGeneralInput.h"
#include "XtGeneralOutput.h"
#include "xtmotor.h"
#include "XT_MotionControler_Client_Lib.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
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
//    limit_parameters.append(new MotorLimitParameter());
//    limit_parameters.append(new MotorLimitParameter());
//    io_limit_parameters.append(new IOLimitParameter());
}

void XtMotor::WaitSync(int thread)
{
    XT_Controler::WaitForAllInsFinish(thread);
}

int XtMotor::GetCurveResource()
{
    return curve_resource++;
    qInfo("current curve Resource %d",thread_resource);
}

int XtMotor::GetThreadResource()
{
    qInfo("current thread Resource %d",thread_resource);
    return thread_resource++;
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
    setName(moter_name);
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
    states.setIsEnabled(false);
    is_init = true;

}

void XtMotor::Init(DeviceStatesGeter *geter)
{
    this->geter = geter;
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
    states.setIsEnabled(true);
}

void XtMotor::Disable()
{
    if(!is_init)
        return;
    XT_Controler::SetCurIoOutput(out_en_id,0);
    states.setIsEnabled(false);
    states.setSeekedOrigin(false);
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

double XtMotor::GetFeedbackPos(int decimal_digit) const
{
    if(encoder_id>0)
    {
        long long val;
        int res = XT_Controler_Extend::Encoder_Read_Value(encoder_id,val);
        if(res!=0)
            return round(val * encoder_ratio*pow(10,decimal_digit))/pow(10,decimal_digit);
    }
    return round(GetOutpuPos()*pow(10,decimal_digit))/pow(10,decimal_digit);
}

double XtMotor::GetCurrentTragetPos()
{
    return current_target;
}

void XtMotor::SetCurrentTragetPos(double target)
{
   current_target = target;
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

bool XtMotor::checkAlarm()
{
    if(getAlarmState())
    {
        AppendError(u8"轴");
        AppendError(name);
        AppendError(u8")伺服报警!");
        qInfo("%s alarm",name.toStdString().c_str());
        return true;
    }
    return false;
}

bool XtMotor::getAlarmState()
{
    bool temp = false;
    if(parameters.reverseAlarmIO())
    {
        temp = !alm.Value();
    }
    else
    {
        temp = alm.Value();
    }
    if(temp)
    {
        if(states.seekedOrigin())
            states.setSeekedOrigin(false);
        return true;
    }
    return false;
}

bool XtMotor::clearAlarmState()
{
    XT_Controler::SetCurIoOutput(out_en_id,0);
    XT_Controler::SetCurIoOutput(out_clr_id,1);
    QThread::msleep(500);
    XT_Controler::SetCurIoOutput(out_en_id,1);
    XT_Controler::SetCurIoOutput(out_clr_id,0);
    return true;
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

void XtMotor::ResetVel(int thread)
{
    if(!is_init)
        return;
    if(thread==-1)
        thread = default_using_thread;
    XT_Controler::SET_MAX_VEL(thread,axis_id,max_vel);
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

bool XtMotor::setTrig(bool reverse_trig, double trig_pos, int io_id, bool trig_state)
{
   bool result = XT_Controler_Extend::Set_Axis_Trig_Output(axis_id,reverse_trig?1:0,trig_pos,0,1,io_id,trig_state?1:0,0);
    if(result == 0)
    {
        qInfo("Set_Axis_Trig_Output fail");
        return false;
    }
    return true;
}

bool XtMotor::clearTrig()
{
    int result = XT_Controler_Extend::Clear_Axis_Trig(axis_id);
    if(result == 0)
    {
        qInfo("Clear_Axis_Trig fail");
        return false;
    }
    return true;
}

//void XtMotor::CheckLimit(double &pos)
//{
//    if(!qIsFinite(pos))
//    {
//        qInfo("target_position %f is illegal,change to %f",pos,min_range);
//        pos = min_range;
//    }

//    if(pos>max_range)
//    {
//        qInfo("target_position %f to big,change to %f",pos,max_range);
//        pos=max_range;
//    }

//    if(pos<min_range)
//    {
//        qInfo("target_position %f to small,change to %f",pos,min_range);
//        pos = min_range;
//    }
//}

bool XtMotor::MoveToPos(double pos,int thread)
{
    if (!is_init) return false;
    if(is_debug) return true;
    if(parameters.firstCheckArrived()&&fabs(pos - GetFeedbackPos())<parameters.positionError())
            return true;
    if(!(checkState()&&checkLimit(pos)&&checkInterface(pos)))return false;
    if(thread==-1)
        thread = default_using_thread;
    XT_Controler::SGO(thread, axis_id, pos);
    XT_Controler::TILLSTOP(thread, axis_id);
    current_target = pos;
    return true;
}

bool XtMotor::SlowMoveToPos(double pos, double low_vel, int thread)
{
    if(is_debug) return true;
    if(!(checkState()&&checkLimit(pos)&&checkInterface(pos)))return false;
    if(thread==-1)
        thread = default_using_thread;
    if(low_vel > max_vel)
        low_vel = max_vel;
    XT_Controler::SET_MAX_VEL(thread,axis_id,low_vel);
    XT_Controler::SGO(thread, axis_id, pos);
    XT_Controler::TILLSTOP(thread, axis_id);
    XT_Controler::SET_MAX_VEL(thread,axis_id,max_vel);
    current_target = pos;
    return true;
}

bool XtMotor::SGO(double pos,int thread)
{
    if(is_debug) return true;
    if(!(checkState()&&checkLimit(pos)&&checkInterface(pos)))return false;
    if(thread==-1)
        thread = default_using_thread;
    XT_Controler::SGO(thread, axis_id, pos);
    current_target = pos;
    return true;
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
    if(is_debug)return true;
    if(!(checkState(false)))return false;
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
    if(is_debug)return true;
    if(!(checkState()))return false;
    int current = 0;
    while (fabs(GetFeedbackPos() - target_position) > parameters.positionError()) {
        if(current > timeout)
        {
            qInfo("%s wait target_position:%f time out, current_position:%f",name.toStdString().c_str(),target_position,GetFeedbackPos());
            current_target = GetFeedbackPos();
            return false;
        }
        Sleep(10);
        current+=10;
    }
//    qInfo("%s arrived %f time %d",name.toStdString().c_str(),target_position,current);
    if(parameters.useDelay())
        Sleep(parameters.arrivedDelay());
//    current_target = GetFeedbackPos();
    return true;
}

bool XtMotor::WaitLessThanTargetPos(double target_position, int timeout)
{
    if(is_debug)return true;
    if(!(checkState()))return false;
    int current = 0;
    while (GetFeedbackPos() > target_position) {
        if(current > timeout)
        {
            qInfo("%s wait less than target_position:%f time out, current_position:%f",name.toStdString().c_str(),target_position,GetFeedbackPos());
            return false;
        }
        Sleep(10);
        current+=10;
    }
    return true;
}

bool XtMotor::WaitArrivedTargetPos(double target_position, double arived_error, int timeout)
{
    if(is_debug)return true;
    if(!(checkState()))return false;
    int current = 0;
    while (fabs(GetFeedbackPos() - target_position) > arived_error) {
        if(current > timeout)
        {
            qInfo("%s wait target_position:%f time out, current_position:%f",name.toStdString().c_str(),target_position,GetFeedbackPos());
            current_target = GetFeedbackPos();
            return false;
        }
        Sleep(10);
        current+=10;
    }
    return true;
}

bool XtMotor::WaitArrivedTargetPos(int timeout)
{
    return  WaitArrivedTargetPos(current_target,timeout);
}


bool XtMotor::MoveToPosSync(double pos, int thread,int time_out)
{
//    if(is_debug)return true;
//    if(!(checkState()&&checkLimit(pos)&&checkInterface(pos)))return false;
//    double currPos = GetFeedbackPos();
//    double targetPos = pos;
    //qInfo("MoveToPosSync pos %f time_out %d",pos,time_out);
    if(!MoveToPos(pos,thread))
    {
        qInfo("%s move to pos %f fail",name.toStdString().c_str(),pos);
        return false;
    }
    return  WaitArrivedTargetPos(pos,time_out);
//    int count = time_out;
//    while ( fabs(currPos - targetPos) >= parameters.positionError())
//    {
//        Sleep(10);
//        currPos = GetFeedbackPos();
//        count-=10;
//        if (count < 1) {
//            qInfo("%s Motion Timeout.target pos:%f current pos:%f",name.toStdString().c_str(),pos,currPos);
//            current_target = currPos;
//            return false;
//        }
//    }
//    current_target = currPos;
//    return true;
}

bool XtMotor::MoveToPosSync(double pos, double arrived_error, int thread, int time_out)
{
    if(!MoveToPos(pos,thread))
    {
        qInfo("%s move to pos %f fail",name.toStdString().c_str(),pos);
        return false;
    }
    return  WaitArrivedTargetPos(pos,arrived_error,time_out);
}

bool XtMotor::MoveToMinPosSync(int time_out)
{
    bool result =MoveToPos(min_range);
    result &= WaitArrivedTargetPos(time_out);
    return result;
}

bool XtMotor::MoveToPosSafty(double pos,int thread)
{
    if(is_debug)return true;
    double limit_pos;
    if(!getInterfaceLimit(pos,limit_pos))
        return false;
    if(!(checkState()&&checkLimit(limit_pos)&&checkInterface(limit_pos)))
        return false;
    if(thread==-1)
        thread = default_using_thread;
    if(abs(current_target -limit_pos)> 0.000001)
        XT_Controler::SGO(thread, axis_id, limit_pos);
    current_target = limit_pos;
    return true;
}

bool XtMotor::CheckArrivedTargetPos(double target_position)
{
    if(is_debug)return true;
    if(!(checkState()))return false;
    double feedbackPos = GetFeedbackPos();
    if(fabs(GetFeedbackPos() - target_position) < parameters.positionError())
        return true;
    return false;
}

bool XtMotor::CheckArrivedTargetPos()
{
    if(is_debug)return true;
    if(!(checkState()))return false;
    if(fabs(GetFeedbackPos() - current_target) < parameters.positionError())
        return true;
    return false;
}

bool XtMotor::MoveToPosSaftySync(double pos,int thread,int timeout)
{
    if(is_debug)return true;
    if(!(checkState()))return false;
    if(CheckArrivedTargetPos(pos))
        return true;
    while (timeout > 0) {
        if(CheckArrivedTargetPos(pos))
            return true;
        else
            if(!MoveToPosSafty(pos,thread))
                return false;
        Sleep(30);
        timeout-=30;
    }
    qInfo("MoveTo Pos :%f Safty time out, current_position:%f",pos,GetFeedbackPos());
    current_target = GetFeedbackPos();
    return false;
}


bool XtMotor::SlowMoveToPosSync(double pos, double low_vel, int thread)
{
    if(is_debug)return true;
    if(!(checkState()&&checkLimit(pos)&&checkInterface(pos)))return false;
    double currPos = GetFeedbackPos();
    double targetPos = pos;
    SlowMoveToPos(pos,low_vel,thread);
    int count = 10000;
    while ( fabs(currPos - targetPos) >= parameters.positionError())
    {
        currPos = GetFeedbackPos();
        Sleep(10);
        count-=10;
        if (count < 1) {
            qInfo("Motion Timeout.");
            current_target = currPos;
            return false;
        }
    }
    current_target = currPos;
    return true;
}

bool XtMotor::StepMove(double step, int thread)
{
    if(is_debug)return true;
    if(!checkState(false))return false;
    if(!checkLimit(GetFeedbackPos() + step))return false;
    if(thread==-1)
        thread = default_using_thread;
    if(step>0)
        XT_Controler_Extend::SGO_INCREASE_LIMIT(thread, axis_id, step, max_range);
    else
        XT_Controler_Extend::SGO_INCREASE_LIMIT(thread, axis_id, step, min_range);
    current_target = GetFeedbackPos() + step;
    return true;
}

bool XtMotor::StepMoveSync(double step, int thread)
{
    if(is_debug)return true;
    if(!checkState(false))return false;
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
            qInfo("Motion Timeout. id %d init %d,targetPos %f currPos %f",axis_sub_id,is_init,targetPos,currPos);
            current_target = currPos;
            return false;
        }
    }
    current_target = currPos;
    return true;
}

bool XtMotor::StepMoveSync(double step, bool dir, int thread)
{
    if(is_debug)return true;
    double temp_step = step;
    if(!dir)
        temp_step = - step;
    return StepMoveSync(temp_step,thread);
}

bool XtMotor::SeekOrigin(int thread)
{
    if(!checkState(false))return false;
    if(thread==-1)
        thread = default_using_thread;
    ClearAxisDelay();
    XT_Controler_Extend::PROFILE_AXIS_SEEK_ORIGIN(thread,axis_sub_id);
    return true;
}

bool XtMotor::StopSeeking(int thread)
{
    if(!checkState(false))return false;
    if(thread==-1)
        thread = default_using_thread;
    XT_Controler::ClearInsBuffer(thread);
    XT_Controler::STOP_S(thread, axis_id);
    return true;
}



bool XtMotor::WaitSeekDone(int thread,int timeout)
{
    if(is_debug)return true;
    if(!checkState(false))return false;
    if(thread==-1)
        thread = default_using_thread;
    int buffer_len, finish;
    while(timeout>0)
    {
        XT_Controler::GetThreadInsBufferRemainCount(thread, buffer_len, finish);
        if (buffer_len == 0 && finish == 1)
        {
            QThread::msleep(100);
            SetVel(max_vel);
            SetFeedbackZero(GetOutpuPos());
            current_target = GetOutpuPos();
            qInfo("axis %s seek origin %f success",name.toStdString().c_str(),GetOutpuPos());
            states.setSeekedOrigin(true);
            return true;
        }
        timeout-=10;
        QThread::msleep(10);
    }
    XT_Controler::ClearInsBuffer(thread);
    XT_Controler::STOP_S(thread, axis_id);
    current_target = GetOutpuPos();
    qInfo("axis %s seek origin fail!",name.toStdString().c_str());
    return false;
}

bool XtMotor::WaitStop(int timeout)
{
    if(is_debug)return true;
    if(!checkState())return false;
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
    current_target = GetFeedbackPos();
    return false;
}

bool XtMotor::SearchPosByADC(double vel, double search_limit, double threshold, bool search_above, double &result)
{
    if(is_debug)return true;
    if(!is_init)
        return false;
    if(!states.isEnabled())
        return false;
    int timeout = 100;
    double adc_value;
    int thread = default_using_thread;
    if(!(checkState()&&checkLimit(search_limit)&&checkInterface(search_limit)))return false;
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
    SetVel(old_vel, thread);
    qInfo("SearchPosByADC timeout false");
    return false;
}

bool XtMotor::SearchPosByIO(double vel, double search_limit, bool search_rise, int io_id, double &result)
{
    if(is_debug)return true;
    if(!is_init)
        return false;
    if(!states.isEnabled())
        return false;
    int thread = default_using_thread;
    int timeout = 30000;
    double currPos = GetOutpuPos();
    double negative = min_range;
    double old_vel = GetMaxVel();
    qInfo("Tray Vel:%f",vel);
    SetVel(vel, thread);
    if(!(checkState()&&checkLimit(search_limit)&&checkInterface(search_limit)))return false;
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
    current_target = GetFeedbackPos();
    XT_Controler::WaitForAllInsFinish(0);
}

bool XtMotor::checkState(bool check_seeked_origin)
{
    if(!is_init)
    {
        qInfo(u8"%s轴未初始化",name.toStdString().c_str());
        AppendError(u8"轴未初始化");
        return false;
    }
    if(!states.isEnabled())
    {
        qInfo(u8"%s轴未使能",name.toStdString().c_str());
        AppendError(u8"轴未使能");
        return false;
    }
    if(check_seeked_origin&&(!states.seekedOrigin()))
    {
        qInfo(u8"%s轴未未回零",name.toStdString().c_str());
        AppendError(u8"轴未回零");
        return false;
    }
    return true;
}

bool XtMotor::checkLimit(const double pos)
{
    if(!qIsFinite(pos))
    {
        AppendError(QString(u8"%1目标位置 %2 为非法值").arg(name).arg(pos));
        qInfo("%s目标位置 %f 为非法值",name.toStdString().c_str(),pos);
        return false;
    }

    if(pos>max_range)
    {
        AppendError(QString(u8"%1 目标位置 %2 超过轴上限位 %3").arg(name).arg(pos).arg(max_range));
        qInfo(u8"%s 目标位置 %f 超过轴上限位 %f",name.toStdString().c_str(),pos,max_range);
        return false;
    }

    if(pos<min_range)
    {
        AppendError(QString(u8"%1 目标位置 %2 超过轴下限位 %3").arg(name).arg(pos).arg(min_range));
        qInfo(u8"%s 目标位置 %f 超过轴下限位 %f",name.toStdString().c_str(),pos,min_range);
        return false;
    }
    return true;
}

bool XtMotor::getInterfaceLimit(double target_pos,double& result_pos)
{
    if(parallel_limit_parameters.size()<=0)
        return false;
    result_pos = target_pos;
    for (int i = 0; i < parallel_limit_parameters.size(); ++i) {
        ParallelLimitParameter * temp_parameter = parallel_limit_parameters[i];
        //与检测区间有干涉
        double start_x = 0,end_x = 0,start_y = 0,end_y = 0;
        if(temp_parameter->effectMotorXName()!="")
        {
            DeviceStatesGeter::motorState motor_state = geter->getMotorState(temp_parameter->effectMotorXName());
            if(!motor_state.result)
                return false;
            start_x = motor_state.current_position;
            end_x = motor_state.target_position;
//            start_x = parallel_limit_motors[3*i+1]->GetFeedbackPos();
//            end_x = parallel_limit_motors[3*i+1]->GetCurrentTragetPos();
        }
        if(temp_parameter->effectMotorYName()!="")
        {
            DeviceStatesGeter::motorState motor_state = geter->getMotorState(temp_parameter->effectMotorYName());
            if(!motor_state.result)
                return false;
            start_y = motor_state.current_position;
            end_y = motor_state.target_position;
//            start_y = parallel_limit_motors[3*i+2]->GetFeedbackPos();
//            end_y = parallel_limit_motors[3*i+2]->GetCurrentTragetPos();
        }
        if(temp_parameter->hasInInterferenceSpance(start_x,end_x,start_y,end_y))
        {
            DeviceStatesGeter::motorState motor_state = geter->getMotorState(temp_parameter->motorName());
            if(!motor_state.result)
                return false;
            double temp_pos = temp_parameter->getSafePosition(target_pos,motor_state.current_position);
            if(fabs(temp_pos - target_pos)> fabs(result_pos - target_pos))
            {
                result_pos = temp_pos;
            }
        }
    }
    qInfo("getInterfaceLimit result_pos %f",result_pos);
    return true;
}

bool XtMotor::checkInterface(const double pos)
{
//    qInfo("%s CheckLimit %d,%d,%d",name.toStdString().c_str(),vertical_limit_parameters.size(),parallel_limit_parameters.size(),io_limit_parameters.size());
    double current_pos = GetFeedbackPos();
    for (int i = 0; i < vertical_limit_parameters.size(); ++i) {
        VerticalLimitParameter* temp_parameter = vertical_limit_parameters[i];
        //与检测区间有干涉
        if(temp_parameter->hasInterferenceWithMoveSpance(current_pos,pos))
        {
            //在限制区间
            DeviceStatesGeter::motorState motor_state = geter->getMotorState(temp_parameter->motorName());
            if(!motor_state.result)
                return false;
            if(!temp_parameter->checkInLimitSpance(motor_state.current_position,motor_state.target_position))
            {
                QString errorMessage = QString( u8"%1从%2到%3的过程可能会与%4相撞").arg(name).arg(current_pos).arg(pos).arg(temp_parameter->motorName());
                AppendError(errorMessage);
                qCritical(errorMessage.toStdString().c_str());
                return false;
            }
        }
    }
    for (int i = 0; i < parallel_limit_parameters.size(); ++i) {
        ParallelLimitParameter * temp_parameter = parallel_limit_parameters[i];
        //与检测区间有干涉
        double start_x = 0,end_x = 0,start_y = 0,end_y = 0;
        if(temp_parameter->effectMotorXName()!="")
        {
            DeviceStatesGeter::motorState motor_state = geter->getMotorState(temp_parameter->effectMotorXName());
            if(!motor_state.result)
                return false;
            start_x = motor_state.current_position;
            end_x = motor_state.target_position;
//            start_x = parallel_limit_motors[3*i+1]->GetFeedbackPos();
//            end_x = parallel_limit_motors[3*i+1]->GetCurrentTragetPos();
        }
        if(temp_parameter->effectMotorYName()!="")
        {
            DeviceStatesGeter::motorState motor_state = geter->getMotorState(temp_parameter->effectMotorYName());
            if(!motor_state.result)
                return false;
            start_y = motor_state.current_position;
            end_y = motor_state.target_position;
//            start_y = parallel_limit_motors[3*i+2]->GetFeedbackPos();
//            end_y = parallel_limit_motors[3*i+2]->GetCurrentTragetPos();
        }
        if(temp_parameter->hasInInterferenceSpance(start_x,end_x,start_y,end_y))
        {
            //检测在安全距离
            DeviceStatesGeter::motorState motor_state = geter->getMotorState(temp_parameter->motorName());
            if(!motor_state.result)
                return false;
            if(!temp_parameter->checkInSafeDistance(current_pos,pos,motor_state.current_position,motor_state.target_position))
            {
                AppendError(QString( u8"%1从%2到%3的过程与%4的安全距离不够").arg(name).arg(current_pos).arg(pos).arg(temp_parameter->motorName()));
                return false;
            }
        }
    }
    for (int i = 0; i < io_limit_parameters.size(); ++i) {
        IOLimitParameter* temp_parameter = io_limit_parameters[i];
        //与检测区间有干涉
        if(temp_parameter->hasInterferenceWithMoveSpance(current_pos,pos))
        {
            //在限制区间
            if(temp_parameter->crashSpance())
            {
                bool result = true,temp_result;
                QString temp_name = "",temp_io;
                for(int i = 0; i < temp_parameter->inputIOName().size(); ++i)
                {
                    temp_io =  temp_parameter->inputIOName()[i].toString();
                    DeviceStatesGeter::IoState io_state = geter->getInputIoState(temp_io);
                    if(!io_state.result)
                        return false;
                    temp_result = temp_parameter->checkInputInLimitSpance(i,io_state.current_state);
                    if(!temp_result)
                    {
                        temp_name.append(temp_io);
                        temp_name.append(" ");
                        result = false;
                    }
                }
                for (int i = 0; i < temp_parameter->outputIOName().size(); ++i)
                {
                    temp_io =  temp_parameter->outputIOName()[i].toString();
                    DeviceStatesGeter::IoState io_state = geter->getOutputIoState(temp_io);
                    if(!io_state.result)
                        return false;
                    temp_result = temp_parameter->checkOutputLimitSpance(i,io_state.current_state);
                    if(!temp_result)
                    {
                        temp_name.append(temp_io);
                        temp_name.append(" ");
                        result = false;
                    }
                }
                if(result)
                {
                    AppendError(QString( u8"%1从%2到%3的过程可能会与%4相撞").arg(name).arg(current_pos).arg(pos).arg(temp_name));
                    return false;
                }
            }
            else
            {
                QString temp_io;
                for(int i = 0; i < temp_parameter->inputIOName().size(); ++i)
                {
                    temp_io = temp_parameter->inputIOName()[i].toString();
                    DeviceStatesGeter::IoState io_state = geter->getInputIoState(temp_io);
                    if(!io_state.result)
                        return false;
                    if(!temp_parameter->checkInputInLimitSpance(i,io_state.current_state))
                    {
                        AppendError(QString( u8"%1从%2到%3的过程可能会与%4相撞").arg(name).arg(current_pos).arg(pos).arg(temp_parameter->inputIOName()[i].toString()));
                        return false;
                    }
                }
                for (int i = 0; i < temp_parameter->outputIOName().size(); ++i)
                {
                    temp_io =  temp_parameter->outputIOName()[i].toString();
                    DeviceStatesGeter::IoState io_state = geter->getOutputIoState(temp_io);
                    if(!io_state.result)
                        return false;
                    if(!temp_parameter->checkOutputLimitSpance(i,io_state.current_state))
                    {
                        AppendError(QString( u8"%1从%2到%3的过程可能会与%4相撞").arg(name).arg(current_pos).arg(pos).arg(temp_parameter->outputIOName()[i].toString()));
                        return false;
                    }
                }
            }
        }
    }
    return true;
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
