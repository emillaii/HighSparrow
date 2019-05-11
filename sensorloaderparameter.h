#ifndef SENSORLOADERPARAMETER_H
#define SENSORLOADERPARAMETER_H

#include "propertybase.h"



class SensorLoaderParameter:public PropertyBase
{
    Q_OBJECT
public:
    SensorLoaderParameter():PropertyBase (){}
    Q_PROPERTY(double vcmWorkForce READ vcmWorkForce WRITE setVcmWorkForce NOTIFY vcmWorkForceChanged)
    Q_PROPERTY(double vcmWorkSpeed READ vcmWorkSpeed WRITE setVcmWorkSpeed NOTIFY vcmWorkSpeedChanged)
    Q_PROPERTY(double vcmMargin READ vcmMargin WRITE setVcmMargin NOTIFY vcmMarginChanged)
    Q_PROPERTY(double pickSensorZ READ pickSensorZ WRITE setPickSensorZ NOTIFY pickSensorZChanged)
    Q_PROPERTY(double placeSensorZ READ placeSensorZ WRITE setPlaceSensorZ NOTIFY placeSensorZChanged)
    Q_PROPERTY(QString sensorLocationName READ sensorLocationName WRITE setSensorLocationName NOTIFY sensorLocationNameChanged)
    Q_PROPERTY(QString vacancyLocationName READ vacancyLocationName WRITE setVacancyLocationName NOTIFY vacancyLocationNameChanged)
    Q_PROPERTY(QString calibrationGlassLocationName READ calibrationGlassLocationName WRITE setCalibrationGlassLocationName NOTIFY calibrationGlassLocationNameChanged)
    Q_PROPERTY(QString sutLocationName READ sutLocationName WRITE setSutLocationName NOTIFY sutLocationNameChanged)
    Q_PROPERTY(QString sutSensorLocationName READ sutSensorLocationName WRITE setSutSensorLocationName NOTIFY sutSensorLocationNameChanged)
    Q_PROPERTY(QString sutProductLocationName READ sutProductLocationName WRITE setSutProductLocationName NOTIFY sutProductLocationNameChanged)
    Q_PROPERTY(int finishDelay READ finishDelay WRITE setFinishDelay NOTIFY finishDelayChanged)
    Q_PROPERTY(double pickProductZ READ pickProductZ WRITE setPickProductZ NOTIFY pickProductZChanged)
    Q_PROPERTY(double placeProductZ READ placeProductZ WRITE setPlaceProductZ NOTIFY placeProductZChanged)
    Q_PROPERTY(double zOffset READ zOffset WRITE setZOffset NOTIFY zOffsetChanged)
    Q_PROPERTY(double pickNgSensorZ READ pickNgSensorZ WRITE setPickNgSensorZ NOTIFY pickNgSensorZChanged)
    Q_PROPERTY(double placeNgSensorZ READ placeNgSensorZ WRITE setPlaceNgSensorZ NOTIFY placeNgSensorZChanged)

    double vcmWorkForce() const
    {
        return m_vcmWorkForce;
    }
    double vcmWorkSpeed() const
    {
        return m_vcmWorkSpeed;
    }

    double vcmMargin() const
    {
        return m_vcmMargin;
    }

    QString sensorLocationName() const
    {
        return m_sensorLocationName;
    }

    QString vacancyLocationName() const
    {
        return m_vacancyLocationName;
    }

    QString sutLocationName() const
    {
        return m_sutLocationName;
    }

    QString sutSensorLocationName() const
    {
        return m_sutSensorLocationName;
    }

    QString sutProductLocationName() const
    {
        return m_sutProductLocationName;
    }

    int finishDelay() const
    {
        return m_finishDelay;
    }

    double pickProductZ() const
    {
        return m_pickProductZ;
    }

    double placeProductZ() const
    {
        return m_placeProductZ;
    }

    double pickSensorZ() const
    {
        return m_pickSensorZ;
    }

    double placeSensorZ() const
    {
        return m_placeSensorZ;
    }

    double zOffset() const
    {
        return m_zOffset;
    }

    double pickNgSensorZ() const
    {
        return m_pickNgSensorZ;
    }

