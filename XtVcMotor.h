﻿#ifndef XTVCMOTOR_H
#define XTVCMOTOR_H
#include <windows.h>
#include <QObject>
#include <QTime>
#include <QString>
#include <qvector.h>
#include "xtmotor.h"
#include "VCM_init_Struct.h"

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
    void ConfigSUT_Z_VCM();
    void Init(const QString& motor_name) override;
    void Init(const QString& motor_name,VCM_Parameter_struct parameters);
    void SetADC(int can_id,int data_ch) override;
    void SetEncoderFeedback(int can_id, int data_ch, double ratio) override;
    void SetFeedbackZero(double new_value = 0) override;

    void Enable() override;
    void Disable() override;

    //these are running state
    double GetOutpuPos() const override;
    double GetFeedbackPos() const override;
    double GetCurADC() const override;
    bool IsRunning() const override;
    //these are run parameter
    double GetPostiveRange() const override;
    double GetNegativeRange() const override;
    int AxisId() const override;
    double GetMaxVel() const override;
    double GetMaxAcc() const override;
    double GetMaxJerk() const override;


    void SetVel(double vel,int thread = -1) override;
    void SetAcc(double vel,int thread = -1);
    void SetJerk(double jerk,int thread = -1) override;
    void SetPostiveRange(double range) override;
    void SetNegativeRange(double range) override;
    void Home(int thread = -1) override;

//    void SGO(double pos, int thread) override;
//    void TILLSTOP(int thread = -1) override;
//    void MoveToPos(double pos,int thread = -1) override;
//    bool WaitMoveStop(int timeout=30000) override;
//    bool MoveToPosSync(double pos,int thread = -1) override;

    //void SeekOrigin(int originDirection, double originRange, double originOffset) override;
    void SeekOrigin(int thread = -1) override;

    bool WaitSeekDone(int thread = -1, int timeout = 10000) override;

    void GetMasterAxisID() override;

    bool SearchPosByADC(double vel, double search_limit, double threshold, bool search_above, double &result) override;
    bool SearchPosByForce(double slow_speed, double  search_limit, double force, double margin, double &result);
    bool SearchPosByForce(double &result, double force, double  search_limit = -1);
    void RestoreForce();
    void ShowSetting();
    void SetSoftLanding(double slow_speed, double slow_acc, double force, double start_pos, double target_pos, double margin);
    bool DoSoftLanding();
    bool DoSoftLandingReturn();
    bool WaitSoftLandingDone(int timeout = 30000);

    static QVector<VCM_Resource_struct> all_parameter;
    static void InitAllVCM();

private:
    VCM_Resource_struct vcm_resource;
    VCM_Parameter_struct parameters;
    bool direction_is_opposite;
    bool origin_result = true;

    int vcm_id;
    static int vcm_count;
};

#endif // XTVCMOTOR_H
