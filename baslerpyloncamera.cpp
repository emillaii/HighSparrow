#include "baslerpyloncamera.h"
#include <QElapsedTimer>
#include <QPixmap>
BaslerPylonCamera::BaslerPylonCamera(QString name)
    : QQuickImageProvider(QQuickImageProvider::Image),
      cameraChannelName(name)
{
    name.compare(DOWNLOOK_VISION_CAMERA) == 0 ?  isLiveView = true : isLiveView = false;
    isGrabbing = false;
    PylonInitialize();
    Init();
}

void BaslerPylonCamera::OnImageGrabbed( CInstantCamera&, const CGrabResultPtr& ptrGrabResult)
{
   QMutexLocker locker(&mutex);
   CopyBufferToQImage(ptrGrabResult, latestImage);
   emit callQmlRefeshImg();
   if (isLiveView) {
       imageChanged(latestImage);
   }
}

BaslerPylonCamera::~BaslerPylonCamera()
{
    if (camera.IsOpen()) camera.Close();
    this->wait();
}

void BaslerPylonCamera::Init(){
    try {
        CTlFactory& tlFactory = CTlFactory::GetInstance();
        DeviceInfoList_t devices;
        if ( tlFactory.EnumerateDevices(devices) == 0 )
        {
            qCritical("No Camera Connected");
            return;
        }
        for (size_t i=0; i < devices.size(); i++)
        {
            if (devices[i].GetUserDefinedName().compare(cameraChannelName.toStdString().c_str()) == 0)
            {
                camera.Attach(tlFactory.CreateDevice(devices[i]));
                camera.RegisterConfiguration( new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);
                camera.RegisterImageEventHandler(this, RegistrationMode_Append, Cleanup_Delete);
                camera.Open();
            }
        }
    } catch (const GenericException &e){
        qCritical(e.GetDescription());
    }
}

void BaslerPylonCamera::Close()
{
    isReady = false;
    while(isGrabbing)
    {
        QThread::msleep(100);
    }
    if(camera.IsOpen())
    {
        camera.Close();
    }
}

void BaslerPylonCamera::Open()
{
    if((!camera.IsOpen())&&(!isGrabbing))
    {
       camera.Open();
        this->start();
    }
}

bool BaslerPylonCamera::IsOpend()
{
    return camera.IsOpen();
}

void BaslerPylonCamera::run(){
    if (camera.CanWaitForFrameTriggerReady())
    {
        camera.StartGrabbing( GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera);
        isReady = true;
        while(isReady&&GrabImage()) {
            {
                isGrabbing = true;
                QThread::msleep(100);
            }
            isGrabbing = false;
        }
    }
}

bool BaslerPylonCamera::GrabImage()
{
    try {
        if ( camera.WaitForFrameTriggerReady( 500, TimeoutHandling_ThrowException))
        {
             camera.ExecuteSoftwareTrigger();
        }
    } catch (const GenericException &e){
        qCritical(e.GetDescription());
        return false;
    }
    return true;
}

QImage BaslerPylonCamera::getImage()
{
    QMutexLocker locker(&mutex);
    QImage image_copy = latestImage.copy();
    return image_copy;
}

bool BaslerPylonCamera::isLiveOn() { return isLiveView; }

bool BaslerPylonCamera::isCameraGrabbing() { return isGrabbing; }

void BaslerPylonCamera::toogleLiveView()
{
     QMutexLocker locker(&mutex);
     isLiveView = !isLiveView;
}

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
