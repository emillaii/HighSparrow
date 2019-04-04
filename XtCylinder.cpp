#include "XtCylinder.h"
#include "xtmotion.h"

#include <QThread>

XtCylinder::XtCylinder(XtGeneralOutput *output_io, XtGeneralInput *input_fold_io, XtGeneralInput *input_unfold_io, QString name)
{
    out = output_io;
    in_fold = input_fold_io;
    in_unfold = input_unfold_io;
    this->name = std::move(name);
}

bool XtCylinder::Set(bool new_state, bool wait_done, int timeout,int input_null_delay)
{
    if(!XtMotion::IsInit())
        return false;
    out->Set(new_state);
    if(!wait_done)
        return true;
    int count = timeout;
    bool state_fold , state_unfold;
    if(new_state)
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
            if((in_fold == nullptr&&state_fold)||(in_unfold == nullptr&&state_unfold))
                QThread::msleep(input_null_delay);
            return true;
        }
        count-=10;
        QThread::msleep(10);
    }
    qInfo("%s current value:%d , target value:%d , %s current value:%d , target value:%d",
          in_fold == nullptr?"none":in_fold->Name().toStdString().c_str(),
          in_fold == nullptr?0:in_fold->Value(),
          state_fold,
          in_unfold==nullptr?"none":in_unfold->Name().toStdString().c_str(),
          in_unfold==nullptr?0:in_unfold->Value(),
          state_unfold);
    return false;
}

void XtCylinder::SET(int thread, bool new_state)
{
    out->Set(new_state, thread);
}

bool XtCylinder::Wait(bool target_state, int timeout,int input_null_delay)
{
    int count = timeout;
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
            if((in_fold == nullptr&&state_fold)||(in_unfold == nullptr&&state_unfold))
                QThread::msleep(input_null_delay);
            return true;
        }
        count-=10;
        QThread::msleep(10);
    }
    qInfo("%scurrentvalue:%dtargetvalue:%d %scurrentvalue:%dtargetvalue:%d", in_fold->Name().toStdString().c_str(), in_fold->Value(),state_fold,in_unfold->Name().toStdString().c_str(),in_unfold->Value(),state_unfold);
    return false;
}

bool XtCylinder::Value()
{
    return out->Value();
}

bool XtCylinder::CheckState(bool target_state)
{
    if(target_state)
    {
        return out->Value()&&(in_fold == nullptr||in_fold->Value());
    }
    else
    {
        return (!out->Value())&&(in_fold == nullptr||in_unfold->Value());
    }
}

