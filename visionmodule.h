#ifndef VISIONMODULE_H
#define VISIONMODULE_H

#include <QObject>
#include <errorcode.h>
#include <QQuickImageProvider>
#include <AVL.h>

class BaslerPylonCamera;

struct PRResultStruct {
    PRResultStruct() : x(0), y(0), theta(0), ret(false)
    {}
    PRResultStruct(double x, double y, double theta, bool ret)
        : x(x), y(y), theta(theta),
          ret(ret)
    {}
    double x;
    double y;
    double theta;
    bool ret;
    QString imageName;
};

class VisionModule: public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    VisionModule(BaslerPylonCamera *downlookCamera, BaslerPylonCamera * uplookCamera, BaslerPylonCamera* pickarmCamera);
    /*
     * Use the most generic NCC template matching
     */
    ErrorCodeStruct PR_Generic_NCC_Template_Matching(QString camera_name, QString pr_name, PRResultStruct &prResult);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    Q_INVOKABLE void saveImage(int channel);
    Q_INVOKABLE void testVision();
private:
    QString last_uplook_pr_result;
    QString last_downlook_pr_result;
    QString last_pickarm_pr_result;
    void displayPRResult(const QString, const PRResultStruct);

    bool grabImageFromCamera(QString cameraName, avl::Image &image);
    BaslerPylonCamera * downlookCamera;
    BaslerPylonCamera * uplookCamera;
    BaslerPylonCamera * pickarmCamera;
signals :
    void callQmlRefeshImg();  // Preview 1
};

#endif // VISIONMODULE_H
