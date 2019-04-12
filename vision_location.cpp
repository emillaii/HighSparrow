#include "vision_location.h"

VisionLocation::VisionLocation()
{

}

void VisionLocation::Init(VisionModule *vison,Pixel2Mech* mapping, WordopLight *lighting)
{
    this->vison = vison;
    this->mapping = mapping;
    this->lighting = lighting;
    loadParam();
}

void VisionLocation::loadParam()
{
    PropertyBase::loadJsonConfig(PR_PARAMETER_FILE_PATH,PR_AA1_TOOL_UPLOOK,&parameters);
}

void VisionLocation::saveParam()
{
    PropertyBase::saveJsonConfig(PR_PARAMETER_FILE_PATH,PR_AA1_TOOL_UPLOOK,&parameters);
}

bool VisionLocation::performPR(PrOffset &offset)
{
    PRResultStruct pr_result;
    ErrorCodeStruct temp =  vison->PR_Generic_NCC_Template_Matching(parameters.cameraName(), parameters.prFileName(), pr_result);
    if(ErrorCode::OK == temp.code)
    {
        QPointF mech;
        if(mapping->CalcMechDistance(QPointF(pr_result.x,pr_result.y),mech))
        {
           offset.X = mech.x();
           offset.Y = mech.y();
           offset.Theta = pr_result.theta;
           return true;
        }
    }
    return false;
}

bool VisionLocation::performPR(PRResultStruct &pr_result)
{
    ErrorCodeStruct temp =  vison->PR_Generic_NCC_Template_Matching(parameters.cameraName(), parameters.prFileName(), pr_result);
    return  ErrorCode::OK == temp.code;
}

void VisionLocation::OpenLight()
{
    lighting->SetBrightness(parameters.lightChannel(),parameters.lightBrightness());
}

void VisionLocation::CloseLight()
{
    lighting->SetBrightness(parameters.lightChannel(),0);
}
