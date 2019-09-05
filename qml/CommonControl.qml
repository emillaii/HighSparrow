import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width

    contentItem: ColumnLayout{
        spacing: 0

        ColumnLayout{
            RowLayout {
                Label {
                    text: qsTr("")
                }
            }
            GroupBox{
                title:qsTr("常用操作")
                ColumnLayout{
                    GroupBox{
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
                                        sh_lens_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                                        logicManager.pickArmMoveToLensTray1Pos()
                                    }
                                }
                                Button{
                                    text:qsTr("取lens")
                                    onClicked:{
                                        sh_lens_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                                        logicManager.pickArmMoveToPickLensFromTray1()
                                    }
                                }
                                Button{
                                    text:qsTr("放lens")
                                    onClicked: {
                                        sh_lens_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                                        logicManager.pickArmMoveToPlaceLensToTray1()
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
                                    id:t_nrow2
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
                                    id:t_ncol2
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
                                        sh_lens_tray.setTrayCurrent(t_ncol2.text-1,t_nrow2.text-1,1)
                                        logicManager.pickArmMoveToLensTray2Pos()
                                    }
                                }
                                Button{
                                    text:qsTr("取lens")
                                    onClicked: {
                                        sh_lens_tray.setTrayCurrent(t_ncol2.text-1,t_nrow2.text-1,1)
                                        logicManager.pickArmMoveToPickLensFromTray2()
                                    }
                                }
                                Button{
                                    text:qsTr("放lens")
                                    onClicked: {
                                        sh_lens_tray.setTrayCurrent(t_ncol2.text-1,t_nrow2.text-1,1)
                                        logicManager.pickArmMoveToPlaceLensToTray2()
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
                                        logicManager.pickArmMoveToPickLensFromLut()
                                    }
                                }
                                Button{
                                    text:qsTr("放lens到LUT")
                                    onClicked: {
                                        logicManager.pickArmMoveToPlaceLensToLut()
                                    }
                                }
                            }
                        }
                    }
                    GroupBox{
                        title:qsTr("sensor抓放")
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
                                        sh_sensor_tray.setTrayCurrent(ts_ncol.text-1,ts_nrow.text-1,1)
                                        logicManager.pickArmMoveToSensorTrayPos()
                                    }
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("reject Tray Move")
                                }
                            }
                            RowLayout{
                                Label{
                                    text: qsTr("目标行")
                                }
                                TextField{
                                    id:tr_nrow
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
                                    id:tr_ncol
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
                                        sh_reject_tray.setTrayCurrent(tr_ncol.text-1,tr_nrow.text-1,1)
                                        logicManager.pickArmMoveToRejectTrayPos()
                                    }
                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("Sensor/Product取放")
                                }
                            }
                            RowLayout{
                                Button{
                                    text:qsTr("取Sensor")
                                    onClicked: {
                                        sh_sensor_tray.setTrayCurrent(ts_ncol.text-1,ts_nrow.text-1,0)
                                        logicManager.pickArmMoveToPickSensorFromTray()
                                    }
                                }
                                Button{
                                    text:qsTr("放NgSensor")
                                    onClicked:{
                                        sh_reject_tray.setTrayCurrent(tr_ncol.text-1,tr_nrow.text-1,1)
                                        logicManager.pickArmMoveToPlaceSensorToTray()
                                    }
                                }
//                                Button{
//                                    text:qsTr("放成品")
//                                    onClicked:{
//                                        //todo
//                                    }
//                                }
                            }
                            RowLayout{
                                Label{
                                    text:qsTr("SUT")
                                }

                            }
                            RowLayout{

                                Button{
                                    text:qsTr("放sensor")
                                    width: 40
                                    height: 40
                                    onClicked: {
                                        logicManager.pickArmMoveToPlaceSensorToSut()
                                    }
                                }
//                                Button{
//                                    text:qsTr("取成品")
//                                    width:40
//                                    height: 40
//                                    onClicked: {
//                                        //todo
//                                    }
//                                }
                                Button{
                                    text:qsTr("取Ng Sensor")
                                    width:40
                                    height: 40
                                    onClicked: {
                                        logicManager.pickArmMoveToPickSensorFromSut()
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
                                    text:qsTr("移动到夾lens位置")
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
                                        //lutModule.moveToAA1PickLens(false,true)
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
                                            if(special_ouput.checked !== baseModuleManager.getOutput("POGOPIN下"))
                                                special_ouput.toggle()
                                            //if(index%2==0&&!swich_id.checked)swich_id.toggle()
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
                                            //console.log(text)
                                            if(sut_vacuum.checked !== baseModuleManager.getOutput(text))
                                                sut_vacuum.toggle()
                                            //if(index%2==0&&!swich_id.checked)swich_id.toggle()
                                        }
                                    }
                                    onCheckedChanged:{
                                        baseModuleManager.setOutput(text, checked)
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
