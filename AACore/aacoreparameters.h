#ifndef AACOREPARAMETERS_H
#define AACOREPARAMETERS_H

#include <QObject>
#include <QVariant>
#include "Utils/propertybase.h"
#include "Utils/config.h"

class AACoreParameters : public PropertyBase
{
    Q_OBJECT
    double m_EFL = 1.98;

    int m_MaxIntensity = 50;

    int m_MinArea = 10000;

    int m_MaxArea = 90000;

    int m_SensorXRatio = 892;

    int m_SensorYRatio = 892;

    double m_ROIRatio = 1.45;

    bool m_firstRejectSensor = false;

    int m_rejectTimes = 3;

    QVariantList m_WeightList;

    double m_EstimatedFOVSlope = -15;

    double m_EstimatedAAFOV = 70;

    double m_FrequencyForCheckGlue = 5;

    int m_tiltRelationship = 0;

    int m_aaScanMTFFrequency = 1;

    int m_aaScanCurveFitOrder = 4;

    int m_aaScanOversampling = 0;

    int m_aaScanCurveFitErrorThreshold = -10;

    int m_mtfFrequency = 1;

    QString m_x1 = "";

    QString m_y1;

    QString m_z1;

QString m_rx1;

QString m_ry1;

QString m_rz1;

QString m_x2;

QString m_y2;

QString m_z2;

QString m_rx2;

QString m_ry2;

QString m_rz2;

public:
    explicit AACoreParameters(){
        for (int i = 0; i < 4*4; i++) // 4 field of view * 4 edge number
        {
            m_WeightList.push_back(QVariant(0.25));
        }
    }
    Q_PROPERTY(double EFL READ EFL WRITE setEFL NOTIFY paramsChanged)
    Q_PROPERTY(int MaxIntensity READ MaxIntensity WRITE setMaxIntensity NOTIFY paramsChanged)
    Q_PROPERTY(int MinArea READ MinArea WRITE setMinArea NOTIFY paramsChanged)
    Q_PROPERTY(int MaxArea READ MaxArea WRITE setMaxArea NOTIFY paramsChanged)
    Q_PROPERTY(int SensorXRatio READ SensorXRatio WRITE setSensorXRatio NOTIFY paramsChanged)
    Q_PROPERTY(int SensorYRatio READ SensorYRatio WRITE setSensorYRatio NOTIFY paramsChanged)
    Q_PROPERTY(double ROIRatio READ ROIRatio WRITE setROIRatio NOTIFY paramsChanged)
    Q_PROPERTY(QVariantList WeightList READ WeightList WRITE setWeightList NOTIFY paramsChanged)
    Q_PROPERTY(bool firstRejectSensor READ firstRejectSensor WRITE setFirstRejectSensor NOTIFY firstRejectSensorChanged)
    Q_PROPERTY(int rejectTimes READ rejectTimes WRITE setRejectTimes NOTIFY rejectTimesChanged)
    Q_PROPERTY(double EstimatedFOVSlope READ EstimatedFOVSlope WRITE setEstimatedFOVSlope NOTIFY paramsChanged)
    Q_PROPERTY(double EstimatedAAFOV READ EstimatedAAFOV WRITE setEstimatedAAFOV NOTIFY paramsChanged)
    Q_PROPERTY(double FrequencyForCheckGlue READ FrequencyForCheckGlue WRITE setFrequencyForCheckGlue NOTIFY FrequencyForCheckGlueChanged)
    Q_PROPERTY(int tiltRelationship READ tiltRelationship WRITE setTiltRelationship NOTIFY tiltRelationshipChanged)
    Q_PROPERTY(int mtfFrequency READ mtfFrequency WRITE setMTFFrequency NOTIFY mtfFrequencyChanged)
    Q_PROPERTY(int aaScanMTFFrequency READ aaScanMTFFrequency WRITE setAAScanMTFFrequency NOTIFY aaScanMTFFrequencyChanged)
    Q_PROPERTY(int aaScanCurveFitOrder READ aaScanCurveFitOrder WRITE setAAScanCurveFitOrder NOTIFY aaScanCurveFitOrderChanged)
    Q_PROPERTY(int aaScanOversampling READ aaScanOversampling WRITE setAAScanOversampling NOTIFY aaScanOversamplingChanged)
    Q_PROPERTY(int aaScanCurveFitErrorThreshold READ aaScanCurveFitErrorThreshold WRITE setAAScanCurveFitErrorThreshold NOTIFY aaScanCurveFitErrorThresholdChanged)

