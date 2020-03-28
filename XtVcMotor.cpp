#include "XtVcMotor.h"
#include "C_SoftLanding_dll.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include "XT_MotionControler_Client_Lib.h"
#include <QElapsedTimer>

int XtVcMotor::vcm_count = 0;
QVector<VCM_Resource_struct> XtVcMotor::all_parameter;
QMutex XtVcMotor::g_mutex;
QMutex XtVcMotor::setSoftLanding_mutex;
XtVcMotor::XtVcMotor()
{
    axis_id = -1;
    name = "";
    is_init = false;
    direction_is_opposite = false;
    vcm_id = vcm_count + 1;
    vcm_count += 1;
}

void XtVcMotor::ConfigVCM()
{
    if (parameters.direction() != 0)
        direction_is_opposite = true;
    SetRunDirect(vcm_id, parameters.direction(), parameters.scale());
    SetPosModeSpeed(vcm_id, max_vel);
    SetPosModeAcc(vcm_id, max_acc);
    SetPosModejerk(vcm_id, max_jerk);
    SetPosLimit(vcm_id, max_range, min_range);
    qInfo("ConfigVCM::vcm_id:%d,max_range:%f min_range:%f", vcm_id, max_range, min_range);

    SetSoftlandingSlot(vcm_id, GetCurveResource());
    double current[4]{ 0.2, 0.5, 0.8, 1.2 };
    double force[4]{ 10, 330, 660, 1000 };
    if (parameters.needMapForce())
    {
        MapCurrent2Force(vcm_id, current, force, 4);
    }
    is_init = true;
    states.setIsEnabled(true);
    error_code = get_motor_error(vcm_id);
    qInfo("%s init error code %d", name.toStdString().c_str(), error_code);
}

void XtVcMotor::ChangeDiretion(bool befor_seek)
{
    int direction = parameters.direntionAfterSeek();
    if (befor_seek)
        direction = parameters.direction();
    if (direction != 0)
        direction_is_opposite = true;
    else
        direction_is_opposite = false;
    qInfo("VCM %d:dirention:%d", vcm_id, direction);
    SetRunDirect(vcm_id, direction, parameters.scale());
}

void XtVcMotor::Init()
{
    name = parameters.motorName();
    setName(name);
    origin.Init(name + "_O");
    origin2.Init(name + "_O2");
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
    VCMT_resource_alloc(all_parameter.data(), all_parameter.length());
    Soft_landing_dll_init(1);

    int timeout = 30000;
    while (timeout > 0)
    {
        int res = Get_Init_Ready();
        if (res == 1)
        {
            SetDebugLog(0);
            return;
        }
        if (res == -1)
        {
            qInfo("VCM on CAN ID is NOT exist!");
            return;
        }
        timeout -= 10;
        QThread::msleep(10);
    }
    qInfo("VCM Soft_landing_dll_init failed!");
}

void XtVcMotor::showSettingDialog()
{
    ShowSettingDlg();
}

void XtVcMotor::Init(const QString &motor_name) {}

void XtVcMotor::SetADC(int can_id, int data_ch) {}

void XtVcMotor::SetEncoderFeedback(int can_id, int data_ch, double ratio) {}

void XtVcMotor::SetFeedbackZero(double new_value)
{
    if (!is_init)
        return;
}

void XtVcMotor::Enable()
{
    if (!is_init)
        return;
    states.setIsEnabled(true);
    SetServoOnOff(vcm_id, states.isEnabled());
}

void XtVcMotor::Disable()
{
    if (!is_init)
        return;
    states.setIsEnabled(false);
    SetServoOnOff(vcm_id, states.isEnabled());
    states.setSeekedOrigin(false);
}

double XtVcMotor::GetOutpuPos() const
{
    if (!is_init)
        return 0;
    {
        double val;
        int res = XT_Controler_Extend::Get_Cur_Axis_Pos(axis_id, val);
        if (res != 0)
        {
            // qInfo("vcm now output pos is %f",val);
            return val;
        }
    }
    return 0;
}

