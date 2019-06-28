#ifndef SINGLEHEADLSUTPARAMETER_H
#define SINGLEHEADLSUTPARAMETER_H

#include "propertybase.h"


class SingleHeadLSutParameter : public PropertyBase{
    Q_OBJECT
private:
    double  m_ZOffset;
    double  m_LensSoftlandingVel;
    double  m_LensSoftlandingForce;
    QString m_motorXName;
    QString m_motorYName;
    QString m_motorZName;
    QString m_lutVacuumName;
    QString m_sutVacuumName;
    QString m_cylinderName;
    QString m_sutDownLookLocationName;
    QString m_updownlookUpLocationName;
    QString m_updownlookDownLocationName;
    QString m_lutUplookLocationName;
    QString m_loadlookLocationName;
    QString m_mushroomLocationName;
    QString m_lutGripperLoactionName;

public:
    explicit SingleHeadLSutParameter(){}
    Q_PROPERTY(double ZOffset READ ZOffset WRITE setZOffset NOTIFY ZOffsetChanged)
    Q_PROPERTY(double LensSoftlandingVel READ LensSoftlandingVel WRITE setLensSoftlandingVel NOTIFY LensSoftlandingVelChanged)
    Q_PROPERTY(double LensSoftlandingForce READ LensSoftlandingForce WRITE setLensSoftlandingForce NOTIFY LensSoftlandingForceChanged)
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(QString motorZName READ motorZName WRITE setMotorZName NOTIFY motorZNameChanged)
    Q_PROPERTY(QString lutVacuumName READ lutVacuumName WRITE setLutVacuumName NOTIFY lutVacuumNameChanged)
    Q_PROPERTY(QString sutVacuumName READ sutVacuumName WRITE setSutVacuumName NOTIFY sutVacuumNameChanged)
    Q_PROPERTY(QString cylinderName READ cylinderName WRITE setCylinderName NOTIFY cylinderNameChanged)
    Q_PROPERTY(QString sutDownLookLocationName READ sutDownLookLocationName WRITE setSutDownLookLocationName NOTIFY sutDownLookLocationNameChanged)
    Q_PROPERTY(QString updownlookUpLocationName READ updownlookUpLocationName WRITE setUpdownlookUpLocationName NOTIFY updownlookUpLocationNameChanged)
    Q_PROPERTY(QString updownlookDownLocationName READ updownlookDownLocationName WRITE setUpdownlookDownLocationName NOTIFY updownlookDownLocationNameChanged)
    Q_PROPERTY(QString lutUplookLocationName READ lutUplookLocationName WRITE setLutUplookLocationName NOTIFY lutUplookLocationNameChanged)
    Q_PROPERTY(QString loadlookLocationName READ loadlookLocationName WRITE setLoadlookLocationName NOTIFY loadlookLocationNameChanged)
    Q_PROPERTY(QString mushroomLocationName READ mushroomLocationName WRITE setMushroomLocationName NOTIFY mushroomLocationNameChanged)
    Q_PROPERTY(QString lutGripperLoactionName READ lutGripperLoactionName WRITE setLutGripperLoactionName NOTIFY lutGripperLoactionNameChanged)

double ZOffset() const
{
    return m_ZOffset;
}

double LensSoftlandingVel() const
{
    return m_LensSoftlandingVel;
}

double LensSoftlandingForce() const
{
    return m_LensSoftlandingForce;
}

QString motorXName() const
{
    return m_motorXName;
}

QString motorYName() const
{
    return m_motorYName;
}

QString motorZName() const
{
    return m_motorZName;
}

QString lutVacuumName() const
{
    return m_lutVacuumName;
}

QString sutVacuumName() const
{
    return m_sutVacuumName;
}

QString cylinderName() const
{
    return m_cylinderName;
}

QString sutDownLookLocationName() const
{
    return m_sutDownLookLocationName;
}

QString updownlookUpLocationName() const
{
    return m_updownlookUpLocationName;
}

QString updownlookDownLocationName() const
{
    return m_updownlookDownLocationName;
}

QString lutUplookLocationName() const
{
    return m_lutUplookLocationName;
}

QString loadlookLocationName() const
{
    return m_loadlookLocationName;
}

QString mushroomLocationName() const
{
    return m_mushroomLocationName;
}

QString lutGripperLoactionName() const
{
    return m_lutGripperLoactionName;
}

public slots:
void setZOffset(double ZOffset)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_ZOffset, ZOffset))
        return;

    m_ZOffset = ZOffset;
    emit ZOffsetChanged(m_ZOffset);
}

void setLensSoftlandingVel(double LensSoftlandingVel)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_LensSoftlandingVel, LensSoftlandingVel))
        return;

    m_LensSoftlandingVel = LensSoftlandingVel;
    emit LensSoftlandingVelChanged(m_LensSoftlandingVel);
}

