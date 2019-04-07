#ifndef SUT_MODULE_H
#define SUT_MODULE_H

#include "baslerpyloncamera.h"
#include "material_carrier.h"
#include "sut_parameter.h"
#include "visionmodule.h"
#include "wordoplight.h"



class SutModule
{
public:
    SutModule(QString name, XtMotor* motor_x,XtMotor* motor_y,XtVcMotor* motor_z,XtVacuum* vacuum,BaslerPylonCamera* camera,WordopLight* lighting,VisionModule* vision);
private:
    SutParameter parameters;
    MaterialCarrier* carrier;
    BaslerPylonCamera* camera;
    WordopLight* lighting;
    VisionModule* vision;
public:
    bool MoveToPR(bool use_offset = false);
    bool MoveToLoadPos();
    bool MoveToOCPos();
    bool Move_XY_Sync(double x,double y);
};

#endif // SUT_MODULE_H