double XtVcMotor::GetFeedbackPos(int decimal_digit) const
{
    if (!is_init)
        return 0;

    {
        //        return GetOutpuPos();
        double val;
        int times = 10;
        while (times > 0)
        {
            int res = 0;
            bool lockres = g_mutex.tryLock(10);
            if (lockres)
            {
                res = GetNowPos(vcm_id, val);
                g_mutex.unlock();
            }
            else
            {
                qInfo("%s VCM GetNowPos tryLock failed!", this->Name().toStdString().c_str());
            }
            if (res == 1)
            {
                //                qInfo("%s VCM GetNowPos success! %d", this->Name().toStdString().c_str(), times);
                break;
            }
            QThread::msleep(20);
            // qInfo("%s VCM GetNowPos failed! %d", this->Name().toStdString().c_str(), times);
            times--;
            // qInfo("%s VCM GetNowPos failed CP1! %d", this->Name().toStdString().c_str(), times);
        }
        if (times == 0)
        {
            qInfo("%s GetFeedbackPos timed out", this->Name().toStdString().c_str());
            return GetOutpuPos();
        }
        if (direction_is_opposite)
            val = -val;
        // qInfo("%s GetFeedbackPos val :%f",this->Name().toStdString().c_str(), val);
        return round(val * pow(10, decimal_digit)) / pow(10, decimal_digit);
    }
    return 0;
}

double XtVcMotor::GetCurADC() const
{
    if (adc != nullptr)
        return adc->GetValue();
    return 0;
}

bool XtVcMotor::IsRunning() const
{
    if (!is_init)
        return false;
    int be_run;
    XT_Controler_Extend::Get_Cur_Axis_State(axis_id, be_run);
    return be_run == 1;
}

bool XtVcMotor::getAlarmState()
{
    // if(error_code == get_motor_error(vcm_id))
    if (0 != get_motor_error(vcm_id))    // Return 0 if success
    {
        if (states.seekedOrigin())
            states.setSeekedOrigin(false);
        return true;
    }
    return false;
}

bool XtVcMotor::clearAlarmState()
{
    //return ResetDevice(vcm_id);
    return true;
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
    if (!is_init)
        return;
    SetPosModeSpeed(vcm_id, vel);
}

void XtVcMotor::SetAcc(double vel, int thread)
{
    if (!is_init)
        return;
    SetPosModeAcc(vcm_id, vel);
}

void XtVcMotor::SetJerk(double jerk, int thread)
{
    if (!is_init)
        return;
    if (thread == -1)
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
    if (!checkState(false))
        return false;
    int result;
    ChangeDiretion(true);
    if (parameters.findOriginCurrent() > 0)
        result = Touch_Go_Zero(vcm_id, parameters.findOriginCurrent(), parameters.touchDistance());
    else
    {

        SetZeroPos(vcm_id, 0);
        result = Init_Go_Zero(vcm_id);
    }
    if (result == 1)
    {
        states.setSeekedOrigin(true);
        qInfo("axis %s seek origin succees!", name.toStdString().c_str());
    }
    else
    {
        states.setSeekedOrigin(false);
        AppendError(QString(u8"%1回零失败").arg(name));
        qFatal("axis %s seek origin fail!", name.toStdString().c_str());
    }
    ChangeDiretion();
    return states.seekedOrigin();
}

