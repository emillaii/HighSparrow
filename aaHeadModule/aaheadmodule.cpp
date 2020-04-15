#include "aaHeadModule/aaheadmodule.h"
#include "config.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include <QtMath>

AAHeadModule::AAHeadModule()
{
    //connect(&this->parameters, &AAHeadParameters::paramsChanged, this, &AAHeadModule::updateParams);
}

void AAHeadModule::loadJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("AA_HEAD_PARAMS", &parameters);
    temp_map.insert("AA_HEAD_POSITION", &this->mushroom_position);
    temp_map.insert("AA_PICK_LENS_POSITION", &this->pick_lens_position);
    temp_map.insert("pr2Bond_offset", &this->pr2Bond_offset);
    PropertyBase::loadJsonConfig(file_name, temp_map);
}

void AAHeadModule::saveJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("AA_HEAD_PARAMS", &this->parameters);
    temp_map.insert("AA_HEAD_POSITION", &this->mushroom_position);
    temp_map.insert("AA_PICK_LENS_POSITION", &this->pick_lens_position);
    temp_map.insert("pr2Bond_offset", &this->pr2Bond_offset);
    PropertyBase::saveJsonConfig(file_name,temp_map);
}

void AAHeadModule::Init(QString name, XtMotor *motor_x, XtMotor *motor_y, XtMotor *motor_z, XtMotor *motor_a, XtMotor *motor_b, XtMotor *motor_c, XtGeneralOutput *gripper,
                        XtGeneralOutput * uv1,
                        XtGeneralOutput * uv2,
                        XtGeneralOutput * uv3,
                        XtGeneralOutput * uv4,
                        int thread_id,
                        MaterialCarrier* sut_carrier)
{
//    this->motor_x = motor_x;
//    this->motor_y = motor_y;
//    this->motor_z = motor_z;
    this->motor_x = sut_carrier->motor_x;
    this->motor_y = sut_carrier->motor_y;
    this->motor_z = sut_carrier->motor_z;
    this->motor_a = motor_a;
    this->motor_b = motor_b;
    this->motor_c = motor_c;
    this->gripper = gripper;
    this->uv1 = uv1;
    this->uv2 = uv2;
    this->uv3 = uv3;
    this->uv4 = uv4;
    this->thread_id = thread_id;
    setName(parameters.moduleName());

}

bool AAHeadModule::moveToMushroomPosition(bool moveXYC)
{
    if (moveXYC)
        return moveToDiffrentZSync(mushroom_position.Z());
    else {
        mPoint6D point = this->GetFeedBack();
        return moveToSync(point.X,point.Y,mushroom_position.Z(),mushroom_position.A(),mushroom_position.B(),point.C);
    }
}

bool AAHeadModule::moveToPickLensPosition()
{
    //return  moveToDiffrentZSync(parameters.PickLensPositionZ());
    return moveToSync(pick_lens_position.X(), pick_lens_position.Y(), pick_lens_position.Z(), pick_lens_position.A(), pick_lens_position.B(), pick_lens_position.C());
}

bool AAHeadModule::moveToOCPosition()
{
    return  moveToDiffrentZSync(parameters.OCPositionZ());
}

bool AAHeadModule::moveToAAPosition()
{
    return  moveToSync(last_aa_position.X(),last_aa_position.Y(),last_aa_position.Z(),last_aa_position.A(),last_aa_position.B(),last_aa_position.C());
}

void AAHeadModule::setUplookResult(double x, double y, double theta)
{
    uplook_x = x;
    uplook_y = y;
    uplook_theta = theta;
}

bool AAHeadModule::moveToUplookResultPosition()
{
    return stepMove_XYC_ToSync(uplook_x,uplook_y,uplook_theta);
}

