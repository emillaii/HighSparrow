#ifndef VISIONMODULE_H
#define VISIONMODULE_H

#include <QObject>
#include <utils/errorcode.h>
#include <QQuickImageProvider>
#include <AVL.h>
#include "utils/imageprovider.h"
#include "thread_worker_base.h"
#include "utils/imageprovider.h"
#include "./rep_vision_replica.h"

class BaslerPylonCamera;

struct PRResultStruct {
    PRResultStruct() : x(0), y(0), theta(0), width(0), height(0), ret(false)
    {}
    PRResultStruct(double x, double y, double theta, bool ret, double width, double height)
        : x(x), y(y), theta(theta),
          width(width), height(height), ret(ret)
    {}
    double x;
    double y;
    double ori_x;
    double ori_y;
    double theta;
    double width;
    double height;
    bool ret;
    QString rawImageName;
    QString imageName;
};

struct RegionJudgeState
{
    avl::Region region1;
    atl::Array< avl::Region > regionArray1;
    atl::Array< avl::Region > regionArray2;
    atl::Array< avl::Region > regionArray3;
};


struct WidthJudgeState
{
    avl::Image image1;
    atl::Conditional< atl::Array< atl::Conditional< avl::Path > > > pathArray1;
    atl::Array< avl::Path > pathArray2;
    atl::Array< avl::Path > pathArray3;
    atl::Array< avl::Path > pathArray4;
    atl::Array< avl::Path > pathArray5;
    atl::Array< avl::Path > pathArray6;
    atl::Conditional< avl::Path > path1;
    atl::Conditional< avl::Path > path2;
    atl::Conditional< avl::Path > path3;
    atl::Array< avl::Point2D > point2DArray1;
    atl::Conditional< avl::Path > path4;
    atl::Conditional< atl::Array< atl::Conditional< avl::Segment2D > > > segment2DArray1;
    avl::Profile profile1;
    atl::Array< float > realArray1;
    atl::Array< avl::Segment2D > segment2DArray2;
    atl::Conditional< atl::String > string1;
    atl::Array< bool > boolArray1;
    atl::Array< bool > boolArray2;
    atl::Array< avl::Segment2D > segment2DArray3;
    atl::Array< avl::Segment2D > segment2DArray4;
    atl::Conditional< atl::String > string2;
    atl::Array< atl::Conditional< avl::Path > > pathArray7;
    avl::Image image2;
    atl::Array< atl::Conditional< avl::Path > > pathArray8;
    avl::Image image3;
    avl::Image image4;
    atl::Conditional< atl::Array< atl::Conditional< atl::String > > > stringArray1;
    atl::Conditional< atl::Array< atl::Conditional< atl::String > > > stringArray2;
    atl::Conditional< atl::Array< atl::Conditional< atl::String > > > stringArray3;
    avl::Image image5;
    atl::Conditional< atl::String > string3;
    atl::String string4;
    atl::String string5;
    atl::Array< atl::Conditional< atl::String > > stringArray4;
};

struct SmallHoleDetectionParam
{
    bool detectSmallHole;
    int smallHoleEdgeResponse;
    int smallHoleScanWidth;
    int smallHoleScanCount;
    double smallHoleRadiusMax;
    double smallHoleRadiusMin;

    SmallHoleDetectionParam()
    {
        detectSmallHole = false;
        smallHoleEdgeResponse = 10;
        smallHoleScanWidth = 10;
        smallHoleScanCount = 10;
        smallHoleRadiusMax = 7;
        smallHoleRadiusMin = 6;
    }
};

class VisionServer;