    Q_PROPERTY(QString x1 READ x1 WRITE setX1 NOTIFY x1Changed)
    Q_PROPERTY(QString y1 READ y1 WRITE setY1 NOTIFY y1Changed)
    Q_PROPERTY(QString z1 READ z1 WRITE setZ1 NOTIFY z1Changed)
    Q_PROPERTY(QString rx1 READ rx1 WRITE setRX1 NOTIFY rx1Changed)
    Q_PROPERTY(QString ry1 READ ry1 WRITE setRY1 NOTIFY ry1Changed)
    Q_PROPERTY(QString rz1 READ rz1 WRITE setRZ1 NOTIFY rz1Changed)

    Q_PROPERTY(QString x2 READ x2 WRITE setX2 NOTIFY x2Changed)
    Q_PROPERTY(QString y2 READ y2 WRITE setY2 NOTIFY y2Changed)
    Q_PROPERTY(QString z2 READ z2 WRITE setZ2 NOTIFY z2Changed)
    Q_PROPERTY(QString rx2 READ rx2 WRITE setRX2 NOTIFY rx2Changed)
    Q_PROPERTY(QString ry2 READ ry2 WRITE setRY2 NOTIFY ry2Changed)
    Q_PROPERTY(QString rz2 READ rz2 WRITE setRZ2 NOTIFY rz2Changed)

    double EFL() const
    {
        return m_EFL;
    }
    int MaxIntensity() const
    {
        return m_MaxIntensity;
    }

    int MinArea() const
    {
        return m_MinArea;
    }

    int MaxArea() const
    {
        return m_MaxArea;
    }

    int SensorXRatio() const
    {
        return m_SensorXRatio;
    }

    int SensorYRatio() const
    {
        return m_SensorYRatio;
    }

    double ROIRatio() const
    {
        return m_ROIRatio;
    }

    bool firstRejectSensor() const
    {
        return m_firstRejectSensor;
    }

    int rejectTimes() const
    {
        return m_rejectTimes;
    }

    QVariantList WeightList() const
    {
        return m_WeightList;
    }

    double EstimatedFOVSlope() const
    {
        return m_EstimatedFOVSlope;
    }

    double EstimatedAAFOV() const
    {
        return m_EstimatedAAFOV;
    }

    double FrequencyForCheckGlue() const
    {
        return m_FrequencyForCheckGlue;
    }

    int tiltRelationship() const
    {
        return m_tiltRelationship;
    }

    int aaScanMTFFrequency() const
    {
        return m_aaScanMTFFrequency;
    }

    int aaScanCurveFitOrder() const
    {
        return m_aaScanCurveFitOrder;
    }

    int aaScanOversampling() const
    {
        return m_aaScanOversampling;
    }

    int aaScanCurveFitErrorThreshold() const
    {
        return m_aaScanCurveFitErrorThreshold;
    }

    int mtfFrequency() const
    {
        return m_mtfFrequency;
    }

    QString x1() const
    {
        return m_x1;
    }

    QString y1() const
    {
        return m_y1;
    }

    QString z1() const
    {
        return m_z1;
    }

    QString rx1() const
    {
        return m_rx1;
    }

    QString ry1() const
    {
        return m_ry1;
    }

    QString rz1() const
    {
        return m_rz1;
    }

    QString x2() const
    {
        return m_x2;
    }

    QString y2() const
    {
        return m_y2;
    }

    QString z2() const
    {
        return m_z2;
    }

    QString rx2() const
    {
        return m_rx2;
    }

    QString ry2() const
    {
        return m_ry2;
    }

