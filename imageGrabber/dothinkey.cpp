﻿#include "imageGrabber/dothinkey.h"
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
        m_CameraChannels[channel].m_iPhyType = iniParser_->ReadIniData("Sensor", "PhyType", 0);
        m_CameraChannels[channel].m_iLanes = iniParser_->ReadIniData("Sensor", "LANES", 0);
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

int Dothinkey::DothinkeySetSensorEnable(int channel, BYTE &pin, bool bEna)
{
    int iRet;
    int m_nDevID = 0;
    if (bEna)
    {
        // 设置reset pin, pwdn pin
        BYTE Pwdn2=0;
        BYTE Pwdn1=0;
        BYTE Reset=0;

        SensorEnable(pin^RESET_H, TRUE, m_nDevID);
        Sleep(50);
        SensorEnable(pin, TRUE, m_nDevID);
        Sleep(50);

        Pwdn2 = pin & PWDN_H ?  PWDN2_L : PWDN2_H;   //pwdn2 neg to pwdn1
        Pwdn1 = pin & PWDN_H ?  PWDN_H : PWDN_L;     //pwdn1
        Reset = pin & RESET_H ?  RESET_H : RESET_L;  //reset

        pin = Pwdn2 | Pwdn1 | Reset;
        iRet = SensorEnable(pin, 1, m_nDevID); //reset
    }
    else
    {
        iRet = SensorEnable(pin, FALSE, m_nDevID);
    }

    return iRet;
}

