#ifndef SUT_MODULE_H
#define SUT_MODULE_H

#include "baslerpyloncamera.h"
#include "calibration.h"
#include "material_carrier.h"
#include "sut_parameter.h"
#include "vision_location.h"
#include "visionmodule.h"
#include "wordoplight.h"

class SutModule : public QObject
{
    Q_OBJECT
public:
    SutModule();
    void Init(MaterialCarrier* carrier,VisionLocation* vision_downlook_location,VisionLocation* updownlook_location, XtVacuum *vacuum);
    void loadParams();
    SutParameter parameters;
    Position3D load_position;
    Position3D downlook_position;
    Position3D tool_downlook_position;
    Position3D mushroom_positon;
    Position3D tool_uplook_positon;
public slots:
    void updateParams();
private:
    MaterialCarrier* carrier;
    VisionLocation* vision_downlook_location;
    VisionLocation* vision_updownlook_location;
    XtVacuum* vacuum;

    PRResultStruct pr_result;
public:
    Q_INVOKABLE bool moveToLoadPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToDownlookPR(PrOffset &offset,bool close_lighting = true,bool check_autochthonous = false);
    Q_INVOKABLE bool moveToDownlookPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToUpDwonlookPR(PrOffset &offset,bool close_lighting = true,bool check_autochthonous = false);
    Q_INVOKABLE bool moveToToolDownlookPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToToolUplookPos(bool check_autochthonous = false);
    Q_INVOKABLE bool moveToMushroomPos(bool check_autochthonous = false);
    bool stepMove_XY_Sync(double x,double y);
};

#endif // SUT_MODULE_H
