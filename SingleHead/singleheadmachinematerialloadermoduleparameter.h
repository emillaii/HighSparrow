#ifndef SINGLEHEADMACHINEMATERIALLOADERMODULEPARAMETER_H
#define SINGLEHEADMACHINEMATERIALLOADERMODULEPARAMETER_H

#include "Utils/propertybase.h"



class SingleHeadMachineMaterialLoaderModuleParameter:public PropertyBase
{
    Q_OBJECT

    double m_vcm1Svel = 0;

    double m_vcm1PickForce = 0;
    double m_vcm1Margin = 0;
    double m_vcm2Svel = 0;

    double m_vcm2PickForce = 0;
    double m_vcm2Margin = 0;
    int m_vcm1FinishDelay = 0;
    int m_vcm2FinishDelay = 0;

    QString m_sensorVisionName = "";
    QString m_sensorVacancyVisionName = "";
    QString m_sutVisionName = "";
    QString m_sutSensorVisionName = "";
    QString m_sutProductVisionName = "";
    QString m_lensVisionName = "";
    QString m_lensVancyVisionName = "";
    QString m_lutVisionName = "";
    QString m_lutLensVisionName = "";

    //    double m_zOffset = 0;
    double m_escapeHeight = 0;
    double m_escapeX = 0;
    double m_escapeY = 0;
    QString m_sutVacuumName = "";
    QString m_lutVacuumName = "";

    int m_changeTrayTimeOut = 180000;

public:
    SingleHeadMachineMaterialLoaderModuleParameter():PropertyBase(){}

    Q_PROPERTY(double vcm1Svel READ vcm1Svel WRITE setVcm1Svel NOTIFY vcm1SvelChanged)

    Q_PROPERTY(double vcm1PickForce READ vcm1PickForce WRITE setVcm1PickForce NOTIFY vcm1PickForceChanged)
    Q_PROPERTY(double vcm1Margin READ vcm1Margin WRITE setVcm1Margin NOTIFY vcm1MarginChanged)
    Q_PROPERTY(double vcm2Svel READ vcm2Svel WRITE setVcm2Svel NOTIFY vcm2SvelChanged)

    Q_PROPERTY(double vcm2PickForce READ vcm2PickForce WRITE setVcm2PickForce NOTIFY vcm2PickForceChanged)
    Q_PROPERTY(double vcm2Margin READ vcm2Margin WRITE setVcm2Margin NOTIFY vcm2MarginChanged)
    Q_PROPERTY(int vcm1FinishDelay READ vcm1FinishDelay WRITE setVcm1FinishDelay NOTIFY vcm1FinishDelayChanged)
    Q_PROPERTY(int vcm2FinishDelay READ vcm2FinishDelay WRITE setVcm2FinishDelay NOTIFY vcm2FinishDelayChanged)
    Q_PROPERTY(QString sensorVisionName READ sensorVisionName WRITE setSensorVisionName NOTIFY sensorVisionNameChanged)
    Q_PROPERTY(QString sensorVacancyVisionName READ sensorVacancyVisionName WRITE setSensorVacancyVisionName NOTIFY sensorVacancyVisionNameChanged)
    Q_PROPERTY(QString sutVisionName READ sutVisionName WRITE setSutVisionName NOTIFY sutVisionNameChanged)
    Q_PROPERTY(QString sutSensorVisionName READ sutSensorVisionName WRITE setSutSensorVisionName NOTIFY sutSensorVisionNameChanged)
    Q_PROPERTY(QString sutProductVisionName READ sutProductVisionName WRITE setSutProductVisionName NOTIFY sutProductVisionNameChanged)
    Q_PROPERTY(QString lensVisionName READ lensVisionName WRITE setLensVisionName NOTIFY lensVisionNameChanged)
    Q_PROPERTY(QString lensVacancyVisionName READ lensVacancyVisionName WRITE setLensVacancyVisionName NOTIFY lensVacancyVisionNameChanged)
    Q_PROPERTY(QString lutVisionName READ lutVisionName WRITE setLutVisionName NOTIFY lutVisionNameChanged)
    Q_PROPERTY(QString lutLensVisionName READ lutLensVisionName WRITE setLutLensVisionName NOTIFY lutLensVisionNameChanged)

