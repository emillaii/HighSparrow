#include "imageGrabber\iniparser.h"
#include <fstream>
#include <algorithm>
std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

iniParser::iniParser(){}

iniParser::~iniParser(){}

void iniParser::SetIniFilename(std::string sFilename)
{
    this->sFilename = sFilename;
}

int iniParser::ReadIniData(std::string sSection, std::string sSectionKey, int nDefault)
{
    std::wstring stemp = s2ws(sSection);
    LPCWSTR lpcwstrSection = stemp.c_str();
    std::wstring stemp1 = s2ws(sSectionKey);
    LPCWSTR lpcwstrSectionKey = stemp1.c_str();
    std::wstring stemp2 = s2ws(sFilename);
    LPCWSTR lpcwstrFilename = stemp2.c_str();
    return GetPrivateProfileInt(lpcwstrSection, lpcwstrSectionKey, nDefault, lpcwstrFilename);
}

BOOL iniParser::GetI2CData(pSensorTab pSensor, QStringList &cmd_list, QStringList &otp_list)
{
    std::string sReg, sVal;
    std::string strTmp[10];
    int tmp = 0;
    int state = -1;
    int reg = 0;
    int val = 0;
    int val_char_len = 4;
    if((pSensor->mode%2)==0)
        val_char_len = 6;
    USHORT *paraList;
    USHORT paraListSize = 0;
    USHORT sleepParaListSize = 0;
    USHORT af_InitParaListSize = 0;
    USHORT afAutoParaListSize = 0;
    USHORT af_FarParaListSize = 0;
    USHORT af_NearParaListSize = 0;
    USHORT exposure_ParaListSize = 0;
    USHORT gain_ParaListSize = 0;
    paraList = pSensor->ParaList;
    paraListSize = pSensor->ParaListSize;
    strTmp[0] = "[ParaList]";
    strTmp[1] = "[SleepParaList]";
    strTmp[2] = "[AF_InitParaList]";
    strTmp[3] = "[AFAutoParaList]";
    strTmp[4] = "[AF_FarParaList]";
    strTmp[5] = "[AF_NearParaList]";
    strTmp[6] = "[Exposure_ParaList]";
    strTmp[7] = "[Gain_ParaList]";
    strTmp[8] = "[SensorID_ParaList]";
    strTmp[9] = "[OTP_ParaList]";

    for (int i = 0; i < 10; i++)
    {
        std::transform(strTmp[i].begin(), strTmp[i].end(), strTmp[i].begin(), ::tolower);
    }
    std::string line;
    std::ifstream fs(sFilename);
    if (fs.is_open())
    {
        while (getline(fs, line))
        {
            tmp = (int)line.find("//");
            if (tmp == 0)
            {
                continue;
            }
            else if (tmp > 0)
            {
                line = line.substr(0, tmp);
            }
            tmp = (int)line.find("]");
            if (tmp == 0)
            {
                continue;
            }
            else if (tmp > 0)
            {
                line = line.substr(0, tmp + 1);
            }
            //printf("%s\r\n", line.c_str());
            std::transform(line.begin(), line.end(), line.begin(), ::tolower);
            std::remove(line.begin(), line.end(), ' ');
            if (line.length() == 0) continue;
            if (line == strTmp[0])
            {
                state = 0;
                paraListSize = 0;
                continue;
            }
            else if (line == strTmp[1])
            {
                state = 1;
                sleepParaListSize = 0;
                continue;
            }
            else if (line == strTmp[2])
            {
                state = 2;
                af_InitParaListSize = 0;
                continue;
            }
            else if (line == strTmp[3])
            {
                state = 3;
                afAutoParaListSize = 0;
                continue;
            }
            else if (line == strTmp[4])
            {
                state = 4;
                af_FarParaListSize = 0;
                continue;
            }
            else if (line == strTmp[5])
            {
                state = 5;
                af_NearParaListSize = 0;
                continue;
            }
            else if (line == strTmp[6])
            {
                state = 6;
                exposure_ParaListSize = 0;
                continue;
            }
            else if (line == strTmp[7])
            {
                state = 7;
                gain_ParaListSize = 0;
                continue;
            }

            else if (line == strTmp[8])
            {
                state = 8;
                gain_ParaListSize = 0;
                continue;
            }
            else if (line == strTmp[9])
            {
                state = 9;
                gain_ParaListSize = 0;
                continue;
            }

            if (state == 0)
            {
                int tmp_1 = (int)line.find(",", 0);
                sReg = line.substr(0, tmp_1);
                sVal = line.substr(tmp_1 + 1, val_char_len);
                if (!sscanf_s(sReg.c_str(), "0x%x", &reg))
                    sscanf_s(sReg.c_str(), "%d", &reg);
                if (!sscanf_s(sVal.c_str(), "0x%x", &val))
                    sscanf_s(sVal.c_str(), "%d", &val);
                *(paraList + paraListSize) = reg;
                *(paraList + paraListSize + 1) = val;
                paraListSize += 2;
                //printf("Paralist: %s Reg: %s Val: %s\r\n", line.c_str(), sReg.c_str(), sVal.c_str());
            }

            if (state == 8)
            {
                cmd_list.append(line.c_str());
            }

            if (state == 9)
            {
                otp_list.append(line.c_str());
            }
        }
        fs.close();
    }
    pSensor->ParaListSize = paraListSize;
    return true;
}
