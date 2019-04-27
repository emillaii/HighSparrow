#ifndef TRAYLOADERPARAMETER_H
#define TRAYLOADERPARAMETER_H

#include "propertybase.h"


class TrayLoaderModuleParameter:public PropertyBase{
private:
    double m_position1;
    double m_position2;
    double m_position3;
    double m_position4;
    double m_position5;
    double m_position6;

    QString m_motorLTIEName;
    QString m_motorLTOEName;
    QString m_motorLTKX1Name;
    QString m_motorLTKX2Name;
    QString m_motorLTLName;

    QString m_cylinderClipName;
    QString m_cylinderLTK1Name;
    QString m_cylinderLTK2Name;
    QString m_cylinderTrayName;

public:
    TrayLoaderModuleParameter():PropertyBase(){}
    Q_PROPERTY(double position1 READ position1 WRITE setPosition1 NOTIFY position1Changed)
    Q_PROPERTY(double position2 READ position2 WRITE setPosition2 NOTIFY position2Changed)
    Q_PROPERTY(double position3 READ position3 WRITE setPosition3 NOTIFY position3Changed)
    Q_PROPERTY(double position4 READ position4 WRITE setPosition4 NOTIFY position4Changed)
    Q_PROPERTY(double position5 READ position5 WRITE setPosition5 NOTIFY position5Changed)
    Q_PROPERTY(double position6 READ position6 WRITE setPosition6 NOTIFY position6Changed)
    Q_PROPERTY(QString motorLTIEName READ motorLTIEName WRITE setMotorLTIEName NOTIFY motorTLIENameChanged)
    Q_PROPERTY(QString motorLTOEName READ motorLTOEName WRITE setMotorLTOEName NOTIFY motorLTOENameChanged)
    Q_PROPERTY(QString motorLTKX1Name READ motorLTKX1Name WRITE setMotorLTKX1Name NOTIFY motorLTKX1NameChanged)
    Q_PROPERTY(QString motorLTKX2Name READ motorLTKX2Name WRITE setMotorLTKX2Name NOTIFY motorLTKX2NameChanged)
    Q_PROPERTY(QString motorLTLXName READ motorLTLXName WRITE setMotorLTLName NOTIFY motorLTLNameChanged)
    Q_PROPERTY(QString cylinderClipName READ cylinderClipName WRITE setCylinderClip NOTIFY cylinderClipChanged)
    Q_PROPERTY(QString cylinderLTK1Name READ cylinderLTK1Name WRITE setCylinderLTK1Name NOTIFY cylinderLTK1NameChanged)
    Q_PROPERTY(QString cylinderLTK2Name READ cylinderLTK2Name WRITE setCylinderLTK2Name NOTIFY cylinderLTK2NameChanged)
    Q_PROPERTY(QString cylinderTrayName READ cylinderTrayName WRITE setCylinderTrayName NOTIFY cylinderTrayNameChanged)

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

double position4() const
{
    return m_position4;
}

double position5() const
{
    return m_position5;
}

double position6() const
{
    return m_position6;
}

QString motorLTIEName() const
{
    return m_motorLTIEName;
}

QString motorLTOEName() const
{
    return m_motorLTOEName;
}

QString motorLTKX1Name() const
{
    return m_motorLTKX1Name;
}

QString motorLTKX2Name() const
{
    return m_motorLTKX2Name;
}

QString motorLTLXName() const
{
    return m_motorLTLName;
}

QString cylinderClipName() const
{
    return m_cylinderClipName;
}

QString cylinderLTK1Name() const
{
    return m_cylinderLTK1Name;
}

QString cylinderLTK2Name() const
{
    return m_cylinderLTK2Name;
}

QString cylinderTrayName() const
{
    return m_cylinderTrayName;
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

void setPosition4(double position4)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_position4, position4))
        return;

    m_position4 = position4;
    emit position4Changed(m_position4);
}

void setPosition5(double position5)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_position5, position5))
        return;

    m_position5 = position5;
    emit position5Changed(m_position5);
}

void setPosition6(double position6)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_position6, position6))
        return;

    m_position6 = position6;
    emit position6Changed(m_position6);
}

void setMotorLTIEName(QString motorLTIEName)
{
    if (m_motorLTIEName == motorLTIEName)
        return;

    m_motorLTIEName = motorLTIEName;
    emit motorTLIENameChanged(m_motorLTIEName);
}

void setMotorLTOEName(QString motorLTOEName)
{
    if (m_motorLTOEName == motorLTOEName)
        return;

    m_motorLTOEName = motorLTOEName;
    emit motorLTOENameChanged(m_motorLTOEName);
}

void setMotorLTKX1Name(QString motorLTKX1Name)
{
    if (m_motorLTKX1Name == motorLTKX1Name)
        return;

    m_motorLTKX1Name = motorLTKX1Name;
    emit motorLTKX1NameChanged(m_motorLTKX1Name);
}

void setMotorLTKX2Name(QString motorLTKX2Name)
{
    if (m_motorLTKX2Name == motorLTKX2Name)
        return;

    m_motorLTKX2Name = motorLTKX2Name;
    emit motorLTKX2NameChanged(m_motorLTKX2Name);
}

void setMotorLTLName(QString motorLTLName)
{
    if (m_motorLTLName == motorLTLName)
        return;

    m_motorLTLName = motorLTLName;
    emit motorLTLNameChanged(m_motorLTLName);
}

void setCylinderClip(QString cylinderClipName)
{
    if (m_cylinderClipName == cylinderClipName)
        return;

    m_cylinderClipName = cylinderClipName;
    emit cylinderClipChanged(m_cylinderClipName);
}

void setCylinderLTK1Name(QString cylinderLTK1Name)
{
    if (m_cylinderLTK1Name == cylinderLTK1Name)
        return;

    m_cylinderLTK1Name = cylinderLTK1Name;
    emit cylinderLTK1NameChanged(m_cylinderLTK1Name);
}

void setCylinderLTK2Name(QString cylinderLTK2Name)
{
    if (m_cylinderLTK2Name == cylinderLTK2Name)
        return;

    m_cylinderLTK2Name = cylinderLTK2Name;
    emit cylinderLTK2NameChanged(m_cylinderLTK2Name);
}

void setCylinderTrayName(QString cylinderTrayName)
{
    if (m_cylinderTrayName == cylinderTrayName)
        return;

    m_cylinderTrayName = cylinderTrayName;
    emit cylinderTrayNameChanged(m_cylinderTrayName);
}

signals:
void position1Changed(double position1);
void position2Changed(double position2);
void position3Changed(double position3);
void position4Changed(double position4);
void position5Changed(double position5);
void position6Changed(double position6);
void motorTLIENameChanged(QString motorLTIEName);
void motorLTOENameChanged(QString motorLTOEName);
void motorLTKX1NameChanged(QString motorLTKX1Name);
void motorLTKX2NameChanged(QString motorLTKX2Name);
void motorLTLNameChanged(QString motorLTLXName);
void cylinderClipChanged(QString cylinderClipName);
void cylinderLTK1NameChanged(QString cylinderLTK1Name);
void cylinderLTK2NameChanged(QString cylinderLTK2Name);
void cylinderTrayNameChanged(QString cylinderTrayName);
};

#endif // TRAYLOADERPARAMETER_H