class VisionModule: public QObject ,public QQuickImageProvider
{
//    Q_ENUMS(HandleCameraChannel)
    Q_OBJECT
    Q_ENUMS(HandleCameraChannel)
public:
    enum HandleCameraChannel
    {
        CAMERA_CHANNEL_UPLOOK_VISION_CAMERA = 0,
        CAMERA_CHANNEL_DOWNLOOK_VISION_CAMERA,
        CAMERA_CHANNEL_PICKARM_VISION_CAMERA,
        CAMERA_CHANNEL_CAMERA_AA2_DL,
        CAMERA_CHANNEL_CAMERA_SPA_DL,
        CAMERA_CHANNEL_CAMERA_LPA_UL,
        CAMERA_CHANNEL_CAMERA_LPA_BARCODE
    };
    VisionModule();
    VisionModule(BaslerPylonCamera *downlookCamera, BaslerPylonCamera * uplookCamera,
                 BaslerPylonCamera* pickarmCamera, BaslerPylonCamera * aa2DownlookCamera,
                 BaslerPylonCamera* sensorPickarmCamera, BaslerPylonCamera* sensorUplookCamera,
                 BaslerPylonCamera* barcodeCamera, QString name, int serverMode);
    QVector<QPoint> Read_Dispense_Path();
    /*
     * Use the most generic NCC template matching
     */
    ErrorCodeStruct PR_Generic_NCC_Template_Matching(QString camera_name,
                                                     QString pr_name,
                                                     PRResultStruct &prResult,
                                                     double object_score = 0.8,
                                                     int retryCount = 3,
                                                     SmallHoleDetectionParam *paramStruct = nullptr);
    ErrorCodeStruct PR_Edge_Template_Matching(QString camera_name, QString pr_name, PRResultStruct &prResult);
    ErrorCodeStruct Glue_Inspection(double resolution, double minWidth, double maxWidth, double maxAvgWidth,
                                    QString beforeImage, QString afterImage, QString *glueInspectionImageName,
                                    double *outMinGlueWidth, double *outMaxGlueWidth, double *outMaxAvgGlueWidth);
    /*
     * Glue inspection
     */
    void RegionJudge( RegionJudgeState& state, const avl::Image& inSubtractImage, const avl::Image& inAfterImage, bool& outRegionOk, atl::Conditional< avl::Region >& outRegion, avl::Region& outRegion1, avl::Region& outRegion2, atl::Conditional< avl::Region >& outRegion3 );
    void WidthJudge( WidthJudgeState& state, bool inRegionOk, atl::Conditional< const avl::Region& > inRegion, float inResolution, float inMinWidth, float inMaxWidth, const avl::Image& inAfterImage, float inMaxAveWidth, bool& outResultOK, avl::Image& outResultImage, atl::Conditional< float >& outMaxWidth, atl::Conditional< float >& outMinWidth, atl::Conditional< float >& outAveWidth );

    /*
     * Prism Type Special PR
     */
    ErrorCodeStruct PR_Prism_Only_Matching(QString camera_name, PRResultStruct &prResult);
    ErrorCodeStruct PR_Prism_SUT_Matching(QString camera_name, PRResultStruct &prResult);
    ErrorCodeStruct PR_Prism_SUT_Two_Circle_Matching(QString camera_name, PRResultStruct &prResult);

    /*
     * Edge Fitting Algorithm
     */
    ErrorCodeStruct PR_Edge_Fitting(QString camera_name, QString pr_name, PRResultStruct &prResult, double object_score = 0.6, bool detect_small_hole = false);

    Q_INVOKABLE void aaDebugImage(QString input_filename, int threshold, int min_area, int max_area);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    Q_INVOKABLE void saveImage(int channel);
    bool saveImage(QString cameraName, QString imageName);
    Q_INVOKABLE void testVision();
    ImageProvider *aaDebugImageProvider;
    ImageProvider visionModuleImageProviders[10];
private:
    QString last_uplook_pr_result;
    QString last_downlook_pr_result;
    QString last_pickarm_pr_result;
    void displayPRResult(const QString, const PRResultStruct);
    void diffenenceImage(QImage image1, QImage image2);
    bool grabImageFromCamera(QString cameraName, avl::Image &image);
    bool saveImageAndCheck(avl::Image image1, QString imageName);
    BaslerPylonCamera * downlookCamera;
    BaslerPylonCamera * uplookCamera;
    BaslerPylonCamera * pickarmCamera;
    BaslerPylonCamera * aa2DownlookCamera;
    BaslerPylonCamera * sensorPickarmCamera;
    BaslerPylonCamera * sensorUplookCamera;
    BaslerPylonCamera * barcodeCamera;
    ImageProvider downlookCameraPrResultImageProvider;
    ImageProvider uplookCameraPrResultImageProvider;
    ImageProvider pickarmCameraPrResultImageProvider;
    ImageProvider glueInspectionResultImageProvider;
    QMutex mutex;
    //Glue Inspection
    int GlueLineMinArea = 8000;
    int GlueInnerFrameMinArea = 18000;
    float Resolution = 0.0284f;
    float MinWidth = 0.18f;
    VisionServer *server = nullptr;
    SilicoolVisionReplica *visionRep = nullptr;
    QRemoteObjectNode node;
    QRemoteObjectHost host;
    Qt::HANDLE threadId;
    int serverMode;
signals :
    void callQmlRefeshImg(int);
    void displayUplookImage(QString cameraName);
    QImage grabImageFromMainThreadSig(QString cameraName);
public:
    bool is_debug = false;
private slots:
    QImage grabImageFromMainThreadSlot(QString cameraName);
};

#endif // VISIONMODULE_H
