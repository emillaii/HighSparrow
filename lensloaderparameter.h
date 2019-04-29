#ifndef LENSPICKARMMODULEPARAMETER_H
#define LENSPICKARMMODULEPARAMETER_H

#include "propertybase.h"



class LensLoaderModuleParameter:public PropertyBase
{
    Q_OBJECT
public:
    LensLoaderModuleParameter():PropertyBase(){}
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(double vcmWorkForce READ vcmWorkForce WRITE setVcmWorkForce NOTIFY vcmWorkForceChanged)
    Q_PROPERTY(double vcmWorkSpeed READ vcmWorkSpeed WRITE setVcmWorkSpeed NOTIFY vcmWorkSpeedChanged)
    Q_PROPERTY(double vcmMargin READ vcmMargin WRITE setVcmMargin NOTIFY vcmMarginChanged)
    Q_PROPERTY(double pickLensZ READ pickLensZ WRITE setPickLensZ NOTIFY pickLensZChanged)
    Q_PROPERTY(double placeLensZ READ placeLensZ WRITE setPlaceLensZ NOTIFY placeLensZChanged)
    Q_PROPERTY(double visonPositionX READ visonPositionX WRITE setVisonPositionX NOTIFY visonPositionXChanged)
    Q_PROPERTY(QString lensLocationName READ lensLocationName WRITE setLensLocationName NOTIFY lensLocationNameChanged)
    Q_PROPERTY(QString vacancyLocationName READ vacancyLocationName WRITE setVacancyLocationName NOTIFY vacancyLocationNameChanged)
    Q_PROPERTY(QString lutLocationName READ lutLocationName WRITE setLutLocationName NOTIFY lutLocationNameChanged)
    Q_PROPERTY(QString lutLensLocationName READ lutLensLocationName WRITE setLutLensLocationName NOTIFY lutLensLocationNameChanged)
    Q_PROPERTY(QString lpaUpdownlookUpLocationName READ lpaUpdownlookUpLocationName WRITE setLpaUpdownlookUpLocation NOTIFY lpaUpdownlookUpkLocationChanged)
    Q_PROPERTY(QString lpaUpdownlookDownLocationName READ lpaUpdownlookDownLocationName WRITE setLpaupdownlookDownLocationName NOTIFY lpaUpdownlookDownLocaationNameChanged)
    Q_PROPERTY(int finishDelay READ finishDelay WRITE setFinishDelay NOTIFY finishDelayChanged)
    int runMode() const
    {
        return m_runMode;
    }
    double vcmWorkForce() const
    {
        return m_vcmWorkForce;
    }

    double vcmWorkSpeed() const
    {
        return m_vcmWorkSpeed;
    }

    double vcmMargin() const
    {
        return m_vcmMargin;
    }

    double pickLensZ() const
    {
        return m_pickLensZ;
    }

    double placeLensZ() const
    {
        return m_placeLensZ;
    }

    double visonPositionX() const
    {
        return m_visonPositionX;
    }
    QString lensLocationName() const
    {
        return m_lensLocationName;
    }

    QString vacancyLocationName() const
    {
        return m_vacancyLocationName;
    }

    QString lutLocationName() const
    {
        return m_lutLocationName;
    }

    QString lutLensLocationName() const
    {
        return m_lueLensLocationName;
    }

    QString lpaUpdownlookUpLocationName() const
    {
        return m_uplookPickLocation;
    }

    QString lpaUpdownlookDownLocationName() const
    {
        return m_uplookCameraLocaation;
    }

