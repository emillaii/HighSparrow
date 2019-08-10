#ifndef LOGICMANAGER_H
#define LOGICMANAGER_H

#include "basemodulemanager.h"

#include <QThread>
#include "Utils/unitlog.h"

enum CommandType{
    IDLE,
    STOP,
    MOTION_INIT,
    MOTION_HOME,
    MOTION_STOP_HOME,     // Interrupt
    MODE_AUTO_RUN,
    AA_MOVETO_MUSHROOM_CMD,
    AA_MOVETO_PICK_LENS_CMD,
    AA_MOVETO_OC_CMD,
    SUT_MOVETO_MUSHROOM_CMD,
    SUT_MOVETO_PR_CMD,
    LUT_MOVETO_LOAD_CMD,
    LUT_MOVETO_UNLOAD_CMD,
    LUT_MOVETO_LOAD_UPLOOK_CMD,
    LUT_MOVETO_AA1_UPLOOK_CMD,
    LUT_MOVETO_AA2_UPLOOK_CMD,
    LUT_PICK_LENS_TO_AA2_CMD,
    LUT_PICK_LENS_TO_AA1_CMD,
    PERFORM_CALIBRATION,
    PERFORM_LOCATION,
    PERFORM_OC,
    PERFORM_LOOP_TEST
};

class LogicManager : public QThread
{
    Q_OBJECT
public:
    explicit LogicManager(BaseModuleManager* device_manager, QObject *parent = nullptr);

    Q_PROPERTY(int currentMode READ currentMode WRITE setCurrentMode)
    Q_PROPERTY(QString stateMessage READ stateMessage WRITE setStateMessage NOTIFY stateMessageChanged)

    Q_INVOKABLE void updateParams();
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

    Q_INVOKABLE void lutPickLensToAA1();
    Q_INVOKABLE void lutPickLensToAA2();

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
    Q_INVOKABLE void lensPickArmMoveToUpdownlookDownPos();
    Q_INVOKABLE void lensPickArmMoveToUpdownlookUpPos();


    Q_INVOKABLE void sensorTrayLoaderModuleMovetoPushMotorSafePosotion();
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoDownTrayPosition();
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoGetTrayPosition();
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoTrayWorkPosition();
    Q_INVOKABLE void sensorTrayLoaderModuleMovetoVacancyTrayPosition();

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
    void receiveCommand(int cmd);
signals:
    void stateMessageChanged(QString stateMessage);
    bool sendMsgSignal(QString,QString);
private:
    BaseModuleManager * baseModuleManage;
    SfrWorkerController * sfrWorkerController = Q_NULLPTR;
    void moveToCmd(int);
    int m_currentMode;
    QString m_stateMessage;

    QString location_name;
    QString calibration_name;
protected:
    void run();
};

#endif // LOGICMANAGER_H
