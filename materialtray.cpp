#include "materialtray.h"

MaterialTray::MaterialTray():QObject (),ErrorBase ("MateryTray")
{
    connect(&standards_parameters,&TrayStandardsParameter::trayCountChanged,this,&MaterialTray::changeTrayCount);
}

void MaterialTray::calculateDelta()
{
    int digit =100;
    standards_parameters.setColumnDeta(round(digit*(first_tray_end_position.X() - parameters[0]->tray_start_position.X())/(standards_parameters.columnCount()-1))/digit);
    standards_parameters.setRowDelta(round(digit*(first_tray_end_position.Y() - parameters[0]->tray_start_position.Y())/(standards_parameters.rowCount()-1))/digit);
}

void MaterialTray::loadJsonConfig(QString file_name)
{
    qInfo("parameters.size() %d",parameters.size());
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LENS_PICKARM_PARAMS", &standards_parameters);
    temp_map.insert("END_POSITION", &first_tray_end_position);
    PropertyBase::loadJsonConfig(file_name, temp_map);
    temp_map.clear();
    for (int i = 0; i < standards_parameters.trayCount(); ++i)
    {
        temp_map.insert(QString("TRAY").append(QString::number(i + 1)).append("_PARAMETER"), parameters[i]);
        temp_map.insert(QString("TRAY").append(QString::number(i + 1)).append("_START_POSITION"), &parameters[i]->tray_start_position);
    }
    PropertyBase::loadJsonConfig(file_name, temp_map);
    qInfo("parameters.size() %d",parameters.size());
}

void MaterialTray::saveJsonConfig(QString file_name)
{
    QMap<QString,PropertyBase*> temp_map;
    temp_map.insert("LENS_PICKARM_PARAMS", &standards_parameters);
    temp_map.insert("END_POSITION", &first_tray_end_position);
    for (int i = 0; i < standards_parameters.trayCount(); ++i)
    {
        temp_map.insert(QString("TRAY").append(QString::number(i + 1)).append("_PARAMETER"), parameters[i]);
        temp_map.insert(QString("TRAY").append(QString::number(i + 1)).append("_START_POSITION"), &parameters[i]->tray_start_position);
    }
    PropertyBase::saveJsonConfig(file_name, temp_map);
}

QString MaterialTray::getMaterialStateName(int material_state)
{
    switch (material_state) {
    case MaterialState::IsEmpty:
        return "Empty";
    case MaterialState::IsRawLens:
        return "RawLens";
    case MaterialState::IsNgLens:
        return "NgLens";
    case MaterialState::IsRawSensor:
        return "RawSensor";
    case MaterialState::IsNgSensor:
        return "NgSensor";
    case MaterialState::IsGoodProduct:
        return "GoodProduct";
    case MaterialState::IsNgProduct:
        return "NgProduct";
    default:
        return "";
    }
}

int MaterialTray::getMaterialStateFromName(QString name)
{
    if(name == "Empty")
        return MaterialState::IsEmpty;
    else if(name =="RawLens")
        return MaterialState::IsRawLens;
    else if(name == "NgLens")
        return MaterialState::IsNgLens;
    else if(name == "RawSensor")
        return MaterialState::IsRawSensor;
    else if(name == "NgSensor")
        return MaterialState::IsNgSensor;
    else if(name == "GoodProduct")
        return MaterialState::IsGoodProduct;
    else if(name == "NgProduct")
        return MaterialState::IsNgProduct;
    else {
        return -1;
    }
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
//todo move to fail的处理，不能一直加
bool MaterialTray::findNextPositionOfInitState(int tray_index)
{
    int max_index = standards_parameters.columnCount()*standards_parameters.rowCount() - 1;
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    int curren_index = current_tray->currentIndex();
    while (true)
    {
        if(current_tray->trayMaterialState(curren_index) == current_tray->initState())
        {
            if(curren_index == max_index)
                current_tray->setNeedChange(true);
            current_tray->setCurrentIndex(curren_index);
//            qInfo("findNextPositionOfInitState find index %d tray %d",curren_index,tray_index);
            return true;
        }
        if(curren_index >= max_index)
        {
            current_tray->setNeedChange(true);
            return false;
        }
        curren_index++;
    }
}

bool MaterialTray::findLastPositionOfState(int state, int tray_index)
{
    int max_index = standards_parameters.columnCount()*standards_parameters.rowCount() - 1;
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    // Check if the availableIndex in current tray is smaller than MaxIndex
    if (max_index>current_tray->availableIndex())
    {
        max_index = current_tray->availableIndex();
    }
    int curren_index = 0;
    while (true)
    {
        int temp_state = current_tray->trayMaterialState(curren_index);
        if(temp_state == state)
        {
            if(curren_index == max_index && state == current_tray->initState())
                current_tray->setNeedChange(true);
            current_tray->setCurrentIndex(curren_index);
//            qInfo("findLastPositionOfState find index %d tray %d",curren_index,tray_index);
            return true;
        }
        if(curren_index >= max_index||temp_state == current_tray->initState())
        {
            if(state == current_tray->initState())
                current_tray->setNeedChange(true);
            return false;
        }
        curren_index++;
    }
}

bool MaterialTray::isTrayNeedChange(int tray_index)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    return current_tray->needChange();
}

