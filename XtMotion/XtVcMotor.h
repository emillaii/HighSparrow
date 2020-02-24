#ifndef XTVCMOTOR_H
#define XTVCMOTOR_H
#include <windows.h>
#include <QObject>
#include <QTime>
#include <QString>
#include <qvector.h>
#include "XtMotion/xtmotor.h"
#include "VCM_init_Struct.h"
#include "XtMotion/xtvcmotorparameter.h"

struct VCM_Parameter_struct
{
    double MaxVel;
    double MaxAcc;
    double MaxJerk;
    double MaxPos;
    double MinPos;
    int CanID;
    int direction;
    double scale;
};

Q_DECLARE_TYPEINFO(VCM_Resource_struct, Q_PRIMITIVE_TYPE);

class XtVcMotor : public XtMotor
{
public:
    XtVcMotor();
    void ConfigVCM();
    void ChangeDiretion(bool befor_seek = false);
//    void ConfigSUT_Z_VCM();
//    void Init(const QString& motor_name) override;
    void Init();
//    void SetEncoderFeedback(int can_id, int data_ch, double ratio) override;
    void SetFeedbackZero(double new_value = 0) override;

    void Enable() override;
    void Disable() override;

    //these are running state
    double GetOutpuPos() const override;
    double GetFeedbackPos(int decimal_digit = 4) const override;
    bool getAlarmState() override;
    //these are run parameter
//    double GetPostiveRange() const override;
//    double GetNegativeRange() const override;
    int AxisId() const override;
    double GetMaxVel() const override;
    double GetMaxAcc() const override;
    double GetMaxJerk() const override;


    void SetVel(double vel,int thread = -1) override;
    void SetAcc(double vel,int thread = -1) override;
    void SetJerk(double jerk,int thread = -1) override;
//    void SetPostiveRange(double range) override;
//    void SetNegativeRange(double range) override;
//    void Home(int thread = -1) override;


    bool SeekOrigin(int thread = -1) override;

    bool WaitSeekDone(int thread = -1, int timeout = 10000) override;

    void GetMasterAxisID() override;

    bool SearchPosByForce(const double speed,const double force,const double  limit,const double margin,const int timeout = 30000);
    bool SearchPosByForce(const double speed,const double force,const int timeout = 30000);
    void RestoreForce();
    void ShowSetting();
    void SetSoftLanding(double slow_speed, double slow_acc, double force, double start_pos, double target_pos, double margin);
    bool DoSoftLanding();
    bool DoSoftLandingReturn();
    bool resetSoftLanding(int timeout = 30000);
    bool WaitSoftLandingDone(int timeout = 30000);

    static QVector<VCM_Resource_struct> all_parameter;
    static void InitAllVCM();
public:
    VcMotorParameter parameters;
private:
    bool direction_is_opposite;

    int vcm_id;
    bool is_softlanding = false;
    bool is_softlanded = false;
    bool is_returning = false;
    LONG64 error_code;
public:
    VCM_Resource_struct vcm_resource;
    static int vcm_count;
};

#endif // XTVCMOTOR_H
