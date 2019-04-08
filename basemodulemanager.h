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

class BaseModuleManager : public PropertyBase
{
    Q_OBJECT
    int m_downlookLighting;

    int m_uplookLighting;

    int m_lightPanelLighting;

public:
    explicit BaseModuleManager(QObject *parent = nullptr);
    ~BaseModuleManager();

    Q_PROPERTY(int downlookLighting READ downlookLighting WRITE setDownlookLighting NOTIFY lightingValueChanged)
    Q_PROPERTY(int uplookLighting READ uplookLighting WRITE setUplookLighting NOTIFY lightingValueChanged)
    Q_PROPERTY(int lightPanelLighting READ lightPanelLighting WRITE setLightPanelLighting NOTIFY lightPanelValueChanged)
//    QMap<QString,BaslerPylonCamera> cameras;
    QMap<QString,XtMotor*> motors;
    QMap<QString,XtGeneralInput*> input_ios;
    QMap<QString,XtGeneralOutput*> output_ios;

    BaslerPylonCamera * pylonDownlookCamera = Q_NULLPTR;
    BaslerPylonCamera * pylonUplookCamera = Q_NULLPTR;
    VisionModule * visionModule = Q_NULLPTR;
    WordopLight * lightingModule = Q_NULLPTR;
    LontryLight * lightPanel = Q_NULLPTR;

    AAHeadModule* aa_head_module = Q_NULLPTR;
    MaterialCarrier* lut_carrier = Q_NULLPTR;
    MaterialCarrier* sut_carrier = Q_NULLPTR;
    LutModule *lut_module = Q_NULLPTR;
    SutModule *sut_module = Q_NULLPTR;

    int downlookLighting() const
    {
        return m_downlookLighting;
    }

    int uplookLighting() const
    {
        return m_uplookLighting;
    }

    int lightPanelLighting() const
    {
        return m_lightPanelLighting;
    }

signals:
    void lightingValueChanged(int downlookLighting);

    void lightPanelValueChanged(int lightPanelLighting);

public slots:
    void setDownlookLighting(int downlookLighting)
    {
        if (m_downlookLighting == downlookLighting)
            return;

        m_downlookLighting = downlookLighting;
        lightingModule->SetBrightness(LIGHTING_AA1_DL, (uint8_t)downlookLighting);
        emit lightingValueChanged(m_downlookLighting);
    }
    void setUplookLighting(int uplookLighting)
    {
        if (m_uplookLighting == uplookLighting)
            return;

        m_uplookLighting = uplookLighting;
        lightingModule->SetBrightness(LIGHTING_LUT_UL, (uint8_t)uplookLighting);
        emit lightingValueChanged(m_uplookLighting);
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
    void EnableMotors();
    void DisableAllMotors();
    Q_INVOKABLE bool allMotorsSeekOrigin();
    Q_INVOKABLE void stopSeeking();

    XtMotor* GetMotorByName(QString name);
    XtVcMotor *GetVcMotorByName(QString name);
    XtGeneralOutput *GetOutputIoByName(QString name);
    XtGeneralInput *GetInputIoByName(QString name);
    XtVacuum *GetVacuumByName(QString name);
    XtCylinder *GetCylinderByName(QString name);


};

#endif // dBASEMODULEMANAGER_H
