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
    LUT_MOVETO_LOAD_UPLOOK_CMD,
    LUT_MOVETO_AA_UPLOOK_CMD,
    LUT_PICK_LENS_TO_AA_CMD,
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
    Q_INVOKABLE void lutMoveToLoadUplookPos();
    Q_INVOKABLE void lutMoveToAAUplookPos();

    Q_INVOKABLE void lutPickLensToAA();

    Q_INVOKABLE void performChartCalibration();
    Q_INVOKABLE void performCalibration(QString calibration_name);

    Q_INVOKABLE void performLocation(QString location_name);

    Q_INVOKABLE void performOC();
    Q_INVOKABLE void performLoopTest(int mode);

    //lens
    Q_INVOKABLE void pickArmMoveToLensTray1Pos();
    Q_INVOKABLE void pickArmMoveToLensTray2Pos();
    Q_INVOKABLE void pickArmMoveToLutPos();
    Q_INVOKABLE void pickArmMoveToPickLensFromTray1();
    Q_INVOKABLE void pickArmMoveToPickLensFromTray2();
    Q_INVOKABLE void pickArmMoveToPickLensFromLut();
    Q_INVOKABLE void pickArmMoveToPlaceLensToTray1();
    Q_INVOKABLE void pickArmMoveToPlaceLensToTray2();
    Q_INVOKABLE void pickArmMoveToPlaceLensToLut();
    //Sensor
    Q_INVOKABLE void pickArmMoveToSensorTrayPos();
    Q_INVOKABLE void pickArmMoveToRejectTrayPos();
    Q_INVOKABLE void pickArmMoveToSutPos();
    Q_INVOKABLE void pickArmMoveToPickSensorFromTray();
    Q_INVOKABLE void pickArmMoveToPickSensorFromSut();
    Q_INVOKABLE void pickArmMoveToPlaceSensorToTray();
    Q_INVOKABLE void pickArmMoveToPlaceSensorToSut();

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
