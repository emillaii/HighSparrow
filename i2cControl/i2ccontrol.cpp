#include "i2ccontrol.h"

i2cControl::i2cControl(QObject *parent) : QObject(parent)
{
    hDLL = LoadLibrary(L"usbi2cio.dll");
    if( hDLL != NULL) {
        qInfo("usbi2cio.dll load successfully");
        GetDllVersion=(GETDLLVERSION)GetProcAddress(hDLL,"DAPI_GetDllVersion");
        OpenDeviceInstance=(OPENDEVICEINSTANCE)GetProcAddress(hDLL,"DAPI_OpenDeviceInstance");
        CloseDeviceInstance=(CLOSEDEVICEINSTANCE)GetProcAddress(hDLL,"DAPI_CloseDeviceInstance");
        DetectDevice=(DETECTDEVICE)GetProcAddress(hDLL,"DAPI_DetectDevice");
        GetDeviceCount=(GETDEVICECOUNT)GetProcAddress(hDLL,"DAPI_GetDeviceCount");
        GetDeviceInfo=(GETDEVICEINFO)GetProcAddress(hDLL,"DAPI_GetDeviceInfo");
        OpenDeviceBySerialId=(OPENDEVICEBYSERIALID)GetProcAddress(hDLL,"DAPI_OpenDeviceBySerialId");
        GetSerialId=(GETSERIALID)GetProcAddress(hDLL,"DAPI_GetSerialId");
        ConfigIoPorts=(CONFIGIOPORTS)GetProcAddress(hDLL,"DAPI_ConfigIoPorts");
        GetIoConfig=(GETIOCONFIG)GetProcAddress(hDLL,"DAPI_GetIoConfig");
        ReadIoPorts=(READIOPORTS)GetProcAddress(hDLL,"DAPI_ReadIoPorts");
        WriteIoPorts=(WRITEIOPORTS)GetProcAddress(hDLL,"DAPI_WriteIoPorts");
        ReadI2c=(READI2C)GetProcAddress(hDLL,"DAPI_ReadI2c");
        WriteI2c=(WRITEI2C)GetProcAddress(hDLL,"DAPI_WriteI2c");
        WORD version = GetDllVersion();
        qInfo("UsbI2cIo.dll version: %d", version);
    }else {
        qCritical("UsbI2cIo.dll load fail");
    }
}

bool i2cControl::readi2c()
{
    TransI2C.byTransType = I2C_TRANS_8ADR;
    LONG lReadCnt;
    // initialize I2C transaction structure
    TransI2C.wMemoryAddr = 0x00;
    TransI2C.bySlvDevAddr = 0x00;
    TransI2C.wCount = 1;
    for (int i = 0; i < 255; i++)
    {
        TransI2C.bySlvDevAddr++;
        lReadCnt = ReadI2c(hDevice[0], &TransI2C);
        if(lReadCnt == TransI2C.wCount) {
            qInfo("Read value[%d]: %x %x", i, TransI2C.Data[i], TransI2C.bySlvDevAddr);
        }
    }

    return true;
}

bool i2cControl::openDevice()
{
    int deviceCount = GetDeviceCount(LPSTR("UsbI2cIo"));
    qInfo("Device count: %d", deviceCount);
    if (deviceCount > 0)
    {
        if(deviceOnFlag)
            return true;
        //Try to open device
        hDevice[0]=OpenDeviceInstance(LPSTR("UsbI2ddcIo"), 0);
        if(hDevice == INVALID_HANDLE_VALUE) {
            qCritical("Cannot find UsbI2cIo");
        } else {
            deviceOnFlag = true;
            return true;
        }
    } else {
        qInfo("Cannot find UsbI2cIo device");
    }
    closeDevice();
    return false;
}

bool i2cControl::closeDevice()
{
    if(deviceOnFlag)
    {
        return CloseDeviceInstance(hDevice[0]);
        deviceOnFlag = false;
    }
    return true;
}

