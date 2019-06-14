#ifndef LUT_PARAMERTER_H
#define LUT_PARAMERTER_H
#include "propertybase.h"

class LutParameter:public PropertyBase
{
    Q_OBJECT
public:
    LutParameter():PropertyBase(){}
    Q_PROPERTY(double pickForce READ pickForce WRITE setPickForce NOTIFY paramsChanged)
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(QString motorZName READ motorZName WRITE setMotorZName NOTIFY motorZNameChanged)
    Q_PROPERTY(QString vacuum1Name READ vacuum1Name WRITE setVacuum1Name NOTIFY vacuum1NameChanged)
    Q_PROPERTY(QString vacuum2Name READ vacuum2Name WRITE setVacuum2Name NOTIFY vacuum2NameChanged)
    Q_PROPERTY(QString uplookLocationName READ uplookLocationName WRITE setUplookLocationName NOTIFY uplookLocationNameChanged)
    Q_PROPERTY(QString loadlookLocationName READ loadlookLocationName WRITE setLoadlookLocationName NOTIFY loadlookLocationNameChanged)
    Q_PROPERTY(QString mushroomLocationName READ mushroomLocationName WRITE setMushroomLocationName NOTIFY mushroomLocationNameChanged)
    Q_PROPERTY(double accumulatedHour READ accumulatedHour WRITE setAccumulatedHour NOTIFY accumulatedHourChanged)
    double pickForce() const
    {
        return m_PickForce;
    }

    QString motorXName() const
    {
        return m_motorXName;
    }

    QString motorYName() const
    {
        return m_motorYName;
    }

    QString motorZName() const
    {
        return m_motorZName;
    }

    QString vacuum1Name() const
    {
        return m_vacuum1Name;
    }

    QString vacuum2Name() const
    {
        return m_vacuum2Name;
    }

    QString uplookLocationName() const
    {
        return m_uplookLocationName;
    }

    QString loadlookLocationName() const
    {
        return m_loadlookLocationName;
    }

    QString mushroomLocationName() const
    {
        return m_mushroomLocationName;
    }

    double accumulatedHour() const
    {
        return m_accumulatedHour;
    }

public slots:
    void setPickForce(double PickForce)
    {
        m_PickForce = PickForce;
        emit paramsChanged();
    }

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

    void setMotorZName(QString motorZName)
    {
        if (m_motorZName == motorZName)
            return;

        m_motorZName = motorZName;
        emit motorZNameChanged(m_motorZName);
    }

    void setVacuum1Name(QString vacuum1Name)
    {
        if (m_vacuum1Name == vacuum1Name)
            return;

        m_vacuum1Name = vacuum1Name;
        emit vacuum1NameChanged(m_vacuum1Name);
    }

    void setVacuum2Name(QString vacuum2Name)
    {
        if (m_vacuum2Name == vacuum2Name)
            return;

        m_vacuum2Name = vacuum2Name;
        emit vacuum2NameChanged(m_vacuum2Name);
    }

    void setUplookLocationName(QString uplookLocationName)
    {
        if (m_uplookLocationName == uplookLocationName)
            return;

        m_uplookLocationName = uplookLocationName;
        emit uplookLocationNameChanged(m_uplookLocationName);
    }

    void setLoadlookLocationName(QString loadlookLocationName)
    {
        if (m_loadlookLocationName == loadlookLocationName)
            return;

        m_loadlookLocationName = loadlookLocationName;
        emit loadlookLocationNameChanged(m_loadlookLocationName);
    }

    void setMushroomLocationName(QString mushroomLocationName)
    {
        if (m_mushroomLocationName == mushroomLocationName)
            return;

        m_mushroomLocationName = mushroomLocationName;
        emit mushroomLocationNameChanged(m_mushroomLocationName);
    }

    void setAccumulatedHour(double accumulatedHour)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_accumulatedHour, accumulatedHour))
            return;

        m_accumulatedHour = accumulatedHour;
        emit accumulatedHourChanged(m_accumulatedHour);
    }

