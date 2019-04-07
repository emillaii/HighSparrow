#include "XtGeneralOutput.h"
//#include "xtmotion.h"
#include "XT_MotionControler_Client_Lib.h"
#include "XT_MotionControlerExtend_Client_Lib.h"

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

void XtGeneralOutput::Set(bool value, int thread)
{
    if(output_id<0)
        return;
//    if(!XtMotion::IsInit())
//        return;
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
    if(output_id<0)
        return;
//    if(!XtMotion::IsInit())
//        return;
    XT_Controler_Extend::DIGITOUT_PRECIOUTTRIG_SET_OUTPUT_IO(thread,output_id,0,1);
    XT_Controler::TILLTIME(thread,time_span);
    XT_Controler_Extend::DIGITOUT_PRECIOUTTRIG_SET_OUTPUT_IO(thread,output_id,0,1);
}

bool XtGeneralOutput::Value()
{
    if(output_id<0)
        return false;
//    if(!XtMotion::IsInit())
//        return false;
    int state;
    int res = XT_Controler_Extend::Get_IoOut_State(output_id, state);
    if(res!=1)
        return false;
    return (state==1);
}

QString XtGeneralOutput::Name()
{
    return name;
}

XtGeneralOutput::operator bool()
{
    return Value();
}


