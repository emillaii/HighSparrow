#ifndef LOGICMANAGER_H
#define LOGICMANAGER_H

#include "basemodulemanager.h"

#include <QThread>
#include <aacore.h>
#include <unitlog.h>

class LogicManager : public QThread
{
    Q_OBJECT
public:
    explicit LogicManager(BaseModuleManager* device_manager, QObject *parent = nullptr);


    Q_PROPERTY(int currentMode READ currentMode WRITE setCurrentMode)
    Q_PROPERTY(QString stateMessage READ stateMessage WRITE setStateMessage)

    Q_INVOKABLE void updateParams();
    Q_INVOKABLE void loadFlowchart(QString);
    AACore * aaCore = Q_NULLPTR;

    void performMotionThread();

    Q_INVOKABLE void init();
    Q_INVOKABLE void home();
    Q_INVOKABLE void stopHome();
    Q_INVOKABLE void stop();

    Q_INVOKABLE void autoRun(QString);

    Q_INVOKABLE void aaMoveToMushroomPos();
    Q_INVOKABLE void aaMoveToPickLensPos();
    Q_INVOKABLE void aaMoveToOCPos();

    Q_INVOKABLE void sutMoveToMushroomPos();
    Q_INVOKABLE void sutMoveToPRPos();

    Q_INVOKABLE void lutMoveToLoadPos();
    Q_INVOKABLE void lutMoveToUnloadPos();
    Q_INVOKABLE void lutMoveToLoadUplookPos();
    Q_INVOKABLE void lutMoveToAA1UplookPos();
    Q_INVOKABLE void lutMoveToAA2UplookPos();

    Q_INVOKABLE void performChartCalibration();
    Q_INVOKABLE void performCalibration(QString calibration_name);
    Q_INVOKABLE void performLocation(QString location_name);

    Q_INVOKABLE void performOC();
    Q_INVOKABLE void performLoopTest(int mode);

    //lenpickarmModule
    Q_INVOKABLE void lensPickArmMoveToTray1Pos();
    Q_INVOKABLE void lensPickArmMoveToTray2Pos();
    Q_INVOKABLE void lensPickArmMoveToTray1StartPos();
    Q_INVOKABLE void lensPickArmMoveToTray2StartPos();
    Q_INVOKABLE void lensPickArmMoveToTray1EndPos();
    Q_INVOKABLE void lensPickArmMoveToLutPos1();
    Q_INVOKABLE void lensPickArmMoveToLutPos2();
    Q_INVOKABLE void lensPickArmMoveToPickLensFromTray1();
    Q_INVOKABLE void lensPickArmMoveToPickLensFromTray2();
    Q_INVOKABLE void lensPickArmMoveToPickLensFromLut();
    Q_INVOKABLE void lensPickArmMoveToPlaceLensToTray1();
    Q_INVOKABLE void lensPickArmMoveToPlaceLensToTray2();
    Q_INVOKABLE void lensPickArmMoveToPlaceLensToLut();
    Q_INVOKABLE void lensPickArmLensPR();//取
    Q_INVOKABLE void lensPickArmVacancyTrayPR();//放
    Q_INVOKABLE void lensPickArmLUTPR();//取
    Q_INVOKABLE void lensPickArmLensMeasureHeight();//tray lens测高
    Q_INVOKABLE void lensPickArmLUTMeasureHeight();//lut 测高
    Q_INVOKABLE void lensPickArmMoveToUplookCameraPos();
    Q_INVOKABLE void lensPickArmMoveToUplookPickerPos();

    int currentMode() const
    {
        return m_currentMode;
    }

    QString stateMessage() const
    {
        return m_stateMessage;
    }

public slots:
    void setCurrentMode(int currentMode)
    {
        m_currentMode = currentMode;
    }

    void setStateMessage(QString stateMessage)
    {
        m_stateMessage = stateMessage;
    }



private:
    BaseModuleManager * baseModuleManage;
    SfrWorkerController * sfrWorkerController = Q_NULLPTR;
    void moveToCmd(int);
    int m_currentMode;
    QString m_stateMessage;
    Unitlog unitlog;

    QString location_name;
    QString calibration_name;
protected:
    void run();
};

#endif // LOGICMANAGER_H
