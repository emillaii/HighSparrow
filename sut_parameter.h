#ifndef SUT_PARAMETER_H
#define SUT_PARAMETER_H
#include "propertybase.h"

class SutParameter : public PropertyBase
{
    Q_OBJECT
public:
    explicit SutParameter(){}
    Q_PROPERTY(double OCX READ OCX WRITE setOCX NOTIFY paramsChanged)
    Q_PROPERTY(double OCY READ OCY WRITE setOCY NOTIFY paramsChanged)
    Q_PROPERTY(double OCZ READ OCZ WRITE setOCZ NOTIFY paramsChanged)
    Q_PROPERTY(double LoadX READ LoadX WRITE setLoadX NOTIFY paramsChanged)
    Q_PROPERTY(double LoadY READ LoadY WRITE setLoadY NOTIFY paramsChanged)
    Q_PROPERTY(double LoadZ READ LoadZ WRITE setLoadZ NOTIFY paramsChanged)
    Q_PROPERTY(int Lighting READ Lighting WRITE setLighting NOTIFY paramsChanged)
    Q_PROPERTY(QString prName READ prName WRITE setPRName NOTIFY paramsChanged)
    Q_PROPERTY(double Force READ Force WRITE setForce NOTIFY paramsChanged)
    double OCX() const
    {
        return m_OCX;
    }
    double OCY() const
    {
        return m_OCY;
    }

    double OCZ() const
    {
        return m_OCZ;
    }

    double LoadX() const
    {
        return m_LoadX;
    }

    double LoadY() const
    {
        return m_LoadY;
    }

    double LoadZ() const
    {
        return m_LoadZ;
    }

    int Lighting() const
    {
        return m_Lighting;
    }

    double Force() const
    {
        return m_Force;
    }

    QString prName() const
    {
        return m_prName;
    }

signals:
    void paramsChanged();

public slots:
    void setOCX(double OCX)
    {
        m_OCX = OCX;
        emit paramsChanged();
    }
    void setOCY(double OCY)
    {
        m_OCY = OCY;
        emit paramsChanged();
    }

    void setOCZ(double OCZ)
    {
        m_OCZ = OCZ;
        emit paramsChanged();
    }

    void setLoadX(double LoadX)
    {
        m_LoadX = LoadX;
        emit paramsChanged();
    }

    void setLoadY(double LoadY)
    {
        m_LoadY = LoadY;
        emit paramsChanged();
    }

    void setLoadZ(double LoadZ)
    {
        m_LoadZ = LoadZ;
        emit paramsChanged();
    }

    void setLighting(int Lighting)
    {
        m_Lighting = Lighting;
        emit paramsChanged();
    }

    void setForce(double Force)
    {
        m_Force = Force;
        emit paramsChanged();
    }

    void setPRName(QString prName)
    {
        if (m_prName == prName)
            return;
        m_prName = prName;
        emit paramsChanged();
    }

private:
    double m_OCX = 0;
    double m_OCY = 0;
    double m_OCZ = 0;
    double m_LoadX = 0;
    double m_LoadY = 0;
    double m_LoadZ = 0;
    int m_Lighting = 0;
    double m_Force = 0;
    QString m_prName = "";
};

#endif // SUT_PARAMETER_H
