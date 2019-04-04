#ifndef HIGHSPRROW_H
#define HIGHSPRROW_H
#include <QObject>
#include <errorcode.h>
#include <baslerpyloncamera.h>
#include <visionmodule.h>
#include <aaheadmodule.h>
#include <aacore.h>
#include <wordoplight.h>
#include "STD.h"

class HighSprrow : public QObject
{
    Q_OBJECT
public:
    HighSprrow();
    ~HighSprrow();
private:
    ErrorCodeStruct InitComponent();
public:
    BaslerPylonCamera * pylonDownlookCamera = Q_NULLPTR;
    BaslerPylonCamera * pylonUplookCamera = Q_NULLPTR;
    VisionModule * visionModule = Q_NULLPTR;
    SfrWorkerController *sfrWorkerController = Q_NULLPTR;
    AAHeadModule * aaHeadModule = Q_NULLPTR;
    AACore * aaCore = Q_NULLPTR;
    WordopLight * light_controller = Q_NULLPTR;

public:
    Q_INVOKABLE void init();
    Q_INVOKABLE void home();
public slots:
    void homeAllMotors();
};

#endif // HIGHSPRROW_H
