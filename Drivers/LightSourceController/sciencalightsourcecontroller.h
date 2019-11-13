#ifndef SCIENCALIGHTSOURCECONTROLLER_H
#define SCIENCALIGHTSOURCECONTROLLER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <exception>
#include "propertybase.h"

enum SLSCCommand{
    SelectChannel = 0x01,
    SetBrightness = 0x02,
    GetBrightness = 0x03
};

struct SLSCCommandStruct
{
private:
    quint8 head0 = 0x55;
    quint8 head1 = 0xaa;

public:
    quint8 cmd;
    quint8 data1;
    quint8 data2;
    quint8 data3;
    quint8 data4;

private:
    quint8 checkCode;

public:
    void calculateCheckCode()
    {
        checkCode = ~(head0 + head1 + cmd + data1 + data2 + data3 + data4);
    }

    QString toString()
    {
        QString s("cmd: %1, data: %2 %3 %4 %5");
        return s.arg(cmd, 0, 16).arg(data1, 0, 16).arg(data2, 0, 16).arg(data3, 0, 16).arg(data4, 0, 16);
    }
};

class SciencaLightSourceController: public PropertyBase
{
    Q_OBJECT

    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(int colorTemperatureIntensity READ colorTemperatureIntensity WRITE setColorTemperatureIntensity NOTIFY colorTemperatureIntensityChanged)
    Q_PROPERTY(int waveLengthIntensity READ waveLengthIntensity WRITE setWaveLengthIntensity NOTIFY waveLengthIntensityChanged)

public:
    enum Channel{
        ColorTemperature = 0,
        WaveLength = 1
    };
    Q_ENUM(Channel)


    explicit SciencaLightSourceController();
    ~SciencaLightSourceController();

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();
    Q_INVOKABLE void setIntensity(Channel channel);

    Q_INVOKABLE int getIntensity(Channel channel);

private:
    void selectChannel(int channel);
    void writeCmd(SLSCCommandStruct& cmd);
    SLSCCommandStruct readRsp();
    void readData(char* data, int len);

public:
    QString portName() const
    {
        return m_portName;
    }

    int colorTemperatureIntensity() const
    {
        return m_colorTemperatureIntensity;
    }

    int waveLengthIntensity() const
    {
        return m_waveLengthIntensity;
    }

public slots:
    void setPortName(QString portName)
    {
        if (m_portName == portName)
            return;

        m_portName = portName;
        emit portNameChanged(m_portName);
        saveJsonConfig(configFileName, configSectionName);
    }

    void setColorTemperatureIntensity(int colorTemperatureIntensity)
    {
        if (m_colorTemperatureIntensity == colorTemperatureIntensity)
            return;

        m_colorTemperatureIntensity = colorTemperatureIntensity;
        emit colorTemperatureIntensityChanged(m_colorTemperatureIntensity);
        saveJsonConfig(configFileName, configSectionName);
    }

    void setWaveLengthIntensity(int waveLengthIntensity)
    {
        if (m_waveLengthIntensity == waveLengthIntensity)
            return;

        m_waveLengthIntensity = waveLengthIntensity;
        emit waveLengthIntensityChanged(m_waveLengthIntensity);
        saveJsonConfig(configFileName, configSectionName);
    }

signals:
    void portNameChanged(QString portName);

    void colorTemperatureIntensityChanged(int colorTemperatureIntensity);

    void waveLengthIntensityChanged(int waveLengthIntensity);

private:
    const QString configFileName = "sciencaLightSourceControllerParam.json";
    const QString configSectionName = "sciencaLightSourceControllerParam";

    bool isOpen;
    int currenChannel;
    QSerialPort device;
    QString m_portName;
    int m_colorTemperatureIntensity;
    int m_waveLengthIntensity;
};

#endif // SCIENCALIGHTSOURCECONTROLLER_H
