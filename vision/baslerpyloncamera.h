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
    Q_PROPERTY(bool isGrabbing READ isGrabbing WRITE setiIsGrabbing)
    class CSampleImageEventHandler : public CImageEventHandler
    {
    private:
        BaslerPylonCamera *pylonCamera;
    public:
        CSampleImageEventHandler(BaslerPylonCamera *pylonCamera) {
            this->pylonCamera = pylonCamera;
        }
        virtual void OnImageGrabbed( CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult)
        {
            pylonCamera->updateImage(ptrGrabResult);
        }
    };
public:
    BaslerPylonCamera(QString);
    ~BaslerPylonCamera();
    bool Init();
    Q_INVOKABLE void close();
    Q_INVOKABLE void pauseLiveView(bool pause);
    Q_INVOKABLE void toggleLiveView();
    Q_INVOKABLE void open();
    bool IsOpend();
    bool GrabImage();
    void CopyBufferToQImage(CGrabResultPtr, QImage&);
    QImage getImage();
    bool isCameraGrabbing();
    void switchCameraChannel();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    QImage getNewImage();

    bool is_triged = false;
    bool got_new = false;
    bool need_triged = false;
    QString getCameraChannelname();
    void updateImage(const CGrabResultPtr& ptrGrabResult);
    bool isGrabbing() const
    {
        return m_isGrabbing;
    }

public slots:
    void setiIsGrabbing(bool isGrabbing)
    {
        m_isGrabbing = isGrabbing;
        if (!isGrabbing) emit cameraCloseEvent();
    }

    void setIsPauseLiveView(bool isPauseLiveView)
    {
        m_isPauseLiveView = isPauseLiveView;
    }

protected:
    void run() override;
private:
    CInstantCamera camera;
    QMutex mutex;
    QMutex trig_mutex;
    bool isReady;
    bool m_isPauseLiveView = false;
    QString cameraChannelName;
    QImage latestImage;
    bool m_isGrabbing = false;
    CSampleImageEventHandler *imageHandler;
signals:
    void imageChanged(QImage);
    void callQmlRefeshImg();
    void noCameraEvent();
    void cameraCloseEvent();
    void cameraPauseEvent();
};

#endif // BASLERCAMERA_H
