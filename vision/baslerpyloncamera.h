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
    enum BaslerCameraControl_Type{
        Type_Basler_Freerun, //设置相机的内触发
        Type_Basler_Line1, //设置相机的外触发
        Type_Basler_ExposureTimeAbs, //设置相机的曝光时间
        Type_Basler_GainRaw, //设置相机的增益
        Type_Basler_AcquisitionFrameRateAbs, //设置相机的频率
        Type_Basler_Width, //图片的宽度
        Type_Basler_Height, //图片的高度
        Type_Basler_LineSource, //灯的触发信号
    };
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
    ~BaslerPylonCamera() override;
    bool Init();
    Q_INVOKABLE void close();
    Q_INVOKABLE void open();
    Q_INVOKABLE void setCameraExposureTime(double);
    Q_INVOKABLE void pauseLiveView(bool pause);
    Q_INVOKABLE void toggleLiveView();
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
    void setCamera(BaslerPylonCamera::BaslerCameraControl_Type index, double tmpValue = 0.0); // 设置各种参数
    double getCameraParam(BaslerPylonCamera::BaslerCameraControl_Type index); // 获取各种参数
    void setFeatureTriggerSourceType(QString type); // 设置种类
    QString getFeatureTriggerSourceType(); // 获取种类：软触发、外触发等
    void setIsPauseLiveView(bool isPauseLiveView)
    {
        m_isPauseLiveView = isPauseLiveView;
    }
public slots:
    void setiIsGrabbing(bool isGrabbing)
    {
        m_isGrabbing = isGrabbing;
        if (!isGrabbing) emit cameraCloseEvent();
    }

protected:
    void run() override;
private:
    CInstantCamera camera;
    QMutex mutex;
    QMutex trig_mutex;
    bool isReady;
    QString cameraChannelName;
    QImage latestImage;
    bool m_isPauseLiveView = false;
    bool m_isGrabbing = false;
    CSampleImageEventHandler *imageHandler;
    QString m_currentMode;
signals:
    void imageChanged(QImage);
    void callQmlRefeshImg();
    void noCameraEvent();
    void cameraCloseEvent();
    void cameraPauseEvent();
};

#endif // BASLERCAMERA_H
