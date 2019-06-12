#ifndef XTGENERALINPUT_H
#define XTGENERALINPUT_H
#include "errorcode.h"
#include <windows.h>
#include <QObject>
#include <QTime>
#include <QString>

class XtGeneralInput:public ErrorBase
{
public:
    XtGeneralInput();
    XtGeneralInput(QString name, int id);
    operator bool();

    void Init(const QString& input_name);
    void Init(int id_in_profile);
    int ID();
    bool Value();
    bool checkState(bool check_state);
    bool Wait(bool value,int timeout);
    QString Name();
private:
    QString name;
    int input_id;
public:
    static int count;
};

#endif // XTGENERALINPUT_H
