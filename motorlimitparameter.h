#ifndef MOTORLIMITPARAMETER_H
#define MOTORLIMITPARAMETER_H

#include "propertybase.h"



class MotorLimitParameter:public PropertyBase
{
public:
    MotorLimitParameter():PropertyBase(){}
    Q_PROPERTY(bool allowMove READ allowMove WRITE setAllowMove NOTIFY allowMoveChanged)
    Q_PROPERTY(double startLimit READ startLimit WRITE setStartLimit NOTIFY startLimitChanged)
    Q_PROPERTY(double endLimit READ endLimit WRITE setEndLimit NOTIFY endLimitChanged)
    Q_PROPERTY(QString motorName READ motorName WRITE setMotorName NOTIFY motorNameChanged)
    Q_PROPERTY(double startPosition READ startPosition WRITE setStartPosition NOTIFY startPositionChanged)
    Q_PROPERTY(double endPosition READ endPosition WRITE setEndPosition NOTIFY endPositionChanged)
    bool allowMove() const
    {
        return m_allowMove;
    }
    double startLimit() const
    {
        return m_startLimit;
    }

    double endLimit() const
    {
        return m_endLimit;
    }

    QString motorName() const
    {
        return m_motorName;
    }

    double startPosition() const
    {
        return m_startPosition;
    }

    double endPosition() const
    {
        return m_endPosition;
    }

public slots:
    void setAllowMove(bool allowMove)
    {
        if (m_allowMove == allowMove)
            return;

        m_allowMove = allowMove;
        emit allowMoveChanged(m_allowMove);
    }
    void setStartLimit(double startLimit)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_startLimit, startLimit))
            return;

        m_startLimit = startLimit;
        emit startLimitChanged(m_startLimit);
    }

    void setEndLimit(double endLimit)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_endLimit, endLimit))
            return;

        m_endLimit = endLimit;
        emit endLimitChanged(m_endLimit);
    }

    void setMotorName(QString motorName)
    {
        if (m_motorName == motorName)
            return;

        m_motorName = motorName;
        emit motorNameChanged(m_motorName);
    }

    void setStartPosition(double startPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_startPosition, startPosition))
            return;

        m_startPosition = startPosition;
        emit startPositionChanged(m_startPosition);
    }

    void setEndPosition(double endPosition)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_endPosition, endPosition))
            return;

        m_endPosition = endPosition;
        emit endPositionChanged(m_endPosition);
    }

signals:
    void allowMoveChanged(bool allowMove);
    void startLimitChanged(double startLimit);

    void endLimitChanged(double endLimit);

    void motorNameChanged(QString motorName);

    void startPositionChanged(double startPosition);

    void endPositionChanged(double endPosition);

private:
    bool m_allowMove = false;
    double m_startLimit = 0;
    double m_endLimit = 0;
    QString m_motorName = "" ;
    double m_startPosition = 0;
    double m_endPosition = 0;
};

#endif // MOTORLIMITPARAMETER_H
