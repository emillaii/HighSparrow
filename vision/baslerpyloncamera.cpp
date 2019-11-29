#include "vision/baslerpyloncamera.h"
#include <QElapsedTimer>
#include <QPixmap>
using namespace Pylon;
using namespace GenApi;
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
    this->setiIsGrabbing(true);
    trig_mutex.unlock();
    emit callQmlRefeshImg();
}

void BaslerPylonCamera::setCamera(BaslerPylonCamera::BaslerCameraControl_Type index, double tmpValue)
{
    INodeMap &cameraNodeMap = camera.GetNodeMap();
    switch (index) {
    case Type_Basler_Freerun: {
        CEnumerationPtr  ptrTriggerSel = cameraNodeMap.GetNode ("TriggerSelector");
        ptrTriggerSel->FromString("FrameStart");
        CEnumerationPtr  ptrTrigger  = cameraNodeMap.GetNode ("TriggerMode");
        #ifdef Real_Freerun
                ptrTrigger->SetIntValue(0);
        #else //Software
                ptrTrigger->SetIntValue(1);
                CEnumerationPtr  ptrTriggerSource = cameraNodeMap.GetNode ("TriggerSource");
                ptrTriggerSource->FromString("Software");
        #endif
    } break;
    case Type_Basler_Line1: {
        CEnumerationPtr  ptrTriggerSel = cameraNodeMap.GetNode ("TriggerSelector");
        ptrTriggerSel->FromString("FrameStart");
        CEnumerationPtr  ptrTrigger  = cameraNodeMap.GetNode ("TriggerMode");
        ptrTrigger->SetIntValue(1);
        CEnumerationPtr  ptrTriggerSource = cameraNodeMap.GetNode ("TriggerSource");
        ptrTriggerSource->FromString("Line1");
    } break;
    case Type_Basler_ExposureTimeAbs: {
        const CFloatPtr exposureTime = cameraNodeMap.GetNode("ExposureTimeAbs");
        exposureTime->SetValue(tmpValue);
    } break;
    case Type_Basler_GainRaw: {
        const CIntegerPtr cameraGen = cameraNodeMap.GetNode("GainRaw");
        cameraGen->SetValue(tmpValue);
    } break;
    case Type_Basler_AcquisitionFrameRateAbs: {
        const CBooleanPtr frameRate = cameraNodeMap.GetNode("AcquisitionFrameRateEnable");
        frameRate->SetValue(TRUE);
        const CFloatPtr frameRateABS = cameraNodeMap.GetNode("AcquisitionFrameRateAbs");
        frameRateABS->SetValue(tmpValue);
    } break;
    case Type_Basler_Width: {
        const CIntegerPtr widthPic = cameraNodeMap.GetNode("Width");
        widthPic->SetValue(tmpValue);
    } break;
    case Type_Basler_Height: {
        const CIntegerPtr heightPic = cameraNodeMap.GetNode("Height");
        heightPic->SetValue(tmpValue);
    } break;
    case Type_Basler_LineSource: {
        CEnumerationPtr  ptrLineSource = cameraNodeMap.GetNode ("LineSource");
        ptrLineSource->SetIntValue(2);
    } break;
    default:
        break;
    }
}

double BaslerPylonCamera::getCameraParam(BaslerPylonCamera::BaslerCameraControl_Type index)
{
    INodeMap &cameraNodeMap = camera.GetNodeMap();
    switch (index) {
    case Type_Basler_ExposureTimeAbs: {
        const CFloatPtr exposureTime = cameraNodeMap.GetNode("ExposureTimeAbs");
        return exposureTime->GetValue();
    } break;
    case Type_Basler_GainRaw: {
        const CIntegerPtr cameraGen = cameraNodeMap.GetNode("GainRaw");
        return cameraGen->GetValue();
    } break;
    case Type_Basler_AcquisitionFrameRateAbs: {
        const CBooleanPtr frameRate = cameraNodeMap.GetNode("AcquisitionFrameRateEnable");
        frameRate->SetValue(TRUE);
        const CFloatPtr frameRateABS = cameraNodeMap.GetNode("AcquisitionFrameRateAbs");
        return frameRateABS->GetValue();
    } break;
    case Type_Basler_Width: {
        const CIntegerPtr widthPic = cameraNodeMap.GetNode("Width");
        return widthPic->GetValue();
    } break;
    case Type_Basler_Height: {
        const CIntegerPtr heightPic = cameraNodeMap.GetNode("Height");
        return heightPic->GetValue();
    } break;
    default:
        return -1;
        break;
    }
}

