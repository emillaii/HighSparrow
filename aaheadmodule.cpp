#include "aaheadmodule.h"
#include "config.h"

AAHeadModule::AAHeadModule(XtMotor* motor_x,XtMotor* motor_y,XtMotor* motor_z,XtMotor* motor_a,XtMotor* motor_b,XtMotor* motor_c,XtVacuum * v)
{
    this->motor_x = motor_x;
    this->motor_y = motor_y;
    this->motor_z = motor_z;
    this->motor_a = motor_a;
    this->motor_b = motor_b;
    this->motor_c = motor_c;
    this->v = v;
    this->loadParams();
    connect(&this->parameters, &AAHeadParameters::paramsChanged, this, &AAHeadModule::updateParams);
}

void AAHeadModule::loadParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("AA_HEAD_PARAMS", &parameters);
    PropertyBase::loadJsonConfig(AA_HEAD_MODULE_JSON, temp_map);
}

void AAHeadModule::updateParams()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("AA_HEAD_PARAMS", &this->parameters);
    PropertyBase::saveJsonConfig(AA_HEAD_MODULE_JSON,temp_map);
}

bool AAHeadModule::moveToPickLensPsotion()
{
    return  moveToSync(parameters.PickLensPositionX(),parameters.PickLensPositionY(),parameters.PickLensPositionZ(),parameters.PickLensPositionA(),parameters.PickLensPositionB(),parameters.PickLensPositionC());
}

bool AAHeadModule::moveToOCPsotion()
{
    return  moveToSync(parameters.OCPositionX(),parameters.OCPositionY(),parameters.OCPositionZ(),parameters.OCPositionA(),parameters.OCPositionB(),parameters.OCPositionC());
}

bool AAHeadModule::Move_XY_Sync(double x, double y)
{
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    bool result = motor_x->WaitArrivedTargetPos(x);
    result &= motor_y->WaitArrivedTargetPos(y);
    return result;
}

bool AAHeadModule::Move_AB_Sync(double a, double b)
{
    motor_a->MoveToPos(a);
    motor_b->MoveToPos(b);
    bool result = motor_a->WaitArrivedTargetPos(a);
    result &= motor_b->WaitArrivedTargetPos(b);
    return result;
}

bool AAHeadModule::moveToSync(double x, double y, double z, double a, double b, double c)
{
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    motor_z->MoveToPos(z);
    motor_a->MoveToPos(a);
    motor_b->MoveToPos(b);
    motor_c->MoveToPos(c);
    bool result = motor_x->WaitArrivedTargetPos(x);
    result &= motor_y->WaitArrivedTargetPos(y);
    result &= motor_z->WaitArrivedTargetPos(z);
    result &= motor_a->WaitArrivedTargetPos(a);
    result &= motor_b->WaitArrivedTargetPos(b);
    result &= motor_c->WaitArrivedTargetPos(c);
    return result;
}

