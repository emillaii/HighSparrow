#ifndef IMAGEGRABBINGWORKERTHREAD_H
#define IMAGEGRABBINGWORKERTHREAD_H
#include <QThread>
#include <QImage>
#include "dothinkey.h"
#include <QQuickImageProvider>
#include <QMutex>

class ImageGrabbingWorkerThread : public QThread, public QQuickImageProvider
{
    Q_OBJECT
public:
    ImageGrabbingWorkerThread(Dothinkey* dk, QObject *parent = 0);
    void stop();
    void toggleMTFLive(int count);
    static QImage cvMat2QImage(const cv::Mat& mat);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

protected:
    void run() override;
    bool forceStop;
    bool mtf_live;
    int mtf_test_count;
private:
    Dothinkey* dk;
    int index;
    QString resultData;
    QMutex mutex;
    QImage latestImage;
signals:
    void done();
    void imageGrabbed();
    void callQmlRefeshSensorImg();
public slots:
    void onImageGrabbed();
};

#endif // IMAGEGRABBINGWORKERTHREAD_H
