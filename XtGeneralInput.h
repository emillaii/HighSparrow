#ifndef XTGENERALINPUT_H
#define XTGENERALINPUT_H
#include <windows.h>
#include <QObject>
#include <QTime>
#include <QString>

class XtGeneralInput
{
public:
    XtGeneralInput();
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
};

#endif // XTGENERALINPUT_H
