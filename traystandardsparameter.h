#ifndef MATERIALSTANDARDSPARAMETER_H
#define MATERIALSTANDARDSPARAMETER_H

#include "propertybase.h"



class TrayStandardsParameter:public PropertyBase
{
    Q_OBJECT
public:
    TrayStandardsParameter():PropertyBase (){}
    Q_PROPERTY(int columnCount READ columnCount WRITE setColumnCount NOTIFY columnCountChanged)
    Q_PROPERTY(int rowCount READ rowCount WRITE setRowCount NOTIFY rowCountChanged)
    Q_PROPERTY(bool useByRow READ useByRow WRITE setUseByRow NOTIFY useByRowChanged)
    Q_PROPERTY(double columnDeta READ columnDeta WRITE setColumnDeta NOTIFY columnDetaChanged)
    Q_PROPERTY(double rowDelta READ rowDelta WRITE setRowDelta NOTIFY rowDeltaChanged)
    Q_PROPERTY(int trayCount READ trayCount WRITE setTrayCount NOTIFY trayCountChanged)
    int columnCount() const
    {
        return m_columnCount;
    }

    int rowCount() const
    {
        return m_rowCount;
    }

    bool useByRow() const
    {
        return m_useByRow;
    }

    double columnDeta() const
    {
        return m_columnDeta;
    }

    double rowDelta() const
    {
        return m_rowDelta;
    }

    int trayCount() const
    {
        return m_trayCount;
    }

public slots:
    void setColumnCount(int columnCount)
    {
        if (m_columnCount == columnCount)
            return;

        m_columnCount = columnCount;
        emit countChanged(m_columnCount*m_rowCount);
        emit columnCountChanged(m_columnCount);
    }

    void setRowCount(int rowCount)
    {
        if (m_rowCount == rowCount)
            return;

        m_rowCount = rowCount;
        emit countChanged(m_columnCount*m_rowCount);
        emit rowCountChanged(m_rowCount);
    }

    void setUseByRow(bool useByRow)
    {
        if (m_useByRow == useByRow)
            return;

        m_useByRow = useByRow;
        emit useByRowChanged(m_useByRow);
    }

    void setColumnDeta(double columnDeta)
    {
        if (qFuzzyCompare(m_columnDeta, columnDeta))
            return;

        m_columnDeta = columnDeta;
        emit columnDetaChanged(m_columnDeta);
    }

    void setRowDelta(double rowDelta)
    {
        if (qFuzzyCompare(m_rowDelta, rowDelta))
            return;

        m_rowDelta = rowDelta;
        emit rowDeltaChanged(m_rowDelta);
    }

    void setTrayCount(int trayCount)
    {
        if (m_trayCount == trayCount)
            return;

        m_trayCount = trayCount;
        emit trayCountChanged(m_trayCount);
    }

signals:
    void countChanged(int count);

    void columnCountChanged(int columnCount);

    void rowCountChanged(int rowCount);

    void useByRowChanged(bool useByRow);

    void columnDetaChanged(double columnDeta);

    void rowDeltaChanged(double rowDelta);

    void trayCountChanged(int trayCount);

private:
    int m_columnCount = 1;
    int m_rowCount = 1;
    bool m_useByRow = false;
    double m_columnDeta = 0;
    double m_rowDelta = 0;
    int m_trayCount = 1;
};

#endif // MATERIALSTANDARDSPARAMETER_H
