#ifndef MATERIAL_PICKER_H
#define MATERIAL_PICKER_H

#include "XtMotion/XtVacuum.h"
#include "XtMotion/XtVcMotor.h"



class MaterialPicker
{
public:
    MaterialPicker(){}
    void Init(XtVcMotor* motor_z,XtMotor* motor_t,XtVacuum* vacuum)
    {
        this->motor_z = motor_z;
        this->motor_t = motor_t;
        this->vacuum = vacuum;
    }
    XtVcMotor* motor_z = Q_NULLPTR;
    XtMotor* motor_t = Q_NULLPTR;
    XtVacuum* vacuum = Q_NULLPTR;
};

#endif // MATERIAL_PICKER_H
