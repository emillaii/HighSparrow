#ifndef LUT_PARAMERTER_H
#define LUT_PARAMERTER_H
#include "propertybase.h"

class LutParameter:public PropertyBase
{
public:
    LutParameter():PropertyBase(){}
    Q_PROPERTY(double Pick1X READ Pick1X WRITE setPick1X NOTIFY paramChanged)
    Q_PROPERTY(double Pick1Y READ Pick1Y WRITE setPick1Y NOTIFY paramChanged)
    Q_PROPERTY(double Pick1Z READ Pick1Z WRITE setPick1Z NOTIFY paramChanged)
    Q_PROPERTY(double UnPick1X READ UnPick1X WRITE setUnPick1X NOTIFY paramChanged)
    Q_PROPERTY(double UnPick1Y READ UnPick1Y WRITE setUnPick1Y NOTIFY paramChanged)
    Q_PROPERTY(double UnPick1Z READ UnPick1Z WRITE setUnPick1Z NOTIFY paramChanged)
    Q_PROPERTY(double Pick2X READ Pick2X WRITE setPick2X NOTIFY paramChanged)
    Q_PROPERTY(double Pick2Y READ Pick2Y WRITE setPick2Y NOTIFY paramChanged)
    Q_PROPERTY(double Pick2Z READ Pick2Z WRITE setPick2Z NOTIFY paramChanged)
    Q_PROPERTY(double UnPick2X READ UnPick2X WRITE setUnPick2X NOTIFY paramChanged)
    Q_PROPERTY(double UnPick2Y READ UnPick2Y WRITE setUnPick2Y NOTIFY paramChanged)
    Q_PROPERTY(double UnPick2Z READ UnPick2Z WRITE setUnPick2Z NOTIFY paramChanged)
    Q_PROPERTY(double LoadX READ LoadX WRITE setLoadX NOTIFY paramChanged)
    Q_PROPERTY(double LoadY READ LoadY WRITE setLoadY NOTIFY paramChanged)
    Q_PROPERTY(double LoadZ READ LoadZ WRITE setLoadZ NOTIFY paramChanged)
    Q_PROPERTY(double PickForce READ PickForce WRITE setPickForce NOTIFY paramChanged)
    Q_PROPERTY(int Lighting READ Lighting WRITE setLighting NOTIFY paramChanged)

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
        m_Pick1X = Pick1X;
        emit paramChanged();
    }
    void setPick1Y(double Pick1Y)
    {
        m_Pick1Y = Pick1Y;
        emit paramChanged();
    }

    void setPick1Z(double Pick1Z)
    {
        m_Pick1Z = Pick1Z;
        emit paramChanged();
    }

    void setPick2X(double Pick2X)
    {
        m_Pick2X = Pick2X;
        emit paramChanged();
    }

    void setPick2Y(double Pick2Y)
    {
        m_Pick2Y = Pick2Y;
        emit paramChanged();
    }

    void setPick2Z(double Pick2Z)
    {
        m_Pick2Z = Pick2Z;
        emit paramChanged();
    }

    void setLoadX(double LoadX)
    {
        m_LoadX = LoadX;
        emit paramChanged();
    }

    void setLoadY(double LoadY)
    {
        m_LoadY = LoadY;
        emit paramChanged();
    }

    void setLoadZ(double LoadZ)
    {
        m_LoadZ = LoadZ;
        emit paramChanged();
    }

    void setPickForce(double PickForce)
    {
        m_PickForce = PickForce;
        emit paramChanged();
    }

    void setLighting(int Lighting)
    {
        if (m_Lighting == Lighting)
            return;
        m_Lighting = Lighting;
        emit paramChanged();
    }

    void setUnPick1X(double UnPick1X)
    {
        m_UnPick1X = UnPick1X;
        emit paramChanged();
    }

    void setUnPick1Y(double UnPick1Y)
    {
        m_UnPick1Y = UnPick1Y;
        emit paramChanged();
    }

    void setUnPick1Z(double UnPick1Z)
    {
        m_UnPick1Z = UnPick1Z;
        emit paramChanged();
    }

    void setUnPick2X(double UnPick2X)
    {
        m_UnPick2X = UnPick2X;
        emit paramChanged();
    }

    void setUnPick2Y(double UnPick2Y)
    {
        m_UnPick2Y = UnPick2Y;
        emit paramChanged();
    }

    void setUnPick2Z(double UnPick2Z)
    {
        m_UnPick2Z = UnPick2Z;
        emit paramChanged();
    }

signals:
    void paramChanged();

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
