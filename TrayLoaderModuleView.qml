import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width
    visible: true
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
                id: trayLoaderModuleViewSwitch
            }
        }

        ColumnLayout{
            visible:  trayLoaderModuleViewSwitch.checked
            RowLayout {
                Label {
                    text: qsTr("")
                }
            }
            GroupBox{
                title: qsTr("进盘弹夹")
                ColumnLayout{
                    RowLayout{
                        Label{
                            text:qsTr("起始位置")
                        }
                        TextField{
                            text:tray_clipin_parameter.firstTrayPos
                            horizontalAlignment:TextInput.AlignHCenter
                            validator: DoubleValidator{
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_clipin_parameter.setFirstTrayPos(text);
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {}
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
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_clipin_parameter.setLastTrayPos(text);
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {}
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
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                tray_clipin_parameter.setColumnDelta(text)
                            }
                        }
                        Button{
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
                            text:qsTr("移动")
                            onClicked: {}
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
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_clipout_parameter.setFirstTrayPos(text);
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {}
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
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_clipout_parameter.setLastTrayPos(text);
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {}
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
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                tray_clipout_parameter.setColumnDelta(text)
                            }
                        }
                        Button{
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
                            onClicked: {}
                        }
                    }
                }
            }
            GroupBox{
                title:qsTr("tray轨道")
                ColumnLayout{
                    RowLayout{
                        Label{
                            text:qsTr("ltkx1 位置1")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltkx1GetPos
                            validator: DoubleValidator{
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_loader_module_parameters.setLtkx1GetPos(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {}
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTKX1Name)
                                tray_loader_module_parameters.setLtkx1GetPos(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("ltkx1 位置2")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltkx1SetPos
                            validator: DoubleValidator{
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_loader_module_parameters.setLtkx1SetPos(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {}
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTKX1Name)
                                tray_loader_module_parameters.setLtkx1SetPos(x)
                            }
                        }
                    }
                    RowLayout{
                        Layout.alignment: Qt.AlignRight
                        Button{
                            text: qsTr("ltkx1 cylinder操作")
                            onClicked: {}
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("ltkx2 位置1")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltkx2GetPos
                            validator: DoubleValidator{
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_loader_module_parameters.setLtkx2GetPos(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {}
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTKX2Name)
                                tray_loader_module_parameters.setLtkx2GetPos(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("ltkx2 位置2")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltkx2SetPos
                            validator: DoubleValidator{
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_loader_module_parameters.setLtkx2SetPos(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {}
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTKX2Name)
                                tray_loader_module_parameters.setLtkx2SetPos(x)
                            }
                        }
                    }
                    RowLayout{
                        Layout.alignment: Qt.AlignRight
                        Button{
                            text: qsTr("ltkx2 cylinder操作")
                            onClicked: {}
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("ltl 位置1")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltlGetPos
                            validator: DoubleValidator{
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_loader_module_parameters.setLtlGetPos(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {}
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTLXName)
                                tray_loader_module_parameters.setLtlGetPos(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("ltl 位置2")
                        }
                        TextField{
                            text:tray_loader_module_parameters.ltlSetPos
                            validator: DoubleValidator{
                                decimals: 3
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                tray_loader_module_parameters.setLtlSetPos(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {}
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTLXName)
                                tray_loader_module_parameters.setLtlSetPos(x)
                            }
                        }
                    }
                    RowLayout{
                        Layout.alignment: Qt.AlignRight
                        Button{
                            text: qsTr("ltl cylinder操作")
                            onClicked: {}
                        }
                    }
                }
            }
        }
    }
}
