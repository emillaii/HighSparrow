#ifndef TRAYLOADERPARAMETER_H
#define TRAYLOADERPARAMETER_H

#include "propertybase.h"


class TrayLoaderModuleParameter:public PropertyBase{
private:
    double m_position1;
    double m_position2;
    double m_position3;

public:
    Q_PROPERTY(double position1 READ position1 WRITE setPosition1 NOTIFY position1Changed)
    Q_PROPERTY(double position2 READ position2 WRITE setPosition2 NOTIFY position2Changed)
    Q_PROPERTY(double position3 READ position3 WRITE setPosition3 NOTIFY position3Changed)


double position1() const
{
    return m_position1;
}
double position2() const
{
    return m_position2;
}

double position3() const
{
    return m_position3;
}

public slots:
void setPosition1(double position1)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_position1, position1))
        return;

    m_position1 = position1;
    emit position1Changed(m_position1);
}
void setPosition2(double position2)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_position2, position2))
        return;

    m_position2 = position2;
    emit position2Changed(m_position2);
}

void setPosition3(double position3)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_position3, position3))
        return;

    m_position3 = position3;
    emit position3Changed(m_position3);
}

signals:
void position1Changed(double position1);
void position2Changed(double position2);
void position3Changed(double position3);
};

#endif // TRAYLOADERPARAMETER_H
