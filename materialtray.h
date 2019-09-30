#ifndef MATERIALTRAY_H
#define MATERIALTRAY_H

#include "utils/errorcode.h"
#include "trayparameter.h"
#include "traystandardsparameter.h"

#include <qpoint.h>

#include <utils/pixel2mech.h>
enum MaterialState
{
    IsEmpty = 0,
    IsRawLens = 1,
    IsNgLens = 2,
    IsRawSensor = 3,
    IsNgSensor = 4,
    IsGoodProduct = 5,
    IsNgProduct = 6
};


class MaterialTray:public QObject,public ErrorBase
{
    Q_OBJECT
public:
    MaterialTray();
    Q_INVOKABLE void calculateDelta();
    void loadJsonConfig(QString file_name);
    void saveJsonConfig(QString file_name);
    static QString getMaterialStateName(int material_state);
    static int getMaterialStateFromName(QString name);
    QPointF getPosition(int column_index,int row_index,int tray_index = 0);
    QPointF getPositionByIndex(int index,int tray_index = 0);
    bool findNextPositionOfInitState(int tray_index = 0);
    bool findLastPositionOfState(int state, int tray_index);
    bool isTrayNeedChange(int tray_index);
    bool checkFinishPercent(int tray_index,double percent);
    int getCurrentIndex(int tray_index = 0);
    int getLastIndex();
    QPointF getCurrentPosition(int tray_index = 0);
    int getCurrentMaterialState(int tray_index = 0);
    void setCurrentMaterialState(int state,int tray_index = 0);
    int getMaterialState(int column_index,int row_index,int tray_index);
    int getMaterialState(int index,int tray_index);
    void setMaterialState(int column_index,int row_index,int state,int tray_index);
    void setMaterialState(int index,int state,int tray_index);
    QVariantMap getCurrentMaterialData(int tray_index = 0);
    void setCurrentMaterialData(int tray_index,QVariantMap material_data);
    QVariantMap getMaterialData(int column_index,int row_index,int tray_index);
    void setMaterialData(int column_index,int row_index,int tray_index,QVariantMap material_data);
    QVariantMap getMaterialData(int index,int tray_index);
    void setMaterialData(int index,int tray_index,QVariantMap material_data);
    PrOffset getTrayCurrentPrOffset(int tray_index);
    PrOffset getTrayPrOffset(int index,int tray_index);
    void setTrayCurrentPrOffset(PrOffset pr_offset,int tray_index);
    void setTrayPrOffset(PrOffset pr_offset,int index,int tray_index);
    Q_INVOKABLE void setTrayCurrent(const int column_index,const int row_index,const int tray_index);
    Q_INVOKABLE void setTrayFirst(const int column_index,const int row_index,const int tray_index);
    void setTrayCurrent(const int index,const int tray_index);
    void getTrayCurrent(int& column_index,int& row_index,const int tray_index);
    QString getTrayName(int tray_index);
    int getTrayNumber(int tray_index);
    void resetTrayState(int tray_index = 0);
    void resetAllTrayState();
    void copyTrayState(int origin_tray_index,int target_tray_index);
    QPointF getStartPosition(int tray_index);
    QPointF getEndPosition();
    void setTrayCurrentNg(const int tray_index);
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

public:
    TrayStandardsParameter standards_parameters;
    QList<TrayParameter*> parameters;
    Position first_tray_end_position;
};

#endif // MATERIALTRAY_H
