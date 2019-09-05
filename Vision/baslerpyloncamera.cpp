#include "Vision/baslerpyloncamera.h"
#include <QElapsedTimer>
#include <QPixmap>
BaslerPylonCamera::BaslerPylonCamera(QString name)
    : QQuickImageProvider(QQuickImageProvider::Image),
      cameraChannelName(name)
{
    PylonInitialize();
    Init();
}

void BaslerPylonCamera::updateImage(const CGrabResultPtr& ptrGrabResult)
{
    if (ptrGrabResult->HasCRC()) {
        if (!ptrGrabResult->CheckCRC()) {
            uint32_t errCode = ptrGrabResult->GetErrorCode();
            Pylon::String_t errDes = ptrGrabResult->GetErrorDescription();
            qCritical("Camera : %s CRC check fail, image discard.", this->cameraChannelName.toStdString().c_str());
            qCritical("Error code: %d, Error Description: %s", errCode, errDes.c_str());
            return;
        }
    }
    QMutexLocker locker(&mutex);
    CopyBufferToQImage(ptrGrabResult, latestImage);
    trig_mutex.lock();
    is_triged = false;
    got_new = true;
    trig_mutex.unlock();
    emit callQmlRefeshImg();
}

BaslerPylonCamera::~BaslerPylonCamera()
{
    close();
}

bool BaslerPylonCamera::Init(){
    bool isFound = false;
    try {
        CTlFactory& tlFactory = CTlFactory::GetInstance();
        DeviceInfoList_t devices;
        if ( tlFactory.EnumerateDevices(devices) == 0 )
        {
            qCritical("No Camera Connected");
            emit noCameraEvent();
            return false;
        }
        for (size_t i=0; i < devices.size(); i++)
        {
            if (devices[i].GetUserDefinedName().compare(cameraChannelName.toStdString().c_str()) == 0)
            {
                isFound = true;
                camera.Attach(tlFactory.CreateDevice(devices[i]));
                camera.RegisterConfiguration( new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);
                imageHandler = new CSampleImageEventHandler(this);
                camera.RegisterImageEventHandler(imageHandler, RegistrationMode_Append, Cleanup_Delete);
                camera.Open();
            }
        }
    } catch (const GenericException &e){
        qCritical(e.GetDescription());
        emit noCameraEvent();
        return false;
    }
    if (!isFound) emit noCameraEvent();
    return isFound;
}

void BaslerPylonCamera::close()
{
    isReady = false;
    while(isGrabbing())
    {
        QThread::msleep(100);
    }
    qInfo("Grabbing stopped");
    if(camera.IsOpen())
    {
        qInfo("Camera close");
        camera.Close();
    }
    if (imageHandler != nullptr)
        camera.DeregisterImageEventHandler(imageHandler);
    camera.DestroyDevice();
}

void BaslerPylonCamera::open()
{
    bool ret = Init();
    if (!ret) return;
    if((!camera.IsOpen())&&(!isGrabbing()))
    {
       camera.Open();
       this->start();
    } else this->start();
}

bool BaslerPylonCamera::IsOpend()
{
    return camera.IsOpen();
}

void BaslerPylonCamera::run(){
    if (camera.CanWaitForFrameTriggerReady())
    {
        camera.StartGrabbing( GrabStrategy_LatestImageOnly, GrabLoop_ProvidedByInstantCamera);
        isReady = true;
        setiIsGrabbing(true);
        while(isReady&&GrabImage()) {
            {
                for (int cnt = 0;cnt<100;cnt++) {
                 QThread::msleep(1);
                 if(need_triged)
                 {
                     need_triged = false;
                     break;
                 }
                }
            }
        }
        setiIsGrabbing(false);
        return;
    }
}

bool BaslerPylonCamera::GrabImage()
{

    trig_mutex.lock();
    is_triged = true;
    got_new = false;
    trig_mutex.unlock();

    try {
        if ( camera.WaitForFrameTriggerReady( 1000, TimeoutHandling_ThrowException))
        {
             camera.ExecuteSoftwareTrigger();
        }
    } catch (const GenericException &e){
        qCritical(e.GetDescription());
        setiIsGrabbing(false);
        return false;
    }
    return true;
}

QImage BaslerPylonCamera::getImage()
{
    QMutexLocker locker(&mutex);
    QImage image_copy = latestImage.copy();
    return std::move(image_copy);
}

bool BaslerPylonCamera::isCameraGrabbing() { return isGrabbing(); }

void BaslerPylonCamera::CopyBufferToQImage(CGrabResultPtr pInBuffer, QImage& outImage)
{
    uchar* buff = (uchar*)pInBuffer->GetBuffer();
    int nHeight = pInBuffer->GetHeight();
    int nWidth = pInBuffer->GetWidth();
    QImage imgBuff(buff, nWidth, nHeight, QImage::Format_Indexed8);
    outImage = imgBuff;
    if(pInBuffer->GetPixelType() == PixelType_Mono8) {
        uchar* pCursor = outImage.bits();
        if ( outImage.bytesPerLine() != nWidth ) {
            for ( int y=0; y<nHeight; ++y ) {
                pCursor = outImage.scanLine( y );
                for ( int x=0; x<nWidth; ++x ) {
                    *pCursor =* buff;
                    ++pCursor;
                    ++buff;
                 }
            }
        } else {
            memcpy( outImage.bits(), buff, nWidth * nHeight );
        }
    }
}

QImage BaslerPylonCamera::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return this->getImage();
}

QImage BaslerPylonCamera::getNewImage()
{
    bool already_trig,has_new_img;
    trig_mutex.lock();
    already_trig = is_triged;
    trig_mutex.unlock();

    if(!already_trig)
        need_triged = true;
    for(int tim = 0; tim<500; tim++)
    {
        trig_mutex.lock();
        has_new_img = got_new;
        trig_mutex.unlock();
        if(has_new_img)
        {
            qInfo("getNewImage in %d", tim);
            return this->getImage();
        }
        QThread::msleep(1);
    }
    qInfo("getNewImage timeout, got old one");
    return this->getImage();
}

QString BaslerPylonCamera::getCameraChannelname()
{
    return this->cameraChannelName;
}
