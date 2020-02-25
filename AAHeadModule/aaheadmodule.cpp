#include "AAHeadModule/aaheadmodule.h"
#include "Utils/config.h"
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
    temp_map.insert("AA_PICK_LENS_POSITION", &this->aaPickLensPosition);
    temp_map.insert("BOND_OFFSET", &this->bondOffset);
    temp_map.insert("AA_HEAD_XYZ_POSITION",&this->aaHeadXYZPosition);//huawei aa
    PropertyBase::loadJsonConfig(file_name, temp_map);
}

void AAHeadModule::saveJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("AA_HEAD_PARAMS", &this->parameters);
    temp_map.insert("AA_HEAD_POSITION", &this->mushroom_position);
    temp_map.insert("AA_PICK_LENS_POSITION", &this->aaPickLensPosition);
    temp_map.insert("BOND_OFFSET", &this->bondOffset);
    temp_map.insert("AA_HEAD_XYZ_POSITION",&this->aaHeadXYZPosition);//huawei aa
    PropertyBase::saveJsonConfig(file_name,temp_map);
}

void AAHeadModule::Init(XtMotor *motor_a, XtMotor *motor_b, XtMotor *motor_c, XtGeneralOutput *gripper,
                        XtGeneralOutput * uv1,
                        XtGeneralOutput * uv2,
                        XtGeneralOutput * uv3,
                        XtGeneralOutput * uv4,
                        XtMotor* motor_aax,
                        XtMotor* motor_aay,
                        XtMotor* motor_aaz,
                        int thread_id,
                        MaterialCarrier* sut_carrier
                        )
{
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
    this->motor_aax = motor_aax;
    this->motor_aay = motor_aay;
    this->motor_aaz = motor_aaz;

    this->thread_id = thread_id;
    setName(parameters.moduleName());
}

bool AAHeadModule::moveToMushroomPosition(bool moveXYC)
{
    if (moveXYC)
        return moveToDiffrentZSync(mushroom_position.Z());
    else {
        mPoint6D point = this->GetFeedBack();
        moveToSync(point.X,point.Y,mushroom_position.Z(),mushroom_position.A(),mushroom_position.B(),point.C);
    }
}


void AAHeadModule::openUVTillTime(int till_time)
{
    int mini_time = 100;
    XT_Controler::SET_OUTPUT_IO(thread_id,uv1->GetID(),1);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv2->GetID(),1);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv3->GetID(),1);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv4->GetID(),1);
    QThread::msleep(mini_time);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv1->GetID(),0);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv2->GetID(),0);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv3->GetID(),0);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv4->GetID(),0);
    QThread::msleep(till_time);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv1->GetID(),1);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv2->GetID(),1);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv3->GetID(),1);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv4->GetID(),1);
    QThread::msleep(mini_time);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv1->GetID(),0);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv2->GetID(),0);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv3->GetID(),0);
    XT_Controler::SET_OUTPUT_IO(thread_id,uv4->GetID(),0);
}

void AAHeadModule::openGripper()
{
    qInfo("open gripper is called");
    gripper->Set(false);
    Sleep(250);
}

void AAHeadModule::closeGripper()
{
    qInfo("close gripper is called");
    gripper->Set(true);
    Sleep(250);
}

bool AAHeadModule::moveToMushroomPosWithCOffset(double cOffset)
{
    return moveToSync(mushroom_position.X(),mushroom_position.Y(),mushroom_position.Z(),
                      mushroom_position.A(), mushroom_position.B(),mushroom_position.C() + cOffset);
}

bool AAHeadModule::moveToPickLensPositionSync()
{
    return moveToPikLensPositionAsync() & waitArrivedPickLesPosition();
}

bool AAHeadModule::moveToPikLensPositionAsync()
{
    return motor_a->MoveToPos(aaPickLensPosition.A()) &
            motor_b->MoveToPos(aaPickLensPosition.B())&
            motor_c->MoveToPos(aaPickLensPosition.C());
}

bool AAHeadModule::waitArrivedPickLesPosition()
{
    return motor_a->WaitArrivedTargetPos(aaPickLensPosition.A()) &
            motor_b->WaitArrivedTargetPos(aaPickLensPosition.B())&
            motor_c->WaitArrivedTargetPos(aaPickLensPosition.C());
}

bool AAHeadModule::stepInterpolation_AB_Sync(double step_a, double step_b)
{
    double dy = qSin(qDegreesToRadians(step_a))*parameters.rotateZOffset();
    double new_z = qCos(qDegreesToRadians(step_a))*parameters.rotateZOffset();
    double dx = qSin(qDegreesToRadians(step_b))*new_z;
    new_z = qCos(qDegreesToRadians(step_b))*new_z;
    double dz = new_z - parameters.rotateZOffset();
    qInfo("motor x : %s y: %s z: %s", motor_x->Name().toStdString().c_str(), motor_y->Name().toStdString().c_str(), motor_z->Name().toStdString().c_str());
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
    qInfo("Rotate Z Offset: %f", parameters.rotateZOffset());
    qInfo("x: %f y: %f z:%f a:%f b:%f step_a: %f step_b: %f dx: %f  dy: %f dz: %f", x, y, z, a, b, step_a, step_b, dx, dy, dz);
    return XYZAB_Interpolation(x,y,z,a,b);
}

mPoint6D AAHeadModule::GetFeedBack()
{
    return mPoint6D(motor_x->GetFeedbackPos(),motor_y->GetFeedbackPos(),motor_z->GetFeedbackPos(),motor_a->GetFeedbackPos(),motor_b->GetFeedbackPos(),motor_c->GetFeedbackPos());
}

bool AAHeadModule::moveAAHead_XYZToPos()//huawei aa
{
    motor_aax->MoveToPos(aaHeadXYZPosition.X());
    motor_aay->MoveToPos(aaHeadXYZPosition.Y());
    motor_aaz->MoveToPos(aaHeadXYZPosition.Z());
    bool result = motor_aax->WaitArrivedTargetPos(aaHeadXYZPosition.X());
    result &= motor_aay->WaitArrivedTargetPos(aaHeadXYZPosition.Y());
    result &= motor_aaz->WaitArrivedTargetPos(aaHeadXYZPosition.Z());
    return result;

}

bool AAHeadModule::moveToDiffrentZSync(double z)
{
    return  moveToSync(mushroom_position.X(),mushroom_position.Y(),z,mushroom_position.A(),mushroom_position.B(),mushroom_position.C());
}

bool AAHeadModule::moveToSync(double x, double y, double z, double a, double b, double c)
{
    motor_a->MoveToPos(a);
    motor_b->MoveToPos(b);
    motor_c->MoveToPos(c);
    bool result = motor_a->WaitArrivedTargetPos(a);
    result &= motor_b->WaitArrivedTargetPos(b);
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
    qInfo("motor _crashed test");
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

