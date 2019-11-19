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
    AA_MOVETO_MUSHROOM_CMD,
    PERFORM_CALIBRATION,
    PERFORM_LOCATION,
    PERFORM_OC,
};

class LogicManager : public QThread
{
    Q_OBJECT
public:
    explicit LogicManager(BaseModuleManager* device_manager, QObject *parent = nullptr);

    Q_PROPERTY(int currentMode READ currentMode WRITE setCurrentMode)
//    Q_PROPERTY(QString stateMessage READ stateMessage WRITE setStateMessage NOTIFY stateMessageChanged)
    Q_PROPERTY(QString stateMessage READ stateMessage WRITE setStateMessage)


    Q_INVOKABLE void updateParams();

    Q_INVOKABLE void init();
    Q_INVOKABLE void home();
    Q_INVOKABLE void stopHome();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void aaMoveToMushroomPos();

    Q_INVOKABLE void performChartCalibration();
    Q_INVOKABLE void performCalibration(QString calibration_name);

    Q_INVOKABLE void performLocation(QString location_name);

    Q_INVOKABLE void performOC();

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
signals:
//    void stateMessageChanged(QString stateMessage);
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
