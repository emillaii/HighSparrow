#include "traymapmodel.h"


TrayMapModel::TrayMapModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_column(0)
    , m_row(0)
    , m_currentState()
{
}

TrayMapModel* TrayMapModel::instance(TrayMapID trayId)
{
    switch(trayId)
    {
    case LensTray:
    {
        static TrayMapModel* lensTray = new TrayMapModel;
        return lensTray;
    }

    case SensorTray:
    {
        static TrayMapModel* sensorTray = new TrayMapModel;
        return sensorTray;
    }

    case ProductTray:
    {
        static TrayMapModel* productTray = new TrayMapModel;
        return productTray;
    }

    case RejectTray:
    {
        static TrayMapModel* rejectTray = new TrayMapModel;
        return rejectTray;
    }
    }

    return nullptr;
}

int TrayMapModel::rowCount(const QModelIndex & parent) const
{
    if (parent.isValid())
        return 0;

    return int(m_row);
}

int TrayMapModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return int(m_column);
}

QVariant TrayMapModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    uint idx = cellIndex({index.column(), index.row()});
    if(idx < m_column*m_row)
    {
        return QVariant(m_currentState[idx]);
    }
    else
    {
        return QVariant();
    }
}

void TrayMapModel::resetModel(uint row, uint column, UnitStatus status)
{
    beginResetModel();

    m_row = row;
    m_column = column;
    m_currentState.resize(row*column);

    for(auto& unitStatus : m_currentState)
    {
        unitStatus = status;
    }

    endResetModel();
}

void TrayMapModel::setUnitStatus(uint idx, UnitStatus status)
{
    if(idx < m_column*m_row)
    {
        m_currentState[idx] = status;

        std::tuple<int, int> coord = cellCoordinatesFromIndex(idx);
        emit dataChanged(index(std::get<ROW>(coord), std::get<COL>(coord)),
                         index(std::get<ROW>(coord), std::get<COL>(coord)), {Qt::DisplayRole});
    }
}

TrayMapModel::UnitStatus TrayMapModel::unitStatus(uint idx) const
{
    if(idx < m_column*m_row)
    {
        return m_currentState[idx];
    }
    else
    {
        QString error(tr("Index out of range."));
        throw std::exception(error.toStdString().c_str());
    }
}

QHash<int, QByteArray> TrayMapModel::roleNames() const
{
    return { {Qt::DisplayRole, "Status"} };
}

std::tuple<uint, uint> TrayMapModel::cellCoordinatesFromIndex(uint cellIndex) const
{
    return {cellIndex % m_column, cellIndex / m_column};
}

uint TrayMapModel::cellIndex(const std::tuple<uint, uint> &coordinates) const
{
    return std::get<COL>(coordinates)*m_column + std::get<ROW>(coordinates);
}
