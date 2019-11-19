#include "utils/pixel2mech.h"

Pixel2Mech::Pixel2Mech()
{
    transformation = QMatrix();
    center_of_pixel = center_of_mech = QPointF(0.0,0.0);
    has_calibration = false;
}

Pixel2Mech::Pixel2Mech(const QMatrix &matrix, const QPointF & center_of_pixel)
{
    ChangeParameter(matrix,center_of_pixel);
}

void Pixel2Mech::ChangeParameter(const QMatrix &matrix, const QPointF &center_of_pixel)
{
    transformation = matrix;
    this->center_of_pixel = center_of_pixel;
    center_of_mech = transformation.map(center_of_pixel);
    has_calibration = true;
}
QMatrix Pixel2Mech::DoCalibration(const QPointF pixel_points[], const QPointF mech_points[], const QPointF &center_of_pixel)
{
    CMatrix M;
    M = getAffineTransform(pixel_points,mech_points);
    double * p_M_data = M.GetMatrixData();
    transformation.setMatrix(p_M_data[0],p_M_data[3],p_M_data[1],p_M_data[4],p_M_data[2],p_M_data[5]);
    this->center_of_pixel = center_of_pixel;
    center_of_mech = transformation.map(center_of_pixel);
    has_calibration = true;
    return transformation;
}

bool Pixel2Mech::CalcMechDistance(const QPointF &pixel_point,QPointF &mech_point)
{
    if(!has_calibration)
    {
        qInfo("Missing calibration");
        return false;
    }
    QPointF mech = transformation.map(pixel_point);
    QPointF mech_distence = mech - center_of_mech;
    mech_point.setX(mech_distence.x());
    mech_point.setY(mech_distence.y());
    return true;
}

QPointF Pixel2Mech::pixel2MechPoint(const QPointF &pixelPoint)
{
    if(has_calibration!=true)
        return QPointF(0.0,0.0);
    return transformation.map(pixelPoint);
}

QPointF Pixel2Mech::getMechCenter() const
{
    if(has_calibration!=true)
        return QPointF(0.0,0.0);
    return center_of_mech;
}

bool Pixel2Mech::hasCalibration()
{
    return has_calibration;
}

double Pixel2Mech::getXResolution()
{
    return fabs(transformation.m12());
}

double Pixel2Mech::getYResoultion()
{
    return fabs(transformation.m21());
}


/* Calculates coefficients of affine transformation
 * which maps (xi,yi) to (ui,vi), (i=1,2,3):
 *
 * ui = c00*xi + c01*yi + c02
 *
 * vi = c10*xi + c11*yi + c12
 *
 * Coefficients are calculated by solving linear system:
 *         A     *     X  = B
 * / x0 y0 1 0 0 0 \ /c00\ /u0\
 * | 0 0 0 x0 y0 1 | |c01| |v0|
 * | x1 y1 1 0 0 0 | |c02| |u1|
 * | 0 0 0 x1 y1 1 | |c10| |v1|
 * | x2 y2 1 0 0 0 | |c11| |u2|
 * \ 0 0 0 x2 y2 1 / |c12| |v2|
 *
 * where:
 * cij - matrix coefficients
 */
CMatrix Pixel2Mech::getAffineTransform(const QPointF src[], const QPointF dst[])
{

    double a[6*6]={0.0}, b[6];
    CMatrix M;
//fill the array
    for(int i=0;i<3;i++)
    {
        a[i*2]=a[i*2+19]=src[i].x();
        a[i*2+6]=a[i*2+19+6]=src[i].y();
        a[i*2+6+6]=a[i*2+19+6+6]=1;

        b[i*2] = dst[i].x();
        b[i*2+1] = dst[i].y();
    }
//make matrix by array
    CMatrix A(6,6,a);
    CMatrix B(6,1,b);
//calc the answer:AX=B than X = A\B
    M = B.LeftDiv(A);
    return M;
}
