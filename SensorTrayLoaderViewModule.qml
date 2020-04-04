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
        ListModel{
            id:motorsModel
            ListElement{
                modelData:"STPO"
            }
            ListElement{
                modelData:"STIE"
            }
            ListElement{
                modelData:"STK"
            }
            ListElement{
                modelData:"STOE"
            }
            ListElement{
                modelData:"STL"
            }
        }
        MotionPopup{
            id:sensor_tray_loader_module_motions
        }

        ColumnLayout{
            RowLayout {
                Label {
                    text: qsTr("")
                }
                Button{
                    text:qsTr("手动操作")
                    onClicked:{
                        sensor_tray_loader_module_motions.open()
                    }
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
                            text:sensor_entrance_clip_parameter.startPosition
                            horizontalAlignment:TextInput.AlignHCenter
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                sensor_entrance_clip_parameter.setStartPosition(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleSTIEMovetoColumnIndex(0)
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTIEName)
                                sensor_entrance_clip_parameter.setStartPosition(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("结束位置")
                        }
                        TextField{
                            text:sensor_entrance_clip_parameter.endPosition
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                sensor_entrance_clip_parameter.setEndPosition(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleSTIEMovetoColumnIndex(sensor_clip_standard_parameter.count-1);
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTIEName)
                                sensor_entrance_clip_parameter.setEndPosition(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("列数")
                        }
                        TextField{
                            text:sensor_clip_standard_parameter.count
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
                            text:sensor_clip_standard_parameter.delta
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
                                var y1 = sensor_entrance_clip_parameter.startPosition;
                                var y2 = sensor_entrance_clip_parameter.endPosition;
                                var delta = (y2-y1)/(sensor_clip_standard_parameter.count-1);
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
                            text:sensor_entrance_clip_parameter.currentIndex+1
                            validator: IntValidator{
                                bottom: 1
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                var idx = text-1
                                sensor_entrance_clip_parameter.setCurrentIndex(idx)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleSTIEMovetoColumnIndex(col_in.text-1)
                            }
                        }
                        Button{
                            text:qsTr("弹出")
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleMovetoPushoutPosition()
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
                            text:sensor_exit_clip_parameter.startPosition
                            horizontalAlignment:TextInput.AlignHCenter
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                sensor_exit_clip_parameter.setStartPosition(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleSTOEMovetoColumnIndex(0)
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTOEName)
                                sensor_exit_clip_parameter.setStartPosition(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("结束位置")
                        }
                        TextField{
                            text:sensor_exit_clip_parameter.endPosition
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            onEditingFinished: {
                                sensor_exit_clip_parameter.setEndPosition(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleSTOEMovetoColumnIndex(sensor_clip_standard_parameter.count-1)
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTOEName)
                                sensor_exit_clip_parameter.setEndPosition(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("列数")
                        }
                        TextField{
                            text:sensor_clip_standard_parameter.count
                            validator: IntValidator{
                                bottom: 2
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                sensor_clip_standard_parameter.setCount()
                            }
                        }
                        Label{
                            text:qsTr("列距")
                        }
                        TextField{
                            text:sensor_clip_standard_parameter.delta
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
                                var y1 = sensor_exit_clip_parameter.startPosition;
                                var y2 = sensor_exit_clip_parameter.endPosition;
                                var delta = (y2-y1)/(sensor_clip_standard_parameter.count-1);
                                sensor_clip_standard_parameter.setDelta(delta)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("目标位置")
                        }
                        TextField{
                            id:col_out
                            text:sensor_exit_clip_parameter.currentIndex+1
                            validator: IntValidator{
                                bottom: 1
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                var idx = text-1
                                sensor_exit_clip_parameter.setCurrentIndex(idx)
                            }
                        }
                        Button{
                            text:qsTr("移动")
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleSTOEMovetoColumnIndex(col_out.text-1)
                            }
                        }
                    }
                }
            }
            GroupBox{
                title:qsTr("tray轨道")
                ColumnLayout{
                    RowLayout{
                        Label{
                            text:qsTr("弹夹推盘位置")
                        }
                        TextField{
                            text:sensor_tray_loader_module_parameter.pushoutPosition
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                sensor_tray_loader_module_parameter.setPushoutPosition(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleMovetoPushoutPosition()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSPOName)
                                sensor_tray_loader_module_parameter.setPushoutPosition(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("STL 夾盘位置")
                        }
                        TextField{
                            text:sensor_tray_loader_module_parameter.getTrayPosition
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                sensor_tray_loader_module_parameter.setGetTrayPosition(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleMovetoGetTrayPosition()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorTrayName)
                                sensor_tray_loader_module_parameter.setGetTrayPosition(x)
                            }
                        }
                    }
                    RowLayout{
                        Label{
                            text:qsTr("STL拉盘结束位置")
                        }
                        TextField{
                            text:sensor_tray_loader_module_parameter.putTrayPosition
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                sensor_tray_loader_module_parameter.setPutTrayPosition(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleMovetoPutTrayPosition()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorTrayName)
                                sensor_tray_loader_module_parameter.setPutTrayPosition(x)
                            }
                        }
                    }

                    RowLayout{
                        Label{
                            text:qsTr("STL工作位置")
                        }
                        TextField{
                            text:sensor_tray_loader_module_parameter.trayWorkPosition
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                sensor_tray_loader_module_parameter.setTrayWorkPosition(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                sensorTrayLoaderModule.movetoTrayWorkPosition()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorTrayName)
                                sensor_tray_loader_module_parameter.setTrayWorkPosition(x)
                            }
                        }
                    }

                    RowLayout{
                        Label{
                            text:qsTr("STL放盘給STK推盘位置")
                        }
                        TextField{
                            text:sensor_tray_loader_module_parameter.downTrayPosition
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                sensor_tray_loader_module_parameter.setDownTrayPosition(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleMovetoDownTrayPosition()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorTrayName)
                                sensor_tray_loader_module_parameter.setDownTrayPosition(x)
                            }
                        }
                    }

                    RowLayout{
                        Label{
                            text:qsTr("STK从STL接盘位置")
                        }
                        TextField{
                            text:sensor_tray_loader_module_parameter.startKickTrayPosition
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                sensor_tray_loader_module_parameter.setStartKickTrayPosition(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleMovetoStartKickTrayPosition()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTKName)
                                sensor_tray_loader_module_parameter.setStartKickTrayPosition(x)
                            }
                        }
                    }

                    RowLayout{
                        Label{
                            text:qsTr("STK 放下/开始推 工作盘位置")
                        }
                        TextField{
                            text:sensor_tray_loader_module_parameter.vacancyTrayPosition
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                sensor_tray_loader_module_parameter.setVacancyTrayPosition(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleMovetoVacancyTrayPosition()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTKName)
                                sensor_tray_loader_module_parameter.setVacancyTrayPosition(x)
                            }
                        }
                    }

                    RowLayout{
                        Label{
                            text:qsTr("STK推盘到出料彈夾位置")
                        }
                        TextField{
                            text:sensor_tray_loader_module_parameter.finishKickTrayPosition
                            validator: DoubleValidator{
                                decimals: 6
                                notation: DoubleValidator.StandardNotation
                            }
                            horizontalAlignment: TextInput.AlignHCenter
                            onEditingFinished: {
                                sensor_tray_loader_module_parameter.setFinishKickTrayPosition(text)
                            }
                        }
                        Button{
                            text:title_move_to
                            onClicked: {
                                logicManager.sensorTrayLoaderModuleMovetoFinishKickTrayPosition()
                            }
                        }
                        Button{
                            text:title_read_encoder
                            onClicked: {
                                var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTKName)
                                sensor_tray_loader_module_parameter.setFinishKickTrayPosition(x)
                            }
                        }
                    }
                }
            }
        }
    }
}
