#include "lut_module.h"

LutModule::LutModule(QString name, XtMotor* motor_x,XtMotor* motor_y,XtVcMotor* motor_z,BaslerPylonCamera* camera,WordopLight* lighting,VisionModule* vision,XtVacuum* load_vacuum,XtVacuum* unload_vacuun)
{
    this->carrier = new MaterialCarrier(name,motor_x,motor_y,motor_z,load_vacuum);
    this->camera = camera;
    this->lighting =lighting;
    this->vision = vision;
    this->load_vacuum = load_vacuum;
    this->unload_vacuum = unload_vacuun;
}

bool LutModule::MoveToPR(PRResultStruct &pr_result)
{
    lighting->ChangeBrightnessSignal(LIGHTING_UPLOOK,parameters.Lighting());
    bool result = carrier->Move_Vision_Sync();
    if(result)
        vision->PR_Generic_NCC_Template_Matching(DOWNLOOK_VISION_CAMERA,pr_result);
    return result;
}

bool LutModule::MoveToLoadPos()
{
    return  carrier->Move_SZ_SX_XY_Z_Sync(parameters.LoadX(),parameters.LoadY(),parameters.LoadZ());
}

bool LutModule::MoveToPick1Lens()
{
    bool result = grabber->Set(false);
    if(result)
        result = carrier->Move_SZ_SX_XY_Z_Sync(parameters.Pick1X(),parameters.Pick1Y(),parameters.Pick1Z());
    if(result)
    {
        //todo one fuction
        double reuslt_pos;
        result = carrier->ZSerchByForce(reuslt_pos,parameters.PickForce(),-1,0,load_vacuum);
        if(result)
            grabber->Set(false);
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::MoveToUnPick1Lens()
{
    bool result = carrier->Move_SZ_SX_XY_Z_Sync(parameters.UnPick1X(),parameters.UnPick1Y(),parameters.UnPick1Z());
    if(result)
    {
        //todo one fuction
        double reuslt_pos;
        result = carrier->ZSerchByForce(reuslt_pos,parameters.PickForce(),-1,1,unload_vacuum);
        if(result)
            grabber->Set(true);
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::MoveToPick2Lens()
{
    bool result = grabber->Set(false);
    if(result)
        result = carrier->Move_SZ_SX_XY_Z_Sync(parameters.Pick2X(),parameters.Pick2Y(),parameters.Pick2Z());
    if(result)
    {
        double reuslt_pos;
        result = carrier->ZSerchByForce(reuslt_pos,parameters.PickForce(),-1,0,load_vacuum);
        if(result)
            grabber->Set(false);
        result &= carrier->ZSerchReturn();
    }
    return result;
}

bool LutModule::MoveToUnPick2Lens()
{
    bool result = carrier->Move_SZ_SX_XY_Z_Sync(parameters.UnPick2X(),parameters.UnPick2Y(),parameters.UnPick2Z());
    if(result)
    {
        double reuslt_pos;
        result = carrier->ZSerchByForce(reuslt_pos,parameters.PickForce(),-1,1,unload_vacuum);
        if(result)
            grabber->Set(true);
        result &= carrier->ZSerchReturn();
    }
    return result;
}
