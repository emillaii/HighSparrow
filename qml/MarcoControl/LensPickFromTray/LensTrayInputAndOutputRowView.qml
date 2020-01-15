import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
ColumnLayout{
    RowLayout{ Label { text: qsTr("进盘弹夹"); font.bold: true } }
    RowLayout{
        Label{
            text: qsTr("起始位置")
        }
        TextField{
            text:tcp_tray_clipin_parameter.firstTrayPos
            horizontalAlignment:TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_tray_clipin_parameter.setFirstTrayPos(text);
            }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTIEName)
                tcp_tray_clipin_parameter.setFirstTrayPos(x)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
                logicManager.performHandling(tcp_tray_loader_module_parameters.moduleName,TrayLoaderModule.ENTRANCE_CLIP_TOP)
            }
        }
        //-------------------------------------------------------------------
        Label{
            text: qsTr("结束位置")
        }
        TextField{
            text:tcp_tray_clipin_parameter.lastTrayPos
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_tray_clipin_parameter.setLastTrayPos(text)
            }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTIEName)
                tcp_tray_clipin_parameter.setLastTrayPos(x)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
                logicManager.performHandling(tcp_tray_loader_module_parameters.moduleName,TrayLoaderModule.ENTRANCE_CLIP_BOTTOM)
            }
        }
        //-------------------------------------------------------------------
        Label{
            text:qsTr("列数")
        }
        TextField{
            text:tcp_tray_clipin_parameter.columnCount
            validator: IntValidator{
                bottom: 2
            }
            horizontalAlignment: TextInput.AlignHCenter
            onEditingFinished: {
                tcp_tray_clipin_parameter.setColumnCount(text)
                //Calculate clip_in delta
                var y1 = tcp_tray_clipin_parameter.firstTrayPos;
                var y2 = tcp_tray_clipin_parameter.lastTrayPos;
                var delta = (y2-y1)/(tcp_tray_clipin_parameter.columnCount-1);
                tcp_tray_clipin_parameter.setColumnDelta(delta);
            }
        }
        Label{
            text:qsTr("列距")
        }
        TextField{
            text:tcp_tray_clipin_parameter.columnDelta
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            horizontalAlignment: TextInput.AlignHCenter
            readOnly: true
        }
        //-------------------------------------------------------------------
        Label{
            text:qsTr("换料位置")
        }
        TextField{
            text:tcp_tray_clipin_parameter.changeClipPos
            horizontalAlignment:TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_tray_clipin_parameter.setChangeClipPos(text);
            }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTIEName)
                tcp_tray_clipin_parameter.setChangeClipPos(x)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
                logicManager.performHandling(tcp_tray_loader_module_parameters.moduleName,TrayLoaderModule.ENTRANCE_CLIP_CHANGE_POS)
            }
        }
    }
    RowLayout{ Label { text: qsTr("出盘弹夹"); font.bold: true } }
    RowLayout{
        Label{
            text:qsTr("起始位置")
        }
        TextField{
            text:tcp_tray_clipout_parameter.firstTrayPos
            horizontalAlignment:TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_tray_clipout_parameter.setFirstTrayPos(text);
            }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTOEName)
                tcp_tray_clipout_parameter.setFirstTrayPos(x)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
                logicManager.performHandling(tcp_tray_loader_module_parameters.moduleName,TrayLoaderModule.EXIT_CLIP_TOP)
            }
        }
        Label{
           text:qsTr("结束位置")
        }
        TextField{
            text:tcp_tray_clipout_parameter.lastTrayPos
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcp_tray_clipout_parameter.setLastTrayPos(text);
            }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTOEName)
                tcp_tray_clipout_parameter.setLastTrayPos(x)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
                logicManager.performHandling(tcp_tray_loader_module_parameters.moduleName,TrayLoaderModule.EXIT_CLIP_BOTTOM)
            }
        }
        Label{
            text:qsTr("列数")
        }
        TextField{
            text: tcp_tray_clipout_parameter.columnCount
            validator: IntValidator{
                bottom: 2
            }
            horizontalAlignment: TextInput.AlignHCenter
            onEditingFinished: {
                tcp_tray_clipout_parameter.setColumnCount(text)
                //Calculate delta
                var y1 = tcp_tray_clipout_parameter.firstTrayPos;
                var y2 = tcp_tray_clipout_parameter.lastTrayPos;
                var delta = (y2-y1)/(tcp_tray_clipout_parameter.columnCount-1);
                tcp_tray_clipout_parameter.setColumnDelta(delta);
            }
        }
        Label{
            text:qsTr("列距")
        }
        TextField{
            text:tcp_tray_clipout_parameter.columnDelta
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            horizontalAlignment: TextInput.AlignHCenter
            readOnly: true
        }
    }
    RowLayout{ Label { text: qsTr("Tray 导轨"); font.bold: true; } }
    RowLayout{
        Label{
            text:qsTr("LTK_X1 接盘位置")
        }
        TextField{
           text:tcp_tray_loader_module_parameters.ltkx1PressPos
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           horizontalAlignment: TextInput.AlignHCenter
           onEditingFinished: {
               tcp_tray_loader_module_parameters.setLtkx1PressPos(text)
           }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTKX1Name)
                tcp_tray_loader_module_parameters.setLtkx1PressPos(x)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
            }
        }
        Label{
            text:qsTr("LTK_X1 Relay位置")
        }
        TextField{
           text:tcp_tray_loader_module_parameters.ltkx1RelayPos
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           horizontalAlignment: TextInput.AlignHCenter
           onEditingFinished: {
               tcp_tray_loader_module_parameters.ltkx1RelayPos(text)
           }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTKX1Name)
                tcp_tray_loader_module_parameters.setLtkx1RelayPos(x)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
            }
        }
        Label{
            text:qsTr("LTK_X1 Release位置")
        }
        TextField{
           text:tcp_tray_loader_module_parameters.ltkx1ReleasePos
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           horizontalAlignment: TextInput.AlignHCenter
           onEditingFinished: {
               tcp_tray_loader_module_parameters.setLtkx1ReleasePos(text)
           }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTKX1Name)
                tcp_tray_loader_module_parameters.setLtkx1ReleasePos(x)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("LTK_X2 接盘位置")
        }
        TextField{
           text:tcp_tray_loader_module_parameters.ltkx2PressPos
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           horizontalAlignment: TextInput.AlignHCenter
           onEditingFinished: {
               tcp_tray_loader_module_parameters.setLtkx2PressPos(text)
           }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTKX2Name)
                tcp_tray_loader_module_parameters.setLtkx2PressPos(x)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
            }
        }
        Label{
            text:qsTr("LTK_X2 Release位置")
        }
        TextField{
           text:tcp_tray_loader_module_parameters.ltkx2ReleasePos
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           horizontalAlignment: TextInput.AlignHCenter
           onEditingFinished: {
               tcp_tray_loader_module_parameters.setLtkx2ReleasePos(text)
           }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTKX2Name)
                tcp_tray_loader_module_parameters.setLtkx2ReleasePos(x)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
            }
        }
        Label{
            text:qsTr("LTK_X2 Safe Distance")
        }
        TextField{
           text:tcp_tray_loader_module_parameters.ltkx2SafeDistance
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           horizontalAlignment: TextInput.AlignHCenter
           onEditingFinished: {
               tcp_tray_loader_module_parameters.ltkx2SafeDistance(text)
           }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTKX2Name)
                tcp_tray_loader_module_parameters.setLtkx2SafeDistance(x)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
            }
        }
    }
    RowLayout {
        Label{
            text:qsTr("LTL 接盘位置")
        }
        TextField{
            text:tcp_tray_loader_module_parameters.ltlPressPos
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            horizontalAlignment: TextInput.AlignHCenter
            onEditingFinished: {
                if(logicManager.trayLoaderModuleCheckLTLXGetPos(text)){
                    tcp_tray_loader_module_parameters.setLtlPressPos(text)
                    tcp_tray_loader_module_parameters.setLtkx1ReleasePos(text+166.7538)
                    tcp_tray_loader_module_parameters.setLtkx1RelayPos(text+166.7538-310)
                }else{
                    tcp_tray_loader_module_parameters.setLtlPressPos(310-166.7538)
                    tcp_tray_loader_module_parameters.setLtkx1ReleasePos(310)
                    tcp_tray_loader_module_parameters.setLtkx1RelayPos(0);
                }
            }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTLXName)
                if(logicManager.trayLoaderModuleCheckLTLXGetPos(x)){
                    tcp_tray_loader_module_parameters.setLtlPressPos(x)
                    tcp_tray_loader_module_parameters.setLtkx1ReleasePos(x+166.758)
                    tcp_tray_loader_module_parameters.setLtkx1RelayPos(text+166.7538-310)
                }else{
                    tcp_tray_loader_module_parameters.setLtlPressPos(310-166.7538)
                    tcp_tray_loader_module_parameters.setLtkx1ReleasePos(310)
                    tcp_tray_loader_module_parameters.setLtkx1RelayPos(0);
                }
            }
        }
        Button{
            text:title_move_to
            onClicked: {
            }
        }
        Label{
            text:qsTr("LTL 工作位置")
        }
        TextField{
            text:tcp_tray_loader_module_parameters.ltlWorkPos
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            horizontalAlignment: TextInput.AlignHCenter
            onEditingFinished: {
                tcp_tray_loader_module_parameters.ltlWorkPos(text);
            }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTLXName)
                tcp_tray_loader_module_parameters.setLtlWorkPos(x)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
            }
        }
        Label{
            text:qsTr("LTL 放盘位置")
//            onClicked: {
//                logicManager.trayLoaderModuleLTKX1MovetoSetPos()
//            }
        }
        TextField{
            text:tcp_tray_loader_module_parameters.ltlReleasePos
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            horizontalAlignment: TextInput.AlignHCenter
            onEditingFinished: {
                if(logicManager.trayLoaderModuleCheckLTLXSetPos(text)){
                    tcp_tray_loader_module_parameters.setLtlReleasePos(text)
                    tcp_tray_loader_module_parameters.setLtkx2PressPos(text-653)
                }else{
                    tcp_tray_loader_module_parameters.setLtlReleasePos(653)
                    tcp_tray_loader_module_parameters.setLtkx2PressPos(0)
                }
            }
        }
        Button{
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tcp_tray_loader_module_parameters.motorLTLXName)
                if(logicManager.trayLoaderModuleCheckLTLXSetPos(x)){
                    tcp_tray_loader_module_parameters.setLtlReleasePos(x)
                    tcp_tray_loader_module_parameters.setLtkx2PressPos(x-653)
                }else{
                    tcp_tray_loader_module_parameters.setLtlReleasePos(653)
                    tcp_tray_loader_module_parameters.setLtkx2PressPos(0)
                }
            }
        }
        Button{
            text:title_move_to
            onClicked: {
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("推杆气缸")
            onClicked: {
                baseModuleManager.toogleIoState(tcp_tray_loader_module_parameters.tcpCylinderClipIOName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if(tcp_tray_loader_module_states.tcpCylinderClipSensorState)
                        return "lightGreen"
                    else
                        return "red"
                }
            }
        }
        Button{
            text: qsTr("LTK_X1夹爪气缸")
            onClicked: {
                baseModuleManager.toogleIoState(tcp_tray_loader_module_parameters.tcpCylinderLTKX1IOName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if(tcp_tray_loader_module_states.tcpCylinderLTK1SensorState)
                        return "lightGreen"
                    else
                        return "red"
                }
            }
        }
        Button{
            text: qsTr("LTK_X2夹爪气缸")
            onClicked: {
                baseModuleManager.toogleIoState(tcp_tray_loader_module_parameters.tcpCylinderLTKX2IOName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if(tcp_tray_loader_module_states.tcpCylinderLTK2SensorState)
                        return "lightGreen"
                    else
                        return "red"
                }
            }
        }
        Button{
            text: qsTr("LTL气缸")
            onClicked: {
                baseModuleManager.toogleIoState(tcp_tray_loader_module_parameters.tcpCylinderLTLIOName)
            }
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: {
                    if(tcp_tray_loader_module_states.tcpCylinderLTLSensorState)
                        return "lightGreen"
                    else
                        return "red"
                }
            }
        }
    }
    RowLayout{ Label { text: qsTr("测试"); font.bold: true } }
    RowLayout{
        Label{
            text: qsTr("进盘位置")
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
            text:qsTr("推出Tray盘")
            onClicked: {
                logicManager.trayLoaderModuleEjectTray()
            }
        }
        Button{
            text:qsTr("LTK_X1取Tray盘")
            onClicked: {
            }
        }
        Button{
            text:qsTr("LTL取盘")
            onClicked: {
            }
        }
    }
    RowLayout{
        Label{
            text: qsTr("出盘位置")
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
        Button{
            text:qsTr("LTL放盘")
        }
    }
    RowLayout{
        Button{
            text:qsTr("进盘动作测试")
        }
        Button{
            text:qsTr("出盘动作测试")
        }
        Button{
            text:qsTr("进出盘循环测试")
        }
    }
}
