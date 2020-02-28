#ifndef DISPENSER_PARAMETER_H
#define DISPENSER_PARAMETER_H

#include "Utils/propertybase.h"
#include "Utils/commonmethod.h"

class DispenserParameter:public PropertyBase
{
    Q_OBJECT
public:
    DispenserParameter():PropertyBase(){}
    Q_PROPERTY(double openOffset READ openOffset WRITE setOpenOffset NOTIFY openOffsetChanged)
    Q_PROPERTY(double closeOffset READ closeOffset WRITE setCloseOffset NOTIFY closeOffsetChanged)
    Q_PROPERTY(double maximumSpeed READ maximumSpeed WRITE setMaximumSpeed NOTIFY maximumSpeedChanged)
    Q_PROPERTY(double endSpeed READ endSpeed WRITE setEndSpeed NOTIFY endSpeedChanged)
    Q_PROPERTY(QString dispenseIo READ dispenseIo WRITE setDispenseIo NOTIFY DispenseIoChanged)
//    Q_PROPERTY(double theta READ theta WRITE setTheta NOTIFY ThetaChanged)
    double openOffset() const
    {
        return m_openOffset;
    }
    double closeOffset() const
    {
        return m_closeOffset;
    }

    double maximumSpeed() const
    {
        return m_maximumSpeed;
    }

    double endSpeed() const
    {
        return m_endSpeed;
    }

//    double theta() const
//    {
//        return m_theta;
//    }

    QString dispenseIo() const
    {
        return m_dispenseIo;
    }

public slots:
    void setOpenOffset(double openOffset)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_openOffset, openOffset))
            return;

        m_openOffset = SET_PRECISION(openOffset, 6);
        emit openOffsetChanged(m_openOffset);
    }
    void setCloseOffset(double closeOffset)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_closeOffset, closeOffset))
            return;

        m_closeOffset = SET_PRECISION(closeOffset, 6);
        emit closeOffsetChanged(m_closeOffset);
    }

    void setMaximumSpeed(double maximumSpeed)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maximumSpeed, maximumSpeed))
            return;

        m_maximumSpeed = maximumSpeed;
        emit maximumSpeedChanged(m_maximumSpeed);
    }

    void setEndSpeed(double endSpeed)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_endSpeed, endSpeed))
            return;

        m_endSpeed = endSpeed;
        emit endSpeedChanged(m_endSpeed);
    }

//    void setTheta(double theta)
//    {
//        qWarning("Floating point comparison needs context sanity check");
//        if (qFuzzyCompare(m_theta, theta))
//            return;

//        m_theta = theta;
//        emit ThetaChanged(m_theta);
//    }

    void setDispenseIo(QString dispenseIo)
    {
        if (m_dispenseIo == dispenseIo)
            return;

        m_dispenseIo = dispenseIo;
        emit DispenseIoChanged(m_dispenseIo);
    }

signals:
    void openOffsetChanged(double openOffset);

    void closeOffsetChanged(double closeOffset);

    void maximumSpeedChanged(double maximumSpeed);

    void endSpeedChanged(double endSpeed);

    void dispenseZPosChanged(double dispenseZPos);

    void dispenseZOffsetChanged(double dispenseZOffset);

    void dispenseXOffsetChanged(double dispenseXOffset);

    void dispenseYOffsetChanged(double dispenseYOffset);

//    void ThetaChanged(double theta);

    void DispenseIoChanged(QString dispenseIo);

private:
    double m_openOffset = 0;
    double m_closeOffset = 0;
    double m_maximumSpeed = 0;
    double m_endSpeed = 0;
    //    double m_theta = 0;
    QString m_dispenseIo ="SUT_DISPENSE";
};

#endif // DISPENSER_PARAMETER_H