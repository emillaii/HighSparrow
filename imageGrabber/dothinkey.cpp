#include "imageGrabber/dothinkey.h"
#include <QTime>
#include <QDateTime>
#include "utils/commonmethod.h"
bool Dothinkey::CameraChannel::CloseCameraChannel()
{
    if (this->m_iDevID>= 0) {
        CloseDevice(m_iDevID);
        return true;
    }
    return false;
}

Dothinkey::Dothinkey(QObject *parent) : PropertyBase(parent)
{
//    loadParams();
}

Dothinkey::~Dothinkey()
{
    //Clear things here
}

void Dothinkey::saveJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("DOTHINKEY_PARAMS", this);
    PropertyBase::saveJsonConfig(file_name, temp_map);
}

void Dothinkey::loadParams(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("DOTHINKEY_PARAMS", this);
    PropertyBase::loadJsonConfig(file_name, temp_map);
}

void Dothinkey::DothinkeySetConfigFile(std::string filename)
{
    this->iniFilename = filename;
}

QString Dothinkey::readSensorID()
{
    return this->currentSensorID();
}

BOOL Dothinkey::DothinkeyEnum()
{
    qDebug("[DothinkeyEnum] is called");
    DeviceName[4] = { 0 };
    int DeviceNum;
    EnumerateDevice(DeviceName, 4, &DeviceNum);
    if (DeviceNum == 0)
    {
        //Log::GetInstance()->Write("[DothinkeyEnum] Cannot find any device!");
        qCritical("[DothinkeyEnum] Cannot find any device!");
        return DT_ERROR_FAILED;
    }
    for (int i = 0; i < DeviceNum; i++)
    {
        if (DeviceName[i] != nullptr)
        {
            QString str = "[DothinkeyEnum] Device Found: ";
            str.append(DeviceName[i]);
            qInfo(str.toStdString().c_str());
        }
    }
    return DT_ERROR_OK;
}

//Only one channel of dothinkey
BOOL Dothinkey::DothinkeyOpen()
{
    m_CameraChannels[0].CloseCameraChannel();

    int iDevIDA = -1;
    if (OpenDevice(this->DeviceName[0], &iDevIDA, 0) != DT_ERROR_OK)
    {
        qCritical("[DothinkeyOpen] Open device fail!");
        return DT_ERROR_FAILED;
    }
    BYTE pSN[32];
    int iBufferSize = 32;
    int pRetLen = 0;
    if (GetDeviceSN(pSN, iBufferSize, &pRetLen, iDevIDA) == DT_ERROR_OK)
    {
        std::string s(reinterpret_cast<const char *>(pSN), 32);
        if (s.length() > 0) {
            qInfo("[DothinkeyOpen] Open device success!");
            m_CameraChannels[0].m_iDevID = iDevIDA;
        }
    }
    return DT_ERROR_OK;
}

BOOL Dothinkey::DothinkeyClose()
{
    qInfo("Close device!");
    isGrabbing = false;
    setCurrentSensorID("");
    for (CameraChannel cc: m_CameraChannels)
    {
        cc.CloseCameraChannel();
    }
    return DT_ERROR_OK;
}

