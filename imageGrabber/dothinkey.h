#ifndef DOTHINKEY_H
#define DOTHINKEY_H

#include <string>
#include <Windows.h>
#include "imagekit.h"
#include "dtccm2.h"
#include "imageGrabber\iniparser.h"
#include <QObject>
#include <QPixmap>
#include "propertybase.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#define CAMERA_START 1
#define CAMERA_STOP 2
#define GRAB_START 1
#define GRAB_STOP 2

class Dothinkey : public PropertyBase
{
    Q_OBJECT
public:
    explicit Dothinkey(QObject *parent = 0);
    ~Dothinkey();
    Q_PROPERTY(QString currentSensorID READ currentSensorID WRITE setCurrentSensorID NOTIFY paramsChanged)
    Q_PROPERTY(QString IniFilename READ IniFilename WRITE setIniFilename NOTIFY paramsChanged)
    Q_INVOKABLE bool initSensor();
    void loadParams(QString file_name);
    BOOL DothinkeyEnum();   //Enumerate the dothinkey devices
    BOOL DothinkeyOpen();   //Open Camera Devices
    BOOL DothinkeyClose();  //Close Camera Devices
    BOOL DothinkeyLoadIniFile(int channel);   // 0 is camera channel 0, 1 is camera channel 1
    BOOL DothinkeyStartCamera(int channel);
    // Hardcore OTP in UV, temp solution, move to ini file later
    BOOL DothinkeyOTP(int serverMode);
    BOOL DothinkeyOTPEx();
    QImage* DothinkeyGrabImage(int channel);
    cv::Mat DothinkeyGrabImageCV(int channel, bool &ret);
    void DothinkeySetConfigFile(std::string filename);
    QString readSensorID();
    BOOL DothinkeyIsGrabbing();
    struct CameraChannel
    {
        CameraChannel()
            : m_iDevID(-1)
            , m_fMclk(12.0f)
            , m_fAvdd(2.8f)
            , m_fDovdd(1.8f)
            , m_fDvdd(1.5f)
            , m_fAfvcc(0.028f)
            , m_vpp(0.0f)
            , m_uFocusPos(0)
        {}

        bool CloseCameraChannel();
        SensorTab current_sensor;
        char* m_charCurrentDevice;
        int m_iDevID;
        ULONG m_GrabSize;
        float m_fMclk;
        float m_fAvdd;
        float m_fDovdd;
        float m_fDvdd;
        float m_fAfvcc;
        float m_vpp;
        UINT m_uFocusPos;
        float m_fFrameFps;
        int m_iFrameErr;
        int m_iFrameCnt;
    };

    QString IniFilename() const
    {
        return m_IniFilename;
    }

    QString currentSensorID() const
    {
        return m_currentSensorID;
    }

public slots:
    void saveJsonConfig(QString file_name);
    void setIniFilename(QString IniFilename)
    {
        if (m_IniFilename == IniFilename)
            return;

        m_IniFilename = IniFilename;
        emit paramsChanged(m_IniFilename);
    }

    void setCurrentSensorID(QString currentSensorID)
    {
        if (m_currentSensorID == currentSensorID)
            return;

        m_currentSensorID = currentSensorID;
        emit paramsChanged(m_currentSensorID);
    }

signals:
    void paramsChanged(QString IniFilename);

private:
    char *DeviceName[4];

    BOOL SetVoltageMclk(SensorTab CurrentSensor, int iDevID, float Mclk, float Avdd, float Dvdd, float Dovdd, float Afvcc, float vpp);
    void MipiRaw10ToP10(BYTE *pIn, BYTE *pOut, int width, int height);
    BOOL SaveBmpFile(std::string sfilename, BYTE *pBuffer, UINT width, UINT height);

    CameraChannel m_CameraChannels[2];
    std::string iniFilename;

    QString m_IniFilename;
    QString m_currentSensorID = "";

    bool isGrabbing = false;
    QStringList cmd_list;
    QStringList otp_list;
};

#endif // DOTHINKEY_H
