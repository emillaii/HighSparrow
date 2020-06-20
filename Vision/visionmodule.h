#ifndef VISIONMODULE_H
#define VISIONMODULE_H

#include <QObject>
#include <Utils/errorcode.h>
#include <QQuickImageProvider>
#include "Utils/imageprovider.h"
#include <AVL.h>

class BaslerPylonCamera;
class HIKCamera;

struct PRResultStruct {
    PRResultStruct() : x(0), y(0), theta(0), width(0), height(0), ret(false)
    {}
    PRResultStruct(double x, double y, double theta, bool ret, double width, double height)
        : x(x), y(y), theta(theta),
          width(width), height(height), ret(ret)
    {}
    double x;
    double y;
    double roi_x;
    double roi_y;
    double theta;
    double width;
    double height;
    bool ret;
    QString imageName;
};

struct ProfileCalcState
{
    avl::ImageProfileAlongPathState imageProfileAlongPathState1;
    avl::Profile profile1;
    avl::Path path1;
    avl::Profile profile2;
    avl::Profile profile3;
    atl::Array< float > realArray1;
};

struct StepMacro_1State
{
    atl::Array< avl::Point2D > point2DArray1;
    atl::Array< avl::Point2D > point2DArray2;
    atl::Array< avl::Point2D > point2DArray3;
    avl::Path path1;
    avl::Path path2;
    avl::Path path3;
    ProfileCalcState profileCalcState1;
    ProfileCalcState profileCalcState2;
    ProfileCalcState profileCalcState3;
};

struct TOFResult {
    TOFResult() : a(0),b(0),c(0),d(0),e(0),f(0),g(0),h(0),i(0),j(0),k(0),l(0), ret(false) {}
    double a;
    double b;
    double c;
    double d;
    double e;
    double f;
    double g;
    double h;
    double i;
    double j;
    double k;
    double l;
    bool ret;
    QString imageName;
};

class VisionModule: public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    VisionModule(BaslerPylonCamera *downlookCamera, BaslerPylonCamera * uplookCamera, BaslerPylonCamera* pickarmCamera, HIKCamera* hikCamera);
    QVector<QPoint> VisionModule::Read_Dispense_Path();
    /*
     * Use the most generic NCC template matching
     */
    ErrorCodeStruct PR_Generic_NCC_Template_Matching(QString camera_name, QString pr_name, PRResultStruct &prResult);
    ErrorCodeStruct PR_Edge_Template_Matching(QString camera_name, QString pr_name, PRResultStruct &prResult);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    /*
     * Edge Fitting Algorithm
     */
    ErrorCodeStruct PR_Edge_Fitting(QString camera_name, QString pr_name, PRResultStruct &prResult, double object_score = 0.6, bool detect_small_hole = false);

    void saveImage(int channel, QString filename);
    Q_INVOKABLE void saveImage(int channel);
    Q_INVOKABLE void aaDebugImage(QString input_filename, int threshold, int min_area, int max_area);
    Q_INVOKABLE void reconnect(int channel);
    ImageProvider *aaDebugImageProvider;

    //For Huawei TOF
    TOFResult imageProcessing1(QString filename, double y1, double y2, double y4, double y5, double intensity_percentage, double alpha, bool grabFromCamera = false);
    TOFResult imageProcessing2(QString filename, double y1, double y2, double y3, double intensity_percentage, double alpha, bool grabFromCamera = false);
    void Y1Line( float inX, int inY, atl::Array< avl::Point2D >& outArray );
    void CenterXLine( float inY, int inX, atl::Array< avl::Point2D >& outArray );
    void CenterYLine( float inX, int inY, atl::Array< avl::Point2D >& outArray );
    void ProfileCalc( ProfileCalcState& state, const avl::Image& inImage, const avl::Path& inScanPath, float& outCenterY1, float& outValue1, float& outValue2, float& outHalfHeightWidth, avl::Profile& outProfile , double intensity_percentange = 0.5);
    void StepMacro_1( StepMacro_1State& state, float inX, int inY, float inX2, float inX3, const avl::Image& inImage, float& outCenterY1, float& outValueY1_1, float& outValueY1_2, float& outHalfHeightWidth, float& outCenterY2, float& outValueY2_1, float& outValueY2_2, float& outCenterY3, float& outValueY3_1, float& outValueY3_2, float& outHalfHeightWidth2, float& outHalfHeightWidth3, avl::Profile& outProfile, avl::Profile& outProfile1, avl::Profile& outProfile2, double intensity_profile = 0.5 );

private:
    QString last_uplook_pr_result;
    QString last_downlook_pr_result;
    QString last_pickarm_pr_result;
    bool grabImageFromCamera(QString cameraName, avl::Image &image);
    BaslerPylonCamera * downlookCamera = Q_NULLPTR;
    BaslerPylonCamera * uplookCamera = Q_NULLPTR;
    BaslerPylonCamera * pickarmCamera = Q_NULLPTR;
    HIKCamera * hikCamera = Q_NULLPTR;
public:
    bool is_debug = false;
signals:
    void callQmlRefeshImg(int);
};

#endif // VISIONMODULE_H
