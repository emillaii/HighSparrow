#include "highsprrow.h"
#include <QDir>

HighSprrow::HighSprrow()
{
    if (!QDir(CONFIG_DIR).exists()) {QDir().mkdir(CONFIG_DIR);}
    aaHeadModule = new AAHeadModule();
    baseModuleManager = new BaseModuleManager();
    logicManager = new LogicManager();
}

HighSprrow::~HighSprrow()
{

}

ErrorCodeStruct HighSprrow::InitComponent()
{
    ErrorCodeStruct ret = { OK, "" };
    return ret;
}

void HighSprrow::init()
{
    InitComponent();
}

void HighSprrow::home()
{
}

void HighSprrow::homeAllMotors()
{
    qInfo("Home All Motors");
}

