#ifndef AAHEADMODULE_H
#define AAHEADMODULE_H

#include "XtMotion/XtCylinder.h"
#include "XtMotion/XtVacuum.h"
#include "material_carrier.h"
#include "position_define.h"
#include "XtMotion/xtmotor.h"


#include <QObject>
#include <propertybase.h>
#include <visionmodule.h>
#include <aaheadparameters.h>

//#define AA_Z_OFFSET (31.5)

class AAHeadModule : public QObject,public ErrorBase
{
    Q_OBJECT
public:
    AAHeadModule();
    void Init(QString name,XtMotor* motor_x,XtMotor* motor_y,XtMotor* motor_z,XtMotor* motor_a,XtMotor* motor_b,XtMotor* motor_c,XtGeneralOutput * gripper,
              XtGeneralOutput * uv1,
              XtGeneralOutput * uv2,
              XtGeneralOutput * uv3,
              XtGeneralOutput * uv4,
              int thread_id,
              MaterialCarrier* sut_carrier);
    void saveJsonConfig(QString file_name);
    void loadJsonConfig(QString file_name);
    Q_INVOKABLE bool moveToMushroomPosition(bool moveXYC = true);
    Q_INVOKABLE bool moveToPickLensPosition();
    Q_INVOKABLE bool moveToOCPosition();
    bool moveToAAPosition();
    void setUplookResult(double x,double y,double theta);
    bool moveToUplookResultPosition();
    void openUVTillTime(int till_time);
    void openGripper();
    void closeGripper();
    Q_INVOKABLE bool stepMove_XY_Sync(double step_x,double step_y);
    Q_INVOKABLE bool stepMove_XYC_Sync(double step_x,double step_y,double step_c);
    Q_INVOKABLE bool stepMove_AB_Sync(double step_a,double step_b);
    Q_INVOKABLE bool stepInterpolation_AB_Sync(double step_a,double step_b);
    bool stepMove_Z_Sync(double step_z);
    void SetAAPosion(mPoint6D point);
    void MotoAAPosition();
    mPoint6D GetFeedBack();
    void sendSensrRequest(int sut_state);
    bool waitForLoadSensor(bool &is_run,int time_out = 60000);
    bool moveToSync(double x, double y, double z,double c);
signals:
    void sendSensrRequestToSut(int sut_state);
public slots:
    void receiveLensFromLut(double offset_x,double offset_y,double offset_theta);
    void receiveSensorFromSut(double offset_x,double offset_y,double offset_theta);
private:
    bool moveToDiffrentZSync(double z);
    bool moveToSync(double x, double y, double z, double a, double b, double c);
    bool stepMove_XYC_ToSync(double x, double y,double c);
    bool XYZAB_Interpolation(double x, double y, double z, double a, double b);

public:
    AAHeadParameters parameters;
    Position6D mushroom_position;
    bool waiting_lens;
    double uplook_x;
    double uplook_y;
    double uplook_theta;
    bool waiting_sensor;
    double offset_x;
    double offset_y;
    double offset_theta;
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
    Position6D last_aa_position;
    int thread_id;
};

#endif // AAHEADMODULE_H
