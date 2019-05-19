#ifndef IOLIMITPARAMETER_H
#define IOLIMITPARAMETER_H

#include "propertybase.h"

#include <QVariantList>



class IOLimitParameter:public PropertyBase
{
    Q_OBJECT
public:
    IOLimitParameter():PropertyBase(){}
    bool checkInLimitSpance(bool io_state)
    {
        if(io_state == m_inputIOState)
        {
            qInfo("check In Limit Spance");
            return true;
        }
        qInfo("check not In Limit Spance");
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
                qInfo("check pos (%f,%f) has Interference With(%f,%f)",temp_start,temp_end,m_moveSpance[2*i].toDouble(),m_moveSpance[2*i+1].toDouble());
                return true;
            }
            qInfo("check pos (%f,%f)  has no Interference With (%f,%f)",temp_start,temp_end,m_moveSpance[2*i].toDouble(),m_moveSpance[2*i+1].toDouble());
        }
        return false;
    }
    Q_PROPERTY(QVariantList moveSpance READ moveSpance WRITE setMoveSpance NOTIFY moveSpanceChanged)
    Q_PROPERTY(QString inputIOName READ inputIOName WRITE setInputIOName NOTIFY inputIONameChanged)
    Q_PROPERTY(bool inputIOState READ inputIOState WRITE setInputIOState NOTIFY inputIOStateChanged)
    QVariantList moveSpance() const
    {
        return m_moveSpance;
    }
    QString inputIOName() const
    {
        return m_inputIOName;
    }

    bool inputIOState() const
    {
        return m_inputIOState;
    }

public slots:
    void setMoveSpance(QVariantList moveSpance)
    {
        if (m_moveSpance == moveSpance)
            return;

        m_moveSpance = moveSpance;
        emit moveSpanceChanged(m_moveSpance);
    }
    void setInputIOName(QString inputIOName)
    {
        if (m_inputIOName == inputIOName)
            return;

        m_inputIOName = inputIOName;
        emit inputIONameChanged(m_inputIOName);
    }

    void setInputIOState(bool inputIOState)
    {
        if (m_inputIOState == inputIOState)
            return;

        m_inputIOState = inputIOState;
        emit inputIOStateChanged(m_inputIOState);
    }

signals:
    void moveSpanceChanged(QVariantList moveSpance);

    void inputIONameChanged(QString inputIOName);

    void inputIOStateChanged(bool inputIOState);

private:
    QVariantList m_moveSpance;
    QString m_inputIOName = "";
    bool m_inputIOState = false;
};

#endif // IOLIMITPARAMETER_H
