#ifndef CALIBRATION_PARAMETER_H
#define CALIBRATION_PARAMETER_H

#include "propertybase.h"
#include "config.h"
class CalibrationParameter:public PropertyBase
{
    Q_OBJECT
public:
    CalibrationParameter():PropertyBase(){}
    Q_PROPERTY(double imageWidth READ imageWidth WRITE setimageWidth NOTIFY imageWidthChanged)
    Q_PROPERTY(double imageHeight READ imageHeight WRITE setimageHeight NOTIFY imageHeightChanged)
    Q_PROPERTY(double originX READ originX WRITE setOriginX NOTIFY originXChanged)
    Q_PROPERTY(double originY READ originY WRITE setOriginY NOTIFY originYChanged)
    Q_PROPERTY(double matrix11 READ matrix11 WRITE setMatrix11 NOTIFY matrix11Changed)
    Q_PROPERTY(double matrix12 READ matrix12 WRITE setMatrix12 NOTIFY matrix12Changed)
    Q_PROPERTY(double matrix21 READ matrix21 WRITE setMatrix21 NOTIFY matrix21Changed)
    Q_PROPERTY(double matrix22 READ matrix22 WRITE setMatrix22 NOTIFY matrix22Changed)
    Q_PROPERTY(double deltaX READ deltaX WRITE setDeltaX NOTIFY deltaXChanged)
    Q_PROPERTY(double deltaY READ deltaY WRITE setDeltaY NOTIFY deltaYChanged) 
    Q_PROPERTY(QString calibrationName READ calibrationName WRITE setCalibrationName NOTIFY calibrationNameChanged)
    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY locationNameChanged)
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(double calibrationStep READ calibrationStep WRITE setCalibrationStep NOTIFY calibrationStepChanged)
    double imageWidth() const
    {
        return m_imageWidth;
    }

    double imageHeight() const
    {
        return m_imageHeight;
    }

    double originX() const
    {
        return m_originX;
    }

    double originY() const
    {
        return m_originY;
    }

    double matrix11() const
    {
        return m_matrix11;
    }

    double matrix12() const
    {
        return m_matrix12;
    }

    double matrix21() const
    {
        return m_matrix21;
    }

    double matrix22() const
    {
        return m_matrix22;
    }

    double deltaX() const
    {
        return m_deltaX;
    }

    double deltaY() const
    {
        return m_deltaY;
    }

    QString calibrationName() const
    {
        return m_calibrationName;
    }

    QString locationName() const
    {
        return m_locationName;
    }

    QString motorXName() const
    {
        return m_motorXName;
    }

    QString motorYName() const
    {
        return m_motorYName;
    }

    double calibrationStep() const
    {
        return m_calibrationStep;
    }

public slots:
    void setOriginX(double originX)
    {
        if (qFuzzyCompare(m_originX, originX))
            return;
        m_originX = originX;
        emit originXChanged(m_originX);
    }

    void setOriginY(double originY)
    {
        if (qFuzzyCompare(m_originY, originY))
            return;

        m_originY = originY;
        emit originYChanged(m_originY);
    }

    void setMatrix11(double matrix11)
    {
        if (qFuzzyCompare(m_matrix11, matrix11))
            return;

        m_matrix11 = matrix11;
        emit matrix11Changed(m_matrix11);
    }

    void setMatrix12(double matrix12)
    {
        if (qFuzzyCompare(m_matrix12, matrix12))
            return;

        m_matrix12 = matrix12;
        emit matrix12Changed(m_matrix12);
    }

    void setMatrix21(double matrix21)
    {
        if (qFuzzyCompare(m_matrix21, matrix21))
            return;

        m_matrix21 = matrix21;
        emit matrix21Changed(m_matrix21);
    }

    void setMatrix22(double matrix22)
    {
        if (qFuzzyCompare(m_matrix22, matrix22))
            return;

        m_matrix22 = matrix22;
        emit matrix22Changed(m_matrix22);
    }

    void setDeltaX(double deltaX)
    {
        if (qFuzzyCompare(m_deltaX, deltaX))
            return;

        m_deltaX = deltaX;
        emit deltaXChanged(m_deltaX);
    }

    void setDeltaY(double deltaY)
    {
        if (qFuzzyCompare(m_deltaY, deltaY))
            return;

        m_deltaY = deltaY;
        emit deltaYChanged(m_deltaY);
    }

    void setimageWidth(double imageWidth)
    {
        if (qFuzzyCompare(m_imageWidth, imageWidth))
            return;

        m_imageWidth = imageWidth;
        emit imageWidthChanged(m_imageWidth);
    }

    void setimageHeight(double imageHeight)
    {
        if (qFuzzyCompare(m_imageHeight, imageHeight))
            return;

        m_imageHeight = imageHeight;
        emit imageHeightChanged(m_imageHeight);
    }

    void setCalibrationName(QString calibrationName)
    {
        if (m_calibrationName == calibrationName)
            return;

        m_calibrationName = calibrationName;
        emit calibrationNameChanged(m_calibrationName);
    }

    void setLocationName(QString locationName)
    {
        if (m_locationName == locationName)
            return;

        m_locationName = locationName;
        emit locationNameChanged(m_locationName);
    }

    void setMotorXName(QString motorXName)
    {
        if (m_motorXName == motorXName)
            return;

        m_motorXName = motorXName;
        emit motorXNameChanged(m_motorXName);
    }

    void setMotorYName(QString motorYName)
    {
        if (m_motorYName == motorYName)
            return;

        m_motorYName = motorYName;
        emit motorYNameChanged(m_motorYName);
    }

    void setCalibrationStep(double calibrationStep)
    {
        if (qFuzzyCompare(m_calibrationStep, calibrationStep))
            return;

        m_calibrationStep = calibrationStep;
        emit calibrationStepChanged(m_calibrationStep);
    }

signals:
    void originXChanged(double originX);

    void originYChanged(double originY);

    void matrix11Changed(double matrix11);

    void matrix12Changed(double matrix12);

    void matrix21Changed(double matrix21);

    void matrix22Changed(double matrix22);

    void deltaXChanged(double deltaX);

    void deltaYChanged(double deltaY);

    void imageWidthChanged(double imageWidth);

    void imageHeightChanged(double imageHeight);

    void calibrationNameChanged(QString calibrationName);

    void locationNameChanged(QString locationName);

    void motorXNameChanged(QString motorXName);

    void motorYNameChanged(QString motorYName);

    void calibrationStepChanged(double calibrationStep);

private:
    double m_imageWidth = DOWNLOOK_VISION_CAMERA_WIDTH;
    double m_imageHeight = DOWNLOOK_VISION_CAMERA_HEIGHT;
    double m_originX = 0;
    double m_originY = 0;
    double m_matrix11 = 0;
    double m_matrix12 = 0;
    double m_matrix21 = 0;
    double m_matrix22 = 0;
    double m_deltaX = 0;
    double m_deltaY = 0;
    QString m_calibrationName = "Calibration";
    QString m_locationName = "";
    QString m_motorXName = "";
    QString m_motorYName = "";
    double m_calibrationStep = 1;
};

#endif // CALIBRATION_PARAMETER_H
