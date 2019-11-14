#include <qthread.h>
#include <utility>
//#include "xtmotion.h"
#include "XtVacuum.h"
bool XtVacuum::no_material = false;
XtVacuum::XtVacuum():ErrorBase ()
{
}

void XtVacuum::Init(XtGeneralOutput *output_io, XtGeneralInput *input_io, XtGeneralOutput *output_break_io)
{
    in_io = input_io;
    parts.append(in_io);
    out_io = output_io;
    parts.append(out_io);
    break_io = output_break_io;
    parts.append(break_io);
    setName(parameters.vacuumName());
}

bool XtVacuum::Set(bool new_state, bool wait_done, bool open_break)
{
    qInfo("%s set value %d",parameters.vacuumName().toStdString().c_str(),new_state);
    if(no_material)
    {
        QThread::msleep(parameters.excuteTime());
        return true;
    }
    out_io->Set(parameters.reserveValue()?(!new_state):new_state);
    if(open_break&&(nullptr != break_io)&&(!new_state))
        break_io->Set(true);
    if(!wait_done)
        return true;
    return Wait(new_state);
}

bool XtVacuum::SetSimulation(bool new_state,bool open_io)
{
    bool result = true;
    if(open_io)
        result = out_io->Set(parameters.reserveValue()?(!new_state):new_state);
    QThread::msleep(parameters.excuteTime());
    return result;
}

void XtVacuum::SET(int thread, bool new_state)
{
    out_io->Set(parameters.reserveValue()?(!new_state):new_state, thread);
}

bool XtVacuum::Wait(bool target_state)
{
    if(is_debug)return true;
    int count = parameters.outTime();
    while(count>0)
    {
        if (in_io == nullptr||in_io->Value() == target_state)
        {
            int temp_time = parameters.outTime() - count;
            if(parameters.finishDelay()>0)
                QThread::msleep(parameters.finishDelay());
            if((nullptr != break_io)&&(!target_state))
                break_io->Set(false);
            qDebug("%s wait time %d",parameters.vacuumName().toStdString().c_str(),temp_time);
            return true;
        }
        count-=10;
        QThread::msleep(10);
    }
    if((nullptr != break_io)&&(!target_state))
        break_io->Set(false);
    AppendError(QString(u8"%1等待%2状态超时，超时时间%3").arg(parameters.vacuumName()).arg(target_state).arg(parameters.outTime()));
    qInfo(u8"%s等待%d状态超时,超时时间%d.",parameters.vacuumName().toStdString().c_str(),target_state,parameters.outTime());
    return false;
}

bool XtVacuum::checkHasMaterielSync()
{
    if(no_material)
        return true;
    bool enable_value = parameters.reserveValue()?false:true;
    if(out_io->Value() == enable_value)
    {
        return in_io->Value();
    }
    else
    {
        out_io->Set(enable_value);
        int time = parameters.checkOutTime();
        bool result = in_io->Value();
        while(time > 0)
        {
            result = in_io->Value();
            if(result)
                break;
            QThread::msleep(10);
            time-=10;
        }
        out_io->Set(!enable_value);
        return result;
    }
}

bool XtVacuum::getHasMateriel(int thread)
{
    return in_io->getRegState(thread);
}

bool XtVacuum::checkHasMateriel(int thread)
{
    bool enable_value = parameters.reserveValue()?false:true;
    bool result = true;
    bool origin_value = out_io->Value();
    if(origin_value != enable_value)
        result &= out_io->SetStateAndTillTimeSpan(enable_value,parameters.checkOutTime(),thread);
    result &= in_io->getValueToReg(thread);
    if(origin_value != enable_value)
        result &= out_io->SET(origin_value,thread);
    if(!result)
    {
        qInfo("%s checkHasMateriel fail",parameters.vacuumName().toStdString().c_str());
    }
    return result;
}
bool XtVacuum::checkState(bool check_state)
{
    return out_io->checkState(check_state)&&in_io->checkState(check_state);
}
