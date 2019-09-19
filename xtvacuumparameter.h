#ifndef XTVACUUMPARAMETER_H
#define XTVACUUMPARAMETER_H

#include "PropertyBase.h"



class XtVacuumParameter:public PropertyBase
{
    Q_OBJECT
public:
    XtVacuumParameter():PropertyBase(){}
    Q_PROPERTY(QString vacuumName READ vacuumName WRITE setVacuumName NOTIFY vacuumNameChanged)
    Q_PROPERTY(QString outIoName READ outIoName WRITE setOutIoName NOTIFY outIoNameChanged)
    Q_PROPERTY(QString inIoName READ inIoName WRITE setInIoName NOTIFY inIoNameChanged)
    Q_PROPERTY(QString breakIoName READ breakIoName WRITE setBreakIoName NOTIFY breakIoNameChanged)
    Q_PROPERTY(int outTime READ outTime WRITE setOutTime NOTIFY outTimeChanged)
    Q_PROPERTY(int finishDelay READ finishDelay WRITE setFinishDelay NOTIFY finishDelayChanged)
    Q_PROPERTY(int checkOutTime READ checkOutTime WRITE setCheckOutTime NOTIFY checkOutTimeChanged)
    Q_PROPERTY(bool reserveValue READ reserveValue WRITE setReserveValue NOTIFY reserveValueChanged)
    Q_PROPERTY(int excuteTime READ excuteTime WRITE setExcuteTime NOTIFY excuteTimeChanged)
    QString vacuumName() const
    {
        return m_vacuumName;
    }
    QString outIoName() const
    {
        return m_outIoName;
    }

    QString inIoName() const
    {
        return m_inIoName;
    }

    QString breakIoName() const
    {
        return m_breakIoName;
    }

    int outTime() const
    {
        return m_outTime;
    }

    int finishDelay() const
    {
        return m_finishDelay;
    }

    int checkOutTime() const
    {
        return m_checkOutTime;
    }

    bool reserveValue() const
    {
        return m_reserveValue;
    }

    int excuteTime() const
    {
        return m_excuteTime;
    }

public slots:
    void setVacuumName(QString vacuumName)
    {
        if (m_vacuumName == vacuumName)
            return;

        m_vacuumName = vacuumName;
        emit vacuumNameChanged(m_vacuumName);
    }
    void setOutIoName(QString outIoName)
    {
        if (m_outIoName == outIoName)
            return;

        m_outIoName = outIoName;
        emit outIoNameChanged(m_outIoName);
    }

    void setInIoName(QString inIoName)
    {
        if (m_inIoName == inIoName)
            return;

        m_inIoName = inIoName;
        emit inIoNameChanged(m_inIoName);
    }

    void setBreakIoName(QString breakIoName)
    {
        if (m_breakIoName == breakIoName)
            return;

        m_breakIoName = breakIoName;
        emit breakIoNameChanged(m_breakIoName);
    }

    void setOutTime(int outTime)
    {
        if (m_outTime == outTime)
            return;

        m_outTime = outTime;
        emit outTimeChanged(m_outTime);
    }

    void setFinishDelay(int finishDelay)
    {
        if (m_finishDelay == finishDelay)
            return;

        m_finishDelay = finishDelay;
        emit finishDelayChanged(m_finishDelay);
    }

    void setCheckOutTime(int checkOutTime)
    {
        if (m_checkOutTime == checkOutTime)
            return;

        m_checkOutTime = checkOutTime;
        emit checkOutTimeChanged(m_checkOutTime);
    }

    void setReserveValue(bool reserveValue)
    {
        if (m_reserveValue == reserveValue)
            return;

        m_reserveValue = reserveValue;
        emit reserveValueChanged(m_reserveValue);
    }

    void setExcuteTime(int excuteTime)
    {
        if (m_excuteTime == excuteTime)
            return;

        m_excuteTime = excuteTime;
        emit excuteTimeChanged(m_excuteTime);
    }

signals:
    void vacuumNameChanged(QString vacuumName);
    void outIoNameChanged(QString outIoName);

    void inIoNameChanged(QString inIoName);

    void breakIoNameChanged(QString breakIoName);

    void outTimeChanged(int outTime);

    void finishDelayChanged(int finishDelay);

    void checkOutTimeChanged(int checkOutTime);

    void reserveValueChanged(bool reserveValue);

    void excuteTimeChanged(int excuteTime);

private:
    QString m_vacuumName = "Vcauum";
    QString m_outIoName = "";
    QString m_inIoName = "";
    QString m_breakIoName = "";
    int m_outTime = 200;
    int m_finishDelay = 0;
    int m_checkOutTime = 200;
    bool m_reserveValue = false;
    int m_excuteTime = 100;
};

#endif // XTVACUUMPARAMETER_H