    //    Q_PROPERTY(double zOffset READ zOffset WRITE setZOffset NOTIFY zOffsetChanged)
    Q_PROPERTY(double escapeHeight READ escapeHeight WRITE setEscapeHeight NOTIFY escapeHeightChanged)
    Q_PROPERTY(double escapeX READ escapeX WRITE setEscapeX NOTIFY escapeXChanged)
    Q_PROPERTY(double escapeY READ escapeY WRITE setEscapeY NOTIFY escapeYChanged)
    Q_PROPERTY(QString sutVacuumName READ sutVacuumName WRITE setSutVacuumName NOTIFY sutVacuumNameChanged)
    Q_PROPERTY(QString lutVacuumName READ lutVacuumName WRITE setLutVacuumName NOTIFY lutVacuumNameChanged)

    Q_PROPERTY(int changeTrayTimeOut READ changeTrayTimeOut WRITE setChangeTrayTimeOut NOTIFY changeTrayTimeOutChanged)



    double vcm1Svel() const
    {
        return m_vcm1Svel;
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

    QString sensorVisionName() const
    {
        return m_sensorVisionName;
    }

    QString sensorVacancyVisionName() const
    {
        return m_sensorVacancyVisionName;
    }

    QString sutVisionName() const
    {
        return m_sutVisionName;
    }

    QString sutSensorVisionName() const
    {
        return m_sutSensorVisionName;
    }

    QString sutProductVisionName() const
    {
        return m_sutProductVisionName;
    }

    QString lensVisionName() const
    {
        return m_lensVisionName;
    }

    QString lensVacancyVisionName() const
    {
        return m_lensVancyVisionName;
    }

    QString lutVisionName() const
    {
        return m_lutVisionName;
    }

    QString lutLensVisionName() const
    {
        return m_lutLensVisionName;
    }



    //    double zOffset() const
    //    {
    //        return m_zOffset;
    //    }

    double escapeHeight() const
    {
        return m_escapeHeight;
    }

    double escapeX() const
    {
        return m_escapeX;
    }

    double escapeY() const
    {
        return m_escapeY;
    }

    QString sutVacuumName() const
    {
        return m_sutVacuumName;
    }

    QString lutVacuumName() const
    {
        return m_lutVacuumName;
    }



    int changeTrayTimeOut() const
    {
        return m_changeTrayTimeOut;
    }

public slots:


    void setVcm1Svel(double vcm1Svel)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vcm1Svel, vcm1Svel))
            return;

        m_vcm1Svel = vcm1Svel;
        emit vcm1SvelChanged(m_vcm1Svel);
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

    void setSensorVisionName(QString sensorVisionName)
    {
        if (m_sensorVisionName == sensorVisionName)
            return;

        m_sensorVisionName = sensorVisionName;
        emit sensorVisionNameChanged(m_sensorVisionName);
    }

    void setSensorVacancyVisionName(QString sensorVacancyVisionName)
    {
        if (m_sensorVacancyVisionName == sensorVacancyVisionName)
            return;

        m_sensorVacancyVisionName = sensorVacancyVisionName;
        emit sensorVacancyVisionNameChanged(m_sensorVacancyVisionName);
    }

    void setSutVisionName(QString sutVisionName)
    {
        if (m_sutVisionName == sutVisionName)
            return;

        m_sutVisionName = sutVisionName;
        emit sutVisionNameChanged(m_sutVisionName);
    }

    void setSutSensorVisionName(QString sutSensorVisionName)
    {
        if (m_sutSensorVisionName == sutSensorVisionName)
            return;

        m_sutSensorVisionName = sutSensorVisionName;
        emit sutSensorVisionNameChanged(m_sutSensorVisionName);
    }

    void setSutProductVisionName(QString sutProductVisionName)
    {
        if (m_sutProductVisionName == sutProductVisionName)
            return;

        m_sutProductVisionName = sutProductVisionName;
        emit sutProductVisionNameChanged(m_sutProductVisionName);
    }

    void setLensVisionName(QString lensVisionName)
    {
        if (m_lensVisionName == lensVisionName)
            return;

        m_lensVisionName = lensVisionName;
        emit lensVisionNameChanged(m_lensVisionName);
    }

    void setLensVacancyVisionName(QString lensVancyVisionName)
    {
        if (m_lensVancyVisionName == lensVancyVisionName)
            return;

        m_lensVancyVisionName = lensVancyVisionName;
        emit lensVacancyVisionNameChanged(m_lensVancyVisionName);
    }

    void setLutVisionName(QString lutVisionName)
    {
        if (m_lutVisionName == lutVisionName)
            return;

        m_lutVisionName = lutVisionName;
        emit lutVisionNameChanged(m_lutVisionName);
    }

    void setLutLensVisionName(QString lutLensVisionName)
    {
        if (m_lutLensVisionName == lutLensVisionName)
            return;

        m_lutLensVisionName = lutLensVisionName;
        emit lutLensVisionNameChanged(m_lutLensVisionName);
    }


    //    void setZOffset(double zOffset)
    //    {
    //        qWarning("Floating point comparison needs context sanity check");
    //        if (qFuzzyCompare(m_zOffset, zOffset))
    //            return;

    //        m_zOffset = zOffset;
    //        emit zOffsetChanged(m_zOffset);
    //    }

    void setEscapeHeight(double escapeHeight)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_escapeHeight, escapeHeight))
            return;

        m_escapeHeight = escapeHeight;
        emit escapeHeightChanged(m_escapeHeight);
    }

    void setEscapeX(double escapeX)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_escapeX, escapeX))
            return;

        m_escapeX = escapeX;
        emit escapeXChanged(m_escapeX);
    }

    void setEscapeY(double escapeY)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_escapeY, escapeY))
            return;

        m_escapeY = escapeY;
        emit escapeYChanged(m_escapeY);
    }

    void setSutVacuumName(QString sutVacuumName)
    {
        if (m_sutVacuumName == sutVacuumName)
            return;

        m_sutVacuumName = sutVacuumName;
        emit sutVacuumNameChanged(m_sutVacuumName);
    }

    void setLutVacuumName(QString lutVacuumName)
    {
        if (m_lutVacuumName == lutVacuumName)
            return;

        m_lutVacuumName = lutVacuumName;
        emit lutVacuumNameChanged(m_lutVacuumName);
    }



    void setChangeTrayTimeOut(int changeTrayTimeOut)
    {
        if (m_changeTrayTimeOut == changeTrayTimeOut)
            return;

        m_changeTrayTimeOut = changeTrayTimeOut;
        emit changeTrayTimeOutChanged(m_changeTrayTimeOut);
    }

