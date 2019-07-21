#ifndef SUT_PARAMETER_H
#define SUT_PARAMETER_H
#include "propertybase.h"

class SutParameter : public PropertyBase
{
    Q_OBJECT
public:
    explicit SutParameter(){}
    Q_PROPERTY(double Force READ Force WRITE setForce NOTIFY paramsChanged)
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(QString motorZName READ motorZName WRITE setMotorZName NOTIFY motorZNameChanged)
    Q_PROPERTY(QString vacuumName READ vacuumName WRITE setVacuum1Name NOTIFY vacuum1NameChanged)
    Q_PROPERTY(QString cylinderName READ cylinderName WRITE setCylinderName NOTIFY cylinderNameChanged)
    Q_PROPERTY(QString downlookLocationName READ downlookLocationName WRITE setDownlookLocationName NOTIFY downlookLocationNameChanged)
    Q_PROPERTY(QString updownlookUpLocationName READ updownlookUpLocationName WRITE setUpdownlookUpLocationName NOTIFY updownlookUpLocationNameChanged)
    Q_PROPERTY(QString updownlookDownLocationName READ updownlookDownLocationName WRITE setUpdownlookDownLocationName NOTIFY updownlookDownLocationNameChanged)
    Q_PROPERTY(double cameraTheta READ cameraTheta WRITE setCameraTheta NOTIFY cameraThetaChanged)
    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)
    double Force() const
    {
        return m_Force;
    }

    QString motorXName() const
    {
        return m_motorXName;
    }

    QString motorYName() const
    {
        return m_motorYName;
    }

    QString motorZName() const
    {
        return m_motorZName;
    }

    QString vacuumName() const
    {
        return m_vacuumName;
    }

    QString cylinderName() const
    {
        return m_sylinderName;
    }

    QString downlookLocationName() const
    {
        return m_downlookLocationName;
    }

    QString updownlookUpLocationName() const
    {
        return m_updownlookUpLocationName;
    }

    QString updownlookDownLocationName() const
    {
        return m_updownlookDownLocationName;
    }
    double cameraTheta() const
    {
        return m_cameraTheta;
    }

    QString moduleName() const
    {
        return m_moduleName;
    }

public slots:
    void setForce(double Force)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_Force, Force))
            return;

        m_Force = Force;
        emit paramsChanged(m_Force);
    }

    void setMotorXName(QString motorXName)
    {
        if (m_motorXName == motorXName)
            return;

        m_motorXName = motorXName;
        emit motorXNameChanged(m_motorXName);
    }

    void setMotorYName(QString motorYName)
    {
        if (m_motorYName == motorYName)
            return;

        m_motorYName = motorYName;
        emit motorYNameChanged(m_motorYName);
    }

    void setMotorZName(QString motorZName)
    {
        if (m_motorZName == motorZName)
            return;

        m_motorZName = motorZName;
        emit motorZNameChanged(m_motorZName);
    }

    void setVacuum1Name(QString vacuumName)
    {
        if (m_vacuumName == vacuumName)
            return;

        m_vacuumName = vacuumName;
        emit vacuum1NameChanged(m_vacuumName);
    }

    void setCylinderName(QString sylinderName)
    {
        if (m_sylinderName == sylinderName)
            return;

        m_sylinderName = sylinderName;
        emit cylinderNameChanged(m_sylinderName);
    }

    void setDownlookLocationName(QString downlookLocationName)
    {
        if (m_downlookLocationName == downlookLocationName)
            return;

        m_downlookLocationName = downlookLocationName;
        emit downlookLocationNameChanged(m_downlookLocationName);
    }

    void setUpdownlookUpLocationName(QString updownlookUpLocationName)
    {
        if (m_updownlookUpLocationName == updownlookUpLocationName)
            return;

        m_updownlookUpLocationName = updownlookUpLocationName;
        emit updownlookUpLocationNameChanged(m_updownlookUpLocationName);
    }

    void setUpdownlookDownLocationName(QString updownlookDownLocationName)
    {
        if (m_updownlookDownLocationName == updownlookDownLocationName)
            return;

        m_updownlookDownLocationName = updownlookDownLocationName;
        emit updownlookDownLocationNameChanged(m_updownlookDownLocationName);
    }
    void setCameraTheta(double cameraTheta)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_cameraTheta, cameraTheta))
            return;

        m_cameraTheta = cameraTheta;
        emit cameraThetaChanged(m_cameraTheta);
    }

    void setModuleName(QString moduleName)
    {
        if (m_moduleName == moduleName)
            return;

        m_moduleName = moduleName;
        emit moduleNameChanged(m_moduleName);
    }

signals:
    void paramsChanged(double Force);

    void motorXNameChanged(QString motorXName);

    void motorYNameChanged(QString motorYName);

    void motorZNameChanged(QString motorZName);

    void vacuum1NameChanged(QString vacuumName);

    void cylinderNameChanged(QString cylinderName);

    void downlookLocationNameChanged(QString downlookLocationName);

    void updownlookUpLocationNameChanged(QString updownlookUpLocationName);

    void updownlookDownLocationNameChanged(QString updownlookDownLocationName);

    void mushroomLocationNameChanged(QString mushroomLocationName);

    void cameraThetaChanged(double cameraTheta);

    void moduleNameChanged(QString moduleName);

