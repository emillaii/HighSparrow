#include "dispense_module.h"
#include <config.h>

DispenseModule::DispenseModule():QObject ()
{
}

void DispenseModule::Init(Calibration *calibration, Dispenser *dispenser,VisionModule* vision,MaterialCarrier* carrier,XtGeneralOutput* dispense_io)
{
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
    temp_map.insert("Dispense_PARAMS", &parameters);
    temp_map.insert("Dispenser_PARAMS", &this->dispenser->parameters);
    PropertyBase::loadJsonConfig("config//Dispense.json", temp_map);
}

void DispenseModule::saveConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("Dispense_PARAMS", &this->parameters);
    temp_map.insert("Dispenser_PARAMS", &this->dispenser->parameters);
    PropertyBase::saveJsonConfig("config//lutConfig.json", temp_map);
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
}

void DispenseModule::setMapPosition(double x, double y, double pr_theta)
{
    this->x = x;
    this->y = y;
    this->pr_theta = pr_theta;
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
        dispense_io->Set(true);
        Sleep(1000);
        if(record_z)
            parameters.setDispenseZPos(carrier->GetFeedBackPos().Z);
        dispense_io->Set(false);
        carrier->ZSerchReturn();
    }
    carrier->Move_SZ_XY_Z_Sync(start_pos.X,start_pos.Y,start_pos.Z);
}

void DispenseModule::calulateOffset()
{
    mPoint3D end_pos = carrier->GetFeedBackPos();
    double x = parameters.dispenseXOffset() + end_pos.X - start_pos.X;
    double y = parameters.dispenseYOffset() + end_pos.Y - start_pos.Y;
    parameters.setDispenseXOffset(x);
    parameters.setDispenseYOffset(y);
}

QVector<mPoint3D> DispenseModule::getDispensePath()
{
    if(mechPoints.size() < 2)
        updatePath();
    double x,y;
    QVector<mPoint3D> dispense_path = QVector<mPoint3D>();
    double temp_theta = pr_theta - parameters.initTheta();
    for (int i = 0; i < mechPoints.size(); ++i) {
        x = mechPoints[i].x() + x;
        y = mechPoints[i].y() + y;
        dispense_path.push_back(mPoint3D(x*cos(temp_theta) + y*sin(temp_theta) + parameters.dispenseXOffset(),y*cos(temp_theta)- x*sin(temp_theta) + parameters.dispenseYOffset(),parameters.dispenseZPos() - parameters.dispenseZOffset()));
    }
    return dispense_path;
}

bool DispenseModule::performDispense()
{
   QVector<mPoint3D> temp_path = getDispensePath();
   if(temp_path.size()<2)return false;
   QVector<DispensePathPoint> dispense_path;
   for (int i = 0; i < dispense_path.size(); ++i)
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
   return dispenser->Dispense(dispense_path);
}