signals:
    void paramsChanged();

    void motorXNameChanged(QString motorXName);

    void motorYNameChanged(QString motorYName);

    void motorZNameChanged(QString motorZName);

    void vacuum1NameChanged(QString vacuum1Name);

    void vacuum2NameChanged(QString vacuum2Name);

    void uplookLocationNameChanged(QString uplookLocationName);

    void loadlookLocationNameChanged(QString loadlookLocationName);

    void mushroomLocationNameChanged(QString mushroomLocationName);

    void accumulatedHourChanged(double accumulatedHour);

private:
    double m_PickForce = 0;
    QString m_motorXName = "LUT_X";
    QString m_motorYName = "LUT_Y";
    QString m_motorZName = "LUT_Z";
    QString m_vacuum1Name = "LUT_V1";
    QString m_vacuum2Name = "LUT_V2";
    QString m_uplookLocationName = "";
    QString m_loadlookLocationName = "";
    QString m_mushroomLocationName = "";
    double m_accumulatedHour = 0;
};

class LutState:public PropertyBase
{
    Q_OBJECT
    Q_PROPERTY(int lutTrayID READ lutTrayID WRITE setLutTrayID NOTIFY lutTrayIDChanged)
    Q_PROPERTY(int lutLensID READ lutLensID WRITE setLutLensID NOTIFY lutLensIDChanged)
    Q_PROPERTY(int lutNgTrayID READ lutNgTrayID WRITE setLutNgTrayID NOTIFY lutNgTrayIDChanged)
    Q_PROPERTY(int lutNgLensID READ lutNgLensID WRITE setLutNgLensID NOTIFY lutNgLensIDChanged)
    Q_PROPERTY(int aa1TrayID READ aa1TrayID WRITE setAa1TrayID NOTIFY aa1TrayIDChanged)
    Q_PROPERTY(int aa1LensID READ aa1LensID WRITE setAa1LensID NOTIFY aa1LensIDChanged)
    Q_PROPERTY(int aa2TrayID READ aa2TrayID WRITE setAa2TrayID NOTIFY aa2TrayIDChanged)
    Q_PROPERTY(int aa2LensID READ aa2LensID WRITE setAa2LensID NOTIFY aa2LensIDChanged)
    Q_PROPERTY(bool waitLens READ waitLens WRITE setWaitLens NOTIFY waitLensChanged)
    Q_PROPERTY(QString servingIP READ servingIP WRITE setServingIP NOTIFY servingIPChanged)
    Q_PROPERTY(bool lutHasLens READ lutHasLens WRITE setLutHasLens NOTIFY lutHasLensChanged)
    Q_PROPERTY(bool pickingLens READ pickingLens WRITE setPickingLens NOTIFY pickingLensChanged)
    Q_PROPERTY(QString cmd READ cmd WRITE setCmd NOTIFY cmdChanged)
    Q_PROPERTY(bool pickedLens READ pickedLens WRITE setPickedLens NOTIFY pickedLensChanged)
    Q_PROPERTY(bool unpickedNgLens READ unpickedNgLens WRITE setUnpickedNgLens NOTIFY unpickedNgLensChanged)
public:
    int lutTrayID() const
    {
        return m_lutTrayID;
    }

    int lutLensID() const
    {
        return m_lutLensID;
    }

    int lutNgTrayID() const
    {
        return m_lutNgTrayID;
    }

    int lutNgLensID() const
    {
        return m_lutNgLensID;
    }

    int aa1TrayID() const
    {
        return m_aa1TrayID;
    }

    int aa1LensID() const
    {
        return m_aa1LensID;
    }

    int aa2TrayID() const
    {
        return m_aa2TrayID;
    }

    int aa2LensID() const
    {
        return m_aa2LensID;
    }

    bool waitLens() const
    {
        return m_waitLens;
    }

    QString servingIP() const
    {
        return m_serviceIP;
    }

    bool lutHasLens() const
    {
        return m_lutHasLens;
    }

    bool pickingLens() const
    {
        return m_pickingLens;
    }

    QString cmd() const
    {
        return m_cmd;
    }

