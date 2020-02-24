#ifndef CHARTHEIGHTCONTROLMODULEPARAMETER_H
#define CHARTHEIGHTCONTROLMODULEPARAMETER_H

#include "Utils/propertybase.h"

class ChartHeightControlModuleParameter :public PropertyBase
{
    Q_OBJECT


public:

    explicit ChartHeightControlModuleParameter();

    Q_PROPERTY(QString motorLFName READ motorLFName WRITE setMotorLFName NOTIFY motorLFNameChanged)
    Q_PROPERTY(QString motorLRName READ motorLRName WRITE setMotorLRName NOTIFY motorLRNameChanged)
    Q_PROPERTY(QString motorRFName READ motorRFName WRITE setMotorRFName NOTIFY motorRFNameChanged)
    Q_PROPERTY(QString motorRRName READ motorRRName WRITE setMotorRRName NOTIFY motorRRNameChanged)


    QString motorLFName() const
    {
        return m_motorLFName;
    }
    QString motorLRName() const
    {
        return m_motorLRName;
    }

    QString motorRFName() const
    {
        return m_motorRFName;
    }

    QString motorRRName() const
    {
        return m_motorRRName;
    }

public slots:
    void setMotorLFName(QString motorLFName)
    {
        if (m_motorLFName == motorLFName)
            return;

        m_motorLFName = motorLFName;
        emit motorLFNameChanged(m_motorLFName);
    }
    void setMotorLRName(QString motorLRName)
    {
        if (m_motorLRName == motorLRName)
            return;

        m_motorLRName = motorLRName;
        emit motorLRNameChanged(m_motorLRName);
    }

    void setMotorRFName(QString motorRFName)
    {
        if (m_motorRFName == motorRFName)
            return;

        m_motorRFName = motorRFName;
        emit motorRFNameChanged(m_motorRFName);
    }

    void setMotorRRName(QString motorRRName)
    {
        if (m_motorRRName == motorRRName)
            return;

        m_motorRRName = motorRRName;
        emit motorRRNameChanged(m_motorRRName);
    }

signals:
    void motorLFNameChanged(QString motorLFName);
    void motorLRNameChanged(QString motorLRName);
    void motorRFNameChanged(QString motorRFName);
    void motorRRNameChanged(QString motorRRName);

private:

    QString m_motorLFName = "";

    QString m_motorLRName = "";

    QString m_motorRFName = "";

    QString m_motorRRName = "";

};


#endif // CHARTHEIGHTCONTROLMODULEPARAMETER_H
