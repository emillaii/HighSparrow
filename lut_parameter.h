#ifndef LUT_PARAMERTER_H
#define LUT_PARAMERTER_H
#include "propertybase.h"

class LutParameter:public PropertyBase
{
    Q_OBJECT
public:
    LutParameter():PropertyBase(){}
    Q_PROPERTY(double pickForce READ pickForce WRITE setPickForce NOTIFY paramsChanged)
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(QString motorZName READ motorZName WRITE setMotorZName NOTIFY motorZNameChanged)
    Q_PROPERTY(QString vacuum1Name READ vacuum1Name WRITE setVacuum1Name NOTIFY vacuum1NameChanged)
    Q_PROPERTY(QString vacuum2Name READ vacuum2Name WRITE setVacuum2Name NOTIFY vacuum2NameChanged)
    Q_PROPERTY(QString uplookLocationName READ uplookLocationName WRITE setUplookLocationName NOTIFY uplookLocationNameChanged)
    Q_PROPERTY(QString loadlookLocationName READ loadlookLocationName WRITE setLoadlookLocationName NOTIFY loadlookLocationNameChanged)
    Q_PROPERTY(QString mushroomLocationName READ mushroomLocationName WRITE setMushroomLocationName NOTIFY mushroomLocationNameChanged)
    double pickForce() const
    {
        return m_PickForce;
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

    QString uplookLocationName() const
    {
        return m_uplookLocationName;
    }

    QString loadlookLocationName() const
    {
        return m_loadlookLocationName;
    }

    QString mushroomLocationName() const
    {
        return m_mushroomLocationName;
    }

public slots:
    void setPickForce(double PickForce)
    {
        m_PickForce = PickForce;
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

    void setUplookLocationName(QString uplookLocationName)
    {
        if (m_uplookLocationName == uplookLocationName)
            return;

        m_uplookLocationName = uplookLocationName;
        emit uplookLocationNameChanged(m_uplookLocationName);
    }

    void setLoadlookLocationName(QString loadlookLocationName)
    {
        if (m_loadlookLocationName == loadlookLocationName)
            return;

        m_loadlookLocationName = loadlookLocationName;
        emit loadlookLocationNameChanged(m_loadlookLocationName);
    }

    void setMushroomLocationName(QString mushroomLocationName)
    {
        if (m_mushroomLocationName == mushroomLocationName)
            return;

        m_mushroomLocationName = mushroomLocationName;
        emit mushroomLocationNameChanged(m_mushroomLocationName);
    }

signals:
    void paramsChanged();

    void motorXNameChanged(QString motorXName);

    void motorYNameChanged(QString motorYName);

    void motorZNameChanged(QString motorZName);

    void vacuum1NameChanged(QString vacuum1Name);

    void vacuum2NameChanged(QString vacuum2Name);

    void uplookLocationNameChanged(QString uplookLocationName);

    void loadlookLocationNameChanged(QString loadlookLocationName);

    void mushroomLocationNameChanged(QString mushroomLocationName);

private:
    double m_PickForce = 0;
    QString m_motorXName = "LUT_X";
    QString m_motorYName = "LUT_Y";
    QString m_motorZName = "LUT_Z";
    QString m_vacuum1Name = "LUT_V1";
    QString m_vacuum2Name = "LUT_V2";
    QString m_uplookLocationName = "";
    QString m_loadlookLocationName = "";
    QString m_mushroomLocationName = "";
};

#endif // LUT_PARAMERTER_H
