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
    AAHeadModule(QString name,XtMotor* motor_x,XtMotor* motor_y,XtMotor* motor_z,XtMotor* motor_a,XtMotor* motor_b,XtMotor* motor_c,XtVacuum * v);

public slots:
    void updateParams();

private:
    XtMotor* motor_x = Q_NULLPTR;
    XtMotor* motor_y = Q_NULLPTR;
    XtMotor* motor_z = Q_NULLPTR;
    XtMotor* motor_a = Q_NULLPTR;
    XtMotor* motor_b = Q_NULLPTR;
    XtMotor* motor_c = Q_NULLPTR;
    XtVacuum * v = Q_NULLPTR;
    VisionModule * visionModule;
    void loadParams();
public:
    AAHeadParameters parameters;
public:
    Q_INVOKABLE bool moveToPickLensPosition();
    Q_INVOKABLE bool moveToOCPosition();
    Q_INVOKABLE bool stepMove_XY_Sync(double step_x,double step_y);
    Q_INVOKABLE bool stepMove_AB_Sync(double step_a,double step_b);
    Q_INVOKABLE bool stepMove_Z_Sync(double step_z);
private:
    bool moveToSync(double x, double y, double z, double a, double b, double c);

};

#endif // AAHEADMODULE_H
