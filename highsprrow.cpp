#include "highsprrow.h"
#include <QDir>
#include <QProcess>
#include <qmessagebox.h>

HighSprrow::HighSprrow()
{
    if (!QDir(CONFIG_DIR).exists()) {QDir().mkdir(CONFIG_DIR);}
    if (!QDir(PR_CONFIG_DIR).exists()) {QDir().mkdir(PR_CONFIG_DIR);}
    if (!QDir(SYSTERM_PARAM_DIR).exists()) {QDir().mkdir(SYSTERM_PARAM_DIR);}
    worker_manager = new WorkersManager(this);
    baseModuleManager = new BaseModuleManager();
    baseModuleManager->loadProfile();
    logicManager = new LogicManager(baseModuleManager);
    baseModuleManager->registerWorkers(worker_manager);
    connect(logicManager,&LogicManager::sendMsgSignal,worker_manager,&WorkersManager::receiveMsgSignal,Qt::BlockingQueuedConnection);
    connect(&baseModuleManager->aaCoreNew, &AACoreNew::callQmlRefeshImg, this, &HighSprrow::receiveImageFromAACore);
}

HighSprrow::~HighSprrow()
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

