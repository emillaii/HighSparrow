#ifndef XTMOTION_H
#define XTMOTION_H
#include <QVector>
#include <QHash>
#include "xtmotor.h"
#include "XtVcMotor.h"
#include "VCM_init_Struct.h"
#include "XtGeneralInput.h"
#include "XtGeneralOutput.h"
#include "XtVacuum.h"
#include <QString>
#include "sixaxisposition.h"
#include "XtCylinder.h"
#include <QJsonObject>

class XtMotion
{
public:
    static wchar_t ip[];
    static wchar_t profile_path[];


    static bool LoadProfile();
    static bool Init();
    static bool IsInit();
    static bool IsHomeFinished();
    static bool AllMotorsSeekOrigin();
    static bool WaitAllMotorsTillStop(int timeout = 30000);
    static int GetCurveResource();
    static int GetThreadResource();
    static int GetAxisIdResource();

    static double sut_safty_z_pos;
    static bool Sut_Z_XY_Z_MoveSync(double x, double y, double z, int timeout = 30000);
    static bool Sut_Z0_Y0_X_YZ_MoveSync(double x, double y, double z, int timeout = 30000);
    static bool Sut_Z_XYZ_MoveSync(double x, double y, double z, int timeout = 30000);
    static bool AA_RollPitch_MoveSync(double roll,double pitch, int timeout = 30000);
    static bool FeedLensToAAHead(double speed, double start_pos, double targetPos = -1);
    static bool GetLensFromAAHead(double speed, double start_pos, double targetPos = -1);
    static void SetFeedLensTarget(double value);
    ///
    /// \brief GetMotor
    /// \param name
    /// \return
    ///
    static XtMotor *GetMotor(QString name);
    static void EnableAllMotors();

    static XtMotor motor_aa_x;
    static XtMotor motor_aa_y;
    static XtMotor motor_aa_z;
    ///
    /// \brief 這是繞Y軸旋轉的 this is Rotate around the Y axis
    ///
    static XtMotor motor_aa_roll;
    ///
    /// \brief 這是繞Z軸旋轉的 this is Rotate around the Z axis
    ///
    static XtMotor motor_aa_yaw;
    ///
    /// \brief 這是繞X軸旋轉的 this is Rotate around the X axis
    ///
    static XtMotor motor_aa_pitch;
    static XtMotor motor_sut_x;
    static XtMotor motor_sut_y;
    //static XtMotor motor_sut_z;
    static XtVcMotor motor_sut_z;
    static XtMotor motor_pa_x;
    static XtMotor motor_pa_y;
    static XtMotor motor_pa_z;
    static XtMotor motor_pa_theta1;
    static XtMotor motor_pa_theta2;
    static XtVcMotor motor_pa_vcm1;
    static XtVcMotor motor_pa_vcm2;
    static XtMotor motor_tray_x;

    static QJsonObject motor_extend_param;

    static XtGeneralInput xtin_gel_up_sensor;
    static XtGeneralInput xtin_gel_down_sensor;
    static XtGeneralInput xtin_gel_contact_sensor;
    static XtGeneralInput xtin_tray_ent;
    static XtGeneralInput xtin_tray_exit;
    static XtGeneralInput xtin_tray_in;
    static XtGeneralInput xtin_tray_out;
    static XtGeneralInput xtin_tray_s;
    static XtGeneralInput xtin_cmos_vacuum_s;
    static XtGeneralInput xtin_lens_vacuum_s;
    static XtGeneralInput xtin_pick_cmos_s;
    static XtGeneralInput xtin_pick_lens_s;
    static XtGeneralInput xtin_tray_ban_unban;
    static XtGeneralInput xtin_tray_ban_ban;
    static XtGeneralInput xtin_tray_lock1_up;
    static XtGeneralInput xtin_tray_lock1_down;
    static XtGeneralInput xtin_tray_lock2_up;
    static XtGeneralInput xtin_tray_lock2_down;
    static XtGeneralInput xtin_tray_up_up;
    static XtGeneralInput xtin_tray_up_down;

    static XtGeneralOutput xtout_gel_up;
    static XtGeneralOutput xtout_gel_down;
    static XtGeneralOutput xtout_cmos_vacuum;
    static XtGeneralOutput xtout_lens_vacuum;
    static XtGeneralOutput xtout_pogopin;
    static XtGeneralOutput xtout_dispense;
    static XtGeneralOutput xtout_gripper;
    static XtGeneralOutput xtout_pick_cmos;
    static XtGeneralOutput xtout_pick_lens;
    static XtGeneralOutput xtout_pick_cmos_r;
    static XtGeneralOutput xtout_pick_lens_r;
    static XtGeneralOutput xtout_uv_1;
    static XtGeneralOutput xtout_uv_2;
    static XtGeneralOutput xtout_uv_3;
    static XtGeneralOutput xtout_uv_4;
    static XtGeneralOutput xtout_tray_ban;
    static XtGeneralOutput xtout_tray_lock1;
    static XtGeneralOutput xtout_tray_lock2;
    static XtGeneralOutput xtout_tray_up;

    static XtVacuum xtv_pick_cmos;
    static XtVacuum xtv_pick_lens;
    static XtVacuum xtv_cmos_hold;
    static XtVacuum xtv_lens_hold;

    static XtCylinder xtc_pogopin;
    static XtCylinder xtc_tray_ban;
    static XtCylinder xtc_tray_lock1;
    static XtCylinder xtc_tray_lock2;
    static XtCylinder xtc_tray_up;

    static QVector<XtMotor *>AllMotors;
    static QVector<XtGeneralInput *> AllInputs;
    static int in_num_in_profile;
    static QVector<XtGeneralOutput *> AllOutputs;

//    static PickArm pick_arm;
//    static TrayLoader tray_loader;
//    static AAHeadControler aa_head;

//    static QVector<SixAxisPosition *> AAHeadPosModify;
    static bool LoadPosModifyData(QString path = "./AAHeadPosModifyData.json");
    static void CleanPosModifyData();
    static bool SavePosModifyData(QString path = "./AAHeadPosModifyData.json");

    static void LoadMotorExtendParams(QString path = "config//XtMotorExtendParams.json");
    static void SaveMotorExtendParams(QString path = "config//XtMotorExtendParams.json");

private:

    static void AddMotors();
    static void AddIOs();

    static int curve_resource;
    static int thread_resource;
    static int axis_id_resource;

    static bool profile_loaded;
    static bool is_init;
    static bool is_home;

    static VCM_Parameter_struct pa_vcm1_parameters;
    static VCM_Parameter_struct pa_vcm2_parameters;
    static VCM_Parameter_struct sut_vcm_parameters;

    static double feed_lens_target;

};

#endif // XTMOTION_H
