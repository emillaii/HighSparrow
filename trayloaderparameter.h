#ifndef TRAYLOADERPARAMETER_H
#define TRAYLOADERPARAMETER_H

#include "propertybase.h"


class TrayLoaderModuleParameter:public PropertyBase{
    Q_OBJECT
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

    double m_ltkx1GetPos = 0;
    double m_ltkx1SetPos = 0;
    double m_ltlGetPos = 0;
    double m_ltlSetPos = 0;
    double m_ltkx2GetPos = 0;
    double m_ltkx2SetPos = 0;

public:
    TrayLoaderModuleParameter():PropertyBase(){}
    Q_PROPERTY(double ltkx1PressPos READ ltkx1PressPos WRITE setLtkx1PressPos NOTIFY ltkx1PressPosChanged)
    Q_PROPERTY(double ltkx1ReleasePos READ ltkx1ReleasePos WRITE setLtkx1ReleasePos NOTIFY ltkx1ReleasePosChanged)
    Q_PROPERTY(double ltlPressPos READ ltlPressPos WRITE setLtlPressPos NOTIFY ltlPressPosChanged)
    Q_PROPERTY(double ltlReleasePos READ ltlReleasePos WRITE setLtlReleasePos NOTIFY ltlReleasePosChanged)
    Q_PROPERTY(double ltkx2PressPos READ ltkx2PressPos WRITE setLtkx2PressPos NOTIFY ltkx2PressPosChanged)
    Q_PROPERTY(double ltkx2ReleasePos READ ltkx2ReleasePos WRITE setLtkx2ReleasePos NOTIFY ltkx2ReleasePosChanged)
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

double ltkx1PressPos() const
{
    return m_ltkx1GetPos;
}

double ltkx1ReleasePos() const
{
    return m_ltkx1SetPos;
}

double ltlPressPos() const
{
    return m_ltlGetPos;
}

double ltlReleasePos() const
{
    return m_ltlSetPos;
}

double ltkx2PressPos() const
{
    return m_ltkx2GetPos;
}

double ltkx2ReleasePos() const
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

void setLtkx1PressPos(double ltkx1GetPos)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ltkx1GetPos, ltkx1GetPos))
        return;

    m_ltkx1GetPos = ltkx1GetPos;
    emit ltkx1PressPosChanged(m_ltkx1GetPos);
}

void setLtkx1ReleasePos(double ltkx1SetPos)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ltkx1SetPos, ltkx1SetPos))
        return;

    m_ltkx1SetPos = ltkx1SetPos;
    emit ltkx1ReleasePosChanged(m_ltkx1SetPos);
}

void setLtlPressPos(double ltlGetPos)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ltlGetPos, ltlGetPos))
        return;

    m_ltlGetPos = ltlGetPos;
    emit ltlPressPosChanged(m_ltlGetPos);
}

void setLtlReleasePos(double ltlSetPos)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ltlSetPos, ltlSetPos))
        return;

    m_ltlSetPos = ltlSetPos;
    emit ltlReleasePosChanged(m_ltlSetPos);
}

void setLtkx2PressPos(double ltkx2GetPos)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ltkx2GetPos, ltkx2GetPos))
        return;

    m_ltkx2GetPos = ltkx2GetPos;
    emit ltkx2PressPosChanged(m_ltkx2GetPos);
}

void setLtkx2ReleasePos(double ltkx2SetPos)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ltkx2SetPos, ltkx2SetPos))
        return;

    m_ltkx2SetPos = ltkx2SetPos;
    emit ltkx2ReleasePosChanged(m_ltkx2SetPos);
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
void ltkx1PressPosChanged(double ltkx1PressPos);
void ltkx1ReleasePosChanged(double ltkx1ReleasePos);
void ltlPressPosChanged(double ltlPressPos);
void ltlReleasePosChanged(double ltlReleasePos);
void ltkx2PressPosChanged(double ltkx2PressPos);
void ltkx2ReleasePosChanged(double ltkx2ReleasePos);
};

#endif // TRAYLOADERPARAMETER_H
