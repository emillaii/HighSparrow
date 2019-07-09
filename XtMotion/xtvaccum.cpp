#include "xtvaccum.h"

#include <utility>
#include "xtmotion.h"
#include "XtVacuum.h"

XtVacuum::XtVacuum(XtGeneralOutput *output_io, XtGeneralInput *input_io,XtGeneralOutput *oupout_break_io, QString name)
{
    in_io = input_io;
    out_io = output_io;
    break_io = oupout_break_io;
    this->name = std::move(name);
}

bool XtVacuum::Set(bool new_state, bool wait_done,int finish_delay, int timeout,int input_null_delay)
{
    if(!XtMotion::IsInit())
        return false;
    out_io->Set(new_state);
//    if(nullptr != break_io&&(!new_state))
//        break_io->Set(true);
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
//            if(finish_delay>0)
//                QThread::msleep(finish_delay);
            return true;
        }
        count-=10;
        QThread::msleep(10);
    }
    return false;
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
        int time = 1000;
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

void XtVacuum::Jet(int wait_time,int close_time)
{
    double delay = close_time;
    if(break_io == nullptr)
        return;
    QThread::msleep(wait_time);
    break_io->Set(true);
    static int using_xt_thread = -1;
    if(using_xt_thread<0)
        using_xt_thread = XtMotion::GetThreadResource();
//    break_io->Set(1,using_xt_thread);
    XT_Controler::TILLTIME(using_xt_thread,close_time);
    break_io->Set(0,using_xt_thread);
}

void XtVacuum::UnJet()
{
    break_io->Set(0);
}

void XtVacuum::WaitUnJet()
{
    if(break_io == nullptr)
        return;
    int time = 0;
    while(break_io->Value())
    {
            time++;
         QThread::msleep(10);
    }
    qInfo("wait time:%d",10*time);
}
