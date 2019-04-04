#ifndef BASEMODULE_H
#define BASEMODULE_H

#include <QObject>

class BaseModule : public QObject
{
    Q_OBJECT
public:
    explicit BaseModule(QObject *parent = nullptr);
    ~BaseModule();

    void init();
    void homeMotors();
    void setVisionModule();


signals:

public slots:
};

#endif // BASEMODULE_H
