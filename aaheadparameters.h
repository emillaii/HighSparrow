#ifndef AAHEADPARAMETERS_H
#define AAHEADPARAMETERS_H

#include <QObject>
#include "Utils/propertybase.h"
#include "Utils/config.h"

class AAHeadParameters : public PropertyBase
{
    Q_OBJECT
public:
    explicit AAHeadParameters(){}
    Q_PROPERTY(double PickLensPositionZ READ PickLensPositionZ WRITE setPickLensPositionZ NOTIFY paramsChanged)
    Q_PROPERTY(double OCPositionZ READ OCPositionZ WRITE setOCPositionZ NOTIFY paramsChanged)
    Q_PROPERTY(double rotateZOffset READ rotateZOffset WRITE setRotateZOffset NOTIFY rotateZOffsetChanged)
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(QString motorZName READ motorZName WRITE setMotorZName NOTIFY motorZNameChanged)
    Q_PROPERTY(QString motorAName READ motorAName WRITE setMotorAName NOTIFY motorANameChanged)
    Q_PROPERTY(QString motorBName READ motorBName WRITE setMotorBName NOTIFY motorBNameChanged)
    Q_PROPERTY(QString motorCName READ motorCName WRITE setMotorCName NOTIFY motorCNameChanged)
    Q_PROPERTY(QString gripperName READ gripperName WRITE setGripperName NOTIFY gripperNameChanged)
    Q_PROPERTY(QString uv1Name READ uv1Name WRITE setUv1Name NOTIFY uv1NameChanged)
    Q_PROPERTY(QString uv2Name READ uv2Name WRITE setUv2Name NOTIFY uv2NameChanged)
    Q_PROPERTY(QString uv3Name READ uv3Name WRITE setUv3Name NOTIFY uv3NameChanged)
    Q_PROPERTY(QString uv4Name READ uv4Name WRITE setUv4Name NOTIFY uv4NameChanged)
    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)

double PickLensPositionZ() const
{
    return m_PickLensPositionZ;
}

double OCPositionZ() const
{
    return m_OCPositionZ;
}

double rotateZOffset() const
{
    return m_rotateZOffset;
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

QString motorAName() const
{
    return m_motorAName;
}

QString motorBName() const
{
    return m_motorBName;
}

QString motorCName() const
{
    return m_motorCName;
}

QString gripperName() const
{
    return m_gripperName;
}

QString uv1Name() const
{
    return m_uv1Name;
}

QString uv2Name() const
{
    return m_uv2Name;
}

QString uv3Name() const
{
    return m_uv3Name;
}

QString uv4Name() const
{
    return m_uv4Name;
}

QString moduleName() const
{
    return m_moduleName;
}

public slots:
void setPickLensPositionZ(double PickLensPositionZ)
{
    m_PickLensPositionZ = PickLensPositionZ;
    emit paramsChanged();
}

void setOCPositionZ(double OCPositionZ)
{
    m_OCPositionZ = OCPositionZ;
    emit paramsChanged();
}

void setRotateZOffset(double rotateZOffset)
{
    if (qFuzzyCompare(m_rotateZOffset, rotateZOffset))
        return;

    m_rotateZOffset = rotateZOffset;
    emit rotateZOffsetChanged(m_rotateZOffset);
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

void setMotorAName(QString motorAName)
{
    if (m_motorAName == motorAName)
        return;

    m_motorAName = motorAName;
    emit motorANameChanged(m_motorAName);
}

void setMotorBName(QString motorBName)
{
    if (m_motorBName == motorBName)
        return;

    m_motorBName = motorBName;
    emit motorBNameChanged(m_motorBName);
}

void setMotorCName(QString motorCName)
{
    if (m_motorCName == motorCName)
        return;

    m_motorCName = motorCName;
    emit motorCNameChanged(m_motorCName);
}

void setGripperName(QString gripperName)
{
    if (m_gripperName == gripperName)
        return;

    m_gripperName = gripperName;
    emit gripperNameChanged(m_gripperName);
}

void setUv1Name(QString uv1Name)
{
    if (m_uv1Name == uv1Name)
        return;

    m_uv1Name = uv1Name;
    emit uv1NameChanged(m_uv1Name);
}

void setUv2Name(QString uv2Name)
{
    if (m_uv2Name == uv2Name)
        return;

    m_uv2Name = uv2Name;
    emit uv2NameChanged(m_uv2Name);
}

void setUv3Name(QString uv3Name)
{
    if (m_uv3Name == uv3Name)
        return;

    m_uv3Name = uv3Name;
    emit uv3NameChanged(m_uv3Name);
}

void setUv4Name(QString uv4Name)
{
    if (m_uv4Name == uv4Name)
        return;

    m_uv4Name = uv4Name;
    emit uv4NameChanged(m_uv4Name);
}

void setModuleName(QString moduleName)
{
    if (m_moduleName == moduleName)
        return;

    m_moduleName = moduleName;
    emit moduleNameChanged(m_moduleName);
}

signals:
void paramsChanged();
void rotateZOffsetChanged(double rotateZOffset);

void motorXNameChanged(QString motorXName);

void motorYNameChanged(QString motorYName);

void motorZNameChanged(QString motorZName);

void motorANameChanged(QString motorAName);

void motorBNameChanged(QString motorBName);

void motorCNameChanged(QString motorCName);

void gripperNameChanged(QString gripperName);

void uv1NameChanged(QString uv1Name);

void uv2NameChanged(QString uv2Name);

void uv3NameChanged(QString uv3Name);

void uv4NameChanged(QString uv4Name);

void moduleNameChanged(QString moduleName);

private:
double m_PickLensPositionZ = 0;
double m_OCPositionZ = 0;
double m_rotateZOffset = 31.5;
QString m_motorXName = "AA_X";
QString m_motorYName = "AA_Y";
QString m_motorZName = "AA_Z";
QString m_motorAName = "AA_A";
QString m_motorBName = "AA_B";
QString m_motorCName = "AA_C";
QString m_gripperName = "AA_Gripper";
QString m_uv1Name = "AA_UV1";
QString m_uv2Name = "AA_UV2";
QString m_uv3Name = "AA_UV3";
QString m_uv4Name = "AA_UV4";
QString m_moduleName = "AA1Head";
};

#endif // AAHEADPARAMETERS_H