bool XtVcMotor::WaitSeekDone(int thread, int timeout)
{
    if (!checkState(false))
        return false;
    qInfo("WaitSeekDone %d", states.seekedOrigin());
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

bool XtVcMotor::SearchPosByForce(
    const double speed, const double force, const double limit, const double margin, const int timeout)
{
    if (is_debug)
        return true;
    if (!(checkState() && checkLimit(limit) && checkInterface(limit)))
        return false;
    double start_pos = GetOutpuPos();
    qInfo("start_pos: %f,force:%f,search_limit:%f speed:%f margin:%f", start_pos, force, limit, speed, margin);
    SetSoftLanding(speed, max_acc, force, start_pos, limit, margin);
    bool res;
    res = DoSoftLanding();
    res &= WaitSoftLandingDone(timeout);
    if (res)
        qInfo("sooftlanding pos:%f", GetFeedbackPos());
    else
        AppendError(QString(u8"搜索位置失败 开始位置 %1 力 %2 目标位置 %3 速度 %4 到位区间 %5")
                        .arg(start_pos)
                        .arg(force)
                        .arg(limit)
                        .arg(speed)
                        .arg(margin));
    return res;
}

bool XtVcMotor::SearchPosByForce(const double speed, const double force, const int timeout)
{
    qInfo("start SearchPosByForce");
    if (is_debug)
        return true;
    if (!(checkState() && checkLimit(max_range) && checkInterface(max_range)))
        return false;
    double start_pos = GetOutpuPos();
    double limit = start_pos + (max_range - start_pos) / 2;
    double margin = fabs(max_range - start_pos) / 2.01;
    SetSoftLanding(speed, max_acc, force, start_pos, limit, margin);
    bool res;
    qInfo("Finish SetSoftLanding");
    res = DoSoftLanding();

    qInfo("Finish DoSoftLanding");
    res &= WaitSoftLandingDone(timeout);

    qInfo("Finish WaitSoftLandingDone");
    if (res)
        qInfo("sooftlanding pos:%f", GetFeedbackPos());
    else
        AppendError(QString(u8"搜索位置失败 开始位置 %1 力 %2 目标位置 %3 速度 %4 到位区间 %5")
                        .arg(start_pos)
                        .arg(force)
                        .arg(limit)
                        .arg(speed)
                        .arg(margin));
    return res;
}

void XtVcMotor::RestoreForce()
{
    if (!is_init)
        return;
    qInfo("RestoreForce");
    SetCurrentLimit(vcm_id, 10, -10);
}

bool XtVcMotor::MoveToPos(double pos, int thread)
{
    while (true)
    {
        if (XtMotor::MoveToPos(pos, thread))
        {
            return true;
        }
        else
        {
            auto rsp = SI::ui.getUIResponse(
                "MotionError", QObject::tr("%1 move to pos %2 failed! %3").arg(name).arg(pos).arg(getErrorMsg()),
                MsgBoxIcon::Error, SI::ui.retryIgnoreButtons);
            if (rsp == SI::ui.Retry)
            {
                continue;
            }
            else if (rsp == SI::ui.Ignore)
            {
                return false;
            }
            else
            {
                qFatal("Unknown UI response: %s", rsp.toUtf8().data());
            }
        }
    }
}

bool XtVcMotor::WaitArrivedTargetPos(double target_position, int timeout)
{
    while (true)
    {
        if (XtMotor::WaitArrivedTargetPos(target_position, timeout))
        {
            return true;
        }
        else
        {
            auto rsp = SI::ui.getUIResponse("MotionError",
                                            QObject::tr("%1 wait arriving target pos %2 failed! %3")
                                                .arg(name)
                                                .arg(target_position)
                                                .arg(getErrorMsg()),
                                            MsgBoxIcon::Error, SI::ui.retryIgnoreButtons);
            if (rsp == SI::ui.Retry)
            {
                continue;
            }
            else if (rsp == SI::ui.Ignore)
            {
                return false;
            }
            else
            {
                qFatal("Unknown UI response: %s", rsp.toUtf8().data());
            }
        }
    }
}

bool XtVcMotor::WaitArrivedTargetPos(double target_position, double arived_error, int timeout)
{
    while (true)
    {
        if (XtMotor::WaitArrivedTargetPos(target_position, arived_error, timeout))
        {
            return true;
        }
        else
        {
            auto rsp = SI::ui.getUIResponse("MotionError",
                                            QObject::tr("%1 wait arriving target pos %2 with pos error %3 failed! %4")
                                                .arg(name)
                                                .arg(target_position)
                                                .arg(arived_error)
                                                .arg(getErrorMsg()),
                                            MsgBoxIcon::Error, SI::ui.retryIgnoreButtons);
            if (rsp == SI::ui.Retry)
            {
                continue;
            }
            else if (rsp == SI::ui.Ignore)
            {
                return false;
            }
            else
            {
                qFatal("Unknown UI response: %s", rsp.toUtf8().data());
            }
        }
    }
}

bool XtVcMotor::WaitArrivedTargetPos(int timeout)
{
    return XtMotor::WaitArrivedTargetPos(timeout);
}

QString XtVcMotor::getErrorMsg()
{
    auto errorCode = get_motor_error(vcm_id);
    switch (errorCode)
    {
        case 0:
            return QString();
        case 1:
            return "HardwareError: error_over_current";
        case 1 << 1:
            return "HardwareError: error_go_zero";
        case 1 << 2:
            return "HardwareError: error_over_temp";
        case 1 << 3:
            return "HardwareError: error_leakage_current";
        case -1:
            return "HardwareError: ControllerError";
        default:
            return QString("HardwareError: Unkown error: %1").arg(errorCode);
    }
}

void XtVcMotor::ShowSetting()
{
    if (!is_init)
        return;
    ShowSettingDlg();
}

void XtVcMotor::SetSoftLanding(
    double slow_speed, double slow_acc, double force, double start_pos, double target_pos, double margin)
{
    if (!is_init)
        return;
    qInfo("slow_speed:%f start_pos: %f,force:%f,target_pos:%f,margin:%f", slow_speed, start_pos, force, target_pos,
          margin);
    QMutexLocker locker(&setSoftLanding_mutex);
    SetFastSpeed(vcm_id, max_vel);
    SetFastAcc(vcm_id, max_acc);
    SetSlowSpeed(vcm_id, slow_speed);
    SetSlowAcc(vcm_id, slow_acc);
    SetForce(vcm_id, force);
    SetSoftPos(vcm_id, target_pos);
    SetSoftLandingMargin(vcm_id, margin);
    SetZeroPos(vcm_id, start_pos);
    UpdataParam(vcm_id);
}

bool XtVcMotor::DoSoftLanding()
{
    if (!is_init)
        return false;
    is_softlanding = true;
    int res = Do_SoftDown(vcm_id);
    if (res == 0)
        return true;
    AppendError(QString(u8"%1 软着陆伸出失败 错误码 %2").arg(name).arg(res));
    qInfo("VCM %d DoSoftLanding Failed! code: %d", vcm_id, res);
    return false;
}

bool XtVcMotor::DoSoftLandingReturn()
{
    qInfo("VCM %s DoSoftLandingReturn start", name.toStdString().c_str());
    if (is_debug)
        return true;
    if (!is_init)
        return false;
    is_returning = true;
    int res = Do_SoftUp(vcm_id);
    if (res == 0)
        return true;
    AppendError(QString(u8"%1 软着陆返回失败 错误码 %2").arg(name).arg(res));
    return false;
}

bool XtVcMotor::resetSoftLanding(int timeout)
{
    bool ret = true;
    if (is_softlanding | is_returning)
        if (!WaitSoftLandingDone(timeout))
            ret = false;
    if (is_softlanded)
    {
        ret = DoSoftLandingReturn();
        ret &= WaitSoftLandingDone(timeout);
    }
    if (!ret)
    {
        RestoreForce();
        AppendError(QString(u8"%1 软着陆复位失败").arg(name));
    }
    return ret;
}

bool XtVcMotor::WaitSoftLandingDone(int timeout)
{
    qInfo("WaitSoftLandingDone Start");
    if (is_debug)
        return true;
    if (!checkState(false))
        return false;
    int out_time = timeout;
    while (out_time > 0)
    {
        int res = CheckPosReady(vcm_id);
        if (res == 1)
        {
            is_softlanded = is_softlanding;
            is_softlanding = false;
            is_returning = false;
            qInfo("%s WaitSoftLandingDone %d", name.toStdString().c_str(), timeout - out_time);
            return true;
        }
        out_time -= 10;
        QThread::msleep(10);
    }

    qInfo("WaitSoftLandingDone While loop finish");
    is_softlanded = is_softlanding;
    is_softlanding = false;
    is_returning = false;

    // qInfo("get_motor_error Start");
    LONG64 temp_code = get_motor_error(vcm_id);

    // qInfo("get_motor_error Finish");
    if (error_code == temp_code)
    {
        qInfo("CheckPosReady fail but action success outtime %d", timeout);
        return true;
    }
    AppendError(
        QString(u8"%1 等待软着陆伸出/软着陆返回完成超时 错误码 %2 - %3").arg(name).arg(temp_code).arg(error_code));
    error_code = temp_code;
    //    qInfo("WaitSoftLandingDone fail");
    return false;
}
