#ifndef PICKARMXXYPPARAMETER_H
#define PICKARMXXYPPARAMETER_H

#include "propertybase.h"



class PickArmXXYPParameter:public PropertyBase
{
    Q_OBJECT
public:
    PickArmXXYPParameter():PropertyBase(){}
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorTrayName READ motorTrayName WRITE setMotorTrayName NOTIFY motorTrayNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(QString motorZName READ motorZName WRITE setMotorZName NOTIFY motorZNameChanged)
    Q_PROPERTY(QString motorTName READ motorTName WRITE setMotorTName NOTIFY motorTNameChanged)
    Q_PROPERTY(QString vacuumName READ vacuumName WRITE setVacuumName NOTIFY vacuumNameChanged)

    QString motorTrayName() const
    {
        return m_motorTrayName;
    }

    QString motorYName() const
    {
        return m_motorYName;
    }

    QString motorZName() const
    {
        return m_motorZName;
    }

    QString motorXName() const
    {
        return m_motorXName;
    }

    QString motorTName() const
    {
        return m_motorTName;
    }

    QString vacuumName() const
    {
        return m_vacuumName;
    }

public slots:

    void setMotorTrayName(QString motorTrayName)
    {
        if (m_motorTrayName == motorTrayName)
            return;

        m_motorTrayName = motorTrayName;
        emit motorTrayNameChanged(m_motorTrayName);
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

    void setMotorXName(QString motorXName)
    {
        if (m_motorXName == motorXName)
            return;

        m_motorXName = motorXName;
        emit motorXNameChanged(m_motorXName);
    }

    void setMotorTName(QString motorTName)
    {
        if (m_motorTName == motorTName)
            return;

        m_motorTName = motorTName;
        emit motorTNameChanged(m_motorTName);
    }

    void setVacuumName(QString vacuumName)
    {
        if (m_vacuumName == vacuumName)
            return;

        m_vacuumName = vacuumName;
        emit vacuumNameChanged(m_vacuumName);
    }

signals:
    void visonPositionXChanged(QString motorXName);
    void motorTrayNameChanged(QString motorTrayName);

    void motorYNameChanged(QString motorYName);

    void motorZNameChanged(QString motorZName);

    void motorXNameChanged(QString motorXName);

    void motorTNameChanged(QString motorTName);

    void vacuumNameChanged(QString vacuumName);

private:
    QString m_motorXName = "LPA_X";
    QString m_motorTrayName = "LTL_X";
    QString m_motorYName = "LPA_Y";
    QString m_motorZName = "LPA_Z";
    QString m_motorTName = "LPA_T";
    QString m_vacuumName = "LENS_V";
};

#endif // PICKARMXXYPPARAMETER_H
