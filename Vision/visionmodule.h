#ifndef VISIONMODULE_H
#define VISIONMODULE_H

#include <QObject>
#include <Utils/errorcode.h>
#include <QQuickImageProvider>
#include <AVL.h>
//#include "imageprovider.h"
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
    double roi_x;
    double roi_y;
    double theta;
    double width;
    double height;
    bool ret;
    QString imageName;
};

class VisionModule: public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    VisionModule(BaslerPylonCamera *downlookCamera, BaslerPylonCamera * uplookCamera, BaslerPylonCamera* pickarmCamera);
    QVector<QPoint> VisionModule::Read_Dispense_Path();
    /*
     * Use the most generic NCC template matching
     */
    ErrorCodeStruct PR_Generic_NCC_Template_Matching(QString camera_name, QString pr_name, PRResultStruct &prResult);
    ErrorCodeStruct PR_Edge_Template_Matching(QString camera_name, QString pr_name, PRResultStruct &prResult);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    Q_INVOKABLE void saveImage(int channel);
    Q_INVOKABLE void testVision();
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
//    ImageProvider downlookCameraPrResultImageProvider;
//    ImageProvider uplookCameraPrResultImageProvider;
//    ImageProvider pickarmCameraPrResultImageProvider;
//    ImageProvider glueInspectionResultImageProvider;
signals :
    void callQmlRefeshImg();
public:
    bool is_debug = false;
};

#endif // VISIONMODULE_H
