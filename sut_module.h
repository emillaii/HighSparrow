#ifndef SUT_MODULE_H
#define SUT_MODULE_H

#include "baslerpyloncamera.h"
#include "material_carrier.h"
#include "sut_parameter.h"
#include "visionmodule.h"
#include "wordoplight.h"

class SutModule : public QObject
{
    Q_OBJECT
public:
    SutModule(MaterialCarrier* carrier,BaslerPylonCamera* camera,WordopLight* lighting,VisionModule* vision);
    SutParameter parameters;
    MaterialCarrier* carrier;
public slots:
    void updateParams();
private:
    BaslerPylonCamera* camera;
    WordopLight* lighting;
    VisionModule* vision;

    PRResultStruct pr_result;
    void loadParams();
public:
    Q_INVOKABLE bool moveToPR();
    Q_INVOKABLE bool moveToLoadPos();
    Q_INVOKABLE bool moveToOCPos();
    bool stepMove_XY_Sync(double x,double y);
};

#endif // SUT_MODULE_H
