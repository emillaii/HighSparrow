#ifndef MOTORLIMITPARAMETER_H
#define MOTORLIMITPARAMETER_H

#include "propertybase.h"

#include <QVariantList>



class VerticalLimitParameter:public PropertyBase
{
    Q_OBJECT
public:
    VerticalLimitParameter():PropertyBase(){}
    bool checkInLimitSpance(double start_position,double end_position)
    {
        double temp_start,temp_end;
        if(start_position > end_position)
        {
            temp_start = end_position;
            temp_end = start_position;
        }
        else
        {
            temp_start = start_position;
            temp_end = end_position;
        }
        for (int i = 0; i < m_limitSpance.size()/2; ++i)
        {
           if(m_limitSpance[2*i].toDouble()<=temp_start&&temp_end<=m_limitSpance[2*i+1].toDouble())
           {
//               qInfo("check pos (%f,%f) In Limit (%f,%f)",temp_start,temp_end,m_limitSpance[2*i].toDouble(),m_limitSpance[2*i+1].toDouble());
               return true;
           }
//           qInfo("check pos (%f,%f) not in Limit (%f,%f)",temp_start,temp_end,m_limitSpance[2*i].toDouble(),m_limitSpance[2*i+1].toDouble());
        }
        return false;
    }
    bool hasInterferenceWithMoveSpance(double start_position,double end_position)
    {
        double temp_start,temp_end;
        if(start_position > end_position)
        {
            temp_start = end_position;
            temp_end = start_position;
        }
        else
        {
            temp_start = start_position;
            temp_end = end_position;
        }
        for (int i = 0; i < m_moveSpance.size()/2; ++i)
        {
            if(m_moveSpance[2*i].toDouble()<=temp_end&&m_moveSpance[2*i+1].toDouble()>=temp_start)
            {
//                qInfo("check pos (%f,%f) has Interference With (%f,%f)",temp_start,temp_end,m_moveSpance[2*i].toDouble(),m_moveSpance[2*i+1].toDouble());
                return true;
            }

//            qInfo("check pos (%f,%f) has no Interference With (%f,%f)",temp_start,temp_end,m_moveSpance[2*i].toDouble(),m_moveSpance[2*i+1].toDouble());
        }
        return false;
    }
    Q_PROPERTY(QVariantList moveSpance READ moveSpance WRITE setMoveSpance NOTIFY moveSpanceChanged)
    Q_PROPERTY(QString motorName READ motorName WRITE setMotorName NOTIFY motorNameChanged)
    Q_PROPERTY(QVariantList limitSpance READ limitSpance WRITE setLimitSpance NOTIFY limitSpanceChanged)

    QVariantList moveSpance() const
    {
        return m_moveSpance;
    }
    QString motorName() const
    {
        return m_motorName;
    }

    QVariantList limitSpance() const
    {
        return m_limitSpance;
    }

public slots:
    void setMoveSpance(QVariantList moveSpance)
    {
        if (m_moveSpance == moveSpance)
            return;

        m_moveSpance = moveSpance;
        emit moveSpanceChanged(m_moveSpance);
    }
    void setMotorName(QString motorName)
    {
        if (m_motorName == motorName)
            return;

        m_motorName = motorName;
        emit motorNameChanged(m_motorName);
    }

    void setLimitSpance(QVariantList limitSpance)
    {
        if (m_limitSpance == limitSpance)
            return;

        m_limitSpance = limitSpance;
        emit limitSpanceChanged(m_limitSpance);
    }

signals:
    void moveSpanceChanged(QVariantList moveSpance);
    void motorNameChanged(QString motorName);

    void limitSpanceChanged(QVariantList limitSpance);

private:
    QVariantList m_moveSpance;
    QString m_motorName = "";
    QVariantList m_limitSpance;
};

#endif // MOTORLIMITPARAMETER_H
