#ifndef POSITIONDEFINE_H
#define POSITIONDEFINE_H

#include "propertybase.h"

#include <qpoint.h>

class Position:public PropertyBase
{
    Q_OBJECT
public:
    Position():PropertyBase(){m_X = 0;m_Y = 0;}
    Position(QPointF point):PropertyBase(){m_X = point.x();m_Y = point.y();}
    void SetPosition(QPointF point){m_X = point.x();m_Y = point.y();}
    Position(double x,double y):PropertyBase(){m_X= x;m_Y= y;}
    QPointF ToPointF(){return QPointF(m_X,m_Y);}
    Q_PROPERTY(double X READ X WRITE setX NOTIFY XChanged)
    Q_PROPERTY(double Y READ Y WRITE setY NOTIFY YChanged)

    double X() const
    {
        return m_X;
    }
    double Y() const
    {
        return m_Y;
    }

public slots:
    void setX(double X)
    {
        if (qFuzzyCompare(m_X, X))
            return;

        m_X = X;
        emit XChanged(m_X);
    }
    void setY(double Y)
    {
        if (qFuzzyCompare(m_Y, Y))
            return;

        m_Y = Y;
        emit YChanged(m_Y);
    }

signals:
    void XChanged(double X);
    void YChanged(double Y);

private:
    double m_X;
    double m_Y;
};

struct mPositionT
{
    mPositionT() {}
    mPositionT(double x,double y,double theta){X= x;Y= y;Theta= theta;}
    double X = 0;
    double Y = 0;
    double Theta = 0;
};
class PositionT:public PropertyBase
{
    Q_OBJECT
public:
    PositionT() {}
    PositionT(double x,double y,double theta){m_X= x;m_Y= y;m_Theta= theta;}
    mPositionT tomPointT(){return mPositionT(m_X,m_Y,m_Theta);}
    void setValue(mPositionT value){setX(value.X);setY(value.Y);setTheta(value.Theta);}
    Q_PROPERTY(double X READ X WRITE setX NOTIFY XChanged)
    Q_PROPERTY(double Y READ Y WRITE setY NOTIFY YChanged)
    Q_PROPERTY(double Theta READ Theta WRITE setTheta NOTIFY ThetaChanged)
    double X() const
    {
        return m_X;
    }
    double Y() const
    {
        return m_Y;
    }
    double Theta() const
    {
        return m_Theta;
    }

public slots:
    void setX(double X)
    {
        if (qFuzzyCompare(m_X, X))
            return;

        m_X = X;
        emit XChanged(m_X);
    }
    void setY(double Y)
    {
        if (qFuzzyCompare(m_Y, Y))
            return;

        m_Y = Y;
        emit YChanged(m_Y);
    }
    void setTheta(double Theta)
    {
        if (qFuzzyCompare(m_Theta, Theta))
            return;

        m_Theta = Theta;
        emit ThetaChanged(m_Theta);
    }

signals:
    void XChanged(double X);
    void YChanged(double Y);
    void ThetaChanged(double Theta);

private:
    double m_X = 0;
    double m_Y = 0;
    double m_Theta = 0;
};

struct mPoint3D
{
    mPoint3D() {}
    mPoint3D(double x,double y,double z){X= x;Y= y;Z= z;}
    double X = 0;
    double Y = 0;
    double Z = 0;
} typedef SUTPos ;

struct mPoint6D
{
    mPoint6D() {}
    mPoint6D(double x,double y,double z,double a,double b,double c){X = x;Y = y;Z = z;A = a;B = b;C = c;}
    double X = 0;
    double Y = 0;
    double Z = 0;
    double A = 0;
    double B = 0;
    double C = 0;
};
class Position3D:public PropertyBase
{
    Q_OBJECT
public:
    Position3D():PropertyBase(){}
    Position3D(mPoint3D point):PropertyBase(){m_X = point.X;m_Y = point.Y;m_Z = point.Z;}
    void SetPosition(mPoint3D point){m_X = point.X;m_Y = point.Y;m_Z = point.Z;}
    Position3D(double x,double y,double z):PropertyBase(){m_X= x;m_Y= y;m_Z= z;}
    Q_PROPERTY(double X READ X WRITE setX NOTIFY XChanged)
    Q_PROPERTY(double Y READ Y WRITE setY NOTIFY YChanged)
    Q_PROPERTY(double Z READ Z WRITE setZ NOTIFY ZChanged)
    double X() const
    {
        return m_X;
    }
    double Y() const
    {
        return m_Y;
    }

