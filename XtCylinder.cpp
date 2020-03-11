#include "XtCylinder.h"
//#include "xtmotion.h"

#include <QThread>
#include <QDebug>
XtCylinder::XtCylinder():ErrorBase (){}

void XtCylinder::Init(XtGeneralOutput *output_io,
                      XtGeneralInput *input_fold_io,
                      XtGeneralInput *input_unfold_io,
                      XtGeneralOutput *output_io_zero)
{
    out = output_io;
    parts.append(out);
    out_zero = output_io_zero;
    parts.append(out_zero);
    in_fold = input_fold_io;
    parts.append(in_fold);
    in_unfold = input_unfold_io;
    parts.append(in_unfold);
    setName(parameters.cylinderName());
}
bool XtCylinder::Set(bool new_state, bool wait_done)
{
    if(out_zero != nullptr)
    {
        if(new_state)
        {
            if(out->Value())
                return true;
            out_zero->Set(false);
            QThread::msleep(10);
            out->Set(true);
        }
        else
        {
            if(out_zero->Value())
                return true;
            out->Set(false);
            QThread::msleep(10);
            out_zero->Set(true);
        }
    }
    else{
        if(out->Value() == new_state)
            return true;
        out->Set(new_state);
    }
    if(!wait_done)
        return true;
    return Wait(new_state);

}

void XtCylinder::SET(int thread, bool new_state)
{
    out->Set(new_state, thread);
}

bool XtCylinder::Wait(bool target_state)
{
    if(is_debug)return true;
    int count = parameters.outTime();
    bool state_fold , state_unfold;
    if(target_state)
    {
        state_fold = true;
        state_unfold = false;
    }
    else
    {
        state_fold = false;
        state_unfold = true;
    }
    while(count>0)
    {
        if ((in_fold == nullptr||in_fold->Value() == state_fold)&&(in_unfold == nullptr||in_unfold->Value() == state_unfold))
        {
            int temp_time = parameters.outTime() - count;
            if(target_state)
            {
                if(parameters.finishOneDelay()>0)
                    QThread::msleep(parameters.finishOneDelay());
            }
            else
            {
                if(parameters.finishZeroDelay()>0)
                    QThread::msleep(parameters.finishZeroDelay());
            }
            return true;
        }
        count-=10;
        QThread::msleep(10);
    }
    AppendError(QString(u8"等待气缸%1到位（状态%2）超时,超时时间%3").arg(parameters.cylinderName()).arg(target_state).arg(parameters.outTime()));
    qInfo("%s current value:%d , target value:%d , %s current value:%d , target value:%d",
          in_fold == nullptr?"none":in_fold->Name().toStdString().c_str(),
          in_fold == nullptr?0:in_fold->Value(),
          state_fold,
          in_unfold==nullptr?"none":in_unfold->Name().toStdString().c_str(),
          in_unfold==nullptr?0:in_unfold->Value(),
          state_unfold);
    return false;
}

bool XtCylinder::Value()
{
    return out->Value();
}

bool XtCylinder::checkState(bool target_state)
{
    if(target_state)
    {
        return out->Value()&&(in_fold == nullptr||in_fold->Value())&&(in_unfold == nullptr||(!in_unfold->Value()));
    }
    else
    {
        if(out_zero == nullptr)
            return out->checkState(false)&&(in_unfold == nullptr||in_unfold->Value())&&(in_fold == nullptr||(!in_fold->Value()));
        else
            return out_zero->Value()&&(in_unfold == nullptr||in_unfold->Value())&&(in_fold == nullptr||(!in_fold->Value()));
    }
}

bool XtCylinder::SetValue(bool value)
{
    if (out->Set(value))
        return true;
    else
        return false;
}

