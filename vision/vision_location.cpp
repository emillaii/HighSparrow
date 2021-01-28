#include "vision/vision_location.h"
#include <QThread>
#include <qelapsedtimer.h>
#include <utils/commonutils.h>
#define PI 3.1415926535898
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
    OpenLight();
    offset.ReSet();
    current_result.ReSet();
    PRResultStruct pr_result;
    QThread::msleep(parameters.waitImageDelay());
    ErrorCodeStruct temp;
    //ToDo: Add enum for prism PR
    if (parameters.prismPRType() == 1) {
        temp = vison->PR_Prism_Only_Matching(parameters.cameraName(), pr_result);
    } else if (parameters.prismPRType() == 2) {
        temp = vison->PR_Prism_SUT_Matching(parameters.cameraName(), pr_result);
    } else if (parameters.prismPRType() == 3) {
        temp = vison->PR_Prism_SUT_Two_Circle_Matching(parameters.cameraName(), pr_result);
    } else {
        SmallHoleDetectionParam paramStruct;
        paramStruct.detectSmallHole = parameters.enableSmallHoleDetection();
        paramStruct.smallHoleScanWidth = parameters.smallCircleScanWidth();
        paramStruct.smallHoleScanCount = parameters.smallCircleScanCount();
        paramStruct.smallHoleEdgeResponse = parameters.smallCircleEdgeResponse();
        paramStruct.smallHoleRadiusMax = parameters.smallCircleRadiusMax();
        paramStruct.smallHoleRadiusMin = parameters.smallCircleRadiusMin();
        temp = vison->PR_Generic_NCC_Template_Matching(parameters.cameraName(),
                                                       parameters.prFileName(),
                                                       pr_result,
                                                       parameters.objectScore(),
                                                       parameters.retryCount(),
                                                       &paramStruct);
    }
    last_image_name = pr_result.rawImageName;
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
            current_result = offset = temp_offset;
            qInfo("mech: %f %f %f %f %f", temp_offset.X, temp_offset.Y, temp_offset.Theta,temp_offset.O_X,temp_offset.O_Y);
            if (parameters.closeLightAfterPR())
                CloseLight();
            return true;
        } else {
            qInfo("CalcMechDistance Fail");
        }
    }
    else {
        qWarning("perform pr fail: %s %s ",parameters.cameraName().toStdString().c_str(), parameters.prFileName().toStdString().c_str());
    }
    AppendError(QString(u8"Perform PR Fail "));
    AppendError(temp.errorMessage);
    return false;
}

