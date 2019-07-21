#include "vision/vision_location.h"
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
    offset.ReSet();
    current_offset.ReSet();
    PRResultStruct pr_result;
    QThread::msleep(parameters.waitImageDelay());
    ErrorCodeStruct temp =  vison->PR_Generic_NCC_Template_Matching(parameters.cameraName(), parameters.prFileName(), pr_result, parameters.objectScore());
    last_image_name = pr_result.imageName;
    if(ErrorCode::OK == temp.code)
    {
        QPointF mech;
        QPointF mech_o;
        PrOffset temp_offset;
        qInfo("Perform PR Success. PR_Result: %f %f %f %f %f", pr_result.x, pr_result.y, pr_result.theta,pr_result.ori_x,pr_result.ori_y);
        if (!need_conversion) {
            offset.X = pr_result.x;
            offset.Y = pr_result.y;
            offset.Theta = pr_result.theta;
            offset.W = pr_result.width;
            offset.H = pr_result.height;
            return true;
        }
        if(mapping->CalcMechDistance(QPointF(pr_result.x,pr_result.y),mech)&&mapping->CalcMechDistance(QPointF(pr_result.ori_x,pr_result.ori_y),mech_o))
        {
           temp_offset.X = mech_o.x();
           temp_offset.Y = mech_o.y();
           temp_offset.O_X = mech.x() - mech_o.x();
           temp_offset.O_Y = mech.y() - mech_o.y();
            if(abs(temp_offset.X)>parameters.maximumLength()||abs(temp_offset.Y)>parameters.maximumLength()||abs(temp_offset.O_X)>parameters.maximumOffset()||abs(temp_offset.O_Y)>parameters.maximumOffset())
            {
                qInfo("pr result too big: %f %f %f %f %f", temp_offset.X, temp_offset.Y, temp_offset.Theta,temp_offset.O_X,temp_offset.O_Y);
                AppendError(QString(u8" pr result too big"));
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
               qInfo("pr result too big: %f %f %f %f %f", temp_offset.X, temp_offset.Y, temp_offset.Theta,temp_offset.O_X,temp_offset.O_Y);
               AppendError(QString(u8"theta result too big"));
               return false;
           }
            current_offset = offset = temp_offset;
           qInfo("mech: %f %f %f %f %f", temp_offset.X, temp_offset.Y, temp_offset.Theta,temp_offset.O_X,temp_offset.O_Y);
           return true;
        } else {
            qInfo("CalcMechDistance Fail");
        }
    }
    else {
        qInfo("perform pr fail: %s %s ",parameters.cameraName().toStdString().c_str(), parameters.prFileName().toStdString().c_str());
    }
    AppendError(QString(u8"Perform PR Fail"));
    return false;
}

bool VisionLocation::performPR()
{
    current_offset.ReSet();
    PrOffset offset;
    PRResultStruct pr_result;
    QThread::msleep(parameters.waitImageDelay());
    ErrorCodeStruct temp =  vison->PR_Generic_NCC_Template_Matching(parameters.cameraName(), parameters.prFileName(), pr_result, parameters.objectScore());
    last_image_name = pr_result.imageName;
    if(ErrorCode::OK == temp.code)
    {
        QPointF mech;
        qInfo("Perform PR Success. PR_Result: %f %f %f", pr_result.x, pr_result.y, pr_result.theta);
        if(mapping->CalcMechDistance(QPointF(pr_result.x,pr_result.y),mech))
        {
           offset.X = mech.x();
           offset.Y = mech.y();
            if(fabs(offset.X)>parameters.maximumLength()||fabs(offset.Y)>parameters.maximumLength())
            {
                AppendError(QString(u8" pr result too big"));
                qInfo("pr result too big: %f %f %f", offset.X, offset.Y, offset.Theta);
                return false;
            }
           if(fabs(pr_result.theta) < parameters.maximunAngle())
               offset.Theta = pr_result.theta;
           else if(fabs(pr_result.theta - 90) < parameters.maximunAngle())
               offset.Theta = pr_result.theta - 90;
           else if(fabs(pr_result.theta - 180) < parameters.maximunAngle())
               offset.Theta = pr_result.theta - 180;
           else if(fabs(pr_result.theta - 270) < parameters.maximunAngle())
               offset.Theta = pr_result.theta -270;
           else if(fabs(pr_result.theta - 360) < parameters.maximunAngle())
               offset.Theta = pr_result.theta -360;
           else
           {
               AppendError(QString(u8"theta result too big"));
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
    AppendError(QString(u8"Perform PR Fail"));
    return false;
}

bool VisionLocation::performPR(PRResultStruct &pr_result)
{
    QThread::msleep(parameters.waitImageDelay());
    ErrorCodeStruct temp =  vison->PR_Generic_NCC_Template_Matching(parameters.cameraName(), parameters.prFileName(), pr_result, parameters.objectScore());
    last_image_name = pr_result.imageName;
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
//    QThread::msleep(30);
}

void VisionLocation::CloseLight()
{
    lighting->setBrightness(parameters.lightChannel(),0);
//    QThread::msleep(30);
}

QString VisionLocation::getLastImageName()
{
    return last_image_name;
}

bool VisionLocation::saveImage(QString imageName)
{
    return vison->saveImage(parameters.cameraName(), imageName);
}
