#include "highsprrow.h"
#include <QDir>
#include <QProcess>

HighSprrow::HighSprrow()
{
    if (!QDir(CONFIG_DIR).exists()) {QDir().mkdir(CONFIG_DIR);}
    baseModuleManager = new BaseModuleManager();
    baseModuleManager->LoadProfile();
    logicManager = new LogicManager();
//    aaHeadModule = new AAHeadModule(Q_NULLPTR,Q_NULLPTR,Q_NULLPTR,
//                                    Q_NULLPTR,Q_NULLPTR,Q_NULLPTR,
//                                    Q_NULLPTR);
//    sutModule = new SutModule(Q_NULLPTR, Q_NULLPTR, Q_NULLPTR,
//                              Q_NULLPTR, Q_NULLPTR, Q_NULLPTR,
//                              Q_NULLPTR, Q_NULLPTR);
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

void HighSprrow::callQProcess(QString arg)
{
    QStringList arguments;
    arguments << "/c" << arg.toStdString().c_str();
    QProcess process;
    process.setWorkingDirectory(QDir::currentPath());
    qInfo("Learn PR %s,current directory:%s", arg.toStdString().c_str(),QDir::currentPath().toStdString().c_str());
    process.startDetached("cmd.exe", arguments);
    process.waitForStarted();
}

void HighSprrow::homeAllMotors()
{
    qInfo("Home All Motors");
}

