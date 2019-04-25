#ifndef TRAYCLIP_H
#define TRAYCLIP_H

#include "trayclipstandardsparameter.h"

#include <QObject>


class TrayClip : public QObject{
    Q_OBJECT
public:
    TrayClip();
    void Init();
    void calculateDelta();
    double getPosition(int column_index);
    int getCurrentIndex();
    double getCurrentPosition();
    ClipStandardsParameter standards_parameters;
};

#endif // TRAYCLIP_H
