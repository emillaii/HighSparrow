#ifndef VISIONSERVER_H
#define VISIONSERVER_H

#include <QObject>
#include "./rep_vision_source.h"
#include "vision/baslerpyloncamera.h"

class VisionServer : public SilicoolVisionSimpleSource
{
    Q_OBJECT
public:
    explicit VisionServer(BaslerPylonCamera *downlookCamera, BaslerPylonCamera * uplookCamera,
                          BaslerPylonCamera* pickarmCamera, BaslerPylonCamera * aa2DownlookCamera,
                          BaslerPylonCamera* sensorPickarmCamera, BaslerPylonCamera* sensorUplookCamera,
                          BaslerPylonCamera* barcodeCamera);

signals:

public slots:
    virtual QByteArray getImageEx(QString cameraName) override;
    virtual QByteArray content() const override{
        return this->m_content;
    };
private:
    QByteArray m_content;
    BaslerPylonCamera *downlookCamera = nullptr;
    BaslerPylonCamera * uplookCamera = nullptr;
    BaslerPylonCamera* pickarmCamera = nullptr;
    BaslerPylonCamera * aa2DownlookCamera = nullptr;
    BaslerPylonCamera* sensorPickarmCamera = nullptr;
    BaslerPylonCamera* sensorUplookCamera = nullptr;
    BaslerPylonCamera* barcodeCamera = nullptr;
};

#endif // VISIONSERVER_H
