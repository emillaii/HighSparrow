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


class XtMotor
{
public:
//static void Move2AxisToPos(XtMotor* axis1,  double pos1, XtMotor *axis2, double pos2, int thread = -1);
static void WaitSync(int thread);

public:
    XtMotor();
    virtual void Init(const QString& motor_name);
    virtual void SetADC(int can_id,int data_ch);
    virtual void SetEncoderFeedback(int can_id, int data_ch, double ratio);
    virtual void SetFeedbackZero(double new_value = 0);

    virtual QString Name() const;
    virtual void Enable();
    virtual void Disable();

//these are running state
    virtual double GetOutpuPos() const;
    virtual double GetFeedbackPos() const;
    virtual double GetCurVel() const;
    virtual double GetCurAcc() const;
    virtual double GetCurADC() const;
    virtual bool IsRunning() const;
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

    void SGO(double pos, int thread = -1);
    void TILLSTOP(int thread = -1);
    void TILLTIME(int ms, int thread = -1);
    virtual void MoveToPos(double pos,int thread = -1);
    virtual void SlowMoveToPos(double pos,double vel_ratio = 0.2, int thread = -1);
    virtual bool WaitMoveStop(int timeout=30000);
    bool WaitArrivedTargetPos(double target_position,int timeout=30000);
    virtual bool MoveToPosSync(double pos,int thread = -1);
    virtual bool SlowMoveToPosSync(double pos,double vel_ratio =0.2,int thread = -1);
    virtual void StepMove(double step,int thread = -1);
    virtual bool StepMoveSync(double step, int thread = -1);
    virtual bool StepMoveSync(double step, bool dir, int thread = -1);

    //void SeekOrigin(int originDirection, double originRange, double originOffset);
    virtual void SeekOrigin(int thread = -1);
    void StopSeeking(int thread = -1);
    virtual bool WaitSeekDone(int thread = -1, int timeout = 20000);
    virtual bool WaitStop(int timeout = 10000);

    virtual bool SearchPosByADC(double vel, double search_limit, double threshold, bool search_above, double &result);
    virtual bool SearchPosByIO(double vel, double search_limit, bool search_rise, int io_id, double &result);
    bool IsInsideRange(double target_pos,double error = 0.01);
    int default_using_thread;

    XtMotorExtendParameters extend_parameters;

    void SetAxisDelay();
    void ClearAxisDelay();
    
    virtual void GetMasterAxisID();

protected:
    bool is_init;

    static int curve_resource;
    static int thread_resource;
    static int axis_id_resource;

    QString name;
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

    XtADCModule *adc;

    XtGeneralOutput en;
    XtGeneralOutput clr;

    XtGeneralInput inp;
    XtGeneralInput alm;
    XtGeneralInput rdy;
    XtGeneralInput origin;

    void ChangeCurPos(double pos);
    void CheckLimit(double &pos);
};

#endif // XTMOTER_H
