#ifndef TOWERLIGHTBUZZERPARAMETER_H
#define TOWERLIGHTBUZZERPARAMETER_H

#include "Utils/propertybase.h"

class TowerLightBuzzerParameter:public PropertyBase{
    Q_OBJECT

public:
    TowerLightBuzzerParameter(){}
    Q_PROPERTY(QString towerLightBuzzerName READ towerLightBuzzerName WRITE setTowerLightBuzzerName NOTIFY towerLightBuzzerNameChanged)
    Q_PROPERTY(QString buzzerName READ buzzerName WRITE setBuzzerName NOTIFY buzzerNameChanged)
    Q_PROPERTY(QString redLightName READ redLightName WRITE setRedLightName NOTIFY redLightChanged)
    Q_PROPERTY(QString yellowLightName READ yellowLightName WRITE setYellowLightName NOTIFY yellowLightNameChanged)
    Q_PROPERTY(QString greenLightName READ greenLightName WRITE setgreenLightName NOTIFY greenLightNameChanged)


    QString buzzerName() const
    {
        return m_buzzerName;
    }
    QString redLightName() const
    {
        return m_redLightName;
    }

    QString yellowLightName() const
    {
        return m_yellowLightName;
    }

    QString greenLightName() const
    {
        return m_greenLightName;
    }

    QString towerLightBuzzerName() const
    {
        return m_towerLightBuzzerName;
    }

public slots:
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

    void setYellowLightName(QString yellowLightName)
    {
        if (m_yellowLightName == yellowLightName)
            return;

        m_yellowLightName = yellowLightName;
        emit yellowLightNameChanged(m_yellowLightName);
    }

    void setgreenLightName(QString greenLightName)
    {
        if (m_greenLightName == greenLightName)
            return;

        m_greenLightName = greenLightName;
        emit greenLightNameChanged(m_greenLightName);
    }

    void setTowerLightBuzzerName(QString towerLightBuzzerName)
    {
        if (m_towerLightBuzzerName == towerLightBuzzerName)
            return;

        m_towerLightBuzzerName = towerLightBuzzerName;
        emit towerLightBuzzerNameChanged(m_towerLightBuzzerName);
    }

signals:
    void buzzerNameChanged(QString buzzerName);
    void redLightChanged(QString redLightName);
    void yellowLightNameChanged(QString yellowLightName);
    void greenLightNameChanged(QString greenLightName);

    void towerLightBuzzerNameChanged(QString towerLightBuzzerName);

private:
    QString m_buzzerName = "";
    QString m_redLightName = "";
    QString m_yellowLightName = "";
    QString m_greenLightName = "";

    QString m_towerLightBuzzerName = "";
};


#endif // TOWERLIGHTBUZZERPARAMETER_H