int i2cControl::vcm_move(int slaveId, int addr, int pos)
{
    qInfo("Slave Id: %x Address: %x vcm move to pos: %d", slaveId, addr, pos);
    bool ret = WriteReg(slaveId, addr, pos, I2C_WR_modes::I2CMODE_ADDR8_VALUE16);
    return ret;
}
BOOL i2cControl::WriteReg(unsigned int slaveId, unsigned int regAddr, unsigned int value, I2C_WR_modes mode)
{
    if(mode == I2CMODE_ADDR8_VALUE8)
       {
           TransI2C.byTransType = I2C_TRANS_8ADR;
           TransI2C.wCount = 1;      // number of bytes to write
           TransI2C.Data[0] = value;  // Data byte 0 value
       } else if (mode == I2CMODE_ADDR8_VALUE16) {
           TransI2C.byTransType = I2C_TRANS_8ADR;
           TransI2C.wCount = 2;      // number of bytes to write
           TransI2C.Data[1] = value;  // Data byte 0 value
           TransI2C.Data[0] = value>>8;
       } else if (mode == I2CMODE_ADDR16_VALUE8) {
           TransI2C.byTransType = I2C_TRANS_16ADR;
           TransI2C.wCount = 1;      // number of bytes to write
           TransI2C.Data[0] = value;
       }
       else if (mode == I2CMODE_ADDR16_VALUE16) {
           TransI2C.byTransType = I2C_TRANS_16ADR;
           TransI2C.wCount = 2;      // number of bytes to write
           TransI2C.Data[1] = value;  // Data byte 0 value
           TransI2C.Data[0] = value>>8;
       } else if (mode == I2CMODE_ADDR16_VALUE32) {
           TransI2C.byTransType = I2C_TRANS_16ADR;
           TransI2C.wCount = 4;      // number of bytes to write
           TransI2C.Data[3] = value;
           TransI2C.Data[2] = value>>8;
           TransI2C.Data[1] = value>>16;  // Data byte 0 value
           TransI2C.Data[0] = value>>24;
       }
       if (!deviceOnFlag) {
           qWarning("i2c device cannot find. Write I2C fail.");
           return false;
       }
       LONG lWriteCnt;
       TransI2C.wMemoryAddr = regAddr;
       TransI2C.bySlvDevAddr = slaveId;   // I2C device Id (R/W bit is handled by API function)
       lWriteCnt = WriteI2c(hDevice[0], &TransI2C);
       if(lWriteCnt == TransI2C.wCount) {
           qInfo("write i2c success TransI2C.bySlvDevAddr: %x %x" ,  TransI2C.bySlvDevAddr, TransI2C.wMemoryAddr);
       }
       else {
           qWarning("Fail to write the i2c: %d", lWriteCnt);
           return false;
       }
       return true;
}

BOOL i2cControl::WriteRegs(unsigned int slaveId, unsigned int regAddr, unsigned int length, unsigned int *value, I2C_WR_modes mode)
{
    if(mode == I2CMODE_ADDR8_VALUE8)
    {
        TransI2C.byTransType = I2C_TRANS_8ADR;
    } else if (mode == I2CMODE_ADDR8_VALUE16) {
        TransI2C.byTransType = I2C_TRANS_8ADR;
    } else if (mode == I2CMODE_ADDR16_VALUE8) {
        TransI2C.byTransType = I2C_TRANS_16ADR;
    } else if (mode == I2CMODE_ADDR16_VALUE16) {
        TransI2C.byTransType = I2C_TRANS_16ADR;
    } else if (mode == I2CMODE_ADDR16_VALUE32) {
        TransI2C.byTransType = I2C_TRANS_16ADR;
    }
    if (!deviceOnFlag) {
        qCritical("i2c device cannot find. Write I2C fail.");
        return false;
    }

    TransI2C.bySlvDevAddr = slaveId;
    int maxDataLen = 1088;  // I2C_TRANS.Data数组的长度
    int writedDataLen = 0;  // 已写入到I2C_TRANS.Data数组的长度
    int batchIndex = 0;
    LONG lWriteCnt; // I2C device Id (R/W bit is handled by API function)

    TransI2C.wMemoryAddr = regAddr;
    for(int i = 0; i < length; i++)
    {
        if(mode == I2CMODE_ADDR8_VALUE8 || mode == I2CMODE_ADDR16_VALUE8)
        {
            TransI2C.Data[writedDataLen] = value[i];
            //qInfo("[%d]: %04X %04X",i, TransI2C.Data[writedDataLen], value[i]);
            writedDataLen += 1;
        } else if (mode == I2CMODE_ADDR8_VALUE16 || mode == I2CMODE_ADDR16_VALUE16) {
            unsigned short tmpValue = value[i];
            memcpy(&TransI2C.Data[writedDataLen], &tmpValue, 2);
            if(NEED_CONVERT_ENDIAN)
            {
                swap(TransI2C.Data[writedDataLen], TransI2C.Data[writedDataLen + 1]);
            }
            writedDataLen += 2;
        }
        else if (mode == I2CMODE_ADDR16_VALUE32) {
            memcpy(&TransI2C.Data[writedDataLen], &value[i], 4);
            if(NEED_CONVERT_ENDIAN)
            {
                swap(TransI2C.Data[writedDataLen], TransI2C.Data[writedDataLen + 3]);
                swap(TransI2C.Data[writedDataLen + 1], TransI2C.Data[writedDataLen + 2]);
            }
            writedDataLen += 4;
        }

        if(writedDataLen == maxDataLen || i == length - 1)
        {
            TransI2C.wCount = writedDataLen;
            lWriteCnt = WriteI2c(hDevice[0], &TransI2C);
            const char* log = "Batch write i2c %s. Batch index: %d, Data len: %d, TransI2C.bySlvDevAddr: %x %x";
            if(lWriteCnt == TransI2C.wCount) {
                qInfo(log, "successful", batchIndex, TransI2C.wCount, TransI2C.bySlvDevAddr, TransI2C.wMemoryAddr);
                // clear data for next batch
                TransI2C.wMemoryAddr += maxDataLen;
                writedDataLen = 0;
                batchIndex ++;
            }
            else {
                qInfo("failed Batch index: %d Data len: %d slaveId: %04X regAddr: %04X", batchIndex, TransI2C.wCount, TransI2C.bySlvDevAddr, TransI2C.wMemoryAddr);
                return false;
            }
        }
    }
    return true;
}

