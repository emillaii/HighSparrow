#include "Vision/wordoplight.h"
#include <QMutexLocker>
#include <QThread>

WordopLight::WordopLight()
{
    port_name = "";
    // Todo Need to place the com port to config file.
    Init("com1");
    OnOff(0, true);
    OnOff(1, true);
    OnOff(2, true);
    OnOff(3, true);
    connect(this,&WordopLight::ChangeBrightnessSignal,this,&WordopLight::ChangeBrightness,Qt::QueuedConnection);
    connect(this,&WordopLight::ChangeDoneSignal,this,&WordopLight::ChangeDone,Qt::QueuedConnection);
}

WordopLight::~WordopLight()
{
    if(is_init)
      port.close();
}

bool WordopLight::Init(const QString &com_port)
{
    if(is_init)
        return true;
    creator_tid = QThread::currentThreadId();
    port_name = com_port;
    port.setPortName(com_port);
    port.setBaudRate(QSerialPort::Baud19200);
    port.setParity(QSerialPort::NoParity);
    port.setDataBits(QSerialPort::Data8);
    port.setStopBits(QSerialPort::OneStop);
    port.setFlowControl(QSerialPort::NoFlowControl);
    QObject::connect(&port, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));

    bool res = port.open(QIODevice::ReadWrite);
    if(!res){
        qCritical() << "Open port failed!" << com_port << "error message:" << port.error();
        return false;
    }
    port.clearError();
    port.clear();
    is_init = true;
    return true;
}

bool WordopLight::setBrightness(int ch, uint8_t brightness)
{
    if(QThread::currentThreadId()==creator_tid)
    {
        change_result = false;
        ChangeBrightness(ch,brightness);
        return change_result;
    }
    QMutexLocker locker(&cmd_mutex);
    change_result = false;
    change_done = false;
    emit ChangeBrightnessSignal(ch,brightness);
    //qInfo("%d ChangeBrightnessSignal", QThread::currentThreadId());
    QThread::msleep(100);
    return true;
    int timeout = 1000;
    while(timeout>0)
    {
        if(change_done)
            return change_result;
        QThread::msleep(10);
        timeout-=10;
    }
    return false;
}

bool WordopLight::OnOff(int ch, bool on_or_off)
{
    if(is_init!=true)
        return false;
    uint8_t buf[8];
    Pack *pack;
    LongCommand *cmd;
    pack = (Pack *)buf;
    pack->head = 0x40;
    pack->len = 5;
    pack->dev_code = DEV_CODE;
    pack->dev_id = device_id;
    cmd = (LongCommand *)pack->data;
    cmd->cmd = CMD_SET_ONOFF;
    cmd->chn = ch;
    cmd->data = on_or_off?1:0;
    buf[7] = CalcChecksum(buf,7);
    cmd_mutex.lock();
    port.write((const char*)buf,8);
    port.flush();
    cmd_mutex.unlock();
    return true;
}

uint8_t WordopLight::CalcChecksum(uint8_t data[], int len)
{
    uint8_t ret= 0;
    for(int i =0;i<len;i++)
    {
        ret+=data[i];
    }
    return ret;
}

void WordopLight::readyReadSlot()
{
//todo: now just read all then drop it
    recv_buffer.append(port.readAll());
    if(recv_buffer.capacity()>50)
    {
        recv_buffer.clear();
    }
}

void WordopLight::ChangeBrightness(int ch, uint8_t brightness)
{
    if(!is_init){
        qCritical() <<"WordopLight is not inited!";
        return;
    }
    uint8_t buf[8];
    Pack *pack;
    LongCommand *cmd;
    pack = (Pack *)buf;
    pack->head = 0x40;
    pack->len = 5;
    pack->dev_code = DEV_CODE;
    pack->dev_id = device_id;
    cmd = (LongCommand *)pack->data;
    cmd->cmd = CMD_SET_BRT;
    cmd->chn = ch;
    cmd->data = brightness;
    buf[7] = CalcChecksum(buf,7);
    int time = 2;
    do{
        int res = port.write((const char*)buf,8);
        if(-1 != res)
        {
            bool bres = port.waitForBytesWritten(300);
            if(bres)
               {
                now_brightness[ch] = brightness;
                change_result = true;
                return;
                if(QThread::currentThreadId()==creator_tid)
                {
                    change_result = true;
                    return;
                }
                qInfo("ChangeDoneSignal");
                emit ChangeDoneSignal(true);
                return;
            }
            else {
                qInfo("waitForBytesWritten(); failed");
            }
        }
        else {
            qInfo("port.write(); failed");
        }
        time--;
        port.clearError();
        port.clear();
    }
    while(time>0);
//    port.flush();
    change_result = false;
    return;
    if(QThread::currentThreadId()==creator_tid)
    {
        change_result = false;
        return;
    }
    qInfo("ChangeDoneSignal");
    emit ChangeDoneSignal(false);
    return;
}

void WordopLight::ChangeDone(bool result)
{
    qInfo("ChangeDone");
    change_result = result;
    change_done = true;
}