bool MaterialTray::checkFinishPercent(int tray_index, double percent)
{
    double current = getCurrentIndex(tray_index);
    double max_count = standards_parameters.rowCount()*standards_parameters.columnCount();
    double current_percent = current/max_count;
    if(current_percent>percent)
        return true;
    return false;

}

int MaterialTray::getCurrentIndex(int tray_index)
{
    return parameters[getTrayIndex(tray_index)]->currentIndex();
}

int MaterialTray::getLastIndex()
{
    return standards_parameters.rowCount()*standards_parameters.columnCount() - 1;
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
    return current_tray->trayMaterialState(current_tray->currentIndex());
}

void MaterialTray::setCurrentMaterialState(int state, int tray_index)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    current_tray->setTrayMaterialState(current_tray->currentIndex(),state);
}

int MaterialTray::getMaterialState(int column_index, int row_index,int tray_index)
{
    return parameters[getTrayIndex(tray_index)]->trayMaterialState(getMaterialIndex(getColumnIndex(column_index),getRowIndex(row_index)));
}

int MaterialTray::getMaterialState(int index, int tray_index)
{
    return parameters[getTrayIndex(tray_index)]->trayMaterialState(getMaterialIndex(index));
}

QVariantMap MaterialTray::getCurrentMaterialData(int tray_index)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    return current_tray->trayMaterialData(current_tray->currentIndex());
}

void MaterialTray::setCurrentMaterialData(int tray_index, QVariantMap material_data)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    current_tray->setTrayMaterialData(current_tray->currentIndex(),material_data);
}

QVariantMap MaterialTray::getMaterialData(int column_index, int row_index, int tray_index)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    return current_tray->trayMaterialData(getMaterialIndex(getColumnIndex(column_index),getRowIndex(row_index)));
}

void MaterialTray::setMaterialData(int column_index, int row_index, int tray_index, QVariantMap material_data)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    current_tray->setTrayMaterialData(getMaterialIndex(getColumnIndex(column_index),getRowIndex(row_index)),material_data);
}

QVariantMap MaterialTray::getMaterialData(int index, int tray_index)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    return current_tray->trayMaterialData(getMaterialIndex(index));
}

void MaterialTray::setMaterialData(int index, int tray_index, QVariantMap material_data)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    current_tray->setTrayMaterialData(getMaterialIndex(index),material_data);
}

PrOffset MaterialTray::getTrayCurrentPrOffset(int tray_index)
{
    QVariantMap temp_data = getCurrentMaterialData(tray_index);
    PrOffset temp_offset;
    temp_offset.X = temp_data["pr_offset_x"].toDouble();
    temp_offset.Y = temp_data["pr_offset_y"].toDouble();
    temp_offset.Theta = temp_data["pr_offset_t"].toDouble();
    qInfo("getTrayCurrentPrOffset tray_index:%d,current_index %d,pr_offset_x:%f,pr_offset_y:%f",tray_index,getCurrentIndex(tray_index),temp_offset.X,temp_offset.Y);
    return temp_offset;

}

PrOffset MaterialTray::getTrayPrOffset(int index, int tray_index)
{
    QVariantMap temp_data = getMaterialData(index,tray_index);
    PrOffset temp_offset;
    temp_offset.X = temp_data["pr_offset_x"].toDouble();
    temp_offset.Y = temp_data["pr_offset_y"].toDouble();
    temp_offset.Theta = temp_data["pr_offset_t"].toDouble();
    return temp_offset;
}

void MaterialTray::setTrayCurrentPrOffset(PrOffset pr_offset, int tray_index)
{
    QVariantMap temp_data = getCurrentMaterialData(tray_index);
    temp_data["pr_offset_x"] = pr_offset.X;
    temp_data["pr_offset_y"] = pr_offset.Y;
    temp_data["pr_offset_t"] = pr_offset.Theta;
    //temp_data["pr_offset_t"] = 0;
    setCurrentMaterialData(tray_index,temp_data);
    qInfo("setTrayCurrentPrOffset tray_index:%d,current_index %d,pr_offset_x:%f,pr_offset_y:%f，pr_offset_t:%f",tray_index,getCurrentIndex(tray_index),pr_offset.X,pr_offset.Y,pr_offset.Theta);
}

void MaterialTray::setTrayPrOffset(PrOffset pr_offset, int index, int tray_index)
{
    QVariantMap temp_data = getMaterialData(index,tray_index);
    temp_data["pr_offset_x"] = pr_offset.X;
    temp_data["pr_offset_y"] = pr_offset.Y;
    temp_data["pr_offset_t"] = pr_offset.Theta;
    setCurrentMaterialData(tray_index,temp_data);
    setCurrentMaterialData(tray_index,temp_data);
    qInfo("setTrayPrOffset tray_index:%d,index:%d,current_index %d,pr_offset_x:%f,pr_offset_y%f",tray_index,index,getCurrentIndex(tray_index),pr_offset.X,pr_offset.Y);
}

