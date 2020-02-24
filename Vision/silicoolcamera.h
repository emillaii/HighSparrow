#ifndef SILICOOLCAMERA_H
#define SILICOOLCAMERA_H

#include <QObject>
#include <QImage>
#include <QThread>
#include <QQuickImageProvider>

#define VisionDir "vision//"

class SilicoolCamera : public QThread {
    Q_OBJECT
    Q_PROPERTY(QString cameraName READ cameraName WRITE setCameraName NOTIFY cameraNameChanged)
    Q_PROPERTY(double fps READ fps WRITE setFPS NOTIFY fpsChanged)
    QString m_cameraName;

double m_fps = 0;

public:
    enum SCReturnCode{
        SC_OK = 0,
        SC_CANNOT_FIND_DEVICE,
        SC_INIT_CAMERA_FAIL,
        SC_GRAB_FAIL,
        SC_ERROR
    };
public:
    virtual SCReturnCode SCCameraInit(QString cameraName) = 0;
    virtual SCReturnCode SCGetImage(QImage &outputImage) = 0;
    virtual SCReturnCode SCCameraStop() = 0;
    Q_INVOKABLE virtual bool scSaveImage() = 0;
    QString cameraName() const
    {
        return m_cameraName;
    }
    double fps() const
    {
        return m_fps;
    }

    public slots:
    void setCameraName(QString cameraName)
    {
        if (m_cameraName == cameraName)
            return;

        m_cameraName = cameraName;
        emit cameraNameChanged(m_cameraName);
    }
    void setFPS(double fps)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_fps, fps))
            return;

        m_fps = fps;
        emit fpsChanged(m_fps);
    }

    signals:
    void cameraNameChanged(QString cameraName);
    void fpsChanged(double fps);
    };

#endif // SILICOOLCAMERA_H