BOOL Dothinkey::DothinkeyLoadIniFile(int channel) {
    SensorTab *pCurrentSensor;
    pCurrentSensor = &(this->m_CameraChannels[channel].current_sensor);
    iniParser *iniParser_ = new iniParser();
    QString filename = this->m_IniFilename;
    filename.replace("file:///", "");
    std::string str_filename = filename.toStdString();
    iniParser_->SetIniFilename(str_filename);
    if (channel == 0 || channel == 1) {
        m_CameraChannels[channel].m_fMclk = (float)iniParser_->ReadIniData("Sensor", "mclk", 0x01) / 1000;
        m_CameraChannels[channel].m_fAvdd = (float)iniParser_->ReadIniData("Sensor", "avdd", 0x00) / 1000;
        m_CameraChannels[channel].m_fDovdd = (float)iniParser_->ReadIniData("Sensor", "dovdd", 0x00) / 1000;
        m_CameraChannels[channel].m_fDvdd = (float)iniParser_->ReadIniData("Sensor", "dvdd", 0x00) / 1000;
    }
    pCurrentSensor->width = iniParser_->ReadIniData("Sensor", "width", 0);
    pCurrentSensor->height = iniParser_->ReadIniData("Sensor", "height", 0);
    pCurrentSensor->type = iniParser_->ReadIniData("Sensor", "type", 2);
    pCurrentSensor->port = iniParser_->ReadIniData("Sensor", "port", 0);
    pCurrentSensor->pin = iniParser_->ReadIniData("Sensor", "pin", 0);
    pCurrentSensor->SlaveID = iniParser_->ReadIniData("Sensor", "SlaveID", 0);
    pCurrentSensor->mode = iniParser_->ReadIniData("Sensor", "mode", 0);
    pCurrentSensor->FlagReg = iniParser_->ReadIniData("Sensor", "FlagReg", 0);
    pCurrentSensor->FlagMask = iniParser_->ReadIniData("Sensor", "FlagMask", 0xff);
    pCurrentSensor->FlagData = iniParser_->ReadIniData("Sensor", "FlagData", 0);
    pCurrentSensor->FlagReg1 = iniParser_->ReadIniData("Sensor", "FlagReg1", 0);
    pCurrentSensor->FlagMask1 = iniParser_->ReadIniData("Sensor", "FlagMask1", 0x0);
    pCurrentSensor->FlagData1 = iniParser_->ReadIniData("Sensor", "FlagData1", 0);
    pCurrentSensor->outformat = iniParser_->ReadIniData("Sensor", "outformat", 0x00);
    pCurrentSensor->mclk = iniParser_->ReadIniData("Sensor", "mclk", 0x01);
    pCurrentSensor->avdd = iniParser_->ReadIniData("Sensor", "avdd", 0x00);
    pCurrentSensor->dovdd = iniParser_->ReadIniData("Sensor", "dovdd", 0x00);
    pCurrentSensor->dvdd = iniParser_->ReadIniData("Sensor", "dvdd", 0x00);
    pCurrentSensor->ParaList = new USHORT[8192 * 4];
    pCurrentSensor->ParaListSize = 0;
    pCurrentSensor->SleepParaList = new USHORT[8192 * 4];
    pCurrentSensor->SleepParaListSize = 0;
    cmd_list.clear();
    otp_list.clear();
    iniParser_->GetI2CData(pCurrentSensor, cmd_list, otp_list);
    delete iniParser_;
    return DT_ERROR_OK;
}

/*
 * Dothinkey Start Camera Funcion
 * params: int channel : Select camera channel
 * This function is used to power on the camera modules
 */