int Dothinkey::DothinkeySetSensorPower(int channel, bool bOnOff)
{
    int iRet = DT_ERROR_OK;
    int m_nDevID = 0;

    SENSOR_POWER Power[7];
    int Volt[7];
    int Rise[7];
    BOOL OnOff[7];
    int Current[7];
    CURRENT_RANGE Range[7];
    int SampleSpeed[7];

    DWORD dwDevKit = GetKitType(m_nDevID);
    if (bOnOff)
    {
        Power[0] = POWER_AVDD;
        Volt[0] = (int)(this->m_CameraChannels[channel].m_fAvdd*1000); // 2.8V
        Current[0] = 600; // 500mA
        Rise[0] = 100;
        OnOff[0] = TRUE;
        Range[0] = CURRENT_RANGE_MA;
        SampleSpeed[0] = 200;

        Power[1] = POWER_DOVDD;
        Volt[1] = (int)(this->m_CameraChannels[channel].m_fDovdd*1000); // 1.8V
        Current[1] = 600; // 500mA
        Rise[1] = 100;
        OnOff[1] = TRUE;
        Range[1] = CURRENT_RANGE_MA;
        SampleSpeed[1] = 200;

        Power[2] = POWER_DVDD;
        Volt[2] = (int)(this->m_CameraChannels[channel].m_fDvdd*1000);// 1.8V
        Current[2] =  600;// 600mA
        Rise[2] = 100;
        OnOff[2] = TRUE;
        Range[2] = CURRENT_RANGE_MA;
        SampleSpeed[2] = 200;

        Power[3] = POWER_AFVCC;
        Volt[3] = (int)(2800*100); // 2.8V
        Current[3] = 600; // 600mA
        Rise[3] = 100;
        OnOff[3] = TRUE;
        Range[3] = CURRENT_RANGE_MA;
        SampleSpeed[3] = 200;

        // MU950-TOF机型支持AUX电压
        if (dwDevKit == MU950_TOF)
        {
            Power[4] = POWER_AUX1;
        }
        else if(dwDevKit == G40 || dwDevKit == G41 || dwDevKit == G42 || dwDevKit == G42|| dwDevKit == G22|| dwDevKit == G22L)
        {
            Power[4] = POWER_VPP2;
        }
        else
        {
            Power[4] = POWER_VPP;
        }
        Volt[4] = (int)(this->m_CameraChannels[channel].m_vpp*1000); // 2.8V
        Current[4] = 600; // 600mA
        Rise[4] = 100;
        OnOff[4] = TRUE;
        Range[4] = CURRENT_RANGE_MA;
        SampleSpeed[4] = 200;

        if (dwDevKit == G40 || dwDevKit == G41 || dwDevKit == G42 || dwDevKit == G42|| dwDevKit == G22|| dwDevKit == G22L)
        {
            Power[5] = POWER_AUX1;
        }
        else
        {
            Power[5] = POWER_OISVDD;
        }
//        Volt[5] = (int)(m_fOisvdd * 1000);
//        Current[5] = 600;
//        Rise[5] = 100;
//        OnOff[5] = FALSE;               // 根据需要开启
//        Range[5] = CURRENT_RANGE_MA;
//        SampleSpeed[5] = 200;

//        Power[6] = POWER_AVDD2;
//        Volt[6] = (int)(m_fAvdd2 * 1000);;
//        Current[6] = 600;
//        Rise[6] = 100;
//        OnOff[6] = FALSE;               // 根据需要开启
//        Range[6] = CURRENT_RANGE_MA;
//        SampleSpeed[6] = 200;

        // 设置电压斜率
        iRet = PmuSetRise(Power, Rise, 5, m_nDevID);
        if (iRet != DT_ERROR_OK)
        {
            qWarning("PmuSetRise failed with err:%d\r\n", iRet);
        }

        iRet = PmuSetSampleSpeed(Power, SampleSpeed, 5, m_nDevID);
        if (iRet != DT_ERROR_OK)
        {
            qWarning("PmuSetSampleSpeed failed with err:%d\r\n", iRet);
        }

        // 先设置电压值为0
        Volt[0] = 0;
        Volt[1] = 0;
        Volt[2] = 0;
        Volt[3] = 0;
        Volt[4] = 0;
        Volt[5] = 0;
        Volt[6] = 0;

        // 设置电压值
        iRet = PmuSetVoltage(Power, Volt, 5, m_nDevID);
        if (iRet != DT_ERROR_OK)
        {
            qWarning("PmuSetVoltage failed with err:%d\r\n", iRet);
            return iRet;
        }

        Sleep(10);

        // 设置电压值
        Volt[0] = (int)(this->m_CameraChannels[channel].m_fAvdd*1000);
        Volt[1] = (int)(this->m_CameraChannels[channel].m_fDovdd*1000);
        Volt[2] = (int)(this->m_CameraChannels[channel].m_fDvdd*1000);
        Volt[3] = 1800;
        Volt[4] = 1800;
        Volt[5] = 0; //(int)(m_fOisvdd*1000);
        Volt[6] = 0; //(int)(m_fAvdd2*1000);
        iRet = PmuSetVoltage(Power, Volt, 5, m_nDevID);
        if (iRet != DT_ERROR_OK)
        {
            qWarning("PmuSetVoltage failed with err:%d\r\n", iRet);
            return iRet;
        }

        OnOff[0] = TRUE;
        OnOff[1] = TRUE;
        OnOff[2] = TRUE;
        OnOff[3] = TRUE;
        OnOff[4] = TRUE;

        iRet = PmuSetOnOff(Power, OnOff, 5, m_nDevID);
        if (iRet != DT_ERROR_OK)
        {
            qWarning("PmuSetOnOff failed with err:%d\r\n", iRet);
            return iRet;
        }

        // 设置限流
        iRet = PmuSetOcpCurrentLimit(Power, Current, 5,m_nDevID);
        if (iRet != DT_ERROR_OK)
        {
            qWarning("PmuSetOcpCurrentLimit failed with err:%d\r\n", iRet);
            return iRet;
        }

        // 设置量程
        iRet = PmuSetCurrentRange(Power,Range,5,m_nDevID);
        if (iRet != DT_ERROR_OK)
        {
            qWarning("PmuSetCurrentRange failed with err:%d\r\n", iRet);
            return iRet;
        }

        // 设置电流采集触发点（MU950(TOF)机型支持，当高于设置的触发点电流时，启动电流采集）
        // 设置AUX路电源电流触发点
        PmuCurrentMeasurement_t sPmuCurrentMeasurementAux1;

        // (2019/6/6 MU950(TOF)机型AUX,DVDD,AFVCC路支持电流采集触发点)
        sPmuCurrentMeasurementAux1.PowerType = POWER_AUX1;
        sPmuCurrentMeasurementAux1.bPosEn = TRUE;
        sPmuCurrentMeasurementAux1.lfTriggerPoint = 0;              //当电流大于TriggerPoint后，启动电流采集
        memset(sPmuCurrentMeasurementAux1.Rsv, 0x00, sizeof(sPmuCurrentMeasurementAux1.Rsv));

        iRet = PmuSetCurrentMeasurement(&sPmuCurrentMeasurementAux1, m_nDevID);
        if (iRet != DT_ERROR_OK)
        {
            qWarning("PmuSetCurrentMeasurement failed with err:%d\r\n", iRet);
            return iRet;
        }

        // 设置DVDD路电源电流触发点
        PmuCurrentMeasurement_t sPmuCurrentMeasurementDvdd;

        // (2019/6/6 MU950(TOF)机型AUX,DVDD,AFVCC路支持电流采集触发点)
        sPmuCurrentMeasurementDvdd.PowerType = POWER_DVDD;
        sPmuCurrentMeasurementDvdd.bPosEn = TRUE;
        sPmuCurrentMeasurementDvdd.lfTriggerPoint = 0;              //当电流大于TriggerPoint后，启动电流采集
        memset(sPmuCurrentMeasurementDvdd.Rsv, 0x00, sizeof(sPmuCurrentMeasurementDvdd.Rsv));

        iRet = PmuSetCurrentMeasurement(&sPmuCurrentMeasurementDvdd, m_nDevID);
        if (iRet != DT_ERROR_OK)
        {
            qWarning("PmuSetCurrentMeasurement failed with err:%d\r\n", iRet);
            return iRet;
        }
    }
    else
    {
       // 关闭电源
       Power[0] = POWER_AVDD;
       Volt[0] = 0;
       OnOff[0] = FALSE;

       Power[1] = POWER_DOVDD;
       Volt[1] = 0;
       OnOff[1] = FALSE;

       Power[2] = POWER_DVDD;
       Volt[2] = 0;
       OnOff[2] = FALSE;

       Power[3] = POWER_AFVCC;
       Volt[3] = 0;
       OnOff[3] = FALSE;

       // MU950-TOF机型支持AUX电压
       if (dwDevKit == MU950_TOF)
       {
           Power[4] = POWER_AUX1;
       }
       else if (dwDevKit == G40 || dwDevKit == G41 || dwDevKit == G42 || dwDevKit == G42)
       {
           Power[4] = POWER_VPP2;
       }
       else
       {
           Power[4] = POWER_VPP;
       }
       Volt[4] = 0;
       OnOff[4] = FALSE;

       if (dwDevKit == G40 || dwDevKit == G41 || dwDevKit == G42)
       {
           Power[5] = POWER_AUX1;
       }
       else
       {
           Power[5] = POWER_OISVDD;
       }
       Volt[5] = 0;
       OnOff[5] = FALSE;

       Power[6] = POWER_AVDD2;
       Volt[6] = 0;
       OnOff[6] = FALSE;

       /* 先设置电压为0 */
       iRet = PmuSetVoltage(Power, Volt, 7, m_nDevID);
       if (iRet != DT_ERROR_OK)
       {
           qWarning("PmuSetVoltage failed with err:%d\r\n", iRet);
           return iRet;
       }
       Sleep(150);

       /* 关闭电源 */
       iRet = PmuSetOnOff(Power, OnOff, 7, m_nDevID);
       if (iRet != DT_ERROR_OK)
       {
           qWarning("PmuSetOnOff failed with err:%d\r\n", iRet);
           return iRet;
       }
    }

    return iRet;
}

