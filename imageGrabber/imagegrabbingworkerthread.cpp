#include "imageGrabber\imagegrabbingworkerthread.h"
#include <Qmutex>
#include "sfr.h"
#include <QFileDialog>
#include <QTextStream>
#include "utils/commonutils.h"
QImage ImageGrabbingWorkerThread::cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        return QImage();
    }
}

ImageGrabbingWorkerThread::ImageGrabbingWorkerThread(Dothinkey* dk, QObject *)
    : forceStop(false)
{
    this->dk = dk;
    m_pImgProvider = new ImageProvider();
}

void ImageGrabbingWorkerThread::run()
{
    qInfo("Start thread");
    forceStop = false;
    while(true) {
        if(this->forceStop) break;
        QMutexLocker locker(&mutex);
        QImage* newImage =  dk->DothinkeyGrabImage(0);
        latestImage = newImage->copy();
        delete newImage;
        latestImage = latestImage.scaled(720,480);
        m_pImgProvider->setImage(latestImage);
        emit callQmlRefeshImg();
        locker.unlock();
        QThread::msleep(200); //Slow down the cpu cooldown
    }
}

void ImageGrabbingWorkerThread::stop()
{
    forceStop = true;
}

void ImageGrabbingWorkerThread::toggleMTFLive(int count)
{
    mtf_live = true;
    mtf_test_count = count;
    resultData = "";
    index = 0;
}

void ImageGrabbingWorkerThread::saveImage()
{
    if (!this->isRunning())
    {
        qInfo("Image Grabber is OFF");
        return;
    }
    QMutexLocker locker(&mutex);
    QString imageName;
    imageName.append(getGrabberLogDir())
                    .append(getCurrentTimeString())
                    .append(".bmp");
    QImage* newImage =  dk->DothinkeyGrabImage(0);
    newImage->save(imageName);
    locker.unlock();
}
