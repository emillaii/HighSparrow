#ifndef XTCYLINDER_H
#define XTCYLINDER_H
#include "XtGeneralInput.h"
#include "XtGeneralOutput.h"
#include "xtcylinderparameter.h"
#include "errorcode.h"
#include <QString>
#define CYLINDER_TIMEOUT 3000
class XtCylinder:public ErrorBase
{
public:
    XtCylinder();
    void Init(XtGeneralOutput *output_io, XtGeneralInput *input_fold_io,XtGeneralInput *input_unfold_io,XtGeneralOutput *output_io_zero = nullptr);
    bool Set(bool new_state, bool wait_done = true, int timeout = CYLINDER_TIMEOUT);
    void SET(int thread, bool new_state);
    bool Wait(bool target_state,int timeout = CYLINDER_TIMEOUT);
    bool Value();
    bool CheckState(bool target_state);
public:
    XtCylinderParameter parameters;
private:
    XtGeneralInput *in_fold;
    XtGeneralInput *in_unfold;
    XtGeneralOutput *out;
    XtGeneralOutput *out_zero;
};

#endif // XTCYLINDER_H
