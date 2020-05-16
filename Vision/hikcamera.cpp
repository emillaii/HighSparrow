#include "hikcamera.h"
#include <QImage>

HIKCamera::HIKCamera()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
    m_hDevHandle = Q_NULLPTR;
}

QImage HIKCamera::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return this->getImage();
}

HIKCamera::~HIKCamera()
{
    if (m_hDevHandle)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = Q_NULLPTR;
    }
}

HIKCamera::SCReturnCode HIKCamera::SCCameraInit(QString ip)
{
    if (Open(ip) == MV_OK){
        return SC_OK;
    } else {
        return SC_INIT_CAMERA_FAIL;
    }
}

HIKCamera::SCReturnCode HIKCamera::SCCameraStop()
{
    if (Close() == MV_OK) {
        return SC_OK;
    } else {
        return SC_ERROR;
    }
}

bool HIKCamera::scSetExposureTime(float value){
   return MV_CC_SetFloatValue(m_hDevHandle, "ExposureTime", value);
}


bool HIKCamera::scSaveImage()
{
    QString imageName = "test.jpg";
//    imageName.append(getDir(VisionDir))
//            .append(this->cameraName())
//            .append("-")
//            .append(getCurrentDateTime2())
//            .append(".jpg");
    if (this->isGrabbing)
    {
        QImage image = this->getImage();
        image.save(imageName);
    } else {
        return false;
    }
    //UIOperation::getIns()->showMessage("title", imageName.append(" is saved"), MsgBoxIcon::Information, "OK");
    return true;
}

HIKCamera::SCReturnCode HIKCamera::SCGetImage(QImage &)
{
//    qCDebug(hikCamera()) <<  __FUNCTION__;
    return SC_OK;
}

int HIKCamera::EnumDevices(MV_CC_DEVICE_INFO_LIST *pstDevList)
{
    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, pstDevList);
    if (MV_OK != nRet)
    {
        return nRet;
    }
    return MV_OK;
}

int HIKCamera::Open(QString ip)
{
    QStringList server_octets = ip.split(".");
    int s1 = server_octets.at(0).toLong();
    int s2 = server_octets.at(1).toLong();
    int s3 = server_octets.at(2).toLong();
    int s4 = server_octets.at(3).toLong();
    int nRet = MV_OK;
    MV_CC_DEVICE_INFO_LIST m_stDevList;
    EnumDevices(&m_stDevList);
    for (size_t i = 0; i < m_stDevList.nDeviceNum; i++) {
        MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
        if (Q_NULLPTR == pDeviceInfo)
        {
            continue;
        }
        if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
        {
            int nIp1, nIp2, nIp3, nIp4;
            nIp1 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
            nIp2 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
            nIp3 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
            nIp4 = (m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

            wchar_t* pUserName = Q_NULLPTR;
            QString userDefinedName;
            memcpy(userDefinedName.data(),pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName,sizeof(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName));
            qInfo("userDefinedName: %s", userDefinedName.toStdString().c_str());
            QString string = QString("[%1]GigE (%2.%3.%4.%5)").arg(QString::number(i), QString::number(nIp1), QString::number(nIp2), QString::number(nIp3), QString::number(nIp4));
            qInfo("Detected : %s", string.toStdString().c_str());
            if (Q_NULLPTR != pUserName)
            {
                delete(pUserName);
                pUserName = NULL;
            }
            if (nIp1 == s1 && nIp2 == s2 && nIp3 == s3 && nIp4 == s4) {
                qInfo("Found the target IP");
                int ret = Open(m_stDevList.pDeviceInfo[i]);
                if (ret == MV_OK) {
                    StartGrabbing();
                }
                start(); // Start the thread
                break;
            }
        }
    }
    return nRet;
}

int HIKCamera::Open(MV_CC_DEVICE_INFO* pstDeviceInfo)
{
    if (Q_NULLPTR == pstDeviceInfo)
    {
        return MV_E_PARAMETER;
    }

    int nRet = MV_OK;
    if(m_hDevHandle == Q_NULLPTR)
    {
        nRet  = MV_CC_CreateHandle(&m_hDevHandle, pstDeviceInfo);
        if (MV_OK != nRet)
        {
            return nRet;
        }
    }

    nRet = MV_CC_OpenDevice(m_hDevHandle);
    if (MV_OK != nRet)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = Q_NULLPTR;

        return nRet;
    }

    return MV_OK;
}

int HIKCamera::StartGrabbing()
{
    isGrabbing = true;
    return MV_CC_StartGrabbing(m_hDevHandle);
}


int HIKCamera::GetOneFrameTimeout(unsigned char *pData, unsigned int *pnDataLen, unsigned int nDataSize, MV_FRAME_OUT_INFO_EX *pFrameInfo, int nMsec)
{
    if (Q_NULLPTR == pnDataLen)
    {
        return MV_E_PARAMETER;
    }

    int nRet = MV_OK;

    *pnDataLen  = 0;

    nRet = MV_CC_GetOneFrameTimeout(m_hDevHandle, pData, nDataSize, pFrameInfo, nMsec);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    *pnDataLen = pFrameInfo->nFrameLen;
    return nRet;
}