void setLensSoftlandingForce(double LensSoftlandingForce)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_LensSoftlandingForce, LensSoftlandingForce))
        return;

    m_LensSoftlandingForce = LensSoftlandingForce;
    emit LensSoftlandingForceChanged(m_LensSoftlandingForce);
}

void setMotorXName(QString motorXName)
{
    if (m_motorXName == motorXName)
        return;

    m_motorXName = motorXName;
    emit motorXNameChanged(m_motorXName);
}

void setMotorYName(QString motorYName)
{
    if (m_motorYName == motorYName)
        return;

    m_motorYName = motorYName;
    emit motorYNameChanged(m_motorYName);
}

void setMotorZName(QString motorZName)
{
    if (m_motorZName == motorZName)
        return;

    m_motorZName = motorZName;
    emit motorZNameChanged(m_motorZName);
}

void setLutVacuumName(QString lutVacuumName)
{
    if (m_lutVacuumName == lutVacuumName)
        return;

    m_lutVacuumName = lutVacuumName;
    emit lutVacuumNameChanged(m_lutVacuumName);
}

void setSutVacuumName(QString sutVacuumName)
{
    if (m_sutVacuumName == sutVacuumName)
        return;

    m_sutVacuumName = sutVacuumName;
    emit sutVacuumNameChanged(m_sutVacuumName);
}

void setCylinderName(QString cylinderName)
{
    if (m_cylinderName == cylinderName)
        return;

    m_cylinderName = cylinderName;
    emit cylinderNameChanged(m_cylinderName);
}

void setSutDownLookLocationName(QString sutDownLookLocationName)
{
    if (m_sutDownLookLocationName == sutDownLookLocationName)
        return;

    m_sutDownLookLocationName = sutDownLookLocationName;
    emit sutDownLookLocationNameChanged(m_sutDownLookLocationName);
}

void setUpdownlookUpLocationName(QString updownlookUpLocationName)
{
    if (m_updownlookUpLocationName == updownlookUpLocationName)
        return;

    m_updownlookUpLocationName = updownlookUpLocationName;
    emit updownlookUpLocationNameChanged(m_updownlookUpLocationName);
}

void setUpdownlookDownLocationName(QString updownlookDownLocationName)
{
    if (m_updownlookDownLocationName == updownlookDownLocationName)
        return;

    m_updownlookDownLocationName = updownlookDownLocationName;
    emit updownlookDownLocationNameChanged(m_updownlookDownLocationName);
}

void setLutUplookLocationName(QString lutUplookLocationName)
{
    if (m_lutUplookLocationName == lutUplookLocationName)
        return;

    m_lutUplookLocationName = lutUplookLocationName;
    emit lutUplookLocationNameChanged(m_lutUplookLocationName);
}

void setLoadlookLocationName(QString loadlookLocationName)
{
    if (m_loadlookLocationName == loadlookLocationName)
        return;

    m_loadlookLocationName = loadlookLocationName;
    emit loadlookLocationNameChanged(m_loadlookLocationName);
}

void setMushroomLocationName(QString mushroomLocationName)
{
    if (m_mushroomLocationName == mushroomLocationName)
        return;

    m_mushroomLocationName = mushroomLocationName;
    emit mushroomLocationNameChanged(m_mushroomLocationName);
}

void setLutGripperLoactionName(QString lutGripperLoactionName)
{
    if (m_lutGripperLoactionName == lutGripperLoactionName)
        return;

    m_lutGripperLoactionName = lutGripperLoactionName;
    emit lutGripperLoactionNameChanged(m_lutGripperLoactionName);
}

signals:
void ZOffsetChanged(double ZOffset);
void LensSoftlandingVelChanged(double ZOffset);
void LensSoftlandingForceChanged(double ZOffset);
void motorXNameChanged(QString motorXName);
void motorYNameChanged(QString motorYName);
void motorZNameChanged(QString motorZName);
void lutVacuumNameChanged(QString lutVacuumName);
void sutVacuumNameChanged(QString sutVacuumName);
void cylinderNameChanged(QString cylinderName);
void sutDownLookLocationNameChanged(QString sutDownLookLocationName);
void updownlookUpLocationNameChanged(QString updownlookUpLocationName);
void updownlookDownLocationNameChanged(QString updownlookDownLocationName);
void lutUplookLocationNameChanged(QString lutUplookLocationName);
void loadlookLocationNameChanged(QString loadlookLocationName);
void mushroomLocationNameChanged(QString mushroomLocationName);
void lutGripperLoactionNameChanged(QString lutGripperLoactionName);
};

#endif // SINGLEHEADLSUTPARAMETER_H
