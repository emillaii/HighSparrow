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
    Q_PROPERTY(double loadPosArrivedY READ loadPosArrivedY WRITE setLoadPosArriedY NOTIFY loadPosArrivedYChanged)
    Q_PROPERTY(double prPosArrivedY READ prPosArrivedY WRITE setPrPosArrivedY NOTIFY prPosArrivedYChanged)
    Q_PROPERTY(double mushroomPosArrivedY READ mushroomPosArrivedY WRITE setMushroomPosArrivedY NOTIFY mushroomPosArrivedYChanged)
    Q_PROPERTY(double zArrivedEorror READ zArrivedEorror WRITE setzArrivedEorror NOTIFY zArrivedEorrorChanged)
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

    double loadPosArrivedY() const
    {
        return m_yArrivedError;
    }

    double zArrivedEorror() const
    {
        return m_zArrivedEorror;
    }

    double prPosArrivedY() const
    {
        return m_prPosArrivedY;
    }

    double mushroomPosArrivedY() const
    {
        return m_mushroomPosArrivedY;
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

    void setLoadPosArriedY(double yArrivedError)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_yArrivedError, yArrivedError))
            return;

        m_yArrivedError = yArrivedError;
        emit loadPosArrivedYChanged(m_yArrivedError);
    }

    void setzArrivedEorror(double zArrivedEorror)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_zArrivedEorror, zArrivedEorror))
            return;

        m_zArrivedEorror = zArrivedEorror;
        emit zArrivedEorrorChanged(m_zArrivedEorror);
    }

    void setPrPosArrivedY(double prPosArrivedY)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_prPosArrivedY, prPosArrivedY))
            return;

        m_prPosArrivedY = prPosArrivedY;
        emit prPosArrivedYChanged(m_prPosArrivedY);
    }

    void setMushroomPosArrivedY(double mushroomPosArrivedY)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_mushroomPosArrivedY, mushroomPosArrivedY))
            return;

        m_mushroomPosArrivedY = mushroomPosArrivedY;
        emit mushroomPosArrivedYChanged(m_mushroomPosArrivedY);
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

    void loadPosArrivedYChanged(double loadPosArrivedY);

    void zArrivedEorrorChanged(double zArrivedEorror);

    void prPosArrivedYChanged(double prPosArrivedY);

    void mushroomPosArrivedYChanged(double mushroomPosArrivedY);

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
    QString m_moduleName = "Sut1Module";
    double m_yArrivedError = 10;
    double m_zArrivedEorror = 0.1;
    double m_prPosArrivedY = 10;
    double m_mushroomPosArrivedY = 1;
};

class SutState:public PropertyBase
{
    Q_OBJECT
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(int stationNumber READ stationNumber WRITE setStationNumber NOTIFY stationNumberChanged)
    Q_PROPERTY(bool disableStation READ disableStation WRITE setDisableStation NOTIFY disableStationChanged)
    Q_PROPERTY(bool handlyChangeSensor READ handlyChangeSensor WRITE setHandlyChangeSensor NOTIFY handlyChangeSensorChanged)
    Q_PROPERTY(int sutMaterialState READ sutMaterialState WRITE setSutMaterialState NOTIFY sutMaterialStateChanged)

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

    int runMode() const
    {
        return m_runMode;
    }

    bool handlyChangeSensor() const
    {
        return m_handlyChangeSensor;
    }

    int stationNumber() const
    {
        return m_stationNumber;
    }

    bool disableStation() const
    {
        return m_disableStation;
    }

    int sutMaterialState() const
    {
        return m_sutMaterialState;
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

    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }

    void setHandlyChangeSensor(bool handlyChangeSensor)
    {
        if (m_handlyChangeSensor == handlyChangeSensor)
            return;

        m_handlyChangeSensor = handlyChangeSensor;
        emit handlyChangeSensorChanged(m_handlyChangeSensor);
    }

    void setStationNumber(int stationNumber)
    {
        if (m_stationNumber == stationNumber)
            return;

        m_stationNumber = stationNumber;
        emit stationNumberChanged(m_stationNumber);
    }

    void setDisableStation(bool disableStation)
    {
        if (m_disableStation == disableStation)
            return;

        m_disableStation = disableStation;
        emit disableStationChanged(m_disableStation);
    }

    void setSutMaterialState(int sutMaterialState)
    {
        if (m_sutMaterialState == sutMaterialState)
            return;

        m_sutMaterialState = sutMaterialState;
        emit sutMaterialStateChanged(m_sutMaterialState);
    }

signals:
    void sutHasSensorChanged(bool sutHasSensor);

    void sutHasNgSensorChanged(bool sutHasNgSensor);

    void sutHasProductChanged(bool sutHasProduct);

    void allowLoadSensorChanged(bool allowLoadSensor);

    void loadingSensorChanged(bool loadingSensor);

    void waitLoadingChanged(bool waitLoading);

    void sutHasNgProductChanged(bool sutHasNgProduct);

    void runModeChanged(int runMode);

    void handlyChangeSensorChanged(bool handlyChangeSensor);

    void stationNumberChanged(int stationNumber);

    void disableStationChanged(bool disableStation);

    void sutMaterialStateChanged(int sutMaterialState);

private:
    bool m_sutHasSensor = false;
    bool m_sutHasNgSensor = false;
    bool m_sutHasProduct = false;
    bool m_allowLoadSensor = false;
    bool m_loadingSensor = false;
    bool m_waitLoading = false;
    bool m_sutHasNgProduct = false;
    int m_runMode = 0;
    bool m_handlyChangeSensor = false;
    int m_stationNumber = 0;
    bool m_disableStation = false;
    int m_sutMaterialState = 0;
};

#endif // SUT_PARAMETER_H