    QString rz2() const
    {
        return m_rz2;
    }

public slots:
    void setEFL(double EFL)
    {
        m_EFL = EFL;
        emit paramsChanged();
    }
    void setMaxIntensity(int MaxIntensity)
    {
        m_MaxIntensity = MaxIntensity;
        emit paramsChanged();
    }

    void setMinArea(int MinArea)
    {
        m_MinArea = MinArea;
        emit paramsChanged();
    }

    void setMaxArea(int MaxArea)
    {
        m_MaxArea = MaxArea;
        emit paramsChanged();
    }

    void setSensorXRatio(int SensorXRatio)
    {
        m_SensorXRatio = SensorXRatio;
        emit paramsChanged();
    }

    void setSensorYRatio(int SensorYRatio)
    {
        m_SensorYRatio = SensorYRatio;
        emit paramsChanged();
    }

    void setROIRatio(double ROIRatio)
    {
        m_ROIRatio = ROIRatio;
        emit paramsChanged();
    }

    void setFirstRejectSensor(bool firstRejectSensor)
    {
        if (m_firstRejectSensor == firstRejectSensor)
            return;

        m_firstRejectSensor = firstRejectSensor;
        emit firstRejectSensorChanged(m_firstRejectSensor);
    }

    void setRejectTimes(int rejectTimes)
    {
        if (m_rejectTimes == rejectTimes)
            return;

        m_rejectTimes = rejectTimes;
        emit rejectTimesChanged(m_rejectTimes);
    }

    void setWeightValue(int index, double value)
    {
        qInfo("Set Weight Value: %d %f", index, value);
        m_WeightList[index].setValue(value);
        emit paramsChanged();
    }

    void setWeightList(QVariantList WeightList)
    {
        if (m_WeightList == WeightList)
            return;

        m_WeightList = WeightList;
        emit paramsChanged();
    }

    void setEstimatedFOVSlope(double EstimatedFOVSlope)
    {
        m_EstimatedFOVSlope = EstimatedFOVSlope;
        emit paramsChanged();
    }

    void setEstimatedAAFOV(double EstimatedAAFOV)
    {
        m_EstimatedAAFOV = EstimatedAAFOV;
        emit paramsChanged();
    }

