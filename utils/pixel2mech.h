#ifndef PIXEL2MECH_H
#define PIXEL2MECH_H
#include "Matrix/Matrix.h"
#include <QPointF>
#include <QMatrix>

struct PrOffset
{
public:
    PrOffset() {X = 0;Y = 0;O_X = 0;O_Y = 0;W = 0;H = 0;Theta = 0;}
    PrOffset(double x,double y,double theta) {X = x;Y = y;Theta = theta;}
    void ReSet(){X = 0;Y = 0;Theta = 0;}
    double X;
    double Y;
    double Theta;
    double W;
    double H;
    double O_X;
    double O_Y;
};

class Pixel2Mech
{
public:
    Pixel2Mech();
//用以前校正过的数据来初始化
    Pixel2Mech(const QMatrix &matrix, const QPointF & center_of_pixel);
    void ChangeParameter(const QMatrix &matrix, const QPointF & center_of_pixel);
//三点校正，提供三个像素点，和对应的三个机械点，还有相机的中心点的坐标，返回变换矩阵供存档
    QMatrix DoCalibration(const QPointF pixel_points[3], const QPointF mech_points[3],const QPointF & center_of_pixel);
//计算传入的像素点和相机中心的机械距离
    bool CalcMechDistance(const QPointF &pixel_point,QPointF &mech_point);

    QPointF pixel2MechPoint(const QPointF &pixelPoint);

    QPointF getMechCenter() const;

    bool hasCalibration();

    double getXResolution();

    double getYResoultion();
private:
    //是否经过校正
    bool has_calibration = false;
    //变换矩阵
    QMatrix transformation;
    //相机中心坐标
    QPointF center_of_pixel;
    //机械中心坐标
    QPointF center_of_mech;
    //求仿射转换的矩阵
    CMatrix getAffineTransform(const QPointF src[], const QPointF dst[]);
};

#endif // PIXEL2MECH_H
