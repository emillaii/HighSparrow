#ifndef IMAGEGRABBINGWORKERTHREAD_H
#define IMAGEGRABBINGWORKERTHREAD_H
#include <QThread>
#include <QImage>
#include "ImageGrabber/dothinkey.h"
#include <QMutex>
#include "Utils/imageprovider.h"

class ImageGrabbingWorkerThread : public QThread
{
    Q_OBJECT
public:
    ImageGrabbingWorkerThread(Dothinkey* dk, QObject *parent = 0);
    void stop();
    static QImage cvMat2QImage(const cv::Mat& mat);
    ImageProvider *m_pImgProvider;
    Q_INVOKABLE void saveImage();
protected:
    void run() override;
    bool forceStop;
    int mtf_test_count;
private:
    Dothinkey* dk;
    int index;
    QString resultData;
    QMutex mutex;
    QImage latestImage;
signals:
    void callQmlRefeshImg();
};

#endif // IMAGEGRABBINGWORKERTHREAD_H
