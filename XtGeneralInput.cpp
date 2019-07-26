#include "XtGeneralInput.h"
#include "XT_MotionControler_Client_Lib.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include <QThread>
int XtGeneralInput::count = 0;
XtGeneralInput::XtGeneralInput(void)
{
    name = "";
    input_id = -1;
}

XtGeneralInput::XtGeneralInput(QString name, int id)
{
    this->name = name;
    this->input_id = id;
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
     if(input_id<0)
     {
         count+=2;
         input_id = count;
     }

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
    int state;
    int res = XT_Controler_Extend::Get_IoIn_State(input_id, state);
    if(res!=1)
        return false;
    return (state==1);
}

bool XtGeneralInput::checkState(bool check_state)
{
    if(input_id<0)
        return false;
    int state;
    int res = XT_Controler_Extend::Get_IoIn_State(input_id, state);
    if(res!=1)
        return false;
    return (state == check_state);
}

bool XtGeneralInput::getValueToReg(int thread)
{
    if(input_id<0)
        return false;
    XT_Controler::USE_LOCAL_REG(thread);
    XT_Controler::GET_INPUT_IO(thread,input_id,input_id);
    return true;
}

bool XtGeneralInput::getRegState(int thread)
{
    if(input_id<0)
        return false;
    XT_Controler::WaitForAllInsFinish(thread);
    XT_Controler::USE_LOCAL_REG(thread);
    double reg_valur[1];
    XT_Controler::ReadControlerRegVal(thread,input_id,input_id,reg_valur);
    qInfo("getRegState %f",reg_valur[0]);
    return reg_valur[0] != 0.0;
}

bool XtGeneralInput::Wait(bool value, int timeout)
{
    if(input_id<0)
        return false;
    if(input_id == -1)
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
