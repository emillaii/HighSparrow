#include "vision_location.h"
#include <QThread>
VisionLocation::VisionLocation():ErrorBase ()
{
}

void VisionLocation::Init(VisionModule *vison,Pixel2Mech* mapping, WordopLight *lighting)
{
    this->vison = vison;
    this->mapping = mapping;
    this->lighting = lighting;
    setName(parameters.locationName());
}

void VisionLocation::loadParam()
{
    PropertyBase::loadJsonConfig(PR_PARAMETER_FILE_PATH,PR_AA1_TOOL_UPLOOK,&parameters);
}

void VisionLocation::saveParam()
{
    PropertyBase::saveJsonConfig(PR_PARAMETER_FILE_PATH,PR_AA1_TOOL_UPLOOK,&parameters);
}

bool VisionLocation::performPR(PrOffset &offset, bool need_conversion)
{
//    offset.X = 0; offset.Y = 0; offset.Theta = 0;
//    return true;
    offset.ReSet();
    current_offset.ReSet();
    PRResultStruct pr_result;
    ErrorCodeStruct temp =  vison->PR_Generic_NCC_Template_Matching(parameters.cameraName(), parameters.prFileName(), pr_result);
    if(ErrorCode::OK == temp.code)
    {
        QPointF mech;
        PrOffset temp_offset;
        qInfo("Perform PR Success. PR_Result: %f %f %f", pr_result.x, pr_result.y, pr_result.theta);
        if (!need_conversion) {
            offset.X = pr_result.x;
            offset.Y = pr_result.y;
            offset.Theta = pr_result.theta;
            offset.W = pr_result.width;
            offset.H = pr_result.height;
            return true;
        }
        if(mapping->CalcMechDistance(QPointF(pr_result.x,pr_result.y),mech))
        {
           temp_offset.X = mech.x();
           temp_offset.Y = mech.y();
            if(abs(temp_offset.X)>parameters.maximumLength()||abs(temp_offset.Y)>parameters.maximumLength())
            {
                qInfo("pr result too big: %f %f %f", temp_offset.X, temp_offset.Y, temp_offset.Theta);
                return false;
            }
           if(abs(pr_result.theta) < parameters.maximunAngle())
               temp_offset.Theta = pr_result.theta;
           else if(abs(pr_result.theta - 90) < parameters.maximunAngle())
               temp_offset.Theta = pr_result.theta - 90;
           else if(abs(pr_result.theta - 180) < parameters.maximunAngle())
               temp_offset.Theta = pr_result.theta - 180;
           else if(abs(pr_result.theta - 270) < parameters.maximunAngle())
               temp_offset.Theta = pr_result.theta -270;
           else if(abs(pr_result.theta - 360) < parameters.maximunAngle())
               temp_offset.Theta = pr_result.theta -360;
           else
           {
               qInfo("theta result too big: %f %f %f", temp_offset.X, temp_offset.Y, temp_offset.Theta);
               return false;
           }
            current_offset = offset = temp_offset;
           qInfo("mech: %f %f %f", temp_offset.X, temp_offset.Y, temp_offset.Theta);
           return true;
        } else {
            qInfo("CalcMechDistance Fail");
        }
    }
    else {
        qInfo("perform pr fail: %s %s ",parameters.cameraName().toStdString().c_str(), parameters.prFileName().toStdString().c_str());
    }
    return false;
}

bool VisionLocation::performPR()
{
    current_offset.ReSet();
    PrOffset offset;
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
           current_offset = offset;
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
    qInfo("CameraName: %s prFilename: %s PR_Result: %f %f %f",parameters.cameraName().toStdString().c_str(), parameters.prFileName().toStdString().c_str(),
          pr_result.x, pr_result.y, pr_result.theta);
//    qInfo("camera %s perform PR result:%d name:%s",parameters.cameraName().toStdString().c_str(),temp.code,parameters.prFileName().toStdString().c_str());
    return  ErrorCode::OK == temp.code;
}

QPointF VisionLocation::getCurrentOffset()
{
    return QPointF(current_offset.X,current_offset.Y);
}

void VisionLocation::OpenLight()
{
    lighting->setBrightness(parameters.lightChannel(),parameters.lightBrightness());
    QThread::msleep(30);
}

void VisionLocation::CloseLight()
{
    lighting->setBrightness(parameters.lightChannel(),0);
    QThread::msleep(30);
}
