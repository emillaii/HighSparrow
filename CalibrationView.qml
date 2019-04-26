import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2



ItemDelegate {
/*
    property var moveTo1 : function(){
        console.log("Move To Downlook Position")
        sutModule.moveToDownlookPos()
    }
    property var cali1 : function(){
        console.log("Start Downlook Cali")
        baseModuleManager.performDownlookCalibration()
    }
    property var perform1 : function(){
        console.log("only PR")
        highSprrow.performOnlySUTDownlookPR()
    }

    property var moveTo2 : function(){
        console.log("Move To Tool Downlook Position")
        sutModule.moveToToolDownlookPos(true)
    }
    property var cali2 : function(){
        console.log("Start Mushroom Cali")
        baseModuleManager.performUpDownlookCalibration()
    }
    property var perform2 : function(){
        console.log("Perform PR")
        sutModule.toolDownlookPR(false,true)
    }

    property var moveTo3 : function(){
        console.log("Move To Tool Uplook Position")
        lutModule.moveToToolUplookPos(true)
    }
    property var cali3 : function(){
        console.log("Start Mushroom Cali")
        baseModuleManager.performUpDownlookUpCalibration()
    }
    property var perform3 : function(){
        console.log("Perform PR")
        lutModule.toolUplookPR(false,true)
    }

    property var moveTo4 : function(){
        console.log("Move To Mushroom Position")
        lutModule.moveToAA1MushroomLens(true)
    }
    property var cali4 : function(){
        console.log("Start Mushroom Cali")
        baseModuleManager.performAA1MushroomHeadCalibration()
    }
    property var perform4 : function(){
        console.log("Perform PR")
        highSprrow.performLUTMushroomPR()
    }

    property var moveTo5 : function(){
        console.log("Move To Uplook Position")
        lutModule.moveToAA1UplookPos(true)
    }
    property var cali5 : function(){
        console.log("Start Uplook Cali")
        baseModuleManager.performUplookCalibration()
    }
    property var perform5 : function(){
        console.log("Perform PR")
        highSprrow.performLUTUplookPR()
    }

    property var moveTo6 : function(){
        console.log("料盘lens视觉移动")
        logicManager.lensPickArmMoveToTray1StartPos()
    }
    property var cali6 : function(){
        console.log("料盘lens视觉校正")
        baseModuleManager.performLPALensCalibration()
    }
    property var perform6 : function(){
        console.log("料盘lens视觉执行PR")
        highSprrow.performLPALensPR()
    }

    property var moveTo7 : function(){
        console.log("料盘空位视觉移动")
        logicManager.lensPickArmMoveToTray1StartPos()
    }
    property var cali7 : function(){
        console.log("此项无校正方法···")
    }
    property var perform7 : function(){
        console.log("料盘空位视觉执行PR")
        highSprrow.performVacancyLensPR()
    }

    property var moveTo8 : function(){
        console.log("Lut Lens 视觉移动")
        logicManager.lensPickArmMoveToLutPos2()
    }
    property var cali8 : function(){
        console.log("此项无此方法···")
    }
    property var perform8 : function(){
        console.log("Lut Lens 视觉执行PR")
        highSprrow.performLUTLensPR()
    }
//*/
    width: parent.width
    contentItem: ColumnLayout {
        spacing: 0
        RowLayout {
            ColumnLayout {
                RowLayout {
                    Label {
                        id: itemLabel
                        text: model.label
                        font.pixelSize: 20
                    }
                }
            }
            Switch {
                id: calibrationViewSwitch
            }
        }
        ColumnLayout {
            visible: calibrationViewSwitch.checked
            Layout.alignment: Qt.AlignTop
            ListModel{
                id: calibration_model
/*
                ListElement{
                    name:"SUT Downlook PR"
                    item: "prSUTDownlookParams"
                    isCali:1
                }
                ListElement{
                    name:"SUT ToolDownlook PR"
                    item:"prAA1ToolDownlookParams"
                    isCali:1
                }
                ListElement{
                    name:"LUT ToolUplook PR"
                    item:"prAA1ToolUplookParams"
                    isCali:1
                }
                ListElement{
                    name:"LUT Mushroom PR"
                    item:"prAA1MushroomParams"
                    isCali:1
                }
                ListElement{
                    name:"LUT Uplook PR"
                    item:"prAA1LUTUplookParams"
                    isCali:1
                }
                ListElement{
                    name:"料盘Lens视觉"
                    item:"prLENSLpaLook"
                    isCali:1
                }
                ListElement{
                    name:"料盘空位视觉"
                    item:"prVACANCYLpaLook"
                    isCali:0
                }
                ListElement{
                    name:"Lut Lens 视觉"
                    item:"prLENSLutLook"
                    isCali:0
                }

//*/
            }
            ListView{
                model:vl_parameter_list
                height: 2000
                delegate: GroupBox{
/*
                    property var delegateMap:{
                        "prSUTDownlookParams":prSUTDownlookParams,
                        "prAA1ToolDownlookParams":prAA1ToolDownlookParams,
                        "prAA1ToolUplookParams":prAA1ToolUplookParams,
                        "prAA1MushroomParams":prAA1MushroomParams,
                        "prAA1LUTUplookParams":prAA1LUTUplookParams,
                        "prLENSLpaLook":prLENSLpaLook,
                        "prVACANCYLpaLook":prVACANCYLpaLook,
                        "prLENSLutLook":prLENSLutLook
                    }
                    property var movetoMap:{
                        "prSUTDownlookParams":moveTo1,
                        "prAA1ToolDownlookParams":moveTo2,
                        "prAA1ToolUplookParams":moveTo3,
                        "prAA1MushroomParams":moveTo4,
                        "prAA1LUTUplookParams":moveTo5,
                        "prLENSLpaLook":moveTo6,
                        "prVACANCYLpaLook":moveTo7,
                        "prLENSLutLook":moveTo8
                    }

                    property var caliMap:{
                        "prSUTDownlookParams":cali1,
                        "prAA1ToolDownlookParams":cali2,
                        "prAA1ToolUplookParams":cali3,
                        "prAA1MushroomParams":cali4,
                        "prAA1LUTUplookParams":cali5,
                        "prLENSLpaLook":cali6,
                        "prVACANCYLpaLook":cali7,
                        "prLENSLutLook":cali8
                    }

                    property var performMap:{
                        "prSUTDownlookParams":perform1,
                        "prAA1ToolDownlookParams":perform2,
                        "prAA1ToolUplookParams":perform3,
                        "prAA1MushroomParams":perform4,
                        "prAA1LUTUplookParams":perform5,
                        "prLENSLpaLook":perform6,
                        "prVACANCYLpaLook":perform7,
                        "prLENSLutLook":perform8
                    }
//*/

                    title:locationName
                    ColumnLayout{
                        RowLayout{
                            FileDialog{
                                id:file_dialog
                                title:"选择加载PR文件"
                                selectExisting: true
                                selectFolder: false
                                selectMultiple: false

                                nameFilters: ["avdata文件 (*.avdata)"]
                                onAccepted:{
                                    setPrFileName(fileUrl)
                                    //delegateMap[item].setPrFileName(fileUrl)
                                }
                            }
                            Dial{
                                width: 25
                                from: 0
                                value: lightBrightness
                                to: 255
                                stepSize: 1

                                Label {
                                    text: lightBrightness
                                    color: "white"
                                    font.pixelSize: Qt.application.font.pixelSize * 3
                                    anchors.centerIn: parent
                                }
                                onValueChanged: {
                                    if (calibrationViewSwitch.checked) {
                                        setLightBrightness(value)
                                        //console.log("after setLightBrightness")
                                        lightingController.setUplookLighting(value)
                                    }
                                }
                            }
                            Button{
                                text:qsTr("读取PR文件")
                                onClicked: {
                                    file_dialog.open()
                                }
                            }
                            TextField{
                                color: "#57f529"
                                text: prFileName
                                font.pixelSize: 14
                            }
                        }
                        RowLayout{
                            Button{
                                visible: needCalibration
                                text:qsTr("校正")
                                onClicked:{
                                    logicManager.performCalibration(calibrationName)
                                }
                            }
                            Button{
                                text:qsTr("执行PR")
                                onClicked:{
                                    logicManager.performLocation(locationName)
                                }
                            }
                        }
                    }
                }
            }

/*

            GroupBox {
                 title: qsTr("Chart")
                 CalibrationChart {
                 }
            }
            GroupBox {
                title: qsTr("SUT Downlook PR")
                CalibrationDownlookView {
                }
            }GroupBox {
                title: qsTr("SUT ToolDownlook PR")
                CalibrationAA1ToolDownlookView{}
            }GroupBox {
                title: qsTr("LUT ToolUplook PR")
                CalibrationAA1ToolUplookView{}
            }
            GroupBox {
                title: qsTr("LUT Mushroom PR")
                CalibrationAA1MushroomView {
                }
            }
            GroupBox {
                title: qsTr("LUT Uplook PR")
                CalibrationUplookView {
                }
            }
            GroupBox {
                title: qsTr("Chart")
                CalibrationChart {
                }
            }
            GroupBox{
                title:qsTr("料盘Lens视觉")
                CalibrationPrLENSLpaLookView{}
            }
            GroupBox{
                title:qsTr("料盘空位视觉")
                CalibrationPrVACANCYLpaLookView{}
            }
            GroupBox{
                title:qsTr("Lut Lens 视觉")
                CalibrationPrLENSLutLookView{}
            }
//*/


        }
    }
}