    double placeNgSensorZ() const
    {
        return m_placeNgSensorZ;
    }

    QString calibrationGlassLocationName() const
    {
        return m_calibrationGlassLocationName;
    }

public slots:
    void setVcmWorkForce(double vcmWorkForce)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcmWorkForce, vcmWorkForce))
            return;

        m_vcmWorkForce = vcmWorkForce;
        emit vcmWorkForceChanged(m_vcmWorkForce);
    }
    void setVcmWorkSpeed(double vcmWorkSpeed)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcmWorkSpeed, vcmWorkSpeed))
            return;

        m_vcmWorkSpeed = vcmWorkSpeed;
        emit vcmWorkSpeedChanged(m_vcmWorkSpeed);
    }

    void setVcmMargin(double vcmMargin)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcmMargin, vcmMargin))
            return;

        m_vcmMargin = vcmMargin;
        emit vcmMarginChanged(m_vcmMargin);
    }


    void setSensorLocationName(QString sensorLocationName)
    {
        if (m_sensorLocationName == sensorLocationName)
            return;

        m_sensorLocationName = sensorLocationName;
        emit sensorLocationNameChanged(m_sensorLocationName);
    }

    void setVacancyLocationName(QString vacancyLocationName)
    {
        if (m_vacancyLocationName == vacancyLocationName)
            return;

        m_vacancyLocationName = vacancyLocationName;
        emit vacancyLocationNameChanged(m_vacancyLocationName);
    }

    void setSutLocationName(QString sutLocationName)
    {
        if (m_sutLocationName == sutLocationName)
            return;

        m_sutLocationName = sutLocationName;
        emit sutLocationNameChanged(m_sutLocationName);
    }

    void setSutSensorLocationName(QString sutSensorLocationName)
    {
        if (m_sutSensorLocationName == sutSensorLocationName)
            return;

        m_sutSensorLocationName = sutSensorLocationName;
        emit sutSensorLocationNameChanged(m_sutSensorLocationName);
    }
    void setSutProductLocationName(QString sutProductLocationName)
    {
        if (m_sutProductLocationName == sutProductLocationName)
            return;

        m_sutProductLocationName = sutProductLocationName;
        emit sutProductLocationNameChanged(m_sutProductLocationName);
    }

    void setFinishDelay(int finishDelay)
    {
        if (m_finishDelay == finishDelay)
            return;

        m_finishDelay = finishDelay;
        emit finishDelayChanged(m_finishDelay);
    }

    void setPickProductZ(double pickProductZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pickProductZ, pickProductZ))
            return;

        m_pickProductZ = pickProductZ;
        emit pickProductZChanged(m_pickProductZ);
    }

    void setPlaceProductZ(double placeProductZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_placeProductZ, placeProductZ))
            return;

        m_placeProductZ = placeProductZ;
        emit placeProductZChanged(m_placeProductZ);
    }

    void setPickSensorZ(double pickSensorZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pickSensorZ, pickSensorZ))
            return;

        m_pickSensorZ = pickSensorZ;
        emit pickSensorZChanged(m_pickSensorZ);
    }

    void setPlaceSensorZ(double placeSensorZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_placeSensorZ, placeSensorZ))
            return;

        m_placeSensorZ = placeSensorZ;
        emit placeSensorZChanged(m_placeSensorZ);
    }

    void setZOffset(double zOffset)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_zOffset, zOffset))
            return;

        m_zOffset = zOffset;
        emit zOffsetChanged(m_zOffset);
    }

    void setPickNgSensorZ(double pickNgSensorZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pickNgSensorZ, pickNgSensorZ))
            return;

        m_pickNgSensorZ = pickNgSensorZ;
        emit pickNgSensorZChanged(m_pickNgSensorZ);
    }

    void setPlaceNgSensorZ(double placeNgSensorZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_placeNgSensorZ, placeNgSensorZ))
            return;

        m_placeNgSensorZ = placeNgSensorZ;
        emit placeNgSensorZChanged(m_placeNgSensorZ);
    }

    void setCalibrationGlassLocationName(QString calibrationGlassLocationName)
    {
        if (m_calibrationGlassLocationName == calibrationGlassLocationName)
            return;

        m_calibrationGlassLocationName = calibrationGlassLocationName;
        emit calibrationGlassLocationNameChanged(m_calibrationGlassLocationName);
    }