private:
    double m_Force = 0;
    QString m_motorXName = "SUT_X";
    QString m_motorYName = "SUT_Y";
    QString m_motorZName = "SUT_Z";
    QString m_vacuumName = "SUT_V";
    QString m_sylinderName = "SUT_P";
    QString m_downlookLocationName;
    QString m_updownlookUpLocationName;
    QString m_updownlookDownLocationName;
    double m_cameraTheta = 0;
    QString m_moduleName = "SUT1Module";
};

class SutState:public PropertyBase
{
    Q_OBJECT
    Q_PROPERTY(bool sutHasSensor READ sutHasSensor WRITE setSutHasSensor NOTIFY sutHasSensorChanged)
    Q_PROPERTY(bool sutHasNgSensor READ sutHasNgSensor WRITE setSutHasNgSensor NOTIFY sutHasNgSensorChanged)
    Q_PROPERTY(bool sutHasProduct READ sutHasProduct WRITE setSutHasProduct NOTIFY sutHasProductChanged)
    Q_PROPERTY(bool sutHasNgProduct READ sutHasNgProduct WRITE setSutHasNgProduct NOTIFY sutHasNgProductChanged)
    Q_PROPERTY(bool allowLoadSensor READ allowLoadSensor WRITE setAllowLoadSensor NOTIFY allowLoadSensorChanged)
    Q_PROPERTY(bool loadingSensor READ loadingSensor WRITE setLoadingSensor NOTIFY loadingSensorChanged)
    Q_PROPERTY(bool waitLoading READ waitLoading WRITE setWaitLoading NOTIFY waitLoadingChanged)

public:
    bool sutHasSensor() const
    {
        return m_sutHasSensor;
    }

    bool sutHasNgSensor() const
    {
        return m_sutHasNgSensor;
    }

    bool sutHasProduct() const
    {
        return m_sutHasProduct;
    }

    bool allowLoadSensor() const
    {
        return m_allowLoadSensor;
    }

    bool loadingSensor() const
    {
        return m_loadingSensor;
    }

    bool waitLoading() const
    {
        return m_waitLoading;
    }

    bool sutHasNgProduct() const
    {
        return m_sutHasNgProduct;
    }

public slots:
    void setSutHasSensor(bool sutHasSensor)
    {
        if (m_sutHasSensor == sutHasSensor)
            return;

        m_sutHasSensor = sutHasSensor;
        emit sutHasSensorChanged(m_sutHasSensor);
    }

    void setSutHasNgSensor(bool sutHasNgSensor)
    {
        if (m_sutHasNgSensor == sutHasNgSensor)
            return;

        m_sutHasNgSensor = sutHasNgSensor;
        emit sutHasNgSensorChanged(m_sutHasNgSensor);
    }

    void setSutHasProduct(bool sutHasProduct)
    {
        if (m_sutHasProduct == sutHasProduct)
            return;

        m_sutHasProduct = sutHasProduct;
        emit sutHasProductChanged(m_sutHasProduct);
    }

    void setAllowLoadSensor(bool allowLoadSensor)
    {
        if (m_allowLoadSensor == allowLoadSensor)
            return;

        m_allowLoadSensor = allowLoadSensor;
        emit allowLoadSensorChanged(m_allowLoadSensor);
    }

    void setLoadingSensor(bool loadingSensor)
    {
        if (m_loadingSensor == loadingSensor)
            return;

        m_loadingSensor = loadingSensor;
        emit loadingSensorChanged(m_loadingSensor);
    }

    void setWaitLoading(bool waitLoading)
    {
        if (m_waitLoading == waitLoading)
            return;

        m_waitLoading = waitLoading;
        emit waitLoadingChanged(m_waitLoading);
    }

    void setSutHasNgProduct(bool sutHasNgProduct)
    {
        if (m_sutHasNgProduct == sutHasNgProduct)
            return;

        m_sutHasNgProduct = sutHasNgProduct;
        emit sutHasNgProductChanged(m_sutHasNgProduct);
    }

signals:
    void sutHasSensorChanged(bool sutHasSensor);

    void sutHasNgSensorChanged(bool sutHasNgSensor);

    void sutHasProductChanged(bool sutHasProduct);

    void allowLoadSensorChanged(bool allowLoadSensor);

    void loadingSensorChanged(bool loadingSensor);

    void waitLoadingChanged(bool waitLoading);

    void sutHasNgProductChanged(bool sutHasNgProduct);

private:
    bool m_sutHasSensor = false;
    bool m_sutHasNgSensor = false;
    bool m_sutHasProduct = false;
    bool m_allowLoadSensor = false;
    bool m_loadingSensor = false;
    bool m_waitLoading = false;
    bool m_sutHasNgProduct = false;
};

#endif // SUT_PARAMETER_H
