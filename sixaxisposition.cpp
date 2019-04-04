#include "sixaxisposition.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QJsonObject>


SixAxisPosition::SixAxisPosition(double a, double b, double c, double x, double y, double z, QObject* parent):QObject (parent)
{
    _a =a;
    _b =b;
    _c =c;
    _x =x;
    _y =y;
    _z =z;

}

SixAxisPosition::~SixAxisPosition()
{

}

double SixAxisPosition::A() const
{
    return _a;
}

double SixAxisPosition::B() const
{
    return _b;
}
double SixAxisPosition::C() const
{
    return _c;
}
double SixAxisPosition::X() const
{
    return _x;
}
double SixAxisPosition::Y() const
{
    return _y;
}
double SixAxisPosition::Z() const
{
    return _z;
}

void SixAxisPosition::SetA(const double A)
{
    _a = A;
}

void SixAxisPosition::SetB(const double B)
{
    _b = B;
}

void SixAxisPosition::SetC(const double C)
{
    _c = C;
}

void SixAxisPosition::SetX(const double X)
{
    _x = X;
}

void SixAxisPosition::SetY(const double Y)
{
    _y = Y;
}

void SixAxisPosition::SetZ(const double Z)
{
    _z = Z;
}

void SixAxisPosition::read(const QJsonObject &json)
{
    const QMetaObject *metaobject =this->metaObject();
    int p_count = metaobject->propertyCount();
    for(int i=0;i<p_count;i++)
    {
        QMetaProperty metaproperty = metaobject->property(i);
        const char *name = metaproperty.name();
        if(json.contains(name))
        {
            this->setProperty(name,json[name]);
        }
    }
}

void SixAxisPosition::write(QJsonObject &json) const
{
    const QMetaObject *metaobject =this->metaObject();
    int p_count = metaobject->propertyCount();
    for(int i=0;i<p_count;i++)
    {
        QMetaProperty metaproperty = metaobject->property(i);
        const char *name = metaproperty.name();
        json[name] = this->property(name).toDouble();
    }
}
