#ifndef XTADCMODULE_H
#define XTADCMODULE_H
#include <windows.h>
#include <qthread.h>
#include <qmutex.h>
#include "XT_MotionControler_Client_Lib.h"

class XtADCModule : public QThread
{
    Q_OBJECT

private:
    static int xt_data_channel_count;

public:
    XtADCModule(QObject* parent=nullptr);
    bool InitResource(int module_can_id, int adc_channel);
    double GetValue();
    void Stop();
    bool isRunning();
signals:
    void GotValue(double new_value);

protected:
    void run() override;

private:
    QMutex read_write_locker;

    bool running;
    int module_can_id;
    int adc_channel;
    int data_channel_id;
    double value;
};

#endif // XTADCMODULE_H
