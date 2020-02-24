#include "SingleHead/chartheightcontrolmodule.h"

ChartHeightControlModule::ChartHeightControlModule()
{

}

void ChartHeightControlModule::init(XtMotor *motor_lf, XtMotor *motor_lr, XtMotor *motor_rf, XtMotor *motor_rr)
{
    this->motor_lf = motor_lf;
    this->motor_lr = motor_lr;
    this->motor_rf = motor_rf;
    this->motor_rr = motor_rr;
}
