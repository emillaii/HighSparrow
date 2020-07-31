#ifndef XTMOTOR_H
#define XTMOTOR_H
#include <windows.h>
#include <QObject>
#include <QDebug>
#include <QTime>
#include <QString>
#include "XtMotion/xtadcmodule.h"
#include "XtMotion/XtGeneralInput.h"
#include "XtMotion/XtGeneralOutput.h"
#include "Utils/propertybase.h"
#include "Utils/errorcode.h"
#include "verticallimitparameter.h"
#include "iolimitparameter.h"
#include "parallellimitparameter.h"
#include "XtMotion/xtmotorparameter.h"

class XtMotorExtendParameters : public PropertyBase
{
    Q_OBJECT
    double m_Delay;

public:
    XtMotorExtendParameters();

    Q_PROPERTY(double Delay READ Delay WRITE setDelay);
    double Delay() const;
public slots:
    void setDelay(double Delay);
};


class XtMotor:public ErrorBase
{
public:
static int GetCurveResource();
static int GetThreadResource();
public:
    XtMotor();
    virtual void Init(const QString& motor_name);
//    virtual void SetEncoderFeedback(int can_id, int data_ch, double ratio);
    virtual void SetFeedbackZero(double new_value = 0);

    virtual QString Name() const;
    virtual void Enable();
    virtual void Disable();

//these are running state
    virtual double GetOutpuPos() const;
    virtual double GetFeedbackPos(int decimal_digit = 4) const;
    double GetCurrentTragetPos();
//    virtual double GetCurVel() const;
//    virtual double GetCurAcc() const;
//    virtual double GetCurADC() const;
//    virtual bool IsRunning() const;
    virtual bool checkAlarm();
    virtual bool getAlarmState();
//these are run parameter
//    virtual double GetPostiveRange() const;
//    virtual double GetNegativeRange() const;
    virtual int AxisId() const;
    virtual double GetMaxVel() const;
    virtual double GetMaxAcc() const;
    virtual double GetMaxJerk() const;


    virtual void SetVel(double vel,int thread = -1);
    virtual void SetAcc(double vel, int thread = -1);
    virtual void SetJerk(double jerk,int thread = -1);
//    virtual void SetPostiveRange(double range);
//    virtual void SetNegativeRange(double range);
//    virtual void Home(int thread = -1);

    virtual bool MoveToPos(double pos,int thread = -1);
//    virtual bool WaitMoveStop(int timeout = 30000);
    bool WaitArrivedTargetPos(double target_position,int timeout=10000);
    virtual bool MoveToPosSync(double pos,int thread = -1,int time_out = 30000);
    virtual bool SlowMoveToPos(double pos, double low_vel, int thread = -1);
    virtual bool SlowMoveToPosSync(double pos, double low_vel, int thread = -1);
    virtual bool StepMove(double step,int thread = -1);
    virtual bool StepMoveSync(double step, int thread = -1);
    virtual bool StepMoveSync(double step, bool dir, int thread = -1);

    virtual bool SeekOrigin(int thread = -1);
    bool StopSeeking(int thread = -1);
    virtual bool WaitSeekDone(int thread = -1, int timeout = 50000);
//    virtual bool WaitStop(int timeout = 10000);

//    bool IsInsideRange(double target_pos,double error = 0.01);
    int default_using_thread;

    XtMotorExtendParameters extend_parameters;

    
    virtual void GetMasterAxisID();
public:
    XtMotorParameter parameters;
    XtMotorState states;
protected:
    bool is_init;
    static int curve_resource;
    static int thread_resource;

    QString name = "";
    int axis_id;
    int axis_sub_id;


    bool is_enable;

    int out_en_id;
    int out_clr_id;

    int in_inp_id;
    int in_alm_id;
    int in_rdy_id;
    int in_origin_id;

    double max_vel;
    double max_acc;
    double max_jerk;
    double max_range;
    double min_range;

    int encoder_id;
    double encoder_ratio;
    double current_target;

    XtADCModule *adc;

    XtGeneralOutput en;
    XtGeneralOutput clr;

    XtGeneralInput inp;
    XtGeneralInput alm;
    XtGeneralInput rdy;
    XtGeneralInput origin;
    XtGeneralInput origin2;

    void ChangeCurPos(double pos);
    bool checkState(bool check_seeked_origin = true);
    bool checkLimit(const double pos);
    bool checkInterface(const double pos);
public:
    static int axis_id_resource;
    QList<VerticalLimitParameter*> vertical_limit_parameters;
    QList<ParallelLimitParameter*> parallel_limit_parameters;
    QList<XtMotor*> vertical_limit_motors;
    QList<XtMotor*> parallel_limit_motors;
    QList<IOLimitParameter*> io_limit_parameters;
    QList<XtGeneralInput*> limit_ios;
};

#endif // XTMOTER_H
