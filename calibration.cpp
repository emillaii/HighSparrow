#include "calibration.h"
#include <QMessageBox>
#include <visionavadaptor.h>
#include <QFile>
#include <QTextStream>

Calibration::Calibration(QString name,QString file_name,XtMotor *motor_x, XtMotor *motor_y, WordopLight *light_controller, int ch, int lighting, QString camera_name, QString pr_name, QObject *parent)
{
    this->name = name;
    this->file_name =file_name;
    this->motor_x = motor_x;
    this->motor_y = motor_y;
    this->light_controller =light_controller;
    this->ch = ch;
    this->lighting = lighting;
    this->camera_name = camera_name;
    this->pr_name = pr_name;
}

void Calibration::ChangeParameter(WordopLight *light_controller, int ch, int lighting, QString camera_name, QString pr_name)
{
    this->light_controller =light_controller;
    this->ch = ch;
    this->lighting = lighting;
    this->camera_name = camera_name;
    this->pr_name = pr_name;
}

void Calibration::loadJsonConfig()
{
    if(PropertyBase::loadJsonConfig(file_name,name, &parameters))
        mapping.ChangeParameter(QMatrix(parameters.matrix11(),
                                        parameters.matrix12(),
                                        parameters.matrix21(),
                                        parameters.matrix22(),
                                        parameters.deltaX(),
                                        parameters.deltaY()),
                                QPointF(parameters.imageWidth()/2,parameters.imageHeight()/2));
}
void Calibration::saveJsonConfig()
{
    PropertyBase::saveJsonConfig(file_name,name, &parameters);
}
bool Calibration::performCalibration()
{
    if(nullptr == motor_x||nullptr == motor_y)
    {
        AppendLineError(u8"轴为空");
        return false;
    }
    double xMove = 1;
    double yMove = 1;

    double pixel_x,pixel_y;
    QVector<QPointF> pixelPoints;
    QVector<QPointF> motorPoints;

    light_controller->SetBrightness(ch,lighting);

    for (int i = 1; i<=4; i++)
    {
        if (i == 1)
        {
            if(!motor_x->StepMoveSync(xMove, true))
            {
                AppendLineError(u8"轴走位失败");
                return false;
            }
        }else if (i == 2)
        {
            if(!motor_x->StepMoveSync(xMove*2, false))
            {
                AppendLineError(u8"轴走位失败");
                return false;
            }
        } else if (i == 3)
        {
            if(!motor_x->StepMoveSync(xMove, true))
            {
                AppendLineError(u8"轴走位失败");
                return false;
            }
            if(!motor_y->StepMoveSync(yMove, true))
            {
                AppendLineError(u8"轴走位失败");
                return false;
            }
        } else if (i == 4)
        {
            if(!motor_y->StepMoveSync(yMove*2, false))
            {
                AppendLineError(u8"轴走位失败");
                return false;
            }
        }
        Sleep(1000);
        if (GetPixelPoint(pixel_x,pixel_y))
        {
            qInfo((name + " mech x: %f y: %f").toStdString().data(), motor_x->GetFeedbackPos(), motor_y->GetFeedbackPos());
            pixelPoints.append(QPointF(pixel_x,pixel_y));
            motorPoints.append(QPointF(motor_x->GetFeedbackPos() , motor_y->GetFeedbackPos()));
        } else
        {
            qInfo(("error in performing " + name +" PR").toStdString().data());
            return false;
        }
    }
    if(coordinateA2BMapping(pixelPoints,motorPoints))
    {
        parameters.setOriginX(mOriginB.x());
        parameters.setOriginX(mOriginB.y());
        parameters.setMatrix11(mA2BMatrix.m11());
        parameters.setMatrix12(mA2BMatrix.m12());
        parameters.setMatrix21(mA2BMatrix.m21());
        parameters.setMatrix22(mA2BMatrix.m22());
        parameters.setDeltaX(mA2BMatrix.dx());
        parameters.setDeltaY(mA2BMatrix.dy());
        mapping.ChangeParameter(QMatrix(parameters.matrix11(),
                                   parameters.matrix12(),
                                   parameters.matrix21(),
                                   parameters.matrix22(),
                                   parameters.deltaX(),
                                   parameters.deltaY()),
                                   QPointF(parameters.imageWidth()/2,parameters.imageHeight()/2));

        saveJsonConfig();
    }
    return true;
}

