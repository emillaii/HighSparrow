#ifndef VISIONMODULE_H
#define VISIONMODULE_H

#include <QObject>
#include <utils/errorcode.h>
#include <QQuickImageProvider>
#include <AVL.h>
#include "utils/imageprovider.h"
#include "thread_worker_base.h"
#include "utils/imageprovider.h"
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
    QString imageName;
};

class VisionModule: public ThreadWorkerBase,public QQuickImageProvider
{
    Q_OBJECT
public:
    VisionModule(BaslerPylonCamera *downlookCamera, BaslerPylonCamera * uplookCamera,
                 BaslerPylonCamera* pickarmCamera, BaslerPylonCamera * aa2DownlookCamera,
                 BaslerPylonCamera* sensorPickarmCamera, QString name);
    QVector<QPoint> VisionModule::Read_Dispense_Path();
    /*
     * Use the most generic NCC template matching
     */
    ErrorCodeStruct PR_Generic_NCC_Template_Matching(QString camera_name, QString pr_name, PRResultStruct &prResult, double object_score = 0.8);
    ErrorCodeStruct PR_Generic_NCC_Template_Matching_Retry(QString camera_name, QString pr_name, PRResultStruct &prResult, double object_score);
    ErrorCodeStruct PR_Edge_Template_Matching(QString camera_name, QString pr_name, PRResultStruct &prResult);
    ErrorCodeStruct Glue_Inspection();
    /*
     * Prism Type Special PR
     */
    ErrorCodeStruct PR_Prism_Only_Matching(QString camera_name, PRResultStruct &prResult);
    ErrorCodeStruct PR_Prism_SUT_Matching(QString camera_name, PRResultStruct &prResult);
    Q_INVOKABLE void aaDebugImage(QString input_filename, int threshold, int min_area, int max_area);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    Q_INVOKABLE void saveImage(int channel);
    bool saveImage(QString cameraName, QString imageName);
    Q_INVOKABLE void testVision();
    ImageProvider *aaDebugImageProvider;
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
    ImageProvider downlookCameraPrResultImageProvider;
    ImageProvider uplookCameraPrResultImageProvider;
    ImageProvider pickarmCameraPrResultImageProvider;
    ImageProvider glueInspectionResultImageProvider;
    QMutex mutex;
signals :
    void callQmlRefeshImg(int);
public:
    bool is_debug = false;
//ThreadWorkerBase Interface
public slots:
    void startWork(int run_mode) override;
    void stopWork(bool wait_finish) override;
    void resetLogic() override;
    void performHandlingOperation(int cmd, QVariant params);
public:
    PropertyBase *getModuleState() override;
    void receivceModuleMessage(QVariantMap module_message) override;
    QMap<QString, PropertyBase *> getModuleParameter() override;
    void setModuleParameter(QMap<QString, PropertyBase *>) override;
};

#endif // VISIONMODULE_H
