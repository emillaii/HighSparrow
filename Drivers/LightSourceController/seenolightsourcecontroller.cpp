#include "seenolightsourcecontroller.h"


SeenoLightSourceController::SeenoLightSourceController():isOpen(false)
{
    loadJsonConfig(configFileName, configSectionName);
}

SeenoLightSourceController::~SeenoLightSourceController()
{
    close();
}

void SeenoLightSourceController::open()
{
    if(isOpen)
    {
        qInfo("Has been opened!");
        setIntensity(brightness());
        return;
    }
    device.setPortName(m_portName);
    device.setBaudRate(9600);
    device.setStopBits(QSerialPort::StopBits::OneStop);
    device.setParity(QSerialPort::Parity::NoParity);
    if(device.open(QSerialPort::OpenModeFlag::ReadWrite))
    {
        qInfo("Open port successful!");
        isOpen = true;
        setIntensity(brightness());
    }
    else {
        qCritical("Open port failed!");
    }
}

void SeenoLightSourceController::close()
{
    if(isOpen)
    {
        setIntensity(0);
        device.close();
        isOpen = false;
    }
}

void SeenoLightSourceController::setIntensity(int brightness)
{
    if(!isOpen){qCritical() << tr("Did not init!"); return;}

    try {
        qInfo("brightness: %d", brightness);
        SLSCCommandStruct cmd;
        cmd.cmd = SetBrightness;
        ushort value = brightness;
        cmd.data1 = value & 0xff;
        cmd.data2 = (value & 0xff00) >> 8;
        cmd.calculateCheckCode();
        writeCmd(cmd);
    } catch (std::exception& e) {
        qCritical() << e.what();
    }
}

int SeenoLightSourceController::getIntensity()
{
    if(!isOpen){qCritical() << tr("Did not init!"); return -1;}

    try {
        SLSCCommandStruct cmd;
        cmd.cmd = GetBrightness;
        cmd.calculateCheckCode();
        writeCmd(cmd);
        SLSCCommandStruct rsp = readRsp();
        readRsp();
        int high8 = rsp.data2;
        return rsp.data1 + (high8 << 8);
    } catch (std::exception& e) {
        qCritical() << e.what();
        return -1;
    }
}

void SeenoLightSourceController::writeCmd(SLSCCommandStruct& cmd)
{
    qint64 writtenDataLen = device.write(reinterpret_cast<const char*>(&cmd), sizeof (cmd));
    if(writtenDataLen != sizeof (cmd))
    {
        throw std::exception("Written data len error!");
    }
}

SLSCCommandStruct SeenoLightSourceController::readRsp()
{
    SLSCCommandStruct cmd;
    readData(reinterpret_cast<char*>(&cmd), sizeof (cmd));
    return cmd;
}

void SeenoLightSourceController::readData(char *data, int len)
{
    int totalReadDataLen = 0;
    while (totalReadDataLen < len) {
        if(!device.waitForReadyRead(200))
        {
            throw std::exception("Wait for ready read time out!");
        }
        qint64 readDataLen = device.read(data + totalReadDataLen, len - totalReadDataLen);
        totalReadDataLen += readDataLen;
    }
}
