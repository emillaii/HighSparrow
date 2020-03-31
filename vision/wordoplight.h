#ifndef WORDOPLIGHT_H
#define WORDOPLIGHT_H
#include <QSerialPort>
#include <QString>
#include <QObject>
#include <qmutex.h>
#include "config.h"
#include "thread_worker_base.h"

typedef enum {
    LIGHTING_UPLOOK,
    LIGHTING_DOWNLOOK,
    LIGHTING_PICKARM,
} LIGHTING_CHANNEL;

class WordopLight: public QObject, public ThreadWorkerBase
{
    Q_PROPERTY(int downlookLighting READ downlookLighting WRITE setDownlookLighting NOTIFY paramsChanged)
    Q_PROPERTY(int downlookCoaxialLighting READ downlookCoaxialLighting WRITE setDownlookCoaxialLighting NOTIFY paramsChanged)
    Q_PROPERTY(int uplookLighting READ uplookLighting WRITE setUplookLighting NOTIFY paramsChanged)
    Q_PROPERTY(int pickarmLighting READ pickarmLighting WRITE setPickarmLighting NOTIFY paramsChanged)
    Q_PROPERTY(int aa2DownlookLighting READ aa2DownlookLighting WRITE setAA2DownlookLighting NOTIFY paramsChanged)
    Q_PROPERTY(int aa2DownlookCoaxialLighting READ aa2DownlookCoaxialLighting WRITE setAA2DownlookCoaxialLighting NOTIFY paramsChanged)
    Q_PROPERTY(int sensorPickarmLighting READ sensorPickarmLighting WRITE setSensorPickarmLighting NOTIFY paramsChanged)
    Q_PROPERTY(int sensorUplookLighting READ sensorUplookLighting WRITE setSensorUplookLighting NOTIFY sensorUplookLightingChanged)
    Q_PROPERTY(int barcodeCameraLighting READ barcodeCameraLighting WRITE setBarcodeCameraLighting NOTIFY barcodeCameraLightingChanged)

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
    WordopLight(int mode, QString name);
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

    void sendMessageToNextModule(int channel, double value){
        QJsonObject params;
        params.insert("channel", channel);
        params.insert("value", value);
        if (mode == 0)
            emit sendMessageToModule(LIGHTING_CONTROLLER_2, "ChangeLighting", params);
        else {
            emit sendMessageToModule(LIGHTING_CONTROLLER_1, "ChangeLighting", params);
        }
    }

    int downlookLighting() const
    {
        return m_downlookLighting;
    }

    int downlookCoaxialLighting() const
    {
        return m_downlookCoaxialLighting;
    }

    int uplookLighting() const
    {
        return m_uplookLighting;
    }

    int pickarmLighting() const
    {
        return m_pickarmLighting;
    }

    int aa2DownlookLighting() const
    {
        return m_aa2DownlookLighting;
    }

    int aa2DownlookCoaxialLighting() const
    {
        return m_aa2DownlookCoaxialLighting;
    }

    int sensorPickarmLighting() const
    {
        return m_sensorPickarmLighting;
    }

signals:
    void ChangeBrightnessSignal(int ch, uint8_t brightness);
    void ChangeDoneSignal(bool result);

    void paramsChanged(int downlookLighting);

    void sensorUplookLightingChanged(int sensorUplookLighting);

    void barcodeCameraLightingChanged(int barcodeCameraLighting);

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
    if (mode == 0){
        setBrightness(LIGHTING_AA1_DL, (uint8_t)downlookLighting);
    }
    else {
        sendMessageToNextModule(LIGHTING_AA1_DL, (uint8_t)downlookLighting);
    }
}

void setDownlookCoaxialLighting(int downlookCoaxialLighting)
{
    qInfo("Set downlook Coaxial lighting1 %d", downlookCoaxialLighting);
    if (m_downlookCoaxialLighting == downlookCoaxialLighting)
        return;

    m_downlookCoaxialLighting = downlookCoaxialLighting;
    emit paramsChanged(m_downlookCoaxialLighting);
    if (mode == 0){
        setBrightness(LIGHTING_AA1_DL_COAXIAL, (uint8_t)downlookCoaxialLighting);
    }
    else {
        sendMessageToNextModule(LIGHTING_AA1_DL_COAXIAL, (uint8_t)downlookCoaxialLighting);
    }
}

void setUplookLighting(int uplookLighting)
{
    qInfo("Set uplook lighting %d", uplookLighting);
    if (m_uplookLighting == uplookLighting)
        return;

    m_uplookLighting = uplookLighting;
    emit paramsChanged(m_uplookLighting);
    if (mode==0){
        setBrightness(LIGHTING_LUT_UL, (uint8_t)uplookLighting);
    } else {
        sendMessageToNextModule(LIGHTING_LUT_UL, (uint8_t)uplookLighting);
    }
}