BOOL Dothinkey::DothinkeyStartCamera(int channel)
{
    int res;
    SensorTab *pSensor = nullptr;
    ULONG *grabSize = nullptr;
    m_currentSensorID = "0";
    int iDevID = -1;
    float fMclk = 0;
    float fAvdd = 0;
    float fDvdd = 0;
    float fDovdd = 0;
    float fAfvcc = 0;
    UINT vpp;
    if (channel == 0 || channel == 1) {
        pSensor = &(m_CameraChannels[channel].current_sensor);
        iDevID = m_CameraChannels[channel].m_iDevID;
        grabSize = &(m_CameraChannels[channel].m_GrabSize);
        fMclk = m_CameraChannels[channel].m_fMclk;
        fAvdd = m_CameraChannels[channel].m_fAvdd;
        fDvdd = m_CameraChannels[channel].m_fDvdd;
        fDovdd = m_CameraChannels[channel].m_fDovdd;
        fAfvcc = m_CameraChannels[channel].m_fAfvcc;
        vpp = m_CameraChannels[channel].m_vpp;
    }
    SetSoftPinPullUp(IO_NOPULL, 0);
    if (SetSensorClock(false, (USHORT)(0 * 10), iDevID) != DT_ERROR_OK)
    {
        CloseDevice(iDevID);
        qCritical("[DothinkeyStartCamera] Set Clock Fail!");
        return false;
    }
    Sleep(1);

    if (SetVoltageMclk(*pSensor, iDevID, fMclk, fAvdd, fDvdd, fDovdd, fAfvcc, vpp) != DT_ERROR_OK)
    {
        CloseDevice(iDevID);
        qCritical("[DothinkeyStartCamera] Set Voltage and Mclk Failed!");
        return false;
    }
    qInfo("[DothinkeyStartCamera] Start Camera Success!");

    //I2C init
    res = SetSensorI2cRate(I2C_400K, iDevID);
    qDebug("[DothinkeyStartCamera]SetSensorI2cRate(I2C_400K, iDevID) = %d",res);
    res = SetSensorI2cRapid(false, iDevID);
    qDebug("[DothinkeyStartCamera]SetSensorI2cRapid(TRUE, iDevID) = %d",res);
    SetMipiImageVC(0, true, 1, iDevID);
    //check sensor is on line or not ,if on line,init sensor to work....
    res =SensorEnable(pSensor->pin ^ 0x02, 1, iDevID); //reset
    qDebug("[DothinkeyStartCamera]SensorEnable(pSensor->pin ^ 0x02, 1, iDevID) = %d",res);
    Sleep(20);
    res =SensorEnable(pSensor->pin, 1, iDevID);
    qDebug("[DothinkeyStartCamera]SensorEnable(pSensor->pin, 1, iDevID) = %d",res);
    Sleep(50);

    //check sensor is on line...
    res = SensorIsMe(pSensor, CHANNEL_A, 0, iDevID);
    qDebug("[DothinkeyStartCamera]SensorIsMe(pSensor, CHANNEL_A, 0, iDevID) = %d",res);
    if  (res != DT_ERROR_OK)
    {
        qCritical("[DothinkeyStartCamera] Sensor is not ok!");
        return false;
    }
    //init sensor....
    if (InitSensor(pSensor->SlaveID,
        pSensor->ParaList,
        pSensor->ParaListSize,
        pSensor->mode, iDevID) != DT_ERROR_OK)
    {
        qCritical("[DothinkeyStartCamera] Init Sensor Failed! size: %d mode: %d\r\n", pSensor->ParaListSize, pSensor->mode);
        return false;
    }

    if (pSensor->type == D_YUV || pSensor->type == D_YUV_SPI || pSensor->type == D_YUV_MTK_S)
        SetYUV422Format(pSensor->outformat, iDevID);
    else
        SetRawFormat(pSensor->outformat, iDevID);
    res = InitRoi(0, 0, pSensor->width, pSensor->height, 0, 0, 1, 1, pSensor->type, TRUE, iDevID);
    qDebug("[DothinkeyStartCamera]InitRoi(0, 0, pSensor->width, pSensor->height, 0, 0, 1, 1, pSensor->type, TRUE, iDevID) = %d",res);
    res = SetSensorPort(pSensor->port, pSensor->width, pSensor->height, iDevID);
    qDebug("[DothinkeyStartCamera]SetSensorPort(pSensor->port, pSensor->width, pSensor->height, iDevID) = %d",res);
    res = CalculateGrabSize(grabSize, iDevID);
    qDebug("[DothinkeyStartCamera]CalculateGrabSize(grabSize, iDevID) = %d",res);
    //open video....
    res = OpenVideo(*grabSize, iDevID);
    qDebug("[DothinkeyStartCamera]OpenVideo(*grabSize, iDevID) = %d",res);
    //SetMonoMode(true, iDevID);
    //InitDisplay and InitIsp are required to call in order to capture frame
    res = InitDisplay(nullptr, pSensor->width, pSensor->height, pSensor->type, CHANNEL_A, NULL, iDevID);
    qDebug("[DothinkeyStartCamera]InitDisplay(nullptr, pSensor->width, pSensor->height, pSensor->type, CHANNEL_A, NULL, iDevID) = %d",res);
    res = InitIsp(pSensor->width, pSensor->height, pSensor->type, CHANNEL_A, iDevID);
    qDebug("[DothinkeyStartCamera]InitIsp(pSensor->width, pSensor->height, pSensor->type, CHANNEL_A, iDevID) = %d",res);
    isGrabbing = true;
    //TODO: Move that to test item or in dothinkey config file
    USHORT value_1 =0;
    qInfo("slaveID = %04X, mode = %d", pSensor->SlaveID, pSensor->mode);

    //ToDo: Waiting for test
    QString sensor_id = "";
    for (int i = 0; i < cmd_list.size(); i++){
        qInfo("cmd: %s", cmd_list[i].toStdString().c_str());
        QStringList cmd = cmd_list[i].split(QRegExp("\\,"),QString::SkipEmptyParts);
        if (cmd.size()>0) {
            if (cmd[0].compare("setregex", Qt::CaseInsensitive) == 0){
                uint slaveId = CommonMethod::getIntFromHexOrDecString(cmd[1]);
                uint addr = CommonMethod::getIntFromHexOrDecString(cmd[2]);
                uint value = CommonMethod::getIntFromHexOrDecString(cmd[3]);
                uint mode = CommonMethod::getIntFromHexOrDecString(cmd[4]);
                qInfo("Set Register slaveId: %x addr: %x value: %x mode: %d", slaveId, addr, value, mode);
                WriteSensorReg(pSensor->SlaveID, (USHORT)addr,  (USHORT)value, mode);
            } else if (cmd[0].compare("getregex", Qt::CaseInsensitive) == 0){
                QString temp = "";
                uint slaveId = CommonMethod::getIntFromHexOrDecString(cmd[1]);
                uint addr = CommonMethod::getIntFromHexOrDecString(cmd[2]);
                uint mode = CommonMethod::getIntFromHexOrDecString(cmd[3]);
                qInfo("Get Register slaveId: %x addr: %x", slaveId, addr);
                bool ret = ReadSensorReg(pSensor->SlaveID, addr, &value_1, mode);
                qInfo("Read value result: %d value: %04x", ret, value_1);
                sensor_id.append(temp.sprintf("%02X", value_1));
            } else if (cmd[0].compare("sleep", Qt::CaseInsensitive) == 0){
                uint time = cmd[1].toInt(0);
                Sleep(10);
            }
        }
    }
    qInfo("Sensor Id : %s", sensor_id.toStdString().c_str());
    setCurrentSensorID(sensor_id);
    return true;
}