    int finishDelay() const
    {
        return m_finishDelay;
    }

public slots:
    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }
    void setVcmWorkForce(double vcmWorkForce)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcmWorkForce, vcmWorkForce))
            return;

        m_vcmWorkForce = vcmWorkForce;
        emit vcmWorkForceChanged(m_vcmWorkForce);
    }

    void setVcmWorkSpeed(double vcmWorkSpeed)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcmWorkSpeed, vcmWorkSpeed))
            return;

        m_vcmWorkSpeed = vcmWorkSpeed;
        emit vcmWorkSpeedChanged(m_vcmWorkSpeed);
    }

    void setVcmMargin(double vcmMargin)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcmMargin, vcmMargin))
            return;

        m_vcmMargin = vcmMargin;
        emit vcmMarginChanged(m_vcmMargin);
    }

    void setPickLensZ(double pickLensZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pickLensZ, pickLensZ))
            return;

        m_pickLensZ = pickLensZ;
        emit pickLensZChanged(m_pickLensZ);
    }

    void setPlaceLensZ(double placeLensZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_placeLensZ, placeLensZ))
            return;

        m_placeLensZ = placeLensZ;
        emit placeLensZChanged(m_placeLensZ);
    }

    void setVisonPositionX(double visonPositionX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_visonPositionX, visonPositionX))
            return;

        m_visonPositionX = visonPositionX;
        emit visonPositionXChanged(m_visonPositionX);
    }

    void setLensLocationName(QString lensLocationName)
    {
        if (m_lensLocationName == lensLocationName)
            return;

        m_lensLocationName = lensLocationName;
        emit lensLocationNameChanged(m_lensLocationName);
    }

    void setVacancyLocationName(QString vacancyLocationName)
    {
        if (m_vacancyLocationName == vacancyLocationName)
            return;

        m_vacancyLocationName = vacancyLocationName;
        emit vacancyLocationNameChanged(m_vacancyLocationName);
    }

    void setLutLocationName(QString lutLocationName)
    {
        if (m_lutLocationName == lutLocationName)
            return;

        m_lutLocationName = lutLocationName;
        emit lutLocationNameChanged(m_lutLocationName);
    }

    void setLutLensLocationName(QString lueLensLocationName)
    {
        if (m_lueLensLocationName == lueLensLocationName)
            return;

        m_lueLensLocationName = lueLensLocationName;
        emit lutLensLocationNameChanged(m_lueLensLocationName);
    }

    void setLpaUpdownlookUpLocation(QString uplookPickLocation)
    {
        if (m_uplookPickLocation == uplookPickLocation)
            return;

        m_uplookPickLocation = uplookPickLocation;
        emit lpaUpdownlookUpkLocationChanged(m_uplookPickLocation);
    }

    void setLpaupdownlookDownLocationName(QString uplookCameraLocaation)
    {
        if (m_uplookCameraLocaation == uplookCameraLocaation)
            return;

        m_uplookCameraLocaation = uplookCameraLocaation;
        emit lpaUpdownlookDownLocaationNameChanged(m_uplookCameraLocaation);
    }

    void setFinishDelay(int finishDelay)
    {
        if (m_finishDelay == finishDelay)
            return;

        m_finishDelay = finishDelay;
        emit finishDelayChanged(m_finishDelay);
    }

signals:
    void runModeChanged(int runMode);
    void vcmWorkForceChanged(double vcmWorkForce);

    void vcmWorkSpeedChanged(double vcmWorkSpeed);

    void vcmMarginChanged(double vcmMargin);

    void pickLensZChanged(double pickLensZ);

    void placeLensZChanged(double placeLensZ);

    void visonPositionXChanged(double visonPositionX);
    void lensLocationNameChanged(QString lensLocationName);

    void vacancyLocationNameChanged(QString vacancyLocationName);

    void lutLocationNameChanged(QString lutLocationName);

    void lutLensLocationNameChanged(QString lutLensLocationName);

    void lpaUpdownlookUpkLocationChanged(QString lpaUpdownlookUpLocationName);

    void lpaUpdownlookDownLocaationNameChanged(QString lpaUpdownlookDownLocationName);

    void finishDelayChanged(int finishDelay);

private:
    int m_runMode = 0;
    double m_vcmWorkForce = 0;
    double m_vcmWorkSpeed = 0;
    double m_vcmMargin = 0;
    double m_pickLensZ = 0;
    double m_placeLensZ = 0;
    double m_visonPositionX = 0;
    QString m_lensLocationName = "";
    QString m_vacancyLocationName = "";
    QString m_lutLocationName = "";
    QString m_lueLensLocationName = "";
    QString m_uplookPickLocation = "";
    QString m_uplookCameraLocaation = "";
    int m_finishDelay = 100;
};

