#include "startcameradoe.h"

StartCameraDOE::StartCameraDOE(QObject *parent):
    QThread(parent), isRun(false), isStopped(true)
{

}

void StartCameraDOE::init(SingleheadLSutModule* lsut, SingleHeadMachineMaterialLoaderModule* materialLoader,
                          SingleHeadMachineMaterialPickArm* materialPickArm, MaterialTray* sensorTray,
                          Dothinkey*dothinKey)
{
    this->lsut = lsut;
    this->materialLoader = materialLoader;
    this->materialPickArm = materialPickArm;
    this->sensorTray = sensorTray;
    this->dothinKey = dothinKey;
}

void StartCameraDOE::startDOE(int meterialCount, int doeTimes)
{
    if(!isStopped)
    {
        return;
    }
    this->materialCount = meterialCount;
    this->doeTimes = doeTimes;
    isRun = true;
    start();
}

void StartCameraDOE::stopDOE()
{
    isRun = false;
}

void StartCameraDOE::run()
{
    isStopped = false;
    nPass = 0;
    nFail = 0;
    emit passFailCountChanged(nPass, nFail);
    try {
        lsut->moveToLoadSensorPosition();
        lsut->pogopin->Set(false);
        for(int i = 0; i < doeTimes; i++)
        {
            sensorTray->setTrayCurrent(0, 0);
            for(int i = 0; i < materialCount; i++)
            {
                if(i != 0)
                {
                    sensorTray->setTrayCurrent(sensorTray->getCurrentIndex() + 1, 0);
                }
                materialLoader->performHandlingOperation(SingleHeadMachineMaterialLoaderModule::SENSOR_TRAY|
                                                         SingleHeadMachineMaterialLoaderModule::SENSOR_PR|
                                                         SingleHeadMachineMaterialLoaderModule::PICKER2_TO_WORKPOS|
                                                         SingleHeadMachineMaterialLoaderModule::PICKER2_PICK_SENSOR_FROM_TRAY);
                checkStop();
                materialLoader->performHandlingOperation(SingleHeadMachineMaterialLoaderModule::SUT_PR_POS|
                                               SingleHeadMachineMaterialLoaderModule::SUT_PR|
                                               SingleHeadMachineMaterialLoaderModule::PICKER2_TO_WORKPOS|
                                               SingleHeadMachineMaterialLoaderModule::PICKER2_PLACE_SENSOR_TO_SUT);
                checkStop();
                lsut->pogopin->Set(true);
                if(dothinKey->DothinkeyEnum() && dothinKey->DothinkeyOpen() &&
                        dothinKey->DothinkeyLoadIniFile(0) && dothinKey->DothinkeyStartCamera(0))
                {
                    nPass++;
                }
                else {
                    nFail++;
                }
                emit passFailCountChanged(nPass, nFail);
                QThread::msleep(200);
                dothinKey->DothinkeyClose();
                lsut->pogopin->Set(false);
                checkStop();
                materialLoader->performHandlingOperation(SingleHeadMachineMaterialLoaderModule::SUT_PR_POS|
                                               SingleHeadMachineMaterialLoaderModule::NG_SENSOR_PR|
                                               SingleHeadMachineMaterialLoaderModule::PICKER2_TO_WORKPOS|
                                               SingleHeadMachineMaterialLoaderModule::PICKER2_PICK_NG_SENSOR_FROM_SUT);
                checkStop();
                materialLoader->performHandlingOperation(SingleHeadMachineMaterialLoaderModule::SENSOR_TRAY|
                                               SingleHeadMachineMaterialLoaderModule::SENSOR_VACANCY_PR|
                                               SingleHeadMachineMaterialLoaderModule::PICKER2_TO_WORKPOS|
                                               SingleHeadMachineMaterialLoaderModule::PICKER2_PLACE_SENSOR_TO_TRAY);
                checkStop();
            }

        }
    }
    catch(StopLoopException&)
    {

    }
    isStopped = true;
}

void StartCameraDOE::checkStop()
{
    if(!isRun)
    {
        throw StopLoopException();
    }
}

