#ifndef SEENOLIGHTSOURCECONTROLLER_H
#define SEENOLIGHTSOURCECONTROLLER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <exception>
#include "propertybase.h"

enum SLSCCommand{
    SetBrightness = 0x01,
    GetBrightness = 0x02
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

class SeenoLightSourceController: public PropertyBase
{
    Q_OBJECT

    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)

public:
    enum Channel{
        ColorTemperature = 0,
        WaveLength = 1
    };
    Q_ENUM(Channel)


    explicit SeenoLightSourceController();
    ~SeenoLightSourceController();

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();
    Q_INVOKABLE void setIntensity(int brightness);
    Q_INVOKABLE int getIntensity();

private:
    void writeCmd(SLSCCommandStruct& cmd);
    SLSCCommandStruct readRsp();
    void readData(char* data, int len);

public:
    QString portName() const
    {
        return m_portName;
    }

    int brightness() const
    {
        return m_brightness;
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

    void setBrightness(int brightness)
    {
        if (m_brightness == brightness)
            return;

        m_brightness = brightness;
        emit brightnessChanged(m_brightness);
        saveJsonConfig(configFileName, configSectionName);
    }

signals:
    void portNameChanged(QString portName);

    void brightnessChanged(int brightness);

private:
    const QString configFileName = "config//seenoLightSourceControllerParam.json";
    const QString configSectionName = "seenoLightSourceControllerParam";

    bool isOpen;
    QSerialPort device;
    QString m_portName;
    int m_brightness;
};

#endif // SEENOLIGHTSOURCECONTROLLER_H
