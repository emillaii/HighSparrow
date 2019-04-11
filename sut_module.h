#ifndef SUT_MODULE_H
#define SUT_MODULE_H

#include "baslerpyloncamera.h"
#include "calibration.h"
#include "material_carrier.h"
#include "sut_parameter.h"
#include "visionmodule.h"
#include "wordoplight.h"

class SutModule : public QObject
{
    Q_OBJECT
public:
    SutModule();
    void Init(MaterialCarrier* carrier,Calibration* down_calibration,Calibration* updown_calibration,WordopLight* lighting,VisionModule* vision);
    void loadParams();
    SutParameter parameters;
    Position3D load_position;
    Position3D downlook_position;
    Position3D toollook_position;
    Position3D mushroom_positon;
public slots:
    void updateParams();
private:
    MaterialCarrier* carrier;
    Calibration* down_calibration;
    Calibration* updown_calibration;
    WordopLight* lighting;
    VisionModule* vision;

    PRResultStruct pr_result;
public:
    Q_INVOKABLE bool moveToDownlookPR(PrOffset &offset,bool close_lighting = true);
    Q_INVOKABLE bool moveToLoadPos();
    Q_INVOKABLE bool moveToDownlookPos();
    Q_INVOKABLE bool moveToUpDwonlookPR(PrOffset &offset,bool close_lighting = true);
    Q_INVOKABLE bool moveToToollookPos();
    Q_INVOKABLE bool moveToMushroomPos();
    bool stepMove_XY_Sync(double x,double y);
};

#endif // SUT_MODULE_H
