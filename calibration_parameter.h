#ifndef CALIBRATION_PARAMETER_H
#define CALIBRATION_PARAMETER_H

#include "propertybase.h"

class CalibrationParameter:public PropertyBase
{
    Q_OBJECT
public:
    CalibrationParameter():PropertyBase(){}
    Q_PROPERTY(double imageWidth READ imageWidth WRiTE setimageWidth NOTiFY imageWidthChanged)
    Q_PROPERTY(double imageHeight READ imageHeight WRiTE setimageHeight NOTiFY imageHeightChanged)
    Q_PROPERTY(double originX READ originX WRITE setOriginX NOTIFY originXChanged)
    Q_PROPERTY(double originY READ originY WRITE setOriginY NOTIFY originYChanged)
    Q_PROPERTY(double matrix11 READ matrix11 WRITE setMatrix11 NOTIFY matrix11Changed)
    Q_PROPERTY(double matrix12 READ matrix12 WRITE setMatrix12 NOTIFY matrix12Changed)
    Q_PROPERTY(double matrix21 READ matrix21 WRITE setMatrix21 NOTIFY matrix21Changed)
    Q_PROPERTY(double matrix22 READ matrix22 WRITE setMatrix22 NOTIFY matrix22Changed)
    Q_PROPERTY(double deltaX READ deltaX WRITE setDeltaX NOTIFY deltaXChanged)
    Q_PROPERTY(double deltaY READ deltaY WRITE setDeltaY NOTIFY deltaYChanged) 
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

public slots:
    void setOriginX(double originX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_originX, originX))
            return;

        m_originX = originX;
        emit originXChanged(m_originX);
    }

    void setOriginY(double originY)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_originY, originY))
            return;

        m_originY = originY;
        emit originYChanged(m_originY);
    }

    void setMatrix11(double matrix11)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_matrix11, matrix11))
            return;

        m_matrix11 = matrix11;
        emit matrix11Changed(m_matrix11);
    }

    void setMatrix12(double matrix12)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_matrix12, matrix12))
            return;

        m_matrix12 = matrix12;
        emit matrix12Changed(m_matrix12);
    }

    void setMatrix21(double matrix21)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_matrix21, matrix21))
            return;

        m_matrix21 = matrix21;
        emit matrix21Changed(m_matrix21);
    }

    void setMatrix22(double matrix22)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_matrix22, matrix22))
            return;

        m_matrix22 = matrix22;
        emit matrix22Changed(m_matrix22);
    }

    void setDeltaX(double deltaX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_deltaX, deltaX))
            return;

        m_deltaX = deltaX;
        emit deltaXChanged(m_deltaX);
    }

    void setDeltaY(double deltaY)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_deltaY, deltaY))
            return;

        m_deltaY = deltaY;
        emit deltaYChanged(m_deltaY);
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

private:
    double m_imageWidth = 0;
    double m_imageHeight = 0;
    double m_originX = 0;
    double m_originY = 0;
    double m_matrix11 = 0;
    double m_matrix12 = 0;
    double m_matrix21 = 0;
    double m_matrix22 = 0;
    double m_deltaX = 0;
    double m_deltaY = 0;
};

#endif // CALIBRATION_PARAMETER_H