    void setFrequencyForCheckGlue(double FrequencyForCheckGlue)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_FrequencyForCheckGlue, FrequencyForCheckGlue))
            return;

        m_FrequencyForCheckGlue = FrequencyForCheckGlue;
        emit FrequencyForCheckGlueChanged(m_FrequencyForCheckGlue);
    }

    void setTiltRelationship(int tiltRelationship)
    {
        if (m_tiltRelationship == tiltRelationship)
            return;

        m_tiltRelationship = tiltRelationship;
        emit tiltRelationshipChanged(m_tiltRelationship);
    }

    void setAAScanMTFFrequency(int aaScanMTFFrequency)
    {
        if (m_aaScanMTFFrequency == aaScanMTFFrequency)
            return;

        m_aaScanMTFFrequency = aaScanMTFFrequency;
        emit aaScanMTFFrequencyChanged(m_aaScanMTFFrequency);
    }

    void setAAScanCurveFitOrder(int aaScanCurveFitOrder)
    {
        if (m_aaScanCurveFitOrder == aaScanCurveFitOrder)
            return;

        m_aaScanCurveFitOrder = aaScanCurveFitOrder;
        emit aaScanCurveFitOrderChanged(m_aaScanCurveFitOrder);
    }

    void setAAScanOversampling(int aaScanOversampling)
    {
        if (m_aaScanOversampling == aaScanOversampling)
            return;

        m_aaScanOversampling = aaScanOversampling;
        emit aaScanOversamplingChanged(m_aaScanOversampling);
    }

    void setAAScanCurveFitErrorThreshold(int aaScanCurveFitErrorThreshold)
    {
        if (m_aaScanCurveFitErrorThreshold == aaScanCurveFitErrorThreshold)
            return;

        m_aaScanCurveFitErrorThreshold = aaScanCurveFitErrorThreshold;
        emit aaScanCurveFitErrorThresholdChanged(m_aaScanCurveFitErrorThreshold);
    }

    void setMTFFrequency(int mtfFrequency)
    {
        if (m_mtfFrequency == mtfFrequency)
            return;

        m_mtfFrequency = mtfFrequency;
        emit mtfFrequencyChanged(m_mtfFrequency);
    }

    void setX1(QString x1)
    {
        if (m_x1 == x1)
            return;

        m_x1 = x1;
        emit x1Changed(m_x1);
    }

    void setY1(QString y1)
    {
        if (m_y1 == y1)
            return;

        m_y1 = y1;
        emit y1Changed(m_y1);
    }

    void setZ1(QString z1)
    {
        if (m_z1 == z1)
            return;

        m_z1 = z1;
        emit z1Changed(m_z1);
    }

    void setRX1(QString rx1)
    {
        if (m_rx1 == rx1)
            return;

        m_rx1 = rx1;
        emit rx1Changed(m_rx1);
    }

    void setRY1(QString ry1)
    {
        if (m_ry1 == ry1)
            return;

        m_ry1 = ry1;
        emit ry1Changed(m_ry1);
    }

    void setRZ1(QString rz1)
    {
        if (m_rz1 == rz1)
            return;

        m_rz1 = rz1;
        emit rz1Changed(m_rz1);
    }

    void setX2(QString x2)
    {
        if (m_x2 == x2)
            return;

        m_x2 = x2;
        emit x2Changed(m_x2);
    }

    void setY2(QString y2)
    {
        if (m_y2 == y2)
            return;

        m_y2 = y2;
        emit y2Changed(m_y2);
    }

    void setZ2(QString z2)
    {
        if (m_z2 == z2)
            return;

        m_z2 = z2;
        emit z2Changed(m_z2);
    }

    void setRX2(QString rx2)
    {
        if (m_rx2 == rx2)
            return;

        m_rx2 = rx2;
        emit rx2Changed(m_rx2);
    }

    void setRY2(QString ry2)
    {
        if (m_ry2 == ry2)
            return;

        m_ry2 = ry2;
        emit ry2Changed(m_ry2);
    }

    void setRZ2(QString rz2)
    {
        if (m_rz2 == rz2)
            return;

        m_rz2 = rz2;
        emit rz2Changed(m_rz2);
    }

signals:
    void paramsChanged();
    void firstRejectSensorChanged(bool firstRejectSensor);
    void rejectTimesChanged(int rejectTimes);
    void FrequencyForCheckGlueChanged(double FrequencyForCheckGlue);
    void tiltRelationshipChanged(int tiltRelationship);
    void aaScanMTFFrequencyChanged(int aaScanMTFFrequency);
    void aaScanCurveFitOrderChanged(int aaScanCurveFitOrder);
    void aaScanOversamplingChanged(int aaScanOversampling);
    void aaScanCurveFitErrorThresholdChanged(int aaScanCurveFitErrorThreshold);
    void mtfFrequencyChanged(int mtfFrequency);
    void x1Changed(QString x1);
    void y1Changed(QString y1);
    void z1Changed(QString z1);
    void rx1Changed(QString rx1);
    void ry1Changed(QString ry1);
    void rz1Changed(QString rz1);
    void x2Changed(QString x2);
    void y2Changed(QString y2);
    void z2Changed(QString z2);
    void rx2Changed(QString rx2);
    void ry2Changed(QString ry2);
    void rz2Changed(QString rz2);
};
class AACoreStates: public PropertyBase
{
    Q_OBJECT
public:
    explicit AACoreStates(){}
    bool needSendLensRequest(){return(!hasLens())&&(!isWaitingLens());}
    bool needSendSensorRequest(){return (!hasSensor())&&(!isWaitingSensor());}

