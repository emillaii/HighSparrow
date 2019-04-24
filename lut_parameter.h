#ifndef LUT_PARAMERTER_H
#define LUT_PARAMERTER_H
#include "propertybase.h"

class LutParameter:public PropertyBase
{
    Q_OBJECT
public:
    LutParameter():PropertyBase(){}
    Q_PROPERTY(double pickForce READ pickForce WRITE setPickForce NOTIFY paramsChanged)
    Q_PROPERTY(int Lighting READ Lighting WRITE setLighting NOTIFY paramsChanged)
    Q_PROPERTY(QString prName READ prName WRITE setPRName NOTIFY paramsChanged)
    Q_PROPERTY(int UpDnLookLighting READ UpDnLookLighting WRITE setUpDnLookLighting NOTIFY paramsChanged)
    Q_PROPERTY(QString upDownLookPrName READ upDownLookPrName WRITE setUpDownLookPrName NOTIFY paramsChanged)
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(QString motorZName READ motorZName WRITE setMotorZName NOTIFY motorZNameChanged)
    Q_PROPERTY(QString vacuum1Name READ vacuum1Name WRITE setVacuum1Name NOTIFY vacuum1NameChanged)
    Q_PROPERTY(QString vacuum2Name READ vacuum2Name WRITE setVacuum2Name NOTIFY vacuum2NameChanged)
    double pickForce() const
    {
        return m_PickForce;
    }

    int Lighting() const
    {
        return m_Lighting;
    }

    QString prName() const
    {
        return m_prName;
    }

    int UpDnLookLighting() const
    {
        return m_UpDnLookLighting;
    }

    QString upDownLookPrName() const
    {
        return m_upDownLookPrName;
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

    QString vacuum1Name() const
    {
        return m_vacuum1Name;
    }

    QString vacuum2Name() const
    {
        return m_vacuum2Name;
    }

public slots:
    void setPickForce(double PickForce)
    {
        m_PickForce = PickForce;
        emit paramsChanged();
    }

    void setLighting(int Lighting)
    {
        if (m_Lighting == Lighting)
            return;
        m_Lighting = Lighting;
        emit paramsChanged();
    }

    void setPRName(QString prName)
    {
        if (m_prName == prName)
            return;
        m_prName = prName;
        emit paramsChanged();
    }

    void setUpDnLookLighting(int UpDnLookLighting)
    {
        if (m_UpDnLookLighting == UpDnLookLighting)
            return;

        m_UpDnLookLighting = UpDnLookLighting;
        emit paramsChanged();
    }

    void setUpDownLookPrName(QString upDownLookPrName)
    {
        if (m_upDownLookPrName == upDownLookPrName)
            return;

        m_upDownLookPrName = upDownLookPrName;
        emit paramsChanged();
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

    void setVacuum1Name(QString vacuum1Name)
    {
        if (m_vacuum1Name == vacuum1Name)
            return;

        m_vacuum1Name = vacuum1Name;
        emit vacuum1NameChanged(m_vacuum1Name);
    }

    void setVacuum2Name(QString vacuum2Name)
    {
        if (m_vacuum2Name == vacuum2Name)
            return;

        m_vacuum2Name = vacuum2Name;
        emit vacuum2NameChanged(m_vacuum2Name);
    }

signals:
    void paramsChanged();

    void motorXNameChanged(QString motorXName);

    void motorYNameChanged(QString motorYName);

    void motorZNameChanged(QString motorZName);

    void vacuum1NameChanged(QString vacuum1Name);

    void vacuum2NameChanged(QString vacuum2Name);

private:
    double m_PickForce = 0;
    int m_Lighting = 0;
    QString m_prName = "";
    int m_UpDnLookLighting = 0;
    QString m_upDownLookPrName = "";
    QString m_motorXName = "LUT_X";
    QString m_motorYName = "LUT_Y";
    QString m_motorZName = "LUT_Z";
    QString m_vacuum1Name = "LUT_V1";
    QString m_vacuum2Name = "LUT_V2";
};

#endif // LUT_PARAMERTER_H
