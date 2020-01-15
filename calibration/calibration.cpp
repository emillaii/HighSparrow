#include "calibration/calibration.h"
#include <QMessageBox>
#include <visionavadaptor.h>
#include <QFile>
#include <QTextStream>

Calibration::Calibration(QString name,QString file_path, QObject *parent)
{
    this->name = name;
    this->file_path = file_path;
    setName(name);
}

void Calibration::Init(XtMotor *motor_x, XtMotor *motor_y, VisionLocation *location)
{
    this->motor_x = motor_x;
    parts.append(motor_x);
    this->motor_y = motor_y;
    parts.append(motor_y);
    this->location =location;
    parts.append(location);
//    setName(parameters.calibrationName());
    QPointF image_center(parameters.imageWidth()/2,parameters.imageHeight()/2);
    mapping.ChangeParameter(QMatrix(parameters.matrix11(),
                                    parameters.matrix12(),
                                    parameters.matrix21(),
                                    parameters.matrix22(),
                                    parameters.deltaX(),
                                    parameters.deltaY()),
                            image_center);
    mOriginB = mA2BMapping.pixel2MechPoint(image_center);
    mRotationB = caculateRotationAngle();
}

bool Calibration::performCalibration()
{
    qInfo("Performing calibration");
    if(nullptr == motor_x||nullptr == motor_y)
    {
        AppendLineError(u8"轴为空");
        return false;
    }
    double xMove = parameters.calibrationStep();
    double yMove = parameters.calibrationStep();

    double pixel_x,pixel_y;
    QVector<QPointF> pixelPoints;
    QVector<QPointF> motorPoints;
    if (location)
    {
        location->OpenLight();
        QThread::msleep(200);
    }
    else {
        qInfo("This is not require vision");
    }

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
        parameters.setOriginY(mOriginB.y());
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
        if (!location) {
            double sensorSizeX = 0, sensorSizeY = 0, angle = 0;
            calculateMatrixAttribute(pixelPoints, motorPoints, sensorSizeX, sensorSizeY, angle);
            emit updata_aaCore_sensor_parameters_signal(sensorSizeX, sensorSizeY, angle);
        }
    }
    return true;
}

bool Calibration::performPRResult(PrOffset offset)
{
    if(motor_x == nullptr || motor_y == nullptr)
    {
        qInfo("motor is null");
        return false;
    }
    double cur_x = motor_x->GetFeedbackPos();
    double cur_y = motor_y->GetFeedbackPos();
    motor_x->StepMove(-offset.X);
    motor_y->StepMove(-offset.Y);
    bool result = motor_x->WaitArrivedTargetPos(cur_x - offset.X);
    result &= motor_y->WaitArrivedTargetPos(cur_y - offset.Y);
    return result;
}

double Calibration::getRotationAngle()
{
    return  mRotationB;
}

bool Calibration::getDeltaDistanceFromCenter(const QPointF pixelPoint, QPointF &distanceMech)
{

    if(mapping.hasCalibration())
    {
        mapping.CalcMechDistance(pixelPoint,distanceMech);
        qInfo("%s mech Result (%f,%f)",name.toStdString().c_str(),distanceMech.x(),distanceMech.y());
        return true;
    }
    return false;
}

QPointF Calibration::getOnePxielDistance()
{
    QPointF zero_zero,one_one;
   if(!getDeltaDistanceFromCenter(QPointF(0,0),zero_zero))
       return QPointF();
   if(!getDeltaDistanceFromCenter(QPointF(1,1),one_one))
       return QPointF();
   return QPointF(one_one.x()-zero_zero.x(),one_one.y()-zero_zero.y());
}

QPointF Calibration::getOneXPxielDistance()
{
    QPointF zero_zero,one_one;
   if(!getDeltaDistanceFromCenter(QPointF(0,0),zero_zero))
       return QPointF();
   if(!getDeltaDistanceFromCenter(QPointF(1,0),one_one))
       return QPointF();
   return QPointF(one_one.x()-zero_zero.x(),one_one.y()-zero_zero.y());
}

QPointF Calibration::getOneYPxielDistance()
{
    QPointF zero_zero,one_one;
   if(!getDeltaDistanceFromCenter(QPointF(0,0),zero_zero))
       return QPointF();
   if(!getDeltaDistanceFromCenter(QPointF(0,1),one_one))
       return QPointF();
   return QPointF(one_one.x()-zero_zero.x(),one_one.y()-zero_zero.y());
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

Pixel2Mech *Calibration::getCaliMapping()
{
    return &mapping;
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
    double sin2cos2 = qSqrt(parameters.matrix11()*parameters.matrix11()+parameters.matrix12()*parameters.matrix12());
    double cos_a =  qAcos(parameters.matrix11()/sin2cos2)*180/3.14159265;
    double sin_b =  qAsin(parameters.matrix12()/sin2cos2)*180/3.14159265;
    //todo 判断象限
    return cos_a;
}

bool Calibration::GetPixelPoint(double &x, double &y)
{
    PRResultStruct prResult;
    if(location->performPR(prResult))
    {
        x = prResult.x;
        y = prResult.y;
        qInfo((name + " pixel x: %f y: %f t: %f image: %s").toStdString().data(), prResult.x, prResult.y,prResult.theta,prResult.imageName.toStdString().c_str());
        return  true;
    }
    return  false;
}


bool Calibration::calculateMatrixAttribute(QVector<QPointF> p, QVector<QPointF> m, double &scaleX, double &scaleY, double &closestAngle)
{
    double diff_py10 = p[1].y() - p[0].y();
    double diff_px10 = p[1].x() - p[0].x();
    double diff_py30 = p[3].y() - p[0].y();
    double diff_px30 = p[3].x() - p[0].x();
    double diff_mx10 = m[1].x() - m[0].x();
    double diff_mx30 = m[3].x() - m[0].x();
    double diff_my10 = m[1].y() - m[0].y();
    double diff_my30 = m[3].y() - m[0].y();
    double a_11 = (diff_py10 * diff_mx30 - diff_py30 * diff_mx10) / (diff_px30 * diff_py10 - diff_py30 * diff_px10);
    double a_12 = (diff_mx10 - diff_px10 * a_11) / diff_py10;
    double a_13 = (m[0].x() - p[0].x() * a_11 - p[0].y() * a_12);

    double a_21 = (diff_py10 * diff_my30 - diff_py30 * diff_my10) / (diff_px30 * diff_py10 - diff_py30 * diff_px10);
    double a_22 = (diff_my10 - diff_px10 * a_21) / diff_py10;
    double a_23 = (m[0].y() - p[0].x() * a_21 - p[0].y() * a_22);

    double s_x = sqrt(a_11 * a_11 + a_21 * a_21);
    double s_y = sqrt(a_12 * a_12 + a_22 * a_22);
    if (s_x == 0 || s_y == 0) { return false; }
    scaleX = 1/s_x;
    scaleY = 1/s_y;
    double angle = acos(a_11 / s_x) * 180;
    double steppedAngle = angle - 360*((int)((int)angle/360));
    closestAngle = 0;
    if (steppedAngle <= 45) { closestAngle = 0; }
    else if (steppedAngle <= 135) { closestAngle = 90; }
    else if (steppedAngle <= 225) { closestAngle = 180; }
    else if (steppedAngle <= 315) { closestAngle = 270; }
    return true;
}


