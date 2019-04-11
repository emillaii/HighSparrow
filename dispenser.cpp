﻿#include "dispenser.h"

DispensePathPoint::DispensePathPoint()
{

}

DispensePathPoint::DispensePathPoint(int dem_of_point, QVector<double> point, PATH_POINT_TYPE point_type)
{
    dem = dem_of_point;
    for(int i=0; i<dem; i++)
    {
        p.append(point[i]);
    }
    type = point_type;
}


int Dispenser::dispenser_count = 0;
Dispenser::Dispenser()
{
    state = DISPENSER_IDLE;
}

void Dispenser::Init(QString file_path, QString name, int curve_id, int thread_curve, int thread_trig, QVector<XtMotor *> executive_motors, XtGeneralOutput *output_io)
{
    dispenser_count++;
    this->file_path = file_path;
    this->name = name;
    this->curve_id = curve_id;
    this->thread_curve = thread_curve;
    this->executive_motors = executive_motors;
    this->dem = executive_motors.length();
    this->output_io = output_io;
}

Dispenser::~Dispenser()
{

}
bool Dispenser::Dispense(QVector<DispensePathPoint> &dispense_path)
{
    if(GetState()!= DISPENSER_IDLE)
    {
        qInfo("Dispenser NOT READY!");
        return false;
    }
    for(int i=0; i<dispense_path.length(); i++)
    {
        if(dispense_path[i].dem!=dem)
        {
            qInfo("Dispenser's Dem is %d BUT dem of point %d is %d",dem,i,dispense_path[i].dem);
            return false;
        }
    }
    int nPoint_Index = 0;
    QVector<int> axis;
    QVector<int> axis_combine;
    QVector<double> axis_max_vel;
    QVector<double> axis_max_acc;
    QVector<double> axis_max_jerk;
    const XtMotor * m;
    for(int i = 0; i<executive_motors.length(); i++)
    {
        m = executive_motors[i];
        axis.append(m->AxisId());
        axis_combine.append(1);
        axis_max_vel.append(m->GetMaxVel());
        axis_max_acc.append(m->GetMaxAcc());
        axis_max_jerk.append(m->GetMaxJerk());
    }
    int res = XT_Controler_Extend::Set_Curve_Param(curve_id, 0.1, dem, axis.data(),
                                                   axis_max_vel.data(), axis_max_acc.data(), axis_max_jerk.data(),
                                                   axis_combine.data());
    if (1 != res)
    {
        qInfo("Set_Curve_Param failed.");
        state = DISPENSER_ERROR;
        return false;
    }

    double first_line_len=0;
    for(int i=0; i<dispense_path.length(); i++)
    {
        if(i==2)
        res = XT_Controler_Extend::Append_Line_Pos(curve_id, dem, axis.data(), dispense_path[i].p.data(),
                                                   parameters.maximumSpeed(), parameters.endSpeed(), 0, nPoint_Index);
        else {
            res = XT_Controler_Extend::Append_Line_Pos(curve_id, dem, axis.data(), dispense_path[i].p.data(),
                                                       parameters.maximumSpeed(), parameters.endSpeed(), 0, nPoint_Index);

        }
        qInfo("point %d : %f,%f",i, dispense_path[i].p.data()[0],dispense_path[i].p.data()[1]);
        if(1!=res)
        {
            qInfo("error in adding No%d point!",i);
            state = DISPENSER_ERROR;
            return false;
        }
//        if(i!=0 && dispense_path[i].type != PATH_POINT_LINE)
//        {
//            int value=0;
//            if(dispense_path[i].type == PATH_POINT_OPEN_VALVE)
//                value =1;
//            else if(dispense_path[i].type == PATH_POINT_CLOSE_VALVE)
//                value = 0;
//            res = XT_Controler_Extend::Set_Cur_Trig_Output(curve_id, 1, -output_distence_offset, output_time_offset / 1000, valve_output_id, value);
//        }
        if(i==1)
        {
            first_line_len = XT_Controler_Extend::Curve_Get_LengthPos(curve_id,nPoint_Index);
            qInfo("first_line_len is %f",first_line_len);
            double half_len = first_line_len/2.0;
            if(parameters.openOffset()>half_len)
            {
                qInfo("%f Too BIG, set to %f",parameters.openOffset(),half_len);
                parameters.setOpenOffset(half_len);

            }
            if(parameters.openOffset()<-half_len)
            {
                qInfo("%f Too SMALL, set to %f",parameters.openOffset(),-half_len);
                parameters.setOpenOffset(-half_len);
            }
            res = XT_Controler_Extend::Set_Cur_Trig_Output(curve_id, 0, first_line_len/2+parameters.openOffset(), 0, output_io->GetID(), 1);
            qInfo("point:%d length:%f offset:%f",i,first_line_len, first_line_len/2+parameters.openOffset());
            if(1!=res)
            {
                qInfo("error in adding IO open In No%d point!",i);
                state = DISPENSER_ERROR;
                return false;
            }
            qInfo("Dispenser Open Position Set To %f", first_line_len/2+parameters.openOffset());
        }
        if((i==dispense_path.length()-1)&&(i>1))
        {
            res = XT_Controler_Extend::Append_Line_Pos(curve_id, dem, axis.data(), dispense_path[0].p.data(),
                                                       parameters.maximumSpeed(), parameters.endSpeed(), 0, nPoint_Index);
            qInfo("point end0%d : %f,%f",i, dispense_path[0].p.data()[0],dispense_path[0].p.data()[1]);
            if(1!=res)
            {
                qInfo("error in adding No%d point!",i+1);
                state = DISPENSER_ERROR;
                return false;
            }
            res = XT_Controler_Extend::Append_Line_Pos(curve_id, dem, axis.data(), dispense_path[1].p.data(),
                                                       parameters.maximumSpeed(), parameters.endSpeed(), 0, nPoint_Index);
            qInfo("point end1%d : %f,%f",i, dispense_path[1].p.data()[0],dispense_path[1].p.data()[1]);
            if(1!=res)
            {
                qInfo("error in adding No%d point!",i+2);
                state = DISPENSER_ERROR;
                return false;
            }

            double half_len = first_line_len/2.0;
            if(parameters.closeOffset()>half_len)
            {
                qInfo("%f Too BIG, set to %f",parameters.closeOffset(),half_len);
                parameters.setCloseOffset(half_len);

            }
            if(parameters.closeOffset()<-half_len)
            {
                qInfo("%f Too SMALL, set to %f",parameters.closeOffset(),-half_len);
                parameters.setCloseOffset(-half_len);
            }

            res = XT_Controler_Extend::Set_Cur_Trig_Output(curve_id, 0, first_line_len/2+parameters.closeOffset(), 0, output_io->GetID(), 0);
            qInfo("point:%d length:%f offset:%f",i,first_line_len, first_line_len/2+parameters.closeOffset());
            if(1!=res)
            {
                qInfo("error in adding IO close In No%d point!",i);
                state = DISPENSER_ERROR;
                return false;
            }
            qInfo("Dispenser Close Position Set To %f", first_line_len/2+parameters.closeOffset());
        }


    }

    res = XT_Controler_Extend::Exec_Curve(curve_id, thread_curve, thread_trig, 1);
    if(1!=res)
    {
        qInfo("dispensing path execute fail");
        state = DISPENSER_ERROR;
        return false;
    }
    return true;
}

void Dispenser::CleanUpCurve()
{
    XT_Controler_Extend::Clear_Exec(curve_id);
}

DISPENSER_STATE Dispenser::GetState()
{
    if(0.0 == parameters.maximumSpeed() || state == DISPENSER_NOT_INIT)
        return DISPENSER_NOT_INIT;
    if(state == DISPENSER_ERROR)
        return DISPENSER_ERROR;
    int res = XT_Controler_Extend::Check_Finish(curve_id);
    if(res!=1)
        return DISPENSER_BUSY;
    return DISPENSER_IDLE;
}

bool Dispenser::WaitFinish(int timeout)
{
    int time = timeout;
    while(time>0)
    {
        if(GetState() == DISPENSER_IDLE)
            return true;
        time-=10;
        Sleep(10);
    }
    return false;
}