BOOL Dothinkey::DothinkeyOTPEx()
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QByteArray byteData_year;
    int year = date.year();
    int year_MSB = year/100;
    int year_LSB = year - year_MSB*100;
    bool result_otp = true;
    //QStringList otp_list = params.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    byte uAddr = 0xA4; //Default Slave ID

    for (int i = 0; i < otp_list.size(); i++){
        qInfo("otp command: %s", otp_list[i].toStdString().c_str());
        QStringList cmd_list = otp_list[i].split(QRegExp("\\,"),QString::SkipEmptyParts);
        if (cmd_list.size() > 0){
            //Slave ID Handle
            if (cmd_list[0].compare("SlaveID", Qt::CaseInsensitive) == 0){
                if (cmd_list.size()>1) { //Protocol : Slave ID CMD / Slave ID
                    uAddr = CommonMethod::getIntFromHexOrDecString(cmd_list[1]);
                }
            } // Set Register Handle
            else if (cmd_list[0].compare("SetRegEx", Qt::CaseInsensitive) == 0){
                if (cmd_list.size() > 3) { //Protocol : SetReg / Address / Value / mode
                    uAddr = CommonMethod::getIntFromHexOrDecString(cmd_list[1]);
                    uint addr = CommonMethod::getIntFromHexOrDecString(cmd_list[2]);
                    uint value  = 0;
                    if (cmd_list[3].compare("YY_H", Qt::CaseSensitive) == 0){
                        value = year_MSB;
                    } else if (cmd_list[3].compare("YY_L", Qt::CaseSensitive) == 0){
                        value = year_LSB;
                    } else if (cmd_list[3].compare("MM", Qt::CaseSensitive) == 0) {
                        value = date.month();
                    } else if (cmd_list[3].compare("DD", Qt::CaseSensitive) == 0) {
                        value = date.day();
                    } else if (cmd_list[3].compare("HH", Qt::CaseSensitive) == 0) {
                        value = time.hour();
                    } else if (cmd_list[3].compare("MI", Qt::CaseSensitive) == 0) {
                        value = time.minute();
                    } else if (cmd_list[3].compare("SS", Qt::CaseSensitive) == 0) {
                        value = time.second();
                    } else {
                        value = CommonMethod::getIntFromHexOrDecString(cmd_list[3]);
                    }
                    uint mode = CommonMethod::getIntFromHexOrDecString(cmd_list[4]);
                    qInfo("SetRegEx uAddr: 0x%X, addr: 0x%4X, value : %d, mode: %d.", uAddr, addr, value, mode);
                    int result = WriteSensorReg(uAddr, addr, value, mode);
                    if(result != 1) {
                        qCritical("WriteSensorReg fail. uAddr: 0x%X addr: 0x%4X value: %d mode: %d", uAddr, addr, value, mode);
                        result_otp = false;
                    }
                }
            } // Sleep Handle
            else if (cmd_list[0].compare("Delay", Qt::CaseInsensitive) == 0){
                if (cmd_list.size() > 1) { //Protocol : Sleep / ms delay
                    uint delay_in_ms = CommonMethod::getIntFromHexOrDecString(cmd_list[1]);
                    qInfo("Sleep : 0x%X", delay_in_ms);
                    Sleep(delay_in_ms);
                }
            }
        }
    }
    return result_otp;
}

