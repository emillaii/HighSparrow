#ifndef AADATA_H
#define AADATA_H

#include <QObject>
#include <QPointF>
#include <QTimer>
#include <QList>
#include <QPointF>
#include <QThread>

class AAData : public QThread
{
    Q_OBJECT
    Q_PROPERTY(QPointF wValue READ wValue NOTIFY wValueChanged)
    Q_PROPERTY(QPointF wCCValue READ wCCValue)
    Q_PROPERTY(QPointF wULValue READ wULValue)
    Q_PROPERTY(QPointF wURValue READ wURValue)
    Q_PROPERTY(QPointF wLLValue READ wLLValue)
    Q_PROPERTY(QPointF wLRValue READ wLRValue)
    Q_PROPERTY(NOTIFY wValueClear)
    Q_PROPERTY(NOTIFY wValueChanged)

public:
    AAData(QObject *parent=Q_NULLPTR);
    void addData(int i, double x, double y);
    void clear();
    void plot();
    QPointF wValue() const{
        return m_wValue;
    }
    QPointF wCCValue() const
    {
        return m_wCCValue;
    }

    QPointF wULValue() const
    {
        return m_wULValue;
    }

    QPointF wURValue() const
    {
        return m_wURValue;
    }

    QPointF wLLValue() const
    {
        return m_wLLValue;
    }

    QPointF wLRValue() const
    {
        return m_wLRValue;
    }

signals:
    void wValueChanged();
    void wValueClear();
private slots:
    void wTimeout();
private:
    QTimer * m_wTimer;
    QPointF m_wValue;
    QList<QPointF> m_CCPointsList;
    QList<QPointF> m_ULPointsList;
    QList<QPointF> m_URPointsList;
    QList<QPointF> m_LLPointsList;
    QList<QPointF> m_LRPointsList;
    QPointF m_wCCValue;
    QPointF m_wULValue;
    QPointF m_wURValue;
    QPointF m_wLLValue;
    QPointF m_wLRValue;

protected:
    void run();
};


#endif // AADATA_H
