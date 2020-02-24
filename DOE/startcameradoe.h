#ifndef STARTCAMERADOE_H
#define STARTCAMERADOE_H

#include <QObject>
#include "SingleHead/singlehead_lsut_module.h"
#include "SingleHead/singleheadmachinematerialloadermodule.h"
#include "SingleHead/singleheadmachinematerialpickarm.h"
#include "ImageGrabber/dothinkey.h"
#include <stdexcept>
#include "materialtray.h"

struct StopLoopException
{

};

class StartCameraDOE : public QThread
{
    Q_OBJECT
public:
    explicit StartCameraDOE(QObject *parent = nullptr);

public:
    void init(SingleheadLSutModule* lsut, SingleHeadMachineMaterialLoaderModule* materialLoader,
              SingleHeadMachineMaterialPickArm* materialPickArm, MaterialTray* sensorTray,
              Dothinkey*dothinKey);
    Q_INVOKABLE void startDOE(int meterialCount, int doeTimes);
    Q_INVOKABLE void stopDOE();

signals:
    void passFailCountChanged(int nPass, int nFail);

public slots:

protected:
    void run() override;

private:
    void checkStop();

private:
    SingleheadLSutModule* lsut;
    SingleHeadMachineMaterialLoaderModule* materialLoader;
    SingleHeadMachineMaterialPickArm* materialPickArm;
    MaterialTray* sensorTray;
    Dothinkey* dothinKey;
    int materialCount;
    int doeTimes;
    int nPass;
    int nFail;
    volatile bool isRun;
    bool isStopped;
};

#endif // STARTCAMERADOE_H
