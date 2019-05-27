#ifndef SINGLEHEADMACHINEMATERIALLOADERMODULEPARAMETER_H
#define SINGLEHEADMACHINEMATERIALLOADERMODULEPARAMETER_H

#include "propertybase.h"



class SingleHeadMachineMaterialLoaderModuleParameter:public PropertyBase
{
    Q_OBJECT
    int m_runMode = 0;
    double m_saftyZHeight;
    double m_vcm1BaseHeight;
    double m_vcm2BaseHeight;
    double m_vcmSafeHeight;
    double m_vcm1Svel;
    double m_vcm1Sacc;
    double m_vcm1PickForce;
    double m_vcm1Margin;
    double m_vcm2Svel;
    double m_vcm2Sacc;
    double m_vcm2PickForce;
    double m_vcm2Margin;
    int m_vcm1FinishDelay;
    int m_vcm2FinishDelay;

public:
    SingleHeadMachineMaterialLoaderModuleParameter():PropertyBase(){}
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(double saftyZHeight READ saftyZHeight WRITE setSaftyZHeight NOTIFY saftyZHeightChanged)
    Q_PROPERTY(double vcm1BaseHeight READ vcm1BaseHeight WRITE setVcm1BaseHeight NOTIFY vcm1BaseHeightChanged)
    Q_PROPERTY(double vcm2BaseHeight READ vcm2BaseHeight WRITE setVcm2BaseHeight NOTIFY vcm2BaseHeightChanged)
    Q_PROPERTY(double vcmSafeHeight READ vcmSafeHeight WRITE setVcmSafeHeight NOTIFY vcmSafeHeightChanged)
    Q_PROPERTY(double vcm1Svel READ vcm1Svel WRITE setVcm1Svel NOTIFY vcm1SvelChanged)
    Q_PROPERTY(double vcm1Sacc READ vcm1Sacc WRITE setVcm1Sacc NOTIFY vcm1SaccChanged)
    Q_PROPERTY(double vcm1PickForce READ vcm1PickForce WRITE setVcm1PickForce NOTIFY vcm1PickForceChanged)
    Q_PROPERTY(double vcm1Margin READ vcm1Margin WRITE setVcm1Margin NOTIFY vcm1MarginChanged)
    Q_PROPERTY(double vcm2Svel READ vcm2Svel WRITE setVcm2Svel NOTIFY vcm2SvelChanged)
    Q_PROPERTY(double vcm2Sacc READ vcm2Sacc WRITE setVcm2Sacc NOTIFY vcm2SaccChanged)
    Q_PROPERTY(double vcm2PickForce READ vcm2PickForce WRITE setVcm2PickForce NOTIFY vcm2PickForceChanged)
    Q_PROPERTY(double vcm2Margin READ vcm2Margin WRITE setVcm2Margin NOTIFY vcm2MarginChanged)
    Q_PROPERTY(int vcm1FinishDelay READ vcm1FinishDelay WRITE setVcm1FinishDelay NOTIFY vcm1FinishDelayChanged)
    Q_PROPERTY(int vcm2FinishDelay READ vcm2FinishDelay WRITE setVcm2FinishDelay NOTIFY vcm2FinishDelayChanged)


    int runMode() const
    {
        return m_runMode;
    }
    double saftyZHeight() const
    {
        return m_saftyZHeight;
    }

    double vcm1BaseHeight() const
    {
        return m_vcm1BaseHeight;
    }

    double vcm2BaseHeight() const
    {
        return m_vcm2BaseHeight;
    }

    double vcmSafeHeight() const
    {
        return m_vcmSafeHeight;
    }

    double vcm1Svel() const
    {
        return m_vcm1Svel;
    }

    double vcm1Sacc() const
    {
        return m_vcm1Sacc;
    }

    double vcm1PickForce() const
    {
        return m_vcm1PickForce;
    }

    double vcm1Margin() const
    {
        return m_vcm1Margin;
    }

    double vcm2Svel() const
    {
        return m_vcm2Svel;
    }

    double vcm2Sacc() const
    {
        return m_vcm2Sacc;
    }

    double vcm2PickForce() const
    {
        return m_vcm2PickForce;
    }

    double vcm2Margin() const
    {
        return m_vcm2Margin;
    }

    int vcm1FinishDelay() const
    {
        return m_vcm1FinishDelay;
    }

