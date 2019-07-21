#ifndef CYLINDERPARAMETER_H
#define CYLINDERPARAMETER_H

#include "propertybase.h"



class XtCylinderParameter:public PropertyBase
{
    Q_OBJECT
public:
    XtCylinderParameter():PropertyBase(){}
    Q_PROPERTY(QString cylinderName READ cylinderName WRITE setCylinderName NOTIFY cylinderNameChanged)
    Q_PROPERTY(QString oneOutName READ oneOutName WRITE setOneOutName NOTIFY oneOutNameChanged)
    Q_PROPERTY(QString zeroOutName READ zeroOutName WRITE setZeroOutName NOTIFY zeroOutNameChanged)
    Q_PROPERTY(QString oneInName READ oneInName WRITE setOneInName NOTIFY oneInNameChanged)
    Q_PROPERTY(QString zeroInName READ zeroInName WRITE setZeroInName NOTIFY zeroInNameChanged)
    Q_PROPERTY(int finishOneDelay READ finishOneDelay WRITE setFinishOneDelay NOTIFY finishOneDelayChanged)
    Q_PROPERTY(int finishZeroDelay READ finishZeroDelay WRITE setFinishZeroDelay NOTIFY finishZeroDelayChanged)
    Q_PROPERTY(int outTime READ outTime WRITE setOutTime NOTIFY outTimeChanged)
    QString cylinderName() const
    {
        return m_cylinderName;
    }

    QString oneOutName() const
    {
        return m_oneOutName;
    }

    QString zeroOutName() const
    {
        return m_zeroOutName;
    }

    QString oneInName() const
    {
        return m_oneInName;
    }

    QString zeroInName() const
    {
        return m_zeroInName;
    }

    int finishOneDelay() const
    {
        return m_finishOneDelay;
    }

    int finishZeroDelay() const
    {
        return m_finishZeroDelay;
    }

    int outTime() const
    {
        return m_outTime;
    }

public slots:
    void setCylinderName(QString cylinderName)
    {
        if (m_cylinderName == cylinderName)
            return;

        m_cylinderName = cylinderName;
        emit cylinderNameChanged(m_cylinderName);
    }

    void setOneOutName(QString oneOutName)
    {
        if (m_oneOutName == oneOutName)
            return;

        m_oneOutName = oneOutName;
        emit oneOutNameChanged(m_oneOutName);
    }

    void setZeroOutName(QString zeroOutName)
    {
        if (m_zeroOutName == zeroOutName)
            return;

        m_zeroOutName = zeroOutName;
        emit zeroOutNameChanged(m_zeroOutName);
    }

    void setOneInName(QString oneInName)
    {
        if (m_oneInName == oneInName)
            return;

        m_oneInName = oneInName;
        emit oneInNameChanged(m_oneInName);
    }

    void setZeroInName(QString zeroInName)
    {
        if (m_zeroInName == zeroInName)
            return;

        m_zeroInName = zeroInName;
        emit zeroInNameChanged(m_zeroInName);
    }

    void setFinishOneDelay(int finishOneDelay)
    {
        if (m_finishOneDelay == finishOneDelay)
            return;

        m_finishOneDelay = finishOneDelay;
        emit finishOneDelayChanged(m_finishOneDelay);
    }

    void setFinishZeroDelay(int finishZeroDelay)
    {
        if (m_finishZeroDelay == finishZeroDelay)
            return;

        m_finishZeroDelay = finishZeroDelay;
        emit finishZeroDelayChanged(m_finishZeroDelay);
    }

    void setOutTime(int outTime)
    {
        if (m_outTime == outTime)
            return;

        m_outTime = outTime;
        emit outTimeChanged(m_outTime);
    }

signals:
    void cylinderNameChanged(QString cylinderName);

    void oneOutNameChanged(QString oneOutName);

    void zeroOutNameChanged(QString zeroOutName);

    void oneInNameChanged(QString oneInName);

    void zeroInNameChanged(QString zeroInName);

    void finishOneDelayChanged(int finishOneDelay);

    void finishZeroDelayChanged(int finishZeroDelay);

    void outTimeChanged(int outTime);

private:
    QString m_cylinderName = "Cylinder";
    QString m_oneOutName = "";
    QString m_zeroOutName = "";
    QString m_oneInName = "";
    QString m_zeroInName = "";
    int m_finishOneDelay = 100;
    int m_finishZeroDelay = 100;
    int m_outTime = 3000;
};

#endif // CYLINDERPARAMETER_H
