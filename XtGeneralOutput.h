#ifndef XTGENERALOUTPUT_H
#define XTGENERALOUTPUT_H
#include "errorcode.h"

#include <windows.h>
#include <QObject>
#include <QTime>
#include <QString>

class XtGeneralOutput:public ErrorBase
{
public:
    XtGeneralOutput();
    XtGeneralOutput(QString name,int id);
    void Init(const QString& output_name);
    void Init(int id_in_profile);

    operator bool();
    operator int();
    XtGeneralOutput &operator =(const bool &b );
    int GetID();
    void Set(bool value,int thread = -1);
    void SetOnTllTimeSpan(int time_span, int thread);
    void SetOnTillTimeSpan(int time_span, int thread);
    void WaitForFInish(int thread);
    bool Value();
    QString Name();
private:
    QString name;
    int output_id = -1;
    bool is_hp;
public:
    static int count;
};

#endif // XTGENERALOUTPUT_H
