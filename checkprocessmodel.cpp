#include "checkprocessitem.h"
#include "checkprocessmodel.h"

CheckProcessModel::CheckProcessModel(QObject *parent)
{
    mRoleNames.insert(ItemName,"title");
    mRoleNames.insert(Operate,"operate");
    //1.提示放入蘑菇头，确定LUT的 AA1UplookPosition和AA2UplookPosition及其等待位置（移动、校正LUT的MushroomCalibration、执行LUT的MushroomPR、记录位置）
    PCheckProcessItem checkLUTPos = new CheckProcessItem();
    checkLUTPos->setData(ItemName,u8"1.放入蘑菇头，确定LUT的 AA1UplookPosition和AA2UplookPosition及其等待位置");
    checkLUTPos->setData(Operate,u8"");

    PCheckProcessItem AA1UplookPosition = new CheckProcessItem();
    AA1UplookPosition->setData(ItemName,"AA1UplookPosition");
    PCheckProcessItem AA1PosX = new CheckProcessItem;
    PCheckProcessItem AA1PosY = new CheckProcessItem;
    PCheckProcessItem AA1PosZ = new CheckProcessItem;
    AA1PosX->setData(ItemName,"X");
    AA1PosY->setData(ItemName,"Y");
    AA1PosZ->setData(ItemName,"Z");
    PCheckProcessItem MoveAA1Pos = new CheckProcessItem;
    MoveAA1Pos->setData(ItemName,u8"移动");
    AA1UplookPosition->appendNode(AA1PosX);
    AA1UplookPosition->appendNode(AA1PosY);
    AA1UplookPosition->appendNode(AA1PosZ);
    AA1UplookPosition->appendNode(MoveAA1Pos);

    PCheckProcessItem AA2UplookPosition = new CheckProcessItem();
    AA2UplookPosition->setData(ItemName,"AA2UplookPosition");
    PCheckProcessItem AA2PosX = new CheckProcessItem;
    PCheckProcessItem AA2PosY = new CheckProcessItem;
    PCheckProcessItem AA2PosZ = new CheckProcessItem;
    AA2PosX->setData(ItemName,"X");
    AA2PosY->setData(ItemName,"Y");
    AA2PosZ->setData(ItemName,"Z");
    PCheckProcessItem MoveAA2Pos = new CheckProcessItem;
    MoveAA2Pos->setData(ItemName,u8"移动");
    AA2UplookPosition->appendNode(AA2PosX);
    AA2UplookPosition->appendNode(AA2PosY);
    AA2UplookPosition->appendNode(AA2PosZ);
    AA2UplookPosition->appendNode(MoveAA2Pos);

    checkLUTPos->appendNode(AA1UplookPosition);
    checkLUTPos->appendNode(AA2UplookPosition);


    mRootEntrys.append(checkLUTPos);
    //2.提示放置好Chart图、拿走蘑菇头
    PCheckProcessItem checkChart = new CheckProcessItem();
    checkChart->setData(ItemName,u8"2.放置好Chart图、拿走蘑菇头");
    checkChart->setData(Operate,u8"");
    mRootEntrys.append(checkChart);
    //3.提示移走给AA1、AA2手动夹上lens（开关griper、griper状态）
    PCheckProcessItem checkLensPutDown = new CheckProcessItem();
    checkLensPutDown->setData(ItemName,u8"3.移走给AA1、AA2手动夹上lens");
    checkLensPutDown->setData(Operate,u8"");

    PCheckProcessItem AA1Gripper = new CheckProcessItem;
    PCheckProcessItem AA2Gripper = new CheckProcessItem;
    AA1Gripper->setData(ItemName,"AA1gripper");
    AA2Gripper->setData(ItemName,"AA2gripper");
    checkLensPutDown->appendNode(AA1Gripper);
    checkLensPutDown->appendNode(AA2Gripper);

    mRootEntrys.append(checkLensPutDown);
    //4.确定AA头的大致的mushroom位置、高度以视觉清晰为准、大概水平
    PCheckProcessItem checkAAHead = new CheckProcessItem();
    checkAAHead->setData(ItemName,u8"4.确定AA头的大致的mushroom位置、高度");
    checkAAHead->setData(Operate,u8"");

    PCheckProcessItem mushRoomPos = new CheckProcessItem;
    mushRoomPos->setData(ItemName,u8"蘑菇头位置");
    PCheckProcessItem mushRoomAAX = new CheckProcessItem;
    PCheckProcessItem mushRoomAAY = new CheckProcessItem;
    PCheckProcessItem mushRoomAAZ = new CheckProcessItem;
    PCheckProcessItem mushRoomAAA = new CheckProcessItem;
    PCheckProcessItem mushRoomAAB = new CheckProcessItem;
    PCheckProcessItem mushRoomAAC = new CheckProcessItem;

    mushRoomAAX->setData(ItemName,"AA_X");
    mushRoomAAY->setData(ItemName,"AA_Y");
    mushRoomAAZ->setData(ItemName,"AA_Z");
    mushRoomAAA->setData(ItemName,"AA_A");
    mushRoomAAB->setData(ItemName,"AA_B");
    mushRoomAAC->setData(ItemName,"AA_C");

    mushRoomPos->appendNode(mushRoomAAX);
    mushRoomPos->appendNode(mushRoomAAY);
    mushRoomPos->appendNode(mushRoomAAZ);
    mushRoomPos->appendNode(mushRoomAAA);
    mushRoomPos->appendNode(mushRoomAAB);
    mushRoomPos->appendNode(mushRoomAAC);
    checkAAHead->appendNode(mushRoomPos);
    mRootEntrys.append(checkAAHead);
    //5.提示SUT1、SUT2打水平并手动放上sensor并点亮（POGOPIN开关、SUT真空开关及其状态、点亮开关）
    PCheckProcessItem checkSUT = new CheckProcessItem();
    checkSUT->setData(ItemName,u8"5.SUT1、SUT2打水平并手动放上sensor并点亮");
    checkSUT->setData(Operate,u8"");
    PCheckProcessItem POGOPIN = new CheckProcessItem;
    POGOPIN->setData(ItemName,u8"POGOPIN开关");
    checkSUT->appendNode(POGOPIN);
    PCheckProcessItem sutVacuum = new CheckProcessItem;
    sutVacuum->setData(ItemName,u8"SUT真空开关");
    checkSUT->appendNode(sutVacuum);
    mRootEntrys.append(checkSUT);
    //6.确定SUT的DownlookPR位置（移动、校正SUT的DownlookCaliration、执行SUT的DownlookPR、记录位置）
    PCheckProcessItem checkSUTDownLookPR = new CheckProcessItem();
    checkSUTDownLookPR->setData(ItemName,u8"6.校正SUT DownlookPR");
    checkSUTDownLookPR->setData(Operate,u8"确定SUT的DownlookPR位置");
    mRootEntrys.append(checkSUTDownLookPR);
    //7.确定SUT与LUT的位置关系（移动LUT先自动退到等待位置、校正SUT的UpdownlookDownCalibration和SUT的UpdownLookUpCailration、执行SUT的UpDownlookPR、计算SUT的Mushroom位置的X、Y坐标、Z先置为0、手动确定Z）
    PCheckProcessItem checkSutLutPositionalRelation = new CheckProcessItem();
    checkSutLutPositionalRelation->setData(ItemName,u8"7.校正位置关系");
    checkSutLutPositionalRelation->setData(Operate,u8"确定SUT与LUT的位置关系");
    mRootEntrys.append(checkSutLutPositionalRelation);
    //8.做OC、AA (校正CartCalibration、执行ChartCalibrationPR、计算像素比例和图形角度、记录AA结果的A和B到AA头的mushroom位置)
    PCheckProcessItem checkOCNAA = new CheckProcessItem();
    checkOCNAA->setData(ItemName,u8"8.校正OC和AA");
    checkOCNAA->setData(Operate,u8"校正CartCalibration、执行ChartCalibrationPR、计算像素比例和图形角度、记录AA结果的A和B到AA头的mushroom位置");
    mRootEntrys.append(checkOCNAA);
    //9.确定AAmushroom位置的XYC坐标（移动 SUT先自动退到DownlookPR位置、校正LUT的UplookCalibration、执行LUT的UplookPR、记录位置）
    PCheckProcessItem checkAAMushroomXYC = new CheckProcessItem();
    checkAAMushroomXYC->setData(ItemName,u8"9.确定AAmushroom位置");
    checkAAMushroomXYC->setData(Operate,u8"移动SUT先自动退到DownlookPR位置、校正LUT的UplookCalibration、执行LUT的UplookPR、记录位置");
    mRootEntrys.append(checkAAMushroomXYC);
    //10.确定LUT和LPA的位置关系（移动 LUT的load_uplook_position LPA的lut_camera_position、校正lpa_updownlook_up_calibration、校正lpa_updownlook_down_calibration、执行lpa_updownlook_pr）
    PCheckProcessItem checkLutLpaPositionalRelation = new CheckProcessItem();
    checkLutLpaPositionalRelation->setData(ItemName,u8"10.确定LUT和LPA的位置关系");
    checkLutLpaPositionalRelation->setData(Operate,u8"移动 LUT的load_uplook_position LPA的lut_camera_position、校正lpa_updownlook_up_calibration、校正lpa_updownlook_down_calibration、执行lpa_updownlook_pr");
    mRootEntrys.append(checkLutLpaPositionalRelation);
    //11.确定lut_uplook_picker_position，放料角度(移动、校正lut_downlook_calibration 执行lut_downlook_pr,位置记录，计算吸头偏差)
    PCheckProcessItem checkDischargeAngle = new CheckProcessItem();
    checkDischargeAngle->setData(ItemName,u8"11.校正LUT位置及放料角度");
    checkDischargeAngle->setData(Operate,u8"移动、校正lut_downlook_calibration 执行lut_downlook_pr,位置记录，计算吸头偏差");
    mRootEntrys.append(checkDischargeAngle);
    //12.确定lpa_downlook_load_position和lpa_downlook_unload_position位置（移动、校正、执行PR，记录位置、计算AApick等相关位置calculateRelativePosition、取AANglens、lpa取NGlens、lpa放lens、aapicklens）
    PCheckProcessItem checkLpaDownlookLoadNLpaDownLookUnload = new CheckProcessItem();
    checkLpaDownlookLoadNLpaDownLookUnload->setData(ItemName,u8"12.校正LPA位置");
    checkLpaDownlookLoadNLpaDownLookUnload->setData(Operate,u8"移动、校正、执行PR，记录位置、计算AApick等相关位置calculateRelativePosition、取AANglens、lpa取NGlens、lpa放lens、aapicklens");
    mRootEntrys.append(checkLpaDownlookLoadNLpaDownLookUnload);
    //13.LensTrayloader位置和操作
    PCheckProcessItem checkLensTrayLoader = new CheckProcessItem();
    checkLensTrayLoader->setData(ItemName,u8"13.校正LensTrayLoader");
    checkLensTrayLoader->setData(Operate,u8"LensTrayloader位置和操作");
    mRootEntrys.append(checkLensTrayLoader);
    //14.Lens盘规格和操作（校正、PR、视觉测高、原地测高、取lens放Nglens）
    PCheckProcessItem checkLensTray = new CheckProcessItem();
    checkLensTray->setData(ItemName,u8"14.校正Lens Tray盘");
    checkLensTray->setData(Operate,u8"校正、PR、视觉测高、原地测高、取lens放Nglens");
    mRootEntrys.append(checkLensTray);
    //15.SensorTrayloader位置和操作
    PCheckProcessItem checkSensorTrayLoader = new CheckProcessItem();
    checkSensorTrayLoader->setData(ItemName,u8"15.校正SensorTrayLoader");
    checkSensorTrayLoader->setData(Operate,u8"SensorTrayloader位置和操作");
    mRootEntrys.append(checkSensorTrayLoader);
    //16.Sensor盘规格和操作
    PCheckProcessItem checkSensorTray = new CheckProcessItem();
    checkSensorTray->setData(ItemName,u8"16.校正sensor Tray盘");
    checkSensorTray->setData(Operate,u8"Sensor盘规格和操作");
    mRootEntrys.append(checkSensorTray);
    //17.LPA的SUT位置和操作
    PCheckProcessItem checkLpaNSut = new CheckProcessItem();
    checkLpaNSut->setData(ItemName,u8"17.校正LPA和SUT");
    checkLpaNSut->setData(Operate,u8"LPA的SUT位置和操作");
    mRootEntrys.append(checkLpaNSut);
}

