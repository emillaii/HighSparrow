#ifndef XTVACUUMPARAMETER_H
#define XTVACUUMPARAMETER_H

#include "Utils/propertybase.h"



class XtVacuumParameter:public PropertyBase
{
    Q_OBJECT
public:
    XtVacuumParameter():PropertyBase(){}
    Q_PROPERTY(QString vacuumName READ vacuumName WRITE setVacuumName NOTIFY vacuumNameChanged)
    Q_PROPERTY(QString outIoName READ outIoName WRITE setOutIoName NOTIFY outIoNameChanged)
    Q_PROPERTY(QString inIoName READ inIoName WRITE setInIoName NOTIFY inIoNameChanged)
    Q_PROPERTY(QString breakIoName READ breakIoName WRITE setBreakIoName NOTIFY breakIoNameChanged)
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

signals:
    void vacuumNameChanged(QString vacuumName);
    void outIoNameChanged(QString outIoName);

    void inIoNameChanged(QString inIoName);

    void breakIoNameChanged(QString breakIoName);

private:
    QString m_vacuumName = "Vcauum";
    QString m_outIoName = "";
    QString m_inIoName = "";
    QString m_breakIoName = "";
};

#endif // XTVACUUMPARAMETER_H
