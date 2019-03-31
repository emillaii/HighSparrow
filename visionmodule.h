#ifndef VISIONMODULE_H
#define VISIONMODULE_H

#include <QObject>
#include <errorcode.h>
#include <QQuickImageProvider>

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
    VisionModule();
    /*
     * Use the most generic NCC template matching
     */
    ErrorCodeStruct PR_Generic_NCC_Template_Matching(QString camera_name, PRResultStruct &prResult);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    PRResultStruct last_uplook_pr_result;
    PRResultStruct last_downlook_pr_result;
    PRResultStruct last_pickarm_pr_result;
    void displayPRResult(const QString, const PRResultStruct);
signals :
    void callQmlRefeshImg();  // Preview 1
};

#endif // VISIONMODULE_H