int Dothinkey::DothinkeyInitSoftPin(int channel, BYTE byPort)
{
    int iRet = DT_ERROR_OK;
    int m_nDevID = 0;

    // 初始化柔性IO
    UCHAR PinDef[26] = {PIN_NC};
    if (byPort == PORT_SONY_LVDS || byPort == PORT_PANASONIC)					// ULM928 spi定义
    {
        PinDef[0] = PIN_NC;
        PinDef[1] = PIN_NC;
        PinDef[2] = PIN_NC;
        PinDef[3] = PIN_NC;
        PinDef[4] = PIN_NC;
        PinDef[5] = PIN_NC;
        PinDef[6] = PIN_NC;
        PinDef[7] = PIN_NC;
        PinDef[8] = PIN_NC;
        PinDef[9] = PIN_NC;
        PinDef[10] = PIN_NC;
        PinDef[11] = PIN_NC;
        PinDef[12] = PIN_NC;
        PinDef[13] = PIN_NC;
        PinDef[14] = PIN_NC;
        PinDef[15] = PIN_NC;
        PinDef[16] = PIN_NC;
        PinDef[17] = PIN_NC;
        PinDef[18] = PIN_NC;				//PIN_PWDN PIN_SPI_CS 15
        PinDef[19] = PIN_NC;
        PinDef[20] = PIN_SPI_SCK;		//19 //PIN_SPI_SCK
        PinDef[21] = PIN_SPI_SDO;		//PIN_SPI_SDO 18
        PinDef[22] = PIN_SPI_CS;
        PinDef[23] = PIN_NC;		    //PIN_SPI_SDO
        PinDef[24] = PIN_GPIO3;			//PIN_SPI_SDI/PIN_SPI_SDA //hs
        PinDef[25] = PIN_GPIO4;
        //vs
    }
    else if (byPort == PORT_MIPI || byPort == PORT_HISPI)
    {
        PinDef[0] = PIN_NC;
        PinDef[1] = PIN_NC;
        PinDef[2] = PIN_NC;
        PinDef[3] = PIN_NC;
        PinDef[4] = PIN_NC;
        PinDef[5] = PIN_NC;
        PinDef[6] = PIN_NC;
        PinDef[7] = PIN_NC;
        PinDef[8] = PIN_NC;
        PinDef[9] = PIN_NC;
        PinDef[10] = PIN_NC;
        PinDef[11] = PIN_NC;
        PinDef[12] = PIN_NC;
        PinDef[13] = PIN_NC;
        PinDef[14] = PIN_NC;
        PinDef[15] = PIN_NC;
        PinDef[16] = PIN_NC;		// CS
        PinDef[17] = PIN_MCLK;		// MCLK
        PinDef[18] = PIN_PWDN;		// PWDN
        PinDef[19] = PIN_RESET;		// RST
        PinDef[20] = PIN_SCL;       // SCL
        PinDef[21] = PIN_SDA;		// SDA
        PinDef[22] = PIN_SPI_SCK;		// PO2
        PinDef[23] = PIN_SPI_CS;     // PO1
        PinDef[24] = PIN_SPI_SDI;     // PO3
        PinDef[25] = PIN_SPI_SDO;		// PO4
    }
    else
    {
        PinDef[0] = PIN_D10;			//
        PinDef[1] = PIN_D0;
        PinDef[2] = PIN_D2;
        PinDef[3] = PIN_D1;
        PinDef[4] = PIN_D3;
        PinDef[5] = PIN_D4;
        PinDef[6] = PIN_D5;
        PinDef[7] = PIN_D6;
        PinDef[8] = PIN_D7;
        PinDef[9] = PIN_D8;
        PinDef[10] = PIN_D9;
        PinDef[11] = PIN_NC;
        PinDef[12] = PIN_PCLK;
        PinDef[13] = PIN_HSYNC;
        PinDef[14] = PIN_VSYNC;
        PinDef[15] = PIN_NC;
        PinDef[16] = PIN_NC;
        PinDef[17] = PIN_NC;
        PinDef[18] = PIN_NC;
        PinDef[19] = PIN_NC;
        PinDef[20] = PIN_MCLK;
        PinDef[21] = PIN_D11;
        PinDef[22] = PIN_RESET;
        PinDef[23] = PIN_PWDN;
        PinDef[24] = PIN_SDA;
        PinDef[25] = PIN_SCL;
    }

    iRet = SetSoftPin(PinDef,m_nDevID);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("SetSoftPin failed with err:%d\r\n", iRet);
        return iRet;
    }

    iRet = EnableSoftPin(TRUE,m_nDevID);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("EnableSoftPin failed with err:%d\r\n", iRet);
        return iRet;
    }

    iRet = SetSoftPinPullUp(TRUE,m_nDevID);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("SetSoftPinPullUp failed with err:%d\r\n", iRet);
        return iRet;
    }

    iRet = EnableGpio(TRUE,m_nDevID);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("EnableGpio failed with err:%d\r\n", iRet);
        return iRet;
    }

    return iRet;
}