double Calibration::getRotationAngle()
{
    return  mRotationB;
}

bool Calibration::getDeltaDistanceFromCenter(const QPointF pixelPoint, QPointF &distanceMech)
{

    if(mapping.hasCalibration())
    {
        mapping.CalcMechDistance(pixelPoint);
        qInfo("%s mech Result (%f,%f)",name.toStdString().c_str(),distanceMech.x(),distanceMech.y());
        return true;
    }
    return false;
}

bool Calibration::getMechPoint(QPointF pixelPoint, QPointF &mechPoint)
{
    if(mapping.hasCalibration())
    {
        mechPoint = mapping.pixel2MechPoint(pixelPoint);
        return true;
    }
    return false;
}

bool Calibration::getCaliMapping(Pixel2Mech &caliMapping)
{
    if(mapping.hasCalibration())
    {
        caliMapping.ChangeParameter(QMatrix(parameters.matrix11(),
                                           parameters.matrix12(),
                                           parameters.matrix21(),
                                           parameters.matrix22(),
                                           parameters.deltaX(),
                                           parameters.deltaY()),
                                   QPointF(parameters.imageWidth()/2,parameters.imageHeight()/2));
        return true;
    }
    else
        return false;
}

bool Calibration::coordinateA2BMapping(const QVector<QPointF> &APoints, const QVector<QPointF> &BPoints)
{
    if(APoints.length() <3 || BPoints.length()<3)
    {
        qInfo((name + " fail: num of callibration points is less than 3!").toStdString().data());
        return false;
    }
    QPointF image_center = QPointF(parameters.imageWidth()/2,parameters.imageHeight()/2);
    mA2BMatrix = mA2BMapping.DoCalibration(APoints.data(),BPoints.data(),image_center);
    if(APoints.length()>3 && BPoints.length()>3)
    {
        QPointF A2BPoint=mA2BMapping.pixel2MechPoint(APoints[3]);
        QPointF diff=A2BPoint-BPoints[3];
        if(diff.x()*diff.x()+diff.y()*diff.y()>0.01)
        {
            qInfo((name + " Fail!").toStdString().data());
        }
        else
        {
            qInfo((name + " Success!").toStdString().data());
        }

        qInfo("valid B Point: x: %f, y: %f",BPoints[3].x(),BPoints[3].y());
        qInfo("A2B Point: x: %f, y: %f", A2BPoint.x(),A2BPoint.y());
        qInfo("diff_x: %f, diff_y: %f",diff.x(),diff.y());
        QPointF A2BPoint0=mA2BMapping.pixel2MechPoint(QPoint(0,1));
        qInfo("(0,1)A2BPoint0.x %f,A2BPoint0.y: %f",A2BPoint0.x(),A2BPoint0.y());
        QPointF A2BPoint1=mA2BMapping.pixel2MechPoint(QPoint(1,0));
        qInfo("(1,0)A2BPoint1.x %f,A2BPoint1.y: %f",A2BPoint1.x(),A2BPoint1.y());
    }
    else
    {
        qInfo(("There is no valid point in " + name).toStdString().data());
    }
    mIsMapping = true;
    mOriginB = mA2BMapping.pixel2MechPoint(image_center);
    mRotationB = caculateRotationAngle();
    qInfo((name + " center: x: %f, y: %f rad: %f").toStdString().data(),mOriginB.x(),mOriginB.y(),mRotationB);
    return true;
}

double Calibration::caculateRotationAngle()
{
    return  qAcos(mA2BMatrix.m11()/qSqrt(mA2BMatrix.m11()*mA2BMatrix.m11()+mA2BMatrix.m12()*mA2BMatrix.m12()));
}

bool Calibration::GetPixelPoint(double &x, double &y)
{
    if(nullptr == visionModule)return false;
    PRResultStruct prResult;
    ErrorCodeStruct temp = visionModule->PR_Generic_NCC_Template_Matching(camera_name,pr_name,prResult);
    x = prResult.x;
    y = prResult.y;
    qInfo((name + " pixel x: %f y: %f t: %f image: %s").toStdString().data(), prResult.x, prResult.y,prResult.theta,prResult.imageName.toStdString().c_str());
    return  temp.code == ErrorCode::OK;
}


