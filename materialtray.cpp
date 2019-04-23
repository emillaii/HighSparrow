#include "materialtray.h"

MaterialTray::MaterialTray():QObject ()
{
    connect(&standards_parameters,&TrayStandardsParameter::trayCountChanged,this,&MaterialTray::changeTrayCount);
}

void MaterialTray::Init()
{
    loadJsonConfig();
}

void MaterialTray::calculateDelta()
{
    standards_parameters.setColumnDeta((first_tray_end_position.X() - parameters[0]->tray_start_position.X())/(standards_parameters.columnCount()-1));
    standards_parameters.setRowDelta((first_tray_end_position.Y() - parameters[0]->tray_start_position.Y())/(standards_parameters.rowCount()-1));
}

void MaterialTray::loadJsonConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LENS_PICKARM_PARAMS", &standards_parameters);
    temp_map.insert("END_POSITION", &first_tray_end_position);
    temp_map.insert("TRAY1_PARAMETER", parameters[0]);
    temp_map.insert("TRAY2_PARAMETER", parameters[1]);
    temp_map.insert("TRAY1_START_POSITION", &parameters[0]->tray_start_position);
    temp_map.insert("TRAY2_START_POSITION", &parameters[1]->tray_start_position);
    PropertyBase::loadJsonConfig("config//materilTray.json", temp_map);
}

void MaterialTray::saveJsonConfig()
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LENS_PICKARM_PARAMS", &standards_parameters);
    temp_map.insert("END_POSITION", &first_tray_end_position);
    temp_map.insert("TRAY1_PARAMETER", parameters[0]);
    temp_map.insert("TRAY2_PARAMETER", parameters[1]);
    temp_map.insert("TRAY1_START_POSITION", &parameters[0]->tray_start_position);
    temp_map.insert("TRAY2_START_POSITION", &parameters[1]->tray_start_position);
    PropertyBase::saveJsonConfig("config//materilTray.json", temp_map);
}

QPointF MaterialTray::getRelativePosition(int column_index, int row_index)
{
    return getOffsetPositon(getColumnIndex(column_index),getRowIndex(row_index));
}

QPointF MaterialTray::getPosition(int column, int row,int tray_index)
{
    return  getOffsetPositon(getColumnIndex(column),getRowIndex(row)) + parameters[getTrayIndex(tray_index)]->tray_start_position.ToPointF();
}

QPointF MaterialTray::getPositionByIndex(int index, int tray_index)
{
    int column,row;
    getColumnAndRowIndex(getMaterialIndex(index),column,row);
    return  getOffsetPositon(column,row) + parameters[getTrayIndex(tray_index)]->tray_start_position.ToPointF();
}

bool MaterialTray::findNextPositionOfInitState(int tray_index)
{
    int max_index = standards_parameters.columnCount()*standards_parameters.rowCount() - 1;
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    while (true) {
        if(current_tray->tray_material_state[current_tray->currentIndex()]==current_tray->initState())
            return true;
        if(current_tray->currentIndex()>=max_index)
        {
            current_tray->setNeedChange(true);
            return false;
        }
        current_tray->setCurrentIndex(current_tray->currentIndex() + 1);

    }
}

bool MaterialTray::isTrayNeedChange(int tray_index)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    return current_tray->needChange();
}

int MaterialTray::getCurrentIndex(int tray_index)
{
    return parameters[getTrayIndex(tray_index)]->currentIndex();
}

QPointF MaterialTray::getCurrentPosition(int tray_index)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    int column,row;
    getColumnAndRowIndex(current_tray->currentIndex(),column,row);
    return getOffsetPositon(column,row) + current_tray->tray_start_position.ToPointF();
}

int MaterialTray::getCurrentMaterialState(int tray_index)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    return current_tray->tray_material_state[current_tray->currentIndex()];
}

int MaterialTray::getMaterialState(int column_index, int row_index,int tray_index)
{
    return parameters[getTrayIndex(tray_index)]->tray_material_state[getMaterialIndex(getColumnIndex(column_index),getRowIndex(row_index))];
}

void MaterialTray::setMaterialState(int column_index, int row_index, int state,int tray_index)
{
    parameters[getTrayIndex(tray_index)]->tray_material_state[getMaterialIndex(getColumnIndex(column_index),getRowIndex(row_index))] = state;
}

void MaterialTray::setTrayCurrent(const int column_index, const int row_index, const int tray_index)
{
    parameters[getTrayIndex(tray_index)]->setCurrentIndex(getMaterialIndex(getColumnIndex(column_index),getRowIndex(row_index)));
}

void MaterialTray::getTrayCurrent(int &column_index, int &row_index, const int tray_index)
{
   int temp_index = parameters[getTrayIndex(tray_index)]->currentIndex();
   getColumnAndRowIndex(temp_index,column_index,row_index);
}

void MaterialTray::resetTrayState(int tray_index)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    for (int i = 0; i < current_tray->tray_material_state.count(); ++i)
    {
        current_tray->tray_material_state[i] = current_tray->initState();
    }
    current_tray->setCurrentIndex(0);
}

QPointF MaterialTray::getStartPosition(int tray_index)
{
    return parameters[getTrayIndex(tray_index)]->tray_start_position.ToPointF();
}

QPointF MaterialTray::getEndPosition()
{
    return first_tray_end_position.ToPointF();
}

void MaterialTray::changeTrayCount(int trayCount)
{
    while (trayCount != parameters.count())
    {
        if(trayCount > parameters.count())
        {
            TrayParameter* temp_tray = new TrayParameter();
            temp_tray->changeTrayCount(standards_parameters.columnCount()*standards_parameters.rowCount());
            connect(&standards_parameters,&TrayStandardsParameter::countChanged,temp_tray,&TrayParameter::changeTrayCount);
            parameters.append(temp_tray);
        }
        else
        {
            TrayParameter* temp_tray = parameters.last();
            parameters.removeLast();
            delete temp_tray;
        }
    }
}

QPointF MaterialTray::getOffsetPositon(int column_index, int row_index)
{
    return QPointF(column_index*standards_parameters.columnDeta(),row_index*standards_parameters.rowDelta());
}

int MaterialTray::getMaterialIndex(int column_index, int row_index)
{
    if(standards_parameters.useByRow())
        return row_index*standards_parameters.columnCount() + column_index;
    else
        return row_index + column_index*standards_parameters.rowCount();
}

int MaterialTray::getMaterialIndex(int index)
{
    int max_index = standards_parameters.columnCount()*standards_parameters.rowCount() - 1;
    if(index >= max_index)
        return  max_index;
    if(index < 1) return 0;
    return  index;
}

int MaterialTray::getColumnIndex(int column_index)
{
    if(column_index >= standards_parameters.columnCount())
        return  standards_parameters.columnCount() - 1;
    if(column_index < 1) return 0;
    return  column_index;
}

int MaterialTray::getRowIndex(int row_index)
{
    if(row_index >= standards_parameters.rowCount())
        return  standards_parameters.rowCount() - 1;
    if(row_index < 1) return 0;
    return  row_index;
}

void MaterialTray::getColumnAndRowIndex(const int material_index, int &column_index, int &row_index)
{
    if(standards_parameters.useByRow())
    {
        column_index = material_index%standards_parameters.columnCount();
        row_index = material_index/standards_parameters.columnCount();
    }
    else
    {
        row_index = material_index%standards_parameters.rowCount();
        column_index = material_index/standards_parameters.rowCount();
    }
}

int MaterialTray::getTrayIndex(int tray_index)
{
    if(parameters.count()>tray_index)
        return tray_index;
    else
        return parameters.count() - 1;
}