    double Z() const
    {
        return m_Z;
    }

public slots:
    void setX(double X)
    {
        if (qFuzzyCompare(m_X, X))
            return;

        m_X = X;
        emit XChanged(m_X);
    }
    void setY(double Y)
    {
        if (qFuzzyCompare(m_Y, Y))
            return;

        m_Y = Y;
        emit YChanged(m_Y);
    }

    void setZ(double Z)
    {
        if (qFuzzyCompare(m_Z, Z))
            return;

        m_Z = Z;
        emit ZChanged(m_Z);
    }

signals:
    void XChanged(double X);
    void YChanged(double Y);

    void ZChanged(double Z);

private:
    double m_X = 0;
    double m_Y = 0;
    double m_Z = 0;
};

class Position6D:public PropertyBase
{
    Q_OBJECT
public:
    Position6D():PropertyBase(){}
    Position6D(mPoint6D point):PropertyBase(){m_X = point.X;m_Y = point.Y;m_Z = point.Z;m_A = point.A;m_B = point.B;m_Z = point.C;}
    void SetPosition(mPoint6D point){m_X = point.X;m_Y = point.Y;m_Z = point.Z;m_A = point.A;m_B = point.B;m_Z = point.C;}
    Q_PROPERTY(double X READ X WRITE setX NOTIFY XChanged)
    Q_PROPERTY(double Y READ Y WRITE setY NOTIFY YChanged)
    Q_PROPERTY(double Z READ Z WRITE setZ NOTIFY ZChanged)
    Q_PROPERTY(double A READ A WRITE setA NOTIFY AChanged)
    Q_PROPERTY(double B READ B WRITE setB NOTIFY BChanged)
    Q_PROPERTY(double C READ C WRITE setC NOTIFY CChanged)
    double X() const
    {
        return m_X;
    }
    double Y() const
    {
        return m_Y;
    }

    double Z() const
    {
        return m_Z;
    }

    double A() const
    {
        return m_A;
    }

    double B() const
    {
        return m_B;
    }

    double C() const
    {
        return m_C;
    }

public slots:
    void setX(double X)
    {
        if (qFuzzyCompare(m_X, X))
            return;

        m_X = X;
        emit XChanged(m_X);
    }
    void setY(double Y)
    {
        if (qFuzzyCompare(m_Y, Y))
            return;

        m_Y = Y;
        emit YChanged(m_Y);
    }

    void setZ(double Z)
    {
        if (qFuzzyCompare(m_Z, Z))
            return;

        m_Z = Z;
        emit ZChanged(m_Z);
    }

    void setA(double A)
    {
        if (qFuzzyCompare(m_A, A))
            return;

        m_A = A;
        emit AChanged(m_A);
    }

    void setB(double B)
    {
        if (qFuzzyCompare(m_B, B))
            return;

        m_B = B;
        emit BChanged(m_B);
    }

    void setC(double C)
    {
        if (qFuzzyCompare(m_C, C))
            return;

        m_C = C;
        emit CChanged(m_C);
    }

signals:
    void XChanged(double X);
    void YChanged(double Y);

    void ZChanged(double Z);

    void AChanged(double A);

    void BChanged(double B);

    void CChanged(double C);

private:
    double m_X = 0;
    double m_Y = 0;
    double m_Z = 0;
    double m_A = 0;
    double m_B = 0;
    double m_C = 0;
};

#endif // POSITIONDEFINE_H
