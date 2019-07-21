#include "utils/LontryLight.h"

LontryLight::LontryLight()
{

}
LontryLight::~LontryLight()
{
    if(is_init)
        port.close();
}

bool LontryLight::Init(const QString &com_port)
{
    if(is_init)
        return true;
    port.setPortName(com_port);
    port.setBaudRate(QSerialPort::Baud9600);
    port.setParity(QSerialPort::NoParity);
    port.setDataBits(QSerialPort::Data8);
    port.setStopBits(QSerialPort::OneStop);
    port.setFlowControl(QSerialPort::NoFlowControl);
    QObject::connect(&port, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
    bool res = port.open(QIODevice::ReadWrite);
    if(res!=true)
        return false;
    port.clearError();
    port.clear();
    is_init = true;
    return true;
}

bool LontryLight::ReInit(const QString &com_port)
{
    if (is_init) {
        port.close();
        is_init = false;
    }
    return Init(com_port);
}


//example:40 06 01 01 1a 00 00 98 0d 0a is set ch0 bri to 0x98
bool LontryLight::SetBrightness(int ch, uint16_t brightness)
{
    if(is_init!=true)
        return false;
    uint16_t set_value = brightness +0x18;
    uint8_t buf[10];
    Pack *pack;
    pack = (Pack *)buf;
    pack->head = 0x40;
    pack->len = 6;
    pack->dev_code = DEV_CODE;
    pack->dev_id = device_id;
    pack->cmd = CMD_SET_BRT;
    pack->chn = ch;
    pack->data0 = brightness>>8;
    pack->data1 = brightness&0xff;
    buf[8]=0x0d;
    buf[9]=0x0a;
    bool res = cmd_mutex.tryLock(1000);
    if(!res)return false;
    port.write((const char*)buf,10);
    now_brightness[ch] = brightness;
    port.flush();
    cmd_mutex.unlock();
    return true;
}

int LontryLight::GetBrightness(int ch)
{
    if(ch>9||ch<0)
        return 0;
    return now_brightness[ch];
}

bool LontryLight::SetPWM(int ch, uint16_t pwm)
{
    //todo:
    return false;
}

bool LontryLight::SetTrigMode(int ch, uint16_t mode)
{
    //todo:
    return false;
}

bool LontryLight::OnOff(int ch, bool on_or_off)
{
    if(is_init!=true)
        return false;
    uint8_t buf[10];
    Pack *pack;
    pack = (Pack *)buf;
    pack->head = 0x40;
    pack->len = 6;
    pack->dev_code = DEV_CODE;
    pack->dev_id = device_id;
    pack->cmd = CMD_SET_ONOFF;
    pack->chn = ch;
    pack->data0 = on_or_off?1:0;
    pack->data1 = 0;
    buf[8]=0x0d;
    buf[9]=0x0a;
    bool res = cmd_mutex.tryLock(1000);
    if(!res)return false;
    port.write((const char*)buf,10);
    port.flush();
    cmd_mutex.unlock();
    return true;
}

bool LontryLight::GetConfig(int ch, uint16_t &brightness, uint16_t &trig_mode, uint16_t onoff)
{
    //todo:
    return false;
}

void LontryLight::readyReadSlot()
{
    //todo: now just read all then drop it
        recv_buffer.append(port.readAll());
        if(recv_buffer.capacity()>50)
        {
            recv_buffer.clear();
        }
}