signals:

    void vcm1SvelChanged(double vcm1Svel);

    void vcm1PickForceChanged(double vcm1PickForce);
    void vcm1MarginChanged(double vcm1Margin);
    void vcm2SvelChanged(double vcm2Svel);

    void vcm2PickForceChanged(double vcm2PickForce);
    void vcm2MarginChanged(double vcm2Margin);
    void vcm1FinishDelayChanged(int vcm1FinishDelay);
    void vcm2FinishDelayChanged(int vcm2FinishDelay);
    void sensorVisionNameChanged(QString sensorVisionName);
    void sensorVacancyVisionNameChanged(QString sensorVacancyVisionName);
    void sutVisionNameChanged(QString sutVisionName);
    void sutSensorVisionNameChanged(QString sutSensorVisionName);
    void sutProductVisionNameChanged(QString sutProductVisionName);
    void lensVisionNameChanged(QString lensVisionName);
    void lensVacancyVisionNameChanged(QString lensVacancyVisionName);
    void lutVisionNameChanged(QString lutVisionName);
    void lutLensVisionNameChanged(QString lutLensVisionName);

    void zOffsetChanged(double zOffset);
    void escapeHeightChanged(double escapeHeight);
    void escapeXChanged(double escapeX);
    void escapeYChanged(double escapeY);
    void sutVacuumNameChanged(QString sutVacuumName);
    void lutVacuumNameChanged(QString lutVacuumName);

    void changeTrayTimeOutChanged(double changeTrayTimeOut);
};

