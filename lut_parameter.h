#ifndef LUT_PARAMERTER_H
#define LUT_PARAMERTER_H

#include "PropertyBase.h"

class LutParameter:public PropertyBase
{
public:
    LutParameter():PropertyBase(){}
    Q_PROPERTY(double Pick1X READ Pick1X WRITE setPick1X NOTIFY Pick1XChanged)
    Q_PROPERTY(double Pick1Y READ Pick1Y WRITE setPick1Y NOTIFY Pick1YChanged)
    Q_PROPERTY(double Pick1Z READ Pick1Z WRITE setPick1Z NOTIFY Pick1ZChanged)
    Q_PROPERTY(double UnPick1X READ UnPick1X WRITE setUnPick1X NOTIFY UnPick1XChanged)
    Q_PROPERTY(double UnPick1Y READ UnPick1Y WRITE setUnPick1Y NOTIFY UnPick1YChanged)
    Q_PROPERTY(double UnPick1Z READ UnPick1Z WRITE setUnPick1Z NOTIFY UnPick1ZChanged)
    Q_PROPERTY(double Pick2X READ Pick2X WRITE setPick2X NOTIFY Pick2XChanged)
    Q_PROPERTY(double Pick2Y READ Pick2Y WRITE setPick2Y NOTIFY Pick2YChanged)
    Q_PROPERTY(double Pick2Z READ Pick2Z WRITE setPick2Z NOTIFY Pick2ZChanged)
    Q_PROPERTY(double UnPick2X READ UnPick2X WRITE setUnPick2X NOTIFY UnPick2XChanged)
    Q_PROPERTY(double UnPick2Y READ UnPick2Y WRITE setUnPick2Y NOTIFY UnPick2YChanged)
    Q_PROPERTY(double UnPick2Z READ UnPick2Z WRITE setUnPick2Z NOTIFY UnPick2ZChanged)
    Q_PROPERTY(double LoadX READ LoadX WRITE setLoadX NOTIFY LoadXChanged)
    Q_PROPERTY(double LoadY READ LoadY WRITE setLoadY NOTIFY LoadYChanged)
    Q_PROPERTY(double LoadZ READ LoadZ WRITE setLoadZ NOTIFY LoadZChanged)
    Q_PROPERTY(double PickForce READ PickForce WRITE setPickForce NOTIFY PickForceChanged)
    Q_PROPERTY(int Lighting READ Lighting WRITE setLighting NOTIFY LightingChanged)

    double Pick1X() const
    {
        return m_Pick1X;
    }
    double Pick1Y() const
    {
        return m_Pick1Y;
    }

    double Pick1Z() const
    {
        return m_Pick1Z;
    }

    double Pick2X() const
    {
        return m_Pick2X;
    }

    double Pick2Y() const
    {
        return m_Pick2Y;
    }