class LensPickArmModuleState:public PropertyBase
{
    Q_OBJECT
public:
    LensPickArmModuleState():PropertyBase(){}
    Q_PROPERTY(bool hasTray READ hasTray WRITE setHasTray NOTIFY hasTrayChanged)
    Q_PROPERTY(bool lutHasLens READ lutHasLens WRITE setLutHasLens NOTIFY lutHasLensChanged)
    Q_PROPERTY(bool lutHasNgLens READ lutHasNgLens WRITE setLutHasNgLens NOTIFY lutHasNgLensChanged)
    Q_PROPERTY(bool needloadLens READ needloadLens WRITE setneedloadLens NOTIFY needloadLensChanged)
    Q_PROPERTY(int currentTray READ currentTray WRITE setCurrentTray NOTIFY currentTrayChanged)
    Q_PROPERTY(bool needChangTray READ needChangTray WRITE setNeedChangTray NOTIFY needChangTrayChanged)
    Q_PROPERTY(bool allowChangeTray READ allowChangeTray WRITE setAllowChangeTray NOTIFY allowChangeTrayChanged)
    Q_PROPERTY(bool hasPickedLens READ hasPickedLens WRITE setHasPickedLens NOTIFY hasPickedLensChanged)
    Q_PROPERTY(bool hasPickedNgLens READ hasPickedNgLens WRITE setHasPickedNgLens NOTIFY hasPickedNgLensChanged)
public:
    bool hasTray() const
    {
        return m_hasTray;
    }

    bool lutHasLens() const
    {
        return m_lutHasLens;
    }

    bool lutHasNgLens() const
    {
        return m_lutHasNgLens;
    }

    bool needloadLens() const
    {
        return m_needloadLens;
    }

    int currentTray() const
    {
        return m_currentTray;
    }

    bool needChangTray() const
    {
        return m_needChangTray;
    }

    bool allowChangeTray() const
    {
        return m_allowChangeTray;
    }

    bool hasPickedLens() const
    {
        return m_hasPickedLens;
    }

    bool hasPickedNgLens() const
    {
        return m_hasPickedNgLens;
    }

public slots:
    void setHasTray(bool hasTray)
    {
        if (m_hasTray == hasTray)
            return;

        m_hasTray = hasTray;
        emit hasTrayChanged(m_hasTray);
    }

    void setLutHasLens(bool lutHasLens)
    {
        if (m_lutHasLens == lutHasLens)
            return;

        m_lutHasLens = lutHasLens;
        emit lutHasLensChanged(m_lutHasLens);
    }

    void setLutHasNgLens(bool lutHasNgLens)
    {
        if (m_lutHasNgLens == lutHasNgLens)
            return;

        m_lutHasNgLens = lutHasNgLens;
        emit lutHasNgLensChanged(m_lutHasNgLens);
    }

    void setneedloadLens(bool needloadLens)
    {
        if (m_needloadLens == needloadLens)
            return;

        m_needloadLens = needloadLens;
        emit needloadLensChanged(m_needloadLens);
    }

    void setCurrentTray(int currentTray)
    {
        if (m_currentTray == currentTray)
            return;

        m_currentTray = currentTray;
        emit currentTrayChanged(m_currentTray);
    }

    void setNeedChangTray(bool needChangTray)
    {
        if (m_needChangTray == needChangTray)
            return;

        m_needChangTray = needChangTray;
        emit needChangTrayChanged(m_needChangTray);
    }

    void setAllowChangeTray(bool allowChangeTray)
    {
        if (m_allowChangeTray == allowChangeTray)
            return;

        m_allowChangeTray = allowChangeTray;
        emit allowChangeTrayChanged(m_allowChangeTray);
    }

    void setHasPickedLens(bool hasPickedLens)
    {
        if (m_hasPickedLens == hasPickedLens)
            return;

        m_hasPickedLens = hasPickedLens;
        emit hasPickedLensChanged(m_hasPickedLens);
    }

    void setHasPickedNgLens(bool hasPickedNgLens)
    {
        if (m_hasPickedNgLens == hasPickedNgLens)
            return;

        m_hasPickedNgLens = hasPickedNgLens;
        emit hasPickedNgLensChanged(m_hasPickedNgLens);
    }

signals:
    void hasTrayChanged(bool hasTray);

    void lutHasLensChanged(bool lutHasLens);

    void lutHasNgLensChanged(bool lutHasNgLens);

    void needloadLensChanged(bool needloadLens);

    void currentTrayChanged(int currentTray);

    void needChangTrayChanged(bool needChangTray);

    void allowChangeTrayChanged(bool allowChangeTray);

    void hasPickedLensChanged(bool hasPickedLens);

    void hasPickedNgLensChanged(bool hasPickedNgLens);

private:
    bool m_hasTray = true;
    bool m_lutHasLens = false;
    bool m_lutHasNgLens = false;
    bool m_needloadLens = false;
    int m_currentTray = false;
    bool m_needChangTray = false;
    bool m_allowChangeTray = false;
    bool m_hasPickedLens = false;
    bool m_hasPickedNgLens = false;
};

#endif // LENSPICKARMMODULEPARAMETER_H