bool VisionLocation::performPR()
{
    QElapsedTimer timer; timer.start();
    OpenLight();
    current_result.ReSet();
    PrOffset offset;
    qInfo("PerformPR: %s with wait delay: %f", parameters.locationName().toStdString().c_str(), parameters.waitImageDelay());
    QThread::msleep(parameters.waitImageDelay());
    ErrorCodeStruct temp;
    if (parameters.prismPRType() == 1) {
        temp =  vison->PR_Prism_Only_Matching(parameters.cameraName(), current_pixel_result);
    } else if (parameters.prismPRType() == 2) {
        temp = vison->PR_Prism_SUT_Matching(parameters.cameraName(), current_pixel_result);
    } else if (parameters.prismPRType() == 3) {
        temp = vison->PR_Prism_SUT_Two_Circle_Matching(parameters.cameraName(), current_pixel_result);
    } else {
        SmallHoleDetectionParam paramStruct;
        paramStruct.detectSmallHole = parameters.enableSmallHoleDetection();
        paramStruct.smallHoleScanWidth = parameters.smallCircleScanWidth();
        paramStruct.smallHoleScanCount = parameters.smallCircleScanCount();
        paramStruct.smallHoleEdgeResponse = parameters.smallCircleEdgeResponse();
        paramStruct.smallHoleRadiusMax = parameters.smallCircleRadiusMax();
        paramStruct.smallHoleRadiusMin = parameters.smallCircleRadiusMin();
        temp =  vison->PR_Generic_NCC_Template_Matching(parameters.cameraName(),
                                                        parameters.prFileName(),
                                                        current_pixel_result,
                                                        parameters.objectScore(),
                                                        parameters.retryCount(),
                                                        &paramStruct);
    }
    last_image_name = current_pixel_result.rawImageName;
    if(ErrorCode::OK == temp.code)
    {
        QPointF mech;
        QPointF mech_o;
        PrOffset temp_offset;
        qInfo("Perform PR %s Success. PR_Result: %f %f %f %f %f",parameters.locationName().toStdString().c_str(), current_pixel_result.x, current_pixel_result.y, current_pixel_result.theta,current_pixel_result.ori_x,current_pixel_result.ori_y);
        if(mapping->CalcMechDistance(QPointF(current_pixel_result.x,current_pixel_result.y),mech)&&mapping->CalcMechDistance(QPointF(current_pixel_result.ori_x,current_pixel_result.ori_y),mech_o))
        {
            temp_offset.X = mech.x();
            temp_offset.Y = mech.y();
            temp_offset.O_X = mech_o.x();
            temp_offset.O_Y = mech_o.y();
            if(abs(temp_offset.O_X)>parameters.maximumLength()||abs(temp_offset.O_Y)>parameters.maximumLength()||abs(temp_offset.X)>parameters.maximumOffset()||abs(temp_offset.Y)>parameters.maximumOffset())
            {
                qWarning("pr result too big: %f %f %f %f %f", temp_offset.X, temp_offset.Y, temp_offset.Theta,temp_offset.O_X,temp_offset.O_Y);
                AppendError(QString(u8" pr result too big"));
                return false;
            }
            if(abs(current_pixel_result.theta) < parameters.maximunAngle())
                temp_offset.Theta = current_pixel_result.theta;
            else if(abs(current_pixel_result.theta - 90) < parameters.maximunAngle())
                temp_offset.Theta = current_pixel_result.theta - 90;
            else if(abs(current_pixel_result.theta - 180) < parameters.maximunAngle())
                temp_offset.Theta = current_pixel_result.theta - 180;
            else if(abs(current_pixel_result.theta - 270) < parameters.maximunAngle())
                temp_offset.Theta = current_pixel_result.theta -270;
            else if(abs(current_pixel_result.theta - 360) < parameters.maximunAngle())
                temp_offset.Theta = current_pixel_result.theta -360;
            else
            {
                qWarning("pr result too big: %f %f %f %f %f", temp_offset.X, temp_offset.Y, temp_offset.Theta,temp_offset.O_X,temp_offset.O_Y);
                AppendError(QString(u8"theta result too big"));
                return false;
            }
            current_result = offset = temp_offset;
            qInfo("mech: %f %f %f %f %f", temp_offset.X, temp_offset.Y, temp_offset.Theta,temp_offset.O_X,temp_offset.O_Y);
            if (parameters.closeLightAfterPR())
                CloseLight();
            parameters.setPerformTime(timer.elapsed());
            qInfo("PerformTime %d",parameters.performTime());
            return true;
        } else {
            qInfo("CalcMechDistance Fail");
        }
    }
    else {
        qWarning("perform pr fail: %s %s ",parameters.cameraName().toStdString().c_str(), parameters.prFileName().toStdString().c_str());
    }
    AppendError(QString(u8"Perform PR (%1) Fail ").arg(parameters.cameraName()));
    AppendError(temp.errorMessage);
    return false;
}

bool VisionLocation::performNoMaterialPR()
{
    current_result.ReSet();
    return true;
//    current_result.ReSet();
//    PrOffset offset;
//    if(parameters.waitImageDelay()>0)
//        QThread::msleep(parameters.waitImageDelay());
//    QString imageName;
//    imageName.append(getVisionLogDir())
//            .append(getCurrentTimeString())
//            .append(".jpg");
//    bool result = saveImage(imageName);
//    if(parameters.performTime()>0)
//        QThread::msleep(parameters.performTime());
//    return result;
}

bool VisionLocation::performPR(PRResultStruct &pr_result)
{
    OpenLight();
    QThread::msleep(parameters.waitImageDelay());
    ErrorCodeStruct temp;
    if (parameters.prismPRType() == 1) {
        temp = vison->PR_Prism_Only_Matching(parameters.cameraName(), pr_result);
    } else if (parameters.prismPRType() == 2)
    {
        temp = vison->PR_Prism_SUT_Matching(parameters.cameraName(), pr_result);
    } else if (parameters.prismPRType() == 3) {
        temp = vison->PR_Prism_SUT_Two_Circle_Matching(parameters.cameraName(), pr_result);
    } else {
        SmallHoleDetectionParam paramStruct;
        paramStruct.detectSmallHole = parameters.enableSmallHoleDetection();
        paramStruct.smallHoleScanWidth = parameters.smallCircleScanWidth();
        paramStruct.smallHoleScanCount = parameters.smallCircleScanCount();
        paramStruct.smallHoleEdgeResponse = parameters.smallCircleEdgeResponse();
        paramStruct.smallHoleRadiusMax = parameters.smallCircleRadiusMax();
        paramStruct.smallHoleRadiusMin = parameters.smallCircleRadiusMin();
        temp = vison->PR_Generic_NCC_Template_Matching(parameters.cameraName(),
                                                       parameters.prFileName(),
                                                       pr_result,
                                                       parameters.objectScore(),
                                                       parameters.retryCount(),
                                                       &paramStruct);
    }
    last_image_name = pr_result.rawImageName;
    qInfo("CameraName: %s prFilename: %s PR_Result: %f %f %f",parameters.cameraName().toStdString().c_str(), parameters.prFileName().toStdString().c_str(),
          pr_result.x, pr_result.y, pr_result.theta);
    if (parameters.closeLightAfterPR())
        CloseLight();
    return  ErrorCode::OK == temp.code;
}