    bool pickedLens() const
    {
        return m_pickedLens;
    }

    bool unpickedNgLens() const
    {
        return m_unpickedNgLens;
    }

public slots:
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

    void setAa1TrayID(int aa1TrayID)
    {
        if (m_aa1TrayID == aa1TrayID)
            return;

        m_aa1TrayID = aa1TrayID;
        emit aa1TrayIDChanged(m_aa1TrayID);
    }

    void setAa1LensID(int aa1LensID)
    {
        if (m_aa1LensID == aa1LensID)
            return;

        m_aa1LensID = aa1LensID;
        emit aa1LensIDChanged(m_aa1LensID);
    }

    void setAa2TrayID(int aa2TrayID)
    {
        if (m_aa2TrayID == aa2TrayID)
            return;

        m_aa2TrayID = aa2TrayID;
        emit aa2TrayIDChanged(m_aa2TrayID);
    }

    void setAa2LensID(int aa2LensID)
    {
        if (m_aa2LensID == aa2LensID)
            return;

        m_aa2LensID = aa2LensID;
        emit aa2LensIDChanged(m_aa2LensID);
    }

    void setWaitLens(bool waitLens)
    {
        if (m_waitLens == waitLens)
            return;

        m_waitLens = waitLens;
        emit waitLensChanged(m_waitLens);
    }

    void setServingIP(QString serviceIP)
    {
        if (m_serviceIP == serviceIP)
            return;

        m_serviceIP = serviceIP;
        emit servingIPChanged(m_serviceIP);
    }

    void setLutHasLens(bool lutHasLens)
    {
        if (m_lutHasLens == lutHasLens)
            return;

        m_lutHasLens = lutHasLens;
        emit lutHasLensChanged(m_lutHasLens);
    }

    void setPickingLens(bool pickingLens)
    {
        if (m_pickingLens == pickingLens)
            return;

        m_pickingLens = pickingLens;
        emit pickingLensChanged(m_pickingLens);
    }

    void setCmd(QString cmd)
    {
        if (m_cmd == cmd)
            return;

        m_cmd = cmd;
        emit cmdChanged(m_cmd);
    }

    void setPickedLens(bool pickedLens)
    {
        if (m_pickedLens == pickedLens)
            return;

        m_pickedLens = pickedLens;
        emit pickedLensChanged(m_pickedLens);
    }

    void setUnpickedNgLens(bool unpickedNgLens)
    {
        if (m_unpickedNgLens == unpickedNgLens)
            return;

        m_unpickedNgLens = unpickedNgLens;
        emit unpickedNgLensChanged(m_unpickedNgLens);
    }

signals:
    void lutTrayIDChanged(int lutTrayID);

    void lutLensIDChanged(int lutLensID);

    void lutNgTrayIDChanged(int lutNgTrayID);

    void lutNgLensIDChanged(int lutNgLensID);

    void aa1TrayIDChanged(int aa1TrayID);

    void aa1LensIDChanged(int aa1LensID);

    void aa2TrayIDChanged(int aa2TrayID);

    void aa2LensIDChanged(int aa2LensID);

    void waitLensChanged(bool waitLens);

    void servingIPChanged(QString servingIP);

    void lutHasLensChanged(bool lutHasLens);

    void pickingLensChanged(bool pickingLens);


    void cmdChanged(QString cmd);


    void pickedLensChanged(bool pickedLens);

    void unpickedNgLensChanged(bool unpickedNgLens);

private:
    int m_lutTrayID = -1;
    int m_lutLensID = -1;
    int m_lutNgTrayID = -1;
    int m_lutNgLensID = -1;
    int m_aa1TrayID = -1;
    int m_aa1LensID = -1;
    int m_aa2TrayID = -1;
    int m_aa2LensID = -1;
    bool m_waitLens = false;
    QString m_serviceIP = "";
    bool m_lutHasLens = false;
    bool m_pickingLens = false;
    QString m_cmd = "";
    bool m_pickedLens = false;
    bool m_unpickedNgLens = false;
};

#endif // LUT_PARAMERTER_H
