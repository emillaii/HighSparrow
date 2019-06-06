#ifndef SINGLEHEAD_LSUT_MODULE_H
#define SINGLEHEAD_LSUT_MODULE_H

#include "XtCylinder.h"
#include "material_carrier.h"
#include "singleheadlsutparameter.h"
#include "thread_worker_base.h"
#include "vision_location.h"



class SingleheadLSutModule : public ThreadWorkerBase
{
    Q_OBJECT
public:
    SingleheadLSutModule();
    void Init();
    MaterialCarrier* lut_carrier;
    MaterialCarrier* sut_carrier;
    SingleHeadLSutParameter parameters;
public slots:
    void startWork(int run_mode);
    void stopWork(bool wait_finish);
    void resetLogic();
    void performHandlingOperation(int cmd);
private:
    XtCylinder *pogopin;
    VisionLocation* vision_downlook_location;

};

#endif // SINGLEHEAD_LSUT_MODULE_H
