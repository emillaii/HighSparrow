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
    Q_PROPERTY(QString sylinderName READ sylinderName WRITE setsylinderName NOTIFY sylinderNameChanged)

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

    QString sylinderName() const
    {
        return m_sylinderName;
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

    void setsylinderName(QString sylinderName)
    {
        if (m_sylinderName == sylinderName)
            return;

        m_sylinderName = sylinderName;
        emit sylinderNameChanged(m_sylinderName);
    }

signals:
    void paramsChanged(double Force);

    void motorXNameChanged(QString motorXName);

    void motorYNameChanged(QString motorYName);

    void motorZNameChanged(QString motorZName);

    void vacuum1NameChanged(QString vacuumName);

    void sylinderNameChanged(QString sylinderName);

private:
    double m_Force = 0;
    QString m_motorXName = "SUT_X";
    QString m_motorYName = "SUT_Y";
    QString m_motorZName = "SUT_Z";
    QString m_vacuumName = "SUT_V";
    QString m_sylinderName = "SUT_P";
};

#endif // SUT_PARAMETER_H
