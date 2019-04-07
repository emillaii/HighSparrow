#ifndef LUT_MODULE_H
#define LUT_MODULE_H

#include "material_carrier.h"
#include "wordoplight.h"
#include "baslerpyloncamera.h"
#include "lut_parameter.h"
#include "visionmodule.h"

#include <QObject>

class LutModule
{
public:
    LutModule(QString name, XtMotor* motor_x,XtMotor* motor_y,XtVcMotor* motor_z,BaslerPylonCamera* camera,WordopLight* lighting,VisionModule* vision,XtVacuum* load_vacuum,XtVacuum* unload_vacuum);

private:
    LutParameter parameters;
    MaterialCarrier* carrier;
    BaslerPylonCamera* camera;
    WordopLight* lighting;
    VisionModule* vision;
    XtVacuum* grabber;
    XtVacuum* load_vacuum;
    XtVacuum* unload_vacuum;

public:
    bool MoveToPR(PRResultStruct& pr_result);
    bool MoveToLoadPos();
    bool MoveToPick1Lens();
    bool MoveToUnPick1Lens();
    bool MoveToPick2Lens();
    bool MoveToUnPick2Lens();

};

#endif // LUT_MODULE_H