    int vcm2FinishDelay() const
    {
        return m_vcm2FinishDelay;
    }

public slots:
    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }
    void setSaftyZHeight(double saftyZHeight)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_saftyZHeight, saftyZHeight))
            return;

        m_saftyZHeight = saftyZHeight;
        emit saftyZHeightChanged(m_saftyZHeight);
    }

    void setVcm1BaseHeight(double vcm1BaseHeight)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcm1BaseHeight, vcm1BaseHeight))
            return;

        m_vcm1BaseHeight = vcm1BaseHeight;
        emit vcm1BaseHeightChanged(m_vcm1BaseHeight);
    }

    void setVcm2BaseHeight(double vcm2BaseHeight)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcm2BaseHeight, vcm2BaseHeight))
            return;

        m_vcm2BaseHeight = vcm2BaseHeight;
        emit vcm2BaseHeightChanged(m_vcm2BaseHeight);
    }

    void setVcmSafeHeight(double vcmSafeHeight)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcmSafeHeight, vcmSafeHeight))
            return;

        m_vcmSafeHeight = vcmSafeHeight;
        emit vcmSafeHeightChanged(m_vcmSafeHeight);
    }

    void setVcm1Svel(double vcm1Svel)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcm1Svel, vcm1Svel))
            return;

        m_vcm1Svel = vcm1Svel;
        emit vcm1SvelChanged(m_vcm1Svel);
    }

    void setVcm1Sacc(double vcm1Sacc)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcm1Sacc, vcm1Sacc))
            return;

        m_vcm1Sacc = vcm1Sacc;
        emit vcm1SaccChanged(m_vcm1Sacc);
    }

    void setVcm1PickForce(double vcm1PickForce)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcm1PickForce, vcm1PickForce))
            return;

        m_vcm1PickForce = vcm1PickForce;
        emit vcm1PickForceChanged(m_vcm1PickForce);
    }

    void setVcm1Margin(double vcm1Margin)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcm1Margin, vcm1Margin))
            return;

        m_vcm1Margin = vcm1Margin;
        emit vcm1MarginChanged(m_vcm1Margin);
    }

    void setVcm2Svel(double vcm2Svel)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcm2Svel, vcm2Svel))
            return;

        m_vcm2Svel = vcm2Svel;
        emit vcm2SvelChanged(m_vcm2Svel);
    }

    void setVcm2Sacc(double vcm2Sacc)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcm2Sacc, vcm2Sacc))
            return;

        m_vcm2Sacc = vcm2Sacc;
        emit vcm2SaccChanged(m_vcm2Sacc);
    }

    void setVcm2PickForce(double vcm2PickForce)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcm2PickForce, vcm2PickForce))
            return;

        m_vcm2PickForce = vcm2PickForce;
        emit vcm2PickForceChanged(m_vcm2PickForce);
    }

    void setVcm2Margin(double vcm2Margin)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcm2Margin, vcm2Margin))
            return;

        m_vcm2Margin = vcm2Margin;
        emit vcm2MarginChanged(m_vcm2Margin);
    }

    void setVcm1FinishDelay(int vcm1FinishDelay)
    {
        if (m_vcm1FinishDelay == vcm1FinishDelay)
            return;

        m_vcm1FinishDelay = vcm1FinishDelay;
        emit vcm1FinishDelayChanged(m_vcm1FinishDelay);
    }

    void setVcm2FinishDelay(int vcm2FinishDelay)
    {
        if (m_vcm2FinishDelay == vcm2FinishDelay)
            return;

        m_vcm2FinishDelay = vcm2FinishDelay;
        emit vcm2FinishDelayChanged(m_vcm2FinishDelay);
    }

signals:
    void runModeChanged(int runMode);
    void saftyZHeightChanged(double saftyZHeight);
    void vcm1BaseHeightChanged(double vcm1BaseHeight);
    void vcm2BaseHeightChanged(double vcm2BaseHeight);
    void vcmSafeHeightChanged(double vcmSafeHeight);
    void vcm1SvelChanged(double vcm1Svel);
    void vcm1SaccChanged(double vcm1Sacc);
    void vcm1PickForceChanged(double vcm1PickForce);
    void vcm1MarginChanged(double vcm1Margin);
    void vcm2SvelChanged(double vcm2Svel);
    void vcm2SaccChanged(double vcm2Sacc);
    void vcm2PickForceChanged(double vcm2PickForce);
    void vcm2MarginChanged(double vcm2Margin);
    void vcm1FinishDelayChanged(int vcm1FinishDelay);
    void vcm2FinishDelayChanged(int vcm2FinishDelay);
};

#endif // SINGLEHEADMACHINEMATERIALLOADERMODULEPARAMETER_H
