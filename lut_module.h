#ifndef LUT_MODULE_H
#define LUT_MODULE_H

#include "material_carrier.h"
#include "wordoplight.h"
#include "baslerpyloncamera.h"
#include "lut_parameter.h"
#include "visionmodule.h"

#include <QObject>

class LutModule : public QObject
{
    Q_OBJECT
public:
    LutModule(MaterialCarrier* carrier, BaslerPylonCamera* camera, WordopLight* lighting,VisionModule* vision, XtVacuum* load_vacuum, XtVacuum* unload_vacuum);
    LutParameter parameters;
    MaterialCarrier* carrier;
public slots:
    void updateParams();
private:
    BaslerPylonCamera* camera;
    WordopLight* lighting;
    VisionModule* vision;
    XtVacuum* grabber;
    XtVacuum* load_vacuum;
    XtVacuum* unload_vacuum;

    PRResultStruct pr_result;
    void loadParams();
public:
    Q_INVOKABLE bool moveToPR();
    Q_INVOKABLE bool moveToLoadPos();
    Q_INVOKABLE bool moveToPick1Lens();
    Q_INVOKABLE bool moveToUnPick1Lens();
    Q_INVOKABLE bool moveToPick2Lens();
    Q_INVOKABLE bool moveToUnPick2Lens();

};

#endif // LUT_MODULE_H
