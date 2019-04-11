#ifndef AAHEADMODULE_H
#define AAHEADMODULE_H

#include "XtVacuum.h"
#include "position_define.h"
#include "xtmotor.h"

#include <QObject>
#include <propertybase.h>
#include <visionmodule.h>
#include <aaheadparameters.h>

class AAHeadModule : public QObject
{
    Q_OBJECT
public:
    AAHeadModule();
    void Init(QString name,XtMotor* motor_x,XtMotor* motor_y,XtMotor* motor_z,XtMotor* motor_a,XtMotor* motor_b,XtMotor* motor_c,XtVacuum * v);
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
    Position6D last_aa_position;
    void loadParams();
public:
    AAHeadParameters parameters;
    Position6D mushroom_position;
    double uplook_x;
    double uplook_y;
    double uplook_theta;
public:
    Q_INVOKABLE bool moveToPickLensPosition();
    Q_INVOKABLE bool moveToOCPosition();
    bool moveToAAPosition();
    void setUplookResult(double x,double y,double theta);
    bool moveToUplookResultPosition();
    Q_INVOKABLE bool stepMove_XY_Sync(double step_x,double step_y);
    Q_INVOKABLE bool stepMove_AB_Sync(double step_a,double step_b);
    bool stepMove_Z_Sync(double step_z);
    void SetAAPosion(mPoint6D point);
    void MotoAAPosition();
    double getZFeedback();
    double getZLogicPos();
private:
    bool moveToDiffrentZSync(double z);
    bool moveToSync(double x, double y, double z, double a, double b, double c);
    bool stepMove_XYC_ToSync(double x, double y,double c);

};

#endif // AAHEADMODULE_H
