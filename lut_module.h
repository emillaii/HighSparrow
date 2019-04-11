#ifndef LUT_MODULE_H
#define LUT_MODULE_H

#include "material_carrier.h"
#include "wordoplight.h"
#include "baslerpyloncamera.h"
#include "lut_parameter.h"
#include "visionmodule.h"
#include "calibration.h"

#include <QObject>

class LutModule : public QObject
{
    Q_OBJECT
public:
    LutModule();
    void Init(MaterialCarrier* carrier,Calibration* updown_calibration, WordopLight* lighting,VisionModule* vision, XtVacuum* load_vacuum, XtVacuum* unload_vacuum);
    void loadParams();
    LutParameter parameters;
    Position3D load_position;
    Position3D aa1_updownlook_position;
    Position3D aa1_picklens_position;
    Position3D aa1_unpicklens_position;
    Position3D aa1_uplook_position;
    Position3D aa2_updownlook_position;
    Position3D aa2_picklens_position;
    Position3D aa2_unpicklens_position;
    Position3D aa2_uplook_position;
public slots:
    void updateParams();
private:
    MaterialCarrier* carrier;
    Calibration* updown_calibration;
    WordopLight* lighting;
    VisionModule* vision;
    XtVacuum* grabber;
    XtVacuum* load_vacuum;
    XtVacuum* unload_vacuum;

    PRResultStruct pr_result;
public:
    Q_INVOKABLE bool moveToAA1UpdownlookPos();
    Q_INVOKABLE bool moveToAA1UpDwonlookPR(PrOffset &offset,bool close_lighting = true);
    Q_INVOKABLE bool moveToAA2UpdownlookPos();
    Q_INVOKABLE bool moveToAA2UpDwonlookPR(PrOffset &offset,bool close_lighting = true);
    Q_INVOKABLE bool moveToLoadPos();
    Q_INVOKABLE bool moveToAA1PickLens();
    Q_INVOKABLE bool moveToAA1UnPickLens();
    Q_INVOKABLE bool moveToAA2PickLens();
    Q_INVOKABLE bool moveToAA2UnPickLens();

};

#endif // LUT_MODULE_H