void BaslerPylonCamera::setFeatureTriggerSourceType(QString type)
{
    if(type == "Freerun") {
        setCamera(Type_Basler_Freerun);
    } else if(type == "Line1"){
        setCamera(Type_Basler_Line1);
    }
}

QString BaslerPylonCamera::getFeatureTriggerSourceType()
{
    INodeMap &cameraNodeMap = camera.GetNodeMap();
    CEnumerationPtr  ptrTriggerSel = cameraNodeMap.GetNode("TriggerSelector");
    ptrTriggerSel->FromString("FrameStart");
    CEnumerationPtr  ptrTrigger  = cameraNodeMap.GetNode("TriggerMode");
    ptrTrigger->SetIntValue(1);
    CEnumerationPtr  ptrTriggerSource = cameraNodeMap.GetNode("TriggerSource");
    String_t str = ptrTriggerSource->ToString();
    m_currentMode = QString::fromLocal8Bit(str.c_str());
    double exposureTime = getCameraParam(Type_Basler_ExposureTimeAbs);
    qInfo("Camera : %s running in %s mode, exposureTime: %f", cameraChannelName.toStdString().c_str(), m_currentMode.toStdString().c_str(), exposureTime);
    return m_currentMode;
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
                imageHandler = new CSampleImageEventHandler(this);
                camera.RegisterImageEventHandler(imageHandler, RegistrationMode_Append, Cleanup_Delete);
                camera.Open();
//                if (cameraChannelName.contains(CAMERA_AA1_DL) || cameraChannelName.contains(CAMERA_AA2_DL)) {
//                    setFeatureTriggerSourceType("Line1");
//                } else {
                   camera.RegisterConfiguration( new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);
                   setFeatureTriggerSourceType("Freerun");
//                }
                getFeatureTriggerSourceType();
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
    //camera.DestroyDevice();
}

void BaslerPylonCamera::open()
{
    qInfo("Camera open");
    bool ret = Init();
    if (!ret) return;
    if((!camera.IsOpen())&&(!isGrabbing()))
    {
       camera.Open();
       getFeatureTriggerSourceType();
       this->start();
    } else this->start();
    setIsPauseLiveView(false);
}

void BaslerPylonCamera::setCameraExposureTime(double value)
{
    qInfo("Set Camera %s Exposure Time: %f", cameraChannelName.toStdString().c_str(), value);
    setCamera(Type_Basler_ExposureTimeAbs, value);
    getFeatureTriggerSourceType();
}

bool BaslerPylonCamera::IsOpend()
{
    return camera.IsOpen();
}

void BaslerPylonCamera::run(){
    if (camera.CanWaitForFrameTriggerReady())
    {
        if(m_currentMode == "Freerun")  {
            camera.StartGrabbing(GrabStrategy_LatestImageOnly,GrabLoop_ProvidedByInstantCamera);
        } else if(m_currentMode == "Software") {
            camera.StartGrabbing(GrabStrategy_LatestImageOnly,GrabLoop_ProvidedByInstantCamera);
        } else if(m_currentMode == "Line1") {
            camera.StartGrabbing(GrabStrategy_OneByOne,GrabLoop_ProvidedByInstantCamera);
            return;
        }
        isReady = true;
        setiIsGrabbing(true);
        while(isReady) {
            if (m_isPauseLiveView) {
                QThread::msleep(1000);
            } else {
                if (GrabImage())
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

bool BaslerPylonCamera::isCameraGrabbing() {
    if (m_currentMode == "Line") return true;
    return isGrabbing();
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

QImage BaslerPylonCamera::getNewImage()
{
    if (m_currentMode == "Line1") {  //If the camera is set to hardware trigger mode, return the latest image.
       return this->getImage();
    }
    bool already_trig,has_new_img;
    trig_mutex.lock();
    already_trig = is_triged;
    if(!already_trig)
        got_new = false;
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

void BaslerPylonCamera::pauseLiveView(bool pause)
{
    qInfo("Set pause live view: %d", pause);
    setIsPauseLiveView(pause);
    if (m_isPauseLiveView) {
        emit cameraPauseEvent();
    }
}

void BaslerPylonCamera::toggleLiveView()
{
    setIsPauseLiveView(!this->m_isPauseLiveView);
    qInfo("Pause live view: %d", m_isPauseLiveView);
    if (m_isPauseLiveView) {
        emit cameraPauseEvent();
    }
}
