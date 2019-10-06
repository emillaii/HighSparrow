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
    int getLayerIndex(int layer_index);
    int getCurrentIndex();
    double getCurrentPosition();
    bool updateCurrentIndex();
    void reset();
    ClipStandardsParameter standards_parameters;
};

#endif // TRAYCLIP_H
