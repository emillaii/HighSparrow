#include "chart_calibration.h"
#include "visionavadaptor.h"

ChartCalibration::ChartCalibration(Dothinkey *dothinkey, int max_intensity, int min_area, int max_area, QString name, QString file_name, XtMotor *motor_x, XtMotor *motor_y, QObject *parent)
    :Calibration(name,file_name,motor_x,motor_y,nullptr,0,0,"","",nullptr)
{
    this->dothinkey = dothinkey;
    this->max_intensity = max_intensity;
    this->min_area = min_area;
    this->max_area = max_area;
}

bool ChartCalibration::GetPixelPoint(double &x, double &y)
{
    cv::Mat img = dothinkey->DothinkeyGrabImageCV(0);
    QImage outImage;
    double offsetX, offsetY;
    unsigned int ccIndex = 10000, ulIndex = 0, urIndex = 0, lrIndex = 0, llIndex = 0;

    std::vector<AA_Helper::patternAttr> vector = AA_Helper::AA_Search_MTF_Pattern(img, outImage, false,
                                                                                  ccIndex, ulIndex, urIndex, llIndex, lrIndex,max_intensity,min_area,max_area);

    if( vector.size()<1 || ccIndex > 9 )
    {
        qInfo(("error in get center of " + name ).toStdString().data());
        return false;
    }
    offsetX = vector[ccIndex].center.x();
    offsetY = vector[ccIndex].center.y();
    qInfo((name + " pixel x: %f y: %f").toStdString().data(), offsetX, offsetY);
    x = offsetY;
    y = -offsetX;
        return true;
}
