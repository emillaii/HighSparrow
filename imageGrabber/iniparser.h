#ifndef INIPARSER_H
#define INIPARSER_H

#include <Windows.h>
#include <string>
#include "imagekit.h"
#include <QObject>
#define _SEARCH_DOTHINKEY_FAVORITES_
#define GETYFROMBGR(R, G, B) ((1225*(R) + 2404*(G) + 467*(B) + 2048)>>12)

//#define ROI_OUTPUT_ENABLE
#define ROI_OUTPUT_WIDTH   352
#define ROI_OUTPUT_HEIGHT  288

#define FOCUS_AREA_W 160
#define FOCUS_AREA_H 120
#define FOCUS_AREA_BORDER 3

#define EXP_GAIN_DATA_EMPTY 0xffffffff
#define FUNC_GROUP_MAX 20
#define FUNC_ELEMENT 5


enum RunState
{
    RUNSTATE_NORMAL = 0,
    RUNSTATE_ROI_B0,
    RUNSTATE_QUICK,
    RUNSTATE_FULL,
    RUNSTATE_AF,
    RUNSTATE_FAR,
    RUNSTATE_NEAR,
};

typedef struct _Exp_Gain_Tab
{
    UINT Exp_Range[2];
    UINT Gain_Range[2];

    UINT ExpReg[4];
    UINT GainReg[4];
    UINT ExpRegNum;
    UINT GainRegNum;

    UINT Func_ExpGain[FUNC_ELEMENT * FUNC_GROUP_MAX];
    _Exp_Gain_Tab()
    {
        ExpRegNum = 0;
        GainRegNum = 0;
        memset(ExpReg, EXP_GAIN_DATA_EMPTY, sizeof(ExpReg));
        memset(GainReg, EXP_GAIN_DATA_EMPTY, sizeof(GainReg));
        memset(Exp_Range, EXP_GAIN_DATA_EMPTY, sizeof(Exp_Range));
        memset(Gain_Range, EXP_GAIN_DATA_EMPTY, sizeof(Gain_Range));
        memset(Func_ExpGain, EXP_GAIN_DATA_EMPTY, sizeof(Func_ExpGain));

    }
}Exp_Gain_Tab, *pExp_Gain_Tab;

class iniParser
{
public:
    iniParser();
    ~iniParser();
public:
    void SetIniFilename(std::string sFilename);
    int ReadIniData(std::string sSection, std::string sSectionKey, int nDefault);
    BOOL GetI2CData(pSensorTab pSensor, QStringList &cmd_list, QStringList &otp_list);
protected:
    std::string sFilename;
};

#endif // INIPARSER_H