CheckProcessModel::~CheckProcessModel()
{
    qDeleteAll(mRootEntrys);

}

int CheckProcessModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid()){
        PCheckProcessItem parentNode = (PCheckProcessItem)parent.internalPointer();
        return parentNode->childs.size();
    }
    return mRootEntrys.size();
}

int CheckProcessModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QModelIndex CheckProcessModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!parent.isValid()){
        if((row>=0)&&(row<mRootEntrys.size())){
            return createIndex(row,column,mRootEntrys.at(row));
        }
    }else{
        PCheckProcessItem parentNode = (PCheckProcessItem)parent.internalPointer();
        return createIndex(row,column,parentNode->childs[row]);
    }
    return QModelIndex();
}

QVariant CheckProcessModel::data(const QModelIndex &index, int role) const
{
    PCheckProcessItem node = (PCheckProcessItem)index.internalPointer();
    return node->data(role);
}

QModelIndex CheckProcessModel::parent(const QModelIndex &index) const
{
    PCheckProcessItem node = (PCheckProcessItem)index.internalPointer();
    if(node->parent()==NULL){
        return QModelIndex();
    }else{
        return createIndex(0,1,node->parent());
    }
}

QHash<int, QByteArray> CheckProcessModel::roleNames() const
{
    return mRoleNames;
}

void CheckProcessModel::_addEntryNode(CheckProcessItem *node, CheckProcessItem *parent)
{
    if(parent==NULL){
        mRootEntrys.append(node);
    }else{
        node->setParent(parent);
        parent->appendNode(node);
    }
}
