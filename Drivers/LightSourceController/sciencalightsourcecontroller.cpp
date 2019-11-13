#include "sciencalightsourcecontroller.h"


SciencaLightSourceController::SciencaLightSourceController():
    isOpen(false), currenChannel(-1)
{
    loadJsonConfig(configFileName, configSectionName);
}

SciencaLightSourceController::~SciencaLightSourceController()
{
    close();
}

void SciencaLightSourceController::open()
{
    if(isOpen)
    {
        qInfo("Has been opened!");
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
        setIntensity(WaveLength);
        setIntensity(ColorTemperature);
    }
    else {
        qCritical("Open port failed!");
    }
}

void SciencaLightSourceController::close()
{
    if(isOpen)
    {
        device.close();
        isOpen = false;
    }
}

void SciencaLightSourceController::setIntensity(Channel channel)
{
    if(!isOpen){qCritical() << tr("Did not init!"); return;}

    try {
        if(channel != currenChannel)
        {
            selectChannel(channel);
            currenChannel = channel;
        }
        SLSCCommandStruct cmd;
        cmd.cmd = SetBrightness;
        ushort value = 0;
        if(channel == ColorTemperature)
        {
            value = colorTemperatureIntensity();
        }
        else if(channel == WaveLength)
        {
            value = waveLengthIntensity();
        }
        cmd.data1 = value & 0xff;
        cmd.data2 = (value & 0xff00) >> 8;
        cmd.calculateCheckCode();
        writeCmd(cmd);
        SLSCCommandStruct rsp = readRsp();
        readRsp();  //冗余回复信息
        if(rsp.data1 != cmd.data1 ||
           rsp.data2 != cmd.data2||
           rsp.cmd != GetBrightness ||
           rsp.data3 != currenChannel)
        {
            qCritical() << tr("Response error! Cmd: ") << cmd.toString() << "Rsp: " << rsp.toString();
        }
    } catch (std::exception& e) {
        qCritical() << e.what();
    }

}

int SciencaLightSourceController::getIntensity(Channel channel)
{
    if(!isOpen){qCritical() << tr("Did not init!"); return -1;}

    try {
        if(channel != currenChannel)
        {
            selectChannel(channel);
            currenChannel = channel;
        }
        SLSCCommandStruct cmd;
        cmd.cmd = GetBrightness;
        cmd.calculateCheckCode();
        writeCmd(cmd);
        SLSCCommandStruct rsp = readRsp();
        readRsp();  //冗余回复信息
        if(rsp.cmd != GetBrightness ||
           rsp.data3 != currenChannel)
        {
            qCritical() << tr("Response error! Cmd: ") << cmd.toString() << "Rsp: " << rsp.toString();
            return -1;
        }
        int high8 = rsp.data2;
        return rsp.data1 + (high8 << 8);
    } catch (std::exception& e) {
        qCritical() << e.what();
        return -1;
    }
}

void SciencaLightSourceController::selectChannel(int channel)
{
    SLSCCommandStruct cmd;
    cmd.cmd = SelectChannel;
    cmd.data1 = channel;
    cmd.calculateCheckCode();
    writeCmd(cmd);
    readRsp();
    readRsp(); //冗余回复信息
}

void SciencaLightSourceController::writeCmd(SLSCCommandStruct& cmd)
{
    qint64 writtenDataLen = device.write(reinterpret_cast<const char*>(&cmd), sizeof (cmd));
    if(writtenDataLen != sizeof (cmd))
    {
        throw std::exception("Written data len error!");
    }
}

SLSCCommandStruct SciencaLightSourceController::readRsp()
{
    SLSCCommandStruct cmd;
    readData(reinterpret_cast<char*>(&cmd), sizeof (cmd));
    return cmd;
}

void SciencaLightSourceController::readData(char *data, int len)
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
