#ifndef AAHEADPARAMETERS_H
#define AAHEADPARAMETERS_H

#include <QObject>
#include <propertybase.h>
#include "config.h"

class AAHeadParameters : public PropertyBase
{
    Q_OBJECT
    double m_pickLensPositionX;
    double m_pickLensPositionY;

    QString m_visionUplookPR;

public:
    explicit AAHeadParameters(QObject *parent = nullptr);
    Q_PROPERTY(double pickLensPositionX READ pickLensPositionX WRITE setPickLensPositionX NOTIFY valueChanged);
    Q_PROPERTY(double pickLensPositionY READ pickLensPositionY WRITE setPickLensPositionY NOTIFY valueChanged);
    Q_PROPERTY(QString visionUplookPR READ visionUplookPR WRITE setVisionUplookPR NOTIFY prValueChanged);

private:
    void saveJsonConfig();
    void loadJsonConfig();
double pickLensPositionX() const
{
    return m_pickLensPositionX;
}

double pickLensPositionY() const
{
    return m_pickLensPositionY;
}

QString visionUplookPR() const
{
    return m_visionUplookPR;
}

signals:
void valueChanged(double);
void prValueChanged(QString visionUplookPR);

public slots:

    void setPickLensPositionX(double pickLensPositionX)
    {
        qInfo("%f ", pickLensPositionX);
        m_pickLensPositionX = pickLensPositionX;
        emit valueChanged(m_pickLensPositionX);
        this->saveJsonConfig();
    }
    void setPickLensPositionY(double pickLensPositionY)
    {
        qInfo("%f ", pickLensPositionY);
        m_pickLensPositionY = pickLensPositionY;
        emit valueChanged(m_pickLensPositionY);
        this->saveJsonConfig();
    }
    void setVisionUplookPR(QString visionUplookPR)
    {
        qInfo("%f ", visionUplookPR.toStdString().c_str());
        m_visionUplookPR = visionUplookPR;
        emit prValueChanged(m_visionUplookPR);
        this->saveJsonConfig();
    }
};

#endif // AAHEADPARAMETERS_H
