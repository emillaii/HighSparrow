#ifndef DISPENSE_PATHER_H
#define DISPENSE_PATHER_H

#include "propertybase.h"



class DispenseParameter:public PropertyBase
{
    Q_OBJECT
public:
    DispenseParameter():PropertyBase(){}
    Q_PROPERTY(double initTheta READ initTheta WRITE setInitTheta NOTIFY InitThetaChanged)
    Q_PROPERTY(double dispenseXOffset READ dispenseXOffset WRITE setDispenseXOffset NOTIFY dispenseXOffsetChanged)
    Q_PROPERTY(double dispenseYOffset READ dispenseYOffset WRITE setDispenseYOffset NOTIFY dispenseYOffsetChanged)
    Q_PROPERTY(double dispenseZPos READ dispenseZPos WRITE setDispenseZPos NOTIFY dispenseZPosChanged)
    Q_PROPERTY(double dispenseZOffset READ dispenseZOffset WRITE setDispenseZOffset NOTIFY dispenseZOffsetChanged)
    Q_PROPERTY(double testForce READ testForce WRITE setTestForce NOTIFY testForceChanged)
    double initTheta() const
    {
        return m_InitTheta;
    }
    double dispenseXOffset() const
    {
        return m_dispenseXOffset;
    }

    double dispenseYOffset() const
    {
        return m_dispenseYOffset;
    }

    double dispenseZPos() const
    {
        return m_dispenseZPos;
    }

    double dispenseZOffset() const
    {
        return m_dispenseZOffset;
    }

    double testForce() const
    {
        return m_testForce;
    }

public slots:
    void setInitTheta(double InitTheta)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_InitTheta, InitTheta))
            return;

        m_InitTheta = InitTheta;
        emit InitThetaChanged(m_InitTheta);
    }
    void setDispenseXOffset(double dispenseXOffset)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_dispenseXOffset, dispenseXOffset))
            return;

        m_dispenseXOffset = dispenseXOffset;
        emit dispenseXOffsetChanged(m_dispenseXOffset);
    }

    void setDispenseYOffset(double dispenseYOffset)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_dispenseYOffset, dispenseYOffset))
            return;

        m_dispenseYOffset = dispenseYOffset;
        emit dispenseYOffsetChanged(m_dispenseYOffset);
    }

    void setDispenseZPos(double dispenseZPos)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_dispenseZPos, dispenseZPos))
            return;

        m_dispenseZPos = dispenseZPos;
        emit dispenseZPosChanged(m_dispenseZPos);
    }

    void setDispenseZOffset(double dispenseZOffset)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_dispenseZOffset, dispenseZOffset))
            return;

        m_dispenseZOffset = dispenseZOffset;
        emit dispenseZOffsetChanged(m_dispenseZOffset);
    }

    void setTestForce(double testForce)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_testForce, testForce))
            return;

        m_testForce = testForce;
        emit testForceChanged(m_testForce);
    }

signals:
    void InitThetaChanged(double initTheta);
    void dispenseXOffsetChanged(double dispenseXOffset);

    void dispenseYOffsetChanged(double dispenseYOffset);

    void dispenseZPosChanged(double dispenseZPos);

    void dispenseZOffsetChanged(double dispenseZOffset);

    void testForceChanged(double testForce);

private:
    double m_InitTheta = 0;
    double m_dispenseXOffset = 0;
    double m_dispenseYOffset = 0;
    double m_dispenseZPos = 0;
    double m_dispenseZOffset = 0;
    double m_testForce = 0;
};

#endif // DISPENSE_PATHER_H
