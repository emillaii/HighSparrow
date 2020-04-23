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

void AAData::addData(int index, double x, double y, double rawY){
    if (index == 0) {
        m_CCPointsList.append(QPointF(x, y));
        m_wCCList.push_back(x); m_wCCList.push_back(y);
        m_wCCRealList.push_back(x); m_wCCRealList.push_back(rawY);
    }
    if (index == 1) {
        m_ULPointsList.append(QPointF(x, y));
        m_wULList.push_back(x); m_wULList.push_back(y);
        m_wULRealList.push_back(x); m_wULRealList.push_back(rawY);
    }
    if (index == 2) {
        m_URPointsList.append(QPointF(x, y));
        m_wURList.push_back(x); m_wURList.push_back(y);
        m_wURRealList.push_back(x); m_wURRealList.push_back(rawY);
    }
    if (index == 3) {
        m_LLPointsList.append(QPointF(x, y));
        m_wLLList.push_back(x); m_wLLList.push_back(y);
        m_wLLRealList.push_back(x); m_wLLRealList.push_back(rawY);
    }
    if (index == 4) {
        m_LRPointsList.append(QPointF(x, y));
        m_wLRList.push_back(x); m_wLRList.push_back(y);
        m_wLRRealList.push_back(x); m_wLRRealList.push_back(rawY);
    }
}

void AAData::clear()
{
    m_PointsList.clear();
    m_CCPointsList.clear();
    m_ULPointsList.clear();
    m_URPointsList.clear();
    m_LLPointsList.clear();
    m_LRPointsList.clear();
    m_wCCList.clear();
    m_wULList.clear();
    m_wURList.clear();
    m_wLLList.clear();
    m_wLRList.clear();
    m_wCCRealList.clear();
    m_wULRealList.clear();
    m_wURRealList.clear();
    m_wLLRealList.clear();
    m_wLRRealList.clear();
    emit wValueClear();
}

void AAData::plot(QString chartName)
{
    this->setChartName(chartName);
    emit wValueChanged();
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

void AAData::plotIntensityProfile(float minI, float maxI, std::vector<float> values, int detectedNumberOfError, double negative_di, double positive_di)
{
    if (this->isRunning()) {
        qInfo("stop the thread");
        this->terminate();
    }
    m_minValue = (int)minI; m_maxValue = (int)maxI;
    m_positiveDI = positive_di; m_negativeDI = negative_di;
    m_detectedIntensityError = detectedNumberOfError;
    for (unsigned i = 0; i < values.size(); i++){
        this->m_PointsList.push_back(QPoint(i, values[i]));
    }
    this->setValuesSize(values.size());
    plotProfile = 1;
    this->start();
}

void AAData::run(){
    if (plotProfile == 0) emit wValueChanged();  //AA Data plot
    else if (plotProfile == 1) {
        for (unsigned i = 0; i < m_PointsList.size(); i=i+10){
            this->m_wValue.setX(m_PointsList[i].x());
            this->m_wValue.setY(m_PointsList[i].y());
            QThread::msleep(5);
            emit wValueChanged();
        }
    }
}
