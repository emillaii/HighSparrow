#ifndef HIGHSPRROW_H
#define HIGHSPRROW_H
#include <QObject>
#include <errorcode.h>
#include <baslerpyloncamera.h>
#include <visionmodule.h>
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
public:
    Q_INVOKABLE void init();
    Q_INVOKABLE void home();
public slots:
    void homeAllMotors();
};

#endif // HIGHSPRROW_H
