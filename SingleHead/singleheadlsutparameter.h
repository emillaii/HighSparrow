#ifndef SINGLEHEADLSUTPARAMETER_H
#define SINGLEHEADLSUTPARAMETER_H

#include "Utils/propertybase.h"


class SingleHeadLSutParameter : public PropertyBase{
    Q_OBJECT
private:
    double  m_ZOffset;
    double  m_LensSoftlandingVel;
    double  m_LensSoftlandingForce;
    QString m_motorXName;
    QString m_motorYName;
    QString m_motorZName;
    QString m_lutVacuumName;
    QString m_sutVacuumName;
    QString m_cylinderName;
    QString m_sutDownLookLocationName;
    QString m_mushroomLocationName;
    QString m_lutGripperLoactionName;

    int m_delayBeforGripLens = 0;

    int m_delayAfterGripLens = 0;

public:
    explicit SingleHeadLSutParameter(){}
    Q_PROPERTY(double ZOffset READ ZOffset WRITE setZOffset NOTIFY ZOffsetChanged)
    Q_PROPERTY(double LensSoftlandingVel READ LensSoftlandingVel WRITE setLensSoftlandingVel NOTIFY LensSoftlandingVelChanged)
    Q_PROPERTY(double LensSoftlandingForce READ LensSoftlandingForce WRITE setLensSoftlandingForce NOTIFY LensSoftlandingForceChanged)
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(QString motorZName READ motorZName WRITE setMotorZName NOTIFY motorZNameChanged)
    Q_PROPERTY(QString lutVacuumName READ lutVacuumName WRITE setLutVacuumName NOTIFY lutVacuumNameChanged)
    Q_PROPERTY(QString sutVacuumName READ sutVacuumName WRITE setSutVacuumName NOTIFY sutVacuumNameChanged)
    Q_PROPERTY(QString cylinderName READ cylinderName WRITE setCylinderName NOTIFY cylinderNameChanged)
    Q_PROPERTY(QString sutDownLookLocationName READ sutDownLookLocationName WRITE setSutDownLookLocationName NOTIFY sutDownLookLocationNameChanged)
    Q_PROPERTY(QString mushroomLocationName READ mushroomLocationName WRITE setMushroomLocationName NOTIFY mushroomLocationNameChanged)
    Q_PROPERTY(QString lutGripperLoactionName READ lutGripperLoactionName WRITE setLutGripperLoactionName NOTIFY lutGripperLoactionNameChanged)
    Q_PROPERTY(int delayBeforGripLens READ delayBeforGripLens WRITE setDelayBeforGripLens NOTIFY delayBeforGripLensChanged)
    Q_PROPERTY(int delayAfterGripLens READ delayAfterGripLens WRITE setDelayAfterGripLens NOTIFY delayAfterGripLensChanged)

    double ZOffset() const
    {
        return m_ZOffset;
    }

    double LensSoftlandingVel() const
    {
        return m_LensSoftlandingVel;
    }

