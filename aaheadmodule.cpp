#include "aaheadmodule.h"
#include "config.h"

AAHeadModule::AAHeadModule(QString name,XtMotor* motor_x,XtMotor* motor_y,XtMotor* motor_z,XtMotor* motor_a,XtMotor* motor_b,XtMotor* motor_c,XtVacuum * v)
{
    parameters.setName(name);
    this->motor_x = motor_x;
    this->motor_y = motor_y;
    this->motor_z = motor_z;
    this->motor_a = motor_a;
    this->motor_b = motor_b;
    this->motor_c = motor_c;
    this->v = v;
    this->loadParams();
    //connect(&this->parameters, &AAHeadParameters::paramsChanged, this, &AAHeadModule::updateParams);
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

bool AAHeadModule::moveToPickLensPosition()
{
    return  moveToSync(parameters.PickLensPositionX(),parameters.PickLensPositionY(),parameters.PickLensPositionZ(),parameters.PickLensPositionA(),parameters.PickLensPositionB(),parameters.PickLensPositionC());
}

bool AAHeadModule::moveToOCPosition()
{
    return  moveToSync(parameters.OCPositionX(),parameters.OCPositionY(),parameters.OCPositionZ(),parameters.OCPositionA(),parameters.OCPositionB(),parameters.OCPositionC());
}

bool AAHeadModule::stepMove_XY_Sync(double step_x, double step_y)
{
    double x = step_x + motor_x->GetFeedbackPos();
    double y = step_y + motor_y->GetFeedbackPos();
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    bool result = motor_x->WaitArrivedTargetPos(x);
    result &= motor_y->WaitArrivedTargetPos(y);
    return result;
}

bool AAHeadModule::stepMove_AB_Sync(double step_a, double step_b)
{
    double a = step_a + motor_a->GetFeedbackPos();
    double b = step_b + motor_b->GetFeedbackPos();
    motor_a->MoveToPos(a);
    motor_b->MoveToPos(b);
    bool result = motor_a->WaitArrivedTargetPos(a);
    result &= motor_b->WaitArrivedTargetPos(b);
    return result;
}

bool AAHeadModule::stepMove_Z_Sync(double step_z)
{
    double z = step_z + motor_z->GetFeedbackPos();
    motor_z->MoveToPos(z);
    return motor_z->WaitArrivedTargetPos(z);
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

