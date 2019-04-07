#ifndef AAHEADMODULE_H
#define AAHEADMODULE_H

#include "XtVacuum.h"
#include "xtmotor.h"

#include <QObject>
#include <propertybase.h>
#include <visionmodule.h>
#include <aaheadparameters.h>

class AAHeadModule : public QObject
{
    Q_OBJECT
public:
    AAHeadModule();

public slots:


private:
    XtMotor* x = Q_NULLPTR;
    XtMotor* y = Q_NULLPTR;
    XtMotor* z = Q_NULLPTR;
    XtMotor* a = Q_NULLPTR;
    XtMotor* b = Q_NULLPTR;
    XtMotor* c = Q_NULLPTR;
    XtVacuum * v = Q_NULLPTR;
    VisionModule * visionModule;
public:
    AAHeadParameters aaModuleParams;

};

#endif // AAHEADMODULE_H