// Hardcore OTP in UV, temp solution, move to ini file later
BOOL Dothinkey::DothinkeyOTP(int serverMode)
{
    // 解开保护
    SensorTab *pSensor = nullptr;
    pSensor = &(m_CameraChannels[0].current_sensor);
//    byte uAddr = pSensor->SlaveID;
    bool result_otp = true;
    byte uAddr = 0xA0;
    int result = WriteSensorReg(uAddr, 0x8000, 0x00, 3);
    Sleep(300);
    qInfo("write reg %X  value %02X  result %d", 0x8000, 0x00, result);
    if(result != 1)
        result_otp = false;
    int number = 120*100 + serverMode + 1;
    QByteArray byte;
    byte.resize(sizeof(int));
    memcpy(byte.data(), &number, sizeof(int));
    USHORT value = byte[0];
    // 机台号
    result = WriteSensorReg(uAddr, 0x1F90, value, 3);
    Sleep(300);
    qInfo("write reg %X  value %02X result %d",0x1F90, value,result);
    if(result != 1)
        result_otp = false;
    // 工位号
    value = byte[1];
    result = WriteSensorReg(uAddr, 0x1F91, value, 3);
    Sleep(300);
    qInfo("write reg %X  value %02X result %d",0x1F91, value,result);
    if(result != 1)
        result_otp = false;

    // 镜头标志位
    USHORT lens_label = 0x00;
//    result = WriteSensorReg(uAddr, 0x1E93, lens_label, pSensor->mode);
//    Sleep(300);
//    qInfo("write reg %X  value %02X result %d",0x1F03, lens_label,result);
//    if(result != 1)
//        result_otp = false;

    // 时间
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    int year = date.year();
    byte.resize(sizeof(int));
    memcpy(byte.data(), &year, sizeof(int));
    value = byte[0];
    result = WriteSensorReg(uAddr, 0x1F94, value, 3);
    Sleep(300);
    qInfo("write reg %X  value %02X result %d",0x1F94, value,result);
    if(result != 1)
        result_otp = false;
    value = byte[1];
    result = WriteSensorReg(uAddr, 0x1F95, value, 3);
    Sleep(300);
    qInfo("write reg %X  value %02X result %d",0x1F95,value,result);
    if(result != 1)
        result_otp = false;
    result = WriteSensorReg(uAddr, 0x1F96, date.month(), 3);
    Sleep(300);
    qInfo("write reg %X  value %02X result %d",0x1F96, date.month(),result);
    if(result != 1)
        result_otp = false;
    result = WriteSensorReg(uAddr, 0x1F97, date.day(), 3);
    Sleep(300);
    qInfo("write reg %X  value %02X result %d",0x1F97, date.day(),result);
    if(result != 1)
        result_otp = false;
    result = WriteSensorReg(uAddr, 0x1F98, time.hour(), 3);
    Sleep(300);
    qInfo("write reg %X  value %02X result %d",0x1F98, time.hour(),result);
    if(result != 1)
        result_otp = false;
    result = WriteSensorReg(uAddr, 0x1F99, time.minute(), 3);
    Sleep(300);
    qInfo("write reg %X  value %02X result %d",0x1F99, time.minute(),result);
    if(result != 1)
        result_otp = false;
    result = WriteSensorReg(uAddr, 0x1F9A, time.second(), 3);
    Sleep(300);
    qInfo("write reg %X  value %02X result %d",0x1E9A, time.second(),result);
    if(result != 1)
        result_otp = false;

    // 检查镜头标志位 0 or 1
    value = 0;
    //result = ReadSensorReg(uAddr, 0x1E93, &value, pSensor->mode);
    qInfo("Read reg %X  value %02X", 0x1F93, value);
    qInfo("value = %u, lens_label = %u, result_otp = %u", value, lens_label, result_otp);
    if ((value != lens_label)||(!result_otp))
    {
        qInfo("DothinkeyOTP fail");
        return FALSE;
    }
    else {
        return TRUE;
    }
}

