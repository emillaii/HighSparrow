#include "visionserver.h"
#include <QImage>

VisionServer::VisionServer(BaslerPylonCamera *downlookCamera, BaslerPylonCamera * uplookCamera,
                           BaslerPylonCamera* pickarmCamera, BaslerPylonCamera * aa2DownlookCamera,
                           BaslerPylonCamera* sensorPickarmCamera, BaslerPylonCamera* sensorUplookCamera,
                           BaslerPylonCamera* barcodeCamera) : SilicoolVisionSimpleSource ()
{
    this->downlookCamera = downlookCamera;
    this->uplookCamera = uplookCamera;
    this->pickarmCamera = pickarmCamera;
    this->aa2DownlookCamera = aa2DownlookCamera;
    this->sensorPickarmCamera = sensorPickarmCamera;
    this->sensorUplookCamera = sensorUplookCamera;
    this->barcodeCamera = barcodeCamera;
}

QByteArray VisionServer::getImageEx(QString cameraName)
{
    qDebug() << "getImage" << cameraName;
    BaslerPylonCamera *camera = Q_NULLPTR;
    if (cameraName.contains(DOWNLOOK_VISION_CAMERA)) { camera = downlookCamera; }
    else if (cameraName.contains(UPLOOK_VISION_CAMERA)) { camera = uplookCamera; }
    else if (cameraName.contains(PICKARM_VISION_CAMERA)) { camera = pickarmCamera; }
    else if (cameraName.contains(CAMERA_AA2_DL)) { camera = aa2DownlookCamera; }
    else if (cameraName.contains(CAMERA_SPA_DL)) { camera = sensorPickarmCamera; }
    else if (cameraName.contains(CAMERA_LPA_UL)) { camera = sensorUplookCamera; }
    else if (cameraName.contains(CAMERA_LPA_BARCODE)) { camera = barcodeCamera; }
    QImage img = camera->getNewImage();
    setImgFormat(img.format());
    setWidth(img.width());
    setHeight(img.height());
    QByteArray buffer((char *)img.bits(), img.byteCount());
    return buffer;
}
