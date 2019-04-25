#include <qthread.h>
#include <utility>
//#include "xtmotion.h"
#include "XtVacuum.h"

XtVacuum::XtVacuum():ErrorBase ()
{
}

void XtVacuum::Init(XtGeneralOutput *output_io, XtGeneralInput *input_io, XtGeneralOutput *output_break_io)
{
    in_io = input_io;
    out_io = output_io;
    break_io = output_break_io;
    setName(parameters.vacuumName());
}

bool XtVacuum::Set(bool new_state, bool wait_done,int finish_delay, int timeout,int input_null_delay)
{
    out_io->Set(new_state);
    if((0 != finish_delay)&&(nullptr != break_io)&&(!new_state))
        break_io->Set(true);
    if(!wait_done)
        return true;
    return Wait(new_state,timeout,finish_delay,input_null_delay);
}

void XtVacuum::SET(int thread, bool new_state)
{
    out_io->Set(new_state, thread);
}

bool XtVacuum::Wait(bool target_state, int timeout,int finish_delay,int input_null_delay)
{
    int count = timeout;
    while(count>0)
    {
        if (in_io == nullptr||in_io->Value() == target_state)
        {
            if(nullptr == in_io)
                QThread::msleep(input_null_delay);
            else if(finish_delay>0)
                QThread::msleep(finish_delay);
            if((0 != finish_delay)&&(nullptr != break_io)&&(!target_state))
                break_io->Set(false);
            return true;
        }
        count-=10;
        QThread::msleep(10);
    }
    return false;
    if((0 != finish_delay)&&(nullptr != break_io)&&(!target_state))
        break_io->Set(false);
}

bool XtVacuum::IsVacuum()
{
    if(out_io->Value())
    {
        return in_io->Value();
    }
    else
    {
        out_io->Set(true);
        int time = 200;
        bool result = in_io->Value();
        while(time > 0)
        {
            result = in_io->Value();
            if(result)
                break;
            QThread::msleep(10);
            time-=10;
        }
//        qInfo(u8"checed time : %d",200 - time);
        out_io->Set(false);
        return result;
    }
}
