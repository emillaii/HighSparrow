#ifndef LONTRYLIGHT_H
#define LONTRYLIGHT_H

#include <QSerialPort>
#include <QString>
#include <QObject>
#include <qmutex.h>



class LontryLight: public QObject
{
    Q_OBJECT

    struct Pack
    {
        Pack() {}
        uint8_t head;
        uint8_t len;
        uint8_t dev_code;
        uint8_t dev_id;
        uint8_t cmd;
        uint8_t chn;
        uint8_t data0;
        uint8_t data1;
    };
public:
    LontryLight();
    ~LontryLight();

    bool Init(const QString &com_port);
    bool ReInit(const QString &com_port);
    bool SetBrightness(int ch, uint16_t brightness);
    int GetBrightness(int ch);
    bool SetPWM(int ch,uint16_t pwm);
    bool SetTrigMode(int ch, uint16_t mode);
    bool OnOff(int ch, bool on_or_off);
    bool GetConfig(int ch, uint16_t &brightness, uint16_t &trig_mode, uint16_t onoff);

private:
    const static uint8_t CMD_SET_ID = 0x09;
    const static uint8_t CMD_SET_PWM = 0x11;
    const static uint8_t CMD_SET_BRT = 0x1a;
    const static uint8_t CMD_SET_TRIG = 0x22;
    const static uint8_t CMD_SET_ONOFF = 0x2a;
    const static uint8_t CMD_READ = 0x31;
    const static uint8_t RE_ONE_CH = 0x5b;
    const static uint8_t RE_ALL_CH = 0x64;
    const static uint8_t RE_OK = 0x00;
    const static uint8_t RE_NG = 0x01;

    const static uint8_t DEV_CODE = 0x01;


    uint8_t device_id = 0x01;

    //QSerialPortInfo port_config;
    QSerialPort port;

    bool is_init = false;

    int now_brightness[10]={0};

    QByteArray recv_buffer;
    QByteArray send_buffer;


    QMutex cmd_mutex;

    private slots: void readyReadSlot();
};

#endif // LONTRYLIGHT_H
