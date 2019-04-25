#ifndef CHART_CALIBRATION_H
#define CHART_CALIBRATION_H

#include "calibration.h"
#include "dothinkey.h"

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
private:
    bool GetPixelPoint(double &x, double &y)override;
private:
    Dothinkey *dothinkey;
    int max_intensity;
    int min_area;
    int max_area;
};

#endif // CHART_CALIBRATION_H
