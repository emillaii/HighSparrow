#ifndef AAHEADPARAMETERS_H
#define AAHEADPARAMETERS_H

#include <QObject>
#include <propertybase.h>
#include "config.h"
#include <QChart>
#include <QSplineSeries>
using namespace QtCharts;

class AAHeadParameters : public PropertyBase
{
    Q_OBJECT
public:
    explicit AAHeadParameters(){}
    Q_PROPERTY(double PickLensPositionX READ PickLensPositionX WRITE setPickLensPositionX NOTIFY PickLensPositionXChanged)
    Q_PROPERTY(double PickLensPositionY READ PickLensPositionY WRITE setPickLensPositionY NOTIFY PickLensPositionYChanged)
    Q_PROPERTY(double PickLensPositionZ READ PickLensPositionZ WRITE setPickLensPositionZ NOTIFY PickLensPositionZChanged)
    Q_PROPERTY(double OCPositionX READ OCPositionX WRITE setOCPositionX NOTIFY OCPositionXChanged)
    Q_PROPERTY(double OCPositionY READ OCPositionX WRITE setOCPositionX NOTIFY OCPositionXChanged)
    Q_PROPERTY(double OCPositionZ READ OCPositionX WRITE setOCPositionX NOTIFY OCPositionXChanged)

    double PickLensPositionX() const
    {
        return m_PickLensPositionX;
    }
    double PickLensPositionY() const
    {
        return m_PickLensPositionY;
    }

    double PickLensPositionZ() const
    {
        return m_PickLensPositionZ;
    }

    double OCPositionX() const
    {
        return m_OCPositionX;
    }

public slots:
    void setPickLensPositionX(double PickLensPositionX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_PickLensPositionX, PickLensPositionX))
            return;

        m_PickLensPositionX = PickLensPositionX;
        emit PickLensPositionXChanged(m_PickLensPositionX);
    }
    void setPickLensPositionY(double PickLensPositionY)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_PickLensPositionY, PickLensPositionY))
            return;

        m_PickLensPositionY = PickLensPositionY;
        emit PickLensPositionYChanged(m_PickLensPositionY);
    }

    void setPickLensPositionZ(double PickLensPositionZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_PickLensPositionZ, PickLensPositionZ))
            return;

        m_PickLensPositionZ = PickLensPositionZ;
        emit PickLensPositionZChanged(m_PickLensPositionZ);
    }

    void setOCPositionX(double OCPositionX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_OCPositionX, OCPositionX))
            return;

        m_OCPositionX = OCPositionX;
        emit OCPositionXChanged(m_OCPositionX);
    }

signals:
    void PickLensPositionXChanged(double PickLensPositionX);
    void PickLensPositionYChanged(double PickLensPositionY);

    void PickLensPositionZChanged(double PickLensPositionZ);

    void OCPositionXChanged(double OCPositionX);

private:
    double m_PickLensPositionX;
    double m_PickLensPositionY;
    double m_PickLensPositionZ;
    double m_OCPositionX;
    double m_OCPositionY;
    double m_OCPositionZ;
};

#endif // AAHEADPARAMETERS_H
