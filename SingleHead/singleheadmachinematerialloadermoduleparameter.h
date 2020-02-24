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



    QString m_cameraToPickerOffsetVisionName = "";

    double m_sutPlaceSensorAngle = 90;

    bool m_performPrAsPlacingNgDut;

    QString m_buzzerName = "";

    QString m_redLightName = "";

    QString m_greenLightName = "";

    QString m_yellowLightName = "";

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
    Q_PROPERTY(QString cameraToPickerOffsetVisionName READ cameraToPickerOffsetVisionName WRITE setCameraToPickerOffsetVisionName NOTIFY cameraToPickerOffsetVisionNameChanged)


    //    Q_PROPERTY(double zOffset READ zOffset WRITE setZOffset NOTIFY zOffsetChanged)
    Q_PROPERTY(double escapeHeight READ escapeHeight WRITE setEscapeHeight NOTIFY escapeHeightChanged)
    Q_PROPERTY(double escapeX READ escapeX WRITE setEscapeX NOTIFY escapeXChanged)
    Q_PROPERTY(double escapeY READ escapeY WRITE setEscapeY NOTIFY escapeYChanged)
    Q_PROPERTY(QString sutVacuumName READ sutVacuumName WRITE setSutVacuumName NOTIFY sutVacuumNameChanged)
    Q_PROPERTY(QString lutVacuumName READ lutVacuumName WRITE setLutVacuumName NOTIFY lutVacuumNameChanged)
    Q_PROPERTY(int changeTrayTimeOut READ changeTrayTimeOut WRITE setChangeTrayTimeOut NOTIFY changeTrayTimeOutChanged)
    Q_PROPERTY(double sutPlaceSensorAngle READ sutPlaceSensorAngle WRITE setSutPlaceSensorAngle NOTIFY sutPlaceSensorAngleChanged)

    Q_PROPERTY(bool performPrAsPlacingNgDut READ performPrAsPlacingNgDut WRITE setPerformPrAsPlacingNgDut NOTIFY performPrAsPlacingNgDutChanged)
    Q_PROPERTY(QString buzzerName READ buzzerName WRITE setBuzzerName NOTIFY buzzerNameChanged)
    Q_PROPERTY(QString redLightName READ redLightName WRITE setRedLightName NOTIFY redLightChanged)
    Q_PROPERTY(QString greenLightName READ greenLightName WRITE setGreenLightName NOTIFY greenLightChanged)
    Q_PROPERTY(QString yellowLightName READ yellowLightName WRITE setYellowLightName NOTIFY yellowLightChanged)



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


    QString cameraToPickerOffsetVisionName() const
    {
        return m_cameraToPickerOffsetVisionName;
    }

    double sutPlaceSensorAngle() const
    {
        return m_sutPlaceSensorAngle;
    }

    bool performPrAsPlacingNgDut() const
    {
        return m_performPrAsPlacingNgDut;
    }

    QString buzzerName() const
    {
        return m_buzzerName;
    }

    QString redLightName() const
    {
        return m_redLightName;
    }

    QString greenLightName() const
    {
        return m_greenLightName;
    }

    QString yellowLightName() const
    {
        return m_yellowLightName;
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



    void setCameraToPickerOffsetVisionName(QString cameraToPickerOffsetVisionName)
    {
        if (m_cameraToPickerOffsetVisionName == cameraToPickerOffsetVisionName)
            return;

        m_cameraToPickerOffsetVisionName = cameraToPickerOffsetVisionName;
        emit cameraToPickerOffsetVisionNameChanged(m_cameraToPickerOffsetVisionName);
    }

    void setSutPlaceSensorAngle(double sutPlaceSensorAngle)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_sutPlaceSensorAngle, sutPlaceSensorAngle))
            return;

        m_sutPlaceSensorAngle = sutPlaceSensorAngle;
        emit sutPlaceSensorAngleChanged(m_sutPlaceSensorAngle);
    }

    void setPerformPrAsPlacingNgDut(bool performPrAsPlacingNgDut)
    {
        if (m_performPrAsPlacingNgDut == performPrAsPlacingNgDut)
            return;

        m_performPrAsPlacingNgDut = performPrAsPlacingNgDut;
        emit performPrAsPlacingNgDutChanged(m_performPrAsPlacingNgDut);
    }

    void setBuzzerName(QString buzzerName)
    {
        if (m_buzzerName == buzzerName)
            return;

        m_buzzerName = buzzerName;
        emit buzzerNameChanged(m_buzzerName);
    }

    void setRedLightName(QString redLightName)
    {
        if (m_redLightName == redLightName)
            return;

        m_redLightName = redLightName;
        emit redLightChanged(m_redLightName);
    }

    void setGreenLightName(QString greenLightName)
    {
        if (m_greenLightName == greenLightName)
            return;

        m_greenLightName = greenLightName;
        emit greenLightChanged(m_greenLightName);
    }

    void setYellowLightName(QString yellowLightName)
    {
        if (m_yellowLightName == yellowLightName)
            return;

        m_yellowLightName = yellowLightName;
        emit yellowLightChanged(m_yellowLightName);
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
    void cameraToPickerOffsetVisionNameChanged(QString cameraToPickerOffsetVisionName);
    void sutPlaceSensorAngleChanged(double sutPlaceSensorAngle);
    void performPrAsPlacingNgDutChanged(bool performPrAsPlacingNgDut);
    void picker1PlaceOkProductOffsetXChanged(double picker1PlaceOkProductOffsetX);
    void picker1PlaceOkProductOffsetYChanged(double picker1PlaceOkProductOffsetY);void buzzerNameChanged(QString buzzerName);
    void redLightChanged(QString redLightName);
    void greenLightChanged(QString greenLightName);
    void yellowLightChanged(QString yellowLightName);
};

