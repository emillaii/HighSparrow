#ifndef TRAYCLIPSTANDARDSPARAMETER_H
#define TRAYCLIPSTANDARDSPARAMETER_H

#include "propertybase.h"


class ClipStandardsParameter:public PropertyBase{
    Q_OBJECT
private:
    int m_columnCount=2;
    bool m_fromTop=1;
    double m_columnDelta=0;
    double m_changeClipPos = 0;
    double m_firstTrayPos=0;
    double m_lastTrayPos=0;
    int m_currentIndex=0;

    bool m_needChangeClip = false;

public:
    ClipStandardsParameter():PropertyBase (){
        connect(this,SIGNAL(columnCountChanged(int)),this,SLOT(calColumnDelta()));
        connect(this,SIGNAL(firstTrayPosChanged(double)),this,SLOT(calColumnDelta()));
        connect(this,SIGNAL(lastTrayPosChanged(double)),this,SLOT(calColumnDelta()));
    }
    Q_PROPERTY(int columnCount READ columnCount WRITE setColumnCount NOTIFY columnCountChanged)
    Q_PROPERTY(bool fromTop READ fromTop WRITE setFromTop NOTIFY fromTopChanged)
    Q_PROPERTY(double columnDelta READ columnDelta WRITE setColumnDelta NOTIFY columnDeltaChanged)
    Q_PROPERTY(double changeClipPos READ changeClipPos WRITE setChangeClipPos NOTIFY changeClipPosChanged)
    Q_PROPERTY(double firstTrayPos READ firstTrayPos WRITE setFirstTrayPos NOTIFY firstTrayPosChanged)
    Q_PROPERTY(double lastTrayPos READ lastTrayPos WRITE setLastTrayPos NOTIFY lastTrayPosChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(bool needChangeClip READ needChangeClip WRITE setNeedChangeClip NOTIFY needChangeClipChanged)
int columnCount() const
{
    return m_columnCount;
}
bool fromTop() const
{
    return m_fromTop;
}

double columnDelta() const
{
    return m_columnDelta;
}

double changeClipPos() const
{
    return m_changeClipPos;
}

double firstTrayPos() const
{
    return m_firstTrayPos;
}

double lastTrayPos() const
{
    return m_lastTrayPos;
}

int currentIndex() const
{
    return m_currentIndex;
}

bool needChangeClip() const
{
    return m_needChangeClip;
}

public slots:
void calColumnDelta(){
    double delta = (m_lastTrayPos-m_firstTrayPos)/(m_columnCount-1);
    setColumnDelta(delta);
}
void setColumnCount(int columnCount)
{
    if (m_columnCount == columnCount)
        return;

    m_columnCount = columnCount;
    emit columnCountChanged(m_columnCount);
}
void setFromTop(bool fromTop)
{
    if (m_fromTop == fromTop)
        return;

    m_fromTop = fromTop;
    emit fromTopChanged(m_fromTop);
}

void setColumnDelta(double columnDelta)
{
    if (qFuzzyCompare(m_columnDelta, columnDelta))
        return;

    m_columnDelta = columnDelta;
    emit columnDeltaChanged(m_columnDelta);
}

void setChangeClipPos(double changeClipPos)
{
    if (qFuzzyCompare(m_changeClipPos, changeClipPos))
        return;

    m_changeClipPos = changeClipPos;
    emit changeClipPosChanged(m_changeClipPos);
}

void setFirstTrayPos(double firstTrayPos)
{
    if (qFuzzyCompare(m_firstTrayPos, firstTrayPos))
        return;

    m_firstTrayPos = firstTrayPos;
    emit firstTrayPosChanged(m_firstTrayPos);
}

void setLastTrayPos(double lastTrayPos)
{
    if (qFuzzyCompare(m_lastTrayPos, lastTrayPos))
        return;

    m_lastTrayPos = lastTrayPos;
    emit lastTrayPosChanged(m_lastTrayPos);
}

void setCurrentIndex(int currentIndex)
{
    if (m_currentIndex == currentIndex)
        return;

    m_currentIndex = currentIndex;
    emit currentIndexChanged(m_currentIndex);
}

void setNeedChangeClip(bool needChangeClip)
{
    if (m_needChangeClip == needChangeClip)
        return;

    m_needChangeClip = needChangeClip;
    emit needChangeClipChanged(m_needChangeClip);
}

signals:
void columnCountChanged(int columnCount);
void fromTopChanged(bool fromTop);
void columnDeltaChanged(double columnDelta);
void changeClipPosChanged(double changeClipPos);
void firstTrayPosChanged(double firstTrayPos);
void lastTrayPosChanged(double lastTrayPos);
void currentIndexChanged(int currentIndex);
void needChangeClipChanged(bool needChangeClip);
};

#endif // TRAYCLIPSTANDARDSPARAMETER_H
