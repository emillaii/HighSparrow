#ifndef CHARTHEIGHTCONTROLMODULE_H
#define CHARTHEIGHTCONTROLMODULE_H

#include "XtMotion/xtmotor.h"
#include "Utils/propertybase.h"



class ChartHeightControlModule : public PropertyBase
{
    Q_OBJECT

public:
    ChartHeightControlModule();

    void init(XtMotor *motor_lf, XtMotor *motor_lr, XtMotor *motor_rf, XtMotor *motor_rr);

signals:


public slots:

private:
    XtMotor *motor_lf;
    XtMotor *motor_lr;
    XtMotor *motor_rf;
    XtMotor *motor_rr;

};

#endif // CHARTHEIGHTCONTROLMODULE_H
