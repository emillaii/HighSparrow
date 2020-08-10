#ifndef CHART_CALIBRATION_H
#define CHART_CALIBRATION_H

#include "Vision/calibration.h"
#include "ImageGrabber/dothinkey.h"

#include <QObject>

class ChartCalibration : public Calibration
{
    Q_OBJECT
public:
    explicit ChartCalibration(Dothinkey *dothinkey,int max_intensity, int min_area, int max_area,QString name,QString file_path,QObject *parent = nullptr);

signals:

public slots:
private:
    // Calibration interface
public:
    bool GetPixelPoint(double &x, double &y)override;
    bool GetPixelPoint_HW(QString fileName, double &x, double &y);
private:
    Dothinkey *dothinkey;
    int max_intensity;
    int min_area;
    int max_area;
};

#endif // CHART_CALIBRATION_H
