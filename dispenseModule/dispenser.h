#ifndef DISPENSER_H
#define DISPENSER_H
#include "dispenseModule/dispenser_parameter.h"
#include "xtmotor.h"

#include <windows.h>
#include <QVector>
#include <XT_MotionControler_Client_Lib.h>
#include <XT_MotionControlerExtend_Client_Lib.h>

typedef enum {
    DISPENSER_NOT_INIT,
    DISPENSER_IDLE,
    DISPENSER_BUSY,
    DISPENSER_ERROR,
} DISPENSER_STATE;

typedef enum {
    PATH_POINT_LINE,
    PATH_POINT_OPEN_VALVE,
    PATH_POINT_CLOSE_VALVE,
} PATH_POINT_TYPE;


class DispensePathPoint
{
public:
    DispensePathPoint();
    DispensePathPoint(int dem_of_point, QVector<double> point,  PATH_POINT_TYPE point_type);
    int dem;
    QVector<double> p;
    PATH_POINT_TYPE type;
};


class Dispenser
{
private:
   static int dispenser_count;


public:
    Dispenser();
    void Init(int curve_id,int thread_curve,int thread_trig,QVector<XtMotor *> executive_motors,XtGeneralOutput* output_io, XtGeneralInput *input_io);
    ~Dispenser();
    bool Dispense(QVector<DispensePathPoint> &dispense_path);
    bool WaitForFinish(int time = 60000);
    void CleanUpCurve();

    DISPENSER_STATE GetState();
private:
    double getMaxSpeed(int index);
    double getEndSpeed(int index);
public:
    DispenserParameter parameters;
    bool glueLevelCheck();
private:
    QString file_path;
    QString name;
    int curve_id;
    int thread_curve;
    int thread_trig;
    QVector<XtMotor *> executive_motors;
    int dem;
    XtGeneralOutput* output_io = Q_NULLPTR;
    XtGeneralInput* input_io = Q_NULLPTR;
    DISPENSER_STATE state = DISPENSER_NOT_INIT;
};

#endif // DISPENSER_H