BOOL Dothinkey::SetVoltageMclk(SensorTab CurrentSensor, int iDevID, float Mclk, float Avdd, float Dvdd, float Dovdd, float Afvcc, float vpp)
{
    SENSOR_POWER Power[10] = { POWER_AVDD, POWER_DOVDD, POWER_DVDD, POWER_AFVCC, POWER_VPP };
    int Volt[10] = { 0 };
    int Current[10] = { 300, 300, 300, 300, 100 };//300mA
    BOOL OnOff[10] = { TRUE,TRUE,TRUE,TRUE,TRUE };
    CURRENT_RANGE range[5] = { CURRENT_RANGE_MA, CURRENT_RANGE_MA, CURRENT_RANGE_MA, CURRENT_RANGE_MA, CURRENT_RANGE_MA };
    //set power to 0V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        CloseDevice(iDevID);
        qCritical("[SetVoltageMclk] Set Voltage Failed! \r\n");
        return DT_ERROR_FAILED;
    }
    //wait for the power is all to zero....
    Sleep(50);
    if (PmuSetOnOff(Power, OnOff, 5, iDevID) != DT_ERROR_OK)
    {
        CloseDevice(iDevID);
        qCritical("[SetVoltageMclk] Open PowerOnOff Failed! \r\n");
        return DT_ERROR_FAILED;
    }
    Sleep(50);
    // 1. set power the avdd
    Volt[POWER_DOVDD] = (int)(Dovdd * 1000); // 2.8V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        qCritical("[SetVoltageMclk] Open PowerOn Failed!");
        return DT_ERROR_FAILED;
    }
    Sleep(2);

    // 2, set power the dvdd
    Volt[POWER_DVDD] = (int)(Dvdd * 1000); // 2.8V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        qCritical("[SetVoltageMclk] Open PowerOn Failed!");
        return DT_ERROR_FAILED;
    }
    Sleep(2);

    // 3, set power the dovdd
    Volt[POWER_AVDD] = (int)(Avdd * 1000); // 2.8V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        qCritical("[SetVoltageMclk] Open PowerOn Failed!");
        return DT_ERROR_FAILED;
    }
    Sleep(2);
    //4. set power the afvcc and vpp
    Volt[POWER_AFVCC] = (int)(Afvcc * 1000); // 2.8V
    Volt[POWER_VPP] = (int)(vpp * 1000); // 2.8V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        qCritical("[SetVoltageMclk] Open PowerOn Failed!");
        return DT_ERROR_FAILED;
    }
    //should wait for 50ms to be ready...
    Sleep(50);
    //first set pin definition...
    {
        BYTE  pinDef[40] = { 0 };
        if (CurrentSensor.port == PORT_MIPI || CurrentSensor.port == PORT_HISPI)
        {
            pinDef[0] = 20;
            pinDef[1] = 0;
            pinDef[2] = 2;
            pinDef[3] = 1;
            pinDef[4] = 3;
            pinDef[5] = 4;
            pinDef[6] = 5;
            pinDef[7] = 6;
            pinDef[8] = 7;
            pinDef[9] = 8;
            pinDef[10] = 9;
            pinDef[11] = 20;
            pinDef[12] = 10;
            pinDef[13] = 11;
            pinDef[14] = 12;
            pinDef[15] = 20;
            pinDef[16] = 20;
            pinDef[17] = 13;
            pinDef[18] = 15;
            pinDef[19] = 14;
            pinDef[20] = 19;
            pinDef[21] = 18;
            pinDef[22] = 20;
            pinDef[23] = 16;
            pinDef[24] = 20;
            pinDef[25] = 20;
        }
        else  //standard parallel..
        {

            pinDef[0] = 16;
            pinDef[1] = 0;
            pinDef[2] = 2;
            pinDef[3] = 1;
            pinDef[4] = 3;
            pinDef[5] = 4;
            pinDef[6] = 5;
            pinDef[7] = 6;
            pinDef[8] = 7;
            pinDef[9] = 8;
            pinDef[10] = 9;
            pinDef[11] = 20;
            pinDef[12] = 10;
            pinDef[13] = 11;
            pinDef[14] = 12;
            pinDef[15] = 20;
            pinDef[16] = 20;
            pinDef[17] = 20;
            pinDef[18] = 20;
            pinDef[19] = 20;
            pinDef[20] = 13;
            pinDef[21] = 20;
            pinDef[22] = 14;
            pinDef[23] = 15;
            pinDef[24] = 18;
            pinDef[25] = 19;
        }
        SetSoftPin(pinDef, iDevID);
    }
    EnableSoftPin(TRUE, iDevID);
    EnableGpio(TRUE, iDevID);

    int SampleSpeed[5] = { 100,100,100,100,100 };
    PmuSetSampleSpeed(Power, SampleSpeed, 5, iDevID);
    Sleep(10);
    {
        PmuSetCurrentRange(Power, range, 5, iDevID);
        PmuSetOcpCurrentLimit(Power, Current, 5, iDevID);
    }
    if (SetSensorClock(TRUE, (USHORT)(Mclk * 10), iDevID) != DT_ERROR_OK)
    {
        qCritical("[SetVoltageMclk] Set Mclk Failed!");
        return DT_ERROR_FAILED;
    }
    SetSoftPinPullUp(IO_PULLUP, iDevID);
    return TRUE;
}

