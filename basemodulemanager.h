#ifndef BASEMODULEMANAGER_H
#define BASEMODULEMANAGER_H

#include <QObject>
#include "baslerpyloncamera.h"
#include "visionmodule.h"
#include "wordoplight.h"
#include "propertybase.h"
#include "LontryLight.h"
#include "xtmotor.h"
#include "XT_MotionControler_Client_Lib.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include "XtVacuum.h"
#include "XtCylinder.h"
#include "XtVcMotor.h"
#include "aaheadmodule.h"
#include "material_carrier.h"
#include "lut_module.h"
#include "sut_module.h"
#include "calibration.h"
#include "dothinkey.h"
#include "imagegrabbingworkerthread.h"

class BaseModuleManager : public PropertyBase
{
    Q_OBJECT
    int m_lightPanelLighting;

public:
    explicit BaseModuleManager(QObject *parent = nullptr);
    ~BaseModuleManager();

    Q_PROPERTY(int lightPanelLighting READ lightPanelLighting WRITE setLightPanelLighting NOTIFY lightPanelValueChanged)

    QMap<QString,XtMotor*> motors;
    QMap<QString,XtGeneralInput*> input_ios;
    QMap<QString,XtGeneralOutput*> output_ios;
    QMap<QString,Calibration*> calibrations;

    BaslerPylonCamera * pylonDownlookCamera = Q_NULLPTR;
    BaslerPylonCamera * pylonUplookCamera = Q_NULLPTR;
    VisionModule * visionModule = Q_NULLPTR;
    WordopLight * lightingModule = Q_NULLPTR;
    LontryLight * lightPanel = Q_NULLPTR;
    Dothinkey * dothinkey = Q_NULLPTR;
    ImageGrabbingWorkerThread * imageGrabberThread = Q_NULLPTR;

    AAHeadModule* aa_head_module = Q_NULLPTR;
    MaterialCarrier* lut_carrier = Q_NULLPTR;
    MaterialCarrier* sut_carrier = Q_NULLPTR;
    LutModule *lut_module = Q_NULLPTR;
    SutModule *sut_module = Q_NULLPTR;

    int lightPanelLighting() const
    {
        return m_lightPanelLighting;
    }

signals:
    void lightingValueChanged(int downlookLighting);

    void lightPanelValueChanged(int lightPanelLighting);

public slots:
    Q_INVOKABLE void updateParams()
    {
        if (aa_head_module) aa_head_module->updateParams();
        if (sut_module) sut_module->updateParams();
        if (lut_module) lut_module->updateParams();
        if (dothinkey) dothinkey->updateParams();
    }

    void setLightPanelLighting(int lightPanelLighting)
    {
        if (m_lightPanelLighting == lightPanelLighting)
            return;

        m_lightPanelLighting = lightPanelLighting;

        emit lightPanelValueChanged(m_lightPanelLighting);
    }
private:
    bool is_init;
    bool profile_loaded;
    static wchar_t ip[];
    static wchar_t profile_path[];

    VCM_Parameter_struct lut_vcm_parameters = {
        5000/*MaxVel*/,20000/*MaxAcc*/,200000/*MaxJerk*/,19/*MaxRange*/,0/*MinRange*/,9/*CanID*/,1/*dir*/,1024/*scale*/};
    VCM_Parameter_struct sut_vcm_parameters = {
        500/*MaxVel*/,10000/*MaxAcc*/,200000/*MaxJerk*/,33/*MaxRange*/,0/*MinRange*/,10/*CanID*/,1/*dir*/,5000/*scale*/};
private:
    bool InitStruct();
public:
    bool LoadProfile();
    bool ReadParameters();
    bool SaveParameters();

    Q_INVOKABLE bool initialDevice();
    Q_INVOKABLE bool stepMove(QString name, double step, bool isPositive);
    Q_INVOKABLE double getMotorFeedbackPos(QString name);
    Q_INVOKABLE bool initSensor();
    Q_INVOKABLE bool closeSensor();
    void EnableMotors();
    void DisableAllMotors();
    Q_INVOKABLE bool allMotorsSeekOrigin();
    Q_INVOKABLE void stopSeeking();

    Q_INVOKABLE void performUplookCalibration();
    Q_INVOKABLE void performDownlookCalibration();
    Q_INVOKABLE void performUpDownlookCalibration();
    Q_INVOKABLE void UpdateCalibrationParameters();


    XtMotor* GetMotorByName(QString name);
    XtVcMotor *GetVcMotorByName(QString name);
    XtGeneralOutput *GetOutputIoByName(QString name);
    XtGeneralInput *GetInputIoByName(QString name);
    XtVacuum *GetVacuumByName(QString name);
    XtCylinder *GetCylinderByName(QString name);
};

#endif // dBASEMODULEMANAGER_H