    double Pick2Z() const
    {
        return m_Pick2Z;
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

    double PickForce() const
    {
        return m_PickForce;
    }

    int Lighting() const
    {
        return m_Lighting;
    }

    double UnPick1X() const
    {
        return m_UnPick1X;
    }

    double UnPick1Y() const
    {
        return m_UnPick1Y;
    }

    double UnPick1Z() const
    {
        return m_UnPick1Z;
    }

    double UnPick2X() const
    {
        return m_UnPick2X;
    }

    double UnPick2Y() const
    {
        return m_UnPick2Y;
    }

    double UnPick2Z() const
    {
        return m_UnPick2Z;
    }

public slots:
    void setPick1X(double Pick1X)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_Pick1X, Pick1X))
            return;

        m_Pick1X = Pick1X;
        emit Pick1XChanged(m_Pick1X);
    }
    void setPick1Y(double Pick1Y)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_Pick1Y, Pick1Y))
            return;

        m_Pick1Y = Pick1Y;
        emit Pick1YChanged(m_Pick1Y);
    }

    void setPick1Z(double Pick1Z)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_Pick1Z, Pick1Z))
            return;

        m_Pick1Z = Pick1Z;
        emit Pick1ZChanged(m_Pick1Z);
    }

    void setPick2X(double Pick2X)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_Pick2X, Pick2X))
            return;

        m_Pick2X = Pick2X;
        emit Pick2XChanged(m_Pick2X);
    }

    void setPick2Y(double Pick2Y)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_Pick2Y, Pick2Y))
            return;

        m_Pick2Y = Pick2Y;
        emit Pick2YChanged(m_Pick2Y);
    }

    void setPick2Z(double Pick2Z)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_Pick2Z, Pick2Z))
            return;

        m_Pick2Z = Pick2Z;
        emit Pick2ZChanged(m_Pick2Z);
    }

    void setLoadX(double LoadX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_LoadX, LoadX))
            return;

        m_LoadX = LoadX;
        emit LoadXChanged(m_LoadX);
    }

    void setLoadY(double LoadY)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_LoadY, LoadY))
            return;

        m_LoadY = LoadY;
        emit LoadYChanged(m_LoadY);
    }

    void setLoadZ(double LoadZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_LoadZ, LoadZ))
            return;

        m_LoadZ = LoadZ;
        emit LoadZChanged(m_LoadZ);
    }

    void setPickForce(double PickForce)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_PickForce, PickForce))
            return;

        m_PickForce = PickForce;
        emit PickForceChanged(m_PickForce);
    }

    void setLighting(int Lighting)
    {
        if (m_Lighting == Lighting)
            return;

        m_Lighting = Lighting;
        emit LightingChanged(m_Lighting);
    }

    void setUnPick1X(double UnPick1X)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_UnPick1X, UnPick1X))
            return;

        m_UnPick1X = UnPick1X;
        emit UnPick1XChanged(m_UnPick1X);
    }

    void setUnPick1Y(double UnPick1Y)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_UnPick1Y, UnPick1Y))
            return;

        m_UnPick1Y = UnPick1Y;
        emit UnPick1YChanged(m_UnPick1Y);
    }

    void setUnPick1Z(double UnPick1Z)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_UnPick1Z, UnPick1Z))
            return;

        m_UnPick1Z = UnPick1Z;
        emit UnPick1ZChanged(m_UnPick1Z);
    }

    void setUnPick2X(double UnPick2X)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_UnPick2X, UnPick2X))
            return;

        m_UnPick2X = UnPick2X;
        emit UnPick2XChanged(m_UnPick2X);
    }

    void setUnPick2Y(double UnPick2Y)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_UnPick2Y, UnPick2Y))
            return;

        m_UnPick2Y = UnPick2Y;
        emit UnPick2YChanged(m_UnPick2Y);
    }

    void setUnPick2Z(double UnPick2Z)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_UnPick2Z, UnPick2Z))
            return;

        m_UnPick2Z = UnPick2Z;
        emit UnPick2ZChanged(m_UnPick2Z);
    }

signals:
    void Pick1XChanged(double Pick1X);
    void Pick1YChanged(double Pick1Y);

    void Pick1ZChanged(double Pick1Z);

    void AA1XChanged(double AA1X);

    void AA1YChanged(double AA1Y);

    void AA1ZChanged(double AA1Z);

    void Pick2XChanged(double Pick2X);

    void Pick2YChanged(double Pick2Y);

    void Pick2ZChanged(double Pick2Z);

    void AA2XChanged(double AA2X);

    void AA2YChanged(double AA2Y);

    void AA2ZChanged(double AA2Z);

    void LoadXChanged(double LoadX);

    void LoadYChanged(double LoadY);

    void LoadZChanged(double LoadZ);

    void PickForceChanged(double PickForce);

    void LightingChanged(int Lighting);

    void UnPick1XChanged(double UnPick1X);

    void UnPick1YChanged(double UnPick1Y);

    void UnPick1ZChanged(double UnPick1Z);

    void UnPick2XChanged(double UnPick2X);

    void UnPick2YChanged(double UnPick2Y);

    void UnPick2ZChanged(double UnPick2Z);

private:
    double m_Pick1X = 0;
    double m_Pick1Y = 0;
    double m_Pick1Z = 0;
    double m_Pick2X = 0;
    double m_Pick2Y = 0;
    double m_Pick2Z = 0;
    double m_LoadX = 0;
    double m_LoadY = 0;
    double m_LoadZ = 0;
    double m_PickForce = 0;
    int m_Lighting = 0;
    double m_UnPick1X = 0;
    double m_UnPick1Y = 0;
    double m_UnPick1Z = 0;
    double m_UnPick2X = 0;
    double m_UnPick2Y = 0;
    double m_UnPick2Z = 0;
};

#endif // LUT_PARAMERTER_H
