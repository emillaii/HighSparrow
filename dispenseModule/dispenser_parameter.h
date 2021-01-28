#ifndef DISPENSER_PARAMETER_H
#define DISPENSER_PARAMETER_H

#include "PropertyBase.h"

#include <QVariantList>

class DispenserParameter:public PropertyBase
{
    Q_OBJECT
public:
    DispenserParameter():PropertyBase(){}
    Q_PROPERTY(double openOffset READ openOffset WRITE setOpenOffset NOTIFY openOffsetChanged)
    Q_PROPERTY(double closeOffset READ closeOffset WRITE setCloseOffset NOTIFY closeOffsetChanged)
    Q_PROPERTY(double maximumSpeed READ maximumSpeed WRITE setMaximumSpeed NOTIFY maximumSpeedChanged)
    Q_PROPERTY(int speedCount READ speedCount WRITE setSpeedCount NOTIFY speedCountChanged)
    Q_PROPERTY(QVariantList lineSpeeds READ lineSpeeds WRITE setLineSpeeds NOTIFY lineSpeedsChanged)
    Q_PROPERTY(double endSpeed READ endSpeed WRITE setEndSpeed NOTIFY endSpeedChanged)
    Q_PROPERTY(QString dispenseIo READ dispenseIo WRITE setDispenseIo NOTIFY DispenseIoChanged)
    Q_PROPERTY(QString glueLevelCheckIO READ glueLevelCheckIO WRITE setGlueLevelCheckIO NOTIFY glueLevelCheckIOChanged)
    Q_PROPERTY(bool enableGlueLevelCheck READ enableGlueLevelCheck WRITE setEnableGlueLevelCheck NOTIFY glueLevelCheckChanged)
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

    QVariantList lineSpeeds() const
    {
        return m_lineSpeeds;
    }

    int speedCount() const
    {
        return m_speedCount;
    }

    QString glueLevelCheckIO() const
    {
        return m_glueLevelCheckIO;
    }

    bool enableGlueLevelCheck() const
    {
        return m_enableGlueLevelCheck;
    }

public slots:
    void setOpenOffset(double openOffset)
    {
        if (qFuzzyCompare(m_openOffset, openOffset))
            return;

        m_openOffset = openOffset;
        emit openOffsetChanged(m_openOffset);
    }
    void setCloseOffset(double closeOffset)
    {
        if (qFuzzyCompare(m_closeOffset, closeOffset))
            return;

        m_closeOffset = closeOffset;
        emit closeOffsetChanged(m_closeOffset);
    }

    void setMaximumSpeed(double maximumSpeed)
    {
        if (qFuzzyCompare(m_maximumSpeed, maximumSpeed))
            return;

        m_maximumSpeed = maximumSpeed;
        emit maximumSpeedChanged(m_maximumSpeed);
    }

    void setEndSpeed(double endSpeed)
    {
        if (qFuzzyCompare(m_endSpeed, endSpeed))
            return;

        m_endSpeed = endSpeed;
        emit endSpeedChanged(m_endSpeed);
    }

//    void setTheta(double theta)
//    {
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

    void setLineSpeeds(QVariantList lineSpeeds)
    {
        if (m_lineSpeeds == lineSpeeds)
            return;

        m_lineSpeeds = lineSpeeds;
        emit lineSpeedsChanged(m_lineSpeeds);
    }

    void setLineSpeed(int index, double value)
    {
        if (index < m_lineSpeeds.length()) {
            m_lineSpeeds[index] = value;
            emit lineSpeedsChanged(m_lineSpeeds);
        }
    }

    void setSpeedCount(int speedCount)
    {
        if (m_speedCount == speedCount)
            return;

        m_speedCount = speedCount;
        int temp_count = 2*m_speedCount;
        while(m_lineSpeeds.size()!= temp_count)
        {
            if(m_lineSpeeds.size() > temp_count)
            {
                m_lineSpeeds.removeLast();
            }
            else if (m_lineSpeeds.size() < temp_count)
            {
                m_lineSpeeds.append(m_maximumSpeed);
                m_lineSpeeds.append(m_endSpeed);
            }
        }
        emit speedCountChanged(m_speedCount);
        emit lineSpeedsChanged(m_lineSpeeds);
    }

    void setGlueLevelCheckIO(QString glueLevelCheckIO)
    {
        if (m_glueLevelCheckIO == glueLevelCheckIO)
            return;

        m_glueLevelCheckIO = glueLevelCheckIO;
        emit glueLevelCheckIOChanged(m_glueLevelCheckIO);
    }

    void setEnableGlueLevelCheck(bool enableGlueLevelCheck)
    {
        if (m_enableGlueLevelCheck == enableGlueLevelCheck)
            return;

        m_enableGlueLevelCheck = enableGlueLevelCheck;
        emit glueLevelCheckChanged(m_enableGlueLevelCheck);
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

    void lineSpeedsChanged(QVariantList lineSpeeds);

    void speedCountChanged(int speedCount);

    void glueLevelCheckIOChanged(QString glueLevelCheckIO);

    void glueLevelCheckChanged(bool enableGlueLevelCheck);

private:
    double m_openOffset = 0;
    double m_closeOffset = 0;
    double m_maximumSpeed = 0;
    double m_endSpeed = 0;
    //    double m_theta = 0;
    QString m_dispenseIo ="SUT_DISPENSE";
    QVariantList m_lineSpeeds;
    int m_speedCount = 0;
    QString m_glueLevelCheckIO = "Glue_Level_Check_IO";
    bool m_enableGlueLevelCheck = false;
};

#endif // DISPENSER_PARAMETER_H