    double LensSoftlandingForce() const
    {
        return m_LensSoftlandingForce;
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

    QString lutVacuumName() const
    {
        return m_lutVacuumName;
    }

    QString sutVacuumName() const
    {
        return m_sutVacuumName;
    }

    QString cylinderName() const
    {
        return m_cylinderName;
    }

    QString sutDownLookLocationName() const
    {
        return m_sutDownLookLocationName;
    }

    QString mushroomLocationName() const
    {
        return m_mushroomLocationName;
    }

    QString lutGripperLoactionName() const
    {
        return m_lutGripperLoactionName;
    }

    int delayBeforGripLens() const
    {
        return m_delayBeforGripLens;
    }

    int delayAfterGripLens() const
    {
        return m_delayAfterGripLens;
    }

public slots:
    void setZOffset(double ZOffset)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_ZOffset, ZOffset))
            return;

        m_ZOffset = ZOffset;
        emit ZOffsetChanged(m_ZOffset);
    }

    void setLensSoftlandingVel(double LensSoftlandingVel)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_LensSoftlandingVel, LensSoftlandingVel))
            return;

        m_LensSoftlandingVel = LensSoftlandingVel;
        emit LensSoftlandingVelChanged(m_LensSoftlandingVel);
    }

    void setLensSoftlandingForce(double LensSoftlandingForce)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_LensSoftlandingForce, LensSoftlandingForce))
            return;

        m_LensSoftlandingForce = LensSoftlandingForce;
        emit LensSoftlandingForceChanged(m_LensSoftlandingForce);
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

    void setLutVacuumName(QString lutVacuumName)
    {
        if (m_lutVacuumName == lutVacuumName)
            return;

        m_lutVacuumName = lutVacuumName;
        emit lutVacuumNameChanged(m_lutVacuumName);
    }

    void setSutVacuumName(QString sutVacuumName)
    {
        if (m_sutVacuumName == sutVacuumName)
            return;

        m_sutVacuumName = sutVacuumName;
        emit sutVacuumNameChanged(m_sutVacuumName);
    }

    void setCylinderName(QString cylinderName)
    {
        if (m_cylinderName == cylinderName)
            return;

        m_cylinderName = cylinderName;
        emit cylinderNameChanged(m_cylinderName);
    }

    void setSutDownLookLocationName(QString sutDownLookLocationName)
    {
        if (m_sutDownLookLocationName == sutDownLookLocationName)
            return;

        m_sutDownLookLocationName = sutDownLookLocationName;
        emit sutDownLookLocationNameChanged(m_sutDownLookLocationName);
    }

    void setMushroomLocationName(QString mushroomLocationName)
    {
        if (m_mushroomLocationName == mushroomLocationName)
            return;

        m_mushroomLocationName = mushroomLocationName;
        emit mushroomLocationNameChanged(m_mushroomLocationName);
    }

    void setLutGripperLoactionName(QString lutGripperLoactionName)
    {
        if (m_lutGripperLoactionName == lutGripperLoactionName)
            return;

        m_lutGripperLoactionName = lutGripperLoactionName;
        emit lutGripperLoactionNameChanged(m_lutGripperLoactionName);
    }

    void setDelayBeforGripLens(int delayBeforGripLens)
    {
        if (m_delayBeforGripLens == delayBeforGripLens)
            return;

        m_delayBeforGripLens = delayBeforGripLens;
        emit delayBeforGripLensChanged(m_delayBeforGripLens);
    }

    void setDelayAfterGripLens(int delayAfterGripLens)
    {
        if (m_delayAfterGripLens == delayAfterGripLens)
            return;

        m_delayAfterGripLens = delayAfterGripLens;
        emit delayAfterGripLensChanged(m_delayAfterGripLens);
    }

signals:
    void ZOffsetChanged(double ZOffset);
    void LensSoftlandingVelChanged(double ZOffset);
    void LensSoftlandingForceChanged(double ZOffset);
    void motorXNameChanged(QString motorXName);
    void motorYNameChanged(QString motorYName);
    void motorZNameChanged(QString motorZName);
    void lutVacuumNameChanged(QString lutVacuumName);
    void sutVacuumNameChanged(QString sutVacuumName);
    void cylinderNameChanged(QString cylinderName);
    void sutDownLookLocationNameChanged(QString sutDownLookLocationName);
    void mushroomLocationNameChanged(QString mushroomLocationName);
    void lutGripperLoactionNameChanged(QString lutGripperLoactionName);
    void delayBeforGripLensChanged(int delayBeforGripLens);
    void delayAfterGripLensChanged(int delayAfterGripLens);
};

class LSutState:public PropertyBase
{
    Q_OBJECT

    Q_PROPERTY(bool lutHasLens READ lutHasLens WRITE setLutHasLens NOTIFY lutHasLensChanged)
    Q_PROPERTY(bool lutHasNgLens READ lutHasNgLens WRITE setLutHasNgLens NOTIFY lutHasNgLensChanged)
    Q_PROPERTY(bool sutHasSensor READ sutHasSensor WRITE setSutHasSensor NOTIFY sutHasSensorChanged)
    Q_PROPERTY(bool sutHasNgSensor READ sutHasNgSensor WRITE setSutHasNgSensor NOTIFY sutHasNgSensorChanged)
    Q_PROPERTY(bool allowLoadSensor READ allowLoadSensor WRITE setAllowLoadSensor NOTIFY allowLoadSensorChanged)
    Q_PROPERTY(bool allowLoadLens READ allowLoadLens WRITE setAllowLoadLens NOTIFY allowLoadLensChanged)