void MaterialTray::setMaterialState(int column_index, int row_index, int state,int tray_index)
{
    parameters[getTrayIndex(tray_index)]->setTrayMaterialState(getMaterialIndex(getColumnIndex(column_index),getRowIndex(row_index)),state);
}

void MaterialTray::setMaterialState(int index, int state, int tray_index)
{
    parameters[getTrayIndex(tray_index)]->setTrayMaterialState(getMaterialIndex(getMaterialIndex(index)),state);
}

void MaterialTray::setTrayCurrent(const int column_index, const int row_index, const int tray_index)
{
    parameters[getTrayIndex(tray_index)]->setCurrentIndex(getMaterialIndex(getColumnIndex(column_index),getRowIndex(row_index)));
}

void MaterialTray::setTrayFirst(const int column_index, const int row_index, const int tray_index)
{
    for (int i = getMaterialIndex(getColumnIndex(column_index),getRowIndex(row_index)) -1; i >= 0; --i)
    {
        if(getMaterialState(i,tray_index) == MaterialState::IsRawLens)
        {
            setTrayCurrent(i,tray_index);
            setCurrentMaterialState(MaterialState::IsNgLens,tray_index);
        }
        else if(getMaterialState(i,tray_index) == MaterialState::IsRawSensor)
        {
            setTrayCurrent(i,tray_index);
            setCurrentMaterialState(MaterialState::IsRawSensor,tray_index);
        }
    }
}

void MaterialTray::setTrayCurrent(const int index, const int tray_index)
{
    parameters[getTrayIndex(tray_index)]->setCurrentIndex(getMaterialIndex(index));
}

void MaterialTray::getTrayCurrent(int &column_index, int &row_index, const int tray_index)
{
   int temp_index = parameters[getTrayIndex(tray_index)]->currentIndex();
   getColumnAndRowIndex(temp_index,column_index,row_index);
}

QString MaterialTray::getTrayName(int tray_index)
{
    return parameters[getTrayIndex(tray_index)]->trayName();
}

int MaterialTray::getTrayNumber(int tray_index)
{
    return parameters[getTrayIndex(tray_index)]->trayNumber();
}

void MaterialTray::resetTrayState(int tray_index)
{
    TrayParameter* current_tray = parameters[getTrayIndex(tray_index)];
    current_tray->setTrayNumber(current_tray->trayNumber() + 1);
    for (int i = 0; i < current_tray->trayMaterialState().count(); ++i)
    {
        current_tray->setTrayMaterialState(i,current_tray->initState());
        QVariantMap temp_data;
        temp_data.insert("TrayNumber",QString::number(current_tray->trayNumber()));
        current_tray->setTrayMaterialData(i,temp_data);
    }
    current_tray->setCurrentIndex(0);
    current_tray->setNeedChange(false);
}

void MaterialTray::resetAllTrayState()
{
    for (int i = 0; i < parameters.size(); ++i)
        resetTrayState(i);
}

void MaterialTray::copyTrayState(int origin_tray_index, int target_tray_index)
{
    TrayParameter* origin_tray = parameters[getTrayIndex(origin_tray_index)];
    TrayParameter* target_tray = parameters[getTrayIndex(target_tray_index)];
    target_tray->setTrayNumber(origin_tray->trayNumber());
    target_tray->setCurrentIndex(origin_tray->currentIndex());
    target_tray->setNeedChange(origin_tray->needChange());
    for (int i = 0; i < origin_tray->trayMaterialState().count(); ++i)
    {
        target_tray->setTrayMaterialState(i,origin_tray->trayMaterialState(i));
        target_tray->setTrayMaterialData(origin_tray->trayMaterialData());
    }
}

QPointF MaterialTray::getStartPosition(int tray_index)
{
    return parameters[getTrayIndex(tray_index)]->tray_start_position.ToPointF();
}

QPointF MaterialTray::getEndPosition()
{
    return first_tray_end_position.ToPointF();
}

void MaterialTray::setTrayCurrentNg(const int tray_index)
{
    for (int i = getCurrentIndex(tray_index); i <= getLastIndex(); ++i)
    {
        if(getMaterialState(i,tray_index) == MaterialState::IsRawLens)
        {
            setCurrentMaterialState(MaterialState::IsNgLens,tray_index);
        }
        else if(getMaterialState(i,tray_index) == MaterialState::IsRawSensor)
        {
            setCurrentMaterialState(MaterialState::IsRawSensor,tray_index);
        }
    }
    setTrayCurrent(getLastIndex(),tray_index);
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
    {
        qInfo("tray index too large");
        return parameters.count() - 1;
    }
}
