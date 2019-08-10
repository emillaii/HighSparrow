#ifndef MATERIALTRAY_H
#define MATERIALTRAY_H

#include "Utils/errorcode.h"
#include "trayparameter.h"
#include "traystandardsparameter.h"

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

    QPointF getPositionByIndex(int index,int tray_index = 0);
    QPointF getCurrentPosition(int tray_index = 0);
    QPointF getStartPosition(int tray_index);
    QPointF getEndPosition();

    bool findNextPositionOfInitState(int tray_index = 0);
    bool findLastPositionOfState(int state,int tray_index);
    bool isTrayNeedChange(int tray_index);
    int getCurrentIndex(int tray_index = 0);
    void setCurrentMaterialState(int state,int tray_index = 0);
    void resetTrayState(int tray_index = 0);
    int getMaterialState(int column_index,int row_index,int tray_index);
    int getMaterialState(int index,int tray_index);
    void setTrayType(TrayType type);
    TrayType getTrayType();
    Q_INVOKABLE void setTrayCurrent(const int column_index,const int row_index,const int tray_index = 0);
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
    TrayType trayType;
public:
    TrayStandardsParameter standards_parameters;
    QList<TrayParameter*> parameters;
    Position first_tray_end_position;
};

#endif // MATERIALTRAY_H