    Q_PROPERTY(bool waitLoading READ waitLoading WRITE setWaitLoading NOTIFY waitLoadingChanged)
    Q_PROPERTY(bool waitAAProcess READ waitAAProcess WRITE setWaitAAProcess NOTIFY waitAAProcessChanged)
    Q_PROPERTY(bool hasProduct READ hasProduct WRITE setHasProduct NOTIFY hasProductChanged)
    Q_PROPERTY(bool hasNgProduct READ hasNgProduct WRITE setHasNgProduct NOTIFY hasNgProductChanged)

public:
    bool lutHasLens() const
    {
        return m_lutHasLens;
    }

    bool sutHasSensor() const
    {
        return m_sutHasSensor;
    }

    bool sutHasNgSensor() const
    {
        return m_sutHasNgSensor;
    }


    bool allowLoadSensor() const
    {
        return m_allowLoadSensor;
    }

    bool waitLoading() const
    {
        return m_waitLoading;
    }


    bool allowLoadLens() const
    {
        return m_allowLoadLens;
    }

    bool hasProduct() const
    {
        return m_hasProduct;
    }

    bool lutHasNgLens() const
    {
        return m_lutHasNgLens;
    }

    bool hasNgProduct() const
    {
        return m_hasNgProduct;
    }

    bool waitAAProcess() const
    {
        return m_waitAAProcess;
    }

public slots:
    void setLutHasLens(bool lutHasLens)
    {
        if (m_lutHasLens == lutHasLens)
            return;

        m_lutHasLens = lutHasLens;
        emit lutHasLensChanged(m_lutHasLens);
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


    void setAllowLoadSensor(bool allowLoadSensor)
    {
        if (m_allowLoadSensor == allowLoadSensor)
            return;

        m_allowLoadSensor = allowLoadSensor;
        emit allowLoadSensorChanged(m_allowLoadSensor);
    }

    void setWaitLoading(bool waitLoading)
    {
        if (m_waitLoading == waitLoading)
            return;

        m_waitLoading = waitLoading;
        emit waitLoadingChanged(m_waitLoading);
    }

    void setAllowLoadLens(bool allowLoadLens)
    {
        if (m_allowLoadLens == allowLoadLens)
            return;

        m_allowLoadLens = allowLoadLens;
        emit allowLoadLensChanged(m_allowLoadLens);
    }

    void setHasProduct(bool hasProduct)
    {
        if (m_hasProduct == hasProduct)
            return;

        m_hasProduct = hasProduct;
        emit hasProductChanged(m_hasProduct);
    }

    void setLutHasNgLens(bool lutHasNgLens)
    {
        if (m_lutHasNgLens == lutHasNgLens)
            return;

        m_lutHasNgLens = lutHasNgLens;
        emit lutHasNgLensChanged(m_lutHasNgLens);
    }

    void setHasNgProduct(bool hasNgProduct)
    {
        if (m_hasNgProduct == hasNgProduct)
            return;

        m_hasNgProduct = hasNgProduct;
        emit hasNgProductChanged(m_hasNgProduct);
    }

    void setWaitAAProcess(bool waitAAProcess)
    {
        if (m_waitAAProcess == waitAAProcess)
            return;

        m_waitAAProcess = waitAAProcess;
        emit waitAAProcessChanged(m_waitAAProcess);
    }

signals:

    void lutHasLensChanged(bool lutHasLens);

    void sutHasSensorChanged(bool sutHasSensor);

    void sutHasNgSensorChanged(bool sutHasNgSensor);

    void allowLoadSensorChanged(bool allowLoadSensor);

    void waitLoadingChanged(bool waitLoading);

    void allowLoadLensChanged(bool allowLoadLens);

    void hasProductChanged(bool hasProduct);

    void lutHasNgLensChanged(bool lutHasNgLens);

    void hasNgProductChanged(bool hasNgProduct);

    void waitAAProcessChanged(bool waitAAProcess);

private:
    bool m_lutHasLens = false;
    bool m_sutHasSensor = false;
    bool m_sutHasNgSensor = false;
    bool m_allowLoadSensor = false;
    bool m_waitLoading = false;
    bool m_allowLoadLens = false;
    bool m_hasProduct = false;
    bool m_lutHasNgLens = false;
    bool m_hasNgProduct = false;
    bool m_waitAAProcess = false;
};

#endif // SINGLEHEADLSUTPARAMETER_H
