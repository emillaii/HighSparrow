#ifndef LUT_PARAMERTER_H
#define LUT_PARAMERTER_H
#include "propertybase.h"

class LutParameter:public PropertyBase
{
    Q_OBJECT
public:
    LutParameter():PropertyBase(){}
    Q_PROPERTY(double PickForce READ PickForce WRITE setPickForce NOTIFY paramsChanged)
    Q_PROPERTY(int Lighting READ Lighting WRITE setLighting NOTIFY paramsChanged)
    Q_PROPERTY(QString prName READ prName WRITE setPRName NOTIFY paramsChanged)
    Q_PROPERTY(int UpDnLookLighting READ UpDnLookLighting WRITE setUpDnLookLighting NOTIFY paramsChanged)
    Q_PROPERTY(QString upDownLookPrName READ upDownLookPrName WRITE setUpDownLookPrName NOTIFY paramsChanged)

    double PickForce() const
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

signals:
    void paramsChanged();

private:
    double m_PickForce = 0;
    int m_Lighting = 0;
    QString m_prName = "";
    int m_UpDnLookLighting = 0;
    QString m_upDownLookPrName = 0;
};

#endif // LUT_PARAMERTER_H
