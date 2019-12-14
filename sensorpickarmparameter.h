#ifndef SENSORPICKARMPARAMETER_H
#define SENSORPICKARMPARAMETER_H

#include "propertybase.h"



class SensorPickArmParameter: public PropertyBase
{
    Q_OBJECT
public:
    SensorPickArmParameter():PropertyBase(){}
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(QString motorZName READ motorZName WRITE setMotorZName NOTIFY motorZNameChanged)
    Q_PROPERTY(QString motorTName READ motorTName WRITE setMotorTName NOTIFY motorTNameChanged)
    Q_PROPERTY(QString vacuumName READ vacuumName WRITE setVacuumName NOTIFY vacuumNameChanged)
    Q_PROPERTY(QString motorZ2Name READ motorZ2Name WRITE setMotorZ2Name NOTIFY motorZ2NameChanged)
    Q_PROPERTY(QString motorT2Name READ motorT2Name WRITE setMotorT2Name NOTIFY motorT2NameChanged)
    Q_PROPERTY(QString vacuum2Name READ vacuum2Name WRITE setVacuum2Name NOTIFY vacuum2NameChanged)
    Q_PROPERTY(QString spaVaccum1OutIoName READ spaVaccum1OutIoName WRITE setSpaVaccum1OutIoName NOTIFY spaVaccum1OutIoNameChanged)
    Q_PROPERTY(QString spaVaccum1InIoName READ spaVaccum1InIoName WRITE setSpaVaccum1InIoName NOTIFY spaVaccum1InIoNameChanged)
    Q_PROPERTY(QString spaVaccum2OutIoName READ spaVaccum2OutIoName WRITE setSpaVaccum2OutIoName NOTIFY spaVaccum2OutIoNameChanged)
    Q_PROPERTY(QString spaVaccum2InIoName READ spaVaccum2InIoName WRITE setSpaVaccum2InIoName NOTIFY spaVaccum2InIoNameChanged)
    Q_PROPERTY(double motor1SafeHeight READ motor1SafeHeight WRITE setMotor1SafeHeight NOTIFY motor1SafeHeightChanged)
    Q_PROPERTY(double motor2SafeHeight READ motor2SafeHeight WRITE setMotor2SafeHeight NOTIFY motor2SafeHeightChanged)
    Q_PROPERTY(double processPositionError READ processPositionError WRITE setProcessPositionError NOTIFY processPositionErrorChanged)
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

    double motor1SafeHeight() const
    {
        return m_motor1SafeHeight;
    }

    double motor2SafeHeight() const
    {
        return m_motor2SaftHeight;
    }

    double processPositionError() const
    {
        return m_processPositionError;
    }

    QString spaVaccum1OutIoName() const
    {
        return m_spaVaccum1OutIoName;
    }

    QString spaVaccum1InIoName() const
    {
        return m_spaVaccum1InIoName;
    }

    QString spaVaccum2OutIoName() const
    {
        return m_spaVaccum2OutIoName;
    }

    QString spaVaccum2InIoName() const
    {
        return m_spaVaccum2InIoName;
    }

public slots:
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

    void setMotor1SafeHeight(double motor1SafeHeight)
    {
        
        if (qFuzzyCompare(m_motor1SafeHeight, motor1SafeHeight))
            return;

        m_motor1SafeHeight = motor1SafeHeight;
        emit motor1SafeHeightChanged(m_motor1SafeHeight);
    }

    void setMotor2SafeHeight(double motor2SaftHeight)
    {
        
        if (qFuzzyCompare(m_motor2SaftHeight, motor2SaftHeight))
            return;

        m_motor2SaftHeight = motor2SaftHeight;
        emit motor2SafeHeightChanged(m_motor2SaftHeight);
    }

    void setProcessPositionError(double processPositionError)
    {
        
        if (qFuzzyCompare(m_processPositionError, processPositionError))
            return;

        m_processPositionError = processPositionError;
        emit processPositionErrorChanged(m_processPositionError);
    }

    void setSpaVaccum1OutIoName(QString spaVaccum1OutIoName)
    {
        if (m_spaVaccum1OutIoName == spaVaccum1OutIoName)
            return;

        m_spaVaccum1OutIoName = spaVaccum1OutIoName;
        emit spaVaccum1OutIoNameChanged(m_spaVaccum1OutIoName);
    }

    void setSpaVaccum1InIoName(QString spaVaccum1InIoName)
    {
        if (m_spaVaccum1InIoName == spaVaccum1InIoName)
            return;

        m_spaVaccum1InIoName = spaVaccum1InIoName;
        emit spaVaccum1InIoNameChanged(m_spaVaccum1InIoName);
    }

    void setSpaVaccum2OutIoName(QString spaVaccum2OutIoName)
    {
        if (m_spaVaccum2OutIoName == spaVaccum2OutIoName)
            return;

        m_spaVaccum2OutIoName = spaVaccum2OutIoName;
        emit spaVaccum2OutIoNameChanged(m_spaVaccum2OutIoName);
    }

    void setSpaVaccum2InIoName(QString spaVaccum2InIoName)
    {
        if (m_spaVaccum2InIoName == spaVaccum2InIoName)
            return;

        m_spaVaccum2InIoName = spaVaccum2InIoName;
        emit spaVaccum2InIoNameChanged(m_spaVaccum2InIoName);
    }

signals:
    void motorXNameChanged(QString motorXName);

    void motorYNameChanged(QString motorYName);

    void motorZNameChanged(QString motorZName);

    void motorTNameChanged(QString motorTName);

    void vacuumNameChanged(QString vacuumName);

    void motorZ2NameChanged(QString motorZ2Name);

    void motorT2NameChanged(QString motorT2Name);

    void vacuum2NameChanged(QString vacuum2Name);

    void motor1SafeHeightChanged(double motor1SafeHeight);

    void motor2SafeHeightChanged(double motor2SafeHeight);

    void processPositionErrorChanged(double processPositionError);

    void spaVaccum1OutIoNameChanged(QString spaVaccum1OutIoName);

    void spaVaccum1InIoNameChanged(QString spaVaccum1InIoName);

    void spaVaccum2OutIoNameChanged(QString spaVaccum2OutIoName);

    void spaVaccum2InIoNameChanged(QString spaVaccum2InIoName);

private:
    QString m_motorXName = "";
    QString m_motorYName = "";
    QString m_motorZName = "";
    QString m_motorTName = "";
    QString m_vacuumName = "";
    QString m_motorZ2Name = "";
    QString m_motorT2Name = "";
    QString m_vacuum2Name = "";
    double m_motor1SafeHeight = 0;
    double m_motor2SaftHeight = 0;
    double m_processPositionError = 1;
    QString m_spaVaccum1OutIoName;
    QString m_spaVaccum1InIoName;
    QString m_spaVaccum2OutIoName;
    QString m_spaVaccum2InIoName;
};

#endif // SENSORPICKARMPARAMETER_H
