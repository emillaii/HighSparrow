//#include "xtmotion.h"
#include "XtGeneralInput.h"
#include "XT_MotionControler_Client_Lib.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include <QThread>
XtGeneralInput::XtGeneralInput(void)
{
    name = "";
    input_id = -1;
}

XtGeneralInput::operator bool()
{
    return Value();
}

void XtGeneralInput::Init(const QString &input_name)
{
    if(input_id>-1)
        return;
     this->name = input_name;

     //XtMotion::LoadProfile();
     input_id = XT_Controler_Extend::Profile_Find_IoIn_Name((LPWSTR)name.utf16());
//     if(input_id<0)
//     {
//         XtMotion::in_num_in_profile++;
//         input_id = XtMotion::in_num_in_profile;
//     }
    // XtMotion::AllInputs.append(this);
}

void XtGeneralInput::Init(int id_in_profile)
{
    if(input_id>-1)
        return;
     input_id = id_in_profile;
     //XtMotion::LoadProfile();
     LPWSTR res = XT_Controler_Extend::Profile_Get_IoIn_Name(input_id);
     name = QString::fromStdWString(res);
    // XtMotion::AllInputs.append(this);
}

int XtGeneralInput::ID()
{
    return input_id;
}

bool XtGeneralInput::Value()
{
    if(input_id<0)
        return false;
//    if(!XtMotion::IsInit())
//        return false;
    int state;
    int res = XT_Controler_Extend::Get_IoIn_State(input_id, state);
    if(res!=1)
        return false;
    return (state==1);
}

bool XtGeneralInput::Wait(bool value, int timeout)
{
    if(input_id<0)
        return false;
//    if(!XtMotion::IsInit())
//        return false;
//    if(input_id == -1)
        return false;
    while( timeout > 0 )
    {
        if (Value() == value)
        {
            return true;
        }
        timeout-=10;
        QThread::msleep(10);
    }
    return false;
}

QString XtGeneralInput::Name()
{
    return name;
}
