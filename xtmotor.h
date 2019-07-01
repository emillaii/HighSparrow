#ifndef XTMOTOR_H
#define XTMOTOR_H
#include <windows.h>
#include <QObject>
#include <QTime>
#include <QString>
#include "xtadcmodule.h"
#include "XtGeneralInput.h"
#include "XtGeneralOutput.h"
#include "PropertyBase.h"
#include "errorcode.h"
#include "verticallimitparameter.h"
#include "iolimitparameter.h"
#include "parallellimitparameter.h"
#include "xtmotorparameter.h"
#include "motorstatesgeter.h"

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
//static void Move2AxisToPos(XtMotor* axis1,  double pos1, XtMotor *axis2, double pos2, int thread = -1);
static void WaitSync(int thread);
static int GetCurveResource();
static int GetThreadResource();
public:
    XtMotor();
    virtual void Init(const QString& motor_name);
    void Init(MotorStatesGeter* geter);
    virtual void SetADC(int can_id,int data_ch);
    virtual void SetEncoderFeedback(int can_id, int data_ch, double ratio);
    virtual void SetFeedbackZero(double new_value = 0);

    virtual QString Name() const;
    virtual void Enable();
    virtual void Disable();

//these are running state
    virtual double GetOutpuPos() const;
    virtual double GetFeedbackPos(int decimal_digit = 4) const;
    double GetCurrentTragetPos();
    virtual double GetCurVel() const;
    virtual double GetCurAcc() const;
    virtual double GetCurADC() const;
    virtual bool IsRunning() const;
    virtual bool checkAlarm();
    virtual bool getAlarmState();
//these are run parameter
    virtual double GetPostiveRange() const;
    virtual double GetNegativeRange() const;
    virtual int AxisId() const;
    virtual double GetMaxVel() const;
    virtual double GetMaxAcc() const;
    virtual double GetMaxJerk() const;


    virtual void SetVel(double vel,int thread = -1);
    virtual void SetAcc(double vel, int thread = -1);
    virtual void SetJerk(double jerk,int thread = -1);
    virtual void SetPostiveRange(double range);
    virtual void SetNegativeRange(double range);
    virtual void Home(int thread = -1);

    bool SGO(double pos, int thread = -1);
    void TILLSTOP(int thread = -1);
    void TILLTIME(int ms, int thread = -1);
    virtual bool MoveToPos(double pos,int thread = -1);
    bool stepMoveToPos(double step);
    virtual bool SlowMoveToPos(double pos,double low_vel, int thread = -1);
    virtual bool WaitMoveStop(int timeout = 30000);
    bool WaitArrivedTargetPos(double target_position,int timeout=10000);
    bool WaitArrivedTargetPos(int timeout = 10000);
    virtual bool MoveToPosSync(double pos,int thread = -1,int time_out = 30000);
    bool MoveToPosSafty(double pos,int thread = -1);
    bool CheckArrivedTargetPos(double target_position);
    bool CheckArrivedTargetPos();
    bool MoveToPosSaftySync(double pos,int thread = -1,int timeout = 10000);
    virtual bool SlowMoveToPosSync(double pos,double low_vel,int thread = -1);
    virtual bool StepMove(double step,int thread = -1);
    virtual bool StepMoveSync(double step, int thread = -1);
    virtual bool StepMoveSync(double step, bool dir, int thread = -1);

    //void SeekOrigin(int originDirection, double originRange, double originOffset);
    virtual bool SeekOrigin(int thread = -1);
    bool StopSeeking(int thread = -1);
    virtual bool WaitSeekDone(int thread = -1, int timeout = 100000);
    virtual bool WaitStop(int timeout = 10000);

    virtual bool SearchPosByADC(double vel, double search_limit, double threshold, bool search_above, double &result);
    virtual bool SearchPosByIO(double vel, double search_limit, bool search_rise, int io_id, double &result);
    bool IsInsideRange(double target_pos,double error = 0.01);
    int default_using_thread;

    XtMotorExtendParameters extend_parameters;

    void SetAxisDelay();
    void ClearAxisDelay();
    
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
    MotorStatesGeter* geter;

    void ChangeCurPos(double pos);
//    void CheckLimit(double &pos);
    bool checkState(bool check_seeked_origin = true);
    bool checkLimit(const double pos);
    bool getInterfaceLimit(double target_pos,double& result_pos);
    bool checkInterface(const double pos);
public:
    static int axis_id_resource;
    QList<VerticalLimitParameter*> vertical_limit_parameters;
    QList<ParallelLimitParameter*> parallel_limit_parameters;
    QList<XtMotor*> vertical_limit_motors;
    QList<XtMotor*> parallel_limit_motors;
    QList<IOLimitParameter*> io_limit_parameters;
    QList<XtGeneralInput*> limit_in_ios;
    QList<XtGeneralOutput*> limit_out_ios;
};

#endif // XTMOTER_H
