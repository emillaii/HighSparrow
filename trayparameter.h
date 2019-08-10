#ifndef MATERRIALTRAY_PARAMETER_H
#define MATERRIALTRAY_PARAMETER_H

#include "Utils/position_define.h"


class TrayParameter : public PropertyBase
{
    Q_OBJECT
public:
    TrayParameter():PropertyBase(){}
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int initState READ initState WRITE setInitState NOTIFY initStateChanged)
    Q_PROPERTY(bool needChange READ needChange WRITE setNeedChange NOTIFY needChangeChanged)
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

public slots:
    void changeTrayCount(int count)
    {
        while (count != tray_material_state.count()) {

            if(count > tray_material_state.count())
                tray_material_state.append(m_initState);
            else
                tray_material_state.removeLast();
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

signals:
    void currentIndexChanged(int currentIndex);
    void initStateChanged(int initState);

    void needChangeChanged(bool needChange);

private:
    int m_currentIndex = 0;
    int m_initState = 0;
    bool m_needChange = false;
};

#endif // MATERRIALTRAY_PARAMETER_H