void AAHeadModule::openUVTillTime(int till_time)
{ 
    uv1->SET(true,thread_id);
    uv2->SET(true,thread_id);
    uv3->SET(true,thread_id);
    uv4->SetOnTllTimeSpan(till_time,thread_id);
    uv1->SET(false,thread_id);
    uv2->SET(false,thread_id);
    uv3->SET(false,thread_id);

//    XT_Controler::SET_OUTPUT_IO(thread_id,uv1->GetID(),1);
//    XT_Controler::SET_OUTPUT_IO(thread_id,uv2->GetID(),1);
//    XT_Controler::SET_OUTPUT_IO(thread_id,uv3->GetID(),1);
//    XT_Controler::SET_OUTPUT_IO(thread_id,uv4->GetID(),1);
//    QThread::msleep(till_time);
//    XT_Controler::SET_OUTPUT_IO(thread_id,uv1->GetID(),0);
//    XT_Controler::SET_OUTPUT_IO(thread_id,uv2->GetID(),0);
//    XT_Controler::SET_OUTPUT_IO(thread_id,uv3->GetID(),0);
//    XT_Controler::SET_OUTPUT_IO(thread_id,uv4->GetID(),0);
}

void AAHeadModule::waitUVFinish()
{
    uv1->WaitForFinish(thread_id);
}

bool AAHeadModule::openGripper()
{
    if (!gripper) return true; // For do not have gripper case
    return  gripper->Set(true);
//    Sleep(200);
}

