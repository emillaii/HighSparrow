#ifndef LOGICMANAGER_H
#define LOGICMANAGER_H

#include <QThread>
#include <aacore.h>

class LogicManager : public QThread
{
    Q_OBJECT
public:
    explicit LogicManager(QObject *parent = nullptr);
    Q_INVOKABLE void loadFlowchart(QString);
private:
    SfrWorkerController * sfrWorkerController = Q_NULLPTR;
    AACore * aaCore = Q_NULLPTR;

protected:
    void run();
};

#endif // LOGICMANAGER_H
