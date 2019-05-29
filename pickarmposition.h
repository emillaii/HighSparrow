#ifndef PICKARMPOSITION_H
#define PICKARMPOSITION_H

#include "propertybase.h"

class PickArmPosition : public PropertyBase{
    Q_OBJECT
    double m_x = 0;
    double m_y = 0;
    double m_z = 0;
    double m_v = 0;
    double m_th = 0;

public:
    Q_PROPERTY(double x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(double y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(double z READ z WRITE setZ NOTIFY zChanged)
    Q_PROPERTY(double v READ v WRITE setV NOTIFY vChanged)
    Q_PROPERTY(double th READ th WRITE setTh NOTIFY thChanged)

double x() const
{
    return m_x;
}
double y() const
{
    return m_y;
}

double z() const
{
    return m_z;
}

double v() const
{
    return m_v;
}

double th() const
{
    return m_th;
}

public slots:
void setX(double x)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_x, x))
        return;

    m_x = x;
    emit xChanged(m_x);
}
void setY(double y)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_y, y))
        return;

    m_y = y;
    emit yChanged(m_y);
}

void setZ(double z)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_z, z))
        return;

    m_z = z;
    emit zChanged(m_z);
}

void setV(double v)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_v, v))
        return;

    m_v = v;
    emit vChanged(m_v);
}

void setTh(double th)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_th, th))
        return;

    m_th = th;
    emit thChanged(m_th);
}

signals:
void xChanged(double x);
void yChanged(double y);
void zChanged(double z);
void vChanged(double v);
void thChanged(double th);
};

#endif // PICKARMPOSITION_H
