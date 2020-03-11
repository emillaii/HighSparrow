#ifndef XTCYLINDER_H
#define XTCYLINDER_H
#include "XtGeneralInput.h"
#include "XtGeneralOutput.h"
#include "xtcylinderparameter.h"
#include "utils/errorcode.h"
#include <QString>
class XtCylinder:public ErrorBase
{
public:
    XtCylinder();
    void Init(XtGeneralOutput *output_io, XtGeneralInput *input_fold_io,XtGeneralInput *input_unfold_io,XtGeneralOutput *output_io_zero = nullptr);
    bool Set(bool new_state, bool wait_done = true);
    void SET(int thread, bool new_state);
    bool Wait(bool target_state);
    bool Value();
    bool checkState(bool target_state);
    bool SetValue(bool value);
public:
    XtCylinderParameter parameters;
private:
    XtGeneralInput *in_fold;
    XtGeneralInput *in_unfold;
    XtGeneralOutput *out;
    XtGeneralOutput *out_zero;
};

#endif // XTCYLINDER_H
