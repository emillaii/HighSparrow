#ifndef BASLERCAMERA_H
#define BASLERCAMERA_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QMutex>
#include <pylon/PylonIncludes.h>
#include <config.h>
#include <QQuickImageProvider>

using namespace Pylon;

class BaslerPylonCamera : public QThread, CImageEventHandler, public QQuickImageProvider
{
    Q_OBJECT
public:
    BaslerPylonCamera(QString);
    ~BaslerPylonCamera();
    void Init();
    void Close();
    void Open();
    bool IsOpend();
    bool GrabImage();
    void CopyBufferToQImage(CGrabResultPtr, QImage&);
    QImage getImage();
    void toogleLiveView();
    bool isLiveOn();
    bool isCameraGrabbing();
    void switchCameraChannel();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

protected:
    void run() override;
    void OnImageGrabbed(CInstantCamera&, const CGrabResultPtr&) override;
private:
    CInstantCamera camera;
    QMutex mutex;
    bool isLiveView;
    bool isGrabbing;
    bool isReady;
    QString cameraChannelName;
    QImage latestImage;
signals:
    void imageChanged(QImage);
    void callQmlRefeshImg();
};

#endif // BASLERCAMERA_H
