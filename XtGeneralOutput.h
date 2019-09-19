#ifndef XTGENERALOUTPUT_H
#define XTGENERALOUTPUT_H
#include "utils/errorcode.h"

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
    bool Set(bool value,int thread = -1);
    void SetOnTllTimeSpan(int time_span, int thread);
    bool SET(bool value,int thread);
    void SetOnTillTimeSpan(int time_span, int thread);
    bool SetStateAndTillTimeSpan(byte state,int time_span, int thread);
    void WaitForFinish(int thread);
    bool Value();
    bool checkState(bool check_state);
    QString Name();
private:
    QString name;
    int output_id = -1;
    bool is_hp;
public:
    static int count;
};

#endif // XTGENERALOUTPUT_H