signals:
    void vcmWorkForceChanged(double vcmWorkForce);
    void vcmWorkSpeedChanged(double vcmWorkSpeed);

    void vcmMarginChanged(double vcmMargin);


    void sensorLocationNameChanged(QString sensorLocationName);

    void vacancyLocationNameChanged(QString vacancyLocationName);

    void sutLocationNameChanged(QString sutLocationName);

    void sutSensorLocationNameChanged(QString sutSensorLocationName);

    void sutProductLocationNameChanged(QString sutProductLocationName);

    void finishDelayChanged(int finishDelay);

    void pickProductZChanged(double pickProductZ);

    void placeProductZChanged(double placeProductZ);

    void pickSensorZChanged(double pickSensorZ);

    void placeSensorZChanged(double placeSensorZ);

    void zOffsetChanged(double zOffset);

    void pickNgSensorZChanged(double pickNgSensorZ);

    void placeNgSensorZChanged(double placeNgSensorZ);

    void calibrationGlassLocationNameChanged(QString calibrationGlassLocationName);

private:
    double m_vcmWorkForce = 0;
    double m_vcmWorkSpeed = 0;
    double m_vcmMargin = 0;
    double m_pickSensorZ = 0;
    double m_placeSensorZ = 0;
    QString m_sensorLocationName = "";
    QString m_vacancyLocationName = "";
    QString m_sutLocationName = "";
    QString m_sutSensorLocationName = "";
    QString m_sutProductLocationName = "";
    int m_finishDelay = 100;
    double m_pickProductZ = 0;
    double m_placeProductZ = 0;
    double m_zOffset = 0;
    double m_pickNgSensorZ = 0;
    double m_placeNgSensorZ = 0;
    QString m_calibrationGlassLocationName;
};
class SensorLoaderState:public PropertyBase
{
    Q_OBJECT
public:
    SensorLoaderState():PropertyBase (){}
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(bool hasTray READ hasTray WRITE setHasTray NOTIFY hasTrayChanged)
    Q_PROPERTY(bool sutHasSensor READ sutHasSensor WRITE setSutHasSensor NOTIFY sutHasSensorChanged)
    Q_PROPERTY(bool sutHasNgSensor READ sutHasNgSensor WRITE setSutHasNgSensor NOTIFY sutHasNgSensorChanged)
    Q_PROPERTY(bool sutHasProduct READ sutHasProduct WRITE setSutHasProduct NOTIFY sutHasProductChanged)
    Q_PROPERTY(bool needLoadSensor READ needLoadSensor WRITE setNeedLoadSensor NOTIFY needLoadSensorChanged)
    Q_PROPERTY(bool needChangTray READ needChangTray WRITE setNeedChangTray NOTIFY needChangTrayChanged)
    Q_PROPERTY(bool allowChangeTray READ allowChangeTray WRITE setAllowChangeTray NOTIFY allowChangeTrayChanged)
    Q_PROPERTY(bool hasPickedSensor READ hasPickedSensor WRITE setHasPickedSensor NOTIFY hasPickedSensorChanged)
    Q_PROPERTY(bool hasPickedProduct READ hasPickedProduct WRITE setHasPickedProduct NOTIFY hasPickedProductChanged)
    Q_PROPERTY(bool hasPickedNgSensor READ hasPickedNgSensor WRITE setHasPickedNgSensor NOTIFY hasPickedNgSensorChanged)
    Q_PROPERTY(bool beExchangeMaterial READ beExchangeMaterial WRITE setBeExchangeMaterial NOTIFY beExchangeMaterialChanged)
    Q_PROPERTY(QString cmd READ cmd WRITE setCmd NOTIFY cmdChanged)
    int runMode() const
    {
        return m_runMode;
    }
    bool hasTray() const
    {
        return m_hasTray;
    }

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

    bool needLoadSensor() const
    {
        return m_needLoadSensor;
    }

