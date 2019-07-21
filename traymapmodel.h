#ifndef TRAYMAPMODEL_H
#define TRAYMAPMODEL_H

#include <QAbstractTableModel>

class TrayMapModel : public QAbstractTableModel
{
    Q_OBJECT

    TrayMapModel(QObject *parent = nullptr); // private constructor

public:
    enum UnitStatus
    {
        StatusGood,
        StatusNG,
        StatusIdle,
    };
    Q_ENUM(UnitStatus)

    enum TrayMapID
    {
        LensTray = 0,
        SensorTray,
        ProductTray,
        RejectTray,
    };

    static TrayMapModel* instance(TrayMapID trayId);

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void resetModel(uint row, uint column, UnitStatus status);
    void setUnitStatus(uint idx, UnitStatus status);
    UnitStatus unitStatus(uint idx) const;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::tuple<uint, uint> cellCoordinatesFromIndex(uint cellIndex) const;
    uint cellIndex(const std::tuple<uint, uint> &coordinates) const;

private:
    static constexpr int ROW = 0;
    static constexpr int COL = 1;

    uint m_column;
    uint m_row;
    std::vector<UnitStatus> m_currentState;
};

#endif // TRAYMAPMODEL_H