cv::Mat Dothinkey::DothinkeyGrabImageCV(int channel, bool &grabRet)
{
    grabRet = true;
    SensorTab *pSensor = nullptr;
    ULONG retSize = 0;
    int iDevID = -1;
    UINT crcCount = 0;
    int grabSize;
    if (channel == 0 || channel == 1) {
        pSensor = &(m_CameraChannels[channel].current_sensor);
        iDevID = m_CameraChannels[channel].m_iDevID;
        grabSize = m_CameraChannels[channel].m_GrabSize;
    }

    USHORT width = pSensor->width;
    USHORT height = pSensor->height;
    //BYTE type = pSensor->type;
    FrameInfo frameInfo;
    static LPBYTE bmpBuffer = (LPBYTE)malloc(width * height * 4);
    //allocate the bmp buffer.
    UINT nSize = width * height * 3 + 1024 * 1024;
    LPBYTE CameraBuffer = NULL;
    CameraBuffer = (LPBYTE)malloc(nSize);
    if ((CameraBuffer == NULL))
    {
        qInfo("CameraBuffer is Null");
        cv::Mat img;
        grabRet = false;
        return img;
    }
    memset(CameraBuffer, 0, nSize);
    int ret = GrabFrame(CameraBuffer, grabSize, &retSize, &frameInfo, iDevID);
    if (ret == DT_ERROR_OK)
    {
        GetMipiCrcErrorCount(&crcCount, CHANNEL_A, iDevID);
    } else {
        qInfo("Camera Grab Frame Fail, GrabFrame() returned error code: %d", ret);
        cv::Mat img;
        grabRet = false;
    }
    ImageProcess(CameraBuffer, bmpBuffer, width, height, &frameInfo, iDevID);
    CvSize mSize;
    mSize.height = height;
    mSize.width = width;
    cv::Mat img(height, width, CV_8UC3, bmpBuffer);
    delete(CameraBuffer);
    CameraBuffer = NULL;
    return img;
}

