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

signals:
    void cylinderNameChanged(QString cylinderName);

    void oneOutNameChanged(QString oneOutName);

    void zeroOutNameChanged(QString zeroOutName);

    void oneInNameChanged(QString oneInName);

    void zeroInNameChanged(QString zeroInName);

private:
    QString m_cylinderName = "Cylinder";
    QString m_oneOutName = "";
    QString m_zeroOutName = "";
    QString m_oneInName = "";
    QString m_zeroInName = "";
};

#endif // CYLINDERPARAMETER_H
