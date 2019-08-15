﻿import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width
    visible: true
    contentItem: ColumnLayout{
        spacing: 0
        ListModel{
            id:motorsModel
            ListElement{
                modelData:"LTIE"
            }
            ListElement{
                modelData:"LTK_X1"
            }
            ListElement{
                modelData:"LTK_X2"
            }
            ListElement{
                modelData:"LTL_X"
            }
            ListElement{
                modelData:"LTOE"
            }
        }
        MotionPopup{
            id:tray_loader_module_motions
        }

        ColumnLayout{
            RowLayout {
                Label {
                    text: qsTr("")
                }
                Button{
                    text:qsTr("手动操作")
                    onClicked:{
                        tray_loader_module_motions.open()
                    }
                }
            }
            GroupBox{
                title: qsTr("进盘弹夹")
                ColumnLayout{
                    RowLayout{
                        Label{
                            text:qsTr("换料位置")
                        }
                        TextField{
                            text:tray_clipin_parameter.changeClipPos
                            horizontalAlignment:TextInput.AlignHCenter
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_clipin_parameter.setChangeClipPos(text);
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTIEMovetoChangeClipPos()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTIEName)
                                tray_clipin_parameter.setChangeClipPos(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("起始位置")
                        }
                        TextField{
                            text:tray_clipin_parameter.firstTrayPos
                            horizontalAlignment:TextInput.AlignHCenter
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_clipin_parameter.setFirstTrayPos(text);
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTIEMovetoFirstPos()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTIEName)
                                tray_clipin_parameter.setFirstTrayPos(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("结束位置")
                        }
                        TextField{
                            text:tray_clipin_parameter.lastTrayPos
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_clipin_parameter.setLastTrayPos(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTIEMovetoLastPos()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTIEName)
                                tray_clipin_parameter.setLastTrayPos(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("列数")
                        }
                        TextField{
                            text:tray_clipin_parameter.columnCount
                            validator: IntValidator{
                                bottom: 2
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                tray_clipin_parameter.setColumnCount(text)
                            }
                        }
                        Label{
                            text:qsTr("列距")
                        }
                        TextField{
                            text:tray_clipin_parameter.columnDelta
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                        }
                        Button{
                            visible: false
                            text:qsTr("计算")
                            onClicked: {
                                var y1 = tray_clipin_parameter.firstTrayPos;
                                var y2 = tray_clipin_parameter.lastTrayPos;
                                var delta = (y2-y1)/(tray_clipin_parameter.columnCount-1);
                                tray_clipin_parameter.setColumnDelta(delta);
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("目标位置")
                        }
                        TextField{
                            id:col_in
                            text:tray_clipin_parameter.currentIndex+1
                            validator: IntValidator{
                                bottom: 1
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                var idx = text-1
                                tray_clipin_parameter.setCurrentIndex(idx)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTIEMovetoColumnIndex(col_in.text-1)
                            }
                        }
                        Button{
                            text:qsTr("弹出")
                            onClicked: {
                                logicManager.trayLoaderModuleEjectTray()
                            }
                        }
                    }
                }
            }
            GroupBox{
                title: qsTr("出盘弹夹")
                ColumnLayout{
                    RowLayout{
                        Label{
                            text:qsTr("起始位置")
                        }
                        TextField{
                            text:tray_clipout_parameter.firstTrayPos
                            horizontalAlignment:TextInput.AlignHCenter
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_clipout_parameter.setFirstTrayPos(text);
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTOEMovetoFirstPos()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTOEName)
                                tray_clipout_parameter.setFirstTrayPos(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("结束位置")
                        }
                        TextField{
                            text:tray_clipout_parameter.lastTrayPos
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_clipout_parameter.setLastTrayPos(text);
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTOEMovetoLastPos()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTOEName)
                                tray_clipout_parameter.setLastTrayPos(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("列数")
                        }
                        TextField{
                            text:tray_clipout_parameter.columnCount
                            validator: IntValidator{
                                bottom: 2
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                tray_clipout_parameter.setColumnCount(text)
                            }
                        }
                        Label{
                            text:qsTr("列距")
                        }
                        TextField{
                            text:tray_clipout_parameter.columnDelta
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                        }
                        Button{
                            visible: false
                            text:qsTr("计算")
                            onClicked: {
                                var y1 = tray_clipin_parameter.firstTrayPos;
                                var y2 = tray_clipin_parameter.lastTrayPos;
                                var delta = (y2-y1)/(tray_clipin_parameter.columnCount-1);
                                tray_clipout_parameter.setColumnDelta(delta);
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("目标位置")
                        }
                        TextField{
                            id:col_out
                            text:tray_clipout_parameter.currentIndex+1
                            validator: IntValidator{
                                bottom: 1
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                var idx = text-1
                                tray_clipout_parameter.setCurrentIndex(idx)
                            }
                        }
                        Button{
                            text:qsTr("移动")
                            onClicked: {
                                logicManager.trayLoaderModuleLTOEMovetoColumnIndex(col_out.text-1)
                            }
                        }
                    }
                }
            }
            GroupBox{
                title:qsTr("tray轨道")
                ColumnLayout{
//*
                    RowLayout{
                        Label{
                            text:qsTr("LTK_X1 接盘位置")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltkx1PressPos
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                tray_loader_module_parameters.setLtkx1PressPos(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTKX1MovetoGetPos()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTKX1Name)
                                tray_loader_module_parameters.setLtkx1PressPos(x)
                            }
                        }
                    }
//*/
/*
                    RowLayout{
                        Label{
                            text:qsTr("LTK_X1 中继位置")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltkx1RelayPos
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                tray_loader_module_parameters.setLtkx1RelayPos(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTKX1MoveToRelayPos()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTKX1Name)
                                tray_loader_module_parameters.setLtkx1RelayPos(x)
                            }
                        }
                    }
//*/
/*
                    RowLayout{
                        Label{
                            text:qsTr("LTK_X1 放盘位置")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltkx1ReleasePos
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                tray_loader_module_parameters.setLtkx1ReleasePos(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTKX1MovetoSetPos()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTKX1Name)
                                tray_loader_module_parameters.setLtkx1ReleasePos(x)
                            }
                        }
                    }
//*/

                    RowLayout{
                        Layout.alignment: Qt.AlignRight
                        Button{
                            text: qsTr("夹紧")
                            onClicked: {
                                logicManager.trayLoaderModuleLTKX1CylinderOn()
                            }
                        }
                        Button{
                            text:qsTr("松开")
                            onClicked:{
                                logicManager.trayLoaderModuleLTKX1CylinderOff();
                            }
                        }
                    }
/*
                    RowLayout{
                        Label{
                            text:qsTr("LTK_X2 接盘位置")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltkx2PressPos
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                tray_loader_module_parameters.setLtkx2PressPos(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTKX2MovetoGetPos()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTKX2Name)
                                tray_loader_module_parameters.setLtkx2PressPos(x)
                            }
                        }
                    }
//*/
/*
                    RowLayout{
                        Label{
                            text:qsTr("LTK_X2 放盘位置")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltkx2ReleasePos
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                tray_loader_module_parameters.setLtkx2ReleasePos(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTKX2MovetoSetPos()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTKX2Name)
                                tray_loader_module_parameters.setLtkx2ReleasePos(x)
                            }
                        }
                    }
//*/
/*
                    RowLayout{
                        Layout.alignment: Qt.AlignRight
                        Button{
                            text: qsTr("夹紧")
                            onClicked: {
                                logicManager.trayLoaderModuleLTKX2CylinderOn()
                            }
                        }
                        Button{
                            text:qsTr("松开")
                            onClicked:{
                                logicManager.trayLoaderModuleLTKX2CylinderOff();
                            }
                        }
                    }
//*/
                    RowLayout{
                        Label{
                            text:qsTr("LTL 接盘位置")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltlPressPos
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                if(logicManager.trayLoaderModuleCheckLTLXGetPos(text)){
                                    tray_loader_module_parameters.setLtlPressPos(text)
                                    tray_loader_module_parameters.setLtkx1ReleasePos(text+166.7538)
                                    tray_loader_module_parameters.setLtkx1RelayPos(text+166.7538-310)
                                }else{
                                    tray_loader_module_parameters.setLtlPressPos(310-166.7538)
                                    tray_loader_module_parameters.setLtkx1ReleasePos(310)
                                    tray_loader_module_parameters.setLtkx1RelayPos(0);
                                }
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTLMovetoGetPos()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTLXName)
                                if(logicManager.trayLoaderModuleCheckLTLXGetPos(x)){
                                    tray_loader_module_parameters.setLtlPressPos(x)
                                    tray_loader_module_parameters.setLtkx1ReleasePos(x+166.758)
                                    tray_loader_module_parameters.setLtkx1RelayPos(text+166.7538-310)
                                }else{
                                    tray_loader_module_parameters.setLtlPressPos(310-166.7538)
                                    tray_loader_module_parameters.setLtkx1ReleasePos(310)
                                    tray_loader_module_parameters.setLtkx1RelayPos(0);
                                }
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("LTL 放盘位置")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltlReleasePos
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                if(logicManager.trayLoaderModuleCheckLTLXSetPos(text)){
                                    tray_loader_module_parameters.setLtlReleasePos(text)
                                    tray_loader_module_parameters.setLtkx2PressPos(text-653)
                                }else{
                                    tray_loader_module_parameters.setLtlReleasePos(653)
                                    tray_loader_module_parameters.setLtkx2PressPos(0)
                                }


                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.trayLoaderModuleLTLMovetoSetPos()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTLXName)
                                if(logicManager.trayLoaderModuleCheckLTLXSetPos(x)){
                                    tray_loader_module_parameters.setLtlReleasePos(x)
                                    tray_loader_module_parameters.setLtkx2PressPos(x-653)
                                }else{
                                    tray_loader_module_parameters.setLtlReleasePos(653)
                                    tray_loader_module_parameters.setLtkx2PressPos(0)
                                }
                            }
                        }
                    }
                    RowLayout{
                        Layout.alignment: Qt.AlignRight
                        Button{
                            text: qsTr("夹紧")
                            onClicked: {
                                logicManager.trayLoaderModuleLTLXCylinderOn()
                            }
                        }
                        Button{
                            text:qsTr("松开")
                            onClicked:{
                                logicManager.trayLoaderModuleLTLXCylinderOff()
                            }
                        }
                    }
                    RowLayout{
                        Button{
                            text:qsTr("接盘过程测试")
                            onClicked: {
                                logicManager.trayLoaderModuleLTLXPickUpTray()
                            }
                        }
                        Button{
                            text:qsTr("放盘过程测试")
                            onClicked: {
                                logicManager.trayLoaderModuleLTLXPutDownTray()
                            }
                        }
                    }
                }
            }

        }
    }
}