BOOL i2cControl::ReadRegs(unsigned int slaveId, unsigned int regAddr, unsigned int length, unsigned int *value, I2C_WR_modes mode)
{
    int dataBits = 0;
    if(mode == I2CMODE_ADDR8_VALUE8)
    {
        TransI2C.byTransType = I2C_TRANS_8ADR;
        dataBits = 1;
    } else if (mode == I2CMODE_ADDR8_VALUE16) {
        TransI2C.byTransType = I2C_TRANS_8ADR;
        dataBits = 2;
    } else if (mode == I2CMODE_ADDR16_VALUE8) {
        TransI2C.byTransType = I2C_TRANS_16ADR;
        dataBits = 1;
    }
    else if (mode == I2CMODE_ADDR16_VALUE16) {
        TransI2C.byTransType = I2C_TRANS_16ADR;
        dataBits = 2;
    } else if (mode == I2CMODE_ADDR16_VALUE32) {
        TransI2C.byTransType = I2C_TRANS_16ADR;
        dataBits = 4;
    }
    if (!deviceOnFlag){
        qCritical("i2c device cannot find. Read I2C fail.");
        return false;
    }

    TransI2C.bySlvDevAddr = slaveId;
    TransI2C.wMemoryAddr = regAddr;
    int dataLenToBeRead = length * dataBits;
    int maxDataLen = 1088;  // I2C_TRANS.Data数组的长度
    int readDataLen = 0;  // 已从I2C_TRANS.Data数组读取的长度
    int batchIndex = 0;
    int valueIndex = 0;
    LONG lReadCnt;; // I2C device Id (R/W bit is handled by API function)
    // TransI2C.wCount = length;
    while (dataLenToBeRead > 0) {
        readDataLen = dataLenToBeRead >= maxDataLen ? maxDataLen : dataLenToBeRead;
        TransI2C.wCount = readDataLen;

        lReadCnt = ReadI2c(hDevice[0], &TransI2C);
        QString log = "Batch read i2c %s. Batch index: %d, Data len: %d, TransI2C.bySlvDevAddr: %x %x";
        if(lReadCnt == TransI2C.wCount) {
            qInfo(log.toStdString().c_str(), "successful", batchIndex, TransI2C.wCount, TransI2C.bySlvDevAddr, TransI2C.wMemoryAddr);

            int convertedDataLen = 0;
            while (convertedDataLen < readDataLen) {
                if(mode == I2CMODE_ADDR8_VALUE8 || mode == I2CMODE_ADDR16_VALUE8)
                {
                    value[valueIndex] = TransI2C.Data[convertedDataLen];
                    convertedDataLen += 1;
                } else if (mode == I2CMODE_ADDR8_VALUE16 || mode == I2CMODE_ADDR16_VALUE16) {
                    unsigned short tmpValue = 0;
                    if(NEED_CONVERT_ENDIAN)
                    {
                        swap(TransI2C.Data[convertedDataLen], TransI2C.Data[convertedDataLen + 1]);
                    }
                    memcpy(&tmpValue, &TransI2C.Data[convertedDataLen], 2);
                    value[valueIndex] = tmpValue;
                    convertedDataLen += 2;
                }
                else if (mode == I2CMODE_ADDR16_VALUE32) {
                    if(NEED_CONVERT_ENDIAN)
                    {
                        swap(TransI2C.Data[convertedDataLen], TransI2C.Data[convertedDataLen + 3]);
                        swap(TransI2C.Data[convertedDataLen + 1], TransI2C.Data[convertedDataLen + 2]);
                    }
                    value[valueIndex] = *((unsigned int*)(&TransI2C.Data[convertedDataLen]));
                    convertedDataLen += 4;
                }
                valueIndex ++;
            }

            dataLenToBeRead -= readDataLen;
            batchIndex++;
            TransI2C.wMemoryAddr += readDataLen;
        }
        else {
            qCritical(log.toStdString().c_str(), "failed", batchIndex, TransI2C.wCount, TransI2C.bySlvDevAddr, TransI2C.wMemoryAddr);
            return false;
        }
    }

    return true;
}
