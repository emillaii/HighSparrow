import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width

    contentItem: ColumnLayout{
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
                id: commonControlViewSwitch
            }
        }

        ColumnLayout{
            visible:  commonControlViewSwitch.checked
            RowLayout {
                Label {
                    text: qsTr("")
                }
            }
            GroupBox{
                visible: commonControlViewSwitch.checked
                title:qsTr("常用操作")
                ColumnLayout{
                    GroupBox{
                        visible: baseModuleManager.getServerMode()===0
                        title: qsTr("Lens抓放")
                        ColumnLayout{
                            RowLayout{
                                Label{
                                    text:qsTr("Lens Tray盘1位置移动")
                                }
                            }
                            RowLayout{
                                Label{
                                    text: qsTr("目标行")
                                }
                                TextField{
                                    id:t1_nrow
                                    text: "1"
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator{
                                        bottom: 1
                                    }
                                }
                                Label{
                                    text: qsTr("目标列")
                                }
                                TextField{
                                    id:t1_ncol
                                    text: "1"
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator{
                                        bottom: 1
                                    }
                                }
                                Button{
                                    text:title_move_to
                                    width: 40
                                    height: 40
                                    onClicked: {
                                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                                        logicManager.lensPickArmMoveToTray1Pos()
                                    }
                                }
                                Button{
                                    text:qsTr("取lens")
                                    onClicked:{
                                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                                        logicManager.lensPickArmMoveToPickLensFromTray1()
                                    }
                                }
                                Button{
                                    text:qsTr("放lens")
                                    onClicked: {
                                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                                        logicManager.lensPickArmMoveToPlaceLensToTray1()
                                    }
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("Lens Tray盘2位置移动")
                                }
                            }
                            RowLayout{
                                Label{
                                    text: qsTr("目标行")
                                }
                                TextField{
                                    id:t_nrow
                                    text: "1"
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator{
                                        bottom: 1
                                    }
                                }
                                Label{
                                    text: qsTr("目标列")
                                }
                                TextField{
                                    id:t_ncol
                                    text: "1"
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator{
                                        bottom: 1
                                    }
                                }
                                Button{
                                    text:title_move_to
                                    width: 40
                                    height: 40
                                    onClicked: {
                                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                                        logicManager.lensPickArmMoveToTray2Pos()
                                    }
                                }
                                Button{
                                    text:qsTr("取lens")
                                    onClicked: {
                                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                                        logicManager.lensPickArmMoveToTray2Pos()
                                    }
                                }
                                Button{
                                    text:qsTr("放lens")
                                    onClicked: {
                                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                                        logicManager.lensPickArmMoveToPlaceLensToTray2()
                                    }
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("LUT取放lens")
                                }
                            }
                            RowLayout{
                                Button{
                                    text:qsTr("从LUT取NGlens")
                                    onClicked: {
                                        logicManager.lensPickArmMoveToPickLensFromLut()
                                    }
                                }
                                Button{
                                    text:qsTr("放lens到LUT")
                                    onClicked: {
                                        logicManager.lensPickArmMoveToPlaceLensToLut()
                                    }
                                }
                            }
                        }
                    }
                    GroupBox{
                        title:qsTr("Dispense")
                        ColumnLayout{
                            RowLayout{
                                Label{
                                    text:qsTr("移动到点胶位置")
                                }
                            }
                            RowLayout{
                                Label {
                                    text: qsTr("Offset_X")
                                }
                                TextField {
                                    text: dispenseParams.dispenseXOffset
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: DoubleValidator {
                                        decimals: 6
                                        notation: DoubleValidator.StandardNotation
                                    }
                                    onEditingFinished: {
                                        dispenseParams.setDispenseXOffset(text)
                                    }
                                }

                                Label {
                                    text: qsTr("Offset_Y")
                                }
                                TextField {
                                    text: dispenseParams.dispenseYOffset
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: DoubleValidator {
                                        decimals: 6
                                        notation: DoubleValidator.StandardNotation
                                    }
                                    onEditingFinished: {
                                        dispenseParams.setDispenseYOffset(text)
                                    }
                                }
                                Button {
                                    text: qsTr("去喷胶位置")
                                    width: 20
                                    height: 40
                                    onClicked: {
                                        dispenseModule.moveToDispenseDot(false)
                                    }
                                }
                                Button{
                                    text: qsTr("喷胶")
                                    width: 20
                                    height: 40
                                    onClicked: {
                                        aaNewCore.performHandlingOperation(1)
                                    }
                                }
                            }
                        }
                    }
                    GroupBox{
                        title:qsTr("AA")
                        Timer {
                            id:timer
                            interval: 500; running: true; repeat: true
                        }
                        ColumnLayout{
                            RowLayout{
                                Label{
                                    text:qsTr("AA取放lens")
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("移动到放lens位置")
                                }
                                Button{
                                    text:qsTr("移动")
                                    onClicked: {
                                        lutModule.moveToAA1PickLensPos(true)
                                    }
                                }
                                Button{
                                    text:qsTr("PICK")
                                    onClicked: {
                                        logicManager.lutPickLensToAA1()
                                    }
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("移动到Unpick Lens Position")
                                }
                                Button{
                                    text:qsTr("移动")
                                    onClicked: {
                                        lutModule.moveToAA1PickLensPos()
                                    }
                                }
                                Button {
                                    text: qsTr("UnPick")
                                    width: 40
                                    height: 40
                                    onClicked: {
                                        //logicManager.lutPickLensToAA1()
                                        lutModule.moveToAA1UnPickLens()
                        //                lutModule.moveToAA1PickLens(false,true)
                                    }
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("LUT & AA move to uplook position")
                                }
                                Button {
                                    text: title_move_to
                                    width: 40
                                    height: 40
                                    onClicked: {
                                        lutModule.moveToAA1UplookPos()
                                    }
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("UV开关")
                                }
                                Button{
                                    text:qsTr("开启")
                                    onClicked: {

                                    }
                                }
                                Button{
                                    text:qsTr("关闭")
                                    onClicked: {

                                    }
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("Gripper")
                                }
                                Button{
                                    text:qsTr("开启")
                                    onClicked: {
                                        baseModuleManager.setOutput("AA1_GripON",true)
                                    }
                                }
                                Button{
                                    text:qsTr("关闭")
                                    onClicked: {
                                        baseModuleManager.setOutput("AA1_GripON",false)
                                    }
                                }
                            }
                            RowLayout{
                                Switch {
                                    id:special_ouput
                                    text: qsTr("SUT Pressor")
                                    Connections{
                                        target: timer
                                        onTriggered: {
//                                            if (dialog.visible) {
//                                                if(special_ouput.checked !== baseModuleManager.getOutput("POGOPIN下"))
//                                                    special_ouput.toggle()
//                                                //if(index%2==0&&!swich_id.checked)swich_id.toggle()
//                                            }
                                        }
                                    }
                                    onCheckedChanged:{
                                        baseModuleManager.setOutput("POGOPIN下", checked)
                                        baseModuleManager.setOutput("POGOPIN上", !checked)
                                    }
                                }
                            }
                            RowLayout{
                                Switch {
                                    id:sut_vacuum
                                    text: baseModuleManager.getServerMode()==0?qsTr("SUT1吸真空"):qsTr("SUT2吸真空")
                                    Connections{
                                        target: timer
                                        onTriggered: {
//                                            if (dialog.visible) {
//                                                if(sut_vacuum.checked !== baseModuleManager.getOutput(text))
//                                                    sut_vacuum.toggle()
//                                                //if(index%2==0&&!swich_id.checked)swich_id.toggle()
//                                            }
                                        }
                                    }
                                    onCheckedChanged:{
                                        baseModuleManager.setOutput(text, checked)
                                    }
                                }
                            }
                        }
                    }
                    GroupBox{
                        visible: baseModuleManager.getServerMode()==1
                        title:qsTr("SPA")
                        ColumnLayout{
                            RowLayout{
                                Label{
                                    text:qsTr("Sensor Tray Move")
                                }
                            }
                            RowLayout{
                                Label{
                                    text: qsTr("目标行")
                                }
                                TextField{
                                    id:ts_nrow
                                    text: "1"
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator{
                                        bottom: 1
                                    }
                                }
                                Label{
                                    text: qsTr("目标列")
                                }
                                TextField{
                                    id:ts_ncol
                                    text: "1"
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator{
                                        bottom: 1
                                    }
                                }
                                Button{
                                    text:title_move_to
                                    width: 40
                                    height: 40
                                    onClicked: {
                                        material_tray.setTrayCurrent(ts_ncol.text-1,ts_nrow.text-1,0)
                                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY1)
                                    }
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("Camera Tray Move")
                                }
                            }
                            RowLayout{
                                Label{
                                    text: qsTr("目标行")
                                }
                                TextField{
                                    id:tc_nrow
                                    text: "1"
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator{
                                        bottom: 1
                                    }
                                }
                                Label{
                                    text: qsTr("目标列")
                                }
                                TextField{
                                    id:tc_ncol
                                    text: "1"
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator{
                                        bottom: 1
                                    }
                                }
                                Button{
                                    text:title_move_to
                                    width: 40
                                    height: 40
                                    onClicked: {
                                        material_tray.setTrayCurrent(tc_ncol.text-1,tc_nrow.text-1,1)
                                        sensorLoaderModule.performHandling(SensorLoaderModule.SENSOR_TRAY2)
                                    }
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("Sensor/Camera取放")
                                }
                            }
                            RowLayout{
                                Button{
                                    text:qsTr("从Sensor盘取料")
                                    onClicked: {
                                        material_tray.setTrayCurrent(ts_ncol.text-1,ts_nrow.text-1,0)
                                        sensorLoaderModule.performHandling(SensorLoaderModule.PICK_SENSOR_FROM_TRAY
                                                                           +SensorLoaderModule.TO_PICK1
                                                                           +SensorLoaderModule.SENSOR_PR
                                                                           +SensorLoaderModule.SENSOR_TRAY1)
                                    }
                                }
                                Button{
                                    text:qsTr("放NgSensor到Camera盘上")
                                    onClicked:{
                                        material_tray.setTrayCurrent(tc_ncol.text-1,tc_nrow.text-1,1)
                                        //6232
                                        sensorLoaderModule.performHandling(SensorLoaderModule.PLACE_NG_SENSOR_TO_TRAY
                                                                           +SensorLoaderModule.TO_PICK2
                                                                           +SensorLoaderModule.VACANCY_PR
                                                                           +SensorLoaderModule.SENSOR_TRAY2)
                                    }
                                }
                                Button{
                                    text:qsTr("到Camera盘放成品")
                                    onClicked:{
                                        material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,1)
                                        sensorLoaderModule.performHandling(SensorLoaderModule.PLACE_PRODUCT_TO_TRAY
                                                                           +SensorLoaderModule.TO_PICK2
                                                                           +SensorLoaderModule.VACANCY_PR
                                                                           +SensorLoaderModule.SENSOR_TRAY2)
                                    }
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("SUT1")
                                }
                                Button{
                                    text:qsTr("放sensors")
                                    width: 40
                                    height: 40
                                    onClicked: {
                                        //2110
                                        sensorLoaderModule.performHandling(SensorLoaderModule.PLACE_SENSOR_TO_SUT1
                                                                           +SensorLoaderModule.TO_PICK1
                                                                           +SensorLoaderModule.RESET_PR)
                                    }
                                }
                                Button{
                                    text:qsTr("取成品")
                                    width:40
                                    height: 40
                                    onClicked: {
                                        //7263
                                        sensorLoaderModule.performHandling(SensorLoaderModule.PICK_PRODUCT_FROM_SUT1
                                                                           +SensorLoaderModule.TO_PICK2
                                                                           +SensorLoaderModule.PRODUCT_PR
                                                                           +SensorLoaderModule.SUT_POS1)
                                    }
                                }
                                Button{
                                    text:qsTr("取Ng Sensor")
                                    width:40
                                    height: 40
                                    onClicked: {
                                        //4253
                                        sensorLoaderModule.performHandling(SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT1
                                                                           +SensorLoaderModule.TO_PICK2
                                                                           +SensorLoaderModule.NG_SENSOR_PR
                                                                           +SensorLoaderModule.SUT_POS1)
                                    }
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("SUT2")
                                }
                                Button{
                                    text:qsTr("放sensors")
                                    width:40
                                    height: 40
                                    onClicked: {
                                        //2114
                                        sensorLoaderModule.performHandling(SensorLoaderModule.PLACE_SENSOR_TO_SUT1
                                                                           +SensorLoaderModule.TO_PICK1
                                                                           +SensorLoaderModule.RESET_PR
                                                                           +SensorLoaderModule.SUT_POS2)
                                    }
                                }
                                Button{
                                    text:qsTr("取成品")
                                    width:40
                                    height: 40
                                    onClicked: {
                                        //7264
                                        sensorLoaderModule.performHandling(SensorLoaderModule.PICK_PRODUCT_FROM_SUT1
                                                                           +SensorLoaderModule.TO_PICK2
                                                                           +SensorLoaderModule.PRODUCT_PR
                                                                           +SensorLoaderModule.SUT_POS2)
                                    }
                                }
                                Button{
                                    text:qsTr("取Ng Sensor")
                                    width:40
                                    height: 40
                                    onClicked: {
                                        //254
                                        sensorLoaderModule.performHandling(SensorLoaderModule.TO_PICK2
                                                                           +SensorLoaderModule.NG_SENSOR_PR
                                                                           +SensorLoaderModule.SUT_POS2)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}