BOOL Dothinkey::DothinkeyStartCameraEx(int channel)
{
    int iRet;
    int m_nDevID = 0;
    SensorTab *pSensor = nullptr;
    pSensor = &(m_CameraChannels[channel].current_sensor);
//    iRet = SetSensorPowerOnOff(true, m_nDevID);
    iRet = DothinkeySetSensorPower(channel, true);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("SetSensorPower failed with err:%d\n", iRet);
    }
    /* 初始化柔性接口 */
    iRet = DothinkeyInitSoftPin(channel, pSensor->port);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("InitSoftPin failed with err:%d\n", iRet);
    }
    /* Mipi Port,Lane个数设置 */
    MipiCtrlEx_t sMipiCtrlEx;
    iRet = GetMipiCtrlEx(&sMipiCtrlEx, m_nDevID);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("GetMipiCtrlEx failed with err:%d\n", iRet);
    }

    sMipiCtrlEx.bVCFilterEn = true;
    sMipiCtrlEx.byPhyType = m_CameraChannels[channel].m_iPhyType;
    sMipiCtrlEx.byLaneCnt = m_CameraChannels[channel].m_iLanes;;

    // 启用MIPI LP-10状态检查
    // 注意MIPI_CTRL_CLK_LP10_CHK要求CLK Lane使用非连续时钟，并且同时启用了MIPI_CTRL_NON_CONT
    //sMipiCtrlEx.dwCtrl |= MIPI_CTRL_CLK_LP01_CHK | MIPI_CTRL_DAT_LP01_CHK | MIPI_CTRL_NON_CONT;
    if (iRet != DT_ERROR_OK)
    {
        qWarning("SetMipiCtrlEx failed with err:%d\n", iRet);
    }

    iRet = GetMipiCtrlEx(&sMipiCtrlEx, m_nDevID);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("GetMipiCtrlEx failed with err:%d\n", iRet);
    }

    /* 初始化sensor时钟 */
    iRet = SetSensorClock(TRUE,(USHORT)(m_CameraChannels[channel].m_fMclk*10), m_nDevID);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("SetSensorClock failed with err:%d\n", iRet);
    }

    /* 等待时钟或PLL稳定 */
    Sleep(50);

    /* 使能Sensor， Sensor进入工作状态，一般是使复位管脚设为高电平 */
    iRet = DothinkeySetSensorEnable(channel, pSensor->pin, TRUE);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("SetSensorEnable failed with err:%d\n", iRet);
    }

    /* sensorisme */
