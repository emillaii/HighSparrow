#ifndef IOLIMITPARAMETER_H
#define IOLIMITPARAMETER_H

#include "errorcode.h"
#include "propertybase.h"

#include <QVariantList>



class  IOLimitParameter:public PropertyBase,public ErrorBase
{
    Q_OBJECT
public:
    IOLimitParameter():PropertyBase(),ErrorBase(){}
    bool checkInputInLimitSpance(int i,bool io_state)
    {
        if(io_state == m_inputIOState[i].toBool())
            return true;
        AppendError(QString(u8"检测到输入IO(%1)状态为%2,要求状态为%3。").arg(m_inputIOName[i].toString()).arg(io_state).arg(m_inputIOState[i].toBool()));
        qInfo("check %s not In Limit state %d",m_inputIOName[i].toString().toStdString().c_str(),m_inputIOState[i].toBool());
        return false;
    }

    bool checkOutputLimitSpance(int i,bool io_state)
    {
        if(io_state == m_outputIOState[i].toBool())
            return true;
        AppendError(QString(u8"检测到输出IO(%1)状态为%2,要求状态为%3。").arg(m_outputIOName[i].toString()).arg(io_state).arg(m_outputIOState[i].toBool()));
        qInfo("check %s not In Limit state %d",m_outputIOName[i].toString().toStdString().c_str(),m_outputIOState[i].toBool());
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
                return true;
        }
        return false;
    }
    Q_PROPERTY(QVariantList moveSpance READ moveSpance WRITE setMoveSpance NOTIFY moveSpanceChanged)
    Q_PROPERTY(QVariantList inputIOName READ inputIOName WRITE setInputIOName NOTIFY inputIONameChanged)
    Q_PROPERTY(QVariantList outputIOName READ outputIOName WRITE setOutputIOName NOTIFY outputIONameChanged)
    Q_PROPERTY(QVariantList inputIOState READ inputIOState WRITE setInputIOState NOTIFY inputIOStateChanged)
    Q_PROPERTY(QVariantList outputIOState READ outputIOState WRITE setOutputIOState NOTIFY outputIOStateChanged)
    Q_PROPERTY(bool crashSpance READ crashSpance WRITE setCrashSpance NOTIFY crashSpanceChanged)
    QVariantList moveSpance() const
    {
        return m_moveSpance;
    }
    QVariantList inputIOName() const
    {
        return m_inputIOName;
    }

    QVariantList outputIOName() const
    {
        return m_outputIOName;
    }

    QVariantList inputIOState() const
    {
        return m_inputIOState;
    }

    QVariantList outputIOState() const
    {
        return m_outputIOState;
    }

public slots:
    void setMoveSpance(QVariantList moveSpance)
    {
        if (m_moveSpance == moveSpance)
            return;

        m_moveSpance = moveSpance;
        emit moveSpanceChanged(m_moveSpance);
    }
    void setInputIOName(QVariantList inputIOName)
    {
        if (m_inputIOName == inputIOName)
            return;

        m_inputIOName = inputIOName;
        emit inputIONameChanged(m_inputIOName);
    }

    void setOutputIOName(QVariantList outputIOName)
    {
        if (m_outputIOName == outputIOName)
            return;

        m_outputIOName = outputIOName;
        emit outputIONameChanged(m_outputIOName);
    }

    void setInputIOState(QVariantList inputIOState)
    {
        if (m_inputIOState == inputIOState)
            return;

        m_inputIOState = inputIOState;
        emit inputIOStateChanged(m_inputIOState);
    }

    void setOutputIOState(QVariantList outputIOState)
    {
        if (m_outputIOState == outputIOState)
            return;

        m_outputIOState = outputIOState;
        emit outputIOStateChanged(m_outputIOState);
    }

    void setCrashSpance(bool crashSpance)
    {
        if (m_crashSpance == crashSpance)
            return;

        m_crashSpance = crashSpance;
        emit crashSpanceChanged(m_crashSpance);
    }

signals:
    void moveSpanceChanged(QVariantList moveSpance);
    void inputIONameChanged(QVariantList inputIOName);

    void outputIONameChanged(QVariantList outputIOName);

    void inputIOStateChanged(QVariantList inputIOState);

    void outputIOStateChanged(QVariantList outputIOState);
    void crashSpanceChanged(bool crashSpance);

public:
    QList<int> input_io_indexs;
    QList<int> output_io_indexs;
    bool crashSpance() const
    {
        return m_crashSpance;
    }

private:
    QVariantList m_moveSpance;
    QVariantList m_inputIOName;
    QVariantList m_outputIOName;
    QVariantList m_inputIOState;
    QVariantList m_outputIOState;
    bool m_crashSpance = false;
};

#endif // IOLIMITPARAMETER_H
