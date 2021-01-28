#ifndef AADATA_H
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

    Q_PROPERTY(QVariantList wCCList READ wCCList)
    Q_PROPERTY(QVariantList wULList READ wULList)
    Q_PROPERTY(QVariantList wURList READ wURList)
    Q_PROPERTY(QVariantList wLLList READ wLLList)
    Q_PROPERTY(QVariantList wLRList READ wLRList)

    Q_PROPERTY(QVariantList wCCRealList READ wCCRealList)
    Q_PROPERTY(QVariantList wULRealList READ wULRealList)
    Q_PROPERTY(QVariantList wURRealList READ wURRealList)
    Q_PROPERTY(QVariantList wLLRealList READ wLLRealList)
    Q_PROPERTY(QVariantList wLRRealList READ wLRRealList)

    Q_PROPERTY(QString chartName READ chartName WRITE setChartName)

    Q_PROPERTY(double minValue READ minValue)
    Q_PROPERTY(double maxValue READ maxValue)
    Q_PROPERTY(double negativeDI READ negativeDI)
    Q_PROPERTY(double positiveDI READ positiveDI)

    Q_PROPERTY(double dev READ dev WRITE setDev)
    Q_PROPERTY(double xTilt READ xTilt WRITE setXTilt)
    Q_PROPERTY(double yTilt READ yTilt WRITE setYTilt)
    Q_PROPERTY(double wCCPeakZ READ wCCPeakZ WRITE setWCCPeakZ)
    Q_PROPERTY(double wULPeakZ READ wULPeakZ WRITE setWULPeakZ)
    Q_PROPERTY(double wURPeakZ READ wURPeakZ WRITE setWURPeakZ)
    Q_PROPERTY(double wLLPeakZ READ wLLPeakZ WRITE setWLLPeakZ)
    Q_PROPERTY(double wLRPeakZ READ wLRPeakZ WRITE setWLRPeakZ)
    Q_PROPERTY(double zPeak READ zPeak WRITE setZPeak NOTIFY zPeakChanged)
    Q_PROPERTY(QString layer0 READ layer0 WRITE setLayer0)
    Q_PROPERTY(QString layer1 READ layer1 WRITE setLayer1)
    Q_PROPERTY(QString layer2 READ layer2 WRITE setLayer2)
    Q_PROPERTY(QString layer3 READ layer3 WRITE setLayer3)

    Q_PROPERTY(bool inProgress READ inProgress WRITE setInProgress)

    Q_PROPERTY(int valuesSize READ valuesSize WRITE setValuesSize NOTIFY valuesSizeChanged)
    Q_PROPERTY(int detectedIntensityError READ detectedIntensityError WRITE detectedIntensityError)

    Q_PROPERTY(NOTIFY wValueClear)
    Q_PROPERTY(NOTIFY wValueChanged)
    Q_PROPERTY(NOTIFY wInProgressChanged)
public:
    AAData(QObject *parent=Q_NULLPTR);
    //i is index, x is x-axis data, y will be used for showing curve fit data, rawY will be used for showing raw data
    void addData(int i, double x, double y, double rawY);
    void incrementData(double y1, double y2, double y3, double y4, double y5);
    void plotIntensityProfile(float minI, float maxI, std::vector<float> values, int detectedNumberOfError, double negativeDI, double positiveDI);
    void clear();
    void plot(QString chartName = "Silicool AA");
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

    double zPeak() const
    {
        return m_zPeak;
    }

    QVariantList wCCList() const
    {
        return m_wCCList;
    }

    QVariantList wULList() const
    {
        return m_wULList;
    }

    QVariantList wURList() const
    {
        return m_wURList;
    }

    QVariantList wLLList() const
    {
        return m_wLLList;
    }

    QVariantList wLRList() const
    {
        return m_wLRList;
    }

    QString chartName() const
    {
        return m_chartName;
    }

    QVariantList wCCRealList() const
    {
        return m_wCCRealList;
    }

    QVariantList wULRealList() const
    {
        return m_wULRealList;
    }

    QVariantList wURRealList() const
    {
        return m_wURRealList;
    }

    QVariantList wLLRealList() const
    {
        return m_wLLRealList;
    }

    QVariantList wLRRealList() const
    {
        return m_wLRRealList;
    }

    bool inProgress() const
    {
        return m_inProgress;
    }

    int valuesSize() const
    {
        return m_valuesSize;
    }

    int detectedIntensityError() const
    {
        return m_detectedIntensityError;
    }

    double negativeDI() const
    {
        return m_negativeDI;
    }

    double positiveDI() const
    {
        return m_positiveDI;
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

    void setZPeak(double zPeak)
    {
        if (qFuzzyCompare(m_zPeak, zPeak))
            return;

        m_zPeak = zPeak;
        emit zPeakChanged(m_zPeak);
    }

    void setChartName(QString chartName)
    {
        m_chartName = chartName;
    }

    void setInProgress(bool inProgress)
    {
        m_inProgress = inProgress;
        emit wInProgressChanged();
    }

    void setValuesSize(int valuesSize)
    {
        m_valuesSize = valuesSize;
        emit valuesSizeChanged(m_valuesSize);
    }

    void detectedIntensityError(int detectedIntensityError)
    {
        m_detectedIntensityError = detectedIntensityError;
    }

signals:
    void wValueChanged();
    void wValueClear();
    void zPeakChanged(double zPeak);
    void wInProgressChanged();
    void valuesSizeChanged(int valuesSize);

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

    double m_positiveDI;

    double m_negativeDI;

    double m_zPeak;

    QVariantList m_wCCList;

    QVariantList m_wULList;

    QVariantList m_wURList;

    QVariantList m_wLLList;

    QVariantList m_wLRList;

    QString m_chartName;

    QVariantList m_wCCRealList;

    QVariantList m_wULRealList;

    QVariantList m_wURRealList;

    QVariantList m_wLLRealList;

    QVariantList m_wLRRealList;

    bool m_inProgress;

    int m_valuesSize = 10000;

    int m_detectedIntensityError;

protected:
    void run();
};


#endif // AADATA_H
