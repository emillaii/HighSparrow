#include "xtadcmodule.h"

using namespace XT_Controler;


int XtADCModule::xt_data_channel_count = 0;

XtADCModule::XtADCModule(QObject *parent):QThread(parent)
{
    running = false;
    module_can_id = -1;
    adc_channel = -1;
    xt_data_channel_count++;
    data_channel_id = xt_data_channel_count;
    value = 0;
}

bool XtADCModule::InitResource(int module_can_id, int adc_channel)
{
    int res = XT_Controler::beCurConnectServerAndInterfaceBoard();
    if(res!=1)
        return false;
    this->module_can_id = module_can_id;
    this->adc_channel = adc_channel;
    SET_INPUT_DATA_MAP(0, module_can_id,  adc_channel+20, datatype_float32, data_channel_id);
    RESET_INPUT_DATA_FIFO(0, data_channel_id, 50);
    WaitForAllInsFinish(0);
    running = true;
    start();
    return true;
}

double XtADCModule::GetValue()
{
    if(!running)
        return 0;
    double temp;
    read_write_locker.tryLock(0);
    temp = value;
    read_write_locker.unlock();
    return temp;
}

void XtADCModule::Stop()
{
    running = false;
}

void XtADCModule::run()
{
    int res = 0;
    int cnt = 0;
    double result[50];
    uint64_t result_order[50];
    while(running)
    {
        QThread::msleep(10);
        res = GetInputChannelData(data_channel_id, 50, cnt, result, result_order);
        if(res!=0&&cnt>0)
        {
            read_write_locker.lock();
            value = result[cnt-1];
            read_write_locker.unlock();
            double new_value = value;
            emit GotValue(new_value);
        }
    }
}
