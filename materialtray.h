#ifndef MATERIALTRAY_H
#define MATERIALTRAY_H

#include "Utils/errorcode.h"
#include "trayparameter.h"
#include "traystandardsparameter.h"

#include "Vision/pixel2mech.h"

#include <qpoint.h>
enum MaterialState
{
    IsEmpty = 0,
    IsRaw = 1,
    IsNg = 2,
    IsProduct = 3,
    IsInUse = 4
};

enum TrayType {
    SENSOR_TRAY,
    LENS_TRAY,
    PRODUCT_TRAY,
    REJECT_TRAY
};

class MaterialTray:public QObject,public ErrorBase
{
    Q_OBJECT
public:
    MaterialTray();
    void loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);

    QPointF getPosition(int column_index,int row_index,int tray_index = 0);
    QPointF getPositionByIndex(int index,int tray_index = 0);
    QPointF getCurrentPosition(int tray_index = 0);
    QPointF getStartPosition(int tray_index);
    QPointF getEndPosition();

    bool findNextPositionOfInitState(int tray_index = 0);
    bool findLastPositionOfState(int state,int tray_index);
    bool isTrayNeedChange(int tray_index);
    int getCurrentIndex(int tray_index = 0);
    int getLastIndex();

    int getCurrentMaterialState(int tray_index = 0);
    void setCurrentMaterialState(int state,int tray_index = 0);
    int getMaterialState(int column_index,int row_index,int tray_index);
    void setMaterialState(int column_index,int row_index, int state, int tray_index);
    int getMaterialState(int index,int tray_index);

    QVariantMap *getCurrentMaterialData(int tray_index = 0);
    QVariantMap *getMaterialData(int column_index,int row_index,int tray_index);
    QVariantMap *getMaterialData(int index,int tray_index);

    void setTrayType(TrayType type);

    Q_INVOKABLE void setTrayCurrent(const int column_index,const int row_index,const int tray_index);
    Q_INVOKABLE void setTrayFirst(const int column_index,const int row_index,const int tray_index);
    void setTrayCurrent(const int index,const int tray_index);
    void getTrayCurrent(int& column_index,int& row_index,const int tray_index);

    QString getTrayName(int tray_index);
    int getTrayNumber(int tray_index);

    PrOffset getTrayCurrentPrOffset(int tray_index);
    PrOffset getTrayPrOffset(int index,int tray_index);
    void setTrayCurrentPrOffset(PrOffset pr_offset,int tray_index);
    void setTrayPrOffset(PrOffset pr_offset,int index,int tray_index);

    void resetTrayState(int tray_index = 0);
    void resetAllTrayState();
    void copyTrayState(int origin_tray_index,int target_tray_index);

    Q_INVOKABLE void calculateDelta();

public slots:
    void changeTrayCount(int trayCount);
private:
    QPointF getOffsetPositon(int column_index,int row_index);

    int getMaterialIndex(int column_index,int row_index);
    int getMaterialIndex(int index);
    int getColumnIndex(int column_index);
    int getRowIndex(int row_index);
    void getColumnAndRowIndex(const int index,int& column_index,int& row_index);
    int getTrayIndex(int tray_index);
    QPointF getRelativePosition(int column_index,int row_index);
    TrayType trayType;
public:
    TrayStandardsParameter standards_parameters;
    QList<TrayParameter*> parameters;
    Position first_tray_end_position;
};

#endif // MATERIALTRAY_H