QImage* Dothinkey::DothinkeyGrabImage(int channel)
{
    SensorTab *pSensor = nullptr;
    ULONG retSize = 0;
    int iDevID = -1;
    UINT crcCount = 0;
    int grabSize;
    if (channel == 0 || channel == 1) {
        pSensor = &(m_CameraChannels[channel].current_sensor);
        iDevID = m_CameraChannels[channel].m_iDevID;
        grabSize = m_CameraChannels[channel].m_GrabSize;
    }

    USHORT width = pSensor->width;
    USHORT height = pSensor->height;
    //BYTE type = pSensor->type;
    FrameInfo frameInfo;
    static LPBYTE bmpBuffer = (LPBYTE)malloc(width * height * 4);
    //allocate the bmp buffer.
    UINT nSize = width * height * 3 + 1024 * 1024;
    LPBYTE CameraBuffer = NULL;
    CameraBuffer = (LPBYTE)malloc(nSize);
    if ((CameraBuffer == NULL))
    {
        return false;
    }
    memset(CameraBuffer, 0, nSize);
    int ret = GrabFrame(CameraBuffer, grabSize, &retSize, &frameInfo, iDevID);
    if (ret == DT_ERROR_OK)
    {
        GetMipiCrcErrorCount(&crcCount, CHANNEL_A, iDevID);
    }
    ImageProcess(CameraBuffer, bmpBuffer, width, height, &frameInfo, iDevID);
    CvSize mSize;
    mSize.height = height;
    mSize.width = width;
    QImage * image = new QImage((const uchar*) bmpBuffer, width, height, QImage::Format_RGB888);
    delete(CameraBuffer);
    CameraBuffer = NULL;
    return image;
}

BOOL Dothinkey::SaveBmpFile(std::string sfilename, BYTE *pBuffer, UINT width, UINT height)
{
    int				 OffBits;
    HFILE			 bmpFile;
    BITMAPFILEHEADER bmpHeader; // Header for Bitmap file
    BITMAPINFO		 bmpInfo;

    OffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    DWORD dwSizeImage = width * height * 3;//IMAGESIZE_X*IMAGESIZE_Y*3;

    bmpHeader.bfType = ((WORD)('M' << 8) | 'B');
    bmpHeader.bfSize = OffBits + dwSizeImage;
    bmpHeader.bfReserved1 = 0;
    bmpHeader.bfReserved2 = 0;
    bmpHeader.bfOffBits = OffBits;

    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = width;
    bmpInfo.bmiHeader.biHeight = height;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 24;
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bmiHeader.biSizeImage = 0;
    bmpInfo.bmiHeader.biXPelsPerMeter = 0;
    bmpInfo.bmiHeader.biYPelsPerMeter = 0;
    bmpInfo.bmiHeader.biClrUsed = 0;
    bmpInfo.bmiHeader.biClrImportant = 0;

    bmpFile = _lcreat(sfilename.c_str(), FALSE);
    if (bmpFile < 0)
    {
        qCritical("Cannot create the bmp file.\r\n");
        return FALSE;
    }

    UINT len;
    len = _lwrite(bmpFile, (LPSTR)&bmpHeader, sizeof(BITMAPFILEHEADER));
    len = _lwrite(bmpFile, (LPSTR)&bmpInfo, sizeof(BITMAPINFOHEADER));
    len = _lwrite(bmpFile, (LPSTR)pBuffer, bmpHeader.bfSize - sizeof(bmpHeader) - sizeof(bmpInfo) + 4);  //+4 is for exact filesize
    _lclose(bmpFile);

    return TRUE;
}

bool Dothinkey::initSensor()
{
//    if (imageThread->isRunning()) {
//        dk->DothinkeyClose();
//        imageThread->stop();
//    }
//    XtMotion::xtout_cmos_vacuum = true;
//    XtMotion::xtout_pogopin = true;
    const int channel = 0;
    bool res = DothinkeyEnum();
    if (!res) { qCritical("Cannot find dothinkey"); return false; }
    res = DothinkeyOpen();
    if (!res) { qCritical("Cannot open dothinkey"); return false; }
    res = DothinkeyLoadIniFile(channel);
    if (!res) { qCritical("Cannot load dothinkey ini file"); return false; }
    res = DothinkeyStartCamera(channel);
    if (!res) { qCritical("Cannot start camera"); return false; }
    //imageThread->start();
    return true;
}

BOOL Dothinkey::DothinkeyIsGrabbing()
{
    return isGrabbing;
}
