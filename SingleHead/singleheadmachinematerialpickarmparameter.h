#ifndef SINGLEHEADMACHINEMATERIALPICKARMPARAMETER_H
#define SINGLEHEADMACHINEMATERIALPICKARMPARAMETER_H

#include "Utils/propertybase.h"


class SingleHeadMachineMaterialPickArmParameter:public PropertyBase{
    Q_OBJECT
public:
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(QString motorTh1Name READ motorTh1Name WRITE setMotorTh1Name NOTIFY motorTh1NameChanged)
    Q_PROPERTY(QString motorTh2Name READ motorTh2Name WRITE setMotorTh2Name NOTIFY motorTh2NameChanged)
    Q_PROPERTY(QString motorVcm1Name READ motorVcm1Name WRITE setMotorVcm1Name NOTIFY motorVcm1NameChanged)
    Q_PROPERTY(QString motorVcm2Name READ motorVcm2Name WRITE setMotorVcm2Name NOTIFY motorVcm2NameChanged)
    Q_PROPERTY(QString motorVcmXName READ motorVcmXName WRITE setMotorVcmXName NOTIFY motorVcmXNameChanged)
    Q_PROPERTY(QString vacuumLensSuctionName READ vacuumLensSuctionName WRITE setVacuumLensSuctionName NOTIFY vacuumLensSuctionNameChanged)
    Q_PROPERTY(QString vacuumSensorSuctionName READ vacuumSensorSuctionName WRITE setVacuumSensorSuctionName NOTIFY vacuumSensorSuctionNameChanged)
    Q_PROPERTY(QString vacuumLUTName READ vacuumLUTName WRITE setVacuumLUTName NOTIFY vacuumLUTNameChanged)
    Q_PROPERTY(QString vacuumSUTName READ vacuumSUTName WRITE setVacuumSUTName NOTIFY vacuumSUTNameChanged)
    Q_PROPERTY(QString cylinderName READ cylinderName WRITE setCylinderName NOTIFY cylinderNameChanged)
    Q_PROPERTY(double pickLensZ READ pickLensZ WRITE setPickLensZ NOTIFY pickLensZChanged)
    Q_PROPERTY(double pickSensorZ READ pickSensorZ WRITE setPickSensorZ NOTIFY pickSensorZChanged)
    Q_PROPERTY(double placeLensZ READ placeLensZ WRITE setPlaceLensZ NOTIFY placeLensZChanged)
    Q_PROPERTY(double placeSensorZ READ placeSensorZ WRITE setPlaceSensorZ NOTIFY placeSensorZChanged)
    Q_PROPERTY(double pickProductZ READ pickProductZ WRITE setPickProductZ NOTIFY pickProductZChanged)
    Q_PROPERTY(double placeProductZ READ placeProductZ WRITE setPlaceProductZ NOTIFY placeProductZChanged)
    Q_PROPERTY(double pickNgLensZ READ pickNgLensZ WRITE setPickNgLensZ NOTIFY pickNgLensZChanged)
    Q_PROPERTY(double placeNgLensZ READ placeNgLensZ WRITE setPlaceNgLensZ NOTIFY placeNgLensZChanged)
    Q_PROPERTY(double pickNgSensorZ READ pickNgSensorZ WRITE setPickNgSensorZ NOTIFY pickNgSensorZChanged)
    Q_PROPERTY(double placeNgSensorZ READ placeNgSensorZ WRITE setPlaceNgSensorZ NOTIFY placeNgSensorZChanged)
    Q_PROPERTY(double pickLensTheta READ pickLensTheta WRITE setPickLensTheta NOTIFY pickLensThetaChanged)
    Q_PROPERTY(double placeLensTheta READ placeLensTheta WRITE setPlaceLensTheta NOTIFY placeLensThetaChanged)

public:
    SingleHeadMachineMaterialPickArmParameter():PropertyBase(){}
    QString motorXName() const
    {
        return m_motorXName;
    }

    QString motorYName() const
    {
        return m_motorYName;
    }

//    QString motorZName() const
//    {
//        return m_motorZName;
//    }

    QString motorTh1Name() const
    {
        return m_motorTh1Name;
    }

    QString motorTh2Name() const
    {
        return m_motorTh2Name;
    }

    QString motorVcm1Name() const
    {
        return m_motorVcm1Name;
    }

    QString motorVcm2Name() const
    {
        return m_motorVcm2Name;
    }

    QString vacuumLensSuctionName() const
    {
        return m_vacuumLensSuctionName;
    }

    QString vacuumSensorSuctionName() const
    {
        return m_vacuumSensorSuctionName;
    }

    QString vacuumLUTName() const
    {
        return m_vacuumLUTName;
    }

    QString vacuumSUTName() const
    {
        return m_vacuumSUTName;
    }

    QString cylinderName() const
    {
        return m_cylinderName;
    }

