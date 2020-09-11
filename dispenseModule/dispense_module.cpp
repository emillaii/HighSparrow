#include "dispenseModule/dispense_module.h"
#include <QMessageBox>
#include <config.h>
#define PI 3.1415926535898
DispenseModule::DispenseModule():QObject ()
{
}

void DispenseModule::Init(QString file_path,QString name,Calibration *calibration, Dispenser *dispenser,VisionModule* vision,MaterialCarrier* carrier,XtGeneralOutput* dispense_io)
{
    this->file_path = file_path;
    this->name = name;
    this->calibration = calibration;
    this->dispenser = dispenser;
    this->vision = vision;
    this->carrier = carrier;
    this->dispense_io = dispense_io;
}

//void DispenseModule::loadConfig()
//{
//    QMap<QString,PropertyBase*> temp_map;
//    temp_map.insert(name, &parameters);
//    QString temp ="";
//            temp.append(name).append("R");
//    temp_map.insert(temp, &this->dispenser->parameters);
//    temp ="";
//    temp.append(file_path).append(name).append(".json");
//    PropertyBase::loadJsonConfig(temp, temp_map);
//}

//void DispenseModule::saveConfig()
//{
//    QMap<QString,PropertyBase*> temp_map;
//    temp_map.insert(name, &parameters);
//    QString temp ="";
//            temp.append(name).append("R");
//    temp_map.insert(temp, &this->dispenser->parameters);
//    temp ="";
//    temp.append(file_path).append(name).append(".json");
//    PropertyBase::saveJsonConfig(temp, temp_map);
//}

void DispenseModule::updatePath()
{
    getDispenseCircleProperties();
    QVector<QPoint> map_path = vision->Read_Dispense_Path();
    mechPoints.clear();

    foreach(QPoint pt, map_path)
    {
        QPointF mechPoint;
        if(calibration->getDeltaDistanceFromCenter(pt, mechPoint))
        {
            mechPoints.push_back(QPointF(-mechPoint.x(), -mechPoint.y()));
        }
    }
    dispenser->parameters.setSpeedCount(mechPoints.size());
    qInfo("read point :%d",mechPoints.size());
    emit callQmlRefeshImg(12);
}

