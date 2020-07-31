#ifndef XTVCMOTORPARAMETER_H
#define XTVCMOTORPARAMETER_H

#include "propertybase.h"



class VcMotorParameter:public PropertyBase
{
    Q_OBJECT
public:
    VcMotorParameter():PropertyBase(){}
    Q_PROPERTY(QString motorName READ motorName WRITE setMotorName NOTIFY motorNameChanged)
    Q_PROPERTY(int canId READ canId WRITE setCanId NOTIFY canIdChanged)
    Q_PROPERTY(int direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(double minRange READ minRange WRITE setMinRange NOTIFY minRangeChanged)
    Q_PROPERTY(double maxRange READ maxRange WRITE setMaxRange NOTIFY maxRangeChanged)
    Q_PROPERTY(double scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(double maxVel READ maxVel WRITE setMaxVel NOTIFY maxVelChanged)
    Q_PROPERTY(double maxAcc READ maxAcc WRITE setMaxAcc NOTIFY maxAccChanged)
    Q_PROPERTY(double maxJerk READ maxJerk WRITE setMaxJerk NOTIFY MaxJerkChanged)
    Q_PROPERTY(double findOriginCurrent READ findOriginCurrent WRITE setFindOriginCurrent NOTIFY FindOriginCurrentChanged)
    Q_PROPERTY(double touchDistance READ touchDistance WRITE setTouchDistance NOTIFY TouchDistanceChanged)
    Q_PROPERTY(int direntionAfterSeek READ direntionAfterSeek WRITE setDirentionAfterSeek NOTIFY direntionAfterSeekChanged)
    Q_PROPERTY(double originOffset READ originOffset WRITE setOriginOffset NOTIFY originOffsetChanged)
    Q_PROPERTY(bool needMapForce READ needMapForce WRITE setNeedMapForce NOTIFY needMapForceChanged)
    QString motorName() const
    {
        return m_motorName;
    }
    int canId() const
    {
        return m_canId;
    }

    int direction() const
    {
        return m_direction;
    }

    double minRange() const
    {
        return m_minRange;
    }

    double maxRange() const
    {
        return m_maxRange;
    }

    double scale() const
    {
        return m_scale;
    }

    double maxVel() const
    {
        return m_maxVel;
    }

    double maxAcc() const
    {
        return m_maxAcc;
    }

    double maxJerk() const
    {
        return m_MaxJerk;
    }

    double findOriginCurrent() const
    {
        return m_LimitCurrent;
    }

    double touchDistance() const
    {
        return m_TouchDistance;
    }

    int direntionAfterSeek() const
    {
        return m_direntionAfterSeek;
    }

    double originOffset() const
    {
        return m_originOffset;
    }

    bool needMapForce() const
    {
        return m_needMapForce;
    }

public slots:
    void setMotorName(QString motorName)
    {
        if (m_motorName == motorName)
            return;

        m_motorName = motorName;
        emit motorNameChanged(m_motorName);
    }
    void setCanId(int canId)
    {
        if (m_canId == canId)
            return;

        m_canId = canId;
        emit canIdChanged(m_canId);
    }


    void setDirection(int direction)
    {
        if (m_direction == direction)
            return;

        m_direction = direction;
        emit directionChanged(m_direction);
    }

    void setMinRange(double minRange)
    {
        if (qFuzzyCompare(m_minRange, minRange))
            return;

        m_minRange = minRange;
        emit minRangeChanged(m_minRange);
    }

    void setMaxRange(double maxRange)
    {
        if (qFuzzyCompare(m_maxRange, maxRange))
            return;

        m_maxRange = maxRange;
        emit maxRangeChanged(m_maxRange);
    }

    void setScale(double scale)
    {
        if (qFuzzyCompare(m_scale, scale))
            return;

        m_scale = scale;
        emit scaleChanged(m_scale);
    }

    void setMaxVel(double maxVel)
    {
        if (qFuzzyCompare(m_maxVel, maxVel))
            return;

        m_maxVel = maxVel;
        emit maxVelChanged(m_maxVel);
    }

    void setMaxAcc(double maxAcc)
    {
        if (qFuzzyCompare(m_maxAcc, maxAcc))
            return;

        m_maxAcc = maxAcc;
        emit maxAccChanged(m_maxAcc);
    }

    void setMaxJerk(double MaxJerk)
    {
        if (qFuzzyCompare(m_MaxJerk, MaxJerk))
            return;

        m_MaxJerk = MaxJerk;
        emit MaxJerkChanged(m_MaxJerk);
    }

    void setFindOriginCurrent(double LimitCurrent)
    {
        if (qFuzzyCompare(m_LimitCurrent, LimitCurrent))
            return;

        m_LimitCurrent = LimitCurrent;
        emit FindOriginCurrentChanged(m_LimitCurrent);
    }

    void setTouchDistance(double TouchDistance)
    {
        if (qFuzzyCompare(m_TouchDistance, TouchDistance))
            return;

        m_TouchDistance = TouchDistance;
        emit TouchDistanceChanged(m_TouchDistance);
    }

    void setDirentionAfterSeek(int direntionAfterSeek)
    {
        if (m_direntionAfterSeek == direntionAfterSeek)
            return;

        m_direntionAfterSeek = direntionAfterSeek;
        emit direntionAfterSeekChanged(m_direntionAfterSeek);
    }

    void setOriginOffset(double originOffset)
    {
        if (qFuzzyCompare(m_originOffset, originOffset))
            return;

        m_originOffset = originOffset;
        emit originOffsetChanged(m_originOffset);
    }

    void setNeedMapForce(bool needMapForce)
    {
        if (m_needMapForce == needMapForce)
            return;

        m_needMapForce = needMapForce;
        emit needMapForceChanged(m_needMapForce);
    }

signals:
    void motorNameChanged(QString motorName);
    void canIdChanged(int canId);

    void directionChanged(int direction);

    void minRangeChanged(double minRange);

    void maxRangeChanged(double maxRange);

    void scaleChanged(double scale);

    void maxVelChanged(double maxVel);

    void maxAccChanged(double maxAcc);

    void MaxJerkChanged(double maxJerk);

    void FindOriginCurrentChanged(double findOriginCurrent);

    void TouchDistanceChanged(double touchDistance);


    void direntionAfterSeekChanged(int direntionAfterSeek);

    void originOffsetChanged(double originOffset);

    void needMapForceChanged(bool needMapForce);

private:
    QString m_motorName = "Unnaming";
    int m_canId = 0;
    int m_direction = 0;
    double m_minRange = 0;
    double m_maxRange = 0;
    double m_scale = 0;
    double m_maxVel = 0;
    double m_maxAcc = 0;
    double m_MaxJerk = 0;
    double m_LimitCurrent = -1;
    double m_TouchDistance = 0;
    int m_direntionAfterSeek = 1;
    double m_originOffset = 0;
    bool m_needMapForce = true;
};

#endif // XTVCMOTORPARAMETER_H
