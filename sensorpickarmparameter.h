#ifndef SENSORPICKARMPARAMETER_H
#define SENSORPICKARMPARAMETER_H

#include "propertybase.h"



class SensorPickArmParameter: public PropertyBase
{
    Q_OBJECT
public:
    SensorPickArmParameter():PropertyBase(){}
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
//    Q_PROPERTY(QString motorTrayName READ motorTrayName WRITE setMotorTrayName NOTIFY motorTrayNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(QString motorZName READ motorZName WRITE setMotorZName NOTIFY motorZNameChanged)
    Q_PROPERTY(QString motorTName READ motorTName WRITE setMotorTName NOTIFY motorTNameChanged)
    Q_PROPERTY(QString vacuumName READ vacuumName WRITE setVacuumName NOTIFY vacuumNameChanged)
    Q_PROPERTY(QString motorZ2Name READ motorZ2Name WRITE setMotorZ2Name NOTIFY motorZ2NameChanged)
    Q_PROPERTY(QString motorT2Name READ motorT2Name WRITE setMotorT2Name NOTIFY motorT2NameChanged)
    Q_PROPERTY(QString vacuum2Name READ vacuum2Name WRITE setVacuum2Name NOTIFY vacuum2NameChanged)
    QString motorXName() const
    {
        return m_motorXName;
    }
//    QString motorTrayName() const
//    {
//        return m_motorTrayName;
//    }

    QString motorYName() const
    {
        return m_motorYName;
    }

    QString motorZName() const
    {
        return m_motorZName;
    }

    QString motorTName() const
    {
        return m_motorTName;
    }

    QString vacuumName() const
    {
        return m_vacuumName;
    }

    QString motorZ2Name() const
    {
        return m_motorZ2Name;
    }

    QString motorT2Name() const
    {
        return m_motorT2Name;
    }

    QString vacuum2Name() const
    {
        return m_vacuum2Name;
    }

public slots:
    void setMotorXName(QString motorXName)
    {
        if (m_motorXName == motorXName)
            return;

        m_motorXName = motorXName;
        emit motorXNameChanged(m_motorXName);
    }
//    void setMotorTrayName(QString motorTrayName)
//    {
//        if (m_motorTrayName == motorTrayName)
//            return;

//        m_motorTrayName = motorTrayName;
//        emit motorTrayNameChanged(m_motorTrayName);
//    }

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

    void setMotorZ2Name(QString motorZ2Name)
    {
        if (m_motorZ2Name == motorZ2Name)
            return;

        m_motorZ2Name = motorZ2Name;
        emit motorZ2NameChanged(m_motorZ2Name);
    }

    void setMotorT2Name(QString motorT2Name)
    {
        if (m_motorT2Name == motorT2Name)
            return;

        m_motorT2Name = motorT2Name;
        emit motorT2NameChanged(m_motorT2Name);
    }

    void setVacuum2Name(QString vacuum2Name)
    {
        if (m_vacuum2Name == vacuum2Name)
            return;

        m_vacuum2Name = vacuum2Name;
        emit vacuum2NameChanged(m_vacuum2Name);
    }

signals:
    void motorXNameChanged(QString motorXName);
//    void motorTrayNameChanged(QString motorTrayName);

    void motorYNameChanged(QString motorYName);

    void motorZNameChanged(QString motorZName);

    void motorTNameChanged(QString motorTName);

    void vacuumNameChanged(QString vacuumName);

    void motorZ2NameChanged(QString motorZ2Name);

    void motorT2NameChanged(QString motorT2Name);

    void vacuum2NameChanged(QString vacuum2Name);

private:
    QString m_motorXName = "";
//    QString m_motorTrayName = "";
    QString m_motorYName = "";
    QString m_motorZName = "";
    QString m_motorTName = "";
    QString m_vacuumName = "";
    QString m_motorZ2Name = "";
    QString m_motorT2Name = "";
    QString m_vacuum2Name = "";
};

#endif // SENSORPICKARMPARAMETER_H
