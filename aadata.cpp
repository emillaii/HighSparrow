#include "aadata.h"
#include <QThread>
void AAData::wTimeout(){
    for (int i = 0; i < m_CCPointsList.size(); i++) {
        m_wCCValue.setX(m_CCPointsList.at(i).x());
        m_wCCValue.setY(m_CCPointsList.at(i).y());
        m_wULValue.setX(m_ULPointsList.at(i).x());
        m_wULValue.setY(m_ULPointsList.at(i).y());
        m_wURValue.setX(m_URPointsList.at(i).x());
        m_wURValue.setY(m_URPointsList.at(i).y());
        m_wLLValue.setX(m_LLPointsList.at(i).x());
        m_wLLValue.setY(m_LLPointsList.at(i).y());
        m_wLRValue.setX(m_LRPointsList.at(i).x());
        m_wLRValue.setY(m_LRPointsList.at(i).y());
        emit wValueChanged();
        QThread::msleep(100);
    }
}

AAData::AAData(QObject *parent) : QThread(parent){
//    m_wTimer = new QTimer(this);
//    m_wTimer->setInterval(1);
//    connect(m_wTimer, &QTimer::timeout, this, &AAData::wTimeout);
//    m_wTimer->start();
}

void AAData::addData(int index, double x, double y){
    if (index == 0) m_CCPointsList.append(QPointF(x, y));
    if (index == 1) m_ULPointsList.append(QPointF(x, y));
    if (index == 2) m_URPointsList.append(QPointF(x, y));
    if (index == 3) m_LLPointsList.append(QPointF(x, y));
    if (index == 4) m_LRPointsList.append(QPointF(x, y));
}

void AAData::clear()
{
    m_CCPointsList.clear();
    m_ULPointsList.clear();
    m_URPointsList.clear();
    m_LLPointsList.clear();
    m_LRPointsList.clear();
    count = 0;
    emit wValueClear();
}

void AAData::plot()
{
    this->start();
}

void AAData::incrementData(double y1, double y2, double y3, double y4, double y5)
{
    m_wCCValue.setX(count); m_wCCValue.setY(y1);
    m_wULValue.setX(count); m_wULValue.setY(y2);
    m_wURValue.setX(count); m_wURValue.setY(y3);
    m_wLLValue.setX(count); m_wLLValue.setY(y4);
    m_wLRValue.setX(count); m_wLRValue.setY(y5);
    count++;
    emit wValueChanged();
}

void AAData::run(){
    qInfo("CC Size : %d UL : %d UR : %d LL: %d LR: %d",
          m_CCPointsList.size(),
          m_ULPointsList.size(),
          m_URPointsList.size(),
          m_LLPointsList.size(),
          m_LRPointsList.size());
    for (int i = 0; i < m_CCPointsList.size(); i++) {
        m_wCCValue.setX(m_CCPointsList.at(i).x());
        m_wCCValue.setY(m_CCPointsList.at(i).y());
        if (m_ULPointsList.size() > i) {
            m_wULValue.setX(m_ULPointsList.at(i).x());
            m_wULValue.setY(m_ULPointsList.at(i).y());
        }
        if (m_URPointsList.size() > i) {
            m_wURValue.setX(m_URPointsList.at(i).x());
            m_wURValue.setY(m_URPointsList.at(i).y());
        }
        if (m_LLPointsList.size() > i) {
            m_wLLValue.setX(m_LLPointsList.at(i).x());
            m_wLLValue.setY(m_LLPointsList.at(i).y());
        }
        if (m_LRPointsList.size() > i) {
            m_wLRValue.setX(m_LRPointsList.at(i).x());
            m_wLRValue.setY(m_LRPointsList.at(i).y());
        }
        emit wValueChanged();
        QThread::msleep(100);
    }

    if (m_ULPointsList.size() > m_CCPointsList.size()) {
        for (int i = m_CCPointsList.size(); i < m_ULPointsList.size(); i++) {
            m_wULValue.setX(m_ULPointsList.at(i).x());
            m_wULValue.setY(m_ULPointsList.at(i).y());
            emit wValueChanged();
        }
    }
    if (m_URPointsList.size() > m_CCPointsList.size()) {
        for (int i = m_CCPointsList.size(); i < m_URPointsList.size(); i++) {
            m_wURValue.setX(m_URPointsList.at(i).x());
            m_wURValue.setY(m_URPointsList.at(i).y());
            emit wValueChanged();
        }
    }
    if (m_LLPointsList.size() > m_CCPointsList.size()) {
        for (int i = m_CCPointsList.size(); i < m_LLPointsList.size(); i++) {
            m_wLLValue.setX(m_LLPointsList.at(i).x());
            m_wLLValue.setY(m_LLPointsList.at(i).y());
            emit wValueChanged();
        }
    }
    if (m_LRPointsList.size() > m_CCPointsList.size()) {
        for (int i = m_CCPointsList.size(); i < m_LRPointsList.size(); i++) {
            m_wLRValue.setX(m_LRPointsList.at(i).x());
            m_wLRValue.setY(m_LRPointsList.at(i).y());
            emit wValueChanged();
        }
    }
}
