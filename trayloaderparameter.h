#ifndef TRAYLOADERPARAMETER_H
#define TRAYLOADERPARAMETER_H

#include "propertybase.h"


class TrayLoaderModuleParameter:public PropertyBase{
private:
    QString m_motorLTIEName;
    QString m_motorLTOEName;
    QString m_motorLTKX1Name;
    QString m_motorLTKX2Name;
    QString m_motorLTLName;

    QString m_cylinderClipName;
    QString m_cylinderLTK1Name;
    QString m_cylinderLTK2Name;
    QString m_cylinderTrayName;

    double m_ltkx1GetPos;
    double m_ltkx1SetPos;
    double m_ltlGetPos;
    double m_ltlSetPos;
    double m_ltkx2GetPos;
    double m_ltkx2SetPos;

public:
    TrayLoaderModuleParameter():PropertyBase(){}
    Q_PROPERTY(double ltkx1GetPos READ ltkx1GetPos WRITE setLtkx1GetPos NOTIFY ltkx1GetPosChanged)
    Q_PROPERTY(double ltkx1SetPos READ ltkx1SetPos WRITE setLtkx1SetPos NOTIFY ltkx1SetPosChanged)
    Q_PROPERTY(double ltlGetPos READ ltlGetPos WRITE setLtlGetPos NOTIFY ltlGetPosChanged)
    Q_PROPERTY(double ltlSetPos READ ltlSetPos WRITE setLtlSetPos NOTIFY ltlSetPosChanged)
    Q_PROPERTY(double ltkx2GetPos READ ltkx2GetPos WRITE setLtkx2GetPos NOTIFY ltkx2GetPosChanged)
    Q_PROPERTY(double ltkx2SetPos READ ltkx2SetPos WRITE setLtkx2SetPos NOTIFY ltkx2SetPosChanged)
    Q_PROPERTY(QString motorLTIEName READ motorLTIEName WRITE setMotorLTIEName NOTIFY motorTLIENameChanged)
    Q_PROPERTY(QString motorLTOEName READ motorLTOEName WRITE setMotorLTOEName NOTIFY motorLTOENameChanged)
    Q_PROPERTY(QString motorLTKX1Name READ motorLTKX1Name WRITE setMotorLTKX1Name NOTIFY motorLTKX1NameChanged)
    Q_PROPERTY(QString motorLTKX2Name READ motorLTKX2Name WRITE setMotorLTKX2Name NOTIFY motorLTKX2NameChanged)
    Q_PROPERTY(QString motorLTLXName READ motorLTLXName WRITE setMotorLTLName NOTIFY motorLTLNameChanged)
    Q_PROPERTY(QString cylinderClipName READ cylinderClipName WRITE setCylinderClip NOTIFY cylinderClipChanged)
    Q_PROPERTY(QString cylinderLTK1Name READ cylinderLTK1Name WRITE setCylinderLTK1Name NOTIFY cylinderLTK1NameChanged)
    Q_PROPERTY(QString cylinderLTK2Name READ cylinderLTK2Name WRITE setCylinderLTK2Name NOTIFY cylinderLTK2NameChanged)
    Q_PROPERTY(QString cylinderTrayName READ cylinderTrayName WRITE setCylinderTrayName NOTIFY cylinderTrayNameChanged)

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

double ltkx1GetPos() const
{
    return m_ltkx1GetPos;
}

double ltkx1SetPos() const
{
    return m_ltkx1SetPos;
}

double ltlGetPos() const
{
    return m_ltlGetPos;
}

double ltlSetPos() const
{
    return m_ltlSetPos;
}

double ltkx2GetPos() const
{
    return m_ltkx2GetPos;
}

double ltkx2SetPos() const
{
    return m_ltkx2SetPos;
}

public slots:

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

void setLtkx1GetPos(double ltkx1GetPos)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ltkx1GetPos, ltkx1GetPos))
        return;

    m_ltkx1GetPos = ltkx1GetPos;
    emit ltkx1GetPosChanged(m_ltkx1GetPos);
}

void setLtkx1SetPos(double ltkx1SetPos)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ltkx1SetPos, ltkx1SetPos))
        return;

    m_ltkx1SetPos = ltkx1SetPos;
    emit ltkx1SetPosChanged(m_ltkx1SetPos);
}

void setLtlGetPos(double ltlGetPos)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ltlGetPos, ltlGetPos))
        return;

    m_ltlGetPos = ltlGetPos;
    emit ltlGetPosChanged(m_ltlGetPos);
}

void setLtlSetPos(double ltlSetPos)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ltlSetPos, ltlSetPos))
        return;

    m_ltlSetPos = ltlSetPos;
    emit ltlSetPosChanged(m_ltlSetPos);
}

void setLtkx2GetPos(double ltkx2GetPos)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ltkx2GetPos, ltkx2GetPos))
        return;

    m_ltkx2GetPos = ltkx2GetPos;
    emit ltkx2GetPosChanged(m_ltkx2GetPos);
}

void setLtkx2SetPos(double ltkx2SetPos)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ltkx2SetPos, ltkx2SetPos))
        return;

    m_ltkx2SetPos = ltkx2SetPos;
    emit ltkx2SetPosChanged(m_ltkx2SetPos);
}

signals:
void motorTLIENameChanged(QString motorLTIEName);
void motorLTOENameChanged(QString motorLTOEName);
void motorLTKX1NameChanged(QString motorLTKX1Name);
void motorLTKX2NameChanged(QString motorLTKX2Name);
void motorLTLNameChanged(QString motorLTLXName);
void cylinderClipChanged(QString cylinderClipName);
void cylinderLTK1NameChanged(QString cylinderLTK1Name);
void cylinderLTK2NameChanged(QString cylinderLTK2Name);
void cylinderTrayNameChanged(QString cylinderTrayName);
void ltkx1GetPosChanged(double ltkx1GetPos);
void ltkx1SetPosChanged(double ltkx1SetPos);
void ltlGetPosChanged(double ltlGetPos);
void ltlSetPosChanged(double ltlSetPos);
void ltkx2GetPosChanged(double ltkx2GetPos);
void ltkx2SetPosChanged(double ltkx2SetPos);
};

#endif // TRAYLOADERPARAMETER_H
