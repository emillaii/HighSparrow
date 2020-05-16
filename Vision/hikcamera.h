#ifndef HIKCAMMERA_H
#define HIKCAMMERA_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QQuickImageProvider>
#include "MvCameraControl.h"
#include "silicoolcamera.h"

class HIKCamera : public SilicoolCamera,  public QQuickImageProvider
{
    Q_OBJECT
public:
    HIKCamera();
    ~HIKCamera();

public:
    SCReturnCode SCCameraInit(QString cameraName) override;
    SCReturnCode SCGetImage(QImage &) override;
    SCReturnCode SCCameraStop() override;
    Q_INVOKABLE bool scSetExposureTime(float value) override;
    Q_INVOKABLE bool scSaveImage() override;
public:
    static int EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList);

    // ch:打开设备 | en:Open Device
    int     Open(MV_CC_DEVICE_INFO* pstDeviceInfo);

    int     Open(QString ip);

    // ch:关闭设备 | en:Close Device
    int     Close();

    // ch:开启抓图 | en:Start Grabbing
    int     StartGrabbing();

    // ch:停止抓图 | en:Stop Grabbing
    int     StopGrabbing();

    // ch:主动获取一帧图像数据 | en:Get one frame initiatively
    int     GetOneFrameTimeout(unsigned char* pData, unsigned int* pnDataLen, unsigned int nDataSize, MV_FRAME_OUT_INFO_EX* pFrameInfo, int nMsec);

    // ch:设置显示窗口句柄 | en:Set Display Window Handle
    int     Display(void* hWnd);

    // ch:保存图片 | en:save image
    int     SaveImage(MV_SAVE_IMAGE_PARAM_EX* pstParam);

    // ch:注册图像数据回调 | en:Register Image Data CallBack
    int     RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser),
                                    void* pUser);

    // ch:注册消息异常回调 | en:Register Message Exception CallBack
    int     RegisterExceptionCallBack(void(__stdcall* cbException)(unsigned int nMsgType, void* pUser),
                                        void* pUser);

    // ch:获取Int型参数，如 Width和Height，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    // en:Get Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
    int     GetIntValue(IN const char* strKey, OUT unsigned int *pnValue);
    int     SetIntValue(IN const char* strKey, IN unsigned int nValue);

    // ch:获取Float型参数，如 ExposureTime和Gain，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    // en:Get Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
    int     GetFloatValue(IN const char* strKey, OUT float *pfValue);
    int     SetFloatValue(IN const char* strKey, IN float fValue);

    // ch:获取Enum型参数，如 PixelFormat，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    // en:Get Enum type parameters, such as PixelFormat, for details please refer to MvCameraNode.xlsx file under SDK installation directory
    int     GetEnumValue(IN const char* strKey, OUT unsigned int *pnValue);
    int     SetEnumValue(IN const char* strKey, IN unsigned int nValue);

    // ch:获取Bool型参数，如 ReverseX，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    // en:Get Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
    int     GetBoolValue(IN const char* strKey, OUT bool *pbValue);
    int     SetBoolValue(IN const char* strKey, IN bool bValue);

    // ch:获取String型参数，如 DeviceUserID，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件UserSetSave
    // en:Get String type parameters, such as DeviceUserID, for details please refer to MvCameraNode.xlsx file under SDK installation directory
    int     GetStringValue(IN const char* strKey, IN OUT char* strValue, IN unsigned int nSize);
    int     SetStringValue(IN const char* strKey, IN const char * strValue);

    // ch:执行一次Command型命令，如 UserSetSave，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    // en:Execute Command once, such as UserSetSave, for details please refer to MvCameraNode.xlsx file under SDK installation directory
    int     CommandExecute(IN const char* strKey);

    // ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
    int     GetOptimalPacketSize();

    // 获取统计参数
    int     GetAllMatchInfo(OUT unsigned int *nLostFrame, OUT unsigned int *nFrameCount);

private:

    void*  m_hDevHandle;
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    QMutex mutex;
    QImage latestImage;
    bool isGrabbing;
public:
    QImage getImage();
    unsigned int nRecvBufSize = 0;

    unsigned char*  m_pBufForSaveImage = Q_NULLPTR;         // 用于保存图像的缓存
    unsigned int    m_nBufSizeForSaveImage;

    unsigned char*  m_pBufForDriver = Q_NULLPTR;            // 用于从驱动获取图像的缓存
    unsigned int    m_nBufSizeForDriver;

protected:
    void run() override;

signals:
    void callQmlRefeshImg();
};

#endif // HIKCAMMERA_H
