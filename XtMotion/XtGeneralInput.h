#ifndef XTGENERALINPUT_H
#define XTGENERALINPUT_H
#include <windows.h>
#include <QObject>
#include <QTime>
#include <QString>
#include "Utils/propertybase.h"

class XtGeneralInput
{
public:
    XtGeneralInput();
    XtGeneralInput(QString name, int id);
    operator bool();

    void Init(const QString& input_name);
    void Init(int id_in_profile);
    int ID();
    bool Value();
    bool Wait(bool value,int timeout);
    QString Name();

private:
    QString name;
    int input_id;
public:
static int count;
};

class XtGeneralInputParameter:public PropertyBase{
    Q_OBJECT

public:
    XtGeneralInputParameter():PropertyBase(){}
    Q_PROPERTY(QString oneInName READ oneInName WRITE setOneInName NOTIFY oneInNameChanged)
    QString oneInName() const
    {
        return m_oneInName;
    }
public slots:
    void setOneInName(QString oneInName)
    {
        if (m_oneInName == oneInName)
            return;

        m_oneInName = oneInName;
        emit oneInNameChanged(m_oneInName);
    }
signals:
    void oneInNameChanged(QString oneInName);
private:
    QString m_oneInName;
};


#endif // XTGENERALINPUT_H