    QString motorVcmXName() const
    {
        return m_motorVcmXName;
    }

    double pickLensZ() const
    {
        return m_pickLensZ;
    }

    double pickSensorZ() const
    {
        return m_pickSensorZ;
    }

    double placeLensZ() const
    {
        return m_placeLensZ;
    }

    double placeSensorZ() const
    {
        return m_placeSensorZ;
    }

    double pickProductZ() const
    {
        return m_pickProductZ;
    }

    double placeProductZ() const
    {
        return m_placeProductZ;
    }

    double pickNgLensZ() const
    {
        return m_pickNgLensZ;
    }

    double placeNgLensZ() const
    {
        return m_placeNgLensZ;
    }

    double pickNgSensorZ() const
    {
        return m_pickNgSensorZ;
    }

    double placeNgSensorZ() const
    {
        return m_placeNgSensorZ;
    }

    double pickLensTheta() const
    {
        return m_pickLensTheta;
    }

    double placeLensTheta() const
    {
        return m_placeLensTheta;
    }

public slots:
    void setMotorXName(QString motorXName)
    {
        if (m_motorXName == motorXName)
            return;

        m_motorXName = motorXName;
        emit motorXNameChanged(m_motorXName);
    }

    void setMotorYName(QString motorYName)
    {
        if (m_motorYName == motorYName)
            return;

        m_motorYName = motorYName;
        emit motorYNameChanged(m_motorYName);
    }

//    void setMotorZName(QString motorZName)
//    {
//        if (m_motorZName == motorZName)
//            return;

//        m_motorZName = motorZName;
//        emit motorZNameChanged(m_motorZName);
//    }

    void setMotorTh1Name(QString motorTh1Name)
    {
        if (m_motorTh1Name == motorTh1Name)
            return;

        m_motorTh1Name = motorTh1Name;
        emit motorTh1NameChanged(m_motorTh1Name);
    }

    void setMotorTh2Name(QString motorTh2Name)
    {
        if (m_motorTh2Name == motorTh2Name)
            return;

        m_motorTh2Name = motorTh2Name;
        emit motorTh2NameChanged(m_motorTh2Name);
    }

    void setMotorVcm1Name(QString motorVcm1Name)
    {
        if (m_motorVcm1Name == motorVcm1Name)
            return;

        m_motorVcm1Name = motorVcm1Name;
        emit motorVcm1NameChanged(m_motorVcm1Name);
    }

    void setMotorVcm2Name(QString motorVcm2Name)
    {
        if (m_motorVcm2Name == motorVcm2Name)
            return;

        m_motorVcm2Name = motorVcm2Name;
        emit motorVcm2NameChanged(m_motorVcm2Name);
    }

    void setVacuumLensSuctionName(QString vacuumLensSuctionName)
    {
        if (m_vacuumLensSuctionName == vacuumLensSuctionName)
            return;

        m_vacuumLensSuctionName = vacuumLensSuctionName;
        emit vacuumLensSuctionNameChanged(m_vacuumLensSuctionName);
    }

    void setVacuumSensorSuctionName(QString vacuumSensorSuctionName)
    {
        if (m_vacuumSensorSuctionName == vacuumSensorSuctionName)
            return;

        m_vacuumSensorSuctionName = vacuumSensorSuctionName;
        emit vacuumSensorSuctionNameChanged(m_vacuumSensorSuctionName);
    }

    void setVacuumLUTName(QString vacuumLUTName)
    {
        if (m_vacuumLUTName == vacuumLUTName)
            return;

        m_vacuumLUTName = vacuumLUTName;
        emit vacuumLUTNameChanged(m_vacuumLUTName);
    }

    void setVacuumSUTName(QString vacuumSUTName)
    {
        if (m_vacuumSUTName == vacuumSUTName)
            return;

        m_vacuumSUTName = vacuumSUTName;
        emit vacuumSUTNameChanged(m_vacuumSUTName);
    }

    void setCylinderName(QString cylinderName)
    {
        if (m_cylinderName == cylinderName)
            return;

        m_cylinderName = cylinderName;
        emit cylinderNameChanged(m_cylinderName);
    }

    void setMotorVcmXName(QString motorVcmXName)
    {
        if (m_motorVcmXName == motorVcmXName)
            return;

        m_motorVcmXName = motorVcmXName;
        emit motorVcmXNameChanged(m_motorVcmXName);
    }

