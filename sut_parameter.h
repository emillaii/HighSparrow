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
};

#endif // SUT_PARAMETER_H
