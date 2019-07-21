﻿#ifndef AADATA_H
#define AADATA_H

#include <QObject>
#include <QPointF>
#include <QTimer>
#include <QList>
#include <QPointF>
#include <QThread>

class AAData : public QThread
{
    Q_OBJECT
    Q_PROPERTY(QPointF wValue READ wValue NOTIFY wValueChanged)
    Q_PROPERTY(QPointF wCCValue READ wCCValue)
    Q_PROPERTY(QPointF wULValue READ wULValue)
    Q_PROPERTY(QPointF wURValue READ wURValue)
    Q_PROPERTY(QPointF wLLValue READ wLLValue)
    Q_PROPERTY(QPointF wLRValue READ wLRValue)

    Q_PROPERTY(double minValue READ minValue)
    Q_PROPERTY(double maxValue READ maxValue)

    Q_PROPERTY(double dev READ dev WRITE setDev)
    Q_PROPERTY(double xTilt READ xTilt WRITE setXTilt)
    Q_PROPERTY(double yTilt READ yTilt WRITE setYTilt)
    Q_PROPERTY(double wCCPeakZ READ wCCPeakZ WRITE setWCCPeakZ)
    Q_PROPERTY(double wULPeakZ READ wULPeakZ WRITE setWULPeakZ)
    Q_PROPERTY(double wURPeakZ READ wURPeakZ WRITE setWURPeakZ)
    Q_PROPERTY(double wLLPeakZ READ wLLPeakZ WRITE setWLLPeakZ)
    Q_PROPERTY(double wLRPeakZ READ wLRPeakZ WRITE setWLRPeakZ)
    Q_PROPERTY(QString layer0 READ layer0 WRITE setLayer0)
    Q_PROPERTY(QString layer1 READ layer1 WRITE setLayer1)
    Q_PROPERTY(QString layer2 READ layer2 WRITE setLayer2)
    Q_PROPERTY(QString layer3 READ layer3 WRITE setLayer3)

    Q_PROPERTY(NOTIFY wValueClear)
    Q_PROPERTY(NOTIFY wValueChanged)

public:
    AAData(QObject *parent=Q_NULLPTR);
    void addData(int i, double x, double y);
    void incrementData(double y1, double y2, double y3, double y4, double y5);
    void plotIntensityProfile(float minI, float maxI, std::vector<float> values);
    void clear();
    void plot();
    QPointF wValue() const{
        return m_wValue;
    }
    QPointF wCCValue() const
    {
        return m_wCCValue;
    }

    QPointF wULValue() const
    {
        return m_wULValue;
    }

    QPointF wURValue() const
    {
        return m_wURValue;
    }

    QPointF wLLValue() const
    {
        return m_wLLValue;
    }

    QPointF wLRValue() const
    {
        return m_wLRValue;
    }

    double dev() const
    {
        return m_dev;
    }

    double wCCPeakZ() const
    {
        return m_wCCPeakZ;
    }

    double wULPeakZ() const
    {
        return m_wULPeakZ;
    }

    double wURPeakZ() const
    {
        return m_wURPeakZ;
    }

    double wLLPeakZ() const
    {
        return m_wLLPeakZ;
    }

    double wLRPeakZ() const
    {
        return m_wLRPeakZ;
    }

    double xTilt() const
    {
        return m_xTilt;
    }

    double yTilt() const
    {
        return m_yTilt;
    }

    QString layer1() const
    {
        return m_layer1;
    }

    QString layer0() const
    {
        return m_layer0;
    }

    QString layer2() const
    {
        return m_layer2;
    }

    QString layer3() const
    {
        return m_layer3;
    }

    double minValue() const
    {
        return m_minValue;
    }

    double maxValue() const
    {
        return m_maxValue;
    }

public slots:
    void setDev(double dev)
    {
        m_dev = dev;
    }

    void setWCCPeakZ(double wCCPeakZ)
    {
        m_wCCPeakZ = wCCPeakZ;
    }

    void setWULPeakZ(double wULPeakZ)
    {
        m_wULPeakZ = wULPeakZ;
    }

    void setWURPeakZ(double wURPeakZ)
    {
        m_wURPeakZ = wURPeakZ;
    }

    void setWLRPeakZ(double wLRPeakZ)
    {
        m_wLRPeakZ = wLRPeakZ;
    }

    void setWLLPeakZ(double wLLPeakZ)
    {
        m_wLLPeakZ = wLLPeakZ;
    }

    void setXTilt(double xTilt)
    {
        m_xTilt = xTilt;
    }

    void setYTilt(double yTilt)
    {
        m_yTilt = yTilt;
    }

    void setLayer1(QString layer1)
    {
        m_layer1 = layer1;
    }

    void setLayer0(QString layer0)
    {
        m_layer0 = layer0;
    }

    void setLayer2(QString layer2)
    {
        m_layer2 = layer2;
    }

    void setLayer3(QString layer3)
    {
        m_layer3 = layer3;
    }

signals:
    void wValueChanged();
    void wValueClear();
private slots:
    void wTimeout();
private:
    int count = 0;
    int plotProfile = 0;
    QTimer * m_wTimer;
    QPointF m_wValue;
    QList<QPointF> m_PointsList;
    QList<QPointF> m_CCPointsList;
    QList<QPointF> m_ULPointsList;
    QList<QPointF> m_URPointsList;
    QList<QPointF> m_LLPointsList;
    QList<QPointF> m_LRPointsList;
    QPointF m_wCCValue;
    QPointF m_wULValue;
    QPointF m_wURValue;
    QPointF m_wLLValue;
    QPointF m_wLRValue;

    double m_dev;

    double m_wCCPeakZ;

    double m_wULPeakZ;

    double m_wURPeakZ;

    double m_wLLPeakZ;

    double m_wLRPeakZ;

    double m_xTilt;

    double m_yTilt;

    QString m_layer1;

    QString m_layer0;

    QString m_layer2;

    QString m_layer3;

    double m_minValue;

    double m_maxValue;

protected:
    void run();
};


#endif // AADATA_H