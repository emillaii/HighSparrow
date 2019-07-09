#ifndef XTVACUUM_H
#define XTVACUUM_H
#include "XtMotion/XtGeneralInput.h"
#include "XtMotion/XtGeneralOutput.h"
#include "errorcode.h"
#include "XtMotion/xtvacuumparameter.h"
#include <QString>
#define VACUUM_TIMEOUT 3000
#define VACUUM_INPUT_NULL_DELAY 1000
#define VACUUM_FINISH_DELAY 20
class XtVacuum :public ErrorBase
{
public:
    XtVacuum();
    void Init(XtGeneralOutput *output_io, XtGeneralInput *input_io,XtGeneralOutput *output_break_io);
    bool Set(bool new_state, bool wait_done = true,int finish_delay = VACUUM_FINISH_DELAY, int timeout = VACUUM_TIMEOUT,int input_null_delay = VACUUM_INPUT_NULL_DELAY);
    void SET(int thread, bool new_state);
    bool Wait(bool target_state,int timeout = VACUUM_TIMEOUT,int finish_delay = VACUUM_FINISH_DELAY,int input_null_delay = VACUUM_INPUT_NULL_DELAY);
    bool IsVacuum();
public:
    XtVacuumParameter parameters;
private:
    QString name;
    XtGeneralInput *in_io;
    XtGeneralOutput *out_io;
    XtGeneralOutput *break_io;
};

#endif // XTVACUUM_H
