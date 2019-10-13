#ifndef MATERRIALTRAY_PARAMETER_H
#define MATERRIALTRAY_PARAMETER_H

#include "Utils/position_define.h"
#include <QVariantMap>


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

    QList<QVariantMap> tray_material_data;
    QList<int> tray_material_state; // List of materail state in current tray whether it is sensor/lens/vacancy/ng, it is desgin for unit mapping to display
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

public slots:
    void changeTrayCount(int count)
    {
        while (count != tray_material_state.count()) {

            if(count > tray_material_state.count())
            {
                tray_material_state.append(m_initState);
                tray_material_data.append(QVariantMap());
            }
            else
            {
                tray_material_state.removeLast();
                tray_material_data.removeLast();
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

signals:
    void currentIndexChanged(int currentIndex);
    void initStateChanged(int initState);

    void needChangeChanged(bool needChange);

    void trayNameChanged(QString trayName);

    void trayNumberChanged(int trayNumber);

private:
    int m_currentIndex = 0;
    int m_initState = 0;
    bool m_needChange = false;
    QString m_trayName;
    int m_trayNumber;
};

#endif // MATERRIALTRAY_PARAMETER_H
