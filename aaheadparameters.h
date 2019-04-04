#ifndef AAHEADPARAMETERS_H
#define AAHEADPARAMETERS_H

#include <QObject>
#include <propertybase.h>

class AAHeadParameters : public PropertyBase
{
    Q_OBJECT
    double m_pickLensPositionX;
    double m_pickLensPositionY;

public:
    explicit AAHeadParameters(QObject *parent = nullptr);
    Q_PROPERTY(double pickLensPositionX READ pickLensPositionX WRITE setPickLensPositionX NOTIFY valueChanged);
    Q_PROPERTY(double pickLensPositionY READ pickLensPositionY WRITE setPickLensPositionY NOTIFY valueChanged);

double pickLensPositionX() const
{
    return m_pickLensPositionX;
}

double pickLensPositionY() const
{
    return m_pickLensPositionY;
}

signals:
    void valueChanged(double);
public slots:

    void setPickLensPositionX(double pickLensPositionX)
    {
        qInfo("%f ", pickLensPositionX);
        m_pickLensPositionX = pickLensPositionX;
        emit valueChanged(m_pickLensPositionX);
    }
    void setPickLensPositionY(double pickLensPositionY)
    {
        qInfo("%f ", pickLensPositionY);
        m_pickLensPositionY = pickLensPositionY;
        emit valueChanged(m_pickLensPositionY);
    }
};

#endif // AAHEADPARAMETERS_H
