#include "calibration/chart_calibration.h"
#include "visionavadaptor.h"
#include "utils/commonutils.h"
ChartCalibration::ChartCalibration(Dothinkey *dothinkey, int max_intensity, int min_area, int max_area, QString name, QString file_name, QObject *parent)
    :Calibration(name,file_name,nullptr)
{
    this->dothinkey = dothinkey;
    this->max_intensity = max_intensity;
    this->min_area = min_area;
    this->max_area = max_area;
}

bool ChartCalibration::GetPixelPoint(double &x, double &y)
{
    bool grabRet = false;
    cv::Mat img = dothinkey->DothinkeyGrabImageCV(0, grabRet);
    if (grabRet != true)
    {
        qInfo("DothinkeyGrabImageCV fail in GetPixelPoint()");
        return false;
    }

    QImage outImage;
    double offsetX, offsetY;
    unsigned int ccIndex = 10000, ulIndex = 0, urIndex = 0, lrIndex = 0, llIndex = 0;
    QString imageName;
    imageName.append(getGrabberLogDir())
                    .append(getCurrentTimeString())
                    .append(".jpg");
    cv::imwrite(imageName.toStdString().c_str(), img);
    std::vector<AA_Helper::patternAttr> vector = AA_Helper::AA_Search_MTF_Pattern(img, outImage, false,
                                                                                  ccIndex, ulIndex, urIndex, llIndex, lrIndex,max_intensity,min_area,max_area);
    this->parameters.setimageWidth(img.cols);
    this->parameters.setimageHeight(img.rows);
    if( vector.size()<1 || ccIndex > 9 )
    {
        qInfo(("error in get center of " + name ).toStdString().data());
        return false;
    }
    offsetX = vector[ccIndex].center.x();
    offsetY = vector[ccIndex].center.y();
    qInfo((name + " pixel x: %f y: %f").toStdString().data(), offsetX, offsetY);
    x = offsetX;
    y = offsetY;
    return true;
}

bool ChartCalibration::calculateMatrixAttribute(QVector<QPointF> p, QVector<QPointF> m, double &scaleX, double &scaleY, double &closestAngle)
{
    double diff_py10 = p[1].y() - p[0].y();
    double diff_px10 = p[1].x() - p[0].x();
    double diff_py30 = p[3].y() - p[0].y();
    double diff_px30 = p[3].x() - p[0].x();
    double diff_mx10 = m[1].x() - m[0].x();
    double diff_mx30 = m[3].x() - m[0].x();
    double diff_my10 = m[1].y() - m[0].y();
    double diff_my30 = m[3].y() - m[0].y();
    double a_11 = (diff_py10 * diff_mx30 - diff_py30 * diff_mx10) / (diff_px30 * diff_py10 - diff_py30 * diff_px10);
    double a_12 = (diff_mx10 - diff_px10 * a_11) / diff_py10;
    double a_13 = (m[0].x() - p[0].x() * a_11 - p[0].y() * a_12);

    double a_21 = (diff_py10 * diff_my30 - diff_py30 * diff_my10) / (diff_px30 * diff_py10 - diff_py30 * diff_px10);
    double a_22 = (diff_my10 - diff_px10 * a_21) / diff_py10;
    double a_23 = (m[0].y() - p[0].x() * a_21 - p[0].y() * a_22);

    double s_x = sqrt(a_11 * a_11 + a_21 * a_21);
    double s_y = sqrt(a_12 * a_12 + a_22 * a_22);
    if (s_x == 0 || s_y == 0) { return false; }
    scaleX = 1/s_x;
    scaleY = 1/s_y;
    double angle = acos(a_11 / s_x) * 180;
    double steppedAngle = angle - 360*((int)((int)angle/360));
    closestAngle = 0;
    if (steppedAngle <= 45) { closestAngle = 0; }
    else if (steppedAngle <= 135) { closestAngle = 90; }
    else if (steppedAngle <= 225) { closestAngle = 180; }
    else if (steppedAngle <= 315) { closestAngle = 270; }
    return true;
}
