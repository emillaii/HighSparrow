#include "XtGeneralOutput.h"
//#include "xtmotion.h"
#include "XT_MotionControler_Client_Lib.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
int XtGeneralOutput::count = 0;
XtGeneralOutput::XtGeneralOutput()
{
    output_id = -1;
    is_hp = false;
    name = "";
}

XtGeneralOutput::XtGeneralOutput(QString name, int id)
{
    this->name = name;
    this->output_id = id;
}

void XtGeneralOutput::Init(const QString &output_name)
{
    if(output_id>-1)
        return;
     this->name = output_name;
     //XtMotion::LoadProfile();
     output_id = XT_Controler_Extend::Profile_Find_IoOut_Name((LPWSTR)name.utf16());
     //XtMotion::AllOutputs.append(this);
}

void XtGeneralOutput::Init(int id_in_profile)
{
    if(output_id>-1)
        return;
    output_id = id_in_profile;
    //XtMotion::LoadProfile();
    LPWSTR res = XT_Controler_Extend::Profile_Get_IoOut_Name(output_id);
    name = QString::fromStdWString(res);
    //XtMotion::AllOutputs.append(this);
}

XtGeneralOutput::operator int()
{
    return output_id;
}

XtGeneralOutput &XtGeneralOutput::operator =(const bool &b)
{
    Set(b);
    return *this;
}

int XtGeneralOutput::GetID()
{
    return output_id;
}

bool XtGeneralOutput::Set(bool value, int thread)
{
    if(is_debug)return true;
    if(output_id<0)
        return false;
    if(thread<0)
    {
        XT_Controler_Extend::DigitOut_PreciousTrig_SetCurIoOutput(output_id,value?1:0);
    }
    else
    {
        XT_Controler_Extend::DIGITOUT_PRECIOUTTRIG_SET_OUTPUT_IO(thread,output_id,0,value?1:0);
    }
}

void XtGeneralOutput::SetOnTllTimeSpan(int time_span, int thread)
{
    if(is_debug)return;
    if(output_id<0)
        return;
    XT_Controler_Extend::DIGITOUT_PRECIOUTTRIG_SET_OUTPUT_IO(thread,output_id,0,1);
    XT_Controler::TILLTIME(thread,time_span);
    XT_Controler_Extend::DIGITOUT_PRECIOUTTRIG_SET_OUTPUT_IO(thread,output_id,0,0);
}

bool XtGeneralOutput::SET(bool value, int thread)
{
    if(is_debug)return true;
    if(output_id<0)
        return false;
    XT_Controler::SET_OUTPUT_IO(thread,output_id,value?1:0);
    return true;
}
void XtGeneralOutput::SetOnTillTimeSpan(int time_span, int thread)
{
    if(is_debug)return;
    if(output_id<0)
        return;
    XT_Controler::SET_OUTPUT_IO(thread,output_id,1);
    XT_Controler::TILLTIME(thread,time_span);
    XT_Controler::SET_OUTPUT_IO(thread,output_id,0);
}

bool XtGeneralOutput::SetStateAndTillTimeSpan(byte state, int time_span, int thread)
{
    if(is_debug)return true;
    if(output_id<0)
        return false;
    XT_Controler::SET_OUTPUT_IO(thread,output_id,state);
    XT_Controler::TILLTIME(thread,time_span);
    return true;
}
void XtGeneralOutput::WaitForFinish(int thread)
{
    XT_Controler::WaitForAllInsFinish(thread);
}

bool XtGeneralOutput::Value()
{
    if(output_id<0)
        return false;
    int state;
    int res = XT_Controler_Extend::Get_IoOut_State(output_id, state);
    if(res!=1)
        return false;
    return (state==1);
}

bool XtGeneralOutput::checkState(bool check_state)
{
    if(output_id<0)
        return false;
    int state;
    int res = XT_Controler_Extend::Get_IoOut_State(output_id, state);
    if(res!=1)
        return false;
    return (state == check_state);
}

QString XtGeneralOutput::Name()
{
    return name;
}

XtGeneralOutput::operator bool()
{
    return Value();
}