int HIKCamera::GetIntValue(IN const char* strKey, OUT unsigned int *pnValue)
{
    if (Q_NULLPTR == strKey || Q_NULLPTR == pnValue)
    {
        return MV_E_PARAMETER;
    }

    MVCC_INTVALUE stParam;
    memset(&stParam, 0, sizeof(MVCC_INTVALUE));
    int nRet = MV_CC_GetIntValue(m_hDevHandle, strKey, &stParam);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    *pnValue = stParam.nCurValue;

    return MV_OK;
}

int HIKCamera::SaveImage(MV_SAVE_IMAGE_PARAM_EX* pstParam)
{
    if (Q_NULLPTR == pstParam)
    {
        return MV_E_PARAMETER;
    }

    return MV_CC_SaveImageEx2(m_hDevHandle, pstParam);
}

//Return image from RAM
QImage HIKCamera::getImage()
{
    QMutexLocker locker(&mutex);
    QImage image_copy = latestImage.copy();
    return std::move(image_copy);
}

int HIKCamera::Close(){
    if (isGrabbing) {
        this->StopGrabbing();
    }
    int nRet = MV_OK;
    if (Q_NULLPTR == m_hDevHandle)
    {
        return MV_E_PARAMETER;
    }
    MV_CC_CloseDevice(m_hDevHandle);
    nRet = MV_CC_DestroyHandle(m_hDevHandle);
    m_hDevHandle = Q_NULLPTR;
    return nRet;
}

int HIKCamera::StopGrabbing()
{
    isGrabbing = false;
    this->wait(); // wait for the running thread stop
    return MV_CC_StopGrabbing(m_hDevHandle);
}

void HIKCamera::run()
{
    QThread::msleep(1000);
    while(isGrabbing)
    {
        int nRet = MV_OK;
        if (Q_NULLPTR == m_pBufForDriver)
        {
            nRet = GetIntValue("PayloadSize", &nRecvBufSize);
            if (nRet != MV_OK)
            {
                qFatal("Failed in getting payload size");
                return;
            } else {
                qInfo("Payload Size: %d", nRecvBufSize);
            }
            m_nBufSizeForDriver = nRecvBufSize;
            m_pBufForDriver = (unsigned char *)malloc(m_nBufSizeForDriver);
            if (Q_NULLPTR == m_pBufForDriver)
            {
                qWarning("malloc m_pBufForDriver failed, run out of memory");
                return;
            } else {
                qInfo("malloc success size for driver: %d", m_nBufSizeForDriver);
            }
        }

        MV_FRAME_OUT_INFO_EX stImageInfo = {0};
        memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

        unsigned int nDataSize = nRecvBufSize;
        unsigned int nImageNum = 1;
        unsigned int nDataLen = 0;

        nRet = GetOneFrameTimeout(m_pBufForDriver, &nDataLen, m_nBufSizeForDriver, &stImageInfo, 1000);
        if (nRet != MV_OK) {
            qFatal("Get Frame Fail");
            isGrabbing = false;
        }
        if (Q_NULLPTR == m_pBufForSaveImage)
        {
            m_nBufSizeForSaveImage = stImageInfo.nWidth * stImageInfo.nHeight * 3 + 2048;

            m_pBufForSaveImage = (unsigned char*)malloc(m_nBufSizeForSaveImage);
            if (Q_NULLPTR == m_pBufForSaveImage)
            {
                qWarning("malloc m_pBufForSaveImage failed, run out of memorye");
                return;
            }else {
                qInfo("malloc m_pBufForSaveImage success size for driver: %d", m_nBufSizeForSaveImage);
            }
        }
        MV_SAVE_IMAGE_PARAM_EX stParam = {0};
        stParam.enImageType = MV_Image_Bmp;
        stParam.enPixelType = stImageInfo.enPixelType;
        stParam.nWidth      = stImageInfo.nWidth;
        stParam.nHeight     = stImageInfo.nHeight;
        stParam.nDataLen    = stImageInfo.nFrameLen;
        stParam.pData       = m_pBufForDriver;
        stParam.pImageBuffer = m_pBufForSaveImage;
        stParam.nBufferSize = m_nBufSizeForSaveImage;
        stParam.nJpgQuality = 80;
        nRet = SaveImage(&stParam);
        mutex.lock();
        QImage imgBuff;
        imgBuff.loadFromData(m_pBufForSaveImage, m_nBufSizeForSaveImage);
        latestImage = imgBuff;
        mutex.unlock();
        callQmlRefeshImg();
    }
    qInfo("End of thread");
    isGrabbing = false;
}