class MaterialLoaderState:public PropertyBase
{
    Q_OBJECT
public:
    MaterialLoaderState():PropertyBase (){}
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)
    Q_PROPERTY(bool hasSensorTray READ hasSensorTray WRITE setHasSensorTray NOTIFY hasSensorTrayChanged)
    Q_PROPERTY(bool sutHasSensor READ sutHasSensor WRITE setSutHasSensor NOTIFY sutHasSensorChanged)
    Q_PROPERTY(bool sutHasNgSensor READ sutHasNgSensor WRITE setSutHasNgSensor NOTIFY sutHasNgSensorChanged)
    Q_PROPERTY(bool sutHasProduct READ sutHasProduct WRITE setSutHasProduct NOTIFY sutHasProductChanged)

    Q_PROPERTY(bool needLoadSensor READ needLoadSensor WRITE setNeedLoadSensor NOTIFY needLoadSensorChanged)
    Q_PROPERTY(bool needChangeSensorTray READ needChangeSensorTray WRITE setNeedChangeSensorTray NOTIFY needChangeSensorTrayChanged)
    Q_PROPERTY(bool allowChangeTray READ allowChangeSensorTray WRITE setAllowChangeSensorTray NOTIFY allowChangeSensorTrayChanged)

    Q_PROPERTY(bool hasPickedSensor READ hasPickedSensor WRITE setHasPickedSensor NOTIFY hasPickedSensorChanged)
    Q_PROPERTY(bool hasPickedProduct READ hasPickedProduct WRITE setHasPickedProduct NOTIFY hasPickedProductChanged)
    Q_PROPERTY(bool hasPickedNgSensor READ hasPickedNgSensor WRITE setHasPickedNgSensor NOTIFY hasPickedNgSensorChanged)
    Q_PROPERTY(bool beExchangeMaterial READ beExchangeMaterial WRITE setBeExchangeMaterial NOTIFY beExchangeMaterialChanged)

    Q_PROPERTY(bool waitingChangeSensorTray READ waitingChangeSensorTray WRITE setWaitingChangeSensorTray NOTIFY waitingChangeSensorTrayChanged)
    Q_PROPERTY(bool finishChangeSensorTray READ finishChangeSensorTray WRITE setFinishChangeSensorTray NOTIFY finishChangeSensorTrayChanged)

    Q_PROPERTY(bool hasLensTray READ hasLensTray WRITE setHasLensTray NOTIFY hasLensTrayChanged)
    Q_PROPERTY(bool lutHasLens READ lutHasLens WRITE setLutHasLens NOTIFY lutHasLensChanged)
    Q_PROPERTY(bool lutHasNgLens READ lutHasNgLens WRITE setLutHasNgLens NOTIFY lutHasNgLensChanged)
    Q_PROPERTY(bool needLoadLens READ needLoadLens WRITE setNeedLoadLens NOTIFY needLoadLensChanged)
    Q_PROPERTY(int currentLensTray READ currentLensTray WRITE setCurrentLensTray NOTIFY currentLensTrayChanged)
    Q_PROPERTY(int currentSensorTray READ currentSensorTray WRITE setCurrentSensorTray NOTIFY currentSensorTrayChanged)
    Q_PROPERTY(int currentRejectTray READ currentRejectTray WRITE setCurrentRejectTray NOTIFY currentRejectTrayChanged)
    Q_PROPERTY(bool needChangeLensTray READ needChangeLensTray WRITE setNeedChangeLensTray NOTIFY needChangeLensTrayChanged)
    Q_PROPERTY(bool allowChangeLensTray READ allowChangeLensTray WRITE setAllowChangeLensTray NOTIFY allowChangeLensTrayChanged)
    Q_PROPERTY(bool hasPickedLens READ hasPickedLens WRITE setHasPickedLens NOTIFY hasPickedLensChanged)
    Q_PROPERTY(bool hasPickedNgLens READ hasPickedNgLens WRITE setHasPickedNgLens NOTIFY hasPickedNgLensChanged)
    Q_PROPERTY(int lutTrayID READ lutTrayID WRITE setLutTrayID NOTIFY lutTrayIDChanged)
    Q_PROPERTY(int lutLensID READ lutLensID WRITE setLutLensID NOTIFY lutLensIDChanged)
    Q_PROPERTY(int pickedLensTrayID READ pickedLensTrayID WRITE setPickedLensTrayID NOTIFY pickedLensTrayIDChanged)
    Q_PROPERTY(int pickedLensID READ pickedLensID WRITE setPickedLensID NOTIFY pickedLensIDChanged)
    Q_PROPERTY(int lutNgTrayID READ lutNgTrayID WRITE setLutNgTrayID NOTIFY lutNgTrayIDChanged)
    Q_PROPERTY(int lutNgLensID READ lutNgLensID WRITE setLutNgLensID NOTIFY lutNgLensIDChanged)
    Q_PROPERTY(bool loadingLens READ loadingLens WRITE setLoadingLens NOTIFY loadingLensChanged)
    Q_PROPERTY(bool waitingChangeLensTray READ waitingChangeLensTray WRITE setWaitingChangeLensTray NOTIFY waitingChangeLensTrayChanged)
    Q_PROPERTY(bool finishChangeLensTray READ finishChangeLensTray WRITE setFinishChangeLensTray NOTIFY finishChangeLensTrayChanged)

    int runMode() const
    {
        return m_runMode;
    }
    bool hasSensorTray() const
    {
        return m_hasSensorTray;
    }

    bool sutHasSensor() const
    {
        return m_sutHasSensor;
    }

    bool sutHasNgSensor() const
    {
        return m_sutHasNgSensor;
    }

    bool sutHasProduct() const
    {
        return m_sutHasProduct;
    }

    bool needLoadSensor() const
    {
        return m_needLoadSensor;
    }

    bool needChangeSensorTray() const
    {
        return m_needChangeSensorTray;
    }

    bool allowChangeSensorTray() const
    {
        return m_allowChangeSensorTray;
    }

    bool hasPickedSensor() const
    {
        return m_hasPickedSensor;
    }

    bool hasPickedProduct() const
    {
        return m_hasPickedProduct;
    }

    bool hasPickedNgSensor() const
    {
        return m_hasPickedNgSensor;
    }

    bool beExchangeMaterial() const
    {
        return m_beExchangeMaterial;
    }

    bool waitingChangeSensorTray() const
    {
        return m_waitingChangeSensorTray;
    }

    bool finishChangeSensorTray() const
    {
        return m_finishChangeSensorTray;
    }

    bool hasLensTray() const
    {
        return m_hasLensTray;
    }

    bool lutHasLens() const
    {
        return m_lutHasLens;
    }

    bool lutHasNgLens() const
    {
        return m_lutHasNgLens;
    }

    bool needLoadLens() const
    {
        return m_needloadLens;
    }

    int currentLensTray() const
    {
        return m_currentLensTray;
    }

    bool needChangeLensTray() const
    {
        return m_needChangeLensTray;
    }

    bool allowChangeLensTray() const
    {
        return m_allowChangeLensTray;
    }

    bool hasPickedLens() const
    {
        return m_hasPickedLens;
    }

    bool hasPickedNgLens() const
    {
        return m_hasPickedNgLens;
    }

    int lutTrayID() const
    {
        return m_lutTrayID;
    }

    int lutLensID() const
    {
        return m_lutLensID;
    }

    int pickedLensTrayID() const
    {
        return m_pickedLensTrayID;
    }

    int pickedLensID() const
    {
        return m_pickedLensID;
    }

    int lutNgTrayID() const
    {
        return m_lutNgTrayID;
    }

    int lutNgLensID() const
    {
        return m_lutNgLensID;
    }

    bool loadingLens() const
    {
        return m_loadingLens;
    }

    bool waitingChangeLensTray() const
    {
        return m_waitingChangeLensTray;
    }

    bool finishChangeLensTray() const
    {
        return m_finishChangeLensTray;
    }


    int currentSensorTray() const
    {
        return m_currentSensorTray;
    }

    int currentRejectTray() const
    {
        return m_currentRejectTray;
    }