    bool needChangTray() const
    {
        return m_needChangTray;
    }

    bool allowChangeTray() const
    {
        return m_allowChangeTray;
    }

    bool hasPickedSensor() const
    {
        return m_hasPickedSensor;
    }

    bool hasPickedProduct() const
    {
        return m_hasPickedProduct;
    }

    bool hasPickedNgSensor() const
    {
        return m_hasPickedNgSensor;
    }

    bool beExchangeMaterial() const
    {
        return m_beExchangeMaterial;
    }

    QString cmd() const
    {
        return m_cmd;
    }

public slots:
    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }
    void setHasTray(bool hasTray)
    {
        if (m_hasTray == hasTray)
            return;

        m_hasTray = hasTray;
        emit hasTrayChanged(m_hasTray);
    }

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

    void setNeedLoadSensor(bool needLoadSensor)
    {
        if (m_needLoadSensor == needLoadSensor)
            return;

        m_needLoadSensor = needLoadSensor;
        emit needLoadSensorChanged(m_needLoadSensor);
    }

    void setNeedChangTray(bool needChangTray)
    {
        if (m_needChangTray == needChangTray)
            return;

        m_needChangTray = needChangTray;
        emit needChangTrayChanged(m_needChangTray);
    }

    void setAllowChangeTray(bool allowChangeTray)
    {
        if (m_allowChangeTray == allowChangeTray)
            return;

        m_allowChangeTray = allowChangeTray;
        emit allowChangeTrayChanged(m_allowChangeTray);
    }

    void setHasPickedSensor(bool hasPickedSensor)
    {
        if (m_hasPickedSensor == hasPickedSensor)
            return;

        m_hasPickedSensor = hasPickedSensor;
        emit hasPickedSensorChanged(m_hasPickedSensor);
    }

    void setHasPickedProduct(bool hasPickedProduct)
    {
        if (m_hasPickedProduct == hasPickedProduct)
            return;

        m_hasPickedProduct = hasPickedProduct;
        emit hasPickedProductChanged(m_hasPickedProduct);
    }

    void setHasPickedNgSensor(bool hasPickedNgSensor)
    {
        if (m_hasPickedNgSensor == hasPickedNgSensor)
            return;

        m_hasPickedNgSensor = hasPickedNgSensor;
        emit hasPickedNgSensorChanged(m_hasPickedNgSensor);
    }

    void setBeExchangeMaterial(bool beExchangeMaterial)
    {
        if (m_beExchangeMaterial == beExchangeMaterial)
            return;

        m_beExchangeMaterial = beExchangeMaterial;
        emit beExchangeMaterialChanged(m_beExchangeMaterial);
    }

    void setCmd(QString cmd)
    {
        if (m_cmd == cmd)
            return;

        m_cmd = cmd;
        emit cmdChanged(m_cmd);
    }

signals:
    void runModeChanged(int runMode);
    void hasTrayChanged(bool hasTray);

    void sutHasSensorChanged(bool sutHasSensor);

    void sutHasNgSensorChanged(bool sutHasNgSensor);

    void sutHasProductChanged(bool sutHasProduct);

    void needLoadSensorChanged(bool needLoadSensor);

    void needChangTrayChanged(bool needChangTray);

    void allowChangeTrayChanged(bool allowChangeTray);

    void hasPickedSensorChanged(bool hasPickedSensor);

    void hasPickedProductChanged(bool hasPickedProduct);

    void hasPickedNgSensorChanged(bool hasPickedNgSensor);

    void beExchangeMaterialChanged(bool beExchangeMaterial);

    void cmdChanged(QString cmd);

private:
    int m_runMode = 0;
    bool m_hasTray = false;
    bool m_sutHasSensor = false;
    bool m_sutHasNgSensor = false;
    bool m_sutHasProduct = false;
    bool m_needLoadSensor = false;
    bool m_needChangTray = false;
    bool m_allowChangeTray = false;
    bool m_hasPickedSensor = false;
    bool m_hasPickedProduct = false;
    bool m_hasPickedNgSensor = false;
    bool m_beExchangeMaterial = false;
    QString m_cmd = "";
};

#endif // SENSORLOADERPARAMETER_H