bool VisionLocation::performGlueInspection(QString beforeDispenseImageName, QString afterDispenseImageName,  QString *glueInspectionImageName,
                                           double min_glue_width, double max_glue_width, double max_avg_glue_width,
                                           double &outMinGlueWidth, double &outMaxGlueWidth, double &outMaxAvgGlueWidth)
{
    double resolution = this->mapping->getXResolution();
    ErrorCodeStruct temp = vison->Glue_Inspection(resolution, min_glue_width, max_glue_width, max_avg_glue_width,
                                                  beforeDispenseImageName, afterDispenseImageName, glueInspectionImageName,
                                                  &outMinGlueWidth, &outMaxGlueWidth, &outMaxAvgGlueWidth);
    if (temp.code == ErrorCode::OK) return true;
    else return false;
}

void VisionLocation::resetResult()
{
    current_result.ReSet();
    current_pixel_result = PRResultStruct();
}

PrOffset VisionLocation::getCurrentResult()
{
    PrOffset temp_offset;
    if(parameters.useOrigin())
    {
        temp_offset.X = current_result.O_X;
        temp_offset.Y = current_result.O_Y;
    }
    else
    {
        temp_offset.X = current_result.X;
        temp_offset.Y = current_result.Y;
    }
    temp_offset.Theta = current_result.Theta;
    return temp_offset;
}

void VisionLocation::setCurrentResult(PrOffset pr_offset)
{
    if(parameters.useOrigin())
    {
        current_result.O_X = pr_offset.X;
        current_result.O_Y = pr_offset.Y;
    }
    else
    {
        current_result.X = pr_offset.X;
        current_result.Y = pr_offset.Y;
    }
    current_result.Theta = pr_offset.Theta;
}

PrOffset VisionLocation::getCurrentResult(bool use_origin)
{
    PrOffset temp_offset;
    //if(this->parameters.useOrigin())
    if(use_origin)
    {
        temp_offset.X = current_result.O_X;
        temp_offset.Y = current_result.O_Y;
    }
    else
    {
        temp_offset.X = current_result.X;
        temp_offset.Y = current_result.Y;
    }
    temp_offset.Theta = current_result.Theta;
    return temp_offset;
}

QPointF VisionLocation::getCurrentResultOffset()
{
    QPointF offset;
    double x = current_result.X - current_result.O_X;
    double y = current_result.Y - current_result.O_Y;
    double temp_theta = -current_result.Theta*PI/180;
    double result_x = x*cos(temp_theta) + y*sin(temp_theta);
    double result_y = y*cos(temp_theta) - x*sin(temp_theta);
    offset.setX(round(result_x*1000)/1000);
    offset.setY(round(result_y*1000)/1000);
    return offset;
}

QPointF VisionLocation::getOffsetResult(QPoint offset)
{
//    QPointF offset;
    double x = current_result.X - current_result.O_X;
    double y = current_result.Y - current_result.O_Y;
    double temp_theta = current_result.Theta*PI/180;
    offset.setX(x*cos(temp_theta) + y*sin(temp_theta));
    offset.setY(y*cos(temp_theta) - x*sin(temp_theta));
    return offset;
}

PRResultStruct VisionLocation::getCurrentPixelResult()
{
    return current_pixel_result;
}

QPointF VisionLocation::getCurrentOffset()
{
    return QPointF(current_result.X,current_result.Y);
}

void VisionLocation::OpenLight()
{
    QElapsedTimer timer; timer.start();
    lighting->setBrightness(parameters.lightChannel(),parameters.lightBrightness());
    // if downlook location, open aux light channel too, for glue inspection
    if (parameters.auxLightChannel() >= 0)
    {
        lighting->setBrightness(parameters.auxLightChannel(),parameters.auxLightBrightness());
    }
    qWarning("[Timelog] %s %d ", __FUNCTION__, timer.elapsed());
//    QThread::msleep(30);
}

void VisionLocation::CloseLight()
{
    lighting->setBrightness(parameters.lightChannel(),0);
    // if downlook location, close aux light channel too, for glue inspection
    if (parameters.auxLightChannel() >= 0)
    {
        lighting->setBrightness(parameters.auxLightChannel(),0);
    }
    //    QThread::msleep(30);
}

void VisionLocation::OpenLight(int channel, uint8_t brightness)
{
    lighting->setBrightness(channel, brightness);
}

void VisionLocation::CloseLight(int channel)
{
    lighting->setBrightness(channel, 0);
}

QString VisionLocation::getLastImageName()
{
    return last_image_name;
}

bool VisionLocation::saveImage(QString imageName)
{
    QThread::msleep(parameters.waitImageDelay());
    return vison->saveImage(parameters.cameraName(), imageName);
}
