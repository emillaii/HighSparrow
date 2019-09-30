#ifndef SENSORCLIPPARAMETER_H
#define SENSORCLIPPARAMETER_H

#include "propertybase.h"

class SensorClipParameter:public PropertyBase
{
    Q_OBJECT
public:
    SensorClipParameter():PropertyBase(){}
    Q_PROPERTY(double startPosition READ startPosition WRITE setStartPosition NOTIFY startPositionChanged)
    Q_PROPERTY(double endPosition READ endPosition WRITE setEndPosition NOTIFY endPositionChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(bool needChange READ needChange WRITE setNeedChange NOTIFY needChangeChanged)
    Q_PROPERTY(bool invertOrder READ invertOrder WRITE setInvertOrder NOTIFY invertOrderChanged)
    Q_PROPERTY(bool finishCurrent READ finishCurrent WRITE setFinishCurrent NOTIFY finishCurrentChanged)
    double startPosition() const
    {
        return m_startPosition;
    }
    double endPosition() const
    {
        return m_endPosition;
    }

    bool needChange() const
    {
        return m_needChange;
    }

    bool invertOrder() const
    {
        return m_invertOrder;
    }

    bool finishCurrent() const
    {
        return m_finishCurrent;
    }

    int currentIndex() const
    {
        return m_currentIndex;
    }

public slots:
    void setStartPosition(double startPosition)
    {
        if (qFuzzyCompare(m_startPosition, startPosition))
            return;

        m_startPosition = startPosition;
        emit startPositionChanged(m_startPosition);
    }
    void setEndPosition(double endPosition)
    {
        if (qFuzzyCompare(m_endPosition, endPosition))
            return;

        m_endPosition = endPosition;
        emit endPositionChanged(m_endPosition);
    }

    void setNeedChange(bool needChange)
    {
        if (m_needChange == needChange)
            return;

        m_needChange = needChange;
        emit needChangeChanged(m_needChange);
    }

    void setInvertOrder(bool invertOrder)
    {
        if (m_invertOrder == invertOrder)
            return;

        m_invertOrder = invertOrder;
        emit invertOrderChanged(m_invertOrder);
    }

    void setFinishCurrent(bool finishCurrent)
    {
        if (m_finishCurrent == finishCurrent)
            return;

        m_finishCurrent = finishCurrent;
        emit finishCurrentChanged(m_finishCurrent);
    }

    void setCurrentIndex(int currentIndex)
    {
        if (m_currentIndex == currentIndex)
            return;

        m_currentIndex = currentIndex;
        emit currentIndexChanged(m_currentIndex);
    }

signals:
    void startPositionChanged(double startPosition);

    void endPositionChanged(double endPosition);

    void needChangeChanged(bool needChange);

    void invertOrderChanged(bool invertOrder);

    void finishCurrentChanged(bool finishCurrent);

    void currentIndexChanged(int currentIndex);

private:
    double m_startPosition = 0;
    double m_endPosition = 0;
    bool m_needChange = false;
    bool m_invertOrder = false;
    bool m_finishCurrent = false;
    int m_currentIndex = 0;
};

class SensorClipStandParameter: public PropertyBase
{
    Q_OBJECT
public:
    SensorClipStandParameter():PropertyBase(){}
    Q_PROPERTY(double count READ count WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(double delta READ delta WRITE setDelta NOTIFY deltaChanged)
    double count() const
    {
        return m_count;
    }
    double delta() const
    {
        return m_delta;
    }

public slots:
    void setCount(double count)
    {
        if (qFuzzyCompare(m_count, count))
            return;

        m_count = count;
        emit countChanged(m_count);
    }
    void setDelta(double delta)
    {
        if (qFuzzyCompare(m_delta, delta))
            return;

        m_delta = delta;
        emit deltaChanged(m_delta);
    }

signals:
    void countChanged(double count);
    void deltaChanged(double delta);

private:
    double m_count = 2;
    double m_delta = 0;
};

#endif // SENSORCLIPPARAMETER_H