void setPickarmLighting(int pickarmLighting)
{
    qInfo("Set pickarm lighting %d", pickarmLighting);
    if (m_pickarmLighting == pickarmLighting)
        return;

    m_pickarmLighting = pickarmLighting;
    emit paramsChanged(m_pickarmLighting);
    if (mode == 0){
        setBrightness(LIGHTING_LPA_DL, (uint8_t)pickarmLighting);
    }
    else {
        sendMessageToNextModule(LIGHTING_LPA_DL, (uint8_t)pickarmLighting);
    }
}

void setAA2DownlookLighting(int aa2DownlookLighting)
{
    qInfo("Set AA2 Downlook lighting %d", aa2DownlookLighting);
    if (m_aa2DownlookLighting == aa2DownlookLighting)
        return;

    m_aa2DownlookLighting = aa2DownlookLighting;
    emit paramsChanged(m_aa2DownlookLighting);
    if (mode == 0){
        sendMessageToNextModule(LIGHTING_AA2_DL, (uint8_t)aa2DownlookLighting);
    }
    else {
        setBrightness(LIGHTING_AA2_DL, (uint8_t)aa2DownlookLighting);
    }
}

void setAA2DownlookCoaxialLighting(int aa2DownlookCoaxialLighting)
{
    qInfo("Set AA2 Downlook Coaxial lighting %d", aa2DownlookCoaxialLighting);
    if (m_aa2DownlookCoaxialLighting == aa2DownlookCoaxialLighting)
        return;

    m_aa2DownlookCoaxialLighting = aa2DownlookCoaxialLighting;
    emit paramsChanged(m_aa2DownlookCoaxialLighting);
    if (mode == 0){
        sendMessageToNextModule(LIGHTING_AA2_DL_COAXIAL, (uint8_t)aa2DownlookCoaxialLighting);
    }
    else {
        setBrightness(LIGHTING_AA2_DL_COAXIAL, (uint8_t)aa2DownlookCoaxialLighting);
    }
}

void setSensorPickarmLighting(int sensorPickarmLighting)
{
    if (m_sensorPickarmLighting == sensorPickarmLighting)
        return;

    m_sensorPickarmLighting = sensorPickarmLighting;
    emit paramsChanged(m_sensorPickarmLighting);
    if (mode == 0){
        sendMessageToNextModule(LIGHTING_SPA_DL, (uint8_t)sensorPickarmLighting);
    }
    else {
        setBrightness(LIGHTING_SPA_DL, (uint8_t)sensorPickarmLighting);
    }
}

void setSensorUplookLighting(int sensorUplookLighting)
{
    qInfo("Set sensor uplook lighting %d", sensorUplookLighting);
    if (m_sensorUplookLighting == sensorUplookLighting)
        return;

    m_sensorUplookLighting = sensorUplookLighting;
    emit sensorUplookLightingChanged(m_sensorUplookLighting);
    if (mode == 0){
        setBrightness(LIGHTING_SENSOR_UL, (uint8_t)sensorUplookLighting);
    }
    else {
        sendMessageToNextModule(LIGHTING_SENSOR_UL, (uint8_t)sensorUplookLighting);
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
    int m_downlookCoaxialLighting = 0;

    int m_uplookLighting = 0;

    int m_pickarmLighting = 0;

    int m_aa2DownlookLighting = 0;
    int m_aa2DownlookCoaxialLighting = 0;

    int m_sensorPickarmLighting = 0;

    int m_sensorUplookLighting = 0;

    int m_barcodeCameraLighting = 0;

private slots:
    void readyReadSlot();

//ThreadWorkerBase Interface
public slots:
    void startWork(int run_mode) override;
    void stopWork(bool wait_finish) override;
    void resetLogic() override;
    void performHandlingOperation(int cmd, QVariant param) override;

    void setBarcodeCameraLighting(int barcodeCameraLighting)
    {
        if (m_barcodeCameraLighting == barcodeCameraLighting)
            return;

        m_barcodeCameraLighting = barcodeCameraLighting;
        emit barcodeCameraLightingChanged(m_barcodeCameraLighting);
    }

public:
    PropertyBase *getModuleState() override;
    void receivceModuleMessage(QVariantMap module_message) override;
    QMap<QString, PropertyBase *> getModuleParameter() override;
    void setModuleParameter(QMap<QString, PropertyBase *>) override;
    int sensorUplookLighting() const
    {
        return m_sensorUplookLighting;
    }
    int barcodeCameraLighting() const
    {
        return m_barcodeCameraLighting;
    }
};

#endif // WORDOPLIGHT_H
