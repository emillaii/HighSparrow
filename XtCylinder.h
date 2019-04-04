#ifndef XTCYLINDER_H
#define XTCYLINDER_H
#include "XtGeneralInput.h"
#include "XtGeneralOutput.h"
#include <QString>
#define CYLINDER_TIMEOUT 3000
#define CYLINDER_INPUT_NULL_DELAY 500
class XtCylinder
{
public:
    XtCylinder(XtGeneralOutput *output_io, XtGeneralInput *input_fold_io,XtGeneralInput *input_unfold_io,QString name = "未命名");
    bool Set(bool new_state, bool wait_done = true, int timeout = CYLINDER_TIMEOUT,int input_null_delay = CYLINDER_INPUT_NULL_DELAY);
    void SET(int thread, bool new_state);
    bool Wait(bool target_state,int timeout = CYLINDER_TIMEOUT,int input_null_delay = CYLINDER_INPUT_NULL_DELAY);
    bool Value();
    bool CheckState(bool target_state);
private:
    QString name;
    XtGeneralInput *in_fold;
    XtGeneralInput *in_unfold;
    XtGeneralOutput *out;
};

#endif // XTCYLINDER_H