    void setPickLensZ(double pickLensZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pickLensZ, pickLensZ))
            return;

        m_pickLensZ = pickLensZ;
        emit pickLensZChanged(m_pickLensZ);
    }

    void setPickSensorZ(double pickSensorZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pickSensorZ, pickSensorZ))
            return;

        m_pickSensorZ = pickSensorZ;
        emit pickSensorZChanged(m_pickSensorZ);
    }

    void setPlaceLensZ(double placeLensZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_placeLensZ, placeLensZ))
            return;

        m_placeLensZ = placeLensZ;
        emit placeLensZChanged(m_placeLensZ);
    }

    void setPlaceSensorZ(double placeSensorZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_placeSensorZ, placeSensorZ))
            return;

        m_placeSensorZ = placeSensorZ;
        emit placeSensorZChanged(m_placeSensorZ);
    }

    void setPickProductZ(double pickProductZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pickProductZ, pickProductZ))
            return;

        m_pickProductZ = pickProductZ;
        emit pickProductZChanged(m_pickProductZ);
    }

    void setPlaceProductZ(double placeProductZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_placeProductZ, placeProductZ))
            return;

        m_placeProductZ = placeProductZ;
        emit placeProductZChanged(m_placeProductZ);
    }

    void setPickNgLensZ(double pickNgLensZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pickNgLensZ, pickNgLensZ))
            return;

        m_pickNgLensZ = pickNgLensZ;
        emit pickNgLensZChanged(m_pickNgLensZ);
    }

    void setPlaceNgLensZ(double placeNgLensZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_placeNgLensZ, placeNgLensZ))
            return;

        m_placeNgLensZ = placeNgLensZ;
        emit placeNgLensZChanged(m_placeNgLensZ);
    }

    void setPickNgSensorZ(double pickNgSensorZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pickNgSensorZ, pickNgSensorZ))
            return;

        m_pickNgSensorZ = pickNgSensorZ;
        emit pickNgSensorZChanged(m_pickNgSensorZ);
    }

    void setPlaceNgSensorZ(double placeNgSensorZ)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_placeNgSensorZ, placeNgSensorZ))
            return;

        m_placeNgSensorZ = placeNgSensorZ;
        emit placeNgSensorZChanged(m_placeNgSensorZ);
    }

    void setPickLensTheta(double pickLensTheta)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_pickLensTheta, pickLensTheta))
            return;

        m_pickLensTheta = pickLensTheta;
        emit pickLensThetaChanged(m_pickLensTheta);
    }

    void setPlaceLensTheta(double placeLensTheta)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_placeLensTheta, placeLensTheta))
            return;

        m_placeLensTheta = placeLensTheta;
        emit placeLensThetaChanged(m_placeLensTheta);
    }

signals:
    void motorXNameChanged(QString motorXName);

    void motorYNameChanged(QString motorYName);

    void motorZNameChanged(QString motorZName);

    void motorTh1NameChanged(QString motorTh1Name);

    void motorTh2NameChanged(QString motorTh2Name);

    void motorVcm1NameChanged(QString motorVcm1Name);

    void motorVcm2NameChanged(QString motorVcm2Name);

    void vacuumLensSuctionNameChanged(QString vacuumLensSuctionName);

    void vacuumSensorSuctionNameChanged(QString vacuumSensorSuctionName);

    void vacuumLUTNameChanged(QString vacuumLUTName);

    void vacuumSUTNameChanged(QString vacuumSUTName);

    void cylinderNameChanged(QString cylinderName);

    void motorVcmXNameChanged(QString motorVcmXName);

    void pickLensZChanged(double pickLensZ);

    void pickSensorZChanged(double pickSensorZ);

    void placeLensZChanged(double placeLensZ);

    void placeSensorZChanged(double placeSensorZ);

    void pickProductZChanged(double pickProductZ);

    void placeProductZChanged(double placeProductZ);

    void pickNgLensZChanged(double pickNgLensZ);

    void placeNgLensZChanged(double placeNgLensZ);

    void pickNgSensorZChanged(double pickNgSensorZ);

    void placeNgSensorZChanged(double placeNgSensorZ);

    void pickLensThetaChanged(double pickLensTheta);

    void placeLensThetaChanged(double placeLensTheta);

private:
    QString m_motorXName = "";
    QString m_motorYName = "";
//    QString m_motorZName = "";
    QString m_motorTh1Name = "";
    QString m_motorTh2Name = "";
    QString m_motorVcm1Name = "";
    QString m_motorVcm2Name = "";
    QString m_vacuumLensSuctionName = "";
    QString m_vacuumSensorSuctionName = "";
    QString m_vacuumLUTName = "";
    QString m_vacuumSUTName = "";
    QString m_cylinderName = "";
    QString m_motorVcmXName = "";
    double m_pickLensZ = 0;
    double m_pickSensorZ = 0;
    double m_placeLensZ = 0;
    double m_placeSensorZ = 0;
    double m_pickProductZ = 0;
    double m_placeProductZ = 0;
    double m_pickNgLensZ = 0;
    double m_placeNgLensZ = 0;
    double m_pickNgSensorZ = 0;
    double m_placeNgSensorZ = 0;
    double m_pickLensTheta = 0;
    double m_placeLensTheta = 0;
};

#endif // SINGLEHEADMACHINEMATERIALPICKARMPARAMETER_H
