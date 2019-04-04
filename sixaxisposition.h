#ifndef SIXAXISPOSITION_H
#define SIXAXISPOSITION_H

#include <QObject>


class SixAxisPosition: public QObject
{
    Q_OBJECT

    Q_PROPERTY(double A READ A WRITE SetA)
    Q_PROPERTY(double B READ B WRITE SetB)
    Q_PROPERTY(double C READ C WRITE SetC)
    Q_PROPERTY(double X READ X WRITE SetX)
    Q_PROPERTY(double Y READ Y WRITE SetY)
    Q_PROPERTY(double Z READ Z WRITE SetZ)

public:
    SixAxisPosition() = default;
    SixAxisPosition(double a, double b, double c, double x, double y, double z, QObject* parent = 0);
    ~SixAxisPosition();

    double A() const;
    double B() const;
    double C() const;
    double X() const;
    double Y() const;
    double Z() const;

    void SetA(const double A);
    void SetB(const double B);
    void SetC(const double C);
    void SetX(const double X);
    void SetY(const double Y);
    void SetZ(const double Z);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

private:
    double _a;
    double _b;
    double _c;
    double _x;
    double _y;
    double _z;
};

#endif // SIXAXISPOSITION_H
