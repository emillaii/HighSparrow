#ifndef PARALLELLIMITPARAMETER_H
#define PARALLELLIMITPARAMETER_H

#include "propertybase.h"

#include <QVariantList>



class ParallelLimitParameter:public PropertyBase
{
    Q_OBJECT
public:
    //防止追尾其它轴
    ParallelLimitParameter():PropertyBase (){}
    bool hasInInterferenceSpance(double start_x,double end_x,double start_y,double end_y)
    {
        double temp_start,temp_end;
        if(start_x > end_x)
        {
            temp_start = end_x;
            temp_end = start_x;
        }
        else
        {
            temp_start = start_x;
            temp_end = end_x;
        }
        bool result = false;
        if(m_effectXSpance.size()<2)
            result = true;
        else
            for (int i = 0; i < m_effectXSpance.size()/2; ++i)
                if(m_effectXSpance[2*i].toDouble()<=temp_end&&m_effectXSpance[2*i+1].toDouble()>=temp_start)
                    result = true;
        if(!result)
            return false;
        if(start_y > end_y)
        {
            temp_start = end_y;
            temp_end = start_y;
        }
        else
        {
            temp_start = start_y;
            temp_end = end_y;
        }
        result = false;
        if(m_effectXSpance.size()<2)
            result = true;
        else
            for (int i = 0; i < m_effectYSpance.size()/2; ++i)
                if(m_effectYSpance[2*i].toDouble()<=temp_end&&m_effectYSpance[2*i+1].toDouble()>=temp_start)
                    result = true;
        return result;
    }
    bool checkInSafeDistance(const double start_position,const double end_position,const double other_start,const double other_end)
    {
        double other_min,max_position;
        if(isDistanceParallel())
            other_min = other_start < other_end?other_start:other_end;
        else
            other_min = other_start < other_end?-other_end:-other_start;
        if(isDistanceAntiparallel())
            max_position = start_position > end_position?start_position:end_position;
        else
            max_position = start_position > end_position?-end_position:-start_position;
        if(other_min - max_position > LimitDistance())
            return true;
        else
            return false;
    }
    Q_PROPERTY(double LimitDistance READ LimitDistance WRITE setLimitDistance NOTIFY LimitDistanceChanged)
    Q_PROPERTY(bool isDistanceAntiparallel READ isDistanceAntiparallel WRITE setIsDistanceAntiparallel NOTIFY isDistanceAntiparallelChanged)
    Q_PROPERTY(QString motorName READ motorName WRITE setMotorName NOTIFY motorNameChanged)
    Q_PROPERTY(bool isDistanceParallel READ isDistanceParallel WRITE setIsDistanceParallel NOTIFY isDistanceParallelChanged)
    Q_PROPERTY(QString effectMotorXName READ effectMotorXName WRITE setEffectMotorXName NOTIFY effectMotorXNameChanged)
    Q_PROPERTY(QString effectMotorYName READ effectMotorYName WRITE setEffectMotorYName NOTIFY effectMotorYNameChanged)
    Q_PROPERTY(QVariantList effectXSpance READ effectXSpance WRITE setEffectXSpance NOTIFY effectXSpanceChanged)
    Q_PROPERTY(QVariantList effectYSpance READ effectYSpance WRITE setEffectYSpance NOTIFY effectYSpanceChanged)

    double LimitDistance() const
    {
        return m_LimitDistance;
    }
    bool isDistanceParallel() const
    {
        return m_isDistanceParallel;
    }

    QString motorName() const
    {
        return m_motorName;
    }

    bool isDistanceAntiparallel() const
    {
        return m_isDistanceAntiparallel;
    }

    QString effectMotorXName() const
    {
        return m_effectMotorXName;
    }

    QString effectMotorYName() const
    {
        return m_effectMotorYName;
    }

    QVariantList effectXSpance() const
    {
        return m_effectXSpance;
    }

    QVariantList effectYSpance() const
    {
        return m_effectYSpance;
    }

public slots:
    void setLimitDistance(double LimitDistance)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_LimitDistance, LimitDistance))
            return;

        m_LimitDistance = LimitDistance;
        emit LimitDistanceChanged(m_LimitDistance);
    }
    void setIsDistanceParallel(bool isDistanceParallel)
    {
        if (m_isDistanceParallel == isDistanceParallel)
            return;

        m_isDistanceParallel = isDistanceParallel;
        emit isDistanceParallelChanged(m_isDistanceParallel);
    }

    void setMotorName(QString motorName)
    {
        if (m_motorName == motorName)
            return;

        m_motorName = motorName;
        emit motorNameChanged(m_motorName);
    }

    void setIsDistanceAntiparallel(bool isDistanceAntiparallel)
    {
        if (m_isDistanceAntiparallel == isDistanceAntiparallel)
            return;

        m_isDistanceAntiparallel = isDistanceAntiparallel;
        emit isDistanceAntiparallelChanged(m_isDistanceAntiparallel);
    }

    void setEffectMotorXName(QString effectMotorXName)
    {
        if (m_effectMotorXName == effectMotorXName)
            return;

        m_effectMotorXName = effectMotorXName;
        emit effectMotorXNameChanged(m_effectMotorXName);
    }

    void setEffectMotorYName(QString effectMotorYName)
    {
        if (m_effectMotorYName == effectMotorYName)
            return;

        m_effectMotorYName = effectMotorYName;
        emit effectMotorYNameChanged(m_effectMotorYName);
    }

    void setEffectXSpance(QVariantList effectXSpance)
    {
        if (m_effectXSpance == effectXSpance)
            return;

        m_effectXSpance = effectXSpance;
        emit effectXSpanceChanged(m_effectXSpance);
    }

    void setEffectYSpance(QVariantList effectYSpance)
    {
        if (m_effectYSpance == effectYSpance)
            return;

        m_effectYSpance = effectYSpance;
        emit effectYSpanceChanged(m_effectYSpance);
    }

signals:
    void LimitDistanceChanged(double LimitDistance);
    void isDistanceParallelChanged(bool isDistanceParallel);

    void motorNameChanged(QString motorName);

    void isDistanceAntiparallelChanged(bool isDistanceAntiparallel);

    void effectMotorXNameChanged(QString effectMotorXName);

    void effectMotorYNameChanged(QString effectMotorYName);

    void effectXSpanceChanged(QVariantList effectXSpance);

    void effectYSpanceChanged(QVariantList effectYSpance);

private:
    double m_LimitDistance = 0;
    bool m_isDistanceParallel = false;
    QString m_motorName = "";
    bool m_isDistanceAntiparallel = false;
    QString m_effectMotorXName = "";
    QString m_effectMotorYName = "";
    QVariantList m_effectXSpance;
    QVariantList m_effectYSpance;
};

#endif // PARALLELLIMITPARAMETER_H
