#include "dispense_module.h"
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
    loadConfig();
}

void DispenseModule::loadConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert(name, &parameters);
    QString temp ="";
            temp.append(name).append("R");
    temp_map.insert(temp, &this->dispenser->parameters);
    temp ="";
    temp.append(file_path).append(name).append(".json");
    PropertyBase::loadJsonConfig(temp, temp_map);
}

void DispenseModule::saveConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert(name, &parameters);
    QString temp ="";
            temp.append(name).append("R");
    temp_map.insert(temp, &this->dispenser->parameters);
    temp ="";
    temp.append(file_path).append(name).append(".json");
    PropertyBase::saveJsonConfig(temp, temp_map);
}

void DispenseModule::updatePath()
{
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
    qInfo("read point :%d",mechPoints.size());
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
    this->pr_theta = -pr_theta;
}
void DispenseModule::moveToDispenseDot(bool record_z)
{
    start_pos = carrier->GetFeedBackPos();
    if(!carrier->StepMove_SZ_XY_Sync(parameters.dispenseXOffset(),parameters.dispenseYOffset()))
    {
        return;
    }
    double result;
    if(carrier->ZSerchByForce(result,parameters.testForce()))
    {
        if(dispense_io != nullptr)
        dispense_io->Set(true);
        else
            qInfo("dispense_io is null");
        Sleep(parameters.openTime());
        if(record_z)
            parameters.setDispenseZPos(carrier->GetFeedBackPos().Z);

        if(dispense_io != nullptr)
            dispense_io->Set(false);
        carrier->ZSerchReturn();
    }
    carrier->Move_SZ_XY_Z_Sync(start_pos.X,start_pos.Y,start_pos.Z);
}

void DispenseModule::calulateOffset(int digit)
{
    mPoint3D end_pos = carrier->GetFeedBackPos();
    double x = parameters.dispenseXOffset() -(end_pos.X - start_pos.X);
    double y = parameters.dispenseYOffset() -(end_pos.Y - start_pos.Y);
    parameters.setDispenseXOffset((round(x*pow(10,4))/pow(10,4)));
    parameters.setDispenseYOffset((round(y*pow(10,4))/pow(10,4)));
}

QVector<mPoint3D> DispenseModule::getDispensePath()
{
    if(mechPoints.size() < 2)
        updatePath();
    QVector<mPoint3D> dispense_path = QVector<mPoint3D>();
    double temp_theta = (pr_theta - parameters.initTheta())*PI/180;
    for (int i = 0; i < mechPoints.size(); ++i) {
        double x = mechPoints[i].x() + pr_x;
        double y = mechPoints[i].y() + pr_y;
        dispense_path.push_back(mPoint3D(x*cos(temp_theta) + y*sin(temp_theta) + pos_x + parameters.dispenseXOffset(),y*cos(temp_theta)- x*sin(temp_theta) + pos_y + parameters.dispenseYOffset(),parameters.dispenseZPos() - parameters.dispenseZOffset()));
    }
    return dispense_path;
}

bool DispenseModule::performDispense()
{
   QVector<mPoint3D> temp_path = getDispensePath();
   if(temp_path.size()<2)
   {
       qInfo("point too small :%d",temp_path.size());
       return false;
   }
   QVector<DispensePathPoint> dispense_path;
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
   if( dispenser->Dispense(dispense_path))
       return dispenser->WaitForFinish();
    qInfo("Dispense fail");
   return false;
}