bool AAHeadModule::closeGripper()
{
    if (!gripper) return true; // For do not have gripper case
    return gripper->Set(false);
//    Sleep(200);
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

bool AAHeadModule::stepMove_XYC_Sync(double step_x, double step_y, double step_c)
{
    double x = step_x + motor_x->GetFeedbackPos();
    double y = step_y + motor_y->GetFeedbackPos();
    double c = step_c + motor_c->GetFeedbackPos();
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    motor_c->MoveToPos(c);
    bool result = motor_x->WaitArrivedTargetPos(x);
    result &= motor_y->WaitArrivedTargetPos(y);
    result &= motor_c->WaitArrivedTargetPos(c);
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

bool AAHeadModule::stepInterpolation_AB_Sync(double step_a, double step_b)
{
    double dy = qSin(qDegreesToRadians(step_a))*parameters.rotateZOffset();
    double new_z = qCos(qDegreesToRadians(step_a))*parameters.rotateZOffset();
    double dx = qSin(qDegreesToRadians(step_b))*new_z;
    new_z = qCos(qDegreesToRadians(step_b))*new_z;
    double dz = new_z - parameters.rotateZOffset();

    if(motor_x->Name().contains("SUT"))
        dx =-dx;
    if(motor_y->Name().contains("SUT"))
        dy =-dy;
    if(motor_z->Name().contains("SUT"))
        dz =-dz;

    double x = -dx + motor_x->GetFeedbackPos();
    double y = dy + motor_y->GetFeedbackPos();
    double z = -dz + motor_z->GetFeedbackPos();
    double a = step_a + motor_a->GetFeedbackPos();
    double b = step_b + motor_b->GetFeedbackPos();
    return XYZAB_Interpolation(x,y,z,a,b);
}

bool AAHeadModule::stepMove_Z_Sync(double step_z)
{
    double z = step_z + motor_z->GetFeedbackPos();
    motor_z->MoveToPos(z);
    return motor_z->WaitArrivedTargetPos(z);
}

void AAHeadModule::SetAAPosion(mPoint6D point)
{
    last_aa_position.SetPosition(point);
}

mPoint6D AAHeadModule::GetFeedBack()
{
    return mPoint6D(motor_x->GetFeedbackPos(),motor_y->GetFeedbackPos(),motor_z->GetFeedbackPos(),motor_a->GetFeedbackPos(),motor_b->GetFeedbackPos(),motor_c->GetFeedbackPos());
}

void AAHeadModule::sendSensorRequest(int sut_state)
{
    qInfo("sendSensrRequest %d",sut_state);
    waiting_sensor = true;
    offset_x = 0;
    offset_y = 0;
    offset_theta = 0;
    emit sendSensrRequestToSut(sut_state);
}

bool AAHeadModule::waitForLoadSensor(bool &is_run)
{
    int time_out = 0;
    while (is_run) {
        if(!waiting_sensor)
        {
            qInfo("wait For LoadSensor succeed.");
            return  true;
        }
        QThread::msleep(1);
        time_out ++;
    }
    qInfo("wait For LoadSensor interrupt.");
    return false;
}

bool AAHeadModule::moveToSync(double x, double y, double z, double c)
{
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    motor_z->MoveToPos(z);
    motor_c->MoveToPos(c);
    bool result = motor_x->WaitArrivedTargetPos(x);
    result &= motor_y->WaitArrivedTargetPos(y);
    result &= motor_z->WaitArrivedTargetPos(z);
    result &= motor_c->WaitArrivedTargetPos(c);
    return result;
}

// not used
bool AAHeadModule::moveToSZ_XYC_Z_Sync(double x, double y, double z, double c)
{
    bool result = motor_z->MoveToPosSync(0);
    if(result)
    {
        result &= motor_x->MoveToPos(x);
        result &= motor_y->MoveToPos(y);
        result &= motor_c->MoveToPos(c);
        result &= motor_x->WaitArrivedTargetPos(x);
        result &= motor_y->WaitArrivedTargetPos(y);
        result &= motor_c->WaitArrivedTargetPos(c);
    }
    if(result)
        result &= motor_z->MoveToPosSync(z);
    return result;
}

bool AAHeadModule::moveToSZ_XYSC_Z_Sync(double x, double y, double z, double c)
{
    //bool result = motor_z->MoveToPosSync(2);
    bool result = motor_x->MoveToPos(x);
    result &= motor_c->MoveToPos(c);
    result &= motor_y->MoveToPosSaftySync(y);
    result &= motor_x->WaitArrivedTargetPos(x);
    result &= motor_c->WaitArrivedTargetPos(c);
    if(result)
        result &= motor_z->MoveToPosSync(z);
    return result;
}

bool AAHeadModule::homeTilt()
{
    motor_a->Home();
    motor_b->Home();
    motor_c->Home();
    return true;
}

void AAHeadModule::receiveLensFromLut(double offset_x, double offset_y, double offset_theta)
{
    qInfo("receiveSensorFromSut %f %f %f",offset_x,offset_y,offset_theta);
    this->uplook_x = offset_x;
    this->uplook_y = offset_y;
    this->uplook_theta = offset_theta;
    receive_lens = true;
    waiting_lens = false;
}

void AAHeadModule::receiveSensorFromSut(double offset_x, double offset_y, double offset_theta)
{
    qInfo("receiveSensorFromSut %f %f %f",offset_x,offset_y,offset_theta);
    this->offset_x = offset_x;
    this->offset_y = offset_y;
    this->offset_theta = offset_theta;
}

void AAHeadModule::performHandlingOperation(int cmd, QVariant param)
{
    bool result = true;
    int temp_value = 10;
    qInfo("performHandlingOperation cmd: %d", cmd);
    if(cmd%temp_value == HandlePosition::PICK_LENS_POS)
        result = moveToPickLensPosition();
    if(!result)
        sendAlarmMessage(OK_OPERATION,GetCurrentError(),ErrorLevel::TipNonblock);
    is_handling = false;
}

bool AAHeadModule::moveToDiffrentZSync(double z)
{
    return  moveToSync(mushroom_position.X(),mushroom_position.Y(),z,mushroom_position.A(),mushroom_position.B(),mushroom_position.C());
}

bool AAHeadModule::moveToSync(double x, double y, double z, double a, double b, double c)
{
    //motor_x->MoveToPos(x);
    //motor_y->MoveToPos(y);
    //motor_z->MoveToPos(z);
    motor_a->MoveToPos(a);
    motor_b->MoveToPos(b);
    motor_c->MoveToPos(c);
    //bool result = motor_x->WaitArrivedTargetPos(x);
    //result &= motor_y->WaitArrivedTargetPos(y);
    //result &= motor_z->WaitArrivedTargetPos(z);
    bool result = motor_a->WaitArrivedTargetPos(a);
    result &= motor_b->WaitArrivedTargetPos(b);
    result &= motor_c->WaitArrivedTargetPos(c);
    return result;
}
bool AAHeadModule::stepMove_XYC_ToSync(double x, double y,double c)
{
    x += motor_x->GetFeedbackPos();
    y += motor_y->GetFeedbackPos();
    c += motor_c->GetFeedbackPos();
    motor_x->MoveToPos(x);
    motor_y->MoveToPos(y);
    motor_c->MoveToPos(c);
    bool result = motor_x->WaitArrivedTargetPos(motor_x->GetFeedbackPos() + x);
    result &= motor_y->WaitArrivedTargetPos(y);
    result &= motor_c->WaitArrivedTargetPos(c);
    return result;
}

bool AAHeadModule::XYZAB_Interpolation(double x, double y, double z, double a, double b)
{
    static int curve_id = XtMotor::GetCurveResource();

    int dem = 5;

    int nPoint_Index = 0;
    int axis[] = {motor_x->AxisId(),motor_y->AxisId(),motor_z->AxisId(),motor_a->AxisId(),motor_b->AxisId()};
    int axis_combine[]={1,1,1,1,1};
    double axis_max_vel[] = {motor_x->GetMaxVel(),motor_y->GetMaxVel(),motor_z->GetMaxVel(),motor_a->GetMaxVel(),motor_b->GetMaxVel()};
    double axis_max_acc[] = {motor_x->GetMaxAcc(),motor_y->GetMaxAcc(),motor_z->GetMaxAcc(),motor_a->GetMaxAcc(),motor_b->GetMaxAcc()};
    double axis_max_jerk[] = {motor_x->GetMaxJerk(),motor_y->GetMaxJerk(),motor_z->GetMaxJerk(),motor_a->GetMaxJerk(),motor_b->GetMaxJerk()};

    double start[] = {motor_x->GetFeedbackPos(),motor_y->GetFeedbackPos(),motor_z->GetFeedbackPos(),motor_a->GetFeedbackPos(),motor_b->GetFeedbackPos()};
    double end[] = {x,y,z,a,b};
    motor_x->SetCurrentTragetPos(x);
    motor_y->SetCurrentTragetPos(y);
    motor_z->SetCurrentTragetPos(z);
    motor_a->SetCurrentTragetPos(a);
    motor_b->SetCurrentTragetPos(b);
    int res = XT_Controler_Extend::Set_Curve_Param(curve_id, 0.1, dem, axis, axis_max_vel, axis_max_acc, axis_max_jerk, axis_combine);
    if(1!=res)
    {
        qInfo("error in Set_Curve_Param");
        return false;
    }
    res = XT_Controler_Extend::Append_Line_Pos(curve_id, dem, axis, start, axis_max_vel[0], axis_max_vel[0], 0, nPoint_Index);
    if(1!=res)
    {
        qInfo("error in set start point");
        return false;
    }
    res = XT_Controler_Extend::Append_Line_Pos(curve_id, dem, axis, end, axis_max_vel[0], 0, 0, nPoint_Index);
    if(1!=res)
    {
        qInfo("error in set end point");
        return false;
    }
    res = XT_Controler_Extend::Exec_Curve(curve_id, motor_x->default_using_thread, motor_y->default_using_thread, 1);
    if(1!=res)
    {
        qInfo("path execute fail");
        return false;
    }
    int time = 3000;
    while(time>0)
    {
        if(XT_Controler_Extend::Check_Finish(curve_id)==1)
            return true;
        time-=10;
        Sleep(10);
    }
    qInfo("path execute timeout");
    return false;
}

QMap<QString, PropertyBase *> AAHeadModule::getModuleParameter()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("AA_HEAD_PARAMS", &parameters);
    temp_map.insert("AA_HEAD_POSITION", &this->mushroom_position);
    temp_map.insert("AA_PICK_LENS_POSITION", &this->pick_lens_position);
    return temp_map;
}

void AAHeadModule::setModuleParameter(QMap<QString, PropertyBase *> parameters)
{
}

void AAHeadModule::receivceModuleMessage(QVariantMap message)
{

}

void AAHeadModule::startWork(int run_mode)
{
}

void AAHeadModule::stopWork(bool wait_finish)
{
}

void AAHeadModule::resetLogic()
{
}

PropertyBase *AAHeadModule::getModuleState()
{
    return Q_NULLPTR;
}
