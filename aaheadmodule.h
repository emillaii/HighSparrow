#ifndef AAHEADMODULE_H
#define AAHEADMODULE_H

#include "XtVacuum.h"
#include "xtmotor.h"

#include <QObject>
#include <propertybase.h>
#include <visionmodule.h>
#include <aaheadparameters.h>

class AAHeadModule : public QObject
{
    Q_OBJECT
public:
    AAHeadModule(XtMotor* motor_x,XtMotor* motor_y,XtMotor* motor_z,XtMotor* motor_a,XtMotor* motor_b,XtMotor* motor_c,XtVacuum * v);

public slots:


private:
    XtMotor* motor_x = Q_NULLPTR;
    XtMotor* motor_y = Q_NULLPTR;
    XtMotor* motor_z = Q_NULLPTR;
    XtMotor* motor_a = Q_NULLPTR;
    XtMotor* motor_b = Q_NULLPTR;
    XtMotor* motor_c = Q_NULLPTR;
    XtVacuum * v = Q_NULLPTR;
    VisionModule * visionModule;
public:
    AAHeadParameters parameters;
public:
    bool moveToPickLensPsotion();
    bool moveToOCPsotion();
    bool Move_XY_Sync(double x,double y);
    bool Move_AB_Sync(double a,double b);
private:
    bool moveToSync(double x, double y, double z, double a, double b, double c);

};

#endif // AAHEADMODULE_H