    Q_PROPERTY(bool isWaitingLens READ isWaitingLens WRITE setIsWaitingLens NOTIFY isWaitingLensChanged)
    Q_PROPERTY(bool isWaitingSensor READ isWaitingSensor WRITE setIsWaitingSensor NOTIFY isWaitingSensorChanged)
    Q_PROPERTY(bool hasLens READ hasLens WRITE setHasLens NOTIFY hasLensChanged)
    Q_PROPERTY(bool hasNgLens READ hasNgLens WRITE setHasNgLens NOTIFY hasNgLensChanged)
    Q_PROPERTY(bool hasSensor READ hasSensor WRITE setHasSensor NOTIFY hasSensorChanged)
    Q_PROPERTY(bool hasNgSensor READ hasNgSensor WRITE setHasNgSensor NOTIFY hasNgSensorChanged)
    Q_PROPERTY(bool hasProduct READ hasProduct WRITE setHasProduct NOTIFY hasProductChanged)
    Q_PROPERTY(bool hasNgProduct READ hasNgProduct WRITE setHasNgProduct NOTIFY hasNgProductChanged)
    bool isWaitingLens() const
    {
        return m_isWaitingLens;
    }
    bool isWaitingSensor() const
    {
        return m_isWaitingSensor;
    }

    bool hasLens() const
    {
        return m_hasLens;
    }

    bool hasNgLens() const
    {
        return m_hasNgLens;
    }

    bool hasSensor() const
    {
        return m_hasSensor;
    }

    bool hasNgSensor() const
    {
        return m_hasNgSensor;
    }

    bool hasProduct() const
    {
        return m_hasProduct;
    }

    bool hasNgProduct() const
    {
        return m_hasNgProduct;
    }

public slots:
    void setIsWaitingLens(bool isWaitingLens)
    {
        if (m_isWaitingLens == isWaitingLens)
            return;

        m_isWaitingLens = isWaitingLens;
        emit isWaitingLensChanged(m_isWaitingLens);
    }
    void setIsWaitingSensor(bool isWaitingSensor)
    {
        if (m_isWaitingSensor == isWaitingSensor)
            return;

        m_isWaitingSensor = isWaitingSensor;
        emit isWaitingSensorChanged(m_isWaitingSensor);
    }

    void setHasLens(bool hasLens)
    {
        if (m_hasLens == hasLens)
            return;

        m_hasLens = hasLens;
        emit hasLensChanged(m_hasLens);
    }

    void setHasNgLens(bool hasNgLens)
    {
        if (m_hasNgLens == hasNgLens)
            return;

        m_hasNgLens = hasNgLens;
        emit hasNgLensChanged(m_hasNgLens);
    }

    void setHasSensor(bool hasSensor)
    {
        if (m_hasSensor == hasSensor)
            return;

        m_hasSensor = hasSensor;
        emit hasSensorChanged(m_hasSensor);
    }

    void setHasNgSensor(bool hasNgSensor)
    {
        if (m_hasNgSensor == hasNgSensor)
            return;

        m_hasNgSensor = hasNgSensor;
        emit hasNgSensorChanged(m_hasNgSensor);
    }

    void setHasProduct(bool hasProduct)
    {
        if (m_hasProduct == hasProduct)
            return;

        m_hasProduct = hasProduct;
        emit hasProductChanged(m_hasProduct);
    }

    void setHasNgProduct(bool hasNgProduct)
    {
        if (m_hasNgProduct == hasNgProduct)
            return;

        m_hasNgProduct = hasNgProduct;
        emit hasNgProductChanged(m_hasNgProduct);
    }

signals:
    void isWaitingLensChanged(bool isWaitingLens);
    void isWaitingSensorChanged(bool isWaitingSensor);

    void hasLensChanged(bool hasLens);

    void hasNgLensChanged(bool hasNgLens);

    void hasSensorChanged(bool hasSensor);

    void hasNgSensorChanged(bool hasNgSensor);

    void hasProductChanged(bool hasProduct);

    void hasNgProductChanged(bool hasNgProduct);

private:
    bool m_isWaitingLens = false;
    bool m_isWaitingSensor = false;
    bool m_hasLens = false;
    bool m_hasNgLens = false;
    bool m_hasSensor = false;
    bool m_hasNgSensor = false;
    bool m_hasProduct = false;
    bool m_hasNgProduct = false;
};

#endif // AACOREPARAMETERS_H
