#ifndef UPHHELPER_H
#define UPHHELPER_H

#include "propertybase.h"

class UPHHelper:public PropertyBase
{
    Q_OBJECT
public:
    UPHHelper();

    void addCurrentNumber(double accumulated_hour);
    void addCurrentReslutNumber(double accumulated_hour);
    void updateYeild(int decimal_digit = 3);
    void updateUPH();
    void clearNumber();
    Q_PROPERTY(int currentNumber READ currentNumber WRITE setCurrentNumber NOTIFY currentNumberChanged)
    Q_PROPERTY(bool addOK READ addOK WRITE setAddOK NOTIFY addOKChanged)
    Q_PROPERTY(int currentResultNumber READ currentResultNumber WRITE setCurrentResultNumber NOTIFY currentResultNumberChanged)
    Q_PROPERTY(double accumulatedYeild READ accumulatedYeild WRITE setAccumulatedYeild NOTIFY accumulatedYeildChanged)
    Q_PROPERTY(int accumulatedUPH READ accumulatedUPH WRITE setAccumulatedUPH NOTIFY accumulatedUPHChanged)
    Q_PROPERTY(int accumulatedOkUPH READ accumulatedOkUPH WRITE setAccumulatedOkUPH NOTIFY accumulatedOkUPHChanged)

    int currentNumber() const
    {
        return m_currentNumber;
    }

    int currentResultNumber() const
    {
        return m_currentNgNumber;
    }

    double accumulatedYeild() const
    {
        return m_accumulatedYeild;
    }

    int accumulatedUPH() const
    {
        return m_accumulatedUPH;
    }

    int accumulatedOkUPH() const
    {
        return m_accumulatedOkUPH;
    }

    bool addOK() const
    {
        return m_addOK;
    }

public slots:
    void setCurrentNumber(int currentNumber)
    {
        if (m_currentNumber == currentNumber)
            return;

        m_currentNumber = currentNumber;
        emit currentNumberChanged(m_currentNumber);
    }

    void setCurrentResultNumber(int currentNgNumber)
    {
        if (m_currentNgNumber == currentNgNumber)
            return;

        m_currentNgNumber = currentNgNumber;
        emit currentResultNumberChanged(m_currentNgNumber);
    }

    void setAccumulatedYeild(double accumulatedYeild)
    {
        if (qFuzzyCompare(m_accumulatedYeild, accumulatedYeild))
            return;

        m_accumulatedYeild = accumulatedYeild;
        emit accumulatedYeildChanged(m_accumulatedYeild);
    }

    void setAccumulatedUPH(int accumulatedUPH)
    {
        if (m_accumulatedUPH == accumulatedUPH)
            return;

        m_accumulatedUPH = accumulatedUPH;
        emit accumulatedUPHChanged(m_accumulatedUPH);
    }

    void setAccumulatedOkUPH(int accumulatedOkUPH)
    {
        if (m_accumulatedOkUPH == accumulatedOkUPH)
            return;

        m_accumulatedOkUPH = accumulatedOkUPH;
        emit accumulatedOkUPHChanged(m_accumulatedOkUPH);
    }

    void setAddOK(bool addOK)
    {
        if (m_addOK == addOK)
            return;

        m_addOK = addOK;
        emit addOKChanged(m_addOK);
    }

signals:
    void currentNumberChanged(int currentNumber);

    void currentResultNumberChanged(int currentResultNumber);

    void accumulatedYeildChanged(double accumulatedYeild);

    void accumulatedUPHChanged(int accumulatedUPH);

    void accumulatedOkUPHChanged(int accumulatedOkUPH);

    void addOKChanged(bool addOK);

private:
    int m_currentNumber = 0;
    int m_currentNgNumber = 0;
    double m_accumulatedYeild = 0;
    int m_accumulatedUPH = 0;
    int m_accumulatedOkUPH = 0;
    double accumulated_hour = 0;
    bool m_addOK = false;
};

#endif // UPHHELPER_H
