﻿#ifndef WORDOPLIGHT_H
#define WORDOPLIGHT_H
#include <QSerialPort>
#include <QString>
#include <QObject>
#include <qmutex.h>
#include "config.h"
typedef enum {
    LIGHTING_UPLOOK,
    LIGHTING_DOWNLOOK,
    LIGHTING_PICKARM,
} LIGHTING_CHANNEL;

class WordopLight: public QObject
{
    Q_PROPERTY(int downlookLighting READ downlookLighting WRITE setDownlookLighting NOTIFY paramsChanged)
    Q_PROPERTY(int uplookLighting READ uplookLighting WRITE setUplookLighting NOTIFY paramsChanged)
    Q_PROPERTY(int pickarmLighting READ pickarmLighting WRITE setPickarmLighting NOTIFY paramsChanged)
    Q_OBJECT

    struct LongCommand
    {
        LongCommand() {}
        uint8_t cmd;
        uint8_t chn;
        uint8_t data;
    };
    struct ShortCommand
    {
        ShortCommand() {}
        uint8_t cmd;
        uint8_t data;
    };
    struct Pack
    {
        Pack() {}
        uint8_t head;
        uint8_t len;
        uint8_t dev_code;
        uint8_t dev_id;
        uint8_t data[1];
    };

public:
    WordopLight(int mode);
    ~WordopLight();

    bool Init(const QString &com_port);
    bool ReInit(const QString &com_port);
    bool setBrightness(int ch, uint8_t brightness);
    int GetBrightness(int ch);
    bool SetPWM(int ch,uint8_t pwm);
    bool SetTrigMode(int ch, uint8_t mode);
    bool OnOff(int ch, bool on_or_off);
    bool GetConfig(int ch, uint8_t &brightness, uint8_t &trig_mode, uint8_t onoff);

    int getUplookLensHolderPRLighting();
    int getDownlookCmosSensorPRLighting();
    int getUpdownlookCalibGlassPRLighting();
    int getPickarmLensHolderPRLighting();
    int getPickarmCmosSensorPRLighting();
    int getPickarmProductPRLighting();

    void setUplookLensHolderPRLighting(int val);
    void setDownlookCmosSensorPRLighting(int val);
    void setUpdownlookCalibGlassPRLighting(int val);
    void setPickarmLensHolderPRLighting(int val);
    void setPickarmCmosSensorPRLighting(int val);
    void setPickarmProductPRLighting(int value);

    int downlookLighting() const
    {
        return m_downlookLighting;
    }

    int uplookLighting() const
    {
        return m_uplookLighting;
    }

    int pickarmLighting() const
    {
        return m_pickarmLighting;
    }

signals:
    void ChangeBrightnessSignal(int ch, uint8_t brightness);
    void ChangeDoneSignal(bool result);

    void paramsChanged(int downlookLighting);

public slots:
void ChangeBrightness(int ch, uint8_t brightness);
void ChangeDone(bool result);

void setDownlookLighting(int downlookLighting)
{
    qInfo("Set downlook lighting %d", downlookLighting);
    if (m_downlookLighting == downlookLighting)
        return;

    m_downlookLighting = downlookLighting;
    emit paramsChanged(m_downlookLighting);
    if (mode == 0)
        setBrightness(LIGHTING_AA1_DL, (uint8_t)downlookLighting);
    else {
        setBrightness(LIGHTING_AA2_DL, (uint8_t)downlookLighting);
    }
}

void setUplookLighting(int uplookLighting)
{
    qInfo("Set uplook lighting %d", uplookLighting);
    if (m_uplookLighting == uplookLighting)
        return;

    m_uplookLighting = uplookLighting;
    emit paramsChanged(m_uplookLighting);
    setBrightness(LIGHTING_LUT_UL, (uint8_t)uplookLighting);
}

void setPickarmLighting(int pickarmLighting)
{
    qInfo("Set pickarm lighting %d", pickarmLighting);
    if (m_pickarmLighting == pickarmLighting)
        return;

    m_pickarmLighting = pickarmLighting;
    emit paramsChanged(m_pickarmLighting);
    if (mode == 0)
        setBrightness(LIGHTING_LPA_DL, (uint8_t)pickarmLighting);
    else {
        setBrightness(LIGHTING_SPA_DL, (uint8_t)pickarmLighting);
    }
}

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

    int mode;
    int uplookLensHolderPRLighting = 0;
    int downlookCmosSensorPRLighting = 0;
    int updownlookCalibGlassPRLighting = 0;
    int pickarmLensHolderPRLighting = 0;
    int pickarmCmosSensorPRLighting = 0;
    int pickarmProductPRLighting = 0;

    uint8_t device_id = 0x00;

    //QSerialPortInfo port_config;
    QSerialPort port;
    QString port_name;

    bool is_init = false;
    Qt::HANDLE creator_tid;

    QByteArray recv_buffer;
    QByteArray send_buffer;

    uint8_t CalcChecksum(QByteArray data);
    uint8_t CalcChecksum(uint8_t data[], int len);

    QMutex cmd_mutex;

    bool change_done = false;
    bool change_result = false;

    uint8_t now_brightness[10]={0};

    int m_downlookLighting = 0;

    int m_uplookLighting = 0;

    int m_pickarmLighting = 0;

private slots:
    void readyReadSlot();
};

#endif // WORDOPLIGHT_H