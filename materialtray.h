#ifndef MATERIALTRAY_H
#define MATERIALTRAY_H

#include "trayparameter.h"
#include "traystandardsparameter.h"

#include <qpoint.h>



class MaterialTray:public QObject
{
    Q_OBJECT
public:
    MaterialTray();
    void calculateDelta();
    QPointF getPosition(int column_index,int row_index,int tray_index = 0);
    bool findNextPositionOfInitState(int tray_index = 0);
    bool isTrayNeedChange(int tray_index);
    int getCurrentIndex(int tray_index = 0);
    QPointF getCurrentPosition(int tray_index = 0);
    int getCurrentMaterialState(int tray_index = 0);
    void setCurrentMaterialState(int state,int tray_index = 0);
    int getMaterialState(int column_index,int row_index,int tray_index = 0);
    void setMaterialState(int column_index,int row_index,int state,int tray_index = 0);
    void resetTrayState(int tray_index = 0);
public slots:
    void changeTrayCount(int trayCount);
private:
    QPointF getOffsetPositon(int column_index,int row_index);
    int getMaterialIndex(int column_index,int row_index);
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