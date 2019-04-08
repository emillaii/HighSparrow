#ifndef XTGENERALOUTPUT_H
#define XTGENERALOUTPUT_H
#include <windows.h>
#include <QObject>
#include <QTime>
#include <QString>

class XtGeneralOutput
{
public:
    XtGeneralOutput();
    XtGeneralOutput(QString name,int id);
    void Init(const QString& output_name);
    void Init(int id_in_profile);

    operator bool();
    operator int();
    XtGeneralOutput &operator =(const bool &b );

    void Set(bool value,int thread = -1);
    void SetOnTllTimeSpan(int time_span, int thread);
    bool Value();
    QString Name();
private:
    QString name;
    int output_id;
    bool is_hp;
public:
    static int count;
};

#endif // XTGENERALOUTPUT_H
