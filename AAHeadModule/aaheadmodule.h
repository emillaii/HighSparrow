#ifndef AAHEADMODULE_H
#define AAHEADMODULE_H

#include "XtMotion/XtCylinder.h"
#include "XtMotion/XtVacuum.h"
#include "material_carrier.h"
#include "Utils/position_define.h"
#include "XtMotion/xtmotor.h"


#include <QObject>
#include "Utils/propertybase.h"
#include "Vision/visionmodule.h"
#include <AAHeadModule/aaheadparameters.h>

//#define AA_Z_OFFSET (31.5)

class AAHeadModule : public QObject,public ErrorBase
{
    Q_OBJECT
public:
    AAHeadModule();
    void Init(XtMotor* motor_a,XtMotor* motor_b,XtMotor* motor_c,XtGeneralOutput * gripper,
              XtGeneralOutput * uv1,
              XtGeneralOutput * uv2,
              XtGeneralOutput * uv3,
              XtGeneralOutput * uv4,
              XtMotor* motor_aax, XtMotor* motor_aay, XtMotor* motor_aaz,
              int thread_id,
              MaterialCarrier* sut_carrier
              );
    void saveJsonConfig(QString file_name);
    void loadJsonConfig(QString file_name);
    Q_INVOKABLE bool moveToMushroomPosition(bool moveXYC = true);
    void openUVTillTime(int till_time);
    void openGripper();
    void closeGripper();
    bool moveToMushroomPosWithCOffset(double aOffset);
    bool moveToPickLensPositionSync();
    Q_INVOKABLE bool moveToPikLensPositionAsync();
    bool waitArrivedPickLesPosition();
    Q_INVOKABLE bool stepInterpolation_AB_Sync(double step_a,double step_b);
    mPoint6D GetFeedBack();

    //huawei aa
    Q_INVOKABLE bool moveAAHead_XYZToPos();
private:
    bool moveToDiffrentZSync(double z);
    bool moveToSync(double x, double y, double z, double a, double b, double c);
    bool XYZAB_Interpolation(double x, double y, double z, double a, double b);

public:
    AAHeadParameters parameters;
    Position6D mushroom_position;
    Position6D aaPickLensPosition;
    Position6D aaHeadXYZPosition;
    PositionT bondOffset;
    bool waiting_lens;
    double uplook_x;
    double uplook_y;
    double uplook_theta;
    bool waiting_sensor;
private:
    XtMotor* motor_x = Q_NULLPTR;
    XtMotor* motor_y = Q_NULLPTR;
    XtMotor* motor_z = Q_NULLPTR;
    XtMotor* motor_a = Q_NULLPTR;
    XtMotor* motor_b = Q_NULLPTR;
    XtMotor* motor_c = Q_NULLPTR;
    XtGeneralOutput * gripper = Q_NULLPTR;
    XtGeneralOutput * uv1 = Q_NULLPTR;
    XtGeneralOutput * uv2 = Q_NULLPTR;
    XtGeneralOutput * uv3 = Q_NULLPTR;
    XtGeneralOutput * uv4 = Q_NULLPTR;
    int thread_id;
    //huawei aa
    XtMotor* motor_aax = Q_NULLPTR;
    XtMotor* motor_aay = Q_NULLPTR;
    XtMotor* motor_aaz = Q_NULLPTR;
};

#endif // AAHEADMODULE_H
