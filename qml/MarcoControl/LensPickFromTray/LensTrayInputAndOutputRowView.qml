import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
ColumnLayout{
    RowLayout{ Label { text: qsTr("进盘弹夹") } }
    RowLayout{
        //-------------------------------------------------------------------
        Button{
            text: qsTr("起始位置")
            onClicked: {
                logicManager.trayLoaderModuleLTIEMovetoFirstPos()
            }
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
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTIEName)
                tray_clipin_parameter.setFirstTrayPos(x)
            }
        }
        //-------------------------------------------------------------------
        Button{
            text: qsTr("结束位置")
            onClicked: {
                logicManager.trayLoaderModuleLTIEMovetoLastPos()
            }
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
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTIEName)
                tray_clipin_parameter.setLastTrayPos(x)
            }
        }
        //-------------------------------------------------------------------
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
        //-------------------------------------------------------------------
        Button{
            text:qsTr("换料位置")
            onClicked: {
                logicManager.trayLoaderModuleLTIEMovetoChangeClipPos()
            }
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
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTIEName)
                tray_clipin_parameter.setChangeClipPos(x)
            }
        }
    }
    RowLayout{ Label { text: qsTr("出盘弹夹") } }
    RowLayout{
        Button{
            text:qsTr("起始位置")
            onClicked: {
                logicManager.trayLoaderModuleLTOEMovetoFirstPos()
            }
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
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTOEName)
                tray_clipout_parameter.setFirstTrayPos(x)
            }
        }
        Button{
           text:qsTr("结束位置")
           onClicked: {
               logicManager.trayLoaderModuleLTOEMovetoLastPos()
           }
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
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTOEName)
                tray_clipout_parameter.setLastTrayPos(x)
            }
        }
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
    }
    RowLayout{ Label { text: qsTr("Tray导轨") } }
    RowLayout{
        Button{
            text:qsTr("LTK_X1 接盘位置")
            onClicked: {
                logicManager.trayLoaderModuleLTKX1MovetoGetPos()
            }
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
            text:title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(tray_loader_module_parameters.motorLTKX1Name)
                tray_loader_module_parameters.setLtkx1PressPos(x)
            }
        }
        Button{
            text:qsTr("LTL 接盘位置")
            onClicked: {
                logicManager.trayLoaderModuleLTLMovetoGetPos()
            }
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
        Button{
            text:qsTr("LTL 放盘位置")
            onClicked: {
                logicManager.trayLoaderModuleLTKX1MovetoSetPos()
            }
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
        Button{ text: qsTr("推杆气缸") }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button{ text: qsTr("LTK_X1夹爪气缸") }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button{ text: qsTr("推杆气缸") }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
    RowLayout{ Label { text: qsTr("测试") } }
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
            enabled: false
            onClicked: {
            }
        }
        Button{
            text:qsTr("LTL取盘")
            enabled: false
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
            enabled: false
        }
    }
    RowLayout{
        Button{
            text:qsTr("进盘动作测试")
            enabled: false
        }
        Button{
            text:qsTr("出盘动作测试")
            enabled: false
        }
        Button{
            text:qsTr("进出盘循环测试")
            enabled: false
        }
    }
}