//    iRet = SensorIsMe(pSensor,CHANNEL_A,FALSE,m_nDevID);
//    if (iRet != DT_ERROR_OK)
//    {
//        qWarning("SensorIsMe failed with err:%d\n", iRet);
//    }
    SetI2CInterval(0, m_nDevID);
    SetSensorI2cRate(TRUE);

    SetMipiImageVC(0,TRUE,CHANNEL_A,m_nDevID);
    UCHAR uPort=1;//目前只有为2的端口
    MasterSpiConfig_t spiconfig;
    spiconfig.byCtrl=0x00;
    spiconfig.byWordLen=0;
    spiconfig.fMhz=5;
    int rect=MasterSpiConfig(uPort,&spiconfig,m_nDevID);
    /* 初始化 sensor参数 */
    iRet = InitSensor(pSensor->SlaveID, pSensor->ParaList, pSensor->ParaListSize, pSensor->mode, m_nDevID);
    if (iRet != DT_ERROR_OK)
    {
       qWarning("InitSensor failed with err:%d\n", iRet);
    }
    /* 初始化设备，主要是采集功能相关 */
    iRet = InitDevice(nullptr, pSensor->width, pSensor->height, pSensor->port, pSensor->type, CHANNEL_A, NULL, m_nDevID);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("InitDevice failed with err:%d\n", iRet);
    }

    /* 由INI文件中的outformat设置BAYER格式 */
    iRet = SetRawFormat(pSensor->outformat,m_nDevID);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("SetRawFormat failed with err:%d\n", iRet);
    }
    int m_uWidth = pSensor->width;
    int m_uHeight = pSensor->height;
    UINT uSize = m_uWidth*m_uHeight;
    iRet = SetRoiEx(0, 0, pSensor->width, pSensor->height, 0, 0 , 1, 1, TRUE,m_nDevID);
    if (iRet != DT_ERROR_OK)
    {
        qWarning("SetRoiEx failed with err:%d\n", iRet);
    }

    /*
      开启视频流，同时指定目标图像缓存区大小，缓存区大小应该考虑最大分辨率情况；
      如果图像处理中，中间图像使用了BRG32格式(使用SSE指令做色彩处理时)，应该是4倍像素量的关系；
      */
    //SetTargetFormatSel(m_iFormatSel);
    iRet = OpenVideo(uSize * 3, m_nDevID);
    return iRet;
}