public slots:
    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }
    void setHasSensorTray(bool hasSensorTray)
    {
        if (m_hasSensorTray == hasSensorTray)
            return;

        m_hasSensorTray = hasSensorTray;
        emit hasSensorTrayChanged(m_hasSensorTray);
    }

    void setSutHasSensor(bool sutHasSensor)
    {
        if (m_sutHasSensor == sutHasSensor)
            return;

        m_sutHasSensor = sutHasSensor;
        emit sutHasSensorChanged(m_sutHasSensor);
    }

    void setSutHasNgSensor(bool sutHasNgSensor)
    {
        if (m_sutHasNgSensor == sutHasNgSensor)
            return;

        m_sutHasNgSensor = sutHasNgSensor;
        emit sutHasNgSensorChanged(m_sutHasNgSensor);
    }

    void setSutHasProduct(bool sutHasProduct)
    {
        if (m_sutHasProduct == sutHasProduct)
            return;

        m_sutHasProduct = sutHasProduct;
        emit sutHasProductChanged(m_sutHasProduct);
    }

    void setNeedLoadSensor(bool needLoadSensor)
    {
        if (m_needLoadSensor == needLoadSensor)
            return;

        m_needLoadSensor = needLoadSensor;
        emit needLoadSensorChanged(m_needLoadSensor);
    }

    void setNeedChangeSensorTray(bool needChangeSensorTray)
    {
        if (m_needChangeSensorTray == needChangeSensorTray)
            return;

        m_needChangeSensorTray = needChangeSensorTray;
        emit needChangeSensorTrayChanged(m_needChangeSensorTray);
    }

    void setAllowChangeSensorTray(bool allowChangeSensorTray)
    {
        if (m_allowChangeSensorTray == allowChangeSensorTray)
            return;

        m_allowChangeSensorTray = allowChangeSensorTray;
        emit allowChangeSensorTrayChanged(m_allowChangeSensorTray);
    }

    void setHasPickedSensor(bool hasPickedSensor)
    {
        if (m_hasPickedSensor == hasPickedSensor)
            return;

        m_hasPickedSensor = hasPickedSensor;
        emit hasPickedSensorChanged(m_hasPickedSensor);
    }

    void setHasPickedProduct(bool hasPickedProduct)
    {
        if (m_hasPickedProduct == hasPickedProduct)
            return;

        m_hasPickedProduct = hasPickedProduct;
        emit hasPickedProductChanged(m_hasPickedProduct);
    }

    void setHasPickedNgSensor(bool hasPickedNgSensor)
    {
        if (m_hasPickedNgSensor == hasPickedNgSensor)
            return;

        m_hasPickedNgSensor = hasPickedNgSensor;
        emit hasPickedNgSensorChanged(m_hasPickedNgSensor);
    }

    void setBeExchangeMaterial(bool beExchangeMaterial)
    {
        if (m_beExchangeMaterial == beExchangeMaterial)
            return;

        m_beExchangeMaterial = beExchangeMaterial;
        emit beExchangeMaterialChanged(m_beExchangeMaterial);
    }

    void setWaitingChangeSensorTray(bool waitingChangeSensorTray)
    {
        if (m_waitingChangeSensorTray == waitingChangeSensorTray)
            return;

        m_waitingChangeSensorTray = waitingChangeSensorTray;
        emit waitingChangeSensorTrayChanged(m_waitingChangeSensorTray);
    }

    void setFinishChangeSensorTray(bool finishChangeSensorTray)
    {
        if (m_finishChangeSensorTray == finishChangeSensorTray)
            return;

        m_finishChangeSensorTray = finishChangeSensorTray;
        emit finishChangeSensorTrayChanged(m_finishChangeSensorTray);
    }

    void setHasLensTray(bool hasLensTray)
    {
        if (m_hasLensTray == hasLensTray)
            return;

        m_hasLensTray = hasLensTray;
        emit hasLensTrayChanged(m_hasLensTray);
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

    void setNeedLoadLens(bool needloadLens)
    {
        if (m_needloadLens == needloadLens)
            return;

        m_needloadLens = needloadLens;
        emit needLoadLensChanged(m_needloadLens);
    }

    void setCurrentLensTray(int currentLensTray)
    {
        if (m_currentLensTray == currentLensTray)
            return;

        m_currentLensTray = currentLensTray;
        emit currentLensTrayChanged(m_currentLensTray);
    }

    void setNeedChangeLensTray(bool needChangeLensTray)
    {
        if (m_needChangeLensTray == needChangeLensTray)
            return;

        m_needChangeLensTray = needChangeLensTray;
        emit needChangeLensTrayChanged(m_needChangeLensTray);
    }

    void setAllowChangeLensTray(bool allowChangeLensTray)
    {
        if (m_allowChangeLensTray == allowChangeLensTray)
            return;

        m_allowChangeLensTray = allowChangeLensTray;
        emit allowChangeLensTrayChanged(m_allowChangeLensTray);
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

    void setLutTrayID(int lutTrayID)
    {
        if (m_lutTrayID == lutTrayID)
            return;

        m_lutTrayID = lutTrayID;
        emit lutTrayIDChanged(m_lutTrayID);
    }

    void setLutLensID(int lutLensID)
    {
        if (m_lutLensID == lutLensID)
            return;

        m_lutLensID = lutLensID;
        emit lutLensIDChanged(m_lutLensID);
    }

    void setPickedLensTrayID(int pickedLensTrayID)
    {
        if (m_pickedLensTrayID == pickedLensTrayID)
            return;

        m_pickedLensTrayID = pickedLensTrayID;
        emit pickedLensTrayIDChanged(m_pickedLensTrayID);
    }

    void setPickedLensID(int pickedLensID)
    {
        if (m_pickedLensID == pickedLensID)
            return;

        m_pickedLensID = pickedLensID;
        emit pickedLensIDChanged(m_pickedLensID);
    }

    void setLutNgTrayID(int lutNgTrayID)
    {
        if (m_lutNgTrayID == lutNgTrayID)
            return;

        m_lutNgTrayID = lutNgTrayID;
        emit lutNgTrayIDChanged(m_lutNgTrayID);
    }

    void setLutNgLensID(int lutNgLensID)
    {
        if (m_lutNgLensID == lutNgLensID)
            return;

        m_lutNgLensID = lutNgLensID;
        emit lutNgLensIDChanged(m_lutNgLensID);
    }

    void setLoadingLens(bool loadingLens)
    {
        if (m_loadingLens == loadingLens)
            return;

        m_loadingLens = loadingLens;
        emit loadingLensChanged(m_loadingLens);
    }

    void setWaitingChangeLensTray(bool waitingChangeLensTray)
    {
        if (m_waitingChangeLensTray == waitingChangeLensTray)
            return;

        m_waitingChangeLensTray = waitingChangeLensTray;
        emit waitingChangeLensTrayChanged(m_waitingChangeLensTray);
    }

    void setFinishChangeLensTray(bool finishChangeLensTray)
    {
        if (m_finishChangeLensTray == finishChangeLensTray)
            return;

        m_finishChangeLensTray = finishChangeLensTray;
        emit finishChangeLensTrayChanged(m_finishChangeLensTray);
    }

    void setCurrentSensorTray(int currentSensorTray)
    {
        if (m_currentSensorTray == currentSensorTray)
            return;

        m_currentSensorTray = currentSensorTray;
        emit currentSensorTrayChanged(m_currentSensorTray);
    }

    void setCurrentRejectTray(int currentRejectTray)
    {
        if (m_currentRejectTray == currentRejectTray)
            return;

        m_currentRejectTray = currentRejectTray;
        emit currentRejectTrayChanged(m_currentRejectTray);
    }

signals:
    void runModeChanged(int runMode);
    void hasSensorTrayChanged(bool hasTray);

    void sutHasSensorChanged(bool sutHasSensor);

    void sutHasNgSensorChanged(bool sutHasNgSensor);

    void sutHasProductChanged(bool sutHasProduct);

    void needLoadSensorChanged(bool needLoadSensor);

    void needChangeSensorTrayChanged(bool needChangTray);

    void allowChangeSensorTrayChanged(bool allowChangeTray);

    void hasPickedSensorChanged(bool hasPickedSensor);

    void hasPickedProductChanged(bool hasPickedProduct);

    void hasPickedNgSensorChanged(bool hasPickedNgSensor);

    void beExchangeMaterialChanged(bool beExchangeMaterial);

    void waitingChangeSensorTrayChanged(bool waitingChangeTray);

    void finishChangeSensorTrayChanged(bool finishChangeTray);

    void hasLensTrayChanged(bool hasLensTray);

    void lutHasLensChanged(bool lutHasLens);

    void lutHasNgLensChanged(bool lutHasNgLens);

    void needLoadLensChanged(bool needLoadLens);

    void currentLensTrayChanged(int currentLensTray);

    void needChangeLensTrayChanged(bool needChangeLensTray);

    void allowChangeLensTrayChanged(bool allowChangeLensTray);

    void hasPickedLensChanged(bool hasPickedLens);

    void hasPickedNgLensChanged(bool hasPickedNgLens);

    void lutTrayIDChanged(int lutTrayID);

    void lutLensIDChanged(int lutLensID);

    void pickedLensTrayIDChanged(int pickedLensTrayID);

    void pickedLensIDChanged(int pickedLensID);

    void lutNgTrayIDChanged(int lutNgTrayID);

    void lutNgLensIDChanged(int lutNgLensID);

    void loadingLensChanged(bool loadingLens);

    void waitingChangeLensTrayChanged(bool waitingChangeLensTray);

    void finishChangeLensTrayChanged(bool finishChangeLensTray);

    void currentSensorTrayChanged(int currentSensorTray);

    void currentRejectTrayChanged(int currentRejectTray);

private:
    int m_runMode = 0;
    bool m_hasSensorTray = false;
    bool m_sutHasSensor = false;
    bool m_sutHasNgSensor = false;
    bool m_sutHasProduct = false;
    bool m_needLoadSensor = false;
    bool m_needChangeSensorTray = false;
    bool m_allowChangeSensorTray = false;
    bool m_hasPickedSensor = false;
    bool m_hasPickedProduct = false;
    bool m_hasPickedNgSensor = false;
    bool m_beExchangeMaterial = false;
    bool m_waitingChangeSensorTray = false;
    bool m_finishChangeSensorTray = false;

    bool m_hasLensTray = true;
    bool m_lutHasLens = false;
    bool m_lutHasNgLens = false;
    bool m_needloadLens = false;
    int m_currentLensTray = 0;
    int m_currentSensorTray = 0;
    int m_currentRejectTray = 0;
    bool m_needChangeLensTray = false;
    bool m_allowChangeLensTray = false;
    bool m_hasPickedLens = false;
    bool m_hasPickedNgLens = false;
    int m_lutTrayID = -1;
    int m_lutLensID = -1;
    int m_pickedLensTrayID = -1;
    int m_pickedLensID = -1;
    int m_lutNgTrayID = -1;
    int m_lutNgLensID = -1;
    bool m_loadingLens = false;
    bool m_waitingChangeLensTray = false;
    bool m_finishChangeLensTray = false;

};


#endif // SINGLEHEADMACHINEMATERIALLOADERMODULEPARAMETER_H
