#ifndef MATERRIALTRAY_PARAMETER_H
#define MATERRIALTRAY_PARAMETER_H

#include "position_define.h"


class TrayParameter : public PropertyBase
{
    Q_OBJECT
public:
    TrayParameter():PropertyBase(){}
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int initState READ initState WRITE setInitState NOTIFY initStateChanged)
    Q_PROPERTY(bool needChange READ needChange WRITE setNeedChange NOTIFY needChangeChanged)
    Q_PROPERTY(QString trayName READ trayName WRITE setTrayName NOTIFY trayNameChanged)
    Q_PROPERTY(int trayNumber READ trayNumber WRITE setTrayNumber NOTIFY trayNumberChanged)
    Q_PROPERTY(int availableIndex READ availableIndex WRITE setAvailableIndex NOTIFY availableIndexChanged)
    Q_PROPERTY(QVariantList trayMaterialData READ trayMaterialData WRITE setTrayMaterialData NOTIFY trayMaterialDataChanged)
    Q_PROPERTY(QVariantList trayMaterialState READ trayMaterialState WRITE setTrayMaterialState NOTIFY trayMaterialStateChanged)
    Position tray_start_position;
    int currentIndex() const
    {
        return m_currentIndex;
    }
    int initState() const
    {
        return m_initState;
    }

    bool needChange() const
    {
        return m_needChange;
    }

    QString trayName() const
    {
        return m_trayName;
    }

    int trayNumber() const
    {
        return m_trayNumber;
    }

    QVariantList trayMaterialData() const
    {
        return m_trayMaterialData;
    }

    QVariantMap trayMaterialData(int index) const
    {
        if(index >= m_trayMaterialData.count())
            return QVariantMap();
        return m_trayMaterialData[index].toMap();
    }

    QVariantList trayMaterialState() const
    {
        return m_trayMaterialState;
    }

    int trayMaterialState(int index) const
    {
        if(index >= m_trayMaterialState.count())
            return -1;
        return m_trayMaterialState[index].toInt();
    }
    int availableIndex() const
    {
        return m_availableIndex;
    }

public slots:
    void changeTrayCount(int count)
    {
        while (count != m_trayMaterialState.count()) {

            if(count > m_trayMaterialState.count())
            {
                m_trayMaterialData.append(QVariantMap());
                m_trayMaterialState.append(m_initState);
            }
            else
            {
                m_trayMaterialData.removeLast();
                m_trayMaterialState.removeLast();
            }
        }
    }
    void setCurrentIndex(int currentIndex)
    {
        if (m_currentIndex == currentIndex)
            return;

        m_currentIndex = currentIndex;
        emit currentIndexChanged(m_currentIndex);
    }
    void setInitState(int initState)
    {
        if (m_initState == initState)
            return;

        m_initState = initState;
        emit initStateChanged(m_initState);
    }

    void setNeedChange(bool needChange)
    {
        if (m_needChange == needChange)
            return;

        m_needChange = needChange;
        emit needChangeChanged(m_needChange);
    }

    void setTrayName(QString trayName)
    {
        if (m_trayName == trayName)
            return;

        m_trayName = trayName;
        emit trayNameChanged(m_trayName);
    }

    void setTrayNumber(int trayNumber)
    {
        if (m_trayNumber == trayNumber)
            return;

        m_trayNumber = trayNumber;
        emit trayNumberChanged(m_trayNumber);
    }

    void setTrayMaterialData(QVariantList trayMaterialData)
    {
        if (m_trayMaterialData == trayMaterialData)
            return;

        m_trayMaterialData = trayMaterialData;
        emit trayMaterialDataChanged(m_trayMaterialData);
    }

    void setTrayMaterialData(int index,QVariantMap trayMaterialData)
    {
        if (index >= m_trayMaterialData.count())
            return;
        m_trayMaterialData[index] = trayMaterialData;
        emit trayMaterialDataChanged(m_trayMaterialData);
    }

    void setTrayMaterialState(QVariantList trayMaterialState)
    {
        if (m_trayMaterialState == trayMaterialState)
            return;

        m_trayMaterialState = trayMaterialState;
        emit trayMaterialStateChanged(m_trayMaterialState);
    }

    void setTrayMaterialState(int index,int trayMaterialState)
    {
        if (index >= m_trayMaterialState.count())
            return;

        m_trayMaterialState[index] = trayMaterialState;
        emit trayMaterialStateChanged(m_trayMaterialState);
    }

    void setAvailableIndex(int availableIndex)
    {
        if (m_availableIndex == availableIndex)
            return;

        m_availableIndex = availableIndex;
        emit availableIndexChanged(m_availableIndex);
    }

signals:
    void currentIndexChanged(int currentIndex);
    void initStateChanged(int initState);

    void needChangeChanged(bool needChange);

    void trayNameChanged(QString trayName);

    void trayNumberChanged(int trayNumber);

    void trayMaterialDataChanged(QVariantList trayMaterialData);

    void trayMaterialStateChanged(QVariantList trayMaterialState);

    void availableIndexChanged(int availableIndex);

private:
    int m_currentIndex = 0;
    int m_initState = 0;
    bool m_needChange = false;
    QString m_trayName = u8"1号Sensor盘";
    int m_trayNumber = 0;
    QVariantList m_trayMaterialData;
    QVariantList m_trayMaterialState;
    int m_availableIndex = 10000;
};

#endif // MATERRIALTRAY_PARAMETER_H