/*
 * Dothinkey Start Camera Funcion
 * params: int channel : Select camera channel
 * This function is used to power on the camera modules
 */
BOOL Dothinkey::DothinkeyStartCamera(int channel)
{
    return DothinkeyStartCameraEx(channel);
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
    {
        MipiCtrlEx_t sMipiCtrlEx;
        int iRet = GetMipiCtrlEx(&sMipiCtrlEx, iDevID);
        if (iRet != DT_ERROR_OK)
        {
            qCritical("[DothinkeyStartCamera] GetMipiCtrlEx Fail!");
        }
        qInfo("[DothinkeyStartCamera] Set LaneCnt: %d Set PhyType: %d", m_CameraChannels[channel].m_iLanes, m_CameraChannels[channel].m_iPhyType);
        sMipiCtrlEx.byLaneCnt = m_CameraChannels[channel].m_iLanes;
        sMipiCtrlEx.byPhyType = m_CameraChannels[channel].m_iPhyType;
        SetMipiCtrlEx(&sMipiCtrlEx, iDevID);
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
                qInfo("Set Register slaveId: %x addr: %x value: %x mode: %d", pSensor->SlaveID, addr, value, pSensor->mode);
                WriteSensorReg(pSensor->SlaveID, (USHORT)addr, (USHORT)value, pSensor->mode);
                Sleep(30);
            } else if (cmd[0].compare("getregex", Qt::CaseInsensitive) == 0){
                QString temp = "";
                uint slaveId = CommonMethod::getIntFromHexOrDecString(cmd[1]);
                uint addr = CommonMethod::getIntFromHexOrDecString(cmd[2]);
                uint mode = CommonMethod::getIntFromHexOrDecString(cmd[3]);
                qInfo("Get Register slaveId: %x addr: %x", slaveId, addr);
                bool ret = ReadSensorReg(pSensor->SlaveID, (USHORT)addr, &value_1, pSensor->mode);
                qInfo("Read value result: %d value: %04x", ret, value_1);
                sensor_id.append(temp.sprintf("%04X", value_1));
            } else if (cmd[0].compare("delay", Qt::CaseInsensitive) == 0 || cmd[0].compare("sleep", Qt::CaseInsensitive) == 0){
                uint time = cmd[1].toInt();
                Sleep(time);
                qInfo("Sleep %d", time);
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
                    if (cmd_list[3].compare("YY_H", Qt::CaseInsensitive) == 0){
                        value = year_MSB;
                    } else if (cmd_list[3].compare("YY_L", Qt::CaseInsensitive) == 0){
                        value = year_LSB;
                    } else if (cmd_list[3].compare("MM", Qt::CaseInsensitive) == 0) {
                        value = date.month();
                    } else if (cmd_list[3].compare("DD", Qt::CaseInsensitive) == 0) {
                        value = date.day();
                    } else if (cmd_list[3].compare("HH", Qt::CaseInsensitive) == 0) {
                        value = time.hour();
                    } else if (cmd_list[3].compare("MI", Qt::CaseInsensitive) == 0) {
                        value = time.minute();
                    } else if (cmd_list[3].compare("SS", Qt::CaseInsensitive) == 0) {
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
    int iDevID = 0;
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

    SetGrabTimeout(8000, iDevID);

    if ((CalibrateSensorPort(8000, iDevID)) == DT_ERROR_TIME_OUT)
    {
        qWarning("Calibrate timeout");
    }

    int ret = GrabFrame(CameraBuffer, nSize, &retSize, &frameInfo, iDevID);
    if (ret == DT_ERROR_OK)
    {
        GetMipiCrcErrorCount(&crcCount, CHANNEL_A, iDevID);
    } else {
        qWarning("Fail....");
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