bool DispenseModule::getDispenseCircleProperties()
{
    QFile file;
    QString val;
    file.setFileName(DISPENSE_PATH_CIRCLE_PROPERTIES);

    if (!file.exists()) {
        qWarning("Dispense path circle properties file does not exist");
        return false;
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject sett2 = d.object();
    QJsonValue patternType = sett2.value(QString("type"));
    QJsonValue radius = sett2.value(QString("radius"));
    QJsonValue center_x = sett2.value(QString("center_x"));
    QJsonValue center_y = sett2.value(QString("center_y"));
    //radius_in_mm = radius.toDouble(0);
    pattern_type = patternType.toInt(0);
    center.setX(center_x.toDouble());
    center.setY(center_y.toDouble());

    if (pattern_type == 1)  //Circle
    {
        QPointF pt;
        pt.setX(center_x.toDouble());
        pt.setY(center_y.toDouble());
        QPointF mechPoint;
        calibration->getDeltaDistanceFromCenter(pt, mechPoint);
        qInfo("mechPoint x: %f mechPoint y: %f", mechPoint.x(), mechPoint.y());
        mechCenter.setX(-mechPoint.x());
        mechCenter.setY(-mechPoint.y());
    }

    return true;
}

void DispenseModule::updateSpeed()
{
    for (int i=0; i<dispenser->parameters.speedCount()*2;i++)
    {
        dispenser->parameters.setLineSpeed(i, dispenser->parameters.maximumSpeed());
    }
}

void DispenseModule::setMapPosition(double pos_x, double pos_y)
{
    this->pos_x = pos_x;
    this->pos_y = pos_y;
}

void DispenseModule::setPRPosition(double pr_x, double pr_y, double pr_theta)
{
    this->pr_x = -pr_x;
    this->pr_y = -pr_y;
    this->pr_theta = pr_theta;
}

void DispenseModule::moveToDispenseDot(bool record_z)
{
    cancalculation = true;
    start_pos = carrier->GetFeedBackPos();
    if(!carrier->StepMove_SZ_XY_Sync(parameters.dispenseXOffset(),parameters.dispenseYOffset()))
    {
        return;
    }
    if(carrier->ZSerchByForce(10,parameters.testForce()))
    {
        QThread::msleep(200);
        double zValue = carrier->motor_z->GetFeedbackPos();
        if(record_z)
        {
            QMessageBox::StandardButton rb = QMessageBox::information(nullptr,tr(u8"标题"),tr(u8"是否应用此高度:%1").arg(zValue),QMessageBox::Yes|QMessageBox::No);
            if(rb==QMessageBox::Yes)
            {
                parameters.setDispenseZPos(zValue);
                if(dispense_io != nullptr)
                {
                    dispense_io->Set(true);
                    Sleep(parameters.openTime());
                    dispense_io->Set(false);
                }
                else
                    qInfo("dispense_io is null");
            }
            carrier->ZSerchReturn();
        }
        else
        {
            carrier->ZSerchReturn();
            if(parameters.dispenseZOffset()>0)
            {
                carrier->motor_z->SlowMoveToPosSync(zValue - parameters.dispenseZOffset(),50);
                QThread::msleep(100);
                if(dispense_io != nullptr)
                {
                    dispense_io->Set(true);
                    Sleep(parameters.openTime());
                    dispense_io->Set(false);
                }
                else
                    qInfo("dispense_io is null");
                QThread::msleep(100);
                //carrier->motor_z->MoveToPosSync(0);
            }
        }
    }
    carrier->Move_SZ_XY_Z_Sync(start_pos.X,start_pos.Y,start_pos.Z);
}

void DispenseModule::calulateOffset(int digit)
{
    if(!cancalculation)return;
    mPoint3D end_pos = carrier->GetFeedBackPos();
    double x = parameters.dispenseXOffset() -(end_pos.X - start_pos.X);
    double y = parameters.dispenseYOffset() -(end_pos.Y - start_pos.Y);
    parameters.setDispenseXOffset((round(x*pow(10,4))/pow(10,4)));
    parameters.setDispenseYOffset((round(y*pow(10,4))/pow(10,4)));
    cancalculation = false;
}

QVector<mPoint3D> DispenseModule::getDispensePath()
{
    if(mechPoints.empty())
        updatePath();
    QVector<mPoint3D> dispense_path = QVector<mPoint3D>();
    double temp_theta = (pr_theta - parameters.initTheta())*PI/180;
    for (int i = 0; i < mechPoints.size(); ++i) {
        double x = mechPoints[i].x();
        double y = mechPoints[i].y();
        dispense_path.push_back(mPoint3D(x*cos(temp_theta) + y*sin(temp_theta) + pos_x + pr_x + parameters.dispenseXOffset(),
                                         y*cos(temp_theta)- x*sin(temp_theta) + pos_y + pr_y + parameters.dispenseYOffset(),
                                         parameters.dispenseZPos() - parameters.dispenseZOffset()));
    }
    return dispense_path;
}

bool DispenseModule::performDispense()
{
    QVector<mPoint3D> temp_path = getDispensePath();
    QVector<DispensePathPoint> dispense_path;
    if(temp_path.size()<2)
    {
       qInfo("point too small :%d",temp_path.size());
       return false;
    }
    if (pattern_type == 0)
    {
        for (int i = 0; i < temp_path.size(); ++i)
        {
           QVector<double> pos;
           PATH_POINT_TYPE type = PATH_POINT_LINE;
           pos.append(temp_path[i].X);
           pos.append(temp_path[i].Y);
           pos.append(temp_path[i].Z);
           if (i == temp_path.size() - 1) type = PATH_POINT_CLOSE_VALVE;
           else if (i == 0) type = PATH_POINT_CLOSE_VALVE;
           else type = PATH_POINT_OPEN_VALVE;
           dispense_path.append(DispensePathPoint(3,pos,type));
        }
    }
    else if (pattern_type == 1)
    {
        for (int i = 0; i < temp_path.size(); ++i)
        {
           QVector<double> pos;
           PATH_POINT_TYPE type = PATH_POINT_LINE;
           pos.append(temp_path[i].X);
           pos.append(temp_path[i].Y);
           pos.append(temp_path[i].Z);
           if (i == 0) type = PATH_POINT_CLOSE_VALVE;
           else type = PATH_POINT_OPEN_VALVE;
           dispense_path.append(DispensePathPoint(3,pos,type));
        }
    }

    qInfo("Dispense start");
    lastDispenseDateTime = QDateTime::currentDateTime();
    parameters.setLastDispenseTime(lastDispenseDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    if (pattern_type == 0)
    {
        if( dispenser->Dispense(dispense_path))
            return dispenser->WaitForFinish();
    }
    else if (pattern_type == 1)
    {
        double temp_theta = (pr_theta - parameters.initTheta())*PI/180;
        double x = mechCenter.x();
        double y = mechCenter.y();
        QPointF dispenseMechCenter;
        dispenseMechCenter.setX(x*cos(temp_theta) + y*sin(temp_theta) + pos_x + pr_x + parameters.dispenseXOffset());
        dispenseMechCenter.setX(y*cos(temp_theta)- x*sin(temp_theta) + pos_y + pr_y + parameters.dispenseYOffset());

        if( dispenser->DispenseCircle(dispense_path, dispenseMechCenter))
            return dispenser->WaitForFinish();
    }
    qInfo("Dispense fail");
    return false;
}
