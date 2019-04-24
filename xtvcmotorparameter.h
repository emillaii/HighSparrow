#ifndef XTVCMOTORPARAMETER_H
#define XTVCMOTORPARAMETER_H

#include "XtVcMotor.h"
#include "propertybase.h"



class VcMotorParameter:public PropertyBase
{
    Q_OBJECT
public:
    VcMotorParameter():PropertyBase(){}
    VCM_Parameter_struct toparameter(){
        VCM_Parameter_struct param;
        param.CanID = canId();
        param.scale = scale();
        param.MaxAcc = maxAcc();
        param.MaxPos = maxRange();
        param.MinPos = minRange();
        param.MaxJerk = MaxJerk();
        param.direction = direction();
        return param;
    }
    Q_PROPERTY(QString motorName READ motorName WRITE setMotorName NOTIFY motorNameChanged)
    Q_PROPERTY(int canId READ canId WRITE setCanId NOTIFY canIdChanged)
    Q_PROPERTY(int channel READ channel WRITE setChannel NOTIFY channelChanged)
    Q_PROPERTY(int direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(double minRange READ minRange WRITE setMinRange NOTIFY minRangeChanged)
    Q_PROPERTY(double maxRange READ maxRange WRITE setMaxRange NOTIFY maxRangeChanged)
    Q_PROPERTY(double scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(double maxVel READ maxVel WRITE setMaxVel NOTIFY maxVelChanged)
    Q_PROPERTY(double maxAcc READ maxAcc WRITE setMaxAcc NOTIFY maxAccChanged)
    Q_PROPERTY(double MaxJerk READ MaxJerk WRITE setMaxJerk NOTIFY MaxJerkChanged)
    QString motorName() const
    {
        return m_motorName;
    }
    int canId() const
    {
        return m_canId;
    }

    int channel() const
    {
        return m_channel;
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

    double MaxJerk() const
    {
        return m_MaxJerk;
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

    void setChannel(int channel)
    {
        if (m_channel == channel)
            return;

        m_channel = channel;
        emit channelChanged(m_channel);
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
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_minRange, minRange))
            return;

        m_minRange = minRange;
        emit minRangeChanged(m_minRange);
    }

    void setMaxRange(double maxRange)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maxRange, maxRange))
            return;

        m_maxRange = maxRange;
        emit maxRangeChanged(m_maxRange);
    }

    void setScale(double scale)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_scale, scale))
            return;

        m_scale = scale;
        emit scaleChanged(m_scale);
    }

    void setMaxVel(double maxVel)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maxVel, maxVel))
            return;

        m_maxVel = maxVel;
        emit maxVelChanged(m_maxVel);
    }

    void setMaxAcc(double maxAcc)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maxAcc, maxAcc))
            return;

        m_maxAcc = maxAcc;
        emit maxAccChanged(m_maxAcc);
    }

    void setMaxJerk(double MaxJerk)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_MaxJerk, MaxJerk))
            return;

        m_MaxJerk = MaxJerk;
        emit MaxJerkChanged(m_MaxJerk);
    }

signals:
    void motorNameChanged(QString motorName);
    void canIdChanged(int canId);

    void channelChanged(int channel);

    void directionChanged(int direction);

    void minRangeChanged(double minRange);

    void maxRangeChanged(double maxRange);

    void scaleChanged(double scale);

    void maxVelChanged(double maxVel);

    void maxAccChanged(double maxAcc);

    void MaxJerkChanged(double MaxJerk);

private:
    QString m_motorName = "Unnaming";
    int m_canId = 0;
    int m_channel = 0;
    int m_direction = 0;
    double m_minRange = 0;
    double m_maxRange = 0;
    double m_scale = 0;
    double m_maxVel = 0;
    double m_maxAcc = 0;
    double m_MaxJerk = 0;
};

#endif // XTVCMOTORPARAMETER_H