class MaterialLoaderState:public PropertyBase
{
    Q_OBJECT
    int m_runMode;

    bool m_sutIsReadyToLoadMaterial = false;

    bool m_hasPickedSensor = false;

    bool m_hasPickedProduct = false;

    bool m_hasPickedNgSensor = false;

    bool m_hasPickedLens = false;

    bool m_hasPickedNgLens = false;

    int m_currentLensTray = 0;

    int m_currentSensorTray = 0;

    int m_currentRejectTray = 0;

    bool m_isPickedProductOk = false;

    int m_currentSensorIndexInPicker;

    int m_currentLensIndexInPicker;

public:
    MaterialLoaderState():PropertyBase (){}
    Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)

    Q_PROPERTY(bool sutIsReadyToLoadMaterial READ sutIsReadyToLoadMaterial WRITE setSutIsReadyToLoadMaterial NOTIFY sutIsReadyToLoadMaterialChanged)

    Q_PROPERTY(bool hasPickedSensor READ hasPickedSensor WRITE setHasPickedSensor NOTIFY hasPickedSensorChanged)
    Q_PROPERTY(bool hasPickedProduct READ hasPickedProduct WRITE setHasPickedProduct NOTIFY hasPickedProductChanged)
    Q_PROPERTY(bool isPickedProductOk READ isPickedProductOk WRITE setIsPickedProductOk NOTIFY isPickedProductOkChanged)
    Q_PROPERTY(bool hasPickedNgSensor READ hasPickedNgSensor WRITE setHasPickedNgSensor NOTIFY hasPickedNgSensorChanged)

    Q_PROPERTY(bool hasPickedLens READ hasPickedLens WRITE setHasPickedLens NOTIFY hasPickedLensChanged)
    Q_PROPERTY(bool hasPickedNgLens READ hasPickedNgLens WRITE setHasPickedNgLens NOTIFY hasPickedNgLensChanged)

    Q_PROPERTY(int currentLensTray READ currentLensTray WRITE setCurrentLensTray NOTIFY currentLensTrayChanged)
    Q_PROPERTY(int currentSensorTray READ currentSensorTray WRITE setCurrentSensorTray NOTIFY currentSensorTrayChanged)
    Q_PROPERTY(int currentRejectTray READ currentRejectTray WRITE setCurrentRejectTray NOTIFY currentRejectTrayChanged)

    Q_PROPERTY(int currentSensorIndexInPicker READ currentSensorIndexInPicker WRITE setCurrentSensorIndexInPicker NOTIFY currentSensorIndexInPickerChanged)
    Q_PROPERTY(int currentLensIndexInPicker READ currentLensIndexInPicker WRITE setCurrentLensIndexInPicker NOTIFY currentLensIndexInPickerChanged)

    bool picker1IsIdle() const
    {
        return !(hasPickedLens() || hasPickedNgLens()
                 || hasPickedProduct() || hasPickedNgSensor());
    }


    int runMode() const
    {
        return m_runMode;
    }
    bool sutIsReadyToLoadMaterial() const
    {
        return m_sutIsReadyToLoadMaterial;
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

    bool hasPickedLens() const
    {
        return m_hasPickedLens;
    }

    bool hasPickedNgLens() const
    {
        return m_hasPickedNgLens;
    }

    int currentLensTray() const
    {
        return m_currentLensTray;
    }

    int currentSensorTray() const
    {
        return m_currentSensorTray;
    }

    int currentRejectTray() const
    {
        return m_currentRejectTray;
    }

    bool isPickedProductOk() const
    {
        return m_isPickedProductOk;
    }

    int currentSensorIndexInPicker() const
    {
        return m_currentSensorIndexInPicker;
    }

    int currentLensIndexInPicker() const
    {
        return m_currentLensIndexInPicker;
    }

public slots:
    void setRunMode(int runMode)
    {
        if (m_runMode == runMode)
            return;

        m_runMode = runMode;
        emit runModeChanged(m_runMode);
    }
    void setSutIsReadyToLoadMaterial(bool sutIsReadyToLoadMaterial)
    {
        if (m_sutIsReadyToLoadMaterial == sutIsReadyToLoadMaterial)
            return;

        m_sutIsReadyToLoadMaterial = sutIsReadyToLoadMaterial;
        emit sutIsReadyToLoadMaterialChanged(m_sutIsReadyToLoadMaterial);
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

    void setCurrentLensTray(int currentLensTray)
    {
        if (m_currentLensTray == currentLensTray)
            return;

        m_currentLensTray = currentLensTray;
        emit currentLensTrayChanged(m_currentLensTray);
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

    void setIsPickedProductOk(bool isPickedProductOk)
    {
        if (m_isPickedProductOk == isPickedProductOk)
            return;

        m_isPickedProductOk = isPickedProductOk;
        emit isPickedProductOkChanged(m_isPickedProductOk);
    }

    void setCurrentSensorIndexInPicker(int currentSensorIndexInPicker)
    {
        if (m_currentSensorIndexInPicker == currentSensorIndexInPicker)
            return;

        m_currentSensorIndexInPicker = currentSensorIndexInPicker;
        emit currentSensorIndexInPickerChanged(m_currentSensorIndexInPicker);
    }

    void setCurrentLensIndexInPicker(int currentLensIndexInPicker)
    {
        if (m_currentLensIndexInPicker == currentLensIndexInPicker)
            return;

        m_currentLensIndexInPicker = currentLensIndexInPicker;
        emit currentLensIndexInPickerChanged(m_currentLensIndexInPicker);
    }

signals:
    void runModeChanged(int runMode);
    void sutIsReadyToLoadMaterialChanged(bool sutIsReadyToLoadMaterial);
    void hasPickedSensorChanged(bool hasPickedSensor);
    void hasPickedProductChanged(bool hasPickedProduct);
    void hasPickedNgSensorChanged(bool hasPickedNgSensor);
    void hasPickedLensChanged(bool hasPickedLens);
    void hasPickedNgLensChanged(bool hasPickedNgLens);
    void currentLensTrayChanged(int currentLensTray);
    void currentSensorTrayChanged(int currentSensorTray);
    void currentRejectTrayChanged(int currentRejectTray);
    void isPickedProductOkChanged(bool isPickedProductOk);
    void currentSensorIndexInPickerChanged(int currentSensorIndexInPicker);
    void currentLensIndexInPickerChanged(int currentLensIndexInPicker);
};


#endif // SINGLEHEADMACHINEMATERIALLOADERMODULEPARAMETER_H
