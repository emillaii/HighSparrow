#include "vision_location.h"

VisionLocation::VisionLocation()
{

}

void VisionLocation::Init(VisionModule *vison,Pixel2Mech* mapping, WordopLight *lighting)
{
    this->vison = vison;
    this->mapping = mapping;
    this->lighting = lighting;
//    loadParam();
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
        qInfo("Perform PR Success. PR_Result: %f %f %f", pr_result.x, pr_result.y, pr_result.theta);
        if(mapping->CalcMechDistance(QPointF(pr_result.x,pr_result.y),mech))
        {
           offset.X = mech.x();
           offset.Y = mech.y();
            if(abs(offset.X)>parameters.maximumLength()||abs(offset.Y)>parameters.maximumLength())
            {
                qInfo("pr result too big: %f %f %f", offset.X, offset.Y, offset.Theta);
                return false;
            }
           if(abs(pr_result.theta) < parameters.maximunAngle())
               offset.Theta = pr_result.theta;
           else if(abs(pr_result.theta - 90) < parameters.maximunAngle())
               offset.Theta = pr_result.theta - 90;
           else if(abs(pr_result.theta - 180) < parameters.maximunAngle())
               offset.Theta = pr_result.theta - 180;
           else if(abs(pr_result.theta - 270) < parameters.maximunAngle())
               offset.Theta = pr_result.theta -270;
           else if(abs(pr_result.theta - 360) < parameters.maximunAngle())
               offset.Theta = pr_result.theta -360;
           else
           {
               qInfo("theta result too big: %f %f %f", offset.X, offset.Y, offset.Theta);
               return false;
           }

           qInfo("mech: %f %f %f", offset.X, offset.Y, offset.Theta);
           return true;
        } else {
            qInfo("CalcMechDistance Fail");
        }
    }
    return false;
}

bool VisionLocation::performPR(PRResultStruct &pr_result)
{
    ErrorCodeStruct temp =  vison->PR_Generic_NCC_Template_Matching(parameters.cameraName(), parameters.prFileName(), pr_result);
    qInfo("PR_Result: %f %f %f", pr_result.x, pr_result.y, pr_result.theta);
//    qInfo("camera %s perform PR result:%d name:%s",parameters.cameraName().toStdString().c_str(),temp.code,parameters.prFileName().toStdString().c_str());
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
