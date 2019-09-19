#ifndef XTVACUUM_H
#define XTVACUUM_H
#include "XtGeneralInput.h"
#include "XtGeneralOutput.h"
#include "utils/errorcode.h"
#include "xtvacuumparameter.h"
#include <QString>
class XtVacuum :public ErrorBase
{
public:
    XtVacuum();
    void Init(XtGeneralOutput *output_io, XtGeneralInput *input_io,XtGeneralOutput *output_break_io);
    bool Set(bool new_state, bool wait_done = true, bool open_break = true);
    bool SetSimulation(bool new_state,bool open_io = false);
    void SET(int thread, bool new_state);
    bool Wait(bool target_state);
    bool checkHasMaterielSync();
    bool checkHasMateriel(int thread);
    bool getHasMateriel(int thread);
    bool checkState(bool check_state);
public:
    XtVacuumParameter parameters;
    static bool no_material;
private:
    XtGeneralInput *in_io;
    XtGeneralOutput *out_io;
    XtGeneralOutput *break_io;
};

#endif // XTVACUUM_H
